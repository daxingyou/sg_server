#include "hero_strategy_manager.hpp"
#include "arena/arena_score_rank_mgr.hpp"
#include "tblh/RaidersTable.tbls.h"
#include "common/redis_client.hpp"
#include "config/config_manager.hpp"
#include "tblh/Comprehensive.tbls.h"
#include "main/area_server.hpp"
#include "main/area_server_fwd.hpp"

all_hero_strategy_map hero_strategy_manager_t::m_all_hero_strategy_map;

bool hero_strategy_manager_t::init()
{
	hero_strategy_manager_t::update_hero_strategy_data();
	
	return true;
}

void hero_strategy_manager_t::update_hero_strategy_data()
{
	rank_list& arena_score_rank_list = arena_score_rank_mgr_t::get_arena_score_rank();
	uint32_t all_role_num = arena_score_rank_list.size();
	if (all_role_num == 0)
		return;

	m_all_hero_strategy_map.clear();

	std::map<uint32_t, RaidersTable*> confs;
	GET_ALL_CONF(RaidersTable, confs);

	std::map<uint32_t, RaidersTable*>::iterator iter_conf = confs.begin();
	rank_list::iterator iter_rank = arena_score_rank_list.begin();


	proto::common::hero_strategy_info hero_info;
	proto::common::hero_strategy_single *p_strategy_info;
	uint32_t role_rank = 0;
	uint32_t hero_tid = 0;
	uint32_t hero_use_rate = 0;
	uint32_t hero_use_hot = 0;
	arena_rank_info_ptr p_rank_info = NULL;
	std::map<uint32_t, hero_strategy_hero_count> hero_count_map;
	std::vector<hero_strategy_hero_count> hero_count_vec;

	hero_strategy_hero_count hero_count;
	for (iter_conf = confs.begin(); iter_conf != confs.end(); ++iter_conf)
	{//200
		hero_count.hero_id = iter_conf->first;
		hero_count_map.insert(std::make_pair(hero_count.hero_id, hero_count));
	}

	for (iter_rank = arena_score_rank_list.begin(); iter_rank != arena_score_rank_list.end(); ++iter_rank)
	{//500
		p_rank_info = *iter_rank;
		if (NULL == p_rank_info)
		{
			log_error("p_rank_info NULL");
			continue;
		}
		for (int32_t i = 0; i < p_rank_info->get_hero_data().hero_list_size(); ++i)
		{//6
			hero_tid = p_rank_info->get_hero_data().hero_list(i).base_attr().id();
			if (hero_count_map.find(hero_tid) == hero_count_map.end())
			{
				log_error("hero_tid[%d] not find", hero_tid);
				continue;
			}
			hero_count_map[hero_tid].hero_use += 1;
		}
	}

	std::map<uint32_t, hero_strategy_hero_count>::iterator iter = hero_count_map.begin();
	for (; iter != hero_count_map.end(); ++iter)
	{//200
		hero_count_vec.push_back(iter->second);
	}
	common::Sort::Qsort(hero_count_vec, 0, hero_count_vec.size()-1, hero_strategy_hero_count::compare);
	hero_count_map.clear();
	for (uint32_t i = 0; i < hero_count_vec.size(); i++)
	{//200
		hero_count_vec[i].hero_hot = i + 1;
		hero_count_map.insert(std::make_pair(hero_count_vec[i].hero_id, hero_count_vec[i]));
	}

	for (iter_conf = confs.begin(); iter_conf != confs.end(); ++iter_conf)
	{//200
		hero_info.Clear();
		role_rank = 0;
		hero_tid = iter_conf->first;

		for (iter_rank = arena_score_rank_list.begin(); iter_rank != arena_score_rank_list.end(); ++iter_rank)
		{//500
			p_rank_info = *iter_rank;
			if (NULL == p_rank_info)
			{
				log_error("p_rank_info NULL");
				continue;
			}
			++role_rank;

			if (hero_info.role_list_size() >= hero_strategy_show_num)
				break;

			for (int32_t i = 0; i < p_rank_info->get_hero_data().hero_list_size(); ++i)
			{//6
				if (p_rank_info->get_hero_data().hero_list(i).base_attr().id() == hero_tid)
				{
					p_strategy_info = hero_info.add_role_list();
					p_rank_info->peek_hero_strategy_info(hero_tid, role_rank, p_strategy_info);
					break;
				}
			}
		}

		hero_use_rate = hero_count_map[hero_tid].hero_use * 100 / all_role_num;
		hero_use_hot = hero_count_map[hero_tid].hero_hot;
		hero_info.set_hero_tid(hero_tid);
		hero_info.set_hero_use_rate(hero_use_rate);
		hero_info.set_hero_use_hot(hero_use_hot);
		m_all_hero_strategy_map.insert(std::make_pair(hero_tid, hero_info));
	}
}

void hero_strategy_manager_t::get_hero_strategy_data_to_chat(uint32_t server_id)
{
	all_hero_strategy_map::iterator iter = m_all_hero_strategy_map.begin();
	proto::server::rh_get_hero_strategy_info_reply reply;
	reply.set_server_id(server_id);
	if (m_all_hero_strategy_map.size() == 0)
	{
		env::server->send_msg_to_transfer(op_cmd::rh_get_hero_strategy_info_reply, 0, reply);
		return;
	}

	uint32_t msg_count = 0;
	for (; iter != m_all_hero_strategy_map.end(); ++iter)
	{
		const proto::common::hero_strategy_info& hero_info = iter->second;
		msg_count += hero_info.ByteSize();
		if (msg_count >= network::MAX_TCP_DATA_SIZE)
		{
			env::server->send_msg_to_transfer(op_cmd::rh_get_hero_strategy_info_reply, 0, reply);
			reply.clear_hero_info();
			msg_count = hero_info.ByteSize();
		}
		reply.add_hero_info()->CopyFrom(hero_info);
	}
	if(reply.hero_info_size() > 0)
		env::server->send_msg_to_transfer(op_cmd::rh_get_hero_strategy_info_reply, 0, reply);
}