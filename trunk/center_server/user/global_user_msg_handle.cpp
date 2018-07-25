#include "global_user_msg_handle.hpp"

#include "game/game_msg_handler.hpp"
#include "global_user_manager.hpp"
#include "country/country_mgr.hpp"
#include "game/game_manager.hpp"
#include "city/city_manager.hpp"
#include "item/item_manager.hpp"
#include "redis_client.hpp"
#include "rank/rank_manager.hpp"
#include "field_boss/field_boss_manager.hpp"
#include "friend/friend_manager.hpp"
#include "DataManager.h"
#include "gate/gate_msg_handler.hpp"
#include "field_boss/field_boss_manager.hpp"
#include "scene/global_scene_manager.hpp"
#include "family/family_manager.hpp"
#include "offline_friend_msg_mgr/offline_friend_msg_mgr.hpp"
#include "user/global_user.hpp"
#include "user/global_user_manager.hpp"
#include "tblh/errcode_enum.hpp"
#include "friend/friend_mgr.hpp"
#include "redis/friend_data_mgr.hpp"
#include "offline/offline_role_manager.hpp"
#include "sys_notice/sys_notice_manager.hpp"
#include "tblh/Comprehensive.tbls.h"
#include "country/country_mgr.hpp"
#include "shop/shop_manager.hpp"
#include "look_role_info/simple_data_manager.hpp"
#include "redis/global_user_data_mgr.hpp"

USING_NS_NETWORK;
USING_NS_COMMON;

server_msg_handler_t<proto::server::data_sync_type_begin, proto::server::data_sync_type_end, global_user_msg_handle_t::data_sync_func> global_user_msg_handle_t::m_data_syncer;
proto::server::eg_sync_data_request global_user_msg_handle_t::m_sync_datas;
uint64_t global_user_msg_handle_t::m_sync_uid = 0;

bool global_user_msg_handle_t::init_game_msg_handler()
{
	bool bresult = true;

    bresult &= game_msg_handler_t::m_game_msg_handle.register_func(op_cmd::ge_syn_game_info_notify, handle_ge_syn_game_info_notify);
	bresult &= game_msg_handler_t::m_game_msg_handle.register_func(op_cmd::ge_update_rank_notify, handle_ge_update_rank_notify);
	bresult &= game_msg_handler_t::m_game_msg_handle.register_func(op_cmd::ge_sync_data_request, handle_ge_sync_data_request);
	bresult &= game_msg_handler_t::m_game_msg_handle.register_func(op_cmd::ge_reload_config_notify, handle_ge_reload_config);
	bresult &= game_msg_handler_t::m_game_msg_handle.register_func(op_cmd::ge_ready_enter_scene_notify, handle_ge_ready_enter_scene_notify);
    bresult &= game_msg_handler_t::m_game_msg_handle.register_func(op_cmd::ge_pk_beat_criminal_notify, handle_ge_pk_beat_criminal_notify);
	bresult &= game_msg_handler_t::m_game_msg_handle.register_func(op_cmd::ge_unreg_user_scene_notify, handle_ge_unreg_user_scene_notify);
	bresult &= game_msg_handler_t::m_game_msg_handle.register_func(op_cmd::ge_reg_user_scene_reply, handle_ge_reg_user_scene_reply);
	bresult &= game_msg_handler_t::m_game_msg_handle.register_func(op_cmd::ge_chat_get_role_info_reply, handle_ge_chat_get_role_info_reply);
	bresult &= game_msg_handler_t::m_game_msg_handle.register_func(op_cmd::ge_user_cross_scene_request, handle_ge_user_cross_scene_request);
	bresult &= game_msg_handler_t::m_game_msg_handle.register_func(op_cmd::ge_tower_trigger_notify, handle_ge_tower_trigger_notify);
	bresult &= game_msg_handler_t::m_game_msg_handle.register_func(op_cmd::ge_offline_data_notify, handle_ge_offline_data_notify);
	
	bresult &= init_data_syncers();

	return bresult;
}

bool global_user_msg_handle_t::handle_ge_ready_enter_scene_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
    PRE_S2S_MSG(proto::server::ge_ready_enter_scene_notify);

    global_user_manager_t::ready_enter_scene(uid);

    return true;
}

