#include "fight_msg_handle.hpp"
#include "fight_manager.hpp"
#include "fight_hero.hpp"
#include "gate/gate_msg_handler.hpp"
#include "tblh/errcode_enum.hpp"
#include "tblh/Comprehensive.tbls.h"
#include "config/config_manager.hpp"

bool fight_msg_handle_t::init_msg_handler()
{
	gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_finish_fight_request, handle_finish_fight_request);
	gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_combat_loading_end_ntf, handle_combat_loading_end_ntf);
	gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_combat_form_request, handle_combat_form_request);
	gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_combat_play_end_ntf, handle_combat_play_end_ntf);
	gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_combat_escape_ntf, handle_cg_combat_escape_ntf);
	gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_combat_auto_fight_request, handle_combat_auto_fight_request);
	gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_combat_before_form_ready_request, handle_combat_before_form_ready_request);
	gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_combat_mark_target_notify, handle_combat_mark_target_notify);
	gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_combat_speak_notify, handle_combat_speak_notify);
	gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_combat_ask_escape_req, handle_combat_ask_escape_req);
	gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_combat_recovery_ask_reply, handle_combat_recovery_ask_reply);
	gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_watch_combat_request, handle_watch_combat_request);
	gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_cancel_watch_combat_request, handle_cancel_watch_combat_request);
	gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_new_role_combat_notify, handle_new_role_combat_notify);
	return true;
}

bool fight_msg_handle_t::handle_finish_fight_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_finish_fight_request);
	role_ptr p_role = role_manager_t::find_role(uid);
	if (p_role == NULL)
	{
		log_error("finish fight p_role is null [%lu]", uid);
		return false;
	}

	fight_manager_t::finish_fight(p_role, msg.result());
	return true;
}

bool fight_msg_handle_t::handle_combat_loading_end_ntf(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_combat_loading_end_ntf);
	role_ptr p_role = role_manager_t::find_role(uid);
	if (p_role == NULL)
	{
		log_error("loading end ntf p_role is null [%lu]", uid);
		return false;
	}

	fight_manager_t::combat_loading_end_ntf(p_role, string_util_t::string_to_uint64(msg.fight_uid()));
	proto::client::gc_combat_loading_end_reply reply;
	p_role->send_msg_to_client(op_cmd::gc_combat_loading_end_reply, reply);
	return true;
}

bool fight_msg_handle_t::handle_combat_form_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_combat_form_request);
	role_ptr p_role = role_manager_t::find_role(uid);
	if (p_role == NULL)
	{
		log_error("combat form request p_role is null [%lu]", uid);
		return false;
	}

	fight_manager_t::combat_disposition_request(p_role, string_util_t::string_to_uint64(msg.fight_uid()), msg.data());
	return true;
}

bool fight_msg_handle_t::handle_combat_play_end_ntf(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_combat_play_end_ntf);
	role_ptr p_role = role_manager_t::find_role(uid);
	if (p_role == NULL)
	{
		log_error("combat play end p_role is null [%lu]", uid);
		return false;
	}

	fight_manager_t::combat_play_end_ntf(p_role, string_util_t::string_to_uint64(msg.fight_uid()), msg.hero_att_round());
	proto::client::gc_combat_play_end_reply reply;
	p_role->send_msg_to_client(op_cmd::gc_combat_play_end_reply, reply);
	return true;
}

bool fight_msg_handle_t::handle_cg_combat_escape_ntf(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_combat_escape_ntf);
	role_ptr p_role = role_manager_t::find_role(uid);
	if (p_role == NULL)
	{
		log_error("combat p_role is null [%lu]", uid);
		return false;
	}

	fight_manager_t::request_escape(p_role, string_util_t::string_to_uint64(msg.fight_uid()));
	return true;
}

bool fight_msg_handle_t::handle_combat_auto_fight_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_combat_auto_fight_request);
	role_ptr p_role = role_manager_t::find_role(uid);
	if (p_role == NULL)
	{
		log_error("combat p_role is null [%lu]", uid);
		return false;
	}

	fight_manager_t::request_switch_auto_fight(p_role, string_util_t::string_to_uint64(msg.fight_uid()), msg.auto_fight(), msg.mutable_hero_data());
	return true;
}

bool fight_msg_handle_t::handle_combat_before_form_ready_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_combat_before_form_ready_request);
	role_ptr p_role = role_manager_t::find_role(uid);
	if (p_role == NULL)
	{
		log_error("combat p_role is null [%lu]", uid);
		return false;
	}

	fight_manager_t::combat_before_form_ready(p_role, string_util_t::string_to_uint64(msg.fight_uid()));
	return true;
}

bool fight_msg_handle_t::handle_combat_mark_target_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_combat_mark_target_notify);
	role_ptr p_role = role_manager_t::find_role(uid);
	if (p_role == NULL)
	{
		log_error("combat p_role is null [%lu]", uid);
		return false;
	}
	uint64_t fight_uid = p_role->get_fight_uid();
	combat_ptr p_combat = fight_manager_t::get_combat(fight_uid);
	if (NULL == p_combat)
	{
		log_error("combat p_combat NULL fight_uid[%lu]", fight_uid);
		return false;
	}
	fight_obj_ptr p_fight_obj = p_combat->find_obj(p_role->get_uid());
	if (NULL == p_fight_obj)
	{
		log_error("combat p_fight_obj NULL uid[%lu]", p_role->get_uid());
		return false;
	}
	p_combat->set_mark_id(msg.mark_id(), msg.fight_hero_id());

	proto::client::gc_combat_mark_target_notify reply;
	reply.set_fight_hero_id(msg.fight_hero_id());
	reply.set_mark_id(msg.mark_id());
