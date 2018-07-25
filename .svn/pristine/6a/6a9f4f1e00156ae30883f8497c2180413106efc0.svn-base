#include "utility.hpp"
#include <math.h>
#include <vector>

#include "gm_cmd_opt.hpp"
#include "common/log_enum.hpp"
#include "item/item_manager.hpp"
#include "log_enum.hpp"
#include "hero/hero_manager.hpp"
#include "message.hpp"
#include "role/role_manager.hpp"
#include "common/ScriptManager.hpp"
#include "script/game_script_func.h"
#include "config/config_manager.hpp"
#include "task/task_manager.hpp"
#include "fight/fight_manager.hpp"
#include "role/money_manager.hpp"
#include "mount/mount_manager.hpp"
#include "monster/monster.hpp"
#include "scene/scene_manager.hpp"
#include "scene/scene.hpp"
#include "item/drop_manager.hpp"
#include "common/DataManager.h"
#include "tblh/errcode_enum.hpp"
#include "mail/mail_manager.hpp"
#include "tblh/MailTable.tbls.h"
#include "arena/arena_manager.hpp"
#include "sys_notice/sys_notice_manager.hpp"
#include "center/center_msg_handler.hpp"
#include "king_war/king_war_manager.hpp"
#include "dungeon/dungeon_manager.hpp"
#include "tblh/HeroPlugin.tbls.h"
#include "common/utility.hpp"
#include "log/log_wrapper.hpp"
// #include "protobuf.hpp"
#include "tblh/Hero.tbls.h"
#include "sys_notice/sys_notice_manager.hpp"
#include "page/page_manager.hpp"
#include "business_activity/business_activity_manager.hpp"

#include "achieve/achieve_common.hpp"

USING_NS_NETWORK;
USING_NS_COMMON;



bool check_gm_param(std::string str) 
{
    if (str.size() >= 10)
    {
        return false;
    }
    for( int i = 0; i < int(str.size()); ++i) 
    {   
        if ((str.at(i) > '9') || (str.at(i) < '0') )
        { 
            return false;
        } 
    } 
	return true;
} 
#define CHECK_GM_STR(str) if (!check_gm_param(str)) { return; }

void gm_cmd_opt_t::send_opt_reply_msg(role_ptr p_role, const proto::client::gc_gm_cmd_reply& reply_msg)
{
	FUNC_CHECK_START
    if (p_role)
    {
		p_role->send_msg_to_client(op_cmd::gc_gm_cmd_reply, reply_msg);
	}

	FUNC_CHECK_END
}

void gm_cmd_opt_t::opt_add_item(role_ptr p_role, std::vector<std::string>& gm_cmd_list, proto::common::role_change_data* p_data)
{
	FUNC_CHECK_START
	if (NULL == p_role)
	{
		log_error("NULL == p_role");
		return;
	}

	int32_t gm_cmd_size = 3;
	if ((int32_t)(uint32_t)gm_cmd_list.size() != gm_cmd_size)
	{
		log_error("cmd_list.size()[%d] != %d", (uint32_t)gm_cmd_list.size(), gm_cmd_size);
		return;
	}
	if (!check_gm_param(gm_cmd_list[1]))
	{
		return;
	}
	uint32_t item_tid = boost::lexical_cast<uint32_t>(gm_cmd_list[1]);
	if (!check_gm_param(gm_cmd_list[2]))
	{
		return;
	}
	uint32_t add_num = boost::lexical_cast<uint32_t>(gm_cmd_list[2]);
	item_manager_t::add_item(p_role, item_tid, add_num, log_enum::source_type_gm_create_item, 0, p_data);

	FUNC_CHECK_END
}

void gm_cmd_opt_t::opt_set_level(role_ptr p_role, std::vector<std::string>& gm_cmd_list, proto::common::role_change_data* p_data)
{
	FUNC_CHECK_START
	if (NULL == p_role)
	{
		log_error("NULL == p_role");
		return;
	}

	int32_t gm_cmd_size = 2;
	if ((int32_t)(uint32_t)gm_cmd_list.size() != gm_cmd_size)
	{
		log_error("cmd_list.size()[%d] != %d", (uint32_t)gm_cmd_list.size(), gm_cmd_size);
		return;
	}

	if (!check_gm_param(gm_cmd_list[1]))
	{
		return;
	}

	uint32_t level = boost::lexical_cast<uint32_t>(gm_cmd_list[1]);
	uint32_t old_level = p_role->get_level();
	p_role->set_level(level, log_enum::source_type_gm_set_level, 0, p_data);
	log_wrapper_t::send_role_exp_log(p_role->get_uid(), old_level, p_role->get_exp(), 0,
		p_role->get_level(), p_role->get_exp(), log_enum::source_type_gm_set_level, 0);

	p_role->check_send_level_reward_mail();

	FUNC_CHECK_END
}


void gm_cmd_opt_t::opt_add_exp(role_ptr p_role, std::vector<std::string>& gm_cmd_list, proto::common::role_change_data* p_data)
{
	FUNC_CHECK_START
	if (NULL == p_role)
	{
		log_error("NULL == p_role");
		return;
	}

	int32_t gm_cmd_size = 2;
	if ((int32_t)(uint32_t)gm_cmd_list.size() != gm_cmd_size)
	{
		log_error("cmd_list.size()[%d] != %d", (uint32_t)gm_cmd_list.size(), gm_cmd_size);
		return;
	}
	if (!check_gm_param(gm_cmd_list[1]))
	{
		return;
	}
	uint32_t exp = boost::lexical_cast<uint32_t>(gm_cmd_list[1]);
	p_role->add_exp(exp, log_enum::source_type_gm_add_exp, 0, p_data);

	FUNC_CHECK_END
}


void gm_cmd_opt_t::opt_add_prestige(role_ptr p_role, std::vector<std::string>& gm_cmd_list)
{
	FUNC_CHECK_START
	if (NULL == p_role)
	{
		log_error("NULL == p_role");
		return;
	}

	int32_t gm_cmd_size = 2;
	if ((int32_t)(uint32_t)gm_cmd_list.size() != gm_cmd_size)
	{
		log_error("cmd_list.size()[%d] != %d", (uint32_t)gm_cmd_list.size(), gm_cmd_size);
		return;
	}
	if (!check_gm_param(gm_cmd_list[1]))
	{
		return;
	}
	uint32_t val = boost::lexical_cast<uint32_t>(gm_cmd_list[1]);
	money_manager_t::add_money(p_role, proto::common::MONEY_TYPE_PRESTIGE, val, log_enum::source_type_gm_add_prestige, 0, true);

	FUNC_CHECK_END
}

void gm_cmd_opt_t::opt_add_money(role_ptr p_role, std::vector<std::string>& gm_cmd_list)
{
	FUNC_CHECK_START
	if (NULL == p_role)
	{
		log_error("NULL == p_role");
		return;
	}

	int32_t gm_cmd_size = 2;
	if ((int32_t)(uint32_t)gm_cmd_list.size() != gm_cmd_size)
	{
		log_error("cmd_list.size()[%d] != %d", (uint32_t)gm_cmd_list.size(), gm_cmd_size);
		return;
	}
	if (!check_gm_param(gm_cmd_list[1]))
	{
		return;
	}
	uint32_t money = boost::lexical_cast<uint32_t>(gm_cmd_list[1]);
	money_manager_t::add_money(p_role, proto::common::MONEY_TYPE_YUANBAO, money, log_enum::source_type_gm_add_money, 0);
	money_manager_t::add_money(p_role, proto::common::MONEY_TYPE_SILVER, money, log_enum::source_type_gm_add_money, 0);
	money_manager_t::add_money(p_role, proto::common::MONEY_TYPE_COPPER, money, log_enum::source_type_gm_add_money, 0);
	money_manager_t::add_money(p_role, proto::common::MONEY_TYPE_ENERGY, money, log_enum::source_type_gm_add_money, 0);
	money_manager_t::add_money(p_role, proto::common::MONEY_TYPE_LIFEFORCE, money, log_enum::source_type_gm_add_money, 0);
	money_manager_t::add_money(p_role, proto::common::MONEY_TYPE_HEROSOUL, money, log_enum::source_type_gm_add_money, 0);
	money_manager_t::add_money(p_role, proto::common::MONEY_TYPE_HONOR, money, log_enum::source_type_gm_add_money, 0);
	money_manager_t::add_money(p_role, proto::common::MONEY_TYPE_CONTRIBUTION, money, log_enum::source_type_gm_add_money, 0);
	money_manager_t::add_money(p_role, proto::common::MONEY_TYPE_ROLE_UNLOCK, money, log_enum::source_type_gm_add_money, 0);

	FUNC_CHECK_END
}

