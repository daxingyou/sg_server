#include "sevenday_login.hpp"
#include "tblh/SevendayLogin.tbls.h"
#include "tblh/errcode_enum.hpp"
#include "cache_key.hpp"
#include "tblh/errcode_enum.hpp"
#include "role/role_unify_save.hpp"
#include "role/role_manager.hpp"
#include "config/config_manager.hpp"
#include "role/money_manager.hpp"
#include "item/item_manager.hpp"
#include "item/drop_manager.hpp"
#include "common/log_enum.hpp"
#include "red_point/red_point_manager.hpp"

USING_NS_COMMON;

sevenday_login_t::sevenday_login_t(uint64_t role_id) :m_role_uid(role_id) 
{
	m_key = cache_key_t::create(m_role_uid, cache_name::sevenday_login);
}

uint32_t sevenday_login_t::get_sevenday_list_info(proto::client::gc_get_7d_login_reply &reply) 
{
	std::map<uint32_t, SevendayLogin*> sevenday_conf_map;
	GET_ALL_CONF(SevendayLogin, sevenday_conf_map);

	for (auto it : sevenday_conf_map )
	{
		SevendayLogin *p_config = it.second;
		if (NULL == p_config)
		{
			log_error("user:[%lu] SevendayLogin_data is NULL", m_role_uid);
			return errcode_enum::notice_unknown;
		}
		
		proto::common::sevenday_login_data_info *p_prize_info = reply.add_prize_info();

		p_prize_info->set_day(it.first);
		p_prize_info->set_day_show_id(p_config->day_show_id());
		p_prize_info->set_show_id_list(p_config->show_id_list());
		p_prize_info->set_status(get_prize_status(it.first));
	}

	return errcode_enum::error_ok;
}

uint32_t sevenday_login_t::get_sevenday_list_prize(uint32_t index, proto::client::gc_get_7d_login_prize_reply &reply)
{
	if (get_prize_status(index) != 1)
	{
		log_error("uer:[%lu] get SevendayPrize[%d] is not complated", m_role_uid, index);
		return errcode_enum::notice_unknown;
	}

	SevendayLogin *p_conf = GET_CONF(SevendayLogin, index);
	if (p_conf == NULL)
	{
		log_error("uer:[%lu] get SevendayLogin Config [%d] is NULL", m_role_uid, index);
		return errcode_enum::notice_unknown;
	}

	auto p_role = role_manager_t::find_role(m_role_uid);
	if (NULL == p_role)
	{
		log_error("user[%lu] is NULL", m_role_uid);
		return false;
	}

	drop_manager_t::drop(p_role, p_conf->drop_id(), log_enum::source_type_bussiness_seven_day_login, proto::common::drop_sys_type_trade, index, reply.mutable_rcd());

	m_sevenday_list.insert(index);

	save_data();
	return errcode_enum::error_ok;
}

void  sevenday_login_t::on_login()
{
	if (get_red_num() > 0 )
	{
		red_point_manager_t::red_point_notify(m_role_uid, proto::common::RED_POINT_TYPE::RED_POINT_TYPE_FULI, 1);
	}
}

uint32_t sevenday_login_t::get_red_num()
{
	std::map<uint32_t, SevendayLogin*> sevenday_conf_map;
	GET_ALL_CONF(SevendayLogin, sevenday_conf_map);

	uint32_t count = 0;
	for (auto it : sevenday_conf_map)
	{
		if (get_prize_status(it.first) == 1)
		{
			count += 1;
		}
	} 
	return count;
}


uint32_t sevenday_login_t::get_prize_status(uint32_t day)
{
	if ( day > m_day )
	{
		return 0;
	}
	else
	{
		auto iter_f = m_sevenday_list.find(day);
		if (iter_f == m_sevenday_list.end())
			return 1;
		else 
			return 2;
	}
	return 0;
}

void sevenday_login_t::load_data(const proto::common::sevenday_login_save_data& data)
{
	m_day = data.day();
	m_login_flag = data.login_flag();
	m_sevenday_list.clear();
	for (int32_t i = 0; i < data.days_size(); ++i)
	{
		m_sevenday_list.insert(data.days(i));
	}

	if (m_day == 0)
	{
		m_day = 1;
		save_data();
	}
}

void sevenday_login_t::peek_data(proto::common::sevenday_login_save_data &data)
{
	data.set_day(m_day);
	data.set_login_flag(m_login_flag);
	for (auto it : m_sevenday_list)
	{
		data.add_days(it);
	}
}

void sevenday_login_t::save_data()
{
	proto::common::sevenday_login_save_data p_data;
	peek_data(p_data);
	role_unify_save::add_task(m_key, p_data, false, false, m_role_uid);
}


void sevenday_login_t::oneday()
{
	m_login_flag = 0;
	m_day++;
	save_data();
}

void sevenday_login_t::init_newbie()
{
	m_day = 1;
	m_login_flag = 0;
	save_data();
}
