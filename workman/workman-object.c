/*
 * workman-object.c: workload manager
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

G_DEFINE_ABSTRACT_TYPE(WorkmanObject, workman_object, G_TYPE_OBJECT);

#define WORKMAN_OBJECT_GET_PRIVATE(obj) (G_TYPE_INSTANCE_GET_PRIVATE((obj), WORKMAN_TYPE_OBJECT, WorkmanObjectPrivate))


struct _WorkmanObjectPrivate {
    gchar *name;
    WorkmanState state;
    GList *attributes;
};


enum {
    PROP_0,
    PROP_NAME,
    PROP_STATE,
    PROP_ATTRIBUTES,
};



static void
workman_object_get_property(GObject *object,
                            guint prop_id,
                            GValue *value,
                            GParamSpec *pspec)
{
    WorkmanObject *self = WORKMAN_OBJECT(object);
    GList *attributes;

    switch (prop_id) {
        case PROP_NAME:
            g_value_set_string(value, self->priv->name);
            break;
        case PROP_STATE:
            g_value_set_flags(value, self->priv->state);
            break;
        case PROP_ATTRIBUTES:
            g_value_set_boxed(value, self->priv->attributes);
            break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
            break;
    }
}


static void
workman_object_set_property(GObject *object,
                            guint prop_id,
                            const GValue *value,
                            GParamSpec *pspec)
{
    WorkmanObject *self = WORKMAN_OBJECT(object);

    switch (prop_id) {
        case PROP_NAME:
            self->priv->name = g_value_dup_string(value);
            break;
        case PROP_STATE:
            self->priv->state = g_value_get_flags(value);
            break;
        case PROP_ATTRIBUTES:
            g_boxed_free(WORKMAN_TYPE_ATTRIBUTE_LIST, self->priv->attributes);
            self->priv->attributes = g_value_dup_boxed(value);
            break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
            break;
    }
}


static void
workman_object_dispose(GObject *object)
{
    WorkmanObject *self = WORKMAN_OBJECT(object);

    g_boxed_free(WORKMAN_TYPE_ATTRIBUTE_LIST, self->priv->attributes);
    self->priv->attributes = NULL;

    G_OBJECT_CLASS(workman_object_parent_class)->dispose(object);
}

static void
workman_object_finalize(GObject *object)
{
    WorkmanObject *obj = WORKMAN_OBJECT(object);
    WorkmanObjectPrivate *priv = obj->priv;

    g_free(priv->name);

    G_OBJECT_CLASS(workman_object_parent_class)->finalize(object);
}


static void
workman_object_class_init(WorkmanObjectClass *klass)
{
    GObjectClass *g_klass = G_OBJECT_CLASS(klass);
    GParamSpec *pspec;

    g_klass->dispose = workman_object_dispose;
    g_klass->finalize = workman_object_finalize;
    g_klass->set_property = workman_object_set_property;
    g_klass->get_property = workman_object_get_property;

    pspec = g_param_spec_string("name",
                                "Name",
                                "The object name",
                                NULL,
                                G_PARAM_READWRITE |
                                G_PARAM_CONSTRUCT_ONLY |
                                G_PARAM_STATIC_STRINGS);
    g_object_class_install_property(g_klass, PROP_NAME, pspec);

    pspec = g_param_spec_flags("state",
                               "State",
                               "The object's WorkmanState",
                               WORKMAN_TYPE_STATE,
                               0,
                               G_PARAM_READWRITE |
                               G_PARAM_CONSTRUCT_ONLY |
                               G_PARAM_STATIC_STRINGS);
    g_object_class_install_property(g_klass, PROP_STATE, pspec);


    /**
     * WorkmanObject:attributes:
     *
     * Type: GLib.List(WorkmanAttribute)
     * Transfer: full
     */
    pspec = g_param_spec_boxed("attributes",
                               "Attributes",
                               "The object's list of WorkmanAttributes",
                               WORKMAN_TYPE_ATTRIBUTE_LIST,
                               G_PARAM_READWRITE |
                               G_PARAM_CONSTRUCT_ONLY |
                               G_PARAM_STATIC_STRINGS);

    g_object_class_install_property(g_klass, PROP_ATTRIBUTES, pspec);

    g_type_class_add_private(klass, sizeof(WorkmanObjectPrivate));
}


static void
workman_object_init(WorkmanObject *object)
{
    WorkmanObjectPrivate *priv;
    object->priv = priv = WORKMAN_OBJECT_GET_PRIVATE(object);

}


const gchar *workman_object_get_name(WorkmanObject *obj)
{
    return obj->priv->name;
}

/**
 * workman_object_get_attributes:
 *
 * Returns: (transfer full) (element-type WorkmanAttribute): the attributes for this object
 */
GList *workman_object_get_attributes(WorkmanObject *self,
                                     WorkmanState state,
                                     GError **error)
{
    GList *new_attrs, *a;

    new_attrs = NULL;

    for (a = self->priv->attributes; a; a = a->next) {
        WorkmanAttribute *attr = WORKMAN_ATTRIBUTE(a->data);
        if (workman_attribute_get_state(attr) & state) {
            g_object_ref(attr);
            new_attrs = g_list_append(new_attrs, attr);
        }
    }

    return new_attrs;
}


/**
 * workman_object_get_attribute:
 *
 * Returns: (transfer full): the attribute or NULL
 */
WorkmanAttribute *workman_object_get_attribute(WorkmanObject *self,
                                               WorkmanState state,
                                               const gchar *name,
                                               GError **error)
{
    GList *a;
    WorkmanAttribute *attr;

    for (a = self->priv->attributes; a; a = a->next) {
        attr = WORKMAN_ATTRIBUTE(a->data);
        if (workman_attribute_get_state(attr) == state &&
            !g_strcmp0(workman_attribute_get_name(attr), name))
            return g_object_ref(attr);
    }

    return NULL;
}


gboolean workman_object_refresh_attributes(WorkmanObject *self,
                                           WorkmanState state,
                                           GError **error)
{
    /* TODO: call plugin->get_attributes and update obj->priv->attributes */
}


gboolean workman_object_save_attributes(WorkmanObject *self,
                                        WorkmanState state,
                                        GError **error)
{
    /* TODO: call plugin->update_attribute for each changed attribute */
}


WorkmanState workman_object_get_state(WorkmanObject *self,
                                      GError **error)
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
