#include "sevenday_target.hpp"
#include "config/config_manager.hpp"
#include "role/role.hpp"
#include "role/role_manager.hpp"
#include "tblh/errcode_enum.hpp"
#include "cache_key.hpp"
#include "item/drop_manager.hpp"
#include "role/role_unify_save.hpp"
#include "tblh/Comprehensive.tbls.h"
#include "role/money_manager.hpp"
#include "tblh/BAExchangeItem.tbls.h"
#include "item/item_manager.hpp"
#include "common/time_manager.hpp"
#include "tblh/SDTargetTable.tbls.h"
#include "red_point/red_point_manager.hpp"


sevenday_target_t::sevenday_target_t(uint64_t role_id) : m_role_uid(role_id), m_day(0)
{
	m_key = cache_key_t::create(m_role_uid, cache_name::sevenday_target);
}

uint32_t sevenday_target_t::get_sevenday_target_info(uint32_t day, proto::client::gc_get_sd_target_info_reply &reply)
{
	role_ptr p_role = role_manager_t::find_role(m_role_uid);
	if (NULL == p_role)
	{
		log_error("[%lu] p_role is NULL", m_role_uid);
		return errcode_enum::notice_unknown;
	}

	reply.set_day(day);
	reply.set_cur_day(m_day);

	std::map<uint32_t, SDTargetTable*> conf_map;
	GET_ALL_CONF(SDTargetTable, conf_map);
	for (auto it : conf_map)
	{
		SDTargetTable *p_conf = it.second;
		if (NULL == p_conf)
			continue;
		
		if ( p_conf->day() != day )
			continue;

		SDTargetCountTable *p_count_conf = GET_CONF(SDTargetCountTable, p_conf->target_id());
		if (NULL == p_count_conf)
			continue;

		uint32_t type = p_count_conf->get_type();
		uint32_t eventid = p_count_conf->get_eventid();
		uint32_t statistical_id = p_role->get_statistical_id(type, eventid);
		uint32_t count = p_role->get_statistical_count(statistical_id);

		//判断条件对应的数据是否OK
		proto::common::sd_target_info *p_info = reply.add_info();
		p_info->set_index( it.first );
		p_info->set_num( count );
		p_info->set_max_num( p_count_conf->get_num() );
		p_info->set_status( get_prize_status(it.first, p_count_conf, count ) );
		p_info->set_target_id( p_conf->target_id() );
	}
	return errcode_enum::error_ok;
}

uint32_t sevenday_target_t::get_sevenday_target_prize(uint32_t id, proto::common::role_change_data *rcd)
{
	role_ptr p_role = role_manager_t::find_role(m_role_uid);
	if (NULL == p_role)
	{
		log_error("[%lu] p_role is NULL", m_role_uid);
		return errcode_enum::notice_unknown;
	}

	SDTargetTable *p_conf =  GET_CONF(SDTargetTable, id);
	if (NULL == p_conf)
		return errcode_enum::notice_unknown;

	SDTargetCountTable *p_count_conf = GET_CONF(SDTargetCountTable, p_conf->target_id());
	if (NULL == p_count_conf)
		return errcode_enum::notice_unknown;

	uint32_t type = p_count_conf->get_type();
	uint32_t eventid = p_count_conf->get_eventid();
	uint32_t statistical_id = p_role->get_statistical_id(type, eventid);
	uint32_t count = p_role->get_statistical_count(statistical_id);

	if (get_prize_status(id, p_count_conf, count) != proto::common::PRIZE_STATUS::PRIZE_STATUS_CAN)
	{
		log_error("user[%lu] target[%u] not complate", m_role_uid, id);
		return errcode_enum::notice_unknown;
	}
	
	uint32_t drop_id = p_count_conf->drop_id();
	bool ret = drop_manager_t::drop(p_role, drop_id, log_enum::source_type_bussiness_sevenday_target, proto::common::drop_sys_type_sevenday_target, id, rcd);
	if (!ret)
	{
		log_error("user[%lu] [%u] SevenDayTarget drop[%u] faild", m_role_uid, id, drop_id);
		return errcode_enum::notice_unknown;
	}

	m_prize_map.insert(id);

	save_data();

	proto::client::gc_active_update_reply reply;
	reply.set_id(proto::common::BUSINESS_ACTIVE_TYPE::LOAD_OF_NMS);
	reply.set_red_num(get_red_num());
	p_role->send_msg_to_client(op_cmd::gc_active_update_reply, reply);

	return errcode_enum::error_ok;
}


//是否在活动中
bool sevenday_target_t::is_active()
{
	return is_get_all_prize() == false;
}

//是否领取了所有奖励
bool sevenday_target_t::is_get_all_prize()
{
	bool ret = true;
	std::map<uint32_t, SDTargetTable*> conf_map;
	GET_ALL_CONF(SDTargetTable, conf_map);
	for (auto it : conf_map)
	{
		SDTargetTable *p_conf = it.second;
		if (NULL == p_conf)
			continue;
		
		auto iter_f = m_prize_map.find(it.first);
		if (iter_f == m_prize_map.end())
		{
			ret = false;
			break;
		}
	}
	return ret;
}

//是否是结束时间了
bool sevenday_target_t::is_end_show_time()
{
	return is_get_all_prize();
}

