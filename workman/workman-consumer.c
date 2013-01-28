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

G_DEFINE_ABSTRACT_TYPE(WorkmanConsumer, workman_consumer, WORKMAN_TYPE_OBJECT);

#define WORKMAN_CONSUMER_GET_PRIVATE(obj) (G_TYPE_INSTANCE_GET_PRIVATE((obj), WORKMAN_TYPE_CONSUMER, WorkmanConsumerPrivate))


struct _WorkmanConsumerPrivate {
    gboolean unused;
};

static void
workman_consumer_class_init(WorkmanConsumerClass *klass)
{
    GObjectClass *g_klass = G_OBJECT_CLASS(klass);

    g_type_class_add_private(klass, sizeof (WorkmanConsumerPrivate));
}


static void
workman_consumer_init(WorkmanConsumer *attr)
{
    WorkmanConsumerPrivate *priv;
    attr->priv = priv = WORKMAN_CONSUMER_GET_PRIVATE(attr);

}

gboolean workman_consumer_set_partition(WorkmanConsumer *consumer,
					WorkmanState state,
					WorkmanPartition *partition,
					GError **error)
{
    WorkmanConsumerClass *klass = WORKMAN_CONSUMER_GET_CLASS(consumer);
    return klass->set_partition(consumer, state, partition, error);
}


/**
 * workman_consumer_get_partition:
 *
 * Returns: (transfer full): the partition
 */
WorkmanPartition *workman_consumer_get_partition(WorkmanConsumer *consumer,
						 WorkmanState state,
						 GError **error)
{
    WorkmanConsumerClass *klass = WORKMAN_CONSUMER_GET_CLASS(consumer);
    return klass->get_partition(consumer, state, error);
}


gboolean workman_consumer_get_persistent(WorkmanConsumer *consumer,
					 GError **error)
{
    WorkmanConsumerClass *klass = WORKMAN_CONSUMER_GET_CLASS(consumer);
    return klass->get_persistent(consumer, error);
}

gboolean workman_consumer_get_active(WorkmanConsumer *consumer,
				     GError **error)
{
    WorkmanConsumerClass *klass = WORKMAN_CONSUMER_GET_CLASS(consumer);
    return klass->get_active(consumer, error);
}


/**
 * workman_consumer_get_processes:
 *
 * Returns: (transfer full) (element-type WorkmanProcess): the partition
 */
GList *workman_consumer_get_processes(WorkmanConsumer *consumer,
				      GError **error)
{
    WorkmanConsumerClass *klass = WORKMAN_CONSUMER_GET_CLASS(consumer);
    return klass->get_processes(consumer, error);
}



/*
 * Local variables:
 *  indent-tabs-mode: nil
 *  c-indent-level: 4
 *  c-basic-offset: 4
 * End:
 */
