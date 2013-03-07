/*
 * workman-lists-c: workload manager
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

#include <config.h>

#include "workman.h"


static GList *
list_copy(GList *list)
{
    g_list_foreach(list, (GFunc)g_object_ref, NULL);
    return g_list_copy(list);
}


static void
list_free(GList *list)
{
    g_list_foreach(list, (GFunc)g_object_unref, NULL);
    g_list_free(list);
}


typedef GList WorkmanAttributeList;
G_DEFINE_BOXED_TYPE(WorkmanAttributeList,
                    workman_attribute_list,
                    list_copy,
                    list_free);

typedef GList WorkmanPartitionList;
G_DEFINE_BOXED_TYPE(WorkmanPartitionList,
                    workman_partition_list,
                    list_copy,
                    list_free);

typedef GList WorkmanConsumerList;
G_DEFINE_BOXED_TYPE(WorkmanConsumerList,
                    workman_consumer_list,
                    list_copy,
                    list_free);

/*
 * Local variables:
 *  indent-tabs-mode: nil
 *  c-indent-level: 4
 *  c-basic-offset: 4
 * End:
 *
 * vim: set expandtab shiftwidth=4 tabstop=4 :
 */
