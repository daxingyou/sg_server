#include "game_manager.hpp"
#include "log.hpp"
#include "main/center_server.hpp"
#include "main/center_server_fwd.hpp"

USING_NS_NETWORK;
USING_NS_COMMON;

std::vector<game_ptr> game_manager_t::m_game_list;

bool game_manager_t::add_game(const network::tcp_socket_ptr& s)
{
    for (size_t idx = 0; idx < m_game_list.size(); ++idx)
    {
        game_ptr p_game = m_game_list.at(idx);
        if (NULL != p_game)
        {
            if (p_game->get_socket() == s && NULL != s)
            {
                try
                {
                    log_error("game socket[%s:%d] has already exist!", s->socket().remote_endpoint().address().to_string().c_str(), s->socket().native());
                }
                catch (boost::system::system_error &ec)
                {
                    log_error("throw exception:%s", ec.what());
                }

                return false;
            }
        }
    }

    game_ptr p_game(new game_t(s));
    if (NULL == p_game)
    {
		log_error("p_game null error");
		return false;
    }

	m_game_list.push_back(p_game);

	return true;
}

void game_manager_t::del_game(const network::tcp_socket_ptr& s)
{
    std::vector<game_ptr>::iterator itr = m_game_list.begin();
    for (; itr != m_game_list.end(); ++itr)
    {
        game_ptr p_game = *itr;
        if (NULL != p_game && s == p_game->get_socket())
        {
            log_info("del game[%d], left %d games", p_game->get_id(), m_game_list.size()-1);

            env::server->post_network_close_socket(p_game->get_socket());
            m_game_list.erase(itr);
            return;
        }
    }
}

void game_manager_t::clear_game()
{
    for (size_t i = 0; i < m_game_list.size(); ++i)
    {
        game_ptr p_game = m_game_list.at(i);
        if (NULL != p_game)
        {
            env::server->post_network_close_socket(p_game->get_socket());
        }
    }
    m_game_list.clear();
}

void game_manager_t::set_game(const network::tcp_socket_ptr& s, uint32_t game_id)
{
    for (size_t idx = 0; idx < m_game_list.size(); ++idx)
    {
        game_ptr p_game = m_game_list.at(idx);
        if (NULL != p_game)
        {
            if (game_id == p_game->get_id())
            {
                del_game(s);
                log_error("the game[id=%d] already exists in the game list!", game_id);
                return;
            }

            if (NULL != s && p_game->get_socket() == s)
            {
                p_game->set_id(game_id);
                log_info("add game[%d], has %d games", game_id, m_game_list.size());
                return;
            }
        }
    }
}

uint32_t game_manager_t::get_game_id_by_socket(const network::tcp_socket_ptr& s)
{
    for (size_t idx = 0; idx < m_game_list.size(); ++idx)
    {
        game_ptr p_game = m_game_list.at(idx);
        if (NULL != p_game)
        {
            if (p_game->get_socket() == s)
            {
                return p_game->get_id();
            }
        }
    }

    return 0;
}

const network::tcp_socket_ptr game_manager_t::get_game_socket_by_id(uint32_t game_id)
{
    for (size_t idx = 0; idx < m_game_list.size(); ++idx)
    {
        game_ptr p_game = m_game_list.at(idx);
        if (NULL != p_game)
        {
            if (p_game->get_id() == game_id)
            {
                return p_game->get_socket();
            }
        }
    }

    return nullptr;
}