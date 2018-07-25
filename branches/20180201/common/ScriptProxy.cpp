/**
 * @author 莫鸿全
 * @data   2011年10月9日 11时02分01秒
 * @brief  脚本代理对象
 *
 */

#include "ScriptProxy.h"

ScriptProxyBase::ScriptProxyBase( void* pthis, LuaState* lua_state, const char* name )
	: L_state( lua_state )
	, m_ref( LUA_NOREF )
{
	if (NULL == L_state)
	{
		log_error("L_state is null");
		return;
	}

	tolua_pushusertype_and_takeownership(L_state->getHandle(), pthis, name);
	m_ref = lua_ref(L_state->getHandle(), true);
	assert( ( m_ref != LUA_REFNIL ) && ( m_ref != LUA_NOREF ) );
}

ScriptProxyBase::~ScriptProxyBase()
{

}

void ScriptProxyBase::dispose()
{
	if (NULL == L_state)
	{
		log_error("L_state is null");
		return;
	}

	assert( ( m_ref != LUA_REFNIL ) && ( m_ref != LUA_NOREF ) );
	lua_unref( L_state->getHandle(), m_ref );
}

void ScriptProxyBase::callRTVoid( const char* func )
{
	if (NULL == L_state)
	{
		log_error("L_state is null");
		return;
	}

	lua_State* lua = L_state->getHandle();
	if( lua_checkstack( lua, 10 ) == 0 )
	{
		log_error( "lua_checkstack run failed, system problem..." );
		return;
	}

	lua_getref( lua, this->m_ref );
	lua_getfield( lua, -1, func );
	lua_insert( lua, -2 );

	if( lua_pcall( lua, 1, 0, 0 ) )
	{
		log_error( "callRTVoid[%s],ErrorMsg[%s]", func, lua_tostring( lua, -1 ) );
		lua_pop( lua, 1 );		///< 弹出错误信息
	}
}

bool ScriptProxyBase::callRTBool( const char* func )
{
	if (NULL == L_state)
	{
		log_error("L_state is null");
		return false;
	}

	lua_State* lua = L_state->getHandle();
	if( lua_checkstack( lua, 10 ) == 0 )
	{
		log_error( "lua_checkstack run failed, system problem..." );
		return false;
	}

	lua_getref( lua, this->m_ref );
	lua_getfield( lua, -1, func );
	lua_insert( lua, -2 );

	if( lua_pcall( lua, 1, 1, 0 ) )
	{
		log_error( "callRTVoid[%s],ErrorMsg[%s]", func, lua_tostring( lua, -1 ) );
		lua_pop( lua, 1 );		///< 弹出错误信息
		return false;
	}	
	int bret = lua_toboolean( lua, -1 );
	lua_pop( lua, 1 );
	return bret ? true : false;
}