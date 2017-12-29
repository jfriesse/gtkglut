/*!
 * \file  gtkglut_callback_reg.c
 * \brief Callbacks registration functions.
 */

/*
 * Callbacks registration functions
 *
 * Copyright (c) 2008-2017 Jan Friesse. All Rights Reserved.
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

/*!
 * \defgroup idletimer Idle and Timer Callbacks
 */

/*
 * GTK handler for idle callback
 */
gboolean __gtkglut_callback_idle_handler(gpointer data)
{
	if (__gtkglut_context->callback_idle)
		__gtkglut_context->callback_idle();

	return TRUE;
}

/*!
 * \brief    Sets the global idle callback.
 * \ingroup  idletimer
 * \param    func    Client function for idle event.
 *
 * When GtkGLUT's glutMainLoop() is doing nothing else,
 * it checks to see if an ``idle'' callback set.
 * If so, GtkGLUT invokes that callback.
 *
 * This callback is *not* bound to any window.
 *
 * \note There is at most **one** idle callback for your entire
 *       application.
 *
 * \see glutTimerFunc(), glutMainLoop(), glutMainLoopEvent()
 */
void glutIdleFunc(void (*func) (void))
{
	__gtkglut_test_inicialization("glutIdleFunc");

	if (__gtkglut_context->callback_idle) {
		if (!g_idle_remove_by_data((void *)__gtkglut_context->callback_idle)) {
			__gtkglut_error("glutIdleFunc",
					"Internal error. Cannot remove idle handler!");
		}
	}

	__gtkglut_context->callback_idle = func;

	if (func)
		g_idle_add(__gtkglut_callback_idle_handler, (void *)func);
}

/*
 * Internal handler of GTK timer events.
 */
gboolean __gtkglut_callback_timer_handler(gpointer data)
{
	__gtkglut_timer_callback_struct *timerData;

	timerData = (__gtkglut_timer_callback_struct *) data;

	timerData->func(timerData->value);

	free(timerData);

	return FALSE;
}

/*!
 * \brief    Sets the Timer callback for the current window.
 * \ingroup  idletimer
 * \param    msecs        Milliseconds till invocation.
 * \param    func    Client function for timer event.
 * \param    value        Arbitrary data; passed to \a callback .
 *
 * After *at least* \a msec milliseconds,
 * GtkGLUT will call \a callback , passing in your
 * user-supplied \a data parameter.  GtkGLUT will
 * call your function only once.
 *
 * This callback is **not** bound to any window.
 *
 * \note Unlike most other callbacks, timers only occur once.
 *
 * \note Unlike most other callbacks, you cannot deregister a
 *       timer callback.
 *
 * \note Unlike most other callbacks, you can register an
 *       arbitrary number of timers.
 *
 * \see glutIdleFunc(), glutMainLoop(), glutMainLoopEvent()
 */
void glutTimerFunc(unsigned int msecs, void (*func) (int value), int value)
{
	__gtkglut_timer_callback_struct *timerData;

	__gtkglut_test_inicialization("glutTimerFunc");

	if (func == NULL) {
		__gtkglut_warning("glutTimerFunc", "Registering NULL callback doesn't make sense.");
	} else {
		timerData = (__gtkglut_timer_callback_struct *)
		    malloc(sizeof(__gtkglut_timer_callback_struct));

		if (timerData == NULL)
			__gtkglut_lowmem();

		timerData->value = value;
		timerData->func = func;
		g_timeout_add(msecs, __gtkglut_callback_timer_handler, timerData);
	}
}

/*!
 * \defgroup windowcallback Window Callbacks
 *
 * GtkGLUT's event processing centers around the concept of callbacks.
 * A callback is a function to be invoked by GtkGLUT when GtkGLUT
 * processes a suitable event.  Many callbacks are associated with
 * windows because most GtkGLUT events come from user-interaction.
 * If a callback is associated with a window, then you can register
 * different callbacks for different windows, and GtkGLUT will set
 * the *current window* to the appropriate window before invoking
 * the callback.
 *
 * If you do not set a callback, GtkGLUT will ignore the
 * corresponding event.  If you have registered a callback and later
 * wish to restore GtkGLUT's default behavior, you can set the callback
 * to NULL.  Exceptions to these rules are noted on a case-by-case basis.
 *
 * If an event is not bound to a window, the status of the
 * *current window* setting is undefined.  If such a callback
 * needs to access an GtkGLUT window, it must first use
 * glutSetWindow() to set the *current window*.  Analogous
 * concerns hold for the *current menu*.
 */

