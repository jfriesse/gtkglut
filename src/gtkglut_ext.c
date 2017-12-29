/*!
    \file  gtkglut_ext.c
    \brief Extra functions (not found in GLUT and/or FreeGLUT,OpenGLUT))
*/

/* 
 * Extra functions (not found in GLUT and/or FreeGLUT,OpenGLUT)).
 *
 * Copyright (c) 2008 Jan Friesse. All Rights Reserved.
 * Written by Jan Friesse, <jfriesse@gmail.comt>
 * Creation date: Sun Mar 16 2008
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * PAWEL W. OLSZTA BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * Except as contained in this notice, the name of Jan Friesse shall not be 
 * used in advertising or otherwise to promote the sale, use or other dealings in 
 * this Software without prior written authorization from Jan Friesse. 
 *
 * Contains some little modified code from FreeGLUT, see COPYING file
 */

#include "GL/gtkglut.h"
#include "gtkglut_internal.h"

/*Get address of glut* function. Modified code from FreeGLUT.*/
GLUTproc __gtkglut_get_proc_address(const char* proc_name) {
  /* optimization: quick initial check */
  if (strncmp(proc_name, "glut", 4 ) != 0 )
       return NULL;

#define __GTKGLUT_CHECK_NAME(x) if( strcmp( proc_name, #x ) == 0) return (GLUTproc)x;
  __GTKGLUT_CHECK_NAME(glutInit);
  __GTKGLUT_CHECK_NAME(glutInitDisplayMode);
  __GTKGLUT_CHECK_NAME(glutInitDisplayString);
  __GTKGLUT_CHECK_NAME(glutInitWindowPosition);
  __GTKGLUT_CHECK_NAME(glutInitWindowSize);
  __GTKGLUT_CHECK_NAME(glutMainLoop);
  __GTKGLUT_CHECK_NAME(glutCreateWindow);
  __GTKGLUT_CHECK_NAME(glutCreateSubWindow);
  __GTKGLUT_CHECK_NAME(glutDestroyWindow);
  __GTKGLUT_CHECK_NAME(glutPostRedisplay);
  __GTKGLUT_CHECK_NAME(glutPostWindowRedisplay);
  __GTKGLUT_CHECK_NAME(glutSwapBuffers);
  __GTKGLUT_CHECK_NAME(glutGetWindow);
  __GTKGLUT_CHECK_NAME(glutSetWindow);
  __GTKGLUT_CHECK_NAME(glutSetWindowTitle);
  __GTKGLUT_CHECK_NAME(glutSetIconTitle);
  __GTKGLUT_CHECK_NAME(glutPositionWindow);
  __GTKGLUT_CHECK_NAME(glutReshapeWindow);
  __GTKGLUT_CHECK_NAME(glutPopWindow);
  __GTKGLUT_CHECK_NAME(glutPushWindow);
  __GTKGLUT_CHECK_NAME(glutIconifyWindow);
  __GTKGLUT_CHECK_NAME(glutShowWindow);
  __GTKGLUT_CHECK_NAME(glutHideWindow);
  __GTKGLUT_CHECK_NAME(glutFullScreen);
  __GTKGLUT_CHECK_NAME(glutSetCursor);
  __GTKGLUT_CHECK_NAME(glutWarpPointer);
  __GTKGLUT_CHECK_NAME(glutEstablishOverlay);
  __GTKGLUT_CHECK_NAME(glutRemoveOverlay);
  __GTKGLUT_CHECK_NAME(glutUseLayer);
  __GTKGLUT_CHECK_NAME(glutPostOverlayRedisplay);
  __GTKGLUT_CHECK_NAME(glutPostWindowOverlayRedisplay);
  __GTKGLUT_CHECK_NAME(glutShowOverlay);
  __GTKGLUT_CHECK_NAME(glutHideOverlay);
  __GTKGLUT_CHECK_NAME(glutCreateMenu);
  __GTKGLUT_CHECK_NAME(glutDestroyMenu);
  __GTKGLUT_CHECK_NAME(glutGetMenu);
  __GTKGLUT_CHECK_NAME(glutSetMenu);
  __GTKGLUT_CHECK_NAME(glutAddMenuEntry);
  __GTKGLUT_CHECK_NAME(glutAddSubMenu);
  __GTKGLUT_CHECK_NAME(glutChangeToMenuEntry);
  __GTKGLUT_CHECK_NAME(glutChangeToSubMenu);
  __GTKGLUT_CHECK_NAME(glutRemoveMenuItem);
  __GTKGLUT_CHECK_NAME(glutAttachMenu);
  __GTKGLUT_CHECK_NAME(glutDetachMenu);
  __GTKGLUT_CHECK_NAME(glutDisplayFunc);
  __GTKGLUT_CHECK_NAME(glutReshapeFunc);
  __GTKGLUT_CHECK_NAME(glutKeyboardFunc);
  __GTKGLUT_CHECK_NAME(glutMouseFunc);
  __GTKGLUT_CHECK_NAME(glutMotionFunc);
  __GTKGLUT_CHECK_NAME(glutPassiveMotionFunc);
  __GTKGLUT_CHECK_NAME(glutEntryFunc);
  __GTKGLUT_CHECK_NAME(glutVisibilityFunc);
  __GTKGLUT_CHECK_NAME(glutIdleFunc);
  __GTKGLUT_CHECK_NAME(glutTimerFunc);
  __GTKGLUT_CHECK_NAME(glutMenuStateFunc);
  __GTKGLUT_CHECK_NAME(glutSpecialFunc);
  __GTKGLUT_CHECK_NAME(glutSpaceballMotionFunc);
  __GTKGLUT_CHECK_NAME(glutSpaceballRotateFunc);
  __GTKGLUT_CHECK_NAME(glutSpaceballButtonFunc);
  __GTKGLUT_CHECK_NAME(glutButtonBoxFunc);
  __GTKGLUT_CHECK_NAME(glutDialsFunc);
  __GTKGLUT_CHECK_NAME(glutTabletMotionFunc);
  __GTKGLUT_CHECK_NAME(glutTabletButtonFunc);
  __GTKGLUT_CHECK_NAME(glutMenuStatusFunc);
  __GTKGLUT_CHECK_NAME(glutOverlayDisplayFunc);
  __GTKGLUT_CHECK_NAME(glutWindowStatusFunc);
  __GTKGLUT_CHECK_NAME(glutKeyboardUpFunc);
  __GTKGLUT_CHECK_NAME(glutSpecialUpFunc);
  __GTKGLUT_CHECK_NAME(glutJoystickFunc);
  __GTKGLUT_CHECK_NAME(glutSetColor);
  __GTKGLUT_CHECK_NAME(glutGetColor);
  __GTKGLUT_CHECK_NAME(glutCopyColormap);
  __GTKGLUT_CHECK_NAME(glutGet);
  __GTKGLUT_CHECK_NAME(glutDeviceGet);
  __GTKGLUT_CHECK_NAME(glutExtensionSupported);
  __GTKGLUT_CHECK_NAME(glutGetModifiers);
  __GTKGLUT_CHECK_NAME(glutLayerGet);
  __GTKGLUT_CHECK_NAME(glutBitmapCharacter);
  __GTKGLUT_CHECK_NAME(glutBitmapWidth);
  __GTKGLUT_CHECK_NAME(glutStrokeCharacter);
  __GTKGLUT_CHECK_NAME(glutStrokeWidth);
  __GTKGLUT_CHECK_NAME(glutBitmapLength);
  __GTKGLUT_CHECK_NAME(glutStrokeLength);
  __GTKGLUT_CHECK_NAME(glutWireSphere);
  __GTKGLUT_CHECK_NAME(glutSolidSphere);
  __GTKGLUT_CHECK_NAME(glutWireCone);
  __GTKGLUT_CHECK_NAME(glutSolidCone);
  __GTKGLUT_CHECK_NAME(glutWireCube);
  __GTKGLUT_CHECK_NAME(glutSolidCube);
  __GTKGLUT_CHECK_NAME(glutWireTorus);
  __GTKGLUT_CHECK_NAME(glutSolidTorus);
  __GTKGLUT_CHECK_NAME(glutWireDodecahedron);
  __GTKGLUT_CHECK_NAME(glutSolidDodecahedron);
  __GTKGLUT_CHECK_NAME(glutWireTeapot);
  __GTKGLUT_CHECK_NAME(glutSolidTeapot);
  __GTKGLUT_CHECK_NAME(glutWireOctahedron);
  __GTKGLUT_CHECK_NAME(glutSolidOctahedron);
  __GTKGLUT_CHECK_NAME(glutWireTetrahedron);
  __GTKGLUT_CHECK_NAME(glutSolidTetrahedron);
  __GTKGLUT_CHECK_NAME(glutWireIcosahedron);
  __GTKGLUT_CHECK_NAME(glutSolidIcosahedron);
  __GTKGLUT_CHECK_NAME(glutVideoResizeGet);
  __GTKGLUT_CHECK_NAME(glutSetupVideoResizing);
  __GTKGLUT_CHECK_NAME(glutStopVideoResizing);
  __GTKGLUT_CHECK_NAME(glutVideoResize);
  __GTKGLUT_CHECK_NAME(glutVideoPan);
  __GTKGLUT_CHECK_NAME(glutReportErrors);
  __GTKGLUT_CHECK_NAME(glutIgnoreKeyRepeat);
  __GTKGLUT_CHECK_NAME(glutSetKeyRepeat);
  __GTKGLUT_CHECK_NAME(glutForceJoystickFunc);
  __GTKGLUT_CHECK_NAME(glutGameModeString);
  __GTKGLUT_CHECK_NAME(glutEnterGameMode);
  __GTKGLUT_CHECK_NAME(glutLeaveGameMode);
  __GTKGLUT_CHECK_NAME(glutGameModeGet);
  /* freeglut+gtkglut extensions */
  __GTKGLUT_CHECK_NAME(glutMainLoopEvent);
  __GTKGLUT_CHECK_NAME(glutLeaveMainLoop);
  __GTKGLUT_CHECK_NAME(glutCloseFunc);
  __GTKGLUT_CHECK_NAME(glutWMCloseFunc);
  __GTKGLUT_CHECK_NAME(glutSetOption);
  __GTKGLUT_CHECK_NAME(glutSetWindowData);
  __GTKGLUT_CHECK_NAME(glutGetWindowData);
  __GTKGLUT_CHECK_NAME(glutSetMenuData);
  __GTKGLUT_CHECK_NAME(glutGetMenuData);
  __GTKGLUT_CHECK_NAME(glutBitmapHeight);
  __GTKGLUT_CHECK_NAME(glutStrokeHeight);
  __GTKGLUT_CHECK_NAME(glutBitmapString);
  __GTKGLUT_CHECK_NAME(glutStrokeString);
  __GTKGLUT_CHECK_NAME(glutWireRhombicDodecahedron);
  __GTKGLUT_CHECK_NAME(glutSolidRhombicDodecahedron);
  __GTKGLUT_CHECK_NAME(glutWireSierpinskiSponge);
  __GTKGLUT_CHECK_NAME(glutSolidSierpinskiSponge);
  __GTKGLUT_CHECK_NAME(glutWireCylinder);
  __GTKGLUT_CHECK_NAME(glutSolidCylinder);
  __GTKGLUT_CHECK_NAME(glutGetProcAddress);
  __GTKGLUT_CHECK_NAME(glutMouseWheelFunc);
#undef __GTKGLUT_CHECK_NAME

  return NULL;
}

