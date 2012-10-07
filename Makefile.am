AUTOMAKE_OPTIONS	= foreign subdir-objects

AM_CPPFLAGS		= -DDATA_DIR=\"$(datadir)\"
AM_CFLAGS		= $(TOMCRYPT_CFLAGS)

dist_man_MANS		= stoken.1

lib_LTLIBRARIES		= libstoken.la
libstoken_la_SOURCES	= src/library.c src/securid.c
libstoken_la_LDFLAGS	= -version-number @APIMAJOR@:@APIMINOR@
include_HEADERS		= src/stoken.h
noinst_HEADERS		= src/common.h src/securid.h src/stoken-internal.h
pkgconfig_DATA		= stoken.pc

bin_PROGRAMS		= stoken
stoken_SOURCES		= src/cli.c src/common.c
stoken_LDADD		= $(LDADD) libstoken.la $(TOMCRYPT_LIBS)

if ENABLE_GUI
bin_PROGRAMS		+= stoken-gui
stoken_gui_SOURCES	= src/gui.c src/common.c
stoken_gui_CFLAGS	= $(AM_CFLAGS) $(GTK_CFLAGS)
stoken_gui_LDADD	= $(LDADD) libstoken.la $(GTK_LIBS) $(TOMCRYPT_LIBS)

dist_man_MANS		+= stoken-gui.1

icondir			= $(datadir)/pixmaps
dist_icon_DATA		= stoken-gui.png

desktopdir		= $(datadir)/applications
dist_desktop_DATA	= stoken-gui.desktop

endif

dist_noinst_SCRIPTS	= autogen.sh

EXTRA_DIST		= .gitignore
DISTCLEANFILES		= *~