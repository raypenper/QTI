#ifndef GDISTANCE_H
#define GDISTANCE_H

#include "core_global.h"

class GVector2d;
class GVector3d;

// 2D����
double CORE_EXPORT gDistance2D(double x0, double y0, double x1, double y1); 

// 2D����
double CORE_EXPORT gDistance2D(const GVector2d &pt0, const GVector2d &pt1); 

// 2D�㵽ֱ�ߵľ���
double CORE_EXPORT gPtLineDistance2D(const GVector2d &pt, const GVector2d &lpt0, 
								     const GVector2d &lpt1, GVector2d *pPm = 0);

// 2D�㵽�߶εľ���
// (pt: ��֪��; lpt0, lpt1: �߶ζ˵�; pPm����߶��������)
double CORE_EXPORT gPtSegDistance2D(const GVector2d &pt, const GVector2d &lpt0, 
							        const GVector2d &lpt1, GVector2d *pPm = 0);

// 3D����
double CORE_EXPORT gDistance3D(const GVector3d &v0, const GVector3d &v1);

// 3D�㵽ֱ�ߵľ���
double CORE_EXPORT gPtLineDistance3D(const GVector3d &pt, const GVector3d &lpt0, 
								     const GVector3d &lpt1, GVector3d *pPm = 0);

// 3D�㵽�߶εľ���
// (pt: ��֪��; lpt0, lpt1: �߶ζ˵�; pPm����߶��������)
double CORE_EXPORT gPtSegDistance3D(const GVector3d &pt, const GVector3d &lpt0, 
									const GVector3d &lpt1, GVector3d *pPm = 0);

//3D�ռ䣬���߶�֮�����̾������
//(pt1��pt2,�����߶��е����꣬lpt0,lpt1,��һ���߶ε������˵㣬lpt2,lpt3,�ڶ����߶����˶˵�,pPm,pPn������������)
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

// �жϵ�pt�Ƿ���[seg0, seg1]���ڵ��߶���
// ���齫���EPSILON��Ϊ����
bool CORE_EXPORT gIsPointOnSeg(const GVector3d& seg0, const GVector3d& seg1, const GVector3d& pt, double eps = 1e-8);

#endif