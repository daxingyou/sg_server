#pragma once

///< 小结构元祖数据解析

#include <assert.h>
#include <string>
#include <map>
#include <vector>
#include "StringTool.h"
#include "VarType.h"

namespace Dawn
{

///<===========================三元组=================================
template< typename T1, typename T2, typename T3 >
struct triple
{
	typedef T1 first_type;
	typedef T2 second_type;
	typedef T3 third_type;

	T1 first;
	T2 second;
	T3 third;

	triple()
		: first(), second(), third(){ }

	triple( const T1& a, const T2& b, const T3& c )
		: first( a ), second( b ), third( c ){ }

	template< typename U1, typename U2, typename U3 >
	triple( const triple< U1, U2, U3 >& p )
		: first( p.first ), second( p.second ), third( p.third ){ }	

	void clear();
};

template< typename T1, typename T2, typename T3 >
void triple< T1, T2, T3 >::clear()
{
	first = T1();
	second = T2();
	third = T3();
}

template< typename T1, typename T2, typename T3 >
inline bool operator == ( const triple< T1, T2, T3 >& a, const triple< T1, T2, T3 >& b )
{
	return a.first == b.first
		&& a.second == b.second
		&& a.third == b.third;
}

template< typename T1, typename T2, typename T3 >
inline bool operator != ( const triple< T1, T2, T3 >& a, const triple< T1, T2, T3 >& b )
{
	return !( a == b );
}

template< typename T1, typename T2, typename T3 >
inline triple< T1, T2, T3 > make_triple ( const T1& a, const T2& b, const T3& c )
{
	return triple< T1, T2, T3 >( a, b, c );
}

///<===========================四元组=================================
template< typename T1, typename T2, typename T3, typename T4 >
struct quadruple
{
	typedef T1 first_type;
	typedef T2 second_type;
	typedef T3 third_type;
	typedef T4 fourth_type;

	T1 first;
	T2 second;
	T3 third;
	T4 fourth;

	quadruple()
		: first(), second(), third(), fourth(){ }

	quadruple( const T1& a, const T2& b, const T3& c, const T4& d )
		: first( a ), second( b ), third( c ), fourth( d ){ }

	template< typename U1, typename U2, typename U3, typename U4 >
	quadruple( const quadruple< U1, U2, U3, U4 >& p )
		: first( p.first ), second( p.second ), third( p.third ), fourth( p.fourth ){ }	

	void clear();
};

template< typename T1, typename T2, typename T3, typename T4 >
void quadruple< T1, T2, T3, T4 >::clear()
{
	first = T1();
	second = T2();
	third = T3();
	fourth = T4();
}

template< typename T1, typename T2, typename T3, typename T4 >
inline bool operator == ( const quadruple< T1, T2, T3, T4 >& a, const quadruple< T1, T2, T3, T4 >& b )
{
	return a.first == b.first
		&& a.second == b.second
		&& a.third == b.third
		&& a.fourth == b.fourth;
}

template< typename T1, typename T2, typename T3, typename T4 >
inline bool operator != ( const quadruple< T1, T2, T3, T4 >& a, const quadruple< T1, T2, T3, T4 >& b )
{
	return !( a == b );
}

template< typename T1, typename T2, typename T3, typename T4 >
inline quadruple< T1, T2, T3, T4 > make_quadruple( const T1& a, const T2& b, const T3& c, const T4& d )
{
	return quadruple< T1, T2, T3, T4 >( a, b, c, d );
}


///<===========================五元组=================================
template< typename T1, typename T2, typename T3, typename T4, typename T5 >
struct quintuple
{
	typedef T1 first_type;
	typedef T2 second_type;
	typedef T3 third_type;
	typedef T4 fourth_type;
	typedef T5 fifth_type;

	T1 first;
	T2 second;
	T3 third;
	T4 fourth;
	T5 fifth;

	quintuple()
		: first(), second(), third(), fourth(), fifth(){ }

	quintuple( const T1& a, const T2& b, const T3& c, const T4& d, const T5& e )
		: first( a ), second( b ), third( c ), fourth( d ), fifth( e ){ }

