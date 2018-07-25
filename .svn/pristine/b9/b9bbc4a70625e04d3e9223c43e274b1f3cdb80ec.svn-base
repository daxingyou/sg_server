#include "family_msg_handle.hpp"
#include "config/config_manager.hpp"
#include "gate/gate_msg_handler.hpp"
#include "hero/hero_manager.hpp"
#include "tblh/Comprehensive.tbls.h"
#include "common.pb.h"
#include "role/role_manager.hpp"
#include "tblh/errcode_enum.hpp"
#include "role/money_manager.hpp"
#include "center/center_msg_handler.hpp"
#include "family_manager.hpp"
#include "scene/scene_manager.hpp"

bool family_msg_handle_t::init_gate_msg_handler()
{
	bool ret = true;

	ret &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_create_family_request, handle_cg_create_family_requsest);
	ret &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_impeach_family_general_request, handle_cg_impeach_family_general_request);

	ret &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_leave_family_war_request, handle_cg_leave_family_war_request);
	ret &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_family_war_member_request, handle_cg_family_war_member_request);
	ret &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_ask_formations_info_request, handle_cg_ask_formations_info_request);
	ret &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_formations_hero_up_request, handle_cg_formations_hero_up_request);
	ret &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_formations_hero_revive_request, handle_cg_formations_hero_revive_request);
	ret &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_family_war_attack_long_request, handle_cg_family_war_attack_long_request);
	ret &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_family_war_fight_data_request, handle_cg_family_war_fight_data_request);
	ret &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_ask_enter_battlefield_requet, handle_cg_ask_enter_battlefield_requet);
	ret &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_long_hurt_info_request, handle_cg_long_hurt_info_request);
	ret &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_family_war_result_info_request, handle_cg_family_war_result_info_request);
	ret &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_formations_hero_down_request, handle_cg_formations_hero_down_request);
	ret &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_get_family_activities_request, handle_cg_get_family_activities_request);

	ret &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_family_prayer_request, handle_cg_family_prayer_request);
	ret &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_family_prayer_gift_request, handle_cg_family_prayer_gift_request);

	ret &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_family_shop_buy_request, handle_cg_family_shop_buy_request);

	return ret;
}


bool family_msg_handle_t::init_center_msg_handler()
{
	bool ret = true;

	ret &= center_msg_handler_t::m_center_msg_handle.register_func(op_cmd::eg_create_family_reply, handle_eg_create_family_reply);
	ret &= center_msg_handler_t::m_center_msg_handle.register_func(op_cmd::eg_impeach_family_general_reply, handle_eg_impeach_family_general_reply);
	ret &= center_msg_handler_t::m_center_msg_handle.register_func(op_cmd::eg_notify_game_impeach_result, handle_eg_notify_game_impeach_result);
	ret &= center_msg_handler_t::m_center_msg_handle.register_func(op_cmd::eg_update_family_info_notify, handle_eg_update_family_info_notify);
	ret &= center_msg_handler_t::m_center_msg_handle.register_func(op_cmd::eg_family_country_change_notify, handle_eg_family_country_change_notify);

	ret &= center_msg_handler_t::m_center_msg_handle.register_func(op_cmd::eg_prepare_family_war_request, handle_eg_prepare_family_war_request);
	ret &= center_msg_handler_t::m_center_msg_handle.register_func(op_cmd::eg_enter_family_war_scene_request, handle_eg_enter_family_war_scene_request);
	ret &= center_msg_handler_t::m_center_msg_handle.register_func(op_cmd::eg_family_war_start_notify, handle_eg_family_war_start_notify);
	ret &= center_msg_handler_t::m_center_msg_handle.register_func(op_cmd::eg_family_war_end_notify, handle_eg_family_war_end_notify);

	ret &= center_msg_handler_t::m_center_msg_handle.register_func(op_cmd::eg_family_prayer_gift_failed_notify, handle_eg_family_prayer_gift_failed_notify);

	ret &= center_msg_handler_t::m_center_msg_handle.register_func(op_cmd::eg_family_shop_buy_reply, handle_eg_family_shop_buy_reply);

	return ret;
}

