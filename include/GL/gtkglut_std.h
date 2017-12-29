/*!
    \file  gtkglut_std.h
    \brief Standard functions header.

    \include gtkglut_std.h
*/

/* 
 * Standard functions header.
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
#ifndef __gtkglut_std_h__
#define __gtkglut_std_h__

#include <GL/gl.h>
#include <GL/glu.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef GLUT_API_VERSION  /* allow this to be overriden */
#define GLUT_API_VERSION 3
#endif

#ifndef GLUT_XLIB_IMPLEMENTATION  /* allow this to be overriden */
#define GLUT_XLIB_IMPLEMENTATION  9
#endif

/* Display mode masks*/
#define GLUT_RGB                        0
#define GLUT_RGBA                       GLUT_RGB
#define GLUT_INDEX                      1
#define GLUT_SINGLE                     0
#define GLUT_DOUBLE                     2
#define GLUT_ACCUM                      4
#define GLUT_ALPHA                      8
#define GLUT_DEPTH                      16
#define GLUT_STENCIL                    32
#define GLUT_MULTISAMPLE                128
#define GLUT_STEREO                     256
#define GLUT_LUMINANCE                  512
  
/* glutGet parameters */
#define GLUT_WINDOW_X                   100
#define GLUT_WINDOW_Y                   101
#define GLUT_WINDOW_WIDTH               102
#define GLUT_WINDOW_HEIGHT              103
#define GLUT_WINDOW_BUFFER_SIZE         104
#define GLUT_WINDOW_STENCIL_SIZE        105
#define GLUT_WINDOW_DEPTH_SIZE          106
#define GLUT_WINDOW_RED_SIZE            107
#define GLUT_WINDOW_GREEN_SIZE          108
#define GLUT_WINDOW_BLUE_SIZE           109
#define GLUT_WINDOW_ALPHA_SIZE          110
#define GLUT_WINDOW_ACCUM_RED_SIZE      111
#define GLUT_WINDOW_ACCUM_GREEN_SIZE    112
#define GLUT_WINDOW_ACCUM_BLUE_SIZE     113
#define GLUT_WINDOW_ACCUM_ALPHA_SIZE    114
#define GLUT_WINDOW_DOUBLEBUFFER        115
#define GLUT_WINDOW_RGBA                116
#define GLUT_WINDOW_PARENT              117
#define GLUT_WINDOW_NUM_CHILDREN        118
#define GLUT_WINDOW_COLORMAP_SIZE       119
#define GLUT_WINDOW_NUM_SAMPLES         120
#define GLUT_WINDOW_STEREO              121
#define GLUT_WINDOW_CURSOR              122
#define GLUT_SCREEN_WIDTH               200
#define GLUT_SCREEN_HEIGHT              201
#define GLUT_SCREEN_WIDTH_MM            202
#define GLUT_SCREEN_HEIGHT_MM           203
#define GLUT_MENU_NUM_ITEMS             300
#define GLUT_DISPLAY_MODE_POSSIBLE      400
#define GLUT_INIT_WINDOW_X              500
#define GLUT_INIT_WINDOW_Y              501
#define GLUT_INIT_WINDOW_WIDTH          502
#define GLUT_INIT_WINDOW_HEIGHT         503
#define GLUT_INIT_DISPLAY_MODE          504
#define GLUT_ELAPSED_TIME               700
#define GLUT_WINDOW_FORMAT_ID           123

/* glutSetCursor parameters */
/* Basic arrows */
#define GLUT_CURSOR_RIGHT_ARROW         0
#define GLUT_CURSOR_LEFT_ARROW          1
/* Symbolic cursor shapees */
#define GLUT_CURSOR_INFO                2
#define GLUT_CURSOR_DESTROY             3
#define GLUT_CURSOR_HELP                4
#define GLUT_CURSOR_CYCLE               5
#define GLUT_CURSOR_SPRAY               6
#define GLUT_CURSOR_WAIT                7
#define GLUT_CURSOR_TEXT                8
#define GLUT_CURSOR_CROSSHAIR           9
/* Directional cursors */
#define GLUT_CURSOR_UP_DOWN             10
#define GLUT_CURSOR_LEFT_RIGHT          11
/* Sizing cursors */
#define GLUT_CURSOR_TOP_SIDE            12
#define GLUT_CURSOR_BOTTOM_SIDE         13
#define GLUT_CURSOR_LEFT_SIDE           14
#define GLUT_CURSOR_RIGHT_SIDE          15
#define GLUT_CURSOR_TOP_LEFT_CORNER     16
#define GLUT_CURSOR_TOP_RIGHT_CORNER    17
#define GLUT_CURSOR_BOTTOM_RIGHT_CORNER 18
#define GLUT_CURSOR_BOTTOM_LEFT_CORNER  19
/* Inherit from parent window */
#define GLUT_CURSOR_INHERIT             100
/* Blank cursor */
#define GLUT_CURSOR_NONE                101
/* Fullscreen crosshair {not avaliable} */
#define GLUT_CURSOR_FULL_CROSSHAIR      102

