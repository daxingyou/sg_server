#include "general_info_manager.hpp"

#include "main/center_server.hpp"
#include "main/center_server_fwd.hpp"
#include "redis_client.hpp"
#include "rank/rank_manager.hpp"
#include "tblh/GeneralEventTargetTable.tbls.h"
#include "common/config_mgr.h"
#include "tblh/rank_common.hpp"
#include "tblh/Comprehensive.tbls.h"

USING_NS_COMMON;

std::map<uint32_t, uint32_t> general_info_manager_t::m_data_map;
std::map<uint32_t, uint32_t> general_info_manager_t::m_data_tmp_map;
std::map<uint32_t, std::vector<uint32_t>> general_info_manager_t::s_event_type_ids_map;

void general_info_manager_t::load_data()
{
	//读取所有数据
	std::vector<uint32_t> ids;
	redis_client_t::get_all_keys<uint32_t, std::vector<uint32_t>>("general_info", ids);

	for (auto id : ids)
	{
		uint32_t num = redis_client_t::get_uint32("general_info", "num", id);
		m_data_map.insert(std::make_pair(id, num));
	}

	sync_general_data();
}

void general_info_manager_t::sync_general_data()
{
	proto::server::eg_sync_general_event_count_info request;
	for (auto it : m_data_map)
	{
		proto::common::general_info_single *p_single = request.add_general_info();
		p_single->set_id(it.first);
		p_single->set_num(it.second);
	}
	env::server->send_msg_to_all_games(op_cmd::eg_sync_general_event_count_info, 0, request);

	sync_max_level_data();
}

void general_info_manager_t::sync_max_level_data()
{
	auto p_conf = GET_CONF(Comprehensive, comprehensive_common::server_level_count);
	uint32_t server_level_count = GET_COMPREHENSIVE_VALUE_1(p_conf);
	if (server_level_count == 0) return;

	//同步当前服务器等级
	std::vector<std::pair<uint64_t, uint32_t>> value_list;
	rank_level_ptr p_rank_level = rank_manager_t::get_rank_level();
	if (p_rank_level) {
		p_rank_level->get_range_by_rank_withscores(0, server_level_count - 1, value_list);
	}

	if (value_list.empty()) return;

	uint32_t max_value = 0;
	for (std::vector<std::pair<uint64_t, uint32_t>>::const_iterator citr = value_list.begin();
		citr != value_list.end(); ++citr) {
		max_value += citr->second;
	}

	uint32_t range = max_value / value_list.size();
	proto::server::eg_sync_server_level reply;
	reply.set_level(range);
	env::server->send_msg_to_all_games(op_cmd::eg_sync_server_level, 0, reply);
}

void general_info_manager_t::add_num(uint32_t id)
{
	m_data_map[id] += 1;
	redis_client_t::set_uint32("general_info", "num", id, m_data_map[id]);

	m_data_tmp_map[id] = m_data_map[id];
	proto::server::eg_sync_general_event_count_info request;
	for (auto it : m_data_tmp_map)
	{
		proto::common::general_info_single *p_single = request.add_general_info();
		p_single->set_id(it.first);
		p_single->set_num(it.second);
	}
	env::server->send_msg_to_all_games(op_cmd::eg_sync_general_event_count_info, 0, request);
	m_data_tmp_map.clear();
}


//统计出对应的数值+1
void general_info_manager_t::add_num(uint32_t type, uint32_t cur_value, uint32_t old_value)
{
	if (s_event_type_ids_map.find(type) == s_event_type_ids_map.end())
		return;

	for (auto id : s_event_type_ids_map[type])
	{
		GeneralEventTargetTable* p_conf = GET_CONF(GeneralEventTargetTable, id);
		if (NULL == p_conf)
			continue;

		//旧值表示已经统计过
		if (old_value >= p_conf->finish_value())
			continue;

		if (cur_value < p_conf->finish_value())
			continue;

		uint32_t event_id = get_general_event_id(type, p_conf->finish_cond());
		add_num(event_id);
	}
}


void general_info_manager_t::add_num(uint32_t type, uint32_t cur_value)
{
	if (s_event_type_ids_map.find(type) == s_event_type_ids_map.end())
		return;

	for (auto id : s_event_type_ids_map[type])
	{
		GeneralEventTargetTable* p_conf = GET_CONF(GeneralEventTargetTable, id);
		if (NULL == p_conf)
			continue;

		if (cur_value != p_conf->finish_value())
			continue;

		uint32_t event_id = get_general_event_id(type, p_conf->finish_cond());
		add_num(event_id);
	}
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


uint32_t general_info_manager_t::get_num(uint32_t id)
{
	if (m_data_map.find(id) == m_data_map.end())
		return 0;

	return m_data_map[id];
}

void general_info_manager_t::peek_data(proto::common::general_info_data* p_data)
{
	if (NULL == p_data)
	{
		log_error("NULL == p_data");
		return;
	}
	for (auto it : m_data_map)
	{
		proto::common::general_info_single* p_single = p_data->add_general_info();
		p_single->set_id(it.first);
		p_single->set_num(it.second);
	}
}

