#ifndef GPLANE3D_H
#define GPLANE3D_H

#include "gvector3d.h"
#include "gpoint3d.h"
#include "core_global.h"

class CORE_EXPORT GPlane3d  
{
protected:
	double mParams[4];		// ax + by + cz +d = 0;
	GVector3d mNormal;		// 法向量
private:
	void compute(const GVector3d &p);
public:
	GPlane3d();
	GPlane3d(const GVector3d &p1, const GVector3d &p2, const GVector3d &p3);
	GPlane3d(const GVector3d &p, const GVector3d &v);
	GPlane3d(const GPlane3d &p);
	virtual ~GPlane3d();

	void operator = (const GPlane3d &other);

	operator const double * () const
	{ return mParams; }

	operator double * ()
	{ return mParams; }

	void set(const GVector3d &p1, const GVector3d &p2, const GVector3d &p3);
	void set(const GVector3d &p, const GVector3d &v);

	const GVector3d & normal() const;
	double a() const { return mParams[0]; }
	double b() const { return mParams[1]; }
	double c() const { return mParams[2]; }
	double d() const { return mParams[3]; }
	
	bool getX(double y, double z, double &x);
	bool getY(double x, double z, double &y);
	bool getZ(double x, double y, double &z);
};

GPoint3d CORE_EXPORT gProjectToPlane(const GVector3d &pt3, const GPlane3d &plane, double *d = 0);

// 根据输入散点计算最小二乘平面
// pts:		散点数组
// n:		散点数目
// prjDirect:	预设投影方向(0: 未定义, 1: gZOX, 2: gXOY, 3: gYOZ)
GPlane3d CORE_EXPORT gGetLSPlane(const GPoint3d *pts, int n, int prjDirect=0);
GPlane3d CORE_EXPORT gGetLSPlane(const GVector3d *pts, int n, int prjDirect=0);

#endif
