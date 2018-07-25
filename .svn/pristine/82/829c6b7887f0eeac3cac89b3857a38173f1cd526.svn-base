#include "user.hpp"
#include "user_manager.hpp"
#include "main/gate_server.hpp"
#include "main/gate_server_fwd.hpp"

USING_NS_NETWORK;
USING_NS_COMMON;

user_t::user_t(uint64_t uid, std::string const& guid)
    : m_uid(uid)
    , m_guid(guid)
{
}

bool user_t::send_msg(const msg_buf_ptr& buf)
{
	const network::tcp_socket_ptr& p_client_socket = get_client_socket();
	if (NULL != p_client_socket)
	{
		env::server->post_network_send_msg(p_client_socket, buf);
		return true;
	}

	return false;
}

////////////////////////////////////////////////////////////////////////////////////////////
void user_t::cancel_all_timer()
{
	cancel_pingpong_timer();
	cancel_reconnect_timer();
	cancel_alive_timer();
}

void user_t::start_pingpong_timer()
{
    if (0 == env::xml->timeout.pingpong)
    {
        return;
    }

    if (NULL == m_pingpong_timer)
    {
        m_pingpong_timer.reset(new boost::asio::deadline_timer(env::server->get_ios()));
    }

    m_pingpong_timer->expires_from_now(boost::posix_time::seconds(env::xml->timeout.pingpong));
    m_pingpong_timer->async_wait(boost::bind(&user_manager_t::pingpong_timeout_callback, boost::asio::placeholders::error, m_uid));
}

void user_t::cancel_pingpong_timer()
{
    if (NULL != m_pingpong_timer)
    {
        m_pingpong_timer->cancel();
	}
}

///////////////////////////////////////////////////////////////////////
void user_t::start_reconnect_timer()
{
    if (0 == env::xml->timeout.reconnect)
    {
        log_warn("client reconnect timeout is 0 !");
        return;
    }

    if (NULL == m_reconnect_timer)
    {
        m_reconnect_timer.reset(new boost::asio::deadline_timer(env::server->get_ios()));
    }

    m_reconnect_timer->expires_from_now(boost::posix_time::seconds(env::xml->timeout.reconnect));
    m_reconnect_timer->async_wait(boost::bind(&user_manager_t::reconnect_timeout_callback, boost::asio::placeholders::error, m_uid));
}

void user_t::cancel_reconnect_timer()
{
    if (NULL != m_reconnect_timer)
    {
        m_reconnect_timer->cancel();
    }
}

////////////////////////////////////////////////////////////////////////////////
void user_t::start_alive_timer()
{
    if (0 == env::xml->timeout.alive)
    {
        log_warn("client alive timeout is 0 !");
        return;
    }

    if (NULL == m_alive_timer)
    {
        m_alive_timer.reset(new boost::asio::deadline_timer(env::server->get_ios()));
    }

    m_alive_timer->expires_from_now(boost::posix_time::seconds(env::xml->timeout.alive));
    m_alive_timer->async_wait(boost::bind(&user_manager_t::alive_timeout_callback, boost::asio::placeholders::error, m_uid));
}

void user_t::cancel_alive_timer()
{
    if (NULL != m_alive_timer)
    {
        m_alive_timer->cancel();
    }
}


bool user_t::is_play_state() const
{
	return m_game_state == common::ROLE_GAME_STATE_PLAY;
}

void user_t::set_game_offline()
{
	m_game_state = common::ROLE_GAME_STATE_OFFLINE;

	start_reconnect_timer();
}

bool user_t::is_login_game() const
{
	if (m_game_state == common::ROLE_GAME_STATE_LOGIN ||
		m_game_state == common::ROLE_GAME_STATE_PLAY ||
		m_game_state == common::ROLE_GAME_STATE_WAIT)
	{
		return true;
	}

	return false;
}

bool user_t::is_offline_state() const
{
	return m_game_state == common::ROLE_GAME_STATE_OFFLINE;
}

bool user_t::is_unreg_state() const
{
	return m_game_state == common::ROLE_GAME_STATE_UNREG;
}

bool user_t::is_init_state() const
{
	return m_game_state == common::ROLE_GAME_STATE_INIT;
}

void user_t::set_login_gate()
{
	m_game_state = common::ROLE_GAME_STATE_GATE;

	// 重置所有定时器
	cancel_all_timer();
	start_pingpong_timer();
}

