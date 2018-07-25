#include "gm_msg_handler.hpp"
#include "network.hpp"
#include "protos_fwd.hpp"
#include "user/global_user_manager.hpp"
#include "gm_func.hpp"
#include "config/config_manager.hpp"
#include <math.h>
#include "cdkey/cdkey_msg_handler.hpp"

USING_NS_NETWORK;
USING_NS_COMMON;

#define ADMIN_ID    uid

server_msg_handler_t<op_cmd::ME_BEGIN, op_cmd::ME_END, gm_msg_handler_t::msg_handler_func> gm_msg_handler_t::m_gm_msg_handler;

bool gm_msg_handler_t::init_msg_handler()
{
    bool ret = true;

	ret &= m_gm_msg_handler.register_func(op_cmd::me_ban_account_notify, handle_me_ban_account_notify);
	ret &= m_gm_msg_handler.register_func(op_cmd::me_access_ip_notify, handle_me_access_ip_notify);
	ret &= m_gm_msg_handler.register_func(op_cmd::me_reload_config_notify, handle_me_reload_config_notify);
	ret &= m_gm_msg_handler.register_func(op_cmd::me_send_mail_request, handle_me_send_mail_request);
	ret &= m_gm_msg_handler.register_func(op_cmd::me_get_user_ol_num_request, handle_me_get_user_ol_num_request);

	if (!cdkey_msg_handler_t::init_web_msg_handler())
	{
		log_error("init  cdkey msg handler failed!");
		return false;
	}
	
    return ret;
}

bool gm_msg_handler_t::handle_me_ban_account_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::gm::me_ban_account_notify);
	gm_func::ban_account(msg.account(), msg.flag());
	return true;
}

bool gm_msg_handler_t::handle_me_access_ip_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::gm::me_access_ip_notify);
	if (msg.has_ip_end())
	{
		gm_func::access_ip(msg.ip_address(), msg.ip_end(), msg.flag());
	}
	else
	{
		gm_func::access_ip(msg.ip_address(), msg.flag());
	}
	return true;
}

bool gm_msg_handler_t::handle_me_reload_config_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	if (!load_config())
	{
		log_error("reload config fail");
	}
	else
	{
		log_info("reload config success");
	}

	return true;
}



bool gm_msg_handler_t::handle_me_send_mail_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::gm::me_send_mail_request);

	gm_func::send_mail(msg);

	return true;
}


bool gm_msg_handler_t::handle_me_get_user_ol_num_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::gm::me_get_user_ol_num_request);

	gm_func::send_ol_nun(msg.admin_id());

	return true;
}


