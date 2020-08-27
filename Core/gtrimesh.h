#ifndef GTRI_MESH_H
#define GTRI_MESH_H

#include <QVector>
#include <QDataStream>
#include "gpoint3d.h"
#include "gvector3d.h"
#include "gmatrix3d.h"
#include "gbounds.h"
#include "core_global.h"

class GInterpolation;

// 可用于追踪
#define GEDGE_TRACE_ABLE				0x00000001

// 删除的边
#define GEDGE_REMOVED					0x80000000

// 边索引类(用索引标记边, 点存放在GTriMesh中)
class CORE_EXPORT GTriEdge
{
public:
	quint32 mUserInt;				// 存放算法的一些临时信息
	quint64 mVexIdxCode;			// 边唯一编号(高位存放v0, 低位存放v1)
	QVector<quint64> mTriIdxs;		// 三角形索引表(仅存放自身三角形下标)
	QVector<quint64> mAdjTriIdxs;	// 邻接三角形索引表(包含自身三角形和邻接三角形, 主要用于块体追踪)

public:
	GTriEdge();

	GTriEdge(int v0, int v1);

	GTriEdge(qint64 code);

	GTriEdge(const GTriEdge &other);

	GTriEdge & operator = (const GTriEdge &other);

	bool operator == (const GTriEdge &other);

	// 得到v0的索引
	int getVex0() const;

	// 得到v1的索引
	int getVex1() const;

	// 设置顶点索引
	void setVex(int v0, int v1);

	// 设置边编号
	void setVexCode(qint64 code);

	// 得到边顶点唯一编号
	static qint64 getVexCode(int v0, int v1);

	// 得到共边的三角形数目
	int triCount() const;

	// 得到三角形索引
	int getTri(int i) const;

	// 得到三角形所在曲面ID
	int getSurfID(int i) const;

	// 追加曲面三角索引
	void appendTri(int id, int tri);

	// 得到邻接三角形数目
	int adjTriCount() const;

	// 得到指定的邻接三角形的下标
	int indexOfAdjTri(int id, int tri) const;

	// 得到邻接三角形索引
	int getAdjTri(int i) const;

	// 得到邻接三角形所在曲面ID
	int getAdjSurfID(int i) const;

	// 追加曲面三角索引
	void appendAdjTri(int id, int tri);
};

// 三角形块体追踪标记宏定义

// 可用于追踪
#define GTRI_TRACE_ABLE				0x00000001

// 追踪时已访问过标记
#define GTRI_ACCESSED				0x00000002

// 已被块体引用标记
#define GTRI_REFERENCED				0x00000004

// 被块体正向引用标记
#define GTRI_FORWARD_REF			0x00000008

// 被块体反向引用标记
#define GTRI_BACKWARD_REF			0x00000010

// 暂时无法确定是否应被块体使用的待定状态标记
#define GTRI_UNDETERMINED			0x00000020

// 删除的三角形
#define GTRI_REMOVED				0x80000000

// 三角索引类(用索引标记三角形, 点存放在GTriMesh中)
class CORE_EXPORT GTriangle 
{
public:
	// 用于块体追踪的标志变量
	// (0位被置表示可用于追踪, 1位被置表示逆时针被引用, 2位被置表示顺时针被引用)
	qint32 mUserInt;					
	qint32 mVexIdxs[3];			// 顶点索引
	qint32 mEdgeIdxs[3];		// 边表索引 
public:
	GTriangle();

	GTriangle(int v0, int v1, int v2);

	GTriangle(const GTriangle &other);

	GTriangle & operator = (const GTriangle &other);

	// 得到顶点索引
	int getVex(int i) const
	{ return mVexIdxs[i]; }

	// 设置顶点
	void setVex(int i, int index)
	{ mVexIdxs[i] = index; }

	// 将GTriangle转换为顶点索引数组
	operator qint32 * ()
	{ return mVexIdxs; }

	// 根据顶点索引得到边下标
	int getEdge(int v0, int v1) const;

	// 得到边表索引
	int getEdgeIdx(int i) const;

	// 得到指定边的对点
	int getTriDiagVex(int v0, int v1) const;
};

class CORE_EXPORT GTriMesh
{
public:
	// 曲面投影方向(未指定, ZOX, XOY, YOZ, 任意指定平面)
	enum GPrjDirect {gUnProject=0, gZOX=1, gXOY, gYOZ, gCustom};

protected:
	QVector<GPoint3d> mPoints;				// 顶点列表
	QVector<GTriangle> mTriangles;			// 三角形列表
	QVector<GTriEdge *> mEdges;				// 边表
	QVector<GVector3d> mNormals;			// 顶点法向量列表
	int mEdgeNum;                           // 三角网边界点个数(三角网顶点表中前mEdgeNum个点为边界点)
	GPrjDirect mPrjDirect;		            // 三角网投影方向
	GVector3d mPrjVec;						// 投影向量
	GBoundingBox mBoundBox;					// 包围盒子

protected:
	// 细分三角形tri
	void subdivTriangle(GTriangle *tri, int edge, int *es, GTriangle &t0, GTriangle &t1);

public:
	GTriMesh();
	virtual ~GTriMesh();

	void clear();

	void clone(GTriMesh *pOther);

	// 对控制点坐标进行缩放
	void scale(double sx, double sy, double sz);

	// 交换Y, Z坐标
	void swapYZ();

	// 交换X, Y坐标
	void swapXY();

