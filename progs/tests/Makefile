SOURCES = bigtest.c cursor_test.c joy_test.c keyup_test.c menu_test.c \
          over_test.c shape_test.c test1.c test10.c test11.c test12.c test13.c \
          test14.c test15.c test16.c test17.c test18.c test19.c test2.c test20.c \
          test21.c test22.c test23.c test24.c test25.c test26.c test27.c test28.c \
          test3.c test4.c test5.c test6.c test7.c test8.c test9.c timer_test.c 
ifeq ($(OS), Windows_NT)
OBJS= $(SOURCES:.c=.exe)
else
OBJS= $(SOURCES:.c=)
endif

ifeq ($(OS), Windows_NT)
CFLAGS_INTERNAL  = -mms-bitfields -IC:/GTK/include/gtkglext-1.0 -IC:/GTK/lib/gtkglext-1.0/include -IC:/GTK/include/gtk-2.0 -IC:/GTK/lib/gtk-2.0/include -IC:/GTK/include/pango-1.0 -IC:/GTK/include/glib-2.0 -IC:/GTK/lib/glib-2.0/include -IC:/GTK/include/cairo -IC:/GTK/include/freetype2 -IC:/GTK/include -IC:/GTK/include/atk-1.0 -I../../include -O2 -Wall -ansi $(CFLAGS)
LDADD   = -L../../ -Wl,-Bstatic -lgtkglut -Wl,-Bdynamic -user32 -Wl,-luuid -LC:/GTK/lib -lgtkglext-win32-1.0 -lgdkglext-win32-1.0 -lglu32 -lopengl32 -luser32 -lkernel32 -lgtk-win32-2.0 -lgdk-win32-2.0 -lgdi32 -limm32  -lshell32 -lole32 -latk-1.0 -lgdk_pixbuf-2.0 -lpangowin32-1.0 -lpangocairo-1.0 -lcairo -lpangoft2-1.0 -lfontconfig -lfreetype -lz -lpango-1.0 -lm -lgobject-2.0 -lgmodule-2.0 -lglib-2.0 -lintl -liconv
else
CFLAGS_INTERNAL  = -I../../include `pkg-config gtkglext-1.0 --cflags`  -O2 -Wall -ansi -fPIC $(CFLAGS)
LDADD   = `pkg-config gtkglext-1.0 --libs` -L../../ -Wl,-Bstatic -lgtkglut -Wl,-Bdynamic -lm
endif

CC      = gcc

all : $(OBJS)
  
ifeq ($(OS), Windows_NT)
%.exe:%.c
	$(CC) $(CFLAGS_INTERNAL) $< $(LDADD) -o $@
else
%:%.c
	$(CC) $(CFLAGS_INTERNAL) $< $(LDADD) -o $@
endif

clean:
	rm -f $(OBJS)

# end of file
