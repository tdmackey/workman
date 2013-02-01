/*
 * workman-attribute.c: workload manager
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

G_DEFINE_TYPE(WorkmanAttribute, workman_attribute, G_TYPE_OBJECT);

#define WORKMAN_ATTRIBUTE_GET_PRIVATE(obj) (G_TYPE_INSTANCE_GET_PRIVATE((obj), WORKMAN_TYPE_ATTRIBUTE, WorkmanAttributePrivate))


struct _WorkmanAttributePrivate {
    gchar *name;
    gchar *format;
    GVariant *value;
    gboolean writable;
};


static void workman_attribute_finalize(GObject *object)
{
    WorkmanAttribute *attr = WORKMAN_ATTRIBUTE(object);
    WorkmanAttributePrivate *priv = attr->priv;

    g_free(priv->name);
    g_free(priv->format);
    if (priv->value)
        g_variant_unref(priv->value);

    G_OBJECT_CLASS(workman_attribute_parent_class)->finalize(object);
}


static void
workman_attribute_class_init(WorkmanAttributeClass *klass)
{
    GObjectClass *g_klass = G_OBJECT_CLASS(klass);

    g_klass->finalize = workman_attribute_finalize;

    g_type_class_add_private(klass, sizeof(WorkmanAttributePrivate));
}


static void
workman_attribute_init(WorkmanAttribute *attr)
{
    WorkmanAttributePrivate *priv;
    attr->priv = priv = WORKMAN_ATTRIBUTE_GET_PRIVATE(attr);

}

WorkmanAttribute *workman_attribute_new(const gchar *name,
                                        const gchar *format,
                                        GVariant *value,
                                        gboolean writable)
{
    return WORKMAN_ATTRIBUTE(g_object_new(WORKMAN_TYPE_ATTRIBUTE,
                                          "name", name,
                                          "format", format,
                                          "writable", writable,
                                          "value", value,
                                          NULL));
}


gboolean workman_attribute_get_writable(WorkmanAttribute *attr)
{
    return attr->priv->writable;
}


/**
 * workman_attribute_get_name:
 *
 * Returns: (transfer none): the attribute name
 */
const gchar *workman_attribute_get_name(WorkmanAttribute *attr)
{
    return attr->priv->name;
}


/**
 * workman_attribute_get_format:
 *
 * Returns: (transfer none): the variant format string
 */
const gchar *workman_attribute_get_format(WorkmanAttribute *attr)
{
    return attr->priv->format;
}


/**
 * workman_attribute_get_value:
 *
 * Returns: (transfer full): the attribute value
 */
GVariant *workman_attribute_get_value(WorkmanAttribute *attr)
{
    if (!attr->priv->value)
        return NULL;

    return g_variant_ref(attr->priv->value);
}


void workman_attribute_set_value(WorkmanAttribute *attr,
                                 GVariant *value,
                                 GError **error)
{
    if (attr->priv->value)
        g_variant_unref(attr->priv->value);
    /* XXX validate new value against priv->format */
    attr->priv->value = value;
    if (attr->priv->value)
        g_variant_ref(attr->priv->value);
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
