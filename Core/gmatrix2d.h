#ifndef GMATRIX2D_H
#define GMATRIX2D_H

#include "core_global.h"

class GVector2d;

class CORE_EXPORT GMatrix2d
{
protected:
	//-----------¾ØÕó´æ´¢Ä£Ê½---------------------------------------------------
	//  |	a0		a1		a2		|	
	//	|	a3		a4		a5		|
	//	|	a6		a7		a8		|
	//--------------------------------------------------------------------------
	double mMatrix[9];
public:
	GMatrix2d();
	GMatrix2d(const double *mMatrix);
	GMatrix2d(const GMatrix2d &m);
	
	operator const double * () const
	{ return mMatrix; }

	operator double * ()
	{ return mMatrix; }

	GMatrix2d& operator = (const GMatrix2d &m);
	GMatrix2d operator * (const GMatrix2d &m) const;
	GMatrix2d& operator *= (const GMatrix2d &m);

	GMatrix2d getTranspose() const;
	void transpose();
	GMatrix2d getInverse() const;
	void inverse();

    static GMatrix2d createZero();
	static GMatrix2d createIdentity();
	static GMatrix2d createRotate(double af);
	static GMatrix2d createRotate(double sinAF, double cosAF);
	static GMatrix2d createTranslate(double x, double y);
	static GMatrix2d createScale(double sx, double sy);
	//static GMatrix2d createMirror(const GVector2d &v);
};

#endif
