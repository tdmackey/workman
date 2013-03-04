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
    GVariant *value;
    gboolean writable;
    WorkmanState state;
};


static void workman_attribute_finalize(GObject *object)
{
    WorkmanAttribute *attr = WORKMAN_ATTRIBUTE(object);
    WorkmanAttributePrivate *priv = attr->priv;

    g_free(priv->name);
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
                                        GVariant *value,
                                        gboolean writable)
{
    g_return_val_if_fail(g_variant_is_of_type(value,G_VARIANT_TYPE_MAYBE),
                         NULL);

    return WORKMAN_ATTRIBUTE(g_object_new(WORKMAN_TYPE_ATTRIBUTE,
                                          "name", name,
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
 * workman_attribute_get_value:
 *
 * Returns: (transfer full): the attribute %G_VARIANT_TYPE_MAYBE value
 */
GVariant *workman_attribute_get_value(WorkmanAttribute *attr)
{
    if (!attr->priv->value)
        return NULL;

    return g_variant_ref(attr->priv->value);
}


gboolean
workman_attribute_set_value(WorkmanAttribute *attr,
                            GVariant *value,
                            GError **error)
{
    g_return_val_if_fail(value != NULL, FALSE);

    if (attr->priv->value) {
        const GVariantType *old_type = g_variant_get_type(attr->priv->value);
        const GVariantType *new_type = g_variant_get_type(value);

        g_return_val_if_fail(g_variant_type_equal(old_type, new_type),
                             FALSE);

        g_variant_unref(attr->priv->value);
    }

    attr->priv->value = value;
    g_variant_ref(attr->priv->value);

    return TRUE;
}


/**
 * workman_attribute_get_state:
 *
 * Returns: the WorkmanState
 */
WorkmanState workman_attribute_get_state(WorkmanAttribute *self)
{
    return self->priv->state;
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
