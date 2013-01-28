/*
 * workman-process.c: workload manager
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

G_DEFINE_TYPE(WorkmanProcess, workman_process, G_TYPE_OBJECT);

#define WORKMAN_PROCESS_GET_PRIVATE(obj) (G_TYPE_INSTANCE_GET_PRIVATE((obj), WORKMAN_TYPE_PROCESS, WorkmanProcessPrivate))


struct _WorkmanProcessPrivate {
    gchar *name;
    guint id;
};


static void workman_process_finalize(GObject *object)
{
    WorkmanProcess *attr = WORKMAN_PROCESS(object);
    WorkmanProcessPrivate *priv = attr->priv;

    g_free(priv->name);

    G_OBJECT_CLASS(workman_process_parent_class)->finalize(object);
}


static void
workman_process_class_init(WorkmanProcessClass *klass)
{
    GObjectClass *g_klass = G_OBJECT_CLASS(klass);

    g_klass->finalize = workman_process_finalize;

    g_type_class_add_private(klass, sizeof(WorkmanProcessPrivate));
}


static void
workman_process_init(WorkmanProcess *attr)
{
    WorkmanProcessPrivate *priv;
    attr->priv = priv = WORKMAN_PROCESS_GET_PRIVATE(attr);

}

WorkmanProcess *workman_process_new(const gchar *name,
                                    guint id)
{
    return WORKMAN_PROCESS(g_object_new(WORKMAN_TYPE_PROCESS,
                                        "name", name,
                                        "id", id,
                                        NULL));
}


/**
 * workman_process_get_name:
 *
 * Returns: (transfer none): the process name
 */
const gchar *workman_process_get_name(WorkmanProcess *attr)
{
    return attr->priv->name;
}


guint workman_process_get_id(WorkmanProcess *attr)
{
    return attr->priv->id;
}


/*
 * Local variables:
 *  indent-tabs-mode: nil
 *  c-indent-level: 4
 *  c-basic-offset: 4
 * End:
 */
