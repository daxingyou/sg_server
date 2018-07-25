#ifndef __CENTER_GATE_MANAGER_HPP__
#define __CENTER_GATE_MANAGER_HPP__

#include "common/common_fwd.hpp"
#include "network/tcp_socket.hpp"
#include <vector>

class gate_t
{
public:
    explicit gate_t(const network::tcp_socket_ptr& s) : m_socket(s) {}

    network::tcp_socket_ptr get_socket() const { return m_socket.lock(); }

    const uint32_t get_id() const { return m_id; }
    void set_id(const uint32_t id) { m_id = id; }

private:
    network::tcp_socket_wptr m_socket;
    uint32_t m_id = 0;
};
typedef boost::shared_ptr<gate_t> gate_ptr;

class gate_manager_t
{
public:
    static void add_gate(const network::tcp_socket_ptr& s);
    static void set_gate(const network::tcp_socket_ptr& s, uint32_t gate_id);
    static void del_gate(const network::tcp_socket_ptr& s);
    static void clear_gate();

    static uint32_t get_gate_size() { return m_gate_list.size(); }
    static uint32_t get_gate_id_by_socket(const network::tcp_socket_ptr& s);
    static const network::tcp_socket_ptr get_gate_socket_by_id(uint32_t gate_id);
    static const std::vector<gate_ptr>& get_gate_list() { return m_gate_list; }

private:
    static std::vector<gate_ptr> m_gate_list;
};

#endif
