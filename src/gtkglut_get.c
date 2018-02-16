/*!
 * \file  gtkglut_get.c
 * \brief GtkGLUT state query methods.
 */

/*
 * GtkGLUT state query methods.
 *
 * Copyright (c) 2008-2018 Jan Friesse. All Rights Reserved.
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

/*
 * Get OpenGL attribute from glut_window gl_config
 */
int __gtkglut_gl_config_attrib(__gtkglut_window_struct * glut_window, int attrib)
{
	GdkGLConfig *gl_config;
	int res;

	res = -1;

	gl_config = gtk_widget_get_gl_config(glut_window->drawing_area);

	if (gl_config) {
		if (!gdk_gl_config_get_attrib(gl_config, attrib, &res)) {
			__gtkglut_warning("__gtkglut_gl_config_attrib",
					  "can't get attrib %d for window %d", glutGetWindow());
		}
	} else {
		__gtkglut_warning("__gtkglut_gl_config_attrib",
				  "can't get valid gl_config for window %d", glutGetWindow());
	}

	return res;
}

/*
 * Generate some format id, which describe visual, returned value:
 *     Visual = 000000000 00000000 0OTTTTTT 00DDDDDD
 *     TTTTTT = Type = GDK_VISUAL_STATIC_GRAY,GDK_VISUAL_GRAYSCALE,...
 *     DDDDDD = Depth in bits (ex. 24 = 24 bits)
 *     O = Byte order ( GDK_LSB_FIRST=0,GDK_MSB_FIRST=1)
 */
int __gtkglut_gl_config_make_format_id(__gtkglut_window_struct * glut_window)
{
	GdkGLConfig *gl_config;
	GdkVisual *visual;
	int res;

	res = -1;

	gl_config = gtk_widget_get_gl_config(glut_window->drawing_area);

	if (gl_config) {
		visual = gdk_gl_config_get_visual(gl_config);

		/*Visual = 000000000 00000000 0OTTTTTT 00DDDDDD
		   TTTTTT = Type = GDK_VISUAL_STATIC_GRAY,GDK_VISUAL_GRAYSCALE,...
		   DDDDDD = Depth in bits (ex. 24 = 24 bits)
		   O = Byte order ( GDK_LSB_FIRST=0,GDK_MSB_FIRST=1)
		 */
		res = (visual->depth & 0x3F) |
		    ((visual->type & 0x3F) << 8) | ((visual->byte_order & 0x1) << 15);
	} else {
		__gtkglut_warning("__gtkglut_gl_config_make_format_id",
		    "can't get valid gl_config for window %d", glutGetWindow());
	}

	return res;

}

/*!
 * \defgroup inputstate Input State
 */

/*!
 * \defgroup state State Query
 *
 * GtkGLUT provides a certain amount of state to track modes
 * of operation as well as status information on various objects
 * such as GtkGLUT windows.  Additionally, there is some static
 * state information about GtkGLUT itself, and system-dependant
 * state about the underlying window system.
 *
 * The state module provides some support for modifying GtkGLUT
 * state, as well as support for querying state.
 */

