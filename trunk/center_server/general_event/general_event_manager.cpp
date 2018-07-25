#include "tblh/GeneralEventTable.tbls.h"
#include "tblh/GeneralEventTargetTable.tbls.h"
#include "general_event_manager.hpp"
#include "common/config_mgr.h"
#include "main/center_server.hpp"
#include "main/center_server_fwd.hpp"
#include "common/time_manager.hpp"
#include "tblh/Comprehensive.tbls.h"
#include "redis_client.hpp"
#include "sys_notice/sys_notice_manager.hpp"
#include "general_info_manager.hpp"
#include "common/utility.hpp"
#include "achieve/achieve_common.hpp"

std::map<uint32_t, std::set<uint32_t>> general_event_manager_t::s_event_table;	//类型对应大势的反表

std::map<uint32_t, proto::common::general_event_data > general_event_manager_t::s_general_event_status;		//第一个开启的玩家

std::map<uint32_t, std::string > general_event_manager_t::s_first_event_name;		//第一个目标达成的玩家

void general_event_manager_t::make_event_table()
{
	std::map<uint32_t, GeneralEventTargetTable*> conf_map;
	GET_ALL_CONF(GeneralEventTargetTable, conf_map);

	for( auto it : conf_map )
	{
		GeneralEventTargetTable *p_conf = it.second;
		if (NULL == p_conf)
			continue;
		s_event_table[p_conf->finish_type()].insert( it.first );
	}
}

void general_event_manager_t::on_event_process( uint32_t id, uint32_t value,  std::string  name, uint64_t uid )
{	
//检测大势是否开启
	check_open_general(id, value, name, uid );

//记录第一人
	record_first_name(id, name);

	//检测大势是否加速
	//is_speed_time();
}

//同步全部数据
void general_event_manager_t::sync_general_event_data()
{
	proto::server::eg_sync_general_event_data request;
	
	for (auto it : s_general_event_status)
	{
		proto::common::general_event_data &p_data = it.second;
		proto::common::general_event_data *p_info = request.add_info();
		p_info->CopyFrom(p_data);
	}
	env::server->send_msg_to_all_games(op_cmd::eg_sync_general_event_data, 0, request);
}

void general_event_manager_t::check_open_general(uint32_t id, uint32_t value, std::string name , uint64_t uid )
{
	uint32_t event_type = get_general_type(id);
	if (event_type != proto::common::Achieve_Event_Type::ACHIEVE_MAIN_ROLE_LEVEL)
		return;

	std::map<uint32_t, GeneralEventTable*> conf_map;
	GET_ALL_CONF(GeneralEventTable, conf_map);

	for (auto it : conf_map)
	{
		GeneralEventTable *p_conf = it.second;
		if (!p_conf)
			continue;
		
		if ( p_conf->level() > value || get_general_status(it.first) != proto::common::GENERAL_EVENT_STATUS::GENERAL_EVENT_CLOSE) 
			continue;
		
		//大势开启
		proto::common::general_event_data data;
		data.set_id(it.first);
		data.set_name(name);
		data.set_status(proto::common::GENERAL_EVENT_STATUS::GENERAL_EVENT_WAIT);
		data.set_start_time( common::time_util_t::get_next_special_time(10) );
		s_general_event_status.insert(std::make_pair(it.first, data ));
		save_event_data(data);
		achieve_common_t::notify_progress_state(uid, proto::common::ACHIEVE_OPEN_GENERAL, value, 1);
	}
}

void general_event_manager_t::record_first_name(uint32_t id, std::string name)
{
	uint32_t event_type = get_general_type(id);
	uint32_t event_id = get_general_eventid(id);

	auto iter_f = s_event_table.find(event_type);
	if (iter_f == s_event_table.end())
		return;

	std::set<uint32_t> &conf_map = iter_f->second;
	for (auto it : conf_map)
	{
		GeneralEventTargetTable *p_conf = GET_CONF( GeneralEventTargetTable, it);
		if (NULL == p_conf)
			continue;

		if ( event_id == p_conf->finish_cond() && p_conf->is_record() == 1)
		{
			auto iter_f = s_first_event_name.find(it);
			if (iter_f == s_first_event_name.end())
			{
				s_first_event_name.insert(std::make_pair(it, name));
				save_first_name(id, name);
				break;
			}
		}
	}
}

void general_event_manager_t::is_speed_time()
{
	uint32_t now = time_util_t::now_time();
 //判断是否加速
	std::map<uint32_t, GeneralEventTable*> conf_map;
	GET_ALL_CONF(GeneralEventTable, conf_map);

	for (auto it : conf_map)
	{
		auto iter_f = s_general_event_status.find(it.first);
		if( iter_f == s_general_event_status.end() )
			continue;	
		
		proto::common::general_event_data &p_data = iter_f->second;
		if (p_data.is_speed_time() > 0)
			continue;
		
		GeneralEventTable *p_conf = it.second;
		if (!p_conf)
			continue;
	
		uint32_t target_id = p_conf->speed_target();
		GeneralEventTargetTable *p_target = GET_CONF(GeneralEventTargetTable, target_id);
		if (!p_target)
			continue;
	
		uint32_t event_id = general_info_manager_t::get_general_event_id( p_target->finish_type(), p_target->finish_cond() );
		uint32_t num = general_info_manager_t::get_num(event_id);
	
		if (num >= p_conf->get_add_speed_cond())
		{
			uint32_t end_time = p_data.start_time() + p_conf->end_open_time() * 3600 ;
				
			//小于一天不加速
			if ( (now + DAY_SECOND) > end_time)
				return;

			p_data.set_is_speed_time(1);

			save_event_data( p_data );

			sync_general_event_data();
		}
	}
}

