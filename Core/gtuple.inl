template<typename TYPE>
int gTupleCompare(const TYPE *s1, const TYPE *s2, int n)
{
	const TYPE *p1 = s1, *p2 = s2;
	while (n--)
	{
		if (*p1 < *p2) return -1;
		else if(*p1 > *p2) return 1;
		p1++;
		p2++;
	}
	return 0;
}

//----------------------------------------------------------------------------
template <int DIMENSION, typename TYPE>
GTuple<DIMENSION,TYPE>::GTuple ()
{
	for (int i=0; i<DIMENSION; ++i)
	{
		mTuple[i] = 0;
	}
}
//----------------------------------------------------------------------------
template <int DIMENSION, typename TYPE>
GTuple<DIMENSION,TYPE>::GTuple (const GTuple& tuple)
{
	for (int i = 0; i < DIMENSION; ++i)
	{
		mTuple[i] = tuple.mTuple[i];
	}
}
//----------------------------------------------------------------------------
template <int DIMENSION, typename TYPE>
GTuple<DIMENSION,TYPE>::GTuple (const TYPE *tuple)
{
	for (int i = 0; i < DIMENSION; ++i)
	{
		mTuple[i] = tuple[i];
	}
}
//----------------------------------------------------------------------------
template <int DIMENSION, typename TYPE>
GTuple<DIMENSION,TYPE>::~GTuple ()
{
}
//----------------------------------------------------------------------------
template <int DIMENSION, typename TYPE>
inline GTuple<DIMENSION,TYPE>::operator const TYPE* () const
{
	return mTuple;
}
//----------------------------------------------------------------------------
template <int DIMENSION, typename TYPE>
inline GTuple<DIMENSION,TYPE>::operator TYPE* ()
{
	return mTuple;
}
//----------------------------------------------------------------------------
template <int DIMENSION, typename TYPE>
inline const TYPE& GTuple<DIMENSION,TYPE>::operator[] (int i) const
{
	return mTuple[i];
}
//----------------------------------------------------------------------------
template <int DIMENSION, typename TYPE>
inline TYPE& GTuple<DIMENSION,TYPE>::operator[] (int i)
{
	return mTuple[i];
}
//----------------------------------------------------------------------------
template <int DIMENSION, typename TYPE>
GTuple<DIMENSION,TYPE>& GTuple<DIMENSION,TYPE>::operator= (const GTuple& tuple)
{
	for (int i = 0; i < DIMENSION; ++i)
	{
		mTuple[i] = tuple.mTuple[i];
	}
	return *this;
}
//----------------------------------------------------------------------------
template <int DIMENSION, typename TYPE>
void GTuple<DIMENSION,TYPE>::set(const TYPE *tuple)
{
	for (int i = 0; i < DIMENSION; ++i)
	{
		mTuple[i] = tuple[i];
	}
}
//----------------------------------------------------------------------------
template <int DIMENSION, typename TYPE>
void GTuple<DIMENSION,TYPE>::set(const GTuple& tuple)
{
	for (int i = 0; i < DIMENSION; ++i)
	{
		mTuple[i] = tuple.mTuple[i];
	}
}
//----------------------------------------------------------------------------
template <int DIMENSION, typename TYPE>
bool GTuple<DIMENSION,TYPE>::operator== (const GTuple& tuple) const
{
	return gTupleCompare(this->mTuple, tuple.mTuple, DIMENSION) == 0;
}
//----------------------------------------------------------------------------
template <int DIMENSION, typename TYPE>
bool GTuple<DIMENSION,TYPE>::operator!= (const GTuple& tuple) const
{
	return gTupleCompare(this->mTuple, tuple.mTuple, DIMENSION) != 0;
}
//----------------------------------------------------------------------------
template <int DIMENSION, typename TYPE>
bool GTuple<DIMENSION,TYPE>::operator< (const GTuple& tuple) const
{
	return gTupleCompare(this->mTuple, tuple.mTuple, DIMENSION) < 0;
}
//----------------------------------------------------------------------------
template <int DIMENSION, typename TYPE>
bool GTuple<DIMENSION,TYPE>::operator<= (const GTuple& tuple) const
{
	return gTupleCompare(this->mTuple, tuple.mTuple, DIMENSION) <= 0;
}
//----------------------------------------------------------------------------
template <int DIMENSION, typename TYPE>
bool GTuple<DIMENSION,TYPE>::operator> (const GTuple& tuple) const
{
	return gTupleCompare(this->mTuple, tuple.mTuple, DIMENSION) > 0;
}
//----------------------------------------------------------------------------
template <int DIMENSION, typename TYPE>
bool GTuple<DIMENSION,TYPE>::operator>= (const GTuple& tuple) const
{
	return gTupleCompare(this->mTuple, tuple.mTuple, DIMENSION) >= 0;
}
//----------------------------------------------------------------------------
static unsigned int hash(const unsigned char *p, int n)
{
	unsigned int h = 0;

	while (n--) 
	{
		h = (h << 4) + *p++;
		h ^= (h & 0xf0000000) >> 23;
		h &= 0x0fffffff;
	}
	return h;
}

template <int DIMENSION, typename TYPE>
unsigned int qHash(const GTuple<DIMENSION, TYPE> &tuple)
{
	return hash( (const unsigned char *)&tuple, sizeof(tuple) );
}
