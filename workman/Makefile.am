
EXTRA_DIST = libworkman.sym

lib_LTLIBRARIES = libworkman-1.0.la

libworkman_1_0_ladir = $(includedir)/workman-1.0/workman
libworkman_1_0_la_HEADERS = \
			workman.h \
			workman-main.h
libworkman_1_0_la_SOURCES = \
			$(libworkman_1_0_la_HEADERS) \
			workman-main.c
libworkman_1_0_la_CFLAGS = \
			-DWORKMAN_BUILD \
			-DG_LOG_DOMAIN="\"workman\"" \
			-I$(top_srcdir) \
			$(GLIB2_CFLAGS) \
			$(GTHREAD2_CFLAGS) \
			$(WARN_CFLAGS)
libworkman_1_0_la_LIBADD = \
			$(GLIB2_LIBS) \
			$(GTHREAD2_LIBS)
libworkman_1_0_la_DEPENDENCIES = \
			libworkman.sym
libworkman_1_0_la_LDFLAGS = \
			-Wl,--version-script=$(srcdir)/libworkman.sym \
			-version-info $(WORKMAN_VERSION_INFO)

if WITH_GOBJECT_INTROSPECTION

Workman-1.0.gir: libworkman-1.0.la $(G_IR_SCANNER) Makefile.am
	$(AM_V_GEN)$(G_IR_SCANNER) \
                --quiet \
                --warn-all \
                --namespace Workman \
                --nsversion 1.0 \
                --include GLib-2.0 \
                --identifier-prefix=workman \
                --symbol-prefix=workman \
                --library=$(builddir)/libworkman-1.0.la \
                --output $@ \
                -I$(top_srcdir) \
                --verbose \
                --pkg=gthread-2.0 \
                --c-include="workman/workman.h" \
                --pkg-export=workman-1.0 \
                $(libworkman_1_0_la_SOURCES:%=$(srcdir)/%)

girdir = $(datadir)/gir-1.0
gir_DATA = Workman-1.0.gir

typelibsdir = $(libdir)/girepository-1.0
typelibs_DATA = Workman-1.0.typelib

%.typelib: %.gir
	$(AM_V_GEN)$(G_IR_COMPILER) \
                --includedir=$(builddir) \
                --includedir=$(girdir) \
                -o $@ $<

CLEANFILES = $(gir_DATA) $(typelibs_DATA)

endif # WITH_GOBJECT_INTROSPECTION