void gm_cmd_opt_t::opt_add_family_money(role_ptr p_role, std::vector<std::string>& gm_cmd_list)
{
	FUNC_CHECK_START
	if (NULL == p_role)
	{
		log_error("NULL == p_role");
		return;
	}

	if ((uint32_t)gm_cmd_list.size() != 2)
	{
		log_error("cmd_list.size()[%d] != 2", (uint32_t)gm_cmd_list.size());
		return;
	}

	CHECK_GM_STR(gm_cmd_list[1]);

	uint32_t money = boost::lexical_cast<uint32_t>(gm_cmd_list[1]);

	// 同步给中心服
	center_msg_handler_t::sync_data_to_center(p_role->get_uid(), proto::server::data_sync_type_family_money, money, true);

	FUNC_CHECK_END
}

void gm_cmd_opt_t::opt_king_me(role_ptr p_role, std::vector<std::string>& gm_cmd_list)
{
	FUNC_CHECK_START
	if (NULL == p_role)
	{
		log_error("NULL == p_role");
		return;
	}

    if((uint32_t)gm_cmd_list.size() != 1)
    {
        log_error("cmd_list.size()[%d] != %d", (uint32_t)gm_cmd_list.size(), 1);
        return;
    }
	// p_role->assign(p_role->get_name(), proto::common::officer_type_king, true);

	FUNC_CHECK_END
}

void gm_cmd_opt_t::opt_join_country(role_ptr p_role, std::vector<std::string>& gm_cmd_list)
{
	FUNC_CHECK_START
	if (NULL == p_role)
	{
		log_error("NULL == p_role");
		return;
	}

    if((uint32_t)gm_cmd_list.size() != 2)
    {
        log_error("cmd_list.size()[%d] != %d", (uint32_t)gm_cmd_list.size(), 2);
        return;
    }
	if (!check_gm_param(gm_cmd_list[1]))
	{
		return;
	}
	// p_role->join_country(boost::lexical_cast<uint32_t>(gm_cmd_list[1]));

	FUNC_CHECK_END
}

void gm_cmd_opt_t::opt_join_family(role_ptr p_role, std::vector<std::string>& gm_cmd_list)
{
	FUNC_CHECK_START
	if (NULL == p_role)
	{
		log_error("NULL == p_role");
		return;
	}

    if((uint32_t)gm_cmd_list.size() != 2)
    {
        log_error("cmd_list.size()[%d] != %d", (uint32_t)gm_cmd_list.size(), 2);
        return;
    }
    // auto type = proto::common::family_member_type_applay;
	if (!check_gm_param(gm_cmd_list[1]))
	{
		return;
	}
	// p_role->join_family(string_util_t::string_to_uint64(gm_cmd_list[1]), type);

	FUNC_CHECK_END
}


void gm_cmd_opt_t::opt_finish_task(role_ptr p_role, std::vector<std::string>& gm_cmd_list)
{
	FUNC_CHECK_START
	if (NULL == p_role)
	{
		log_error("NULL == p_role");
		return;
	}

    if((uint32_t)gm_cmd_list.size() != 2)
    {
        log_error("cmd_list.size()[%d] != %d", (uint32_t)gm_cmd_list.size(), 2);
        return;
    }
	if (!check_gm_param(gm_cmd_list[1]))
	{
		return;
	}
    auto p_task_mgr = p_role->get_task_mgr();
    if(NULL != p_task_mgr)
    {
        proto::common::role_change_data rcd;
		p_task_mgr->finish_task(string_util_t::string_to_uint64(gm_cmd_list[1]), rcd);
	}

	FUNC_CHECK_END
}


void gm_cmd_opt_t::opt_active_task(role_ptr p_role, std::vector<std::string>& gm_cmd_list)
{
	FUNC_CHECK_START
	if (NULL == p_role)
	{
		log_error("NULL == p_role");
		return;
	}

	if ((uint32_t)gm_cmd_list.size() != 2)
	{
		log_error("cmd_list.size()[%d] != %d", (uint32_t)gm_cmd_list.size(), 2);
		return;
	}
	if (!check_gm_param(gm_cmd_list[1]))
	{
		return;
	}
	auto p_task_mgr = p_role->get_task_mgr();
	if (NULL != p_task_mgr)
	{
		proto::common::role_change_data rcd;
		p_task_mgr->active_task_by_id(string_util_t::string_to_uint64(gm_cmd_list[1]), ACCEPT_TASK_BY_GMCMD, rcd);
	}

	FUNC_CHECK_END
}


void gm_cmd_opt_t::opt_finish_all_task(role_ptr p_role)
{
	FUNC_CHECK_START
		if (NULL == p_role)
		{
			log_error("NULL == p_role");
			return;
		}

	auto p_task_mgr = p_role->get_task_mgr();
	if (NULL == p_task_mgr)
	{
		log_error("NULL == p_role");
		return;
	}
	p_task_mgr->finish_all_task();
	FUNC_CHECK_END
}

void gm_cmd_opt_t::opt_approve_join_family(role_ptr p_role, std::vector<std::string>& gm_cmd_list)
{
	FUNC_CHECK_START
	// if (NULL == p_role)
	// {
	// 	log_error("NULL == p_role");
	// 	return;
	// }

    // if((uint32_t)gm_cmd_list.size() != 2)
    // {
    //     log_error("cmd_list.size()[%d] != %d", (uint32_t)gm_cmd_list.size(), 2);
    //     return;
    // }
    // proto::client::cg_approve_join_family_request req;
	// if (!check_gm_param(gm_cmd_list[1]))
	// {
	// 	return;
	// }
    // req.set_role_id((gm_cmd_list[1].c_str()));
    // size_t body_sz = req.ByteSize();
    // network::msg_buf_ptr out(new network::msg_buf_t(sizeof(uint64_t) + body_sz));
    // char* p = out->begin();

    // *((uint64_t*)p) = p_role->get_object_id();
    // p += sizeof(uint64_t);
	// req.SerializeToArray(p, body_sz);

    // const tcp_socket_ptr& p_gate_socket = gate_manager_t::get_gate_socket_by_id(p_role->get_gate_id());
    // if (NULL == p_gate_socket)
    // {
    //     log_error("NULL == p_gate_socket");
    //     return;
    // }
	// family_msg_handle_t::handle_cg_approve_join_family_request(p_gate_socket, out);

	FUNC_CHECK_END
}

void gm_cmd_opt_t::opt_add_hero(role_ptr p_role, std::vector<std::string>& gm_cmd_list, proto::common::role_change_data* p_data)
{
	FUNC_CHECK_START
	if (NULL == p_role)
	{
		log_error("NULL == p_role");
		return;
	}

	int32_t gm_cmd_size = 2;
	if ((int32_t)(uint32_t)gm_cmd_list.size() != gm_cmd_size)
	{
		log_error("cmd_list.size()[%d] != %d", (uint32_t)gm_cmd_list.size(), gm_cmd_size);
		return;
	}
	if (!check_gm_param(gm_cmd_list[1]))
	{
		return;
	}
	uint32_t hero_id = boost::lexical_cast<uint32_t>(gm_cmd_list[1]);
	hero_manager_t::add_hero(p_role, hero_id, log_enum::source_type_gm_add_hero, p_data);

	FUNC_CHECK_END
}

