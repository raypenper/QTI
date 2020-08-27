/* Triangle/triangle intersection test routine,
 * by Tomas Moller, 1997.
 * See article "A Fast Triangle-Triangle Intersection Test",
 * Journal of Graphics Tools, 2(2), 1997
 * updated: 2001-06-20 (added line of intersection)
 *
 * int tri_tri_intersect(double V0[3],double V1[3],double V2[3],
 *                       double U0[3],double U1[3],double U2[3])
 *
 * parameters: vertices of triangle 1: V0,V1,V2
 *             vertices of triangle 2: U0,U1,U2
 * result    : returns 1 if the triangles intersect, otherwise 0
 *
 * Here is a version withouts divisions (a little faster)
 * int NoDivTriTriIsect(double V0[3],double V1[3],double V2[3],
 *                      double U0[3],double U1[3],double U2[3]);
 * 
 * This version computes the line of intersection as well (if they are not coplanar):
 * int tri_tri_intersect_with_isectline(double V0[3],double V1[3],double V2[3], 
 *				        double U0[3],double U1[3],double U2[3],int *coplanar,
 *				        double isectpt1[3],double isectpt2[3]);
 * coplanar returns whether the tris are coplanar
 * isectpt1, isectpt2 are the endpoints of the line of intersection
 */
 
#ifndef GTRI_INTERSECT_H
#define GTRI_INTERSECT_H
#include "core_global.h"

// 检测两个三角形是否相交
// vertices of triangle 1: V0,V1,V2
// vertices of triangle 2: U0,U1,U2
// returns 1 if the triangles intersect, otherwise 0
int CORE_EXPORT tri_tri_intersect(const double V0[3], const double V1[3], const double V2[3], 
					              const double U0[3], const double U1[3], const double U2[3]);
					  
// 检测两个三角形是否相交, 并求出交线
// vertices of triangle 1: V0,V1,V2
// vertices of triangle 2: U0,U1,U2
// isectpt1, isectpt2 are the endpoints of the line of intersection
// returns 1 if the triangles intersect, otherwise 0
int CORE_EXPORT tri_tri_intersect_with_isectline(const double V0[3], const double V1[3], const double V2[3], 
									 const double U0[3], const double U1[3], const double U2[3], int *coplanar, 
									 double isectpt1[3], double isectpt2[3]);

int CORE_EXPORT tri_plane_intersect_with_isectline(double V0[3], double V1[3], double V2[3], 
												   double U0[3], double U1[3], double U2[3], int *coplanar, 
												   double isectpt1[3], double isectpt2[3]);
									 
#endif