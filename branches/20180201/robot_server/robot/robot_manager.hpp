#ifndef __ROBOT_MANAGER_HPP__
#define __ROBOT_MANAGER_HPP__

#include <map>
#include <vector>

#include <boost/shared_ptr.hpp>
#include <boost/atomic.hpp>
#include "robot.hpp"

class robot_manager_t
{
public:
    static void on_robot_start_login();
    static void on_robot_enter_success(uint64_t robot_id, uint64_t uid, uint64_t login_start_time);

	static void add_robot_failed_connect(uint64_t id);
	static void add_robot_failed_loginc(uint64_t robot_id, uint64_t uid, uint32_t state);

public:
	//static  uint64_t get_cur_sec();
    //成功登录人数;
    static boost::atomic<uint32_t> succ_num;
    //开始登录时间;
    static boost::atomic<uint64_t> start_login_time;
    //全部登录成功时间;
    static boost::atomic<uint64_t> end_enter_time;

	static uint32_t m_loop_login_end_id;
	static uint32_t m_loop_login_start_id;
	static std::string m_special_login_id;

	static boost::atomic<uint32_t> fail_logic_num;	// 登陆成功但是服务器消息处理失败
	static boost::atomic<uint32_t> fail_conn_num;	// 直接连接account失败人数

public:
    static bool init_all_robots();
    static bool reset_all_robots();
	static bool loop_interval_login();
	static void loop_login_out();
	static bool loop_check_robot();

    static bool init_one_robot(uint32_t uid);
	static bool init_special_robot();

	static void heartbeat();
private:
    static bool start_up_robot(const robot_ptr& p_robot);
    static robot_ptr get_robot(uint64_t uid);
	static bool add_robot(robot_ptr& p_robot);
    
private:
   typedef std::map<uint64_t, robot_ptr> robot_list_t;
   static robot_list_t m_robot_list;

   typedef std::vector<robot_wptr> robot_groups;
   typedef std::vector<robot_groups> robot_group_vec;
   static robot_group_vec m_robot_group_vec;

   static uint32_t m_step_add;
   static uint32_t m_step_done;
};




#endif  //__ROBOT_MANAGER_HPP__
