#ifndef __GAME_SYS_NOTICE_MANAGER_H__
#define __GAME_SYS_NOTICE_MANAGER_H__

#include "macros.hpp"

#include "tblh/sys_notice_enum.hpp"
#include "sys_notice.hpp"
#include "role/role.hpp"
#include "config_mgr.h"
#include "tblh/SysNoticeTable.tbls.h"
#include "StringTool.h"
#include "utility.hpp"
#include "tblh/LanguageTable.tbls.h"
#include "tblh/SysChannel.tbls.h"
#include "role/role_manager.hpp"
#include <vector>

enum IS_NOTIFY_TYPE : uint32_t
{
	IS_NOTIFY_TYPE_0 = 0,	//不广播 通知自己
	IS_NOTIFY_TYPE_1 = 1,	//广播 所有人
	IS_NOTIFY_TYPE_2 = 2,	//不用处理 客户端自己处理
	IS_NOTIFY_TYPE_3 = 3,	//家族通知
};

class sys_notice_manager_t
{
	// 初始通知信息，上线就发
protected:
	static proto::client::gc_sys_init_notice_notify init_notify;

public:
	static bool init();
	static bool reload();
	static void close();

public:
	static void on_role_login(role_ptr p_role);

	static void broadcast_system_notice(SysNoticeTable* p_table, uint64_t uid, uint64_t family_id, uint64_t country_id, const std::string& text);

	static void broadcast_system_notice_to_scene(const scene_ptr& p_scene, SysNoticeTable* p_table, uint64_t uid, uint64_t family_id, uint64_t country_id, const std::string& text);

	//////////////////////////////////////////////////////////////////////////
	///新加的系统频道
	static void broadcast_system_notice(SysNoticeTable* p_table, uint64_t uid, const std::string& text, uint32_t label = 0);

	static void broadcast_system_notice(uint64_t uid, const std::string& text, uint32_t label = 0);

	static bool broadcast_system_msg_for_money(const uint64_t role_uid, proto::common::MONEY_TYPE type, uint32_t param, uint32_t param_id = 0);

	static uint32_t get_broadcast_count(uint32_t type,uint32_t param);

	static bool update_broadcast_count(uint32_t type, uint32_t param);

	static bool update_sys_notify_from_center(const proto::common::sys_notity_single& single,bool is_from_center = true);

	static bool load_sys_notify_from_center(const proto::common::sys_notity_data& data);

	static sys_notice_ptr get_sys_notice(uint32_t type, uint32_t param);

	static void clear_notice_list();

	static void gm_clear_notify_to_center();

	static void sys_notice_notify(uint64_t uid, proto::common::sys_notify_type type,uint32_t param_id,std::vector<std::string>& str_list);
};

#define BROADCAST_SYS_NOTICE(id, uid, family_id, country_id, args...) \
	SysNoticeTable* p_notice = GET_CONF(SysNoticeTable, id); \
	if (NULL != p_notice && !p_notice->text().empty()) \
	{ \
		std::string notice_text; \
		StringTool::format(notice_text, common::string_util_t::convert_to_utf8(p_notice->text()).c_str(), ##args); \
		sys_notice_manager_t::broadcast_system_notice(p_notice, uid, family_id, country_id, notice_text); \
	} 

#define BROADCAST_SCENE_NOTICE(scene, id, uid, family_id, country_id, args...) \
	SysNoticeTable* p_notice = GET_CONF(SysNoticeTable, id); \
	if (NULL != p_notice && !p_notice->text().empty()) \
	{ \
		std::string notice_text; \
		StringTool::format(notice_text, common::string_util_t::convert_to_utf8(p_notice->text()).c_str(), ##args); \
		sys_notice_manager_t::broadcast_system_notice_to_scene(scene, p_notice, uid, family_id, country_id, notice_text); \
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

#define BROADCAST_SYS_NOTICE_MY_SELF(id, uid,label,type,param_id, args...) \
	LanguageTable* p_notice = GET_CONF(LanguageTable, id); \
	if (NULL != p_notice && !p_notice->str().empty()) \
	{ \
		std::string notice_text; \
		StringTool::format(notice_text, common::string_util_t::convert_to_utf8(p_notice->str()).c_str(), ##args); \
		sys_notice_manager_t::broadcast_system_notice(uid, notice_text,label); \
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
		else if(IS_NOTIFY_TYPE_0 == p_config->is_notify()){ \
			BROADCAST_SYS_NOTICE_MY_SELF(p_config->language_id(),uid,p_config->label(),type, param_id,##args); \
		} \
	}



typedef std::map<uint32_t, sys_notice_ptr> sys_notice_map;
static sys_notice_map m_sys_notice_map;
#endif