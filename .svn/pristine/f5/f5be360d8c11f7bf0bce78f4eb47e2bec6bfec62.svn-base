#include "offline_fight_manager.hpp"

#include "client.pb.h"
#include "tblh/errcode_enum.hpp"
#include "redis_client.hpp"
#include "config_mgr.h"
#include "tblh/Expedition.tbls.h"
#include "tblh/Monster.tbls.h"
#include "user_troop/user_troop_mgr.hpp"
#include "family/family_manager.hpp"
#include "tblh/Comprehensive.tbls.h"
#include "config_mgr.h"

USING_NS_COMMON;

offline_fight_map offline_fight_manager_t::m_offline_fight_map;
std::map<uint32_t, Match*> offline_fight_manager_t::m_match_confs;
std::map<uint32_t, uint32_t> offline_fight_manager_t::m_match_monster;

void offline_fight_manager_t::load_data()
{
	m_offline_fight_map.clear();
	//读取所有竞技场玩家
	std::vector<std::uint64_t> role_uids;
	redis_client_t::get_all_keys<uint64_t, std::vector<std::uint64_t>>("arena_fight_data", role_uids);
	EXPE_LOG("role_uids size[%d]", (int32_t)role_uids.size());
	for (auto role_uid : role_uids)
	{
		//uint32_t update_time = redis_client_t::get_uint32("arena_fight_data", "update_time", role_uid);
		uint32_t fight_value = redis_client_t::get_uint32("arena_fight_data", "fight_value", role_uid);
		//uint32_t today = time_util_t::get_day();
		//uint32_t update_day = time_util_t::get_day(update_time);
		//if (today - 3 >= update_day)
		//{
		//	//过滤不活跃的玩家
		//	continue;
		//}
		//根据战斗力分档存储，1000为一档，超过2万算2万
		auto conf1 = GET_CONF(Comprehensive, comprehensive_common::glory_enemy_sword);
		auto conf2 = GET_CONF(Comprehensive, comprehensive_common::glory_enemy_division);
		uint32_t sword = GET_COMPREHENSIVE_VALUE_1(conf1);
		uint32_t division = GET_COMPREHENSIVE_VALUE_1(conf2);
		if (sword == 0)
			sword = 1000;

		uint32_t val = fight_value / sword;
		if (val < 1)
		{
			val = 1;
		}
		if (val > division)
		{
			val = division;
		}
		m_offline_fight_map[val].push_back(role_uid);
	}
	for (auto it : m_offline_fight_map)
	{
		EXPE_LOG("m_offline_fight_map[%d:%d]", it.first, (int32_t)it.second.size());
	}
}

void offline_fight_manager_t::one_day()
{
	load_data();
}

void offline_fight_manager_t::update_fight_data(uint64_t role_uid, uint32_t fight_value, proto::common::mirror_fight_data* p_fight_data)
{
	uint32_t old_value = redis_client_t::get_uint32("arena_fight_data", "fight_value", role_uid);
	redis_client_t::set_uint32("arena_fight_data", "fight_value", role_uid, fight_value);
	redis_client_t::set_uint32("arena_fight_data", "update_time", role_uid, time_util_t::now_time());
	global_user_ptr p_role = global_user_manager_t::get_global_user_by_uid(role_uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role[%lu]", role_uid);
		return;
	}
	if (p_role->get_family_id() > 0)
	{
		family_ptr p_family = family_manager_t::get_family(p_role->get_family_id());
		if (NULL != p_family)
		{
			p_fight_data->mutable_obj()->set_family_name(p_family->get_name());
		}
	}
	redis_client_t::set_protobuf("arena_fight_data", "fight_data", role_uid, *p_fight_data);

	uint32_t old_val = old_value / 1000;
	uint32_t new_val = fight_value / 1000;
	if (old_val < 1)
	{
		old_val = 1;
	}
	if (old_val > 20)
	{
		old_val = 20;
	}
	if (new_val < 1)
	{
		new_val = 1;
	}
	if (new_val > 20)
	{
		new_val = 20;
	}
	if (0 == old_value)
	{
		m_offline_fight_map[new_val].push_back(role_uid);
		return;
	}
	if (old_val == new_val)
	{
		return;
	}
	auto it = m_offline_fight_map.find(old_val);
	if (it != m_offline_fight_map.end())
	{
		std::vector<uint64_t>& role_vec = it->second;
		auto it_vec = std::find(role_vec.begin(), role_vec.end(), role_uid);
		if (it_vec != role_vec.end())
		{
			role_vec.erase(it_vec);
		}
	}
	m_offline_fight_map[new_val].push_back(role_uid);
}

void offline_fight_manager_t::make_conf()
{
	m_match_confs.clear();
	std::map<uint32_t, Match*> match_map;
	GET_ALL_CONF(Match, match_map);
	for (auto it : match_map)
	{
		auto p_conf = it.second;
		if (NULL == p_conf)
		{
			continue;
		}
		uint32_t min_level = p_conf->level_range().first;
		uint32_t max_level = p_conf->level_range().second;
		for (uint32_t i = min_level; i <= max_level; ++i)
		{
			m_match_confs[i] = p_conf;
		}
	}

	m_match_monster.clear();
	std::map<uint32_t, Monster*> monster_map;
	GET_ALL_CONF(Monster, monster_map);
	for (auto it : monster_map)
	{
		auto p_conf = it.second;
		if (NULL == p_conf)
		{
			continue;
		}
		if (5 != p_conf->type())
		{
			continue;
		}
		m_match_monster[p_conf->arena_level()] = p_conf->id();
	}
}