/*!
 * \brief    Sets the Display callback for the current window.
 * \ingroup  windowcallback
 * \param    func    Client function for normal redisplay event.
 *
 * Sets the display callback for the *current window*.
 * All windows, including subwindows, *must* have
 * a display callback registered.  GtkGLUT will call
 * the \a callback function whenever it thinks that the
 * window may require updating.
 *
 * This callback is bound to the *current window*.
 *
 * \note Unlike most callbacks, it is illegal to try to disable
 *       the display callback by setting it to \a NULL .
 *
 * \note Multiple redisplays may be coalesced into a single
 *       event for invoking the \a callback only once.
 *
 * \see glutPostRedisplay(), glutOverlayDisplayFunc()
 */
void glutDisplayFunc(void (*func) (void))
{
	__gtkglut_window_struct *glut_window;

	__gtkglut_test_inicialization("glutDisplayFunc");

	if (!func) {
		__gtkglut_error("glutDisplayFunc", "Can't display NULL function as callback!");
	}

	glut_window = __gtkglut_get_active_window_with_warning("glutDisplayFunc");

	if (glut_window) {
		glut_window->callback_display = func;
	}
}

/*!
 * \brief    Sets the Reshape callback for the current window.
 * \ingroup  windowcallback
 * \param    func    Client function for reshape-window event.
 *
 * This registers a function with GtkGLUT, which GtkGLUT
 * will invoke whenever the window is reshaped or
 * resized.  Your callback is only invoked when the host
 * window system has actually changed the window size.
 *
 * The parameters to your callback are the new width and
 * height for your window.
 *
 * If you do not provide a reshape callback, GtkGLUT
 * will simply call glViewport(0,0,\a w,\a h).
 *
 * This callback is bound to the *current window*.
 *
 * To ask GtkGLUT about the present dimensions of the
 * *current window*, you can use glutGet().
 *
 * \note Unlike other callbacks, GLUT has an *active*
 *       default behavior if you do not set this.   (Most
 *       event types passively do nothing if you do not
 *       specify a callback to handle them.)
 * \note The reshape callback should always be called, if
 *       registered, when your window is first created.
 * \see glutGet(), glutReshapeWindow()
 */
void glutReshapeFunc(void (*func) (int width, int height))
{
	__gtkglut_window_struct *glut_window;

	__gtkglut_test_inicialization("glutReshapeFunc");

	glut_window = __gtkglut_get_active_window_with_warning("glutReshapeFunc");

	if (glut_window) {
		glut_window->callback_reshape = func;
	}
}

/*
 * Internal registration function for callbacks glutVisibilityFunc and glutWindowStatusFunc.
 */
void __gtkglut_state_func(char *func_name, void (*func) (int state), gboolean status_event)
{
	__gtkglut_window_struct *glut_window;

	__gtkglut_test_inicialization(func_name);

	glut_window = __gtkglut_get_active_window_with_warning(func_name);

	if (glut_window) {
		if (func) {
			/*User want register callback */
			if (!glut_window->signal_visibility.event_added) {
				glut_window->signal_visibility.event_added =
				    glut_window->signal_status.event_added = TRUE;
				gtk_widget_add_events(glut_window->drawing_area,
						      GDK_VISIBILITY_NOTIFY_MASK);
			}

			if (!glut_window->signal_visibility.glut_callback_handler
			    && !glut_window->signal_status.glut_callback_handler) {
				/*Noone have registered callback, register */
				glut_window->signal_visibility.signal_id =
				    g_signal_connect(G_OBJECT
						     (glut_window->drawing_area),
						     "visibility_notify_event",
						     G_CALLBACK
						     (glut_window->signal_visibility.
						      gtk_event_handler),
						     GINT_TO_POINTER(glutGetWindow()));

				glut_window->signal_status.signal_id =
				    glut_window->signal_visibility.signal_id;
			}

			if (status_event)
				glut_window->window_visible = __GTKGLUT_UNDEFINED;
			else
				glut_window->window_status = __GTKGLUT_UNDEFINED;
		} else {
			if ((status_event && glut_window->signal_status.glut_callback_handler
			    && !glut_window->signal_visibility.glut_callback_handler)
			    || (!status_event && !glut_window->signal_status.glut_callback_handler
				&& glut_window->signal_visibility.glut_callback_handler)) {
				/*We can deregister */
				g_signal_handler_disconnect(G_OBJECT
							    (glut_window->drawing_area),
							    glut_window->signal_visibility.
							    signal_id);
			}
		}

		if (!status_event)
			glut_window->signal_visibility.glut_callback_handler = (void *)func;
		else
			glut_window->signal_status.glut_callback_handler = (void *)func;
	}
}

