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

// ���������н�С��
template <class TData> TData gGetMin(const TData &x, const TData &y)
{
	return x < y ? x : y;
}

// ���������нϴ���
template <class TData> TData gGetMax(const TData &x, const TData &y)
{
	return x > y ? x : y;
}

// ����������СԪ�ص��±�
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


// �����������Ԫ�ص��±�
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

// ��������
int CORE_EXPORT gRound( double x );

// ȡ��
int CORE_EXPORT gTrunc( double x );

// ƽ��
double CORE_EXPORT gSqr( double x );

// 2D��ת
void CORE_EXPORT gRotate(double sinAF, double cosAF, double sx, double sy, double &dx, double &dy);

// ����sin��cosֵȷ���Ƕ�
double CORE_EXPORT gGetAngle(double sinAF, double cosAF);

// ������߹�ϵ(��ʱ�뷽��), p����ֱ��(p1,p2)�ڲ� = 1, ��� = -1, ������ = 0, p1��p2�غ� = -2
int CORE_EXPORT gGetPtLineRelative(const GVector2d &p, const GVector2d &p1, const GVector2d &p2);

// ��������v0��v1��[0~2*PI]��Χ�ڵ���ʱ����ת�н�
// (���ڿ���׷����������ʱ��н�����Ϊ��ѡ׷�ٷ���)
double CORE_EXPORT gGetVectorAngle(const GVector2d &v0, const GVector2d &v1);

// ��ptͶӰ��ƽ����, oΪƽ����һ��, nΪƽ�淨����
GVector3d CORE_EXPORT gPrjToPlane(const GVector3d &o, const GVector3d &n, const GVector3d &pt);

// ���Ƿ��ڶ������
bool CORE_EXPORT gPtInPolygon(const GVector2d &point, const GVector2d *polygon, int n);
bool CORE_EXPORT gPtInPolygon(const GPoint2d &point, const GPoint2d *polygon, int n);

// �жϵ������Ƿ�����ʱ���
bool CORE_EXPORT gIsAntiClockwise(const GVector2d *points, int count);
bool CORE_EXPORT gIsAntiClockwise(const GPoint2d *points, int count);

// �жϸ���������Ƿ�Ϊ����(������밴��ʱ��˳������)
bool CORE_EXPORT gIsConcave(const GVector2d *poly, int count);
bool CORE_EXPORT gIsConcave(const GPoint2d *poly, int count);

// ֱ�߶α���ü�                   
bool CORE_EXPORT gLineClip2d(GVector2d &pt0, GVector2d &pt1, const GRect2d &rect);

// ����ε��߲ü� 
void CORE_EXPORT gPolyClip2d(const GPolyLine2d &src, GPolyLine2d &dst, const GRect2d &rect);

// ����pt0, pt1, pt2���㹹��ƽ��ķ�����
GVector3d CORE_EXPORT gGetNormal(const GVector3d &pt0, const GVector3d &pt1, const GVector3d &pt2);
GVector3d CORE_EXPORT gGetNormal(const GVector3d *pts);
GVector3d CORE_EXPORT gGetNormal(const GPoint3d *pts);

// ��3������ʽ��ֵ
double CORE_EXPORT gDeterm3(double a1, double a2, double a3, double b1, double b2, double b3,
				double c1, double c2, double c3);

// ������С�������
bool CORE_EXPORT gFittingLine(double *xArray, double *yArray, int firstIndex, int lastIndex,
				  double &a, double &b);

// ��ά��С���������
bool CORE_EXPORT gFittingPlane(double *x, double *y, double *z, int n, double &a, double &b, double &c);

// ��ά���ɷַ���
void CORE_EXPORT gPCA(GVector2d *pts, int n, double *eigens, GVector2d *vectors);
void CORE_EXPORT gPCA(GPoint2d *pts, int n, double *eigens, GVector2d *vectors);

// �������
bool CORE_EXPORT gInverseMatrix(double *m, int n);

// ѡ��Ԫ���Ƿֽⷨ
void CORE_EXPORT gSelSlove(double **matrix, double *result, int dim);	

// ׷�Ϸ�������ԽǾ���
void CORE_EXPORT gTridagSlove( double *aArray, double *bArray,
					double *cArray, double *dArray,
					int count, double *rArray );

// ׷�Ϸ������չ���ԽǷ��� (������a1��cn����, �ʺϱպ�����ʸ����) 
void CORE_EXPORT gTridagExSlove(double aArray[], double bArray[],
				   double cArray[], double dArray[],
				   int count, double rArray[]); 

// ��һԪ���η���
int CORE_EXPORT gSolveTwoeTimeEqu(double a[3], double r[2]);

// ��һԪ���η���
int CORE_EXPORT gSolveThreeTimeEqu(double a[4], double r[3]);

// ţ�ٵ�������һԪ���η���
double CORE_EXPORT gNewtonSolveThreeTimeEqu(double a[4], double r, 
							  double errLimit = 0.01, int itCount = 32);

// �����������
// func:	��������ָ��
// a, b:	��������
// eps:		���־���
double CORE_EXPORT gRombergInt(double (*func)(double), double a, double b, int nh=1, double eps=1e-6);

// �䲽�������
// func:	��������ָ��
// a, b:	��������
// h:		��С����
// eps:		���־���
double CORE_EXPORT gTrapzInt(double (*func)(double), double a, double b, double h, double eps=1e-6);

// ��������֮������Բ�ֵ. ��������t[0, 1]��form��to֮���ֵ
GVector3d CORE_EXPORT gLerpVector(const GVector3d &from, 
								  const GVector3d &to, 
								  double t);

#endif
