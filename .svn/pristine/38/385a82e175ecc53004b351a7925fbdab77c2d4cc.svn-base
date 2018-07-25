/**
 * @author 莫鸿全
 * @data   2011年10月9日 11时02分01秒
 * @brief  脚本代理对象
 *
 */

#pragma once

#include "LuaState.h"
#include "tolua++.h"
#include "log.hpp"


///< 基础脚本代理类
class ScriptProxyBase
{
protected:
	ScriptProxyBase( void* pthis, LuaState* lua_state, const char* name );
	virtual ~ScriptProxyBase();

public:
	/// @brief 销毁，减少引用lua中的引用计数
	void dispose();
	/// @brief 获得脚本引用
	int getRef() const;

	/// @brief 调用脚本函数
	void callRTVoid( const char* func );

	template< typename P1 >
	void callRTVoid( const char* func, const P1 p1 );

	template< typename P1, typename P2 >
	void callRTVoid( const char* func, const P1 p1, const P2 p2 );

	template< typename P1, typename P2, typename P3 >
	void callRTVoid( const char* func, const P1 p1, const P2 p2, const P3 p3 );

	template< typename P1, typename P2, typename P3, typename P4 >
	void callRTVoid( const char* func, const P1 p1, const P2 p2, const P3 p3, const P4 p4 );

	template< typename P1, typename P2, typename P3, typename P4, typename P5 >
	void callRTVoid( const char* func, const P1 p1, const P2 p2, const P3 p3, const P4 p4, const P5 p5 );

	template< typename P1, typename P2, typename P3, typename P4, typename P5, typename P6 >
	void callRTVoid( const char* func, const P1 p1, const P2 p2, const P3 p3, const P4 p4, const P5 p5, const P6 p6 );

	/// @brief 返回值为bool的函数调用
	bool callRTBool( const char* func );

	template< typename P1 >
	bool callRTBool( const char* func, const P1 p1 );

	template< typename P1, typename P2 >
	bool callRTBool( const char* func, const P1 p1, const P2 p2 );

	template< typename P1, typename P2, typename P3 >
	bool callRTBool( const char* func, const P1 p1, const P2 p2, const P3 p3 );

	template< typename P1, typename P2, typename P3, typename P4 >
	bool callRTBool( const char* func, const P1 p1, const P2 p2, const P3 p3, const P4 p4 );

	template< typename P1, typename P2, typename P3, typename P4, typename P5 >
	bool callRTBool( const char* func, const P1 p1, const P2 p2, const P3 p3, const P4 p4, const P5 p5 );

	template< typename P1, typename P2, typename P3, typename P4, typename P5, typename P6 >
	bool callRTBool( const char* func, const P1 p1, const P2 p2, const P3 p3, const P4 p4, const P5 p5, const P6 p6 );

	///< 封装c++调用lua返回值（参数不同自己加）
	template< typename P1 >
	uint32_t callRTUINT(const char* func, const P1 p1);

	template< typename P1, typename P2 >
	uint32_t callRTUINT(const char* func, const P1 p1, const P2 p2);

protected:
	LuaState*	L_state;	///< lua虚拟机
	int			m_ref;			///< lua中对应table的引用
};


///< 脚本行为结构
struct ScriptActionItem
{
	ScriptActionItem()
	{
		bzero( this, sizeof( *this ) );
	}

	ScriptActionItem( 
		const char cleartype, 
		const int nexttime,
		const int interval, 
		const int maxtimes,
		const int actionid )
		: cleartype( cleartype )
		, nexttime( nexttime )
		, actiontimes( 0 )
		, interval( interval )
		, maxtimes( maxtimes )
		, actionid( actionid )
		, needdelete( false )
	{
		
	}

	ScriptActionItem( const ScriptActionItem& item )
	{
		memcpy( this, &item, sizeof( *this ) );
	}
	char cleartype;		///< 清除类型
	int nexttime;		///< 下次执行时间
	int actiontimes;	///< 已经执行次数
	int interval;		///< 时间间隔，为0表示只执行一次
	int maxtimes;		///< 最多执行次数，为0表示没有限制
	int actionid;		///< 脚本函数ID
	bool needdelete;	///< 是否需要删除
};

///=============================================
///		@brief 脚本对象代理
///=============================================
template< typename ProxyT >
class ScriptProxy : public ScriptProxyBase
{
protected:
	ScriptProxy( ProxyT* me, void* pthis, LuaState* lua_state, const char* name );
	~ScriptProxy();

public:
	void dispose();
	ProxyT* getOwner();
	const ProxyT* getOwner() const;

	///< @brief 脚本循环
	void scriptLoop( const unsigned int now );
	///< @brief 清除注册的脚本动作
	void clearScriptAction( const char cleartype );
	///< @brief 注册脚本执行函数，至少执行一次
	void registerScriptAction( 
		const char cleartype, 
		const int delaytime, 
		const int interval, 
		const int maxtimes, 
		const int actionid,
		const int now );

protected:
	ProxyT*	m_owner;

