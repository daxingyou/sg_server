#ifndef __CHAT_TRANSFER_MANAGER_HPP__
#define __CHAT_TRANSFER_MANAGER_HPP__

#include <boost/shared_ptr.hpp>
#include "tcp_socket.hpp"
#include "log.hpp"
#include "main/chat_server_fwd.hpp"

class transfer_t
{
public:
    transfer_t(const network::tcp_socket_ptr& s)
        : m_socket(s)
    {
    }
    ~transfer_t() {}

    void reset();

    uint32_t get_id() const { return m_id; }
    void set_id(uint32_t val) { m_id = val; }

    network::tcp_socket_ptr get_socket() const { return m_socket.lock(); }

private:
    network::tcp_socket_wptr    m_socket;
    uint32_t                    m_id = 0;
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
    static transfer_ptr get_transfer_by_rand();
    static uint32_t get_transfer_id_by_socket(const network::tcp_socket_ptr& s);
    static const network::tcp_socket_ptr get_transfer_socket_by_id(uint32_t gate_id);

    static uint32_t get_transfer_size() { return (uint32_t)m_transfer_list.size(); }
    static const transfer_vec& get_transfer_list() { return m_transfer_list; }

private:
    static transfer_vec m_transfer_list;
};

#endif