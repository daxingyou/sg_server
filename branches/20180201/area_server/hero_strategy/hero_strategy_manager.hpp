#pragma once

#include "protos_fwd.hpp"

typedef std::map<uint32_t, proto::common::hero_strategy_comment_info> all_hero_strategy_comment_map;
typedef std::map<uint32_t, proto::common::hero_strategy_info> all_hero_strategy_map;


// 武将攻略的武将统计信息
struct hero_strategy_hero_count
{
	static bool compare(const hero_strategy_hero_count& p1, const hero_strategy_hero_count& p2)
	{
		if (p1.hero_use > p2.hero_use)
		{
			return false;
		}
		return true;
	}

	uint32_t hero_id = 0;
	uint32_t hero_hot = 0;
	uint32_t hero_use = 0;
};

class hero_strategy_manager_t
{
public:
	static bool init();
	static void update_hero_strategy_data();
	static void get_hero_strategy_data_to_chat(uint32_t server_id);

private:
	static all_hero_strategy_map m_all_hero_strategy_map;

};
