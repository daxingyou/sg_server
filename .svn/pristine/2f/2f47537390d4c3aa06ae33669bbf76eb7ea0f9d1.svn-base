#include "king_war_msg_handle.hpp"
#include "center/center_msg_handler.hpp"
#include "gate/gate_msg_handler.hpp"
#include "common.pb.h"
#include "server.pb.h"
#include "main/game_server_fwd.hpp"
#include "main/game_server.hpp"
#include "role/role.hpp"
#include "role/role_manager.hpp"
#include "king_war_manager.hpp"
#include "tblh/errcode_enum.hpp"

bool king_war_msg_handle_t::init_center_msg_handler()
{
	bool bresult = true;

	bresult &= center_msg_handler_t::m_center_msg_handle.register_func(op_cmd::eg_king_war_state_notify, handle_eg_king_war_state_notify);
	bresult &= center_msg_handler_t::m_center_msg_handle.register_func(op_cmd::eg_king_war_time_end_notify, handle_eg_king_war_time_end_notify);

	return bresult;
}

bool king_war_msg_handle_t::init_gate_msg_handler()
{
	bool bresult = true;

	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_king_war_scene_num_request, handle_cg_king_war_scene_num_request);
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_enter_king_war_scene_request, handle_cg_enter_king_war_scene_request);
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_king_war_defence_request, handle_cg_king_war_defence_request);
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_king_war_attack_start_request, handle_cg_king_war_attack_start_request);
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_king_war_attack_end_notify, handle_cg_king_war_attack_end_notify);
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_king_war_attack_end_request, handle_cg_king_war_attack_end_request);
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_use_artifact_request, handle_cg_use_artifact_request);
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_ask_king_war_result_request, handle_cg_ask_king_war_result_request);

	return bresult;
}

bool king_war_msg_handle_t::handle_eg_king_war_state_notify(const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::eg_king_war_state_notify);

	king_war_manager_t::update_king_war_state(msg.country_id(), msg);

	return true;
}

bool king_war_msg_handle_t::handle_eg_king_war_time_end_notify(const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::eg_king_war_time_end_notify);

	king_war_manager_t::king_war_ending(msg.country_id());

	return true;
}

bool king_war_msg_handle_t::handle_cg_king_war_scene_num_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_king_war_scene_num_request);

	role_ptr p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("p_role is null [%lu]", uid);
		return false;
	}

	proto::client::gc_king_war_scene_num_reply notify;
	king_war_manager_t::get_king_war_family_num(p_role->get_country_id(), notify);
	p_role->send_msg_to_client(op_cmd::gc_king_war_scene_num_reply, notify);

	return true;
}

bool king_war_msg_handle_t::handle_cg_enter_king_war_scene_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_enter_king_war_scene_request);

	role_ptr p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("p_role is null [%lu]", uid);
		return false;
	}

	uint32_t reply_code = king_war_manager_t::ask_enter_king_war_scene(p_role);
	if (reply_code != errcode_enum::error_ok)
	{
		proto::client::gc_enter_king_war_scene_reply reply;
		reply.set_reply_code(reply_code);
		p_role->send_msg_to_client(op_cmd::gc_enter_king_war_scene_reply, reply);
	}

	return true;
}

bool king_war_msg_handle_t::handle_cg_king_war_defence_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_king_war_defence_request);

	role_ptr p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("p_role is null [%lu]", uid);
		return false;
	}

	uint64_t object_id = common::string_util_t::string_to_uint64(msg.object_id());

	proto::client::gc_king_war_defence_reply reply;
	uint32_t reply_code = king_war_manager_t::ask_defence_long(p_role, object_id);
	reply.set_reply_code(reply_code);
	p_role->send_msg_to_client(op_cmd::gc_king_war_defence_reply, reply);

	return true;
}

bool king_war_msg_handle_t::handle_cg_king_war_attack_start_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_king_war_attack_start_request);

	role_ptr p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("p_role is null [%lu]", uid);
		return false;
	}

	uint64_t object_id = common::string_util_t::string_to_uint64(msg.object_id());

	proto::client::gc_king_war_attack_start_reply reply;
	uint32_t reply_code = king_war_manager_t::ask_attack_long(p_role, object_id);
	reply.set_reply_code(reply_code);
	p_role->send_msg_to_client(op_cmd::gc_king_war_attack_start_reply, reply);

	return true;
}

bool king_war_msg_handle_t::handle_cg_king_war_attack_end_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_king_war_attack_end_notify);

	role_ptr p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("p_role is null [%lu]", uid);
		return false;
	}

	uint64_t object_id = common::string_util_t::string_to_uint64(msg.object_id());

	king_war_manager_t::attack_long_end(p_role, object_id);

	return true;
}

bool king_war_msg_handle_t::handle_cg_king_war_attack_end_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_king_war_attack_end_request);

	role_ptr p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("p_role is null [%lu]", uid);
		return false;
	}

	uint64_t object_id = common::string_util_t::string_to_uint64(msg.object_id());

	proto::client::gc_king_war_attack_end_reply reply;
	king_war_manager_t::ask_hurt_long(p_role, object_id, reply);
	p_role->send_msg_to_client(op_cmd::gc_king_war_attack_end_reply, reply);

	return true;
}

bool king_war_msg_handle_t::handle_cg_use_artifact_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_use_artifact_request);

	role_ptr p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("p_role is null [%lu]", uid);
		return false;
	}

	king_war_manager_t::user_artifacts(p_role, msg.index());

	return true;
}

bool king_war_msg_handle_t::handle_cg_ask_king_war_result_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_ask_king_war_result_request);

	role_ptr p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("p_role is null [%lu]", uid);
		return false;
	}

	proto::client::gc_king_war_result_reply reply;
	king_war_manager_t::ask_king_war_result(p_role, reply);
	p_role->send_msg_to_client(op_cmd::gc_king_war_result_reply, reply);

	return true;
}
