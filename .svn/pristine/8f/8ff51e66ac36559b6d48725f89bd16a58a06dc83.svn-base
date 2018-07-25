#ifndef _GENERAL_EVENT_MANAGER_HPP_
#define _GENERAL_EVENT_MANAGER_HPP_ 

#include "msg_handler.hpp"
#include "protos_fwd.hpp"
#include "cmd_desc.hpp"
/*天下大势*/

class role_t;
typedef boost::shared_ptr<role_t> role_ptr;

class general_event_manager_t 
{
public:
	general_event_manager_t(uint64_t role_uid);

	void load_data( const proto::common::general_event_save_data &data);

	void peek_data( proto::common::general_event_save_data &data);

	void save_data();
public:
	static uint32_t get_general_event_info( role_ptr p_role, proto::client::gc_get_general_event_info_reply &reply);
	
	static uint32_t get_general_event_dtail_info( role_ptr p_role, uint32_t id , proto::client::gc_get_general_event_detail_reply &reply );

	static uint32_t get_general_event_prize( role_ptr p_role ,uint32_t index, proto::common::role_change_data *rcd );

public:
	//获取对应大势的进度
	uint32_t  get_general_event_process( uint32_t id );

	//获取对应大势的完成进度
	uint32_t get_general_event_status( uint32_t id );

	uint32_t get_general_event_time_left(uint32_t id);

	std::string get_general_event_first_name(uint32_t id);

	uint32_t get_add_speed_time( uint32_t id );

	uint32_t get_time_left( uint32_t id );

	uint32_t get_pass_target_num( uint32_t id );

	void one_day();

	void check_exp_buffer();

	void on_level_up();
public:
	uint32_t get_detail_process( uint32_t index );

	uint32_t get_detail_prize_status ( uint32_t index );

	uint32_t get_detail_prize(uint32_t index, proto::common::role_change_data *rcd );
public:
	void check_detail_process( uint32_t index );

	void check_all_detail();

	void on_login();

	uint32_t get_red_num( uint32_t id );
private:
	std::string m_key = "";
	
	uint64_t m_role_uid = 0;

	std::set<uint32_t> is_notify_red_list;		//是否通知了红点

	std::set<uint32_t> m_prize_set;
};



#endif