/* visibility callback state */
#define GLUT_NOT_VISIBLE                0
#define GLUT_VISIBLE                    1

/* Window status  state. */
#define GLUT_HIDDEN                     0
#define GLUT_FULLY_RETAINED             1
#define GLUT_PARTIALLY_RETAINED         2
#define GLUT_FULLY_COVERED              3

/* mouse buttons */
#define GLUT_LEFT_BUTTON                0
#define GLUT_MIDDLE_BUTTON              1
#define GLUT_RIGHT_BUTTON               2

/* menu usage callback state */
#define GLUT_MENU_NOT_IN_USE            0
#define GLUT_MENU_IN_USE                1

/* color index component selection values */
#define GLUT_RED                        0
#define GLUT_GREEN                      1
#define GLUT_BLUE                       2

/* layers for use */
#define GLUT_NORMAL                     0
#define GLUT_OVERLAY                    1

/* mouse button callback state */
#define GLUT_DOWN                       0
#define GLUT_UP                         1

/* glutGetModifiers return mask */
#define GLUT_ACTIVE_SHIFT               1
#define GLUT_ACTIVE_CTRL                2
#define GLUT_ACTIVE_ALT                 4
  
/* function keys */
#define GLUT_KEY_F1                     1
#define GLUT_KEY_F2                     2
#define GLUT_KEY_F3                     3
#define GLUT_KEY_F4                     4
#define GLUT_KEY_F5                     5
#define GLUT_KEY_F6                     6
#define GLUT_KEY_F7                     7
#define GLUT_KEY_F8                     8
#define GLUT_KEY_F9                     9
#define GLUT_KEY_F10                    10
#define GLUT_KEY_F11                    11
#define GLUT_KEY_F12                    12
/* directional keys */
#define GLUT_KEY_LEFT                   100
#define GLUT_KEY_UP                     101
#define GLUT_KEY_RIGHT                  102
#define GLUT_KEY_DOWN                   103
#define GLUT_KEY_PAGE_UP                104
#define GLUT_KEY_PAGE_DOWN              105
#define GLUT_KEY_HOME                   106
#define GLUT_KEY_END                    107
#define GLUT_KEY_INSERT                 108

/* entry/exit callback state */
#define GLUT_LEFT                       0
#define GLUT_ENTERED                    1

/*Font definitions - for binary compatibility. Don't use this in glut programs!!!!!!)*/
extern void *glutStrokeRoman;
extern void *glutStrokeMonoRoman;
extern void *glutBitmap9By15;
extern void *glutBitmap8By13;
extern void *glutBitmapTimesRoman10;
extern void *glutBitmapTimesRoman24;
extern void *glutBitmapHelvetica10;
extern void *glutBitmapHelvetica12;
extern void *glutBitmapHelvetica18;

/*
 * GLUT API macro definitions -- fonts definitions
 *
 * Steve Baker suggested to make it binary compatible with GLUT:
 */
/* stroke font constants {use these in GLUT program} */
#define  GLUT_STROKE_ROMAN               (&glutStrokeRoman)
#define  GLUT_STROKE_MONO_ROMAN          (&glutStrokeMonoRoman)
/* bitmap font constants {use these in GLUT program} */
#define  GLUT_BITMAP_9_BY_15             (&glutBitmap9By15)
#define  GLUT_BITMAP_8_BY_13             (&glutBitmap8By13)
#define  GLUT_BITMAP_TIMES_ROMAN_10      (&glutBitmapTimesRoman10)
#define  GLUT_BITMAP_TIMES_ROMAN_24      (&glutBitmapTimesRoman24)
#define  GLUT_BITMAP_HELVETICA_10        (&glutBitmapHelvetica10)
#define  GLUT_BITMAP_HELVETICA_12        (&glutBitmapHelvetica12)
#define  GLUT_BITMAP_HELVETICA_18        (&glutBitmapHelvetica18)

/* glutDeviceGet parameters */
#define GLUT_HAS_KEYBOARD               600
#define GLUT_HAS_MOUSE                  601
#define GLUT_HAS_SPACEBALL              602
#define GLUT_HAS_DIAL_AND_BUTTON_BOX    603
#define GLUT_HAS_TABLET                 604
#define GLUT_NUM_MOUSE_BUTTONS          605
#define GLUT_NUM_SPACEBALL_BUTTONS      606
#define GLUT_NUM_BUTTON_BOX_BUTTONS     607
#define GLUT_NUM_DIALS                  608
#define GLUT_NUM_TABLET_BUTTONS         609
#define GLUT_DEVICE_IGNORE_KEY_REPEAT   610
#define GLUT_DEVICE_KEY_REPEAT          611
#define GLUT_HAS_JOYSTICK               612
#define GLUT_OWNS_JOYSTICK              613
#define GLUT_JOYSTICK_BUTTONS           614
#define GLUT_JOYSTICK_AXES              615
#define GLUT_JOYSTICK_POLL_RATE         616

