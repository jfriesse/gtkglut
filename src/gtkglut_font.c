/*!
 * \file  gtkglut_font.c
 * \brief Bitmap and stroke fonts
*/

/*
 * GtkGLUT Bitmap and stroke fonts.
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
 * Based on freeglut code (file freeglut_font.c).
 */

/*
 * freeglut_font.c
 *
 * Bitmap and stroke fonts displaying.
 *
 * Copyright (c) 1999-2000 Pawel W. Olszta. All Rights Reserved.
 * Written by Pawel W. Olszta, <olszta@sourceforge.net>
 * Creation date: Thu Dec 16 1999
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
 */

#include "GL/gtkglut.h"
#include "gtkglut_internal.h"

/* -- IMPORT DECLARATIONS -------------------------------------------------- */

/*
 * These are the font faces defined in freeglut_font_data.c file:
 */
#ifdef __cplusplus
extern "C" {
#endif

	extern __gtkglut_SFG_Font __gtkglut_fgFontFixed8x13;
	extern __gtkglut_SFG_Font __gtkglut_fgFontFixed9x15;
	extern __gtkglut_SFG_Font __gtkglut_fgFontHelvetica10;
	extern __gtkglut_SFG_Font __gtkglut_fgFontHelvetica12;
	extern __gtkglut_SFG_Font __gtkglut_fgFontHelvetica18;
	extern __gtkglut_SFG_Font __gtkglut_fgFontTimesRoman10;
	extern __gtkglut_SFG_Font __gtkglut_fgFontTimesRoman24;
	extern __gtkglut_SFG_StrokeFont __gtkglut_fgStrokeRoman;
	extern __gtkglut_SFG_StrokeFont __gtkglut_fgStrokeMonoRoman;

#ifdef __cplusplus
}
#endif
/*
 * Font definitions - for binary compatibility. Don't use this in glut programs!!!!!!)
 */
void *glutStrokeRoman;
void *glutStrokeMonoRoman;
void *glutBitmap9By15;
void *glutBitmap8By13;
void *glutBitmapTimesRoman10;
void *glutBitmapTimesRoman24;
void *glutBitmapHelvetica10;
void *glutBitmapHelvetica12;
void *glutBitmapHelvetica18;

/* -- PRIVATE FUNCTIONS ---------------------------------------------------- */

/*
 * Matches a font ID with a SFG_Font structure pointer.
 * This was changed to match the GLUT header style.
 */
static __gtkglut_SFG_Font *__gtkglut_fghFontByID(void *font)
{
	if (font == GLUT_BITMAP_8_BY_13)
		return &__gtkglut_fgFontFixed8x13;
	if (font == GLUT_BITMAP_9_BY_15)
		return &__gtkglut_fgFontFixed9x15;
	if (font == GLUT_BITMAP_HELVETICA_10)
		return &__gtkglut_fgFontHelvetica10;
	if (font == GLUT_BITMAP_HELVETICA_12)
		return &__gtkglut_fgFontHelvetica12;
	if (font == GLUT_BITMAP_HELVETICA_18)
		return &__gtkglut_fgFontHelvetica18;
	if (font == GLUT_BITMAP_TIMES_ROMAN_10)
		return &__gtkglut_fgFontTimesRoman10;
	if (font == GLUT_BITMAP_TIMES_ROMAN_24)
		return &__gtkglut_fgFontTimesRoman24;

	__gtkglut_warning("__gtkglut_fghFontByID", "font 0x%08x not found", font);
	return NULL;
}

/*
 * Matches a font ID with a SFG_StrokeFont structure pointer.
 * This was changed to match the GLUT header style.
 */
static __gtkglut_SFG_StrokeFont *__gtkglut_fghStrokeByID(void *font)
{
	if (font == GLUT_STROKE_ROMAN)
		return &__gtkglut_fgStrokeRoman;
	if (font == GLUT_STROKE_MONO_ROMAN)
		return &__gtkglut_fgStrokeMonoRoman;

	__gtkglut_warning("__gtkglut_fghStrokeByID", "stroke font 0x%08x not found", font);
	return NULL;
}

