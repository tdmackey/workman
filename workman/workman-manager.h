/*
 * workman-manager.h: workload manager
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

#if !defined(__WORKMAN_H__) && !defined(WORKMAN_BUILD)
# error "Only <workman/workman.h> can be included directly."
#endif

#ifndef __WORKMAN_MANAGER_H__
# define __WORKMAN_MANAGER_H__

# include <glib-object.h>

G_BEGIN_DECLS

# define WORKMAN_TYPE_MANAGER            (workman_manager_get_type ())
# define WORKMAN_MANAGER(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), WORKMAN_TYPE_MANAGER, WorkmanManager))
# define WORKMAN_IS_MANAGER(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), WORKMAN_TYPE_MANAGER))
# define WORKMAN_MANAGER_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), WORKMAN_TYPE_MANAGER, WorkmanManagerClass))
# define WORKMAN_IS_MANAGER_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), WORKMAN_TYPE_MANAGER))
# define WORKMAN_MANAGER_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), WORKMAN_TYPE_MANAGER, WorkmanManagerClass))

typedef struct _WorkmanManager        WorkmanManager;
typedef struct _WorkmanManagerClass   WorkmanManagerClass;
typedef struct _WorkmanManagerPrivate WorkmanManagerPrivate;

struct _WorkmanManager
{
    GObject parent_instance;

    /* public */

    /* private */
    WorkmanManagerPrivate *priv;
};

/* class */
struct _WorkmanManagerClass
{
    /*< private >*/
    GObjectClass parent_class;

    /* class members */

    WorkmanPartition *(*add_partition)(WorkmanManager *mgr,
                                       WorkmanPartition *parent,
                                       WorkmanState state,
                                       GError **error);

    gboolean (*remove_partition)(WorkmanManager *mgr,
                                 WorkmanState state,
                                 GError **error);

    GList *(*get_partitions)(WorkmanManager *mgr,
                             WorkmanState state,
                             GError **error);

    GList *(*get_consumers)(WorkmanManager *mgr,
                            WorkmanState state,
                            GError **error);

    /* Remove from padding when adding new virtual functions */
    gpointer padding[20];
};

GType workman_manager_get_type(void);

WorkmanManager *workman_manager_get_default(GError **error);

WorkmanPartition *workman_manager_add_partition(WorkmanManager *mgr,
                                                WorkmanPartition *parent,
                                                WorkmanState state,
                                                GError **error);

gboolean workman_manager_remove_partition(WorkmanManager *mgr,
                                          WorkmanState state,
                                          GError **error);

GList *workman_manager_get_partitions(WorkmanManager *mgr,
                                      WorkmanState state,
                                      GError **error);

GList *workman_manager_get_consumers(WorkmanManager *mgr,
                                     WorkmanState state,
                                     GError **error);

G_END_DECLS

#endif /* __WORKMAN_OBJECT_H__ */

/*
 * Local variables:
 *  indent-tabs-mode: nil
 *  c-indent-level: 4
 *  c-basic-offset: 4
 * End:
 *
 * vim: set expandtab shiftwidth=4 tabstop=4 :
 */
