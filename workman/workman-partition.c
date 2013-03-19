/*
 * workman-partition.c: workload partition
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

G_DEFINE_TYPE(WorkmanPartition, workman_partition, WORKMAN_TYPE_OBJECT);

#define WORKMAN_PARTITION_GET_PRIVATE(obj) (G_TYPE_INSTANCE_GET_PRIVATE((obj), WORKMAN_TYPE_PARTITION, WorkmanPartitionPrivate))


struct _WorkmanPartitionPrivate {
    WorkmanPartition *parent_active;
    WorkmanPartition *parent_persistent;
    GList *children_active;
    GList *children_persistent;
    GList *consumers_active;
    GList *consumers_persistent;
};


enum {
    PROP_0,
    PROP_PARENT_ACTIVE,
    PROP_PARENT_PERSISTENT,
    PROP_CHILDREN_ACTIVE,
    PROP_CHILDREN_PERSISTENT,
    PROP_CONSUMERS_ACTIVE,
    PROP_CONSUMERS_PERSISTENT,
};


static void
workman_partition_dispose(GObject *object)
{
    WorkmanPartition *self = WORKMAN_PARTITION(object);;

    g_boxed_free(WORKMAN_TYPE_PARTITION_LIST, self->priv->children_active);
    self->priv->children_active = NULL;

    g_boxed_free(WORKMAN_TYPE_PARTITION_LIST, self->priv->children_persistent);
    self->priv->children_persistent = NULL;

    g_boxed_free(WORKMAN_TYPE_CONSUMER_LIST, self->priv->consumers_active);
    self->priv->consumers_active = NULL;

    g_boxed_free(WORKMAN_TYPE_CONSUMER_LIST, self->priv->consumers_persistent);
    self->priv->consumers_persistent = NULL;

    g_object_unref(self->priv->parent_active);
    self->priv->parent_active = NULL;

    g_object_unref(self->priv->parent_persistent);
    self->priv->parent_persistent = NULL;

    G_OBJECT_CLASS(workman_partition_parent_class)->dispose(object);
}


static void
workman_partition_get_property(GObject *object,
                               guint prop_id,
                               GValue *value,
                               GParamSpec *pspec)
{
    WorkmanPartition *self = WORKMAN_PARTITION(object);

    switch (prop_id) {
        case PROP_PARENT_ACTIVE:
            g_value_set_object(value, self->priv->parent_active);
            break;
        case PROP_PARENT_PERSISTENT:
            g_value_set_object(value, self->priv->parent_persistent);
            break;
        case PROP_CHILDREN_ACTIVE:
            g_value_set_boxed(value, self->priv->children_active);
            break;
        case PROP_CHILDREN_PERSISTENT:
            g_value_set_boxed(value, self->priv->children_persistent);
            break;
        case PROP_CONSUMERS_ACTIVE:
            g_value_set_boxed(value, self->priv->consumers_active);
            break;
        case PROP_CONSUMERS_PERSISTENT:
            g_value_set_boxed(value, self->priv->consumers_persistent);
            break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
            break;
    }
}


static void
workman_partition_set_property(GObject *object,
                               guint prop_id,
                               const GValue *value,
                               GParamSpec *pspec)
{
    WorkmanPartition *self = WORKMAN_PARTITION(object);

    switch (prop_id) {
        case PROP_PARENT_ACTIVE:
            g_object_unref(self->priv->parent_active);
            self->priv->parent_active = g_value_dup_object(value);
            break;
        case PROP_PARENT_PERSISTENT:
            g_object_unref(self->priv->parent_persistent);
            self->priv->parent_persistent = g_value_dup_object(value);
            break;
        case PROP_CHILDREN_ACTIVE:
            g_boxed_free(WORKMAN_TYPE_PARTITION_LIST,
                         self->priv->children_active);
            self->priv->children_active = g_value_dup_boxed(value);
            break;
        case PROP_CHILDREN_PERSISTENT:
            g_boxed_free(WORKMAN_TYPE_PARTITION_LIST,
                         self->priv->children_persistent);
            self->priv->children_persistent = g_value_dup_boxed(value);
            break;
        case PROP_CONSUMERS_ACTIVE:
            g_boxed_free(WORKMAN_TYPE_CONSUMER_LIST,
                         self->priv->consumers_active);
            self->priv->consumers_active = g_value_dup_boxed(value);
            break;
        case PROP_CONSUMERS_PERSISTENT:
            g_boxed_free(WORKMAN_TYPE_CONSUMER_LIST,
                         self->priv->consumers_persistent);
            self->priv->consumers_persistent = g_value_dup_boxed(value);
            break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
            break;
    }

}

static void
workman_partition_class_init(WorkmanPartitionClass *klass)
{
    GObjectClass *g_klass = G_OBJECT_CLASS(klass);
    GParamSpec *pspec;

    g_klass->dispose = workman_partition_dispose;
    g_klass->set_property = workman_partition_set_property;
    g_klass->get_property = workman_partition_get_property;

    pspec = g_param_spec_object("parent-active",
                                "Active Parent",
                                "The partition's parent partition in the active state",
                                WORKMAN_TYPE_PARTITION,
                                G_PARAM_READWRITE |
                                G_PARAM_CONSTRUCT_ONLY |
                                G_PARAM_STATIC_STRINGS);
    g_object_class_install_property(g_klass, PROP_PARENT_ACTIVE, pspec);

    pspec = g_param_spec_object("parent-persistent",
                                "Persistent Parent",
                                "The partition's parent partition in the persistent state",
                                WORKMAN_TYPE_PARTITION,
                                G_PARAM_READWRITE |
                                G_PARAM_CONSTRUCT_ONLY |
                                G_PARAM_STATIC_STRINGS);
    g_object_class_install_property(g_klass, PROP_PARENT_PERSISTENT, pspec);


    /**
     * WorkmanPartition:children-active:
     *
     * Type: GLib.List(WorkmanPartition)
     * Transfer: full
     */
    pspec = g_param_spec_boxed("children-active",
                               "Active Children",
                               "The partition's list of children partitions in the active state",
                               WORKMAN_TYPE_PARTITION_LIST,
                               G_PARAM_READWRITE |
                               G_PARAM_CONSTRUCT_ONLY |
                               G_PARAM_STATIC_STRINGS);
    g_object_class_install_property(g_klass, PROP_CHILDREN_ACTIVE, pspec);

    /**
     * WorkmanPartition:children-persistent:
     *
     * Type: GLib.List(WorkmanPartition)
     * Transfer: full
     */
    pspec = g_param_spec_boxed("children-persistent",
                               "Persistent Children",
                               "The partition's list of children partitions in the persistent state",
                               WORKMAN_TYPE_PARTITION_LIST,
                               G_PARAM_READWRITE |
                               G_PARAM_CONSTRUCT_ONLY |
                               G_PARAM_STATIC_STRINGS);
    g_object_class_install_property(g_klass, PROP_CHILDREN_PERSISTENT, pspec);


    /**
     * WorkmanPartition:consumers-active:
     *
     * Type: GLib.List(WorkmanConsumer)
     * Transfer: full
     */
    pspec = g_param_spec_boxed("consumers-active",
                               "Active Consumers",
                               "The partition's list of consumers in the active state",
                               WORKMAN_TYPE_CONSUMER_LIST,
                               G_PARAM_READWRITE |
                               G_PARAM_CONSTRUCT_ONLY |
                               G_PARAM_STATIC_STRINGS);
    g_object_class_install_property(g_klass, PROP_CONSUMERS_ACTIVE, pspec);


    /**
     * WorkmanPartition:consumers-persistent:
     *
     * Type: GLib.List(WorkmanConsumer)
     * Transfer: full
     */
    pspec = g_param_spec_boxed("consumers-persistent",
                               "Persistent Consumers",
                               "The partition's list of consumers in the persistent state",
                               WORKMAN_TYPE_CONSUMER_LIST,
                               G_PARAM_READWRITE |
                               G_PARAM_CONSTRUCT_ONLY |
                               G_PARAM_STATIC_STRINGS);
    g_object_class_install_property(g_klass, PROP_CONSUMERS_PERSISTENT, pspec);

    g_type_class_add_private(klass, sizeof(WorkmanPartitionPrivate));
}


