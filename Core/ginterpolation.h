#ifndef GINTERPOLATION_H
#define GINTERPOLATION_H

#include <QString>
#include <QVector>
#include "gpoint3d.h"
#include "greflection.h"

// 插值器父类
// 插值器定义为y = f(x, z)的形式
class CORE_EXPORT GInterpolation : public GReflection<GInterpolation>
{
protected:
	QVector<GPoint3d> mSrcPoints;	// 原始数据点数组

public:
	GInterpolation();
	virtual ~GInterpolation();

	// 返回插值方法名称(子类应重写)
	static QString methodName()
	{ return ""; }

	// 添加原始数据点
	void appendPoint(const GPoint3d &pt);

	// 检查排除重点
	void check(double eps=0);

	// 原始点数目
	int count() const
	{ return mSrcPoints.count(); }

	// 插值(利用原始数据点计算插值系数)
	virtual void interpolate() = 0;

	// 计算y值(调用该函数前应执行interpolate, 完成插值系数的计算)
	virtual double get(double x, double z) = 0;

	// 初始化注册类映射表, 增加插值器方法名属性
	template <class S>
	static void initGReflectionInfo(GReflectionInfo<GInterpolation> *pInfo)
	{
		pInfo->mProperties["METHOD_NAME"] = S::methodName();
	}

	// 按插值类型(整形)反射插值对象
	static GInterpolation * instance(const QString &className, int ptCount);
};

#endif	// GINTERPOLATION_H