uint32_t  general_event_manager_t::get_general_status(uint32_t id)
{
	auto iter_f = s_general_event_status.find(id);
	if (iter_f == s_general_event_status.end())
		return proto::common::GENERAL_EVENT_STATUS::GENERAL_EVENT_CLOSE;

	proto::common::general_event_data &data = iter_f->second;
	return data.status();
}

//检测天下大势是否开启
void general_event_manager_t::on_timer_check_open( uint32_t id , bool is_intime)
{
	uint32_t now = common::time_util_t::now_time();

	for (auto &it : s_general_event_status)
	{
		proto::common::general_event_data &data = it.second;
		if (data.status() != proto::common::GENERAL_EVENT_STATUS::GENERAL_EVENT_WAIT)
			continue;

		if ( data.start_time() > now  )
			continue;

		data.set_status( proto::common::GENERAL_EVENT_STATUS::GENERAL_EVENT_OPENING );

		save_event_data(data);

		//TODO通知所有人，天下大势开启
		proto::server::eg_sync_general_event_open request;
		request.set_id( it.first );
		request.mutable_info()->CopyFrom( data );
		env::server->send_msg_to_all_games(op_cmd::eg_sync_general_event_open, 0, request);

		GeneralEventTable *p_conf = GET_CONF( GeneralEventTable, it.first );
		if (NULL == p_conf)
			continue;
		BROADCAST_SYS_NOTICE(p_conf->touch_notice(), 0, 0, 0,"");
		break;
	}
}

//检测天下大势是否结束
void general_event_manager_t::on_timer_check_end( uint32_t id, bool is_intime)
{
	is_speed_time();

	uint32_t now = common::time_util_t::now_time();

	for (auto &it : s_general_event_status)
	{
		proto::common::general_event_data &data = it.second;
		if (data.status() != proto::common::GENERAL_EVENT_STATUS::GENERAL_EVENT_OPENING )
			continue;

		GeneralEventTable *p_conf = GET_CONF( GeneralEventTable,  it.first );
		if (!p_conf)
		{
			log_error(" cant't find GeneralEventTable [%d] conf ", it.first);
			continue;
		}

		uint32_t speed_time = data.is_speed_time() > 0 ? p_conf->get_add_speed_time() : 0;
		if ( ( data.start_time() + p_conf->end_open_time() * 3600 ) > now  + (speed_time * 3600) )
			continue;

		data.set_status(proto::common::GENERAL_EVENT_STATUS::GENERAL_EVENT_DONE);
		save_event_data(data);

		//TODO通知所有人，天下大势完成
		proto::server::eg_sync_general_event_done request;
		request.set_id( it.first );
		request.mutable_info()->CopyFrom( data );
		env::server->send_msg_to_all_games(op_cmd::eg_sync_general_event_done, 0, request);

		BROADCAST_SYS_NOTICE(p_conf->open_notice(), 0, 0, 0, "");
		break;
	}
}

bool general_event_manager_t::init()
{
	auto p_conf = GET_CONF(Comprehensive, comprehensive_common::time_world_event);
	uint32_t time_id = GET_COMPREHENSIVE_VALUE_1(p_conf);
	//注册时间回掉id
	time_manager_t::register_func(time_id, general_event_manager_t::on_timer_check_open);
	time_manager_t::register_func(time_id, general_event_manager_t::on_timer_check_end);
	return true;
}


void general_event_manager_t::load_data()
{
	//读取所有数据
	std::vector<uint32_t> ids;
	redis_client_t::get_all_keys<uint32_t, std::vector<uint32_t>>("general_status", ids);

	for (auto id : ids)
	{
		proto::common::general_event_data data;
		std::string  name = redis_client_t::get_string("general_status", "name", id);
		uint32_t start_time = redis_client_t::get_uint32("general_status", "start_time", id);
		uint32_t status     = redis_client_t::get_uint32("general_status", "status", id);
		uint32_t is_speed_time = redis_client_t::get_uint32("general_status", "is_speed_time", id);

		data.set_id(id);
		data.set_name(name);
		data.set_start_time(start_time);
		data.set_status(status);
		data.set_is_speed_time(is_speed_time);
		s_general_event_status[id] = data;
	}

	std::vector<uint32_t> ids_name;
	redis_client_t::get_all_keys<uint32_t, std::vector<uint32_t>>("general_target_first_name", ids_name);

	for (auto id : ids)
	{
		std::string  name = redis_client_t::get_string("general_target_first_name", "name", id);
		s_first_event_name[id] = name;
	}
	sync_general_event_data();
}

void general_event_manager_t::save_event_data( proto::common::general_event_data &data )
{
	redis_client_t::set_string("general_status", "name", data.id(), data.name());
	redis_client_t::set_uint32("general_status", "start_time", data.id(), data.start_time()  );
	redis_client_t::set_uint32("general_status", "status", data.id(), data.status());
	redis_client_t::set_uint32("general_status", "is_speed_time", data.id(), data.is_speed_time());
}

void general_event_manager_t::save_first_name(uint32_t id, std::string &name)
{
	redis_client_t::set_string("general_target_first_name", "name", id, name);
}