/*!
 * \brief    Query an GtkGLUT option or state.
 * \ingroup  state
 * \param    state    Enumerated parameter ID.
 *
 * Query for the current value of one of many possible GtkGLUT
 * state variables.  The current list is:
 *
 * - \a GLUT_ACTION_ON_WINDOW_CLOSE \n
 *   Do something other than exit when the
 *   a window is closed via the window manager.
 *
 * - \a GLUT_DISPLAY_MODE_POSSIBLE
 *
 * - \a GLUT_ELAPSED_TIME
 *
 * - \a GLUT_INIT_DISPLAY_MODE
 *
 * - \a GLUT_INIT_STATE
 *
 * - \a GLUT_INIT_WINDOW_HEIGHT
 *
 * - \a GLUT_INIT_WINDOW_WIDTH
 *
 * - \a GLUT_INIT_WINDOW_X
 *
 * - \a GLUT_INIT_WINDOW_Y
 *
 * - \a GLUT_MENU_NUM_ITEMS
 *
 * - \a GLUT_RENDERING_CONTEXT \n
 *      OpenGL contexts shared with new windows.
 *
 * - \a GLUT_SCREEN_HEIGHT \n
 *      Screen height in pixels.
 *
 * - \a GLUT_SCREEN_HEIGHT_MM \n
 *      Screen height in millimeters.
 *
 * - \a GLUT_SCREEN_WIDTH \n
 *      Screen width in pixels.
 *
 * - \a GLUT_SCREEN_WIDTH_MM \n
 *      Screen width in millimeters.
 *
 * - \a GLUT_VERSION \n
 *      Emulate Freeglut
 *
 * - \a GTKGLUT_VERSION \n
 *      Version of GtkGLUT
 *
 * - \a GLUT_WINDOW_ACCUM_ALPHA_SIZE
 *
 * - \a GLUT_WINDOW_ACCUM_BLUE_SIZE
 *
 * - \a GLUT_WINDOW_ACCUM_GREEN_SIZE
 *
 * - \a GLUT_WINDOW_ACCUM_RED_SIZE
 *
 * - \a GLUT_WINDOW_ALPHA_SIZE
 *
 * - \a GLUT_WINDOW_BLUE_SIZE
 *
 * - \a GLUT_WINDOW_BORDER_WIDTH
 *
 * - \a GLUT_WINDOW_BUFFER_SIZE
 *
 * - \a GLUT_WINDOW_COLORMAP_SIZE
 *
 * - \a GLUT_WINDOW_CURSOR
 *
 * - \a GLUT_WINDOW_DEPTH_SIZE
 *
 * - \a GLUT_WINDOW_DOUBLEBUFFER
 *
 * - \a GLUT_WINDOW_FORMAT_ID \n
 *      return generated format of visual. Returned value is \n
 *        Visual = 000000000 00000000 0OTTTTTT 00DDDDDD  \n
 *        TTTTTT = Type = GDK_VISUAL_STATIC_GRAY,GDK_VISUAL_GRAYSCALE,... \n
 *        DDDDDD = Depth in bits (ex. 24 = 24 bits) \n
 *        O = Byte order ( GDK_LSB_FIRST=0,GDK_MSB_FIRST=1)
 *
 * - \a GLUT_WINDOW_GREEN_SIZE
 *
 * - \a GLUT_WINDOW_HEADER_HEIGHT
 *
 * - \a GLUT_WINDOW_HEIGHT
 *
 * - \a GLUT_WINDOW_NUM_CHILDREN
 *
 * - \a GLUT_WINDOW_NUM_SAMPLES
 *
 * - \a GLUT_WINDOW_PARENT
 *
 * - \a GLUT_WINDOW_RED_SIZE
 *
 * - \a GLUT_WINDOW_RGBA
 *
 * - \a GLUT_WINDOW_STENCIL_SIZE
 *
 * - \a GLUT_WINDOW_STEREO
 *
 * - \a GLUT_WINDOW_WIDTH
 *
 * - \a GLUT_WINDOW_X
 *
 * - \a GLUT_WINDOW_Y
 *
 * \see glutSetOption(), glutDeviceGet(), glutGetModifiers(),
 *      glutLayerGet()
 */
