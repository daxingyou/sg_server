#include "user/global_user.hpp"
#include "user/global_user_manager.hpp"
#include "gate/gate_msg_handler.hpp"
#include "gm/gm_msg_handler.hpp"
#include "tblh/errcode_enum.hpp"
#include "cdkey_manager.hpp"
#include "cdkey_msg_handler.hpp"

bool cdkey_msg_handler_t::init_gate_msg_handler()
{
	bool bresult = true;

	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::ce_exchange_cdkey_request, handle_ce_exchange_cdkey_request);

	return bresult;
}

bool cdkey_msg_handler_t::init_web_msg_handler()
{
	bool bresult = true;

	bresult &= gm_msg_handler_t::m_gm_msg_handler.register_func(op_cmd::me_do_change_cdkey_reply, handle_me_do_change_cdkey_reply);

	return bresult;
}


bool cdkey_msg_handler_t::handle_me_do_change_cdkey_reply(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::gm::me_do_change_cdkey_reply);

	cdkey_manager_t::do_exchange_cdkey_reply( uid, msg.key_type(), msg.reply_code() );

	return true;
}

bool cdkey_msg_handler_t::handle_ce_exchange_cdkey_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::ce_exchange_cdkey_request);
	const global_user_ptr& p_user = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL == p_user) {
		log_error("NULL == p_user[%lu]", uid);
		return false;
	}

	cdkey_manager_t::do_exchange_cdkey_request( p_user, p_user->get_channel_id(), msg.key_code() );

	return true;
}