void gm_cmd_opt_t::opt_add_pk(role_ptr p_role, std::vector<std::string>& gm_cmd_list, proto::common::role_change_data* p_data)
{
	FUNC_CHECK_START
	if (NULL == p_role)
	{
		log_error("NULL == p_role");
		return;
	}

	if ((int32_t)(uint32_t)gm_cmd_list.size() == 2)
	{
		if (!check_gm_param(gm_cmd_list[1]))
		{
			return;
		}
		uint32_t val = boost::lexical_cast<uint32_t>(gm_cmd_list[1]);
		p_role->add_pk_value(val, log_enum::source_type_gm_add_pk, 0);
	}

	FUNC_CHECK_END
}

void gm_cmd_opt_t::opt_chat_cmd(role_ptr p_role, std::vector<std::string>& gm_cmd_list)
{
	FUNC_CHECK_START
	if (NULL == p_role)
	{
		log_error("NULL == p_role");
		return;
	}

	if ((uint32_t)gm_cmd_list.size() != 1)
	{
		log_error("cmd_list.size()[%d] != 1", (uint32_t)gm_cmd_list.size());
		return;
	}
	proto::server::gh_gm_cmd_notify notify;
	for (std::string cmd : gm_cmd_list)
	{
		notify.add_cmds(cmd);
	}
	env::server->send_msg_to_chat(op_cmd::gh_gm_cmd_notify, p_role->get_uid(), notify);

	FUNC_CHECK_END
}

void gm_cmd_opt_t::opt_reload_script()
{
	FUNC_CHECK_START
		if (NULL == ScriptManager::getInstancePtr())
		{
			log_error("NULL == ScriptManager::getInstancePtr()");
			return;
		}
		ScriptManager::getInstancePtr()->load(std::string(c_get_root_dir()));

	FUNC_CHECK_END
}

void gm_cmd_opt_t::opt_reload_config()
{
	FUNC_CHECK_START
		reload_config();

	FUNC_CHECK_END
}

void gm_cmd_opt_t::opt_fight_pve(role_ptr p_role, std::vector<std::string>& gm_cmd_list)
{
	FUNC_CHECK_START
	if (NULL == p_role)
	{
		log_error("NULL == p_role");
		return;
	}

	if ((uint32_t)gm_cmd_list.size() != 2)
	{
		log_error("cmd_list.size()[%d] != 2", (uint32_t)gm_cmd_list.size());
		return;
	}

	uint64_t against_id = string_util_t::string_to_uint64(gm_cmd_list[1]);

	proto::common::fight_param fp;
	fp.set_type(proto::common::fight_type_against);
	fight_manager_t::fight_pve(p_role, against_id, &fp);

	FUNC_CHECK_END
}

void gm_cmd_opt_t::opt_fight_pvp(role_ptr p_role, std::vector<std::string>& gm_cmd_list)
{
	FUNC_CHECK_START
	if (NULL == p_role)
	{
		log_error("NULL == p_role");
		return;
	}

	if ((uint32_t)gm_cmd_list.size() != 2)
	{
		log_error("cmd_list.size()[%d] != 2", (uint32_t)gm_cmd_list.size());
		return;
	}

	uint64_t role_uid = string_util_t::string_to_uint64(gm_cmd_list[1]);
	role_ptr p_target = role_manager_t::find_role(role_uid);
	if (NULL == p_target)
	{
		log_error("NULL == p_target");
		return;
	}

	fight_manager_t::fight_pvp(p_role, p_target);

	FUNC_CHECK_END
}

void gm_cmd_opt_t::opt_fight_loading_end(role_ptr p_role, std::vector<std::string>& gm_cmd_list)
{
	FUNC_CHECK_START
	if (NULL == p_role)
	{
		log_error("NULL == p_role");
		return;
	}

	role_ptr owner = NULL;
	uint64_t fight_uid = 0;
	if ((uint32_t)gm_cmd_list.size() == 2)
	{
		owner = p_role;
		fight_uid = string_util_t::string_to_uint64(gm_cmd_list[1]);
	}
	else if ((uint32_t)gm_cmd_list.size() == 3)
	{
		owner = role_manager_t::find_role(string_util_t::string_to_uint64(gm_cmd_list[1]));
		fight_uid = string_util_t::string_to_uint64(gm_cmd_list[2]);
	}
	else
	{
		log_error("cmd_list.size() error [%d] != 2", (uint32_t)gm_cmd_list.size());
		return;
	}

	if (NULL == owner)
	{
		log_error("owner is null");
		return;
	}

	fight_manager_t::combat_loading_end_ntf(owner, fight_uid);

	FUNC_CHECK_END
}

void gm_cmd_opt_t::opt_jump_scene(role_ptr p_role, std::vector<std::string>& gm_cmd_list)
{
	FUNC_CHECK_START
	if (NULL == p_role)
	{
		log_error("NULL == p_role");
		return;
	}

	if ((uint32_t)gm_cmd_list.size() != 2)
	{
		log_error("cmd_list.size()[%d] != 2", (uint32_t)gm_cmd_list.size());
		return;
	}

	CHECK_GM_STR(gm_cmd_list[1]);

	scene_manager_t::gm_jump_scene(p_role, boost::lexical_cast<uint64_t>(gm_cmd_list[1]));

	FUNC_CHECK_END
}

void gm_cmd_opt_t::opt_reset_today(role_ptr p_role)
{
	FUNC_CHECK_START
	if (NULL == p_role)
	{
		log_error("NULL == p_role");
		return;
	}

	p_role->on_next_day(common::time_util_t::now_time(), common::time_util_t::get_weekday() == 1);

	FUNC_CHECK_END
}

void gm_cmd_opt_t::opt_add_mount(role_ptr p_role, std::vector<std::string>& gm_cmd_list, proto::common::role_change_data* p_data)
{
	FUNC_CHECK_START
		if (NULL == p_role)
		{
			log_error("NULL == p_role");
			return;
		}

	if ((uint32_t)gm_cmd_list.size() != 2)
	{
		log_error("cmd_list.size()[%d] != 2", (uint32_t)gm_cmd_list.size());
		return;
	}

	CHECK_GM_STR(gm_cmd_list[1]);

	uint32_t tid = boost::lexical_cast<uint32_t>(gm_cmd_list[1]);
	mount_manager_t::add_mount(p_role, tid, p_data);

	//     if (reply_code == errcode_enum::error_ok)
	//     {
	//         mount_ptr p_role_mount = p_role->get_mount_data()->get_mount(tid);
	//         if (p_role_mount)
	//         {
	//             proto::common::mount_data_single *p_mount = p_data->mutable_mount()->add_mount_data_list();
	//             p_mount->set_mount_tid(tid);
	//             p_mount->set_mount_state((proto::common::mount_state_type)p_role_mount->get_state());
	//         }
	//     }

	FUNC_CHECK_END
}

void gm_cmd_opt_t::opt_summon_npc(role_ptr p_role, std::vector<std::string>& gm_cmd_list)
{
	FUNC_CHECK_START
	if (NULL == p_role)
	{
		log_error("NULL == p_role");
		return;
	}

	if ((uint32_t)gm_cmd_list.size() != 2)
	{
		log_error("cmd_list.size()[%d] != 2", (uint32_t)gm_cmd_list.size());
		return;
	}

	CHECK_GM_STR(gm_cmd_list[1]);

	const scene_ptr& p_scene = scene_manager_t::find_scene(p_role->get_scene_id());
	if (nullptr == p_scene)
	{
		log_error("nullptr == pScene");
		return;
	}

	uint32_t tid = boost::lexical_cast<uint32_t>(gm_cmd_list[1]);

	p_scene->summon_monster<monster_t>(tid, p_role->get_current_x(), p_role->get_current_y());

	FUNC_CHECK_END
}

