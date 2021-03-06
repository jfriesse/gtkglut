SRC_DIR = src
OBJS_DIR = objs
SOURCES = gtkglut_init.c gtkglut_internal.c gtkglut_get.c \
          gtkglut_callback_reg.c gtkglut_window.c gtkglut_begin.c \
          gtkglut_unimplemented.c gtkglut_object_rendering.c gtkglut_debug.c \
          gtkglut_game_mode.c gtkglut_menu.c gtkglut_refset.c \
          gtkglut_font_data.c gtkglut_stroke_mono_roman.c gtkglut_stroke_roman.c \
          gtkglut_font.c gtkglut_ext.c
OBJS_SUBST= $(SOURCES:.c=.o)
OBJS=$(OBJS_SUBST:%=$(OBJS_DIR)/%)

ifeq ($(OS), Windows_NT)
WIN_GTK ?= C:/GTK
CFLAGS_INTERNAL  = -mms-bitfields -I$(WIN_GTK)/include/gtkglext-1.0 -I$(WIN_GTK)/lib/gtkglext-1.0/include -I$(WIN_GTK)/include/gtk-2.0 -I$(WIN_GTK)/lib/gtk-2.0/include -I$(WIN_GTK)/include/pango-1.0 -I$(WIN_GTK)/include/glib-2.0 -I$(WIN_GTK)/lib/glib-2.0/include -I$(WIN_GTK)/include/cairo -I$(WIN_GTK)/include/freetype2 -I$(WIN_GTK)/include -IC:/GTK/include/atk-1.0 -Iinclude -Wall -ansi $(CFLAGS)
LDADD   = -user32 -Wl,-luuid -L$(WIN_GTK)/lib -lgtkglext-win32-1.0 -lgdkglext-win32-1.0 -lglu32 -lopengl32 -luser32 -lkernel32 -lgtk-win32-2.0 -lgdk-win32-2.0 -lgdi32 -limm32  -lshell32 -lole32 -latk-1.0 -lgdk_pixbuf-2.0 -lpangowin32-1.0 -lpangocairo-1.0 -lcairo -lpangoft2-1.0 -lfontconfig -lfreetype -lz -lpango-1.0 -lm -lgobject-2.0 -lgmodule-2.0 -lglib-2.0 -lintl -liconv
else
CFLAGS_INTERNAL  = `pkg-config gtkglext-1.0 --cflags` -Iinclude/ -Wall -ansi -fPIC $(CFLAGS)
LDADD   = `pkg-config gtkglext-1.0 --libs`
endif

CC      = gcc
INSTALL = install
VER_MAJOR = 0
VER_MINOR = 3
VER_MICRO = 7
PKG_NAME = libgtkglut
LIB_STATIC = $(PKG_NAME).a

ifeq ($(OS), Windows_NT)
LIB_DYNAMIC = $(PKG_NAME)-$(VER_MAJOR).$(VER_MINOR).$(VER_MICRO).dll
else
LIB_DYNAMIC = $(PKG_NAME).so.$(VER_MAJOR).$(VER_MINOR).$(VER_MICRO)
endif

PREFIX ?= /usr/local
DESTDIR = $(PREFIX)

default:$(LIB_STATIC) $(LIB_DYNAMIC) gtkglut-$(VER_MAJOR).$(VER_MINOR).pc

all:default progs.d

docs:
	$(MAKE) -C doc

ifeq ($(OS), Windows_NT)
gtkglut-$(VER_MAJOR).$(VER_MINOR).pc:
else
gtkglut-$(VER_MAJOR).$(VER_MINOR).pc:gtkglut-$(VER_MAJOR).$(VER_MINOR).m4
	m4 -D PREFIX=$(PREFIX) -D VMAJOR=$(VER_MAJOR) -D VMINOR=$(VER_MINOR) -D VMICRO=$(VER_MICRO) gtkglut-$(VER_MAJOR).$(VER_MINOR).m4 > gtkglut-$(VER_MAJOR).$(VER_MINOR).pc
endif

$(LIB_STATIC):$(OBJS) $(SRC_DIR)/gtkglut_internal.h
	ar rcs $(LIB_STATIC) $(OBJS)

ifeq ($(OS), Windows_NT)
$(LIB_DYNAMIC):$(OBJS) $(SRC_DIR)/gtkglut_internal.h
	$(CC) --no-undefined --enable-runtime-pseudo-reloc -shared -Wl,--out-implib,$(LIB_DYNAMIC).a -o $(LIB_DYNAMIC) $(OBJS) $(LDADD)
else
$(LIB_DYNAMIC):$(OBJS) $(SRC_DIR)/gtkglut_internal.h
	$(CC) -shared -Wl,-soname,$(PKG_NAME).so.$(VER_MAJOR).$(VER_MINOR) -o $(LIB_DYNAMIC) $(OBJS) $(LDADD) -lc
	ln -s $(LIB_DYNAMIC) $(PKG_NAME).so.$(VER_MAJOR).$(VER_MINOR)
	ln -s $(LIB_DYNAMIC) $(PKG_NAME).so
endif

$(OBJS_DIR)/%.o:$(SRC_DIR)/%.c
	$(CC) $(CFLAGS_INTERNAL) -c $< -o $@

progs.d:default
	$(MAKE) -C progs

