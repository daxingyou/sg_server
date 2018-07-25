#include <boost/assign/std/map.hpp>

#include "rank_manager.hpp"
#include "tblh/errcode_enum.hpp"
#include "tblh/TimeRank.tbls.h"
#include "tblh/RankType.tbls.h"
#include "config_mgr.h"
#include "time_manager.hpp"

rank_manager_t::rank_sort_map rank_manager_t::m_rank_sort_map;

bool rank_manager_t::init()
{
	boost::assign::insert(m_rank_sort_map)
		(common::rank_common::lv_rank_0, rank_level_ptr(new rank_level_t(common::rank_common::lv_rank_0, "levelsort")))
		(common::rank_common::towers_rank_0, rank_tower_ptr(new rank_tower_t(common::rank_common::towers_rank_0, "towersort")))
		(common::rank_common::criminal_rank_0, rank_criminal_ptr(new rank_criminal_t(common::rank_common::criminal_rank_0, "criminalsort")))
		(common::rank_common::achievement_rank_0, rank_achieve_ptr(new rank_achieve_t(common::rank_common::achievement_rank_0, "achievesort")))
		(common::rank_common::family_rank_0, rank_family_ptr(new rank_family_t(common::rank_common::family_rank_0, "familysort")))
		(common::rank_common::charm_rank_0, rank_charm_ptr(new rank_charm_t(common::rank_common::charm_rank_0, "charmsort")))
		(common::rank_common::recruit_rank_0, rank_recruit_ptr(new rank_recruit_t(common::rank_common::recruit_rank_0, "recruitsort")))
		(common::rank_common::consume_copper_rank_0, rank_copper_ptr(new rank_copper_t(common::rank_common::consume_copper_rank_0, "coppersort")))
		(common::rank_common::consume_golden_rank_0, rank_gold_ptr(new rank_gold_t(common::rank_common::consume_golden_rank_0, "goldsort")))
		(common::rank_common::combat_power_rank_0, rank_power_ptr(new rank_power_t(common::rank_common::combat_power_rank_0, "powersort")));

	for (rank_sort_map::const_iterator citr = m_rank_sort_map.begin();
		citr != m_rank_sort_map.end(); ++citr) {
		const rank_sort_ptr& p_rank = citr->second;
		if (p_rank) {
			p_rank->init();
		}
	}
	std::map<uint32_t, TimeRank*> conf_map;
	GET_ALL_CONF(TimeRank, conf_map);
	for (auto it : conf_map)
	{
		TimeRank* p_conf = it.second;
		if (NULL == p_conf)
		{
			continue;
		}
		time_manager_t::register_func(it.first, on_time_rank);
		bool is_in_time = time_manager_t::is_in_time(it.first);
		for (auto rank_id : p_conf->rank())
		{
			RankType* p_rank_conf = GET_CONF(RankType, rank_id);
			if (NULL == p_rank_conf)
			{
				continue;
			}
			if (1 == p_rank_conf->type() && !is_in_time)
			{
				rank_sort_ptr p_rank = get_rank_sort_by_type(rank_id);
				if (NULL != p_rank)
				{
					p_rank->set_open(false);
				}
			}
		}
	}
	return true;
}


rank_sort_ptr rank_manager_t::get_rank_sort_by_type(uint32_t type)
{
	rank_sort_map::const_iterator citr = m_rank_sort_map.find(type);
	if (citr != m_rank_sort_map.end()) {
		return citr->second;
	}

	return NULL;
}


uint32_t rank_manager_t::get_rank_by_uid(uint32_t type, uint64_t uid)
{
	rank_sort_ptr p_rank = get_rank_sort_by_type(type);
	if (!p_rank) {
		log_error("rank_type[%u] not find", type);
		return 0;
	}

	return p_rank->get_rank_value(uid);
}

void rank_manager_t::update_rank(const proto::server::ge_update_rank_notify& notify)
{
	rank_sort_ptr p_rank_sort = get_rank_sort_by_type(notify.rank_type());
	if (p_rank_sort) {
		p_rank_sort->update_rank_from_game(notify);
	}
}

void rank_manager_t::request_rank(const global_user_ptr& p_user, uint32_t rank_type, uint32_t page, proto::client::ec_rank_reply& reply)
{
	reply.set_page(page);

	if (!p_user) {
		log_error("p_user null error");
		reply.set_reply_code(common::errcode_enum::notice_unknown);
		return;
	}

	rank_sort_ptr p_rank_sort = get_rank_sort_by_type(rank_type);
	if (!p_rank_sort) {
		log_error("user[%lu] rank type[%u] not find", p_user->get_uid(), rank_type);
		reply.set_reply_code(common::errcode_enum::notice_unknown);
		return;
	}

	uint32_t back_code = p_rank_sort->get_page_list(page, reply);
	if (back_code == common::errcode_enum::error_ok && page == 0) {
		switch (rank_type) {
			case common::rank_common::criminal_rank_0: // 全职业通缉 
			case common::rank_common::lv_rank_0: // 等级排行榜 
			case common::rank_common::towers_rank_0: // 千层塔排名榜
			case common::rank_common::charm_rank_0:	// 魅力排行榜
			case common::rank_common::achievement_rank_0: // 成就积分榜 
			case common::rank_common::recruit_rank_0: // 招募次数榜 
			case common::rank_common::consume_copper_rank_0:  //铜币消耗榜 
			case common::rank_common::consume_golden_rank_0:  // 金币消耗榜 
			case common::rank_common::combat_power_rank_0: { // 战力榜 
				p_rank_sort->peek_data(p_user->get_uid(), reply.mutable_self());
				break;
			}
			case common::rank_common::family_rank_0: {
				if (p_user->get_family_id() != 0) {
					p_rank_sort->peek_data(p_user->get_family_id(), reply.mutable_self());
				}
				break;
			}
			default:
				log_error("rank type[%u] not define", rank_type);
				break;
		}
	}

	reply.set_rank_type(rank_type);
	reply.set_reply_code(back_code);
}


