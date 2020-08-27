#ifndef GUTILS_H
#define GUTILS_H

#include <qstring.h>
#include <qfile.h>
#include <qdatastream.h>
#include <stdlib.h>
#include <ctype.h>
#include <qcolor.h>
#include <QPainter>
#include <QPen>
#include <QColor>
#include <QFontMetrics>
#include "gvaluevector.h"
#include "core_global.h"

extern CORE_EXPORT const QString QStringNull;

#define DEFAULT_COLOR_COUNT 18

extern CORE_EXPORT QString gDefaultColorNames[];

extern CORE_EXPORT QColor gDefaultColors[];

// ����2�ֽڸߵ�˳��
void CORE_EXPORT gSwapByte2(char *to, const char *from);

// ����4�ֽڸߵ�˳��
void CORE_EXPORT gSwapByte4(char *to, const char *from);

// ���ļ���������ָ���ֽ�
void CORE_EXPORT gSkipPadBytes(QDataStream &stream, int nbytes);

// �ļ�����
bool CORE_EXPORT gFileCopy(const QString &dst, const QString &src);

// ��������
double CORE_EXPORT gAdjustStep(double step0, int &exp);

// ��������
template <class T>
void gSwap(T &a, T &b)
{
	T tmp = a;
	a = b;
	b = tmp;
}

// ����1ά����
template <class T>
T * gAlloc1(T *&p, int n1)
{
	p = new T[n1];
	return p;
}

// �ͷ�1ά����
template <class T>
void gFree1(T *&p)
{
	delete []p;
	p = NULL;
}

// Ϊ1ά�������ָ����ֵv
template <class T>
void gFill1(T *p, T v, int n1)
{
	int i;
	for(i=0; i<n1; i++) p[i] = v;
}

// 1ά���鿽��
template <class T>
void gCopy1(T *dst, const T *src, int n1)
{
	int i;
	for(i=0; i<n1; i++) dst[i] = src[i];
}

// ����2ά����
template <class T>
T ** gAlloc2(T **&p, int n2, int n1)
{
	int i, size;

	size = sizeof(T);
	p = new T *[n2];
	if(p == NULL) return NULL;

	p[0] = new T[n1*n2];
	if(p[0] == NULL) 
	{
		delete []p;
		p = NULL;
		return NULL;
	}

	for(i=0; i<n2; i++)
	{
		p[i] = p[0] + n1*i;
	}
	return p;
}

// �ͷ�2ά����
template <class T>
void gFree2(T **&p)
{
	delete []p[0];
	delete []p;
	p = NULL;
}

// 2ά�������ָ����ֵv
template <class T>
void gFill2(T **p, T v, int n2, int n1)
{
	int i, j;
	for(i=0; i<n2; i++) 
	{
		for(j=0; j<n1; j++)
			p[i][j] = v;
	}
}

// 2ά���鿽��
template <class T>
void gCopy2(T **dst, const T **src, int n2, int n1)
{
	int i, j;

	for(i=0; i<n2; i++) 
	{
		for(j=0; j<n1; j++)
			dst[i][j] = src[i][j];
	}
}

// ����3ά����
template <class T>
T *** gAlloc3(T ***&p, int n3, int n2, int n1)
{
	int i, j;

	p = new T **[n3];
	if(p == NULL) return NULL;

	p[0] = new T *[n3*n2];
	if(p[0] == NULL)
	{
		delete []p;
		p = NULL;
		return NULL;
	}

	p[0][0] = new T [n3*n2*n1];
	if(p[0][0] == NULL)
	{
		delete []p[0];
		delete []p;
		p = NULL;
		return NULL;
	}

	for(i=0; i<n3; i++)
	{
		p[i] = p[0]+n2*i;
		for(j=0; j<n2; j++)
		{
			p[i][j] = p[0][0]+n1*(j+n2*i);
		}
	}

	return p;
}

// �ͷ���ά����
template <class T>
void gFree3(T ***&p)
{
	delete []p[0][0];
	delete []p[0];
	delete []p;
	p = NULL;
}

// Ϊ3ά�������ָ����ֵv
template <class T>
void gFill3(T ***p, T v, int n3, int n2, int n1)
{
	int i, j, k;
	for(i=0; i<n3; i++) 
	{	
		for(j=0; j<n2; j++)
		{
			for(k=0; k<n1; k++)
				p[i][j][k] = v;
		}
	}
}

// 3ά���鿽��
template <class T>
void gCopy3(T ***dst, const T ***src, int n3, int n2, int n1)
{
	int i, j, k;

	for(i=0; i<n3; i++) 
	{
		for(j=0; j<n2; j++)
		{
			for(k=0; k<n1; k++)
				dst[i][j][k] = src[i][j][k];
		}
	}
}

// �ӻ���������GVector��������
template <class T>
void gReadValues(const QByteArray &buf, GValueVector<T> &v)
{
	Q_INT32 i, n;
	T d;
	v.clear();
	if(buf.count() <= 0) return ;

	QDataStream stream(buf, IO_ReadOnly);
	stream >> n;
	for(i=0; i<n; i++)
	{
		stream >> d;
		v.append(d);
	}
}

// ��GVector�����е�����д�뻺����
template <class T>
void gWriteValues(QByteArray &buf, GValueVector<T> &v)
{
	int i;
	QDataStream stream(buf, IO_WriteOnly);
	stream << (Q_INT32)v.count();
	for(i=0; i<v.count(); i++)
	{
		stream << v[i];
	}
}

template <class T>
void gClearPtrVector(QVector<T> &v)
{
	int i;
	for(i=0; i<v.count(); i++)
	{
		delete v[i];
	}
	v.clear();
}

/************************************************************************/
/*      gDrawText() ���ڻ������С�ߴ������

text           -- �����Ƶ�����
font           -- ����������ʹ�õ����壬����font���õ��ֺ�
textCenter     -- �������ĵ��λ��(��λ����)
width,height   -- ���ֵĿ�ȼ��߶ȣ�����������һ��������Ϊ-1�����ֵ
	              ����һ�����aspectRatio����ó� 
aspectRatio    -- ��߱�
rotationAngle  -- �����������ת��ptOfRot����ת��                       
ptOfRot        -- ��������ڸĵ������ת. 0�������ĵ�, 
                  1�������½ǵ�   
				                                                        */
/************************************************************************/
void CORE_EXPORT gDrawText( QPainter * p, const QString& text, const QFont& font, const QPoint& textCenter, int width, int height, double aspectRatio = 1.0, double rotationAngle = 0.0, int ptOfRot = 1);

QRect CORE_EXPORT gGetTextRect(const QString& text, const QFont& font, const QPoint& textCenter, int width, int height, double aspectRatio = 1.0);

QPolygon CORE_EXPORT gGetTextRegion(const QString& text, const QFont& font, const QPoint& textCenter, int width, int height, double aspectRatio = 1.0, double rotationAngle = 0.0, int ptOfRot = 1);

/*
 *	�����������ĵ��ȡ���ֵ����½�����

	text             -- ��������
	font             -- ����
	centerX, centerY -- �������ĵ�����
	height           -- ����߶�
 */
QPointF CORE_EXPORT gGetLowerLeftPos(const QString& text, const QFont& font, double centerX, double centerY, double height);

#endif
