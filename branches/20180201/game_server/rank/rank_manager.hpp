#pragma once
#include "server.pb.h"
#include "main/game_server_fwd.hpp"

class rank_manager_t
{
public:
	rank_manager_t();
	~rank_manager_t();

	static void update_rank(uint32_t type, uint64_t uid, uint32_t key, const std::vector<std::string>& custom_data);

};