/*! \defgroup gtkglut GtkGLUT Utilities

    Calling a library procedure directly can cause a
    link-time error if the feature does not exist.  GtkGLUT provides
    a facility to query whether the feature exists, and provides
    a function-pointer for the feature, if present.

    As example of usability high level library (GTK+) there is special function
    for loading images via GDKImage.
*/

/*!
    \brief    Determine if an GtkGLUT feature or extension is available.
    \ingroup  gtkglut
    \param    proc_name    Procedure name.

              Given a function name, searches for the function
              (or "procedure", hence "Proc") in
              internal tables.  If the function is found, a pointer
              to the function is returned.  If the function is not
              found, \a NULL is returned.

              In addition to an internal GtkGLUT table, this
              function will also consult glX (on X systems) or
              wgl (on WIN32 and WINCE), if the GtkGLUT tables
              do not have the requested function.  It should
              return any OpenGL, glX, or wgl function if those
              functions are available.

    \note     The code makes no effort towards efficiency.
              It is advised that you cache the pointer
              retrieved from this function if you need a
              certain function repeatedly.
*/
GLUTproc glutGetProcAddress(const char *proc_name) {
  GLUTproc p;

  __gtkglut_test_inicialization("glutGetProcAddress");
  
  /* Try GLUT functions first */
  p = __gtkglut_get_proc_address(proc_name);
  if( p != NULL )
      return p;

  /* Try core GL functions */

  return gdk_gl_get_proc_address(proc_name);
}

