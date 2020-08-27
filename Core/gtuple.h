#ifndef GTUPLE_H
#define GTUPLE_H

template <int DIMENSION, typename TYPE>
class GTuple
{
public:
	// Construction and destruction.  The default constructor does not
	// initialize the tuple elements for native elements.  The tuple elements
	// are initialized for class data whenever TYPE initializes during its
	// default construction.
	GTuple ();
	GTuple (const GTuple& tuple);
	GTuple (const TYPE *tuple);
	~GTuple ();

	// Coordinate access.
	inline operator const TYPE* () const;
	inline operator TYPE* ();
	inline const TYPE& operator[] (int i) const;
	inline TYPE& operator[] (int i);

	// Assignment.
	GTuple& operator= (const GTuple& tuple);

	void set(const TYPE *tuple);

	void set(const GTuple& tuple);

	// Comparison.  The inequalities make the comparisons using memcmp, thus
	// treating the tuple as an array of unsigned bytes.
	bool operator== (const GTuple& tuple) const;
	bool operator!= (const GTuple& tuple) const;
	bool operator<  (const GTuple& tuple) const;
	bool operator<= (const GTuple& tuple) const;
	bool operator>  (const GTuple& tuple) const;
	bool operator>= (const GTuple& tuple) const;

public:
	TYPE mTuple[DIMENSION];
};

typedef GTuple<2, int>		GTuple2i;
typedef GTuple<2, float>	GTuple2f;
typedef GTuple<2, double>	GTuple2d;

typedef GTuple<3, int>		GTuple3i;
typedef GTuple<3, float>	GTuple3f;
typedef GTuple<3, double>	GTuple3d;

template <int DIMENSION, typename TYPE>
unsigned int qHash(const GTuple<DIMENSION, TYPE> &tuple);

#include "gtuple.inl"

#endif