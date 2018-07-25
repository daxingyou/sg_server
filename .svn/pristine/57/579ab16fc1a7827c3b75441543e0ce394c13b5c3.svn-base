#include "time_rank.hpp"
#include "config/config_manager.hpp"
#include "role/role.hpp"
#include "role/role_manager.hpp"
#include "tblh/errcode_enum.hpp"
#include "common/time_manager.hpp"
#include "tblh/time_type_enum.hpp"
#include "tblh/TimeRank.tbls.h"
#include "red_point/red_point_manager.hpp"

time_rank_t::time_rank_t(uint64_t role_id) : m_role_uid( role_id )
{
	
}

void time_rank_t::on_login()
{
	if (is_active())
	{
		red_point_manager_t::red_point_notify(m_role_uid, proto::common::RED_POINT_TYPE::RED_POINT_TYPE_TIME_RANK, 1);
	}
}

//是否在活动中
bool time_rank_t::is_active()
{
	std::map<uint32_t, TimeRank*> conf_map;
	GET_ALL_CONF(TimeRank, conf_map);
	for (auto it : conf_map)
	{
		if (time_manager_t::is_in_time(it.first))
		{
			return true;
		}
	}
	return false;
}

//设置活动开始
void time_rank_t::set_active_start( uint32_t start_time )
{
	role_ptr p_role = role_manager_t::find_role(m_role_uid);
	if (NULL == p_role)
	{
		log_error("[%lu] p_role is NULL", m_role_uid);
		return;
	}

	if (m_start_time != start_time)
	{
		m_start_time = start_time;
	}
	business_activity_manager_t::add_icon_info(p_role, proto::common::BUSINESS_ACTIVE_TYPE::TIME_RANK, get_red_num(), start_time, 0 );
	red_point_manager_t::red_point_notify(m_role_uid, proto::common::RED_POINT_TYPE::RED_POINT_TYPE_TIME_RANK, 1);
}

//设置活动结束
void time_rank_t::set_active_stop()
{
	role_ptr p_role = role_manager_t::find_role(m_role_uid);
	if (NULL == p_role)
	{
		log_error("[%lu] p_role is NULL", m_role_uid);
		return;
	}

	if (!is_active())
	{
		business_activity_manager_t::del_icon_info(p_role, proto::common::BUSINESS_ACTIVE_TYPE::TIME_RANK);
	}
}
