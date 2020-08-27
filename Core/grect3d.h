#ifndef G_RECT3D_H
#define G_RECT3D_H

#include "gvector3d.h"
#include "core_global.h"

class CORE_EXPORT GRect3d
{
protected:
	GVector3d mPoints[2];
public:
	GRect3d();
	GRect3d(const GVector3d &pt0, const GVector3d &pt1);
	GRect3d(double x0, double y0, double z0,
			 double x1, double y1, double z1);
	GRect3d(const GRect3d &other);

	operator const GVector3d * () const
	{ return mPoints; }

	operator GVector3d * ()
	{ return mPoints; }
	
	double x0() const			{ return mPoints[0].x(); }
	double y0() const			{ return mPoints[0].y(); }
	double z0() const			{ return mPoints[0].z(); }
	double x1() const			{ return mPoints[1].x(); }
	double y1() const			{ return mPoints[1].y(); }
	double z1() const			{ return mPoints[1].z(); }

	void setX0(double x)
	{ mPoints[0].setX(x); }
	void setY0(double y)
	{ mPoints[0].setY(y); }
	void setZ0(double z)
	{ mPoints[0].setZ(z);}

	void setX1(double x)
	{ mPoints[1]. setX(x); }
	void setY1(double y)
	{ mPoints[1].setY(y); }
	void setZ1(double z)
	{ mPoints[1].setZ(z); }

	GVector3d getLeftTop() const
	{ return mPoints[0]; }
	GVector3d getRightBottom() const
	{ return mPoints[1]; }
	
	void setLeftTop(const GVector3d &pt)
	{ mPoints[0] = pt; }
	void setLeftTop(double x, double y, double z)
	{ mPoints[0].setXYZ(x, y, z); }

	void setRightBottom(const GVector3d &pt)
	{ mPoints[1] = pt; }
	void setRightBottom(double x, double y, double z)
	{ mPoints[1].setXYZ(x, y, z); }

	void set(const GVector3d &pt0, const GVector3d &pt1)
	{ mPoints[0] = pt0;	mPoints[1] = pt1; }
	
	GRect3d & operator = (const GRect3d &other);

	double getXLength() const;
	double getYLength() const;
	double getZLength() const;

	void adjust(double x0, double y0, double z0,
				double x1, double y1, double z1);

	void move(double dx, double dy, double dz);
};

#endif
