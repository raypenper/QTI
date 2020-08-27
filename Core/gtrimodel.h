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
	int		mVertices[3];		// �����±�
	int		mEdges[3];			// ���±�
	int		mAdjTris[3];		// �ڽ��������±�
	int		mAdjEdges[3];		// �뵱ǰ�����ζ�Ӧ���ڽ������α��±�(ȡֵ��Χ0, 1, 2)
	double	mCosPhis[3];		// ����нǵ�����ֵ
	double	mSinPhis[3];		// ����нǵ�����ֵ
	double	mEdgeLens[3];		// �����α߳�
	double	mSlowness;

	GTriangle(int v0=0, int v1=0, int v2=0, 
			  int t0=-1, int t1=-1, int t2=-1);

	operator const int * () const;
	operator int * ();

	// ���ݱߵĶ����±�õ��ñ����������еı��±�(0, 1, 2)
	int getEdgeIndex(int v0, int v1);
};

struct CORE_EXPORT GTriEdge
{
	int		mVertices[2];		// �����±�
	int		mAdjTris[2];		// �ڽ��������±�

	GTriEdge(int v0=0, int v1=0);

	void setVertices(int v0, int v1);

	operator const int * () const;
	operator int * ();
};

// �������ñ�, ��¼���ö���������κ͵����������е��±�
typedef QVector< QPair<int, int> >	GPTRefList;

// ���澭�������ʷֺ�õ������ǻ�ģ��
class CORE_EXPORT GTriModel
{
public:
	QVector< GVector3d >		mPoints;		// �����
	QVector< GTriEdge >			mEdges;			// �߱�
	QVector< GTriangle >		mTriangles;		// �����α�
	QList< GPTRefList >			mPTRefLists;	// �������ñ�(��¼���õ������κ͵����������е��±�)
	QList< QVector<int> >		mPERefLists;	// ����-�����ñ�

protected:
	// �ж�pos�Ƿ���itri��ָ�����������
	// ����ڷ���true, ���򷵻�false
	// �������ie����pos�������ε���һ���ߵ����
	bool ptInTri(const GVector2d &pos, int itri, QVector<int> &ies);

public:
	GTriModel();
	~GTriModel();

	void clear();

	// ����pos���ĸ���������, it0Ϊ��ʼѰ��������
	int ptInTri(const GVector2d &pos, int it0=0);

	void putSurveyToTri(QVector<GPoint3d> &surveyPoints);

	// ���������������ṹ��������, �����ù�ϵ
	void createReference();

	// �����߱�ṹ
	void createEdges();

	// �������������β���
	void calcTriParam();

	// �������
	void fillSlowness(double s);

	bool save(QDataStream &stream);

	bool load(QDataStream &stream);
};

}

#endif