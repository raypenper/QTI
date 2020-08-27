/****************************************************************************
** GPtrVector.h   1.0   edited Aug 19 06:00 $
**
** Definition of GPtrVector pointer based template class
**
** Created : def
**
** Copyright (C) 1992-2000 Trolltech AS.  All rights reserved.
**
**********************************************************************/

#ifndef GPTRVECTOR_H
#define GPTRVECTOR_H

#include <stdio.h>

template<class type>
class GPtrVector	//指针线性列表
{
private:
	type **m_pdata;
	int m_size;
	int m_count;
	bool m_autoDelete;			//自动删除指针内存标志
private:
	void sift( int k, int m, int (* compare)(type *d1, type *d2) );	//堆排序筛子函数
public:
	GPtrVector<type>();							//构造空表
	GPtrVector<type>( int size );	//预先分配空间size
	~GPtrVector<type>();

	inline int size() const { return m_size; }
	inline int count() const { return m_count; }
	inline bool isEmpty() const { return m_count == 0; }
	inline bool isNull() const { return m_pdata == NULL; }
	inline type ** getData() { return m_pdata; }
	inline const type ** getData() const { return (const type **)m_pdata; }

	inline void setAutoDelete( bool enable ) { m_autoDelete = enable; }
	inline bool autoDelete() { return m_autoDelete; }

	bool prepend( type *data );	//前插
	bool append( type *data );	//后插
	bool insert( int index, type *data );	//插入
	int insertByOrder( type *data, int (* compare)(type *d1, type *d2) ); //插入并有序

	bool removeFirst();					//删除第一元素
	bool removeLast();					//删除尾元素
	bool remove( int index );		//删除index上的元素
	void clear();								//清空

	inline type * at( int index ) const;					//返回index上的元素
	inline bool set( int index, type *data );			//设置index上的元素
	inline type * operator []( int index ) const { return at( index ); }	

	void resize( int size );											//重新分配空间
	void trunclate() { resize(count()); }

	int find( type *d, int index = 0 ) const;						//线性查找
	int find( type *d, int (* compare)(type *d1, type *d2), int index = 0 ) const;

	int bsearch( type *d, int (* compare)(type *d1, type *d2) ) const;	//二分查找

	void heapsort( int (* compare)(type *d1, type *d2) );					//堆排序

	void sort( int (* compare)(type *d1, type *d2) );
};

template<class type>
GPtrVector<type>::GPtrVector()
{
	m_pdata = NULL;
	m_size = 0;
	m_count = 0;
	m_autoDelete = false;
}

template<class type>
GPtrVector<type>::GPtrVector( int size )
{
	if( size > 0 ) m_pdata = new type *[size];
	else m_pdata = NULL;
	m_size = size;
	m_count = 0;
	m_autoDelete = false;
}

template<class type>
GPtrVector<type>::~GPtrVector()
{
	if( m_pdata != NULL ) 
	{
		clear();
		delete []m_pdata;
	}
}

template<class type>
void GPtrVector<type>::clear()
{
	if( m_autoDelete )
	{
		for( int i=0; i<m_count; i++ )
		{
			delete m_pdata[i];
		}
	}
	m_count = 0;
}

template<class type>
void GPtrVector<type>::resize( int size )
{
	int i;
	type **pdata;
	if( size > m_size )			//扩张空间
	{
		pdata = new type *[size];
		for( i=0; i<m_count; i++ ) pdata[i] = m_pdata[i];
		delete []m_pdata;
		m_pdata = pdata;
	}
	else if( size < m_size )	//缩小空间
	{
		if( size == 0 ) 
		{
			clear();
			delete []m_pdata;
			m_pdata = NULL;
			m_count = 0;
		}
		else
		{
			pdata = new type *[size];
			for( i=0; i<m_count&&i<size; i++ ) pdata[i] = m_pdata[i];
			m_count = i;
			if( m_autoDelete )
			{
				for( ;i<m_count; i++ ) delete m_pdata[i];
			}
			delete []m_pdata;
			m_pdata = pdata;
		}
	}
	m_size = size;
}

template<class type>
bool GPtrVector<type>::append( type *data )
{
	if( m_count == m_size )		//需要扩张空间
	{
		type **pdata;
		m_size += m_size/2;
		m_size = (m_size/8+1)*8;	//对齐8边距减少内存碎片
		pdata = new type *[m_size];
		for( int i=0; i<m_count; i++ ) pdata[i] = m_pdata[i];
		if( m_pdata != NULL ) delete []m_pdata;
		m_pdata = pdata;
	}
	m_pdata[m_count++] = data;
	return true;
}

template<class type>
bool GPtrVector<type>::prepend( type *data )
{
	int i;
	if( m_count == m_size )		//需要扩张空间
	{
		type **pdata;
		m_size += m_size/2;
		m_size = (m_size/8+1)*8;	//对齐8边距减少内存碎片
		pdata = new type *[m_size];
		for( i=0; i<m_count; i++ ) pdata[i+1] = m_pdata[i];
		if( m_pdata != NULL ) delete []m_pdata;
		m_pdata = pdata;
	}
	else 
	{
		for( i=m_count; i>0; i-- ) m_pdata[i] = m_pdata[i-1];
	}
	m_pdata[0] = data;
	m_count++;
	return true;
}

