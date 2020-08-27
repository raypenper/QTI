#ifndef GVECTOR2D_H
#define GVECTOR2D_H

#include "gtuple.h"
#include "core_global.h"

class GMatrix2d;
class GPoint2d;

class CORE_EXPORT GVector2d : public GTuple2d
{   
public:
	enum GCompareType {gCompareByX, gCompareByY};

protected:
	static GCompareType mCompareType;	// 排序类型

protected:
	GVector2d operator * (const GMatrix2d &m) const;
	GVector2d& operator *= (const GMatrix2d &m); 

public:
	GVector2d() : GTuple2d() {}
	GVector2d(double x, double y) { setXY(x, y); }
	GVector2d(const double *v) : GTuple2d(v) {}
	GVector2d(const GVector2d &v) : GTuple2d(v) {}
	GVector2d(const GPoint2d &v);

	double x() const	{ return mTuple[0]; }
	double y() const	{ return mTuple[1]; }

	void setX(double x) { mTuple[0] = x; }
	void setY(double y) { mTuple[1] = y; }
	void setXY(double x, double y)
	{ mTuple[0] = x;	mTuple[1] = y; }
	void setVector(const GVector2d &other)
	{ setXY(other.x(), other.y()); }

	GVector2d& operator = (const GVector2d &other);
	GVector2d& operator = (const GPoint2d &other);

	bool operator < (const GPoint2d &other) const;

	bool operator > (const GPoint2d &other) const;

	static void setCompareType(GCompareType t)
	{ mCompareType = t; }

	static GCompareType getCompareType()
	{ return mCompareType; }

	//----矢量操作--------------------------------------------------------------

	// 向量加法 
	GVector2d operator + (const GVector2d &v) const;
	GVector2d& operator += (const GVector2d &v);

	// 向量减法 
	GVector2d operator - (const GVector2d &v) const;
	GVector2d& operator -= (const GVector2d &v);
	GVector2d operator - () const;

	// 向量数乘 
	GVector2d operator * (double d) const;
	GVector2d& operator *= (double d);

	GVector2d operator * (const GVector2d &v) const;
	GVector2d& operator *= (const GVector2d &v);

	// 向量数除 
	GVector2d operator / (double d) const;
	GVector2d& operator /= (double d);

	// 向量叉积 
	double crossMult(const GVector2d &v) const;

	// 向量点积 
	double dotMult(const GVector2d &v) const;

	// 向量长度 
	double getLength() const;
	// 是否为0长度向量 
	bool isZero() const;

	// 得到归一化向量 
	GVector2d getNormal() const;
	// 将向量归一化 
	void normalize();

	// 得到经变换矩阵m变换后的向量 
	GVector2d transformTo(const GMatrix2d &m) const;
};

#endif