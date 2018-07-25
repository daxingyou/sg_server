#include "arena_score_rank_mgr.hpp"
#include "utility.hpp"
#include "common/redis_client.hpp"
#include "common/zdb_ex.hpp"
#include "main/area_server.hpp"
#include "main/area_server_fwd.hpp"
#include "config/config_manager.hpp"
#include "tblh/Comprehensive.tbls.h"
#include "tblh/ArenaClassTable.tbls.h"
#include "time_manager.hpp"
#include "protos_fwd.hpp"

#define RANK_MAX_SIZE 500

arena_rank_info_t::arena_rank_info_t()
{

}

arena_rank_info_t::~arena_rank_info_t()
{

}

bool arena_rank_info_t::has_hero(uint32_t hero_tid)
{
	for (int32_t i = 0; i < m_fight_heroes.hero_list_size(); ++i)
	{
		if (m_fight_heroes.hero_list(i).base_attr().id() == hero_tid)
			return true;
	}
	return false;
}

// void arena_rank_info_t::clear_fight_hero()
// {
// 	m_fight_heroes.clear();
// }
// 
// void arena_rank_info_t::add_fight_hero(uint32_t hero_type, uint32_t hero_id, uint32_t hero_grade)
// {
// 	m_fight_heroes.push_back(Dawn::triple<uint32_t,uint32_t,uint32_t>(hero_type, hero_id, hero_grade));
// }


void arena_rank_info_t::set_hero_data(const proto::common::hero_data& hero_data)
{
	m_fight_heroes.CopyFrom(hero_data);
}

void arena_rank_info_t::set_hero_equip(const proto::common::item_data& equip_data)
{
	m_hero_equips.CopyFrom(equip_data);
}


void arena_rank_info_t::peek_hero_strategy_info(uint32_t hero_tid, uint32_t role_rank, proto::common::hero_strategy_single *p_hero_strategy_info)
{
	if (NULL == p_hero_strategy_info)
	{
		log_error("p_hero_strategy_info NULL");
		return;
	}
	p_hero_strategy_info->set_role_name(get_name());
	p_hero_strategy_info->set_role_rank(role_rank);
	p_hero_strategy_info->set_role_score(get_score());
	p_hero_strategy_info->set_server_id(get_server_id());
	p_hero_strategy_info->set_server_name(get_server_name());
	
	uint32_t temp_hero_tid = 0;
	uint64_t hero_equip_uid = 0;
	uint64_t equip_uid = 0;
	proto::common::item_single *p_equip_single = NULL;

	for (int32_t i = 0; i < m_fight_heroes.hero_list_size(); ++i)
	{
		const proto::common::hero_single& hero_single = m_fight_heroes.hero_list(i);
		temp_hero_tid = hero_single.base_attr().id();
		if (temp_hero_tid == hero_tid)
		{
			for (int32_t j = 0; j < hero_single.base_attr().equips_size(); ++j)
			{
				hero_equip_uid = string_util_t::string_to_uint64(hero_single.base_attr().equips(j).equip_uid());

				for (int32_t k = 0; k < m_hero_equips.item_list_size(); ++k)
				{
					equip_uid = string_util_t::string_to_uint64(m_hero_equips.item_list(k).uid());
					if (hero_equip_uid == equip_uid)
					{
						const proto::common::item_single &equip_single = m_hero_equips.item_list(k);
						p_equip_single = p_hero_strategy_info->mutable_equip_info_list()->add_equip_list();
						p_equip_single->CopyFrom(equip_single);
						break;
					}
				}
			}
			continue;
		}
		p_hero_strategy_info->mutable_hero_info_list()->add_hero_tid_list(temp_hero_tid);
	}
}

void arena_rank_info_t::save_self()
{
	redis_client_t::set_uint32("arena_score_rank", "score", m_role_uid, m_score);
	redis_client_t::set_uint32("arena_score_rank", "stage", m_role_uid, m_stage);
	redis_client_t::set_uint32("arena_score_rank", "time", m_role_uid, m_time);
	redis_client_t::set_uint32("arena_score_rank", "server_id", m_role_uid, m_server_id);
	redis_client_t::set_uint32("arena_score_rank", "fighting_value", m_role_uid, m_fighting_value);
	redis_client_t::set_string("arena_score_rank", "name", m_role_uid, m_name);
	redis_client_t::set_protobuf("arena_score_rank", "fight_heroes", m_role_uid, m_fight_heroes);
	redis_client_t::set_protobuf("arena_score_rank", "hero_equips", m_role_uid, m_hero_equips);
}


