#ifndef GROTATION3D_H
#define GROTATION3D_H

#include "gvector3d.h"
#include "core_global.h"

class GMatrix3d;
class GQuaternion3d;

class CORE_EXPORT GRotation3d
{
protected:
	double mAngle;             // ��ת�Ƕȣ�0 - 360 ��
	GVector3d mAxis;           // ��ת�ᣬΪ��λʸ��
public:
       
	GRotation3d();
	GRotation3d(double axisX, double axisY, double axisZ, double axisW);
	GRotation3d(const GVector3d &axis_v, double angle_d);
	GRotation3d(const GRotation3d &v);

	double angle() const	{ return mAngle; }
	GVector3d axis() const	{ return mAxis; }

	void setAngle(double x);
	void setAxis(const GVector3d &x);
	void set(const GVector3d &axisV, double angleD);
    void set(double axisX, double axisY, double axisZ, double axisW);
    //���� 
    GRotation3d operator * (const GRotation3d &v) const;
    GRotation3d& operator *= (const GRotation3d &v);
    GRotation3d& operator = (const GRotation3d &v);
    //�Ƕ�������ת������Ԫ����
    GQuaternion3d toQuaternion() const;
    GRotation3d fromQuaternion(const GQuaternion3d &q) const;
	GMatrix3d toMatrix() const;
};

#endif
