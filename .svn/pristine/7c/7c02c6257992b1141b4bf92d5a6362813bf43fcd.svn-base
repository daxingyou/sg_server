#include "cross_msg_handler.hpp"
#include "arena/arena_manager.hpp"

USING_NS_NETWORK;
USING_NS_COMMON;

server_msg_handler_t<op_cmd::SR_BEGIN, op_cmd::SR_END, cross_msg_handler_t::msg_handler_func> cross_msg_handler_t::m_cross_msg_handle;

bool cross_msg_handler_t::init_msg_handler()
{
    bool ret = true;
	ret &= m_cross_msg_handle.register_func(op_cmd::sr_syn_cross_info_notify, handle_sr_syn_cross_info_notify);
	ret &= m_cross_msg_handle.register_func(op_cmd::sr_arena_battle_result_notify, handle_sr_arena_battle_result_notify);
	
    return ret;
}

bool cross_msg_handler_t::handle_sr_syn_cross_info_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
    PRE_S2S_MSG(proto::server::sr_syn_cross_info_notify);
    cross_manager_t::set_cross(s, msg.cross_id());
    return true;
}

bool cross_msg_handler_t::handle_sr_arena_battle_result_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::sr_arena_battle_result_notify);

	arena_manager_t::deal_arena_battle_result(msg);
	return true;
}
