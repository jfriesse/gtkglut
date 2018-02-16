/*!
 * \file  gtkglut_init.c
 * \brief Initialization
 */

/*
 * Initialization methods.
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
 *
 * Contains some little modified code from FreeGLUT, see COPYING file
 */

#include "GL/gtkglut.h"
#include "gtkglut_internal.h"

/*!
 * \brief    Requests future windows to open at a given width/height..
 * \ingroup  window
 * \param    width    Width of future windows.
 * \param    height   Height of future windows.
 *
 * This function allows you to request initial dimensions
 * for future windows.
 *
 * There is a callback function to inform you of the new
 * window shape (whether initially opened, changed by
 * your glutReshapeWindow() request, or changed directly
 * by the user).
 * \see glutReshapeWindow(), glutInit(), glutInitWindowPosition(),
 *      glutInitDisplayMode(), glutInitDisplayString(),
 *      glutReshapeFunc(), glutGet()
 */
void glutInitWindowSize(int width, int height)
{

	if (!__gtkglut_context) {
		__gtkglut_context_init();
	}

	if (width > 0)
		__gtkglut_context->window_init_geometry.width = width;
	if (height > 0)
		__gtkglut_context->window_init_geometry.height = height;
}

/*!
 * \brief    Requests future windows to open at a given position.
 * \ingroup  window
 * \param    x    X coordinate.
 * \param    y    Y coordinate.
 *
 *  This function allows you to request an initial position
 *  for future windows.
 *
 * \see glutPositionWindow(), glutInit(), glutInitWindowSize(),
 *      glutInitDisplayMode(), glutInitDisplayString(), glutGet()
 */
void glutInitWindowPosition(int x, int y)
{

	if (!__gtkglut_context) {
		__gtkglut_context_init();
	}

	__gtkglut_context->window_init_geometry.x = x;
	__gtkglut_context->window_init_geometry.y = y;
}

/*!
 * \brief    Set an GtkGLUT option.
 * \ingroup  state
 * \param    what    Enumerated parameter ID.
 * \param    value    New value.
 *
 * Stores the \a value into a state variable named by
 * \a eWhat.
 *
 * Allowable \a eWhat IDs are:
 *
 * - \a GLUT_ACTION_ON_WINDOW_CLOSE \n
 *   Controls what happens when a window is closed by the
 *   user or system.
 *   \a GLUT_ACTION_EXIT
 *   will immediately exit the application (default).
 *   \a GLUT_ACTION_GLUTMAINLOOP_RETURNS
 *   will immediately return from the main loop.
 *   \a GLUT_ACTION_CONTINUE_EXECUTION
 *   will contine execution of remaining windows.
 *
 * - \a GLUT_INIT_DISPLAY_MODE \n
 *   Set the display mode for new windows.
 *
 * - \a GLUT_INIT_WINDOW_HEIGHT \n
 *   Set the initial height of new windows.
 *
 * - \a GLUT_INIT_WINDOW_WIDTH \n
 *   Set the initial width of new windows.
 *
 * - \a GLUT_INIT_WINDOW_X \n
 *   Set the initial horizontal position of new windows.
 *
 * - \a GLUT_INIT_WINDOW_Y \n
 *   Set the initial vertical position of new windows.
 *
 * - \a GLUT_WINDOW_CURSOR \n
 *   Attempt to set the current cursor of *current window*
 *   as by calling glutSetCursor().
 *
 * \see glutGet(), glutDeviceGet(), glutGetModifiers(),
 *      glutLayerGet(), glutDestroyWindow(), glutMainLoop(),
 *      glutInitDisplayMode(), glutInit(), glutInitWindowSize(),
 *      glutInitWindowPosition(), glutSetCursor()
 */
void glutSetOption(int what, int value)
{
	int x, y;

	switch (what) {
	case GLUT_INIT_DISPLAY_MODE:
		glutInitDisplayMode(value);
		break;

	case GLUT_INIT_WINDOW_WIDTH:
		glutInitWindowSize(value, -1);
		break;

	case GLUT_INIT_WINDOW_HEIGHT:
		glutInitWindowSize(-1, value);
		break;

	case GLUT_INIT_WINDOW_X:
	case GLUT_INIT_WINDOW_Y:
		if (!__gtkglut_context) {
			__gtkglut_context_init();
		}

		x = (what == GLUT_INIT_WINDOW_X) ? what : __gtkglut_context->window_init_geometry.x;
		y = (what == GLUT_INIT_WINDOW_Y) ? what : __gtkglut_context->window_init_geometry.y;

		glutInitWindowPosition(x, y);
		break;

	case GLUT_WINDOW_CURSOR:
		glutSetCursor(value);
		break;

	case GLUT_ACTION_ON_WINDOW_CLOSE:
		if (!__gtkglut_context) {
			__gtkglut_context_init();
		}

		__gtkglut_context->action_on_window_close = value;
		break;
	}
}