void gm_cmd_opt_t::opt_drop(role_ptr p_role, std::vector<std::string>& gm_cmd_list, proto::common::role_change_data* p_data)
{
	FUNC_CHECK_START
	if (NULL == p_role)
	{
		log_error("NULL == p_role");
		return;
	}

	if ((uint32_t)gm_cmd_list.size() != 2)
	{
		log_error("cmd_list.size()[%d] != 2", (uint32_t)gm_cmd_list.size());
		return;
	}

	CHECK_GM_STR(gm_cmd_list[1]);

	uint32_t tid = boost::lexical_cast<uint32_t>(gm_cmd_list[1]);
	drop_manager_t::drop(p_role, tid, log_enum::source_type_gm_drop, proto::common::drop_sys_type_gm, 0, p_data);

	FUNC_CHECK_END
}

void gm_cmd_opt_t::opt_jump(role_ptr p_role, std::vector<std::string>& gm_cmd_list)
{
	FUNC_CHECK_START
	if (NULL == p_role)
	{
		log_error("NULL == p_role");
		return;
	}

	if ((uint32_t)gm_cmd_list.size() != 3)
	{
		log_error("cmd_list.size()[%d] != 2", (uint32_t)gm_cmd_list.size());
		return;
	}

	CHECK_GM_STR(gm_cmd_list[1]);

	CHECK_GM_STR(gm_cmd_list[2]);

	uint32_t x = boost::lexical_cast<uint32_t>(gm_cmd_list[1]);
	uint32_t y = boost::lexical_cast<uint32_t>(gm_cmd_list[2]);

	scene_manager_t::gm_jump_scene(p_role, p_role->get_scene_id(), x, y);

	FUNC_CHECK_END
}

void gm_cmd_opt_t::opt_task_pve(role_ptr p_role, std::vector<std::string>& gm_cmd_list, proto::common::role_change_data* p_data)
{
	FUNC_CHECK_START
	if (NULL == p_role)
	{
		log_error("NULL == p_role");
		return;
	}

	if ((uint32_t)gm_cmd_list.size() != 2)
	{
		log_error("cmd_list.size()[%d] != 2", (uint32_t)gm_cmd_list.size());
		return;
	}

	CHECK_GM_STR(gm_cmd_list[1]);

	uint32_t tid = boost::lexical_cast<uint32_t>(gm_cmd_list[1]);
	auto p_task_mgr = p_role->get_task_mgr();
	if (NULL != p_task_mgr)
	{
		p_task_mgr->try_change_task_state(TASK_TARGET_TYPE_KILL_MONSTER, *p_data, tid, 1);
	}

	FUNC_CHECK_END
}

void gm_cmd_opt_t::opt_gm_mail(role_ptr p_role, std::vector<std::string>& gm_cmd_list)
{
	FUNC_CHECK_START
	if (NULL == p_role)
	{
		log_error("NULL == p_role");
		return;
	}

	if ((uint32_t)gm_cmd_list.size() != 2)
	{
		log_error("cmd_list.size()[%d] != 2", (uint32_t)gm_cmd_list.size());
		return;
	}

	CHECK_GM_STR(gm_cmd_list[1]);

	uint32_t mail_content = boost::lexical_cast<uint32_t>(gm_cmd_list[1]);

	mail_manager_t::send_mail(p_role->get_object_id(), proto::common::MAIL_TYPE_SYS, mail_content, proto::common::MAIL_GET_TYPE_GM);

	FUNC_CHECK_END
}

void gm_cmd_opt_t::opt_gm_random_mail(role_ptr p_role, std::vector<std::string>& gm_cmd_list)
{
	FUNC_CHECK_START
	if (NULL == p_role)
	{
		log_error("NULL == p_role");
		return;
	}

	if ((uint32_t)gm_cmd_list.size() != 2)
	{
		log_error("cmd_list.size()[%d] != 2", (uint32_t)gm_cmd_list.size());
		return;
	}

	CHECK_GM_STR(gm_cmd_list[1]);

	uint32_t mail_num = boost::lexical_cast<uint32_t>(gm_cmd_list[1]);

	std::map<uint32_t, MailTable*> confs;
	GET_ALL_CONF(MailTable, confs);

	if (confs.size() == 0)
	{
		log_error("MailTable size zero!");
		return;
	}

	std::vector<MailTable*> confs_vec;

	for (std::map<uint32_t, MailTable*>::iterator ibegin = confs.begin();
		ibegin != confs.end(); ++ibegin)
	{
		MailTable* p_mail = ibegin->second;
		if (NULL != p_mail)
		{
			confs_vec.push_back(p_mail);
		}
	}

	if (confs_vec.empty())
	{
		return;
	}

	for (uint32_t i = 0; i < mail_num; ++i)
	{
		int32_t index = random_util_t::randBetween(0, (int32_t)confs_vec.size() - 1);
		if (index == -1)
		{
			log_error("mail config rand from 0 to [%d] error", (int32_t)confs_vec.size() - 1);
			return;
		}

		MailTable* p_mail = confs_vec[index];
		if (NULL != p_mail)
			mail_manager_t::send_mail(p_role->get_object_id(), proto::common::MAIL_TYPE_SYS, p_mail->id(), proto::common::MAIL_GET_TYPE_GM);
	}

	FUNC_CHECK_END
}


void gm_cmd_opt_t::opt_gm_mail_item(role_ptr p_role, std::vector<std::string>& gm_cmd_list)
{
	FUNC_CHECK_START
	if (NULL == p_role)
	{
		log_error("NULL == p_role");
		return;
	}

	if ((uint32_t)gm_cmd_list.size() != 3)
	{
		log_error("cmd_list.size()[%d] != 2", (uint32_t)gm_cmd_list.size());
		return;
	}

	CHECK_GM_STR(gm_cmd_list[1]);

	CHECK_GM_STR(gm_cmd_list[2]);

	uint32_t item_id = boost::lexical_cast<uint32_t>(gm_cmd_list[1]);
	uint32_t add_num = boost::lexical_cast<uint32_t>(gm_cmd_list[2]);

	MailTable* p_mail = GET_CONF(MailTable, 7);
	if (NULL == p_mail) {
		log_error("p_mail[%u] error null", 7);
		return;
	}

	std::map<uint32_t, uint32_t> item_map;
	item_map.insert(std::make_pair(item_id, add_num));
	mail_manager_t::send_mail(p_role->get_uid(), proto::common::MAIL_TYPE_SYS, 0, common::string_util_t::convert_to_utf8(p_mail->from_name()), common::string_util_t::convert_to_utf8(p_mail->title()), common::string_util_t::convert_to_utf8(p_mail->content()),
		item_map, proto::common::MAIL_GET_TYPE_GM);

	FUNC_CHECK_END
}

void gm_cmd_opt_t::opt_cross_fight(role_ptr p_role)
{
	FUNC_CHECK_START
	if (NULL == p_role)
	{
		log_error("NULL == p_role");
		return;
	}

	proto::server::gr_cross_fight_test_notify ntf;
	p_role->peek_cross_data(ntf.mutable_user(), proto::common::role_cross_type_arena);

	env::server->send_msg_to_area(op_cmd::gr_cross_fight_test_notify, 0, ntf);

	FUNC_CHECK_END
}

void gm_cmd_opt_t::opt_arena_join(role_ptr p_role, proto::common::role_change_data* p_data)
{
	FUNC_CHECK_START
		arena_manager_t::join_arena(p_role, p_data);

	FUNC_CHECK_END
}


void gm_cmd_opt_t::opt_arena_buy_ticket(role_ptr p_role, proto::common::role_change_data* p_data)
{
	FUNC_CHECK_START
		arena_manager_t::buy_ticket(p_role, p_data);

	FUNC_CHECK_END
}

void gm_cmd_opt_t::arena_match(role_ptr p_role)
{
	FUNC_CHECK_START
		arena_manager_t::match_rank_for_battle(p_role);

	FUNC_CHECK_END
}

