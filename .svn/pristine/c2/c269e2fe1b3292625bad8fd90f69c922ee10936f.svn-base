#ifndef __LOGIN_USER_MANAGER_HPP__
#define __LOGIN_USER_MANAGER_HPP__

#include "user.hpp"
#include "protocpp/protos_fwd.hpp"
#include <vector>

typedef std::map<uint64_t, user_ptr> user_map;
typedef std::map<uint64_t, uint32_t> user_gate_map;
typedef std::vector<user_ptr> user_queue;

class user_manager_t
{
public:
    static bool find_socket(const network::tcp_socket_ptr& s);
    static void add_socket(const network::tcp_socket_ptr& s);
    static void del_socket(const network::tcp_socket_ptr& s);
    static void clear_socket();
    static void socket_session_timeout_callback(const boost::system::error_code& ec, const network::tcp_socket_ptr& s);

	static bool check_account(std::string guid);
	static bool check_ip(std::string ip_address);
    static void handle_cl_login_request(const network::tcp_socket_ptr& p_client_socket, uint64_t uid, const std::string&);

    static user_ptr add_user(const network::tcp_socket_ptr& p_client_socket, uint64_t uid, std::string guid);
    static user_ptr get_user(uint64_t uid);
    static bool del_user(uint64_t uid);
    static void clear_user();

    static void clear_client();

	static void add_login_queue(user_ptr p_user);
	static void del_login_queue(user_ptr p_user);
	static void start_login_queue_timer();
	static void cancel_login_queue_timer();
	static void login_queue_callback(const boost::system::error_code& ec);
public:
    static list_t<network::tcp_socket_wptr>::type m_socket_list;
    static user_map m_user_list;
	static user_queue m_login_queue;
	static boost::shared_ptr<boost::asio::deadline_timer> m_login_queue_timer;
};

#endif
