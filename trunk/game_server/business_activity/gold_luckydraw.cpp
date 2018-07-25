#include "gold_luckydraw.hpp"
#include "config/config_manager.hpp"
#include "role/role.hpp"
#include "role/role_manager.hpp"
#include "tblh/errcode_enum.hpp"
#include "cache_key.hpp"
#include "item/drop_manager.hpp"
#include "role/role_unify_save.hpp"
#include "tblh/Comprehensive.tbls.h"
#include "tblh/GoldLuckydraw.tbls.h"
#include "common/time_manager.hpp"
#include "red_point/red_point_manager.hpp"
#include "tblh/time_type_enum.hpp"

gold_luckydraw_t::gold_luckydraw_t(uint64_t role_id) : m_role_uid( role_id ), m_start_time(0)
{
	m_key = cache_key_t::create(m_role_uid, cache_name::gold_luckydraw);
}

uint32_t gold_luckydraw_t::on_login()
{
	if (!is_in_time())
	{
		clear_lucky();
	}
	return 0;
}

uint32_t gold_luckydraw_t::award(uint32_t id)
{
	role_ptr p_role = role_manager_t::find_role(m_role_uid);
	if (NULL == p_role)
	{
		log_error("[%lu] p_role is NULL", m_role_uid);
		return errcode_enum::notice_unknown;
	}
	auto it = std::find(m_award_vec.begin(), m_award_vec.end(), id);
	if (it != m_award_vec.end())
	{
		log_error("role[%lu] gold_luckydraw[%d] already award", m_role_uid, id);
		return errcode_enum::notice_unknown;
	}
	auto p_conf = GET_CONF(GoldLuckydraw, id);
	if (NULL == p_conf)
	{
		log_error("NULL == p_conf GoldLuckydraw[%d]", id);
		return errcode_enum::notice_unknown;
	}
	if (!time_manager_t::is_in_time(id))
	{
		log_error("role[%lu] gold_luckydraw[%d] award failed, not opened", m_role_uid, id);
		return errcode_enum::notice_unknown;
	}
	luckydraw_ptr p_luckydraw = p_role->get_luckydraw();
	if (NULL == p_luckydraw)
	{
		log_error("[%lu] p_role is NULL", m_role_uid);
		return errcode_enum::notice_unknown;
	}
	p_luckydraw->add_lucky_ex(p_conf->award());
	p_luckydraw->save_self();
	m_award_vec.push_back(id);
	save_data();
	return errcode_enum::error_ok;
}

//是否在活动中
bool gold_luckydraw_t::is_active()
{
	return true;
}

//是否领取了所有奖励
bool gold_luckydraw_t::is_get_all_prize()
{
	return false;
}

//是否是结束时间了
bool gold_luckydraw_t::is_end_show_time()
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
uint32_t gold_luckydraw_t::get_active_start_time()
{
	return 0;
}

//获取活动结束时间,放在模块做，方便扩展
uint32_t gold_luckydraw_t::get_active_end_time()
{
	return 0;
}

//设置活动开始
void gold_luckydraw_t::set_active_start( uint32_t start_time )
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
	business_activity_manager_t::add_icon_info(p_role, proto::common::BUSINESS_ACTIVE_TYPE::GOLD_LUCKYDRAW, get_red_num(), start_time, 0 );
}

//设置活动结束
void gold_luckydraw_t::set_active_stop()
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
uint32_t gold_luckydraw_t::get_red_num()
{
	std::map<uint32_t, GoldLuckydraw*> conf_map;
	GET_ALL_CONF(GoldLuckydraw, conf_map);
	for (auto it : conf_map)
	{
		if (time_manager_t::is_in_time(it.first))
		{
			if (std::find(m_award_vec.begin(), m_award_vec.end(), it.first) == m_award_vec.end())
			{
				return 1;
			}
		}
	}
	return 0;
}

bool gold_luckydraw_t::save_data()
{
	proto::common::gold_luckydraw_data data;
	peek_data(data);
	role_unify_save::add_task(m_role_uid, m_key, data);
	return true;
}

bool gold_luckydraw_t::peek_data(proto::common::gold_luckydraw_data &data)
{	
	for (auto id : m_award_vec)
	{
		data.add_gold_luckydraw_id(id);
	}
	return true;
}

bool gold_luckydraw_t::load_data(const proto::common::gold_luckydraw_data &data)
{
	m_award_vec.clear();
	for (auto id : data.gold_luckydraw_id())
	{
		m_award_vec.push_back(id);
	}
	return true;
}

void gold_luckydraw_t::peek_client_data(proto::common::gold_luckydraw_client_data &data)
{
	role_ptr p_role = role_manager_t::find_role(m_role_uid);
	if (NULL == p_role)
	{
		log_error("[%lu] p_role is NULL", m_role_uid);
		return;
	}
	
	std::map<uint32_t, GoldLuckydraw*> conf_map;
	GET_ALL_CONF(GoldLuckydraw, conf_map);
	for (auto it : conf_map)
	{
		GoldLuckydraw *p_conf = it.second;
		if (NULL == p_conf)
			continue;
		proto::common::gold_luckydraw_client_single* p_single = data.add_gold_luckydraw_data();
		if (std::find(m_award_vec.begin(), m_award_vec.end(), it.first) == m_award_vec.end())
		{
			if (time_manager_t::is_in_time(it.first))
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
		p_single->set_gold_luckydraw_id(it.first);
		p_single->set_display(p_conf->display());
		p_single->set_desc(p_conf->desc());
		p_single->set_day(p_conf->day());
	}
}


//0点跨日如果发现，奖励已经领完则关闭此功能
void gold_luckydraw_t::one_day() 
{
	//if(is_get_all_prize() )
	//{
	//	role_ptr p_role = role_manager_t::find_role(m_role_uid);
	//	if (NULL == p_role) {
	//		log_error("role[%lu] is null", m_role_uid);
	//		return;
	//	}
	//	p_role->close_function(proto::common::function_code_type_rechargetotal);
	//}
}

void gold_luckydraw_t::clear_lucky()
{
	role_ptr p_role = role_manager_t::find_role(m_role_uid);
	if (NULL == p_role)
	{
		log_error("[%lu] p_role is NULL", m_role_uid);
		return;
	}
	luckydraw_ptr p_luckydraw = p_role->get_luckydraw();
	if (NULL == p_luckydraw)
	{
		log_error("[%lu] p_role is NULL", m_role_uid);
		return;
	}
	p_luckydraw->clear_lucky_ex();
	p_luckydraw->save_self();
}

uint32_t gold_luckydraw_t::get_expired_time()
{
	std::map<uint32_t, GoldLuckydraw*> conf_map;
	GET_ALL_CONF(GoldLuckydraw, conf_map);
	for (auto it : conf_map)
	{
		if (time_manager_t::is_in_time(it.first))
		{
			return time_manager_t::get_timer_end_time(it.first) - time_util_t::now_time();
		}
	}
	return 0;
}

bool gold_luckydraw_t::is_in_time()
{
	std::map<uint32_t, GoldLuckydraw*> conf_map;
	GET_ALL_CONF(GoldLuckydraw, conf_map);
	for (auto it : conf_map)
	{
		if (time_manager_t::is_in_time(it.first))
		{
			return true;
		}
	}
	return false;
}