int glutGet(GLenum state)
{
	int res;
	__gtkglut_window_struct *glut_window;
	__gtkglut_menu_struct *glut_menu;
	int x, y, width, height;

	if (!__gtkglut_context) {
		__gtkglut_context_init();
	}

	res = -1;

	switch (state) {
	case GLUT_ELAPSED_TIME:
		return (int)(g_timer_elapsed(__gtkglut_context->elapsed_time_timer, NULL) * 1000);
		break;

	case GLUT_INIT_DISPLAY_MODE:
		return __gtkglut_context->display_mode;
		break;

	case GLUT_INIT_WINDOW_HEIGHT:
		return __gtkglut_context->window_init_geometry.height;
		break;

	case GLUT_INIT_WINDOW_WIDTH:
		return __gtkglut_context->window_init_geometry.width;
		break;

	case GLUT_INIT_WINDOW_X:
		return __gtkglut_context->window_init_geometry.x;
		break;

	case GLUT_INIT_WINDOW_Y:
		return __gtkglut_context->window_init_geometry.y;
		break;

	case GLUT_VERSION:
		return 20400;
		break;

	case GTKGLUT_VERSION:
		return __GTKGLUT_MAJOR_VERSION * 10000 + __GTKGLUT_MINOR_VERSION * 100 +
		    __GTKGLUT_MICRO_VERSION;
		break;
	}

	__gtkglut_test_inicialization("glutGet");

	/*
	 * Function, which needs inicialization
	 */
	switch (state) {
	case GLUT_SCREEN_WIDTH:
		res = gdk_screen_get_width(gdk_screen_get_default());
		break;

	case GLUT_SCREEN_HEIGHT:
		res = gdk_screen_get_height(gdk_screen_get_default());
		break;

	case GLUT_SCREEN_WIDTH_MM:
		res = gdk_screen_get_width_mm(gdk_screen_get_default());
		break;

	case GLUT_SCREEN_HEIGHT_MM:
		res = gdk_screen_get_height_mm(gdk_screen_get_default());
		break;
	}

	/*
	 * Function, which needs inicialization and window
	 */
	switch (state) {
	case GLUT_WINDOW_X:
	case GLUT_WINDOW_Y:
	case GLUT_WINDOW_WIDTH:
	case GLUT_WINDOW_HEIGHT:
	case GLUT_WINDOW_BUFFER_SIZE:
	case GLUT_WINDOW_STENCIL_SIZE:
	case GLUT_WINDOW_DEPTH_SIZE:
	case GLUT_WINDOW_RED_SIZE:
	case GLUT_WINDOW_GREEN_SIZE:
	case GLUT_WINDOW_BLUE_SIZE:
	case GLUT_WINDOW_ALPHA_SIZE:
	case GLUT_WINDOW_ACCUM_RED_SIZE:
	case GLUT_WINDOW_ACCUM_GREEN_SIZE:
	case GLUT_WINDOW_ACCUM_BLUE_SIZE:
	case GLUT_WINDOW_ACCUM_ALPHA_SIZE:
	case GLUT_WINDOW_DOUBLEBUFFER:
	case GLUT_WINDOW_RGBA:
	case GLUT_WINDOW_PARENT:
	case GLUT_WINDOW_NUM_CHILDREN:
	case GLUT_WINDOW_COLORMAP_SIZE:
	case GLUT_WINDOW_NUM_SAMPLES:
	case GLUT_WINDOW_STEREO:
	case GLUT_WINDOW_CURSOR:
	case GLUT_WINDOW_FORMAT_ID:
		glut_window = __gtkglut_get_active_window_with_warning("glutGet");

		if (glut_window) {
			if (state == GLUT_WINDOW_X) {
				gdk_window_get_root_origin(glut_window->drawing_area->window, &x,
							   &y);

				res = x;
			}

			if (state == GLUT_WINDOW_Y) {
				gdk_window_get_root_origin(glut_window->drawing_area->window, &x,
							   &y);

				res = y;
			}

			if (state == GLUT_WINDOW_WIDTH) {
				gdk_drawable_get_size(glut_window->drawing_area->window, &width,
						      &height);

				res = width;
			}

			if (state == GLUT_WINDOW_HEIGHT) {
				gdk_drawable_get_size(glut_window->drawing_area->window, &width,
						      &height);

				res = height;
			}

			if (state == GLUT_WINDOW_BUFFER_SIZE)
				res = __gtkglut_gl_config_attrib(glut_window, GDK_GL_BUFFER_SIZE);
			if (state == GLUT_WINDOW_STENCIL_SIZE)
				res = __gtkglut_gl_config_attrib(glut_window, GDK_GL_STENCIL_SIZE);
			if (state == GLUT_WINDOW_DEPTH_SIZE)
				res = __gtkglut_gl_config_attrib(glut_window, GDK_GL_DEPTH_SIZE);
			if (state == GLUT_WINDOW_RED_SIZE)
				res = __gtkglut_gl_config_attrib(glut_window, GDK_GL_RED_SIZE);
			if (state == GLUT_WINDOW_GREEN_SIZE)
				res = __gtkglut_gl_config_attrib(glut_window, GDK_GL_GREEN_SIZE);
			if (state == GLUT_WINDOW_BLUE_SIZE)
				res = __gtkglut_gl_config_attrib(glut_window, GDK_GL_BLUE_SIZE);
			if (state == GLUT_WINDOW_ALPHA_SIZE)
				res = __gtkglut_gl_config_attrib(glut_window, GDK_GL_ALPHA_SIZE);
			if (state == GLUT_WINDOW_ACCUM_RED_SIZE)
				res =
				    __gtkglut_gl_config_attrib(glut_window, GDK_GL_ACCUM_RED_SIZE);
			if (state == GLUT_WINDOW_ACCUM_GREEN_SIZE)
				res =
				    __gtkglut_gl_config_attrib(glut_window,
							       GDK_GL_ACCUM_GREEN_SIZE);
			if (state == GLUT_WINDOW_ACCUM_BLUE_SIZE)
				res =
				    __gtkglut_gl_config_attrib(glut_window, GDK_GL_ACCUM_BLUE_SIZE);
			if (state == GLUT_WINDOW_ACCUM_ALPHA_SIZE)
				res =
				    __gtkglut_gl_config_attrib(glut_window,
							       GDK_GL_ACCUM_ALPHA_SIZE);
			if (state == GLUT_WINDOW_DOUBLEBUFFER)
				res =
				    __gtkglut_gl_config_attrib(glut_window,
							       GDK_GL_DOUBLEBUFFER) ? 1 : 0;
			if (state == GLUT_WINDOW_RGBA)
				res = __gtkglut_gl_config_attrib(glut_window, GDK_GL_RGBA) ? 1 : 0;
			if (state == GLUT_WINDOW_PARENT)
				res =
				    glut_window->parent_window < 0 ? 0 : glut_window->parent_window;
			if (state == GLUT_WINDOW_NUM_CHILDREN)
				res =
				    (glut_window->child_list) ? g_slist_length(glut_window->
									       child_list) : 0;
			if (state == GLUT_WINDOW_COLORMAP_SIZE)
				res = 0;
			if (state == GLUT_WINDOW_NUM_SAMPLES)
				res =
				    __gtkglut_gl_config_attrib(glut_window, GDK_GL_SAMPLE_BUFFERS);
			if (state == GLUT_WINDOW_STEREO)
				res =
				    __gtkglut_gl_config_attrib(glut_window, GDK_GL_STEREO) ? 1 : 0;
			if (state == GLUT_WINDOW_CURSOR)
				res = glut_window->current_cursor;
			if (state == GLUT_WINDOW_FORMAT_ID)
				res = __gtkglut_gl_config_make_format_id(glut_window);
		} else {
			/*
			 * Make test8 happy
			 */
			if (state == GLUT_WINDOW_NUM_CHILDREN)
				res = 0;
		}
		break;
	}

	if (state == GLUT_MENU_NUM_ITEMS) {
		/*
		 * Need actual menu
		 */
		glut_menu = __gtkglut_get_active_menu_with_warning("glutGet");

		if (glut_menu) {
			res = g_list_length(glut_menu->child_items);
		}
	}

	if (state == GLUT_DISPLAY_MODE_POSSIBLE) {
		res = (__gtkglut_context->actual_gl_config) ? 1 : 0;
	}

	return res;
}

