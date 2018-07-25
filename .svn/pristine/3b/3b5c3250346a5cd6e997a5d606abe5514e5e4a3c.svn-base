#include "login_manager.hpp"
#include "log.hpp"
#include "main/account_server.hpp"
#include "main/account_server_fwd.hpp"
#include "protos_fwd.hpp"
#include "serverlist/server_info_manager.hpp"

USING_NS_NETWORK;
USING_NS_COMMON;

login_map login_manager_t::m_login_map;

login_ptr login_manager_t::get_login(uint32_t server_id)
{
	login_map::iterator it = m_login_map.find(server_id);
	
	if (it != m_login_map.end())
	{
		return it->second;
	}
 
    return login_ptr();
}

login_ptr login_manager_t::get_login(const tcp_socket_ptr& s)
{
	if (NULL == s)
	{
		log_error("socket is NULL!");
		return login_ptr();
	}

	login_socket_data_t* p_socket_data = (login_socket_data_t*)s->get_bind_data();
	if (NULL == p_socket_data)
	{
		log_error("p_socket_data is NULL!");
		return login_ptr();
	}

	if (NULL == p_socket_data->p_login)
	{
		log_error("p_socket_data->p_login is NULL!");
		return login_ptr();
	}

	return p_socket_data->p_login;
}

void login_manager_t::add_login(uint32_t server_id, const network::tcp_socket_ptr& s, const std::string& login_ip, uint32_t login_port, const std::string& login_name, uint32_t can_create_new_role )
{
    if (0 == server_id)
    {
        log_error("0 == server_id");
        return;
    }
    if (NULL == s)
    {
        log_error("socket is NULL!");
        return;
    }

    login_ptr p_login = get_login(server_id);
    if (NULL == p_login)
    {
        p_login = login_ptr(new login_t());
        p_login->set_id(server_id);
        m_login_map.insert(std::make_pair(p_login->get_id(), p_login));
	}
	else {
		if (p_login->get_state() != proto::common::SERVER_STATE_CLOSE) {
			log_error("server_id :%u  connect refused, case login_server is online", server_id);
			return;
		}
	}
	
    socket_bind_login(s, p_login);
    p_login->set_ip(login_ip);
    p_login->set_port(login_port);
    p_login->set_name(login_name);
    p_login->set_state(proto::common::SERVER_STATE_FREE);
	p_login->set_can_create_new_role(can_create_new_role);
    log_info("add login_server[%d:%s:%s:%d]", p_login->get_id(), p_login->get_name().c_str(), p_login->get_ip().c_str(), p_login->get_port());

    // 同步account信息给login
    proto::server::nl_syn_account_info_notify syn_msg;
    syn_msg.set_account_id(env::server->get_id());
    env::server->send_msg_to_login(s, op_cmd::nl_syn_account_info_notify, 0, syn_msg);
}

void login_manager_t::del_login(const network::tcp_socket_ptr& s)
{
	if (NULL == s)
	{
		log_error("socket is NULL!");
		return;
	}
    login_ptr p_login = login_manager_t::get_login(s);
    if (NULL == p_login)
    {
        log_error("NULL == p_login");
        return;
    }
    if (p_login->get_state() == uint32_t(proto::common::SERVER_STATE_CLOSE))
    {
        log_error("login_server[%d] has been closed!!", p_login->get_id());
        return;
    }
    if (NULL == p_login->get_socket())
    {
        log_error("p_login->get_socket() is NULL!");
        return;
    }
    if (p_login->get_socket() != s)
    {
        log_error("p_login->get_socket() != s");
        return;
    }

    p_login->set_state(proto::common::SERVER_STATE_CLOSE);

	login_socket_data_t* p_socket_data = (login_socket_data_t*)p_login->get_socket()->get_bind_data();
    SAFE_DELETE_P(p_socket_data);
    p_login->get_socket()->set_bind_data(NULL);
    env::server->post_network_close_socket(p_login->get_socket());

    log_info("delete login_server[%d:%s:%s:%d]", p_login->get_id(), p_login->get_name().c_str(), p_login->get_ip().c_str(), p_login->get_port());
}

void login_manager_t::clear_login()
{
    for (auto& it : m_login_map)
    {
        login_ptr p_login = it.second;
        if (NULL == p_login)
        {
            continue;
        }

        p_login->set_state(proto::common::SERVER_STATE_CLOSE);

        if (NULL != p_login->get_socket())
        {
            login_socket_data_t* p_socket_data = (login_socket_data_t*)p_login->get_socket()->get_bind_data();
            SAFE_DELETE_P(p_socket_data);

            env::server->post_network_close_socket(p_login->get_socket());
        }
    }

	m_login_map.clear();
    log_info("clear all logins!");
}

bool login_manager_t::socket_bind_login(const tcp_socket_ptr& s, login_ptr p_login)
{
	if (NULL == s)
	{
		log_error("NULL == s");
		return false;
	}
	if (NULL == p_login)
	{
		log_error("NULL == p_login");
		return false;
	}

	login_socket_data_t* p_socket_data = (login_socket_data_t*)(s->get_bind_data());
	if (NULL == p_socket_data)
	{
		p_socket_data = new login_socket_data_t();
	}
	p_socket_data->p_login = p_login;
	s->set_bind_data(p_socket_data);
    p_login->reset_socket(s);

	return true;
}

bool login_manager_t::handle_gm_msg(proto::server::ln_send_gm_msg_to_account_request &request)
{
	login_ptr p_login = login_manager_t::get_login(request.server_id());
	if (NULL == p_login || p_login->get_state() == uint32_t(proto::common::SERVER_STATE_CLOSE))
	{
		log_error("login_server[%d] already closed!!", request.server_id());
		return false;
	}
	const proto::common::gm_msg_to_account &p_data = request.msg();
	switch ( request.cmd_type() )
	{
		//设置限制创角
		case op_cmd::tm_set_user_create_limit_request:
			p_login->set_can_create_new_role(p_data.user_create_flag());
		break;
		
		//刷新login列表
		case op_cmd::tm_refresh_login_list:
			server_info_manager_t::load_server_info();
		break;

		default:
		log_error("login_server[%d] undefine cmd_type [%u]!!", request.server_id(), request.cmd_type());
		break;
	}
	return true;
}
