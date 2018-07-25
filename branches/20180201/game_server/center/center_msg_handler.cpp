#include "center_msg_handler.hpp"
#include "network.hpp"
#include "role/role_manager.hpp"
#include "config/config_manager.hpp"
#include "city/city_msg_handler.hpp"
#include "tblh/errcode_enum.hpp"
#include "field_boss/field_boss_msg_handle.hpp"
#include "item/item_msg_handle.hpp"
#include "mail/mail_msg_handle.hpp"
#include "arena/arena_manager.hpp"
#include "king_war/king_war_msg_handle.hpp"
#include "shop/shop_msg_handle.hpp"
#include "role/money_manager.hpp"
#include "red_point/red_point_msg_handle.hpp"
#include "user_troop/user_troop_msg_handler.hpp"
#include "scene/scene_msg_handle.hpp"
#include "dungeon/dungeon_msg_handle.hpp"
#include "expedition/expedition_msg_handle.hpp"
#include "family/family_msg_handle.hpp"
#include "chat/chat_msg_manager.hpp"
#include "scene/scene_manager.hpp"
#include "achieve/achieve_msg_handle.hpp"
#include "sys_notice/sys_notice_msg_handle.hpp"
#include "item/item_manager.hpp"
#include "hero/hero_manager.hpp"
#include "item/package.hpp"
#include "fight/fight_manager.hpp"
#include "item/drop_manager.hpp"
#include "tblh/Comprehensive.tbls.h"
#include "country/country_msg_handle.hpp"
#include "luckydraw/luckydraw_msg_handle.hpp"

USING_NS_NETWORK;
USING_NS_COMMON;

server_msg_handler_t<op_cmd::EG_BEGIN, op_cmd::EG_END, center_msg_handler_t::msg_handler_func> center_msg_handler_t::m_center_msg_handle;
server_msg_handler_t<op_cmd::MG_BEGIN, op_cmd::MG_END, center_msg_handler_t::msg_handler_func> center_msg_handler_t::m_gm_msg_handle;
server_msg_handler_t<op_cmd::RG_BEGIN, op_cmd::RG_END, center_msg_handler_t::msg_handler_func> center_msg_handler_t::m_area_2_game_msg_handle;
server_msg_handler_t<proto::server::data_sync_type_begin, proto::server::data_sync_type_end, center_msg_handler_t::data_sync_func> center_msg_handler_t::m_data_syncer;
proto::server::ge_sync_data_request center_msg_handler_t::m_sync_datas;
uint64_t center_msg_handler_t::m_sync_uid = 0;

