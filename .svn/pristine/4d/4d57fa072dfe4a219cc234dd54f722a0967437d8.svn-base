#include "multi_time_manager.hpp"
#include "tblh/Comprehensive.tbls.h"
#include "common/config_mgr.h"
#include "role/role_manager.hpp"
#include "role/role.hpp"
#include "role/money_manager.hpp"
#include "tblh/errcode_enum.hpp"
#include "common/ContainerParse.h"
USING_NS_COMMON;

std::vector<Dawn::triple<uint32_t, uint32_t, uint32_t>> multi_time_manager_t::m_add_multi_time_conf_list;
bool multi_time_manager_t::init()
{
	Comprehensive* p_timedouble = GET_CONF(Comprehensive, comprehensive_common::timedouble);
	if (NULL == p_timedouble)
	{
		log_error("NULL == p_timedouble timedouble");
		return false;
	}

	Dawn::parseTupleContainer<Dawn::triple<uint32_t, uint32_t, uint32_t>>(m_add_multi_time_conf_list, p_timedouble->parameter1().c_str(), ":$");
	if (m_add_multi_time_conf_list.empty())
	{
		log_error("add_multi_time_conf_list empty error");
		return false;
	}

	return true;
}

uint32_t multi_time_manager_t::get_multi_time(const uint64_t uid,uint32_t type)
{
	role_ptr p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role role[%lu]",uid);
		return 0;
	}

	const std::map<uint32_t, uint32_t>& list = p_role->get_multi_time_map();
	std::map<uint32_t, uint32_t>::const_iterator iter = list.find(type);
	if (iter != list.end())
	{
		return iter->second;
	}

	return 0;
}

uint32_t multi_time_manager_t::get_max_multi_type(const uint64_t uid)
{
	role_ptr p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role role[%lu]", uid);
		return 0;
	}

	update_multi_list(uid);
	const std::map<uint32_t, uint32_t>& list = p_role->get_multi_time_map();
	uint32_t type = 0;
	for (auto& it : list)
	{
		if (it.first > type)
		{
			type = it.first;
		}
	}

	return type;
}


uint32_t multi_time_manager_t::change_multi_state(const uint64_t uid,uint32_t type, proto::common::role_change_data* p_data)
{
	uint32_t reply_code = errcode_enum::error_ok;

	do
	{
		role_ptr p_role = role_manager_t::find_role(uid);
		if (NULL == p_role)
		{
			log_error("NULL == p_role role[%lu]", uid);
			reply_code = errcode_enum::notice_unknown;
			break;
		}

		if (NULL == p_data)
		{
			log_error("NULL == p_data role[%lu]", uid);
			reply_code = errcode_enum::notice_unknown;
			break;
		}
		p_role->get_personal_info(p_data->mutable_per_info());
		proto::common::multi_time_data* p_multi_data = p_data->mutable_multi_time();
		get_multi_time_list(uid, p_multi_data);
		bool cur_multi_state = p_role->get_multi_state();
		uint32_t multi_point = money_manager_t::get_money(p_role, proto::common::MONEY_TYPE_MULTI_POINT);

		if (MULTI_STATE::multi_open == type)
		{	
			if (cur_multi_state)
			{
				p_multi_data->set_state(1);
				break;
			}

			if (0 < multi_point)
			{
				p_role->set_multi_state(true);
				p_multi_data->set_state(1);
				break;
			}
			else
			{
				reply_code = errcode_enum::open_multi_state_fail;
				p_multi_data->set_state(0);
				break;
			}
		} 
		else 
		{	
			if (!cur_multi_state)
			{
				p_multi_data->set_state(0);
				break;
			}
			else
			{
				p_role->set_multi_state(false);
				p_multi_data->set_state(0);
				break;
			}
		}
	} while (false);

	return reply_code;
}


void multi_time_manager_t::update_multi_list(const uint64_t uid)
{
	role_ptr p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role role[%lu]", uid);
		return;
	}
	std::map<uint32_t, uint32_t>& list = p_role->get_multi_time_map();
	const uint32_t cur_time = common::time_util_t::now_time();

	for (std::map<uint32_t, uint32_t>::iterator iter = list.begin(); iter != list.end();)
	{
		if (iter->second <= cur_time)
		{
			MULTI_LOG("role[%lu] multi_time type[%d] end_time[%d] time over del",uid,iter->first, iter->second);
			iter = list.erase(iter);
		}
		else
		{
			++iter;
		}
	}
}

