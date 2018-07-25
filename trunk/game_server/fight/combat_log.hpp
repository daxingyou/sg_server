#pragma once
#include "fight/combat.hpp"

class combat_log_t
{
public:
	combat_log_t();
	~combat_log_t();

	static void write_combat_log(const process_map& action, const proto::common::fight_data& data, uint64_t fight_uid);
	static void read_combat_log(uint64_t fight_uid);
private:
};
