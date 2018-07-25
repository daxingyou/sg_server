#ifndef __CENTER_SYS_NOTICE_MANAGER_H__
#define __CENTER_SYS_NOTICE_MANAGER_H__

#include "macros.hpp"

#include "tblh/sys_notice_enum.hpp"
#include "sys_notice.hpp"
#include "config_mgr.h"
#include "tblh/SysNoticeTable.tbls.h"
#include "StringTool.h"
#include "utility.hpp"
#include "tblh/LanguageTable.tbls.h"
#include "user/global_user_manager.hpp"
#include "tblh/SysChannel.tbls.h"
#include <unordered_map>

enum IS_NOTIFY_TYPE : uint32_t
{
	IS_NOTIFY_TYPE_0 = 0,	//不广播 通知自己
	IS_NOTIFY_TYPE_1 = 1,	//广播 所有人
	IS_NOTIFY_TYPE_2 = 2,	//不用处理 客户端自己处理
};

class sys_notice_manager_t
{
public:
	static void broadcast_system_notice(SysNoticeTable* p_table, uint64_t uid, uint64_t family_id, uint64_t country_id, const std::string& text);
	
	//////////////////////////////////////////////////////////////////////////
	///新加的系统频道
	static void broadcast_system_notice(SysNoticeTable* p_table, uint64_t uid, const std::string& text, uint32_t label = 0);
	static void broadcast_system_notice(uint64_t uid, const std::string& text, uint32_t label = 0);
	static bool update_limit_count(const proto::common::sys_notity_single& single,bool is_from_game = true);

	static sys_notice_ptr get_sys_notice(uint32_t type,uint32_t param);

	static bool	handle_ge_get_sys_notice_list_request(const network::tcp_socket_ptr& s);

	static void update_broadcast_count(uint32_t type, uint32_t param);

	static uint32_t get_broadcast_count(uint32_t type, uint32_t param);

	static void clear_notice_list();

	template<typename T_MSG>
	static void broadcast_msg_to_all(uint16_t cmd, const T_MSG& msg)
	{
		std::string cmd_data;
		network::wrap_msg(cmd_data, msg);

		proto::server::ea_broadcast_notify msg_to_gate;
		msg_to_gate.set_cmd(cmd);
		msg_to_gate.set_data(cmd_data);

		env::server->send_msg_to_all_gate(op_cmd::ea_broadcast_notify, 0, msg_to_gate);
	}
};

#define BROADCAST_SYS_NOTICE(id, uid, family_id, country_id, args...) \
	SysNoticeTable* p_notice = GET_CONF(SysNoticeTable, id); \
	if (NULL != p_notice && !p_notice->text().empty()) \
	{ \
		std::string notice_text; \
		StringTool::format(notice_text, common::string_util_t::convert_to_utf8(p_notice->text()).c_str(), ##args); \
		sys_notice_manager_t::broadcast_system_notice(p_notice, uid, family_id, country_id, notice_text); \
	} 

//////////////////////////////////////////////////////////////////////////
#define BROADCAST_SYS_NOTICE_ALL(id, uid,label,type,param_id, args...) \
	SysNoticeTable* p_notice = GET_CONF(SysNoticeTable, id); \
	if (NULL != p_notice && !p_notice->text().empty()) \
	{ \
		std::string notice_text; \
		StringTool::format(notice_text, common::string_util_t::convert_to_utf8(p_notice->text()).c_str(), ##args); \
		sys_notice_manager_t::broadcast_system_notice(p_notice, uid, notice_text,label); \
		sys_notice_manager_t::update_broadcast_count(type,param_id); \
	}

#define BROADCAST_SYS_NOTICE_MY_SELF(id, uid,label,type,param_id ,args...) \
	LanguageTable* p_notice = GET_CONF(LanguageTable, id); \
	if (NULL != p_notice && !p_notice->str().empty()) \
	{ \
		std::string notice_text; \
		StringTool::format(notice_text, common::string_util_t::convert_to_utf8(p_notice->str()).c_str(), ##args); \
		sys_notice_manager_t::broadcast_system_notice(uid, notice_text,label); \
		sys_notice_manager_t::update_broadcast_count(type,param_id); \
	} 

#define BROADCAST_SYS_NOTICE_SYS_CHANNEL(uid,type,param_id, args...) \
	SysChannel* p_config = GET_CONF(SysChannel, hash_util_t::hash_to_uint32((uint32_t)type, param_id)); \
	if (NULL != p_config && IS_NOTIFY_TYPE_2 != p_config->is_notify()) \
	{ \
		uint32_t count = sys_notice_manager_t::get_broadcast_count(type,param_id); \
		if (IS_NOTIFY_TYPE_1 == p_config->is_notify()) \
		{ \
			if (0 == p_config->count()) \
			{ \
				BROADCAST_SYS_NOTICE_ALL(p_config->notify_id(), uid, p_config->label(), type, param_id, ##args); \
			}else{ \
				if (count <= p_config->count()) \
				{ \
					BROADCAST_SYS_NOTICE_ALL(p_config->notify_id(), uid, p_config->label(), type, param_id, ##args); \
				}else \
				{ \
					BROADCAST_SYS_NOTICE_MY_SELF(p_config->language_id(),uid,p_config->label(),type, param_id,##args); \
				} \
			} \
		} \
		else{ \
			BROADCAST_SYS_NOTICE_MY_SELF(p_config->language_id(),uid,p_config->label(),type, param_id,##args); \
		} \
	}


typedef std::map<uint32_t, sys_notice_ptr> sys_notice_map;
static sys_notice_map m_sys_notice_map;

#endif