void arena_rank_info_t::peek_data(proto::common::arena_role_rank_info* p_data)
{
	if (NULL != p_data)
	{
		p_data->set_uid(string_util_t::uint64_to_string(m_role_uid));
		p_data->set_score(m_score);
		p_data->set_stage(m_stage);
		p_data->set_time(m_time);
		p_data->set_server_id(m_server_id);
		p_data->set_fighting_value(m_fighting_value);
		p_data->set_name(m_name);
		p_data->set_server_name(m_server_name);
		p_data->mutable_fight_heroes()->CopyFrom(m_fight_heroes);
		p_data->mutable_hero_equips()->CopyFrom(m_hero_equips);
	}
}

rank_list arena_score_rank_mgr_t::m_arena_score_rank_list;
rank_map arena_score_rank_mgr_t::m_arena_score_rank_map;


uint32_t arena_score_rank_mgr_t::update_arena_score_rank(uint64_t role_uid, uint32_t score, uint32_t stage,
														 uint32_t server_id, const char *name, const char *server_name,
														 uint32_t fighting_value,
														 const proto::common::hero_data &hero_info,
														 const proto::common::item_data &hero_equip)
{
	arena_rank_info_ptr p_info = NULL;
	auto it = m_arena_score_rank_map.find(role_uid);
	if (it == m_arena_score_rank_map.end())
	{
		//自己不在排行榜内
		if (m_arena_score_rank_list.size() >= RANK_MAX_SIZE)
		{
			//如果排行榜已达上限就比较最后一名
			arena_rank_info_ptr p_last = *m_arena_score_rank_list.rbegin();
			if (NULL == p_last)
			{
				m_arena_score_rank_list.pop_back();
			}
			else if (p_last->get_score() < score)
			{
				//比最后一名积分高，插入
				m_arena_score_rank_list.pop_back();
				auto it_last = m_arena_score_rank_map.find(p_last->get_role_uid());
				if (it_last != m_arena_score_rank_map.end())
				{
					m_arena_score_rank_map.erase(it_last);
				}
				redis_client_t::remove_key("arena_score_rank", p_last->get_role_uid());
			}
			else
			{
				return 0;
			}
		}
		p_info = arena_rank_info_ptr(new arena_rank_info_t);
		p_info->set_role_uid(role_uid);
		p_info->set_score(score);
		p_info->set_stage(stage);
		p_info->set_time(time_util_t::now_time());
		p_info->set_server_id(server_id);
		p_info->set_name(name);
		p_info->set_server_name(server_name);
		p_info->set_fighting_value(fighting_value);
		p_info->set_hero_data(hero_info);
		p_info->set_hero_equip(hero_equip);
		m_arena_score_rank_list.push_back(p_info);
		m_arena_score_rank_list.sort(rank_sort());
		m_arena_score_rank_map.insert(std::make_pair(role_uid, p_info));
	}
	else
	{
		p_info = it->second;
		if (NULL != p_info)
		{
			p_info->set_score(score);
			p_info->set_stage(stage);
			p_info->set_time(time_util_t::now_time());
			p_info->set_server_id(server_id);
			p_info->set_name(name);
			p_info->set_server_name(server_name);
			p_info->set_fighting_value(fighting_value);
			p_info->set_hero_data(hero_info);
			p_info->set_hero_equip(hero_equip);
		}
		m_arena_score_rank_list.sort(rank_sort());
	}
	if (NULL != p_info)
	{
		p_info->save_self();
		auto it_list = std::find(m_arena_score_rank_list.begin(), m_arena_score_rank_list.end(), p_info);
		if (it_list != m_arena_score_rank_list.end())
		{
			return std::distance(m_arena_score_rank_list.begin(), it_list) + 1;
		}
	}

    return 0;
}

