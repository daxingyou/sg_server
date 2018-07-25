#include "center_msg_handler.hpp"
#include "network.hpp"
#include "admin/admin_manager.hpp"
#include "admin/admin_func_manager.hpp"

USING_NS_NETWORK;
USING_NS_COMMON;

server_msg_handler_t<op_cmd::EM_BEGIN, op_cmd::EM_END, center_msg_handler_t::msg_handler_func> center_msg_handler_t::m_center_msg_handler;
server_msg_handler_t<op_cmd::GM_BEGIN, op_cmd::GM_END, center_msg_handler_t::msg_handler_func> center_msg_handler_t::m_game_msg_handler;


bool center_msg_handler_t::init_msg_handler()
{
    bool ret = true;
	// game => gm
	m_game_msg_handler.register_func(op_cmd::gm_data_threshold_notify,	handle_gm_data_threshold_notify);

    return ret;
}

bool center_msg_handler_t::init_center_msg_handler()
{
    bool ret = true;
	// center => gm
	m_center_msg_handler.register_func(op_cmd::em_get_user_ol_num_reply, handle_em_get_user_ol_num_reply);

    return ret;
}


bool center_msg_handler_t::handle_gm_data_threshold_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::gm::gm_data_threshold_notify);

	log_info("role[%lu] the data[%d] is greater than the threshold[%d] param[%d] ", msg.uid(), msg.type(), msg.value(), msg.param());
	return true;
}

bool center_msg_handler_t::handle_em_get_user_ol_num_reply(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::gm::em_get_user_ol_num_reply);

	admin_ptr p_admin = admin_manager_t::get_admin( msg.admin_id() );
	if (p_admin)
		admin_func_manager_t::send_user_num_to_tools(p_admin->get_admin_id(), msg.ol_num());
	return true;
}
