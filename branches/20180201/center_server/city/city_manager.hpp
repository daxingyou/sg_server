#ifndef __CENTER_CITY_MANAGER_HPP__
#define __CENTER_CITY_MANAGER_HPP__

/*
author: wys
time:2016.11.14
desc:center city manager
*/

#include "city.hpp"
#include "protos_fwd.hpp"
#include <map>
#include "tcp_socket.hpp"

class city_manager_t
{
public:
	city_manager_t();
	~city_manager_t();

	static bool			init();
	static city_ptr		get_city(uint32_t city_id);

	static bool			add_city_prosperity_value(uint32_t city_id, uint32_t add_prosperity_value);
	static void			peek_city_list(proto::server::eg_get_city_list_reply& msg);

	static bool			handle_ge_get_city_list_notify(const network::tcp_socket_ptr& s);
public:
	static bool	load_from_config();
	static void	syn_to_game_server(city_ptr p_city);
	// -----------------redis-------------------
	static bool	load_all_from_db();


private:
	typedef std::map<uint32_t, city_ptr> city_map_type;
	static city_map_type m_city_map;
};

#endif