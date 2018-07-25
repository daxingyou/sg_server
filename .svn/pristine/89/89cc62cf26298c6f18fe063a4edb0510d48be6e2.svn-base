#include "recharge_total.hpp"
#include "config/config_manager.hpp"
#include "role/role.hpp"
#include "role/role_manager.hpp"
#include "tblh/errcode_enum.hpp"
#include "cache_key.hpp"
#include "item/drop_manager.hpp"
#include "role/role_unify_save.hpp"
#include "tblh/Comprehensive.tbls.h"
#include "tblh/RechargeTotal.tbls.h"
#include "common/time_manager.hpp"
#include "red_point/red_point_manager.hpp"

recharge_total_t::recharge_total_t(uint64_t role_id) : m_role_uid( role_id ), m_start_time(0)
{
	m_key = cache_key_t::create(m_role_uid, cache_name::recharge_total);
}

uint32_t recharge_total_t::on_login()
{
	uint32_t red_num = get_red_num();
	role_ptr p_role = role_manager_t::find_role(m_role_uid);
	if (NULL != p_role)
	{
		recharge_ptr p_recharge = p_role->get_recharge();
		if (NULL != p_recharge)
		{
			red_num += p_recharge->get_red_num();
		}
	}
	if (red_num > 0)
	{
		red_point_manager_t::red_point_notify(m_role_uid, proto::common::RED_POINT_TYPE::RED_POINT_TYPE_RECHARGE_TOTAL, red_num);
	}
	return 0;
}

uint32_t recharge_total_t::award(uint32_t id, proto::common::role_change_data* p_data)
{
	role_ptr p_role = role_manager_t::find_role(m_role_uid);
	if (NULL == p_role)
	{
		log_error("[%lu] p_role is NULL", m_role_uid);
		return errcode_enum::notice_unknown;
	}
	recharge_ptr p_recharge = p_role->get_recharge();
	if (NULL == p_recharge)
	{
		log_error("[%lu] p_role is NULL", m_role_uid);
		return errcode_enum::notice_unknown;
	}
	if (p_recharge->get_total_cny() < id)
	{
		log_error("role[%lu] recharge_total[%d] not enough[%d]", m_role_uid, p_recharge->get_total_cny(), id);
		return errcode_enum::notice_unknown;
	}
	auto it = std::find(m_recharge_total_vec.begin(), m_recharge_total_vec.end(), id);
	if (it != m_recharge_total_vec.end())
	{
		log_error("role[%lu] recharge_total[%d] already award", m_role_uid, id);
		return errcode_enum::notice_unknown;
	}
	auto p_conf = GET_CONF(RechargeTotal, id);
	if (NULL == p_conf)
	{
		log_error("NULL == p_conf RechargeTotal[%d]", id);
		return errcode_enum::notice_unknown;
	}
	drop_manager_t::drop(p_role, p_conf->award(), log_enum::source_type_recharge_gift, proto::common::drop_sys_type_recharge_gift, id, p_data);
	m_recharge_total_vec.push_back(id);
	save_data();
	return errcode_enum::error_ok;
}

//是否在活动中
bool recharge_total_t::is_active()
{
	return true;
}

//是否领取了所有奖励
bool recharge_total_t::is_get_all_prize()
{
	std::map<uint32_t, RechargeTotal*> conf_map;
	GET_ALL_CONF(RechargeTotal, conf_map);
	if (m_recharge_total_vec.size() == conf_map.size())
		return true;

	return false;
}

//是否是结束时间了
bool recharge_total_t::is_end_show_time()
{
	role_ptr p_role = role_manager_t::find_role(m_role_uid);
	if (NULL == p_role)
	{
		log_error("[%lu] p_role is NULL", m_role_uid);
		return true;
	}

	BusinessActiveTable *p_conf = GET_CONF(BusinessActiveTable, proto::common::BUSINESS_ACTIVE_TYPE::RECHARGE_TOTAL);
	if (NULL == p_conf)
		return true;

	return business_activity_manager_t::check_end_cond(p_role, p_conf, this) == false;
}

//获取活动开始时间, 如果是0表示未参加过，也不需要显示
uint32_t recharge_total_t::get_active_start_time()
{
	BusinessActiveTable *p_conf = GET_CONF(BusinessActiveTable, proto::common::BUSINESS_ACTIVE_TYPE::RECHARGE_TOTAL);
	if (NULL == p_conf)
		return false;

	return time_manager_t::get_timer_start_time(p_conf->time_id());
}

