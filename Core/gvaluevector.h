/****************************************************************************
** GValueVector.h   1.0   edited Aug 19 06:00 $
**
** Definition of GValueVector based template class
**
** Created : def
**
** Copyright (C) 1992-2000 Trolltech AS.  All rights reserved.
**
**********************************************************************/

#ifndef GVALUEVECTOR_H
#define GVALUEVECTOR_H
#include <stdio.h>

template<class type>
class GValueVector	//�����б�
{
private:
	type *m_pdata;
	int m_size;
	int m_count;
private:
	void sift( int k, int m, int (* compare)(type *d1, type *d2) );	//������ɸ�Ӻ���
public:
	GValueVector<type>();							//����ձ�
	GValueVector<type>( int size );		//Ԥ�ȷ���ռ�size
	~GValueVector<type>();

	inline int size() const { return m_size; }
	inline int count() const { return m_count; }
	inline bool isEmpty() const { return m_count == 0; }
	inline bool isNull() const { return m_pdata == NULL; }
	inline type * getData() { return m_pdata; }
	inline const type * getData() const { return m_pdata; }

	bool prepend( type data );						//ǰ��
	bool append( type data );							//���
	bool insert( int index, type data );	//����
	int insertByOrder( type data, int (* compare)(const type &d1, const type &d2) ); //���벢����

	bool removeFirst();										//ɾ����һԪ��
	bool removeLast();										//ɾ��βԪ��
	bool remove( int index );							//ɾ��index�ϵ�Ԫ��
	void clear();													//���

	inline type & at( int index, bool *ok = NULL );	//����index�ϵ�Ԫ��
	inline const type & at(int index, bool *ok=NULL) const;
	inline bool set( int index, type data );			//����index�ϵ�Ԫ
	inline type & operator []( int index ) { return m_pdata[index]; }	
	inline const type & operator [](int index) const { return m_pdata[index]; }

	void resize( int size );											//���·���ռ�
	void truncate() { resize(count()); }

	int find( type d, int index = 0 ) const;						//���Բ���
	int find( type d, int (* compare)(const type &d1, const type &d2), int index = 0 ) const;

	int bsearch( type d, int (* compare)(const type &d1, const type &d2) ) const;	//���ֲ���

	void heapsort( int (* compare)(type *d1, type *d2) );										//������
};

template<class type>
GValueVector<type>::GValueVector()
{
	m_pdata = NULL;
	m_size = 0;
	m_count = 0;
}

template<class type>
GValueVector<type>::GValueVector( int size )
{
	if( size > 0 ) m_pdata = new type [size];
	else m_pdata = NULL;
	m_size = size;
	m_count = 0;
}

template<class type>
GValueVector<type>::~GValueVector()
{
	if( m_pdata != NULL ) 
	{
		delete []m_pdata;
	}
}

template<class type>
void GValueVector<type>::clear()
{
	m_count = 0;
}

template<class type>
void GValueVector<type>::resize( int size )
{
	int i;
	type *pdata;
	if( size > m_size )			//���ſռ�
	{
		pdata = new type [size];
		for( i=0; i<m_count; i++ ) pdata[i] = m_pdata[i];
		delete []m_pdata;
		m_pdata = pdata;
	}
	else if( size < m_size )	//��С�ռ�
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
			pdata = new type [size];
			for( i=0; i<m_count&&i<size; i++ ) pdata[i] = m_pdata[i];
			m_count = i;
			delete []m_pdata;
			m_pdata = pdata;
		}
	}
	m_size = size;
}

template<class type>
bool GValueVector<type>::append( type data )
{
	if( m_count == m_size )		//��Ҫ���ſռ�
	{
		type *pdata;
		int size = m_size+m_size/2;
		size = (size/8+1)*8;	//����8�߾�����ڴ���Ƭ
		pdata = new type [size];
		if( pdata == NULL ) return false;
		m_size = size;
		for( int i=0; i<m_count; i++ ) pdata[i] = m_pdata[i];
		if( m_pdata != NULL ) delete []m_pdata;
		m_pdata = pdata;
	}
	m_pdata[m_count++] = data;
	return true;
}