/*
 * -- INTERFACE FUNCTIONS --------------------------------------------------
 */

/*!
 * \defgroup bitmapfont Text Rendering - Bitmapped Fonts
 *
 * GtkGLUT fixed-width bitmap fonts:
 *  - GLUT_BITMAP_9_BY_15
 *  - GLUT_BITMAP_8_BY_13
 *
 * \image html openglut_bitmap_9by15.png
 * \image html openglut_bitmap_8by13.png
 *
 * GtkGLUT variable width bitmap fonts:
 * - GLUT_BITMAP_HELVETICA_18
 * - GLUT_BITMAP_HELVETICA_12
 * - GLUT_BITMAP_HELVETICA_10
 * - GLUT_BITMAP_TIMES_ROMAN_24
 * - GLUT_BITMAP_TIMES_ROMAN_10
 *
 * \image html openglut_bitmap_helvetica_18.png
 * \image html openglut_bitmap_helvetica_12.png
 * \image html openglut_bitmap_helvetica_10.png
 * \image html openglut_bitmap_roman_24.png
 * \image html openglut_bitmap_roman_10.png
 *
 * The fonts are extracted from an open-source X server, and cover
 * ISO-8859-1, with printable values in the ranges 32--126 and 160--255.
 * Additionally, the fixed-width fonts have additional glyphs in
 * the range 1 through 31.
 *
 * The heights of the fonts are not generally encoded in their names,
 * though the names give some indication of the heights.  For example,
 * the 8x13 fixed-width font is 14 pixels tall.  The fixed width fonts
 * have their widths embedded in their names:  \a GLUT_BITMAP_8_BY_13 and
 * \a GLUT_BITMAP_9_BY_15 .
 */

/*!
 * \brief    Draw a bitmapped character
 * \ingroup  bitmapfont
 * \param    fontID      A bitmapped font identifier.
 * \param    character A character code.
 *
 * Draw a \a character at the current OpenGL raster position
 * using a bitmapped \a font.  The raster position is advanced
 * by the width of the character.
 *
 * Nothing is drawn, and the raster position is unaffected when
 * either:
 * - \a character is out of range
 * - \a font is not a valid GtkGLUT bitmap font
 * - The current OpenGL raster position is invalid
 *
 * \note glutBitmapString() is generally more efficient for
 *       strings of characters.
 *
 * \see glRasterPos(), glutBitmapString(), glutBitmapWidth(),
 *      glutBitmapHeight(), glutStrokeCharacter()
 */
void glutBitmapCharacter(void *fontID, int character)
{
	const GLubyte *face;
	__gtkglut_SFG_Font *font;

	__gtkglut_test_inicialization("glutBitmapCharacter");

	font = __gtkglut_fghFontByID(fontID);

	if (!((character >= 1) && (character < 256))) {
		return;
	}

	if (!font) {
		return;
	}

	/*
	 * Find the character we want to draw (???)
	 */
	face = font->Characters[character];

	glPushClientAttrib(GL_CLIENT_PIXEL_STORE_BIT);
	glPixelStorei(GL_UNPACK_SWAP_BYTES, GL_FALSE);
	glPixelStorei(GL_UNPACK_LSB_FIRST, GL_FALSE);
	glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
	glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
	glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glBitmap(face[0], font->Height,	/* The bitmap's width and height  */
		 font->xorig, font->yorig,	/* The origin in the font glyph   */
		 (float)(face[0]), 0.0,	/* The raster advance -- inc. x,y */
		 (face + 1)	/* The packed bitmap data...      */
	    );
	glPopClientAttrib();
}

