#ifndef GCOORD_CONVERT_H
#define GCOORD_CONVERT_H

#include <QRect>
#include "grect2d.h"
#include "gpoint2d.h"
#include "core_global.h"

// ��ά����<->�߼�����ת������
class GCoordConvert
{
public:
	
	// �õ�X�任����(��/����)
	virtual double getScaleX() = 0;

	// �õ�Y�任����(��/����)
	virtual double getScaleY() = 0;

	// �߼�����X����������X
	virtual int		lpXTodpX(double lx) = 0;	
	
	// ��������X���߼�����X		
	virtual double	dpXTolpX(int dx) = 0;	
	
	// �߼�����Y����������Y		
	virtual int		lpYTodpY(double ly) = 0;	
	
	// ��������Y���߼�����Y		
	virtual double	dpYTolpY(int dy) = 0;	

	// ���Ƶ��߼����굽��������
	virtual QPoint lpTodp(const GPoint2d &lpt)
	{
		return QPoint(lpXTodpX(lpt.x()), lpYTodpY(lpt.y()));
	}

	// ���Ƶ��������굽�߼�����
	virtual GPoint2d dpTolp(const QPoint &dpt)
	{
		return GPoint2d(dpXTolpX(dpt.x()), dpYTolpY(dpt.y()));
	}

	// �����߼����굽��������
	virtual QRect lpTodp(const GRect2d &lr)
	{
		return QRect(lpTodp(GPoint2d(lr.x0(), lr.y1())),
					 lpTodp(GPoint2d(lr.x1(), lr.y0())));
	}

	// �����������굽�߼�����
	virtual GRect2d dpTolp(const QRect &dr)
	{
		return GRect2d(dpXTolpX(dr.left()), dpYTolpY(dr.bottom()),
					   dpXTolpX(dr.right()), dpYTolpY(dr.top()));
	}
};		

#endif