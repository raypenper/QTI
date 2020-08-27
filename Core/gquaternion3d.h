#ifndef GQUATERNION3D_H
#define GQUATERNION3D_H

#include "core_global.h"

class GMatrix3d;
class GVector3d;

class CORE_EXPORT GQuaternion3d
{
protected:
	double mAxis[4];         //数组分别表示x, y, z,w
public:
	GQuaternion3d();
	GQuaternion3d(double x, double y, double z, double w);
	GQuaternion3d(const double *v);
	GQuaternion3d(const GQuaternion3d &v);

	operator double * ()
	{ return mAxis; }
	operator const double * () const
	{ return mAxis; }

	double x() const	{ return mAxis[0]; }
	double y() const	{ return mAxis[1]; }
	double z() const	{ return mAxis[2]; }
    double w() const    { return mAxis[3]; }

	void setX(double x) { mAxis[0] = x; }
	void setY(double y) { mAxis[1] = y; }
	void setZ(double z) { mAxis[2] = z; }
	void setW(double w) { mAxis[3] = w; }
	void set(const double *v);
    void set(double x, double y, double z, double w);
	//----矢量操作--------------------------------------------------------------
	//重载 = /
	GQuaternion3d& operator = (const GQuaternion3d &v);

	//两四元组相乘
	GQuaternion3d operator * (const GQuaternion3d &v) const;
	GQuaternion3d& operator *= (const GQuaternion3d &v); 

    //四元组求模	
	double getMagnitude() const;
	//四元组求逆
	GQuaternion3d getInverse() const;
	//四元组求共扼	
	GQuaternion3d getConjugate() const;
	//求四元组等价的旋转矩阵
    GMatrix3d  toMatrix() const;
    //归一化 
    void  normalize();
    GQuaternion3d  getNormal() const;
};

#endif
