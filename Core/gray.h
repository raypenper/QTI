#ifndef GRAY_H
#define GRAY_H

#include "gvector3d.h"
#include "core_global.h"

// ����������
class CORE_EXPORT GRay
{
public:
	GVector3d mO;				// ����ԭ��
	GVector3d mD;				// ���߷���
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