#ifndef GSPLINE_H
#define GSPLINE_H

#include <QVector>
#include "gpoint2d.h"
#include "gpoint3d.h"
#include "core_global.h"

struct SplineParam	// һ����������ϵ��
{
	// a*t^3 + b*t^2 + c*t + d
	double a, b, c, d;

	SplineParam()
	{
		a = b = c = d = 0;
	}
};

class CORE_EXPORT GSpline2d
{
private:
	GPoint2d *mPoints;
	int mCount;
	SplineParam *mParams[2];
	double *mChordLengths;
	bool mIsClosed;				// �Ƿ�Ϊ���������
protected:
	// ����������������ɢ���б�pts
	// ispΪ�������߷ֶ��±�, t0, t1Ϊ��ǰ�ֶ�������������еĲ���
	// diffΪ��Сƫ��, stepΪ��С����
	// �����ǰ�ֶγ���С��step�����е����С��diff����ϸ��
	void discrete(QVector<GPoint2d> &pts, QVector<int> &incs, int i, int isp, 
				  double t0, double t1, double diff, double step);
public:
	GSpline2d();
	~GSpline2d();
	
	void clear();
	
	// ��ֵ����
	void interpolate(GPoint2d *points, int count);
	
	// �����index������, �ҳ���������tʱ����������
	GPoint2d get(int index, double t);
	
	// �õ���index�����ߵ��ҳ�
	double getChordLength(int index);

	// �õ���index�����ߵ�ϵ��(c=0 x, c=1 y, c=2 z)
	void getParam(int index, int c, double a[]);

	bool getIsClosed()
	{ return mIsClosed; }

	// �����������õ���ɢ����
	// pts:		��ɢ����
	// incs:	����ÿ�Կ��Ƶ�������ɢ��ĸ���
	// diff:	���������ɢ���������е��������������С��diffֹͣ�����µ���ɢ��
	// step:	������ɢ�����С����, ���������ɢ��<step, û�дﵽ���diffҲֹͣ�����
	void discrete(QVector<GPoint2d> &pts, QVector<int> &incs, double diff, double step);

	// ������������ɢ��Ϊn����(n >= ���Ƶ���Ŀ)
	// pts:		��ɢ����
	// incs:	����ÿ�Կ��Ƶ�������ɢ��ĸ���
	// n:		Ԥ����ɢ����Ŀ
	void discrete(QVector<GPoint2d> &pts, QVector<int> &incs, int n);
};

class CORE_EXPORT GSpline3d
{
private:
	GPoint3d *mPoints;
	int mCount;
	SplineParam *mParams[3];
	double *mChordLengths;
protected:
	// ����������������ɢ���б�pts
	// ispΪ�������߷ֶ��±�, t0, t1Ϊ��ǰ�ֶ�������������еĲ���
	// diffΪ��Сƫ��, stepΪ��С����
	// �����ǰ�ֶγ���С��step�����е����С��diff����ϸ��
	void discrete(QVector<GPoint3d> &pts, QVector<int> &incs, int i, int isp, 
				  double t0, double t1, double diff, double step);
public:
	GSpline3d();
	~GSpline3d();

	void clear();

	// ��ֵ����
	void interpolate(GPoint3d *points, int count);

	// �����index������, �ҳ���������tʱ����������
	GPoint3d get(int index, double t);

	// �õ���index�����ߵ��ҳ�
	double getChordLength(int index);

	// �õ���index�����ߵ�ϵ��(c=0 x, c=1 y, c=2 z)
	void getParam(int index, int c, double a[]);

	// �����������õ���ɢ����
	// pts:		��ɢ����
	// incs:	����ÿ�Կ��Ƶ�������ɢ��ĸ���
	// diff:	���������ɢ���������е��������������С��diffֹͣ�����µ���ɢ��
	// step:	������ɢ�����С����, ���������ɢ��<step, û�дﵽ���diffҲֹͣ�����
	void discrete(QVector<GPoint3d> &pts, QVector<int> &incs, double diff, double step);

	// ������������ɢ��Ϊn����(n >= ���Ƶ���Ŀ)
	// pts:		��ɢ����
	// incs:	����ÿ�Կ��Ƶ�������ɢ��ĸ���
	// n:		Ԥ����ɢ����Ŀ
	void discrete(QVector<GPoint3d> &pts, QVector<int> &incs, int n);


	void discrete(QVector<GPoint3d>& discretePts, const QVector<int>& nDivisionPerSeg);
};

#endif