void offline_fight_manager_t::clear_conf()
{
	m_match_confs.clear();
}

void offline_fight_manager_t::get_friend_recommend(uint32_t fighting, std::vector<uint64_t>& uids)
{
	auto conf1 = GET_CONF(Comprehensive, comprehensive_common::glory_enemy_sword);
	auto conf2 = GET_CONF(Comprehensive, comprehensive_common::glory_enemy_division);
	uint32_t sword = GET_COMPREHENSIVE_VALUE_1(conf1);
	uint32_t division = GET_COMPREHENSIVE_VALUE_1(conf2);
	if (sword == 0)
		sword = 1000;

	uint32_t val = fighting / sword;
	if (val < 1)
	{
		val = 1;
	}
	if (val > division)
	{
		val = division;
	}
	//log_warn("val[%d]", val);
	for (uint32_t i = val - 1; i < val + 1; ++i)
	{
		auto it = m_offline_fight_map.find(i);
		if (it == m_offline_fight_map.end())
		{
			continue;
		}
		const std::vector<uint64_t>& temp_vec = it->second;
		uids.insert(uids.end(), temp_vec.begin(), temp_vec.end());
		//log_warn("temp_vec[%d]", (uint32_t)temp_vec.size());
	}
}

void offline_fight_manager_t::make_fight_data(uint32_t level, proto::server::eg_expedition_fight_data_reply& reply)
{
	auto it_conf = m_match_confs.find(level);
	if (it_conf == m_match_confs.end())
	{
		log_error("NULL == p_conf[%d] level invalid", level);
		return;
	}
	auto p_match_conf = it_conf->second;
	if (NULL == p_match_conf)
	{
		log_error("NULL == p_conf[%d] level invalid", level);
		return;
	}
	std::map<uint32_t, Expedition*> exp_map;
	GET_ALL_CONF(Expedition, exp_map);
	for (auto it : exp_map)
	{
		auto p_conf = it.second;
		if (NULL == p_conf)
		{
			continue;
		}
		proto::common::expedition_stage_single* p_single = reply.add_stage_list();
		p_single->set_stage_id(p_conf->id());
		p_single->set_pass_num(0);
		switch (p_conf->fight_level())
		{
		case stage_level_b:
			make_stage_data(p_match_conf->fight_level_b(), p_single);
			break;
		case stage_level_a:
			make_stage_data(p_match_conf->fight_level_a(), p_single);
			break;
		case stage_level_s:
			make_stage_data(p_match_conf->fight_level_s(), p_single);
			break;
		case stage_level_ss:
			make_stage_data(p_match_conf->fight_level_ss(), p_single);
			break;
		case stage_level_sss:
			make_stage_data(p_match_conf->fight_level_sss(), p_single);
			break;
		}
	}
}

void offline_fight_manager_t::refresh_fight_data(uint32_t level, proto::server::eg_expedition_refresh_reply& reply)
{
	auto it_conf = m_match_confs.find(level);
	if (it_conf == m_match_confs.end())
	{
		log_error("NULL == p_conf[%d] level invalid", level);
		return;
	}
	auto p_match_conf = it_conf->second;
	if (NULL == p_match_conf)
	{
		log_error("NULL == p_conf[%d] level invalid", level);
		return;
	}
	std::map<uint32_t, Expedition*> exp_map;
	GET_ALL_CONF(Expedition, exp_map);
	for (auto it : exp_map)
	{
		auto p_conf = it.second;
		if (NULL == p_conf)
		{
			continue;
		}
		proto::common::expedition_stage_single* p_single = reply.add_stage_list();
		p_single->set_stage_id(p_conf->id());
		p_single->set_pass_num(0);
		switch (p_conf->fight_level())
		{
		case stage_level_b:
			make_stage_data(p_match_conf->fight_level_b(), p_single);
			break;
		case stage_level_a:
			make_stage_data(p_match_conf->fight_level_a(), p_single);
			break;
		case stage_level_s:
			make_stage_data(p_match_conf->fight_level_s(), p_single);
			break;
		case stage_level_ss:
			make_stage_data(p_match_conf->fight_level_ss(), p_single);
			break;
		case stage_level_sss:
			make_stage_data(p_match_conf->fight_level_sss(), p_single);
			break;
		}
	}
}