/*!
 * \brief    Draw a string of bitmapped characters
 * \ingroup  bitmapfont
 * \param    fontID    A bitmapped font identifier.
 * \param    string  A NUL-terminated ASCII string.
 *
 * Draw a \a string the current OpenGL raster position
 * using a bitmapped \a font.  The raster position is advanced
 * by the width of the string.
 *
 * The starting raster position is used as
 * the left margin for multi-line strings.
 * Each newline character repositions the raster
 * position at the beginning of the next line.
 *
 * Nothing is drawn, and the raster position is unaffected when
 * either:
 * - \a font is not a valid GtkGLUT bitmap font
 * - \a string is an empty string or NULL pointer
 * - The current OpenGL raster position is invalid
 *
 * \see glRasterPos(), glutBitmapCharacter()
 */
void glutBitmapString(void *fontID, const unsigned char *string)
{
	unsigned char c;
	float x = 0.0f;
	__gtkglut_SFG_Font *font;

	__gtkglut_test_inicialization("glutBitmapString");

	font = __gtkglut_fghFontByID(fontID);

	if (!font) {
		return;
	}

	if (!string || !*string)
		return;

	glPushClientAttrib(GL_CLIENT_PIXEL_STORE_BIT);
	glPixelStorei(GL_UNPACK_SWAP_BYTES, GL_FALSE);
	glPixelStorei(GL_UNPACK_LSB_FIRST, GL_FALSE);
	glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
	glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
	glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	/*
	 * Step through the string, drawing each character.
	 * A newline will simply translate the next character's insertion
	 * point back to the start of the line and down one line.
	 */
	while ((c = *string++))
		if (c == '\n') {
			glBitmap(0, 0, 0, 0, -x, (float)-font->Height, NULL);
			x = 0.0f;
		} else {	/* Not an EOL, draw the bitmap character */

			const GLubyte *face = font->Characters[c];
			glBitmap(face[0], font->Height,	/* Bitmap's width and height    */
				 font->xorig, font->yorig,	/* The origin in the font glyph */
				 (float)(face[0]), 0.0,	/* The raster advance; inc. x,y */
				 (face + 1)	/* The packed bitmap data...    */
			    );
			x += (float)(face[0]);
		}

	glPopClientAttrib();
}

/*!
 * \brief    Return the width of a bitmapped character, in pixels.
 * \ingroup  bitmapfont
 * \param    fontID      A bitmapped font identifier.
 * \param    character A character code.
 *
 * Returns the horizontal OpenGL raster position
 * offset for a \a character in a bitmapped \a font.
 *
 * It is also an upper bound on the width of the bitmapped glyph
 * for \a character, though not all letters will use their full
 * width, especially fixed-width fonts.
 *
 * Returns 0 if the \a character is out of the inclusive
 * range [0,255] or if the \a font is invalid.
 *
 * \note In GLUT, some glyphs could render to the left of the
 *       starting position, in some fonts.  GtkGLUT's fonts all
 *       position all of their glyphs to start at, or to the right of,
 *       the initial position.
 *
 * \see glutBitmapCharacter(), glutBitmapLength(), glutBitmapHeight(),
 *      glutStrokeWidth()
 */
int glutBitmapWidth(void *fontID, int character)
{
	__gtkglut_SFG_Font *font;
	__gtkglut_test_inicialization("glutBitmapWidth");

	font = __gtkglut_fghFontByID(fontID);

	if (!((character > 0 && character < 256))) {
		return 0;
	}

	if (!font) {
		return 0;
	}

	return *(font->Characters[character]);
}

/*!
 * \brief    Return the width of a bitmapped string, in pixels.
 * \ingroup  bitmapfont
 * \param    fontID    A bitmapped font identifier.
 * \param    string  A NUL-terminated ASCII string.
 *
 * Returns the maximum horizontal OpenGL raster position
 * offset for a \a string in a bitmapped \a font.
 *
 * As with glutBitmapString(), newlines are taken into
 * consideration.
 *
 * Returns 0 if the \a font is invalid or if the
 * \a string is empty or \a NULL.
 *
 * \see glutBitmapString(), glutBitmapWidth(), glutBitmapHeight(),
 *      glutStrokeLength()
 */
