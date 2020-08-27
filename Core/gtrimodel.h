#ifndef GTRI_MODEL_H
#define GTRI_MODEL_H

#include <QString>
#include <QVector>
#include <QList>
#include <QPair>
#include <QDataStream>
#include "gvector3d.h"
#include "gvector2d.h"
#include "gpoint3d.h"
#include "core_global.h"

namespace Tomo
{

struct CORE_EXPORT GTriangle
{
	int		mVertices[3];		// 顶点下标
	int		mEdges[3];			// 边下标
	int		mAdjTris[3];		// 邻接三角形下标
	int		mAdjEdges[3];		// 与当前三角形对应的邻接三角形边下标(取值范围0, 1, 2)
	double	mCosPhis[3];		// 顶点夹角的余弦值
	double	mSinPhis[3];		// 顶点夹角的正弦值
	double	mEdgeLens[3];		// 三角形边长
	double	mSlowness;

	GTriangle(int v0=0, int v1=0, int v2=0, 
			  int t0=-1, int t1=-1, int t2=-1);

	operator const int * () const;
	operator int * ();

	// 根据边的顶点下标得到该边在三角形中的边下标(0, 1, 2)
	int getEdgeIndex(int v0, int v1);
};

struct CORE_EXPORT GTriEdge
{
	int		mVertices[2];		// 顶点下标
	int		mAdjTris[2];		// 邻接三角形下标

	GTriEdge(int v0=0, int v1=0);

	void setVertices(int v0, int v1);

	operator const int * () const;
	operator int * ();
};

// 顶点引用表, 记录引用顶点的三角形和点在三角形中的下标
typedef QVector< QPair<int, int> >	GPTRefList;

// 剖面经过三角剖分后得到的三角化模型
class CORE_EXPORT GTriModel
{
public:
	QVector< GVector3d >		mPoints;		// 顶点表
	QVector< GTriEdge >			mEdges;			// 边表
	QVector< GTriangle >		mTriangles;		// 三角形表
	QList< GPTRefList >			mPTRefLists;	// 顶点引用表(记录引用的三角形和点在三角形中的下标)
	QList< QVector<int> >		mPERefLists;	// 顶点-边引用表

protected:
	// 判断pos是否在itri所指向的三角形中
	// 如果在返回true, 否则返回false
	// 如果不在ie返回pos在三角形的那一条边的外侧
	bool ptInTri(const GVector2d &pos, int itri, QVector<int> &ies);

public:
	GTriModel();
	~GTriModel();

	void clear();

	// 计算pos在哪个三角形中, it0为起始寻找三角形
	int ptInTri(const GVector2d &pos, int it0=0);

	void putSurveyToTri(QVector<GPoint3d> &surveyPoints);

	// 根据三角网基本结构创建顶点, 边引用关系
	void createReference();

	// 建立边表结构
	void createEdges();

	// 计算三角网几何参数
	void calcTriParam();

	// 填充慢度
	void fillSlowness(double s);

	bool save(QDataStream &stream);

	bool load(QDataStream &stream);
};

}

#endif