void user_t::set_logout_state()
{
	m_game_state = common::ROLE_GAME_STATE_UNREG;

	start_alive_timer();
}

bool user_t::is_gate_state() const
{
	return m_game_state == common::ROLE_GAME_STATE_GATE;
}

void user_t::set_login_game()
{
	m_game_state = common::ROLE_GAME_STATE_LOGIN;

	// 清理下创角状态
	clear_create_state();
}

void user_t::set_init_state()
{
	m_game_state = common::ROLE_GAME_STATE_INIT;
}

bool user_t::is_login_state() const
{
	return m_game_state == common::ROLE_GAME_STATE_LOGIN;
}

void user_t::set_play_state()
{
	m_game_state = common::ROLE_GAME_STATE_PLAY;
}

bool user_t::is_wait_state() const
{
	return m_game_state == common::ROLE_GAME_STATE_WAIT;
}

void user_t::set_wait_state()
{
	m_game_state = common::ROLE_GAME_STATE_WAIT;
}

void user_t::set_create_state()
{
	m_is_create_state = true;
	m_create_time = common::time_util_t::now_time() + 5;
}

bool user_t::is_create_state()
{
	if (m_is_create_state && common::time_util_t::now_time() < m_create_time ) {
		return true;
	}

	return false;
}

void user_t::clear_create_state()
{
	if (m_is_create_state) {
		m_is_create_state = false;
		m_create_time = 0;
	}
}

//////////////////////////////////////////////////////////////////////////
void socket_data_t::init(const network::tcp_socket_ptr& s)
{
    if (NULL == s)
    {
        log_error("s == NULL");
        return;
    }

	if (env::xml->timeout.session > 0)
	{
        if (NULL == m_session_timer)
        {
            m_session_timer.reset(new boost::asio::deadline_timer(env::server->get_ios()));
        }

        m_session_timer->expires_from_now(boost::posix_time::seconds(env::xml->timeout.session));
        m_session_timer->async_wait(boost::bind(&user_manager_t::socket_session_timeout_callback, boost::asio::placeholders::error, s));
	}
}

void socket_data_t::close()
{
	if (NULL != m_session_timer)
	{
		m_session_timer->cancel();
	}
}

void socket_data_t::set_user(const user_ptr& p_user)
{
	m_user_wptr = p_user;

	// 绑定user之后，user上的pingpang接管这个socket的管理
	close();
}

////////////////////////////////////////////////////////////////////////////////////////////
//bool socket_data_t::check_msg_bomb(uint32_t cmd)
//{
//	if (0 == env::xml->timeout.msg_bomb_interval)
//	{
//		return true;
//	}
//	if (cmd == op_cmd::ca_pingpong_request || cmd == op_cmd::cg_move_request)
//	{
//		return true;
//	}
//	if (cmd != m_last_cmd)
//	{
//		m_last_cmd = cmd;
//		return true;
//	}
//	uint64_t curr_time = time_util_t::now_msecs();
//	if (curr_time - m_last_msg_time < env::xml->timeout.msg_bomb_interval)
//	{
//		++m_msg_bomb_count;
//		if (m_msg_bomb_count >= env::xml->timeout.msg_bomb_count)
//		{
//			return false;
//		}
//	}
//	else
//	{
//		if (m_msg_bomb_count > 0)
//		{
//			--m_msg_bomb_count;
//		}
//	}
//	m_last_msg_time = curr_time;
//	return true;
//}
bool socket_data_t::check_msg_bomb(uint32_t cmd)
{
    if (0 == env::xml->timeout.msg_bomb_interval)
    {
        return true;
    }
    if (cmd == op_cmd::ca_pingpong_request || cmd == op_cmd::cg_move_request)
    {
        return true;
    }

    if (0 == m_msg_bomb_count)
    {
        m_last_msg_time = time_util_t::now_msecs();
    }

    ++m_msg_bomb_count;

    if (m_msg_bomb_count >= env::xml->timeout.msg_bomb_count)
    {
        uint64_t curr_time = time_util_t::now_msecs();
        if (curr_time - m_last_msg_time <= env::xml->timeout.msg_bomb_interval)
        {
            return false;
        }
        else
        {
            m_msg_bomb_count = 0;
            m_last_msg_time = curr_time;
        }
    }
    return true;
}