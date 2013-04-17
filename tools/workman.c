/*
 * workman.c: cmdline interface to libworkman
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

#include <stdio.h>
#include <unistd.h>
#include <getopt.h>
#include <stdlib.h>
#include <errno.h>
#include <glib.h>
#include <config.h>
#include "workman/workman.h"

struct workman_args {
    WorkmanState state;
    int cmd_argc;
    char **cmd_argv;
};

struct workman_cmd {
    const char *name;
    const char *arg_description;
    int min_args;
    int max_args;
    void (*handler)(WorkmanManager *, struct workman_args *);
};


#define WORKMAN_ERROR_EXIT(err, fmt, ...)                                   \
({                                                                          \
    fprintf(stderr, fmt, ##__VA_ARGS__);                                    \
    if (err)                                                                \
        fprintf(stderr, ": %s", err->message);                              \
    fprintf(stderr, "\n");                                                  \
    exit(1);                                                                \
})

#define WORKMAN_USAGE_EXIT(...)                                             \
({                                                                          \
    GError *err = NULL;                                                     \
    WORKMAN_ERROR_EXIT(err, __VA_ARGS__);                                   \
})

static void
list_partitions(WorkmanManager *mgr,
                struct workman_args *args)
{
    GList *partitions, *p;
    GError *err = NULL;
    char *parent_name = NULL;

    if (args->cmd_argc == 1)
        parent_name = args->cmd_argv[0];
    else if (args->cmd_argc != 0)
        WORKMAN_USAGE_EXIT("unknown arguments");

    partitions = workman_manager_get_partitions(mgr, args->state, &err);

    if (partitions) {
        g_warn_if_fail(err == NULL);
        for (p = partitions; p; p = p->next) {
            WorkmanPartition *part = WORKMAN_PARTITION(p->data);

            if (parent_name) {
                WorkmanPartition *parent;
                parent = workman_partition_get_parent(part, args->state, &err);
                if (err)
                    WORKMAN_ERROR_EXIT(err,
                                       "workman_partition_get_parent failed");
                if (!parent ||
                    g_strcmp0(parent_name,
                              workman_object_get_name(WORKMAN_OBJECT(parent))))
                    continue;
            }

            printf("%s\n", workman_object_get_name(WORKMAN_OBJECT(part)));
        }

        g_boxed_free(WORKMAN_TYPE_PARTITION_LIST, partitions);
    }
}

static void list_consumers(WorkmanManager *mgr, struct workman_args *args) {}
static void list_attributes(WorkmanManager *mgr, struct workman_args *args) {}
static void list_processes(WorkmanManager *mgr, struct workman_args *args) {}
static void show_partition(WorkmanManager *mgr, struct workman_args *args) {}
static void show_attribute(WorkmanManager *mgr, struct workman_args *args) {}
static void set_partition(WorkmanManager *mgr, struct workman_args *args) {}
static void set_attribute(WorkmanManager *mgr, struct workman_args *args) {}
static void create_partition(WorkmanManager *mgr, struct workman_args *args) {}

static const struct workman_cmd workman_cmds[] = {
    {"partitions",          "[PARTITION]",              0, 0, list_partitions},
    {"consumers",           "[PARTITION]",              0, 1, list_consumers},
    {"attributes",          "OBJECT",                   1, 1, list_attributes},
    {"processes",           "CONSUMER",                 1, 1, list_processes},
    {"partition",           "PARTITION|CONSUMER",       1, 1, show_partition},
    {"attribute",           "OBJECT ATTRIBUTE",         2, 2, show_attribute},
    {"set-partition",       "PARTITION|CONSUMER",       2, 2, set_partition},
    {"set-attribute",       "OBJECT ATTRIBUTE VALUE",   3, 3, set_attribute},
    {"create-partition",    "NAME",                     1, 1, create_partition},
    {},
};


int
main(int argc, char **argv)
{
    const struct workman_cmd *cmd;
    struct workman_args args;
    static gchar *state = NULL;
    static gchar **cmd_args = NULL;
    GOptionContext *context;
    GError *error = NULL;
    static GString *description;

    static GOptionEntry options[] = {
        {"state", 's', 0, G_OPTION_ARG_STRING, &state, "active, persistent, or all (default=active)\n", NULL},
        {G_OPTION_REMAINING, 0, 0, G_OPTION_ARG_STRING_ARRAY, &cmd_args, NULL, "COMMAND [ARGS...]"},
        {},
    };

    g_type_init();

    context = g_option_context_new(" - workload manager");

    g_option_context_add_main_entries(context, options, NULL);;

    description = g_string_new("Commands:\n");
    for (cmd = workman_cmds; cmd->name; cmd++)
        g_string_append_printf(description,
                               "  %s %s\n",
                               cmd->name,
                               cmd->arg_description);
    g_option_context_set_description(context, description->str);;
    g_string_free(description, TRUE);

    if (!g_option_context_parse(context, &argc, &argv, &error)) {
        g_print("option parsing failed: %s\n", error->message);
        exit(1);
    }

    if (!state || !g_strcmp0(state, "active"))
        args.state = WORKMAN_STATE_ACTIVE;
    else if (!g_strcmp0(state, "persistent"))
        args.state = WORKMAN_STATE_PERSISTENT;
    else if (!g_strcmp0(state, "all"))
        args.state = WORKMAN_STATE_ALL;
    else
        WORKMAN_USAGE_EXIT("invalid state");
    g_free(state);

    if (!cmd_args || !cmd_args[0])
        WORKMAN_USAGE_EXIT("missing command");

    for (cmd = workman_cmds; ; cmd++) {

        if (!cmd->name)
            WORKMAN_USAGE_EXIT("unknown command '%s'", cmd_args[0]);

        if (!g_strcmp0(cmd_args[0], cmd->name)) {

            WorkmanManager *mgr;
            GError *err = NULL;

            args.cmd_argc = 0;
            while (cmd_args[args.cmd_argc + 1])
                args.cmd_argc++;

            args.cmd_argv = &cmd_args[1];

            if (args.cmd_argc < cmd->min_args || args.cmd_argc > cmd->max_args)
                WORKMAN_USAGE_EXIT("invalid number of arguments for '%s'",
                                   cmd->name);

            workman_init(NULL, NULL);

            mgr = workman_manager_get_default(&err);
            if (!mgr)
                WORKMAN_ERROR_EXIT(err, "workman_manager_get_default failed");

            cmd->handler(mgr, &args);
            break;
        }
    }

    g_option_context_free(context);

    return 0;
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