void gm_cmd_opt_t::opt_sys_notice(role_ptr p_role, std::vector<std::string>& gm_cmd_list)
{
	FUNC_CHECK_START
	if (NULL == p_role)
	{
		log_error("NULL == p_role");
		return;
	}

	if ((uint32_t)gm_cmd_list.size() != 2)
	{
		log_error("cmd_list.size()[%d] != 2", (uint32_t)gm_cmd_list.size());
		return;
	}

	CHECK_GM_STR(gm_cmd_list[1]);

	uint32_t id = boost::lexical_cast<uint32_t>(gm_cmd_list[1]);
	
	SysNoticeTable* p_notice = GET_CONF(SysNoticeTable, id);
	if (NULL != p_notice && !p_notice->text().empty())
	{
		sys_notice_manager_t::broadcast_system_notice(p_notice, 0, 0, 0, common::string_util_t::convert_to_utf8(p_notice->text()));
	}

	FUNC_CHECK_END
}

void gm_cmd_opt_t::opt_sys_pkmode(role_ptr p_role, std::vector<std::string>& gm_cmd_list)
{
	FUNC_CHECK_START
	if (NULL == p_role)
	{
		log_error("NULL == p_role");
		return;
	}

	if ((uint32_t)gm_cmd_list.size() != 2)
	{
		log_error("cmd_list.size()[%d] != 2", (uint32_t)gm_cmd_list.size());
		return;
	}

	CHECK_GM_STR(gm_cmd_list[1]);

	uint32_t mode = boost::lexical_cast<uint32_t>(gm_cmd_list[1]);

	p_role->change_pk_mode((proto::common::EM_PK_MODE)mode);

	FUNC_CHECK_END
}

void gm_cmd_opt_t::opt_change_now_time(std::vector<std::string>& gm_cmd_list)
{
	FUNC_CHECK_START
    if ((uint32_t)gm_cmd_list.size() != 2)
    {
        log_error("cmd_list.size()[%d] != 2", (uint32_t)gm_cmd_list.size());
        return;
    }

    std::string param = gm_cmd_list[1];

	uint32_t curr_time = time(NULL);
    uint32_t now_time = common::time_util_t::FormatTime_to_time_t(param);

	if (now_time < curr_time) {
		log_error("new_time[%s] less then real time", param.c_str());
		return;
	}

    //common::time_util_t::set_delta_time(now_time > curr_time ? now_time - curr_time : 0);

	// 同步给中心服
	center_msg_handler_t::sync_data_to_center(0, proto::server::data_sync_type_server_time, now_time - curr_time, true);

	//std::string str_now_time = common::time_util_t::FormatTime_to_string(common::time_util_t::now_time(), true);

	FUNC_CHECK_END
}


void gm_cmd_opt_t::opt_reset_now_time()
{
	FUNC_CHECK_START

	common::time_util_t::set_delta_time(0);

	// 同步给中心服
	center_msg_handler_t::sync_data_to_center(0, proto::server::data_sync_type_server_time, common::time_util_t::get_delta_time(), true);

	//std::string str_now_time = common::time_util_t::FormatTime_to_string(common::time_util_t::now_time(), true);

	FUNC_CHECK_END
}

void gm_cmd_opt_t::opt_king_war_add_gas(role_ptr p_role, std::vector<std::string>& gm_cmd_list)
{
	FUNC_CHECK_START
	if (NULL == p_role)
	{
		log_error("NULL == p_role");
		return;
	}

	if ((uint32_t)gm_cmd_list.size() != 4)
	{
		log_error("cmd_list.size()[%d] != 4", (uint32_t)gm_cmd_list.size());
		return;
	}

	CHECK_GM_STR(gm_cmd_list[1]);
	uint32_t country = boost::lexical_cast<uint32_t>(gm_cmd_list[1]);

	CHECK_GM_STR(gm_cmd_list[2]);
	uint32_t side = boost::lexical_cast<uint32_t>(gm_cmd_list[2]);

	CHECK_GM_STR(gm_cmd_list[3]);
	uint32_t val = boost::lexical_cast<uint32_t>(gm_cmd_list[3]);

	king_war_manager_t::add_long_gas(country, side, val);

	FUNC_CHECK_END
}

void gm_cmd_opt_t::opt_king_war_add_exploit(role_ptr p_role, std::vector<std::string>& gm_cmd_list)
{
	FUNC_CHECK_START
	if (NULL == p_role)
	{
		log_error("NULL == p_role");
		return;
	}

	if ((uint32_t)gm_cmd_list.size() != 2)
	{
		log_error("cmd_list.size()[%d] != 2", (uint32_t)gm_cmd_list.size());
		return;
	}

	CHECK_GM_STR(gm_cmd_list[1]);

	uint32_t val = boost::lexical_cast<uint32_t>(gm_cmd_list[1]);

	king_war_manager_t::gm_add_exploit(p_role, val);

	FUNC_CHECK_END
}

void gm_cmd_opt_t::opt_add_liveness(role_ptr p_role, std::vector<std::string>& gm_cmd_list)
{
	FUNC_CHECK_START
	if (NULL == p_role)
	{
		log_error("NULL == p_role");
		return;
	}

	int32_t gm_cmd_size = 2;
	if ((int32_t)(uint32_t)gm_cmd_list.size() != gm_cmd_size)
	{
		log_error("cmd_list.size()[%d] != %d", (uint32_t)gm_cmd_list.size(), gm_cmd_size);
		return;
	}
	if (!check_gm_param(gm_cmd_list[1]))
	{
		return;
	}
	uint32_t val = boost::lexical_cast<uint32_t>(gm_cmd_list[1]);
	activity_mgr_ptr p_activity_mgr = p_role->get_activity_mgr();
	p_activity_mgr->add_liveness_count(val);

	FUNC_CHECK_END
}

void gm_cmd_opt_t::opt_open_dungeon(role_ptr p_role, std::vector<std::string>& gm_cmd_list)
{
	FUNC_CHECK_START
	if (NULL == p_role)
	{
		log_error("NULL == p_role");
		return;
	}

	if ((uint32_t)gm_cmd_list.size() != 2)
	{
		log_error("cmd_list.size()[%d] != 2", (uint32_t)gm_cmd_list.size());
		return;
	}

	CHECK_GM_STR(gm_cmd_list[1]);

	uint32_t val = boost::lexical_cast<uint32_t>(gm_cmd_list[1]);

	if (dungeon_manager_t::check_dungeon(p_role, val))
	{
		dungeon_manager_t::open_dungeon(p_role, val);
	}

	FUNC_CHECK_END
}


void gm_cmd_opt_t::opt_restart_dungeon(role_ptr p_role)
{
	FUNC_CHECK_START
		if (NULL == p_role)
		{
			log_error("NULL == p_role");
			return;
		}

	if (!p_role->is_in_dungeon())
	{
		log_error("role[%lu] is not in dungeon", p_role->get_uid());
		return;
	}

	uint32_t dungeon_id = p_role->get_role_scene_info().get_dungeon_id();
	if (dungeon_manager_t::check_dungeon(p_role, dungeon_id))
	{
		const scene_ptr& p_scene = scene_manager_t::find_scene(p_role->get_scene_id());
		if (p_scene == NULL)
		{
			log_error("role[%lu] scene[%lu] not find", p_role->get_uid(), p_role->get_scene_id());
			return;
		}

		dungeon_scene_ptr p_dungeon_scene = p_scene->get_dungeon_scene_ptr();
		if (p_dungeon_scene == NULL)
		{
			log_error("role[%lu] scene[%lu] is not dungeon scene", p_role->get_uid(), p_role->get_scene_id());
			return;
		}

// 		if (!p_dungeon_scene->is_can_restart())
// 		{
// 			log_error("role[%lu] dungeon[%lu] can't restart", p_role->get_uid(), p_role->get_scene_id());
// 			return false;
// 		}

		// 先把这个消息给客户端，做表现
		proto::client::gc_dungeon_fight_again_reply reply;
		reply.set_reply_code(common::errcode_enum::error_ok);
		p_role->send_msg_to_client(op_cmd::gc_dungeon_fight_again_reply, reply);

		p_dungeon_scene->restart_dungeon();
	}

	FUNC_CHECK_END
}


