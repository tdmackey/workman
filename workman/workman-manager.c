/*
 * workman-manager.c: workload manager
 *
 * Copyright (C) 2013 Red Hat, Inc.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307  USA
 *
 * Author: Daniel P. Berrange <berrange@redhat.com>
 */

#include <config.h>

#include "workman.h"
#include "workman-manager-linux.h"
#include "workman-consumer-private.h"
#include "workman-partition-private.h"
#include "workman-object-private.h"

G_DEFINE_ABSTRACT_TYPE(WorkmanManager, workman_manager, G_TYPE_OBJECT);

#define WORKMAN_MANAGER_GET_PRIVATE(obj) (G_TYPE_INSTANCE_GET_PRIVATE((obj), WORKMAN_TYPE_MANAGER, WorkmanManagerPrivate))


struct _WorkmanManagerPrivate {
    GList *plugins;
    GList *objects;
};


enum {
    PROP_0,
    PROP_PLUGINS,
    PROP_OBJECTS,
};


static WorkmanManager *default_manager;


static void
workman_manager_dispose(GObject *object)
{
    WorkmanManager *self = WORKMAN_MANAGER(object);

    g_boxed_free(WORKMAN_TYPE_PLUGIN_LIST, self->priv->plugins);
    self->priv->plugins = NULL;

    g_boxed_free(WORKMAN_TYPE_OBJECT_LIST, self->priv->objects);
    self->priv->objects = NULL;

    G_OBJECT_CLASS(workman_manager_parent_class)->dispose(object);
}


static void
workman_manager_get_property(GObject *object,
                             guint prop_id,
                             GValue *value,
                             GParamSpec *pspec)
{
    WorkmanManager *self = WORKMAN_MANAGER(object);

    switch (prop_id) {
        case PROP_PLUGINS:
            g_value_set_boxed(value, self->priv->plugins);
            break;
        case PROP_OBJECTS:
            g_value_set_boxed(value, self->priv->objects);
            break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
            break;
    }
}


static void
workman_manager_set_property(GObject *object,
                             guint prop_id,
                             const GValue *value,
                             GParamSpec *pspec)
{
    WorkmanManager *self = WORKMAN_MANAGER(object);

    switch (prop_id) {
        case PROP_PLUGINS:
            g_boxed_free(WORKMAN_TYPE_PLUGIN_LIST, self->priv->plugins);
            self->priv->plugins = g_value_dup_boxed(value);
            break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
            break;
    }
}


static void
workman_manager_class_init(WorkmanManagerClass *klass)
{
    GObjectClass *g_klass = G_OBJECT_CLASS(klass);
    GParamSpec *pspec;

    g_klass->dispose = workman_manager_dispose;
    g_klass->set_property = workman_manager_set_property;
    g_klass->get_property = workman_manager_get_property;


    /**
     * WorkmanManager:plugins:
     *
     * Type: GLib.List(WorkmanPlugin)
     * Transfer: full
     */
    pspec = g_param_spec_boxed("plugins",
                               "Plugins",
                               "The manager's list of registered plugins",
                               WORKMAN_TYPE_PLUGIN_LIST,
                               G_PARAM_READWRITE |
                               G_PARAM_CONSTRUCT_ONLY |
                               G_PARAM_STATIC_STRINGS);
    g_object_class_install_property(g_klass, PROP_PLUGINS, pspec);

    /**
     * WorkmanManager:objects:
     *
     * Type: GLib.List(WorkmanObject)
     * Transfer: full
     */
    pspec = g_param_spec_boxed("objects",
                               "Objects",
                               "The manager's list of objects",
                               WORKMAN_TYPE_OBJECT_LIST,
                               G_PARAM_READABLE |
                               G_PARAM_STATIC_STRINGS);
    g_object_class_install_property(g_klass, PROP_OBJECTS, pspec);

    g_type_class_add_private(klass, sizeof(WorkmanManagerPrivate));
}


static void
workman_manager_init(WorkmanManager *attr)
{
    WorkmanManagerPrivate *priv;
    attr->priv = priv = WORKMAN_MANAGER_GET_PRIVATE(attr);
}


/**
 * workman_manager_get_default:
 *
 * Returns: (transfer full): the default workload manager object
 */
