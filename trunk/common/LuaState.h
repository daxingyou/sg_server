/**
 * @author 莫鸿全
 * @data   2011年10月9日 11时02分01秒
 * @brief  lua的c++封装
 *
 */

#pragma once


#include "lua.hpp"
#include "log.hpp"

class ScriptProxyBase;

class LuaState
{
	friend class ScriptProxyBase;
protected:
	LuaState();
	virtual ~LuaState();

public:
	/// @brief 重新加载
	/// @return 加载是否成功
	bool reset();
	/// @brief 结束回收
	void final();
	/// @brief 初始化
	bool init();

	const lua_State* getHandle() const;
	lua_State* getHandle();

	/// @brief 判断一个函数是否存在
	/// @param name 函数名称
	/// @return 是否存在
	bool isExistFunction( const char* name ) const;

	/// @brief 加载并执行一个lua文件
	/// @param filename 文件名
	/// @return 加载是否成功
	bool doFile( const char* filename );
	
	/// @brief 从全局lua堆栈中获得一个unsigned int变量
	/// @param name 变量名称
	/// @return 得到的数值，未找到变量返回0
	unsigned int getGlobalNumber( const char* name ) const;

	/// @brief 从全局lua堆栈中获得一个char*变量
	/// @param name 变量名称
	/// @return 得到的字符，未找到变量返回NULL
	const char* getGlobalString( const char* name ) const;

	/// @brief 注册一个C函数供lua脚本使用
	/// @param funcname c函数
	/// @param funclua  lua使用时的接口名称
	void regFunc( const char* funcname, lua_CFunction func );


public:

	/// @brief 返回布尔值函数调用
	bool callRTBool( const char* func );

	/// @brief 无返回值的脚本函数调用
	void callRTVoid( const char* func );

	/// @brief 带参数返回数值函数调用
	template< typename T, typename P1 >
	bool callRT( const char* func, const P1 p1, T& value );

	template< typename T, typename P1, typename P2 >
	bool callRT( const char* func, const P1 p1, const P2 p2, T& value );

	template< typename T, typename P1, typename P2, typename P3 >
	bool callRT( const char* func, const P1 p1, const P2 p2, const P3 p3, T& value );

	template< typename T, typename P1, typename P2, typename P3, typename P4 >
	bool callRT( const char* func, const P1 p1, const P2 p2, const P3 p3, const P4 p4, T& value );

	template< typename T, typename P1, typename P2, typename P3, typename P4, typename P5 >
	bool callRT( const char* func, const P1 p1, const P2 p2, const P3 p3, const P4 p4, const P5 p5, T& value );

	/// @brief 带参数返回字符串函数调用
	template< typename P1 >
	bool callRT( const char* func, const P1 p1, char* value, const int size );

	template< typename P1, typename P2 >
	bool callRT( const char* func, const P1 p1, const P2 p2, char* value, const int size );

	template< typename P1, typename P2, typename P3 >
	bool callRT( const char* func, const P1 p1, const P2 p2, const P3 p3, char* value, const int size );

	template< typename P1, typename P2, typename P3, typename P4 >
	bool callRT( const char* func, const P1 p1, const P2 p2, const P3 p3, const P4 p4, char* value, const int size );

	template< typename P1, typename P2, typename P3, typename P4, typename P5 >
	bool callRT( const char* func, const P1 p1, const P2 p2, const P3 p3, const P4 p4, const P5 p5, char* value, const int size );

	/// @brief 无返回值脚本函数调用
	template< typename P1 >
	void callRTVoid( const char* func, P1 p1 );

	template< typename P1, typename P2 >
	void callRTVoid( const char* func, P1 p1, const P2 p2 );

	template< typename P1, typename P2, typename P3 >
	void callRTVoid( const char* func, P1 p1, const P2 p2, const P3 p3 );

	template< typename P1, typename P2, typename P3, typename P4 >
	void callRTVoid( const char* func, P1 p1, const P2 p2, const P3 p3, const P4 p4 );

	template< typename P1, typename P2, typename P3, typename P4, typename P5 >
	void callRTVoid( const char* func, P1 p1, const P2 p2, const P3 p3, const P4 p4, const P5 p5 );


	/// @brief 向lua堆栈压入元素，通过C++函数重载自动定位
	/// @param v 压入堆栈的值
	void pushValue( const int v );
	void pushValue( const short v );
	void pushValue( const char v );
	void pushValue( const long v );
	void pushValue( const unsigned int v );
	void pushValue( const unsigned short v );
	void pushValue( const unsigned char v );
	void pushValue( const unsigned long v );
	void pushValue( const unsigned long long v );
	void pushValue( const signed long long v );
	void pushValue( const float v );
	void pushValue( const double v );
	void pushValue( const long double v );
	void pushValue( const bool v );
	void pushValue( const char* v );
	void pushValue( const ScriptProxyBase* script );
	template< typename T >
	void pushValue( T* value );


protected:
	/// @brief lua虚拟机
	lua_State* L = NULL;
};

