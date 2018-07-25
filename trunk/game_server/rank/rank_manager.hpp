#pragma once
#include "server.pb.h"
#include "main/game_server_fwd.hpp"

class rank_manager_t
{
public:
	static void update_rank(uint32_t type, uint64_t uid, uint32_t key, const std::vector<std::string>& custom_data);

	static void update_criminal(uint64_t uid, uint32_t pk_value);

	static void update_achievement(uint64_t uid, uint32_t value, uint32_t level);

	static void update_tower(uint64_t uid, uint32_t cur_tower, uint32_t cost_day, uint32_t level);

	static void update_recruit(uint64_t uid, uint32_t val);

	static void update_copper(uint64_t uid, uint32_t val);

	static void update_gold(uint64_t uid, uint32_t val);
};