/*!
 * \defgroup compatability GLUT Compatability API
 */

/*!
 * \brief    Sets the Visibility callback for the current window.
 * \ingroup  compatability
 * \param    func    Client hook for visibility changes.
 *
 * GtkGLUT may call this function when your window's
 * visbility status has changed.  \a status can take
 * on two values: \a GLUT_NOT_VISIBLE or \a GLUT_VISIBLE .
 * If any pixel of your window (including descendants) is
 * visible, your window is \a GLUT_VISIBLE .
 *
 * The callback is bound to the *current window*.
 *
 * \note This is not a polling mechanism.  You are only informed
 *       of transitions that GtkGLUT observes while your
 *      callback is in place.
 * \note This function is superceded by glutWindowStatusFunc().
 * \note This callback is mutually exclusive of glutWindowStatusFunc().
 * \see glutWindowStatusFunc()
 */
void glutVisibilityFunc(void (*func) (int state))
{
	__gtkglut_state_func("glutVisibilityFunc", func, FALSE);
}

/*!
 * \brief    Sets the window status callback.
 * \ingroup  windowcallback
 * \param    func    Client window status hook.
 *
 * When the visibility status of your window changes,
 * GtkGLUT either invokes the \a callback registered
 * by this function or the
 * Visibility \a callback---or neither, if you have
 * not registered either callback.
 *
 * This differs from glutVisbilityFunc() in that the callback
 * has three states, rather than two, that it may receive.
 * These states are:
 *
 * - \a GLUT_VISIBLE (every pixel visible)
 * - \a GLUT_FULLY_COVERED (every pixel obscured)
 * - \a GLUT_HIDDEN  (intermediate)
 *
 * The callback is bound to the *current window*.
 *
 * \note Supercedes glutVisibilityFunc()
 * \see glutVisibilityFunc()
 */
void glutWindowStatusFunc(void (*func) (int state))
{
	__gtkglut_state_func("glutWindowStatusFunc", func, TRUE);
}

/*
 * Function register callback. This is used by glut(Keyboard|Special)[Up]Func.
 * It's little messy, but I hope works well.
 */
void __gtkglut_keyboard_func(char *func_name, gboolean up, gboolean special, void *func)
{
	__gtkglut_window_struct *glut_window;

	__gtkglut_test_inicialization(func_name);

	glut_window = __gtkglut_get_active_window_with_warning(func_name);

	if (glut_window) {
		/*We have window */
		if (func) {
			/*User want register some function */
			if (up) {
				/*We registering UP function */

				/*
				 * Test, if event is added. This is only for first time.
				 * SpecialUP and KeyboardUP share one variable
				 */
				if (!glut_window->signal_keyboard_up.event_added) {
					/*Register event for key release */
					glut_window->signal_keyboard_up.event_added =
					    glut_window->signal_keyboard_special_up.event_added =
					    TRUE;
					gtk_widget_add_events(glut_window->drawing_area,
							      GDK_KEY_RELEASE_MASK);
				}

				/*If no one have registered function, we must register it */
				if (!glut_window->signal_keyboard_up.glut_callback_handler
				    && !glut_window->signal_keyboard_special_up.
				    glut_callback_handler) {
					/*We must register callback */
					glut_window->signal_keyboard_up.signal_id =
					    g_signal_connect(G_OBJECT
							     (glut_window->drawing_area),
							     "key-release-event",
							     G_CALLBACK
							     (glut_window->signal_keyboard_up.
							      gtk_event_handler),
							     GINT_TO_POINTER(glutGetWindow()));
				}
			} else {
				/*We registering NOT UP Function */

				/*Same as for UP */
				if (!glut_window->signal_keyboard.event_added) {
					/*Register event for key press */
					glut_window->signal_keyboard.event_added =
					    glut_window->signal_keyboard_special.event_added = TRUE;
					gtk_widget_add_events(glut_window->drawing_area,
							      GDK_KEY_PRESS_MASK);
				}

				/*Same as for UP */
				if (!glut_window->signal_keyboard.glut_callback_handler
				    && !glut_window->signal_keyboard_special.
				    glut_callback_handler) {
					/*We must register callback */
					glut_window->signal_keyboard.signal_id =
					    g_signal_connect(G_OBJECT
							     (glut_window->drawing_area),
							     "key-press-event",
							     G_CALLBACK
							     (glut_window->signal_keyboard.
							      gtk_event_handler),
							     GINT_TO_POINTER(glutGetWindow()));
				}
			}
		} else {
			/*We deregistering function */
			if (up) {
				/*We deregistering UP function */

				/*Test, if there is nobody, who cares on signal */
				if ((special
				     && !glut_window->signal_keyboard_up.glut_callback_handler
				     && glut_window->signal_keyboard_special_up.
				     glut_callback_handler) || (!special
								&& glut_window->signal_keyboard_up.
								glut_callback_handler
								&& !glut_window->
								signal_keyboard_special_up.
								glut_callback_handler)) {
					g_signal_handler_disconnect(G_OBJECT
								    (glut_window->drawing_area),
								    glut_window->signal_keyboard_up.
								    signal_id);
				}
			} else {
				/*We deregistering NOT UP function */

				/*Test, if there is nobody, who cares on signal */
				if ((special
				     && glut_window->signal_keyboard_special.glut_callback_handler
				     && !glut_window->signal_keyboard.glut_callback_handler)
				    || (!special
					&&
					!glut_window->signal_keyboard_special.glut_callback_handler
					&& glut_window->signal_keyboard.glut_callback_handler)) {
					g_signal_handler_disconnect(G_OBJECT
								    (glut_window->drawing_area),
								    glut_window->signal_keyboard.
								    signal_id);
				}
			}
		}
	}

	if (up && special)
		glut_window->signal_keyboard_special_up.glut_callback_handler = func;
	if (up && !special)
		glut_window->signal_keyboard_up.glut_callback_handler = func;
	if (!up && special)
		glut_window->signal_keyboard_special.glut_callback_handler = func;
	if (!up && !special)
		glut_window->signal_keyboard.glut_callback_handler = func;
}

