/*!
    \file  gtkglut_unimplemented.c
    \brief All unimplemented functions reside in this file.
*/

/* 
 * All unimplemented functions reside in this file.
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
#include "GL/gtkglut.h"
#include "gtkglut_internal.h"

/*Callback registration*/

/*!
    \brief    Defines the display hook for an overlay.
    \ingroup  windowcallback
    \param    func    Client hook for overlay redisplay.

              This callback is bound to <b>both</b>
              the <i>current window</i> and
              the <i>layer</i>.

    \note     Unimplemented.
    \see      glutDisplayFunc(), glutPostOverlayRedisplay(),
              glutEstablishOverlay(), glutUseLayer()
*/
void glutOverlayDisplayFunc(void (*func)(void)) {
  __gtkglut_unimplemented_warning("glutOverlayDisplayFunc");
}

/*!
    \brief    Sets a spaceball button callback.
    \ingroup  input
    \param    func    Client spaceball button hook.

              This function registers a callback for a spaceball
              to report buttons.  A spaceball has six axes of freedom
              (three of motion and three of orientation), plus buttons.
              The spaceball has glutDeviceGet(\a GLUT_NUM_SPACEBALL_BUTTONS)
              and numbers them from 1.  Button state is either
              \a GLUT_UP or \a GLUT_DOWN.

              The callback is bound to the <i>current window</i>.

    \note     GtkGLUT does not implement spaceball support.
    \note     In contrast, mouse buttons are numbered from 0 in
              the GLUT API.  This is a wrinkle.
    \see      glutSpaceballRotateFunc(), glutSpaceballMotiononFunc(),
*/
void glutSpaceballMotionFunc(void (*func)(int x, int y, int z)) {
  __gtkglut_unimplemented_warning("glutSpaceballMotionFunc");
}

/*!
    \brief    Sets a spaceball rotation callback.
    \ingroup  input
    \param    func    Client spaceball rotation hook.

              This function registers a callback for a spaceball
              to report rotation.  A spaceball has six axes of freedom
              (three of motion and three of orientation), plus buttons.
              The spaceball allows you to rotate a control orientation
              +/- 1800 units about each of 3 axes.

              The callback is bound to the <i>current window</i>.

    \note     GtkGLUT does not implement spaceball support.
    \see      glutSpaceballMotionFunc(), glutSpaceballButtonFunc(),
*/
void glutSpaceballRotateFunc(void (*func)(int x, int y, int z)) {
  __gtkglut_unimplemented_warning("glutSpaceballRotateFunc");
}

/*!
    \brief    Sets a spaceball motion callback.
    \ingroup  input
    \param    func    Client spaceball motion hook.

              This function registers a callback for a spaceball
              to report position.  A spaceball has six axes of freedom
              (three of motion and three of orientation), plus buttons.
              The spaceball allows you to move a control point
              in 3D space with a resolution of +/- 1000 units
              along each of 3 axes.

              The callback is bound to the <i>current window</i>.

    \note     GtkGLUT does not implement spaceball support.
    \see      glutSpaceballRotateFunc(), glutSpaceballButtonFunc(),
*/
void glutSpaceballButtonFunc(void (*func)(int button, int state)) {
  __gtkglut_unimplemented_warning("glutSpaceballButtonFunc");
}

/*!
    \brief    Sets a button-box button callback.
    \ingroup  input
    \param    func    Client buttonbox button hook.

              A dials-and-buttons box has buttons numbered from 1 to
              glutDeviceGet(\a GLUT_NUM_BUTTON_BOX_BUTTONS), inclusive.
              The parameters callback are the \a button and its
              \a state as either \a GLUT_UP or \a GLUT_DOWN .

              The callback is bound to the <i>current window</i>.

    \note     GtkGLUT does not implement button-box support.
    \note     Also in contrast, mouse buttons are numbered from 0 in
              the GLUT API.  This is a wrinkle.
    \see      glutDialsFunc()
*/
void glutButtonBoxFunc(void (*func)(int button, int state)) {
  __gtkglut_unimplemented_warning("glutButtonBoxFunc");
}

