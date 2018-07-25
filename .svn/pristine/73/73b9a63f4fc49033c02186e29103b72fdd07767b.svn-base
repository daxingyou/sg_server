#include "user.hpp"
#include "main/login_server.hpp"
#include "main/login_server_fwd.hpp"

USING_NS_NETWORK;
USING_NS_COMMON;

user_t::user_t(uint64_t uid, std::string const& guid)
    : m_uid(uid)
    , m_guid(guid)
    //, m_login_timer(env::server->get_ios())
{
}

bool user_t::send_msg(const msg_buf_ptr& buf)
{
    if (get_client_socket())
    {
        env::server->post_network_send_msg(get_client_socket(), buf);
        return false;
    }
    return true;
}

tcp_socket_ptr user_t::reset_client_socket(tcp_socket_ptr const& s)
{
    tcp_socket_ptr old_socket;
    if (NULL != get_client_socket())
    {
        socket_data_t* data = (socket_data_t*)get_client_socket()->get_bind_data();
        if (NULL != data)
        {
            delete data;
            data = NULL;
        }
        get_client_socket()->set_bind_data(NULL);
        old_socket = get_client_socket();
    }
    m_socket = s;
    return old_socket;
}


////////////////////////////////////////////////////////////////////////////////////////////
bool socket_data_t::check_msg_bomb(uint32_t cmd)
{
	if (0 == env::xml->timeout.msg_bomb_interval)
	{
		return true;
	}
	if (cmd != m_last_cmd)
	{
		m_last_cmd = cmd;
		return true;
	}
	uint64_t curr_time = time_util_t::now_msecs();
	if (curr_time - m_last_msg_time < env::xml->timeout.msg_bomb_interval)
	{
		++m_msg_bomb_count;
		if (m_msg_bomb_count >= env::xml->timeout.msg_bomb_count)
		{
			return false;
		}
	}
	else
	{
		if (m_msg_bomb_count > 0)
		{
			--m_msg_bomb_count;
		}
	}
	m_last_msg_time = curr_time;
	return true;
}