bool global_user_msg_handle_t::handle_ge_syn_game_info_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::ge_syn_game_info_notify);

	game_manager_t::set_game(s, msg.game_id());
	// 注册场景
	global_scene_manager_t::on_add_game(s, msg.regs());

	country_mgr_t::sync_country_data(s);
	city_manager_t::handle_ge_get_city_list_notify(s);
	item_manager_t::handle_ge_get_item_limit_list(s);
	sys_notice_manager_t::handle_ge_get_sys_notice_list_request(s);

	return true;
}

bool global_user_msg_handle_t::handle_ge_update_rank_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::ge_update_rank_notify);
	rank_manager_t::update_rank(msg);
	return true;
}

bool global_user_msg_handle_t::handle_ge_sync_data_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::ge_sync_data_request);

	for (int32_t i = 0; i < msg.items_size(); ++i)
	{
		const proto::server::data_sync_item& item = msg.items(i);
		auto func = m_data_syncer.find_func(item.type());
		if (NULL != func)
		{
			func(uid, item.value());
		}
		else
		{
			log_error("no func register for data sync type[%u]", (uint32_t)item.type());
			return false;
		}

	}

	return true;
}

bool global_user_msg_handle_t::handle_ge_reload_config(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	if (reloadAllDTManager(env::xml->config_path))
	{
		// 重新加载商店配置
		shop_manager_t::reload();

		// 清理所有boss，重新加载配置
		field_boss_manager_t::reload();

		// 加载好友的一些常用设置
		friend_manager_t::reload();
	}

	return true;
}

bool global_user_msg_handle_t::handle_ge_pk_beat_criminal_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::ge_pk_beat_criminal_notify);

	uint32_t rank_val = rank_manager_t::get_rank_by_uid(common::rank_common::criminal_rank_0, msg.criminal_uid());
	if (rank_val != 0) {
		global_user_ptr p_self = global_user_manager_t::get_global_user_by_uid(uid);
		global_user_ptr p_criminal = global_user_manager_t::get_global_user_by_uid(msg.criminal_uid());
		if (NULL != p_self && NULL != p_criminal)
		{
			BROADCAST_SYS_NOTICE(sys_notice_enum::sys_notice_10, 0, 0, 0, p_self->get_name().c_str(), rank_val, p_criminal->get_name().c_str())
		}
	}
	return true;
}

bool global_user_msg_handle_t::handle_ge_unreg_user_scene_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::ge_unreg_user_scene_notify);

	global_user_manager_t::unreg_global_user(uid, msg.unreg_code(), msg.kick_reason());

	return true;
}

bool global_user_msg_handle_t::handle_ge_reg_user_scene_reply(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::ge_reg_user_scene_reply);

	global_user_manager_t::scene_reg_user_success(s, uid, msg.user_data());

	return true;
}

bool global_user_msg_handle_t::handle_ge_chat_get_role_info_reply(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::ge_chat_get_role_info_reply);
	global_user_ptr p_user = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL == p_user)
	{
		log_error("NULL == p_user user[%lu]",uid);
		return false;
	}
	const proto::common::chat_common_data& chat_notify = msg.chat_notify();
	const proto::common::chat_common_role_info& chat_role_data = msg.chat_role_info();
	uint32_t now_time = common::time_util_t::now_time();
	proto::client::ec_personal_chat_notify notify_msg;
	notify_msg.set_src_role_id(string_util_t::uint64_to_string(uid));
	notify_msg.set_src_role_name(p_user->get_name());
	notify_msg.set_chat_info(chat_notify.chat_info());
	notify_msg.set_src_head_id(chat_notify.head_icon_id());
	notify_msg.set_src_level(p_user->get_level());
	notify_msg.set_contact_time(now_time);
	// 更新自己最近联系时间
	const uint64_t target_uid = string_util_t::string_to_uint64(chat_notify.tar_role_id());
	p_user->get_friend_mgr().update_contact_time(target_uid, now_time, true);
	global_user_ptr p_tar_user = global_user_manager_t::get_global_user_by_uid(target_uid);
	// 更新对方最近联系时间
	if (p_tar_user != NULL)
	{
		p_tar_user->get_friend_mgr().update_contact_time(uid, now_time, true);
	}
	else
	{
		friend_data_mgr_t::update_offline_friend_contact_time(target_uid, uid, now_time);
	}

	// 对方不在线 记录离线消息
	if (NULL == p_tar_user || p_tar_user->get_online_state() != ROLE_ONLINE_STATE_ONLINE)
	{
		offline_friend_msg_mgr_t::add_offline_friend_msg(target_uid, notify_msg);
	}
	else
	{
		p_tar_user->send_msg_to_client(op_cmd::ec_personal_chat_notify, notify_msg);
	}

	proto::client::ec_personal_chat_reply reply_msg;
	reply_msg.set_reply_code(errcode_enum::error_ok);
	reply_msg.set_contact_time(now_time);
	proto::common::chat_common_role_info* p_data = reply_msg.mutable_chat_role_data();
	p_data->CopyFrom(chat_role_data);
	p_user->send_msg_to_client(op_cmd::ec_personal_chat_reply, reply_msg);
	return true;
}

