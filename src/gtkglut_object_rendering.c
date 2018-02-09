/*!
    \file  gtkglut_object_rendering.c
    \brief Algorithmic geometric objects.
*/

/*
 * GtkGLUT geometry rendering methods.
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
 * Contains some little modified code from FreeGLUT, see COPYING file
 */

#include "GL/gtkglut.h"
#include "gtkglut_internal.h"
#include <math.h>

/*! \defgroup geometry Geometric Rendering

    This subset of GtkGLUT provides some elementary objects that
    are more or less standard fare for computer graphics.
    The objects all provide coordinates and surface
    normals.
*/

/*!
    \ingroup  geometry
    \brief    Draw a solid sphere centered at the origin.
    \param    radius        Sphere radius.
    \param    slices        The number of divisions around the z axis.
                            (latitudal)
    \param    stacks        The number of divisions along the z axis.
                            (longitudal)

              The glutSolidSphere() function draws a shaded sphere centered at
              the origin.  The surface is created from quadrangles
              (except for triangles as degenerate quads at the poles) in a
              longitude/latitude pattern.  The equatorial great circle lies
              in the xy-plane and is centered on the origin.

    \note     The number of polygons representing the spherical surface is
              proportional to (slices*stacks).

    \see glutWireSphere()
*/
void glutSolidSphere(GLdouble radius,GLint slices, GLint stacks) {
  __gtkglut_test_inicialization("glutSolidSphere");

  gdk_gl_draw_sphere(TRUE,radius,slices,stacks);
}

/*!
    \ingroup  geometry
    \brief    Draw a wireframe sphere centered at the origin.
    \param    radius        Sphere radius.
    \param    slices        The number of divisions around the z axis.
                            (latitudal)
    \param    stacks        The number of divisions along the z axis.
                            (longitudal)

              The glutWireSphere() function draws a wireframe sphere centered
              at the origin.
              The "equatorial" great circle lies in the xy-plane.

    \note     The number of line segments representing the spherical surface is
              proportional to (slices*stacks).

    \see      glutSolidSphere().
*/
void glutWireSphere(GLdouble radius,GLint slices, GLint stacks) {
  __gtkglut_test_inicialization("glutWireSphere");

  gdk_gl_draw_sphere(FALSE,radius,slices,stacks);
}


/*!
    \brief    Draw a solid cube centered at the origin.
    \ingroup  geometry
    \param    size       The width, height and depth of the cube.

              The glutSolidCube() function draws a solid-shaded cube
              with side-length given by \a width.  The vertices of
              the cube are at
              (+/- \a width/2, +/- \a width/2, +/- \a width/2),
              so that the cube is centered at the origin.

    \see      glutWireCube()
*/
void glutSolidCube(GLdouble size) {
  __gtkglut_test_inicialization("glutSolidCube");

  gdk_gl_draw_cube(TRUE,size);
}

/*!
    \brief    Draw a wireframe cube centered at the origin.
    \ingroup  geometry
    \param    size       The width, height and depth of the cube.

              The glutWireCube() function draws an axis-aligned wireframe cube
              with a specified width, height and depth. The vertices of
              the cube are at
              (+/- \a width/2, +/- \a width/2, +/- \a width/2),
              so that the cube is centered at the origin.

    \see      glutSolidCube()
*/
void glutWireCube(GLdouble size) {
  __gtkglut_test_inicialization("glutWireCube");

  gdk_gl_draw_cube(FALSE,size);
}

/*!
    \brief    Draw a solid cone.
    \ingroup  geometry
    \param    base       Cone radius at the base in the xy plane.
    \param    height     Height of cone in positive z direction.
    \param    slices     The number of divisions around the z axis. (latitudal)
    \param    stacks     The number of divisions along the z axis. (longitudal)

              The glutSolidCone() function draws a shaded cone
              with a base in the xy-plane, oriented in the positive z
              direction.

    \note     The number of polygons representing the conical surface is
              proportional to (slices*stacks).

    \see      glutWireCone()
*/
void glutSolidCone(GLdouble base, GLdouble height,GLint slices, GLint stacks) {
  __gtkglut_test_inicialization("glutSolidCone");

  gdk_gl_draw_cone(TRUE,base,height,slices,stacks);
}

