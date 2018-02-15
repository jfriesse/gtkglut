/*!
 * \file gtkglut_debug.c
 * \brief OpenGL debug functions.
 */

/*
 * GtkGLUT OpenGL debug functions.
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

/*!
 * \defgroup opengl OpenGL Utilities
 */

/*!
 * \brief    Reports all available OpenGL errors.
 * \ingroup  opengl
 *
 * Displays as an GtkGLUT warning every OpenGL error
 * that OpenGL remembers giving to us and which
 * we have not processed.  Uses gluErrorString().
 *
 * This is forcibly done by GtkGLUT periodically
 * if \a -gldebug is one of the strings passed into
 * glutInit() via \a argv.
 *
 * \see gluErrorString(), glutInit()
*/
void glutReportErrors(void)
{
	GLenum err;

	err = glGetError();

	while (err != GL_NO_ERROR) {
		__gtkglut_warning("glDebug", "error %d occured: %s", (int)err,
				  (char *)gluErrorString(err));
		err = glGetError();
	}
}
