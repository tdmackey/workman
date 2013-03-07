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
    WorkmanPartition *parent;
    GList *children;
    GList *consumers;
};


enum {
    PROP_0,
    PROP_PARENT,
    PROP_CHILDREN,
    PROP_CONSUMERS,
};


static void
workman_partition_dispose(GObject *object)
{
    WorkmanPartition *self = WORKMAN_PARTITION(self);

    g_boxed_free(WORKMAN_TYPE_PARTITION_LIST, self->priv->children);
    self->priv->children = NULL;

    g_boxed_free(WORKMAN_TYPE_CONSUMER_LIST, self->priv->consumers);
    self->priv->consumers = NULL;

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
        case PROP_PARENT:
            g_value_set_object(value, self->priv->parent);
            break;
        case PROP_CHILDREN:
            g_value_set_boxed(value, self->priv->children);
            break;
        case PROP_CONSUMERS:
            g_value_set_boxed(value, self->priv->consumers);
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
        case PROP_PARENT:
            g_object_unref(self->priv->parent);
            self->priv->parent = g_value_dup_object(value);
            break;
        case PROP_CHILDREN:
            g_boxed_free(WORKMAN_TYPE_PARTITION_LIST, self->priv->children);
            self->priv->children = g_value_dup_boxed(value);
            break;
        case PROP_CONSUMERS:
            g_boxed_free(WORKMAN_TYPE_CONSUMER_LIST, self->priv->consumers);
            self->priv->consumers = g_value_dup_boxed(value);
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

    pspec = g_param_spec_object("parent",
                                "Parent",
                                "The partition's parent partition",
                                WORKMAN_TYPE_PARTITION,
                                G_PARAM_READWRITE |
                                G_PARAM_STATIC_STRINGS);
    g_object_class_install_property(g_klass, PROP_PARENT, pspec);


    /**
     * WorkmanPartition:children:
     *
     * Type: GLib.List(WorkmanPartition)
     * Transfer: full
     */
    pspec = g_param_spec_boxed("children",
                               "Children",
                               "The partition's list of children partitions",
                               WORKMAN_TYPE_PARTITION_LIST,
                               G_PARAM_READWRITE |
                               G_PARAM_CONSTRUCT_ONLY |
                               G_PARAM_STATIC_STRINGS);
    g_object_class_install_property(g_klass, PROP_CHILDREN, pspec);


    /**
     * WorkmanPartition:consumers:
     *
     * Type: GLib.List(WorkmanConsumer)
     * Transfer: full
     */
    pspec = g_param_spec_boxed("consumers",
                               "Consumers",
                               "The partition's list of consumers",
                               WORKMAN_TYPE_CONSUMER_LIST,
                               G_PARAM_READWRITE |
                               G_PARAM_CONSTRUCT_ONLY |
                               G_PARAM_STATIC_STRINGS);
    g_object_class_install_property(g_klass, PROP_CONSUMERS, pspec);

    g_type_class_add_private(klass, sizeof(WorkmanPartitionPrivate));
}


static void
workman_partition_init(WorkmanPartition *attr)
{
    WorkmanPartitionPrivate *priv;
    attr->priv = priv = WORKMAN_PARTITION_GET_PRIVATE(attr);

}


static GList *
workman_partition_copy_objects(GList *list,
                               WorkmanState state,
                               GError **error)
{
    GList *new_list, *l;

    for (new_list = NULL, l = list; l; l = l->next) {
        WorkmanObject *obj = WORKMAN_OBJECT(l->data);
        if (workman_object_get_state(obj, NULL) & state) {
            g_object_ref(obj);
            new_list = g_list_append(new_list, obj);
        }
    }

    return new_list;
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
    return workman_partition_copy_objects(self->priv->consumers, state, error);
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
    return workman_partition_copy_objects(self->priv->children, state, error);
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
    return g_object_ref(self->priv->parent);
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


gboolean
workman_partition_add_consumer(WorkmanPartition *self,
                               WorkmanConsumer *consumer)
{
    g_object_ref(consumer);
    self->priv->consumers = g_list_append(self->priv->consumers, consumer);
    return TRUE;
}


gboolean
workman_partition_remove_consumer(WorkmanPartition *self,
                                  WorkmanConsumer *consumer)
{
    GList *l;

    if (!g_list_find(self->priv->consumers, consumer))
        return FALSE;

    self->priv->consumers = g_list_remove(self->priv->consumers, consumer);
    g_object_unref(consumer);
    return TRUE;
}


gboolean
workman_partition_add_child(WorkmanPartition *self,
                            WorkmanPartition *child)
{
    g_object_ref(child);
    self->priv->children = g_list_append(self->priv->children, child);
}


gboolean
workman_partition_remove_child(WorkmanPartition *self,
                               WorkmanPartition *child)
{
    GList *l;

    if (!g_list_find(self->priv->children, child))
        return FALSE;

    self->priv->children = g_list_remove(self->priv->children, child);
    g_object_unref(child);
    return TRUE;
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
