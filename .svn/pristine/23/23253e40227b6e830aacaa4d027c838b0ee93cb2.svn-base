#include "general_info_manager.hpp"
#include "GeneralEventTargetTable.tbls.h"
#include "common/config_mgr.h"
#include "role/role.hpp"
#include "tblh/LevelLimitTable.tbls.h"
#include "common/config_mgr.h"
#include "red_point/red_point_manager.hpp"
#include "role/role_manager.hpp"
#include "tblh/GeneralEventTable.tbls.h"

uint32_t general_info_manager_t::s_server_level = 0;

uint32_t general_info_manager_t::s_general_level = 0;

std::map<uint32_t, uint32_t> general_info_manager_t::s_event_map;		//天下大势的统计数据

std::map<uint32_t, proto::common::general_event_data> general_info_manager_t::s_general_event_status;	//天下大势的状态

std::map<uint32_t, std::vector<uint32_t>> general_info_manager_t::s_event_type_ids_map;

void general_info_manager_t::update_progress(uint32_t general_type, uint32_t record_val, uint32_t cur_val, role_ptr p_role)
{
    if (NULL == p_role)
    {
        log_error("NULL == p_role");
        return;
    }

	if (s_event_type_ids_map.find(general_type) == s_event_type_ids_map.end())
		return;

	for (auto id : s_event_type_ids_map[general_type])
	{
		GeneralEventTargetTable* p_conf = GET_CONF(GeneralEventTargetTable, id);
		if (NULL == p_conf)
		{
			log_error("GeneralEventTargetTable NULL == p_conf[%d]", id);
			continue;
		}
		// p_conf->t_finish_num() != 0  表示群体统计，可以重复统计
		if ( p_conf->finish_num() == 0 && record_val >= p_conf->finish_value() )
		{
			continue;
		}
		if ( cur_val < p_conf->finish_value() )
		{
			continue;
		}
		uint32_t event_id = get_general_event_id(general_type, p_conf->finish_cond() );
		proto::server::ge_push_general_event_request req;
		req.set_index(event_id);
		req.set_role_uid(p_role->get_uid());
		req.set_name(p_role->get_name());
		req.set_value(cur_val);
		env::server->send_msg_to_center(op_cmd::ge_push_general_event_request, 0, req);
	}
}

//获取
uint32_t general_info_manager_t::get_detail_procees(uint32_t index) 
{ 
	GeneralEventTargetTable* p_conf = GET_CONF(GeneralEventTargetTable, index);
	if (NULL == p_conf)
		return 0;
	
	uint32_t event_id = get_general_event_id(p_conf->finish_type(), p_conf->finish_cond());

	auto iter_f = s_event_map.find(event_id);
	if (iter_f == s_event_map.end())
		return 0;
	
	ROLE_LOG("user general %u, %u", event_id, iter_f->second);
	return iter_f->second;
}	

//获取第一个达成的玩家
std::string general_info_manager_t::get_first_name(uint32_t id) 
{ 
	auto iter_f = s_general_event_status.find(id);
	if (iter_f == s_general_event_status.end())
		return "";

	proto::common::general_event_data  data = iter_f->second;
	return data.name();
}

uint32_t general_info_manager_t::get_general_event_status(uint32_t id) 
{ 
	auto iter_f = s_general_event_status.find(id);
	if (iter_f == s_general_event_status.end())
		return proto::common::PRIZE_STATUS::PRIZE_STATUS_NONE;

	proto::common::general_event_data  data = iter_f->second;
	return data.status();
}

uint32_t general_info_manager_t::get_general_event_time_left(uint32_t id) 
{ 
	auto iter_f = s_general_event_status.find(id);
	if (iter_f == s_general_event_status.end())
		return 0;
	
	proto::common::general_event_data  data = iter_f->second;
	uint32_t start_time = data.start_time();
	uint32_t now = time_util_t::now_time();
	
	GeneralEventTable *p_conf = GET_CONF( GeneralEventTable, id );
	if (!p_conf)
		return 0;
	
	uint32_t end_time = ( p_conf->end_open_time() * 3600 ) + start_time;
	if (data.is_speed_time() > 0)
	{
		end_time -= p_conf->get_add_speed_time() * 3600;
	}
	return end_time > now ? end_time - now : 0;
}