/*
 * Remove NULL parameters from argv array of strings. Array is shrinked.
 */
void __gtkglut_remove_null_params(int *argcp, char **argv)
{
	int fromp, top;
	int newargc;

	fromp = top = 0;
	newargc = *argcp;

	while (fromp < *argcp) {
		if (argv[fromp] != NULL) {
			argv[top] = argv[fromp];
			top++;
		} else {
			newargc--;
		}
		fromp++;
	}

	*argcp = newargc;
}

/*
 * Geometry we must process in two stages. First, get parameters, second, we
 * may need screen_width (for values <0), but gdk_... is avaliable only after
 * gtk_init
 */
void __gtkglut_parse_params_geometry(__gtkglut_geometry_struct * geometry)
{

	if (geometry) {
		glutInitWindowSize(geometry->width, geometry->height);
		if (geometry->x < 0) {
			geometry->x += glutGet(GLUT_SCREEN_WIDTH) - geometry->width;
		}
		if (geometry->y < 0) {
			geometry->y += glutGet(GLUT_SCREEN_HEIGHT) - geometry->height;
		}
		glutInitWindowPosition(geometry->x, geometry->y);

		free(geometry);
	}
}

/*
 * Parse command line arguments
 */
GSList *__gtkglut_parse_params(int *argcp, char **argv, __gtkglut_geometry_struct ** geometry)
{
	int i;
	GSList *new_str_list;
	char *tmpstr;
	int x, y, w, h;

	i = 0;
	new_str_list = NULL;

	while (i < *argcp) {
		if (argv[i] && strcmp(argv[i], "-display") == 0) {
			/*Parameter -display convert to --display */
			tmpstr = g_strdup("--display");
			if (tmpstr == NULL)
				__gtkglut_lowmem();
			new_str_list = g_slist_append(new_str_list, tmpstr);

			argv[i] = tmpstr;
			i += 2;
			continue;
		}

		if (argv[i] && strcmp(argv[i], "-geometry") == 0) {
			argv[i] = NULL;

			if (i + 1 < *argcp) {
				i++;

				if (!(*geometry)) {
					(*geometry) =
					    (__gtkglut_geometry_struct *)
					    malloc(sizeof(__gtkglut_geometry_struct));
					if (!(*geometry))
						__gtkglut_lowmem();
				}

				x = glutGet(GLUT_INIT_WINDOW_X);
				y = glutGet(GLUT_INIT_WINDOW_Y);
				w = glutGet(GLUT_INIT_WINDOW_WIDTH);
				h = glutGet(GLUT_INIT_WINDOW_HEIGHT);

				sscanf(argv[i], "%dx%d%d%d", &w, &h, &x, &y);

				(*geometry)->x = x;
				(*geometry)->y = y;
				(*geometry)->width = w;
				(*geometry)->height = h;

				/*
				 * Geometry we must process in two stages. First, get parameters, second, we
				 * may need screen_width (for values <0), but gdk_... is avaliable only after
				 * gtk_init
				 */
				argv[i] = NULL;
			}
		}

		if (argv[i] && strcmp(argv[i], "-sync") == 0) {
			/*Parameter -sync convert to --sync */
			tmpstr = g_strdup("--sync");
			if (tmpstr == NULL)
				__gtkglut_lowmem();
			new_str_list = g_slist_append(new_str_list, tmpstr);

			argv[i] = tmpstr;
		}

		if (argv[i] && strcmp(argv[i], "-indirect") == 0) {
			__gtkglut_context->direct_render = __GTKGLUT_DIRECT_RENDER_INDIRECT;
			argv[i] = NULL;
		}

		if (argv[i] && strcmp(argv[i], "-direct") == 0) {
			__gtkglut_context->direct_render = __GTKGLUT_DIRECT_RENDER_DIRECT;
			argv[i] = NULL;
		}

		if (argv[i] && strcmp(argv[i], "-iconic") == 0) {
			__gtkglut_context->window_init_iconic = 1;
			argv[i] = NULL;
		}

		if (argv[i] && strcmp(argv[i], "-gldebug") == 0) {
			__gtkglut_context->gl_debug = 1;
			argv[i] = NULL;
		}

		i++;
	}

	__gtkglut_remove_null_params(argcp, argv);
	return new_str_list;
}

