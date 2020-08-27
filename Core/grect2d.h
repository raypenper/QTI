#ifndef GRECT2D_H
#define GRECT2D_H

#include "gvector2d.h"
#include "core_global.h"

class CORE_EXPORT GRect2d 
{
protected:
	GVector2d mPoints[2];
public:
	GRect2d();
	GRect2d(double x0, double y0, double x1, double y1);
	GRect2d(const GRect2d &other);

	~GRect2d();

	GRect2d & operator = (const GRect2d &other);

	operator const GVector2d * () const
	{ return mPoints; }

	operator GVector2d * ()
	{ return mPoints; }

	double width() const			
	{ return mPoints[1].x() - mPoints[0].x(); }
	double height() const			
	{ return mPoints[1].y() - mPoints[0].y(); }

	double x0()	const				{ return mPoints[0].x(); }
	double y0()	const				{ return mPoints[0].y(); }
	double x1()	const				{ return mPoints[1].x(); }
	double y1()	const				{ return mPoints[1].y(); }
	GVector2d leftTop() const		{ return mPoints[0]; }
	GVector2d rightBottom() const	{ return mPoints[1]; }

	void setX0(double x)
	{ mPoints[0].setX(x); }
	void setY0(double y)
	{ mPoints[0].setY(y); }
	void setX1(double x)
	{ mPoints[1]. setX(x); }
	void setY1(double y)
	{ mPoints[1].setY(y); }

	void setLeftTop(double x0, double y0)
	{ mPoints[0].setXY(x0, y0); }
	void setLeftTop(const GVector2d &pt)
	{ mPoints[0] = pt; }

	void setRightBottom(double x1, double y1)
	{ mPoints[1].setXY(x1, y1); }
	void setRightBottom(const GVector2d &pt)
	{ mPoints[1] = pt; }

	void set(double x0, double y0, double x1, double y1);
	void set(const GVector2d &pt0, const GVector2d &pt1);
	void set(const GRect2d &other);

	// ����dx0, dy0, dx1, dy1��չ����
	void adjust(double dx0, double dy0, double dx1, double dy1);

	// �ƶ�����(�ƶ�����dx, dy)
	void move(double dx, double dy);

	// �Ƿ������һ������
	bool contain(const GRect2d &other) const;

	// �Ƿ������
	bool contain(const GVector2d &pt) const;
};

#endif
