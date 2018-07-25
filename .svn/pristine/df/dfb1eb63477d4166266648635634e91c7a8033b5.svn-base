#ifndef __GATE_USER_MANAGER_HPP__
#define __GATE_USER_MANAGER_HPP__

#include "user.hpp"
#include "common_struct.hpp"
#include <boost/shared_ptr.hpp>
#include <boost/asio/deadline_timer.hpp>
#include "network/list.hpp"

typedef std::map<uint64_t, user_ptr> user_map;

class user_manager_t
{
public:
    template<typename T_MSG>
    static void send_to_all_users(uint16_t cmd, const T_MSG& msg)
    {
        if (m_user_list.empty())
        {
            log_warn("send cmd[%s:%d] all users error: m_user_list is empty", CMD_DESC(cmd), cmd);
            return;
        }
        user_map::iterator itr = m_user_list.begin();
        user_map::iterator itr_end = m_user_list.end();
        for (; itr != itr_end; ++itr)
        {
            if (NULL != itr->second && itr->second->is_play_state())
            {
				itr->second->send_msg_to_client(cmd, msg);
            }
        }
    }

public:
    static bool find_socket(const network::tcp_socket_ptr& s);
    static void add_socket(const network::tcp_socket_ptr& s);
    static bool socket_bind_user(const network::tcp_socket_ptr& s, const user_ptr& p_user);
    static bool del_socket(const network::tcp_socket_ptr& s, bool is_del_user);
    static void socket_session_timeout_callback(const boost::system::error_code& ec, const network::tcp_socket_ptr& s);

    static user_ptr get_user(uint64_t uid);
    static user_ptr get_user(const network::tcp_socket_ptr& s);
	static bool add_user(const user_ptr& p_new_user);

	// 清理所有用户和连接（两个都单独做一次清理）
	// 有socket不一定有user对象
	// 有user对象也不一定有socket
    static void clear_all_users_and_socket(bool is_notify_login, uint32_t reason);

	// 删除一个一个user，此时需通知login知道，不然每次都会分配到这个gate
	static void del_user(uint64_t uid, bool is_notify_login = true);

	static void kick_user(const network::tcp_socket_ptr& s, user_ptr p_user, common::KICK_ROLE_REASON kick_reason);

	static void start_sync_user_num_to_login_timer();
    static void handle_sync_user_num_to_login(const boost::system::error_code& ec);
    static void cancel_sync_user_num_to_login();

	static void pingpong_timeout_callback(const boost::system::error_code& ec, uint64_t uid);
	static void reconnect_timeout_callback(const boost::system::error_code& ec, uint64_t uid);
    
public:
	// 通知login下线
    static void notify_login_by_kick_user(uint64_t uid);
	// 通知game|center状态改变
	static void notify_others_unreg_user(const user_ptr& p_user, common::UNREGUSER_RET_TYPE type);

public:
    static network::list_t<network::tcp_socket_wptr>::type m_client_socket_list;
    static user_map m_user_list;
	static boost::shared_ptr<boost::asio::deadline_timer> m_notify_timer;
};

#endif
