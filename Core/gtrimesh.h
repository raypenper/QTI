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

// ������׷��
#define GEDGE_TRACE_ABLE				0x00000001

// ɾ���ı�
#define GEDGE_REMOVED					0x80000000

// ��������(��������Ǳ�, ������GTriMesh��)
class CORE_EXPORT GTriEdge
{
public:
	quint32 mUserInt;				// ����㷨��һЩ��ʱ��Ϣ
	quint64 mVexIdxCode;			// ��Ψһ���(��λ���v0, ��λ���v1)
	QVector<quint64> mTriIdxs;		// ������������(����������������±�)
	QVector<quint64> mAdjTriIdxs;	// �ڽ�������������(�������������κ��ڽ�������, ��Ҫ���ڿ���׷��)

public:
	GTriEdge();

	GTriEdge(int v0, int v1);

	GTriEdge(qint64 code);

	GTriEdge(const GTriEdge &other);

	GTriEdge & operator = (const GTriEdge &other);

	bool operator == (const GTriEdge &other);

	// �õ�v0������
	int getVex0() const;

	// �õ�v1������
	int getVex1() const;

	// ���ö�������
	void setVex(int v0, int v1);

	// ���ñ߱��
	void setVexCode(qint64 code);

	// �õ��߶���Ψһ���
	static qint64 getVexCode(int v0, int v1);

	// �õ����ߵ���������Ŀ
	int triCount() const;

	// �õ�����������
	int getTri(int i) const;

	// �õ���������������ID
	int getSurfID(int i) const;

	// ׷��������������
	void appendTri(int id, int tri);

	// �õ��ڽ���������Ŀ
	int adjTriCount() const;

	// �õ�ָ�����ڽ������ε��±�
	int indexOfAdjTri(int id, int tri) const;

	// �õ��ڽ�����������
	int getAdjTri(int i) const;

	// �õ��ڽ���������������ID
	int getAdjSurfID(int i) const;

	// ׷��������������
	void appendAdjTri(int id, int tri);
};

// �����ο���׷�ٱ�Ǻ궨��

// ������׷��
#define GTRI_TRACE_ABLE				0x00000001

// ׷��ʱ�ѷ��ʹ����
#define GTRI_ACCESSED				0x00000002

// �ѱ��������ñ��
#define GTRI_REFERENCED				0x00000004

// �������������ñ��
#define GTRI_FORWARD_REF			0x00000008

// �����巴�����ñ��
#define GTRI_BACKWARD_REF			0x00000010

// ��ʱ�޷�ȷ���Ƿ�Ӧ������ʹ�õĴ���״̬���
#define GTRI_UNDETERMINED			0x00000020

// ɾ����������
#define GTRI_REMOVED				0x80000000

// ����������(���������������, ������GTriMesh��)
class CORE_EXPORT GTriangle 
{
public:
	// ���ڿ���׷�ٵı�־����
	// (0λ���ñ�ʾ������׷��, 1λ���ñ�ʾ��ʱ�뱻����, 2λ���ñ�ʾ˳ʱ�뱻����)
	qint32 mUserInt;					
	qint32 mVexIdxs[3];			// ��������
	qint32 mEdgeIdxs[3];		// �߱����� 
public:
	GTriangle();

	GTriangle(int v0, int v1, int v2);

	GTriangle(const GTriangle &other);

	GTriangle & operator = (const GTriangle &other);

	// �õ���������
	int getVex(int i) const
	{ return mVexIdxs[i]; }

	// ���ö���
	void setVex(int i, int index)
	{ mVexIdxs[i] = index; }

	// ��GTriangleת��Ϊ������������
	operator qint32 * ()
	{ return mVexIdxs; }

	// ���ݶ��������õ����±�
	int getEdge(int v0, int v1) const;

	// �õ��߱�����
	int getEdgeIdx(int i) const;

	// �õ�ָ���ߵĶԵ�
	int getTriDiagVex(int v0, int v1) const;
};

class CORE_EXPORT GTriMesh
{
public:
	// ����ͶӰ����(δָ��, ZOX, XOY, YOZ, ����ָ��ƽ��)
	enum GPrjDirect {gUnProject=0, gZOX=1, gXOY, gYOZ, gCustom};

protected:
	QVector<GPoint3d> mPoints;				// �����б�
	QVector<GTriangle> mTriangles;			// �������б�
	QVector<GTriEdge *> mEdges;				// �߱�
	QVector<GVector3d> mNormals;			// ���㷨�����б�
	int mEdgeNum;                           // �������߽�����(�������������ǰmEdgeNum����Ϊ�߽��)
	GPrjDirect mPrjDirect;		            // ������ͶӰ����
	GVector3d mPrjVec;						// ͶӰ����
	GBoundingBox mBoundBox;					// ��Χ����

protected:
	// ϸ��������tri
	void subdivTriangle(GTriangle *tri, int edge, int *es, GTriangle &t0, GTriangle &t1);

public:
	GTriMesh();
	virtual ~GTriMesh();

	void clear();

	void clone(GTriMesh *pOther);

	// �Կ��Ƶ������������
	void scale(double sx, double sy, double sz);

	// ����Y, Z����
	void swapYZ();

	// ����X, Y����
	void swapXY();

	// �Զ����������任
	void transform(const GMatrix3d &m);

	// ׷�Ӷ������
	void appendPoints(const GPoint3d *points, int count);

	// ׷�Ӷ���, ���ض������б��е��±�
	int appendPoint(const GPoint3d &pt, bool isCheck=false, double diff=1e-3);

	// ���붥��
	void insertPoint(int i, const GPoint3d &pt);

