#ifndef GCOORD_CONVERT_H
#define GCOORD_CONVERT_H

#include <QRect>
#include "grect2d.h"
#include "gpoint2d.h"
#include "core_global.h"

// 二维物理<->逻辑坐标转换基类
class GCoordConvert
{
public:
	
	// 得到X变换比例(米/像素)
	virtual double getScaleX() = 0;

	// 得到Y变换比例(米/像素)
	virtual double getScaleY() = 0;

	// 逻辑坐标X到物理坐标X
	virtual int		lpXTodpX(double lx) = 0;	
	
	// 物理坐标X到逻辑坐标X		
	virtual double	dpXTolpX(int dx) = 0;	
	
	// 逻辑坐标Y到物理坐标Y		
	virtual int		lpYTodpY(double ly) = 0;	
	
	// 物理坐标Y到逻辑坐标Y		
	virtual double	dpYTolpY(int dy) = 0;	

	// 控制点逻辑坐标到物理坐标
	virtual QPoint lpTodp(const GPoint2d &lpt)
	{
		return QPoint(lpXTodpX(lpt.x()), lpYTodpY(lpt.y()));
	}

	// 控制点物理坐标到逻辑坐标
	virtual GPoint2d dpTolp(const QPoint &dpt)
	{
		return GPoint2d(dpXTolpX(dpt.x()), dpYTolpY(dpt.y()));
	}

	// 矩形逻辑坐标到物理坐标
	virtual QRect lpTodp(const GRect2d &lr)
	{
		return QRect(lpTodp(GPoint2d(lr.x0(), lr.y1())),
					 lpTodp(GPoint2d(lr.x1(), lr.y0())));
	}

	// 矩形物理坐标到逻辑坐标
	virtual GRect2d dpTolp(const QRect &dr)
	{
		return GRect2d(dpXTolpX(dr.left()), dpYTolpY(dr.bottom()),
					   dpXTolpX(dr.right()), dpYTolpY(dr.top()));
	}
};		

#endif