void __gtkglut_free_params(int *argcp, char **argv, GSList ** list)
{
	int i;
	GSList *elem;

	/*
	 * Try to find element, which is in list, and in argv -> item isn't processed
	 * (for example MS Windows and -display)
	 */
	for (i = 0; i < *argcp; i++) {
		if (g_slist_find(*list, argv[i])) {
			/*
			 * Set to NULL -> better than fall down with read unitinialized memory
			 */

			/*
			 * Display is special, delete argument too
			 */
			if (strcmp(argv[i], "--display") == 0 && i + 1 < *argcp) {
				argv[i] = NULL;
				i++;
			}

			argv[i] = NULL;
		}
	}

	__gtkglut_remove_null_params(argcp, argv);

	elem = *list;

	while (elem) {
		free(elem->data);
		elem = g_slist_next(elem);
	}

	g_slist_free(*list);

	*list = NULL;
}

/*!
 * \brief    Set the window creation display mode.
 * \ingroup  window
 * \param    mode    Requested display mode bitmask.
 *
 * glutInitDisplayMode() allows you to control
 * the mode for subsequent GtkGLUT windows.
 *
 * Allowable \a displayMode is a combination of:
 *
 * - \a GLUT_RGB \n
 *   Red, green, blue framebuffer.
 *
 * - \a GLUT_RGBA \n
 *   Red, green, blue, alpha framebuffer.
 *
 * - \a GLUT_INDEX \n
 *   Indexed color framebuffer.
 *
 * - \a GLUT_SINGLE \n
 *   Single-buffered mode.
 *
 * - \a GLUT_DOUBLE \n
 *   Double-buffered mode.
 *
 * - \a GLUT_ACCUM \n
 *   Accumulation buffer.
 *
 * - \a GLUT_ALPHA \n
 *   Alpha channel.
 *
 * - \a GLUT_DEPTH \n
 *   Depth buffering.
 *
 * - \a GLUT_STENCIL \n
 *   Stencil buffering.
 *
 * - \a GLUT_MULTISAMPLE \n
 *   Multisampling mode. (not always available)
 *
 * - \a GLUT_STEREO \n
 *   Left and right framebuffers.
 *
 * - \a GLUT_LUMINANCE \n
 *   Greyscale color mode.
 *
 * The following are *defaults*:
 *
 * - \a GLUT_RGB
 * - \a GLUT_SINGLE
 * - \a GLUT_DEPTH
 *
 * \note Not all features or combinations of features are
 *       valid for all platforms.
 *
 * \note There is no way to change the display mode of an open window.
 *
 * \see glutInit(), glutInitWindowSize(),
 *      glutInitWindowPosition(), glutInitDisplayString(),
 *      glutSwapBuffers()
 */
void glutInitDisplayMode(unsigned int mode)
{

	if (!__gtkglut_context) {
		__gtkglut_context_init();
	}

	__gtkglut_context->display_mode = mode;

	if (__gtkglut_context->is_inicialized) {
		__gtkglut_context->actual_gl_config = __gtkglut_init_gl_config();
	}
}

/*
 * -- INIT DISPLAY STRING PARSING ------------------------------------------
 */

static char *__gtkglut_init_display_string_tokens[] = {
	"alpha", "acca", "acc", "blue", "buffer", "conformant", "depth", "double",
	"green", "index", "num", "red", "rgba", "rgb", "luminance", "stencil",
	"single", "stereo", "samples", "slow", "win32pdf", "win32pfd", "xvisual",
	"xstaticgray", "xgrayscale", "xstaticcolor", "xpseudocolor",
	"xtruecolor", "xdirectcolor",
	"xstaticgrey", "xgreyscale", "xstaticcolour", "xpseudocolour",
	"xtruecolour", "xdirectcolour", "borderless"
};

#define __GTKGLUT_INIT_DISPLAY_STRING_NUM_TOKENS             (sizeof(__gtkglut_init_display_string_tokens) / sizeof(*__gtkglut_init_display_string_tokens))