int glutBitmapLength(void *fontID, const unsigned char *string)
{
	unsigned char c;
	int length = 0, this_line_length = 0;
	__gtkglut_SFG_Font *font;
	__gtkglut_test_inicialization("glutBitmapLength");
	font = __gtkglut_fghFontByID(fontID);
	if (!font)
		return 0;

	if (!string || !*string)
		return 0;

	while ((c = *string++)) {
		if (c != '\n')	/* Not an EOL, increment length of line */
			this_line_length += *(font->Characters[c]);
		else {		/* EOL; reset the length of this line */

			if (length < this_line_length)
				length = this_line_length;
			this_line_length = 0;
		}
	}
	if (length < this_line_length)
		length = this_line_length;

	return length;
}

/*!
 * \brief    Return the height of a given font, in pixels.
 * \ingroup  bitmapfont
 * \param    fontID    A bitmapped font identifier.
 *
 * Return the line-to-line vertical spacing (in pixels)
 * between lines of a bitmapped \a font.
 *
 * Returns 0 if \a font is invalid.
 *
 * \note Does *not* report the height used by individual
 *       characters.  This may limit its usefulness.  (Compare
 *       with other font-metric queries.)
 * \see  glutBitmapCharacter(), glutBitmapString(), glutBitmapWidth(),
 *       glutBitmapLength(), glutStrokeHeight()
 *
 */
int glutBitmapHeight(void *fontID)
{
	__gtkglut_SFG_Font *font;

	__gtkglut_test_inicialization("glutBitmapHeight");

	font = __gtkglut_fghFontByID(fontID);
	if (!font)
		return 0;

	return font->Height;
}

/*!
 * \defgroup fonts Text Rendering - Stroke Fonts
 *
 * GtkGLUT inherits from freeglut and GLUT a very small number
 * of standard fonts.  Some are bitmapped, some are stroked.
 * Some use fixed-width positioning, some do not.  GtkGLUT
 * offers single-character output and string output, as well as
 * rudimentary font metrics: Width of a character, height of
 * the entire font, and length of a string.
 *
 * The stroked fonts are drawn in model space
 * coordinates.
 *
 * Along with their brief descriptions, the fonts are:
 * - \a GLUT_STROKE_ROMAN          A **non-**fixed-width stroked font.
 * - \a GLUT_STROKE_MONO_ROMAN     A fixed-width stroked font.
 *
 * The stroked range from a maximum height of 119.05 model space units to
 * a maximum descender below the baseline of 33.33 units.  The fixed-width
 * stroked font characters are all 104.76 units wide.  These fonts are
 * from the PEX set, and include only the printable ASCII characters
 * from the space (decimal value 32) to the tilde (value 126).
 */

/*!
 * \brief    Draw a stroked character.
 * \ingroup  fonts
 * \param    fontID    A GLUT stroked font identifier.
 * \param    character An ASCII character other than NUL.
 *
 * This function draws one \a character from one stroked font
 * (selected by \a fontID)
 * using OpenGL \a GL_LINE_STRIP.  These characters
 * are drawn at the origin in model space.
 * The the model space origin is translated at the end,
 * according to the \a character width in \a fontID.
 *
 * Does nothing if:
 * - The \a fontID is invalid.
 * - The \a character is out of the font's range.
 *
 * \see glBegin(), glTranslatef(), glutStrokeWidth(), glutStrokeString(),
 *      glutStrokeHeight(), glutBitmapCharacter()
 */
