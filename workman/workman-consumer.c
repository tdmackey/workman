/*
 * workman-consumer.c: workload consumer
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
#include "workman-consumer-private.h"

G_DEFINE_TYPE(WorkmanConsumer, workman_consumer, WORKMAN_TYPE_OBJECT);

#define WORKMAN_CONSUMER_GET_PRIVATE(obj) (G_TYPE_INSTANCE_GET_PRIVATE((obj), WORKMAN_TYPE_CONSUMER, WorkmanConsumerPrivate))


struct _WorkmanConsumerPrivate {
    WorkmanPartition *partition_active;
    WorkmanPartition *partition_persistent;
    GList *processes;
};


enum {
    PROP_0,
    PROP_PARTITION_ACTIVE,
    PROP_PARTITION_PERSISTENT,
    PROP_PROCESSES,
};


static void
workman_consumer_dispose(GObject *object)
{
    WorkmanConsumer *self = WORKMAN_CONSUMER(object);

    g_object_unref(self->priv->partition_active);
    self->priv->partition_active = NULL;

    g_object_unref(self->priv->partition_persistent);
    self->priv->partition_persistent = NULL;

    g_boxed_free(WORKMAN_TYPE_PROCESS_LIST, self->priv->processes);
    self->priv->processes = NULL;

    G_OBJECT_CLASS(workman_consumer_parent_class)->dispose(object);
}


static void
workman_consumer_get_property(GObject *object,
                              guint prop_id,
                              GValue *value,
                              GParamSpec *pspec)
{
    WorkmanConsumer *self = WORKMAN_CONSUMER(object);

    switch (prop_id) {
        case PROP_PARTITION_ACTIVE:
            g_value_set_object(value, self->priv->partition_active);
            break;
        case PROP_PARTITION_PERSISTENT:
            g_value_set_object(value, self->priv->partition_persistent);
            break;
        case PROP_PROCESSES:
            g_value_set_boxed(value, self->priv->processes);
            break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
            break;
    }
}


static void
workman_consumer_set_property(GObject *object,
                              guint prop_id,
                              const GValue *value,
                              GParamSpec *pspec)
{
    WorkmanConsumer *self = WORKMAN_CONSUMER(object);

    switch (prop_id) {
        case PROP_PARTITION_ACTIVE:
            g_object_unref(self->priv->partition_active);
            self->priv->partition_active = g_value_dup_object(value);
            break;
        case PROP_PARTITION_PERSISTENT:
            g_object_unref(self->priv->partition_active);
            self->priv->partition_active = g_value_dup_object(value);
            break;
        case PROP_PROCESSES:
            g_boxed_free(WORKMAN_TYPE_PROCESS_LIST, self->priv->processes);
            self->priv->processes = g_value_dup_boxed(value);
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
            break;
    }
}


static void
workman_consumer_class_init(WorkmanConsumerClass *klass)
{
    GObjectClass *g_klass = G_OBJECT_CLASS(klass);
    GParamSpec *pspec;

    g_klass->dispose = workman_consumer_dispose;
    g_klass->set_property = workman_consumer_set_property;
    g_klass->get_property = workman_consumer_get_property;

    pspec = g_param_spec_object("partition-active",
                                "Active Partition",
                                "The consumer's partition in the active state",
                                WORKMAN_TYPE_PARTITION,
                                G_PARAM_READWRITE |
                                G_PARAM_CONSTRUCT_ONLY |
                                G_PARAM_STATIC_STRINGS);
    g_object_class_install_property(g_klass, PROP_PARTITION_ACTIVE, pspec);

    pspec = g_param_spec_object("partition-persistent",
                                "Persistent Partition",
                                "The consumer's partition in the persistent state",
                                WORKMAN_TYPE_PARTITION,
                                G_PARAM_READWRITE |
                                G_PARAM_CONSTRUCT_ONLY |
                                G_PARAM_STATIC_STRINGS);
    g_object_class_install_property(g_klass, PROP_PARTITION_PERSISTENT, pspec);


    /**
     * WorkmanConsumer:processes:
     *
     * Type: GLib.List(WorkmanProcess)
     * Transfer: full
     */
    pspec = g_param_spec_boxed("processes",
                               "Processes",
                               "The consumer's list of processes",
                               WORKMAN_TYPE_PROCESS_LIST,
                               G_PARAM_READWRITE |
                               G_PARAM_CONSTRUCT_ONLY |
                               G_PARAM_STATIC_STRINGS);
    g_object_class_install_property(g_klass, PROP_PROCESSES, pspec);

    g_type_class_add_private(klass, sizeof(WorkmanConsumerPrivate));
}


static void
workman_consumer_init(WorkmanConsumer *self)
{
    WorkmanConsumerPrivate *priv;
    self->priv = priv = WORKMAN_CONSUMER_GET_PRIVATE(self);

}

gboolean
workman_consumer_set_partition(WorkmanConsumer *consumer,
                               WorkmanState state,
                               WorkmanPartition *partition,
                               GError **error)
{
    /* TODO:
     * - ensure state is compatible with consumer and partition states
     * - call plugin->set_parent
     * - update private data structure
     */
}


/**
 * workman_consumer_get_partition:
 *
 * Returns: (transfer full): the partition
 */
WorkmanPartition *
workman_consumer_get_partition(WorkmanConsumer *self,
                               WorkmanState state,
                               GError **error)
{
    if (state == WORKMAN_STATE_ACTIVE)
        return g_object_ref(self->priv->partition_active);

    if (state == WORKMAN_STATE_PERSISTENT)
        return g_object_ref(self->priv->partition_persistent);

    return NULL;
}


/**
 * workman_consumer_get_processes:
 *
 * Returns: (transfer full) (element-type WorkmanProcess): the processes
 */
GList *
workman_consumer_get_processes(WorkmanConsumer *self,
                               GError **error)
{
    return g_boxed_copy(WORKMAN_TYPE_PROCESS_LIST,
                        self->priv->processes);
}


gboolean
workman_consumer_refresh_processes(WorkmanConsumer *self,
                                   GError **error)
{
    /* TODO: call plugin */
    return FALSE;
}


WorkmanConsumer *
workman_consumer_new(const gchar *name,
                     WorkmanState state,
                     GList *attributes,
                     WorkmanPartition *partition_active,
                     WorkmanPartition *partition_persistent,
                     GList *processes)
{
    if (partition_active && !(state & WORKMAN_STATE_ACTIVE))
        return NULL;

    if (partition_persistent && !(state & WORKMAN_STATE_PERSISTENT))
        return NULL;

    return WORKMAN_CONSUMER(g_object_new(WORKMAN_TYPE_CONSUMER,
                                         "name", name,
                                         "state", state,
                                         "attributes", attributes,
                                         "partition-active", partition_active,
                                         "partition-persistent", partition_persistent,
                                         "processes", processes,
                                         NULL));
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
