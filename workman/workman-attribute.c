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


enum {
    PROP_0,
    PROP_NAME,
    PROP_VALUE,
    PROP_WRITABLE,
    PROP_STATE,
};


static void
workman_attribute_finalize(GObject *object)
{
    WorkmanAttribute *attr = WORKMAN_ATTRIBUTE(object);
    WorkmanAttributePrivate *priv = attr->priv;

    g_variant_unref(priv->value);
    g_free(priv->name);

    G_OBJECT_CLASS(workman_attribute_parent_class)->finalize(object);
}


static void
workman_attribute_get_property(GObject *object,
                               guint prop_id,
                               GValue *value,
                               GParamSpec *pspec)
{
    WorkmanAttribute *self = WORKMAN_ATTRIBUTE(object);

    switch (prop_id) {
        case PROP_NAME:
            g_value_set_string(value, self->priv->name);
            break;
        case PROP_VALUE:
            g_value_set_variant(value, self->priv->value);
            break;
        case PROP_WRITABLE:
            g_value_set_boolean(value, self->priv->writable);
            break;
        case PROP_STATE:
            g_value_set_flags(value, self->priv->state);
            break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
            break;
    }
}


static void
workman_attribute_set_property(GObject *object,
                               guint prop_id,
                               const GValue *value,
                               GParamSpec *pspec)
{
    WorkmanAttribute *self = WORKMAN_ATTRIBUTE(object);
    WorkmanState state;

    switch (prop_id) {
        case PROP_NAME:
            self->priv->name = g_value_dup_string(value);
            break;
        case PROP_VALUE:
            workman_attribute_set_value(self,
                                        g_value_get_variant(value),
                                        NULL);
            break;
        case PROP_WRITABLE:
            self->priv->writable = g_value_get_boolean(value);
            break;
        case PROP_STATE:
            state = g_value_get_flags(value);
            g_return_if_fail(state != WORKMAN_STATE_ALL);
            self->priv->state = state;
            break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
            break;
    }
}


static void
workman_attribute_class_init(WorkmanAttributeClass *klass)
{
    GObjectClass *g_klass = G_OBJECT_CLASS(klass);
    GParamSpec *pspec;

    g_klass->finalize = workman_attribute_finalize;
    g_klass->set_property = workman_attribute_set_property;
    g_klass->get_property = workman_attribute_get_property;

    pspec = g_param_spec_string("name",
                                "Name",
                                "The attribute name",
                                NULL,
                                G_PARAM_READWRITE |
                                G_PARAM_CONSTRUCT_ONLY |
                                G_PARAM_STATIC_STRINGS);
    g_object_class_install_property(g_klass, PROP_NAME, pspec);

    pspec = g_param_spec_variant("value",
                                 "Value",
                                 "The attribute value",
                                 G_VARIANT_TYPE_MAYBE,
                                 NULL,
                                 G_PARAM_READWRITE |
                                 G_PARAM_CONSTRUCT_ONLY |
                                 G_PARAM_STATIC_STRINGS);
    g_object_class_install_property(g_klass, PROP_VALUE, pspec);

    pspec = g_param_spec_boolean("writable",
                                 "Writable",
                                 "Whether the attribute's value is writable",
                                 FALSE,
                                 G_PARAM_READWRITE |
                                 G_PARAM_CONSTRUCT_ONLY |
                                 G_PARAM_STATIC_STRINGS);
    g_object_class_install_property(g_klass, PROP_WRITABLE, pspec);

    pspec = g_param_spec_flags("state",
                               "State",
                               "The object's WorkmanState",
                               WORKMAN_TYPE_STATE,
                               0,
                               G_PARAM_READWRITE |
                               G_PARAM_CONSTRUCT_ONLY |
                               G_PARAM_STATIC_STRINGS);
    g_object_class_install_property(g_klass, PROP_STATE, pspec);

    g_type_class_add_private(klass, sizeof(WorkmanAttributePrivate));
}


static void
workman_attribute_init(WorkmanAttribute *attr)
{
    WorkmanAttributePrivate *priv;
    attr->priv = priv = WORKMAN_ATTRIBUTE_GET_PRIVATE(attr);

}

WorkmanAttribute *workman_attribute_new(const gchar *name,
                                        WorkmanState state,
                                        GVariant *value,
                                        gboolean writable)
{
    g_return_val_if_fail(g_variant_is_of_type(value,G_VARIANT_TYPE_MAYBE),
                         NULL);
    g_return_val_if_fail(state != WORKMAN_STATE_ALL, NULL);

    return WORKMAN_ATTRIBUTE(g_object_new(WORKMAN_TYPE_ATTRIBUTE,
                                          "name", name,
                                          "state", state,
                                          "value", value,
                                          "writable", writable,
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


/**
 * workman_attribute_set_value:
 * @attr: a #WorkmanAttribute
 * @value: (transfer floating): the attribute %G_VARIANT_TYPE_MAYBE value to set
 * @error: (allow-none): return location for a #GError, or %NULL
 *
 * Returns: %TRUE if the operation succeeded
 */
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
    g_variant_ref_sink(attr->priv->value);

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