bool global_user_msg_handle_t::handle_ge_user_cross_scene_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::ge_user_cross_scene_request);

	global_user_ptr p_user = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL == p_user)
	{
		log_error("user[%lu] null error", uid);
		return false;
	}

	uint32_t back_code = global_user_manager_t::user_cross_scene_check(p_user, msg.data());

	proto::server::eg_user_cross_scene_reply reply;
	reply.set_reply_code(back_code);
	if (reply.mutable_data()) reply.mutable_data()->CopyFrom(msg.data());
	env::server->send_msg_to_game(s, op_cmd::eg_user_cross_scene_reply, uid, reply);

	return true;
}

bool global_user_msg_handle_t::handle_ge_tower_trigger_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::ge_tower_trigger_notify);

	global_user_ptr p_user = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL == p_user)
	{
		log_error("user[%lu] null error", uid);
		return false;
	}
	p_user->get_friend_mgr().tower_trigger_notify(msg);
	family_ptr p_family = family_manager_t::get_family(p_user->get_family_id());
	if (NULL != p_family)
	{
		p_family->tower_trigger_notify(uid, msg);
	}
	return true;
}

bool global_user_msg_handle_t::handle_ge_offline_data_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::ge_offline_data_notify);

	offline_role_manager_t::offline(uid, msg.ord());
	return true;
}

bool global_user_msg_handle_t::init_data_syncers()
{
	bool ret = true;

	ret &= m_data_syncer.register_func(proto::server::data_sync_type_level, on_sync_level);
	ret &= m_data_syncer.register_func(proto::server::data_sync_type_fighting_value, on_sync_fighting_value);
	ret &= m_data_syncer.register_func(proto::server::data_sync_type_country_money, on_country_money_change);
	ret &= m_data_syncer.register_func(proto::server::data_sync_type_server_time, on_sync_server_time);
	ret &= m_data_syncer.register_func(proto::server::data_sync_type_family_money, on_gm_add_family_money);
	ret &= m_data_syncer.register_func(proto::server::data_sync_type_family_contribution, on_sync_family_contribution);
	ret &= m_data_syncer.register_func(proto::server::data_sync_type_field_boss, on_gm_summon_boss);
	ret &= m_data_syncer.register_func(proto::server::data_sync_type_head_frame, on_sync_head_frame);
	ret &= m_data_syncer.register_func(proto::server::data_sync_type_main_plugin, on_sync_main_plugin);

	return ret;
}


bool global_user_msg_handle_t::init_client_msg_handler()
{
	bool bresult = true;

	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::ce_rank_request, handle_ce_rank_request);
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::ce_rank_time_request, handle_ce_rank_time_request);
	return bresult;
}

bool global_user_msg_handle_t::handle_ce_rank_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::ce_rank_request);

	const global_user_ptr& p_user = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL == p_user) {
		log_error("NULL == p_user[%lu]", uid);
		return false;
	}

	proto::client::ec_rank_reply reply;
	rank_manager_t::request_rank(p_user, msg.rank_type(), msg.page(), reply);
	p_user->send_msg_to_client(op_cmd::ec_rank_reply, reply);

	return true;
}


bool global_user_msg_handle_t::handle_ce_rank_time_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::ce_rank_time_request);

	const global_user_ptr& p_user = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL == p_user) {
		log_error("NULL == p_user[%lu]", uid);
		return false;
	}

	proto::client::ec_rank_time_reply reply;
	rank_manager_t::request_rank_time(reply);
	p_user->send_msg_to_client(op_cmd::ec_rank_time_reply, reply);

	return true;
}

bool global_user_msg_handle_t::on_sync_level(uint64_t uid, std::string val)
{
	uint32_t level = string_util_t::string_to_uint32(val);
	global_user_ptr p_user = global_user_manager_t::get_global_user_by_uid(uid);
	if (p_user != NULL)
	{
		p_user->set_level(level);
	}
	else
	{
		global_user_data_mgr_t::set_user_level(uid, level);
	}

	return true;
}

