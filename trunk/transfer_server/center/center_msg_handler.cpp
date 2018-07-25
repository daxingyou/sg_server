#include "center_msg_handler.hpp"
#include "main/transfer_server.hpp"
#include "main/transfer_server_fwd.hpp"
#include "center/center_manager.hpp"

USING_NS_NETWORK;
USING_NS_COMMON;

server_msg_handler_t<op_cmd::EF_BEGIN, op_cmd::EF_END, center_msg_handler_t::msg_handler_func> center_msg_handler_t::m_center_msg_handle;

bool center_msg_handler_t::init_msg_handler()
{
	m_center_msg_handle.register_func(op_cmd::ef_syn_center_info_notify, handle_ef_syn_center_info_notify);
	m_center_msg_handle.register_func(op_cmd::ef_test_request, handle_ef_test_request);
    return true;
}


bool center_msg_handler_t::handle_ef_syn_center_info_notify(const network::tcp_socket_ptr& s, const msg_buf_ptr& msg_buf)
{
    PRE_S2S_MSG(proto::server::ef_syn_center_info_notify);

    center_manager_t::set_center(s, msg.server_id());
	
    return true;
}

bool center_msg_handler_t::handle_ef_test_request(const network::tcp_socket_ptr& s, const msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::ef_test_request);

	uint32_t server_id = center_manager_t::get_center_id_by_socket(s);
	log_info("center_msg center_id[%d] test[%d]", server_id, msg.test1());

	proto::server::fe_test_reply reply;
	reply.set_test1(222);
	reply.set_role_game_server_id(msg.role_game_server_id());
	env::server->send_msg_to_all_center(op_cmd::fe_test_reply, 0, reply);
	env::server->send_msg_to_center(s, op_cmd::fe_test_reply, 0, reply);
	env::server->send_msg_to_center(server_id, op_cmd::fe_test_reply, 0, reply);

	return true;
}
