#ifndef __WORLD_CUP_MGR_HPP__
#define __WORLD_CUP_MGR_HPP__

#include <map>
#include "server.pb.h"
#include "macros.hpp"
#include <boost/shared_ptr.hpp>
#include "world_cup_battle.hpp"
typedef std::map<uint64_t, world_cup_battle_ptr> world_cup_battle_map;

class world_cup_manager_t
{
public:
	static void load_data(const proto::common::world_cup_data& data);
	static void peek_data(proto::common::world_cup_data* p_data, const proto::server::re_world_cup_bet_data_reply& reply);

	static void update_battle_info(const proto::common::world_cup_battle& battle_info);
private:
	static world_cup_battle_map battle_map;
};
#endif