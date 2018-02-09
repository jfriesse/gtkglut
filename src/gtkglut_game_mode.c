/*!
    \file  gtkglut_game_mode.c
    \brief The game mode handling code..
*/

/*
 * The game mode handling code..
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
 */

#include "GL/gtkglut.h"
#include "gtkglut_internal.h"

/*! \defgroup gamemode Game Mode

    Game mode provides an exclusive full-screen mode
    of operation for applications such as games.

    Game mode suffers from some serious problems, however.

    \note     In GtkGLUT, this functions are unimplemented.
    \todo     Implement
    \todo
       Replacing game mode with a better
       design was discussed at some length in early 2004.

       One idea was to decompose gamemode into component parts
       and implement them separately, where each part then either
       succeeds totally or fails totally:

       - Resolution change.
       - Fullscreen.
       - Focus grabbing.
       - Settable video frequencies at current resolution.

       Applications could ask for any or all of these separately.
       Some of this is already apparently intended in unimplemented
       API entry points inherited from freeglut.  However, the
       meaning of the entry points is undocumented.

*/

/*!
    \brief    Set the game mode display string
    \ingroup  gamemode
    \param    string    A configuration parameter as a string.

              Sets the gamemode status according to an undocumented string.

              Glancing at old GLUT 3.7, the freeglut codebase that we
              inherited does not implement more than a single
              GLUT gamemode ``criteria''. It may not even do that
              much correctly.

              In principle, this code lets you set the video dimensions,
              rendering depth, and video refresh rate.  In practice,
              the combination that you request may be unsupportable
              and the target host may even refuse to honor any such
              changes.

              If you use this function but do not set all options,
              the following defaults may be substituted for some
              values:

               - \a width 640
               - \a height 480
               - \a depth 16
               - \a refresh 72
    \note     In GtkGLUT, this function is unimplemented.
    \todo     Implement
    \see      glutGameModeString(), glutEnterGameMode(), glutLeaveGameMode(),
              glutGameModeGet()
*/
void  glutGameModeString(const char *string) {
  __gtkglut_unimplemented_warning("glutGameModeString");
}

/*!
    \brief    Enter game mode
    \ingroup  gamemode

              Any combination of the following may apply:

               - Resolution change, possibly to the requested
                 resolution, possibly to a "nearest match".
               - Refresh frequency change.
               - A window with a drawable area equal to the requested
                 screen resolution will be opened.
               - The mouse may be restricted to operate within
                 your window.

    \note     In GtkGLUT, this function is unimplemented.
    \todo     Implement

    \see      glutGameModeString(), glutEnterGameMode(), glutLeaveGameMode(),
              glutGameModeGet()
*/
int  glutEnterGameMode(void) {
  __gtkglut_unimplemented_warning("glutEnterGameMode");

  return FALSE;
}

/*!
    \brief    Leave game mode, returning to normal desktop mode
    \ingroup  gamemode

    \note     In GtkGLUT, this function is unimplemented.
    \todo     Implement
    \see      glutGameModeString(), glutEnterGameMode(), glutLeaveGameMode(),
              glutGameModeGet()
*/
void  glutLeaveGameMode(void){
  __gtkglut_unimplemented_warning("glutLeaveGameMode");
}

/*!
    \brief    Return the value of a game mode parameter
    \ingroup  gamemode
    \param    mode     The parameter value to be returned

              \a pname is one of:

              - \a GLUT_GAME_MODE_ACTIVE \n
                   Return non-zero if we are presently in gamemode.

              - \a GLUT_GAME_MODE_POSSIBLE \n
                   Return whether the requested gamemode settings are viable.
                   (May also actually change the mode?)  Does not necessarily
                   tell you whether entering gamemode will have any effect.

              - \a GLUT_GAME_MODE_WIDTH \n
                   Return the game mode width. (in pixels)

              - \a GLUT_GAME_MODE_HEIGHT \n
                   Return the game mode height. (in pixels)

              - \a GLUT_GAME_MODE_PIXEL_DEPTH \n
                   Return the game mode pixel depth. (in bits)

              - \a GLUT_GAME_MODE_REFRESH_RATE \n
                   Return the game mode vertical refresh frequency. (in Hz)

              - \a GLUT_GAME_MODE_DISPLAY_CHANGED \n
                   Return non-zero if we are presently in gamemode.
                   (Same as \a GLUT_GAME_MODE_ACTIVE.)

              If \a pname is unknown, a warning is printed and a value of
              -1 is returned.

    \note     In GtkGLUT, this function is unimplemented and always return 0.
    \todo     Implement
    \see      glutGameModeString(), glutEnterGameMode(), glutLeaveGameMode(),
              glutGameModeGet()
*/
int glutGameModeGet(GLenum mode) {
  __gtkglut_unimplemented_warning("glutGameModeGet");

  /*From specification, we can always return 0*/
  return 0;
}

