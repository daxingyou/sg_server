#include "scene_msg_handle.hpp"
#include "log.hpp"
#include "scene.hpp"
#include "scene_manager.hpp"
#include "role/role.hpp"
#include "config/config_manager.hpp"
#include "task/task_manager.hpp"
#include "client.pb.h"
#include "tblh/errcode_enum.hpp"
#include "center/center_msg_handler.hpp"
#include "gate/gate_msg_handler.hpp"
#include "user_troop/user_troop_mgr.hpp"

USING_NS_NETWORK;
USING_NS_COMMON;

bool scene_msg_handle_t::init_gate_msg_handler()
{
	bool bresult = true;

	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_enter_scene_request, handle_cg_enter_scene_request);
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_jump_scene_request, handle_cg_jump_scene_request);
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_jump_reading_start_notify, handle_cg_jump_reading_start_notify);
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_jump_reading_break_notify, handle_cg_jump_reading_break_notify);
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_troop_jump_end_request, handle_cg_troop_jump_end_request);
    
	return bresult;
}

bool scene_msg_handle_t::init_center_msg_handler()
{
	bool bresult = true;

	bresult &= center_msg_handler_t::m_center_msg_handle.register_func(op_cmd::eg_game_scene_reg_reply, handle_eg_game_scene_reg_reply);

	return bresult;
}

bool scene_msg_handle_t::handle_cg_enter_scene_request(const network::tcp_socket_ptr& s, const msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_enter_scene_request);

	role_ptr p_role = role_manager_t::find_role(uid);
	if (NULL == p_role) {
		log_error("p_role[%lu] is null ", uid);
		return false;
	}

	// 第一次登陆的处理
	if (p_role->is_login_scene()) p_role->ready_enter_scene();

	proto::client::gc_enter_scene_reply reply;
	p_role->role_enter_scene(common::string_util_t::string_to_uint64(msg.scene_id()), reply);
	p_role->send_msg_to_client(op_cmd::gc_enter_scene_reply, reply);

	return true;
}

bool scene_msg_handle_t::handle_cg_jump_scene_request(const network::tcp_socket_ptr& s, const msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_jump_scene_request);

	role_ptr p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("p_role[%lu] is null", uid);
		return false;
	}

	uint32_t back_code = 0;

	// jump_id != 0 表示跳转点传送,同地图传送返回客户端坐标
	if (msg.jump_id() != 0)
	{
		back_code = scene_manager_t::role_jump_point(p_role, msg.jump_id());
	}
	else if (msg.map_tid() != 0) // map_tid != 0 表示静态地图传送
	{
		back_code = scene_manager_t::role_jump_scene(p_role, proto::server::user_change_scene, msg.map_tid());
	}
	else
	{
		log_error("ERROR:role[%lu] jump map[%u] and jump_id[%u]", uid, msg.map_tid(), msg.jump_id());
		return false;
	}

	if (back_code != 0)
	{
		proto::client::gc_jump_scene_reply ntf;
		ntf.set_reply_code(back_code);// 设置跳地图出错
		p_role->send_msg_to_client(op_cmd::gc_jump_scene_reply, ntf);
	}

	return true;
}

bool scene_msg_handle_t::handle_cg_jump_reading_start_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_jump_reading_start_notify);

	role_ptr p_role = role_manager_t::find_role(uid);
	if (NULL == p_role) {
		log_error("p_role[%lu] is null", uid);
		return false;
	}

	uint32_t back_code = p_role->start_transmission(proto::common::transmission_type_user);
	proto::client::gc_jump_reading_start_notify ntf;
	ntf.set_type(proto::common::transmission_type_user);
	ntf.set_reply_code(back_code);
	p_role->send_msg_to_client(op_cmd::gc_jump_reading_start_notify, ntf);

	return true;
}

bool scene_msg_handle_t::handle_cg_jump_reading_break_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_jump_reading_break_notify);

	role_ptr p_role = role_manager_t::find_role(uid);
	if (NULL == p_role) {
		log_error("p_role[%lu] is null", uid);
		return false;
	}

	p_role->cancel_transmission();

	return true;
}

bool scene_msg_handle_t::handle_cg_troop_jump_end_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_troop_jump_end_request);

	role_ptr p_role = role_manager_t::find_role(uid);
	if (NULL == p_role) {
		log_error("p_role[%lu] is null", uid);
		return false;
	}

	uint32_t back_code = game_troop_mgr_t::troop_transmission_reading_end(p_role);
	if (back_code != common::errcode_enum::error_ok) {
		proto::client::gc_troop_jump_end_reply reply;
		reply.set_reply_code(back_code);
		p_role->send_msg_to_client(op_cmd::gc_troop_jump_end_reply, reply);
	}

	return true;
}

bool scene_msg_handle_t::handle_eg_game_scene_reg_reply(const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::eg_game_scene_reg_reply);

	scene_manager_t::reg_game_scene_back(msg);

	return true;
}
