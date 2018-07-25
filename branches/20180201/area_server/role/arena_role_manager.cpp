#include "arena_role_manager.hpp"
#include "config/config_manager.hpp"
#include "tblh/Comprehensive.tbls.h"

arena_role_manager_t::ARENA_ROLE_MAP arena_role_manager_t::m_role_map;

void arena_role_manager_t::add_role(arena_role_ptr p_role)
{
	if (NULL == p_role)
	{
		log_error("arena_role_manager p_role NULL");
		return;
	}

	m_role_map.insert(std::make_pair(p_role->get_role_uid(), p_role));
}

arena_role_ptr arena_role_manager_t::add_role(uint64_t role_uid, uint32_t score, uint32_t arena_level, const proto::common::role_cross_data &cross_data)
{
	arena_role_ptr role = arena_role_ptr(new arena_role_t(role_uid));
	if (NULL == role)
	{
		log_error("new arena_role_ptr fail");
		return NULL;
	}
	role->set_score(score);
	role->set_arena_level(arena_level);
	role->set_cross_data(cross_data);
	add_role(role);
	return role;
}

arena_role_ptr arena_role_manager_t::find_role(uint64_t role_uid)
{
	ARENA_ROLE_MAP::iterator iter = m_role_map.find(role_uid);
	if (iter != m_role_map.end())
		return iter->second;
	return arena_role_ptr();
}

void arena_role_manager_t::clear_some_role()
{
	auto conf = GET_CONF(Comprehensive, comprehensive_common::arena_role_clear_time);
	uint32_t clear_time = GET_COMPREHENSIVE_VALUE_1(conf);

	ARENA_ROLE_MAP::iterator iter = m_role_map.begin();
	ARENA_ROLE_MAP::iterator iter_end = m_role_map.end();
	
	uint32_t now_time = time_util_t::now_time();
	// 清理3天没玩过的人
	arena_role_ptr p_role = arena_role_ptr();
	for (; iter != iter_end;)
	{
		p_role = iter->second;
		if (NULL == p_role)
		{
			log_error("arena p_role NULL");
			continue;
		}
		if (now_time - p_role->get_last_play_time() > (clear_time * 24 * 3600))
		{
			iter = m_role_map.erase(iter);
		}
		else
		{
			++iter;
		}
	}
}