template<class type>
bool GValueVector<type>::prepend( type data )
{
	int i;
	if( m_count == m_size )		//��Ҫ���ſռ�
	{
		type *pdata;
		int size = m_size+m_size/2;
		size = (size/8+1)*8;	//����8�߾�����ڴ���Ƭ
		pdata = new type [size];
		if( pdata == NULL ) return false;
		m_size = size;
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
bool GValueVector<type>::insert( int index, type data )
{
	if( index > m_count ) return false;

	int i;
	if( m_count == m_size )		//��Ҫ���ſռ�
	{
		type *pdata;
		int size = m_size+m_size/2;
		size = (size/8+1)*8;	//����8�߾�����ڴ���Ƭ
		pdata = new type [size];
		if( pdata == NULL ) return false;
		m_size = size;
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
int GValueVector<type>::insertByOrder( type data, int (* compare)(const type &d1, const type &d2) )
{
	int i, index;
	if( m_count == m_size )		//��Ҫ���ſռ�
	{
		type *pdata;
		m_size += m_size/2;
		m_size = (m_size/8+1)*8;	//����8�߾�����ڴ���Ƭ
		pdata = new type [m_size];
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
bool GValueVector<type>::removeFirst()
{
	if( m_count == 0 ) return false;

	for( int i=1; i<m_count; i++ ) m_pdata[i-1] = m_pdata[i];	
	m_count--;
	return true;
}

template<class type>
bool GValueVector<type>::removeLast()
{
	if( m_count == 0 ) return false;

	m_count--;
	return true;
}

template<class type>
bool GValueVector<type>::remove( int index )
{
	if( index >= m_count ) return false;

	m_count--;
	for( int i=index; i<m_count; i++ ) m_pdata[i] = m_pdata[i+1];
	return true;
}

template<class type>
type & GValueVector<type>::at( int index, bool *ok )
{
	if( ok != NULL ) 
	{
		if( 0 <= index && index < m_count ) *ok = true;
		else {
			*ok = false;
			index = 0;
		}
	}
	return m_pdata[index];
}

template<class type>
const type & GValueVector<type>::at( int index, bool *ok ) const
{
	if( ok != NULL ) 
	{
		if( 0 <= index && index < m_count ) *ok = true;
		else {
			*ok = false;
			index = 0;
		}
	}
	return m_pdata[index];
}

template<class type>
bool GValueVector<type>::set( int index, type data )
{
	if( index >= m_count ) return false;
	m_pdata[index] = data;
	return true;
}

template<class type>
int GValueVector<type>::find( type d, int index ) const
{
	for( int i=index; i<m_count; i++ )
	{
		if( m_pdata[i] == d ) return i;
	}
	return -1;
}

template<class type>
int GValueVector<type>::find( type d, int (* compare)(const type &d1, const type &d2), int index ) const
{
	for( int i=index; i<m_count; i++ )
	{
		if( compare(m_pdata[i], d) == 0 ) return i;
	}
	return -1;
}

template<class type>
int GValueVector<type>::bsearch( type d, int (* compare)(const type &d1, const type &d2) ) const
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
void GValueVector<type>::sift( int k, int m, int (* compare)(type *d1, type *d2) )
{
    int i,j;
    type rp;

    i = k;	j = 2*i+1;	rp = m_pdata[k];
    while( j <= m ) 
	{
	    if( j < m && compare(&m_pdata[j], &m_pdata[j+1]) < 0 ) j++;
	    if( compare(&m_pdata[j], &rp) < 0 ) break;
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
void GValueVector<type>::heapsort( int (* compare)(type *d1, type *d2) )
{
	int i;
	type rp;

    for( i=m_count/2-1; i>=0; i--) sift( i, m_count-1, compare );
    for( i=m_count-1; i>=1; i--)
	{
	    rp = m_pdata[0];
	    m_pdata[0] = m_pdata[i];
	    m_pdata[i] = rp;
	    sift( 0, i-1, compare );
	}
}

#endif
