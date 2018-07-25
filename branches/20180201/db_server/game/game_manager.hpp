#ifndef __DB_GAME_MANAGER_HPP__
#define __DB_GAME_MANAGER_HPP__

#include "common/common_fwd.hpp"
#include "common/log.hpp"
#include "network/tcp_socket.hpp"
#include <vector>
#include <boost/tuple/tuple.hpp>

class game_t
{
public:
    explicit game_t(const network::tcp_socket_ptr& s) : m_socket(s) { m_role_list.clear();  }

    const network::tcp_socket_ptr get_socket() const { return m_socket.lock(); }

    const uint32_t get_id() const { return m_id; }
    void set_id(const uint32_t id) { m_id = id; }

    bool find_role(uint64_t uid)
    {
        std::vector<uint64_t>::iterator result = find(m_role_list.begin(), m_role_list.end(), uid);
        if (result != m_role_list.end())
        {
            return true;
        }
        return false;
    }

    void add_role(uint64_t uid)
    {
        if (find_role(uid))
        {
            return;
        }
        m_role_list.push_back(uid);
        //log_warn("add user[%lu] to the game[%d] list, has %d users in the game[%d]", uid, m_id, m_role_list.size(), m_id);
    }
    
    void del_role(uint64_t uid)
    {
        std::vector<uint64_t>::iterator result = find(m_role_list.begin(), m_role_list.end(), uid);
        if (result == m_role_list.end())
        {
            return;
        }
        m_role_list.erase(result);
        //log_warn("delete user[%lu] from the game[%d] list, left %d users in the game[%d]", uid, m_id, m_role_list.size(), m_id);
    }

    uint32_t get_role_size()
    {
        return (uint32_t)m_role_list.size();
    }

private:
    network::tcp_socket_wptr m_socket;
    uint32_t m_id = 0;
    std::vector<uint64_t> m_role_list;
};
typedef boost::shared_ptr<game_t> game_ptr;

class game_manager_t
{
public:
    static void add_game(const network::tcp_socket_ptr& s);
    static void set_game(const network::tcp_socket_ptr& s, uint32_t game_id);
    static void del_game(const network::tcp_socket_ptr& s);
    static void clear_game();

    static uint32_t get_game_size();

    static void add_role_to_game(const network::tcp_socket_ptr& s, uint64_t uid);
    static void del_role_from_game(const network::tcp_socket_ptr& s, uint64_t uid);

    static const std::vector<game_ptr>& get_game_list() { return m_game_list; }

    static uint32_t get_game_id_by_socket(const network::tcp_socket_ptr& s);
    static const network::tcp_socket_ptr get_game_socket_by_id(uint32_t game_id);
    static boost::tuple<uint32_t, const network::tcp_socket_ptr> get_game_data_by_uid(uint64_t uid);

private:
    static std::vector<game_ptr> m_game_list;
};

#endif