/*!
    \brief    Load image from file.
    \ingroup  gtkglut
    \param    file_name   Name of file with image to load.
    \param    width       pointer to integer, where will be stored width of readed image
    \param    height      pointer to integer, where will be stored height of readed image
    \param    format      pointer to integer, where will be stored format of readed image (\a GL_RGB and \a GL_RGBA)

    Applications often wish to load textures from images.
    Historically, one has been left to either implement one's
    own reader from scratch, or hunt down an image-format reader
    such as libpng for the PNG format and implement an interface
    to read the private format used to communicate to the library.
    While these are certainly viable options, they are nuisances.

    The proposed usage to get the image into a texture is to call:

    <code><pre>
    unsigned char *image=glutReadImageFile("test.png",&width,&height,&format);
    </pre></code>

    and then call
    
    <code><pre>
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        format,
        width,
        height,
        0,
        format,
        GL_UNSIGNED_BYTE,
        image
    );
    free( image );
    </pre></code>

    On invalid image, function return NULL.
    
  \note GtkGLUT does not guarantee that a valid image is returned,
     nor does it guarantee that the image has power-of-2 width and height.
     In practice, then, you should check at least those three values between
     the call to glutReadImageFile() and glTexImage2D().)

*/
unsigned char *glutReadImageFile(const char *file_name,GLsizei *width,GLsizei *height,GLint *format) {
  __gtkglut_test_inicialization("glutReadImageFile");
  GdkPixbuf *pixbuf;
  int no_channels;
  int rowstride;
  unsigned char *res;
  
  pixbuf=gdk_pixbuf_new_from_file(file_name,NULL);

  if (!pixbuf)
    return NULL;

  if (gdk_pixbuf_get_bits_per_sample(pixbuf)!=8)
    return NULL;

  if (gdk_pixbuf_get_colorspace (pixbuf) != GDK_COLORSPACE_RGB)
    return NULL;
  
  *width=gdk_pixbuf_get_width(pixbuf);
  *height=gdk_pixbuf_get_height(pixbuf);

  no_channels=gdk_pixbuf_get_n_channels(pixbuf);

  switch (no_channels) {
    case 3:
      *format=GL_RGB;
    break;

    case 4:
      *format=GL_RGBA;
    break;

    default:
      /*Unsupported format*/
      return NULL;  
  }
  
  rowstride= gdk_pixbuf_get_rowstride(pixbuf);

  res=(unsigned char*)malloc((*width)*(*height)*no_channels);

  if (!res)
    return NULL;

  memcpy(res,gdk_pixbuf_get_pixels (pixbuf),(*width)*(*height)*no_channels);

  gdk_pixbuf_unref(pixbuf);

  return res;
}