inline const lua_State* LuaState::getHandle() const
{
	return L;
}

inline lua_State* LuaState::getHandle()
{
	return L;
}

inline void LuaState::pushValue( const int v )
{
	lua_pushnumber( L, ( lua_Number )v );
}

inline void LuaState::pushValue( const short v )
{
	lua_pushnumber( L, ( lua_Number )v );
}

inline void LuaState::pushValue( const char v )
{
	lua_pushnumber( L, ( lua_Number )v );
}

inline void LuaState::pushValue( const long v )
{
	lua_pushnumber( L, ( lua_Number )v );
}

inline void LuaState::pushValue( const unsigned int v )
{
	lua_pushnumber( L, ( lua_Number )v );
}

inline void LuaState::pushValue( const unsigned short v )
{
	lua_pushnumber( L, ( lua_Number )v );
}

inline void LuaState::pushValue( const unsigned char v )
{
	lua_pushnumber( L, ( lua_Number )v );
}

inline void LuaState::pushValue( const unsigned long v )
{
	lua_pushnumber( L, ( lua_Number )v );
}

inline void LuaState::pushValue( const float v )
{
	lua_pushnumber( L, ( lua_Number )v );
}

inline void LuaState::pushValue( const double v )
{
	lua_pushnumber( L, ( lua_Number )v );
}

inline void LuaState::pushValue( const long double v )
{
	lua_pushnumber( L, ( lua_Number )v );
}

inline void LuaState::pushValue( const unsigned long long v )
{
	lua_pushnumber( L, ( lua_Number )v );
}

inline void LuaState::pushValue( const signed long long v )
{
	lua_pushnumber( L, ( lua_Number )v );
}

inline void LuaState::pushValue( const bool v )
{
	lua_pushboolean( L, v );
}

inline void LuaState::pushValue( const char* v )
{
	v ? lua_pushstring( L, v ) : lua_pushnil( L );
}

template< typename T >
inline void LuaState::pushValue( T* value )
{
	pushValue( ( const ScriptProxyBase* )value );
}

template< typename T, typename P1 >
bool LuaState::callRT( const char* func, const P1 p1, T& value )
{
	if( isExistFunction( func ) )
	{
		lua_getglobal( L, func );
		pushValue( p1 );
		lua_checkstack( L, 10 );
		if( lua_pcall( L, 1, 1, 0 ) )
		{
			log_error( "脚本错误:[%s]", lua_tostring( L, -1 ) );
			lua_pop( L, 1 ); ///< 弹出错误信息
			return false;
		}
		value = ( T )lua_tonumber( L, -1 );
		return true;
	}
	log_error( "函数不存在:[%s]", func );
	return false;
}

template< typename T, typename P1, typename P2 >
bool LuaState::callRT( const char* func, const P1 p1, const P2 p2, T& value )
{
	if( isExistFunction( func ) )
	{
		lua_getglobal( L, func );
		pushValue( p1 );
		pushValue( p2 );
		lua_checkstack( L, 10 );
		if( lua_pcall( L, 2, 1, 0 ) )
		{
			log_error( "脚本错误:[%s]", lua_tostring( L, -1 ) );
			lua_pop( L, 1 ); ///< 弹出错误信息
			return false;
		}
		value = ( T )lua_tonumber( L, -1 );
		return true;
	}
	log_error( "函数不存在:[%s]", func );
	return false;
}

template< typename T, typename P1, typename P2, typename P3 >
bool LuaState::callRT( const char* func, const P1 p1, const P2 p2, const P3 p3, T& value )
{
	if( isExistFunction( func ) )
	{
		lua_getglobal( L, func );
		pushValue( p1 );
		pushValue( p2 );
		pushValue( p3 );
		lua_checkstack( L, 10 );
		if( lua_pcall( L, 3, 1, 0 ) )
		{
			log_error( "脚本错误:[%s]", lua_tostring( L, -1 ) );
			lua_pop( L, 1 ); ///< 弹出错误信息
			return false;
		}
		value = ( T )lua_tonumber( L, -1 );
		return true;
	}
	log_error( "函数不存在:[%s]", func );
	return false;	
}

