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

G_DEFINE_ABSTRACT_TYPE(WorkmanManager, workman_manager, G_TYPE_OBJECT);

#define WORKMAN_MANAGER_GET_PRIVATE(obj) (G_TYPE_INSTANCE_GET_PRIVATE((obj), WORKMAN_TYPE_MANAGER, WorkmanManagerPrivate))


struct _WorkmanManagerPrivate {
    gboolean unused;
};

static void
workman_manager_class_init(WorkmanManagerClass *klass)
{
    GObjectClass *g_klass = G_OBJECT_CLASS(klass);

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
 * Returns: (transfer full): the default workload mnager object
 */
WorkmanManager *workman_manager_get_default(GError **error)
{
    /* XXX return an instance of WorkmanManagerLinux */
    return NULL;
}


/**
 * workman_manager_add_partition:
 * @mgr: (transfer none): the manager object
 * @parent: (transfer none)(allow-none): the parent of the new partition
 * @state: the state to create the new partition in
 *
 * Returns: (transfer full): the new partition
 */
WorkmanPartition *workman_manager_add_partition(WorkmanManager *mgr,
                                                WorkmanPartition *parent,
                                                WorkmanState state,
                                                GError **error)
{
    WorkmanManagerClass *klass = WORKMAN_MANAGER_GET_CLASS(mgr);
    return klass->add_partition(mgr, parent, state, error);
}

gboolean workman_manager_remove_partition(WorkmanManager *mgr,
                                          WorkmanState state,
                                          GError **error)
{
    WorkmanManagerClass *klass = WORKMAN_MANAGER_GET_CLASS(mgr);
    return klass->remove_partition(mgr, state, error);
}


/**
 * workman_manager_get_partitions:
 *
 * Returns: (transfer full)(element-type WorkmanPartition): the list of partitions
 */
GList *workman_manager_get_partitions(WorkmanManager *mgr,
                                      WorkmanState state,
                                      GError **error)
{
    WorkmanManagerClass *klass = WORKMAN_MANAGER_GET_CLASS(mgr);
    return klass->get_partitions(mgr, state, error);
}


/**
 * workman_manager_get_consumers:
 *
 * Returns: (transfer full)(element-type WorkmanConsumer): the list of consumers
 */
GList *workman_manager_get_consumers(WorkmanManager *mgr,
                                     WorkmanState state,
                                     GError **error)
{
    WorkmanManagerClass *klass = WORKMAN_MANAGER_GET_CLASS(mgr);
    return klass->get_consumers(mgr, state, error);
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
