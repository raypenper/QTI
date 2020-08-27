#include <limits>
#include <QTime>
#include "gtrilti.h"
#include "gtrimodel.h"
#include "gmath.h"
#include "gdistance.h"
#include "gequroot.h"

namespace Tomo
{

double GTriLTI::MAX_TIME = std::numeric_limits<double>::max();

#define LTI_ZERO	1e-12

GTriLTI::GTriLTI( GTriModel *pModel/*=NULL*/ )
{
	mpModel = pModel;
}

GTriLTI::~GTriLTI()
{

}

void GTriLTI::setModel( GTriModel *pModel )
{
	mpModel = pModel;
}

GTriModel * GTriLTI::getModel()
{
	return mpModel;
}

const QVector<double> GTriLTI::getTrvalTimes() const
{
	return mTrvalTimes;
}

struct LTITriEdge
{
	GTriModel * mpModel;			// ģ��ָ��
	double *	mModelTrvalTimes;	// ��������������ʱ��
	int			mTriIndex;			// �������±�
	int			mEdgeIndex;			// �������еı��±�(0, 1, 2)

	LTITriEdge(int it=0, int ie=0, GTriModel *pModel=0, double *ts=0)
	{
		mTriIndex = it;
		mEdgeIndex = ie;
		mpModel = pModel;
		mModelTrvalTimes = ts;
	}

	inline double calcEdgeTrvalTime() const
	{
		const GTriangle &tri = mpModel->mTriangles[mTriIndex];
		double t0 = mModelTrvalTimes[tri.mVertices[mEdgeIndex]];
		double t1 = mModelTrvalTimes[tri.mVertices[(mEdgeIndex+1)%3]];
		return t0 + t1;
	}

