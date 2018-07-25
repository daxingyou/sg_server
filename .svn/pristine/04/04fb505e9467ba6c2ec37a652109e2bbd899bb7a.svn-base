#include "user_manager.hpp"
#include "log.hpp"
#include "main/login_server.hpp"
#include "main/login_server_fwd.hpp"
#include "gate/gate_manager.hpp"
#include "common/global_id.hpp"
#include "common/common_struct.hpp"
#include "common/redis_client.hpp"
#include "errcode_enum.hpp"

USING_NS_NETWORK;
USING_NS_COMMON;

list_t<tcp_socket_wptr>::type user_manager_t::m_socket_list;
user_map user_manager_t::m_user_list;
user_queue user_manager_t::m_login_queue;
boost::shared_ptr<boost::asio::deadline_timer> user_manager_t::m_login_queue_timer;

bool user_manager_t::find_socket(const tcp_socket_ptr& s)
{
    if (NULL == s)
    {
        log_error("socket is NULL!");
        return false;
    }

    for (auto itr = m_socket_list.begin(); itr != m_socket_list.end(); ++itr)
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

    
    m_socket_list.push_front(s);

    s->m_iter = new ITER_T(m_socket_list.begin());

    socket_data_t* p_socket_data = new socket_data_t();
    if (env::xml->timeout.session > 0)
    {
        p_socket_data->p_session_timer.reset(new boost::asio::deadline_timer(env::server->get_ios()));
        p_socket_data->p_session_timer->expires_from_now(boost::posix_time::seconds(env::xml->timeout.session));
        p_socket_data->p_session_timer->async_wait(boost::bind(&user_manager_t::socket_session_timeout_callback, boost::asio::placeholders::error, s));
    }
    else
    {
        log_warn("xml->timeout.session is 0");
    }
    s->set_bind_data(p_socket_data);

    try
    {
        ROLE_LOG("add new socket[%s: %d] success!", s->socket().remote_endpoint().address().to_string().c_str(), s->socket().native());
    }
    catch (boost::system::system_error &ec)
    {
        log_error("throw exception: %s", ec.what());
    }
}

void user_manager_t::del_socket(const tcp_socket_ptr& s)
{
    if (NULL == s)
    {
        log_error("socket is NULL!");
        return;
    }

    if (!find_socket(s))
    {
        log_error("not find socket[%d]!", s->socket().native());
        return;
    }

    socket_data_t* p_socket_data = (socket_data_t*)s->get_bind_data();
    if (NULL != p_socket_data)
    {
        if (NULL != p_socket_data->p_user)
        {
            del_user(p_socket_data->p_user->get_uid());
        }

        if (NULL != p_socket_data->p_session_timer)
        {
            p_socket_data->p_session_timer->cancel();
        }
        delete p_socket_data;
        p_socket_data = NULL;
        s->set_bind_data(NULL);
    }
    
    ITER_T* p_iter = (ITER_T*)s->m_iter;
    if (NULL != p_iter)
    {
        m_socket_list.erase(p_iter->i);
        delete p_iter;
        p_iter = NULL;
    }

    env::server->post_network_close_socket(s);
}

void user_manager_t::clear_socket()
{
    for (auto itr = m_socket_list.begin(); itr != m_socket_list.end(); ++itr)
    {
        tcp_socket_ptr p_socket = (*itr).lock();
        if (NULL != p_socket)
        {
            socket_data_t* p_socket_data = (socket_data_t*)p_socket->get_bind_data();
            if (NULL != p_socket_data)
            {
                if (NULL != p_socket_data->p_session_timer)
                {
                    p_socket_data->p_session_timer->cancel();
                }
                delete p_socket_data;
                p_socket_data = NULL;
            }
            p_socket->set_bind_data(NULL);

            ITER_T* it = (ITER_T*)p_socket->m_iter;
            if (NULL != it)
            {
                delete it;
                it = NULL;
            }

            env::server->post_network_close_socket(p_socket);
        }
    }
    m_socket_list.clear();
}