template< typename T, typename P1, typename P2, typename P3, typename P4 >
bool LuaState::callRT( const char* func, const P1 p1, const P2 p2, const P3 p3, const P4 p4, T& value )
{
	if( isExistFunction( func ) )
	{
		lua_getglobal( L, func );
		pushValue( p1 );
		pushValue( p2 );
		pushValue( p3 );
		pushValue( p4 );
		lua_checkstack( L, 10 );
		if( lua_pcall( L, 4, 1, 0 ) )
		{
			log_error( "脚本错误:[%s]", lua_tostring( L, -1 ) );
			lua_pop( L, 1 ); ///< 弹出错误信息
			return false;
		}
		value = ( T )lua_tonumber( L, -1 );
		return true;
	}
	log_error( "函数不存在:[%s]", func );
	return false;
}

template< typename T, typename P1, typename P2, typename P3, typename P4, typename P5 >
bool LuaState::callRT( const char* func, const P1 p1, const P2 p2, const P3 p3, const P4 p4, const P5 p5, T& value )
{
	if( isExistFunction( func ) )
	{
		lua_getglobal( L, func );
		pushValue( p1 );
		pushValue( p2 );
		pushValue( p3 );
		pushValue( p4 );
		pushValue( p5 );
		lua_checkstack( L, 10 );
		if( lua_pcall( L, 5, 1, 0 ) )
		{
			log_error( "脚本错误:[%s]", lua_tostring( L, -1 ) );
			lua_pop( L, 1 ); ///< 弹出错误信息
			return false;
		}
		value = ( T )lua_tonumber( L, -1 );
		return true;
	}
	log_error( "函数不存在:[%s]", func );
	return false;
}

template< typename P1 >
bool LuaState::callRT( const char* func, const P1 p1, char* value, const int size )
{
	if( isExistFunction( func ) )
	{
		lua_getglobal( L, func );
		pushValue( p1 );
		lua_checkstack( L, 10 );
		if( lua_pcall( L, 1, 1, 0 ) )
		{
			log_error( "脚本错误:[%s]", lua_tostring( L, -1 ) );
			lua_pop( L, 1 ); ///< 弹出错误信息
			return false;
		}
		const char* ret = lua_tostring( L, -1 );
		if( ret != NULL )
		{
			strncpy( value, ret, size );
			return true;
		}
		log_error( "返回值错误:[%s]", func );
		return false;
	}
	log_error( "函数不存在:[%s]", func );
	return false;	
}

template< typename P1, typename P2 >
bool LuaState::callRT( const char* func, const P1 p1, const P2 p2, char* value, const int size )
{
	if( isExistFunction( func ) )
	{
		lua_getglobal( L, func );
		pushValue( p1 );
		pushValue( p2 );
		lua_checkstack( L, 10 );
		if( lua_pcall( L, 2, 1, 0 ) )
		{
			log_error( "脚本错误:[%s]", lua_tostring( L, -1 ) );
			lua_pop( L, 1 ); ///< 弹出错误信息
			return false;
		}
		const char* ret = lua_tostring( L, -1 );
		if( ret != NULL )
		{
			strncpy( value, ret, size );
			return true;
		}
		log_error( "返回值错误:[%s]", func );
		return false;
	}
	log_error( "函数不存在:[%s]", func );
	return false;	
}

template< typename P1, typename P2, typename P3 >
bool LuaState::callRT( const char* func, const P1 p1, const P2 p2, const P3 p3, char* value, const int size )
{
	if( isExistFunction( func ) )
	{
		lua_getglobal( L, func );
		pushValue( p1 );
		pushValue( p2 );
		pushValue( p3 );
		lua_checkstack( L, 10 );
		if( lua_pcall( L, 3, 1, 0 ) )
		{
			log_error( "脚本错误:[%s]", lua_tostring( L, -1 ) );
			lua_pop( L, 1 ); ///< 弹出错误信息
			return false;
		}
		const char* ret = lua_tostring( L, -1 );
		if( ret != NULL )
		{
			strncpy( value, ret, size );
			return true;
		}
		log_error( "返回值错误:[%s]", func );
		return false;
	}
	log_error( "函数不存在:[%s]", func );
	return false;
}

template< typename P1, typename P2, typename P3, typename P4 >
bool LuaState::callRT( const char* func, const P1 p1, const P2 p2, const P3 p3, const P4 p4, char* value, const int size )
{
	if( isExistFunction( func ) )
	{
		lua_getglobal( L, func );
		pushValue( p1 );
		pushValue( p2 );
		pushValue( p3 );
		pushValue( p4 );
		lua_checkstack( L, 10 );
		if( lua_pcall( L, 4, 1, 0 ) )
		{
			log_error( "脚本错误:[%s]", lua_tostring( L, -1 ) );
			lua_pop( L, 1 ); ///< 弹出错误信息
			return false;
		}
		const char* ret = lua_tostring( L, -1 );
		if( ret != NULL )
		{
			strncpy( value, ret, size );
			return true;
		}
		log_error( "返回值错误:[%s]", func );
		return false;
	}
	log_error( "函数不存在:[%s]", func );
	return false;
}