/*!
 * \brief    Set the window creation display mode.
 * \ingroup  window
 * \param    string    Requested display mode string.
 *
 * glutInitDisplayString() permits you to define a display
 * mode for subsequent windows that you open.  In most
 * regards, control is at least as fine as with
 * glutInitDisplaymode().
 *
 * The \a displayMode parameter is case-sensitive, and
 * tokens are separated by ASCII TABs (\\t) and SPACEs.
 *
 * - \a index \n
 *   Enables \a GLUT_INDEX.
 *
 * - \a luminance \n
 *   Enables \a GLUT_LUMINANCE.
 *   Enables \a GLUT_STENCIL.
 *
 * - \a red \n
 *   Number of red channel bits.
 *
 * - \a green \n
 *   Number of green channel bits.
 *
 * - \a blue \n
 *   Number of blue channel bits.
 *
 * - \a alpha \n
 *   Number of alpha channel bits.
 *   Enables \a GLUT_ALPHA.
 *
 * - \a rgb \n
 *   Number of \a RGB channel bits, no aplha bits.
 *   Enables \a GLUT_RGB.
 *
 * - \a rgba \n
 *   Number of \a RGBA channel bits.
 *   Enables \a GLUT_RGBA.
 *
 * - \a depth \n
 *   Number of depth buffer bits.
 *
 * - \a stencil \n
 *   Number of stencil buffer bits.
 *
 * - \a double \n
 *   Enables \a GLUT_DOUBLE.
 *
 * - \a single \n
 *   Enables \a GLUT_SINGLE.
 *
 * - \a stereo \n
 *   Enables \a GLUT_STEREO.
 *
 * - \a acca \n
 *   Number of \a RGBA accumulation bits.
 *   Enables \a GLUT_ACCUM.
 *
 * - \a acc \n
 *   Number of \a RGB accumulation bits.
 *   Enables \a GLUT_ACCUM.
 *
 * - \a samples \n
 *   Number of samples for GLX's \a SGIS_Multisample.
 *   Enables \a GLUT_MULTISAMPLE.
 *
 * - \a buffer \n
 *   Sets bits in index mode?
 *
 * - \a conformant \n
 *   Conformant with what?
 *   Enables \a GLUT_DEPTH.
 *
 * - \a slow \n
 *   Indicates if a frame-buffer is slow.
 *
 * - \a num \n
 *   Appears to select a frame-buffer configuration
 *   by number from an unspecified list.  Probably
 *   very non-portable.
 *
 * A special capability  name indicating where the
 * value represents the Nth frame buffer configuration
 * matching the description string
 *
 * - \a win32pdf \n
 *   Win32 specific: Pixel Format Descriptor
 *
 * - \a win32pfd \n
 *   Win32 specific: Pixel Format Descriptor
 *
 * - \a xvisual \n
 *   X11 specific: X Visual
 *
 * - \a xstaticgray \n
 *   X11 specific: "staticgray" mode.
 *
 * - \a xgrayscale \n
 *   X11 specific: "grayscale" mode.
 *
 * - \a xstaticcolor \n
 *   X11 specific: "staticcolor" mode.
 *
 * - \a xpseudocolor \n
 *   X11 specific: "pseudocolor" mode.
 *
 * - \a xtruecolor \n
 *   X11 specific: "trueolor" mode.
 *
 * - \a xdirectcolor \n
 *   X11 specific: "directcolor" mode.
 *
 * \note Conflicting modes, such as \a single and \a double
 *       have the same interaction as for glutInitDisplayMode().
 *
 * \note Most of the features (ex. platform specific) is not implemented. In particular,
 *       numeric parameters and comparator specifications are lacking.
 *       See GLUT 3.7 sources for example.
 *
 * \see  glutInit(), glutInitWindowPosition(), glutInitWindowSize(),
 *       glutInitDisplayMode()
 */