	std::list< ScriptActionItem > m_script_action_list;

};


///=============================================
///		@brief ScriptProxyBase 内联函数
///=============================================
inline int ScriptProxyBase::getRef() const
{
	return m_ref;
}

template< typename P1 >
void ScriptProxyBase::callRTVoid( const char* func, const P1 p1 )
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

	this->L_state->pushValue( p1 );

	if( lua_pcall( lua, 2, 0, 0 ) )
	{
		log_error( "脚本错误:callRTVoid[%s],ErrorMsg[%s]", func, lua_tostring( lua, -1 ) );
		lua_pop( lua, 1 );		///< 弹出错误信息
	}	
}

template< typename P1, typename P2 >
void ScriptProxyBase::callRTVoid( const char* func, const P1 p1, const P2 p2 )
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

	this->L_state->pushValue( p1 );
	this->L_state->pushValue( p2 );

	if( lua_pcall( lua, 3, 0, 0 ) )
	{
		log_error( "脚本错误:callRTVoid[%s],ErrorMsg[%s]", func, lua_tostring( lua, -1 ) );
		lua_pop( lua, 1 );		///< 弹出错误信息
	}		
}

template< typename P1, typename P2, typename P3 >
void ScriptProxyBase::callRTVoid( const char* func, const P1 p1, const P2 p2, const P3 p3 )
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

	this->L_state->pushValue( p1 );
	this->L_state->pushValue( p2 );
	this->L_state->pushValue( p3 );

	if( lua_pcall( lua, 4, 0, 0 ) )
	{
		log_error( "脚本错误:callRTVoid[%s],ErrorMsg[%s]", func, lua_tostring( lua, -1 ) );
		lua_pop( lua, 1 );		///< 弹出错误信息
	}	
}

template< typename P1, typename P2, typename P3, typename P4 >
void ScriptProxyBase::callRTVoid( const char* func, const P1 p1, const P2 p2, const P3 p3, const P4 p4 )
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

	this->L_state->pushValue( p1 );
	this->L_state->pushValue( p2 );
	this->L_state->pushValue( p3 );
	this->L_state->pushValue( p4 );

	if( lua_pcall( lua, 5, 0, 0 ) )
	{
		log_error( "脚本错误:callRTVoid[%s],ErrorMsg[%s]", func, lua_tostring( lua, -1 ) );
		lua_pop( lua, 1 );		///< 弹出错误信息
	}		
}

template< typename P1, typename P2, typename P3, typename P4, typename P5 >
void ScriptProxyBase::callRTVoid( const char* func, const P1 p1, const P2 p2, const P3 p3, const P4 p4, const P5 p5 )
{
	if (NULL == L_state)
	{
		log_error("L_state is null");
		return;
	}

	lua_State* lua = L_state->getHandle();
	if( lua_checkstack( lua, 12 ) == 0 )
	{
		log_error( "lua_checkstack run failed, system problem..." );
		return;
	}

	lua_getref( lua, this->m_ref );
	lua_getfield( lua, -1, func );
	lua_insert( lua, -2 );

	this->L_state->pushValue( p1 );
	this->L_state->pushValue( p2 );
	this->L_state->pushValue( p3 );
	this->L_state->pushValue( p4 );
	this->L_state->pushValue( p5 );

	if( lua_pcall( lua, 6, 0, 0 ) )
	{
		log_error( "脚本错误:callRTVoid[%s],ErrorMsg[%s]", func, lua_tostring( lua, -1 ) );
		lua_pop( lua, 1 );		///< 弹出错误信息
	}	
}

template< typename P1, typename P2, typename P3, typename P4, typename P5, typename P6 >
void ScriptProxyBase::callRTVoid( const char* func, const P1 p1, const P2 p2, const P3 p3, const P4 p4, const P5 p5, const P6 p6 )
{
	if (NULL == L_state)
	{
		log_error("L_state is null");
		return;
	}

	lua_State* lua = L_state->getHandle();
	if( lua_checkstack( lua, 12 ) == 0 )
	{
		log_error( "lua_checkstack run failed, system problem..." );
		return;
	}

	lua_getref( lua, this->m_ref );
	lua_getfield( lua, -1, func );
	lua_insert( lua, -2 );

	this->L_state->pushValue( p1 );
	this->L_state->pushValue( p2 );
	this->L_state->pushValue( p3 );
	this->L_state->pushValue( p4 );
	this->L_state->pushValue( p5 );
	this->L_state->pushValue( p6 );

	if( lua_pcall( lua, 7, 0, 0 ) )
	{
		log_error( "脚本错误:callRTVoid[%s],ErrorMsg[%s]", func, lua_tostring( lua, -1 ) );
		lua_pop( lua, 1 );		///< 弹出错误信息
	}	
}

