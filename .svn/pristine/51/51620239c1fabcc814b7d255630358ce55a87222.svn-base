#include "gate_manager.hpp"
#include "main/game_server_fwd.hpp"
#include "main/game_server.hpp"
#include "role/role_manager.hpp"

USING_NS_NETWORK;
USING_NS_COMMON;

gate_vec gate_manager_t::m_gate_list;

void gate_manager_t::add_gate(const tcp_socket_ptr& s)
{
    if (NULL == s)
    {
        log_error("NULL == s");
        return;
    }

    gate_ptr p_gate = get_gate_by_socket(s);
    if (NULL == p_gate)
    {
        p_gate.reset(new gate_t());
        p_gate->set_socket(s);
        m_gate_list.push_back(p_gate);
        return;
    }
    else
    {
        try
        {
            log_error("gate socket[%s: %d] has already exit!", s->socket().remote_endpoint().address().to_string().c_str(), s->socket().native());
        }
        catch (boost::system::system_error &ec)
        {
            log_error("throw exception: %s", ec.what());
        }
    }
}

void gate_manager_t::set_gate(const tcp_socket_ptr& s, uint32_t gate_id, uint32_t server_id)
{
    if (NULL == s)
    {
        log_error("NULL == s");
        return;
    }
    if (gate_id == 0)
    {
        log_error("gate_id == 0");
        return;
    }
    gate_ptr p_gate = get_gate_by_socket(s);
    if (NULL == p_gate)
    {
        log_error("NULL == p_gate");
        return;
    }

    p_gate->set_id(gate_id);
	p_gate->set_server_id(server_id);

    // 同步game服务器信息给该gate服务器
    proto::server::ga_syn_game_info_notify noti;
    noti.set_game_id(env::server->get_id());
	noti.set_game_type(env::server->get_type());
    env::server->send_msg_to_gate(s, op_cmd::ga_syn_game_info_notify, 0, noti);

    log_info("add gate[server_id=%d: gate_id=%d], has %d gates", server_id, gate_id, m_gate_list.size());
}

void gate_manager_t::del_gate(const tcp_socket_ptr& s)
{
    if (NULL == s)
    {
        log_error("NULL == s");
        return;
    }

    auto it = m_gate_list.begin();
    auto it_end = m_gate_list.end();
    uint32_t gate_id = 0;
    for (; it != it_end; ++it)
    {
        gate_ptr p_gate = (*it);
        if (NULL != p_gate && p_gate->get_socket() == s)
        {
            log_info("del gate[server_id=%d: gate_id=%d], left %d gates", p_gate->get_server_id(), p_gate->get_id(), m_gate_list.size()-1);
            gate_id = p_gate->get_id();
            env::server->post_network_close_socket(p_gate->get_socket());
            m_gate_list.erase(it);
            break;
        }
    }

    //彻底删除对应gate上的玩家
    if (gate_id > 0)
    {
        role_manager_t::del_role(gate_id);
    }
}

void gate_manager_t::clear_gate()
{
    for (auto p_gate : m_gate_list)
    {
        if (NULL != p_gate)
        {
            env::server->post_network_close_socket(p_gate->get_socket());
        }
    }
    m_gate_list.clear();
}

//-------------------------------------------------------------------------------------------
gate_ptr gate_manager_t::get_gate_by_socket(const tcp_socket_ptr& s)
{
    if (NULL == s)
    {
        log_error("NULL == s");
        return gate_ptr();
    }

    for (auto p_gate : m_gate_list)
    {
        if (NULL != p_gate && p_gate->get_socket() == s)
        {
            return p_gate;
        }
    }
    return gate_ptr();
}

gate_ptr gate_manager_t::get_gate_by_id(uint32_t gate_id, uint32_t server_id)
{
    if (0 == gate_id)
    {
        log_error("0 == gate_id");
        return gate_ptr();
    }

    for (auto p_gate : m_gate_list)
    {
        if (NULL != p_gate && p_gate->get_id() == gate_id && p_gate->get_server_id() == server_id)
        {
            return p_gate;
        }
    }
    return gate_ptr();
}

uint32_t gate_manager_t::get_gate_id_by_socket(const tcp_socket_ptr& s)
{
    if (NULL == s)
    {
        log_error("NULL == s");
        return 0;
    }

    for (auto p_gate : m_gate_list)
    {
        if (NULL != p_gate && p_gate->get_socket() == s)
        {
            return p_gate->get_id();
        }
    }

    return 0;
}


const tcp_socket_ptr gate_manager_t::get_gate_socket_by_id(uint32_t gate_id, uint32_t server_id)
{
    if (0 == gate_id)
    {
        log_error("0 == gate_id");
        return NULL;
    }

    for (auto p_gate : m_gate_list)
    {
        if (NULL != p_gate && p_gate->get_id() == gate_id && p_gate->get_server_id() == server_id)
        {
            return p_gate->get_socket();
        }
    }

    return NULL;
}

uint32_t gate_manager_t::get_server_id_by_gate_socket(const network::tcp_socket_ptr& s)
{
	if (NULL == s)
	{
		log_error("NULL == s");
		return 0;
	}

	for (auto p_gate : m_gate_list)
	{
		if (NULL != p_gate && p_gate->get_socket() == s)
		{
			return p_gate->get_server_id();
		}
	}

	return 0;
}
