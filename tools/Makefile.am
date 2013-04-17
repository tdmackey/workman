bin_PROGRAMS = workman

workman_SOURCES = workman.c

workman_CFLAGS = \
		 -I$(top_srcdir) \
		 $(GOBJECT2_CFLAGS) \
		 $(WARN_CFLAGS) \
		 $(NULL)

workman_LDADD = \
		../workman/libworkman-1.0.la \
		$(NULL)

