#ifndef __CENTER_GAME_MANAGER_HPP__
#define __CENTER_GAME_MANAGER_HPP__

#include "common/common_fwd.hpp"
#include "network/tcp_socket.hpp"
#include <vector>

class game_t
{
public:
    explicit game_t(const network::tcp_socket_ptr& s) : m_socket(s) {}

    network::tcp_socket_ptr get_socket() const { return m_socket.lock(); }

    const uint32_t get_id() const { return m_id; }
    void set_id(const uint32_t id) { m_id = id; }

private:
    network::tcp_socket_wptr m_socket;
    uint32_t m_id = 0;
};
typedef boost::shared_ptr<game_t> game_ptr;

class game_manager_t
{
public:
    static bool add_game(const network::tcp_socket_ptr& s);
    static void set_game(const network::tcp_socket_ptr& s, uint32_t game_id);
    static void del_game(const network::tcp_socket_ptr& s);
    static void clear_game();

    static uint32_t get_game_size() { return m_game_list.size(); }
    static uint32_t get_game_id_by_socket(const network::tcp_socket_ptr& s);
    static const network::tcp_socket_ptr get_game_socket_by_id(uint32_t game_id);
    static const std::vector<game_ptr>& get_game_list() { return m_game_list; }

private:
    static std::vector<game_ptr> m_game_list;
};

#endif