static void
workman_partition_init(WorkmanPartition *attr)
{
    WorkmanPartitionPrivate *priv;
    attr->priv = priv = WORKMAN_PARTITION_GET_PRIVATE(attr);

}


/**
 * workman_partition_get_consumers:
 *
 * Returns: (transfer full) (element-type WorkmanConsumer): the assigned consumers
 */
GList *
workman_partition_get_consumers(WorkmanPartition *self,
                                WorkmanState state,
                                GError **error)
{
    return workman_list_merge(self->priv->consumers_active,
                              self->priv->consumers_persistent,
                              state);
}


/**
 * workman_partition_get_children:
 *
 * Returns: (transfer full) (element-type WorkmanPartition): the children partitions of this partition
 */
GList *
workman_partition_get_children(WorkmanPartition *self,
                               WorkmanState state,
                               GError **error)
{
    return workman_list_merge(self->priv->children_active,
                              self->priv->children_persistent,
                              state);
}


/**
 * workman_partition_get_parent:
 *
 * Returns: (transfer full): the parent partition
 */
WorkmanPartition *
workman_partition_get_parent(WorkmanPartition *self,
                             WorkmanState state,
                             GError **error)
{
    if (state == WORKMAN_STATE_ACTIVE)
        return g_object_ref(self->priv->parent_active);

    if (state == WORKMAN_STATE_PERSISTENT)
        return g_object_ref(self->priv->parent_persistent);

    return NULL;
}


/**
 * workman_partition_set_parent:
 *
 * Returns: TRUE on success
 */