/*!
    \brief    Draw a wireframe cone.
    \ingroup  geometry
    \param    base       Cone radius at the base in the xy plane.
    \param    height     Height of cone in positive z direction.
    \param    slices     The number of divisions around the z axis. (latitudal)
    \param    stacks     The number of divisions along the z axis. (longitudal)

              The glutWireCone() function draws a wireframe cone
              with a base in the xy plane oriented in positive z.

    \note     The number of line segments representing the conical surface is
              proportional to (slices*stacks).

    \see      glutSolidCone()
*/
void glutWireCone(GLdouble base, GLdouble height,GLint slices, GLint stacks) {
  __gtkglut_test_inicialization("glutWireCone");

  gdk_gl_draw_cone(FALSE,base,height,slices,stacks);
}

/*!
    \brief    Draw a solid torus.
    \ingroup  geometry
    \param    innerRadius    Radius of ``tube''
    \param    outerRadius    Radius of ``path''
    \param    nsides          Facets around ``tube''
    \param    rings          Joints along ``path''

              This function effectively wraps a cylinder with \a nSides slats
              and bends it at \a nRings facets around a circular
              path, forming a torus, or ``donut''.  The center is
              at the origin and the ``path'' rings around the
              z axis.

              The torus parameters can be explored interactively
              with the GtkGLUT shapes demo.

    \note     \a dInnerRadius and \a dOuterRadius are <b>not</b>
              analogous to similar measurements of an anulus.

    \see      glutWireTorus()

*/
void glutSolidTorus(GLdouble innerRadius, GLdouble outerRadius, GLint nsides, GLint rings) {
  __gtkglut_test_inicialization("glutSolidTorus");

  gdk_gl_draw_torus(TRUE,innerRadius,outerRadius,nsides,rings);
}

/*!
    \brief    Draw a wireframe torus.
    \ingroup  geometry
    \param    innerRadius    Radius of ``tube''
    \param    outerRadius    Radius of ``path''
    \param    nsides          Facets around ``tube''
    \param    rings          Joints along ``path''

              This function effectively wraps a cylinder with \a nSides slats
              and bends it at \a nRings facets around a circular
              path, forming a torus, or ``donut''.  The center is
              at the origin and the ``path'' rings around the
              z axis.

              The torus parameters can be explored interactively
              with the GtkGLUT shapes demo.

    \note     \a dInnerRadius and \a dOuterRadius are <b>not</b>
              analogous to similar measurements of an anulus.

    \see      glutSolidTorus()

*/
void glutWireTorus(GLdouble innerRadius, GLdouble outerRadius, GLint nsides, GLint rings) {
  __gtkglut_test_inicialization("glutWireTorus");

  gdk_gl_draw_torus(FALSE,innerRadius,outerRadius,nsides,rings);
}

/*!
    \brief      Draw a solid dodecahedron.
    \ingroup    geometry

                This function draws a regular, solid, 12-sided polyhedron
                centered at the origin.
                The distance from the origin to the vertices is
                sqrt(3).
                The facets are pentagons.

    \see        glutWireDodecahedron(), glutSolidRhombicDodecahedron(),
                glutWireRhombicDodecahedron()

*/
void glutSolidDodecahedron(void) {
  __gtkglut_test_inicialization("glutSolidDodecahedron");

  gdk_gl_draw_dodecahedron(TRUE);
}

/*!
    \brief      Draw a wireframe dodecahedron.
    \ingroup    geometry

                This function draws a regular, wireframe 12-sided polyhedron
                centered at the origin.
                The distance from the origin to the vertices is
                sqrt(3).
                The facets are pentagons.
                No facet is normal any of the \a x, \a y, or \a z
                axes.

    \see        glutSolidDodecahedron(), glutWireRhombicDodecahedron(),
                glutSolidRhombicDodecahedron()

*/
void glutWireDodecahedron(void) {
  __gtkglut_test_inicialization("glutWireDodecahedron");

  gdk_gl_draw_dodecahedron(FALSE);
}