bool family_msg_handle_t::handle_cg_create_family_requsest(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_create_family_request);

	const role_ptr& p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role[%lu]", uid);
		return false;
	}

	uint32_t ret = family_manager_t::create_family_request(p_role, msg.name(), msg.declaration());
	if (common::errcode_enum::error_ok != ret)
	{
		proto::client::gc_create_family_reply reply;
		reply.set_reply_code(ret);
		p_role->send_msg_to_client(op_cmd::gc_create_family_reply, reply);
	}

	return true;
}


bool family_msg_handle_t::handle_cg_impeach_family_general_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_impeach_family_general_request);
	auto p_role = role_manager_t::find_role(uid);
	proto::client::gc_impeach_family_general_reply reply;
	if(NULL == p_role)
	{
		log_error("NULL == p_role[%lu]", uid);
		return false;
	}

	auto conf = GET_CONF(Comprehensive, comprehensive_common::family_create_cost);
	uint32_t impeach_money_limit = GET_COMPREHENSIVE_VALUE_1(conf);
    auto money = money_manager_t::get_money(p_role, proto::common::MONEY_TYPE_YUANBAO);
    if(money  < impeach_money_limit)
    {
        log_error("role [%llu] has [%llu] gold while [%llu] required money not enough can't impeach family general", p_role->get_uid(), money, impeach_money_limit);
        reply.set_reply_code(common::errcode_enum::notice_gold_money_not_enough);
        p_role->send_msg_to_client(op_cmd::gc_impeach_family_general_reply, reply);
        return false;
    }

    
    money_manager_t::use_money(p_role, proto::common::MONEY_TYPE_YUANBAO, impeach_money_limit, log_enum::source_type_family_impeach_cost, 0);
    proto::server::ge_impeach_family_general_request request;
    env::server->send_msg_to_center(op_cmd::ge_impeach_family_general_request, uid, request);
    
	return true;
}

bool family_msg_handle_t::handle_eg_create_family_reply( const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::eg_create_family_reply);

    const role_ptr& p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role[%lu]", uid);
		return false;
	}

    proto::client::gc_create_family_reply reply;
	reply.set_reply_code(msg.reply_code());

	// 创建不成功还钱
	if (common::errcode_enum::error_ok != msg.reply_code())
	{
		auto conf = GET_CONF(Comprehensive, comprehensive_common::family_create_cost);
		uint32_t create_family_cost = GET_COMPREHENSIVE_VALUE_1(conf);
		money_manager_t::add_money(p_role, proto::common::MONEY_TYPE_YUANBAO, create_family_cost, log_enum::source_type_family_create_fail_return, 0);
		FAMILY_LOG("role[%lu] create family failed, back money[%u]", uid, create_family_cost);
	}
	else
	{
		p_role->join_family(msg.info().id(), msg.info().name(), msg.info().officer(), msg.info().country_id());

		reply.set_id(common::string_util_t::uint64_to_string(msg.info().id()));
		proto::common::personal_info *p_info = reply.mutable_rcd()->mutable_per_info();
		p_role->get_personal_info(p_info);
	}

	p_role->send_msg_to_client(op_cmd::gc_create_family_reply, reply);

    return true;
}


bool family_msg_handle_t::handle_eg_update_family_info_notify(const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::eg_update_family_info_notify);

	const role_ptr& p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role[%lu]", uid);
		return false;
	}

	if (msg.id() != 0)
	{
		p_role->join_family(msg.id(), msg.name(), msg.officer(), msg.country_id());
	}
	else
	{
		p_role->leave_family();
	}

	return true;
}


bool family_msg_handle_t::handle_eg_family_country_change_notify(const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::eg_family_country_change_notify);

	for (int32_t i = 0; i < msg.role_list_size(); ++i) {
		role_ptr p_role = role_manager_t::find_role(msg.role_list(i));
		if (NULL != p_role) {
			p_role->set_country_id(msg.country_id());
		}
	}

	return true;
}