/*!
 * \brief    Query the Alt, Shift and Ctrl keys.
 * \ingroup  inputstate
 *
 * Return the logical \a OR combination of the
 * following symbolic bitmasks:
 *
 * - \a GLUT_ACTIVE_SHIFT
 * - \a GLUT_ACTIVE_CTRL
 * - \a GLUT_ACTIVE_ALT
 *
 * \todo Does not differentiate between the left and right forms
 *       of the modifiers (GLUT compatibility).
 * \see  glutSetOption(), glutGet(), glutDeviceGet(),
 *       glutLayerGet()
 */
int glutGetModifiers(void)
{
	__gtkglut_window_struct *glut_window;
	int res;
	/*
	 * Short name for __gtkglut_context->keyboard_modifiers
	 */
	guint mod;

	__gtkglut_test_inicialization("glutGetModifiers");

	glut_window = __gtkglut_get_active_window_with_warning("glutGetModifiers");
	res = 0;

	if (glut_window) {
		if (__gtkglut_context->keyboard_modifiers == (guint) __GTKGLUT_UNDEFINED) {
			__gtkglut_warning("glutGetModifiers",
					  "can be called only in mouse or keyboard callback");
		} else {
			mod = __gtkglut_context->keyboard_modifiers;

			if (mod & GDK_SHIFT_MASK)
				res |= GLUT_ACTIVE_SHIFT;
			if (mod & GDK_CONTROL_MASK)
				res |= GLUT_ACTIVE_CTRL;
			if (mod & GDK_MOD1_MASK)
				res |= GLUT_ACTIVE_ALT;
		}
	}

	return res;
}