/*!
    \brief    Sets a dials-box button callback.
    \ingroup  input
    \param    func    Client dials-box dial hook.

              A dials-and-buttons box has dials numbered from 1 to
              glutDeviceGet(\a GLUT_NUM_BUTTON_BOX_DIALS), inclusive.
              The parameters to \a callback are the \a dial and its
              \a value the latter being an absolute rotation in
              degrees.

              The callback is bound to the <i>current window</i>.

    \note     GtkGLUT does not implement dials-box support.
    \see      glutButtonBoxFunc()
*/
void glutDialsFunc(void (*func)(int dial, int value)) {
  __gtkglut_unimplemented_warning("glutDialsFunc");
}

/*!
    \brief    Sets a tablet motion callback.
    \ingroup  input
    \param    func    Client tablet motion hook.

              This function registers a \a callback by which GtkGLUT
              reports a puck or stylus position in the range
              of [0, 2000] along the \a x and \a y axes.

              The callback is bound to the <i>current window</i>.

    \note     GtkGLUT does not implement tablet support.
    \note     This API does not include tilt information.
    \see      glutTabletButtonFunc(), glutMouseFunc()
*/
void glutTabletMotionFunc(void (*func)(int x, int y)) {
  __gtkglut_unimplemented_warning("glutTabletMotionFunc");
}

/*!
    \brief    Sets a tablet button callback.
    \ingroup  input
    \param    func    Client tablet button hook.

              This function registers a \a callback by which you receive
              reports a tablet button status feature.  Buttons are
              reported with \a button in the range 1 to
              glutDeviceGet(\a GLUT_NUM_TABLET_BUTTONS).  \a state is either
              \a GLUT_UP or \a GLUT_DOWN and \a x and \a y are the
              tablet coordinate (see glutTabletMotionFunc() for the bounds
              of \a x and \a y).

              The callback is bound to the <i>current window</i>.

    \note     GtkGLUT does not implement tablet support.
    \note     Buttons are <b>not</b> pressure-sensitive.
    \see      glutTabletButtonFunc(), glutMouseFunc()
*/
void glutTabletButtonFunc(void (*func)(int button, int state, int x, int y)) {
  __gtkglut_unimplemented_warning("glutTabletButtonFunc");
}

/*!
    \brief    Reports joystick state for the current window.
    \ingroup  input
    \param    func      Client function for joystick events
    \param    pollInterval  Approximate (minimum) millisecond interval

              The callback is called roughly every \a pollinterval
              milliseconds, and will give the joystick status.

              The \a buttons bitmask is a bit-wise \a OR of:
              - GLUT_JOYSTICK_BUTTON_A
              - GLUT_JOYSTICK_BUTTON_B
              - GLUT_JOYSTICK_BUTTON_C
              - GLUT_JOYSTICK_BUTTON_D

              The axis values are in the range [-1000,1000].

    \note     GtkGLUT does not implement joystick support.
*/
void glutJoystickFunc(void (*func)(unsigned int buttonMask,int x, int y, int z), int pollInterval) {
  __gtkglut_unimplemented_warning("glutJoystickFunc");
}

/*!
    \brief    Forces a joystick poll and callback.
    \ingroup  input

              Forces the GtkGLUT joystick code to poll your
              joystick(s) and to call your joystick callbacks
              with the result.  The operation completes, including
              callbacks, before glutForceJoystickFunc() returns.

    \note     GtkGLUT does not implement joystick support.
    \see      glutJoystickFunc()
*/
void glutForceJoystickFunc(void) {
  __gtkglut_unimplemented_warning("glutForceJoystickFunc");
}

/*Overlay*/
/*! \defgroup overlays Overlays

    Some windows may be able to handle one or more overlays.
    That is, some windows may be able to have a secondary bitmap
    that is placed over the normal display.  This allows two or more
    bitmapped images to be displayed in a single window without
    directly intefering with one other---beyond the fact that one
    may obscure the other.  The value here is that then the overlain
    bitmap may be removed or hidden and the underlain graphics
    will appear without requiring a redraw.

    Old GLUT only supported indexed color for overlays.  freeglut
    ignores all overlay requests.  GtkGLUT has inherited freeglut's
    treatment.  GtkGLUT has neither resolved to fully support
    overlays, nor to remove them.
*/