gboolean
workman_partition_set_parent(WorkmanPartition *self,
                             WorkmanState state,
                             WorkmanPartition *parent,
                             GError **error)
{
    /* TODO: call plugin->set_parent and update obj->priv->parent */
    return FALSE;
}


WorkmanPartition *
workman_partition_new(const gchar *name,
                      WorkmanState state,
                      GList *attributes,
                      WorkmanPartition *parent_active,
                      WorkmanPartition *parent_persistent,
                      GList *children_active,
                      GList *children_persistent,
                      GList *consumers_active,
                      GList *consumers_persistent)
{
    if ((parent_active || children_active || consumers_active) &&
        !(state & WORKMAN_STATE_ACTIVE))
        return NULL;

    if ((parent_persistent || children_persistent || consumers_persistent) &&
        !(state & WORKMAN_STATE_PERSISTENT))
        return NULL;

    return WORKMAN_PARTITION(g_object_new(WORKMAN_TYPE_PARTITION,
                                          "name", name,
                                          "state", state,
                                          "attributes", attributes,
                                          "parent-active", parent_active,
                                          "parent-persistent", parent_persistent,
                                          "children-active", children_active,
                                          "children-persistent", children_persistent,
                                          "consumers-active", consumers_active,
                                          "consumers-persistent", consumers_persistent,
                                          NULL));
}


gboolean
workman_partition_add_consumer_priv(WorkmanPartition *self,
                                    WorkmanState state,
                                    WorkmanConsumer *consumer)
{
    WorkmanPartitionPrivate *priv = self->priv;
    gboolean ret = FALSE;
    WorkmanState obj_state, cons_state;

    obj_state = workman_object_get_state(WORKMAN_OBJECT(self), NULL);
    g_return_val_if_fail(state & obj_state, FALSE);

    cons_state = workman_object_get_state(WORKMAN_OBJECT(consumer), NULL);
    g_return_val_if_fail(state & cons_state, FALSE);

    if (state & WORKMAN_STATE_ACTIVE) {
        g_object_ref(consumer);
        priv->consumers_active = g_list_append(priv->consumers_active,
                                               consumer);
        ret = TRUE;
    }

    if (state & WORKMAN_STATE_PERSISTENT) {
        g_object_ref(consumer);
        priv->consumers_persistent = g_list_append(priv->consumers_persistent,
                                                   consumer);
        ret = TRUE;
    }

    return ret;
}


gboolean
workman_partition_remove_consumer_priv(WorkmanPartition *self,
                                       WorkmanState state,
                                       WorkmanConsumer *consumer)
{
    WorkmanPartitionPrivate *priv = self->priv;
    gboolean ret = FALSE;

    if (state & WORKMAN_STATE_ACTIVE &&
        g_list_find(priv->consumers_active, consumer)) {

        priv->consumers_active = g_list_remove(priv->consumers_active,
                                               consumer);
        g_object_unref(consumer);
        ret = TRUE;
    }

    if (state & WORKMAN_STATE_PERSISTENT &&
        g_list_find(priv->consumers_persistent, consumer)) {

        priv->consumers_persistent = g_list_remove(priv->consumers_persistent,
                                                   consumer);
        g_object_unref(consumer);
        ret = TRUE;
    }

    return ret;
}


gboolean
workman_partition_add_child_priv(WorkmanPartition *self,
                                 WorkmanState state,
                                 WorkmanPartition *child)
{
    WorkmanPartitionPrivate *priv = self->priv;
    gboolean ret = FALSE;
    WorkmanState obj_state, child_state;

    obj_state = workman_object_get_state(WORKMAN_OBJECT(self), NULL);
    g_return_val_if_fail(state & obj_state, FALSE);

    child_state = workman_object_get_state(WORKMAN_OBJECT(child), NULL);
    g_return_val_if_fail(state & child_state, FALSE);

    if (state & WORKMAN_STATE_ACTIVE) {
        g_object_ref(child);
        priv->children_active = g_list_append(priv->children_active,
                                              child);
        ret = TRUE;
    }

    if (state & WORKMAN_STATE_PERSISTENT) {
        g_object_ref(child);
        priv->children_persistent = g_list_append(priv->children_persistent,
                                                  child);
        ret = TRUE;
    }

    return ret;
}


gboolean
workman_partition_remove_child_priv(WorkmanPartition *self,
                                    WorkmanState state,
                                    WorkmanPartition *child)
{
    WorkmanPartitionPrivate *priv = self->priv;
    gboolean ret = FALSE;

    if (state & WORKMAN_STATE_ACTIVE &&
        g_list_find(priv->children_active, child)) {

        priv->children_active = g_list_remove(priv->children_active,
                                              child);
        g_object_unref(child);
        ret = TRUE;
    }

    if (state & WORKMAN_STATE_PERSISTENT &&
        g_list_find(priv->children_persistent, child)) {

        priv->children_persistent = g_list_remove(priv->children_persistent,
                                                  child);
        g_object_unref(child);
        ret = TRUE;
    }

    return ret;
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
