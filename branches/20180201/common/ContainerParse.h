#pragma once

///< 容器数据相关解析
#include <vector>
#include <set>
#include <map>
#include <string>
#include "StringTool.h"
#include "MyTuple.h"



namespace Dawn
{

template< typename T, typename Container >
void parseContainer( Container& container, const char* str, const char* conn = "-" )
{
	if( NULL == str )
		return;
	std::vector< std::string > v;
	StringTool::stringtok( v, str, conn );

	std::vector< std::string >::const_iterator it;
	for( it = v.begin(); it != v.end(); ++it )
	{
		T value;
		parse_proper_type( value, *it );
		container.insert( container.end(), value );
	}
}	

///< 双重容器
template< typename T, typename SubContainer, typename Container >
void parseDoubleContainer( Container& container, const char* str, const char* conn = "-;" )
{
	if( NULL == conn || NULL == str )
		return;	
	assert( strlen( conn ) == 2 );
	std::vector< std::string > vt;
	char conn_t[2] = {'\0'};
	conn_t[0] = conn[1];
	StringTool::stringtok( vt, str, conn_t );

	std::vector< std::string >::const_iterator it;
	for( it = vt.begin(); it != vt.end(); ++it )
	{
		const std::string t = *it;
		if( t.empty() )
			continue;

		std::vector< std::string > v;
		conn_t[0] = conn[0];
		StringTool::stringtok( v, t, conn_t );

		SubContainer sc;
		std::vector< std::string >::const_iterator sit;
		for( sit = v.begin(); sit != v.end(); ++sit )
		{
			T value;
			parse_proper_type( value, *sit );
			sc.insert( sc.end(), value );
		}
		
		container.insert( container.end(), sc );
	}
}	

template< typename T1, typename T2 >
void parseMapContainer( std::map< T1, T2 >& container, const char* str, const char* conn = "-;" )
{
	if( NULL == conn || NULL == str )
		return;	
	assert( strlen( conn ) == 2 );
	std::vector< std::string > vt;
	char conn_t[2] = {'\0'};
	conn_t[0] = conn[1];
	StringTool::stringtok( vt, str, conn_t );

	std::vector< std::string >::const_iterator it;
	for( it = vt.begin(); it != vt.end(); ++it )
	{
		const std::string t = *it;
		if( t.empty() )
			continue;

		std::vector< std::string > v;
		conn_t[0] = conn[0];
		StringTool::stringtok( v, t, conn_t );
		//assert( v.size() == 2 );
		if (v.size() != 2)
		{
			continue;
		}

		T1 value1;
		T2 value2;

		parse_proper_type( value1, v[0] );
		parse_proper_type( value2, v[1] );

		container.insert( std::make_pair( value1, value2 ) );
	}
}

template< typename T1, typename T2 >
void parseSpecilMapContainer( std::map< T1, std::list< std::pair< T2, T2 > > >& container, const char* str, const char* conn = "|-;$" )
{
	if( NULL == conn || NULL == str )
		return;	
	assert( strlen( conn ) == 4 );
	std::vector< std::string > vt;
	char conn_t[2] = {'\0'};
	conn_t[0] = conn[3];
	StringTool::stringtok( vt, str, conn_t );
	
	std::vector< std::string >::const_iterator it;
	for( it = vt.begin(); it != vt.end(); ++it )
	{
		const std::string t = *it;
		if( t.empty() )
			continue;
		
		std::vector< std::string > v;
		conn_t[0] = conn[0];
		StringTool::stringtok( v, t, conn_t );
		assert( v.size() == 2 );
		// 解析索引
		T1 value1;
		std::list< std::pair< T2, T2 > > l;
		parse_proper_type( value1, v[0] );
		//T2 value2;
		{
			
			conn_t[0] = conn[2];
			std::vector< std::string > v_1;
			StringTool::stringtok( v_1, v[1], conn_t );
			std::vector< std::string >::const_iterator it_1;
			for( it_1 = v_1.begin(); it_1 != v_1.end(); ++it_1 )
			{
				const std::string t_1 = *it_1;
				if( t_1.empty() )
					continue;
				std::vector< std::string > v_2;
				conn_t[0] = conn[1];
				StringTool::stringtok( v_2, t_1, conn_t );
				assert( v_2.size() == 2 );
				T2 value_1;
				T2 value_2;
				parse_proper_type( value_1, v_2[0] );
				parse_proper_type( value_2, v_2[1] );

				std::pair< T2, T2 > add;
				add.first = value_1;
				add.second = value_2;
				l.push_back( add );
			}
		}
		container.insert( std::make_pair( value1, l ) );
	}
}

template< typename T1, typename T2 >
void parseSpecilMapContainer( std::map< T1, std::vector< std::pair< T2, T2 > > >& container, const char* str, const char* conn = "|-;$" )
{
	if( NULL == conn || NULL == str )
		return;	
	assert( strlen( conn ) == 4 );
	std::vector< std::string > vt;
	char conn_t[2] = {'\0'};
	conn_t[0] = conn[3];
	StringTool::stringtok( vt, str, conn_t );

	std::vector< std::string >::const_iterator it;
	for( it = vt.begin(); it != vt.end(); ++it )
	{
		const std::string t = *it;
		if( t.empty() )
			continue;

		std::vector< std::string > v;
		conn_t[0] = conn[0];
		StringTool::stringtok( v, t, conn_t );
		assert( v.size() == 2 );
		// 解析索引
		T1 value1;
		std::vector< std::pair< T2, T2 > > l;
		parse_proper_type( value1, v[0] );
		//T2 value2;
		{

			conn_t[0] = conn[2];
			std::vector< std::string > v_1;
			StringTool::stringtok( v_1, v[1], conn_t );
			std::vector< std::string >::const_iterator it_1;
			for( it_1 = v_1.begin(); it_1 != v_1.end(); ++it_1 )
			{
				const std::string t_1 = *it_1;
				if( t_1.empty() )
					continue;
				std::vector< std::string > v_2;
				conn_t[0] = conn[1];
				StringTool::stringtok( v_2, t_1, conn_t );
				assert( v_2.size() == 2 );
				T2 value_1;
				T2 value_2;
				parse_proper_type( value_1, v_2[0] );
				parse_proper_type( value_2, v_2[1] );

				std::pair< T2, T2 > add;
				add.first = value_1;
				add.second = value_2;
				l.push_back( add );
			}
		}
		container.insert( std::make_pair( value1, l ) );
	}
}

template< typename T1, typename T2 >
void parseSpecilMapTupleContainer( std::map< T1, std::vector< T2 > >& container, const char* str, const char* conn = "|-;$" )
{
	if( NULL == conn || NULL == str )
		return;	
	assert( strlen( conn ) == 4 );
	std::vector< std::string > vt;
	char conn_t[2] = {'\0'};
	conn_t[0] = conn[3];
	StringTool::stringtok( vt, str, conn_t );

	std::vector< std::string >::const_iterator it;
	for( it = vt.begin(); it != vt.end(); ++it )
	{
		const std::string t = *it;
		if( t.empty() )
			continue;

		std::vector< std::string > v;
		conn_t[0] = conn[0];
		StringTool::stringtok( v, t, conn_t );
		assert( v.size() == 2 );
		// 解析索引
		T1 value1;
		std::vector< T2 > l;
		parse_proper_type( value1, v[0] );

		//T2 value2;
		{

			conn_t[0] = conn[2];
			std::vector< std::string > v_1;
			StringTool::stringtok( v_1, v[1], conn_t );
			std::vector< std::string >::const_iterator it_1;
			for( it_1 = v_1.begin(); it_1 != v_1.end(); ++it_1 )
			{
				const std::string t_1 = *it_1;
				if( t_1.empty() )
					continue;

				conn_t[0] = conn[1];
				T2 tuple;
				parseTuple( tuple, t_1.c_str(), conn_t );
				l.push_back( tuple );
			}
		}

		container.insert( std::make_pair( value1, l ) );
	}
}


template< typename T >
void parseSpecilVectorContainer( std::vector< std::map< T, T > >& container, const char* str, const char* conn = "-;$" )
{
	if( NULL == conn || NULL == str )
		return;	
	assert( strlen( conn ) == 3 );
	std::vector< std::string > vt;
	char conn_t[2] = {'\0'};
	conn_t[0] = conn[2];
	StringTool::stringtok( vt, str, conn_t );
	
	std::vector< std::string >::const_iterator it;
	for( it = vt.begin(); it != vt.end(); ++it )
	{
		const std::string t = *it;
		if( t.empty() )
			continue;

		std::map< T, T > l;
		{
			conn_t[0] = conn[1];
			std::vector< std::string > v_1;
			StringTool::stringtok( v_1, t, conn_t );
			std::vector< std::string >::const_iterator it_1;
			for( it_1 = v_1.begin(); it_1 != v_1.end(); ++it_1 )
			{
				const std::string t_1 = *it_1;
				if( t_1.empty() )
					continue;
				std::vector< std::string > v_2;
				conn_t[0] = conn[0];
				StringTool::stringtok( v_2, t_1, conn_t );
				assert( v_2.size() == 2 );
				T value_1;
				T value_2;
				parse_proper_type( value_1, v_2[0] );
				parse_proper_type( value_2, v_2[1] );

				std::pair< T, T > add;
				add.first = value_1;
				add.second = value_2;
				l[value_1] = value_2;
			}
		}
		container.push_back( l );
	}
}

template< typename T >
void parseArrayContainer( T* container, const int num, const char* str, const char* conn = "-;" )
{
	if( NULL == conn || NULL == str )
		return;	
	assert( strlen( conn ) == 2 );

	std::vector< std::string > vt;
	char conn_t[2] = {'\0'};
	conn_t[0] = conn[1];
	StringTool::stringtok( vt, str, conn_t );

	std::vector< std::string >::const_iterator it;
	for( it = vt.begin(); it != vt.end(); ++it )
	{
		const std::string t = *it;
		if( t.empty() )
			continue;

		std::vector< std::string > v;
		conn_t[0] = conn[0];
		StringTool::stringtok( v, t, conn_t );
		assert( v.size() == 2 );

		int index;
		T value;

		parse_proper_type( index, v[0] );
		parse_proper_type( value, v[1] );

		assert( index >= 0 && index < num );

		container[index] = value;
	}
}

};