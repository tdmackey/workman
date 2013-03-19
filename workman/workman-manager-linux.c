/*
 * workman-manager-linux.c: workload manager
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
 * Author: Josh Poimboeuf <jpoimboe@redhat.com>
 */

#include <config.h>

#include "workman.h"
#include "workman-manager-linux.h"

G_DEFINE_TYPE(WorkmanManagerLinux, workman_manager_linux, WORKMAN_TYPE_MANAGER);

#define WORKMAN_MANAGER_LINUX_GET_PRIVATE(obj) (G_TYPE_INSTANCE_GET_PRIVATE((obj), WORKMAN_TYPE_MANAGER_LINUX, WorkmanManagerLinuxPrivate))


struct _WorkmanManagerLinuxPrivate {
    gboolean reserved;
};


static void
workman_manager_linux_class_init(WorkmanManagerLinuxClass *klass)
{
    g_type_class_add_private(klass, sizeof(WorkmanManagerLinuxPrivate));
}


static void
workman_manager_linux_init(WorkmanManagerLinux *attr)
{
    WorkmanManagerLinuxPrivate *priv;
    attr->priv = priv = WORKMAN_MANAGER_LINUX_GET_PRIVATE(attr);
}


WorkmanManagerLinux *
workman_manager_linux_new(void)
{
    GList *plugins = NULL;

    /* TODO: allocate plugins and add to list */

    return WORKMAN_MANAGER_LINUX(g_object_new(WORKMAN_TYPE_MANAGER_LINUX,
                                              "plugins", plugins,
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