void glutInitDisplayString(char *string)
{
	int glut_state_flag = 0;
	char *token;
	char *buffer;
	unsigned int i;

	if (!string)
		return;

	buffer = g_strdup(string);

	token = strtok(buffer, " \t");
	while (token) {
		/*
		 * Process this token
		 */
		for (i = 0; i < __GTKGLUT_INIT_DISPLAY_STRING_NUM_TOKENS; i++) {
			if (strcmp(token, __gtkglut_init_display_string_tokens[i]) == 0)
				break;
		}

		switch (i) {
		case 0:	/* "alpha":  Alpha color buffer precision in bits */
			glut_state_flag |= GLUT_ALPHA;	/* Somebody fix this for me! */
			break;

		case 1:	/* "acca":  Red, green, blue, and alpha accumulation buffer
				   precision in bits */
			break;

		case 2:	/* "acc":  Red, green, and blue accumulation buffer precision
				   in bits with zero bits alpha */
			glut_state_flag |= GLUT_ACCUM;	/* Somebody fix this for me! */
			break;

		case 3:	/* "blue":  Blue color buffer precision in bits */
			break;

		case 4:	/* "buffer":  Number of bits in the color index color buffer
				 */
			break;

		case 5:	/* "conformant":  Boolean indicating if the frame buffer
				   configuration is conformant or not */
			break;

		case 6:	/* "depth":  Number of bits of precsion in the depth buffer */
			glut_state_flag |= GLUT_DEPTH;	/* Somebody fix this for me! */
			break;

		case 7:	/* "double":  Boolean indicating if the color buffer is
				   double buffered */
			glut_state_flag |= GLUT_DOUBLE;
			break;

		case 8:	/* "green":  Green color buffer precision in bits */
			break;

		case 9:	/* "index":  Boolean if the color model is color index or not */
			glut_state_flag |= GLUT_INDEX;
			break;

		case 10:	/* "num":  A special capability  name indicating where the
				   value represents the Nth frame buffer configuration
				   matching the description string */
			break;

		case 11:	/* "red":  Red color buffer precision in bits */
			break;

		case 12:	/* "rgba":  Number of bits of red, green, blue, and alpha in
				   the RGBA color buffer */
			glut_state_flag |= GLUT_RGBA;	/* Somebody fix this for me! */
			break;

		case 13:	/* "rgb":  Number of bits of red, green, and blue in the
				   RGBA color buffer with zero bits alpha */
			glut_state_flag |= GLUT_RGBA;	/* Somebody fix this for me! */
			break;

		case 14:	/* "luminance":  Number of bits of red in the RGBA and zero
				   bits of green, blue (alpha not specified) of color buffer
				   precision */
			glut_state_flag |= GLUT_LUMINANCE;	/* Somebody fix this for me! */
			break;

		case 15:	/* "stencil":  Number of bits in the stencil buffer */
			glut_state_flag |= GLUT_STENCIL;	/* Somebody fix this for me! */
			break;

		case 16:	/* "single":  Boolean indicate the color buffer is single
				   buffered */
			glut_state_flag |= GLUT_SINGLE;
			break;

		case 17:	/* "stereo":  Boolean indicating the color buffer supports
				   OpenGL-style stereo */
			glut_state_flag |= GLUT_STEREO;
			break;

		case 18:	/* "samples":  Indicates the number of multisamples to use
				   based on GLX's SGIS_multisample extension (for
				   antialiasing) */
			glut_state_flag |= GLUT_MULTISAMPLE;	/*Somebody fix this for me! */
			break;

		case 19:	/* "slow":  Boolean indicating if the frame buffer
				   configuration is slow or not */
			break;

		case 20:	/* "win32pdf": (incorrect spelling but was there before */
		case 21:	/* "win32pfd":  matches the Win32 Pixel Format Descriptor by
				   number */
			break;

		case 22:	/* "xvisual":  matches the X visual ID by number */
			break;

		case 23:	/* "xstaticgray": */
		case 29:	/* "xstaticgrey":  boolean indicating if the frame buffer
				   configuration's X visual is of type StaticGray */
			break;

		case 24:	/* "xgrayscale": */
		case 30:	/* "xgreyscale":  boolean indicating if the frame buffer
				   configuration's X visual is of type GrayScale */
			break;

		case 25:	/* "xstaticcolor": */
		case 31:	/* "xstaticcolour":  boolean indicating if the frame buffer
				   configuration's X visual is of type StaticColor */
			break;

		case 26:	/* "xpseudocolor": */
		case 32:	/* "xpseudocolour":  boolean indicating if the frame buffer
				   configuration's X visual is of type PseudoColor */
			break;

		case 27:	/* "xtruecolor": */
		case 33:	/* "xtruecolour":  boolean indicating if the frame buffer
				   configuration's X visual is of type TrueColor */
			break;

		case 28:	/* "xdirectcolor": */
		case 34:	/* "xdirectcolour":  boolean indicating if the frame buffer
				   configuration's X visual is of type DirectColor */
			break;

		case 35:	/* "borderless":  windows should not have borders */
			break;

		case 36:	/* Unrecognized */
			__gtkglut_warning("glutInitDisplayString",
					  "WARNING - Display string token not recognized:  %s",
					  token);
			glut_state_flag = __GTKGLUT_UNDEFINED;
			break;
		}

		token = strtok(NULL, " \t");
	}

	free(buffer);

	/*
	 * We will make use of this value when creating a new OpenGL context...
	 */
	glutInitDisplayMode(glut_state_flag);
}