bool family_msg_handle_t::handle_eg_impeach_family_general_reply(const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::eg_impeach_family_general_reply);
    auto p_role = role_manager_t::find_role(uid);
    proto::client::gc_impeach_family_general_reply reply;

    if( NULL == p_role)
    {
        log_error("null role");
        return false;
    }

    if(msg.reply_code() != errcode_enum::error_ok)
    {
		auto conf = GET_CONF(Comprehensive, comprehensive_common::family_create_cost);
		uint32_t impeach_money_limit = GET_COMPREHENSIVE_VALUE_1(conf);

        money_manager_t::add_money(p_role, proto::common::MONEY_TYPE_YUANBAO, impeach_money_limit, log_enum::source_type_family_impeach_fail_return, 0);
        reply.set_reply_code(msg.reply_code());
        p_role->send_msg_to_client(op_cmd::gc_impeach_family_general_reply, reply);
        log_error("impeach  fail on center");
        return false;
    }

    reply.set_reply_code(msg.reply_code());
	proto::common::personal_info *p_info = reply.mutable_rcd()->mutable_per_info();
	p_role->get_personal_info(p_info);
    p_role->send_msg_to_client(op_cmd::gc_impeach_family_general_reply, reply);

    return true;
}

bool family_msg_handle_t::handle_eg_notify_game_impeach_result(const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::eg_impeach_family_general_reply);

    //to do return money 
    return true;
}


bool family_msg_handle_t::handle_cg_get_family_activities_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_get_family_activities_request);

	const role_ptr& p_role = role_manager_t::find_role(uid);
	if (NULL == p_role) {
		log_error("NULL == p_role[%lu]", uid);
		return false;
	}

	activity_mgr_ptr p_activity_mgr = p_role->get_activity_mgr();
	if (NULL == p_activity_mgr) {
		log_error("NULL == p_activity_mgr[%lu]", uid);
		return false;
	}

	proto::client::gc_get_family_activities_reply reply;
	if (p_role->get_family_id() > 0) {
		p_activity_mgr->peek_family_activity(reply);
		reply.set_reply_code(common::errcode_enum::error_ok);
	} else {
		reply.set_reply_code(common::errcode_enum::notice_no_family);
	}
	p_role->send_msg_to_client(op_cmd::gc_get_family_activities_reply, reply);

	return true;
}

//////////////////////////////////////////////////////////////////////////

bool family_msg_handle_t::handle_cg_leave_family_war_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_leave_family_war_request);

	role_ptr p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("p_role[%lu] is null ", uid);
		return false;
	}

	if (p_role->get_scene_type() == proto::server::em_scene_type_family_war)
	{
		scene_manager_t::role_back_scene(p_role, proto::server::user_change_scene);
	}

	return true;
}


bool family_msg_handle_t::handle_cg_family_war_member_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_family_war_member_request);

	const role_ptr& p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("p_role[%lu] is null ", uid);
		return false;
	}

	proto::client::gc_family_war_member_reply reply;
	family_manager_t::get_family_war_member(p_role, reply);
	p_role->send_msg_to_client(op_cmd::gc_family_war_member_reply, reply);

	return true;
}


bool family_msg_handle_t::handle_cg_ask_formations_info_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_ask_formations_info_request);

	const role_ptr& p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("p_role[%lu] is null ", uid);
		return false;
	}

	proto::client::gc_ask_formations_info_reply reply;
	family_manager_t::get_role_formations_info(p_role, reply);
	p_role->send_msg_to_client(op_cmd::gc_ask_formations_info_reply, reply);

	return true;
}


bool family_msg_handle_t::handle_cg_formations_hero_up_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_formations_hero_up_request);

	const role_ptr& p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("p_role[%lu] is null ", uid);
		return false;
	}

	uint32_t back_code = family_manager_t::ask_formations_hero_up(p_role, msg);
	if (back_code != common::errcode_enum::error_ok)
	{
		proto::client::gc_formations_hero_up_reply reply;
		reply.set_reply_code(back_code);
		p_role->send_msg_to_client(op_cmd::gc_formations_hero_up_reply, reply);
	}

	return true;
}


