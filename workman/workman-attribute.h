/*
 * workman-attribute.h: workload manager
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

#ifndef __WORKMAN_ATTRIBUTE_H__
# define __WORKMAN_ATTRIBUTE_H__

# include <glib-object.h>

G_BEGIN_DECLS

#define WORKMAN_TYPE_ATTRIBUTE            (workman_attribute_get_type ())
#define WORKMAN_ATTRIBUTE(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), WORKMAN_TYPE_ATTRIBUTE, WorkmanAttribute))
#define WORKMAN_IS_ATTRIBUTE(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), WORKMAN_TYPE_ATTRIBUTE))
#define WORKMAN_ATTRIBUTE_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), WORKMAN_TYPE_ATTRIBUTE, WorkmanAttributeClass))
#define WORKMAN_IS_ATTRIBUTE_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), WORKMAN_TYPE_ATTRIBUTE))
#define WORKMAN_ATTRIBUTE_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), WORKMAN_TYPE_ATTRIBUTE, WorkmanAttributeClass))

typedef struct _WorkmanAttribute        WorkmanAttribute;
typedef struct _WorkmanAttributeClass   WorkmanAttributeClass;
typedef struct _WorkmanAttributePrivate WorkmanAttributePrivate;


struct _WorkmanAttribute
{
    GObject parent_instance;

    /* public */

    /* private */
    WorkmanAttributePrivate *priv;
};

/* class */
struct _WorkmanAttributeClass
{
    /*< private >*/
    GObjectClass parent_class;

    /* class members */
    gpointer padding[20];
};


GType workman_attribute_get_type(void);

WorkmanAttribute *workman_attribute_new(const gchar *name,
                                        const gchar *format,
                                        GVariant *value,
                                        gboolean writable);

gboolean workman_attribute_get_writable(WorkmanAttribute *attr);
const gchar *workman_attribute_get_name(WorkmanAttribute *attr);

const gchar *workman_attribute_get_format(WorkmanAttribute *attr);

GVariant *workman_attribute_get_value(WorkmanAttribute *attr);

void workman_attribute_set_value(WorkmanAttribute *attr,
                                 GVariant *value,
                                 GError **error);

G_END_DECLS

#endif /* __WORKMAN_ATTRIBUTE_H__ */

/*
 * Local variables:
 *  indent-tabs-mode: nil
 *  c-indent-level: 4
 *  c-basic-offset: 4
 * End:
 */