void arena_score_rank_mgr_t::save_history_score_rank()
{
    //save rank map to history
    std::ostringstream ss;

	//std::string fight_heroes = "";
 //   uint32_t num = get_current_num();
 //   for(auto itr = m_arena_score_rank_map.rbegin(); itr != m_arena_score_rank_map.rend(); itr++)
 //   {
 //       ss<<itr->second.uid()<<itr->second.num();
 //       redis_client_t::set_uint32("arena_score_rank_history", "score", ss.str(), itr->second.score());
 //       redis_client_t::set_uint32("arena_score_rank_history", "stage", ss.str(), itr->second.stage());
 //       redis_client_t::set_uint32("arena_score_rank_history", "time", ss.str(), itr->second.time());
 //       redis_client_t::set_uint32("arena_score_rank_history", "server_id", ss.str(), itr->second.server_id());
	//	redis_client_t::set_uint32("arena_score_rank_history", "rank", ss.str(), itr->second.rank());
	//	redis_client_t::set_uint32("arena_score_rank", "fighting_value", itr->second.uid(), itr->second.fighting_value());
 //       redis_client_t::set_uint32("arena_score_rank_history", "num", ss.str(), num);
	//	redis_client_t::set_string("arena_score_rank_history", "name", ss.str(), itr->second.name());
	//	pack_rank_hero_data(itr->second.fight_heroes(), fight_heroes);
	//	redis_client_t::set_string("arena_score_rank_history", "fight_heroes", ss.str(), fight_heroes);

 //       ss.str("");
 //   }

}


arena_rank_info_ptr arena_score_rank_mgr_t::find_user_arena_info(uint64_t role_uid)
{
	auto it = m_arena_score_rank_map.find(role_uid);
	if (it == m_arena_score_rank_map.end())
	{
		return NULL;
	}
	return it->second;
}

uint32_t arena_score_rank_mgr_t::get_current_num()
{
	auto conf_rank_save = GET_CONF(Comprehensive, comprehensive_common::arena_season_rank_save);
	uint32_t rank_save = GET_COMPREHENSIVE_VALUE_1(conf_rank_save);

    return rank_save;
}

uint32_t arena_score_rank_mgr_t::get_rank(uint64_t role_uid)
{
	auto it = m_arena_score_rank_map.find(role_uid);
	if (it == m_arena_score_rank_map.end())
	{
		return 0;
	}
	arena_rank_info_ptr p_info = it->second;
	if (NULL == p_info)
	{
		return 0;
	}
	auto it_list = std::find(m_arena_score_rank_list.begin(), m_arena_score_rank_list.end(), p_info);
    if(it_list == m_arena_score_rank_list.end())
    {
        return 0;
    }

	return std::distance(m_arena_score_rank_list.begin(), it_list) + 1;
}

bool arena_score_rank_mgr_t::load_from_redis(uint32_t time_id)
{
	uint32_t now_time = time_util_t::now_time();
    std::vector<uint64_t> uids;
	redis_client_t::get_all_keys<uint64_t, std::vector<uint64_t>>("arena_score_rank", uids);

	for (auto& uid : uids)
	{
		arena_rank_info_ptr p_info = arena_rank_info_ptr(new arena_rank_info_t);
		p_info->set_role_uid(uid);
		uint32_t score = redis_client_t::get_uint32("arena_score_rank", "score", uid);
		uint32_t stage = redis_client_t::get_uint32("arena_score_rank", "stage", uid);
		uint32_t last_time = redis_client_t::get_uint32("arena_score_rank", "time", uid);
		p_info->set_server_id(redis_client_t::get_uint32("arena_score_rank", "server_id", uid));
		p_info->set_fighting_value(redis_client_t::get_uint32("arena_score_rank", "fighting_value", uid));
		p_info->set_name(redis_client_t::get_string("arena_score_rank", "name", uid));
		proto::common::hero_data hero_data;
		proto::common::item_data equip_data;;
		redis_client_t::get_protobuf("arena_score_rank", "fight_heroes", uid, hero_data);
		redis_client_t::get_protobuf("arena_score_rank", "hero_equips", uid, equip_data);
		p_info->set_hero_data(hero_data);
		p_info->set_hero_equip(equip_data);
		
		if (!time_manager_t::is_same_period(time_id, last_time))
		{
			ARENA_LOG("role[%lu] arena score reset", uid);
			//重置
			score = reset_stage(stage);
			p_info->set_score(score);
			p_info->set_stage(stage);
			p_info->set_time(now_time);
			p_info->save_self();
		}
		else
		{
			p_info->set_score(score);
			p_info->set_stage(stage);
			p_info->set_time(last_time);
		}
		m_arena_score_rank_map.insert(std::make_pair(uid, p_info));
		m_arena_score_rank_list.push_back(p_info);
	}
	m_arena_score_rank_list.sort(rank_sort());
    return true;
}