bool center_msg_handler_t::init_msg_handler()
{
    bool ret = true;
	// center => game
	ret &= m_center_msg_handle.register_func(op_cmd::eg_sync_data_request, handle_eg_sync_data_request);
	ret &= m_center_msg_handle.register_func(op_cmd::eg_reg_user_scene_request, handle_eg_reg_user_scene_request);
	ret &= m_center_msg_handle.register_func(op_cmd::eg_relogin_scene_request, handle_eg_relogin_scene_request);
	ret &= m_center_msg_handle.register_func(op_cmd::eg_unreg_user_scene_notify, handle_eg_unreg_user_scene_notify);
	ret &= m_center_msg_handle.register_func(op_cmd::eg_chat_get_role_info_request, handle_eg_chat_get_role_info_request);
	ret &= m_center_msg_handle.register_func(op_cmd::eg_user_cross_scene_reply, handle_eg_user_cross_scene_reply);
	ret &= m_center_msg_handle.register_func(op_cmd::eg_offline_data_notify, handle_eg_offline_data_notify);
	ret &= m_center_msg_handle.register_func(op_cmd::eg_tower_trigger_notify, handle_eg_tower_trigger_notify);
	ret &= m_center_msg_handle.register_func(op_cmd::eg_two_side_friend_notify, handle_eg_two_side_friend_notify);
	ret &= m_center_msg_handle.register_func(op_cmd::eg_redbag_draw_notify, handle_eg_redbag_draw_notify);
	ret &= m_center_msg_handle.register_func(op_cmd::eg_offline_arena_fight_request, handle_eg_offline_arena_fight_request);
	ret &= m_center_msg_handle.register_func(op_cmd::eg_offline_arena_record_award_notify, handle_eg_offline_arena_record_award_notify);
	ret &= m_center_msg_handle.register_func(op_cmd::eg_offline_arena_class_award_notify, handle_eg_offline_arena_class_award_notify);
	ret &= m_center_msg_handle.register_func(op_cmd::eg_offline_arena_buy_reply, handle_eg_offline_arena_buy_reply);

	// gm => game
	ret &= m_gm_msg_handle.register_func(op_cmd::mg_reload_config_notify,		handle_mg_reload_config_notify);

	// area => game
	ret &= m_area_2_game_msg_handle.register_func(op_cmd::rg_arena_match_reply, handle_rg_arena_match_reply);
	ret &= m_area_2_game_msg_handle.register_func(op_cmd::rg_arena_battle_result_notify, handle_rg_arena_battle_result_notify);
	ret &= m_area_2_game_msg_handle.register_func(op_cmd::rg_get_arena_rank_data_reply, handle_rg_get_arena_rank_data_reply);
	ret &= m_area_2_game_msg_handle.register_func(op_cmd::rg_world_cup_play_reply, handle_rg_world_cup_play_reply);
	ret &= m_area_2_game_msg_handle.register_func(op_cmd::rg_world_cup_bet_reply, handle_rg_world_cup_bet_reply);
	ret &= m_area_2_game_msg_handle.register_func(op_cmd::rg_world_cup_bet_reward_reply, handle_rg_world_cup_bet_reward_reply);
	ret &= m_area_2_game_msg_handle.register_func(op_cmd::rg_arena_cancel_match_reply, handle_rg_arena_cancel_match_reply);
	ret &= m_area_2_game_msg_handle.register_func(op_cmd::rg_arena_self_rank_reply, handle_rg_arena_self_rank_reply);

	// city message
	if (!city_msg_handler_t::init_msg_handler())
	{
		log_error("init city msg handler failed!");
		return false;
	}

	// scene message
	if (!scene_msg_handle_t::init_center_msg_handler())
	{
		log_error("init scene msg handler failed!");
		return false;
	}

	// country
	if (!country_msg_handle_t::init_center_msg_handler())
	{
		log_error("init country msg handler failed!");
		return false;
	}

	// family message
	if (!family_msg_handle_t::init_center_msg_handler())
	{
		log_error("init family msg handler failed!");
		return false;
	}

	// dungeon message
	if (!dungeon_msg_handle_t::init_center_msg_handler())
	{
		log_error("init dungeon_msg handler failed!");
		return false;
	}

	// field message
	if (!field_boss_msg_handle_t::init_center_msg_handler())
	{
		log_error("init field_boss msg handler failed!");
		return false;
	}

	// mail message
	if (!mail_msg_handle_t::init_center_msg_handler())
	{
		log_error("init mail msg handler failed!");
		return false;
	}

	// city message
	if (!item_msg_handle_t::init_center_msg_handler())
	{
		log_error("init item center msg handler failed!");
		return false;
	}

	// king_war message
	if (!king_war_msg_handle_t::init_center_msg_handler())
	{
		log_error("init king war msg handler failed!");
		return false;
	}

	// shop_war message
	if (!shop_msg_handle_t::init_center_msg_handler())
	{
		log_error("init shop msg handler failed!");
		return false;
	}

	// red_point message
	if (!red_point_msg_handle_t::init_center_msg_handler())
	{
		log_error("init red_point msg handler failed!");
		return false;
	}

	// user_troop message
	if (!user_troop_msg_handle_t::init_center_msg_handler())
	{
		log_error("init user_troop msg handler failed!");
		return false;
	}

	// expedition message
	if (!expedition_msg_handle_t::init_center_msg_handler())
	{
		log_error("init expedition msg handler failed!");
		return false;
	}

	// achieve message
	if (!achieve_msg_handle_t::init_center_msg_handler())
	{
		log_error("init achieve msg handler failed!");
		return false;
	}

	// sys_notice message
	if (!sys_notice_msg_handle_t::init_msg_handler())
	{
		log_error("init sys_notice msg handler failed!");
		return false;
	}

	if (!luckydraw_msg_handle_t::init_center_msg_handler())
	{
		log_error("init luckydraw msg handler failed!");
		return false;
	}


    if(!init_data_syncers())
    {
        log_error("init data syncers failed!");
		return false;
    }
    return ret;
}

