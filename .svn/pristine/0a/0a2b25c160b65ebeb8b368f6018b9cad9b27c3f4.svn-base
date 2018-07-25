#ifndef __GATE_GAME_MANAGER_HPP__
#define __GATE_GAME_MANAGER_HPP__

#include <boost/shared_ptr.hpp>
#include "tcp_socket.hpp"
#include "log.hpp"
#include "main/gate_server_fwd.hpp"

enum game_server_type
{
	game_server_type_normal = 0,	// 普通
	game_server_type_cross = 1,		// 跨服
};

class game_t
{
public:
    game_t(const network::tcp_socket_ptr& s)
        : m_socket(s)
    {
        m_user_list.clear();
    }

	// 这个不清理用户
    void reset();

    uint32_t get_id() const { return m_id; }
    void set_id(uint32_t val) { m_id = val; }

	uint32_t get_type() const { return m_type; }
	void set_type(uint32_t val) { m_type = val; }

    network::tcp_socket_ptr get_socket() { return m_socket.lock(); }

    bool find_user(uint64_t uid)
    {
        std::vector<uint64_t>::iterator result = find(m_user_list.begin(), m_user_list.end(), uid);
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
        m_user_list.push_back(uid);
        ROLE_LOG("add user[%lu] to the game[%d] list, has %d users in the game[%d]", uid, m_id, m_user_list.size(), m_id);
    }

    bool del_user(uint64_t uid)
    {
        std::vector<uint64_t>::iterator result = find(m_user_list.begin(), m_user_list.end(), uid);
        if (result == m_user_list.end())
        {
            return false;
        }
        m_user_list.erase(result);
        ROLE_LOG("delete user[%lu] from the game[%d] list, left %d users in the game[%d]", uid, m_id, m_user_list.size(), m_id);
        return true;
    }

    void clear_user()
    {
        m_user_list.clear();
        log_info("clear all users from the game[%d]", m_id);
    }

private:
    network::tcp_socket_wptr    m_socket;
    uint32_t                    m_id = 0;
	uint32_t					m_type = 0;
    std::vector<uint64_t>       m_user_list; // 包含在线和不在线的是所有该game上的users
};
typedef boost::shared_ptr<game_t> game_ptr;
typedef std::vector<game_ptr> game_vec;

class game_manager_t
{
public:
    static void add_game(const network::tcp_socket_ptr& s);
    static void set_game(const network::tcp_socket_ptr& s, uint32_t game_id, uint32_t game_type);
    static void del_game(const network::tcp_socket_ptr& s);
    static void clear_all_game();

    static uint32_t get_game_size();
    static uint32_t get_cross_size();

    static game_ptr get_best_game();
    static game_ptr get_game_by_socket(const network::tcp_socket_ptr& s);
    static game_ptr get_game_by_game_id(uint32_t game_id, uint32_t game_type);
    static game_ptr get_game_by_user_uid(uint64_t uid, uint32_t game_type);
    static uint32_t get_game_id_by_socket(const network::tcp_socket_ptr& s);
	static uint32_t get_game_type_by_socket(const network::tcp_socket_ptr& s);
    static const network::tcp_socket_ptr get_game_socket_by_id(uint32_t game_id, uint32_t game_type);
    static const network::tcp_socket_ptr get_game_socket_by_user_uid(uint64_t uid, uint32_t game_type = game_server_type_normal);

    static const game_vec& get_game_list() { return m_game_list; }

	//////////////////////////////////////////////////////////////////////////
	static void add_user_to_game(const network::tcp_socket_ptr& s, uint64_t uid);

	static void del_user_from_game(uint64_t uid);

public:
    static game_vec m_game_list;
};

#endif
