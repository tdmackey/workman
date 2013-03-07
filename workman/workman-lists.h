/*
 * workman-lists.h: workload manager
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

#if !defined(__WORKMAN_H__) && !defined(WORKMAN_BUILD)
# error "Only <workman/workman.h> can be included directly."
#endif

#ifndef __WORKMAN_LISTS_H__
# define __WORKMAN_LISTS_H__

G_BEGIN_DECLS

# define WORKMAN_TYPE_ATTRIBUTE_LIST (workman_attribute_list_get_type())
# define WORKMAN_TYPE_PARTITION_LIST (workman_partition_list_get_type())
# define WORKMAN_TYPE_CONSUMER_LIST (workman_consumer_list_get_type())

GType workman_attribute_list_get_type(void) G_GNUC_CONST;
GType workman_partition_list_get_type(void) G_GNUC_CONST;
GType workman_consumer_list_get_type(void) G_GNUC_CONST;


G_END_DECLS

#endif /* __WORKMAN_LISTS_H__ */

/*
 * Local variables:
 *  indent-tabs-mode: nil
 *  c-indent-level: 4
 *  c-basic-offset: 4
 * End:
 *
 * vim: set expandtab shiftwidth=4 tabstop=4 :
 */