	template< typename U1, typename U2, typename U3, typename U4, typename U5 >
	quintuple( const quintuple< U1, U2, U3, U4, U5 >& p )
		: first( p.first ), second( p.second ), third( p.third ), fourth( p.fourth ), fifth( p.fifth ){ }	

	void clear();
};

template< typename T1, typename T2, typename T3, typename T4, typename T5 >
void quintuple< T1, T2, T3, T4, T5 >::clear()
{
	first = T1();
	second = T2();
	third = T3();
	fourth = T4();
	fifth = T5();
}

template< typename T1, typename T2, typename T3, typename T4, typename T5 >
inline bool operator == ( const quintuple< T1, T2, T3, T4, T5 >& a, const quintuple< T1, T2, T3, T4, T5 >& b )
{
	return a.first == b.first
		&& a.second == b.second
		&& a.third == b.third
		&& a.fourth == b.fourth
		&& a.fifth == b.fifth;
}

template< typename T1, typename T2, typename T3, typename T4, typename T5 >
inline bool operator != ( const quintuple< T1, T2, T3, T4, T5 >& a, const quintuple< T1, T2, T3, T4, T5 >& b )
{
	return !( a == b );
}

template< typename T1, typename T2, typename T3, typename T4, typename T5 >
inline quintuple< T1, T2, T3, T4, T5 > make_quintuple( const T1& a, const T2& b, const T3& c, const T4& d, const T5& e )
{
	return quintuple< T1, T2, T3, T4, T5 >( a, b, c, d, e );
}
//quintuple
//fifth

///<===========================六元组=================================
template< typename T1, typename T2, typename T3, typename T4, typename T5, typename T6 >
struct sextuple
{
	typedef T1 first_type;
	typedef T2 second_type;
	typedef T3 third_type;
	typedef T4 fourth_type;
	typedef T5 fifth_type;
	typedef T6 sixth_type;

	T1 first;
	T2 second;
	T3 third;
	T4 fourth;
	T5 fifth;
	T6 sixth;

	sextuple()
		: first(), second(), third(), fourth(), fifth(), sixth(){ }

	sextuple( const T1& a, const T2& b, const T3& c, const T4& d, const T5& e, const T6& f )
		: first( a ), second( b ), third( c ), fourth( d ), fifth( e ), sixth( f ){ }

	template< typename U1, typename U2, typename U3, typename U4, typename U5, typename U6 >
	sextuple( const sextuple< U1, U2, U3, U4, U5, U6 >& p )
		: first( p.first ), second( p.second ), third( p.third ), fourth( p.fourth ), fifth( p.fifth ), sixth( p.sixth ){ }	

	void clear();
};

template< typename T1, typename T2, typename T3, typename T4, typename T5, typename T6 >
void sextuple< T1, T2, T3, T4, T5, T6 >::clear()
{
	first = T1();
	second = T2();
	third = T3();
	fourth = T4();
	fifth = T5();
	sixth = T6();
}

template< typename T1, typename T2, typename T3, typename T4, typename T5, typename T6 >
inline bool operator == ( const sextuple< T1, T2, T3, T4, T5, T6 >& a, const sextuple< T1, T2, T3, T4, T5, T6 >& b )
{
	return a.first == b.first
		&& a.second == b.second
		&& a.third == b.third
		&& a.fourth == b.fourth
		&& a.fifth == b.fifth
		&& a.sixth == b.sixth;
}

template< typename T1, typename T2, typename T3, typename T4, typename T5, typename T6 >
inline bool operator != ( const sextuple< T1, T2, T3, T4, T5, T6 >& a, const sextuple< T1, T2, T3, T4, T5, T6 >& b )
{
	return !( a == b );
}

template< typename T1, typename T2, typename T3, typename T4, typename T5, typename T6 >
inline sextuple< T1, T2, T3, T4, T5, T6 > make_sextuple( const T1& a, const T2& b, const T3& c, const T4& d, const T5& e, const T6& f )
{
	return sextuple< T1, T2, T3, T4, T5, T6 >( a, b, c, d, e, f );
}
//sextuple
//sixth

///<===========================七元组=================================
template< typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7 >
struct septuple
{
	typedef T1 first_type;
	typedef T2 second_type;
	typedef T3 third_type;
	typedef T4 fourth_type;
	typedef T5 fifth_type;
	typedef T6 sixth_type;
	typedef T7 seventh_type;