	// 对顶点进行坐标变换
	void transform(const GMatrix3d &m);

	// 追加多个顶点
	void appendPoints(const GPoint3d *points, int count);

	// 追加顶点, 返回顶点在列表中的下标
	int appendPoint(const GPoint3d &pt, bool isCheck=false, double diff=1e-3);

	// 插入顶点
	void insertPoint(int i, const GPoint3d &pt);

	// 删除顶点i
	void removePoint(int i);

	// 追加多个三角形
	void appendTris(GTriangle *tris, int count);

	// 追加三角形
	void appendTri(const GTriangle &tri);

	// 顶点数目
	int ptCount() const;

	// 三角形数目
	int triCount() const;

	// 得到指定下标三角形
	const GTriangle & getTri(int index);

	// 得到指定下标三角形指针
	GTriangle * getTriPtr(int index);

	// 得到指定下标控制点
	const GPoint3d & getPt(int index) const;

	// 得到指定下标控制点指针
	GPoint3d * getPtPtr(int index);

	// 得到指定下标控制点的法向量
	const GVector3d & getNormal(int index);

	// 边界边数目
	int edgeCount() const;

	// 得到指定下标边界指针
	GTriEdge * getEdgePtr(int index);

	// 根据边顶点编号对得到对应的边指针
	GTriEdge * getEdgePtrByVex(int v0, int v1);

	// 根据边顶点编号对得到对应的边下标
	int getEdgeIdxByVex(int v0, int v1);

	// 计算顶点法向量
	void calNormals();

	// 为三角网控制点设置法向量
	void setNormals(int count, GVector3d *normals);

	// 清空边表
	void clearEdge();

	// 根据当前三角网, 建立自身边表
	// id:		对应Surface的ID
	void createEdge(int id);

	// 将三角形按指定边进行2分
	void split2(int itri, int ie, const GPoint3d &pt);

	// 加密三角网
	void increase(int id, GInterpolation *pInp, double step, int nsubdiv);

	// 给定点是否在三角网控制点附近
	int ptInPoint(const GPoint3d &pt, double diff=1e-3);

	// 在指定投影方向上, 给定点投影是否在三角网控制点的投影附近
	int ptInPoint2D(const GPoint3d &pt, GPrjDirect direct, double diff=1e-3);

	// 得到指定三角形的对角点
	const GPoint3d & getTriDiagPt(int it, const GPoint3d &pt0, const GPoint3d &pt1);

	// 得到指定三角形的引用方向
	int getTriRefDirect(int it, const GPoint3d &pt0, const GPoint3d &pt1);

	// 计算包围盒子
	const GBoundingBox & calcBoundBox();

	// 得到包围盒子
	const GBoundingBox & getBoundingBox() const;

	// 保存到流中
	bool save(QDataStream &stream);

	// 导出三角网部分(不包括边表信息)
	bool saveOut(QDataStream &stream);

	// 从流中载入
	bool load(QDataStream &stream);

	// 追踪三角网边界，并将边界的点移到顶点表的前N个
	// 函数执行成功返回边界点的个数，否则返回-1
	// 函数会修改mEdgeNum成员变量为三角网的边界点个数
	// 注：追踪边界时会丢失边表信息
	int traceBoundary();

	// 获得边界点序列的索引
	QVector<qint32> traceBoundPts();

	// 获得边界点数目
	int getEdgeNum() { return mEdgeNum; }

	// 计算曲面投影方向
	GPrjDirect calcProjDirect();

	// 向指定方向投影
	void project();

	// 反投影
	void unProject();

	// 计算并返回投影矩阵
	GMatrix3d calcPrjMatrix();

	// 计算并返回逆投影矩阵
	GMatrix3d calcUnPrjMatrix();

	// 获取三角网的投影方向
	GPrjDirect getProjDirect()
	{ return mPrjDirect; }

	// 设置三角网投影方向
	void setProjDirect(GPrjDirect prjDirect)
	{ mPrjDirect = prjDirect; }

	const GVector3d & getProjVec()
	{ return mPrjVec; }

	void setProjVec(const GVector3d &v)
	{ mPrjVec = v; }

	// 设置外边界点数目
	void setEdgeNum( qint32 edgeNum );

	// 寻找并有序化边界点, 使得边界点排列在顶点序列的前端
	// 调用前应当确保已经调用了createEdge(), 建立了正确的边表
	void sortBorder();

	// 优化外边界三角形, 如果一个三角形的三个点都是外边界点进行对边交换
	void optEdge(int id);

	// 交换对边
	// id:	三角网对应Surface的ID号
	// it:	待交换对边的三角形索引
	// ie:	三角形的内部边索引(0, 1, 2)
	void swapTri(int id, int it, int ie);

	void removeTriangle(int i);

	//----局部修改函数-------------------------------------------------------------------
	// 局部修改函数组, 不会修改已有顶点, 三角形和边的索引序号, 采用标记的方法记录删
	// 除的顶点, 三角形和边, 完成局部修改后可以提一次性提交更改
	
	// 添加三角形和边关系
	void localAppendTri(int v0, int v1, int v2);

	// 删除三角形(同时会删除边对象中对三角形的引用)
	void localRemoveTri(int it);

	// 删除边(同时会删除引用的三角形及三角形的边关系引用)
	void localRemoveEdge(int ie);

	// 删除顶点
	void localRemovePt(int ip);

	// 提交局部修改
	void localCommit();

};


#endif
