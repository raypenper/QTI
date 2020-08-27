#ifndef GTRI_LTI_H
#define GTRI_LTI_H

#include <QVector>
#include <QList>
#include <QPair>
#include "gpoint3d.h"

namespace Tomo
{

class GTriModel;

// 基于三角网的线性旅行时插值正演类
class GTriLTI
{
protected:
	// 用于LTI的顶点额外信息结构
	struct LTIPointInfo
	{
		int		mHeapIndex;		// 顶点在堆中的下标
		bool	mIsFixed;		// 是否做过子震源

		LTIPointInfo(int index=-1, bool isFixed=false)
		{
			mHeapIndex = index;
			mIsFixed = isFixed;
		}
	};

protected:
	static double		MAX_TIME;		// 最大旅行时
	QVector<double>		mTrvalTimes;	// 三角网顶点的旅行时表
	GTriModel *			mpModel;		// 三角网格模型
	QVector<LTIPointInfo> mLTIPointInfos;	// LTI插值顶点信息表
	QVector<int>		mLTIHeap;		// LTI插值计算时的最小时间堆(存储顶点下标)

	QVector<double>		mMidTrvalTimes;	// 边中点的旅行时表

protected:

	// 向下调整LTI堆
	void siftDown(int index);

	// 向上调整LTI堆
	void siftUp(int index); 

	// 用线性插值估计射线最小旅行时和位置
	double plotTimeAndPos(const GPoint3d &pos, int itri, int ie, GPoint3d &nextPos);

	// 二次插值
	double plotTimeAndPos2(const GPoint3d &pos, int itri, int ie, GPoint3d &nextPos, int ie0=-1);

	// 得到下一步可用于插值的边
	void getNextPlotEdges(const GPoint3d &nextPos, int itri, int iedge, QList< QPair<int, int> > &nextPlotEdges);

	// 从检波点开始追踪射线路径, 并且返回检波点旅行时
	double traceRayPath(const GPoint3d &spt, const GPoint3d &rpt, QVector<GPoint3d> &ray);

	double traceRayPath2(const GPoint3d &spt, const GPoint3d &rpt, QVector<GPoint3d> &ray);


	// 固定pt1和pt2, 扰动pt点, 优化射线路径
	void blend(int itri1, int itri2, int &ie2, const GPoint3d &pt1, const GPoint3d &pt2, GPoint3d &pt);

	// 逐段迭代优化射线路径
	// traceEdges:	记录了射线轨迹中顶点所在三角形的边(首尾2个顶点的边无意义)
	// ray:			射线轨迹, 顶点的userInt字段记录了顶点所在三角形
	void blendRay(QVector<int> &traceEdges, QVector<GPoint3d> &ray, int ip0, int ip1);

public:
	GTriLTI(GTriModel *pModel=NULL);
	~GTriLTI();

	void setModel(GTriModel *pModel);

	GTriModel * getModel();

	// 正演初至和射线轨迹
	void forward(const GPoint3d &spt, const QVector<GPoint3d> &rpts,
				 QVector<double> &fbts, QList< QVector<GPoint3d> > &rays);

	// 二次插值正演初至和射线轨迹
	void forward2(const GPoint3d &spt, const QVector<GPoint3d> &rpts,
		QVector<double> &fbts, QList< QVector<GPoint3d> > &rays);

	// 仅计算旅行时表格
	void forward(const GPoint3d &spt);

	// 返回顶点旅行时表格
	const QVector<double> getTrvalTimes() const;
};

}

#endif