	T1 first;
	T2 second;
	T3 third;
	T4 fourth;
	T5 fifth;
	T6 sixth;
	T7 seventh;

	septuple()
		: first(), second(), third(), fourth(), fifth(), sixth(), seventh(){ }

	septuple( const T1& a, const T2& b, const T3& c, const T4& d, const T5& e, const T6& f, const T7& g )
		: first( a ), second( b ), third( c ), fourth( d ), fifth( e ), sixth( f ), seventh( g ){ }

	template< typename U1, typename U2, typename U3, typename U4, typename U5, typename U6, typename U7 >
	septuple( const septuple< U1, U2, U3, U4, U5, U6, U7 >& p )
		: first( p.first ), second( p.second ), third( p.third ), fourth( p.fourth ), fifth( p.fifth ), sixth( p.sixth ), seventh( p.seventh ){ }	

	void clear();
};

template< typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7 >
void septuple< T1, T2, T3, T4, T5, T6, T7 >::clear()
{
	first = T1();
	second = T2();
	third = T3();
	fourth = T4();
	fifth = T5();
	sixth = T6();
	seventh = T7();
}

template< typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7 >
inline bool operator == ( const septuple< T1, T2, T3, T4, T5, T6, T7 >& a, const septuple< T1, T2, T3, T4, T5, T6, T7 >& b )
{
	return a.first == b.first
		&& a.second == b.second
		&& a.third == b.third
		&& a.fourth == b.fourth
		&& a.fifth == b.fifth
		&& a.sixth == b.sixth
		&& a.seventh == b.seventh;
}

template< typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7 >
inline bool operator != ( const septuple< T1, T2, T3, T4, T5, T6, T7 >& a, const septuple< T1, T2, T3, T4, T5, T6, T7 >& b )
{
	return !( a == b );
}

template< typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7 >
inline septuple< T1, T2, T3, T4, T5, T6, T7 > make_septuple( const T1& a, const T2& b, const T3& c, const T4& d, const T5& e, const T6& f, const T7& g )
{
	return septuple< T1, T2, T3, T4, T5, T6, T7 >( a, b, c, d, e, f, g );
}
//septuple
//seventh

///<===========================八元组=================================
template< typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8 >
struct octuple
{
	typedef T1 first_type;
	typedef T2 second_type;
	typedef T3 third_type;
	typedef T4 fourth_type;
	typedef T5 fifth_type;
	typedef T6 sixth_type;
	typedef T7 seventh_type;
	typedef T8 eighth_type;

	T1 first;
	T2 second;
	T3 third;
	T4 fourth;
	T5 fifth;
	T6 sixth;
	T7 seventh;
	T8 eighth;

	octuple()
		: first(), second(), third(), fourth(), fifth(), sixth(), seventh(), eighth(){ }

	octuple( const T1& a, const T2& b, const T3& c, const T4& d, const T5& e, const T6& f, const T7& g, const T8& h )
		: first( a ), second( b ), third( c ), fourth( d ), fifth( e ), sixth( f ), seventh( g ), eighth( h ){ }

	template< typename U1, typename U2, typename U3, typename U4, typename U5, typename U6, typename U7, typename U8 >
	octuple( const octuple< U1, U2, U3, U4, U5, U6, U7, U8 >& p )
		: first( p.first ), second( p.second ), third( p.third ), fourth( p.fourth ), fifth( p.fifth ), sixth( p.sixth ), seventh( p.seventh ), eighth( p.eighth ){ }	

