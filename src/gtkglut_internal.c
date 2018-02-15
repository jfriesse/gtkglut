/*!
 * \file  gtkglut_internal.c
 * \brief GtkGLUT Internal functions.
 */

/*
 * GtkGLUT Internal functions.
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

__gtkglut_context_struct *__gtkglut_context = NULL;

/*
 * Mapping from GLUT cursor ID to GDK cursor ID
 */
__gtkglut_cursors_cache_struct __gtkglut_cursors_cache_default[] = {
	{GDK_ARROW},
	{GDK_TOP_LEFT_ARROW},
	{GDK_HAND1},
	{GDK_PIRATE},
	{GDK_QUESTION_ARROW},
	{GDK_EXCHANGE},
	{GDK_SPRAYCAN},
	{GDK_WATCH},
	{GDK_XTERM},
	{GDK_CROSSHAIR},
	{GDK_SB_V_DOUBLE_ARROW},
	{GDK_SB_H_DOUBLE_ARROW},
	{GDK_TOP_SIDE},
	{GDK_BOTTOM_SIDE},
	{GDK_LEFT_SIDE},
	{GDK_RIGHT_SIDE},
	{GDK_TOP_LEFT_CORNER},
	{GDK_TOP_RIGHT_CORNER},
	{GDK_BOTTOM_RIGHT_CORNER},
	{GDK_BOTTOM_LEFT_CORNER},
	{GDK_CROSSHAIR},
};

/*
 * Show error on lowmemory and exit application
 */
void __gtkglut_lowmem(void)
{
	fprintf(stderr, "GtkGLUT low memory! Exiting!\n");

	exit(1);
}

/*
 * Show user defined error and exit apllication
 */
void __gtkglut_error(char *func_name, char *fmt, ...)
{
	va_list args;

	va_start(args, fmt);
	fprintf(stderr, "GtkGLUT Error ");
	fprintf(stderr, "(%s): ", func_name);
	vfprintf(stderr, fmt, args);
	fprintf(stderr, "\n");
	va_end(args);

	exit(2);
}

/*
 * Show user defined warning
 */
void __gtkglut_warning(char *func_name, char *fmt, ...)
{
	va_list args;

	va_start(args, fmt);
	fprintf(stderr, "GtkGLUT Warning ");
	fprintf(stderr, "(%s): ", func_name);
	vfprintf(stderr, fmt, args);
	fprintf(stderr, "\n");
	va_end(args);
}

/*
 * Test, if library was inicialized properly. If not, exit
 */
void __gtkglut_test_inicialization(char *func_name)
{

	if (!__gtkglut_context || !__gtkglut_context->is_inicialized) {
		__gtkglut_error(func_name, "library wasn't initialized!", func_name);
	}
}

void __gtkglut_init_cursors_cache(void)
{
	__gtkglut_cursors_cache_struct *res;
	unsigned int i;

	res = (__gtkglut_cursors_cache_struct *) malloc(sizeof(__gtkglut_cursors_cache_default));
	if (!res)
		__gtkglut_lowmem();

	for (i = 0;
	     i <
	     sizeof(__gtkglut_cursors_cache_default) / sizeof(__gtkglut_cursors_cache_default[0]);
	     i++) {
		res[i] = __gtkglut_cursors_cache_default[i];
		res[i].cursor = NULL;
	}

	__gtkglut_context->cursors_cache = res;
}

/*
 * Init global GtkGLUT context
 */
void __gtkglut_context_init(void)
{

	__gtkglut_context = (__gtkglut_context_struct *) malloc(sizeof(__gtkglut_context_struct));
	if (__gtkglut_context == NULL) {
		__gtkglut_lowmem();
	}

	__gtkglut_context->is_inicialized = 0;
	__gtkglut_context->window_init_iconic = 0;
	__gtkglut_context->gl_debug = 0;
	__gtkglut_context->direct_render = __GTKGLUT_DIRECT_RENDER_NOT_SET;
	glutInitWindowSize(__GTKGLUT_WINDOW_INIT_WIDTH, __GTKGLUT_WINDOW_INIT_HEIGHT);
	glutInitWindowPosition(__GTKGLUT_WINDOW_INIT_X, __GTKGLUT_WINDOW_INIT_Y);
	glutInitDisplayMode(__GTKGLUT_INIT_DISPLAY_MODE);
	__gtkglut_context->elapsed_time_timer = g_timer_new();
	__gtkglut_context->callback_idle = NULL;
	__gtkglut_context->actual_gl_config = NULL;
	__gtkglut_context->window_array = NULL;
	__gtkglut_context->menu_array = NULL;
	__gtkglut_context->current_gl_drawable = NULL;
	__gtkglut_context->current_gl_context = NULL;
	__gtkglut_context->current_window = -1;
	__gtkglut_context->current_menu = -1;
	__gtkglut_init_cursors_cache();
	__gtkglut_context->cursor_none = NULL;
	__gtkglut_context->keyboard_modifiers = __GTKGLUT_UNDEFINED;
	__gtkglut_context->menu_status_func = NULL;
	__gtkglut_context->menu_state_func = NULL;
	__gtkglut_context->action_on_window_close = GLUT_ACTION_EXIT;
	__gtkglut_context->no_active_toplevel_windows = 0;
	__gtkglut_context->current_active_menu_window_id = -1;
}

/*
 * Compatibility feature. Convert display mode from GLUT to GtkGLExt
 */
GdkGLConfigMode __gtkglut_convert_glut_mode_to_gtkglext(int mode)
{
	int res;

	res = 0;

	if (mode & GLUT_RGB)
		res |= GDK_GL_MODE_RGB;
	if (mode & GLUT_RGBA)
		res |= GDK_GL_MODE_RGBA;
	if (mode & GLUT_INDEX)
		res |= GDK_GL_MODE_INDEX;
	if (mode & GLUT_SINGLE)
		res |= GDK_GL_MODE_SINGLE;
	if (mode & GLUT_DOUBLE)
		res |= GDK_GL_MODE_DOUBLE;
	if (mode & GLUT_ACCUM)
		res |= GDK_GL_MODE_ACCUM;
	if (mode & GLUT_ALPHA)
		res |= GDK_GL_MODE_ALPHA;
	if (mode & GLUT_DEPTH)
		res |= GDK_GL_MODE_DEPTH;
	if (mode & GLUT_STENCIL)
		res |= GDK_GL_MODE_STENCIL;
	if (mode & GLUT_MULTISAMPLE)
		res |= GDK_GL_MODE_MULTISAMPLE;
	if (mode & GLUT_STEREO)
		res |= GDK_GL_MODE_STEREO;
	if (mode & GLUT_LUMINANCE) {
		__gtkglut_warning("__gtkglut_convert_glut_mode_to_gtkglext",
				  "luminance is not supported");
	}

	return (GdkGLConfigMode) res;
}

GdkGLConfig *__gtkglut_init_gl_config(void)
{
	if (__gtkglut_context->display_mode == (unsigned int)__GTKGLUT_UNDEFINED)
		return NULL;
	return
	    gdk_gl_config_new_by_mode(__gtkglut_convert_glut_mode_to_gtkglext
				      (__gtkglut_context->display_mode));
}

void __gtkglut_gl_debug()
{
	if (__gtkglut_context->gl_debug) {
		glutReportErrors();
	}
}

/*
 * Show warning on unimplement function
 */
void __gtkglut_unimplemented_warning(char *func_name)
{
	__gtkglut_warning(func_name, "function is not implemented");
}
