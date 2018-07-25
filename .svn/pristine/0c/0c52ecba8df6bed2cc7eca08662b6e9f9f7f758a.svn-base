#include "achieve_msg_handle.hpp"
#include "achieve/achieve_manager.hpp"
#include "gate/gate_msg_handler.hpp"
#include "achieve/achieve_method.hpp"
#include "active_reward_manager.hpp"
#include "center/center_msg_handler.hpp"
achieve_bind_func_t<proto::common::Achieve_Event_Type::ACHIEVE_EVENT_TYPE_BEGIN, proto::common::Achieve_Event_Type::ACHIEVE_EVENT_TYPE_END, achieve_msg_handle_t::event_msg_handler_func> achieve_msg_handle_t::m_event_msg_handle;

bool achieve_msg_handle_t::init_msg_handler()
{
	gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_get_achieve_infolist_request, handle_cg_get_achieve_infolist_request);
	gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_get_achieve_record_request, handle_cg_get_achieve_record_request);
	gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_get_achieve_reward_request, handle_cg_get_achieve_reward_request);
	gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_active_reward_list_request, handle_cg_get_active_reward_list_request);
	gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_get_active_reward_request, handle_cg_get_active_reward_request);
	return true;
}

bool achieve_msg_handle_t::init_event_handler()
{
	achieve_manager_t::init();
	bool bresult = true;
	bresult &= m_event_msg_handle.register_func(proto::common::Achieve_Event_Type::ACHIEVE_MAIN_ROLE_LEVEL, achieve_common_t::achieve_main_role_level);
	bresult &= m_event_msg_handle.register_func(proto::common::Achieve_Event_Type::ACHIEVE_HERO_NUMBER, achieve_common_t::achieve_total_unify_handle);
	bresult &= m_event_msg_handle.register_func(proto::common::Achieve_Event_Type::ACHIEVE_EQUIP_RECAST, achieve_common_t::achieve_total_unify_handle);

	bresult &= m_event_msg_handle.register_func(proto::common::Achieve_Event_Type::ACHIEVE_TOTAL_COPPER, achieve_common_t::achieve_total_unify_handle);
	bresult &= m_event_msg_handle.register_func(proto::common::Achieve_Event_Type::ACHIEVE_TREASURE_ACTIVITY_NUMBER, achieve_common_t::achieve_total_unify_handle);
	bresult &= m_event_msg_handle.register_func(proto::common::Achieve_Event_Type::ACHIEVE_TASK_FINISH_COUNTRY_NUMBER, achieve_common_t::achieve_total_unify_handle);
	
	bresult &= m_event_msg_handle.register_func(proto::common::Achieve_Event_Type::ACHIEVE_TASK_FINISH_FAMILY_NUMBER, achieve_common_t::achieve_total_unify_handle);
	bresult &= m_event_msg_handle.register_func(proto::common::Achieve_Event_Type::ACHIEVE_TRADE_NUMBER, achieve_common_t::achieve_total_unify_handle);
	bresult &= m_event_msg_handle.register_func(proto::common::Achieve_Event_Type::ACHIEVE_TRADE_TOTAL_GAIN_NUMBER, achieve_common_t::achieve_total_unify_handle);
	
	bresult &= m_event_msg_handle.register_func(proto::common::Achieve_Event_Type::ACHIEVE_TASK_FINISH_NUMBER, achieve_common_t::achieve_total_unify_handle);
	bresult &= m_event_msg_handle.register_func(proto::common::Achieve_Event_Type::ACHIEVE_PACKAGE_COPPER_NUMBER, achieve_common_t::achieve_direct_replace_model_handle);
	bresult &= m_event_msg_handle.register_func(proto::common::Achieve_Event_Type::ACHIEVE_EMPIRE_CITY_CHALLENGE_NUMBER, achieve_common_t::achieve_total_unify_handle);
	
	bresult &= m_event_msg_handle.register_func(proto::common::Achieve_Event_Type::ACHIEVE_HERO_NUMBER_LEVEL, achieve_common_t::achieve_hero_cultivate);
	bresult &= m_event_msg_handle.register_func(proto::common::Achieve_Event_Type::ACHIEVE_HERO_NUMBER_TYPE, achieve_common_t::achieve_hero_cultivate);
	bresult &= m_event_msg_handle.register_func(proto::common::Achieve_Event_Type::ACHIEVE_HERO_NUMBER_STAR, achieve_common_t::achieve_hero_cultivate);
	
	bresult &= m_event_msg_handle.register_func(proto::common::Achieve_Event_Type::ACHIEVE_HERO_NUMBER_SKILL_LEVEL, achieve_common_t::achieve_hero_cultivate);
	bresult &= m_event_msg_handle.register_func(proto::common::Achieve_Event_Type::ACHIEVE_HERO_NUMBER_TALENT_LEVEL, achieve_common_t::achieve_hero_cultivate);
	bresult &= m_event_msg_handle.register_func(proto::common::Achieve_Event_Type::ACHIEVE_EQUIP_TYPE_NUMBER, achieve_common_t::achieve_unify_handle);

	bresult &= m_event_msg_handle.register_func(proto::common::Achieve_Event_Type::ACHIEVE_SHOP_BUY_NUMBER, achieve_common_t::achieve_unify_handle);
	bresult &= m_event_msg_handle.register_func(proto::common::Achieve_Event_Type::ACHIEVE_MOUNT_NUMBER_TYPE, achieve_common_t::achieve_unify_handle);
	bresult &= m_event_msg_handle.register_func(proto::common::Achieve_Event_Type::ACHIEVE_COUNTRY_BOUNTY_STAR_NUMBER, achieve_common_t::achieve_unify_handle);
	
	bresult &= m_event_msg_handle.register_func(proto::common::Achieve_Event_Type::ACHIEVE_CHANNEL_CHAT, achieve_common_t::achieve_unify_handle);
	bresult &= m_event_msg_handle.register_func(proto::common::Achieve_Event_Type::ACHIEVE_FAMILY, achieve_common_t::achieve_unify_handle);
	bresult &= m_event_msg_handle.register_func(proto::common::Achieve_Event_Type::ACHIEVE_TREASURE_ACTIVITY_SPECIAL_EVENT_NUMBER, achieve_common_t::achieve_unify_handle);

	bresult &= m_event_msg_handle.register_func(proto::common::Achieve_Event_Type::ACHIEVE_SUCCESSION_LOGIN, achieve_common_t::achieve_replace_model);
	bresult &= m_event_msg_handle.register_func(proto::common::Achieve_Event_Type::ACHIEVE_TOTAL_LOGIN, achieve_common_t::achieve_unify_handle);
	bresult &= m_event_msg_handle.register_func(proto::common::Achieve_Event_Type::ACHIEVE_TASK_FINISH_CHASE_GHOST_NUMBER, achieve_common_t::achieve_total_unify_handle);
	
	bresult &= m_event_msg_handle.register_func(proto::common::Achieve_Event_Type::ACHIEVE_TASK_FINISH_TRIAL_NUMBER, achieve_common_t::achieve_total_unify_handle);
	bresult &= m_event_msg_handle.register_func(proto::common::Achieve_Event_Type::ACHIEVE_TASK_FINISH_EXP_CIRCLE_NUMBER, achieve_common_t::achieve_total_unify_handle);
	bresult &= m_event_msg_handle.register_func(proto::common::Achieve_Event_Type::ACHIEVE_TASK_FINISH_STAR_TRIAL_NUMBER, achieve_common_t::achieve_unify_handle);
	
	bresult &= m_event_msg_handle.register_func(proto::common::Achieve_Event_Type::ACHIEVE_CHASE_GHOST_CIRCLE_NUMBER, achieve_common_t::achieve_total_unify_handle);
	bresult &= m_event_msg_handle.register_func(proto::common::Achieve_Event_Type::ACHIEVE_TASK_EXP_CIRCLE_ITEM_QUALITY_NUMBER, achieve_common_t::achieve_unify_handle);
	bresult &= m_event_msg_handle.register_func(proto::common::Achieve_Event_Type::ACHIEVE_FIELD_BOSS_KILL_NUMBER, achieve_common_t::achieve_total_unify_handle);
	
	bresult &= m_event_msg_handle.register_func(proto::common::Achieve_Event_Type::ACHIEVE_ARENA_BATTLE_SINGLE_WIN_NUMBER, achieve_common_t::achieve_replace_model);
	bresult &= m_event_msg_handle.register_func(proto::common::Achieve_Event_Type::ACHIEVE_ARENA_BATTLE_TOTAL_WIN_NUMBER, achieve_common_t::achieve_total_unify_handle);
	bresult &= m_event_msg_handle.register_func(proto::common::Achieve_Event_Type::ACHIEVE_MAIN_ROLE_FIGHT, achieve_common_t::achieve_replace_model);
	
	bresult &= m_event_msg_handle.register_func(proto::common::Achieve_Event_Type::ACHIEVE_HAVE_FRIEND_NUMBER, achieve_common_t::achieve_total_unify_handle);
	bresult &= m_event_msg_handle.register_func(proto::common::Achieve_Event_Type::ACHIEVE_TRADE_SINGLE_GAIN, achieve_common_t::achieve_replace_model);
	bresult &= m_event_msg_handle.register_func(proto::common::Achieve_Event_Type::ACHIEVE_TOWER_LEVEL, achieve_common_t::achieve_replace_model);

	bresult &= m_event_msg_handle.register_func(proto::common::Achieve_Event_Type::ACHIEVE_EXPEDITION_SINGLE_POINT, achieve_common_t::achieve_replace_model);
	bresult &= m_event_msg_handle.register_func(proto::common::Achieve_Event_Type::ACHIEVE_EXPEDITION_STORE_BUY_NUMBER, achieve_common_t::achieve_total_unify_handle);
	bresult &= m_event_msg_handle.register_func(proto::common::Achieve_Event_Type::ACHIEVE_EXPEDITION_STORE_TOTAL_COST_HONOUR, achieve_common_t::achieve_total_unify_handle);
	
	bresult &= m_event_msg_handle.register_func(proto::common::Achieve_Event_Type::ACHIEVE_EXPEDITION_HELP_OTHERS, achieve_common_t::achieve_total_unify_handle);
	bresult &= m_event_msg_handle.register_func(proto::common::Achieve_Event_Type::ACHIEVE_EXPEDITION_CHALLENGE_PASS_NUMBER, achieve_common_t::achieve_total_unify_handle);
	bresult &= m_event_msg_handle.register_func(proto::common::Achieve_Event_Type::ACHIEVE_FAMILY_WAR_NUMBER, achieve_common_t::achieve_total_unify_handle);
	
	bresult &= m_event_msg_handle.register_func(proto::common::Achieve_Event_Type::ACHIEVE_FAMILY_WAR_KILL_PLAYER_NUMBER, achieve_common_t::achieve_total_unify_handle);
	bresult &= m_event_msg_handle.register_func(proto::common::Achieve_Event_Type::ACHIEVE_FAMILY_WAR_DRAGON_HARM, achieve_common_t::achieve_total_unify_handle);
	bresult &= m_event_msg_handle.register_func(proto::common::Achieve_Event_Type::ACHIEVE_FAMILY_WAR_OCCUPIED_SUN_OR_MOON_NUMBER, achieve_common_t::achieve_total_unify_handle);

	bresult &= m_event_msg_handle.register_func(proto::common::Achieve_Event_Type::ACHIEVE_KING_WAR_NUMBER, achieve_common_t::achieve_total_unify_handle);
	bresult &= m_event_msg_handle.register_func(proto::common::Achieve_Event_Type::ACHIEVE_KING_WAR_KILL_PLAYER_NUMBER, achieve_common_t::achieve_total_unify_handle);
	bresult &= m_event_msg_handle.register_func(proto::common::Achieve_Event_Type::ACHIEVE_KING_WAR_DRAGON_HARM, achieve_common_t::achieve_total_unify_handle);
	
	bresult &= m_event_msg_handle.register_func(proto::common::Achieve_Event_Type::ACHIEVE_KING_WAR_QUICK_REVIVE, achieve_common_t::achieve_total_unify_handle);
	bresult &= m_event_msg_handle.register_func(proto::common::Achieve_Event_Type::ACHIEVE_KING_WAR_START_WEAPON, achieve_common_t::achieve_total_unify_handle);
	bresult &= m_event_msg_handle.register_func(proto::common::Achieve_Event_Type::ACHIEVE_GRADE_PASS_DUNGEON_NUMBER, achieve_common_t::achieve_unify_handle);

	bresult &= m_event_msg_handle.register_func(proto::common::Achieve_Event_Type::ACHIEVE_DUNGEON_TOTAL_PASS, achieve_common_t::achieve_total_unify_handle);
	bresult &= m_event_msg_handle.register_func(proto::common::Achieve_Event_Type::ACHIEVE_RANK_TYPE_LEVEL, achieve_common_t::achieve_total_unify_handle);
	bresult &= m_event_msg_handle.register_func(proto::common::Achieve_Event_Type::ACHIEVE_PK_VALUE, achieve_common_t::achieve_total_unify_handle);

	bresult &= m_event_msg_handle.register_func(proto::common::Achieve_Event_Type::ACHIEVE_HERO_UP_LEVEL_NUMBER, achieve_common_t::achieve_total_unify_handle);
	bresult &= m_event_msg_handle.register_func(proto::common::Achieve_Event_Type::ACHIEVE_STRENGTHEN_EQUIP_NUMBER, achieve_common_t::achieve_total_unify_handle);
	bresult &= m_event_msg_handle.register_func(proto::common::Achieve_Event_Type::ACHIEVE_TOWER_NUMBER, achieve_common_t::achieve_total_unify_handle);
	bresult &= m_event_msg_handle.register_func(proto::common::Achieve_Event_Type::ACHIEVE_PATROL_KILL_MONSTER_GROUP_NUMBER, achieve_common_t::achieve_total_unify_handle);
	bresult &= m_event_msg_handle.register_func(proto::common::Achieve_Event_Type::ACHIEVE_OPEN_GENERAL, achieve_common_t::achieve_unify_handle);

	bresult &= m_event_msg_handle.register_func(proto::common::Achieve_Event_Type::ACHIEVE_FIELD_BOSS_ATTACK_NUMBER, achieve_common_t::achieve_total_unify_handle);
	bresult &= m_event_msg_handle.register_func(proto::common::Achieve_Event_Type::ACHIEVE_EXPEDITION_WIN_NUMBER, achieve_common_t::achieve_total_unify_handle);
	bresult &= m_event_msg_handle.register_func(proto::common::Achieve_Event_Type::ACHIEVE_ELITE_PAGE_QUICK_FIGHT_NUMBER, achieve_common_t::achieve_total_unify_handle);
	bresult &= m_event_msg_handle.register_func(proto::common::Achieve_Event_Type::ACHIEVE_TOWER_QUICK_FIGHT_NUMBER, achieve_common_t::achieve_total_unify_handle);

	bresult &= m_event_msg_handle.register_func(proto::common::Achieve_Event_Type::ACHIEVE_OFFLINE_BATTLE_TOTAL_POINT, achieve_common_t::achieve_total_unify_handle);
	bresult &= m_event_msg_handle.register_func(proto::common::Achieve_Event_Type::ACHIEVE_ARENA_BATTLE_TOTAL_NUMBER, achieve_common_t::achieve_total_unify_handle);
	bresult &= m_event_msg_handle.register_func(proto::common::Achieve_Event_Type::ACHIEVE_BASE_PASS_DUNGEON_NUMBER, achieve_common_t::achieve_unify_handle);

	return bresult;
}