	void clear();
};

template< typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8 >
void octuple< T1, T2, T3, T4, T5, T6, T7, T8 >::clear()
{
	first = T1();
	second = T2();
	third = T3();
	fourth = T4();
	fifth = T5();
	sixth = T6();
	seventh = T7();
	eighth = T8();
}

template< typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8 >
inline bool operator == ( const octuple< T1, T2, T3, T4, T5, T6, T7, T8 >& a, const octuple< T1, T2, T3, T4, T5, T6, T7, T8 >& b )
{
	return a.first == b.first
		&& a.second == b.second
		&& a.third == b.third
		&& a.fourth == b.fourth
		&& a.fifth == b.fifth
		&& a.sixth == b.sixth
		&& a.seventh == b.seventh
		&& a.eighth == b.eighth;
}

template< typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8 >
inline bool operator != ( const octuple< T1, T2, T3, T4, T5, T6, T7, T8 >& a, const octuple< T1, T2, T3, T4, T5, T6, T7, T8 >& b )
{
	return !( a == b );
}

template< typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8 >
inline octuple< T1, T2, T3, T4, T5, T6, T7, T8 > make_octuple( const T1& a, const T2& b, const T3& c, const T4& d, const T5& e, const T6& f, const T7& g, const T8& h )
{
	return octuple< T1, T2, T3, T4, T5, T6, T7, T8 >( a, b, c, d, e, f, g, h );
}
//octuple
//eighth

///<===============================解析元祖======================================
template< typename T >
void parse_proper_type( T& dest, const std::string& src )
{
	VarType var( src );
	dest = var;
}

template< typename T >
void parseTuple( T& one, const char* str, const char* conn = "-" )
{
	if( NULL == str )
		return;	
	parse_proper_type( one, str );
}

template< typename T1, typename T2 >
void parseTuple( std::pair< T1, T2 >& pair, const char* str, const char* conn = "-" )
{
	if( NULL == str )
		return;
	std::vector< std::string > v;
	StringTool::stringtok( v, str, conn );
	if( v.empty() )
		return;

	if( 2 == v.size() )
	{
		parse_proper_type(pair.first, v[0]);
		parse_proper_type(pair.second, v[1]);
	}
	else
	{
		parse_proper_type(pair.first, "0");
		parse_proper_type(pair.second, "0");
	}
}

template< typename T1, typename T2, typename T3 >
void parseTuple( std::pair< T1, std::pair< T2, T3 > >& nest_pair, const char* str, const char* conn = "-" )
{
	if( NULL == str )
		return;
	std::vector< std::string > v;
	StringTool::stringtok( v, str, conn );
	if( v.empty() )
		return;

	assert( 3 == v.size() );
	parse_proper_type( nest_pair.first, v[0] );
	parse_proper_type( nest_pair.second.first, v[1] );
	parse_proper_type( nest_pair.second.second, v[2] );
}

template< typename T1, typename T2, typename T3 >
void parseTuple( Dawn::triple< T1, T2, T3 >& triple, const char* str, const char* conn = "-" )
{
	if( NULL == str )
		return;
	std::vector< std::string > v;
	StringTool::stringtok( v, str, conn );
	if( v.empty() )
		return;

	if (3 == v.size())
	{
		parse_proper_type(triple.first, v[0]);
		parse_proper_type(triple.second, v[1]);
		parse_proper_type(triple.third, v[2]);
	}
	else
	{
		parse_proper_type(triple.first, "0");
		parse_proper_type(triple.second, "0");
		parse_proper_type(triple.third, "0");
	}
	
}

template< typename T1, typename T2, typename T3, typename T4 >
void parseTuple( Dawn::quadruple< T1, T2, T3, T4 >& quadruple, const char* str, const char* conn = "-" )
{
	if( NULL == str )
		return;
	std::vector< std::string > v;
	StringTool::stringtok( v, str, conn );
	if( v.empty() )
		return;

	assert( 4 == v.size() );
	parse_proper_type( quadruple.first, v[0] );
	parse_proper_type( quadruple.second, v[1] );
	parse_proper_type( quadruple.third, v[2] );
	parse_proper_type( quadruple.fourth, v[3] );
}

//quintuple
//fifth
template< typename T1, typename T2, typename T3, typename T4, typename T5 >
void parseTuple( Dawn::quintuple< T1, T2, T3, T4, T5 >& quintuple, const char* str, const char* conn = "-" )
{
	if( NULL == str )
		return;
	std::vector< std::string > v;
	StringTool::stringtok( v, str, conn );
	if( v.empty() )
		return;

	assert( 5 == v.size() );
	parse_proper_type( quintuple.first, v[0] );
	parse_proper_type( quintuple.second, v[1] );
	parse_proper_type( quintuple.third, v[2] );
	parse_proper_type( quintuple.fourth, v[3] );
	parse_proper_type( quintuple.fifth, v[4] );
}

//sextuple
//sixth
template< typename T1, typename T2, typename T3, typename T4, typename T5, typename T6 >
void parseTuple( Dawn::sextuple< T1, T2, T3, T4, T5, T6 >& sextuple, const char* str, const char* conn = "-" )
{
	if( NULL == str )
		return;
	std::vector< std::string > v;
	StringTool::stringtok( v, str, conn );
	if( v.empty() )
		return;

	assert( 6 == v.size() );
	parse_proper_type( sextuple.first, v[0] );
	parse_proper_type( sextuple.second, v[1] );
	parse_proper_type( sextuple.third, v[2] );
	parse_proper_type( sextuple.fourth, v[3] );
	parse_proper_type( sextuple.fifth, v[4] );
	parse_proper_type( sextuple.sixth, v[5] );
}

//septuple
//seventh
template< typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7 >
void parseTuple( Dawn::septuple< T1, T2, T3, T4, T5, T6, T7 >& septuple, const char* str, const char* conn = "-" )
{
	if( NULL == str )
		return;
	std::vector< std::string > v;
	StringTool::stringtok( v, str, conn );
	if( v.empty() )
		return;

	assert( 7 == v.size() );
	parse_proper_type( septuple.first, v[0] );
	parse_proper_type( septuple.second, v[1] );
	parse_proper_type( septuple.third, v[2] );
	parse_proper_type( septuple.fourth, v[3] );
	parse_proper_type( septuple.fifth, v[4] );
	parse_proper_type( septuple.sixth, v[5] );
	parse_proper_type( septuple.seventh, v[6] );
}

//octuple
//eighth
template< typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8 >
void parseTuple( Dawn::octuple< T1, T2, T3, T4, T5, T6, T7, T8 >& octuple, const char* str, const char* conn = "-" )
{
	if( NULL == str )
		return;
	std::vector< std::string > v;
	StringTool::stringtok( v, str, conn );
	if( v.empty() )
		return;

	assert( 8 == v.size() );
	parse_proper_type( octuple.first, v[0] );
	parse_proper_type( octuple.second, v[1] );
	parse_proper_type( octuple.third, v[2] );
	parse_proper_type( octuple.fourth, v[3] );
	parse_proper_type( octuple.fifth, v[4] );
	parse_proper_type( octuple.sixth, v[5] );
	parse_proper_type( octuple.seventh, v[6] );
	parse_proper_type( octuple.eighth, v[7] );
}



template< typename T, typename Container >
void parseTupleContainer( Container& container, const char* str, const char* conn = "-;" )
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