void gm_cmd_opt_t::opt_dungeon_event(role_ptr p_role, std::vector<std::string>& gm_cmd_list)
{
	FUNC_CHECK_START
	if (NULL == p_role)
	{
		log_error("NULL == p_role");
		return;
	}

	if ((uint32_t)gm_cmd_list.size() != 2)
	{
		log_error("cmd_list.size()[%d] != 2", (uint32_t)gm_cmd_list.size());
		return;
	}

	CHECK_GM_STR(gm_cmd_list[1]);

	uint32_t val = boost::lexical_cast<uint32_t>(gm_cmd_list[1]);

	const scene_ptr& p_scene = scene_manager_t::find_scene(p_role->get_scene_id());
	if (p_scene == NULL)
	{
		log_error("role[%lu] scene[%lu] not find", p_role->get_uid(), p_role->get_scene_id());
		return;
	}

	dungeon_scene_ptr p_dungeon_scene = p_scene->get_dungeon_scene_ptr();
	if (p_dungeon_scene == NULL)
	{
		log_error("role[%lu] scene[%lu] is not dungeon scene", p_role->get_uid(), p_role->get_scene_id());
		return;
	}

	p_dungeon_scene->ask_do_event(val);

	FUNC_CHECK_END
}

void gm_cmd_opt_t::opt_open_function(role_ptr p_role, std::vector<std::string>& gm_cmd_list)
{
	FUNC_CHECK_START
	if (NULL == p_role)
	{
		log_error("NULL == p_role");
		return;
	}

	if ((uint32_t)gm_cmd_list.size() != 2)
	{
		log_error("cmd_list.size()[%d] != 2", (uint32_t)gm_cmd_list.size());
		return;
	}

	CHECK_GM_STR(gm_cmd_list[1]);

	uint32_t val = boost::lexical_cast<uint32_t>(gm_cmd_list[1]);

	p_role->open_function(val);

	FUNC_CHECK_END
}


void gm_cmd_opt_t::opt_open_all_function(role_ptr p_role)
{
	FUNC_CHECK_START
	if (NULL == p_role)
	{
		log_error("NULL == p_role");
		return;
	}

	p_role->open_all_function();

	FUNC_CHECK_END
}

void gm_cmd_opt_t::opt_npc_code(role_ptr p_role, std::vector<std::string>& gm_cmd_list)
{
	FUNC_CHECK_START
	if (NULL == p_role)
	{
		log_error("NULL == p_role");
		return;
	}

	if ((uint32_t)gm_cmd_list.size() != 2)
	{
		log_error("cmd_list.size()[%d] != 2", (uint32_t)gm_cmd_list.size());
		return;
	}

	CHECK_GM_STR(gm_cmd_list[1]);

	uint32_t val = boost::lexical_cast<uint32_t>(gm_cmd_list[1]);

	p_role->set_npc_code(val);

	FUNC_CHECK_END
}

void gm_cmd_opt_t::opt_collect_code(role_ptr p_role, std::vector<std::string>& gm_cmd_list)
{
	FUNC_CHECK_START
	if (NULL == p_role)
	{
		log_error("NULL == p_role");
		return;
	}

	if ((uint32_t)gm_cmd_list.size() != 2)
	{
		log_error("cmd_list.size()[%d] != 2", (uint32_t)gm_cmd_list.size());
		return;
	}

	CHECK_GM_STR(gm_cmd_list[1]);

	uint32_t val = boost::lexical_cast<uint32_t>(gm_cmd_list[1]);

	p_role->set_collect_code(val);

	FUNC_CHECK_END
}

void gm_cmd_opt_t::opt_change_arena_score(role_ptr p_role, std::vector<std::string>& gm_cmd_list)
{
	FUNC_CHECK_START
	if (NULL == p_role)
	{
		log_error("NULL == p_role");
		return;
	}

	if ((uint32_t)gm_cmd_list.size() != 2)
	{
		log_error("cmd_list.size()[%d] != 2", (uint32_t)gm_cmd_list.size());
		return;
	}

	CHECK_GM_STR(gm_cmd_list[1]);

	uint32_t val = boost::lexical_cast<uint32_t>(gm_cmd_list[1]);

	p_role->get_arena().set_score(val);

	FUNC_CHECK_END
}

void gm_cmd_opt_t::opt_add_plugin(role_ptr p_role, std::vector<std::string>& gm_cmd_list)
{
	FUNC_CHECK_START
		if (NULL == p_role)
		{
			log_error("NULL == p_role");
			return;
		}

	int32_t gm_cmd_size = 2;
	if ((int32_t)(uint32_t)gm_cmd_list.size() != gm_cmd_size)
	{
		log_error("cmd_list.size()[%d] != %d", (uint32_t)gm_cmd_list.size(), gm_cmd_size);
		return;
	}
	if (!check_gm_param(gm_cmd_list[1]))
	{
		return;
	}
	uint32_t val = boost::lexical_cast<uint32_t>(gm_cmd_list[1]);

	HeroPlugin* p_config = GET_CONF(HeroPlugin,val);
	if (NULL == p_config)
	{
		log_error("NULL == p_config plugin_id[%d]",val);
		return;
	}
	const uint32_t hero_id = p_config->hero_id();
	if (0 == hero_id)
	{
		log_error("hero id 0 plugin_id[%d]", val);
		return;
	}
	std::vector<hero_ptr> hero_list;
	hero_manager_t::get_all_heroes(p_role, hero_list);
	for (uint32_t i = 0; i < hero_list.size(); ++i)
	{
		hero_ptr p_hero = hero_list[i];
		if (NULL == p_hero)
		{
			log_error("NULL == p_hero role[%lu]", p_role->get_uid());
			continue;
		}
		if (p_hero->get_tid() != hero_id)
			continue;

		p_hero->add_plugin(val);
	}
	FUNC_CHECK_END
}

void gm_cmd_opt_t::opt_add_trade_point(role_ptr p_role, std::vector<std::string>& gm_cmd_list)
{
	FUNC_CHECK_START
		if (NULL == p_role)
		{
			log_error("NULL == p_role");
			return;
		}

	int32_t gm_cmd_size = 2;
	if ((int32_t)(uint32_t)gm_cmd_list.size() != gm_cmd_size)
	{
		log_error("cmd_list.size()[%d] != %d", (uint32_t)gm_cmd_list.size(), gm_cmd_size);
		return;
	}
	if (!check_gm_param(gm_cmd_list[1]))
	{
		return;
	}
	uint32_t val = boost::lexical_cast<uint32_t>(gm_cmd_list[1]);

	p_role->get_trade_info().set_trade_point(val, log_enum::source_type_trade_gm_add_trade_point);

	FUNC_CHECK_END
}


void gm_cmd_opt_t::opt_add_honor(role_ptr p_role, std::vector<std::string>& gm_cmd_list)
{
	FUNC_CHECK_START
		if (NULL == p_role)
		{
			log_error("NULL == p_role");
			return;
		}

	int32_t gm_cmd_size = 2;
	if ((int32_t)(uint32_t)gm_cmd_list.size() != gm_cmd_size)
	{
		log_error("cmd_list.size()[%d] != %d", (uint32_t)gm_cmd_list.size(), gm_cmd_size);
		return;
	}
	if (!check_gm_param(gm_cmd_list[1]))
	{
		return;
	}
	//uint32_t val = boost::lexical_cast<uint32_t>(gm_cmd_list[1]);
	//arena_t& arena_data = p_role->get_arena();
	//arena_data.set_honor(arena_data.get_honor() + val);

	//通知center
	//proto::server::ge_arena_fight_data_notify msg_to_center;
	//msg_to_center.set_fight_value(p_role->get_fighting());
	//p_role->peek_fight_data(msg_to_center.mutable_fight_data());
	//env::server->send_msg_to_center(op_cmd::ge_arena_fight_data_notify, p_role->get_uid(), msg_to_center);
	FUNC_CHECK_END
}

