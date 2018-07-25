#include "general_event_manager.hpp"
#include "config/config_manager.hpp"
#include "role/role.hpp"
#include "role/role_manager.hpp"
#include "tblh/errcode_enum.hpp"
#include "cache_key.hpp"
#include "item/drop_manager.hpp"
#include "role/role_unify_save.hpp"
#include "tblh/Comprehensive.tbls.h"
#include "role/money_manager.hpp"
#include "tblh/GeneralEventTable.tbls.h"
#include "tblh/GeneralEventTargetTable.tbls.h"
#include "general_info_manager.hpp"
#include "red_point/red_point_manager.hpp"
#include "role_buff/role_buff_manager.hpp"
#include "tblh/LevelAddExpRateTable.tbls.h"


general_event_manager_t::general_event_manager_t(uint64_t role_uid):m_role_uid( role_uid )
{
	m_key = cache_name::general_event;
}

void general_event_manager_t::load_data(const proto::common::general_event_save_data &data)
{
	m_prize_set.clear();
	for (int i = 0; i < data.data_size(); ++i)
	{
		m_prize_set.insert(data.data(i));
	}

	is_notify_red_list.clear();
	for (int i = 0; i < data.red_size(); ++i)
	{
		is_notify_red_list.insert(data.red(i));
	}
}

void general_event_manager_t::peek_data(proto::common::general_event_save_data &data)
{
	for (auto it : m_prize_set)
	{
		data.add_data(it);
	}

	for( auto it : is_notify_red_list )
	{
		data.add_red(it);
	}
}

void general_event_manager_t::save_data()
{
	proto::common::general_event_save_data data;
	peek_data(data);
	role_unify_save::add_task(m_role_uid, m_key, data);
}

uint32_t general_event_manager_t::get_general_event_info( role_ptr p_role, proto::client::gc_get_general_event_info_reply &reply)
{
	if (p_role == NULL)
	{
		log_error("p_role is NULL");
		return errcode_enum::notice_unknown;
	}

	uint32_t red_count = 0;
	general_event_manager_t &general_event = p_role->get_general_event();
	std::map<uint32_t, GeneralEventTable*> conf_map;
	GET_ALL_CONF(GeneralEventTable, conf_map);
	for (auto it : conf_map)
	{
		GeneralEventTable *p_conf = it.second;
		if (!p_conf)
			continue;
		
		if (p_conf->is_open() == 0)
			continue;

		proto::common::general_event_info *p_info = reply.add_info();
		p_info->set_id(it.first);
		p_info->set_process( general_event.get_general_event_process(it.first) );
		p_info->set_status(general_event.get_general_event_status(it.first) );
		p_info->set_time_left(general_event.get_general_event_time_left(it.first));
		p_info->set_first_name(general_event.get_general_event_first_name(it.first));
		uint32_t count = general_event.get_red_num(it.first);
		p_info->set_red_num(count);
		red_count += count;
	}
	reply.set_red_num(red_count);

	return errcode_enum::error_ok;
}

uint32_t general_event_manager_t::get_general_event_dtail_info( role_ptr p_role, uint32_t id, proto::client::gc_get_general_event_detail_reply &reply)
{
	if (NULL == p_role)
	{
		log_error("p_role is NULL");
		return errcode_enum::notice_unknown;
	}

	GeneralEventTable *p_conf = GET_CONF( GeneralEventTable, id );
	if (NULL == p_conf)
	{
		log_error("user[%lu] conf[%u] is NULL", p_role->get_uid(), id );
		return errcode_enum::notice_unknown;
	}
	general_event_manager_t &general_event = p_role->get_general_event();

	uint32_t status = general_event.get_general_event_status(id);

	reply.set_time_left(general_event.get_time_left(id)) ;
	reply.set_get_num(general_event.get_pass_target_num(id));
	reply.set_speed_time(general_event.get_add_speed_time(id));
	reply.set_status(status);


	const std::set<uint32_t> conf_list = p_conf->get_target_list();
	for (auto it : conf_list)
	{
		proto::common::general_event_detail *p_info = reply.add_info();
		p_info->set_index(it);
		p_info->set_process( general_event.get_detail_process(it) );

		if( status == proto::common::GENERAL_EVENT_STATUS::GENERAL_EVENT_DONE )
			p_info->set_status(general_event.get_detail_prize_status(it));
		else
			p_info->set_status( proto::common::PRIZE_STATUS::PRIZE_STATUS_NONE );
	}
	
	return errcode_enum::error_ok;
}


uint32_t general_event_manager_t::get_general_event_prize( role_ptr p_role, uint32_t index, proto::common::role_change_data *rcd)
{
	if (NULL == p_role)
	{
		log_error("p_role is NULL");
		return errcode_enum::notice_unknown;
	}

	general_event_manager_t &general_event = p_role->get_general_event();

	
	return general_event.get_detail_prize(index, rcd);
}