/*!
    \brief    Creates an overlay.
    \ingroup  overlays

              Creates an overlay associated with the <i>current window</i>.

    \note     Unimplemented.
    \note     Old GLUT would terminate the program when an overlay
              was not possible.  freeglut and OpenGLUT ignores the request;
              GtkGLUT ignores the request as well.
    \see      glutRemoveOverlay(),
              glutUseLayer(), glutPostOverlayRedisplay(),
              glutPostWindowOverlayRedisplay(),
              glutShowOverlay(), glutHideOverlay()
*/
void glutEstablishOverlay(void) {
  __gtkglut_unimplemented_warning("glutEstablishOverlay");    
}

/*!
    \brief    Allows you to switch between normal and layer mode.
    \ingroup  overlays
    \param    layer    Whether to be in a layer or in the normal window.

              By default, GtkGLUT operates in "normal" mode, with
              respect to layers.  If you have a layer open and wish
              to operate on the layer, you must use glutUseLayer().

              \a layer can take on the following values, indicating
              the layer mode to use:

               - \a GLUT_NORMAL
               - \a GLUT_LAYER

    \note     Unimplemented.
    \note     It is unclear what the consequences are if you are in
              \a GLUT_OVERLAY mode and switch to another (or the same)
              window via glutSetWindow().  What if the target has
              a layer?  What if it doesn't?
    \see      glutEstablishOverlay(), glutRemoveOverlay(),
              glutPostOverlayRedisplay(),
              glutPostWindowOverlayRedisplay(),
              glutShowOverlay(), glutHideOverlay()
*/
void glutUseLayer(GLenum layer) {
  __gtkglut_unimplemented_warning("glutUseLayer");
}

/*!
    \brief    Removes an overlay.
    \ingroup  overlays

              Removes the overlay associated with the <i>current window</i>.

    \note     Unimplemented.
    \see      glutEstablishOverlay(),
              glutUseLayer(), glutPostOverlayRedisplay(),
              glutPostWindowOverlayRedisplay(),
              glutShowOverlay(), glutHideOverlay()
*/
void glutRemoveOverlay(void) {
  __gtkglut_unimplemented_warning("glutRemoveOverlay");
}

/*!
    \brief    Posts a redispaly against the layer for the current window.
    \ingroup  overlays

              For the <i>current window</i>, tells GtkGLUT that you
              wish to have your OverlayDisplay callback invoked.

    \note     Unimplemented.
    \see      glutEstablishOverlay(), glutRemoveOverlay(),
              glutUseLayer(),
              glutPostWindowOverlayRedisplay(),
              glutShowOverlay(), glutHideOverlay(), glutOverlayDisplayFunc(),
              glutPostRedisplay()
*/
void glutPostOverlayRedisplay(void) {
  __gtkglut_unimplemented_warning("glutPostOverlayRedisplay");
}

/*!
    \brief    Make an overlay visible.
    \ingroup  overlays

              Causes a previously-hidden overlay to become
              apparent.  Applies to the <i>current window</i>.

    \note     Unimplemented.
    \see      glutEstablishOverlay(), glutRemoveOverlay(),
              glutUseLayer(), glutPostOverlayRedisplay(),
              glutPostWindowOverlayRedisplay(),
              glutHideOverlay()
*/
void glutShowOverlay(void) {
  __gtkglut_unimplemented_warning("glutShowOverlay");
}

/*!
    \brief    Make an overlay invisible.
    \ingroup  overlays

              Causes a visible overlay to become invisible.
              Applies to the <i>current window</i>.

    \note     Unimplemented.
    \see      glutEstablishOverlay(), glutRemoveOverlay(),
              glutUseLayer(), glutPostOverlayRedisplay(),
              glutPostWindowOverlayRedisplay(),
              glutShowOverlay()
*/
void glutHideOverlay(void) {
  __gtkglut_unimplemented_warning("glutHideOverlay");
}