bool center_msg_handler_t::init_data_syncers()
{
    bool ret = true;
	ret &= m_data_syncer.register_func(proto::server::data_sync_type_country_officer, on_sync_country_officer);
	ret &= m_data_syncer.register_func(proto::server::data_sync_type_family_officer, on_sync_family_officer);
	ret &= m_data_syncer.register_func(proto::server::data_sync_type_server_time, on_sync_server_time);

    return ret;
}

// gm => game---------------------------------------------------------------------------------------------------
bool center_msg_handler_t::handle_mg_reload_config_notify(const network::msg_buf_ptr& msg_buf)
{
	reload_config();

	return true;
}


bool center_msg_handler_t::handle_rg_arena_match_reply(const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::rg_arena_match_reply);
	auto role = role_manager_t::find_role(uid);
	if (NULL == role)
	{
		log_error("null role");
		return false;
	}

	arena_manager_t::deal_match_rank_result(role, msg);

	return true;
}

bool center_msg_handler_t::handle_rg_get_arena_rank_data_reply(const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::rg_get_arena_rank_data_reply);
	auto role = role_manager_t::find_role(uid);
	if (NULL == role)
	{
		log_error("null role");
		return false;
	}

	proto::client::gc_get_arena_rank_data_reply reply;
	if(msg.rand_data().ByteSize() > 0)
		reply.mutable_rand_data()->CopyFrom(msg.rand_data());

	role->send_msg_to_client(op_cmd::gc_get_arena_rank_data_reply, reply);

	return true;
}


bool center_msg_handler_t::handle_rg_world_cup_play_reply(const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::rg_world_cup_play_reply);
	auto role = role_manager_t::find_role(uid);
	if (NULL == role)
	{
		log_error("null role");
		return false;
	}

	proto::client::gc_world_cup_play_reply reply;
	reply.set_reply_code(msg.reply_code());
	role->send_msg_to_client(op_cmd::gc_world_cup_play_reply, reply);

	return true;
}


bool center_msg_handler_t::handle_rg_world_cup_bet_reply(const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::rg_world_cup_bet_reply);
	auto p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("null role");
		return false;
	}
	proto::client::gc_world_cup_bet_reply reply;
	reply.set_reply_code(msg.reply_code());
	if (msg.reply_code() == errcode_enum::error_ok)
	{
		proto::common::personal_info *p_info = reply.mutable_rcd()->mutable_per_info();
		p_role->get_personal_info(p_info);
		reply.mutable_self_bet()->CopyFrom(msg.self_bet());
	}
	else
	{
		money_manager_t::add_money(p_role, proto::common::MONEY_TYPE_COPPER, msg.bet(), log_enum::source_type_world_cup_bet, 0);
		log_warn("role[%lu] bet failed, return money", uid);
		reply.set_odds(msg.odds());
	}

	p_role->send_msg_to_client(op_cmd::gc_world_cup_bet_reply, reply);
	return true;
}


bool center_msg_handler_t::handle_rg_world_cup_bet_reward_reply(const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::rg_world_cup_bet_reward_reply);
	auto p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("null role");
		return false;
	}
	proto::client::gc_world_cup_bet_reward_reply reply;
	reply.set_reply_code(msg.reply_code());
	if (msg.reply_code() == errcode_enum::error_ok)
	{
		money_manager_t::add_money(p_role, proto::common::MONEY_TYPE_COPPER, msg.reward(), log_enum::source_type_world_cup_reward_bet, 0);
		proto::common::personal_info *p_info = reply.mutable_rcd()->mutable_per_info();
		p_role->get_personal_info(p_info);
	}
	p_role->send_msg_to_client(op_cmd::gc_world_cup_bet_reward_reply, reply);
	return true;
}

bool center_msg_handler_t::handle_rg_arena_cancel_match_reply(const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::rg_arena_cancel_match_reply);
	auto p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("null role");
		return false;
	}
	arena_manager_t::cancel_match(p_role);
	proto::client::gc_arena_cancel_match_reply reply;
	reply.set_reply_code(errcode_enum::error_ok);
	p_role->send_msg_to_client(op_cmd::gc_arena_cancel_match_reply, reply);
	return true;
}

bool center_msg_handler_t::handle_rg_arena_self_rank_reply(const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::rg_arena_self_rank_reply);
	auto p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("null role");
		return false;
	}

	proto::client::gc_arena_self_rank_reply reply;
	reply.set_rank(msg.rank());
	p_role->send_msg_to_client(op_cmd::gc_arena_self_rank_reply, reply);

	return true;
}

