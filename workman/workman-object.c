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
};


enum {
    PROP_0,
    PROP_NAME,
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
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
            break;
    }
}


void workman_object_finalize(GObject *object)
{
    WorkmanObject *attr = WORKMAN_OBJECT(object);
    WorkmanObjectPrivate *priv = attr->priv;

    g_free(priv->name);

    G_OBJECT_CLASS(workman_object_parent_class)->finalize(object);
}


static void
workman_object_class_init(WorkmanObjectClass *klass)
{
    GObjectClass *g_klass = G_OBJECT_CLASS(klass);
    GParamSpec *pspec;

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
GList *workman_object_get_attributes(WorkmanObject *obj,
                                     WorkmanState state,
                                     GError **error)
{
    WorkmanObjectClass *klass = WORKMAN_OBJECT_GET_CLASS(obj);
    return klass->get_attributes(obj, state, error);
}


/**
 * workman_object_get_attribute:
 *
 * Returns: (transfer full): the attribute or NULL
 */
WorkmanAttribute *workman_object_get_attribute(WorkmanObject *obj,
                                               WorkmanState state,
                                               const gchar *name,
                                               GError **error)
{
    WorkmanObjectClass *klass = WORKMAN_OBJECT_GET_CLASS(obj);
    return klass->get_attribute(obj, state, name, error);
}


gboolean workman_object_refresh_attributes(WorkmanObject *obj,
                                           WorkmanState state,
                                           GError **error)
{
    WorkmanObjectClass *klass = WORKMAN_OBJECT_GET_CLASS(obj);
    return klass->refresh_attributes(obj, state, error);
}


gboolean workman_object_save_attributes(WorkmanObject *obj,
                                        WorkmanState state,
                                        GError **error)
{
    WorkmanObjectClass *klass = WORKMAN_OBJECT_GET_CLASS(obj);
    return klass->save_attributes(obj, state, error);
}


WorkmanState workman_object_get_state(WorkmanObject *self,
                                      GError **error)
{
    WorkmanObjectClass *obj_class = WORKMAN_OBJECT_GET_CLASS(self);
    return obj_class->get_state(self, error);
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