/*!
 * \brief    Initialize GtkGLUT data structures.
 * \ingroup  mainloop
 * \param    argcp    Pointer to something like main()'s \a argc.
 * \param    argv     Something like main()'s \a argv.
 *
 * This function should be called once, near the start of
 * any GLUT, freeglut, or GtkGLUT program.  It serves two
 * vital roles:
 *
 * - It allows GtkGLUT to initialize internal structures.
 * - It allows GtkGLUT to process command-line arguments
 *   to control the initial window position, etc.
 *
 * You should take note of the interaction between
 * glutInit() and the related functions such as
 * glutInitWindowPosition().  GtkGLUT always uses
 * the most recent configuration information, so
 * if you call glutInit(), then glutInitWindowPosition(),
 * you prevent the user from controlling the initial
 * window position via a command-line parameter.
 *
 * glutInit() will remove from \a pargc, \a argv
 * any parameters that it
 * recognizes in the command line.  The following
 * command-line parameters are suported:
 *
 * - \a -display *display-id*
 *   This allows connection to an alternate X server.
 * - \a -geometry *geometry-spec*
 *   This takes width, height, and
 *   window position.  The position is given as
 *   a signed value (negative values being distance
 *   from the far boundary of the screen).  For example,
 *   a window geometry of 5x7+11-17 is 5 pixels
 *   wide, 7 pixels tall, 11 pixels from the left,
 *   and 17 pixels from the bottom edge of the screen.
 * - \a -direct Insist on only OpenGL direct rendering.
 *   Direct rendering is normally requested but indirect
 *   is normally accepted.
 *   \a -direct is not always available.
 *   If both \a direct and \a indirect are specified on
 *   the command line, the tail-most one determines
 *   GtkGLUT's behavior.
 *   See \a -indirect
 * - \a -indirect Attempt only indirect OpenGL rendering.
 *   \a -indirect is always available.
 *   See \a -direct, \a -maybe-indirect.
 * - \a -iconic Open the window in iconized form.
 * - \a -gldebug Print any detected OpenGL errors via
 *   glutReportErrors().  Presently
 *   done at the bottom of glutMainLoopEvent().
 *
 * \note You really should always call this, even if you are
 *       a WIN32 user.  It provides a way for the user to
 *       directly inform GtkGLUT about preferences without
 *       the application needing to explicitly deal with
 *       those issues.  This is also where GtkGLUT retrieves
 *       your program's name to help disambiguate error and
 *       warning messages it may be forced to emit.
 * \see glutInitWindowPosition(), glutInitWindowSize(),
 *      glutInitDisplayMode(), glutInitDisplayString(),
 *      glutCreateWindow(), glutDisplayFunc(),
 *      glutMainLoop(), glutMainLoopEvent(), glutReportErrors(),
 *      glutSwapBuffers()
 */
void glutInit(int *argcp, char **argv)
{
	GSList *new_str_list;
	__gtkglut_geometry_struct *geometry;

	/*
	 * Initialize context only if needed
	 */
	if (!__gtkglut_context) {
		__gtkglut_context_init();
	}

	/*
	 * Parse parameters
	 */
	geometry = 0;
	new_str_list = __gtkglut_parse_params(argcp, argv, &geometry);

	/*
	 * Initialize GTK.
	 */
	gtk_init(argcp, &argv);

	/*
	 * Initialize GtkGLExt.
	 */
	gtk_gl_init(argcp, &argv);

	__gtkglut_context->is_inicialized = 1;

	__gtkglut_parse_params_geometry(geometry);

	__gtkglut_free_params(argcp, argv, &new_str_list);

	__gtkglut_context->actual_gl_config = __gtkglut_init_gl_config();
}