WorkmanManager *
workman_manager_get_default(GError **error)
{
    if (!default_manager)
        default_manager = WORKMAN_MANAGER(workman_manager_linux_new());

    return g_object_ref(default_manager);
}


/**
 * workman_manager_add_partition:
 * @mgr: (transfer none): the manager object
 * @state: the state to create the new partition in
 * @parent: (transfer none)(allow-none): the parent of the new partition
 *
 * Returns: (transfer full): the new partition
 */
WorkmanPartition *
workman_manager_add_partition(WorkmanManager *mgr,
                              const gchar *name,
                              WorkmanState state,
                              WorkmanPartition *parent,
                              GError **error)
{
    /* TODO: call add_partition on each plugin until one succeeds */
    return NULL;
}

gboolean
workman_manager_remove_partition(WorkmanManager *self,
                                 WorkmanState state,
                                 WorkmanPartition *partition,
                                 GError **error)
{
    WorkmanObject *object;
    WorkmanState obj_state;
    GList *list = NULL;

    object = WORKMAN_OBJECT(partition);
    obj_state = workman_object_get_state(object, error);
    g_return_val_if_fail(obj_state & state, FALSE);

    /* Make sure the partition is empty */
    list = workman_partition_get_children(partition, state, error);
    if (list) {
        g_boxed_free(WORKMAN_TYPE_PARTITION_LIST, list);
        return FALSE;
    }

    list = workman_partition_get_consumers(partition, state, error);
    if (list) {
        g_boxed_free(WORKMAN_TYPE_CONSUMER_LIST, list);
        return FALSE;
    }

    /* TODO:
    plugin = workman_object_get_plugin(object);
    ret = workman_plugin_remove_partition(plugin, partition, state, error);
    if (!ret)
        return FALSE;
    */

    if (obj_state == state || state == WORKMAN_STATE_ALL) {
        self->priv->objects = g_list_remove(self->priv->objects, object);
        g_object_unref(object);
    }
    else {
        /*
         * In this case, the object state was WORKMAN_STATE_ALL and we removed
         * the parent associated with only one of its states, so we don't
         * remove the object from our list.  Instead we just change its state
         * to the other state.
         */
        workman_object_set_state(object, obj_state & ~state);
    }

    return TRUE;
}


WorkmanPartition *
workman_manager_get_partition_by_name(WorkmanManager *self,
                                      const gchar *name,
                                      GError **error)
{
    GList *o;

    for (o = self->priv->objects; o; o = o->next) {
        WorkmanObject *object = WORKMAN_OBJECT(o->data);
        const gchar *obj_name = workman_object_get_name(object);
        if (WORKMAN_IS_PARTITION(object) && !g_strcmp0(obj_name, name))
            return g_object_ref(WORKMAN_PARTITION(object));
    }

    return NULL;
}


/*
 * Given a consumer object, replace its placeholder partition with the real
 * thing.
 */
static gboolean
workman_manager_translate_consumer_partition(WorkmanManager *self,
                                             WorkmanState state,
                                             WorkmanConsumer *consumer)
{
    WorkmanPartition *fake_part, *real_part;
    const gchar *name;

    fake_part = workman_consumer_get_partition(consumer, state, NULL);
    if (!fake_part)
        return TRUE;

    name = workman_object_get_name(WORKMAN_OBJECT(fake_part));
    real_part = workman_manager_get_partition_by_name(self, name, NULL);
    g_return_val_if_fail(real_part, FALSE);

    workman_consumer_set_partition_priv(consumer, state, real_part);
    workman_partition_add_consumer_priv(real_part, state, consumer);

    g_object_unref(fake_part);
    g_object_unref(real_part);

    return TRUE;
}