		T tuple;
		conn_t[0] = conn[0];
		parseTuple( tuple, t.c_str(), conn_t );

		container.insert( container.end(), tuple );
	}
}


template< typename T, typename Container >
void parseDoubleTupleContainer( std::vector< Container >& container, const char* str, const char* conn = "-;|" )
{
	if( NULL == conn || NULL == str )
		return;	
	assert( strlen( conn ) == 3 );
	std::vector< std::string > vt_ex;
	char conn_t_ex[2] = {'\0'};
	conn_t_ex[0] = conn[2];
	StringTool::stringtok( vt_ex, str, conn_t_ex );
	std::vector< std::string >::const_iterator it_ex;
	for( it_ex = vt_ex.begin(); it_ex != vt_ex.end(); ++it_ex )
	{
		std::vector< std::string > vt;
		char conn_t[2] = {'\0'};
		conn_t[0] = conn[1];
		StringTool::stringtok( vt, *it_ex, conn_t );

		Container sub_container;

		std::vector< std::string >::const_iterator it;
		for( it = vt.begin(); it != vt.end(); ++it )
		{
			const std::string t = *it;
			if( t.empty() )
				continue;

			T tuple;
			conn_t[0] = conn[0];
			parseTuple( tuple, t.c_str(), conn_t );

			sub_container.insert( sub_container.end(), tuple );
		}
		container.push_back( sub_container );
	}

}



