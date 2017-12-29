SOURCES = aaindex.c aapoly.c aargb.c accanti.c accpersp.c alpha.c alpha3D.c anti.c \
          bezcurve.c bezmesh.c clip.c colormat.c cube.c depthcue.c dof.c double.c drawf.c \
          feedback.c fog.c fogindex.c font.c hello.c checker.c image.c light.c lines.c list.c \
          material.c mipmap.c model.c movelight.c nurbs.c pickdepth.c picksquare.c plane.c planet.c \
          polyoff.c polys.c quadric.c robot.c sccolorlight.c scenebamb.c scene.c sceneflat.c select.c \
          smooth.c stencil.c stroke.c surface.c teaambient.c teapots.c tess.c tesswind.c texbind.c texgen.c \
          texprox.c texsub.c texturesurf.c torus.c trim.c unproject.c varray.c wrap.c
ifeq ($(OS), Windows_NT)
OBJS= $(SOURCES:.c=.exe)
else
OBJS= $(SOURCES:.c=)
endif

ifeq ($(OS), Windows_NT)
CFLAGS_INTERNAL  = -mms-bitfields -IC:/GTK/include/gtkglext-1.0 -IC:/GTK/lib/gtkglext-1.0/include -IC:/GTK/include/gtk-2.0 -IC:/GTK/lib/gtk-2.0/include -IC:/GTK/include/pango-1.0 -IC:/GTK/include/glib-2.0 -IC:/GTK/lib/glib-2.0/include -IC:/GTK/include/cairo -IC:/GTK/include/freetype2 -IC:/GTK/include -IC:/GTK/include/atk-1.0 -I../../include -O2 -Wall -ansi $(CFLAGS)
LDADD   = -L../../ -Wl,-Bstatic -lgtkglut -Wl,-Bdynamic -user32 -Wl,-luuid -LC:/GTK/lib -lgtkglext-win32-1.0 -lgdkglext-win32-1.0 -lglu32 -lopengl32 -luser32 -lkernel32 -lgtk-win32-2.0 -lgdk-win32-2.0 -lgdi32 -limm32  -lshell32 -lole32 -latk-1.0 -lgdk_pixbuf-2.0 -lpangowin32-1.0 -lpangocairo-1.0 -lcairo -lpangoft2-1.0 -lfontconfig -lfreetype -lz -lpango-1.0 -lm -lgobject-2.0 -lgmodule-2.0 -lglib-2.0 -lintl -liconv
else
CFLAGS_INTERNAL  = `pkg-config gtkglext-1.0 --cflags` -I../../include/ -O2 -Wall -ansi -fPIC $(CFLAGS)
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