void general_info_manager_t::make_conf()
{
	std::map<uint32_t, GeneralEventTargetTable*> all_confs;
	GET_ALL_CONF(GeneralEventTargetTable, all_confs);
	for (auto it : all_confs)
	{
		GeneralEventTargetTable* p_conf = it.second;
		if (NULL == p_conf)
		{
			continue;
		}
		s_event_type_ids_map[p_conf->finish_type()].push_back(p_conf->id());
	}
}


uint32_t general_info_manager_t::get_limit_task( uint32_t level)
{
	LevelLimitTable *p_conf = GET_CONF(LevelLimitTable , level);
	if (NULL == p_conf)
		return 0;

	return p_conf->finish_task();
}
	
uint32_t general_info_manager_t::get_limit_exp(uint32_t level)
{
	LevelLimitTable *p_conf = GET_CONF(LevelLimitTable , level);
	if (NULL == p_conf)
		return 0;

	return p_conf->max_exp();
}

uint32_t general_info_manager_t::get_add_feats(uint32_t level, uint32_t exp)
{
	LevelLimitTable *p_conf = GET_CONF(LevelLimitTable , level);
	if (NULL == p_conf)
		return 0;

	return ( p_conf->exchange_rate() / 10000.0f) * exp;
}


// 同步统计数据
void general_info_manager_t::sync_general_event_count_data(proto::server::eg_sync_general_event_count_info  &reply)
{
	for (int i = 0; i < reply.general_info_size(); ++i)
	{
		const proto::common::general_info_single &p_info = reply.general_info(i);
		s_event_map[p_info.id()] = p_info.num();
	}
}

//天下大势开启
void general_info_manager_t::on_genreal_event_start(proto::server::eg_sync_general_event_open &reply)
{
	const proto::common::general_event_data &p_info = reply.info();
	auto iter_f = s_general_event_status.find(reply.id());
	if (iter_f != s_general_event_status.end())
	{//更新
		s_general_event_status[p_info.id()] = p_info;
	}
	else
	{//新增
		s_general_event_status[p_info.id()] = p_info;
	}
	if (p_info.id() > s_general_level)
	{
		s_general_level = p_info.id();
	}
	//通知所有玩家
	role_manager_t::exec_all_user(exec_all_user_start, reply.id());
}

//天下大势结束
void general_info_manager_t::on_genreal_event_done(proto::server::eg_sync_general_event_done &reply)
{
	const proto::common::general_event_data &p_info = reply.info();
	auto iter_f = s_general_event_status.find(reply.id());
	if (iter_f != s_general_event_status.end())
	{//更新
		s_general_event_status[p_info.id()] = p_info;
	}
	else
	{//新增
		s_general_event_status[p_info.id()] = p_info;
	}
	if (p_info.id() > s_general_level)
	{
		s_general_level = p_info.id();
	}
	//通知所有玩家
	role_manager_t::exec_all_user(exec_all_user_done, reply.id());
}

//统计天下大势的信息
void general_info_manager_t::sync_general_event_data(proto::server::eg_sync_general_event_data &reply)
{
	for (int i = 0; i < reply.info_size(); ++i)
	{
		const proto::common::general_event_data &p_info = reply.info(i);
		s_general_event_status[p_info.id()] = p_info;
		if (p_info.id() > s_general_level)
		{
			s_general_level = p_info.id();
		}
	}
}


void general_info_manager_t::exec_all_user_start(role_ptr p_role, uint32_t id)
{
	if (NULL == p_role)
	{
		log_error("p_role is NULL");
		return;
	}

	//通知红点开启
	red_point_manager_t::red_point_notify(p_role->get_uid(), proto::common::RED_POINT_TYPE::RED_POINT_TYPE_GENERAL_EVENT, 1);

	//检测任务
	task_check( p_role );
}

void general_info_manager_t::exec_all_user_done(role_ptr p_role, uint32_t id)
{
	if (NULL == p_role)
	{
		log_error("p_role is NULL");
		return;
	}

	//通知红点开启
	red_point_manager_t::red_point_notify(p_role->get_uid(), proto::common::RED_POINT_TYPE::RED_POINT_TYPE_GENERAL_EVENT, 1);

	//检测任务
	task_check( p_role );
}

