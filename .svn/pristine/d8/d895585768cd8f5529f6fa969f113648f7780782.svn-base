#pragma once

/*
author: wys
time:2016.12.21
desc:center item manager
*/

#include "item_limit.hpp"
#include "protos_fwd.hpp"
#include <map>
#include "tcp_socket.hpp"

class item_manager_t
{
public:
	item_manager_t();
	~item_manager_t();

	static bool	init();
	static bool close();

	static bool can_give_item(uint32_t item_tid, uint32_t item_num);
	static item_limit_ptr give_item(uint32_t item_tid, uint32_t item_num);

	static item_limit_ptr get_item_limit(uint32_t item_tid);
public:
	static bool	load_item_limit_from_config();
	// -----------------redis-------------------
	static bool	load_item_limit_from_db();
	static bool handle_ge_get_item_limit_list(const network::tcp_socket_ptr& s);

private:
	typedef std::map<uint32_t, item_limit_ptr> item_limit_map;
	static item_limit_map m_item_limit_map;
};