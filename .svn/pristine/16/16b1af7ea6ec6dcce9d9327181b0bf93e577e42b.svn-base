#include "mount_msg_handler.hpp"
#include "gate/gate_msg_handler.hpp"
#include "mount/mount_manager.hpp"
#include "tblh/errcode_enum.hpp"

USING_NS_COMMON;


bool mount_msg_handler_t::init_msg_handler()
{
	bool bresult = true;
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_use_mount_request, handle_cg_use_mount_request);
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_turn_mount_request, handle_cg_turn_mount_request);
	return bresult;
}

bool mount_msg_handler_t::handle_cg_use_mount_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_use_mount_request);
	auto p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("mount_msg p_role NULL");
		return false;
	}
	proto::client::gc_use_mount_reply reply;
	uint32_t reply_code = mount_manager_t::use_mount(p_role, msg.mount_tid());
	reply.set_reply_code(reply_code);
	reply.set_mount_tid(msg.mount_tid());
	reply.set_role_uid(string_util_t::uint64_to_string(uid));
	p_role->send_msg_to_client(op_cmd::gc_use_mount_reply, reply);
	return true;
}

bool mount_msg_handler_t::handle_cg_turn_mount_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_turn_mount_request);
	auto p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("mount_msg p_role NULL");
		return false;
	}
	proto::client::gc_turn_mount_reply reply;
	uint32_t reply_code = errcode_enum::error_ok;
	if (msg.is_cancel())
		mount_manager_t::cancel_turn_mount(p_role, msg.mount_tid());
	else
		mount_manager_t::turn_mount(p_role, msg.mount_tid());

	reply.set_reply_code(reply_code);
	reply.set_mount_tid(msg.mount_tid());
	p_role->send_msg_to_client(op_cmd::gc_turn_mount_reply, reply);

	return true;
}