/*!
    \brief      Draw a solid octahedron.
    \ingroup    geometry

                This function draws a regular, solid 8-sided polyhedron
                centered at the origin.
                The vertices are at
                 (+/-1, 0, 0),
                 (0, +/-1, 0),
                 (0, 0, +/-1).

    \note       We visit the same vertices the same number of times
                as in the wire octahedron, but the order is different.

    \see        glutWireOctahedron()

*/
void glutSolidOctahedron(void) {
  __gtkglut_test_inicialization("glutSolidOctahedron");

  gdk_gl_draw_octahedron(TRUE);
}

/*!
    \brief      Draw a wireframe octahedron.
    \ingroup    geometry

                This function draws a regular wireframe 8-sided polyhedron
                centered at the origin.
                The vertices are at
                 (+/-1, 0, 0),
                 (0, +/-1, 0),
                 (0, 0, +/-1).

    \note       We visit the same vertices the same number of times
                as for the solid octahedron, but the order is different.
    \note       Draws every edge twice.
    \note       The lines have normals, but the normals are from the
                facets, rather than upon the edge.  If you squint too
                hard, the lighting on a wireframe octahedron does
                not look quite right.
    \todo       It may be faster (and look better) to draw each edge
                once, setting the Normal at each edge.  (I don't
                think that this matters all that much, but a lineloop
                was proposed for the wire Cube for speed.)
    \see        glutSolidOctahedron()

*/
void glutWireOctahedron(void) {
  __gtkglut_test_inicialization("glutWireOctahedron");

  gdk_gl_draw_octahedron(FALSE);
}

/*!
    \brief      Draw a solid tetrahedron.
    \ingroup    geometry

                This function draws a regular, solid 4-sided polyhedron
                centered at the origin.
                The distance from the origin to the vertices is 1.

    \todo       See todo-list on glutWireTetrahedron().
    \see        glutWireTetrahedron()

*/
void glutSolidTetrahedron(void) {
  __gtkglut_test_inicialization("glutSolidTetrahedron");

  gdk_gl_draw_tetrahedron(TRUE);
}

/*!
    \brief      Draw a wireframe tetrahedron.
    \ingroup    geometry

                This function draws a regular, wireframe 4-sided polyhedron
                centered at the origin.
                The distance from the origin to the vertices is 1.

    \todo       Merge \a r0 \a r1 \a r2 and \a r3 into one array.
    \todo       Put the normals into the (or an) array.
    \todo       Make the array static const, with file scope, and share
                with glutSolidTetrahedron().
    \todo       Maybe consolidate with the SierpinskySponge?
    \see        glutSolidTetrahedron()

*/
void glutWireTetrahedron(void) {
  __gtkglut_test_inicialization("glutWireTetrahedron");

  gdk_gl_draw_tetrahedron(FALSE);
}

/*!
    \brief      Draw a solid icosahedron.
    \ingroup    geometry

                This function draws a regular, solid 20-sided polyhedron
                centered at the origin.
                The distance from the origin to the vertices is 1.

    \see        glutWireIcosahedron()
*/
void glutSolidIcosahedron(void) {
  __gtkglut_test_inicialization("glutSolidIcosahedron");

  gdk_gl_draw_icosahedron(TRUE);
}

/*!
    \brief      Draw a wireframe icosahedron.
    \ingroup    geometry

                This function draws a regular, solid 20-sided polyhedron
                centered at the origin.
                The distance from the origin to the vertices is 1.
                No facet is normal to any of the \a x, \a y, or \a z
                axes.

    \see        glutSolidIcosahedron()
*/
void glutWireIcosahedron(void) {
  __gtkglut_test_inicialization("glutWireIcosahedron");

  gdk_gl_draw_icosahedron(FALSE);
}

/*!
    \brief    Draw a solid teapot.
    \ingroup  geometry
    \param    size        Scale factor.

              Draws the standard Teapot, solid shaded, using OpenGL evaluators.
              This is the classic "Utah Teapot" of computer graphics.
              The base should lie in the xy-plane with "up" being
              along the positive z axis.

              This is derived from SGI code.  It should also
              be the same as the teapot modeled by Martin Newell
              in 1975.

    \see      glutWireTeapot()
*/
void glutSolidTeapot(GLdouble size) {
  __gtkglut_test_inicialization("glutSolidTeapot");

  gdk_gl_draw_teapot(TRUE,size);
}

