/*
 * workman-consumer.h: workload manager
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

#ifndef __WORKMAN_CONSUMER_H__
# define __WORKMAN_CONSUMER_H__

# include <glib-object.h>

G_BEGIN_DECLS

# define WORKMAN_TYPE_CONSUMER            (workman_consumer_get_type ())
# define WORKMAN_CONSUMER(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), WORKMAN_TYPE_CONSUMER, WorkmanConsumer))
# define WORKMAN_IS_CONSUMER(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), WORKMAN_TYPE_CONSUMER))
# define WORKMAN_CONSUMER_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), WORKMAN_TYPE_CONSUMER, WorkmanConsumerClass))
# define WORKMAN_IS_CONSUMER_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), WORKMAN_TYPE_CONSUMER))
# define WORKMAN_CONSUMER_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), WORKMAN_TYPE_CONSUMER, WorkmanConsumerClass))

typedef struct _WorkmanConsumer        WorkmanConsumer;
typedef struct _WorkmanConsumerClass   WorkmanConsumerClass;
typedef struct _WorkmanConsumerPrivate WorkmanConsumerPrivate;


struct _WorkmanConsumer
{
    WorkmanObject parent_instance;

    /* public */

    /* private */
    WorkmanConsumerPrivate *priv;
};

/* class */
struct _WorkmanConsumerClass
{
    /*< private >*/
    WorkmanObjectClass parent_class;


    /* Remove from padding when adding new virtual functions */
    gpointer padding[20];
};


GType workman_consumer_get_type(void);

gboolean workman_consumer_set_partition(WorkmanConsumer *consumer,
                                        WorkmanState state,
                                        WorkmanPartition *partition,
                                        GError **error);

WorkmanPartition *workman_consumer_get_partition(WorkmanConsumer *consumer,
                                                 WorkmanState state,
                                                 GError **error);

GList *workman_consumer_get_processes(WorkmanConsumer *consumer,
                                      GError **error);

gboolean workman_consumer_refresh_processes(WorkmanConsumer *consumer,
                                            GError **error);

G_END_DECLS

#endif /* __WORKMAN_CONSUMER_H__ */

/*
 * Local variables:
 *  indent-tabs-mode: nil
 *  c-indent-level: 4
 *  c-basic-offset: 4
 * End:
 *
 * vim: set expandtab shiftwidth=4 tabstop=4 :
 */
