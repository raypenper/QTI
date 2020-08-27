#ifndef GPOINT3D_H
#define GPOINT3D_H

#include "gvector3d.h"
#include "core_global.h"

class GMatrix3d;
class GQuaternion3d;
class GRotation3d;

// 代表顶点的类型
enum GPointType
{
	gNormalPt = 0,   // 普通点
	gSourcePt = 1,   // 原始数据点
	gEdgePt   = 2,   // 边界点
	gIsectPt  = 4,   // 交线上点
	gCornerPt = 8,   // 边界上拐点
	gXCrossPt = 16,  // 两条交线的交点
	gRemovedPt = 32	 // 被删除的点
};


class CORE_EXPORT GPoint3d : public GVector3d
{
protected:
	int mUsrInt;						// 扩展整型字段(用于标识点类型)

public:
	GPoint3d();
	GPoint3d(double x, double y, double z, int d=0);
	GPoint3d(const double *v, int d=0);
	GPoint3d(const GPoint3d &v);
	GPoint3d(const GVector3d &v);

	void setXYZU(double x, double y, double z, int u)
	{ mTuple[0] = x;	mTuple[1] = y;	mTuple[2] = z;	mUsrInt = u; }
	
	void setPoint(const GPoint3d &v)
	{ *this = v; }

	int getUsrInt() const
	{ return mUsrInt; }

	void setUsrInt(int i)
	{ mUsrInt = i; }
	
	GPoint3d & operator = (const GPoint3d &other);

	GPoint3d & operator = (const GVector3d &other);
	
	GPoint3d transformTo(const GMatrix3d &tm) const;
 
    GPoint3d transformTo(const GQuaternion3d &q) const;
    
    GPoint3d transformTo(const GRotation3d &rot) const;
};

#endif
