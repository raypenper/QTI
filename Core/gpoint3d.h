#ifndef GPOINT3D_H
#define GPOINT3D_H

#include "gvector3d.h"
#include "core_global.h"

class GMatrix3d;
class GQuaternion3d;
class GRotation3d;

// �����������
enum GPointType
{
	gNormalPt = 0,   // ��ͨ��
	gSourcePt = 1,   // ԭʼ���ݵ�
	gEdgePt   = 2,   // �߽��
	gIsectPt  = 4,   // �����ϵ�
	gCornerPt = 8,   // �߽��Ϲյ�
	gXCrossPt = 16,  // �������ߵĽ���
	gRemovedPt = 32	 // ��ɾ���ĵ�
};


class CORE_EXPORT GPoint3d : public GVector3d
{
protected:
	int mUsrInt;						// ��չ�����ֶ�(���ڱ�ʶ������)

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