template< typename T >
void tryParseTuple( T& one, const char* str, const char* conn = "-" )
{
	if( NULL == str )
		return;	
	parse_proper_type( one, str );
}

template< typename T1, typename T2 >
void tryParseTuple( std::pair< T1, T2 >& pair, const char* str, const char* conn = "-" )
{
	if( NULL == str )
		return;
	std::vector< std::string > v;
	StringTool::stringtok( v, str, conn );
	if( v.empty() )
		return;

	//assert( 2 == v.size() );
	if( v.size() >= 1 )
		parse_proper_type( pair.first, v[0] );
	if( v.size() >= 2 )
		parse_proper_type( pair.second, v[1] );
}

template< typename T1, typename T2, typename T3 >
void tryParseTuple( std::pair< T1, std::pair< T2, T3 > >& nest_pair, const char* str, const char* conn = "-" )
{
	if( NULL == str )
		return;
	std::vector< std::string > v;
	StringTool::stringtok( v, str, conn );
	if( v.empty() )
		return;

	//assert( 3 == v.size() );
	if( v.size() >= 1 )
		parse_proper_type( nest_pair.first, v[0] );
	if( v.size() >= 2 )
		parse_proper_type( nest_pair.second.first, v[1] );
	if( v.size() >= 3 )
		parse_proper_type( nest_pair.second.second, v[2] );
}

template< typename T1, typename T2, typename T3 >
void tryParseTuple( Dawn::triple< T1, T2, T3 >& triple, const char* str, const char* conn = "-" )
{
	if( NULL == str )
		return;
	std::vector< std::string > v;
	StringTool::stringtok( v, str, conn );
	if( v.empty() )
		return;

	//assert( 3 == v.size() );
	if( v.size() >= 1 )
		parse_proper_type( triple.first, v[0] );
	if( v.size() >= 2 )
		parse_proper_type( triple.second, v[1] );
	if( v.size() >= 3 )
		parse_proper_type( triple.third, v[2] );
}

template< typename T1, typename T2, typename T3, typename T4 >
void tryParseTuple( Dawn::quadruple< T1, T2, T3, T4 >& quadruple, const char* str, const char* conn = "-" )
{
	if( NULL == str )
		return;
	std::vector< std::string > v;
	StringTool::stringtok( v, str, conn );
	if( v.empty() )
		return;

	//assert( 4 == v.size() );
	if( v.size() >= 1 )
		parse_proper_type( quadruple.first, v[0] );
	if( v.size() >= 2 )
		parse_proper_type( quadruple.second, v[1] );
	if( v.size() >= 3 )
		parse_proper_type( quadruple.third, v[2] );
	if( v.size() >= 4 )
		parse_proper_type( quadruple.fourth, v[3] );
}

//quintuple
//fifth
template< typename T1, typename T2, typename T3, typename T4, typename T5 >
void tryParseTuple( Dawn::quintuple< T1, T2, T3, T4, T5 >& quintuple, const char* str, const char* conn = "-" )
{
	if( NULL == str )
		return;
	std::vector< std::string > v;
	StringTool::stringtok( v, str, conn );
	if( v.empty() )
		return;

	//assert( 5 == v.size() );
	if( v.size() >= 1 )
		parse_proper_type( quintuple.first, v[0] );
	if( v.size() >= 2 )
		parse_proper_type( quintuple.second, v[1] );
	if( v.size() >= 3 )
		parse_proper_type( quintuple.third, v[2] );
	if( v.size() >= 4 )
		parse_proper_type( quintuple.fourth, v[3] );
	if( v.size() >= 5 )
		parse_proper_type( quintuple.fifth, v[4] );
}

//sextuple
//sixth
template< typename T1, typename T2, typename T3, typename T4, typename T5, typename T6 >
void tryParseTuple( Dawn::sextuple< T1, T2, T3, T4, T5, T6 >& sextuple, const char* str, const char* conn = "-" )
{
	if( NULL == str )
		return;
	std::vector< std::string > v;
	StringTool::stringtok( v, str, conn );
	if( v.empty() )
		return;

	//assert( 6 == v.size() );
	if( v.size() >= 1 )
		parse_proper_type( sextuple.first, v[0] );
	if( v.size() >= 2 )
		parse_proper_type( sextuple.second, v[1] );
	if( v.size() >= 3 )
		parse_proper_type( sextuple.third, v[2] );
	if( v.size() >= 4 )
		parse_proper_type( sextuple.fourth, v[3] );
	if( v.size() >= 5 )
		parse_proper_type( sextuple.fifth, v[4] );
	if( v.size() >= 6 )
		parse_proper_type( sextuple.sixth, v[5] );
}