uint32_t general_event_manager_t::get_time_left(uint32_t id)
{
	return general_info_manager_t::get_general_event_time_left(id);
}

uint32_t general_event_manager_t::get_detail_prize(uint32_t index, proto::common::role_change_data *rcd)
{
	role_ptr p_role = role_manager_t::find_role(m_role_uid);
	if (p_role == NULL)
	{
		log_error("p_role[%lu] is NULL", m_role_uid );
		return errcode_enum::notice_unknown;
	}

	auto iter_f = m_prize_set.find(index);
	if( iter_f != m_prize_set.end())
	{
		log_error("user[%lu] has get [%u] prize", m_role_uid, index );
		return errcode_enum::notice_unknown;
	}

	//判断条件是否达成
	GeneralEventTargetTable *p_conf = GET_CONF(GeneralEventTargetTable, index);
	if (NULL == p_conf)
	{
		log_error("user[%lu] conf[%u] is NULL", p_role->get_uid(), index);
		return errcode_enum::notice_unknown;
	}

	if (p_conf->finish_num() > get_detail_process(index))
	{
		log_error("user[%lu] conf[%u] finish[%u] > process[%u]", p_role->get_uid(), index, p_conf->finish_num(), get_detail_process(index));
		return errcode_enum::notice_unknown;
	}

	bool ret = drop_manager_t::drop(p_role, p_conf->drop_id(), log_enum::srouce_type_general_event, proto::common::drop_sys_type_sevenday_target, index, rcd);
	if (!ret)
	{
		log_error("user[%lu] [%u] GeneralEvent drop faild", m_role_uid, index);
		return errcode_enum::notice_unknown;
	}

	m_prize_set.insert(index);

	save_data();

	return errcode_enum::error_ok;
}

uint32_t general_event_manager_t::get_general_event_process(uint32_t id)
{
	GeneralEventTable *p_conf = GET_CONF(GeneralEventTable, id);
	if (NULL == p_conf)
	{
		log_error("user[%lu] conf[%u] is NULL", m_role_uid, id);
		return 0;
	}
	
	uint32_t count = 0;
	const std::set<uint32_t> &conf_list = p_conf->get_target_list();
	for (auto it : conf_list)
	{
		auto iter_f = m_prize_set.find(it);
		if (iter_f != m_prize_set.end())
			count += 1;
	}
	return count;
}

uint32_t general_event_manager_t::get_general_event_status(uint32_t id)
{
	return general_info_manager_t::get_general_event_status(id);
}

uint32_t general_event_manager_t::get_general_event_time_left(uint32_t id)
{
	return general_info_manager_t::get_general_event_time_left(id);
}

std::string general_event_manager_t::get_general_event_first_name(uint32_t id)
{
	return general_info_manager_t::get_first_name(id);
}


uint32_t general_event_manager_t::get_add_speed_time(uint32_t id)
{
	GeneralEventTable *p_conf = GET_CONF(GeneralEventTable, id);
	if (NULL == p_conf)
	{
		log_error("user[%lu] conf[%u] is NULL", m_role_uid, id);
		return 0;
	}
	
	auto general_event_status = general_info_manager_t::get_general_event_map();

	auto iter_f = general_event_status.find(id);
	if (iter_f == general_event_status.end())
		return 0;

	proto::common::general_event_data &data = iter_f->second;
	if( data.is_speed_time() > 0 )
		return p_conf->get_add_speed_time() * 3600;

	return 0;
}


uint32_t general_event_manager_t::get_pass_target_num(uint32_t id)
{
	GeneralEventTable *p_conf = GET_CONF(GeneralEventTable, id);
	if (NULL == p_conf)
	{
		log_error("user[%lu] conf[%u] is NULL", m_role_uid, id);
		return 0;
	}

	uint32_t speed_target = p_conf->speed_target();

	return general_info_manager_t::get_detail_procees(speed_target);
}

uint32_t general_event_manager_t::get_detail_process(uint32_t index)
{
	role_ptr p_role = role_manager_t::find_role(m_role_uid);
	if (p_role == NULL)
	{
		log_error("p_role[%lu] is NULL", m_role_uid);
		return errcode_enum::notice_unknown;
	}

	//判断条件是否达成
	GeneralEventTargetTable *p_conf = GET_CONF(GeneralEventTargetTable, index);
	if (NULL == p_conf)
	{
		log_error("user[%lu] conf[%u] is NULL", m_role_uid, index);
		return proto::common::PRIZE_STATUS::PRIZE_STATUS_HAS;
	}
	//表示判断自己的数值
	if (p_conf->finish_num() == 0)
	{
		switch (p_conf->finish_type())
		{
		case proto::common::Achieve_Event_Type::ACHIEVE_MAIN_ROLE_LEVEL:
			if (p_role->get_level() >= p_conf->finish_value())
				return 1;
			break;
		case proto::common::Achieve_Event_Type::ACHIEVE_MAIN_ROLE_FIGHT:
			if (p_role->get_fighting() >= p_conf->finish_value())
				return 1;
			break;
		case proto::common::Achieve_Event_Type::ACHIEVE_HONOR:
			uint32_t honor = money_manager_t::get_money(p_role, proto::common::MONEY_TYPE::MONEY_TYPE_HONOR);
			if (honor >= p_conf->finish_value())
				return 1;
			break;
		}
		return 0;
	}
	return general_info_manager_t::get_detail_procees(index);
}

