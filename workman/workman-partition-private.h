/*
 * workman-partition-private.h: workload manager
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
 * Author: Josh Poimboeuf <jpoimboe@redhat.com>
 */

#ifndef __WORKMAN_PARTITION_PRIVATE_H__
# define __WORKMAN_PARTITION_PRIVATE_H__

# include "workman-partition.h"
# include "workman-consumer.h"

G_BEGIN_DECLS


gboolean workman_partition_add_consumer_priv(WorkmanPartition *partition,
                                             WorkmanState state,
                                             WorkmanConsumer *consumer);

gboolean workman_partition_remove_consumer_priv(WorkmanPartition *partition,
                                                WorkmanState state,
                                                WorkmanConsumer *consumer);

gboolean workman_partition_add_child_priv(WorkmanPartition *partition,
                                          WorkmanState state,
                                          WorkmanPartition *child);

gboolean workman_partition_remove_child_priv(WorkmanPartition *partition,
                                             WorkmanState state,
                                             WorkmanPartition *child);

WorkmanPartition *workman_partition_new(const gchar *name,
                                        WorkmanState state,
                                        GList *attributes,
                                        WorkmanPartition *parent_active,
                                        WorkmanPartition *parent_persistent,
                                        GList *children_active,
                                        GList *children_persistent,
                                        GList *consumers_active,
                                        GList *consumers_persistent);

G_END_DECLS

#endif /* __WORKMAN_PARTITION_PRIVATE_H__ */

/*
 * Local variables:
 *  indent-tabs-mode: nil
 *  c-indent-level: 4
 *  c-basic-offset: 4
 * End:
 *
 * vim: set expandtab shiftwidth=4 tabstop=4 :
 */
