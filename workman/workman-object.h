/*
 * workman-object.h: workload manager
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

#ifndef __WORKMAN_OBJECT_H__
# define __WORKMAN_OBJECT_H__

# include <glib-object.h>

G_BEGIN_DECLS

# define WORKMAN_TYPE_OBJECT            (workman_object_get_type ())
# define WORKMAN_OBJECT(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), WORKMAN_TYPE_OBJECT, WorkmanObject))
# define WORKMAN_IS_OBJECT(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), WORKMAN_TYPE_OBJECT))
# define WORKMAN_OBJECT_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), WORKMAN_TYPE_OBJECT, WorkmanObjectClass))
# define WORKMAN_IS_OBJECT_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), WORKMAN_TYPE_OBJECT))
# define WORKMAN_OBJECT_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), WORKMAN_TYPE_OBJECT, WorkmanObjectClass))

typedef struct _WorkmanObject        WorkmanObject;
typedef struct _WorkmanObjectClass   WorkmanObjectClass;
typedef struct _WorkmanObjectPrivate WorkmanObjectPrivate;

struct _WorkmanObject
{
    GObject parent_instance;

    /* public */

    /* private */
    WorkmanObjectPrivate *priv;
};

/* class */
struct _WorkmanObjectClass
{
    /*< private >*/
    GObjectClass parent_class;


    /* Remove from padding when adding new virtual functions */
    gpointer padding[20];
};

GType workman_object_get_type(void);

const gchar *workman_object_get_name(WorkmanObject *obj);

GList *workman_object_get_attributes(WorkmanObject *obj,
                                     WorkmanState state,
                                     GError **error);

WorkmanAttribute *workman_object_get_attribute(WorkmanObject *obj,
                                               WorkmanState state,
                                               const gchar *name,
                                               GError **error);

gboolean workman_object_refresh_attributes(WorkmanObject *obj,
                                           WorkmanState state,
                                           GError **error);

gboolean workman_object_save_attributes(WorkmanObject *obj,
                                        WorkmanState state,
                                        GError **error);

WorkmanState workman_object_get_state(WorkmanObject *obj,
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
