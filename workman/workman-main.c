/*
 * workman-main.c: workload manager
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

#include <stdlib.h>
#include <stdio.h>

#include "workman-main.h"

/**
 * SECTION:workman-main
 * @short_description: Initialize the library
 * @title: Library initialization
 * @stability: Stable
 * @include: workman/workman.h
 *
 * The workman library provides an API for workload management
 *
 * Before using any functions in the workman library, it must be initialized
 * by calling <code>workman_init</code> or <code>workman_init_check</code>.
 *
 * <example>
 * <title>Initializing the workman library</title>
 * <programlisting><![CDATA[
 * int main(int argc, char **argv) {
 *   ...setup...
 *   workman_init(&argc, &argv);
 *   ...more setup...
 *   gtk_main();
 *   return 0;
 * }
 * ]]></programlisting>
 * </example>
 *
 */


/**
 * workman_init:
 * @argc: (inout): Address of the argc parameter of your main() function (or 0
 *     if argv is NULL). This will be changed if any arguments were handled.
 * @argv: (array length=argc) (inout) (allow-none): Address of the
 *     <parameter>argv</parameter> parameter of main(), or %NULL. Any options
 *     understood by GTK+ are stripped before return.
 *
 * Call this function before using any other workman functions in your applications.
 * It will initialize everything needed to operate the toolkit and parses some standard
 * command line options.
 *
 * Although you are expected to pass the @argc, @argv parameters from main() to this
 * function, it is possible to pass NULL if @argv is not available or commandline
 * handling is not required.
 *
 * @argc and @argv are adjusted accordingly so your own code will never see those
 * standard arguments.
 *
 * This method will also turn on debug logging of the library if the
 * <literal>workman_DEBUG</literal> environment variable is set.
 *
 * This function will terminate your program if it was unable to initialize
 * for some reason. If you want the program to fall back to an alternate
 * mode of operation call <code>workman_init_check</code> instead.
 */
void workman_init(int *argc,
                  char ***argv)
{
    GError *err = NULL;
    if (!workman_init_check(argc, argv, &err)) {
        g_error("Could not initialize libworkman: %s\n",
                err->message);
    }
}


static void workman_log_handler(const gchar *log_domain G_GNUC_UNUSED,
                                GLogLevelFlags log_level G_GNUC_UNUSED,
                                const gchar *message,
                                gpointer user_data)
{
    if (user_data)
        fprintf(stderr, "%s\n", message);
}


/**
 * workman_init_check:
 * @argc: (inout): Address of the argc parameter of your main() function (or 0
 *     if argv is NULL). This will be changed if any arguments were handled.
 * @argv: (array length=argc) (inout) (allow-none): Address of the
 *     <parameter>argv</parameter> parameter of main(), or %NULL. Any options
 *     understood by GTK+ are stripped before return.
 * @err: filled with the error information if initialized failed.
 *
 * This function does the same workman as workman_init() with only a single
 * change: It does not terminate the program if the workman library
 * can't be initialized. Instead it returns %FALSE on failure.
 *
 * This way the application can fall back to some other mode of
 * operation.
 *
 * Return value: %TRUE if the library was successfully initialized,
 *     %FALSE otherwise
 */
gboolean workman_init_check(int *argc G_GNUC_UNUSED,
                            char ***argv G_GNUC_UNUSED,
                            GError **err G_GNUC_UNUSED)
{
    /* Threading is always enabled from 2.31.0 onwards */
#if !GLIB_CHECK_VERSION(2, 31, 0)
    if (!g_thread_supported())
        g_thread_init(NULL);
#endif

    /* GLib >= 2.31.0 debug is off by default, so we need to
     * enable it. Older versions are on by default, so we need
     * to disable it.
     */
#if GLIB_CHECK_VERSION(2, 31, 0)
    if (getenv("WORKMAN_DEBUG"))
        g_log_set_handler(G_LOG_DOMAIN, G_LOG_LEVEL_DEBUG,
                          workman_log_handler, (void*)0x1);
#else
    if (!getenv("WORKMAN_DEBUG"))
        g_log_set_handler(G_LOG_DOMAIN, G_LOG_LEVEL_DEBUG,
                          workman_log_handler, NULL);
#endif

    return TRUE;
}

/*
 * Local variables:
 *  indent-tabs-mode: nil
 *  c-indent-level: 4
 *  c-basic-offset: 4
 * End:
 */
