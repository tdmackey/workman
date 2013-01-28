/*
 * workman-partition.h: workload manager
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

#ifndef __WORKMAN_PARTITION_H__
# define __WORKMAN_PARTITION_H__

# include <glib-object.h>

G_BEGIN_DECLS

# define WORKMAN_TYPE_PARTITION            (workman_partition_get_type ())
# define WORKMAN_PARTITION(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), WORKMAN_TYPE_PARTITION, WorkmanPartition))
# define WORKMAN_IS_PARTITION(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), WORKMAN_TYPE_PARTITION))
# define WORKMAN_PARTITION_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), WORKMAN_TYPE_PARTITION, WorkmanPartitionClass))
# define WORKMAN_IS_PARTITION_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), WORKMAN_TYPE_PARTITION))
# define WORKMAN_PARTITION_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), WORKMAN_TYPE_PARTITION, WorkmanPartitionClass))

typedef struct _WorkmanPartition        WorkmanPartition;
typedef struct _WorkmanPartitionClass   WorkmanPartitionClass;
typedef struct _WorkmanPartitionPrivate WorkmanPartitionPrivate;


struct _WorkmanPartition
{
    WorkmanObject parent_instance;

    /* public */

    /* private */
    WorkmanPartitionPrivate *priv;
};

/* class */
struct _WorkmanPartitionClass
{
    /*< private >*/
    WorkmanObjectClass parent_class;

    GList *(*get_consumers)(WorkmanPartition *partition,
                            GError **error);
    GList *(*get_subdivisions)(WorkmanPartition *partition,
                               GError **error);

    /* class members */
    gpointer padding[20];
};


GType workman_partition_get_type(void);

GList *workman_partition_get_consumers(WorkmanPartition *partition,
                                       GError **error);

GList *workman_partition_get_subdivisions(WorkmanPartition *partition,
                                          GError **error);

G_END_DECLS

#endif /* __WORKMAN_PARTITION_H__ */

/*
 * Local variables:
 *  indent-tabs-mode: nil
 *  c-indent-level: 4
 *  c-basic-offset: 4
 * End:
 */