//	reply.set_mark_hero_id(msg.mark_hero_id());

	p_combat->send_msg_to_camp(p_fight_obj->get_camp(), op_cmd::gc_combat_mark_target_notify, reply);
	return true;
}

bool fight_msg_handle_t::handle_combat_speak_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_combat_speak_notify);
	role_ptr p_role = role_manager_t::find_role(uid);
	if (p_role == NULL)
	{
		log_error("combat p_role is null [%lu]", uid);
		return false;
	}

	uint64_t fight_uid = p_role->get_fight_uid();
	combat_ptr p_combat = fight_manager_t::get_combat(fight_uid);
	if (NULL == p_combat)
	{
		log_error("combat p_combat NULL fight_uid[%lu]", fight_uid);
		return false;
	}
	fight_obj_ptr p_fight_obj = p_combat->find_obj(p_role->get_uid());
	if (NULL == p_fight_obj)
	{
		log_error("combat p_fight_obj NULL uid[%lu]", p_role->get_uid());
		return false;
	}

	proto::client::gc_combat_speak_notify reply;
	reply.set_role_uid(string_util_t::uint64_to_string(uid));
	reply.set_word_id(msg.word_id());

	p_combat->send_msg_to_camp(p_fight_obj->get_camp(), op_cmd::gc_combat_speak_notify, reply);
	return true;
}

bool fight_msg_handle_t::handle_combat_ask_escape_req(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_combat_speak_notify);
	role_ptr p_role = role_manager_t::find_role(uid);
	if (p_role == NULL)
	{
		log_error("combat p_role is null [%lu]", uid);
		return false;
	}

	uint64_t fight_uid = p_role->get_fight_uid();
	combat_ptr p_combat = fight_manager_t::get_combat(fight_uid);
	if (NULL == p_combat)
	{
		log_error("combat p_combat NULL fight_uid[%lu]", fight_uid);
		return false;
	}
	fight_obj_ptr p_fight_obj = p_combat->find_obj(p_role->get_uid());
	if (NULL == p_fight_obj)
	{
		log_error("combat p_fight_obj NULL uid[%lu]", p_role->get_uid());
		return false;
	}
	
	proto::client::gc_combat_ask_escape_rep reply;
	reply.set_role_uid(string_util_t::uint64_to_string(uid));

	p_combat->send_msg_to_camp(p_fight_obj->get_camp(), op_cmd::gc_combat_ask_escape_rep, reply);
	return true;
}

bool fight_msg_handle_t::handle_combat_recovery_ask_reply(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_combat_recovery_ask_reply);
	role_ptr p_role = role_manager_t::find_role(uid);
	if (p_role == NULL)
	{
		log_error("combat p_role is null [%lu]", uid);
		return false;
	}
	combat_ptr p_combat = fight_manager_t::check_combat(p_role, p_role->get_fight_uid());
	// 战斗还在
	if (NULL != p_combat)
	{
		// 玩家选择恢复
		if (msg.recovery() == 1)
		{
			p_combat->on_role_login(uid);
		}
		// 玩家选择不恢复
		else
		{
			// 走逃跑的逻辑
			p_combat->request_escape(uid);
		}
	}
	else
	{
		// 如果玩家点了恢复 但此时战斗已经结束 就回复消息失败
		if (msg.recovery() == 1)
		{
			proto::client::gc_combat_recovery_notify ntf;
			ntf.set_result(0);
			p_role->send_msg_to_client(op_cmd::gc_combat_recovery_notify, ntf);
		}
	}

	return true;
}

bool fight_msg_handle_t::handle_watch_combat_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_watch_combat_request);
	role_ptr p_role = role_manager_t::find_role(uid);
	if (p_role == NULL)
	{
		log_error("combat p_role is null [%lu]", uid);
		return false;
	}
	uint64_t watching_role_uid = string_util_t::string_to_uint64(msg.req_role_uid());
	uint64_t fight_uid = string_util_t::string_to_uint64(msg.fight_uid());
	fight_manager_t::watch_combat(p_role, watching_role_uid, fight_uid);
	return true;
}

bool fight_msg_handle_t::handle_cancel_watch_combat_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_cancel_watch_combat_request);
	role_ptr p_role = role_manager_t::find_role(uid);
	if (p_role == NULL)
	{
		log_error("combat p_role is null [%lu]", uid);
		return false;
	}
	fight_manager_t::cancel_watch_combat(p_role, true);

	return true;
}

bool fight_msg_handle_t::handle_new_role_combat_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_new_role_combat_notify);
	role_ptr p_role = role_manager_t::find_role(uid);
	if (p_role == NULL)
	{
		log_error("combat p_role is null [%lu]", uid);
		return false;
	}
	auto conf = GET_CONF(Comprehensive, comprehensive_common::first_fight_id);
	uint32_t first_fight_id = GET_COMPREHENSIVE_VALUE_1(conf);

	proto::common::fight_param fp;
	fp.set_type(proto::common::fight_type_against);
	fight_manager_t::fight_pve(p_role, first_fight_id, &fp);

	return true;
}