void gm_cmd_opt_t::opt_recharge(role_ptr p_role, std::vector<std::string>& gm_cmd_list, proto::common::role_change_data* p_data)
{
	FUNC_CHECK_START
		if (NULL == p_role)
		{
			log_error("NULL == p_role");
			return;
		}
	recharge_ptr p_recharge = p_role->get_recharge();
	if (NULL == p_recharge)
	{
		log_error("NULL == p_role");
		return;
	}
	int32_t gm_cmd_size = 2;
	if ((int32_t)(uint32_t)gm_cmd_list.size() != gm_cmd_size)
	{
		log_error("cmd_list.size()[%d] != %d", (uint32_t)gm_cmd_list.size(), gm_cmd_size);
		return;
	}
	if (!check_gm_param(gm_cmd_list[1]))
	{
		return;
	}
	uint32_t val = boost::lexical_cast<uint32_t>(gm_cmd_list[1]);
	p_recharge->rmb_recharge(val, p_data);

	FUNC_CHECK_END
}


std::string gm_cmd_opt_t::opt_query_recharge(role_ptr p_role)
{
	if (NULL == p_role)
	{
		log_error("NULL == p_role");
		return "";
	}
	recharge_ptr p_recharge = p_role->get_recharge();
	if (NULL == p_recharge)
	{
		log_error("NULL == p_role");
		return "";
	}
	uint32_t cur_total = p_recharge->get_total_cny();
	uint32_t vip_level = p_recharge->get_vip_level();
	std::string str;

	str.append("total_RMB = ");
	str.append(string_util_t::uint32_to_string(cur_total).c_str());
	str.append(", vip_level = ");
	str.append(string_util_t::uint32_to_string(vip_level).c_str());
	return  str;
}

void gm_cmd_opt_t::opt_prison(role_ptr p_role)
{
	p_role->add_pk_value(50, log_enum::source_type_gm_add_pk, 0);
	p_role->move_to_prison();
}

void gm_cmd_opt_t::opt_clear_notice()
{
	sys_notice_manager_t::gm_clear_notify_to_center();
}

void gm_cmd_opt_t::opt_level_reward(role_ptr p_role, std::vector<std::string>& gm_cmd_list, proto::common::role_change_data* p_data)
{
	FUNC_CHECK_START
	if (NULL == p_role)
	{
		log_error("NULL == p_role");
		return;
	}

	if ((uint32_t)gm_cmd_list.size() != 2)
	{
		log_error("cmd_list.size()[%d] != %d", (uint32_t)gm_cmd_list.size(), 2);
		return;
	}
	if (!check_gm_param(gm_cmd_list[1]))
	{
		return;
	}
	uint32_t val = boost::lexical_cast<uint32_t>(gm_cmd_list[1]);
	p_role->get_level_reward(val, p_data);

	FUNC_CHECK_END
}

void gm_cmd_opt_t::opt_offline_arena_rank(role_ptr p_role, std::vector<std::string>& gm_cmd_list, proto::common::role_change_data* p_data)
{
	FUNC_CHECK_START
		if (NULL == p_role)
		{
			log_error("NULL == p_role");
			return;
		}

	if ((uint32_t)gm_cmd_list.size() != 2)
	{
		log_error("cmd_list.size()[%d] != %d", (uint32_t)gm_cmd_list.size(), 2);
		return;
	}
	if (!check_gm_param(gm_cmd_list[1]))
	{
		return;
	}
	uint32_t val = boost::lexical_cast<uint32_t>(gm_cmd_list[1]);
	proto::server::ge_offline_arena_gmrank_notify ntf;
	ntf.set_rank(val);
	env::server->send_msg_to_center(op_cmd::ge_offline_arena_gmrank_notify, p_role->get_uid(), ntf);
	FUNC_CHECK_END
}

void gm_cmd_opt_t::opt_summon_field_boss(std::vector<std::string>& gm_cmd_list)
{
	FUNC_CHECK_START
	if ((uint32_t)gm_cmd_list.size() != 2)
	{
		log_error("cmd_list.size()[%d] != %d", (uint32_t)gm_cmd_list.size(), 2);
		return;
	}
	if (!check_gm_param(gm_cmd_list[1]))
	{
		return;
	}
	uint32_t val = boost::lexical_cast<uint32_t>(gm_cmd_list[1]);
	// 同步给中心服
	center_msg_handler_t::sync_data_to_center(0, proto::server::data_sync_type_field_boss, val, true);
	FUNC_CHECK_END
}

void gm_cmd_opt_t::opt_country_king_family(role_ptr p_role, std::vector<std::string>& gm_cmd_list)
{
	FUNC_CHECK_START
	if (NULL == p_role)
	{
		log_error("NULL == p_role");
		return;
	}

	if ((uint32_t)gm_cmd_list.size() == 2)
	{
		CHECK_GM_STR(gm_cmd_list[1]);
		uint32_t country_id = boost::lexical_cast<uint32_t>(gm_cmd_list[1]);

		proto::server::ge_gm_set_king_family_notify notify;
		notify.set_country_id(country_id);
		notify.set_family_uid(p_role->get_family_id());
		env::server->send_msg_to_center(op_cmd::ge_gm_set_king_family_notify, 0, notify);
	} else if ((uint32_t)gm_cmd_list.size() == 3) {
		CHECK_GM_STR(gm_cmd_list[1]);
		uint32_t country_id = boost::lexical_cast<uint32_t>(gm_cmd_list[1]);

		CHECK_GM_STR(gm_cmd_list[2]);
		uint64_t family_id = boost::lexical_cast<uint64_t>(gm_cmd_list[2]);

		proto::server::ge_gm_set_king_family_notify notify;
		notify.set_country_id(country_id);
		notify.set_family_uid(family_id);
		env::server->send_msg_to_center(op_cmd::ge_gm_set_king_family_notify, 0, notify);
	} else {
		log_error("cmd_list.size()[%d] error", (uint32_t)gm_cmd_list.size());
	}

	FUNC_CHECK_END
}


void gm_cmd_opt_t::opt_add_vigour(role_ptr p_role, std::vector<std::string>& gm_cmd_list)
{
	FUNC_CHECK_START
		if (NULL == p_role)
		{
			log_error("NULL == p_role");
			return;
		}

	if ((uint32_t)gm_cmd_list.size() != 2)
	{
		log_error("cmd_list.size()[%d] != %d", (uint32_t)gm_cmd_list.size(), 2);
		return;
	}

	if (!check_gm_param(gm_cmd_list[1]))
	{
		return;
	}

	uint32_t val = boost::lexical_cast<uint32_t>(gm_cmd_list[1]);
	money_manager_t::add_money(p_role, proto::common::MONEY_TYPE_VIGOUR, val, log_enum::source_type_vigour_gm_add, 0);
	FUNC_CHECK_END
}


void gm_cmd_opt_t::opt_reduce_vigour(role_ptr p_role, std::vector<std::string>& gm_cmd_list)
{
	FUNC_CHECK_START
		if (NULL == p_role)
		{
			log_error("NULL == p_role");
			return;
		}

	if ((uint32_t)gm_cmd_list.size() != 2)
	{
		log_error("cmd_list.size()[%d] != %d", (uint32_t)gm_cmd_list.size(), 2);
		return;
	}

	if (!check_gm_param(gm_cmd_list[1]))
	{
		return;
	}

	uint32_t val = boost::lexical_cast<uint32_t>(gm_cmd_list[1]);
	money_manager_t::use_money(p_role, proto::common::MONEY_TYPE_VIGOUR, val, log_enum::source_type_vigour_gm_reduce, 0);
	FUNC_CHECK_END
}


void gm_cmd_opt_t::opt_test_achieve(role_ptr p_role)
{
	FUNC_CHECK_START
		if (NULL == p_role)
		{
			log_error("NULL == p_role");
			return;
		}

	achieve_common_t::test_add_achieve(p_role->get_uid());
	FUNC_CHECK_END
}