/* glutLayerGet parameters */
#define GLUT_OVERLAY_POSSIBLE           800
#define GLUT_LAYER_IN_USE               801
#define GLUT_HAS_OVERLAY                802
#define GLUT_TRANSPARENT_INDEX          803
#define GLUT_NORMAL_DAMAGED             804
#define GLUT_OVERLAY_DAMAGED            805

/* glutSetKeyRepeat modes. */
#define GLUT_KEY_REPEAT_OFF             0
#define GLUT_KEY_REPEAT_ON              1
#define GLUT_KEY_REPEAT_DEFAULT         2

/* Joystick button masks. */
#define GLUT_JOYSTICK_BUTTON_A          1
#define GLUT_JOYSTICK_BUTTON_B          2
#define GLUT_JOYSTICK_BUTTON_C          4
#define GLUT_JOYSTICK_BUTTON_D          8

/* glutVideoResizeGet parameters. */
#define GLUT_VIDEO_RESIZE_POSSIBLE      900
#define GLUT_VIDEO_RESIZE_IN_USE        901
#define GLUT_VIDEO_RESIZE_X_DELTA       902
#define GLUT_VIDEO_RESIZE_Y_DELTA       903
#define GLUT_VIDEO_RESIZE_WIDTH_DELTA   904
#define GLUT_VIDEO_RESIZE_HEIGHT_DELTA  905
#define GLUT_VIDEO_RESIZE_X             906
#define GLUT_VIDEO_RESIZE_Y             907
#define GLUT_VIDEO_RESIZE_WIDTH         908
#define GLUT_VIDEO_RESIZE_HEIGHT        909

/* GLUT game mode sub-API. */
/* glutGameModeGet. */
#define GLUT_GAME_MODE_ACTIVE           0
#define GLUT_GAME_MODE_POSSIBLE         1
#define GLUT_GAME_MODE_WIDTH            2
#define GLUT_GAME_MODE_HEIGHT           3
#define GLUT_GAME_MODE_PIXEL_DEPTH      4
#define GLUT_GAME_MODE_REFRESH_RATE     5
#define GLUT_GAME_MODE_DISPLAY_CHANGED  6

/*Init functions*/
extern void glutInit(int *argcp, char **argv);
extern void glutInitWindowSize(int width, int height);
extern void glutInitWindowPosition(int x, int y);
extern void glutInitDisplayMode(unsigned int mode);
extern void glutInitDisplayString(char* string);

/*Beginning Event Processing function*/
extern void glutMainLoop(void);

/*Window Management functions*/
extern int glutCreateWindow(char *name);
extern int glutCreateSubWindow(int win,int x, int y, int width, int height);
extern int glutGetWindow(void);
extern void glutSetWindow(int win);
extern void glutDestroyWindow(int win);
extern void glutPostRedisplay(void);
extern void glutPostWindowRedisplay(int win);
extern void glutSwapBuffers(void);
extern void glutPositionWindow(int x, int y);
extern void glutReshapeWindow(int width, int height);
extern void glutFullScreen(void);
extern void glutPopWindow(void);
extern void glutPushWindow(void);
extern void glutShowWindow(void);
extern void glutHideWindow(void);
extern void glutIconifyWindow(void);
extern void glutSetWindowTitle(char *name);
extern void glutSetIconTitle(char *name);
extern void glutSetCursor(int cursor);
extern void glutWarpPointer(int x, int y);

/*Menu management functions*/
extern int glutCreateMenu(void (*func)(int value));
extern void glutSetMenu(int menu);
extern int glutGetMenu(void);
extern void glutDestroyMenu(int menu);
extern void glutAddMenuEntry(char *name, int value);
extern void glutAddSubMenu(char *name, int menu);
extern void glutChangeToMenuEntry(int entry, char *name, int value);
extern void glutChangeToSubMenu(int entry, char *name, int menu);
extern void glutRemoveMenuItem(int entry);
extern void glutAttachMenu(int button);
extern void glutDetachMenu(int button);

/*State Retrieval functions*/
extern int glutGet(GLenum state);
extern int glutGetModifiers(void);
extern int glutLayerGet(GLenum info);
extern int glutDeviceGet(GLenum info);
extern int glutExtensionSupported(char *extension);

