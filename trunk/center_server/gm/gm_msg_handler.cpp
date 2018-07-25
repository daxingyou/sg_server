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
	ret &= m_gm_msg_handler.register_func(op_cmd::me_send_notice_request, handle_me_send_notice_request);
	ret &= m_gm_msg_handler.register_func(op_cmd::me_transport_ntf, handle_me_transport_ntf);
	ret &= m_gm_msg_handler.register_func(op_cmd::me_send_gm_msg_to_account_request, handle_me_send_gm_msg_to_account_request);
	ret &= m_gm_msg_handler.register_func(op_cmd::me_manage_role_ntf, handle_me_manage_role_ntf);
	ret &= m_gm_msg_handler.register_func(op_cmd::me_reload_xml_request, handle_me_reload_xml_request);
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
	PRE_S2S_MSG(proto::server::me_reload_config_notify);
	if (0 == msg.load_type())
	{
		reloadAllDTManager(env::xml->config_path);
	}
	else
	{
		reloadDTManager(env::xml->config_path, msg.file_name());
	}
	proto::server::eg_reload_config_notify ntf;
	ntf.set_load_type(msg.load_type());
	ntf.set_file_name(msg.file_name());
	env::server->send_msg_to_all_games(op_cmd::eg_reload_config_notify, 0, ntf);
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

bool gm_msg_handler_t::handle_me_send_notice_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::gm::me_send_notice_request);

	gm_func::send_notice(msg);

	return true;
}

bool gm_msg_handler_t::handle_me_transport_ntf(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::gm::me_transport_ntf);

	gm_func::transport(msg);

	return true;
}

bool gm_msg_handler_t::handle_me_send_gm_msg_to_account_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::gm::me_send_gm_msg_to_account_request);

	/*消息直接转发到gate - > client - > account*/

	proto::server::ea_send_gm_msg_to_account_request request;
	request.set_cmd_type(msg.cmd_type());
	request.mutable_msg()->CopyFrom( msg.msg() );
	env::server->send_msg_to_one_gate(op_cmd::ea_send_gm_msg_to_account_request, 0, request);
	return true;
}

bool gm_msg_handler_t::handle_me_manage_role_ntf(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::me_manage_role_ntf);

	gm_func::manage_role(msg);

	return true;
}

bool gm_msg_handler_t::handle_me_reload_xml_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::gm::mall_reload_xml_notify);

	if(msg.dest() == "center_server"){
		if(env::server == NULL)
			return false;
		return env::server->load_server_xml(true);
	}
	else if(msg.dest() == "game_server"){
		env::server->send_msg_to_all_games(op_cmd::eg_gm_need_reload_xml_request, 0, msg);
	}

	return true;
}