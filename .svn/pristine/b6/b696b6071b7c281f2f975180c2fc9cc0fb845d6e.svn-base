#ifndef __LOGIN_GATE_MANAGER_HPP__
#define __LOGIN_GATE_MANAGER_HPP__

#include "common/common_fwd.hpp"
#include "common/log.hpp"
#include "network/tcp_socket.hpp"
#include <vector>
#include <set>
#include <boost/tuple/tuple.hpp>

class gate_t
{
public:
    gate_t(const network::tcp_socket_ptr& s)
        : m_socket(s)
    {
        m_user_list.clear();
    }

    uint32_t get_id() const { return m_id; }
    void set_id(uint32_t val) { m_id = val; }

    network::tcp_socket_ptr get_socket() { return m_socket.lock(); }

    uint32_t get_online_users_num() const { return m_online_user_num; }
    void set_online_users_num(uint32_t val) { m_online_user_num = val; }

    const std::string& get_ip() const { return m_ip; }
    void set_ip(std::string val) { m_ip = val; }

    uint32_t get_port() const { return m_port; }
    void set_port(uint32_t val) { m_port = val; }

    bool find_user(uint64_t uid)
    {
		std::set<uint64_t>::iterator result = m_user_list.find(uid);
        if (result != m_user_list.end())
        {
            return true;
        }
        return false;
    }

    void add_user(uint64_t uid)
    {
        if (find_user(uid))
        {
            return;
        }
        m_user_list.insert(uid);
        //log_warn("add user[%lu] to the gate[%d] list, has %d users in the gate[%d]", uid, m_id, (uint32_t)m_user_list.size(), m_id);
    }

    bool del_user(uint64_t uid)
    {
		std::set<uint64_t>::iterator result = m_user_list.find(uid);
        if (result == m_user_list.end())
        {
            return false;
        }
        m_user_list.erase(result);
        //log_warn("delete user[%lu] from the gate[%d] list, left %d users in the gate[%d]", uid, m_id, (uint32_t)m_user_list.size(), m_id);
        return true;
    }

    void clear_user()
    {
        m_user_list.clear();
        //log_warn("clear all users from the gate[%d]", m_id);
    }

private:
    network::tcp_socket_wptr  m_socket;
    uint32_t        m_id = 0;
    std::string     m_ip = "";
    uint32_t        m_port = 0;
    std::set<uint64_t> m_user_list;
    uint32_t        m_online_user_num = 0;
};
typedef boost::shared_ptr<gate_t> gate_ptr;
typedef std::vector<gate_ptr> gate_vec;

class gate_manager_t
{
public:
    static void add_gate(const network::tcp_socket_ptr& s);
    static void set_gate(const network::tcp_socket_ptr& s, uint32_t gate_id);
    static void set_gate(uint64_t uid, uint32_t gate_id);
    static void del_gate(const network::tcp_socket_ptr& s);
    static void clear_gate();

    static gate_ptr get_gate_by_socket(const network::tcp_socket_ptr& s);
    static gate_ptr get_gate_by_gate_id(uint32_t gate_id);
    static gate_ptr get_gate_by_user_uid(uint64_t uid);
    static gate_ptr get_best_gate();
    static uint32_t get_gate_id_by_socket(const network::tcp_socket_ptr& s);
    static const network::tcp_socket_ptr get_gate_socket_by_id(uint32_t gate_id);
    static boost::tuple <uint32_t, std::string> get_all_online_users_num();

    static void add_user_to_gate(const gate_ptr& p_gate, uint64_t uid);
    static void add_user_to_gate(uint32_t gate_id, uint64_t uid);
    static void del_user_from_gate(const network::tcp_socket_ptr& s, uint64_t uid);
	static void del_all_gate_user(uint32_t gate_id);

public:
    static gate_vec m_gate_list;
};

#endif