	// ɾ������i
	void removePoint(int i);

	// ׷�Ӷ��������
	void appendTris(GTriangle *tris, int count);

	// ׷��������
	void appendTri(const GTriangle &tri);

	// ������Ŀ
	int ptCount() const;

	// ��������Ŀ
	int triCount() const;

	// �õ�ָ���±�������
	const GTriangle & getTri(int index);

	// �õ�ָ���±�������ָ��
	GTriangle * getTriPtr(int index);

	// �õ�ָ���±���Ƶ�
	const GPoint3d & getPt(int index) const;

	// �õ�ָ���±���Ƶ�ָ��
	GPoint3d * getPtPtr(int index);

	// �õ�ָ���±���Ƶ�ķ�����
	const GVector3d & getNormal(int index);

	// �߽����Ŀ
	int edgeCount() const;

	// �õ�ָ���±�߽�ָ��
	GTriEdge * getEdgePtr(int index);

	// ���ݱ߶����ŶԵõ���Ӧ�ı�ָ��
	GTriEdge * getEdgePtrByVex(int v0, int v1);

	// ���ݱ߶����ŶԵõ���Ӧ�ı��±�
	int getEdgeIdxByVex(int v0, int v1);

	// ���㶥�㷨����
	void calNormals();

	// Ϊ���������Ƶ����÷�����
	void setNormals(int count, GVector3d *normals);

	// ��ձ߱�
	void clearEdge();

	// ���ݵ�ǰ������, ��������߱�
	// id:		��ӦSurface��ID
	void createEdge(int id);

	// �������ΰ�ָ���߽���2��
	void split2(int itri, int ie, const GPoint3d &pt);

	// ����������
	void increase(int id, GInterpolation *pInp, double step, int nsubdiv);

	// �������Ƿ������������Ƶ㸽��
	int ptInPoint(const GPoint3d &pt, double diff=1e-3);

	// ��ָ��ͶӰ������, ������ͶӰ�Ƿ������������Ƶ��ͶӰ����
	int ptInPoint2D(const GPoint3d &pt, GPrjDirect direct, double diff=1e-3);

	// �õ�ָ�������εĶԽǵ�
	const GPoint3d & getTriDiagPt(int it, const GPoint3d &pt0, const GPoint3d &pt1);

	// �õ�ָ�������ε����÷���
	int getTriRefDirect(int it, const GPoint3d &pt0, const GPoint3d &pt1);

	// �����Χ����
	const GBoundingBox & calcBoundBox();

	// �õ���Χ����
	const GBoundingBox & getBoundingBox() const;

	// ���浽����
	bool save(QDataStream &stream);

	// ��������������(�������߱���Ϣ)
	bool saveOut(QDataStream &stream);

	// ����������
	bool load(QDataStream &stream);

	// ׷���������߽磬�����߽�ĵ��Ƶ�������ǰN��
	// ����ִ�гɹ����ر߽��ĸ��������򷵻�-1
	// �������޸�mEdgeNum��Ա����Ϊ�������ı߽�����
	// ע��׷�ٱ߽�ʱ�ᶪʧ�߱���Ϣ
	int traceBoundary();

	// ��ñ߽�����е�����
	QVector<qint32> traceBoundPts();

	// ��ñ߽����Ŀ
	int getEdgeNum() { return mEdgeNum; }

	// ��������ͶӰ����
	GPrjDirect calcProjDirect();

	// ��ָ������ͶӰ
	void project();

	// ��ͶӰ
	void unProject();

	// ���㲢����ͶӰ����
	GMatrix3d calcPrjMatrix();

	// ���㲢������ͶӰ����
	GMatrix3d calcUnPrjMatrix();

	// ��ȡ��������ͶӰ����
	GPrjDirect getProjDirect()
	{ return mPrjDirect; }

	// ����������ͶӰ����
	void setProjDirect(GPrjDirect prjDirect)
	{ mPrjDirect = prjDirect; }

	const GVector3d & getProjVec()
	{ return mPrjVec; }

	void setProjVec(const GVector3d &v)
	{ mPrjVec = v; }

	// ������߽����Ŀ
	void setEdgeNum( qint32 edgeNum );

	// Ѱ�Ҳ����򻯱߽��, ʹ�ñ߽�������ڶ������е�ǰ��
	// ����ǰӦ��ȷ���Ѿ�������createEdge(), ��������ȷ�ı߱�
	void sortBorder();

	// �Ż���߽�������, ���һ�������ε������㶼����߽����жԱ߽���
	void optEdge(int id);

	// �����Ա�
	// id:	��������ӦSurface��ID��
	// it:	�������Աߵ�����������
	// ie:	�����ε��ڲ�������(0, 1, 2)
	void swapTri(int id, int it, int ie);

	void removeTriangle(int i);

	//----�ֲ��޸ĺ���-------------------------------------------------------------------
	// �ֲ��޸ĺ�����, �����޸����ж���, �����κͱߵ��������, ���ñ�ǵķ�����¼ɾ
	// ���Ķ���, �����κͱ�, ��ɾֲ��޸ĺ������һ�����ύ����
	
	// ��������κͱ߹�ϵ
	void localAppendTri(int v0, int v1, int v2);

	// ɾ��������(ͬʱ��ɾ���߶����ж������ε�����)
	void localRemoveTri(int it);

	// ɾ����(ͬʱ��ɾ�����õ������μ������εı߹�ϵ����)
	void localRemoveEdge(int ie);

	// ɾ������
	void localRemovePt(int ip);

	// �ύ�ֲ��޸�
	void localCommit();

};


#endif
