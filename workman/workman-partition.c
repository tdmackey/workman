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

G_DEFINE_ABSTRACT_TYPE(WorkmanPartition, workman_partition, WORKMAN_TYPE_OBJECT);

#define WORKMAN_PARTITION_GET_PRIVATE(obj) (G_TYPE_INSTANCE_GET_PRIVATE((obj), WORKMAN_TYPE_PARTITION, WorkmanPartitionPrivate))


struct _WorkmanPartitionPrivate {
    gboolean unused;
};

static void
workman_partition_class_init(WorkmanPartitionClass *klass)
{
    GObjectClass *g_klass = G_OBJECT_CLASS(klass);

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
GList *workman_partition_get_consumers(WorkmanPartition *partition,
                                       WorkmanState state,
                                       GError **error)
{
    WorkmanPartitionClass *klass = WORKMAN_PARTITION_GET_CLASS(partition);
    return klass->get_consumers(partition, state, error);
}


/**
 * workman_partition_get_subdivisions:
 *
 * Returns: (transfer full) (element-type WorkmanPartition): the sub-divisons of this partition
 */
GList *workman_partition_get_subdivisions(WorkmanPartition *partition,
                                          WorkmanState state,
                                          GError **error)
{
    WorkmanPartitionClass *klass = WORKMAN_PARTITION_GET_CLASS(partition);
    return klass->get_subdivisions(partition, state, error);
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