/*!
 * \defgroup input Input Callbacks
 */

/*!
 * \brief    Sets the Keyboard callback for the current window.
 * \ingroup  input
 * \param    func    Client function for keyboard event.
 *
 * This callback registration allows you to handle
 * traditional ASCII keyboard input.  A general rule of
 * thumb is that if a key has a common ASCII code,
 * then GtkGLUT assigns that code to the key
 * and calls the Keyboard \a callback with the ASCII code in
 * the \a key parameter.
 * For other keys, you must use glutSpecialFunc().
 * Not all keys can be reported by GtkGLUT.
 *
 * As a convenience, the mouse coordinates, relative
 * to your window, are also returned.
 *
 * This callback is bound to the *current window*.
 *
 * \note This function is not very international-friendly.
 * \see glutKeyboardUpFunc(), glutSpecialFunc()
 */
void glutKeyboardFunc(void (*func) (unsigned char key, int x, int y))
{
	__gtkglut_keyboard_func("glutKeyboardFunc", FALSE, FALSE, (void *)func);
}

/*!
 * \brief    Sets the keyboard key release callback for the current window.
 * \ingroup  input
 * \param    func    Client hook for ASCII key releases.
 *
 * This function provides a way to detect the release of
 * a keyboard key.
 * The keys are reported exactly as with
 * glutKeyboardFunc(), save that the \a callback registered
 * via this function is used to report the event.
 *
 * This callback is bound to the *current window*.
 *
 * \see glutKeyboardFunc(), glutSpecialUpFunc()
 */
void glutKeyboardUpFunc(void (*func) (unsigned char key, int x, int y))
{
	__gtkglut_keyboard_func("glutKeyboardUpFunc", TRUE, FALSE, (void *)func);
}

/*!
 * \brief    Sets the Special callback for the current window
 * \ingroup  input
 * \param    func    Client function for keyboard event.
 *
 * Registers a \a callback for GtkGLUT to call
 * when the user presses "special" keys on the keyboard.
 *
 * The special callback handles some additional keys that
 * are not covered under plain "keyboard" events.
 * The \a key that is passed to the \a callback is one of an
 * enumerated set.
 * The association to keys on your keyboard should
 * be obvious.  Their GLUT symbol names are:
 *
 * - \a GLUT_KEY_F1
 * - \a GLUT_KEY_F2
 * - \a GLUT_KEY_F3
 * - \a GLUT_KEY_F4
 * - \a GLUT_KEY_F5
 * - \a GLUT_KEY_F6
 * - \a GLUT_KEY_F7
 * - \a GLUT_KEY_F8
 * - \a GLUT_KEY_F9
 * - \a GLUT_KEY_F10
 * - \a GLUT_KEY_F11
 * - \a GLUT_KEY_F12
 * - \a GLUT_KEY_LEFT
 * - \a GLUT_KEY_UP
 * - \a GLUT_KEY_RIGHT
 * - \a GLUT_KEY_DOWN
 * - \a GLUT_KEY_PAGE_UP
 * - \a GLUT_KEY_PAGE_DOWN
 * - \a GLUT_KEY_HOME
 * - \a GLUT_KEY_END
 * - \a GLUT_KEY_INSERT
 *
 * To receive other keys, see glutKeyboardFunc().
 *
 * This callback is bound to the *current window*.
 *
 * \see glutSpecialUpFunc(), glutKeyboardFunc()
 */
