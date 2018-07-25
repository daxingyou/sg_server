#include "user_manager.hpp"
#include "log.hpp"
#include "main/gate_server.hpp"
#include "main/gate_server_fwd.hpp"
#include "user_msg_handler.hpp"
#include "protos_fwd.hpp"

USING_NS_NETWORK;
USING_NS_COMMON;

user_map user_manager_t::m_user_list;
list_t<tcp_socket_wptr>::type user_manager_t::m_client_socket_list;
boost::shared_ptr<boost::asio::deadline_timer> user_manager_t::m_notify_timer;

bool user_manager_t::find_socket(const tcp_socket_ptr& s)
{
    if (NULL == s)
    {
        log_error("socket is NULL!");
        return false;
    }

    for (auto itr = m_client_socket_list.begin(); itr != m_client_socket_list.end(); ++itr)
    {
        const tcp_socket_ptr p_socket = (*itr).lock();
        if (NULL != p_socket && s == p_socket)
        {
            return true;
        }
    }

    return false;
}

void user_manager_t::add_socket(const tcp_socket_ptr& s)
{
    if (NULL == s)
    {
        log_error("socket is NULL!");
        return;
    }

    m_client_socket_list.push_front(s);
    s->m_iter = new ITER_T(m_client_socket_list.begin());
    
    socket_data_t* p_socket_data = new socket_data_t();
	if (NULL != p_socket_data)
	{
		p_socket_data->init(s);
		s->set_bind_data(p_socket_data);
	}

    try
    {
		ROLE_LOG("add new socket[%s: %d] success!", s->socket().remote_endpoint().address().to_string().c_str(), s->socket().native());
    }
    catch (boost::system::system_error &ec)
    {
        log_error("throw exception: %s", ec.what());
    }
}

bool user_manager_t::socket_bind_user(const tcp_socket_ptr& s, const user_ptr& p_user)
{
    if (NULL == s)
    {
        log_error("NULL == s");
        return false;
    }

    if (NULL == p_user)
    {
        log_error("NULL == p_user");
        return false;
    }

    socket_data_t* p_socket_data = (socket_data_t*)(s->get_bind_data());
    if (NULL == p_socket_data)
    {
        log_error("NULL == p_socket_data");
        return false;
    }

	tcp_socket_ptr old_socket = p_user->get_client_socket();
	if (old_socket == s)
	{
		// 重复绑定
		log_error("old_socket == s");
		return false;
	}
	
	if (NULL != old_socket)
	{
		// 绑定的时候不应该出现有老的socket
		log_warn("NULL != old_socket");

		user_manager_t::del_socket(old_socket, false);
	}

	p_socket_data->set_user(p_user);

    p_user->reset_client_socket(s);

    return true;
}

bool user_manager_t::del_socket(const tcp_socket_ptr& s, bool is_del_user)
{
    if (NULL == s)
    {
        log_error("socket is NULL!");
        return false;
    }

    if (!find_socket(s))
    {
        log_error("not find socket[%d:%d]!", s->socket().native(), s->get_socket_id());
        return false;
    }

    socket_data_t* p_socket_data = (socket_data_t*)s->get_bind_data();
    if (NULL != p_socket_data)
    {
		p_socket_data->close();

        const user_ptr& p_user = p_socket_data->get_user();
        if (NULL != p_user)
        {
            if (is_del_user)
            {
                del_user(p_user->get_uid());
            }
			else
			{
				p_user->reset_client_socket(NULL);
			}
        }

        delete p_socket_data;
        p_socket_data = NULL;
    }
    s->set_bind_data(NULL);

    ITER_T* p_iter = (ITER_T*)s->m_iter;
    if (NULL != p_iter)
    {
        m_client_socket_list.erase(p_iter->i);
        delete p_iter;
        p_iter = NULL;
    }

    env::server->post_network_close_socket(s);

    if (m_client_socket_list.empty())
    {
        log_info("no client connection!");
    }

    return true;
}

void user_manager_t::socket_session_timeout_callback(const boost::system::error_code& ec, const tcp_socket_ptr& s)
{
    if (ec == boost::asio::error::operation_aborted)
    {
        //
        return;
    }
    else if (ec)
    {
        log_error("Timer error: %s", ec.message().c_str());
        return;
    }

    if (NULL == s)
    {
        log_error("socket is NULL!");
        return;
    }

	ROLE_LOG("client[%d] session timeout!", s->socket().native());

	// 会话超时说明连上了gate 但是没有发ca_connect_request，没有和任何user绑定
	del_socket(s, true);
}

