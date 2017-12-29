prefix=PREFIX
exec_prefix=${prefix}
libdir=${exec_prefix}/lib
includedir=${prefix}/include

api_version=VMAJOR.VMINOR

Name: GtkGLUT
Description: GLUT API implementation over GTK+ and GtkGLExt
Version: VMAJOR.VMINOR.VMICRO
Requires: gtkglext-1.0 gtk+-2.0
Libs: -L${libdir} -lgtkglut
Cflags: -I${includedir}/gtkglut-${api_version} 