bool center_msg_handler_t::handle_rg_arena_battle_result_notify(const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::rg_arena_battle_result_notify);
	auto role = role_manager_t::find_role(uid);
	if (NULL == role)
	{
		log_error("null role");
		return false;
	}

	arena_manager_t::deal_arena_rank_battle_result(role, msg);

	return true;
}

bool center_msg_handler_t::handle_eg_sync_data_request(const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::eg_sync_data_request);

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


bool center_msg_handler_t::handle_eg_reg_user_scene_request(const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::eg_reg_user_scene_request);

	role_manager_t::scene_reg_user(uid, msg);

	return true;
}


bool center_msg_handler_t::handle_eg_relogin_scene_request(const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::eg_relogin_scene_request);

	role_manager_t::scene_relogin_user(uid, msg.gate_id());

	return true;
}


bool center_msg_handler_t::handle_eg_unreg_user_scene_notify(const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::eg_unreg_user_scene_notify);

	role_manager_t::center_unreg_user(uid, msg.unreg_code());

	return true;
}


bool center_msg_handler_t::handle_eg_chat_get_role_info_request(const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::eg_chat_get_role_info_request);

	role_ptr p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role role[%lu]",uid);
		return false;
	}

	proto::server::ge_chat_get_role_info_reply reply;
	uint32_t reply_code = errcode_enum::error_ok;
	proto::common::chat_common_data* p_chat_notify = reply.mutable_chat_notify();
	p_chat_notify->CopyFrom(msg.chat_notify());
	proto::common::chat_common_role_info* p_chat_data = reply.mutable_chat_role_info();
	std::vector<proto::common::chat_param> param_vec;
	for (int i = 0; i < msg.param_list_size(); ++i)
	{
		param_vec.push_back(msg.param_list(i));
	}
	if (!chat_msg_manager_t::chat_get_role_info(uid, p_chat_data, param_vec))
	{
		log_error("role[%lu] personal chat query role info fail!!!",uid);
		reply_code = errcode_enum::notice_unknown;
	}

	reply.set_reply_code(reply_code);
	env::server->send_msg_to_center(op_cmd::ge_chat_get_role_info_reply, uid, reply);
	return true;
}


bool center_msg_handler_t::handle_eg_user_cross_scene_reply(const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::eg_user_cross_scene_reply);

	role_ptr p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role role[%lu]", uid);
		return false;
	}

	if (msg.reply_code() != common::errcode_enum::error_ok)
	{
		log_error("role[%lu] cross server scene failed, reasion: %u", uid, msg.reply_code());
		return false;
	}

	scene_manager_t::center_cross_check_reply(p_role, msg.data());

	return true;
}


bool center_msg_handler_t::handle_eg_offline_data_notify(const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::eg_offline_data_notify);
	role_ptr p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role role[%lu]", uid);
		return false;
	}
	p_role->update_offline_data(msg.ord());
	return true;
}


bool center_msg_handler_t::handle_eg_tower_trigger_notify(const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::eg_tower_trigger_notify);
	role_ptr p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role role[%lu]", uid);
		return false;
	}
	tower_ptr p_tower = p_role->get_tower();
	if (NULL == p_tower)
	{
		log_error("NULL == p_role role[%lu]", uid);
		return false;
	}
	for (int32_t i = 0 ; i < msg.tower_trigger_size(); ++i)
	{
		p_tower->add_other_trigger(msg.tower_trigger(i));
	}
	return true;
}

bool center_msg_handler_t::handle_eg_two_side_friend_notify(const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::eg_two_side_friend_notify);
	role_ptr p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role role[%lu]", uid);
		return false;
	}
	tower_ptr p_tower = p_role->get_tower();
	if (NULL == p_tower)
	{
		log_error("NULL == p_role role[%lu]", uid);
		return false;
	}
	p_tower->send_all_self_trigger(msg.friend_uid());
	return true;
}


bool center_msg_handler_t::handle_eg_redbag_draw_notify(const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::eg_redbag_draw_notify);
	role_ptr p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role role[%lu]", uid);
		return false;
	}
	p_role->draw_notify(msg.money());
	return true;
}

