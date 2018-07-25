#include "game_manager.hpp"
#include "main/gate_server.hpp"
#include "main/gate_server_fwd.hpp"
#include "user/user_manager.hpp"

USING_NS_NETWORK;
USING_NS_COMMON;

void game_t::reset()
{
    m_id = 0;

	if (!m_user_list.empty())
	{
		std::set<uint64_t> temp(m_user_list);

		for (std::set<uint64_t>::const_iterator citr = temp.begin();
			citr != temp.end(); ++citr)
		{
			const user_ptr& p_user = user_manager_t::get_user(*citr);
			if (NULL != p_user)
			{
				p_user->set_kick_reason(common::KICK_ROLE_REASON_GAME_SERVER_CLOSED);

				if (p_user->get_client_socket())
				{
					user_manager_t::del_socket(p_user->get_client_socket(), true);
				}
				else
				{
					user_manager_t::del_user(*citr);
				}
			}
		}

		m_user_list.clear();
	}

    env::server->post_network_close_socket(get_socket());
}

game_vec game_manager_t::m_game_list;

void game_manager_t::add_game(const tcp_socket_ptr& s)
{
    if (NULL == s)
    {
        log_error("NULL == s");
        return;
    }

    game_ptr p_game = get_game_by_socket(s);
    if (NULL == p_game)
    {
        p_game.reset(new game_t(s));
        m_game_list.push_back(p_game);
        return;
    }
    else
    {
		try
		{
            log_error("game socket[%s: %d] has already exit!", s->socket().remote_endpoint().address().to_string().c_str(), s->socket().native());
        }
        catch (boost::system::system_error &ec)
        {
            log_error("throw exception: %s", ec.what());
        }
    }
}

void game_manager_t::set_game(const tcp_socket_ptr& s, uint32_t game_id, uint32_t game_type)
{
    for (size_t idx = 0; idx < m_game_list.size(); ++idx)
    {
        game_ptr p_game = m_game_list.at(idx);
        if (NULL != p_game)
        {
            if (game_id == p_game->get_id() && game_type == p_game->get_type())
            {
                del_game(s);
                log_error("the game[id=%d type=%d] already exists in the game list!", game_id, game_type);
                return;
            }

            if (NULL != s && p_game->get_socket() == s)
            {
                p_game->set_id(game_id);
                p_game->set_type(game_type);
                if (game_server_type_normal == game_type)
                {
                    log_info("add game[%d], has %d games", game_id, (uint32_t)get_game_size());
                }
                else if (game_server_type_cross == game_type)
                {
                    log_info("add cross[%d], has %d crosses", game_id, (uint32_t)get_cross_size());
                }
                else
                {
                    log_error("game type[%d] error!", game_type);
                    return;
                }

                return;
            }
        }
    }
}

void game_manager_t::del_game(const tcp_socket_ptr& s)
{
    if (NULL == s)
    {
        log_error("NULL == s");
        return;
    }

    game_vec::iterator itr = m_game_list.begin();
    game_vec::iterator itr_end = m_game_list.end();
    for (; itr != itr_end; ++itr)
    {
        game_ptr p_game = *itr;
        if (NULL != p_game && (NULL != s && p_game->get_socket() == s))
        {
            uint32_t game_type = p_game->get_type();
            uint32_t game_id = p_game->get_id();
			p_game->reset();
            m_game_list.erase(itr);
            if (game_server_type_normal == game_type)
            {
                log_info("del game[%d], left %d games", game_id, (uint32_t)get_game_size());
            }
            else if (game_server_type_cross == game_type)
            {
                log_info("del cross[%d], left %d crosses", game_id, (uint32_t)get_cross_size());
            }
            else
            {
                log_error("game type[%d] error!", game_type);
                return;
            }
            return;
        }
    }
}

void game_manager_t::clear_all_game()
{
    for (int32_t i = 0; i < (int32_t)m_game_list.size(); ++i)
    {
        game_ptr p_game = m_game_list[i];
        if (NULL != p_game)
        {
            p_game->reset();
        }
    }
    m_game_list.clear();

    log_info("clear all games and crosses success!");
}

//----------------------------------------------------------------------------------------
int32_t g_val = 1;
game_ptr game_manager_t::get_best_game()
{
	game_vec game_list;
	for (size_t i = 0; i < m_game_list.size(); ++i)
	{
		game_ptr p_game = m_game_list.at(i);
		if (NULL != p_game && p_game->get_type() == game_server_type_normal)
		{
			game_list.push_back(p_game);
		}
	}

    int32_t size = (int32_t)game_list.size();
    if (size <= 0)
    {
        log_error("game size[%d] <= 0", size);
        return game_ptr();
    }

    int32_t index = g_val % size - 1;
    if (index < 0)
    {
        index = size - 1;
    }
    g_val += 1;

    const game_ptr& p_game = game_list[index];
    if (NULL == p_game)
    {
        log_error("NULL == p_game, index: %d", index);
        return game_ptr();
    }
    return p_game;
}