void glutSpecialFunc(void (*func) (int key, int x, int y))
{
	__gtkglut_keyboard_func("glutSpecialFunc", FALSE, TRUE, (void *)func);
}

/*!
 * \brief    Sets the special key release callback for the current window
 * \ingroup  input
 * \param    func    Client hook for special key releases.
 *
 * This function provides a way to detect the release of
 * a keyboard \a key.
 * The keys are reported exactly as with
 * glutSpecialFunc(), save that the \a callback registered
 * via this function is used to report the event.
 *
 * This callback is bound to the *current window*.
 *
 * \see glutSpecialFunc(), glutKeyboardUpFunc()
 */
void glutSpecialUpFunc(void (*func) (int key, int x, int y))
{
	__gtkglut_keyboard_func("glutSpecialUpFunc", TRUE, TRUE, (void *)func);
}

/*
 * Mouse button registration
 */
void __gtkglut_register_mouse_func(__gtkglut_window_struct * glut_window)
{
	int i;
	gboolean signal_added;

	/*User want register callback */
	if (!glut_window->signal_mouse_press.event_added) {
		glut_window->signal_mouse_press.event_added =
		    glut_window->signal_mouse_release.event_added =
		    glut_window->signal_mouse_wheel.event_added = TRUE;

		gtk_widget_add_events(glut_window->drawing_area,
				      GDK_BUTTON_PRESS_MASK | GDK_BUTTON_RELEASE_MASK);
	}

	/*Signal is active, if ... */
	signal_added = FALSE;

	/*... we have glut_window registered... */
	if (glut_window->signal_mouse_press.glut_callback_handler)
		signal_added = TRUE;
	/*... or some mouse button for window is active */
	for (i = 0; i < __GTKGLUT_MAX_MOUSE_BUTTONS; i++)
		if (glut_window->mouse_button_menu[i])
			signal_added = TRUE;

	if (!signal_added) {
/*    printf("MOUSE register signal!\n");*/
		glut_window->signal_mouse_press.signal_id =
		    g_signal_connect(G_OBJECT(glut_window->drawing_area),
				     "button_press_event",
				     G_CALLBACK(glut_window->signal_mouse_press.gtk_event_handler),
				     GINT_TO_POINTER(glutGetWindow()));
		glut_window->signal_mouse_release.signal_id =
		    g_signal_connect(G_OBJECT(glut_window->drawing_area),
				     "button_release_event",
				     G_CALLBACK
				     (glut_window->signal_mouse_release.gtk_event_handler),
				     GINT_TO_POINTER(glutGetWindow()));
	}
}

/*
 * Mouse button unregistration
 */
void __gtkglut_unregister_mouse_func(__gtkglut_window_struct * glut_window, int menu_button)
{
	int i;
	gboolean signal_unregister;

	signal_unregister = TRUE;

	/*User want unregister menu_button */

	/*If other button than menu_button is registered, we can't unregister anything */
	for (i = 0; i < __GTKGLUT_MAX_MOUSE_BUTTONS; i++)
		if (glut_window->mouse_button_menu[i] && i != menu_button)
			signal_unregister = FALSE;

	/*If user have still explicit handler, we can't unregister anything */
	if (menu_button != __GTKGLUT_UNDEFINED
	    && glut_window->signal_mouse_press.glut_callback_handler)
		signal_unregister = FALSE;

	if (signal_unregister) {
/*    printf("MOUSE Unregister signal!\n");*/
		/*User want unregister callback */
		g_signal_handler_disconnect(G_OBJECT(glut_window->drawing_area),
					    glut_window->signal_mouse_press.signal_id);
		g_signal_handler_disconnect(G_OBJECT(glut_window->drawing_area),
					    glut_window->signal_mouse_release.signal_id);
	}
}

