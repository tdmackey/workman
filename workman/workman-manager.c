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

G_DEFINE_TYPE(WorkmanManager, workman_manager, G_TYPE_OBJECT);

#define WORKMAN_MANAGER_GET_PRIVATE(obj) (G_TYPE_INSTANCE_GET_PRIVATE((obj), WORKMAN_TYPE_MANAGER, WorkmanManagerPrivate))


struct _WorkmanManagerPrivate {
    gboolean unused;
};

static void
workman_manager_class_init(WorkmanManagerClass *klass)
{
    GObjectClass *g_klass = G_OBJECT_CLASS(klass);

    g_type_class_add_private(klass, sizeof (WorkmanManagerPrivate));
}


static void
workman_manager_init(WorkmanManager *attr)
{
    WorkmanManagerPrivate *priv;
    attr->priv = priv = WORKMAN_MANAGER_GET_PRIVATE(attr);

}


WorkmanManager *workman_manager_new(void)
{
    return WORKMAN_MANAGER(g_object_new(WORKMAN_TYPE_MANAGER,
                                        NULL));
}


/*
 * Local variables:
 *  indent-tabs-mode: nil
 *  c-indent-level: 4
 *  c-basic-offset: 4
 * End:
 */
