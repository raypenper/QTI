#ifndef GVECTOR3D_H
#define GVECTOR3D_H

#include "gtuple.h"
#include "core_global.h"

class GMatrix3d;
class GQuaternion3d;
class GRotation3d;

class CORE_EXPORT GVector3d : public GTuple<3, double>
{
public:
	enum GCompareType {gAscX, gAscY, gAscZ, gDescX, gDescY, gDescZ};

protected:
	static GCompareType mCompareType;	// ��������

protected:
    GVector3d operator * (const GMatrix3d &m) const;
	GVector3d& operator *= (const GMatrix3d &m);    

public:
    GVector3d() : GTuple<3, double>() {}
	GVector3d(double x, double y, double z) { setXYZ(x, y, z); }
	GVector3d(const double *v) : GTuple<3, double>(v) {}
	GVector3d(const GVector3d &v) : GTuple<3, double>(v) {}

	double x() const	{ return mTuple[0]; }
	double y() const	{ return mTuple[1]; }
	double z() const	{ return mTuple[2]; }

	void setX(double x) { mTuple[0] = x; }
	void setY(double y) { mTuple[1] = y; }
	void setZ(double z) { mTuple[2] = z; }
	void setXYZ(double x, double y, double z)
	{ mTuple[0] = x;	mTuple[1] = y;		mTuple[2] = z; }
	void setVector(const GVector3d &other)
	{ setXYZ(other.x(), other.y(), other.z()); }

	bool operator < (const GVector3d &other) const;

	bool operator > (const GVector3d &other) const;

	static void setCompareType(GCompareType t)
	{ mCompareType = t; }

	static GCompareType getCompareType()
	{ return mCompareType; }
	
	GVector3d & operator = (const GVector3d &other)
	{ setXYZ(other.x(), other.y(), other.z());  return *this; }

	//----ʸ������--------------------------------------------------------------
	// �����ӷ� 
	GVector3d operator + (const GVector3d &v) const;
	GVector3d& operator += (const GVector3d &v);

    // �������� 
	GVector3d operator - (const GVector3d &v) const;
	GVector3d& operator -= (const GVector3d &v);
	GVector3d operator - () const;

    // �������� 
	GVector3d operator * (double d) const;
	GVector3d& operator *= (double d);

	GVector3d operator * (const GVector3d &v) const;
	GVector3d& operator *= (const GVector3d &v);

    // �������� 
	GVector3d operator / (double d) const;
	GVector3d& operator /= (double d);

    // ������� 
	GVector3d crossMult(const GVector3d &v) const;
	
	// ������� 
	double dotMult(const GVector3d &v) const;

    // �������� 
	double getLength() const;
	// ������XOYƽ���ϳ��� 
	double getLengthXY() const;
	// ������YOZƽ���ϳ��� 
	double getLengthYZ() const;
	// ������ZOXƽ���ϳ��� 
	double getLengthZX() const;
	// �Ƿ�Ϊ0�������� 
	bool isZero() const;
	// �������ȵ�ƽ�� 
	double getSqrLength() const;

    // �õ���һ������ 
	GVector3d getNormal() const;
	// ��������һ�� 
	void normalize();
	
	// �õ����任����m�任������� 
	GVector3d transformTo(const GMatrix3d &m) const;
	
	GVector3d transformTo(const GQuaternion3d &q) const;
	
	GVector3d transformTo(const GRotation3d &rot) const;
};

#endif