/*!
 * \brief    Sets the mouse-button callback for the current window.
 * \ingroup  input
 * \param    func    Client hook for mouse-buttons.
 *
 * Whenever a mouse button is pressed or released in an GtkGLUT
 * window, GtkGLUT checks if that window has a mouse-button
 * (Mouse) callback registered.  If so, GtkGLUT gives the
 * event to the handler.  \a button is the button number,
 * starting from 0.  \a state is \a GLUT_UP or \a GLUT_DOWN
 * to indicate the button's new state.  The other parameters
 * are the mouse coordinates.
 *
 * Mouse wheel motion can be reported as buttons.  If you
 * do not request otherwise, a wheel spun forward will
 * act like a button clicking down, immediately followed
 * by clicking up.  Spinning the same wheel backward
 * will act like a different button clicking.  Mouse wheel
 * pseudo-buttons are added after all real buttons.
 *
 * While the button is held and the mouse is dragged,
 * you receive mouse-motion events (glutMotionFunc()),
 * even if the mouse is dragged out of the window.
 *
 * This callback is bound to the *current window*.
 *
 * \note Old GLUT defines the symbols \a GLUT_LEFT_BUTTON,
 *       \a GLUT_RIGHT_BUTTON, and \a GLUT_MIDDLE_BUTTON.
 *       However, mice can have more than 3 buttons.
 *       In GtkGLUT you have \a GLUT_FOURTH_BUTTON and \a GLUT_FIFTH_BUTTON
 *       buttons defined.
 *
 * \see glutMotionFunc(), glutPassiveMotionFunc(), glutMouseWheelFunc()
 */
void glutMouseFunc(void (*func) (int button, int state, int x, int y))
{
	__gtkglut_window_struct *glut_window;

	__gtkglut_test_inicialization("glutMouseFunc");

	glut_window = __gtkglut_get_active_window_with_warning("glutMouseFunc");

	if (glut_window) {
		if (glut_window->signal_mouse_press.glut_callback_handler) {
			/*Some callback is registered */
			if (!func) {
				__gtkglut_unregister_mouse_func(glut_window, __GTKGLUT_UNDEFINED);
			}
		} else {
			if (func) {
				/*User want register callback */
				__gtkglut_register_mouse_func(glut_window);
			}
		}

		glut_window->signal_mouse_press.glut_callback_handler = (void *)func;
		glut_window->signal_mouse_release.glut_callback_handler = (void *)func;
	}
}

/*!
 * \brief    Sets the mouse wheel callback for the current window.
 * \ingroup  input
 * \param    func    Client hook for mouse wheel events.
 *
 * If the mouse wheel is spun over your (sub)window,
 * GtkGLUT will, in theory, report this via the MouseWheel
 * callback.  \a wheel is the wheel number, \a direction
 * is +/- 1 (\a GLUT_WHEEL_UP and \a GLUT_WHEEL_DOWN), and \a x and \a y are the mouse coordinates.
 *
 * If you do not register a wheel callback, wheel events will
 * be reported as mouse buttons.
 *
 * This callback is bound to the *current window*.
 *
 * \see glutMouseFunc()
 */
void glutMouseWheelFunc(void (*func) (int wheel, int direction, int x, int y))
{
	__gtkglut_window_struct *glut_window;

	__gtkglut_test_inicialization("glutMouseWheelFunc");

	glut_window = __gtkglut_get_active_window_with_warning("glutMouseWheelFunc");

	if (glut_window) {
		if (glut_window->signal_mouse_wheel.glut_callback_handler) {
			/*Some callback is registered */
			if (!func) {
				/*User want unregister callback */
				g_signal_handler_disconnect(G_OBJECT
							    (glut_window->drawing_area),
							    glut_window->signal_mouse_wheel.
							    signal_id);
			}
		} else {
			if (func) {
				/*User want register callback */
				if (!glut_window->signal_mouse_wheel.event_added) {
					glut_window->signal_mouse_press.event_added =
					    glut_window->signal_mouse_release.event_added =
					    glut_window->signal_mouse_wheel.event_added = TRUE;

					gtk_widget_add_events(glut_window->drawing_area,
							      GDK_BUTTON_PRESS_MASK
							      | GDK_BUTTON_RELEASE_MASK);
				}
				glut_window->signal_mouse_wheel.signal_id =
				    g_signal_connect(G_OBJECT
						     (glut_window->drawing_area),
						     "scroll_event",
						     G_CALLBACK
						     (glut_window->signal_mouse_wheel.
						      gtk_event_handler),
						     GINT_TO_POINTER(glutGetWindow()));
			}
		}

		glut_window->signal_mouse_wheel.glut_callback_handler = (void *)func;
	}
}