bool family_msg_handle_t::handle_cg_formations_hero_revive_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_formations_hero_revive_request);

	const role_ptr& p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("p_role[%lu] is null ", uid);
		return false;
	}

	uint32_t back_code = family_manager_t::ask_formations_revive(p_role, msg);
	if (back_code != common::errcode_enum::error_ok)
	{
		proto::client::gc_formations_hero_revive_reply reply;
		reply.set_reply_code(back_code);
		p_role->send_msg_to_client(op_cmd::gc_formations_hero_revive_reply, reply);
	}

	return true;
}


bool family_msg_handle_t::handle_cg_family_war_attack_long_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_family_war_attack_long_request);

	const role_ptr& p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("p_role[%lu] is null ", uid);
		return false;
	}

	uint32_t back_code = family_manager_t::ask_attack_long(p_role);
	if (back_code != common::errcode_enum::error_ok)
	{
		proto::client::gc_family_war_attack_long_reply reply;
		reply.set_reply_code(back_code);
		p_role->send_msg_to_client(op_cmd::gc_family_war_attack_long_reply, reply);
	}

	return true;
}


bool family_msg_handle_t::handle_cg_family_war_fight_data_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_family_war_fight_data_request);

	const role_ptr& p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("p_role[%lu] is null ", uid);
		return false;
	}

	proto::client::gc_family_war_fight_data_reply reply;
	family_manager_t::get_family_war_fight_data(p_role, reply);
	p_role->send_msg_to_client(op_cmd::gc_family_war_fight_data_reply, reply);

	return true;
}


bool family_msg_handle_t::handle_cg_ask_enter_battlefield_requet(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_ask_enter_battlefield_requet);

	role_ptr p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("p_role[%lu] is null ", uid);
		return false;
	}

	uint32_t back_code = family_manager_t::ask_enter_battlefield(p_role);
	if (back_code != common::errcode_enum::error_ok)
	{
		proto::client::gc_ask_enter_battlefield_reply reply;
		reply.set_reply_code(back_code);
		p_role->send_msg_to_client(op_cmd::gc_ask_enter_battlefield_reply, reply);
	}

	return true;
}


bool family_msg_handle_t::handle_cg_long_hurt_info_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_long_hurt_info_request);

	const role_ptr& p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("p_role[%lu] is null ", uid);
		return false;
	}

	family_manager_t::get_long_hurt_info(p_role, common::string_util_t::string_to_uint64(msg.object_id()));

	return true;
}


bool family_msg_handle_t::handle_cg_family_war_result_info_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_family_war_result_info_request);

	const role_ptr& p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("p_role[%lu] is null ", uid);
		return false;
	}

	proto::client::gc_family_war_result_info_reply reply;
	family_manager_t::get_family_war_result(p_role, reply);
	p_role->send_msg_to_client(op_cmd::gc_family_war_result_info_reply, reply);

	return true;
}


bool family_msg_handle_t::handle_cg_formations_hero_down_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_formations_hero_down_request);

	const role_ptr& p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("p_role[%lu] is null ", uid);
		return false;
	}

	uint32_t back_code = family_manager_t::ask_formations_hero_down(p_role, msg);
	if (back_code != common::errcode_enum::error_ok)
	{
		proto::client::gc_formations_hero_down_reply reply;
		reply.set_reply_code(back_code);
		p_role->send_msg_to_client(op_cmd::gc_formations_hero_down_reply, reply);
	}

	return true;
}

bool family_msg_handle_t::handle_eg_prepare_family_war_request(const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::eg_prepare_family_war_request);

	family_manager_t::prepare_family_war_request(msg);

	return true;
}

bool family_msg_handle_t::handle_eg_enter_family_war_scene_request(const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::eg_enter_family_war_scene_request);

	role_ptr p_role = role_manager_t::find_role(msg.uid());
	if (NULL == p_role)
	{
		log_error("p_role[%lu] is null ", msg.uid());
		return false;
	}

	if (p_role->get_troop_id() > 0)
	{
		log_error("p_role[%lu] in troop can't enter family war scene", msg.uid());
		return false;
	}

	uint32_t back_code = scene_manager_t::role_jump_scene(p_role, proto::server::center_change_scene, msg.family_dungeon_id());
	if (back_code != 0)
	{
		proto::client::gc_jump_scene_reply ntf;
		ntf.set_reply_code(back_code);// 设置跳地图出错
		p_role->send_msg_to_client(op_cmd::gc_jump_scene_reply, ntf);
	}

	return true;
}

