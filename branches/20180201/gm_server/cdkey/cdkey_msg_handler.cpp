#include "cdkey_msg_handler.hpp"
#include "cdkey_manager.hpp"
#include "protocpp/protos_fwd.hpp"

bool cdkey_msg_handler_t::init_msg_handler()
{
	center_msg_handler_t::m_center_msg_handler.register_func(op_cmd::em_do_change_cdkey_request, handler_em_do_change_cdkey_request);

	return true;
}

bool cdkey_msg_handler_t::handler_em_do_change_cdkey_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::gm::em_do_change_cdkey_request);

	cdkey_manager_t::get_use_cdkey_requeset( msg.role_uid(), msg.channel_id(),  msg.key_code() );

	return true;
}