//septuple
//seventh
template< typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7 >
void tryParseTuples( Dawn::septuple< T1, T2, T3, T4, T5, T6, T7 >& septuple, const char* str, const char* conn = "-" )
{
	if( NULL == str )
		return;
	std::vector< std::string > v;
	StringTool::stringtok( v, str, conn );
	if( v.empty() )
		return;

	//assert( 7 == v.size() );
	if( v.size() >= 1 )
		parse_proper_type( septuple.first, v[0] );
	if( v.size() >= 2 )
		parse_proper_type( septuple.second, v[1] );
	if( v.size() >= 3 )
		parse_proper_type( septuple.third, v[2] );
	if( v.size() >= 4 )
		parse_proper_type( septuple.fourth, v[3] );
	if( v.size() >= 5 )
		parse_proper_type( septuple.fifth, v[4] );
	if( v.size() >= 6 )
		parse_proper_type( septuple.sixth, v[5] );
	if( v.size() >= 7 )
		parse_proper_type( septuple.seventh, v[6] );
}

//octuple
//eighth
template< typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8 >
void tryParseTuples( Dawn::octuple< T1, T2, T3, T4, T5, T6, T7, T8 >& octuple, const char* str, const char* conn = "-" )
{
	if( NULL == str )
		return;
	std::vector< std::string > v;
	StringTool::stringtok( v, str, conn );
	if( v.empty() )
		return;

	//assert( 8 == v.size() );
	if( v.size() >= 1 )
		parse_proper_type( octuple.first, v[0] );
	if( v.size() >= 2 )
		parse_proper_type( octuple.second, v[1] );
	if( v.size() >= 3 )
		parse_proper_type( octuple.third, v[2] );
	if( v.size() >= 4 )
		parse_proper_type( octuple.fourth, v[3] );
	if( v.size() >= 5 )
		parse_proper_type( octuple.fifth, v[4] );
	if( v.size() >= 6 )
		parse_proper_type( octuple.sixth, v[5] );
	if( v.size() >= 7 )
		parse_proper_type( octuple.seventh, v[6] );
	if( v.size() >= 8 )
		parse_proper_type( octuple.eighth, v[7] );
}

template< typename T, typename Container >
void tryParseTupleContainer( Container& container, const char* str, const char* conn = "-;" )
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

		T tuple;
		conn_t[0] = conn[0];
		tryParseTuples( tuple, t.c_str(), conn_t );

		container.insert( container.end(), tuple );
	}
}


template< typename T, typename Container >
void tryParseDoubleTupleContainer( std::vector< Container >& container, const char* str, const char* conn = "-;|" )
{
	if( NULL == conn || NULL == str )
		return;	
	assert( strlen( conn ) == 3 );
	std::vector< std::string > vt_ex;
	char conn_t_ex[2] = {'\0'};
	conn_t_ex[0] = conn[2];
	StringTool::stringtok( vt_ex, str, conn_t_ex );
	std::vector< std::string >::const_iterator it_ex;
	for( it_ex = vt_ex.begin(); it_ex != vt_ex.end(); ++it_ex )
	{
		std::vector< std::string > vt;
		char conn_t[2] = {'\0'};
		conn_t[0] = conn[1];
		StringTool::stringtok( vt, *it_ex, conn_t );

		Container sub_container;

		std::vector< std::string >::const_iterator it;
		for( it = vt.begin(); it != vt.end(); ++it )
		{
			const std::string t = *it;
			if( t.empty() )
				continue;

			T tuple;
			conn_t[0] = conn[0];
			tryParseTuples( tuple, t.c_str(), conn_t );

			sub_container.insert( sub_container.end(), tuple );
		}
		container.push_back( sub_container );
	}

}


};
