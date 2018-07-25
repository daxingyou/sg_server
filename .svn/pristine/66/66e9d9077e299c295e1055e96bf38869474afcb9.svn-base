#include "LuaState.h"
#include "ScriptProxy.h"

extern "C" int luaopen_bit( lua_State* lua );

LuaState::LuaState()
{
	init();
}

LuaState::~LuaState()
{
	final();
}

bool LuaState::reset()
{
	final();
	return init();
}

void LuaState::final()
{
	if( L )
	{
		lua_close( L );
		L = NULL;
	}
}

bool LuaState::init()
{
	L = luaL_newstate();
	luaL_openlibs( L );
	luaopen_bit( L );
	lua_checkstack( L, 10240 );
	return true;
}

bool LuaState::isExistFunction( const char* name ) const
{
	lua_getglobal( L, name );
	if( lua_type( L, -1 ) == LUA_TFUNCTION )
	{
		lua_pop( L, 1 );
		return true;
	}
	lua_pop( L, 1 );
	return false;
}

bool LuaState::doFile( const char* filename )
{
	int status = luaL_dofile( L, filename );
	if( status != 0 )
	{
		log_error( "load [%s] failed, error:[%s]", filename, lua_tostring( L, -1 ) );
		lua_pop( L, 1 );	///< 弹出错误信息
		return false;
	}
	return true;
}

unsigned int LuaState::getGlobalNumber( const char* name ) const
{
	if( NULL == name )
		return 0;
	lua_getglobal( L, name );
	if( lua_isnumber( L, -1 ) )
	{
		unsigned int ret = ( unsigned int )lua_tonumber( L, -1 );
		lua_settop( L, -2 );
		return ret;
	}
	return 0;
}

const char* LuaState::getGlobalString( const char* name ) const
{
	if( NULL == name )
		return "";
	lua_getglobal( L, name );
	if( lua_isstring( L, -1 ) )
	{
		const char* ret = ( const char* )lua_tostring( L, -1 );
		lua_settop( L, -2 );
		return ret;
	}
	return "";
}

void LuaState::regFunc( const char* funcname, lua_CFunction func )
{
	if( NULL == funcname )
		return;
	lua_pushstring( L, funcname );
	lua_pushcclosure( L, func, 0 );
	lua_settable( L, LUA_GLOBALSINDEX );
}


bool LuaState::callRTBool( const char* func )
{
	if( isExistFunction( func ) )
	{
		lua_getglobal( L, func );
		if( lua_pcall( L, 0, 0, 0 ) )
		{
			log_error( "function[%s],ErrorMsg[%s]", func, lua_tostring( L, -1 ) );
			lua_pop( L, 1 );	///< 弹出错误信息
			return false;
		}
		return true;
	}
	log_error( "function[%s] does not exist!", func );
	return false;
}

void LuaState::callRTVoid( const char* func )
{
	if( isExistFunction( func ) )
	{
		lua_getglobal( L, func );
		if( lua_pcall( L, 0, 0, 0 ) )
		{
			log_error( "function[%s],ErrorMsg[%s]", func, lua_tostring( L, -1 ) );
			lua_pop( L, 1 );	///< 弹出错误信息
		}
		return;
	}
    log_error("function[%s] does not exist!", func);
}

void LuaState::pushValue( const ScriptProxyBase* script )
{
	if (NULL == script)
	{
		return;
	}

	lua_getref( L, script->getRef() );
}