void glutStrokeCharacter(void *fontID, int character)
{
	const __gtkglut_SFG_StrokeChar *schar;
	const __gtkglut_SFG_StrokeStrip *strip;
	int i, j;
	__gtkglut_SFG_StrokeFont *font;

	__gtkglut_test_inicialization("glutStrokeCharacter");
	font = __gtkglut_fghStrokeByID(fontID);

	if (!(character >= 0)) {
		return;
	}

	if (!(character < font->Quantity))
		return;
	if (!font)
		return;

	schar = font->Characters[character];
	if (!schar)
		return;
	strip = schar->Strips;

	for (i = 0; i < schar->Number; i++, strip++) {
		glBegin(GL_LINE_STRIP);
		for (j = 0; j < strip->Number; j++)
			glVertex2f(strip->Vertices[j].X, strip->Vertices[j].Y);
		glEnd();
	}
	glTranslatef(schar->Right, 0.0, 0.0);
}

/*!
 * \brief    Draw a string of stroked characters.
 * \ingroup  fonts
 * \param    fontID    A GLUT stroked font identifier.
 * \param    string    A NUL-terminated ASCII string.
 *
 * this function draws a \a string in the font indicated
 * by \a fontid.
 * it is *almost* equivalent to calling glutStrokeCharacter()
 * on each character in the \a string, successively.
 * mostly, it is a convenience function to hide the loop,
 * and to treat \\n as a special symbol rather than a normal
 * glyph.
 *
 * the first character displays at the current model space
 * origin,  the origin changes by successive translations.
 *
 * the newline character, \\n (ascii lf) is treated as
 * a newline and resets the origin horizontally
 * while advancing the line 1 font-height down the y-axis.
 *
 * does nothing if:
 * - \a fontid is out of range.
 * - \a string is \a null
 * - \a string is empty
 *
 * unlike glutBitmapString(), there is little performance
 * advantage to using glutStrokeString() as compared with
 * calling glutStrokeCharacter() yourself for every
 * character.
 *
 * \see glutStrokeLength(), glutStrokeCharacter(),
 *      glutStrokeHeight(), glutBitmapString()
 */
void glutStrokeString(void *fontID, const unsigned char *string)
{
	unsigned char c;
	int i, j;
	float length = 0.0;
	__gtkglut_SFG_StrokeFont *font;

	__gtkglut_test_inicialization("glutStrokeString");

	font = __gtkglut_fghStrokeByID(fontID);
	if (!font)
		return;
	if (!string || !*string)
		return;

	/*
	 * Step through the string, drawing each character.
	 * A newline will simply translate the next character's insertion
	 * point back to the start of the line and down one line.
	 */
	while ((c = *string++))
		if (c < font->Quantity) {
			if (c == '\n') {
				glTranslatef(-length, -(float)(font->Height), 0.0);
				length = 0.0;
			} else {	/* Not an EOL, draw the bitmap character */

				const __gtkglut_SFG_StrokeChar *schar = font->Characters[c];
				if (schar) {
					const __gtkglut_SFG_StrokeStrip *strip = schar->Strips;

					for (i = 0; i < schar->Number; i++, strip++) {
						glBegin(GL_LINE_STRIP);
						for (j = 0; j < strip->Number; j++)
							glVertex2f(strip->Vertices[j].X,
								   strip->Vertices[j].Y);

						glEnd();
					}

					length += schar->Right;
					glTranslatef(schar->Right, 0.0, 0.0);
				}
			}
		}
}

/*!
 * \brief    Returns the width in pixels of a character in a given font.
 * \ingroup  fonts
 * \param    fontID    A GLUT stroked font identifier.
 * \param    character A character code.
 *
 * This function reports how far the model space origin will advance
 * if you putput this \a character in the font named by \a fontID.
 * Not all letters will use their full width, especially in
 * fixed-width fonts.
 *
 * Returns 0 if \a character is out of range or if the
 * \a fontID is invalid.
 *
 * \todo Determine if any glyphs are either wider than this
 *       function or if they render outside of the bounding
 *       box given by
 *       *(0,-descent)* by *(width,height-descent)*.
 * \note Historically, this function has been described as
 *       returning a pixel-width, but was implemented to
 *       return the width in model-space units, rounded to integers.
 *       GLUT never resolved this, and freeglut duplicated the
 *       confusion.
 *       GtkGLUT has decided to stay in model-space and to
 *       return the unrounded floating point value.
 *       An unreleased GLUT 3.8 was supposed to include
 *       glutStrokeWidthf() and glutStrokeLengthf() (note
 *       the *f suffixes), but that is not in wide use.
 * \see glutStrokeCharacter(), glutStrokeLength(), glutStrokeHeight()
 *      glutBitmapWidth()
 */
