#include "world_cup_manager.hpp"
#include "main/center_server.hpp"
#include "main/center_server_fwd.hpp"
#include "protos_fwd.hpp"
#include "config/config_manager.hpp"
#include "tblh/errcode_enum.hpp"
USING_NS_NETWORK;
USING_NS_COMMON;

world_cup_battle_map world_cup_manager_t::battle_map;


void world_cup_manager_t::load_data(const proto::common::world_cup_data& data)
{
	battle_map.clear();
	for (int32_t i = 0; i < data.battle_list_size(); ++i)
	{
		const proto::common::world_cup_battle& wcb = data.battle_list(i);
		uint64_t battle_uid = string_util_t::string_to_uint64(wcb.uid());
		world_cup_battle_ptr p_battle;
		world_cup_battle_map::iterator it = battle_map.find(battle_uid);
		if (it == battle_map.end())
		{
			p_battle.reset(new world_cup_battle_t);
			battle_map.insert(std::make_pair(battle_uid, p_battle));
		}
		else
		{
			p_battle = it->second;
		}
		if (NULL != p_battle)
		{
			p_battle->load_data(wcb);
		}
	}
}

void world_cup_manager_t::peek_data(proto::common::world_cup_data* p_data, const proto::server::re_world_cup_bet_data_reply& reply)
{
	if (NULL == p_data)
	{
		log_error("NULL == p_data");
		return;
	}
	for (int32_t i = 0; i < reply.bet_data_size(); ++i)
	{
		const proto::common::world_cup_bet& wcb = reply.bet_data(i);
		uint64_t battle_uid = string_util_t::string_to_uint64(wcb.battle_uid());
		auto it = battle_map.find(battle_uid);
		if (it == battle_map.end())
		{
			continue;
		}
		world_cup_battle_ptr p_battle = it->second;
		if (NULL == p_battle)
		{
			continue;
		}
		proto::common::world_cup_battle* p_single = p_data->add_battle_list();
		p_battle->peek_data(p_single);
		p_single->mutable_self_bet()->CopyFrom(wcb);
	}
}

void world_cup_manager_t::update_battle_info(const proto::common::world_cup_battle& battle_info)
{
	uint64_t battle_id = string_util_t::string_to_uint64(battle_info.uid());
	auto it = battle_map.find(battle_id);
	if (it == battle_map.end())
	{
		log_error("NULL == p_battle[%lu]", battle_id);
		return;
	}
	auto p_battle = it->second;
	if (NULL == p_battle)
	{
		log_error("NULL == p_battle[%lu]", battle_id);
		return;
	}
	p_battle->load_data(battle_info);
}
