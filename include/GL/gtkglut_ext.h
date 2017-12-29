/*!
    \file  gtkglut_ext.h
    \brief Extended functions header.
    
    \include gtkglut_ext.h
*/

/* 
 * Extended functions header.
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
 */

#ifndef __gtkglut_ext_h__
#define __gtkglut_ext_h__

#ifdef __cplusplus
extern "C" {
#endif

/*
 * GLUT API Extension macro definitions -- behaviour when the user clicks on an "x" to close a window
 */
#define GLUT_ACTION_EXIT                         0
#define GLUT_ACTION_GLUTMAINLOOP_RETURNS         1
#define GLUT_ACTION_CONTINUE_EXECUTION           2

/*
 * Create a new rendering context when the user opens a new window?
 */
#define GLUT_CREATE_NEW_CONTEXT                  0
#define GLUT_USE_CURRENT_CONTEXT                 1

/*
 * GLUT API Extension macro definitions -- the glutGet parameters
 */
#define  GLUT_ACTION_ON_WINDOW_CLOSE        0x01F9    
#define  GLUT_VERSION                       0x01FC
#define  GTKGLUT_VERSION                    0x0201

/*4 and 5 button on mouse*/
#define GLUT_FOURTH_BUTTON              3
#define GLUT_FIFTH_BUTTON               4

/*Mouse wheel*/
#define GLUT_WHEEL_UP                   1
#define GLUT_WHEEL_DOWN                -1  

extern void glutMouseWheelFunc(void (*func)(int wheel, int direction, int x, int y));

/* * Font stuff*/
int glutBitmapHeight( void* font );
GLfloat glutStrokeHeight( void* font );
void glutBitmapString( void* font, const unsigned char *string );
void glutStrokeString( void* font, const unsigned char *string );
  
/*Process loop functions*/
extern void glutMainLoopEvent(void);
extern void glutLeaveMainLoop(void);

/*State settings*/
extern void glutSetOption(int what,int value);

/*Close window registration*/
extern void glutCloseFunc(void (*func)(void));
extern void glutWMCloseFunc(void (*func)(void));

/*Data association*/
extern void glutSetWindowData(void *data);
extern void *glutGetWindowData(void);
extern void glutSetMenuData(void *data);
extern void *glutGetMenuData(void);

/*Extra geometric objects*/
extern void glutWireRhombicDodecahedron( void );
extern void glutSolidRhombicDodecahedron( void );
extern void glutWireSierpinskiSponge ( int num_levels, GLdouble offset[3], GLdouble scale ) ;
extern void glutSolidSierpinskiSponge ( int num_levels, GLdouble offset[3], GLdouble scale ) ;
extern void glutWireCylinder( GLdouble radius, GLdouble height, GLint slices, GLint stacks);
extern void glutSolidCylinder( GLdouble radius, GLdouble height, GLint slices, GLint stacks);

typedef void (*GLUTproc)(void);
extern GLUTproc glutGetProcAddress(const char *proc_name);

extern unsigned char *glutReadImageFile(const char *file_name,GLsizei *width,GLsizei *height,GLint *format);

#ifdef __cplusplus
}
#endif
#endif                  /* __gtkglut_ext_h__ */