void general_info_manager_t::exec_all_user_process(role_ptr p_role, uint32_t id)
{
	if (NULL == p_role)
	{
		log_error("p_role is NULL");
		return;
	}
	general_event_manager_t &general_mgr = p_role->get_general_event();
	general_mgr.check_detail_process(id);
}

//任务检查
void general_info_manager_t::task_check(role_ptr p_role)
{
	if (NULL == p_role)
	{
		log_error("p_role is NULL");
		return;
	}

	task_mgr_ptr p_task_mgr = p_role->get_task_mgr();
	if (NULL == p_task_mgr)
		return;

	for (auto it : s_general_event_status)
	{
		proto::common::general_event_data &data = it.second;
		if (data.status() == proto::common::GENERAL_EVENT_STATUS::GENERAL_EVENT_DONE)
		{
			proto::common::role_change_data task_rcd;
			p_task_mgr->try_change_task_state(TASK_TARGET_TYPE_GENERAL_EVENT, task_rcd,  it.first , 1);
		}
	}
}

//任务接取时的检查
void general_info_manager_t::task_check( role_ptr p_role, task_ptr p_task )
{
	if ( NULL == p_task || NULL == p_role )
	{
		log_error("p_role is NULL");
		return;
	}

	for (auto it : s_general_event_status)
	{
		proto::common::general_event_data &data = it.second;
		if (data.status() == proto::common::GENERAL_EVENT_STATUS::GENERAL_EVENT_DONE)
		{
			p_task->advance(TASK_TARGET_TYPE_GENERAL_EVENT, it.first, 1, p_role);
		}
	}
}

void general_info_manager_t::sync_level_data(uint32_t level)
{
	s_server_level = level;
}

uint32_t general_info_manager_t::get_card_index()
{
	if (0 == s_general_level)
	{
		return 0;
	}
	auto it = s_general_event_status.find(s_general_level);
	if (it == s_general_event_status.end())
	{
		log_error("s_general_level[%d] error", s_general_level);
		return 0;
	}
	GeneralEventTable* p_conf = GET_CONF(GeneralEventTable, s_general_level);
	if (NULL == p_conf)
	{
		log_error("NULL == p_conf GeneralEventTable[%d] error", s_general_level);
		return 0;
	}
	const proto::common::general_event_data& ged = it->second;
	uint32_t now_time = time_util_t::now_time();
	if (now_time >= ged.start_time() + p_conf->card_time())
	{
		return s_general_level;
	}
	else
	{
		return s_general_level - 1;
	}
}

uint32_t general_info_manager_t::get_general_card_index()
{
	if (0 == s_general_level)
	{
		return 0;
	}
	auto it = s_general_event_status.find(s_general_level);
	if (it == s_general_event_status.end())
	{
		log_error("s_general_level[%d] error", s_general_level);
		return 0;
	}
	GeneralEventTable* p_conf = GET_CONF(GeneralEventTable, s_general_level);
	if (NULL == p_conf)
	{
		log_error("NULL == p_conf GeneralEventTable[%d] error", s_general_level);
		return 0;
	}
	const proto::common::general_event_data& ged = it->second;
	uint32_t now_time = time_util_t::now_time();
	if (now_time >= ged.start_time() + p_conf->card_time())
	{
		return 0;
	}
	else
	{
		return s_general_level;
	}
}

uint32_t general_info_manager_t::get_general_card_remain_time()
{
	if (0 == s_general_level)
	{
		return 0;
	}
	auto it = s_general_event_status.find(s_general_level);
	if (it == s_general_event_status.end())
	{
		log_error("s_general_level[%d] error", s_general_level);
		return 0;
	}
	GeneralEventTable* p_conf = GET_CONF(GeneralEventTable, s_general_level);
	if (NULL == p_conf)
	{
		log_error("NULL == p_conf GeneralEventTable[%d] error", s_general_level);
		return 0;
	}
	const proto::common::general_event_data& ged = it->second;
	uint32_t now_time = time_util_t::now_time();
	if (now_time >= ged.start_time() + p_conf->card_time())
	{
		return 0;
	}
	else
	{
		return ged.start_time() + p_conf->card_time() - now_time;
	}
}