int glutStrokeWidth(void *fontID, int character)
{
	const __gtkglut_SFG_StrokeChar *schar;
	__gtkglut_SFG_StrokeFont *font;

	__gtkglut_test_inicialization("glutStrokeWidth");
	font = __gtkglut_fghStrokeByID(fontID);

	if (!((character >= 0) && (character < font->Quantity)))
		return 0;
	if (!font)
		return 0;
	schar = font->Characters[character];
	if (!schar)
		return 0;

	return (int)(schar->Right + 0.5);
}

/*!
 * \brief    Returns model space width of a string in a given font.
 * \ingroup  fonts
 * \param    fontID    A GLUT stroked font identifier.
 * \param    string    A C-style (NUL-terminated) string.
 *
 * This function reports the sum of the widths of the
 * characters in a \a string, using the font metrics of
 * a given \a font.
 *
 * Like glutStrokeString(), glutStrokeLength() respects
 * newlines in the input.
 *
 * Returns 0 if:
 * - The \a fontID is out of range.
 * - The \a string is \a NULL.
 * - All characters in the \a string are zero-width.
 *
 * \note Refer to glutStrokeWidth() for notes on the
 *       nature of this function's return value, and for
 *       comparisons to old GLUT and freeglut.
 * \see glutStrokeString(), glutStrokeWidth(), glutStrokeHeight(),
 *      glutBitmapLength()
 */
int glutStrokeLength(void *fontID, const unsigned char *string)
{
	unsigned char c;
	float length = 0.0;
	float this_line_length = 0.0;
	__gtkglut_SFG_StrokeFont *font;

	__gtkglut_test_inicialization("glutStrokeLength");
	font = __gtkglut_fghStrokeByID(fontID);
	if (!font)
		return 0;

	if (!string || !*string)
		return 0;

	while ((c = *string++))
		if (c < font->Quantity) {
			if (c == '\n') {	/* EOL; reset the length of this line */
				if (length < this_line_length)
					length = this_line_length;
				this_line_length = 0.0;
			} else {	/* Not an EOL, increment the length of this line */

				const __gtkglut_SFG_StrokeChar *schar = font->Characters[c];
				if (schar)
					this_line_length += schar->Right;
			}
		}
	if (length < this_line_length)
		length = this_line_length;

	return (int)(length + 0.5);
}

/*!
 * \brief    Returns the height of a given font.
 * \ingroup  fonts
 * \param    fontID    A GLUT stroked font identifier.
 *
 * This function reports the height of a font,
 * given by \a fontID,
 * as a global characteristic of that font.
 *
 * Returns 0 if \a fontID is invalid.
 *
 * \note Does *not* report the height used by individual
 *       characters.  This may limit its usefulness; perhaps we
 *       should change it?  (And/or add a new function.)
 * \todo We have discussed adding a "font descender" query.
 *       We should go ahead and do it.
 * \see glutStrokeCharacter(), glutStrokeString(), glutStrokeWidth(),
 *      glutStrokeLength(), glutBitmapHeight()
 */
GLfloat glutStrokeHeight(void *fontID)
{
	__gtkglut_SFG_StrokeFont *font;

	__gtkglut_test_inicialization("glutStrokeHeight");

	font = __gtkglut_fghStrokeByID(fontID);
	if (!font)
		return 0.0;

	return font->Height;
}
