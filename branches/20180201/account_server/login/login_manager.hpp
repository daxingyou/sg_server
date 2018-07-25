#ifndef __ACCOUNT_LOGIN_MANAGER_HPP__
#define __ACCOUNT_LOGIN_MANAGER_HPP__

#include "login.hpp"
#include <map>
#include "network.hpp"

typedef std::map<uint32_t, login_ptr> login_map;

class login_manager_t
{
public:
    static login_ptr get_login(uint32_t server_id);
	static login_ptr get_login(const network::tcp_socket_ptr& s);
    static void add_login(uint32_t server_id, const network::tcp_socket_ptr& s, const std::string& login_ip, uint32_t login_port, const std::string& login_name, const uint32_t can_create_new_role );
	static void del_login(const network::tcp_socket_ptr& s);
    static void clear_login();

	static bool socket_bind_login(const network::tcp_socket_ptr& s, login_ptr p_login);
public:
    static login_map m_login_map;
};

#endif