template< typename P1 >
bool ScriptProxyBase::callRTBool( const char* func, const P1 p1 )
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

	this->L_state->pushValue( p1 );

	if( lua_pcall( lua, 2, 1, 0 ) )
	{
		log_error( "脚本错误:callRTVoid[%s],ErrorMsg[%s]", func, lua_tostring( lua, -1 ) );
		lua_pop( lua, 1 );		///< 弹出错误信息
		return false;
	}	
	int bret = lua_toboolean( lua, -1 );
	lua_pop( lua, 1 );
	return bret ? true : false;
}

template< typename P1, typename P2 >
bool ScriptProxyBase::callRTBool( const char* func, const P1 p1, const P2 p2 )
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

	this->L_state->pushValue( p1 );
	this->L_state->pushValue( p2 );

	if( lua_pcall( lua, 3, 1, 0 ) )
	{
		log_error( "脚本错误:callRTVoid[%s],ErrorMsg[%s]", func, lua_tostring( lua, -1 ) );
		lua_pop( lua, 1 );		///< 弹出错误信息
		return false;
	}	
	int bret = lua_toboolean( lua, -1 );
	lua_pop( lua, 1 );
	return bret ? true : false;	
}

template< typename P1, typename P2, typename P3 >
bool ScriptProxyBase::callRTBool( const char* func, const P1 p1, const P2 p2, const P3 p3 )
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

	this->L_state->pushValue( p1 );
	this->L_state->pushValue( p2 );
	this->L_state->pushValue( p3 );

	if( lua_pcall( lua, 4, 1, 0 ) )
	{
		log_error( "脚本错误:callRTVoid[%s],ErrorMsg[%s]", func, lua_tostring( lua, -1 ) );
		lua_pop( lua, 1 );		///< 弹出错误信息
		return false;
	}	
	int bret = lua_toboolean( lua, -1 );
	lua_pop( lua, 1 );
	return bret ? true : false;		
}

template< typename P1, typename P2, typename P3, typename P4 >
bool ScriptProxyBase::callRTBool( const char* func, const P1 p1, const P2 p2, const P3 p3, const P4 p4 )
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

	this->L_state->pushValue( p1 );
	this->L_state->pushValue( p2 );
	this->L_state->pushValue( p3 );
	this->L_state->pushValue( p4 );

	if( lua_pcall( lua, 5, 1, 0 ) )
	{
		log_error( "脚本错误:callRTVoid[%s],ErrorMsg[%s]", func, lua_tostring( lua, -1 ) );
		lua_pop( lua, 1 );		///< 弹出错误信息
		return false;
	}	
	int bret = lua_toboolean( lua, -1 );
	lua_pop( lua, 1 );
	return bret ? true : false;			
}

template< typename P1, typename P2, typename P3, typename P4, typename P5 >
bool ScriptProxyBase::callRTBool( const char* func, const P1 p1, const P2 p2, const P3 p3, const P4 p4, const P5 p5 )
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

	this->L_state->pushValue( p1 );
	this->L_state->pushValue( p2 );
	this->L_state->pushValue( p3 );
	this->L_state->pushValue( p4 );
	this->L_state->pushValue( p5 );

	if( lua_pcall( lua, 6, 1, 0 ) )
	{
		log_error( "脚本错误:callRTVoid[%s],ErrorMsg[%s]", func, lua_tostring( lua, -1 ) );
		lua_pop( lua, 1 );		///< 弹出错误信息
		return false;
	}	
	int bret = lua_toboolean( lua, -1 );
	lua_pop( lua, 1 );
	return bret ? true : false;	
}

template< typename P1, typename P2, typename P3, typename P4, typename P5, typename P6 >
bool ScriptProxyBase::callRTBool( const char* func, const P1 p1, const P2 p2, const P3 p3, const P4 p4, const P5 p5, const P6 p6 )
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

	this->L_state->pushValue( p1 );
	this->L_state->pushValue( p2 );
	this->L_state->pushValue( p3 );
	this->L_state->pushValue( p4 );
	this->L_state->pushValue( p5 );
	this->L_state->pushValue( p6 );

	if( lua_pcall( lua, 7, 1, 0 ) )
	{
		log_error( "脚本错误:callRTVoid[%s],ErrorMsg[%s]", func, lua_tostring( lua, -1 ) );
		lua_pop( lua, 1 );		///< 弹出错误信息
		return false;
	}	
	int bret = lua_toboolean( lua, -1 );
	lua_pop( lua, 1 );
	return bret ? true : false;	
}