void gm_cmd_opt_t::opt_add_head(role_ptr p_role, std::vector<std::string>& gm_cmd_list)
{
	FUNC_CHECK_START
		if (NULL == p_role)
		{
			log_error("NULL == p_role");
			return;
		}

	if ((uint32_t)gm_cmd_list.size() != 2)
	{
		log_error("cmd_list.size()[%d] != %d", (uint32_t)gm_cmd_list.size(), 2);
		return;
	}

	if (!check_gm_param(gm_cmd_list[1]))
	{
		return;
	}

	uint32_t val = boost::lexical_cast<uint32_t>(gm_cmd_list[1]);
	p_role->add_head(val);
	FUNC_CHECK_END
}


void gm_cmd_opt_t::opt_change_name(role_ptr p_role, std::vector<std::string>& gm_cmd_list)
{
	FUNC_CHECK_START
		if (NULL == p_role)
		{
			log_error("NULL == p_role");
			return;
		}
		if ((uint32_t)gm_cmd_list.size() != 2)
		{
			log_error("cmd_list.size()[%d] != 2", (uint32_t)gm_cmd_list.size());
			return;
		}

	std::string param = gm_cmd_list[1];

	p_role->change_name_request(param);
	FUNC_CHECK_END
}


void gm_cmd_opt_t::opt_open_all_dungeon(role_ptr p_role)
{
	FUNC_CHECK_START
		if (NULL == p_role)
		{
			log_error("NULL == p_role");
			return;
		}

	p_role->get_dungeon_data().gm_pass_all_dungeon();
	FUNC_CHECK_END
}


void gm_cmd_opt_t::opt_finish_all_elite(role_ptr p_role)
{
	FUNC_CHECK_START
		if (NULL == p_role)
		{
			log_error("NULL == p_role");
			return;
		}
	
	page_manager_t::gm_finish_all_elite(p_role->get_uid());
	
	FUNC_CHECK_END
}


void gm_cmd_opt_t::opt_set_cur_tower(role_ptr p_role, std::vector<std::string>& gm_cmd_list)
{
	FUNC_CHECK_START
		if (NULL == p_role)
		{
			log_error("NULL == p_role");
			return;
		}
	if ((uint32_t)gm_cmd_list.size() != 2)
	{
		log_error("cmd_list.size()[%d] != 2", (uint32_t)gm_cmd_list.size());
		return;
	}
	tower_ptr p_tower = p_role->get_tower();
	if (NULL == p_tower)
	{
		log_error("NULL == p_role");
		return;
	}
	uint32_t param = string_util_t::string_to_uint32(gm_cmd_list[1]);
	p_tower->set_current_tower(param);
	p_tower->save_self();
	FUNC_CHECK_END
}


void gm_cmd_opt_t::opt_set_record_tower(role_ptr p_role, std::vector<std::string>& gm_cmd_list)
{
	FUNC_CHECK_START
		if (NULL == p_role)
		{
			log_error("NULL == p_role");
			return;
		}
	if ((uint32_t)gm_cmd_list.size() != 2)
	{
		log_error("cmd_list.size()[%d] != 2", (uint32_t)gm_cmd_list.size());
		return;
	}
	tower_ptr p_tower = p_role->get_tower();
	if (NULL == p_tower)
	{
		log_error("NULL == p_role");
		return;
	}
	uint32_t param = string_util_t::string_to_uint32(gm_cmd_list[1]);
	p_tower->set_record_tower(param);
	p_tower->save_self();
	FUNC_CHECK_END
}

void gm_cmd_opt_t::opt_merage_item(role_ptr p_role, std::vector<std::string>& gm_cmd_list, proto::common::role_change_data* p_data)
{
	FUNC_CHECK_START
		if (NULL == p_role)
		{
			log_error("NULL == p_role");
			return;
		}

	if ((uint32_t)gm_cmd_list.size() != 3)
	{
		log_error("cmd_list.size()[%d] != %d", (uint32_t)gm_cmd_list.size(), 2);
		return;
	}

	if (!check_gm_param(gm_cmd_list[1]) || !check_gm_param(gm_cmd_list[2]))
	{
		return;
	}

	uint32_t item_tid	 = boost::lexical_cast<uint32_t>(gm_cmd_list[1]);
	uint32_t merage_num = boost::lexical_cast<uint32_t>(gm_cmd_list[2]);
	item_manager_t::merage_item(p_role, item_tid, merage_num, p_data);
	FUNC_CHECK_END
}

void gm_cmd_opt_t::opt_fight_win(role_ptr p_role){
	if (NULL == p_role) {
		log_error("NULL == p_role ");
		return;
	}
	FUNC_CHECK_START
	combat_ptr p_combat = fight_manager_t::check_combat(p_role, p_role->get_fight_uid());
	if (NULL != p_combat) {
		p_combat->gm_win_fight();
	}
	FUNC_CHECK_END
}

void gm_cmd_opt_t::opt_clearcd(role_ptr p_role) {
	if (NULL == p_role) {
		log_error("NULL == p_role ");
			return;
	}
	FUNC_CHECK_START
	combat_ptr p_combat = fight_manager_t::check_combat(p_role, p_role->get_fight_uid());
	if (NULL != p_combat) {
		p_combat->gm_clear_all_hero_cd();
	}

	FUNC_CHECK_END
}

void gm_cmd_opt_t::opt_hy_cmd(role_ptr p_role, std::vector<std::string>& gm_cmd_list)
{
	FUNC_CHECK_START
	if (NULL == p_role)
	{
		log_error("NULL == p_role");
		return;
	}
	std::string param1 = boost::lexical_cast<std::string>(gm_cmd_list[1]);

	task_mgr_ptr task_mgr = p_role->get_task_mgr();
	if (NULL == task_mgr)
	{
		log_error("NULL == p_role");
		return;
	}
	if (param1 == "quicktask") 
	{
		proto::client::gc_task_quick_data_reply msg;
		task_mgr->get_quick_task_info(msg);
		return;
	}

	if (param1 == "quickfinishtask")
	{
		//proto::client::cg_task_quick_complete_request msg; 
		//msg.add_task_list(1);
		//msg.add_task_list(22);
		//msg.add_task_list(23);
		//msg.add_task_list(24);
		//proto::common::role_change_data rcd;
		//task_mgr->complate_quick_task(msg, rcd);
	}

	if (param1 == "shake_tree")
	{
		proto::client::gc_get_money_tree_reply msg1;
		business_activity_manager_t::get_money_tree_info(p_role, msg1);

		proto::client::gc_shake_money_tree_reply msg2;
		business_activity_manager_t::shake_money_tree(p_role, msg2);

		proto::client::gc_levelup_money_tree_reply msg3;
		business_activity_manager_t::level_up_money_tree(p_role, msg3);
	}

	if ( param1 == "page")
	{
		proto::client::gc_quick_fight_elite_page_reply reply1;		//快速扫荡
		proto::client::gc_reset_quick_fight_elite_page_reply reply2;	//重置类
		proto::client::gc_section_reply reply3;

		page_manager_t::quick_fight_elite_page(p_role->get_uid(), 1, 1, reply1);
		page_manager_t::reset_quick_fight_elite_page(p_role->get_uid(), 1, reply2);
		page_manager_t::get_page_list(p_role->get_uid(), reply3);
	}

	if (param1 == "7d")
	{
		//七日登录
		proto::client::gc_get_7d_login_reply msg1;
		business_activity_manager_t::get_sevenday_login_info(p_role, msg1);

		proto::client::gc_get_7d_login_prize_reply msg2;
		business_activity_manager_t::get_sevenday_login_prize(p_role, 1, msg2);
	}

	if (param1 == "month_card")
	{
		proto::client::gc_get_month_card_info_reply msg1;
		business_activity_manager_t::get_month_card_info(p_role, msg1);

		proto::client::gc_get_month_card_prize_reply msg2;
		business_activity_manager_t::get_month_card_ed_prize(p_role, 1, msg2.mutable_rcd());

		business_activity_manager_t::buy_month_card(p_role, 1);
	}

	FUNC_CHECK_END
}