/*
 * Internal registration of mouse motion (passice and active). Used by lutMotionFunc and
 * glutPassiveMotionFunc
 */
void __gtkglut_mouse_motion_func(char *func_name, void *func, gboolean passive)
{
	__gtkglut_window_struct *glut_window;

	__gtkglut_test_inicialization(func_name);

	glut_window = __gtkglut_get_active_window_with_warning(func_name);

	if (glut_window) {
		if (!func) {
			/*User want unregister callback */
			if ((passive && glut_window->signal_passive_motion.glut_callback_handler
			     && !glut_window->signal_motion.glut_callback_handler) ||
			     (!passive && !glut_window->signal_passive_motion.glut_callback_handler &&
			     glut_window->signal_motion. glut_callback_handler)) {
				/*We can deregister */
				g_signal_handler_disconnect(G_OBJECT
							    (glut_window->drawing_area),
							    glut_window->signal_motion.signal_id);
			}
		} else {
			/*User want register callback */
			if (!glut_window->signal_passive_motion.event_added
			    && !glut_window->signal_motion.glut_callback_handler) {
				glut_window->signal_passive_motion.event_added =
				    glut_window->signal_motion.event_added = TRUE;

				gtk_widget_add_events(glut_window->drawing_area,
						      GDK_POINTER_MOTION_MASK);
			}

			if (!glut_window->signal_motion.glut_callback_handler
			    && !glut_window->signal_passive_motion.glut_callback_handler) {
				/*Noone have registered callback, register */
				glut_window->signal_motion.signal_id =
				    g_signal_connect(G_OBJECT
						     (glut_window->drawing_area),
						     "motion_notify_event",
						     G_CALLBACK
						     (glut_window->signal_motion.gtk_event_handler),
						     GINT_TO_POINTER(glutGetWindow()));
				glut_window->signal_passive_motion.signal_id =
				    glut_window->signal_motion.signal_id;
			}
		}

		if (!passive)
			glut_window->signal_motion.glut_callback_handler = func;
		else
			glut_window->signal_passive_motion.glut_callback_handler = func;
	}
}

/*!
 * \brief    Reports mouse-motion while a button is held.
 * \ingroup  input
 * \param    func    Client hook for dragging mouse.
 *
 * This function reports the mouse position when the mouse
 * is dragged starting from within your window.
 * (``Dragging'' occurs when you press one or more mouse
 * buttons in one of your GtkGLUT windows, and then
 * move the mouse around.)
 *
 * This callback is bound to the *current window*.
 *
 * \note Events are reported until the mouse button is released,
 *       even if the mouse leaves the window.
 * \see  glutMouseFunc(), glutPassiveMotion()
 */
void glutMotionFunc(void (*func) (int x, int y))
{
	__gtkglut_mouse_motion_func("glutMotionFunc", (void *)func, FALSE);

}

/*!
 * \brief    Sets the non-dragging (gliding?) mouse-motion callback.
 * \ingroup  input
 * \param    func    Client mouse-glide hook.
 *
 * If you set this callback on a window, then every time that
 * GtkGLUT detects a change in the mouse position inside
 * that window, with no buttons pressed on that mouse,
 * GtkGLUT will invoke \a callback with the window
 * coordinates of the mouse.
 *
 * This callback is bound to the *current window*.
 *
 * \see glutMotionFunc(), glutMouseFunc(), glutEntryFunc()
 */
void glutPassiveMotionFunc(void (*func) (int x, int y))
{
	__gtkglut_mouse_motion_func("glutPassiveMotionFunc", (void *)func, TRUE);
}

/*!
 * \brief    Window mouse entry/leave callback.
 * \ingroup  input
 * \param    func    Client window-entry hook.
 *
 * When the mouse enters or exits a window (or sub-window),
 * GtkGLUT can report this transition via the Entry
 * callback.  \a state is one of \a GLUT_LEFT or
 * \a GLUT_ENTERED respective of whether the mouse
 * left or entered the window.
 *
 * This callback is bound to the *current window*.
 *
 * \see glutPassiveMotionFunc()
 */