void rank_manager_t::request_rank_time(proto::client::ec_rank_time_reply& reply)
{
	std::map<uint32_t, TimeRank*> conf_map;
	GET_ALL_CONF(TimeRank, conf_map);
	for (auto it : conf_map)
	{
		TimeRank* p_conf = it.second;
		if (NULL == p_conf)
		{
			continue;
		}
		uint32_t time_id = it.first;
		uint32_t start_time = time_manager_t::get_timer_start_time(time_id);
		uint32_t end_time = time_manager_t::get_timer_end_time(time_id);
	
		bool flag = false;
		if (time_manager_t::is_in_time(time_id))
		{
			flag = true;
		}
		else
		{
			uint32_t now_time = time_util_t::now_time();
			if (now_time < start_time)
			{
				uint32_t delta_time = start_time - now_time;
				if (delta_time <= 24*3600)
				{
					flag = true;
				}
			}
		}
		if (flag)
		{
			for (auto rank_id : p_conf->rank())
			{
				RankType* p_rank_conf = GET_CONF(RankType, rank_id);
				if (NULL == p_rank_conf)
				{
					continue;
				}
				proto::common::rank_time_single* p_single = reply.add_rank_list();
				p_single->set_rank_id(p_rank_conf->client_id());
				p_single->set_start_time(start_time);
				p_single->set_end_time(end_time);
			}
		}
	}
}

void rank_manager_t::on_time_rank(uint32_t id, bool is_in_time)
{
	if (is_in_time)
	{
		TimeRank* p_conf = GET_CONF(TimeRank, id);
		if (NULL == p_conf)
		{
			log_error("NULL == p_conf TimeRank[%d]", id);
			return;
		}
		for (auto rank_id : p_conf->rank())
		{
			RankType* p_rank_conf = GET_CONF(RankType, rank_id);
			if (NULL == p_rank_conf)
			{
				continue;
			}
			if (1 == p_rank_conf->type())
			{
				rank_sort_ptr p_rank = get_rank_sort_by_type(rank_id);
				if (NULL != p_rank)
				{
					p_rank->set_open(true);
				}
			}
		}
	}
	else
	{
		TimeRank* p_conf = GET_CONF(TimeRank, id);
		if (NULL == p_conf)
		{
			log_error("NULL == p_conf TimeRank[%d]", id);
			return;
		}
		for (auto rank_id : p_conf->rank())
		{
			RankType* p_rank_conf = GET_CONF(RankType, rank_id);
			if (NULL == p_rank_conf)
			{
				continue;
			}
		
			rank_sort_ptr p_rank = get_rank_sort_by_type(rank_id);
			if (NULL != p_rank)
			{
				p_rank->set_open(false);
				p_rank->award();
				if (1 == p_rank_conf->type())
				{
					p_rank->clear_rank();
				}
			}
		}
	}
}

rank_level_ptr rank_manager_t::get_rank_level()
{
	rank_sort_map::const_iterator citr = m_rank_sort_map.find(common::rank_common::lv_rank_0);
	if (citr != m_rank_sort_map.end() && citr->second != NULL) {
		return boost::dynamic_pointer_cast<rank_level_t>(citr->second);
	}

	return NULL;
}

void rank_manager_t::update_rank_level(uint64_t uid, uint32_t lv)
{
	rank_level_ptr p_rank_level = get_rank_level();
	if (p_rank_level) {
		p_rank_level->update_level(uid, lv);
	}
}

rank_charm_ptr rank_manager_t::get_rank_charm()
{
	rank_sort_map::const_iterator citr = m_rank_sort_map.find(common::rank_common::charm_rank_0);
	if (citr != m_rank_sort_map.end() && citr->second != NULL) {
		return boost::dynamic_pointer_cast<rank_charm_t>(citr->second);
	}

	return NULL;
}

void rank_manager_t::update_rank_charm(uint64_t uid, uint32_t charm)
{
	rank_charm_ptr p_rank_charm = get_rank_charm();
	if (p_rank_charm) {
		p_rank_charm->update_charm(uid, charm);
	}
}


rank_power_ptr rank_manager_t::get_rank_power()
{
	rank_sort_map::const_iterator citr = m_rank_sort_map.find(common::rank_common::combat_power_rank_0);
	if (citr != m_rank_sort_map.end() && citr->second != NULL) {
		return boost::dynamic_pointer_cast<rank_power_t>(citr->second);
	}

	return NULL;
}


void rank_manager_t::update_rank_power(uint64_t uid, uint32_t power)
{
	rank_power_ptr p_rank_power = get_rank_power();
	if (p_rank_power) {
		p_rank_power->update_power(uid, power);
	}
}

rank_family_ptr rank_manager_t::get_rank_family()
{
	rank_sort_map::const_iterator citr = m_rank_sort_map.find(common::rank_common::family_rank_0);
	if (citr != m_rank_sort_map.end() && citr->second != NULL) {
		return boost::dynamic_pointer_cast<rank_family_t>(citr->second);
	}

	return NULL;
}

void rank_manager_t::update_rank_family(uint64_t family_id, uint32_t prestige)
{
	rank_family_ptr p_rank_family = get_rank_family();
	if (p_rank_family) {
		p_rank_family->update_prestige(family_id, prestige);
	}
}

void rank_manager_t::rank_del_family(uint64_t family_id)
{
	rank_family_ptr p_rank_family = get_rank_family();
	if (p_rank_family) {
		p_rank_family->del_family(family_id);
	}
}