/*!
    \brief    Posts a redisplay to the indicated window's layer.
    \ingroup  overlays
    \param    win    A window ID.

              This function lets you post an overlay update to the overlay
              of any arbitrary window.

    \note     Unimplemented.
    \see      glutEstablishOverlay(), glutRemoveOverlay(),
              glutUseLayer(), glutPostOverlayRedisplay(),
              glutShowOverlay(), glutHideOverlay(), glutOverlayDisplayFunc(),
              glutPostRedisplay()
*/
void glutPostWindowOverlayRedisplay(int win) {
  __gtkglut_unimplemented_warning("glutPostWindowOverlayRedisplay");
}

/*Key repeat*/
/*!
    \brief    Set autorepeat status.
    \ingroup  inputstate
    \param    ignore    Whether to ignore autorepeated keys.

              If \a ignore is non-zero, then auto-repeat is
              disabled for keyboard callbacks for the
              <i>current window</i>.

    \note     Unimplemented
    \see      glutSetKeyRepeat()
*/
void glutIgnoreKeyRepeat(int ignore) {
  __gtkglut_unimplemented_warning("glutIgnoreKeyRepeat");
}

/*!
    \brief    Sets autorepeat behavior for all GtkGLUT windows.
    \ingroup  inputstate
    \param    repeatMode    On, Off or Default.

              glutSetKeyRepeat() is similar to glutIgnoreKeyRepeat()
              but sets the behavior
              for GtkGLUT in general, rather than for a particular
              window.  The options for \a repeatMode are:

               - \a GLUT_KEY_REPEAT_OFF     \n Turn off repeat for all windows.
               - \a GLUT_KEY_REPEAT_ON      \n Turn on repeat for all windows.
               - \a GLUT_KEY_REPEAT_DEFAULT \n Respect the window's setting.

    \note     Unimplemented
    \see      glutIgnoreKeyRepeat()
*/
void glutSetKeyRepeat(int repeatMode) {
  __gtkglut_unimplemented_warning("glutSetKeyRepeat");
}

/*Video resize*/
/*! \defgroup videomode Video Mode

    GtkGLUT inherited a stubbed out API for several video
    mode controls.  They are not implemented at this time, but
    may be useful in replacing gamemode.
*/

/*!
    \brief    Undocumented, unimplemented.
    \ingroup  videomode
    \param    param    An enumerated tag.

              This function is undocumented.  This function is
              unimplemeneted (presently).

              From GLUT 3.7's implementation, the following is a
              list of symbols that GLUT 3.7 supported.  These are
              not necessarily defined in GtkGLUT at this time,
              nor is support presently available for them:

               - \a GLUT_VIDEO_RESIZE_POSSIBLE (feature presence)
               - \a GLUT_VIDEO_RESIZE_IN_USE
               - \a GLUT_VIDEO_RESIZE_X_DELTA
               - \a GLUT_VIDEO_RESIZE_Y_DELTA
               - \a GLUT_VIDEO_RESIZE_WIDTH_DELTA
               - \a GLUT_VIDEORESIZE_HEIGHT_DELTA
               - \a GLUT_VIDEO_REISZE_X
               - \a GLUT_VIDEO_RESIZE_Y
               - \a GLUT_VIDEO_RESIZE_WIDTH
               - \a GLUT_VIDEO_RESIZE_HEIGHT

              A 0 should be returned for unimplemented features, and
              a warning printed.

    \see      glutVideoResizeGet(), glutSetupVideoResizing(),
              glutStopVideoResizing(), glutVideoResize(), glutVideoPan()
*/
int glutVideoResizeGet(GLenum param) {
  __gtkglut_unimplemented_warning("glutVideoResizeGet");

  return 0;
}

/*!
    \brief    Undocumented, unimplemented.
    \ingroup  videomode

              This function is undocumented.  This function is
              unimplemeneted (presently).

    \see      glutVideoResizeGet(), glutSetupVideoResizing(),
              glutStopVideoResizing(), glutVideoResize(), glutVideoPan()
*/
void glutSetupVideoResizing(void) {
  __gtkglut_unimplemented_warning("glutSetupVideoResizing");
}