void offline_fight_manager_t::refresh_stage_data(uint32_t level, uint32_t stage_id, proto::server::eg_expedition_refresh_stage_reply& reply)
{
	auto it_conf = m_match_confs.find(level);
	if (it_conf == m_match_confs.end())
	{
		log_error("NULL == p_conf[%d] level invalid", level);
		return;
	}
	auto p_match_conf = it_conf->second;
	if (NULL == p_match_conf)
	{
		log_error("NULL == p_match_conf[%d] level invalid", level);
		return;
	}
	Expedition* p_conf = GET_CONF(Expedition, stage_id);
	if (NULL == p_conf)
	{
		log_error("NULL == p_conf[%d] stage invalid", stage_id);
		return;
	}
	proto::common::expedition_stage_single* p_single = reply.mutable_stage_data();
	p_single->set_stage_id(p_conf->id());
	switch (p_conf->fight_level())
	{
	case stage_level_b:
		make_stage_data(p_match_conf->fight_level_b(), p_single);
		break;
	case stage_level_a:
		make_stage_data(p_match_conf->fight_level_a(), p_single);
		break;
	case stage_level_s:
		make_stage_data(p_match_conf->fight_level_s(), p_single);
		break;
	case stage_level_ss:
		make_stage_data(p_match_conf->fight_level_ss(), p_single);
		break;
	case stage_level_sss:
		make_stage_data(p_match_conf->fight_level_sss(), p_single);
		break;
	}
}

uint32_t offline_fight_manager_t::expedition_help(global_user_ptr p_user, const std::vector<uint64_t>& helper_list)
{
	if (NULL == p_user)
	{
		log_error("NULL == p_user");
		return errcode_enum::notice_role_null;
	}
	uint64_t troop_id = p_user->get_troop_id();
	if (troop_id == 0)
	{
		uint32_t reply_code = global_troop_manager::create_new_troop(p_user, EXPEDITION_HELP_TROOP_TYPE);
		if (reply_code != errcode_enum::error_ok)
		{
			return reply_code;
		}
		troop_id = p_user->get_troop_id();
	}
	troop_ptr p_troop = global_troop_manager::get_troop(troop_id);
	if (NULL == p_troop)
	{
		log_error("NULL == p_troop[%lu]", troop_id);
		return errcode_enum::user_troop_err_code_not_exsit;
	}
	if (p_troop->get_leader_id() != p_user->get_uid())
	{
		log_error("p_user[%lu] not leader", p_user->get_uid());
		return errcode_enum::user_troop_err_code_no_right;
	}
	if (p_troop->is_full()) {
		log_error("troop[%lu] is full", troop_id);
		return errcode_enum::user_troop_err_code_full;
	}

	auto unlock_level = GET_CONF(Comprehensive, comprehensive_common::team_unlock_level);
	uint32_t team_unlock_level = GET_COMPREHENSIVE_VALUE_1(unlock_level);

	for (auto helper_uid : helper_list)
	{
		global_user_ptr p_helper = global_user_manager_t::get_global_user_by_uid(helper_uid);
		if (NULL == p_helper)
		{
			continue;
		}

		if (p_user->get_family_id() != p_helper->get_family_id())
		{
			continue;
		}

		if (p_helper->get_troop_id() != 0)
		{
			continue;
		}

		if (p_helper->get_level() < team_unlock_level)
		{
			continue;
		}

		if (!p_helper->is_online()) {
			continue;
		}

		p_troop->invite_join(p_user, p_helper);
	}
	return errcode_enum::error_ok;
}

void offline_fight_manager_t::make_stage_data(const std::vector<uint32_t>& fight_val, proto::common::expedition_stage_single* p_data)
{
	if (NULL == p_data)
	{
		log_error("NULL == p_data");
		return;
	}
	if (fight_val.empty())
	{
		log_error("fight_val empty");
		return;
	}
	std::vector<uint64_t> role_list;
	for (auto val : fight_val)
	{
		auto it = m_offline_fight_map.find(val);
		if (it == m_offline_fight_map.end())
		{
			continue;
		}
		const std::vector<uint64_t>& temp_vec = it->second;
		role_list.insert(role_list.end(), temp_vec.begin(), temp_vec.end());
	}
	for (uint32_t i = 0; i < get_stage_enemy_count(); i++)
	{
		proto::common::mirror_fight_data* p_fight_data = p_data->add_stage_info();
		if (role_list.empty())
		{
			int32_t rand_index = random_util_t::randMin(0, fight_val.size());
			if (-1 == rand_index)
			{
				log_error("fight_val empty");
				return;
			}
			uint32_t val = fight_val.at(rand_index);
			if (m_match_monster.find(val) == m_match_monster.end())
			{
				log_error("m_match_monster[%d] invalid", val);
				continue;
			}
			proto::common::obj_single* p_obj = p_fight_data->mutable_obj();
			p_obj->set_monster_id(m_match_monster[val]);
			p_obj->set_object_type(proto::common::SCENEOBJECT_MONSTER);
		}
		else
		{
			int32_t rand_index = random_util_t::randMin(0, role_list.size());
			if (-1 == rand_index)
			{
				log_error("role_list empty");
				return;
			}
			uint64_t role_uid = role_list[rand_index];
			redis_client_t::get_protobuf("arena_fight_data", "fight_data", role_uid, *p_fight_data);
		}
	}
}

uint32_t offline_fight_manager_t::get_stage_enemy_count()
{
	return 1;
}

