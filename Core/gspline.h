#ifndef GSPLINE_H
#define GSPLINE_H

#include <QVector>
#include "gpoint2d.h"
#include "gpoint3d.h"
#include "core_global.h"

struct SplineParam	// 一段三次曲线系数
{
	// a*t^3 + b*t^2 + c*t + d
	double a, b, c, d;

	SplineParam()
	{
		a = b = c = d = 0;
	}
};

class CORE_EXPORT GSpline2d
{
private:
	GPoint2d *mPoints;
	int mCount;
	SplineParam *mParams[2];
	double *mChordLengths;
	bool mIsClosed;				// 是否为毕曲线拟合
protected:
	// 用样条函数构造离散点列表pts
	// isp为样条曲线分段下标, t0, t1为当前分段在样条拟合器中的参数
	// diff为最小偏差, step为最小步长
	// 如果当前分段长度小于step或者中点误差小于diff则不再细分
	void discrete(QVector<GPoint2d> &pts, QVector<int> &incs, int i, int isp, 
				  double t0, double t1, double diff, double step);
public:
	GSpline2d();
	~GSpline2d();
	
	void clear();
	
	// 插值计算
	void interpolate(GPoint2d *points, int count);
	
	// 计算第index段曲线, 弦长参数等于t时的样条坐标
	GPoint2d get(int index, double t);
	
	// 得到第index段曲线的弦长
	double getChordLength(int index);

	// 得到第index段曲线的系数(c=0 x, c=1 y, c=2 z)
	void getParam(int index, int c, double a[]);

	bool getIsClosed()
	{ return mIsClosed; }

	// 用样条函数得到离散点列
	// pts:		离散点列
	// incs:	样条每对控制点间加入离散点的个数
	// diff:	如果插入离散点后的折线中点与样条函数误差小于diff停止插入新的离散点
	// step:	插入离散点的最小步长, 如果相邻离散点<step, 没有达到误差diff也停止插入点
	void discrete(QVector<GPoint2d> &pts, QVector<int> &incs, double diff, double step);

	// 将样条曲线离散化为n个点(n >= 控制点数目)
	// pts:		离散点列
	// incs:	样条每对控制点间加入离散点的个数
	// n:		预期离散点数目
	void discrete(QVector<GPoint2d> &pts, QVector<int> &incs, int n);
};

class CORE_EXPORT GSpline3d
{
private:
	GPoint3d *mPoints;
	int mCount;
	SplineParam *mParams[3];
	double *mChordLengths;
protected:
	// 用样条函数构造离散点列表pts
	// isp为样条曲线分段下标, t0, t1为当前分段在样条拟合器中的参数
	// diff为最小偏差, step为最小步长
	// 如果当前分段长度小于step或者中点误差小于diff则不再细分
	void discrete(QVector<GPoint3d> &pts, QVector<int> &incs, int i, int isp, 
				  double t0, double t1, double diff, double step);
public:
	GSpline3d();
	~GSpline3d();

	void clear();

	// 插值计算
	void interpolate(GPoint3d *points, int count);

	// 计算第index段曲线, 弦长参数等于t时的样条坐标
	GPoint3d get(int index, double t);

	// 得到第index段曲线的弦长
	double getChordLength(int index);

	// 得到第index段曲线的系数(c=0 x, c=1 y, c=2 z)
	void getParam(int index, int c, double a[]);

	// 用样条函数得到离散点列
	// pts:		离散点列
	// incs:	样条每对控制点间加入离散点的个数
	// diff:	如果插入离散点后的折线中点与样条函数误差小于diff停止插入新的离散点
	// step:	插入离散点的最小步长, 如果相邻离散点<step, 没有达到误差diff也停止插入点
	void discrete(QVector<GPoint3d> &pts, QVector<int> &incs, double diff, double step);

	// 将样条曲线离散化为n个点(n >= 控制点数目)
	// pts:		离散点列
	// incs:	样条每对控制点间加入离散点的个数
	// n:		预期离散点数目
	void discrete(QVector<GPoint3d> &pts, QVector<int> &incs, int n);


	void discrete(QVector<GPoint3d>& discretePts, const QVector<int>& nDivisionPerSeg);
};

#endif