void user_manager_t::socket_session_timeout_callback(const boost::system::error_code& ec, const tcp_socket_ptr& s)
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

    if (NULL == s)
    {
        log_error("socket is NULL!");
        return;
    }

    log_error("socket[%d] session timeout!", s->socket().native());
    del_socket(s);
}


bool user_manager_t::check_account(std::string guid)
{
	bool b_ret = redis_client_t::is_key_exist("account_ban", guid);
	if (b_ret)
	{
		log_error("account[%s] ban", guid.c_str());
	}

	return !b_ret;
}

bool user_manager_t::check_ip(std::string ip_address)
{
	bool b_ret = redis_client_t::is_key_exist("ip_access", ip_address);
	if (!b_ret)
	{
		log_error("ip_address[%s] access failed", ip_address.c_str());
	}
	
	return b_ret;
}

void user_manager_t::handle_cl_login_request(const network::tcp_socket_ptr& p_client_socket, uint64_t uid, const std::string& guid)
{
	if (!check_account(guid))
	{
		log_error("account check failed!");
		proto::client::lc_login_reply llr;
		llr.set_reply_code(errcode_enum::accounts_error_abnormal);
		env::server->send_msg_to_client(p_client_socket, 0, op_cmd::lc_login_reply, llr);
		return;
	}
	if (env::xml->ip_access > 0)
	{
		if (!check_ip(p_client_socket->socket().remote_endpoint().address().to_string()))
		{
			log_error("ip_access check failed!");
			proto::client::lc_login_reply llr;
			llr.set_reply_code(errcode_enum::server_error_shutdown);
			env::server->send_msg_to_client(p_client_socket, 0, op_cmd::lc_login_reply, llr);
			return;
		}
	}
	user_ptr p_user = add_user(p_client_socket, uid, guid);
	if (NULL == p_user)
	{
        log_error("add new user error!");
		proto::client::lc_login_reply llr;
		llr.set_reply_code(1);
		env::server->send_msg_to_client(p_client_socket, 0, op_cmd::lc_login_reply, llr);
	}
	else
	{
        uint32_t all_online_users_num = 0;
        std::string gate_user_num_str = "";
        boost::tie(all_online_users_num, gate_user_num_str) = gate_manager_t::get_all_online_users_num();
		if (all_online_users_num >= env::xml->user_num_max)
		{
            if (env::xml->login_queue_timer > 0)
            {
                add_login_queue(p_user);
            }
		}
		else
		{
            proto::server::la_login_request llr;
			llr.set_guid(p_user->get_guid());
			env::server->send_msg_to_gate(op_cmd::la_login_request, p_user->get_uid(), llr);
		}
	}
}

user_ptr user_manager_t::get_user(uint64_t uid)
{
    user_map::iterator itr = m_user_list.find(uid);
    if (itr != m_user_list.end())
    {
        user_ptr p_user = itr->second;
        if (NULL != p_user)
        {
            return itr->second;
        }
    }
    return user_ptr();
}

user_ptr user_manager_t::add_user(const tcp_socket_ptr& p_client_socket, uint64_t uid, std::string guid)
{
    if (NULL == p_client_socket)
    {
        log_error("p_client_socket is NULL!");
        return NULL;
    }

    // check gate
    // 必须把user分配到他以前登陆连接过的gate上
    gate_ptr p_gate = gate_manager_t::get_gate_by_user_uid(uid);
    if (NULL == p_gate)
    {
        // get gate
        p_gate = gate_manager_t::get_best_gate();
        if (NULL == p_gate)
        {
            log_error("NULL == p_gate, can not find the best gate!");
            return NULL;
        }
        gate_manager_t::add_user_to_gate(p_gate, uid);
    }

    // add user
    user_ptr p_user = get_user(uid); 
    if (NULL == p_user)
    {
        p_user.reset(new user_t(uid, guid));
        m_user_list[uid] = p_user;
        socket_data_t* p_socket_data = (socket_data_t*)p_client_socket->get_bind_data();
        if (NULL != p_socket_data)
        {
            p_socket_data->p_user = p_user;
            p_user->reset_client_socket(p_client_socket);
        }
        ROLE_LOG("add new user[%lu] in the gate[%d]", p_user->get_uid(), p_gate->get_id());
    }
    else
    {
        tcp_socket_ptr old_socket = p_user->reset_client_socket(p_client_socket);
        if (NULL != old_socket)
        {
            void* data = old_socket->get_bind_data();
            if (NULL != data)
            {
                p_client_socket->set_bind_data(data);
                old_socket->set_bind_data(NULL);
                env::server->post_network_close_socket(old_socket);
            }
        }
        ROLE_LOG("user[%lu] was not new", p_user->get_uid());
    }

    return p_user;
}

