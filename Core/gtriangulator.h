#ifndef GTRIANGULATOR_H
#define GTRIANGULATOR_H

#include <QVector>
#include <QLinkedList>
#include <QQueue>
#include "gpoint3d.h"
#include "gvector3d.h"
#include "gtrimesh.h"
#include "core_global.h"

// �������ʷ�ʱʹ�õ��ڲ��ṹ(��¼�������ڽӹ�ϵ)
class CORE_EXPORT GTrianglePrivate
{
public:
	int mIndexs[3];         // ��������

	QLinkedList<GTrianglePrivate>::iterator mNeighborIts[3];  // this���ڽ����� 

	// mNeighborEdges[i]���ڼ�¼mNeighborIts[i]�ڽ��������뵱ǰ���������ڱߵı��±�
	int mNeighborEdges[3];   

	bool mIsRestrict;		// �Ƿ�Ϊ��Լ��������(���޶�����֮���������)
public:
	GTrianglePrivate();

	GTrianglePrivate(int v0, int v1, int v2);

	GTrianglePrivate(const GTrianglePrivate &other);

	GTrianglePrivate& operator = (const GTrianglePrivate &other);
};

class CORE_EXPORT GTriangulator
{
public:
	// �޶�����(�޶��ڲ������ⲿ����, �޶��ⲿ���������ڲ�����)
	enum GRestrictType {gRestrictInner, gRestrictOutter};

	class LopItem   // �Ż�������Ŀ 
	{
	public:
		QLinkedList<GTrianglePrivate>::iterator mIt;  // ���Ż������� 
		int mEdge;                      // �Ż��� 
	public:
		LopItem()
		{ mIt = 0;  mEdge = 0; }

		LopItem(QLinkedList<GTrianglePrivate>::iterator it, int e)
		{ mIt = it;  mEdge = e; }
	};
protected:
	QVector<GPoint3d> mPoints;          // ɢ�����ݵ� 
	QVector<int> mConvex;               // ͹��(�洢ɢ�ҵ��±�) 
	QLinkedList<GTrianglePrivate> mPrivateList;      // �ڲ��������б� 
	QQueue<LopItem> mLopQueue;          // �Ż����� 
	QVector<GPoint3d> **mBlockGrid;		// ���ݵ������(blockAppend()�ĸ������ݽṹ)
	int mBlockRowNum, mBlockColNum;		// �������
	double mBlockMinX, mBlockMinZ;		// ����������
	double mBlockWidth, mBlockHeight;	// ���ߴ�
protected:
	void allocBlockGrid(QVector<GPoint3d> **&blockGrid, int rowNum, int colNum);
	void freeBlockGrid(QVector<GPoint3d> **&blockGrid, int rowNum);

	// �жϵ��Ƿ���͹����
	int isPtInConvex(const GPoint3d &point, const QVector<int> &convex);

	// ��������it�ڲ����pt, �������ηֳ�3��С���� 
	void split3(QLinkedList<GTrianglePrivate>::iterator it, int ptIndex);

	// ��������it��eIndex���ϲ����pt, �������ηֳ�2��С���� 
	void split2(QLinkedList<GTrianglePrivate>::iterator it, int ptIndex, int eIndex);

	// ��������it�͵�eIndex���ڽ�����adjIt�Ĺ������ϲ����pt, �����Ƿֳ�4��С����
	void split4(QLinkedList<GTrianglePrivate>::iterator it, int ptIndex, int eIndex); 

	// �����޶��ų��������μ����޶����
	void setRestrictTri(int begin, int end, GRestrictType rType);
public:
	GTriangulator();
	~GTriangulator();

	void clear();

	// ���ӿ��Ƶ㺯����
	void appendPoints(GPoint3d *points, int count);
	int appendPoint(const GPoint3d &pt, bool isCheck=false);
	void beginBlockAppend(int rowNum, int colNum, 
		double minX, double minZ, double w, double h);
	bool blockAppend(const GPoint3d &pt);
	void endBlockAppend();

	// ����Y, Z����
	void swapYZ();

	// ����X, Y����
	void swapXY();

	// ת��Ϊ��ʱ��
	void reversion();

	// ����͹�� 
	void createConvex();

	// ���ǻ�͹��
	void trangulateConvex(); 

	// ���ݱ�Ѱ���ھ�������
	QLinkedList<GTrianglePrivate>::iterator findNeighbor(int v0, int v1, int &eIndex); 

	// ׷Ѱ�����ڵ�������, ��������������
	// ���pt�ڱ߽���, ��eIndex=(0, 1, 2), ����eIndex = -1 
	// ����ֵ>0��ʾ�ɹ��ҵ�����������, 0��ʾ�õ��Ѿ�����, -1��ʾ�����ҵ�����������
	int findTriangle(QLinkedList<GTrianglePrivate>::iterator it, 
		const GPoint3d &pt, QLinkedList<GTrianglePrivate>::iterator &rit, int &eIndex); 

	// ��������� 
	bool insert(int ptIndex); 

	// �ֲ��Ż�
	void localOptimize(); 

	// Լ�������εı�: ͨ��ָ���Ѳ��������(begin->end)Լ��������
	// ʹ�������εı߲�����ָ�����б��ཻ
	void restrict(int begin, int end);

	// Լ�������εı�: ͨ��ָ���߶����Լ��������
	// edges�洢�����±��
	// ʹ�������εı߲�����ָ�����б��ཻ
	void restrict(int *edges, int n);

	// ���Ѿ�����ĵ��޶���������߽�(���а��޶�)
	void restrictEdge();

	// ���Ѳ���ĵ�����(begin->end)����һ���ն�, 
	// ����ԭ��������ɾ���ն��е�����
	void restrictCavity(int begin, int end);

	// ɾ����Լ����������
	void removeRestrictTri();

	// �Ż��߽�, ���������и��, ����߽���������������������и����
	// ����������п������ཻ���ڱ������ι���, �ú����Ա߽������ν��е���
	// �ڿ��ܵ�����½��жԱ߽���, ʹ�߽������εĶ��㾡���ܲ�ȫ��ȡ�ñ߽��
	void optEdge(int edgeNum);

	// ���ڲ������б�ת����������(���Զ����㷨����)
	void convertToTriMesh(GTriMesh &mesh);

	// ���Ƶ���Ŀ
	int ptCount();

	// �õ����Ƶ�
	const GPoint3d & getPt(int index);

	// �������Ƿ��ڿ��Ƶ���, ����ڷ��ؿ��Ƶ��±�
	// �ú���������������׷�ٺ���, ʵ���˿��ٲ���
	int ptInPoint(const GPoint3d &pt, double diff=1e-3); 

	// ��������Ŀ
	int triCount(); 

	// �õ��ڲ���������ʼ������
	QLinkedList<GTrianglePrivate>::iterator triBegin();

	// �õ��ڲ���������ֹ������
	QLinkedList<GTrianglePrivate>::iterator triEnd();
};

// ������������Բ��
bool gGetCircleCenter(GPoint3d &center, const GPoint3d &pt0,
					  const GPoint3d &pt1, const GPoint3d &pt2);

// ���������ε���� 
inline double gGetTriArea(const GPoint3d &pt0, const GPoint3d &pt1, const GPoint3d &pt2);

#endif