user_ptr user_manager_t::get_user(uint64_t uid)
{
    user_map::iterator itr = m_user_list.find(uid);
    if (itr != m_user_list.end())
    {
		return itr->second;
    }

    return NULL;
}

user_ptr user_manager_t::get_user(const tcp_socket_ptr& s)
{
    if (NULL == s)
    {
        log_error("socket is NULL!");
		return NULL;
    }
    
    if (!find_socket(s))
    {
        log_error("not find socket[%d:%d]!", s->socket().native(), s->get_socket_id());
		return NULL;
    }

    socket_data_t* p_socket_data = (socket_data_t*)s->get_bind_data();
    if (NULL == p_socket_data)
    {
        log_error("p_socket_data is NULL!");
		return NULL;
    }

    return p_socket_data->get_user();
}

void user_manager_t::del_user(uint64_t uid, bool is_notify_login /*= true*/)
{
    user_map::iterator itr = m_user_list.find(uid);
    if (itr != m_user_list.end())
    {
		const user_ptr& p_user = itr->second;
		if (NULL != p_user)
		{
			p_user->cancel_all_timer();
			ROLE_LOG("delete user[%lu], reason: %d", uid, p_user->get_kick_reason());
		}
		else
		{
			log_error("p_user == NULL, uid:%lu", uid);
		}

		// 通知login
		if (is_notify_login)
		{
			notify_login_by_kick_user(uid);
		}

		// 把自己从game列表中删除
		game_manager_t::del_user_from_game(uid);

		m_user_list.erase(itr);
    }
}

void user_manager_t::clear_all_users_and_socket(bool is_notify_login, common::KICK_ROLE_REASON reason)
{
	//TODO:qhj 这里不应该一条一条发 后期在优化
	for (auto itr = m_client_socket_list.begin(); itr != m_client_socket_list.end(); ++itr)
	{
		tcp_socket_ptr p_socket = (*itr).lock();
		if (NULL != p_socket)
		{
			socket_data_t* p_socket_data = (socket_data_t*)p_socket->get_bind_data();
			if (NULL != p_socket_data)
			{
				p_socket_data->close();

				const user_ptr& p_user = p_socket_data->get_user();
				if (NULL != p_user)
				{
					p_user->set_kick_reason(reason);

					del_user(p_user->get_uid(), is_notify_login);
				}

				delete p_socket_data;
				p_socket_data = NULL;
			}

			p_socket->set_bind_data(NULL);

			ITER_T* p_iter = (ITER_T*)p_socket->m_iter;
			if (NULL != p_iter)
			{
				delete p_iter;
				p_iter = NULL;
			}

			env::server->post_network_close_socket(p_socket);
		}
	}

	m_client_socket_list.clear();

    for (auto itr = m_user_list.begin(); itr != m_user_list.end(); ++itr)
    {
        user_ptr p_user = itr->second;
        if (NULL != p_user)
        {
            p_user->cancel_all_timer();

			// 通知login
			if (is_notify_login)
			{
				notify_login_by_kick_user(p_user->get_uid());
			}

			// 把自己从game列表中删除
			game_manager_t::del_user_from_game(p_user->get_uid());
        }
    }

	m_user_list.clear();

	log_info("clear all sockets and users success!");
}

void user_manager_t::start_sync_user_num_to_login_timer()
{
    if (0 == env::xml->timeout.notify_login_time)
    {
        log_warn("notify_login_time is 0 !");
        return;
    }

	if (NULL == m_notify_timer)
	{
		m_notify_timer.reset(new boost::asio::deadline_timer(env::server->get_ios()));
	}

	m_notify_timer->expires_from_now(boost::posix_time::seconds(env::xml->timeout.notify_login_time));
	m_notify_timer->async_wait(boost::bind(&user_manager_t::handle_sync_user_num_to_login, boost::asio::placeholders::error));
}

void user_manager_t::handle_sync_user_num_to_login(const boost::system::error_code& ec)
{
    if (ec == boost::asio::error::operation_aborted)
    {
        
        return;
    }
    else if (ec)
    {
        log_error("Timer error: %s", ec.message().c_str());
        return;
    }

    uint32_t num = uint32_t(m_client_socket_list.size());

    proto::server::al_update_gate_user_num_notify ugpn;
    ugpn.set_num(num);
    env::server->send_msg_to_login(op_cmd::al_update_gate_user_num_notify, 0, ugpn);
    //log_info("the %d users on the gate", num);

    start_sync_user_num_to_login_timer();
}