/*!
    \brief    Undocumented, unimplemented.
    \ingroup  videomode

              This function is undocumented.  This function is
              unimplemeneted (presently).

    \see      glutVideoResizeGet(), glutSetupVideoResizing(),
              glutStopVideoResizing(), glutVideoResize(), glutVideoPan()
*/
void glutStopVideoResizing(void) {
  __gtkglut_unimplemented_warning("glutStopVideoResizing");
}

/*!
    \brief    Undocumented, unimplemented.
    \ingroup  videomode
    \param    x    A horizontal position.
    \param    y    A vertical position.
    \param    w    A width.
    \param    h    A height.

              This function is undocumented.  This function is
              unimplemeneted (presently).

              This function appears to allow setting the video
              display rectangle to a sub-rectangle of the
              hardware buffer.  \a x and \a y are apparently the
              upper left corner, and \a w and \a h are the width
              and height of the rectangle.

              It is not clear what should be done if the values
              are invalid (negative \a x, for example).

    \see      glutVideoResizeGet(), glutSetupVideoResizing(),
              glutStopVideoResizing(), glutVideoResize(), glutVideoPan()
*/
void glutVideoResize(int x,int y,int w,int h) {
  __gtkglut_unimplemented_warning("glutVideoResize");
}

/*!
    \brief    Undocumented, unimplemented.
    \ingroup  videomode
    \param    x    A horizontal position.
    \param    y    A vertical position.
    \param    w    A width.
    \param    h    A height.

              This function is undocumented.  This function is
              unimplemeneted (presently).

              Appears to be for moving the video display about
              (panning) in a much larger graphic area than
              will fit on the screen at current resolution.

    \see      glutVideoResizeGet(), glutSetupVideoResizing(),
              glutStopVideoResizing(), glutVideoResize(), glutVideoPan()
*/
void glutVideoPan(int x,int y,int w,int h) {
  __gtkglut_unimplemented_warning("glutVideoPan");
}

/*Index color suppport*/
/*! \defgroup colormap Colormap Management
*/

/*!
    \brief    Sets an indexed color-mode entry.
    \ingroup  colormap
    \param    cell    The palette entry to change.
    \param    red       New red value for palette entry.
    \param    green     New green value for palette entry.
    \param    blue      New blue value for palette entry.

              glutSetCursor() allows you to set individual color-map entries
              in a \a GLUT_INDEX type of display.  Respects the
              current overlay setting.

    \note      Unimplemented.
    \see      glutGetColor(), glutCopyColorMap()
*/
void glutSetColor(int cell,GLfloat red, GLfloat green, GLfloat blue) {
  __gtkglut_unimplemented_warning("glutSetColor");
}

/*!
    \brief    Gets an indexed color-mode entry's Red, Green, or Blue value.
    \ingroup  colormap
    \param    cell     The palette entry to fetch.
    \param    component Whether to fetch Red, Green, or Blue.

              Allows you to get individual color-map entries
              in a \a GLUT_INDEX type of display.  Respects the
              current layer setting.

              \a component may be any of:

               - \a GLUT_RED
               - \a GLUT_GREEN
               - \a GLUT_BLUE

    \note      Unimplemented.
    \see      glutSetColor(), glutCopyColorMap()
*/
GLfloat glutGetColor(int cell, int component) {
  __gtkglut_unimplemented_warning("glutGetColor");
  return -1.0;
}

/*!
    \brief    Copies a color map between windows.
    \ingroup  colormap
    \param    win    The window to copy <i>to</i>

              Allows you to copy an entire color map from
              one window to another.  This function copies
              <b>from</b> the <i>current window</i>.  It
              copies <b>to</b> the indicated \a window.
              Respects the current layer setting.

    \note      Unimplemented.
    \see      glutSetColor(), glutGetColor()
*/
void glutCopyColormap(int win) {
  __gtkglut_unimplemented_warning("glutCopyColormap");
}
