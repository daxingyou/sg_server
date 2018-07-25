#ifndef __GM_ADMIN_MANAGER_HPP__
#define __GM_ADMIN_MANAGER_HPP__

#include "common/common_fwd.hpp"
#include "network/tcp_socket.hpp"
#include "log.hpp"
#include "admin.hpp"
#include <map>

typedef std::map<uint32_t, admin_ptr> admin_map_type;

class admin_manager_t
{
public:
    static bool is_admin_login(const network::tcp_socket_ptr& s, uint32_t &admin_id);
    static bool is_admin_login(const uint32_t admin_id);

public:
    static admin_ptr get_admin(const network::tcp_socket_ptr& s);
    static admin_ptr get_admin(uint32_t admin_id);

    static bool add_admin(uint32_t admin_id, std::string admin_name, const network::tcp_socket_ptr& s);

    static bool del_admin(const network::tcp_socket_ptr& s);
    static bool del_admin(uint32_t admin_id);
    
    static bool clear_admin();

	static uint32_t gen_admin_id() { return s_admin_id++; }
private:
    static admin_map_type m_admin_map;//保存已经登录的admin

	static uint32_t s_admin_id;
};

#endif
