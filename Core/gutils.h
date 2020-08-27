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

// 交换2字节高低顺序
void CORE_EXPORT gSwapByte2(char *to, const char *from);

// 交换4字节高低顺序
void CORE_EXPORT gSwapByte4(char *to, const char *from);

// 在文件流中跳过指定字节
void CORE_EXPORT gSkipPadBytes(QDataStream &stream, int nbytes);

// 文件拷贝
bool CORE_EXPORT gFileCopy(const QString &dst, const QString &src);

// 调整步长
double CORE_EXPORT gAdjustStep(double step0, int &exp);

// 交换变量
template <class T>
void gSwap(T &a, T &b)
{
	T tmp = a;
	a = b;
	b = tmp;
}

// 分配1维数组
template <class T>
T * gAlloc1(T *&p, int n1)
{
	p = new T[n1];
	return p;
}

// 释放1维数组
template <class T>
void gFree1(T *&p)
{
	delete []p;
	p = NULL;
}

// 为1维数组填充指定数值v
template <class T>
void gFill1(T *p, T v, int n1)
{
	int i;
	for(i=0; i<n1; i++) p[i] = v;
}

// 1维数组拷贝
template <class T>
void gCopy1(T *dst, const T *src, int n1)
{
	int i;
	for(i=0; i<n1; i++) dst[i] = src[i];
}

// 分配2维数组
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

// 释放2维数组
template <class T>
void gFree2(T **&p)
{
	delete []p[0];
	delete []p;
	p = NULL;
}

// 2维数组填充指定数值v
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

// 2维数组拷贝
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

// 分配3维数组
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

// 释放三维数组
template <class T>
void gFree3(T ***&p)
{
	delete []p[0][0];
	delete []p[0];
	delete []p;
	p = NULL;
}

// 为3维数组填充指定数值v
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

// 3维数组拷贝
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

// 从缓存中载入GVector数组内容
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

// 将GVector数组中的内容写入缓存中
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
/*      gDrawText() 用于画任意大小尺寸的文字

text           -- 待绘制的文字
font           -- 绘制文字所使用的字体，忽略font设置的字号
textCenter     -- 文字中心点的位置(单位像素)
width,height   -- 文字的宽度及高度，若其中任意一个被设置为-1，则该值
	              由另一个结合aspectRatio计算得出 
aspectRatio    -- 宽高比
rotationAngle  -- 文字相对于旋转点ptOfRot的旋转角                       
ptOfRot        -- 文字相对于改点进行旋转. 0代表中心点, 
                  1代表左下角点   
				                                                        */
/************************************************************************/
void CORE_EXPORT gDrawText( QPainter * p, const QString& text, const QFont& font, const QPoint& textCenter, int width, int height, double aspectRatio = 1.0, double rotationAngle = 0.0, int ptOfRot = 1);

QRect CORE_EXPORT gGetTextRect(const QString& text, const QFont& font, const QPoint& textCenter, int width, int height, double aspectRatio = 1.0);

QPolygon CORE_EXPORT gGetTextRegion(const QString& text, const QFont& font, const QPoint& textCenter, int width, int height, double aspectRatio = 1.0, double rotationAngle = 0.0, int ptOfRot = 1);

/*
 *	根据文字中心点获取文字的左下角坐标

	text             -- 文字内容
	font             -- 字体
	centerX, centerY -- 文字中心点坐标
	height           -- 字体高度
 */
QPointF CORE_EXPORT gGetLowerLeftPos(const QString& text, const QFont& font, double centerX, double centerY, double height);

#endif