template<class type>
bool GPtrVector<type>::insert( int index, type *data )
{
	if( index > m_count ) return false;

	int i;
	if( m_count == m_size )		//需要扩张空间
	{
		type **pdata;
		m_size += m_size/2;
		m_size = (m_size/8+1)*8;	//对齐8边距减少内存碎片
		pdata = new type *[m_size];
		for( i=0; i<index; i++ ) pdata[i] = m_pdata[i];
		for( i=index; i<m_count; i++ ) pdata[i+1] = m_pdata[i];
		if( m_pdata != NULL ) delete []m_pdata;
		m_pdata = pdata;
	}
	else
	{
		for( i=m_count; i>index; i-- ) m_pdata[i] = m_pdata[i-1];
	}
	m_pdata[index] = data;
	m_count++;
	return true;
}

template<class type>
int GPtrVector<type>::insertByOrder( type *data, int (* compare)(type *d1, type *d2) )
{
	int i, index;
	if( m_count == m_size )		//需要扩张空间
	{
		type **pdata;
		m_size += m_size/2;
		m_size = (m_size/8+1)*8;	//对齐8边距减少内存碎片
		pdata = new type *[m_size];
		for( i=m_count-1; i>=0; i-- )
		{
			if( compare( data, m_pdata[i] ) >= 0 ) break;
			pdata[i+1] = m_pdata[i];
		}
		index = i+1;
		pdata[index] = data;
		for( ; i>=0; i-- ) pdata[i] = m_pdata[i];
		if( m_pdata != NULL ) delete []m_pdata;
		m_pdata = pdata;
	}
	else
	{
		for( i=m_count-1; i>=0; i-- )
		{
			if( compare( data, m_pdata[i] ) >= 0 ) break;
			m_pdata[i+1] = m_pdata[i];
		}
		index = i+1;
		m_pdata[index] = data;
	}
	m_count++;
	return index;
}

template<class type>
bool GPtrVector<type>::removeFirst()
{
	if( m_count == 0 ) return false;

	if( m_autoDelete ) delete m_pdata[0];
	for( int i=1; i<m_count; i++ ) m_pdata[i-1] = m_pdata[i];	
	m_count--;
	return true;
}

template<class type>
bool GPtrVector<type>::removeLast()
{
	if( m_count == 0 ) return false;

	m_count--;
	if( m_autoDelete ) delete m_pdata[m_count];
	return true;
}

template<class type>
bool GPtrVector<type>::remove( int index )
{
	if( index >= m_count ) return false;

	if( m_autoDelete ) delete m_pdata[index];
	m_count--;
	for( int i=index; i<m_count; i++ ) m_pdata[i] = m_pdata[i+1];
	return true;
}

template<class type>
type * GPtrVector<type>::at( int index ) const
{
	if(index < 0 || index >= m_count ) return NULL;
	else return m_pdata[index];
}

template<class type>
bool GPtrVector<type>::set( int index, type *data )
{
	if( index >= m_count ) return false;
	m_pdata[index] = data;
	return true;
}

template<class type>
int GPtrVector<type>::find( type *d, int index ) const
{
	for( int i=index; i<m_count; i++ )
	{
		if( m_pdata[i] == d ) return i;
	}
	return -1;
}

template<class type>
int GPtrVector<type>::find( type *d, int (* compare)(type *d1, type *d2), int index ) const
{
	for( int i=index; i<m_count; i++ )
	{
		if( compare(m_pdata[i], d) == 0 ) return i;
	}
	return -1;
}

template<class type>
int GPtrVector<type>::bsearch( type *d, int (* compare)(type *d1, type *d2) ) const
{
	int i, first, last;

	first = 0;	
	last = m_count-1;
	while( first <= last )
	{
		i = (first+last)/2;
		int r = compare(m_pdata[i], d);
		if( r > 0 ) last = i-1;
		else if( r == 0 ) return i;
		else first = i+1;
	}
	
	return -1;
}

template<class type>
void GPtrVector<type>::sift( int k, int m, int (* compare)(type *d1, type *d2) )
{
    int i,j;
    type *rp;

    i = k;	j = 2*i+1;	rp = m_pdata[k];
    while( j <= m ) 
	{
	    if( j < m && compare(m_pdata[j], m_pdata[j+1]) < 0 ) j++;
	    if( compare(m_pdata[j], rp) < 0 ) break;
	    else 
		{ 
			m_pdata[i] = m_pdata[j]; 
			i = j; 
			j = 2*i+1; 
		}
	}
    m_pdata[i] = rp;
}	

template<class type>
void GPtrVector<type>::heapsort( int (* compare)(type *d1, type *d2) )
{
	int i;
	type *rp;

    for( i=m_count/2-1; i>=0; i--) sift( i, m_count-1, compare );
    for( i=m_count-1; i>=1; i--)
	{
	    rp = m_pdata[0];
	    m_pdata[0] = m_pdata[i];
	    m_pdata[i] = rp;
	    sift( 0, i-1, compare );
	}
}

template<class type>
void GPtrVector<type>::sort(int (* compare)(type *d1, type *d2))
{
	for(int i=0; i<(int)m_count-1; i++)
	{
		for(int j=i+1; j<(int)m_count; j++)
		{
			if(compare(m_pdata[i], m_pdata[j]) > 0)
			{
				type *rp = m_pdata[i];
				m_pdata[i] = m_pdata[j];
				m_pdata[j] = rp;
			}
		}
	}
} 

#endif