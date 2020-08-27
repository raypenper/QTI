#ifndef GTRI_LTI_H
#define GTRI_LTI_H

#include <QVector>
#include <QList>
#include <QPair>
#include "gpoint3d.h"

namespace Tomo
{

class GTriModel;

// ��������������������ʱ��ֵ������
class GTriLTI
{
protected:
	// ����LTI�Ķ��������Ϣ�ṹ
	struct LTIPointInfo
	{
		int		mHeapIndex;		// �����ڶ��е��±�
		bool	mIsFixed;		// �Ƿ���������Դ

		LTIPointInfo(int index=-1, bool isFixed=false)
		{
			mHeapIndex = index;
			mIsFixed = isFixed;
		}
	};

protected:
	static double		MAX_TIME;		// �������ʱ
	QVector<double>		mTrvalTimes;	// ���������������ʱ��
	GTriModel *			mpModel;		// ��������ģ��
	QVector<LTIPointInfo> mLTIPointInfos;	// LTI��ֵ������Ϣ��
	QVector<int>		mLTIHeap;		// LTI��ֵ����ʱ����Сʱ���(�洢�����±�)

	QVector<double>		mMidTrvalTimes;	// ���е������ʱ��

protected:

	// ���µ���LTI��
	void siftDown(int index);

	// ���ϵ���LTI��
	void siftUp(int index); 

	// �����Բ�ֵ����������С����ʱ��λ��
	double plotTimeAndPos(const GPoint3d &pos, int itri, int ie, GPoint3d &nextPos);

	// ���β�ֵ
	double plotTimeAndPos2(const GPoint3d &pos, int itri, int ie, GPoint3d &nextPos, int ie0=-1);

	// �õ���һ�������ڲ�ֵ�ı�
	void getNextPlotEdges(const GPoint3d &nextPos, int itri, int iedge, QList< QPair<int, int> > &nextPlotEdges);

	// �Ӽ첨�㿪ʼ׷������·��, ���ҷ��ؼ첨������ʱ
	double traceRayPath(const GPoint3d &spt, const GPoint3d &rpt, QVector<GPoint3d> &ray);

	double traceRayPath2(const GPoint3d &spt, const GPoint3d &rpt, QVector<GPoint3d> &ray);


	// �̶�pt1��pt2, �Ŷ�pt��, �Ż�����·��
	void blend(int itri1, int itri2, int &ie2, const GPoint3d &pt1, const GPoint3d &pt2, GPoint3d &pt);

	// ��ε����Ż�����·��
	// traceEdges:	��¼�����߹켣�ж������������εı�(��β2������ı�������)
	// ray:			���߹켣, �����userInt�ֶμ�¼�˶�������������
	void blendRay(QVector<int> &traceEdges, QVector<GPoint3d> &ray, int ip0, int ip1);

public:
	GTriLTI(GTriModel *pModel=NULL);
	~GTriLTI();

	void setModel(GTriModel *pModel);

	GTriModel * getModel();

	// ���ݳ��������߹켣
	void forward(const GPoint3d &spt, const QVector<GPoint3d> &rpts,
				 QVector<double> &fbts, QList< QVector<GPoint3d> > &rays);

	// ���β�ֵ���ݳ��������߹켣
	void forward2(const GPoint3d &spt, const QVector<GPoint3d> &rpts,
		QVector<double> &fbts, QList< QVector<GPoint3d> > &rays);

	// ����������ʱ���
	void forward(const GPoint3d &spt);

	// ���ض�������ʱ���
	const QVector<double> getTrvalTimes() const;
};

}

#endif