bool family_msg_handle_t::handle_eg_family_war_start_notify(const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::eg_family_war_start_notify);

	family_manager_t::family_war_start(msg.end_time());

	return true;
}

bool family_msg_handle_t::handle_eg_family_war_end_notify(const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::eg_family_war_end_notify);

	family_manager_t::family_war_end();

	return true;
}

bool family_msg_handle_t::handle_cg_family_prayer_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_family_prayer_request);

	const role_ptr& p_role = role_manager_t::find_role(uid);
	if (NULL == p_role) {
		log_error("p_role[%lu] is null ", uid);
		return false;
	}

	uint32_t back_code = family_manager_t::ask_family_prayer_request(p_role, msg);
	if (back_code != common::errcode_enum::error_ok) {
		proto::client::gc_family_prayer_reply reply;
		reply.set_reply_code(back_code);
		p_role->send_msg_to_client(op_cmd::gc_family_prayer_reply, reply);
	}

	return true;
}

bool family_msg_handle_t::handle_cg_family_prayer_gift_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_family_prayer_gift_request);

	const role_ptr& p_role = role_manager_t::find_role(uid);
	if (NULL == p_role) {
		log_error("p_role[%lu] is null ", uid);
		return false;
	}

	uint32_t back_code = family_manager_t::family_prayer_gift_request(p_role, msg);
	if (back_code != common::errcode_enum::error_ok) {
		proto::client::gc_family_prayer_gift_reply reply;
		reply.set_reply_code(back_code);
		p_role->send_msg_to_client(op_cmd::gc_family_prayer_gift_reply, reply);
	}

	return true;
}

bool family_msg_handle_t::handle_eg_family_prayer_gift_failed_notify(const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::eg_family_prayer_gift_failed_notify);

	const role_ptr& p_role = role_manager_t::find_role(uid);
	if (NULL == p_role) {
		log_error("p_role[%lu] is null ", uid);
		return false;
	}

	proto::client::gc_family_prayer_gift_reply reply;
	family_manager_t::family_prayer_gift_reply(p_role, msg, reply);
	p_role->send_msg_to_client(op_cmd::gc_family_prayer_gift_reply, reply);

	return true;
}

bool family_msg_handle_t::handle_cg_family_shop_buy_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_family_shop_buy_request);

	const role_ptr& p_role = role_manager_t::find_role(uid);
	if (NULL == p_role) {
		log_error("p_role[%lu] is null ", uid);
		return false;
	}

	uint32_t back_code = family_manager_t::family_shop_buy_request(p_role, msg.index(), msg.shop_id(), msg.num());
	if (back_code != common::errcode_enum::error_ok) {
		proto::client::gc_family_shop_buy_reply reply;
		reply.set_reply_code(back_code);
		p_role->send_msg_to_client(op_cmd::gc_family_shop_buy_reply, reply);
	}

	return true;
}

bool family_msg_handle_t::handle_eg_family_shop_buy_reply(const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::eg_family_shop_buy_reply);

	const role_ptr& p_role = role_manager_t::find_role(uid);
	if (NULL == p_role) {
		log_error("p_role[%lu] is null ", uid);
		return false;
	}

	proto::client::gc_family_shop_buy_reply reply;
	family_manager_t::family_shop_center_check_back(p_role, msg, reply);
	proto::common::role_change_data* p_rcd = reply.mutable_change_data();
	if (p_rcd) {
		p_role->get_personal_info(p_rcd->mutable_per_info());
	}
	p_role->send_msg_to_client(op_cmd::gc_family_shop_buy_reply, reply);

	if (reply.reply_code() != common::errcode_enum::error_ok) {
		proto::server::ge_family_shop_back_notify failed_back;
		failed_back.set_refresh_time(msg.refresh_time());
		failed_back.set_index(msg.index());
		failed_back.set_num(msg.num());
		env::server->send_msg_to_center(op_cmd::ge_family_shop_back_notify, p_role->get_uid(), failed_back);
	}

	return true;
}
