AC_INIT(workman, 0.1.6)
AC_CONFIG_SRCDIR(workman/workman-main.c)
AC_CONFIG_AUX_DIR([build-aux])
AM_CONFIG_HEADER(config.h)
AC_CONFIG_MACRO_DIR([m4])
dnl Make automake keep quiet about wildcards & other GNUmake-isms
AM_INIT_AUTOMAKE([-Wno-portability])
AC_CANONICAL_HOST

AM_SILENT_RULES([yes])

GLIB2_REQUIRED=2.22.0
GOBJECT2_REQUIRED=2.10.0
GIO_REQUIRED=2.10.0
GOBJECT_INTROSPECTION_REQUIRED=0.10.8

WORKMAN_MAJOR_VERSION=`echo $VERSION | awk -F. '{print $1}'`
WORKMAN_MINOR_VERSION=`echo $VERSION | awk -F. '{print $2}'`
WORKMAN_MICRO_VERSION=`echo $VERSION | awk -F. '{print $3}'`
WORKMAN_VERSION=$WORKMAN_MAJOR_VERSION.$WORKMAN_MINOR_VERSION.$WORKMAN_MICRO_VERSION$WORKMAN_MICRO_VERSION_SUFFIX
WORKMAN_VERSION_INFO=`expr $WORKMAN_MAJOR_VERSION + $WORKMAN_MINOR_VERSION`:$WORKMAN_MICRO_VERSION:$WORKMAN_MINOR_VERSION
WORKMAN_VERSION_NUMBER=`expr $WORKMAN_MAJOR_VERSION \* 1000000 + $WORKMAN_MINOR_VERSION \* 1000 + $WORKMAN_MICRO_VERSION`

AC_SUBST([WORKMAN_MAJOR_VERSION])
AC_SUBST([WORKMAN_MINOR_VERSION])
AC_SUBST([WORKMAN_MICRO_VERSION])
AC_SUBST([WORKMAN_VERSION])
AC_SUBST([WORKMAN_VERSION_INFO])
AC_SUBST([WORKMAN_VERSION_NUMBER])

AC_PROG_CC
AM_PROG_CC_C_O

AC_LIBTOOL_WIN32_DLL
AC_PROG_LIBTOOL

dnl AC_CONFIG_LIBOBJ_DIR([src])

AC_DEFINE([_GNU_SOURCE], [], [Enable GNU extensions])

# Autoconf 2.61a.99 and earlier don't support linking a file only
# in VPATH builds.  But since GNUmakefile is for maintainer use
# only, it does not matter if we skip the link with older autoconf.
# Automake 1.10.1 and earlier try to remove GNUmakefile in non-VPATH
# builds, so use a shell variable to bypass this.
GNUmakefile=GNUmakefile
m4_if(m4_version_compare([2.61a.100],
        m4_defn([m4_PACKAGE_VERSION])), [1], [],
      [AC_CONFIG_LINKS([$GNUmakefile:$GNUmakefile], [],
        [GNUmakefile=$GNUmakefile])])

WORKMAN_COMPILE_WARNINGS

PKG_CHECK_MODULES(GLIB2, glib-2.0 >= $GLIB2_REQUIRED)
PKG_CHECK_MODULES(GTHREAD2, gthread-2.0 >= $GLIB2_REQUIRED)
PKG_CHECK_MODULES(GOBJECT2, gobject-2.0 >= $GLIB2_REQUIRED)
PKG_CHECK_MODULES(GIO2, gio-2.0 >= $GLIB2_REQUIRED)

GTK_DOC_CHECK([1.10],[--flavour no-tmpl])

# Setup GLIB_MKENUMS to use glib-mkenums even if GLib is uninstalled.
GLIB_MKENUMS=`$PKG_CONFIG --variable=glib_mkenums glib-2.0`
AC_SUBST(GLIB_MKENUMS)

AC_ARG_ENABLE([introspection],
        AS_HELP_STRING([--enable-introspection], [enable GObject introspection]),
        [], [enable_introspection=check])

if test "x$enable_introspection" != "xno" ; then
        PKG_CHECK_MODULES([GOBJECT_INTROSPECTION],
                          [gobject-introspection-1.0 >= $GOBJECT_INTROSPECTION_REQUIRED],
                          [enable_introspection=yes],
                          [
                             if test "x$enable_introspection" = "xcheck"; then
                               enable_introspection=no
                             else
                               AC_MSG_ERROR([gobject-introspection is not available])
                             fi
                          ])
        if test "x$enable_introspection" = "xyes" ; then
          AC_DEFINE([WITH_GOBJECT_INTROSPECTION], [1], [enable GObject introspection support])
          AC_SUBST(GOBJECT_INTROSPECTION_CFLAGS)
          AC_SUBST(GOBJECT_INTROSPECTION_LIBS)
          AC_SUBST([G_IR_SCANNER], [$($PKG_CONFIG --variable=g_ir_scanner gobject-introspection-1.0)])
          AC_SUBST([G_IR_COMPILER], [$($PKG_CONFIG --variable=g_ir_compiler gobject-introspection-1.0)])
        fi
fi
AM_CONDITIONAL([WITH_GOBJECT_INTROSPECTION], [test "x$enable_introspection" = "xyes"])

AC_OUTPUT(Makefile
          workman/Makefile
          tools/Makefile
          docs/Makefile
          docs/workman/Makefile
          docs/workman/version.xml
          workman-1.0.pc
          workman.spec)
