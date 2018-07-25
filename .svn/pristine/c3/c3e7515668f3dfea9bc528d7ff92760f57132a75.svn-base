#ifndef _GENERAL_INFO_MANAGER_HPP_
#define _GENERAL_INFO_MANAGER_HPP_ 

#include "msg_handler.hpp"
#include "protos_fwd.hpp"
#include "cmd_desc.hpp"
#include "task/task.hpp"
/*天下大势*/

class role_t;
typedef boost::shared_ptr<role_t> role_ptr;

class general_info_manager_t
{
public:
	// 同步统计数据
	static void sync_general_event_count_data( proto::server::eg_sync_general_event_count_info  &reply );

	//天下大势开启
	static void on_genreal_event_start( proto::server::eg_sync_general_event_open &reply);

	//天下大势结束
	static void on_genreal_event_done( proto::server::eg_sync_general_event_done &reply);
	
	//统计天下大势的信息
	static void sync_general_event_data( proto::server::eg_sync_general_event_data &reply );

	static void sync_level_data(uint32_t level);
	
	static uint32_t get_server_level() { return s_server_level;  }

	static uint32_t get_general_level() { return s_general_level; }

	//普通抽卡序号（0 - 4）
	static uint32_t get_card_index();
	//天下大势抽卡序号（1 - 4）
	static uint32_t get_general_card_index();
	//天下大势抽卡剩余时间
	static uint32_t get_general_card_remain_time();
public:
	static uint32_t get_general_event_id(uint32_t type, uint32_t eventid) { return (type * 1000) + eventid; }

	static uint32_t get_general_type(uint32_t id) { return  id / 1000; }

	static uint32_t get_general_eventid(uint32_t id) { return id % 1000; }
public:	
	static void update_progress(uint32_t general_type, uint32_t record_val, uint32_t cur_val, role_ptr p_role);

	static uint32_t get_detail_procees(uint32_t index);

	//获取第一个达成的玩家
	static std::string get_first_name(uint32_t id);

	static uint32_t get_general_event_status(uint32_t id);

	static uint32_t get_general_event_time_left(uint32_t id);
public:
	static void make_conf();

	static uint32_t get_limit_task( uint32_t level);
	
	static uint32_t get_limit_exp(uint32_t level);

	static uint32_t get_add_feats(uint32_t level, uint32_t exp);
public:
	static void exec_all_user_start(role_ptr p_role, uint32_t id);

	static void exec_all_user_done(role_ptr p_role, uint32_t id);

	static void exec_all_user_process(role_ptr p_role, uint32_t id);

	static void task_check( role_ptr p_role );

	static void task_check( role_ptr p_role, task_ptr p_task);

	static const std::map<uint32_t, proto::common::general_event_data> &get_general_event_map() { return s_general_event_status; }	//天下大势的状态
private:
	static uint32_t s_server_level;						//服务器等级	

	static uint32_t s_general_level;					//当前开启的天下大势最高等级

	static std::map<uint32_t, uint32_t> s_event_map;		//天下大势的统计数据
	
	static std::map<uint32_t, proto::common::general_event_data> s_general_event_status;	//天下大势的状态

	static std::map<uint32_t, std::vector<uint32_t>> s_event_type_ids_map;		//以完成类型为索引的策划表ID列表
};

#endif