bool arena_score_rank_mgr_t::get_rank_data(uint32_t rank_start_pos, uint32_t server_id, uint64_t uid)
{
	proto::server::rg_get_arena_rank_data_reply reply;
	reply.set_server_id(server_id);

	// 0代表查找玩家自己
	if (rank_start_pos == 0)
	{
		arena_rank_info_ptr p_temp_info = find_user_arena_info(uid);
		if (NULL == p_temp_info)
		{
			env::server->send_msg_to_transfer(op_cmd::rg_get_arena_rank_data_reply, uid, reply);
			return true;
		}
		proto::common::arena_role_rank_info *p_rank_info = reply.mutable_rand_data()->add_info_list();
		p_temp_info->peek_data(p_rank_info);
		p_rank_info->set_rank(get_rank(uid));
		env::server->send_msg_to_transfer(op_cmd::rg_get_arena_rank_data_reply, uid, reply);
		return true;
	}

	auto conf_show_num = GET_CONF(Comprehensive, comprehensive_common::arena_rank_show_each_num);
	uint32_t show_num = GET_COMPREHENSIVE_VALUE_1(conf_show_num);

	if (rank_start_pos > m_arena_score_rank_map.size())
	{
		return false;
	}

	auto iter = m_arena_score_rank_list.begin();
	for (uint32_t i = 1; i < rank_start_pos; ++i)
	{
		if (iter == m_arena_score_rank_list.end())
			break;
		++iter;
	}

	for (uint32_t i = 0; i < show_num; ++i)
	{
		if (iter == m_arena_score_rank_list.end())
			break;
		auto p_rank_info = *iter;
		if (NULL == p_rank_info)
		{
			++iter;
			continue;
		}
		proto::common::arena_role_rank_info *p_info = reply.mutable_rand_data()->add_info_list();
		p_rank_info->peek_data(p_info);
		p_info->set_rank(get_rank(p_rank_info->get_role_uid()));
		++iter;
	}
	env::server->send_msg_to_transfer(op_cmd::rg_get_arena_rank_data_reply, uid, reply);

	return true;
}

void arena_score_rank_mgr_t::get_16th_final_player(std::vector<uint64_t>& player_vec)
{
	uint32_t count = 0;
	for (auto it : m_arena_score_rank_list)
	{
		uint64_t player_uid = it->get_role_uid();
		if (player_uid > 0)
		{
			player_vec.push_back(player_uid);
			++count;
		}
		if (count >= 32)
		{
			break;
		}
	}
}

uint32_t arena_score_rank_mgr_t::reset_stage(uint32_t& stage)
{
	auto p_conf = GET_CONF(ArenaClassTable, stage);
	if (NULL == p_conf)
	{
		log_error("NULL == p_conf[%d]", stage);
		return 0;
	}
	stage = p_conf->reset_stage();
	p_conf = GET_CONF(ArenaClassTable, stage);
	if (NULL == p_conf)
	{
		log_error("NULL == p_conf[%d]", stage);
		return 0;
	}
	return p_conf->score();
}

void arena_score_rank_mgr_t::reset_stage()
{
	uint32_t now_time = time_util_t::now_time();
	for (auto p_info : m_arena_score_rank_list)
	{
		if (NULL == p_info)
		{
			continue;
		}
		uint32_t stage = p_info->get_stage();
		uint32_t score = reset_stage(stage);
		p_info->set_stage(stage);
		p_info->set_score(score);
		p_info->set_time(now_time);
		p_info->save_self();
	}
	m_arena_score_rank_list.sort(rank_sort());
}