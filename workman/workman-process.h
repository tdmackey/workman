/*
 * workman-process.h: workload manager
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

#ifndef __WORKMAN_PROCESS_H__
# define __WORKMAN_PROCESS_H__

# include <glib-object.h>

G_BEGIN_DECLS

# define WORKMAN_TYPE_PROCESS            (workman_process_get_type ())
# define WORKMAN_PROCESS(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), WORKMAN_TYPE_PROCESS, WorkmanProcess))
# define WORKMAN_IS_PROCESS(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), WORKMAN_TYPE_PROCESS))
# define WORKMAN_PROCESS_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), WORKMAN_TYPE_PROCESS, WorkmanProcessClass))
# define WORKMAN_IS_PROCESS_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), WORKMAN_TYPE_PROCESS))
# define WORKMAN_PROCESS_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), WORKMAN_TYPE_PROCESS, WorkmanProcessClass))

typedef struct _WorkmanProcess        WorkmanProcess;
typedef struct _WorkmanProcessClass   WorkmanProcessClass;
typedef struct _WorkmanProcessPrivate WorkmanProcessPrivate;


struct _WorkmanProcess
{
    GObject parent_instance;

    /* public */

    /* private */
    WorkmanProcessPrivate *priv;
};

/* class */
struct _WorkmanProcessClass
{
    /*< private >*/
    GObjectClass parent_class;

    /* class members */
    gpointer padding[20];
};


GType workman_process_get_type(void);

WorkmanProcess *workman_process_new(const gchar *name,
                                    guint id);

const gchar *workman_process_get_name(WorkmanProcess *attr);
guint workman_process_get_id(WorkmanProcess *attr);

G_END_DECLS

#endif /* __WORKMAN_PROCESS_H__ */

/*
 * Local variables:
 *  indent-tabs-mode: nil
 *  c-indent-level: 4
 *  c-basic-offset: 4
 * End:
 */