	inline bool operator < (const LTITriEdge &other) const
	{
		return calcEdgeTrvalTime() < other.calcEdgeTrvalTime();
	}
};

inline void plotTrvalTime(double ta, double tb, double ab, double bc, double ac, 
						  double sinphi, double cosphi, double slowness, 
						  double &tc, double &r)
{
	double tba = tb - ta;
	double e = ab * ab * slowness * slowness - tba * tba;
	if (e >= LTI_ZERO)
	{
		e = sqrt(e);
		tc = ta + ac/ab * (cosphi*tba + sinphi*e);
		r = ac * (cosphi - tba * sinphi / e); 
		if (r >= 0 && r <= ab) return ;
	}

	double tc1 = ta + ac * slowness;
	double tc2 = tb + bc * slowness;
	if (tc1 < tc2)
	{
		r = 0;
		tc = tc1;
	}
	else
	{
		r = ab;
		tc = tc2;
	}
}

inline void plotTrvalTime2(double ta, double tb, double tm, double ab, double bc, double ac, 
	double sinphi, double cosphi, double slowness, 
	double &tc, double &r)
{
	double k1, k2, k3;
	k1 = 2*(ta - 2*tm + tb);
	k2 = -3*ta + 4*tm - tb;
	k3 = ta;

	plotTrvalTime(ta, tb, ab, bc, ac, sinphi, cosphi, slowness, tc, r);

	if (k1 <= 0) return ;

	double a, b, c, d, e;
	double ab_2 = ab*ab;
	double ab_4 = ab_2*ab_2;
	double ac_2 = ac*ac;
	double k1_2 = k1*k1;
	double k2_2 = k2*k2;
	double k1k2 = k1*k2;
	double s_2 = slowness*slowness;
	double p[5], x[4];
	p[0] = 4 * k1_2;
	p[1] = 4*k1k2*ab - 8*k1_2*ac*cosphi;
	p[2] = 4*k1_2*ac_2 + k2_2*ab_2 - 8*k1k2*ab*ac*cosphi - s_2*ab_4;
	p[3] = 4*k1k2*ab*ac_2 + 2*s_2*ab_4*ac*cosphi - 2*k2_2*ab_2*ac*cosphi;
	p[4] = ab_2*ac_2*(k2_2 - s_2*ab_2*cosphi*cosphi);

	double r0 = r;
	//if (fabs(p[0]) > 1e-6)
	{
		int nr = solve_quartic_equation(p, x);
		if (nr > 0)
		{
			for (int i=0; i<nr; i++)
			{
				double xi = x[i];
				if (xi >= 0 && xi <= ab)
				{
					double u = xi / ab;
					double e = xi*xi + ac*ac - 2*xi*ac*cosphi;
					if (e < 0) e = 0;
					double t = k1*u*u + k2*u + k3 + slowness*sqrt(e);
					if (t < tc && fabs(xi-r0) < 0.5*ab)
					{
						tc = t;
						r = xi;
					}
				}
			}
			//if (tc != GTriLTI::MAX_TIME) return ;
		}
	}
}


double GTriLTI::plotTimeAndPos2(const GPoint3d &pos, int itri, int ie, GPoint3d &nextPos, int ie0)
{
	int ip0, ip1;
	if (ie0 < 0 || (ie0+1)%3 == ie)
	{
		ip0 = ie;		ip1 = (ie+1)%3;
	}
	else
	{
		ip1 = ie;		ip0 = (ie+1)%3;
	}
	const GTriangle &tri = mpModel->mTriangles[itri];
	int ia = tri[ip0];
	int ib = tri[ip1];
	const GPoint3d &pa = mpModel->mPoints[ia];
	const GPoint3d &pb = mpModel->mPoints[ib];
	int ei = tri.mEdges[ie];
	double tm = mMidTrvalTimes[ei];
	//tm = (mTrvalTimes[ia] + mTrvalTimes[ib]) * 0.5;
	double ab = tri.mEdgeLens[ie];
	double bc = gDistance3D(pb, pos);
	double ac = gDistance3D(pa, pos);

	double cosPhi, sinPhi;
	if (ie0 < 0)
	{
		double e = ab * ac;
		if (e < LTI_ZERO)
		{
			cosPhi = 1;
			sinPhi = 0;
		}
		else
		{
			cosPhi = (ab*ab+ac*ac-bc*bc)/(2*ab*ac);
			if (cosPhi > 1) cosPhi = 1;
			else if (cosPhi < -1) cosPhi = -1;
			sinPhi = sqrt(1-cosPhi*cosPhi);
		}
	}
	else
	{
		cosPhi = tri.mCosPhis[ip0];
		sinPhi = tri.mSinPhis[ip0];
	}

	double tc, r;
	plotTrvalTime2(mTrvalTimes[ia], mTrvalTimes[ib], tm,
		ab, bc, ac,
		sinPhi, cosPhi, tri.mSlowness, 
		tc, r);

	double s = r / ab;
	nextPos.setXYZU(pa.x()+s*(pb.x()-pa.x()), pa.y()+s*(pb.y()-pa.y()),
		pa.z()+s*(pb.z()-pa.z()),0);
	return tc;
}

void GTriLTI::siftUp(int index)
{
	if (index < 0 || index >= mLTIHeap.count()) return ;
	int i = index;
	int j = (index - 1) / 2;
	int rp = mLTIHeap[i];

	while (j >= 0)
	{
		if (mTrvalTimes[rp] < mTrvalTimes[mLTIHeap[j]])
		{
			mLTIHeap[i] = mLTIHeap[j];
			mLTIPointInfos[mLTIHeap[j]].mHeapIndex = i;
			i = j;
			j = (j - 1) >> 1;
		}
		else break ;
	}
	mLTIHeap[i] = rp;
	mLTIPointInfos[rp].mHeapIndex = i;
}

void GTriLTI::siftDown(int index)
{
	if (index < 0 || index >= mLTIHeap.count()) return ;

	int i = index;
	int j = 2*i + 1;
	int rp = mLTIHeap[i];
	int end = mLTIHeap.count()-1;
	while (j <= end)
	{
		if (j < end && mTrvalTimes[mLTIHeap[j+1]] < mTrvalTimes[mLTIHeap[j]])
			j++;

		if (mTrvalTimes[mLTIHeap[j]] < mTrvalTimes[rp])
		{
			mLTIHeap[i] = mLTIHeap[j];
			mLTIPointInfos[mLTIHeap[j]].mHeapIndex = i;
			i = j;
			j = 2 * j + 1;
		}
		else break;
	}
	mLTIHeap[i] = rp;
	mLTIPointInfos[rp].mHeapIndex = i;
}

void GTriLTI::forward(const GPoint3d &spt)
{
	mTrvalTimes.fill(MAX_TIME, mpModel->mPoints.count());
	int sit = spt.getUsrInt();
	if (sit < 0) return ;

	int np = mpModel->mPoints.count();
	// ����LTI���ݵĶ�����Ϣ��
	mLTIPointInfos = QVector<LTIPointInfo>(np);

	// �߷��ʱ�
	QVector<bool> edgeVisited(mpModel->mTriangles.count()*3, false);
	// �����С��
	mLTIHeap.clear();

	// �����ڵ������������������������ʱ
	const GTriangle &stri = mpModel->mTriangles[sit];
	for (int i=0; i<3; i++)
	{
		const GVector3d &pos = mpModel->mPoints[stri[i]];
		double dist = gDistance3D(pos, spt);
		mTrvalTimes[stri[i]] = dist * stri.mSlowness;

		if (stri.mAdjTris[i] >= 0)
		{
			mLTIHeap << stri[i];
		}
	}

	// ȷ���Ѷ�Ԫ��ʱ����С
	for (int i=1; i<mLTIHeap.count(); i++)
	{
		if (mTrvalTimes[mLTIHeap[i]] < mTrvalTimes[mLTIHeap[0]])
		{
			qSwap(mLTIHeap[0], mLTIHeap[i]);
		}
	}
	for (int i=0; i<mLTIHeap.count(); i++)
	{
		mLTIPointInfos[mLTIHeap[i]].mHeapIndex = i;
		mLTIPointInfos[mLTIHeap[i]].mIsFixed = true;
	}

	// �������ñ�
	QList< GPTRefList > &refLists = mpModel->mPTRefLists;

	while (!mLTIHeap.isEmpty())
	{
		int pi = mLTIHeap[0];
		mLTIPointInfos[pi].mIsFixed = true;

		mLTIHeap[0] = mLTIHeap.last();
		mLTIHeap.pop_back();
		if (mLTIHeap.count() > 1) siftDown(0);

		GPTRefList &refList = refLists[pi];
		// �����������ñ�
		for (int i=0; i<refList.count(); i++)
		{
			int itri = refList[i].first;
			const GTriangle &tri = mpModel->mTriangles[itri];
			int ip = refList[i].second;

			int ies[2];
			ies[0] = ip;
			ies[1] = (ip + 2) % 3;

			for (int j=0; j<2; j++)
			{
				int ie = ies[j];
				int ia = tri[ie];
				int ib = tri[(ie+1)%3];
				int ic = tri[(ie+2)%3];

				if (mLTIPointInfos[ic].mIsFixed || 
					!(mLTIPointInfos[ia].mIsFixed && mLTIPointInfos[ib].mIsFixed) ||
					edgeVisited[itri*3 + ie]) continue ;

				double tc, r;
				plotTrvalTime(mTrvalTimes[ia], mTrvalTimes[ib],
					tri.mEdgeLens[ie], tri.mEdgeLens[(ie+1)%3], tri.mEdgeLens[(ie+2)%3],
					tri.mSinPhis[ie], tri.mCosPhis[ie], tri.mSlowness, 
					tc, r);

				edgeVisited[itri*3 + ie] = true;

				if (tc < mTrvalTimes[ic])
				{
					mTrvalTimes[ic] = tc;
					if (mLTIPointInfos[ic].mHeapIndex < 0)
					{
						mLTIHeap << ic;
						siftUp(mLTIHeap.count() - 1);
					}
					else
					{
						siftUp(mLTIPointInfos[ic].mHeapIndex);
					}
				}
			}
		}
	}
}

void GTriLTI::forward( const GPoint3d &spt, const QVector<GPoint3d> &rpts, QVector<double> &fbts, QList< QVector<GPoint3d> > &rays )
{
	//QTime time;
	//time.start();

	int nrpt = rpts.count();
	fbts.fill(-1, nrpt);

	rays.clear();
	for (int i=0; i<nrpt; i++)
	{
		rays << QVector<GPoint3d>();
	}

	int sit = spt.getUsrInt();
	if (sit < 0) return ;

	
	mTrvalTimes.fill(MAX_TIME, mpModel->mPoints.count());

	int np = mpModel->mPoints.count();
	// ����LTI���ݵĶ�����Ϣ��
	mLTIPointInfos = QVector<LTIPointInfo>(np);

	// �߷��ʱ�
	QVector<bool> edgeVisited(mpModel->mTriangles.count()*3, false);
	// �����С��
	mLTIHeap.clear();

	// �����ڵ������������������������ʱ
	const GTriangle &stri = mpModel->mTriangles[sit];
	for (int i=0; i<3; i++)
	{
		const GVector3d &pos = mpModel->mPoints[stri[i]];
		double dist = gDistance3D(pos, spt);
		mTrvalTimes[stri[i]] = dist * stri.mSlowness;

		if (stri.mAdjTris[i] >= 0)
		{
			mLTIHeap << stri[i];
		}
	}

	// ȷ���Ѷ�Ԫ��ʱ����С
	for (int i=1; i<mLTIHeap.count(); i++)
	{
		if (mTrvalTimes[mLTIHeap[i]] < mTrvalTimes[mLTIHeap[0]])
		{
			qSwap(mLTIHeap[0], mLTIHeap[i]);
		}
	}
	for (int i=0; i<mLTIHeap.count(); i++)
	{
		mLTIPointInfos[mLTIHeap[i]].mHeapIndex = i;
		mLTIPointInfos[mLTIHeap[i]].mIsFixed = true;
	}

	// �������ñ�
	QList< GPTRefList > &refLists = mpModel->mPTRefLists;

	while (!mLTIHeap.isEmpty())
	{
		int pi = mLTIHeap[0];
		mLTIPointInfos[pi].mIsFixed = true;

		mLTIHeap[0] = mLTIHeap.last();
		mLTIHeap.pop_back();
		if (mLTIHeap.count() > 1) siftDown(0);

		GPTRefList &refList = refLists[pi];
		// �����������ñ�
		for (int i=0; i<refList.count(); i++)
		{
			int itri = refList[i].first;
			const GTriangle &tri = mpModel->mTriangles[itri];
			int ip = refList[i].second;

			int ies[2];
			ies[0] = ip;
			ies[1] = (ip + 2) % 3;

			for (int j=0; j<2; j++)
			{
				int ie = ies[j];
				int ia = tri[ie];
				int ib = tri[(ie+1)%3];
				int ic = tri[(ie+2)%3];
				
				if (mLTIPointInfos[ic].mIsFixed || 
					!(mLTIPointInfos[ia].mIsFixed && mLTIPointInfos[ib].mIsFixed) ||
					edgeVisited[itri*3 + ie]) continue ;
				
				double tc, r;
				plotTrvalTime(mTrvalTimes[ia], mTrvalTimes[ib],
						tri.mEdgeLens[ie], tri.mEdgeLens[(ie+1)%3], tri.mEdgeLens[(ie+2)%3],
						tri.mSinPhis[ie], tri.mCosPhis[ie], tri.mSlowness, 
						tc, r);

				edgeVisited[itri*3 + ie] = true;

				if (tc < mTrvalTimes[ic])
				{
					mTrvalTimes[ic] = tc;
					if (mLTIPointInfos[ic].mHeapIndex < 0)
					{
						mLTIHeap << ic;
						siftUp(mLTIHeap.count() - 1);
					}
					else
					{
						siftUp(mLTIPointInfos[ic].mHeapIndex);
					}
				}
			}
		}

		//printf("Len = %d\n", mLTIHeap.count());
	}

	//for (int i=0; i<1; i++)
	for (int i=0; i<rpts.count(); i++)
	{
		fbts[i] = traceRayPath(spt, rpts[i], rays[i]);
	}

	//printf("end %d\n", time.elapsed());
}

void GTriLTI::forward2(const GPoint3d &spt, const QVector<GPoint3d> &rpts, QVector<double> &fbts, QList< QVector<GPoint3d> > &rays)
{
	//QTime time;
	//time.start();

	int nrpt = rpts.count();
	fbts.fill(-1, nrpt);

	rays.clear();
	for (int i=0; i<nrpt; i++)
	{
		rays << QVector<GPoint3d>();
	}

	int sit = spt.getUsrInt();
	if (sit < 0) return ;


	mTrvalTimes.fill(MAX_TIME, mpModel->mPoints.count());
	mMidTrvalTimes.fill(MAX_TIME, mpModel->mEdges.count());

	int np = mpModel->mPoints.count();
	// ����LTI���ݵĶ�����Ϣ��
	mLTIPointInfos = QVector<LTIPointInfo>(np);

	// �߷��ʱ�
	QVector<bool> edgeVisited(mpModel->mTriangles.count()*3, false);
	// �����С��
	mLTIHeap.clear();

	// �����ڵ������������������������ʱ
	const GTriangle &stri = mpModel->mTriangles[sit];

	for (int i=0; i<3; i++)
	{
		const GVector3d &pos = mpModel->mPoints[stri[i]];
		double dist = gDistance3D(pos, spt);
		mTrvalTimes[stri[i]] = dist * stri.mSlowness;

		//if (stri.mAdjTris[i] >= 0)
		{
			mLTIHeap << stri[i];
		}

		// ������е�����ʱ
		int ei = stri.mEdges[i];
		GVector3d midPos = (mpModel->mPoints[stri[i]] + mpModel->mPoints[stri[(i+1)%3]]) * 0.5;
		dist = gDistance3D(midPos, spt);
		mMidTrvalTimes[ei] = dist * stri.mSlowness;
	}

	// ȷ���Ѷ�Ԫ��ʱ����С
	for (int i=1; i<mLTIHeap.count(); i++)
	{
		if (mTrvalTimes[mLTIHeap[i]] < mTrvalTimes[mLTIHeap[0]])
		{
			qSwap(mLTIHeap[0], mLTIHeap[i]);
		}
	}
	for (int i=0; i<mLTIHeap.count(); i++)
	{
		mLTIPointInfos[mLTIHeap[i]].mHeapIndex = i;
		mLTIPointInfos[mLTIHeap[i]].mIsFixed = true;
	}

	// �������ñ�
	QList< GPTRefList > &refLists = mpModel->mPTRefLists;

	while (!mLTIHeap.isEmpty())
	{
		int pi = mLTIHeap[0];
		mLTIPointInfos[pi].mIsFixed = true;

		mLTIHeap[0] = mLTIHeap.last();
		mLTIHeap.pop_back();
		if (mLTIHeap.count() > 1) siftDown(0);

		GPTRefList &refList = refLists[pi];
		// �����������ñ�
		for (int i=0; i<refList.count(); i++)
		{
			int itri = refList[i].first;
			const GTriangle &tri = mpModel->mTriangles[itri];
			int ip = refList[i].second;

			int ies[2];
			ies[0] = ip;
			ies[1] = (ip + 2) % 3;

			for (int j=0; j<2; j++)
			{
				int ie = ies[j];
				int ia = tri[ie];
				int ib = tri[(ie+1)%3];
				int ic = tri[(ie+2)%3];
				int ei = tri.mEdges[ie];

				if (mLTIPointInfos[ic].mIsFixed || 
					!(mLTIPointInfos[ia].mIsFixed && mLTIPointInfos[ib].mIsFixed) ||
					edgeVisited[itri*3 + ie]) continue ;

				double tm, tc, r;
				tm = mMidTrvalTimes[ei];
				//tm = 0.5 * (mTrvalTimes[ia] + mTrvalTimes[ib]);

				plotTrvalTime2(mTrvalTimes[ia], mTrvalTimes[ib], tm,
					tri.mEdgeLens[ie], tri.mEdgeLens[(ie+1)%3], tri.mEdgeLens[(ie+2)%3],
					tri.mSinPhis[ie], tri.mCosPhis[ie], tri.mSlowness, 
					tc, r);
				double tcc = gDistance3D(mpModel->mPoints[ic], spt) * tri.mSlowness;

				// ��ֵ�����е��ʱ��
				double tc1, tc2, r1, r2, bc, ac;
				GVector3d midPos;
				midPos = (mpModel->mPoints[ia] + mpModel->mPoints[ic])*0.5;
				bc = gDistance3D(mpModel->mPoints[ib], midPos);
				ac = gDistance3D(mpModel->mPoints[ia], midPos);
				plotTrvalTime2(mTrvalTimes[ia], mTrvalTimes[ib], tm,
					tri.mEdgeLens[ie], bc, ac,
					tri.mSinPhis[ie], tri.mCosPhis[ie], tri.mSlowness, 
					tc1, r1);
				//double tcm1 = 0.5 * (mTrvalTimes[ia] + mTrvalTimes[ib]);
				//if (tc1 > tcm1) tc1 = tcm1;
				if (mMidTrvalTimes[tri.mEdges[(ie+2)%3]] > tc1)
				{
					mMidTrvalTimes[tri.mEdges[(ie+2)%3]] = tc1;
				}

				midPos = (mpModel->mPoints[ib] + mpModel->mPoints[ic]) * 0.5;
				bc = gDistance3D(midPos, mpModel->mPoints[ia]);
				ac = gDistance3D(midPos, mpModel->mPoints[ib]);
				plotTrvalTime2(mTrvalTimes[ib], mTrvalTimes[ia], tm,
					tri.mEdgeLens[ie], bc, ac,
					tri.mSinPhis[(ie+1)%3], tri.mCosPhis[(ie+1)%3], tri.mSlowness, 
					tc2, r2);
				//double tcm2 = 0.5 * (mTrvalTimes[ib] + mTrvalTimes[ia]);
				//if (tc2 > tcm2) tc2 = tcm2;
				if (mMidTrvalTimes[tri.mEdges[(ie+1)%3]] > tc2)
				{
					mMidTrvalTimes[tri.mEdges[(ie+1)%3]] = tc2;
				}

				edgeVisited[itri*3 + ie] = true;

				if (tc < mTrvalTimes[ic])
				{
					mTrvalTimes[ic] = tc;
					if (mLTIPointInfos[ic].mHeapIndex < 0)
					{
						mLTIHeap << ic;
						siftUp(mLTIHeap.count() - 1);
					}
					else
					{
						siftUp(mLTIPointInfos[ic].mHeapIndex);
					}
				}
			}
		}

		//printf("Len = %d\n", mLTIHeap.count());
	}

	FILE *fp = fopen("eps.txt", "w");
	fprintf(fp, "X\tY\tT\n");
	for (int i=0; i<mpModel->mPoints.count(); i++)
	{
		const GVector3d &pos = mpModel->mPoints[i];
		double err = fabs(mTrvalTimes[i] - gDistance3D(pos, spt));
		fprintf(fp, "%.4f\t%.4f\t%.8f\n", pos.x(), pos.y(), mTrvalTimes[i]);
		//mTrvalTimes[i] = gDistance2D(pos, spt);
	}
	fclose(fp);

	for (int i=0; i<mpModel->mEdges.count(); i++)
	{
		const GTriEdge &edge = mpModel->mEdges[i];
		GVector3d pos = (mpModel->mPoints[edge[0]] + mpModel->mPoints[edge[1]]) * 0.5;
		//mMidTrvalTimes[i] = gDistance2D(pos, spt);
	}

	//for (int i=0; i<rpts.count(); i++)
	//for (int i=66; i<67; i++)
	for (int i=0; i<rpts.count(); i++)
	{
		fbts[i] = traceRayPath2(spt, rpts[i], rays[i]);
	}
	//printf("end %d\n", time.elapsed());
}

double GTriLTI::plotTimeAndPos(const GPoint3d &pos, int itri, int ie, GPoint3d &nextPos)
{
	const GTriangle &tri = mpModel->mTriangles[itri];
	int ia = tri[ie];
	int ib = tri[(ie+1)%3];
	const GPoint3d &pa = mpModel->mPoints[ia];
	const GPoint3d &pb = mpModel->mPoints[ib];
	double ab = tri.mEdgeLens[ie];
	double bc = gDistance3D(pb, pos);
	double ac = gDistance3D(pa, pos);

	double cosPhi, sinPhi;
	double e = ab * ac;
	if (e < LTI_ZERO)
	{
		cosPhi = 1;
		sinPhi = 0;
	}
	else
	{
		cosPhi = (ab*ab+ac*ac-bc*bc)/(2*ab*ac);
		sinPhi = sqrt(1-cosPhi*cosPhi);
	}

	double tc, r;
	plotTrvalTime(mTrvalTimes[ia], mTrvalTimes[ib],
				  ab, bc, ac,
				  sinPhi, cosPhi, tri.mSlowness, 
				  tc, r);

	double s = r / ab;
	nextPos = pa + (pb - pa) * s;
	return tc;
}

void GTriLTI::getNextPlotEdges(const GPoint3d &nextPos, int itri, int iedge, QList< QPair<int, int> > &nextPlotEdges)
{
	static double eps = 1e-6;
	const GTriangle &tri = mpModel->mTriangles[itri];
	int ip = -1;
	for (int i=0; i<3; i++)
	{
		double d = gDistance3D(nextPos, mpModel->mPoints[tri[i]]);
		if (d < eps)
		{
			ip = i;
			break ;
		}
	}

	// �ڶ�����
	if (ip >= 0)
	{
		GPTRefList &refList = mpModel->mPTRefLists[tri[ip]];
		for (int i=0; i<refList.count(); i++)
		{
			int it = refList[i].first;
			ip = refList[i].second;
			// ��������������
			if (it == itri && iedge >= 0) continue;

			nextPlotEdges << QPair<int, int>(it, (ip+1)%3);
		}
	}
	else if (iedge >= 0)	// �ڱ߽���
	{
		int it = tri.mAdjTris[iedge];
		if (it >= 0)
		{
			int ie = tri.mAdjEdges[iedge];
			nextPlotEdges << QPair<int, int>(it, (ie+1)%3) <<
				QPair<int, int>(it, (ie+2)%3);
		}
	}
	else	// ���������ڲ�
	{
		nextPlotEdges << QPair<int, int>(itri, 0) << QPair<int, int>(itri, 1) << QPair<int, int>(itri, 2);
	}
}

double GTriLTI::traceRayPath(const GPoint3d &spt, const GPoint3d &rpt, QVector<GPoint3d> &ray)
{
	int sit, itri, iedge;
	double fbt = -1;

	sit = spt.getUsrInt();
	if (sit < 0) return -1;

	itri = rpt.getUsrInt();
	if (itri < 0) return -1;

	ray << rpt;
	QList< QPair<int, int> > nextPlotEdges;
	getNextPlotEdges(rpt, itri, -1, nextPlotEdges);
	//nextPlotEdges << QPair<int, int>(itri, 0) << QPair<int, int>(itri, 1) << QPair<int, int>(itri, 2);

	fbt = -1;
	while (!nextPlotEdges.isEmpty())
	{
		// ��ֵ��һ������λ��
		double minTime = MAX_TIME;
		GPoint3d nextPos, pos;
		itri = iedge = -1;
		for (int i=0; i<nextPlotEdges.count(); i++)
		{
			int it = nextPlotEdges[i].first;
			int ie = nextPlotEdges[i].second;
			const GTriangle &tri = mpModel->mTriangles[it];
			if (it != sit && tri.mAdjTris[ie] < 0) continue ;
			 
			// �����ڵ�����������, ֹͣ׷��
			if (it == sit)
			{
				ray.append(spt);
				// ����첨����ڵ���ͬһ����������, ���⴦��
				if (fbt < 0) fbt = gDistance3D(rpt, spt) * tri.mSlowness;
				//fbt += gDistance3D(ray[0], spt) * tri.mSlowness;
				return fbt;
			}

			// ��ֵʱ���λ��
			double tc = plotTimeAndPos(ray.last(), it, ie, pos);
			if (tc < minTime && it >= 0)
			{
				minTime = tc;
				itri = it;
				iedge = ie;
				nextPos = pos;
			}
		}

		// ��¼�첨�����ʱ��
		if (fbt < 0) fbt = minTime;
		if (itri < 0) break ;
		// ���뵱ǰλ��
		nextPos.setUsrInt(itri);
		// �ۼƼ첨��ʱ��
		//fbt += gDistance3D(nextPos, ray[0]) * mpModel->mTriangles[itri].mSlowness;
		ray.append(nextPos);
		

		// Ѱ����һ�εĲ�ֵ��
		nextPlotEdges.clear();
		getNextPlotEdges(nextPos, itri, iedge, nextPlotEdges);
	}
	
	ray.clear();
	return -1;
}

double GTriLTI::traceRayPath2(const GPoint3d &spt, const GPoint3d &rpt, QVector<GPoint3d> &ray)
{
	int sit, itri, iedge;
	double fbt = -1;

	sit = spt.getUsrInt();
	if (sit < 0) return -1;

	itri = rpt.getUsrInt();
	if (itri < 0) return -1;

	ray << rpt;
	QList< QPair<int, int> > nextPlotEdges;
	getNextPlotEdges(rpt, itri, -1, nextPlotEdges);
	//nextPlotEdges << QPair<int, int>(itri, 0) << QPair<int, int>(itri, 1) << QPair<int, int>(itri, 2);
	int iedge0 = -1;

	fbt = -1;
	while (!nextPlotEdges.isEmpty())
	{
		// ��ֵ��һ������λ��
		double minTime = MAX_TIME;
		GPoint3d nextPos, pos;
		itri = iedge = -1;
		for (int i=0; i<nextPlotEdges.count(); i++)
		{
			int it = nextPlotEdges[i].first;
			int ie = nextPlotEdges[i].second;
			const GTriangle &tri = mpModel->mTriangles[it];
			if (it != sit && tri.mAdjTris[ie] < 0) continue ;

			// �����ڵ�����������, ֹͣ׷��
			if (it == sit)
			{
				ray.prepend(spt);
				// ����첨����ڵ���ͬһ����������, ���⴦��
				if (fbt < 0) fbt = gDistance3D(rpt, spt) * tri.mSlowness;
				//fbt += gDistance2D(ray[0], spt) * tri.mSlowness;
				return fbt;
			}

			// ��ֵʱ���λ��
			double tc = plotTimeAndPos2(ray[0], it, ie, pos, iedge0);
			if (tc < minTime && it >= 0)
			{
				minTime = tc;
				itri = it;
				iedge = ie;
				nextPos = pos;
			}
		}

		// ��¼�첨�����ʱ��
		if (fbt < 0) fbt = minTime;
		if (itri < 0) break ;
		// ���뵱ǰλ��
		nextPos.setUsrInt(itri);
		// �ۼƼ첨��ʱ��
		//fbt += gDistance2D(nextPos, ray[0]) * mpModel->mTriangles[itri].mSlowness;
		ray.prepend(nextPos);


		// Ѱ����һ�εĲ�ֵ��
		nextPlotEdges.clear();
		getNextPlotEdges(nextPos, itri, iedge, nextPlotEdges);
		const GTriangle &tri0 = mpModel->mTriangles[itri];
		if (iedge >= 0) iedge0 = tri0.mAdjEdges[iedge];
	}

	ray.clear();
	return -1;
}

}