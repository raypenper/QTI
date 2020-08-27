#ifndef GDISTANCE_H
#define GDISTANCE_H

#include "core_global.h"

class GVector2d;
class GVector3d;

// 2D距离
double CORE_EXPORT gDistance2D(double x0, double y0, double x1, double y1); 

// 2D距离
double CORE_EXPORT gDistance2D(const GVector2d &pt0, const GVector2d &pt1); 

// 2D点到直线的距离
double CORE_EXPORT gPtLineDistance2D(const GVector2d &pt, const GVector2d &lpt0, 
								     const GVector2d &lpt1, GVector2d *pPm = 0);

// 2D点到线段的距离
// (pt: 已知点; lpt0, lpt1: 线段端点; pPm存放线段上最近点)
double CORE_EXPORT gPtSegDistance2D(const GVector2d &pt, const GVector2d &lpt0, 
							        const GVector2d &lpt1, GVector2d *pPm = 0);

// 3D距离
double CORE_EXPORT gDistance3D(const GVector3d &v0, const GVector3d &v1);

// 3D点到直线的距离
double CORE_EXPORT gPtLineDistance3D(const GVector3d &pt, const GVector3d &lpt0, 
								     const GVector3d &lpt1, GVector3d *pPm = 0);

// 3D点到线段的距离
// (pt: 已知点; lpt0, lpt1: 线段端点; pPm存放线段上最近点)
double CORE_EXPORT gPtSegDistance3D(const GVector3d &pt, const GVector3d &lpt0, 
									const GVector3d &lpt1, GVector3d *pPm = 0);

//3D空间，两线段之间的最短距离距离
//(pt1，pt2,两个线段中点坐标，lpt0,lpt1,第一段线段的两个端点，lpt2,lpt3,第二段线段两端端点,pPm,pPn存放最近的两点)
bool CORE_EXPORT gSegSegDistance3D(const GVector3d &lpt0, const GVector3d &lpt1, 
									const GVector3d &lpt2, const GVector3d &lpt3,
									double &d, GVector3d *pPm0 = 0, GVector3d *pPm1 = 0);

// TODO: buggy
bool CORE_EXPORT gLineSegDistance(const GVector3d &lpt0, 
								  const GVector3d &lpt1, 
								  const GVector3d &spt0, 
								  const GVector3d &spt1,
								  double &d, 
								  GVector3d *pPm0, 
								  GVector3d *pPm1);

bool CORE_EXPORT gIsPointOnTri(const GVector3d& point,
							   const GVector3d vertices[3],
							   const double eps = 1e-8);

bool CORE_EXPORT gIsPointOnLine(const GVector3d& l0, const GVector3d& l1, const GVector3d& pt, double eps = 1e-8);

// 判断点pt是否在[seg0, seg1]所在的线段上
// 建议将误差EPSILON作为参数
bool CORE_EXPORT gIsPointOnSeg(const GVector3d& seg0, const GVector3d& seg1, const GVector3d& pt, double eps = 1e-8);

#endif