/*
 * We don't have overlay implementation, so Layer get is very easy
 */

/*!
 * \brief    Query overlay state/option variables.
 * \ingroup  overlays
 * \param    info    Enumerated parameter ID.
 *
 * Returns some useful information about layers.
 * Or, it would be useful if layers were implemented...
 * \a eWhat may be given any of the following values:
 *
 * - \a GLUT_HAS_OVERLAY
 *
 * - \a GLUT_LAYER_IN_USE
 *
 * - \a GLUT_NORMAL_DAMAGED \n
 *   0 unless the window system has told us that
 *   the normal layer is damaged (glutPostRedisplay()
 *   does not affect this).
 *
 * - \a GLUT_OVERLAY_DAMAGED \n
 *   Returns -1 if no layer in use.
 *
 * - \a GLUT_OVERLAY_POSSIBLE
 *
 * - \a GLUT_TRANSPARENT_INDEX \n
 *   Returns -1 if no layer in use.
 *
 * All information relates to the *current window*
 * and any overlay that it may have.
 *
 * \note GtkGLUT does not support overlays.
 * \see glutSetOption(), glutGet(), glutDeviceGet(), glutGetModifiers()
 */
int glutLayerGet(GLenum info)
{
	int res;
	__gtkglut_window_struct *glut_window;

	__gtkglut_test_inicialization("glutLayerGet");

	if (info == GLUT_OVERLAY_POSSIBLE)
		return FALSE;

	res = 0;
	glut_window = __gtkglut_get_active_window_with_warning("glutLayerGet");

	if (glut_window) {
		switch (info) {
		case GLUT_OVERLAY_POSSIBLE:
			res = FALSE;
			break;
		case GLUT_LAYER_IN_USE:
			res = GLUT_NORMAL;
			break;
		case GLUT_HAS_OVERLAY:
			res = FALSE;
			break;
		case GLUT_TRANSPARENT_INDEX:
			res = 0;
			break;
		case GLUT_NORMAL_DAMAGED:
			res = glut_window->damaged;
			break;
		case GLUT_OVERLAY_DAMAGED:
			res = -1;
			break;
		default:
			__gtkglut_warning("glutLayerGet", "unknown state %d", info);
		}
	}

	return res;
}

