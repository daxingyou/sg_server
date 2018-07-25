#pragma once 
#include <string>
#include <vector>

class VarType
{
public:
	VarType() : _b_misc( 1, '\0' ){}

	VarType( const VarType& vt ){ _b_misc = vt._b_misc; }

	template< typename X >
	explicit VarType( const X& value )
	{
		put( value );
	}

	/**
	 *	赋值操作
	 **/

	template< typename X >
	inline VarType & operator = ( const X& value )
	{
		put( value );
		return *this;
	}

	inline VarType & operator = ( const VarType& vt )
	{
		this->_b_misc = vt._b_misc;
		return *this;
	}


	/**
	 *	填充数据
	 **/
	void put( const void* data, const unsigned int len )
	{
		this->_b_misc.resize( len + 1 );
		if( len )
		{
			bcopy( data, &_b_misc[0], len );
		}
		_b_misc[len] = '\0';
	}

	template< typename X >
	void put( const X& value )
	{
		std::ostringstream oss;
		oss << value;
		_b_misc.resize( oss.str().length() + 1 );
		strcpy( ( char* )(&_b_misc[0]), oss.str().c_str() );
	}

	void put( const unsigned char& value )
	{
		put( static_cast< unsigned short >( value ) );
	}



	template< const void* >
	void put( const void *value );

	/**
	 *	运算符重载
	 **/
	operator unsigned char() const
	{
		return atoi( ( const char* )(*this) );
	}

	operator char() const
	{
		return atoi( ( const char* )(*this) );
	}

	operator unsigned short() const
	{
		return atoi( ( const char* )(*this) );
	}

	operator short() const
	{
		return atoi( ( const char* )(*this) );
	}

	operator unsigned int() const
	{
		return atoi( ( const char* )(*this) );
	}

	operator int() const
	{
		return atoi( ( const char* )(*this) );
	}

	operator unsigned long() const
	{
		return strtoul( ( const char* )(*this), NULL, 10 );
	}

	operator long() const
	{
		return strtol( ( const char* )(*this), NULL, 10 );
	}
#ifdef _MSC_VER
	operator __int64() const
	{
		return _atoi64( ( const char* )(*this) );
	}

	operator unsigned __int64() const
	{
		return _strtoui64( ( const char* )(*this), NULL, 10 );
	}
#else
	operator long long() const
	{
		return strtoll( ( const char* )(*this), NULL, 10 );
	}

	operator unsigned long long() const
	{
		return strtoull( ( const char* )(*this), NULL, 10 );
	}
#endif

	operator float() const
	{
		return ( float )atof( ( const char* )(*this) );
	}
	
	operator double() const
	{
		return atof( ( const char* )(*this) );
	}

	operator const char*() const
	{
		return ( const char* )(&_b_misc[0]);
	}

	operator const void*() const
	{
		return ( const void* )(&_b_misc[0]);
	}

	unsigned int size() const
	{
		if( _b_misc.empty() )
			return 0;
		else
			return _b_misc.size() - 1;
	}

	bool empty() const
	{
		return _b_misc.empty() || ( 1== _b_misc.size() );
	}


	bool valid()
	{
		return !empty();
	}

private:
	/// @brief 二进制原始数据
	std::vector< unsigned char > _b_misc;
};
