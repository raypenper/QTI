#ifndef GTRIANGULATOR_H
#define GTRIANGULATOR_H

#include <QVector>
#include <QLinkedList>
#include <QQueue>
#include "gpoint3d.h"
#include "gvector3d.h"
#include "gtrimesh.h"
#include "core_global.h"

// 三角网剖分时使用的内部结构(记录三角形邻接关系)
class CORE_EXPORT GTrianglePrivate
{
public:
	int mIndexs[3];         // 顶点索引

	QLinkedList<GTrianglePrivate>::iterator mNeighborIts[3];  // this的邻接三角 

	// mNeighborEdges[i]用于记录mNeighborIts[i]邻接三角形与当前三角形相邻边的边下标
	int mNeighborEdges[3];   

	bool mIsRestrict;		// 是否为受约束三角形(在限定区域之外的三角形)
public:
	GTrianglePrivate();

	GTrianglePrivate(int v0, int v1, int v2);

	GTrianglePrivate(const GTrianglePrivate &other);

	GTrianglePrivate& operator = (const GTrianglePrivate &other);
};

class CORE_EXPORT GTriangulator
{
public:
	// 限定类型(限定内部保留外部三角, 限定外部保留保留内部三角)
	enum GRestrictType {gRestrictInner, gRestrictOutter};

	class LopItem   // 优化队列项目 
	{
	public:
		QLinkedList<GTrianglePrivate>::iterator mIt;  // 待优化三角形 
		int mEdge;                      // 优化边 
	public:
		LopItem()
		{ mIt = 0;  mEdge = 0; }

		LopItem(QLinkedList<GTrianglePrivate>::iterator it, int e)
		{ mIt = it;  mEdge = e; }
	};
protected:
	QVector<GPoint3d> mPoints;          // 散乱数据点 
	QVector<int> mConvex;               // 凸壳(存储散乱点下标) 
	QLinkedList<GTrianglePrivate> mPrivateList;      // 内部三角形列表 
	QQueue<LopItem> mLopQueue;          // 优化队列 
	QVector<GPoint3d> **mBlockGrid;		// 数据点块网格(blockAppend()的辅助数据结构)
	int mBlockRowNum, mBlockColNum;		// 点块行列
	double mBlockMinX, mBlockMinZ;		// 点块起点坐标
	double mBlockWidth, mBlockHeight;	// 点块尺寸
protected:
	void allocBlockGrid(QVector<GPoint3d> **&blockGrid, int rowNum, int colNum);
	void freeBlockGrid(QVector<GPoint3d> **&blockGrid, int rowNum);

	// 判断点是否在凸壳中
	int isPtInConvex(const GPoint3d &point, const QVector<int> &convex);

	// 在三角形it内插入点pt, 将三角形分成3个小三角 
	void split3(QLinkedList<GTrianglePrivate>::iterator it, int ptIndex);

	// 在三角形it的eIndex边上插入点pt, 将三角形分成2个小三角 
	void split2(QLinkedList<GTrianglePrivate>::iterator it, int ptIndex, int eIndex);

	// 在三角形it和第eIndex边邻接三角adjIt的公共边上插入点pt, 将它们分成4个小三角
	void split4(QLinkedList<GTrianglePrivate>::iterator it, int ptIndex, int eIndex); 

	// 将被限定排出的三角形加上限定标记
	void setRestrictTri(int begin, int end, GRestrictType rType);
public:
	GTriangulator();
	~GTriangulator();

	void clear();

	// 增加控制点函数组
	void appendPoints(GPoint3d *points, int count);
	int appendPoint(const GPoint3d &pt, bool isCheck=false);
	void beginBlockAppend(int rowNum, int colNum, 
		double minX, double minZ, double w, double h);
	bool blockAppend(const GPoint3d &pt);
	void endBlockAppend();

	// 交换Y, Z坐标
	void swapYZ();

	// 交换X, Y坐标
	void swapXY();

	// 转换为逆时针
	void reversion();

	// 创建凸壳 
	void createConvex();

	// 三角化凸壳
	void trangulateConvex(); 

	// 根据边寻找邻居三角形
	QLinkedList<GTrianglePrivate>::iterator findNeighbor(int v0, int v1, int &eIndex); 

	// 追寻点所在的三角形, 返回所在三角形
	// 如果pt在边界上, 则eIndex=(0, 1, 2), 否则eIndex = -1 
	// 返回值>0表示成功找到插入三角形, 0表示该点已经插入, -1表示不能找到插入三角形
	int findTriangle(QLinkedList<GTrianglePrivate>::iterator it, 
		const GPoint3d &pt, QLinkedList<GTrianglePrivate>::iterator &rit, int &eIndex); 

	// 插入点入网 
	bool insert(int ptIndex); 

	// 局部优化
	void localOptimize(); 

	// 约束三角形的边: 通过指定已插入点序列(begin->end)约束三角网
	// 使得三角形的边不能与指定序列边相交
	void restrict(int begin, int end);

	// 约束三角形的边: 通过指定边顶点对约束三角网
	// edges存储顶点下标对
	// 使得三角形的边不能与指定序列边相交
	void restrict(int *edges, int n);

	// 用已经加入的点限定三角网外边界(进行凹限定)
	void restrictEdge();

	// 用已插入的点序列(begin->end)构造一个空洞, 
	// 并从原三角网上删除空洞中的三角
	void restrictCavity(int begin, int end);

	// 删除受约束的三角形
	void removeRestrictTri();

	// 优化边界, 进行曲面切割后, 如果边界三角形三个顶点均来自切割边线
	// 则该三角形有可能与相交面邻边三角形共面, 该函数对边界三角形进行调整
	// 在可能的情况下进行对边交换, 使边界三角形的顶点尽可能不全部取用边界点
	void optEdge(int edgeNum);

	// 将内部三角列表转换成三角网(不自动计算法向量)
	void convertToTriMesh(GTriMesh &mesh);

	// 控制点数目
	int ptCount();

	// 得到控制点
	const GPoint3d & getPt(int index);

	// 给定点是否在控制点上, 如果在返回控制点下标
	// 该函数调用了三角形追踪函数, 实现了快速查找
	int ptInPoint(const GPoint3d &pt, double diff=1e-3); 

	// 三角形数目
	int triCount(); 

	// 得到内部三角网起始迭代器
	QLinkedList<GTrianglePrivate>::iterator triBegin();

	// 得到内部三角网终止迭代器
	QLinkedList<GTrianglePrivate>::iterator triEnd();
};

// 计算给定三点的圆心
bool gGetCircleCenter(GPoint3d &center, const GPoint3d &pt0,
					  const GPoint3d &pt1, const GPoint3d &pt2);

// 计算三角形的面积 
inline double gGetTriArea(const GPoint3d &pt0, const GPoint3d &pt1, const GPoint3d &pt2);

#endif