uint32_t general_event_manager_t::get_detail_prize_status(uint32_t index)
{
	role_ptr p_role = role_manager_t::find_role(m_role_uid);
	if (p_role == NULL)
	{
		log_error("p_role[%lu] is NULL", m_role_uid);
		return errcode_enum::notice_unknown;
	}

	auto iter_f = m_prize_set.find(index);
	if (iter_f != m_prize_set.end())
		return proto::common::PRIZE_STATUS::PRIZE_STATUS_HAS;


	//判断条件是否达成
	GeneralEventTargetTable *p_conf = GET_CONF(GeneralEventTargetTable, index);
	if (NULL == p_conf)
	{
		log_error("user[%lu] conf[%u] is NULL", m_role_uid, index);
		return proto::common::PRIZE_STATUS::PRIZE_STATUS_HAS;
	}

	//表示判断自己的数值
	if (p_conf->finish_num() == 0)
	{
		switch (p_conf->finish_type())
		{
		case proto::common::Achieve_Event_Type::ACHIEVE_MAIN_ROLE_LEVEL: 
			if (p_role->get_level() >= p_conf->finish_value())
				return proto::common::PRIZE_STATUS::PRIZE_STATUS_CAN;
			break;
		case proto::common::Achieve_Event_Type::ACHIEVE_MAIN_ROLE_FIGHT: 
			if( p_role->get_fighting() >= p_conf->finish_value() )
				return proto::common::PRIZE_STATUS::PRIZE_STATUS_CAN;
			break;
		case proto::common::Achieve_Event_Type::ACHIEVE_HONOR:
			uint32_t honor = money_manager_t::get_money(p_role, proto::common::MONEY_TYPE::MONEY_TYPE_HONOR);
			if( honor >= p_conf->finish_value() )
				return proto::common::PRIZE_STATUS::PRIZE_STATUS_CAN;
			break;
		}
		return proto::common::PRIZE_STATUS::PRIZE_STATUS_NONE;
	}


	if (p_conf->finish_num() > get_detail_process(index))
		return proto::common::PRIZE_STATUS::PRIZE_STATUS_NONE;

	return proto::common::PRIZE_STATUS::PRIZE_STATUS_CAN;
}

//检测进度,每次有进度变化需要通知
void general_event_manager_t::check_detail_process(uint32_t index)
{
	role_ptr p_role = role_manager_t::find_role(m_role_uid);
	if (p_role == NULL)
	{
		log_error("p_role[%lu] is NULL", m_role_uid);
		return;
	}

	//判断条件是否达成
	GeneralEventTargetTable *p_conf = GET_CONF(GeneralEventTargetTable, index);
	if (NULL == p_conf)
	{
		log_error("user[%lu] conf[%u] is NULL", m_role_uid, index);
		return;
	}

	if (get_detail_prize_status(index) != proto::common::PRIZE_STATUS::PRIZE_STATUS_CAN )
		return;

	//通知红点开启
	red_point_manager_t::red_point_notify(p_role->get_uid(), proto::common::RED_POINT_TYPE::RED_POINT_TYPE_GENERAL_DETAIL, 1);

	if (p_conf->drop_id() == 0)
	{
		m_prize_set.insert(index);
		save_data();
	}

	//开启条件
	std::vector<uint32_t> &conf_vec = p_conf->open_fun_list();
	for (auto it : conf_vec)
	{
		if( p_role->is_function_open(it) )
			continue;	
		
		p_role->open_function(it);
	}
}

