#ifndef GRAY_H
#define GRAY_H

#include "gvector3d.h"
#include "core_global.h"

// 几何射线类
class CORE_EXPORT GRay
{
public:
	GVector3d mO;				// 射线原点
	GVector3d mD;				// 射线方向
public:
	GRay();
	
	GRay(const GVector3d &o, const GVector3d &d);
	
	GRay(const GRay &other);

	GRay & operator = (const GRay &other);

	const GVector3d & getO() 
	{ return mO; }

	void setO(const GVector3d &o)
	{ mO = o; }

	const GVector3d & getD()
	{ return mD; }

	void setD(const GVector3d &d) 
	{ mD = d; }
};

#endif