template< typename P1 >
uint32_t ScriptProxyBase::callRTUINT(const char* func, const P1 p1)
{
	if (NULL == L_state)
	{
		log_error("L_state is null");
		return 0;
	}

	lua_State* lua = L_state->getHandle();
	if (lua_checkstack(lua, 10) == 0)
	{
		log_error("lua_checkstack run failed, system problem...");
		return 0;
	}

	lua_getref(lua, this->m_ref);
	lua_getfield(lua, -1, func);
	lua_insert(lua, -2);

	this->L_state->pushValue(p1);

	if (lua_pcall(lua, 2, 1, 0))
	{
		log_error("脚本错误:callRTVoid[%s],ErrorMsg[%s]", func, lua_tostring(lua, -1));
		lua_pop(lua, 1);		///< 弹出错误信息
		return 0;
	}

	if (lua_isnumber(lua, -1))
	{
		uint32_t ret = static_cast<uint32_t>(lua_tonumber(lua, -1));
		lua_pop(lua, 1);
		return ret;
	}

	return 0;
}

template< typename P1, typename P2 >
uint32_t ScriptProxyBase::callRTUINT(const char* func, const P1 p1, const P2 p2)
{
	if (NULL == L_state)
	{
		log_error("L_state is null");
		return 0;
	}

	lua_State* lua = L_state->getHandle();
	if (lua_checkstack(lua, 10) == 0)
	{
		log_error("lua_checkstack run failed, system problem...");
		return 0;
	}

	lua_getref(lua, this->m_ref);
	lua_getfield(lua, -1, func);
	lua_insert(lua, -2);

	this->L_state->pushValue(p1);
	this->L_state->pushValue(p2);

	if (lua_pcall(lua, 3, 1, 0))
	{
		log_error("脚本错误:callRTVoid[%s],ErrorMsg[%s]", func, lua_tostring(lua, -1));
		lua_pop(lua, 1);		///< 弹出错误信息
		return false;
	}

	if (lua_isnumber(lua, -1))
	{
		uint32_t ret = static_cast<uint32_t>(lua_tonumber(lua, -1));
		lua_pop(lua, 1);
		return ret;
	}

	return 0;
}

///=============================================
///		@brief ScriptProxy 内联函数
///=============================================

template< typename ProxyT >
ScriptProxy< ProxyT >::ScriptProxy( ProxyT* me, void* pthis, LuaState* lua_state, const char* name )
	: ScriptProxyBase( pthis, lua_state, name )
	, m_owner( me )
{
	
}

template< typename ProxyT >
ScriptProxy< ProxyT >::~ScriptProxy()
{
	m_owner = NULL;
}


template< typename ProxyT >
void ScriptProxy< ProxyT >::dispose()
{
	ScriptProxyBase::dispose();
	m_owner = NULL;
}

template< typename ProxyT >
inline ProxyT* ScriptProxy< ProxyT >::getOwner()
{
	return m_owner;
}

template< typename ProxyT >
inline const ProxyT* ScriptProxy< ProxyT >::getOwner() const
{
	return m_owner;
}

///< @brief 脚本循环
template< typename ProxyT >
inline void ScriptProxy< ProxyT >::scriptLoop( const unsigned int now )
{
	if( m_script_action_list.empty() )
		return;
	std::list< ScriptActionItem >::iterator it = m_script_action_list.begin();
	while( it != m_script_action_list.end() )
	{
		if( it->needdelete == true )
		{
			it = m_script_action_list.erase( it );
			continue;
		}

		if( it->nexttime <= ( int )now )
		{
			++it->actiontimes;
			bool end = false;
			if( it->interval == 0 || ( it->maxtimes > 0 && it->maxtimes == it->actiontimes ) )
			{
				end = true;
			}
			this->callRTVoid( "fireAction", it->actionid, end );
			if( end )
			{
				it = m_script_action_list.erase( it );
				continue;
			}
			it->nexttime = now + it->interval;
		}
		++it;
	}
}


///< @brief 清除注册的脚本动作
template< typename ProxyT >
inline void ScriptProxy< ProxyT >::clearScriptAction( const char cleartype )
{
	std::list< ScriptActionItem >::iterator it;
	for( it = m_script_action_list.begin(); it != m_script_action_list.end(); ++it )
	{
		if( it->cleartype == cleartype && it->needdelete == false )
		{
			this->callRTVoid( "clearAction", it->actionid );
			it->needdelete = true;
		}		
	}
}


///< @brief 注册脚本执行函数，至少执行一次
template< typename ProxyT >
inline void ScriptProxy< ProxyT >::registerScriptAction( 
	const char cleartype, 
	const int delaytime, 
	const int interval, 
	const int maxtimes, 
	const int actionid,
	const int now )
{
	int nexttime = delaytime + now;
	m_script_action_list.push_back( ScriptActionItem( cleartype, nexttime, interval, maxtimes, actionid ) );
}
