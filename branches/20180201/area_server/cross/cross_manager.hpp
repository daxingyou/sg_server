#ifndef __CROSS_MANAGER_HPP__
#define __CROSS_MANAGER_HPP__

#include "common/common_fwd.hpp"
#include "network/tcp_socket.hpp"
#include <vector>

class cross_t
{
public:
    explicit cross_t(const network::tcp_socket_ptr& s) : m_socket(s) {}

    network::tcp_socket_ptr get_socket() const { return m_socket.lock(); }

    const uint32_t get_id() const { return m_id; }
    void set_id(const uint32_t id) { m_id = id; }

private:
    network::tcp_socket_wptr m_socket;
    uint32_t m_id = 0;
};
typedef boost::shared_ptr<cross_t> cross_ptr;
 
class cross_manager_t
{
public:
    static bool add_cross(const network::tcp_socket_ptr& s);
    static void set_cross(const network::tcp_socket_ptr& s, uint32_t cross_id);
    static void del_cross(const network::tcp_socket_ptr& s);
    static void clear_cross();

    static uint32_t get_cross_size() { return m_cross_list.size(); }
    static uint32_t get_cross_id_by_socket(const network::tcp_socket_ptr& s);
    static const network::tcp_socket_ptr get_cross_socket_by_id(uint32_t cross_id);
    static const std::vector<cross_ptr>& get_cross_list() { return m_cross_list; }

	static const network::tcp_socket_ptr get_avg_cross_socket();

private:
    static std::vector<cross_ptr> m_cross_list;
	static size_t m_cur_index;
};

#endif
