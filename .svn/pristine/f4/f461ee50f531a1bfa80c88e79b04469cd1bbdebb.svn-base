#ifndef __CENTER_GENERAL_INFO_MANAGER_H__
#define __CENTER_GENERAL_INFO_MANAGER_H__

#include "server.pb.h"
#include <map>
#include <vector>

class general_info_manager_t
{
public:
	general_info_manager_t(){}
	~general_info_manager_t(){}
public:
	static void load_data();

	static void add_num(uint32_t id);

	static void add_num(uint32_t type, uint32_t cur_value, uint32_t old_value );	//统计分解值

	static void add_num(uint32_t type, uint32_t cur_value);	//不统计分解

	static uint32_t get_num(uint32_t id);

	static void peek_data(proto::common::general_info_data* p_data);

	static void sync_general_data();

	static bool init() { load_data(); return true; }

	static void make_conf();

	static uint32_t get_general_event_id(uint32_t type, uint32_t eventid) { return (type * 1000) + eventid; }

	static uint32_t get_general_type(uint32_t id) { return  id / 1000; }

	static void sync_max_level_data();

	static void one_day() { sync_max_level_data(); }
private:
	static std::map<uint32_t, uint32_t> m_data_map;
	
	static std::map<uint32_t, uint32_t> m_data_tmp_map;

	static std::map<uint32_t, std::vector<uint32_t>> s_event_type_ids_map;
};

#endif