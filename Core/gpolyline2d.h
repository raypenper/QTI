#ifndef GPOLYLINE2D_H
#define GPOLYLINE2D_H

#include <QVector>
#include "gpoint2d.h"
#include "grect2d.h"
#include "core_global.h"

class CORE_EXPORT GPolyLine2d
{
protected:
	QVector<GPoint2d> mPoints;			// ���Ƶ��
protected:
	// �ݹ麯�����ܵ�i������
	void biIncrease(int i, double step);

	// �ݹ麯�����ܵ�i������(�պ��������)
	void biCloseIncrease(int &i, double d, const GPoint2d &p0, const GPoint2d &p1);
public:
	GPolyLine2d();
	GPolyLine2d(const GPolyLine2d &other);

	virtual ~GPolyLine2d();

	GPolyLine2d & operator = (const GPolyLine2d &other);

	// ������п��Ƶ�
	virtual void clear();

	// ���ؿ��Ƶ���Ŀ
	int count() const;

	// ����ָ���Ŀ��Ƶ�
	const GPoint2d & get(int index) const;

	// ���ؿ��Ƶ�����ͷָ��
	operator GPoint2d *();

	// ����ָ�����Ƶ�
	void set(int index, const GPoint2d &pt);

	const GPoint2d & begin()
	{ return mPoints[0]; }

	const GPoint2d & end()
	{ return mPoints[count()-1]; }

	// ��β��׷�ӿ��Ƶ�
	void addLast(const GPoint2d &point);

	// ��ͷ��������Ƶ�
	void addFirst(const GPoint2d &point);

	// ��ָ��λ�ò�����Ƶ�
	bool insert(int index, const GPoint2d &point);

	// ɾ��β�����Ƶ�
	void removeLast();

	// ɾ��ͷ�����Ƶ�
	void removeFirst();

	// ɾ��ָ�����Ƶ�
	bool remove(int index);

	// �����Ƶ����з�ת
	void inverse();

	// ����x����, �õ����ڵķֶ��±�
	int getRangeIndex(double x);

	int getRangeIndex(double x, double y, double diff, double *pDistance = 0);

	// �������x���괦���ߵ�y����
	double getY(double x, bool *ok = 0);

	// ���Ƿ��ڼ��ο��Ƶ���
	int ptInPoint(double x, double y, double diff);
	int ptInPoint(const GPoint2d &point, double diff);

	//���Ƿ�������
	bool ptInLine(double x, double y, double diff);
	bool ptInLine(const GPoint2d &point, double diff);

	// ��㵽���ߵ���̾���, ri��������̾���ķֶ��±�
	double getDistance(const GPoint2d &point, int *ri = 0);

	// �õ�������point, �����ߵ�ri�ֶεĲ���t
	// (һ��: Ӧ������ptInLineȷ���������ߵĵ�ri�ֶ���)
	double getParamT(const GPoint2d &point, int ri);

	// ����׷�ӿ��Ƶ�
	void append(GPolyLine2d *pLine, int i0, int i1);

	// ����ǰ����Ƶ�
	void prepend(GPolyLine2d *pLine, int i0, int i1);

	// ���ּ�������(�Ӻ���ǰ�������)
	void biIncrease(double step);

	// ���ּ��ܱպ�����(�Ӻ���ǰ�������)
	void biCloseIncrease(double step);

	// �������߿��Ƶľ��η�Χ
	GRect2d calcluteCtrlRect();

	// ��������ɾ��λ�ýӽ��ĵ�, diffΪ�ӽ�����ֵ
	void removeSamePts(double diff = 10e-8);

	//�����������
	double getArea();

	// �������ε����������
	GPoint2d getCenter();
};

#endif