bool global_user_msg_handle_t::on_sync_fighting_value(uint64_t uid, std::string val)
{
	uint32_t fighting_value = string_util_t::string_to_uint32(val);
	global_user_ptr p_user = global_user_manager_t::get_global_user_by_uid(uid);
	if (p_user != NULL)
	{
		p_user->set_fighting_value(fighting_value);
	}
	else
	{
		global_user_data_mgr_t::set_user_fighting_value(uid, fighting_value);
	}
	return true;
}

bool global_user_msg_handle_t::on_country_money_change(uint64_t uid, std::string val)
{
	/*int32_t country_money_changed = string_util_t::string_to_int32(val);
	uint32_t country_id = redis_client_t::get_uint32("global_user", "country_id", uid);
	country_ptr p_country = country_mgr_t::FindObject(country_id);
	if (NULL != p_country)
	{
		if (country_money_changed > 0)
		{
			p_country->add_money(uid, country_money_changed);
		}
		else
		{
			p_country->consume_money(uid, abs(country_money_changed));
		}
	}*/

	return true;
}

bool global_user_msg_handle_t::on_sync_server_time(uint64_t uid, std::string val)
{
	uint32_t time_delta = string_util_t::string_to_uint32(val);

	common::time_util_t::set_delta_time(time_delta);

	// 转发所有game
	proto::server::eg_sync_data_request sync_data_request;
	proto::server::data_sync_item* p_item = sync_data_request.add_items();
	if (NULL != p_item)
	{
		p_item->set_type(proto::server::data_sync_type_server_time);
		p_item->set_value(boost::lexical_cast<std::string>(val));
	}
	env::server->send_msg_to_all_games(op_cmd::eg_sync_data_request, 0, sync_data_request);

	return true;
}

bool global_user_msg_handle_t::on_gm_add_family_money(uint64_t uid, std::string val)
{
	uint32_t add_val = string_util_t::string_to_uint32(val);

	// 目前只对在线玩家有效
	global_user_ptr p_user = global_user_manager_t::get_global_user_by_uid(uid);
	if (p_user != NULL)
	{
		uint64_t family_id = p_user->get_family_id();
		const family_ptr& p_family = family_manager_t::get_family(family_id);
		if (NULL != p_family)
		{
			p_family->add_money(add_val, proto::server::family_money_change_gm_add, uid);
			ROLE_LOG("user[%lu] add family[%lu] money[%u] success", uid, family_id, add_val);
		}
	}

	return true;
}

bool global_user_msg_handle_t::on_sync_family_contribution(uint64_t uid, std::string val)
{
	uint32_t contribution_val = string_util_t::string_to_uint32(val);

	// 家族贡献只会在玩家在线的情况下更新
	global_user_ptr p_user = global_user_manager_t::get_global_user_by_uid(uid);
	if (p_user != NULL)
	{
		p_user->set_family_contribution(contribution_val);
	}

	return true;
}

bool global_user_msg_handle_t::on_gm_summon_boss(uint64_t uid, std::string val)
{
	uint32_t id = string_util_t::string_to_uint32(val);

	field_boss_manager_t::gm_summon_field_boss(id);

	return true;
}

bool global_user_msg_handle_t::on_sync_head_frame(uint64_t uid, std::string val)
{
	uint32_t head_frame = string_util_t::string_to_uint32(val);
	ROLE_LOG("role[%lu] head_frame[%d]", uid, head_frame);

	global_user_ptr p_user = global_user_manager_t::get_global_user_by_uid(uid);
	if (p_user != NULL)
	{
		p_user->set_hero_plugin(head_frame);
	}
	else
	{
		global_user_data_mgr_t::set_user_hero_plugin(uid, head_frame);
	}

	return true;
}

bool global_user_msg_handle_t::on_sync_main_plugin(uint64_t uid, std::string val)
{
	uint32_t main_plugin = string_util_t::string_to_uint32(val);
	ROLE_LOG("role[%lu] main_plugin[%d]", uid, main_plugin);

	global_user_ptr p_user = global_user_manager_t::get_global_user_by_uid(uid);
	if (p_user != NULL)
	{
		p_user->set_plugin(main_plugin);
	} 
	else
	{
		global_user_data_mgr_t::set_user_plugin(uid, main_plugin);
	}

	return true;
}
