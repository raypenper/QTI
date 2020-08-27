#ifndef GINTERPOLATION_H
#define GINTERPOLATION_H

#include <QString>
#include <QVector>
#include "gpoint3d.h"
#include "greflection.h"

// ��ֵ������
// ��ֵ������Ϊy = f(x, z)����ʽ
class CORE_EXPORT GInterpolation : public GReflection<GInterpolation>
{
protected:
	QVector<GPoint3d> mSrcPoints;	// ԭʼ���ݵ�����

public:
	GInterpolation();
	virtual ~GInterpolation();

	// ���ز�ֵ��������(����Ӧ��д)
	static QString methodName()
	{ return ""; }

	// ���ԭʼ���ݵ�
	void appendPoint(const GPoint3d &pt);

	// ����ų��ص�
	void check(double eps=0);

	// ԭʼ����Ŀ
	int count() const
	{ return mSrcPoints.count(); }

	// ��ֵ(����ԭʼ���ݵ�����ֵϵ��)
	virtual void interpolate() = 0;

	// ����yֵ(���øú���ǰӦִ��interpolate, ��ɲ�ֵϵ���ļ���)
	virtual double get(double x, double z) = 0;

	// ��ʼ��ע����ӳ���, ���Ӳ�ֵ������������
	template <class S>
	static void initGReflectionInfo(GReflectionInfo<GInterpolation> *pInfo)
	{
		pInfo->mProperties["METHOD_NAME"] = S::methodName();
	}

	// ����ֵ����(����)�����ֵ����
	static GInterpolation * instance(const QString &className, int ptCount);
};

#endif	// GINTERPOLATION_H