clean:
	rm -f $(OBJS) $(LIB_STATIC) $(LIB_DYNAMIC) $(PKG_NAME).so.$(VER_MAJOR).$(VER_MINOR) $(PKG_NAME).so gtkglut-$(VER_MAJOR).$(VER_MINOR).pc
	$(MAKE) -C progs clean
	$(MAKE) -C doc clean

ifeq ($(OS), Windows_NT)
.nsis:default
	"c:\program files\nsis\makensis" win32\gtkglut.nsi
endif

install: default
	$(INSTALL) -d $(DESTDIR)/lib/
	$(INSTALL) -m 755 $(LIB_STATIC) $(DESTDIR)/lib/
	$(INSTALL) -m 755 $(LIB_DYNAMIC) $(DESTDIR)/lib/
	ln -sf $(LIB_DYNAMIC) $(DESTDIR)/lib/$(PKG_NAME).so.$(VER_MAJOR).$(VER_MINOR)
	ln -sf $(LIB_DYNAMIC) $(DESTDIR)/lib/$(PKG_NAME).so
	$(INSTALL) -d $(DESTDIR)/lib/pkgconfig
	$(INSTALL) -m 644 gtkglut-$(VER_MAJOR).$(VER_MINOR).pc $(DESTDIR)/lib/pkgconfig
	$(INSTALL) -d $(DESTDIR)/include/gtkglut-$(VER_MAJOR).$(VER_MINOR)/GL/
	$(INSTALL) -m 644 include/GL/*.h $(DESTDIR)/include/gtkglut-$(VER_MAJOR).$(VER_MINOR)/GL/
	$(INSTALL) -d $(DESTDIR)/share/doc/gtkglut-$(VER_MAJOR).$(VER_MINOR).$(VER_MICRO)
	$(INSTALL) -m 644 COPYING $(DESTDIR)/share/doc/gtkglut-$(VER_MAJOR).$(VER_MINOR).$(VER_MICRO)

install-doc:docs
	$(INSTALL) -d $(DESTDIR)/share/doc/gtkglut-$(VER_MAJOR).$(VER_MINOR).$(VER_MICRO)/html/
	$(INSTALL) -m 644 doc/html/* $(DESTDIR)/share/doc/gtkglut-$(VER_MAJOR).$(VER_MINOR).$(VER_MICRO)/html/

install-compat:
	$(INSTALL) -d $(DESTDIR)/X11/lib/
	ln -sf ../../lib/$(LIB_DYNAMIC) $(DESTDIR)/X11/lib/libglut.so.3.7
	ln -sf ../../lib/$(LIB_DYNAMIC) $(DESTDIR)/X11/lib/libglut.so.3
	ln -sf ../../lib/$(LIB_DYNAMIC) $(DESTDIR)/X11/lib/libglut.so
	$(INSTALL) -d $(DESTDIR)/X11/include/GL/
	ln -sf ../../../include/gtkglut-$(VER_MAJOR).$(VER_MINOR)/GL/glut.h $(DESTDIR)/X11/include/GL/glut.h
	ln -sf ../../../include/gtkglut-$(VER_MAJOR).$(VER_MINOR)/GL/gtkglut_std.h $(DESTDIR)/X11/include/GL/gtkglut_std.h
	ln -sf ../../../include/gtkglut-$(VER_MAJOR).$(VER_MINOR)/GL/gtkglut_ext.h $(DESTDIR)/X11/include/GL/gtkglut_ext.h
	ln -sf ../../../include/gtkglut-$(VER_MAJOR).$(VER_MINOR)/GL/gtkglut.h $(DESTDIR)/X11/include/GL/gtkglut.h

uninstall:
	rm -f $(DESTDIR)/lib/$(LIB_STATIC)
	rm -f $(DESTDIR)/lib/$(LIB_DYNAMIC)
	rm -f $(DESTDIR)/lib/$(PKG_NAME).so.$(VER_MAJOR).$(VER_MINOR)
	rm -f $(DESTDIR)/lib/$(PKG_NAME).so
	rm -f $(DESTDIR)/lib/pkgconfig/gtkglut-$(VER_MAJOR).$(VER_MINOR).pc
	rm -rf $(DESTDIR)/include/gtkglut-$(VER_MAJOR).$(VER_MINOR)/
	rm -rf $(DESTDIR)/share/doc/gtkglut-$(VER_MAJOR).$(VER_MINOR).$(VER_MICRO)

uninstall-compat:
	rm -f $(DESTDIR)/X11/lib/libglut.so.3.7
	rm -f $(DESTDIR)/X11/lib/libglut.so.3
	rm -f $(DESTDIR)/X11/lib/libglut.so
	rm -f $(DESTDIR)/X11/include/GL/glut.h
	rm -f $(DESTDIR)/X11/include/GL/gtkglut_std.h
	rm -f $(DESTDIR)/X11/include/GL/gtkglut_ext.h
	rm -f $(DESTDIR)/X11/include/GL/gtkglut.h

srcpkg:clean
	svn export . gtkglut-$(VER_MAJOR).$(VER_MINOR).$(VER_MICRO)
	tar -cjf gtkglut-$(VER_MAJOR).$(VER_MINOR).$(VER_MICRO).tar.bz2 gtkglut-$(VER_MAJOR).$(VER_MINOR).$(VER_MICRO)
	rm -rf gtkglut-$(VER_MAJOR).$(VER_MINOR).$(VER_MICRO)
# end of file
