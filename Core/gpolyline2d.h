#ifndef GPOLYLINE2D_H
#define GPOLYLINE2D_H

#include <QVector>
#include "gpoint2d.h"
#include "grect2d.h"
#include "core_global.h"

class CORE_EXPORT GPolyLine2d
{
protected:
	QVector<GPoint2d> mPoints;			// 控制点表
protected:
	// 递归函数加密第i段折线
	void biIncrease(int i, double step);

	// 递归函数加密第i段折线(闭合区域加密)
	void biCloseIncrease(int &i, double d, const GPoint2d &p0, const GPoint2d &p1);
public:
	GPolyLine2d();
	GPolyLine2d(const GPolyLine2d &other);

	virtual ~GPolyLine2d();

	GPolyLine2d & operator = (const GPolyLine2d &other);

	// 清除所有控制点
	virtual void clear();

	// 返回控制点数目
	int count() const;

	// 返回指定的控制点
	const GPoint2d & get(int index) const;

	// 返回控制点数组头指针
	operator GPoint2d *();

	// 设置指定控制点
	void set(int index, const GPoint2d &pt);

	const GPoint2d & begin()
	{ return mPoints[0]; }

	const GPoint2d & end()
	{ return mPoints[count()-1]; }

	// 在尾部追加控制点
	void addLast(const GPoint2d &point);

	// 在头部插入控制点
	void addFirst(const GPoint2d &point);

	// 在指定位置插入控制点
	bool insert(int index, const GPoint2d &point);

	// 删除尾部控制点
	void removeLast();

	// 删除头部控制点
	void removeFirst();

	// 删除指定控制点
	bool remove(int index);

	// 将控制点序列反转
	void inverse();

	// 根据x坐标, 得到所在的分段下标
	int getRangeIndex(double x);

	int getRangeIndex(double x, double y, double diff, double *pDistance = 0);

	// 计算给定x坐标处折线的y坐标
	double getY(double x, bool *ok = 0);

	// 点是否在几何控制点上
	int ptInPoint(double x, double y, double diff);
	int ptInPoint(const GPoint2d &point, double diff);

	//点是否在线上
	bool ptInLine(double x, double y, double diff);
	bool ptInLine(const GPoint2d &point, double diff);

	// 求点到折线的最短距离, ri存放求得最短距离的分段下标
	double getDistance(const GPoint2d &point, int *ri = 0);

	// 得到给定点point, 在折线第ri分段的参数t
	// (一般: 应当先用ptInLine确保点在折线的第ri分段上)
	double getParamT(const GPoint2d &point, int ri);

	// 批量追加控制点
	void append(GPolyLine2d *pLine, int i0, int i1);

	// 批量前插控制点
	void prepend(GPolyLine2d *pLine, int i0, int i1);

	// 二分加密折线(从后向前倒序加密)
	void biIncrease(double step);

	// 二分加密闭合折线(从后向前倒序加密)
	void biCloseIncrease(double step);

	// 计算折线控制的矩形范围
	GRect2d calcluteCtrlRect();

	// 从折线中删除位置接近的点, diff为接近门限值
	void removeSamePts(double diff = 10e-8);

	//计算多边形面积
	double getArea();

	// 计算多边形的重心坐标点
	GPoint2d getCenter();
};

#endif