/*!
    \brief    Draw a wireframe teapot.
    \ingroup  geometry
    \param    size        Scale factor.

              This function draws the standard Teapot in wireframe
              using OpenGL evaluators.
              This is the classic "Utah Teapot" of computer graphics.
              The base should lie in the xy-plane with "up" being
              along the positive z axis.

              This is derived from SGI code.  It should also
              be the same as the teapot modeled by Martin Newell
              in 1975.

    \see      glutSolidTeapot()
*/
void glutWireTeapot(GLdouble size) {
  __gtkglut_test_inicialization("glutWireTeapot");

  gdk_gl_draw_teapot(FALSE,size);
}


/*** PART OF FREEGLUT*/
#ifndef __GTKGLUT_M_PI
#    define  __GTKGLUT_M_PI  3.14159265358979323846
#endif


double __gtkglut_rdod_r[14][3] = { { 0.0, 0.0, 1.0 },
  {  0.707106781187,  0.000000000000,  0.5 }, {  0.000000000000,  0.707106781187,  0.5 }, { -0.707106781187,  0.000000000000,  0.5 }, {  0.000000000000, -0.707106781187,  0.5 },
  {  0.707106781187,  0.707106781187,  0.0 }, { -0.707106781187,  0.707106781187,  0.0 }, { -0.707106781187, -0.707106781187,  0.0 }, {  0.707106781187, -0.707106781187,  0.0 },
  {  0.707106781187,  0.000000000000, -0.5 }, {  0.000000000000,  0.707106781187, -0.5 }, { -0.707106781187,  0.000000000000, -0.5 }, {  0.000000000000, -0.707106781187, -0.5 },
  {  0.0, 0.0, -1.0 } } ;
int __gtkglut_rdod_v [12][4] = { { 0,  1,  5,  2 }, { 0,  2,  6,  3 }, { 0,  3,  7,  4 }, { 0,  4,  8, 1 },
                       { 5, 10,  6,  2 }, { 6, 11,  7,  3 }, { 7, 12,  8,  4 }, { 8,  9,  5, 1 },
                       { 5,  9, 13, 10 }, { 6, 10, 13, 11 }, { 7, 11, 13, 12 }, { 8, 12, 13, 9 } } ;
double __gtkglut_rdod_n[12][3] = {
  {  0.353553390594,  0.353553390594,  0.5 }, { -0.353553390594,  0.353553390594,  0.5 }, { -0.353553390594, -0.353553390594,  0.5 }, {  0.353553390594, -0.353553390594,  0.5 },
  {  0.000000000000,  1.000000000000,  0.0 }, { -1.000000000000,  0.000000000000,  0.0 }, {  0.000000000000, -1.000000000000,  0.0 }, {  1.000000000000,  0.000000000000,  0.0 },
  {  0.353553390594,  0.353553390594, -0.5 }, { -0.353553390594,  0.353553390594, -0.5 }, { -0.353553390594, -0.353553390594, -0.5 }, {  0.353553390594, -0.353553390594, -0.5 }
  } ;


/*!
    \brief    Draw a wireframe rhombic dodecahedron
    \ingroup  geometry

              This function draws a wireframe dodecahedron whose
              facets are rhombic and
              whose vertices are at unit radius.
              No facet lies normal to any coordinate axes.
              The polyhedron is centered at the origin.

    \see      glutSolidRhombicDodecahedron(), glutWireDodecahedron(),
              glutSolidDodecahedron()
*/
void glutWireRhombicDodecahedron(void) {
  int i;

  __gtkglut_test_inicialization("glutWireRhombicDodecahedron");

  for ( i = 0; i < 12; i++ ) {
    glBegin ( GL_LINE_LOOP ) ;
      glNormal3dv ( __gtkglut_rdod_n[i] ) ;
      glVertex3dv ( __gtkglut_rdod_r[__gtkglut_rdod_v[i][0]] ) ;
      glVertex3dv ( __gtkglut_rdod_r[__gtkglut_rdod_v[i][1]] ) ;
      glVertex3dv ( __gtkglut_rdod_r[__gtkglut_rdod_v[i][2]] ) ;
      glVertex3dv ( __gtkglut_rdod_r[__gtkglut_rdod_v[i][3]] ) ;
    glEnd () ;
  }
}

