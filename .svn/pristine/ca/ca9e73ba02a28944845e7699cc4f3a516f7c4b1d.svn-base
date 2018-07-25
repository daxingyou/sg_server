#include "hero_strategy_manager.hpp"
#include "arena/arena_score_rank_mgr.hpp"
#include "tblh/RaidersTable.tbls.h"
#include "redis_client.hpp"
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
	const arena_rank_ptr& p_arena_rank = arena_score_rank_mgr_t::get_arena_rank_ptr();
	if (!p_arena_rank) {
		log_error("p_arena_rank null error");
		return;
	}

	uint32_t all_role_num = p_arena_rank->get_rank_count();
	if (all_role_num == 0) {
		return;
	}

	m_all_hero_strategy_map.clear();

	// 武将攻略的武将统计信息
	struct hero_strategy_hero_count
	{
		hero_strategy_hero_count() {};

		uint32_t hero_use = 0;
		std::vector<proto::common::hero_strategy_single> m_heros; // 拥有这个武将的玩家
	};
	std::map<uint32_t, hero_strategy_hero_count> hero_count_map;

	std::map<uint32_t, RaidersTable*> confs;
	GET_ALL_CONF(RaidersTable, confs);
	for (std::map<uint32_t, RaidersTable*>::iterator itr = confs.begin();
		itr != confs.end(); ++itr) {
		hero_count_map.insert(std::make_pair(itr->first, hero_strategy_hero_count()));
	}

	const arena_rank_ptr& arena_rank = arena_score_rank_mgr_t::get_arena_rank_ptr();
	if (!arena_rank) {
		log_error("arena_rank null error");
		return;
	}

	std::vector<uint64_t> list;
	arena_rank->get_range_by_rank(0, -1, list);

	if (list.empty()) {
		return;
	}

	uint32_t rank_index = 0;
	for (const auto& uid : list) {//500
		++rank_index;

		proto::common::hero_data m_hero;
		if (!redis_client_t::get_protobuf("arena_score_rank", "fight_heroes", uid, m_hero)) {
			log_error("user[%lu] get fight_heroes error", uid);
			continue;
		}

		proto::common::item_data m_hero_equips;
		redis_client_t::get_protobuf("arena_score_rank", "hero_equips", uid, m_hero_equips);

		proto::common::hero_strategy_hero_info strategy_hero_info;
		for (int32_t i = 0; i < m_hero.hero_list_size(); ++i) {//6
			strategy_hero_info.add_hero_tid_list(m_hero.hero_list(i).base_attr().id());
		}

		for (int32_t i = 0; i < m_hero.hero_list_size(); ++i) {//6
			const proto::common::hero_single& hero_single = m_hero.hero_list(i);
			uint32_t hero_tid = hero_single.base_attr().id();
			std::map<uint32_t, hero_strategy_hero_count>::iterator itr_find = hero_count_map.find(hero_tid);
			if (itr_find == hero_count_map.end()) {
				log_error("hero_tid[%u] not find", hero_tid);
				continue;
			}
			itr_find->second.hero_use += 1;

			if (itr_find->second.m_heros.size() < hero_strategy_show_num) {
				proto::common::hero_strategy_single hero_strategy_info;
				hero_strategy_info.set_role_name(redis_client_t::get_string("arena_score_rank", "name", uid));
				hero_strategy_info.set_role_rank(rank_index);
				hero_strategy_info.set_role_score(redis_client_t::get_uint32("arena_score_rank", "score", uid));
				hero_strategy_info.set_server_id(redis_client_t::get_uint32("arena_score_rank", "server_id", uid));
				hero_strategy_info.set_server_name(redis_client_t::get_string("arena_score_rank", "server_name", uid));

				// 装备搭配列表
				for (int32_t j = 0; j < hero_single.base_attr().equips_size(); ++j) {
					uint64_t hero_equip_uid = string_util_t::string_to_uint64(hero_single.base_attr().equips(j).equip_uid());
					for (int32_t k = 0; k < m_hero_equips.item_list_size(); ++k) {
						uint64_t equip_uid = string_util_t::string_to_uint64(m_hero_equips.item_list(k).uid());
						if (hero_equip_uid == equip_uid) {
							proto::common::item_single* p_equip_single = hero_strategy_info.mutable_equip_info_list()->add_equip_list();
							if (p_equip_single) p_equip_single->CopyFrom(m_hero_equips.item_list(k));
							break;
						}
					}
				}
				hero_strategy_info.mutable_hero_info_list()->CopyFrom(strategy_hero_info);
				itr_find->second.m_heros.push_back(hero_strategy_info);
			}
		}
	}

	std::vector<std::pair<uint32_t, uint32_t>> sort_vec;
	for (std::map<uint32_t, hero_strategy_hero_count>::const_iterator citr = hero_count_map.begin();
		citr != hero_count_map.end(); ++citr) {
		sort_vec.push_back(std::make_pair(citr->first, citr->second.hero_use));
	}

	std::sort(sort_vec.begin(), sort_vec.end(), [](const std::pair<uint32_t, uint32_t>& h1, const std::pair<uint32_t, uint32_t>& h2) {
		if (h1.second != h2.second) {
			return h1.second > h2.second;
		}

		return h1.first < h2.first;
	});

	uint32_t index = 0;
	for (std::vector<std::pair<uint32_t, uint32_t>>::const_iterator citr = sort_vec.begin();
		citr != sort_vec.end(); ++citr) {
		std::map<uint32_t, hero_strategy_hero_count>::iterator itr_find = hero_count_map.find(citr->first);
		if (itr_find != hero_count_map.end()) {
			proto::common::hero_strategy_info hero_info;
			hero_info.set_hero_tid(citr->first);			
			hero_info.set_hero_use_rate(citr->second * 100 / all_role_num);
			hero_info.set_hero_use_hot(++index);

			for (uint32_t i = 0; i < itr_find->second.m_heros.size(); ++i) {
				proto::common::hero_strategy_single* p_hero_single = hero_info.add_role_list();
				if (p_hero_single) {
					p_hero_single->Swap(&itr_find->second.m_heros[i]);
				}
			}
			m_all_hero_strategy_map.insert(std::make_pair(citr->first, hero_info));
		}
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