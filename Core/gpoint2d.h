#ifndef GPOINT2D_H
#define GPOINT2D_H

#include "gvector2d.h"
#include "core_global.h"

class GMatrix2d;

class CORE_EXPORT GPoint2d : public GVector2d
{
protected:
	qint32 mUsrInt;						// À©Õ¹ÕûÐÍ×Ö¶Î

public:
	GPoint2d();
	GPoint2d(double x, double y, int d=0);
	GPoint2d(const double *v, int d=0);
	GPoint2d(const GPoint2d &v);
	GPoint2d(const GVector2d &v);

	void setXYU(double x, double y, int u)
	{ mTuple[0] = x;	mTuple[1] = y;	mUsrInt = u; }
	void setPoint(const GPoint2d &v)
	{ *this = v; }

	int getUsrInt() const
	{ return mUsrInt; }
	void setUsrInt(int i)
	{ mUsrInt = i; }

	GPoint2d transformTo(const GMatrix2d &m) const;
	
	GPoint2d & operator = (const GPoint2d &other);

	GPoint2d & operator = (const GVector2d &other);
};

#endif
