#ifndef __GAME_GATE_MANAGER_HPP__
#define __GAME_GATE_MANAGER_HPP__

#include <boost/shared_ptr.hpp>
#include "tcp_socket.hpp"
#include "log.hpp"

class gate_t
{
public:
    gate_t() {}
    ~gate_t() {}

    uint32_t get_id() const { return m_id; }
    void set_id(uint32_t val) { m_id = val; }

	uint32_t get_server_id() const { return m_server_id; }
	void set_server_id(uint32_t val) { m_server_id = val; }

    network::tcp_socket_ptr get_socket() const { return m_socket.lock(); }
    void set_socket(network::tcp_socket_wptr val) { m_socket = val; }

private:
    uint32_t m_id = 0;
	uint32_t m_server_id = 0;
    network::tcp_socket_wptr m_socket;
};
typedef boost::shared_ptr<gate_t> gate_ptr;
typedef std::vector<gate_ptr> gate_vec;

class gate_manager_t
{
public:
    static void add_gate(const network::tcp_socket_ptr& s);
    static void set_gate(const network::tcp_socket_ptr& s, uint32_t gate_id, uint32_t server_id);
    static void del_gate(const network::tcp_socket_ptr& s);
    static void clear_gate();

    static gate_ptr get_gate_by_socket(const network::tcp_socket_ptr& s);
    static gate_ptr get_gate_by_id(uint32_t gate_id, uint32_t server_id);
    static uint32_t get_gate_id_by_socket(const network::tcp_socket_ptr& s);
    static const network::tcp_socket_ptr get_gate_socket_by_id(uint32_t gate_id, uint32_t server_id);

	static uint32_t get_server_id_by_gate_socket(const network::tcp_socket_ptr& s);

    static const gate_vec& get_gate_list() { return m_gate_list; }

private:
    static gate_vec m_gate_list;
};


#endif