//获取活动结束时间,放在模块做，方便扩展
uint32_t recharge_total_t::get_active_end_time()
{
	BusinessActiveTable *p_conf = GET_CONF(BusinessActiveTable, proto::common::BUSINESS_ACTIVE_TYPE::RECHARGE_TOTAL);
	if (NULL == p_conf)
		return false;

	return time_manager_t::get_timer_end_time(p_conf->time_id());
}

//设置活动开始
void recharge_total_t::set_active_start( uint32_t start_time )
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
		save_data();
	}
	business_activity_manager_t::add_icon_info(p_role, proto::common::BUSINESS_ACTIVE_TYPE::RECHARGE_TOTAL, get_red_num(), start_time, 0 );
}

//设置活动结束
void recharge_total_t::set_active_stop()
{
	role_ptr p_role = role_manager_t::find_role(m_role_uid);
	if (NULL == p_role)
	{
		log_error("[%lu] p_role is NULL", m_role_uid);
		return;
	}

	if (is_end_show_time())
	{
		business_activity_manager_t::del_icon_info(p_role, proto::common::BUSINESS_ACTIVE_TYPE::RECHARGE_TOTAL);
	}
}

//获取红点
uint32_t recharge_total_t::get_red_num()
{
	role_ptr p_role = role_manager_t::find_role(m_role_uid);
	if (NULL == p_role)
	{
		log_error("[%lu] p_role is NULL", m_role_uid);
		return errcode_enum::notice_unknown;
	}
	recharge_ptr p_recharge = p_role->get_recharge();
	if (NULL == p_recharge)
	{
		log_error("[%lu] p_role is NULL", m_role_uid);
		return errcode_enum::notice_unknown;
	}
	uint32_t count = 0;
	std::map<uint32_t, RechargeTotal*> conf_map;
	GET_ALL_CONF(RechargeTotal, conf_map);
	for (auto it : conf_map)
	{
		RechargeTotal *p_conf = it.second;
		if (NULL == p_conf)
			continue;

		if (p_recharge->get_total_cny() >= it.first)
		{
			count += 1;
		}
	}
	return count - m_recharge_total_vec.size();
}

bool recharge_total_t::save_data()
{
	proto::common::recharge_total_data data;
	peek_data(data);
	role_unify_save::add_task(m_role_uid, m_key, data);
	return true;
}

bool recharge_total_t::peek_data(proto::common::recharge_total_data &data)
{	
	for (auto id : m_recharge_total_vec)
	{
		data.add_recharge_total_id(id);
	}
	return true;
}

bool recharge_total_t::load_data(const proto::common::recharge_total_data &data)
{
	m_recharge_total_vec.clear();
	for (auto id : data.recharge_total_id())
	{
		m_recharge_total_vec.push_back(id);
	}
	return true;
}

void recharge_total_t::peek_client_data(proto::common::recharge_total_client_data &data)
{
	role_ptr p_role = role_manager_t::find_role(m_role_uid);
	if (NULL == p_role)
	{
		log_error("[%lu] p_role is NULL", m_role_uid);
		return;
	}
	recharge_ptr p_recharge = p_role->get_recharge();
	if (NULL == p_recharge)
	{
		log_error("[%lu] p_role is NULL", m_role_uid);
		return;
	}
	data.set_recharge_total(p_recharge->get_total_cny());
	std::map<uint32_t, RechargeTotal*> conf_map;
	GET_ALL_CONF(RechargeTotal, conf_map);
	for (auto it : conf_map)
	{
		RechargeTotal *p_conf = it.second;
		if (NULL == p_conf)
			continue;
		proto::common::recharge_total_client_single* p_single = data.add_info();
		if (std::find(m_recharge_total_vec.begin(), m_recharge_total_vec.end(), it.first) == m_recharge_total_vec.end())
		{
			if (p_recharge->get_total_cny() >= it.first)
			{
				p_single->set_state(1);
			}
			else
			{
				p_single->set_state(0);
			}
		}
		else
		{
			p_single->set_state(2);
		}
		p_single->set_recharge_total_id(it.first);
		p_single->set_display(p_conf->display());
	}
}


//0点跨日如果发现，奖励已经领完则关闭此功能
void recharge_total_t::one_day() {
	if(is_get_all_prize() ){
		role_ptr p_role = role_manager_t::find_role(m_role_uid);
		if (NULL == p_role) {
			log_error("role[%lu] is null", m_role_uid);
			return;
		}
		p_role->close_function(proto::common::function_code_type_rechargetotal);
	}
}