/*!
    \brief    Draw a solid rhombic dodecahedron
    \ingroup  geometry

              This function draws a solid-shaded dodecahedron
              whose facets are rhombic and
              whose vertices are at unit radius.
              No facet lies normal to any coordinate axes.
              The polyhedron is centered at the origin.

    \see      glutWireRhombicDodecahedron(), glutWireDodecahedron(),
              glutSolidDodecahedron()
*/
void  glutSolidRhombicDodecahedron(void) {
 int i ;

  __gtkglut_test_inicialization("glutSolidRhombicDodecahedron");

  glBegin ( GL_QUADS ) ;
  for ( i = 0; i < 12; i++ ) {
    glNormal3dv ( __gtkglut_rdod_n[i] ) ;
    glVertex3dv ( __gtkglut_rdod_r[__gtkglut_rdod_v[i][0]] ) ;
    glVertex3dv ( __gtkglut_rdod_r[__gtkglut_rdod_v[i][1]] ) ;
    glVertex3dv ( __gtkglut_rdod_r[__gtkglut_rdod_v[i][2]] ) ;
    glVertex3dv ( __gtkglut_rdod_r[__gtkglut_rdod_v[i][3]] ) ;
  }

  glEnd () ;
}

/* Magic Numbers:  r0 = ( 1, 0, 0 )
 *                 r1 = ( -1/3, 2 sqrt(2) / 3, 0 )
 *                 r2 = ( -1/3, -sqrt(2) / 3, sqrt(6) / 3 )
 *                 r3 = ( -1/3, -sqrt(2) / 3, -sqrt(6) / 3 )
 * |r0| = |r1| = |r2| = |r3| = 1
 * Distance between any two points is 2 sqrt(6) / 3
 *
 * Normals:  The unit normals are simply the negative of the coordinates of the point not on the surface.
 */

#define __GTKGLUT_NUM_TETR_FACES     4

static GLdouble __gtkglut_tet_r[4][3] = { {             1.0,             0.0,             0.0 },
                                { -0.333333333333,  0.942809041582,             0.0 },
                                { -0.333333333333, -0.471404520791,  0.816496580928 },
                                { -0.333333333333, -0.471404520791, -0.816496580928 } } ;

static GLint __gtkglut_tet_i[4][3] =  /* Vertex indices */
{
  { 1, 3, 2 }, { 0, 2, 3 }, { 0, 3, 1 }, { 0, 1, 2 }
} ;

/*!
    \brief    Draw a wireframe Spierspinski's sponge.
    \ingroup  geometry
    \param    num_levels    Recursive depth.
    \param    offset        Location vector.
    \param    scale         Relative size.

              This function recursively draws a few levels of
              Sierpinski's Sponge
              in wireframe.
              If \a num_levels is 0, draws 1 tetrahedron.
              The \a offset is a translation.
              The \a z axis is normal to the base.
              The sponge is centered at the origin.

    \note     Runtime is exponential in \a num_levels .

    \see      glutSolidSierpinskiSponge()
*/
void glutWireSierpinskiSponge (int num_levels,GLdouble offset[3],GLdouble scale) {
  int i, j ;

  __gtkglut_test_inicialization("glutWireSierpinskiSponge");

  if ( num_levels == 0 )
  {

    for ( i = 0 ; i < __GTKGLUT_NUM_TETR_FACES ; i++ )
    {
      glBegin ( GL_LINE_LOOP ) ;
      glNormal3d ( -__gtkglut_tet_r[i][0], -__gtkglut_tet_r[i][1], -__gtkglut_tet_r[i][2] ) ;
      for ( j = 0; j < 3; j++ )
      {
        double x = offset[0] + scale * __gtkglut_tet_r[__gtkglut_tet_i[i][j]][0] ;
        double y = offset[1] + scale * __gtkglut_tet_r[__gtkglut_tet_i[i][j]][1] ;
        double z = offset[2] + scale * __gtkglut_tet_r[__gtkglut_tet_i[i][j]][2] ;
        glVertex3d ( x, y, z ) ;
      }

      glEnd () ;
    }
  }
  else
  {
    GLdouble local_offset[3] ;  /* Use a local variable to avoid buildup of roundoff errors */
    num_levels -- ;
    scale /= 2.0 ;
    for ( i = 0 ; i < __GTKGLUT_NUM_TETR_FACES ; i++ )
    {
      local_offset[0] = offset[0] + scale * __gtkglut_tet_r[i][0] ;
      local_offset[1] = offset[1] + scale * __gtkglut_tet_r[i][1] ;
      local_offset[2] = offset[2] + scale * __gtkglut_tet_r[i][2] ;
      glutWireSierpinskiSponge ( num_levels, local_offset, scale ) ;
    }
  }
}