gboolean
workman_manager_refresh_objects(WorkmanManager *self,
                                GError **error)
{
    GList *o = NULL, *plugin_objs = NULL;
    WorkmanManagerPrivate *priv = self->priv;

     /*
     * The plugins may return consumer objects whose partitions are represented
     * by temporary placeholder objects, which allow the plugins to specify
     * relationships to other objects which they don't "own".  relationships to
     * other objects which they don't "own".  For example, the libvirt plugin
     * may report a list of "real" consumer objects and "fake" placeholder
     * partition objects, so that it can specify the names of the partitions to
     * which its consumers belong.  The "real" versions of the partition
     * objects will be reported by the cgroups plugin.
     */

    /* Get all objects from all plugins */
    /* TODO:
    for (p = priv->plugins; p; p = p->next) {
        GError *tmp_error = NULL;
        WorkmanPlugin *plugin = WORKMAN_PLUGIN(p->data);

        plugin_objs = g_list_concat(plugin_objs,
                                    workman_plugin_get_objects(plugin,
                                                               &tmp_error));
        if (tmp_error) {
            g_propagate_error(error, tmp_error);
            g_boxed_free(WORKMAN_TYPE_OBJECT_LIST, plugin_objs);
            return FALSE;
        }
    }
    */

    g_boxed_free(WORKMAN_TYPE_OBJECT_LIST, priv->objects);
    priv->objects = NULL;

    /* Copy all plugin-owned objects, ignoring placeholder objects */
    /* TODO:
    for (o = plugin_objs; o; o = o->next) {
        WorkmanObject *object = WORKMAN_OBJECT(o->data);
        if (workman_object_get_plugin(o, NULL)) {

            g_object_unref(plugin);

            g_return_val_if_fail(!g_list_find(priv->objects, object), FALSE);

            priv->objs = g_list_append(priv->objs, object);
            g_object_ref(object);
        }
    }
    */

    /*
     * Now go through the consumers and translate their placeholder partition
     * objects into real partition objects.
     */
    for (o = priv->objects; o; o = o->next) {
        WorkmanObject *object = WORKMAN_OBJECT(o->data);
        if (WORKMAN_IS_CONSUMER(object)) {
            gboolean ret;
            WorkmanConsumer *consumer = WORKMAN_CONSUMER(object);

            ret = workman_manager_translate_consumer_partition(self,
                                                               WORKMAN_STATE_ACTIVE,
                                                               consumer);
            g_return_val_if_fail(ret, FALSE);

            ret = workman_manager_translate_consumer_partition(self,
                                                               WORKMAN_STATE_PERSISTENT,
                                                               consumer);
            g_return_val_if_fail(ret, FALSE);
        }
    }

    g_boxed_free(WORKMAN_TYPE_OBJECT_LIST, plugin_objs);
    return TRUE;
}

/**
 * workman_manager_get_partitions:
 *
 * Returns: (transfer full)(element-type WorkmanPartition): the list of partitions
 */
GList *
workman_manager_get_partitions(WorkmanManager *self,
                               WorkmanState state,
                               GError **error)
{
    GList *o, *parts = NULL;

    if (!self->priv->objects)
        if (!workman_manager_refresh_objects(self, error))
            return NULL;

    for (o = self->priv->objects; o; o = o->next) {
        if (WORKMAN_IS_PARTITION(o->data)) {
            WorkmanObject *object = WORKMAN_OBJECT(o->data);
            WorkmanState object_state = workman_object_get_state(object, NULL);
            if (object_state & state) {
                parts = g_list_append(parts, object);
                g_object_ref(object);
            }
        }
    }

    return parts;
}


/**
 * workman_manager_get_consumers:
 *
 * Returns: (transfer full)(element-type WorkmanConsumer): the list of consumers
 */
GList *
workman_manager_get_consumers(WorkmanManager *self,
                              WorkmanState state,
                              GError **error)
{
    GList *o, *consumers = NULL;

    if (!self->priv->objects)
        if (!workman_manager_refresh_objects(self, error))
            return NULL;

    for (o = self->priv->objects; o; o = o->next) {
        if (WORKMAN_IS_CONSUMER(o->data)) {
            WorkmanObject *object = WORKMAN_OBJECT(o->data);
            WorkmanState object_state = workman_object_get_state(object, NULL);
            if (object_state & state) {
                consumers = g_list_append(consumers, object);
                g_object_ref(object);
            }
        }
    }

    return consumers;
}


/*
 * Local variables:
 *  indent-tabs-mode: nil
 *  c-indent-level: 4
 *  c-basic-offset: 4
 * End:
 *
 * vim: set expandtab shiftwidth=4 tabstop=4 :
 */