bool user_manager_t::del_user(uint64_t uid)
{
    user_map::iterator itr = m_user_list.find(uid);
    if (itr != m_user_list.end())
    {
        user_ptr p_user = itr->second;
        if (NULL != p_user)
        {
            del_login_queue(p_user);
            m_user_list.erase(itr);
            ROLE_LOG("del user[%lu] socket[%d]", uid, p_user->get_client_socket()->socket().native());
            return true;
        }
    }
    return false;
}

void user_manager_t::clear_user()
{
    m_user_list.clear();
	m_login_queue.clear();
}


void user_manager_t::clear_client()
{
    clear_socket();
    clear_user();
    log_info("clear all clients success!");
}

void user_manager_t::add_login_queue(user_ptr p_user)
{
	if (m_login_queue.empty())
	{
		start_login_queue_timer();
	}
	user_queue::iterator it = find(m_login_queue.begin(), m_login_queue.end(), p_user);
	if (it == m_login_queue.end())
	{
		m_login_queue.push_back(p_user);
	}
}

void user_manager_t::del_login_queue(user_ptr p_user)
{
	user_queue::iterator it = find(m_login_queue.begin(), m_login_queue.end(), p_user);
	if (it != m_login_queue.end())
	{
		m_login_queue.erase(it);
		if (m_login_queue.empty())
		{
			cancel_login_queue_timer();
		}
	}
}

void user_manager_t::start_login_queue_timer()
{
	uint32_t timeout = env::xml->login_queue_timer;
    if (0 == timeout)
    {
		log_warn("login_queue_timer is 0 !");
        return;
    }
    m_login_queue_timer.reset(new boost::asio::deadline_timer(env::server->get_ios()));
	m_login_queue_timer->expires_from_now(boost::posix_time::seconds(timeout));
	m_login_queue_timer->async_wait(boost::bind(&user_manager_t::login_queue_callback, boost::asio::placeholders::error));
}

void user_manager_t::cancel_login_queue_timer()
{
	m_login_queue_timer->cancel();
}

void user_manager_t::login_queue_callback(const boost::system::error_code& ec)
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

    uint32_t all_online_users_num = 0;
    std::string gate_user_num_str = "";
    boost::tie(all_online_users_num, gate_user_num_str) = gate_manager_t::get_all_online_users_num();
	uint32_t login_num = env::xml->user_num_max - all_online_users_num;
	if (login_num > env::xml->login_queue_process)
	{
		login_num = env::xml->login_queue_process;
	}
	for (uint32_t i = 0; i < login_num; ++i)
	{
		if (m_login_queue.empty())
		{
			cancel_login_queue_timer();
			return;
		}
		user_ptr p_user = m_login_queue[0];
		m_login_queue.erase(m_login_queue.begin());
		proto::server::la_login_request llr;
		llr.set_guid(p_user->get_guid());
		env::server->send_msg_to_gate(op_cmd::la_login_request, p_user->get_uid(), llr);
	}
	for (uint32_t i = 0; i < m_login_queue.size(); ++i)
	{
		user_ptr p_user = m_login_queue[i];
		if (NULL == p_user)
		{
			continue;
		}
		proto::client::lc_login_queue_notify noti;
		noti.set_pos(i + 1);
		p_user->send_msg_to_client(op_cmd::lc_login_queue_notify, noti);
	}
	start_login_queue_timer();
}