template< typename P1, typename P2, typename P3, typename P4, typename P5 >
bool LuaState::callRT( const char* func, const P1 p1, const P2 p2, const P3 p3, const P4 p4, const P5 p5, char* value, const int size )
{
	if( isExistFunction( func ) )
	{
		lua_getglobal( L, func );
		pushValue( p1 );
		pushValue( p2 );
		pushValue( p3 );
		pushValue( p4 );
		pushValue( p5 );
		lua_checkstack( L, 10 );
		if( lua_pcall( L, 5, 1, 0 ) )
		{
			log_error( "脚本错误:[%s]", lua_tostring( L, -1 ) );
			lua_pop( L, 1 ); ///< 弹出错误信息
			return false;
		}
		const char* ret = lua_tostring( L, -1 );
		if( ret != NULL )
		{
			strncpy( value, ret, size );
			return true;
		}
		log_error( "返回值错误:[%s]", func );
		return false;
	}
	log_error( "函数不存在:[%s]", func );
	return false;
}

template< typename P1 >
void LuaState::callRTVoid( const char* func, P1 p1 )
{
	if( isExistFunction( func ) )
	{
		lua_getglobal( L, func );
		pushValue( p1 );
		lua_checkstack( L, 10 );
		if( lua_pcall( L, 1, 0, 0 ) )
		{
			log_error( "脚本错误:[%s]", lua_tostring( L, -1 ) );
			lua_pop( L, 1 ); ///< 弹出错误信息
		}
		return;
	}
	log_error( "函数不存在:[%s]", func );
}

template< typename P1, typename P2 >
void LuaState::callRTVoid( const char* func, P1 p1, const P2 p2 )
{
	if( isExistFunction( func ) )
	{
		lua_getglobal( L, func );
		pushValue( p1 );
		pushValue( p2 );
		lua_checkstack( L, 10 );
		if( lua_pcall( L, 2, 0, 0 ) )
		{
			log_error( "脚本错误:[%s]", lua_tostring( L, -1 ) );
			lua_pop( L, 1 ); ///< 弹出错误信息
		}
		return;
	}
	log_error( "函数不存在:[%s]", func );
}

template< typename P1, typename P2, typename P3 >
void LuaState::callRTVoid( const char* func, P1 p1, const P2 p2, const P3 p3 )
{
	if( isExistFunction( func ) )
	{
		lua_getglobal( L, func );
		pushValue( p1 );
		pushValue( p2 );
		pushValue( p3 );
		lua_checkstack( L, 10 );
		if( lua_pcall( L, 3, 0, 0 ) )
		{
			log_error( "脚本错误:[%s]", lua_tostring( L, -1 ) );
			lua_pop( L, 1 ); ///< 弹出错误信息
		}
		return;
	}
	log_error( "函数不存在:[%s]", func );
}

template< typename P1, typename P2, typename P3, typename P4 >
void LuaState::callRTVoid( const char* func, P1 p1, const P2 p2, const P3 p3, const P4 p4 )
{
	if( isExistFunction( func ) )
	{
		lua_getglobal( L, func );
		pushValue( p1 );
		pushValue( p2 );
		pushValue( p3 );
		pushValue( p4 );
		lua_checkstack( L, 10 );
		if( lua_pcall( L, 4, 0, 0 ) )
		{
			log_error( "脚本错误:[%s]", lua_tostring( L, -1 ) );
			lua_pop( L, 1 ); ///< 弹出错误信息
		}
		return;
	}
	log_error( "函数不存在:[%s]", func );
}

template< typename P1, typename P2, typename P3, typename P4, typename P5 >
void LuaState::callRTVoid( const char* func, P1 p1, const P2 p2, const P3 p3, const P4 p4, const P5 p5 )
{
	if( isExistFunction( func ) )
	{
		lua_getglobal( L, func );
		pushValue( p1 );
		pushValue( p2 );
		pushValue( p3 );
		pushValue( p4 );
		pushValue( p5 );
		lua_checkstack( L, 10 );
		if( lua_pcall( L, 5, 0, 0 ) )
		{
			log_error( "脚本错误:[%s]", lua_tostring( L, -1 ) );
			lua_pop( L, 1 ); ///< 弹出错误信息
		}
		return;
	}
	log_error( "函数不存在:[%s]", func );
}
