#ifndef GBOUNDS_H
#define GBOUNDS_H

#include "gvector3d.h"
#include "core_global.h"

class CORE_EXPORT GBoundingBox
{
public:
	GVector3d mLower, mUpper;
public:
	GBoundingBox();
	GBoundingBox(const GBoundingBox &other);
	GBoundingBox(const GVector3d &lower, const GVector3d &upper);
	GBoundingBox(double lx, double ly, double lz,
				 double ux, double uy, double uz);
	
	GBoundingBox& operator = (const GBoundingBox &src);
	
	void set(const GVector3d &lower, const GVector3d &upper);
	void set(double lx, double ly, double lz,
			 double ux, double uy, double uz);

	void setLower(const GVector3d &lower)
	{ mLower = lower; }
	void setLower(double x, double y, double z)
	{ mLower.setXYZ(x, y, z); }

	void setUpper(const GVector3d &upper)
	{ mUpper = upper; }
	void setUpper(double x, double y, double z)
	{ mUpper.setXYZ(x, y, z); }

	const GVector3d & lower() const	{ return mLower; }
	const GVector3d & upper() const	{ return mUpper; }

	GVector3d getCenter() const;
	double getRadius() const;

	double getXLen() const
	{ return mUpper.x() - mLower.x(); }

	double getYLen() const
	{ return mUpper.y() - mLower.y(); }

	double getZLen() const
	{ return mUpper.z() - mLower.z(); }
};

class CORE_EXPORT GBoundingSphere
{
public:
	GVector3d mCenter;
	double mRadius;
public:
	GBoundingSphere();
	GBoundingSphere(const GBoundingSphere &other);
	GBoundingSphere(const GVector3d &center, double radius);
	GBoundingSphere(double x, double y, double z, double radius);
	
	void operator = (const GBoundingSphere &src);
	
	void set(const GVector3d &center, double radius);
	void set(double x, double y, double z, double radius);
	void setCenter(const GVector3d &center)
	{ mCenter = center; }
	void setCenter(double x, double y, double z)
	{ mCenter.setXYZ(x, y, z); }
	void setRadius(double radius)
	{ mRadius = radius; }
	
	const GVector3d & center() const	{ return mCenter; }
	double radius() const		{ return mRadius; }

	GVector3d getLower() const;
	GVector3d getUpper() const;
};

#endif
