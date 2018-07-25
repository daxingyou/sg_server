#include "login_msg_handler.hpp"
#include "login_manager.hpp"
#include "main/account_server.hpp"
#include "main/account_server_fwd.hpp"
#include "network.hpp"
#include <vector>
#include "serverlist/server_info_manager.hpp"

USING_NS_NETWORK;
USING_NS_COMMON;

server_msg_handler_t<op_cmd::LN_BEGIN, op_cmd::LN_END, login_msg_handler_t::msg_handler_func> login_msg_handler_t::m_login_msg_handle;

bool login_msg_handler_t::init_msg_handler()
{
	m_login_msg_handle.register_func(op_cmd::ln_run_server_notify, handle_ln_run_server_notify);
	m_login_msg_handle.register_func(op_cmd::ln_close_server_notify, handle_ln_close_server_notify);
	m_login_msg_handle.register_func(op_cmd::ln_user_num_notify, handle_ln_user_num_notify);
    return true;
}

bool login_msg_handler_t::handle_ln_run_server_notify(const tcp_socket_ptr& s, const msg_buf_ptr& msg_buf)
{
	PRE_S2ACCOUNT_MSG(proto::server::ln_run_server_notify);

	//判断login_server_id是否在我的列表中
	if ( !server_info_manager_t::is_in_login_server_list(msg.server_id()) )
	{
		log_error("undefine server_id[%d],server_name[%s],ip[%s],port:[%d] connect failed!", msg.server_id(), msg.server_name().c_str(), msg.login_ip().c_str(), msg.login_port() );
		env::server->post_network_close_socket(s);
		return true;
	}

    login_manager_t::add_login(msg.server_id(), s, msg.login_ip(), msg.login_port(), msg.server_name(), msg.can_create_new_role() );
	return true;
}

bool login_msg_handler_t::handle_ln_close_server_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
    /*PRE_S2ACCOUNT_MSG(proto::server::ln_close_server_notify);
    login_ptr p_login = login_manager_t::get_login(msg.server_id());
    if (NULL == p_login || p_login->get_state() == uint32_t(proto::common::SERVER_STATE_CLOSE))
    {
        log_error("login_server[%d] already closed!!", msg.server_id());
        env::server->post_network_close_socket(s);
        return false;
    }
    p_login->set_state(uint32_t(proto::common::SERVER_STATE_CLOSE));

    log_info("login_server[%d] closed!!", msg.server_id());*/
	return true;
}

bool login_msg_handler_t::handle_ln_user_num_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2ACCOUNT_MSG(proto::server::ln_user_num_notify);
	login_ptr p_login = login_manager_t::get_login(msg.server_id());
	if (NULL == p_login || p_login->get_state() == uint32_t(proto::common::SERVER_STATE_CLOSE))
	{
		log_error("login_server[%d] already closed!!", msg.server_id());
		return false;
	}
	p_login->set_user_num(msg.user_num());

	log_info("login_server[%d] notify user num[%d]!!", msg.server_id(), msg.user_num());
	return true;
}