void user_manager_t::cancel_sync_user_num_to_login()
{
    if (NULL != m_notify_timer)
    {
        m_notify_timer->cancel();
    }
}


void user_manager_t::pingpong_timeout_callback(const boost::system::error_code& ec, uint64_t uid)
{
	if (ec == boost::asio::error::operation_aborted)
	{
		//
		return;
	}
	else if (ec)
	{
		log_error("Timer error: %s", ec.message().c_str());
		return;
	}

	const user_ptr& p_user = get_user(uid);
	if (NULL == p_user)
	{
		log_error("p_user[%lu] null error when pingpong_timeout_callback", uid);
		return;
	}

	p_user->set_kick_reason(KICK_ROLE_REASON_PINGPONG_TIMEOUT);

	switch (p_user->get_game_state())
	{
	case common::ROLE_GAME_STATE_INIT:		// 对于一个还未绑定任何socket的user 直接删除
		{
			log_warn("user[%lu] long time no bind socket, so delete!", uid);

			del_user(uid);
		}
		break;
	case common::ROLE_GAME_STATE_GATE:		// 对于在登陆到gate，还没进入游戏的直接删除
		{
			log_warn("user[%lu] long time no login game, so delete!", uid);

			if (NULL != p_user->get_client_socket())
			{
				del_socket(p_user->get_client_socket(), true);
			}
			else
			{
				del_user(uid);
			}
		}
		break;
	case common::ROLE_GAME_STATE_LOGIN:
	case common::ROLE_GAME_STATE_PLAY:
	case common::ROLE_GAME_STATE_WAIT:
		{
			ROLE_LOG("user[%lu] pingpong timeout, then waitting to reconnect...", uid);

			// 对于在线玩家，通知下线
			if (p_user->is_play_state())
			{
				notify_others_unreg_user(p_user, common::UNREGUSER_RET_OFFLINE);
			}

			del_socket(p_user->get_client_socket(), false);

			p_user->set_game_offline();
		}
		break;
	default:	// 对于离线和注销状态不用处理
		{
			log_error("user[%lu] pingpong_timeout_callback error state", uid);
		}
		break;
	}
}

void user_manager_t::reconnect_timeout_callback(const boost::system::error_code& ec, uint64_t uid)
{
	if (ec == boost::asio::error::operation_aborted)
	{
		
		return;
	}
	else if (ec)
	{
		log_error("Timer error: %s", ec.message().c_str());
		return;
	}

	//重连超时，服务器认为客户端已经真正下线，所以客户端必须返回登陆界面
	ROLE_LOG("user[%lu] reconnect timeout, client need to relogin!", uid);

	const user_ptr& p_user = get_user(uid);
	if (NULL == p_user)
	{
		log_error("p_user[%lu] null error when reconnect_timeout_callback", uid);
		return;
	}

	p_user->set_kick_reason(KICK_ROLE_REASON_RECONNECT_TIMEOUT);

	notify_others_unreg_user(p_user, common::UNREGUSER_RET_LOGOUT);

	if (NULL != p_user->get_client_socket())
	{
		log_warn("when reconnect timeout, but the socket of user[%lu] is not deleted!", p_user->get_uid());

		del_socket(p_user->get_client_socket(), false);
	}

	p_user->set_logout_state();

	// 下线，把自己从game列表中删除
	game_manager_t::del_user_from_game(uid);
}

void user_manager_t::alive_timeout_callback(const boost::system::error_code& ec, uint64_t uid)
{
	if (ec == boost::asio::error::operation_aborted)
	{
		
		return;
	}
	else if (ec)
	{
		log_error("Timer error: %s", ec.message().c_str());
		return;
	}

	ROLE_LOG("user[%lu] cache alive timeout!", uid);

	const user_ptr& p_user = get_user(uid);
	if (NULL == p_user)
	{
		log_error("p_user[%lu] null error when alive_timeout_callback", uid);
		return;
	}

	p_user->set_kick_reason(KICK_ROLE_REASON_ALIVE_TIMEOUT);

	if (NULL != p_user->get_client_socket())
	{
		log_warn("alive timeout, but the socket of user[%lu] is not deleted!", p_user->get_uid());

		del_socket(p_user->get_client_socket(), true);
	}
	else
	{
		del_user(uid);
	}
}

