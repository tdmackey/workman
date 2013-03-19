/*
 * workman-manager-linux.h: workload manager
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

#if !defined(__WORKMAN_H__) && !defined(WORKMAN_BUILD)
# error "Only <workman/workman.h> can be included directly."
#endif

#ifndef __WORKMAN_MANAGER_LINUX_H__
# define __WORKMAN_MANAGER_LINUX_H__

# include <glib-object.h>

G_BEGIN_DECLS

# define WORKMAN_TYPE_MANAGER_LINUX            (workman_manager_linux_get_type ())
# define WORKMAN_MANAGER_LINUX(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), WORKMAN_TYPE_MANAGER_LINUX, WorkmanManagerLinux))
# define WORKMAN_IS_MANAGER_LINUX(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), WORKMAN_TYPE_MANAGER_LINUX))
# define WORKMAN_MANAGER_LINUX_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), WORKMAN_TYPE_MANAGER_LINUX, WorkmanManagerLinuxClass))
# define WORKMAN_IS_MANAGER_LINUX_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), WORKMAN_TYPE_MANAGER_LINUX))
# define WORKMAN_MANAGER_LINUX_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), WORKMAN_TYPE_MANAGER_LINUX, WorkmanManagerLinuxClass))

typedef struct _WorkmanManagerLinux        WorkmanManagerLinux;
typedef struct _WorkmanManagerLinuxClass   WorkmanManagerLinuxClass;
typedef struct _WorkmanManagerLinuxPrivate WorkmanManagerLinuxPrivate;

struct _WorkmanManagerLinux
{
    WorkmanManager parent_instance;

    /* public */

    /* private */
    WorkmanManagerLinuxPrivate *priv;
};

/* class */
struct _WorkmanManagerLinuxClass
{
    /*< private >*/
    WorkmanManagerClass parent_class;


    /* Remove from padding when adding new virtual functions */
    gpointer padding[20];
};

GType workman_manager_linux_get_type(void);


WorkmanManagerLinux *workman_manager_linux_new(void);


G_END_DECLS

#endif /* __WORKMAN_MANAGER_LINUX_H__ */

/*
 * Local variables:
 *  indent-tabs-mode: nil
 *  c-indent-level: 4
 *  c-basic-offset: 4
 * End:
 *
 * vim: set expandtab shiftwidth=4 tabstop=4 :
 */
