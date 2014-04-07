INCLUDES =						\
	-I$(top_srcdir)					\
	-I$(top_builddir)

nautilus_extensiondir=$(NAUTILUS_EXTENSION_DIR)

nautilus_extension_LTLIBRARIES=libnautilus-skype.la

libnautilus_skype_la_CFLAGS = 	                \
	-DDATADIR=\"$(datadir)\"					    \
	-DEMBLEMDIR=\"$(EMBLEM_DIR)\"					\
	-Wall                                           \
	$(WARN_CFLAGS)                                  \
	$(DISABLE_DEPRECATED_CFLAGS)					\
	$(NAUTILUS_CFLAGS)                              \
	$(GLIB_CFLAGS)

if DEBUG
libnautilus_skype_la_CFLAGS += -DND_DEBUG
else
libnautilus_skype_la_CFLAGS += -DG_DISABLE_ASSERT -DG_DISABLE_CHECKS
endif

libnautilus_skype_la_SOURCES = \
	nautilus-skype.c       \
	nautilus-skype.h       \
	nautilus-skype-hooks.h \
	nautilus-skype-hooks.c \
	skype-command-client.h \
	skype-command-client.c \
	skype-client.c skype-client.h \
	g-util.h \
	async-io-coroutine.h \
	skype-client-util.c \
	skype-client-util.h \
	skype.c

libnautilus_skype_la_LDFLAGS = -module -avoid-version
libnautilus_skype_la_LIBADD  = $(NAUTILUS_LIBS) $(GLIB_LIBS)
