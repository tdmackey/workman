/*
 * workman-consumer-private.h: workload manager
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

#ifndef __WORKMAN_CONSUMER_PRIVATE_H__
# define __WORKMAN_CONSUMER_PRIVATE_H__

# include "workman-partition.h"
# include "workman-consumer.h"

G_BEGIN_DECLS


WorkmanConsumer *workman_consumer_new(const gchar *name,
                                      WorkmanState state,
                                      GList *attributes,
                                      WorkmanPartition *partition_active,
                                      WorkmanPartition *partition_persistent,
                                      GList *processes);

void workman_consumer_set_partition_priv(WorkmanConsumer *consumer,
                                         WorkmanState state,
                                         WorkmanPartition *partition);


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
