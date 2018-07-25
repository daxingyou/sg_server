#pragma once

#include "protos_fwd.hpp"

typedef std::map<uint32_t, proto::common::hero_strategy_comment_info> all_hero_strategy_comment_map;
typedef std::map<uint32_t, proto::common::hero_strategy_info> all_hero_strategy_map;


class hero_strategy_manager_t
{
public:
	static bool init();
	static void update_hero_strategy_data();
	static void get_hero_strategy_data_to_chat(uint32_t server_id);

private:
	static all_hero_strategy_map m_all_hero_strategy_map;

};