game_ptr game_manager_t::get_game_by_socket(const tcp_socket_ptr& s)
{
    for (int32_t i = 0; i < (int32_t)m_game_list.size(); ++i)
    {
        game_ptr p_game = m_game_list[i];
        if (NULL != p_game && (NULL != s && p_game->get_socket() == s))
        {
            return p_game;
        }
    }
    return game_ptr();
}

game_ptr game_manager_t::get_game_by_game_id(uint32_t game_id, uint32_t game_type)
{
    for (int32_t i = 0; i < (int32_t)m_game_list.size(); ++i)
    {
        game_ptr p_game = m_game_list[i];
        if (NULL != p_game && (0 != game_id && p_game->get_id() == game_id && p_game->get_type() == game_type))
        {
            return p_game;
        }
    }
    return game_ptr();
}

game_ptr game_manager_t::get_game_by_user_uid(uint64_t uid, uint32_t game_type)
{
    for (size_t idx = 0; idx < m_game_list.size(); ++idx)
    {
        game_ptr p_game = m_game_list.at(idx);
        if (NULL != p_game)
        {
			if (p_game->get_type() != game_type)
			{
				continue;
			}

			if (p_game->find_user(uid))
			{
				return p_game;
			}
        }
    }
    return game_ptr();
}

uint32_t game_manager_t::get_game_id_by_socket(const tcp_socket_ptr& s)
{
    for (size_t idx = 0; idx < m_game_list.size(); ++idx)
    {
        game_ptr p_game = m_game_list.at(idx);
        if (NULL != p_game && (NULL != s && p_game->get_socket() == s))
        {
            return p_game->get_id();
        }
    }

    return 0;
}

uint32_t game_manager_t::get_game_type_by_socket(const network::tcp_socket_ptr& s)
{
	for (size_t idx = 0; idx < m_game_list.size(); ++idx)
	{
		game_ptr p_game = m_game_list.at(idx);
		if (NULL != p_game && (NULL != s && p_game->get_socket() == s))
		{
			return p_game->get_type();
		}
	}

	return 0;
}

const tcp_socket_ptr game_manager_t::get_game_socket_by_id(uint32_t game_id, uint32_t game_type)
{
    for (size_t idx = 0; idx < m_game_list.size(); ++idx)
    {
        game_ptr p_game = m_game_list.at(idx);
        if (NULL != p_game && (0 != game_id && p_game->get_id() == game_id && p_game->get_type() == game_type))
        {
			return p_game->get_socket();
        }
    }

    return NULL;
}

const tcp_socket_ptr game_manager_t::get_game_socket_by_user_uid(uint64_t uid, uint32_t game_type)
{
    const game_ptr& p_game = get_game_by_user_uid(uid, game_type);
    if (NULL == p_game)
    {
        log_error("NULL == p_game by uid[%lu] game_type[%d]", uid, game_type); // 还未登陆场景服就是找不到gameid
        return tcp_socket_ptr();
    }

	return p_game->get_socket();
}

uint32_t game_manager_t::get_game_size()
{
    uint32_t size = 0;
    for (size_t idx = 0; idx < m_game_list.size(); ++idx)
    {
        game_ptr p_game = m_game_list.at(idx);
        if (NULL == p_game)
        {
            continue;
        }

        if (p_game->get_type() == 0)
        {
            ++size;
        }
    }

    return size;
}

uint32_t game_manager_t::get_cross_size()
{
    uint32_t size = 0;
    for (size_t idx = 0; idx < m_game_list.size(); ++idx)
    {
        game_ptr p_game = m_game_list.at(idx);
        if (NULL == p_game)
        {
            continue;
        }

        if (p_game->get_type() == 1)
        {
            ++size;
        }
    }

    return size;
}
//---------------------------------------------------------------------------------------------
void game_manager_t::add_user_to_game(const network::tcp_socket_ptr& s, uint64_t uid)
{
	const game_ptr& p_game = get_game_by_socket(s);
    if (NULL == p_game)
    {
        log_error("NULL == p_game");
        return;
    }

    p_game->add_user(uid);
}

void game_manager_t::del_user_from_game(uint64_t uid)
{
    for (size_t idx = 0; idx < m_game_list.size(); ++idx)
    {
        const game_ptr& p_game = m_game_list.at(idx);
        if (NULL == p_game)
        {
            continue;
        }
        p_game->del_user(uid);
    }
}
