#ifndef _GENERAL_EVENT_MANAGER_HPP_
#define _GENERAL_EVENT_MANAGER_HPP_

//天下大势统计
#include <map>
#include <set>
#include "msg_handler.hpp"
#include "protos_fwd.hpp"
#include "cmd_desc.hpp"


class general_event_manager_t
{
public:
	static bool init();

	//生成反表
	static void make_event_table();
	
	//当进度时间变化
	static void on_event_process( uint32_t id , uint32_t value , std::string name, uint64_t uid );
	
	//同步天下大势数据
	static void sync_general_event_data();
public:
	static void load_data();
	
	static void save_event_data(proto::common::general_event_data &data );
	
	static void save_first_name( uint32_t id , std::string &name );

	static void peek_data();
public:
	static uint32_t get_general_event_id(uint32_t type, uint32_t eventid) { return (type * 1000) + eventid; }

	static uint32_t get_general_type(uint32_t id) { return  id / 1000; }

	static uint32_t get_general_eventid(uint32_t id) { return id % 1000; }

	//检测天下大势是否开启
	static void on_timer_check_open( uint32_t id , bool is_intime );

	//检测天下大势是否结束
	static void on_timer_check_end( uint32_t id , bool is_intime );
public:
	static void check_open_general(uint32_t id, uint32_t value, std::string name, uint64_t uid );

	static void record_first_name( uint32_t id , std::string name);

	static void is_speed_time();

	static uint32_t  get_general_status( uint32_t id );

public:
	static std::map<uint32_t, std::set<uint32_t>> s_event_table;	//类型对应大势的反表

	static std::map<uint32_t, proto::common::general_event_data > s_general_event_status;		//大势数据

	static std::map<uint32_t, std::string > s_first_event_name;		//第一个目标达成的玩家
};

#endif