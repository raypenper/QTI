/**************************************************
Copyright (c) 2020, Deng fei & Fang Peng. All rights reserved.
QTI is available under two different licenses:
GNU General Public License v3.0 (GPLv3)
Commercial license (please contact at Deng Fei(at)dengfei@cdut.cn)
/*************************************************/
#ifndef GTRI_LTI_H
#define GTRI_LTI_H

#include <QVector>
#include <QList>
#include <QPair>
#include "gpoint2d.h"

class GTriModel; 

// forward modeling using traveltime interpolation 
// based on Triangulation Network
class GTriLTI 
{
protected:
	// extra vertex information structure for LTI
	struct LTIPointInfo
	{
		int		mHeapIndex;		// vertex index in heap
		bool	mIsFixed;		// if the vertex was a subseismic source

		LTIPointInfo(int index=-1, bool isFixed=false)
		{
			mHeapIndex = index;
			mIsFixed = isFixed;
		}
	};

protected:
	QVector<double>		mTrvalTimes;	// traveltime table
	QVector<double>		mMidTrvalTimes;	// midpoint traveltime table for QTI
	GTriModel *			mpModel;		// triangular mesh model
	QVector<LTIPointInfo> mLTIPointInfos;	// vertex information for LTI
	QVector<int>		mLTIHeap;		// traveltime heap for LTI(vertex index )

public:
	static double		MAX_TIME;		// maximum traveltime

protected:

	// downward adjustments in heap
	void siftDown(int index);

	// upward adjustments in heap
	void siftUp(int index);

	// evaluate the minimum traveltime and the corresponding coordinate on the edge using LTI method
	double plotTimeAndPos(const GPoint2d &pos, int itri, int ie, GPoint2d &nextPos);
	// evaluate the minimum traveltime and the corresponding coordinate on the edge using QTI method
	double plotTimeAndPos2(const GPoint2d &pos, int itri, int ie, GPoint2d &nextPos, int ie0=-1);

	// derive the next available edge for interpolation
	void getNextPlotEdges(const GPoint2d &nextPos, int itri, int iedge, QList< QPair<int, int> > &nextPlotEdges);

	// ray tracing from source point, and return traveltime of receiving points(using LTI method)
	double traceRayPath(const GPoint2d &spt, const GPoint2d &rpt, QVector<GPoint2d> &ray);
	// ray tracing from source point, and return traveltime of receiving points(using QTI method)
	double traceRayPath2(const GPoint2d &spt, const GPoint2d &rpt, QVector<GPoint2d> &ray);

public:
	GTriLTI(GTriModel *pModel=NULL);
	~GTriLTI();

	void setModel(GTriModel *pModel);
	
	GTriModel * getModel();

	// Forward modeling of first arrivals and ray trajectories using LTI method
	void forward(const GPoint2d &spt, const QVector<GPoint2d> &rpts,
				 QVector<double> &fbts, QList< QVector<GPoint2d> > &rays);

	// Forward modeling of first arrivals and ray trajectories using QTI method
	void forwardQTI(const GPoint2d &spt, const QVector<GPoint2d> &rpts,
				  QVector<double> &fbts, QList< QVector<GPoint2d> > &rays);

	// return the traveltime of all vertices
	const QVector<double> getTrvalTimes() const;
};

#endif