/*Callback Registration functions*/
extern void glutDisplayFunc(void (*func)(void));
extern void glutReshapeFunc(void (*func)(int width, int height));
extern void glutKeyboardFunc(void (*func)(unsigned char key,int x, int y));
extern void glutKeyboardUpFunc(void (*func)(unsigned char key,int x, int y));
extern void glutMouseFunc(void (*func)(int button, int state,int x, int y));
extern void glutMotionFunc(void (*func)(int x, int y));
extern void glutPassiveMotionFunc(void (*func)(int x, int y));
extern void glutVisibilityFunc(void (*func)(int state));
extern void glutEntryFunc(void (*func)(int state));
extern void glutSpecialFunc(void (*func)(int key, int x, int y));
extern void glutSpecialUpFunc(void (*func)(int key, int x, int y));
extern void glutMenuStatusFunc(void (*func)(int status, int x, int y));
extern void glutMenuStateFunc(void (*func)(int status));
extern void glutIdleFunc(void (*func)(void));
extern void glutTimerFunc(unsigned int msecs, void (*func)(int value), int value);
extern void glutWindowStatusFunc(void (*func)(int state));

/*Unimplemented functions*/
extern void glutOverlayDisplayFunc(void (*func)(void));
extern void glutSpaceballMotionFunc(void (*func)(int x, int y, int z));
extern void glutSpaceballRotateFunc(void (*func)(int x, int y, int z));
extern void glutSpaceballButtonFunc(void (*func)(int button, int state));
extern void glutButtonBoxFunc(void (*func)(int button, int state));
extern void glutDialsFunc(void (*func)(int dial, int value));
extern void glutTabletMotionFunc(void (*func)(int x, int y));
extern void glutTabletButtonFunc(void (*func)(int button, int state, int x, int y));
extern void glutJoystickFunc(void (*func)(unsigned int buttonMask,int x, int y, int z), int pollInterval);
extern void glutForceJoystickFunc(void);

/*Overlay functions. Not implemented for now (and propably not implemented ever).*/
extern void glutEstablishOverlay(void);
extern void glutUseLayer(GLenum layer);
extern void glutRemoveOverlay(void);
extern void glutPostOverlayRedisplay(void);
extern void glutShowOverlay(void);
extern void glutHideOverlay(void);
extern void glutPostWindowOverlayRedisplay(int win);

/*Keyrepeat functions. Unimplemented*/
extern void glutIgnoreKeyRepeat(int ignore);
extern void glutSetKeyRepeat(int repeatMode);

/*Geometric Object Rendering*/
extern void glutSolidSphere(GLdouble radius,GLint slices, GLint stacks);
extern void glutWireSphere(GLdouble radius,GLint slices, GLint stacks);
extern void glutSolidCube(GLdouble size);
extern void glutWireCube(GLdouble size);
extern void glutSolidCone(GLdouble base, GLdouble height,GLint slices, GLint stacks);
extern void glutWireCone(GLdouble base, GLdouble height,GLint slices, GLint stacks);
extern void glutSolidTorus(GLdouble innerRadius, GLdouble outerRadius, GLint nsides, GLint rings);
extern void glutWireTorus(GLdouble innerRadius, GLdouble outerRadius, GLint nsides, GLint rings);
extern void glutSolidDodecahedron(void);
extern void glutWireDodecahedron(void);
extern void glutSolidOctahedron(void);
extern void glutWireOctahedron(void);
extern void glutSolidTetrahedron(void);
extern void glutWireTetrahedron(void);
extern void glutSolidIcosahedron(void);
extern void glutWireIcosahedron(void);
extern void glutSolidTeapot(GLdouble size);
extern void glutWireTeapot(GLdouble size);

/* GLUT debugging sub-API. */
extern void glutReportErrors(void);

/*Game mode - Unimplemented*/
extern void  glutGameModeString(const char *string);
extern int  glutEnterGameMode(void);
extern void  glutLeaveGameMode(void);
extern int  glutGameModeGet(GLenum mode);

/*Index mode support - unimplemented*/
extern void glutSetColor(int cell,GLfloat red, GLfloat green, GLfloat blue);
extern GLfloat glutGetColor(int cell, int component);
extern void glutCopyColormap(int win);

/* GLUT video resize sub-API. - unimplemented*/
extern int glutVideoResizeGet(GLenum param);
extern void glutSetupVideoResizing(void);
extern void glutStopVideoResizing(void);
extern void glutVideoResize(int x, int y, int width, int height);
extern void glutVideoPan(int x, int y, int width, int height);

/*Font rendering support*/
extern void glutBitmapCharacter(void *font, int character);
extern int glutBitmapWidth(void *font, int character);
extern void glutStrokeCharacter(void *font, int character);
extern int glutStrokeWidth(void *font, int character);
extern int glutBitmapLength( void* font, const unsigned char* string );
extern int glutStrokeLength( void* font, const unsigned char* string );

#ifdef __cplusplus
}

#endif
#endif                  /* __gtkglut_std_h__ */