bool center_msg_handler_t::handle_eg_offline_arena_fight_request(const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::eg_offline_arena_fight_request);
	proto::server::ge_offline_arena_fight_reply reply;
	reply.set_is_win(0);
	role_ptr p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role role[%lu]", uid);
		env::server->send_msg_to_center(op_cmd::ge_offline_arena_fight_reply, uid, reply);
		return false;
	}
	const proto::common::obj_single& os = msg.enemy_data().obj();
	if (os.object_type() == proto::common::SCENEOBJECT_MONSTER)
	{
		uint32_t against_id = os.monster_id();
		auto p_against_conf = GET_CONF(FightAgainst, against_id);
		if (NULL == p_against_conf)
		{
			log_error("NULL == p_against_conf[%d]", against_id);
			env::server->send_msg_to_center(op_cmd::ge_offline_arena_fight_reply, uid, reply);
			return false;
		}
		fight_manager_t::fill_hero_data(proto::common::combat_camp_2, against_id, CAMP_ENEMY_UNIQUE_OFFSET, 0, 0, p_against_conf->get_against_ids(), msg.mutable_enemy_data()->mutable_heros(), 0);
	}
	proto::common::fight_param fp;
	fp.set_type(proto::common::fight_type_offline_arena);
	fp.set_offline_arena_class(msg.offline_arena_class());
	fight_manager_t::fight_pve(p_role, msg.enemy_data(), &fp);
	return true;
}


bool center_msg_handler_t::handle_eg_offline_arena_record_award_notify(const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::eg_offline_arena_record_award_notify);
	role_ptr p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role role[%lu]", uid);
		return false;
	}
	proto::client::gc_offline_arena_record_award_reply reply;
	for (int32_t i = 0; i < msg.item_list_size(); ++i)
	{
		item_manager_t::add_item_ex(p_role, msg.item_list(i).item_id(), msg.item_list(i).item_num(), log_enum::source_type_offline_arena_record_award, 0, reply.mutable_rcd());
	}
	p_role->send_msg_to_client(op_cmd::gc_offline_arena_record_award_reply, reply);
	return true;
}

bool center_msg_handler_t::handle_eg_offline_arena_class_award_notify(const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::eg_offline_arena_class_award_notify);
	role_ptr p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role role[%lu]", uid);
		return false;
	}
	proto::client::gc_offline_arena_class_award_notify ntf;
	for (int32_t i = 0; i < msg.drop_id_size(); ++i)
	{
		drop_manager_t::drop(p_role, msg.drop_id(i), log_enum::source_type_offline_arena_class_award, proto::common::drop_sys_type_offline_arena, 0, ntf.mutable_rcd());
	}
	ntf.set_cur_class(msg.cur_class());
	p_role->send_msg_to_client(op_cmd::gc_offline_arena_class_award_notify, ntf);
	return true;
}

bool center_msg_handler_t::handle_eg_offline_arena_buy_reply(const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::eg_offline_arena_buy_reply);
	role_ptr p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role role[%lu]", uid);
		return false;
	}
	uint32_t reply_code = msg.reply_code();
	proto::client::gc_offline_arena_buy_reply reply;
	reply.set_reply_code(reply_code);
	if (reply_code != errcode_enum::error_ok)
	{
		auto p_conf = GET_CONF(Comprehensive, comprehensive_common::offline_arena_cost_number);
		uint32_t money_type = GET_COMPREHENSIVE_VALUE_2(p_conf);
		money_manager_t::add_money(p_role, (proto::common::MONEY_TYPE)money_type, msg.money(), log_enum::source_type_offline_arena_buy, 0);
	}
	p_role->get_personal_info(reply.mutable_rcd()->mutable_per_info());
	p_role->send_msg_to_client(op_cmd::gc_offline_arena_buy_reply, reply);
	return true;
}


bool center_msg_handler_t::on_sync_family_officer(uint64_t uid, std::string val)
{
	role_ptr p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("role[%lu] null error", uid);
		return false;
	}
	uint32_t officer = string_util_t::string_to_uint32(val);
	p_role->set_family_officer(officer);

	return true;
}


bool center_msg_handler_t::on_sync_server_time(uint64_t uid, std::string val)
{
	uint32_t time_delta = string_util_t::string_to_uint32(val);

	common::time_util_t::set_delta_time(time_delta);

	return true;
}

bool center_msg_handler_t::on_sync_country_officer(uint64_t uid, std::string val)
{
	role_ptr pRole = role_manager_t::find_role(uid);
	if (NULL == pRole)
	{
		log_error("role[%lu] null error", uid);
		return false;
	}

    uint32_t country_officer = string_util_t::string_to_uint32(val);
	pRole->set_country_officer(country_officer);

    return true;
}