/*!
 * \brief    Query input device state.
 * \ingroup  inputstate
 * \param    info    Enumerated parameter ID.
 *
 * Retrieve system-specific information about
 * attached devices.  Supported device queries are:
 *
 * - \a GLUT_HAS_KEYBOARD \n
 *   Return non-zero if there is a keyboard.
 *
 * - \a GLUT_HAS_MOUSE \n
 *   Return non-zero if there is a mouse.
 *
 * - \a GLUT_HAS_JOYSTICK \n
 *   Return non-zero if there is a joystick.
 *
 * - \a GLUT_HAS_SPACEBALL \n
 *   Return non-zero if there is a spaceball.
 *
 * - \a GLUT_HAS_DIAL_AND_BUTTON_BOX \n
 *   Return non-zero if a dials-and-buttons box is
 *   present.
 *
 * - \a GLUT_HAS_TABLET \n
 *   Return non-zero if a tablet is present.
 *
 * - \a GLUT_NUM_MOUSE_BUTTONS \n
 *   Return the number of mouse buttons.
 *
 * - \a GLUT_JOYSTICK_BUTTONS \n
 *   Return the number of joystick buttons.
 *
 * - \a GLUT_JOYSTICK_AXES \n
 *   Return the number of joystick axes.
 *
 * - \a GLUT_JOYSTICK_POLL_RATE \n
 *   Return the rate at which the joystick is polled,
 *   for the current window.
 *
 * - \a GLUT_OWNS_JOYSTICK \n
 *   Return non-zero if GtkGLUT believes that it has
 *   successfully acquired access to the joystick.
 *
 * - \a GLUT_DEVICE_IGNORE_KEY_REPEAT \n
 *   Return non-zero if the *current window* is
 *   set to disable key repeating.
 *
 * - \a GLUT_DEVICE_KEY_REPEAT \n
 *   Described as returning the key repeat rate in
 *   one place, but actually returns a key repeat mode.
 *
 * - \a GLUT_NUM_BUTTON_BOX_BUTTONS \n
 *   Return the number of buttons on a dials-and-buttons
 *   box, if any.
 *
 * - \a GLUT_NUM_DIALS \n
 *   Return the number of dials on a dials-and-buttons
 *   box, if any.
 *
 * - \a GLUT_NUM_SPACEBALL_BUTTONS \n
 *   Return the number of buttons on a spaceball, if any.
 *
 * - \a GLUT_NUM_TABLET_BUTTONS \n
 *   Return the number of buttons on a tablet, if any.
 *
 * \note Keyboards assumed to exist.
 * \note Mouse assumed to exist and always return 3 buttons.
 * \note Joystick is not supported.
 * \note \a GLUT_DEVICE_KEY_REPEAT is not supported.
 * \note SPACEBALL, TABLET, DIAL_AND_BUTTON_BOX are not supported.
 * \see glutSetOption(), glutGet(), glutGetModifiers(),
 *      glutLayerGet()
 */
int glutDeviceGet(GLenum info)
{
	int res;
	__gtkglut_test_inicialization("glutDeviceGet");

	res = 0;

	switch (info) {
	case GLUT_HAS_KEYBOARD:
		/*
		 * We assume, that have keyboard.
		 */
		res = TRUE;
		break;

	case GLUT_HAS_MOUSE:
		/*
		 * We assume, everybody have mouse
		 */
		res = TRUE;
		break;

	case GLUT_HAS_SPACEBALL:
	case GLUT_HAS_DIAL_AND_BUTTON_BOX:
	case GLUT_HAS_TABLET:
		/*
		 * This devices is unimplemented
		 */
		res = FALSE;
		break;

	case GLUT_NUM_MOUSE_BUTTONS:
		/*
		 * We assume 3 buttons
		 */
		res = 3;
		break;

	case GLUT_NUM_SPACEBALL_BUTTONS:
	case GLUT_NUM_BUTTON_BOX_BUTTONS:
	case GLUT_NUM_DIALS:
	case GLUT_NUM_TABLET_BUTTONS:
		/*
		 * We don't have any support for this devices
		 */
		res = 0;
		break;

	case GLUT_DEVICE_IGNORE_KEY_REPEAT:
		/*
		 * We don't support ignore key repeat
		 */
		res = FALSE;
		break;

	case GLUT_DEVICE_KEY_REPEAT:
		res = GLUT_KEY_REPEAT_ON;
		break;

	case GLUT_JOYSTICK_POLL_RATE:
	case GLUT_HAS_JOYSTICK:
	case GLUT_JOYSTICK_BUTTONS:
	case GLUT_JOYSTICK_AXES:
		/*
		 * No support for joystick
		 */
		res = 0;
		break;
	}

	return res;
}

/*!
 * \brief    Determine if an OpenGL extension is available.
 * \ingroup  opengl
 * \param    extension    A string-name of an extension.
 *
 * Returns 0 if GtkGLUT cannot determine that the requested
 * extension is definitely present.  Only checks extensions
 * from glGetString().
 *
 * \see glGetString()
 */
int glutExtensionSupported(char *extension)
{
	__gtkglut_window_struct *glut_window;
	int res;

	__gtkglut_test_inicialization("glutExtensionSupported");

	glut_window = __gtkglut_get_active_window_with_warning("glutGet");

	res = 0;

	if (glut_window) {
		res = gdk_gl_query_gl_extension(extension);
	}

	return res;
}