void general_event_manager_t::on_login()
{
	role_ptr p_role = role_manager_t::find_role(m_role_uid);
	if (p_role == NULL)
	{
		log_error("p_role[%lu] is NULL", m_role_uid);
		return;
	}

	bool save_flag = false;
	bool notify_red = false;
	std::map<uint32_t, GeneralEventTable*> conf_map;
	GET_ALL_CONF(GeneralEventTable, conf_map);
	for (auto it : conf_map)
	{
		if ( get_general_event_status(it.first) != proto::common::GENERAL_EVENT_STATUS::GENERAL_EVENT_CLOSE && 
			 get_general_event_status(it.first) != proto::common::GENERAL_EVENT_STATUS::GENERAL_EVENT_WAIT )
		{ 
			auto iter_f = is_notify_red_list.find(it.first);
			if (iter_f == is_notify_red_list.end())
			{
				//通知红点开启
				red_point_manager_t::red_point_notify(m_role_uid, proto::common::RED_POINT_TYPE::RED_POINT_TYPE_GENERAL_EVENT, 1);
				is_notify_red_list.insert(it.first);
				save_flag = true;
			}
		}
	
		if ( notify_red == false && get_red_num(it.first) > 0)
			notify_red = true;
	}

	if (notify_red == true)
		red_point_manager_t::red_point_notify(p_role->get_uid(), proto::common::RED_POINT_TYPE::RED_POINT_TYPE_GENERAL_DETAIL, 1);

	if (save_flag == true)
		save_data();

	general_info_manager_t::task_check(p_role);

	check_exp_buffer();
}


uint32_t general_event_manager_t::get_red_num( uint32_t id )
{
	role_ptr p_role = role_manager_t::find_role(m_role_uid);
	if (p_role == NULL)
	{
		log_error("p_role[%lu] is NULL", m_role_uid);
		return 0;
	}

	GeneralEventTable *p_conf = GET_CONF(GeneralEventTable, id);
	if (NULL == p_conf)
	{
		log_error("user[%lu] conf[%u] is NULL", p_role->get_uid(), id);
		return 0;
	}
	uint32_t red = 0;
	general_event_manager_t &general_event = p_role->get_general_event();
	const std::set<uint32_t> conf_list = p_conf->get_target_list();
	for (auto it : conf_list)
	{
		if ( general_event.get_general_event_status(id) == proto::common::GENERAL_EVENT_STATUS::GENERAL_EVENT_DONE)
		{
			if ( general_event.get_detail_prize_status(it) == proto::common::PRIZE_STATUS_CAN )
				red += 1;
		}
		else
			return 0;
	}
	return red;
}

void general_event_manager_t::one_day()
{
	check_exp_buffer();
}


void general_event_manager_t::check_exp_buffer()
{
	role_ptr p_role = role_manager_t::find_role(m_role_uid);
	if (p_role == NULL)
	{
		log_error("p_role[%lu] is NULL", m_role_uid);
		return;
	}

	role_buff_mgr_ptr p_buf_mgr = p_role->get_role_buff_mgr();
	if (NULL == p_buf_mgr)
	{
		log_error("[%lu] p_buf_mgr is NULL", m_role_uid);
		return;
	}

	auto p_conf = GET_CONF(Comprehensive, comprehensive_common::server_level_add_buff);
	uint32_t limit_level = GET_COMPREHENSIVE_VALUE_1(p_conf);
	if (p_role->get_level() < limit_level)
		return;

	uint32_t level = general_info_manager_t::get_server_level();
	if (p_role->get_level() >= level)
	{
		p_buf_mgr->del_role_buff_by_type(role_buff_type::role_buff_type_addexp, log_enum::source_type_role_buff_server_level, p_role->get_level(), true);
		return;
	}

	uint32_t lost_level = level - p_role->get_level();
	uint32_t buff_id = 0;

	std::map<uint32_t, LevelAddExpRateTable*> conf_map;
	GET_ALL_CONF( LevelAddExpRateTable, conf_map);
	for (auto it : conf_map)
	{
		LevelAddExpRateTable *p_conf = it.second;
		if (!p_conf)
			continue;

		if (lost_level >= p_conf->level_up() && lost_level <= p_conf->level_down())
		{
			buff_id = p_conf->buff_id();
			break;
		}
	}

	if (buff_id > 0)
	{
		p_buf_mgr->add_role_buff(buff_id, log_enum::source_type_role_buff_server_level, level);
		p_buf_mgr->notify_to_client();
	}
}

void general_event_manager_t::check_all_detail()
{
	role_ptr p_role = role_manager_t::find_role(m_role_uid);
	if (p_role == NULL)
	{
		log_error("p_role[%lu] is NULL", m_role_uid);
		return;
	}

	general_event_manager_t &general_event = p_role->get_general_event();
	std::map<uint32_t, GeneralEventTable*> conf_map;
	GET_ALL_CONF(GeneralEventTable, conf_map);
	for (auto it : conf_map)
	{
		GeneralEventTable *p_conf = it.second;
		if (!p_conf)
			continue;

		uint32_t status = general_event.get_general_event_status(it.first);
		if (status != proto::common::GENERAL_EVENT_STATUS::GENERAL_EVENT_DONE)
			continue;

		const std::set<uint32_t> conf_list = p_conf->get_target_list();
		for (auto it_d : conf_list)
		{
			check_detail_process(it_d);
		}
	}
}


void general_event_manager_t::on_level_up()
{
	check_all_detail();

	check_exp_buffer();
}



