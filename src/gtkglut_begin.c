/*!
    \file  gtkglut_begin.c
    \brief Main loop methods. 
*/ 

/* 
 * GtkGLUT main loop methods.
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

/*! \defgroup mainloop Main Loop
    \brief    GtkGLUT Initialisation and Main Loop

    GtkGLUT's event model is based on callbacks (see the
    callbacks module).  This is inherited from old GLUT in design,
    and from freeglut in implementation.  The model works
    simply by letting you tell GtkGLUT which function of yours
    to call when certain events happen.  Certain events, such as
    the redisplay event for updating your window graphics, can
    be postponed and coalesced so that one redisplay may occur
    in response to multiple hints to GtkGLUT that you require a
    a redisplay.  Some events are manufactured by GtkGLUT, such as
    timers.  Others may be caused by your own request, such
    as redisplays.  Still others may only be caused by external
    stimulus.

    You have two ways to enter GtkGLUT's event processing
    model.  One is via glutMainLoop(), and the other is via
    glutMainLoopEvent().

    When GtkGLUT has dispatched all pending events, it will do
    one of three things:

     - Sleep.  This is the default.  It allows GtkGLUT to
       conserve your computer's resources, freeing up the CPU
       for other tasks or even allowing your OS to slow the CPU
       down when usage falls off.
     - Call your "idle" function (see glutIdleFunc()).  This
       allows you to specify some activity to be performed when
       your application is doing nothing else.  If you register
       an idle function and call glutMainLoop(), your program
       will never rest.
     - Exit the event processing.  This happens if you either
       called glutMainLoopEvent() instead of glutMainLoop()
       (glutMainLoopEvent() resolves all pending events, then
        returns to you),
       or if certain specific events occur, such as glutLeaveMainLoop().

    Old GLUT did not offer glutMainLoopEvent() nor glutLeaveMainLoop(),
    and would only exit glutMainLoop() if the user closed one of
    your windows or if you called exit().  In no case would old GLUT
    return control to your code by simply returning from glutMainLoop().

    freeglut added some mechanisms to control this.  One is an
    explicit glutLeaveMainLoop().  Another is a mode of execution
    that tells GtkGLUT what to do if a user closes one of your
    windows.

    GtkGLUT inherited freeglut's variations.

    ---



*/

/*!
    \brief    The standard GLUT event loop entry point.
    \ingroup  mainloop

              This is the main driving force for an event-driven
              GtkGLUT program.  It alternates between calling
              glutMainLoopEvent() to process pending events and then
              either sleeping or calling your idle function
              (see glutIdleFunc()).

              This function <i>can</i> return, but GLUT's version
              of this function never returned.  And you must
              do special things to GtkGLUT to cause GtkGLUT's
              version to return.

              The cross-reference section for this function's
              documentation should ideally contain every
              callback, but the list would be tediously long and
              prone to omissions.
   \see      glutMainLoopEvent(), glutLeaveMainLoop(), glutIdleFunc()
*/
void glutMainLoop(void) {
  __gtkglut_test_inicialization("glutMainLoop");

  if (__gtkglut_context->no_active_toplevel_windows<1) {
    /*User don't create any window*/

    if (__gtkglut_context->action_on_window_close==GLUT_ACTION_EXIT) {
      /*And set action_on_close to exit -> exit*/
      exit(0);
    }
  } else {
    gtk_main();
  }
}

/*!
    \brief    Dispatches all pending events.
    \ingroup  mainloop

              The general outline of this function is to first drain
              the queue of windowsystem events, in most cases dispatching
              each as it is found.  After the queue is empty, we check
              for timer-based events, coalesced window events (e.g.,
              redisplays), and windows that need to be closed.

              The cross-reference section for this function's
              documentation should ideally contain every
              callback, but the list would be tediously long and
              prone to omissions.

    \note     Does not necessarily dispatch events that are received
              <i>after</i> this function starts processing.
    \note     At first glance, this function may not seem to afford any
              new capability that you couldn't get with an idle callback
              or glutLeaveMainLoop().  However there are other GLUT-like
              libraries that may have their own window event processing
              loops.  Having glutMainLoopEvent() allows you to ask
              GtkGLUT to do its work in a batch, then return to whatever
              processing the other library (or libraries) require.
    \see      glutIdleFunc(), glutLeaveMainLoop(), glutMainLoop()
*/
void glutMainLoopEvent(void) {
  __gtkglut_test_inicialization("glutMainLoopEvent");

  gtk_main_iteration_do(FALSE);
}


/*!
    \brief    Breaks out of GtkGLUT's glutMainLoop()
    \ingroup  mainloop

              This function allows you to unilaterally tell GtkGLUT
              that you are done and wish to exit.  This is useful if
              you have also told GtkGLUT to return to you rather than
              to call exit() directly.

    \see      glutMainLoop(), exit()
*/
void glutLeaveMainLoop(void) {
  __gtkglut_test_inicialization("glutLeaveMainLoop");

  gtk_main_quit();
}

