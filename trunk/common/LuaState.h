/**
 * @author Ī��ȫ
 * @data   2011��10��9�� 11ʱ02��01��
 * @brief  lua��c++��װ
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
	/// @brief ���¼���
	/// @return �����Ƿ�ɹ�
	bool reset();
	/// @brief ��������
	void final();
	/// @brief ��ʼ��
	bool init();

	const lua_State* getHandle() const;
	lua_State* getHandle();

	/// @brief �ж�һ�������Ƿ����
	/// @param name ��������
	/// @return �Ƿ����
	bool isExistFunction( const char* name ) const;

	/// @brief ���ز�ִ��һ��lua�ļ�
	/// @param filename �ļ���
	/// @return �����Ƿ�ɹ�
	bool doFile( const char* filename );
	
	/// @brief ��ȫ��lua��ջ�л��һ��unsigned int����
	/// @param name ��������
	/// @return �õ�����ֵ��δ�ҵ���������0
	unsigned int getGlobalNumber( const char* name ) const;

	/// @brief ��ȫ��lua��ջ�л��һ��char*����
	/// @param name ��������
	/// @return �õ����ַ���δ�ҵ���������NULL
	const char* getGlobalString( const char* name ) const;

	/// @brief ע��һ��C������lua�ű�ʹ��
	/// @param funcname c����
	/// @param funclua  luaʹ��ʱ�Ľӿ�����
	void regFunc( const char* funcname, lua_CFunction func );


public:

	/// @brief ���ز���ֵ��������
	bool callRTBool( const char* func );

	/// @brief �޷���ֵ�Ľű���������
	void callRTVoid( const char* func );

	/// @brief ������������ֵ��������
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

	/// @brief �����������ַ�����������
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

	/// @brief �޷���ֵ�ű���������
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


	/// @brief ��lua��ջѹ��Ԫ�أ�ͨ��C++���������Զ���λ
	/// @param v ѹ���ջ��ֵ
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
	/// @brief lua�����
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
			log_error( "�ű�����:[%s]", lua_tostring( L, -1 ) );
			lua_pop( L, 1 ); ///< ����������Ϣ
			return false;
		}
		value = ( T )lua_tonumber( L, -1 );
		return true;
	}
	log_error( "����������:[%s]", func );
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
			log_error( "�ű�����:[%s]", lua_tostring( L, -1 ) );
			lua_pop( L, 1 ); ///< ����������Ϣ
			return false;
		}
		value = ( T )lua_tonumber( L, -1 );
		return true;
	}
	log_error( "����������:[%s]", func );
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
			log_error( "�ű�����:[%s]", lua_tostring( L, -1 ) );
			lua_pop( L, 1 ); ///< ����������Ϣ
			return false;
		}
		value = ( T )lua_tonumber( L, -1 );
		return true;
	}
	log_error( "����������:[%s]", func );
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
			log_error( "�ű�����:[%s]", lua_tostring( L, -1 ) );
			lua_pop( L, 1 ); ///< ����������Ϣ
			return false;
		}
		value = ( T )lua_tonumber( L, -1 );
		return true;
	}
	log_error( "����������:[%s]", func );
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
			log_error( "�ű�����:[%s]", lua_tostring( L, -1 ) );
			lua_pop( L, 1 ); ///< ����������Ϣ
			return false;
		}
		value = ( T )lua_tonumber( L, -1 );
		return true;
	}
	log_error( "����������:[%s]", func );
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
			log_error( "�ű�����:[%s]", lua_tostring( L, -1 ) );
			lua_pop( L, 1 ); ///< ����������Ϣ
			return false;
		}
		const char* ret = lua_tostring( L, -1 );
		if( ret != NULL )
		{
			strncpy( value, ret, size );
			return true;
		}
		log_error( "����ֵ����:[%s]", func );
		return false;
	}
	log_error( "����������:[%s]", func );
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
			log_error( "�ű�����:[%s]", lua_tostring( L, -1 ) );
			lua_pop( L, 1 ); ///< ����������Ϣ
			return false;
		}
		const char* ret = lua_tostring( L, -1 );
		if( ret != NULL )
		{
			strncpy( value, ret, size );
			return true;
		}
		log_error( "����ֵ����:[%s]", func );
		return false;
	}
	log_error( "����������:[%s]", func );
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
			log_error( "�ű�����:[%s]", lua_tostring( L, -1 ) );
			lua_pop( L, 1 ); ///< ����������Ϣ
			return false;
		}
		const char* ret = lua_tostring( L, -1 );
		if( ret != NULL )
		{
			strncpy( value, ret, size );
			return true;
		}
		log_error( "����ֵ����:[%s]", func );
		return false;
	}
	log_error( "����������:[%s]", func );
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
			log_error( "�ű�����:[%s]", lua_tostring( L, -1 ) );
			lua_pop( L, 1 ); ///< ����������Ϣ
			return false;
		}
		const char* ret = lua_tostring( L, -1 );
		if( ret != NULL )
		{
			strncpy( value, ret, size );
			return true;
		}
		log_error( "����ֵ����:[%s]", func );
		return false;
	}
	log_error( "����������:[%s]", func );
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
			log_error( "�ű�����:[%s]", lua_tostring( L, -1 ) );
			lua_pop( L, 1 ); ///< ����������Ϣ
			return false;
		}
		const char* ret = lua_tostring( L, -1 );
		if( ret != NULL )
		{
			strncpy( value, ret, size );
			return true;
		}
		log_error( "����ֵ����:[%s]", func );
		return false;
	}
	log_error( "����������:[%s]", func );
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
			log_error( "�ű�����:[%s]", lua_tostring( L, -1 ) );
			lua_pop( L, 1 ); ///< ����������Ϣ
		}
		return;
	}
	log_error( "����������:[%s]", func );
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
			log_error( "�ű�����:[%s]", lua_tostring( L, -1 ) );
			lua_pop( L, 1 ); ///< ����������Ϣ
		}
		return;
	}
	log_error( "����������:[%s]", func );
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
			log_error( "�ű�����:[%s]", lua_tostring( L, -1 ) );
			lua_pop( L, 1 ); ///< ����������Ϣ
		}
		return;
	}
	log_error( "����������:[%s]", func );
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
			log_error( "�ű�����:[%s]", lua_tostring( L, -1 ) );
			lua_pop( L, 1 ); ///< ����������Ϣ
		}
		return;
	}
	log_error( "����������:[%s]", func );
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
			log_error( "�ű�����:[%s]", lua_tostring( L, -1 ) );
			lua_pop( L, 1 ); ///< ����������Ϣ
		}
		return;
	}
	log_error( "����������:[%s]", func );
}