void glutEntryFunc(void (*func) (int state))
{
	__gtkglut_window_struct *glut_window;

	__gtkglut_test_inicialization("glutEntryFunc");

	glut_window = __gtkglut_get_active_window_with_warning("glutEntryFunc");

	if (glut_window) {
		if (glut_window->signal_entry.glut_callback_handler) {
			/*Some callback is registered */
			if (!func) {
				/*User want unregister callback */
				g_signal_handler_disconnect(G_OBJECT
							    (glut_window->drawing_area),
							    glut_window->signal_entry.signal_id);
			}
		} else {
			if (func) {
				/*User want register callback */
				if (!glut_window->signal_entry.event_added) {
					glut_window->signal_entry.event_added = TRUE;
					gtk_widget_add_events(glut_window->drawing_area,
							      GDK_LEAVE_NOTIFY_MASK);
				}
				glut_window->signal_entry.signal_id =
				    g_signal_connect(G_OBJECT
						     (glut_window->drawing_area),
						     "leave_notify_event",
						     G_CALLBACK
						     (glut_window->signal_entry.gtk_event_handler),
						     GINT_TO_POINTER(glutGetWindow()));
			}
		}

		glut_window->signal_entry.glut_callback_handler = (void *)func;
	}

}

/*!
 * \defgroup menucallback Menu Callbacks
 */

/*!
 * \brief    Modern variant of glutMenuStateFunc()
 * \ingroup  menucallback
 * \param    func    Client menu status hook.
 *
 * Broadly, GtkGLUT operates in two modes.  At any
 * given time, it is either in menu mode (with a popup
 * menu display, possibly with subitems) or it is
 * not.
 *
 * When moving from non-menu to menu status,
 * \a callback (if defined) will be called with
 * \a GLUT_MENU_IN_USE .  Conversely, when moving
 * from menu to non-menu status,
 * \a callback (if defined) will be called with
 * \a GLUT_MENU_NOT_IN_USE .
 *
 * This differs from glutMenuStateFunc() in that
 * \a callback is also given the \a x and \a y
 * coordinates of the mouse when the menu state
 * transition took place.
 *
 * This callback is bound to **both** the
 * *current window* and the *current menu*.
 *
 * \see glutMenuStateFunc()
 */
void glutMenuStatusFunc(void (*func) (int status, int x, int y))
{
	__gtkglut_test_inicialization("glutMenuStatusFunc");

	__gtkglut_context->menu_status_func = func;
}

/*!
 * \brief    Deprecated variant of glutMenuStatusFunc()
 * \ingroup  compatability
 * \param    func    Client menu status hook.
 *
 * Broadly, GtkGLUT operates in two modes.  At any
 * given time, it is either in menu mode (with a popup
 * menu display, possibly with subitems) or it is
 * not.
 *
 * When moving from non-menu to menu status,
 * \a callback (if defined) will be called with
 * \a GLUT_MENU_IN_USE .  Conversely, when moving
 * from menu to non-menu status,
 * \a callback (if defined) will be called with
 * \a GLUT_MENU_NOT_IN_USE .
 *
 * This callback is bound to **both** the
 * *current window* and the *current menu*.
 *
 * \see glutMenuStatusFunc()
 */
void glutMenuStateFunc(void (*func) (int status))
{
	__gtkglut_test_inicialization("glutMenuStateFunc");

	__gtkglut_context->menu_state_func = func;
}

/*!
 * \brief    Window destruction callback.
 * \ingroup  windowcallback
 * \param    func    Client window destruction hook.
 *
 * When a window is destroyed by user-action in
 * traditional GLUT, the application terminates.
 * In freeglut and GtkGLUT, the application can
 * choose to persist and treat the window close
 * event as a normal event.  This callback is
 * how that event is transmitted to the application.
 *
 * This callback is bound to the *current window*.
 *
 * \see glutDestroyWindow(), glutWMCloseFunc()
 */
void glutCloseFunc(void (*func) (void))
{
	__gtkglut_window_struct *glut_window;

	__gtkglut_test_inicialization("glutCloseFunc");

	glut_window = __gtkglut_get_active_window_with_warning("glutCloseFunc");

	if (glut_window) {
		glut_window->callback_close = func;
	}
}

/*!
 * \brief    Window destruction callback.
 * \ingroup  compatability
 * \param    func    Client window destruction hook.
 *
 * Superceded by glutCloseFunc
 *
 * When a window is destroyed by user-action in
 * traditional GLUT, the application terminates.
 * In freeglut and GtkGLUT, the application can
 * choose to persist and treat the window close
 * event as a normal event.  This callback is
 * how that event is transmitted to the application.
 * This callback is bound to the *current window*.
 *
 * \note This function is **exactly** the same as
 *       glutCloseFunc().  In fact, this function
 *       is just a call to the other function, with the same
 *       parameter.
 *
 * \see glutDestroyWindow(), glutCloseFunc()
 */
void glutWMCloseFunc(void (*func) (void))
{
	glutCloseFunc(func);
}