bool achieve_msg_handle_t::init_center_msg_handler()
{
	center_msg_handler_t::m_center_msg_handle.register_func(op_cmd::eg_notify_progress_state_notify, handle_eg_notify_progress_state_notify);
	return true;
}

bool achieve_msg_handle_t::handle_cg_get_achieve_infolist_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_get_achieve_infolist_request);
	achieve_manager_t::get_achieve_info_list(uid);
	return true;
}

bool achieve_msg_handle_t::handle_cg_get_achieve_record_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_get_achieve_record_request);
	achieve_manager_t::get_person_achieve_record_list(uid);
	return true;
}

bool achieve_msg_handle_t::handle_cg_get_achieve_reward_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_get_achieve_reward_request);
	auto p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role[%lu]", uid);
		return false;
	}
	proto::client::gc_get_achieve_reward_reply reply;
	uint32_t type = msg.type();
	uint32_t tid = msg.tid();
	uint32_t level = msg.level();
	uint32_t reply_code = achieve_manager_t::get_achieve_reward(uid, type, tid, level, reply.mutable_rcd());

	reply.set_reply_code(reply_code);
	p_role->send_msg_to_client(op_cmd::gc_get_achieve_reward_reply, reply);

	return true;
}

bool achieve_msg_handle_t::handle_cg_get_active_reward_list_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_active_reward_list_request);
	active_reward_manager_t::get_active_reward_list(uid);
	return true;
}

bool achieve_msg_handle_t::handle_cg_get_active_reward_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_get_active_reward_request);

	role_ptr p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role[%lu]", uid);
		return false;	
	}
	proto::client::gc_get_active_reward_reply reply;
	uint32_t reply_code = active_reward_manager_t::get_active_reward_gift(uid,msg.tid(),reply.mutable_rcd());
	reply.set_reply_code(reply_code);
	reply.set_tid(msg.tid());
	p_role->send_msg_to_client(op_cmd::gc_get_active_reward_reply, reply);
	return true;
}

bool achieve_msg_handle_t::handle_eg_notify_progress_state_notify(const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::eg_notify_progress_state_notify);
	achieve_common_t::notify_progress_state(uid, (proto::common::Achieve_Event_Type)msg.type(), msg.param1(), msg.param2());
	return true;
}