uint32_t multi_time_manager_t::push_to_multi_list(const uint64_t uid, const uint32_t item_id, proto::common::role_change_data* p_data /*= NULL*/)
{
	role_ptr p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role role[%lu]", uid);
		return errcode_enum::notice_unknown;
	}
	update_multi_list(uid);
	std::map<uint32_t, uint32_t>& list = p_role->get_multi_time_map();
	uint32_t add_type = 0, add_time = 0;
	for (auto& it : m_add_multi_time_conf_list)
	{
		if (it.first == item_id)
		{
			add_type = it.second;
			add_time = it.third;
		}
	}

	if (0 == add_time || 0 == add_type)
	{
		return errcode_enum::notice_config_error;
	}

	const uint32_t cur_time = common::time_util_t::now_time();
	std::map<uint32_t, uint32_t>::iterator iter = list.find(add_type);
	if (iter != list.end())
	{
		const uint32_t old_time = iter->second;
		list[add_type] = old_time + add_time;
		MULTI_LOG("role[%lu] add multi_type[%d] end_time[%d]", uid, add_type, old_time + add_time);
	}
	else if (get_multi_point_times() > list.size())
	{
		list.insert(std::make_pair(add_type, cur_time + add_time));
		MULTI_LOG("role[%lu] add multi_type[%d] end_time[%d]", uid, add_type, cur_time + add_time);
	}

	if (NULL != p_data)
	{
		proto::common::multi_time_data* p_multi_time = p_data->mutable_multi_time();
		for (auto& it : list)
		{
			proto::common::public_struct* p_public = p_multi_time->add_time_long_list();
			p_public->set_type(it.first);
			p_public->set_param(it.second);
		}
		p_role->get_personal_info(p_data->mutable_per_info());
		p_multi_time->set_state(p_role->get_multi_state());
	}
	
	return errcode_enum::error_ok;
}

bool multi_time_manager_t::is_exist_add_multi_time(const uint32_t item_id)
{
	for (auto& it : m_add_multi_time_conf_list)
	{
		if (it.first == item_id)
		{
			return true;
		}
	}

	return false;
}

uint32_t multi_time_manager_t::multi_exp_buff(const uint64_t uid, proto::common::role_change_data* p_data)
{
	role_ptr p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role role[%lu]", uid);
		return 0;
	}

	uint32_t time_long_multi = get_max_multi_type(uid);
	uint32_t point_multi = 0;
	if (p_role->get_multi_state())
	{
		point_multi = get_multi_point_times();
	}
	uint32_t multi_add = 0;
	multi_add = time_long_multi + point_multi;

	if (NULL != p_data)
	{
		if (0 != multi_add && p_role->get_multi_state())
		{
			money_manager_t::use_money(p_role, proto::common::MONEY_TYPE_MULTI_POINT, 1, log_enum::source_type_monster_fight_pve, 0, true, p_data);
			uint32_t multi_point = money_manager_t::get_money(p_role, proto::common::MONEY_TYPE_MULTI_POINT);
			if (0 == multi_point)
			{
				p_role->set_multi_state(false);
				p_data->mutable_multi_time()->set_state(0);
				get_multi_time_list(uid, p_data->mutable_multi_time());
			}
		}
	}
	
	return multi_add;
}

void multi_time_manager_t::get_multi_time_list(const uint64_t uid, proto::common::multi_time_data* p_data /*= NULL*/)
{
	role_ptr p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role role[%lu]", uid);
		return;
	}

	std::map<uint32_t, uint32_t>& list = p_role->get_multi_time_map();

	if (NULL != p_data)
	{
		for (auto& it : list)
		{
			proto::common::public_struct* p_public = p_data->add_time_long_list();
			p_public->set_type(it.first);
			p_public->set_param(it.second);
		}
	}
}

uint32_t multi_time_manager_t::get_multi_point_times()
{
	auto countdouble_multiple = GET_CONF(Comprehensive, comprehensive_common::countdouble_multiple);
	uint32_t val = GET_COMPREHENSIVE_VALUE_1(countdouble_multiple);
	return val;
}

uint32_t multi_time_manager_t::get_multi_time_max_upper()
{
	auto timedouble_max = GET_CONF(Comprehensive, comprehensive_common::timedouble_max);
	uint32_t val = GET_COMPREHENSIVE_VALUE_1(timedouble_max);
	return val;
}
