#pragma once
#include <boost/shared_ptr.hpp>
#include "tcp_socket.hpp"
#include "log.hpp"

class transfer_t
{
public:
	transfer_t() {}
    ~transfer_t() {}

    uint32_t get_id() const { return m_id; }
    void set_id(uint32_t val) { m_id = val; }

    network::tcp_socket_ptr get_socket() const { return m_socket.lock(); }
    void set_socket(network::tcp_socket_wptr val) { m_socket = val; }

private:
    uint32_t m_id = 0;
    network::tcp_socket_wptr m_socket;
};
typedef boost::shared_ptr<transfer_t> transfer_ptr;
typedef std::vector<transfer_ptr> transfer_vec;

class transfer_manager_t
{
public:
    static void add_transfer(const network::tcp_socket_ptr& s);
    static void set_transfer(const network::tcp_socket_ptr& s, uint32_t gate_id);
    static void del_transfer(const network::tcp_socket_ptr& s);
    static void clear_transfer();

    static transfer_ptr get_transfer_by_socket(const network::tcp_socket_ptr& s);
    static transfer_ptr get_transfer_by_id(uint32_t gate_id);
    static uint32_t get_transfer_id_by_socket(const network::tcp_socket_ptr& s);
    static const network::tcp_socket_ptr get_transfer_socket_by_id(uint32_t gate_id);

    static const transfer_vec& get_transfer_list() { return m_transfer_list; }

private:
    static transfer_vec m_transfer_list;
};