/*!
    \brief    Draw a solid Spierspinski's sponge.
    \ingroup  geometry
    \param    num_levels    Recursive depth.
    \param    offset        Location vector.
    \param    scale         Relative size.

              This function recursively draws a few levels of
              a solid-shaded Sierpinski's Sponge.
              If \a num_levels is 0, draws 1 tetrahedron.
              The \a offset is a translation.
              The \a z axis is normal to the base.
              The sponge is centered at the origin.

    \note     Runtime is exponential in \a num_levels .

    \todo     Consider removing the \a offset parameter from the
              API (use a helper function).

    \see      glutWireSierpinskiSponge()
*/
void glutSolidSierpinskiSponge ( int num_levels, GLdouble offset[3], GLdouble scale ) {
  int i, j ;

  __gtkglut_test_inicialization("glutSolidSierpinskiSponge");

  if ( num_levels == 0 )
  {
    glBegin ( GL_TRIANGLES ) ;

    for ( i = 0 ; i < __GTKGLUT_NUM_TETR_FACES ; i++ )
    {
      glNormal3d ( -__gtkglut_tet_r[i][0], -__gtkglut_tet_r[i][1], -__gtkglut_tet_r[i][2] ) ;
      for ( j = 0; j < 3; j++ )
      {
        double x = offset[0] + scale * __gtkglut_tet_r[__gtkglut_tet_i[i][j]][0] ;
        double y = offset[1] + scale * __gtkglut_tet_r[__gtkglut_tet_i[i][j]][1] ;
        double z = offset[2] + scale * __gtkglut_tet_r[__gtkglut_tet_i[i][j]][2] ;
        glVertex3d ( x, y, z ) ;
      }
    }

    glEnd () ;
  }
  else
  {
    GLdouble local_offset[3] ;  /* Use a local variable to avoid buildup of roundoff errors */
    num_levels -- ;
    scale /= 2.0 ;
    for ( i = 0 ; i < __GTKGLUT_NUM_TETR_FACES ; i++ )
    {
      local_offset[0] = offset[0] + scale * __gtkglut_tet_r[i][0] ;
      local_offset[1] = offset[1] + scale * __gtkglut_tet_r[i][1] ;
      local_offset[2] = offset[2] + scale * __gtkglut_tet_r[i][2] ;
      glutSolidSierpinskiSponge ( num_levels, local_offset, scale ) ;
    }
  }
}

/*
 * Compute lookup table of cos and sin values forming a cirle
 *
 * Notes:
 *    It is the responsibility of the caller to free these tables
 *    The size of the table is (n+1) to form a connected loop
 *    The last entry is exactly the same as the first
 *    The sign of n can be flipped to get the reverse loop
 */
static void __gtkglut_circle_table(double **sint,double **cost,const int n)
{
    int i;

    /* Table size, the sign of n flips the circle direction */

    const int size = abs(n);

    /* Determine the angle between samples */

    const double angle = 2*__GTKGLUT_M_PI/(double)( ( n == 0 ) ? 1 : n );

    /* Allocate memory for n samples, plus duplicate of first entry at the end */

    *sint = (double *) calloc(sizeof(double), size+1);
    *cost = (double *) calloc(sizeof(double), size+1);

    /* Bail out if memory allocation fails, fgError never returns */

    if (!(*sint) || !(*cost))
    {
        free(*sint);
        free(*cost);
        __gtkglut_lowmem();
    }

    /* Compute cos and sin around the circle */

    (*sint)[0] = 0.0;
    (*cost)[0] = 1.0;

    for (i=1; i<size; i++)
    {
        (*sint)[i] = sin(angle*i);
        (*cost)[i] = cos(angle*i);
    }

    /* Last sample is duplicate of the first */

    (*sint)[size] = (*sint)[0];
    (*cost)[size] = (*cost)[0];
}