//获取红点
uint32_t sevenday_target_t::get_red_num()
{
	role_ptr p_role = role_manager_t::find_role(m_role_uid);
	if (NULL == p_role)
	{
		log_error("[%lu] p_role is NULL", m_role_uid);
		return errcode_enum::notice_unknown;
	}

	uint32_t red_num = 0;
	std::map<uint32_t, SDTargetTable*> conf_map;
	GET_ALL_CONF(SDTargetTable, conf_map);
	for (auto it : conf_map)
	{
		SDTargetTable *p_conf = it.second;
		if (NULL == p_conf)
			continue;

		if (p_conf->day() > m_day)
			continue;

		SDTargetCountTable *p_count_conf = GET_CONF(SDTargetCountTable, p_conf->target_id());
		if (NULL == p_count_conf)
			continue;

		uint32_t type = p_count_conf->get_type();
		uint32_t eventid = p_count_conf->get_eventid();
		uint32_t statistical_id = p_role->get_statistical_id(type, eventid);
		uint32_t count = p_role->get_statistical_count(statistical_id);

		if (get_prize_status(it.first, p_count_conf, count) == proto::common::PRIZE_STATUS::PRIZE_STATUS_CAN)
		{
			red_num += 1;
			break;
		}
	}
	return red_num;
}

bool sevenday_target_t::save_data()
{
	proto::common::sd_target_save_data data;
	peek_data(data);
	role_unify_save::add_task(m_role_uid, m_key, data);
	return true;
}

bool sevenday_target_t::peek_data(proto::common::sd_target_save_data &data)
{
	data.set_day(m_day);
	for( auto it : m_prize_map )
	{ 
		data.add_info(it);
	}
	return true;
}

bool sevenday_target_t::load_data(const proto::common::sd_target_save_data &data)
{
	m_day = data.day();
	m_prize_map.clear();

	for (int i = 0; i < data.info_size(); ++i)
	{
		m_prize_map.insert(data.info(i));
	}

	if (m_day == 0)
		m_day = 1;
	return true;
}

uint32_t sevenday_target_t::get_prize_status(uint32_t id, SDTargetCountTable *p_conf, uint32_t count)
{
	if( p_conf == NULL )
		return proto::common::PRIZE_STATUS::PRIZE_STATUS_NONE; 

	auto iter_f = m_prize_map.find(id);
	if (iter_f != m_prize_map.end())
		return proto::common::PRIZE_STATUS::PRIZE_STATUS_HAS; 
	
	if( p_conf->get_num() > count )
		return proto::common::PRIZE_STATUS::PRIZE_STATUS_NONE; 
	
	return proto::common::PRIZE_STATUS::PRIZE_STATUS_CAN; 
}


uint32_t sevenday_target_t::get_sevenday_target_red_num(proto::client::gc_get_sd_target_red_reply &reply)
{
	role_ptr p_role = role_manager_t::find_role(m_role_uid);
	if (NULL == p_role)
	{
		log_error("[%lu] p_role is NULL", m_role_uid);
		return errcode_enum::notice_unknown;
	}
	
	reply.set_cur_day(m_day);
	std::map<uint32_t, uint32_t> red_map;
	std::map<uint32_t, SDTargetTable*> conf_map;
	GET_ALL_CONF(SDTargetTable, conf_map);
	for (auto it : conf_map)
	{
		SDTargetTable *p_conf = it.second;
		if (NULL == p_conf)
			continue;

		SDTargetCountTable *p_count_conf = GET_CONF(SDTargetCountTable, p_conf->target_id());
		if (NULL == p_count_conf)
			continue;

		uint32_t type = p_count_conf->get_type();
		uint32_t eventid = p_count_conf->get_eventid();
		uint32_t statistical_id = p_role->get_statistical_id(type, eventid);
		uint32_t count = p_role->get_statistical_count(statistical_id);

		if (get_prize_status(it.first, p_count_conf, count) == proto::common::PRIZE_STATUS::PRIZE_STATUS_CAN)
		{
			red_map[p_conf->day()] += 1;
		}
	}

	for( auto it : red_map )
	{
		proto::common::sd_target_red_info *p_info = reply.add_info();
		p_info->set_day(it.first);
		p_info->set_red(it.second);
	}
	return errcode_enum::error_ok;
}


void sevenday_target_t::check_red_num()
{
	uint32_t red_num = get_red_num();

	if (red_num > m_last_red_num) {
		m_last_red_num = red_num;	//用于优化七日登录的检测与消息通知

		role_ptr p_role = role_manager_t::find_role(m_role_uid);
		if (NULL == p_role)
			return;

		proto::client::gc_active_update_reply reply;
		reply.set_id(proto::common::BUSINESS_ACTIVE_TYPE::LOAD_OF_NMS);
		reply.set_red_num(red_num);
		p_role->send_msg_to_client(op_cmd::gc_active_update_reply, reply);

		red_point_manager_t::red_point_notify(m_role_uid, proto::common::RED_POINT_TYPE::RED_POINT_TYPE_FULI, 1);
	}
}

void sevenday_target_t::one_day()
{ 
	m_day += 1; 
	save_data(); 
	check_red_num();
}


