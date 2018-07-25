#pragma once

#include <boost/shared_ptr.hpp>
#include "tcp_socket.hpp"
#include "log.hpp"

class center_t
{
public:
    center_t(const network::tcp_socket_ptr& s)
        : m_socket(s)
    {
    }

    void reset();

    uint32_t get_id() const { return m_id; }
    void set_id(uint32_t val) { m_id = val; }

    network::tcp_socket_ptr get_socket() { return m_socket.lock(); }

private:
    network::tcp_socket_wptr    m_socket;
    uint32_t                    m_id = 0;
};
typedef boost::shared_ptr<center_t> center_ptr;
typedef std::vector<center_ptr> center_vec;

class center_manager_t
{
public:
    static void add_center(const network::tcp_socket_ptr& s);
    static void set_center(const network::tcp_socket_ptr& s, uint32_t center_id);
    static void del_center(const network::tcp_socket_ptr& s);
	static void clear_center();

    static const center_vec& get_center_list() { return m_center_list; }

    static center_ptr get_center_by_socket(const network::tcp_socket_ptr& s);
    static center_ptr get_center_by_id(uint32_t center_id);
    static uint32_t get_center_id_by_socket(const network::tcp_socket_ptr& s);
    static const network::tcp_socket_ptr get_center_socket_by_id(uint32_t center_id);
	
public:
    static center_vec m_center_list;
};