/*!
    \brief    Draw a wireframe cylinder.
    \ingroup  geometry
    \param    radius     Radius of cylinder.
    \param    height     Z height.
    \param    slices     Number of divisions around the z axis.
    \param    stacks     Number of divisions along the z axis.

              glutWireCylinder() draws a wireframe of a cylinder,
              the center of whose base is at the origin, and
              whose axis parallels the z axis.

    \see      glutSolidCylinder()
*/
void glutWireCylinder( GLdouble radius, GLdouble height, GLint slices, GLint stacks) {
  int i,j;

  /* Step in z and radius as stacks are drawn. */
  double z = 0.0;
  const double zStep = height / ( ( stacks > 0 ) ? stacks : 1 );

  /* Pre-computed circle */
  double *sint,*cost;

  __gtkglut_test_inicialization("glutWireCylinder");

  __gtkglut_circle_table(&sint,&cost,-slices);

  /* Draw the stacks... */

  for (i=0; i<=stacks; i++) {
    if (i==stacks)
      z = height;

     glBegin(GL_LINE_LOOP);

       for( j=0; j<slices; j++ )
          {
            glNormal3d(cost[j],        sint[j],        0.0);
            glVertex3d(cost[j]*radius, sint[j]*radius, z  );
          }

     glEnd();

     z += zStep;
  }

  /* Draw the slices */

  glBegin(GL_LINES);

      for (j=0; j<slices; j++)
      {
          glNormal3d(cost[j],        sint[j],        0.0   );
          glVertex3d(cost[j]*radius, sint[j]*radius, 0.0   );
          glVertex3d(cost[j]*radius, sint[j]*radius, height);
      }

  glEnd();

  /* Release sin and cos tables */

  free(sint);
  free(cost);
}

/*!
    \brief    Draw a solid cylinder.
    \ingroup  geometry
    \param    radius     Radius of the cylinder.
    \param    height     Z height.
    \param    slices     Divisions around z axis.
    \param    stacks     Divisions along z axis.

              glutSolidCylinder() draws a shaded cylinder,
              the center of whose base is at the origin and
              whose axis is along the positive z axis.

    \see      glutWireCylinder()
*/
void glutSolidCylinder( GLdouble radius, GLdouble height, GLint slices, GLint stacks) {
  int i,j;

  /* Step in z and radius as stacks are drawn. */

  double z0,z1;
  const double zStep = height / ( ( stacks > 0 ) ? stacks : 1 );

  /* Pre-computed circle */

  double *sint,*cost;

  __gtkglut_test_inicialization("glutSolidCylinder");

  __gtkglut_circle_table(&sint,&cost,-slices);

  /* Cover the base and top */

  glBegin(GL_TRIANGLE_FAN);
      glNormal3d(0.0, 0.0, -1.0 );
      glVertex3d(0.0, 0.0,  0.0 );
      for (j=0; j<=slices; j++)
        glVertex3d(cost[j]*radius, sint[j]*radius, 0.0);
  glEnd();

  glBegin(GL_TRIANGLE_FAN);
      glNormal3d(0.0, 0.0, 1.0   );
      glVertex3d(0.0, 0.0, height);
      for (j=slices; j>=0; j--)
        glVertex3d(cost[j]*radius, sint[j]*radius, height);
  glEnd();

  /* Do the stacks */

  z0 = 0.0;
  z1 = zStep;

  for (i=1; i<=stacks; i++)
  {
      if (i==stacks)
          z1 = height;

      glBegin(GL_QUAD_STRIP);
          for (j=0; j<=slices; j++ )
          {
              glNormal3d(cost[j],        sint[j],        0.0 );
              glVertex3d(cost[j]*radius, sint[j]*radius, z0  );
              glVertex3d(cost[j]*radius, sint[j]*radius, z1  );
          }
      glEnd();

      z0 = z1; z1 += zStep;
  }

  /* Release sin and cos tables */

  free(sint);
  free(cost);
}