void user_manager_t::kick_user(const network::tcp_socket_ptr& s, user_ptr p_user, common::KICK_ROLE_REASON kick_reason)
{
    if (NULL == p_user)
    {
        log_error("NULL == p_user");
        return;
    }
    uint64_t uid = p_user->get_uid();
    std::string account = p_user->get_guid();

	if (NULL == s)
	{
		log_error("NULL == s");
		return;
	}
    uint32_t socket_id = s->socket().native();

	if (s != p_user->get_client_socket())
	{
		log_error("s != p_user->get_client_socket() uid[%lu]", uid);
		return;
	}

    p_user->set_kick_reason(kick_reason);

    switch (kick_reason)
    {
    case KICK_ROLE_REASON_NORMAL:
        {
			if (p_user->is_login_game())
			{
				// 对于在线玩家，通知下线
				if (p_user->is_play_state())
				{
					notify_others_unreg_user(p_user, common::UNREGUSER_RET_OFFLINE);
				}

				del_socket(s, false);

				p_user->cancel_pingpong_timer();

				p_user->set_game_offline();
			}
			else
			{
				del_socket(s, true);
			}
        }
        break;
    case KICK_ROLE_REASON_MSG_BOMB:
    case KICK_ROLE_REASON_BAN_ACCOUNT:
        {
			if (p_user->is_login_game())
			{
				notify_others_unreg_user(p_user, common::UNREGUSER_RET_LOGOUT);
			}

			del_socket(s, true);
        }
        break;
    default:
        break;
    }

    ROLE_LOG("The user[uid=%lu guid=%s socket=%d] has been kicked, reason:%d", uid, account.c_str(), socket_id, kick_reason);
}

void user_manager_t::notify_login_by_kick_user(uint64_t uid)
{
    proto::server::al_kick_role_notify al_kick;
    env::server->send_msg_to_login(op_cmd::al_kick_role_notify, uid, al_kick);
}

//////////////////////////////////////////////////////////////////////////

bool user_manager_t::add_user(const user_ptr& p_new_user)
{
	if (NULL == p_new_user)
	{
		log_error("p_new_user null error");
		return false;
	}

	std::pair<user_map::iterator, bool> res = m_user_list.insert(std::make_pair(p_new_user->get_uid(), p_new_user));
	if (!res.second)
	{
		log_error("user[%lu] insert error, it is exsit!!!", p_new_user->get_uid());
		return false;
	}

	ROLE_LOG("add new user[%lu]", p_new_user->get_uid());
	return true;
}

void user_manager_t::notify_others_unreg_user(const user_ptr& p_user, common::UNREGUSER_RET_TYPE type)
{
	if (NULL == p_user)
	{
		log_error("p_user is NULL!");
		return;
	}

	const network::tcp_socket_ptr& p_game_socket = game_manager_t::get_game_socket_by_user_uid(p_user->get_uid());
	const network::tcp_socket_ptr& p_cross_socket = p_user->get_cross_id() != 0 ? game_manager_t::get_game_socket_by_id(p_user->get_cross_id(), game_server_type_cross) : NULL;

	// 对于还未登录场景的玩家不处理
	if (NULL != p_game_socket || NULL != p_cross_socket)
	{
		proto::server::ag_unreg_user_scene_notify ag_notify;
		ag_notify.set_uid(p_user->get_uid());
		ag_notify.set_unreg_code(type);
		ag_notify.set_kick_reason(p_user->get_kick_reason());

		if (NULL != p_game_socket)
		{
			env::server->send_msg_to_game(p_game_socket, op_cmd::ag_unreg_user_scene_notify, p_user->get_uid(), ag_notify);
		}

		if (NULL != p_cross_socket)
		{
			env::server->send_msg_to_game(p_cross_socket, op_cmd::ag_unreg_user_scene_notify, p_user->get_uid(), ag_notify);
		}
	}

	// 如果是登出，正常先在game登出，然后game通知center登出
	// 如果不是登出或者这个时候没有和game建立连接就直接通知center登出
	if (type != common::UNREGUSER_RET_LOGOUT || NULL == p_game_socket)
	{
		proto::server::ae_unreg_user_scene_notify ae_notify;
		ae_notify.set_uid(p_user->get_uid());
		ae_notify.set_unreg_code(type);
		ae_notify.set_kick_reason(p_user->get_kick_reason());
		env::server->send_msg_to_center(op_cmd::ae_unreg_user_scene_notify, p_user->get_uid(), ae_notify);
	}
}
