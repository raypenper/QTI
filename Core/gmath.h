#ifndef GMATH_H
#define GMATH_H

#include <math.h>
#include "core_global.h"

class GVector2d;
class GVector3d;
class GPoint2d;
class GPoint3d;
class GPolyLine2d;
class GRect2d;

#define PI 3.1415926535
#define RADIAN_TO_ANGLE 57.29577958
#define ANGLE_TO_RADIAN 0.01745329
#define GMATH_ZERO		1.0E-8
#define IS_ZERO(x)		(fabs(x) < GMATH_ZERO)
#define IRD(x)	((unsigned __int64 &)x)

// 求两个数中较小者
template <class TData> TData gGetMin(const TData &x, const TData &y)
{
	return x < y ? x : y;
}

// 求两个数中较大者
template <class TData> TData gGetMax(const TData &x, const TData &y)
{
	return x > y ? x : y;
}

// 求数组中最小元素的下标
template <class TData> int gGetMin(const TData *data, int begin, int end)
{
	int index = begin;
	TData min = data[index];
	for(int i=begin+1; i<=end; i++)
	{
		if(data[i] < min)
		{
			min = data[i];
			index = i;
		}
	}
	return index;
}


// 求数组中最大元素的下标
template <class TData> int gGetMax(const TData *data, int begin, int end)
{
	int index = begin;
	TData max = data[index];
	for(int i=begin+1; i<=end; i++)
	{
		if(data[i] > max)
		{
			max = data[i];
			index = i;
		}
	}
	return index;
}

// 四舍五入
int CORE_EXPORT gRound( double x );

// 取整
int CORE_EXPORT gTrunc( double x );

// 平方
double CORE_EXPORT gSqr( double x );

// 2D旋转
void CORE_EXPORT gRotate(double sinAF, double cosAF, double sx, double sy, double &dx, double &dy);

// 根据sin和cos值确定角度
double CORE_EXPORT gGetAngle(double sinAF, double cosAF);

// 计算点线关系(逆时针方向), p点在直线(p1,p2)内侧 = 1, 外侧 = -1, 在线上 = 0, p1和p2重合 = -2
int CORE_EXPORT gGetPtLineRelative(const GVector2d &p, const GVector2d &p1, const GVector2d &p2);

// 计算向量v0与v1在[0~2*PI]范围内的逆时针绕转夹角
// (对于块体追踪两向量逆时针夹角最大的为首选追踪方向)
double CORE_EXPORT gGetVectorAngle(const GVector2d &v0, const GVector2d &v1);

// 将pt投影到平面上, o为平面上一点, n为平面法向量
GVector3d CORE_EXPORT gPrjToPlane(const GVector3d &o, const GVector3d &n, const GVector3d &pt);

// 点是否在多边形中
bool CORE_EXPORT gPtInPolygon(const GVector2d &point, const GVector2d *polygon, int n);
bool CORE_EXPORT gPtInPolygon(const GPoint2d &point, const GPoint2d *polygon, int n);

// 判断点序列是否是逆时针的
bool CORE_EXPORT gIsAntiClockwise(const GVector2d *points, int count);
bool CORE_EXPORT gIsAntiClockwise(const GPoint2d *points, int count);

// 判断给定多边形是否为凹的(顶点必须按逆时针顺序排列)
bool CORE_EXPORT gIsConcave(const GVector2d *poly, int count);
bool CORE_EXPORT gIsConcave(const GPoint2d *poly, int count);

// 直线段编码裁剪                   
bool CORE_EXPORT gLineClip2d(GVector2d &pt0, GVector2d &pt1, const GRect2d &rect);

// 多边形单边裁剪 
void CORE_EXPORT gPolyClip2d(const GPolyLine2d &src, GPolyLine2d &dst, const GRect2d &rect);

// 计算pt0, pt1, pt2三点构成平面的法向量
GVector3d CORE_EXPORT gGetNormal(const GVector3d &pt0, const GVector3d &pt1, const GVector3d &pt2);
GVector3d CORE_EXPORT gGetNormal(const GVector3d *pts);
GVector3d CORE_EXPORT gGetNormal(const GPoint3d *pts);

// 求3阶行列式的值
double CORE_EXPORT gDeterm3(double a1, double a2, double a3, double b1, double b2, double b3,
				double c1, double c2, double c3);

// 线性最小二乘拟合
bool CORE_EXPORT gFittingLine(double *xArray, double *yArray, int firstIndex, int lastIndex,
				  double &a, double &b);

// 二维最小二乘面拟合
bool CORE_EXPORT gFittingPlane(double *x, double *y, double *z, int n, double &a, double &b, double &c);

// 二维主成分分析
void CORE_EXPORT gPCA(GVector2d *pts, int n, double *eigens, GVector2d *vectors);
void CORE_EXPORT gPCA(GPoint2d *pts, int n, double *eigens, GVector2d *vectors);

// 求逆矩阵
bool CORE_EXPORT gInverseMatrix(double *m, int n);

// 选主元三角分解法
void CORE_EXPORT gSelSlove(double **matrix, double *result, int dim);	

// 追赶法求解三对角矩阵
void CORE_EXPORT gTridagSlove( double *aArray, double *bArray,
					double *cArray, double *dArray,
					int count, double *rArray );

// 追赶法求解扩展三对角方程 (方程中a1与cn存在, 适合闭合三切矢方程) 
void CORE_EXPORT gTridagExSlove(double aArray[], double bArray[],
				   double cArray[], double dArray[],
				   int count, double rArray[]); 

// 解一元二次方程
int CORE_EXPORT gSolveTwoeTimeEqu(double a[3], double r[2]);

// 解一元三次方程
int CORE_EXPORT gSolveThreeTimeEqu(double a[4], double r[3]);

// 牛顿迭代法解一元三次方程
double CORE_EXPORT gNewtonSolveThreeTimeEqu(double a[4], double r, 
							  double errLimit = 0.01, int itCount = 32);

// 龙贝格求积法
// func:	被积函数指针
// a, b:	积分区间
// eps:		积分精度
double CORE_EXPORT gRombergInt(double (*func)(double), double a, double b, int nh=1, double eps=1e-6);

// 变步长求积法
// func:	被积函数指针
// a, b:	积分区间
// h:		最小步长
// eps:		积分精度
double CORE_EXPORT gTrapzInt(double (*func)(double), double a, double b, double h, double eps=1e-6);

// 两个向量之间的线性插值. 按照数字t[0, 1]在form到to之间插值
GVector3d CORE_EXPORT gLerpVector(const GVector3d &from, 
								  const GVector3d &to, 
								  double t);

#endif
