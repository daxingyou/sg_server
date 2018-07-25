#ifndef __DB_ACCOUNT_MANAGER_HPP__
#define __DB_ACCOUNT_MANAGER_HPP__

#include <boost/shared_ptr.hpp>
#include "tcp_socket.hpp"
#include "log.hpp"
#include "main/db_server_fwd.hpp"

class account_t
{
public:
    account_t(const network::tcp_socket_ptr& s)
        : m_socket(s)
    {
    }
    ~account_t() {}

    void reset();

    uint32_t get_id() const { return m_id; }
    void set_id(uint32_t val) { m_id = val; }

    network::tcp_socket_ptr get_socket() { return m_socket.lock(); }

private:
    network::tcp_socket_wptr    m_socket;
    uint32_t                    m_id = 0;
};
typedef boost::shared_ptr<account_t> account_ptr;
typedef std::vector<account_ptr> account_vec;

class account_manager_t
{
public:
    static void add_account(const network::tcp_socket_ptr& s);
    static void set_account(const network::tcp_socket_ptr& s, uint32_t account_id);
    static void del_account(const network::tcp_socket_ptr& s);
    static void clear_account();

    static account_ptr get_account_by_socket(const network::tcp_socket_ptr& s);
    static account_ptr get_account_by_account_id(uint32_t account_id);
    static account_ptr get_account_by_rand();
    static uint32_t get_account_id_by_socket(const network::tcp_socket_ptr& s);
    static const network::tcp_socket_ptr get_account_socket_by_id(uint32_t account_id);

    static uint32_t get_account_size() { return (uint32_t)m_account_list.size(); }
    static const account_vec& get_account_list() { return m_account_list; }

public:
    static account_vec m_account_list;
};

#endif
