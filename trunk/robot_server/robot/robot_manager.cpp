#include "robot_manager.hpp"

#include "robot.hpp"
#include "common/log.hpp"
#include "main/robot_server_fwd.hpp"
#include "robot_common/robot_common_struct.hpp"
#include "robot_common/sTime.hpp"

USING_NS_COMMON;

robot_manager_t::robot_list_t robot_manager_t::m_robot_list;

robot_manager_t::robot_group_vec robot_manager_t::m_robot_group_vec;
uint32_t robot_manager_t::m_step_add = 0;
uint32_t robot_manager_t::m_step_done = 0;

boost::atomic<uint32_t> robot_manager_t::succ_num;
boost::atomic<uint64_t> robot_manager_t::start_login_time;
boost::atomic<uint64_t> robot_manager_t::end_enter_time;

boost::atomic<uint32_t> robot_manager_t::fail_logic_num;
boost::atomic<uint32_t> robot_manager_t::fail_conn_num;

uint32_t robot_manager_t::m_loop_login_start_id = 0;
uint32_t robot_manager_t::m_loop_login_end_id = 0;
std::string robot_manager_t::m_special_login_id = "";

void robot_manager_t::on_robot_start_login()
{
	start_login_time = common::time_util_t::now_msecs();
}

uint32_t total_time = 0;
void robot_manager_t::on_robot_enter_success(uint64_t robot_id, uint64_t uid, uint64_t login_start_time, uint32_t use_time)
{
    succ_num.fetch_add(1, boost::memory_order_relaxed);
    uint32_t cur_enter_nums = robot_manager_t::succ_num.load(boost::memory_order_relaxed);
    log_info("-----------The success enter game robots[%lu] num: %d/%d use_time[%u]----------", uid, cur_enter_nums, env::robot_config->number, use_time);
    total_time += use_time;
    if (cur_enter_nums == env::robot_config->number)
    {
		end_enter_time = common::time_util_t::now_msecs();

        //log_info("==========all the %d robots enter success, cost time %.2f(seconds)==========", cur_enter_nums,
        //    (end_enter_time.load(boost::memory_order_relaxed) - start_login_time.load(boost::memory_order_relaxed)) / 1000.f);

        log_info("==========all the %d robots enter success, cost time %.2f(seconds)==========", cur_enter_nums, total_time / 1000.f);
    }
}


void robot_manager_t::add_robot_failed_connect(uint64_t id)
{
	log_error("ERROR:robot[%lu] connect failed...!!", id);

	fail_conn_num.fetch_add(1, boost::memory_order_relaxed);
	log_error("ERROR:all the %d robot can't connect, all the %d robot error logic",
		robot_manager_t::fail_conn_num.load(boost::memory_order_relaxed),
		robot_manager_t::fail_logic_num.load(boost::memory_order_relaxed));
}


void robot_manager_t::add_robot_failed_loginc(uint64_t robot_id, uint64_t uid, uint32_t state)
{
	log_error("ERROR:robot[%lu] uid[%lu] login failed...LOGIN STATE [%d] !!", robot_id, uid, state);

	fail_logic_num.fetch_add(1, boost::memory_order_relaxed);
	log_error("ERROR:all the %d robot can't connect, all the %d robot error logic",
	robot_manager_t::fail_conn_num.load(boost::memory_order_relaxed),
	robot_manager_t::fail_logic_num.load(boost::memory_order_relaxed));
}

bool robot_manager_t::init_all_robots()
{
	if (env::robot_config->heart_group == 0) {
		log_error("init robot heart_group == 0");
		return false;
	}

	if (env::robot_config->sepcial.empty() && env::robot_config->number == 0) {
		log_error("init robot manager failed!");
		return false;
	}

	m_loop_login_start_id = env::robot_config->start_id;
	m_loop_login_end_id = env::robot_config->start_id + env::robot_config->number;
	m_special_login_id = env::robot_config->sepcial;

	m_step_add = 0;
	m_step_done = 0;
	m_robot_group_vec.resize(env::robot_config->heart_group);

	return true;
}

bool robot_manager_t::reset_all_robots()
{
   robot_list_t::iterator itr = m_robot_list.begin();
   for ( ; itr != m_robot_list.end(); ++itr)
   {
       robot_ptr p_robot = itr->second;
       if (NULL != p_robot)
       {
           p_robot->disconnect();
       }
   }

   m_robot_list.clear();

   m_step_add = 0;
   m_step_done = 0;

   m_robot_group_vec.clear();
   m_robot_group_vec.resize(env::robot_config->heart_group);

   return true;
}


bool robot_manager_t::loop_interval_login()
{
	// 优先登陆特殊账号模式
	if (!m_special_login_id.empty())
	{
		if (!init_special_robot())
		{
			return false;
		}
		
		return false;
	}
	else if (m_loop_login_start_id < m_loop_login_end_id)
	{
		if (!init_one_robot(m_loop_login_start_id))
		{
			return false;
		}

		m_loop_login_start_id++;

		if (m_loop_login_start_id < m_loop_login_end_id)
			return true;
		else
			return false;
	}

	return false;
}


void robot_manager_t::loop_login_out()
{
	reset_all_robots();

	robot_manager_t::init_all_robots();

	succ_num = 0;
	fail_logic_num = 0;
	fail_conn_num = 0;
}


bool robot_manager_t::loop_check_robot()
{
	robot_list_t::iterator itr = m_robot_list.begin();
	for (; itr != m_robot_list.end(); ++itr)
	{
		robot_ptr p_robot = itr->second;
		if (p_robot)
		{
			if (!p_robot->is_task_done())
			{
				return false;
			}
		}
	}

	return true;
}

bool robot_manager_t::init_one_robot(uint32_t uid)
{
	boost::shared_ptr<robot_t> p_robot(new robot_t(uid));
	if (NULL == p_robot) {
		log_error("create robot[%u] failed!", uid);
		return false;
	}

	if (!start_up_robot(p_robot)) {
		log_error("start up robot[%lu] failed!", p_robot->get_robot_id());
		robot_manager_t::add_robot_failed_connect(p_robot->get_robot_id());
		return false;
	}

	if (!add_robot(p_robot)) {
		log_error("robot[%u] add in list error", uid);
		return false;
	}

    return true;
}


bool robot_manager_t::init_special_robot()
{
	boost::shared_ptr<robot_t> p_robot(new robot_t(m_special_login_id));
	if (NULL == p_robot) {
		log_error("create robot[%s] failed!", m_special_login_id.c_str());
		return false;
	}

	if (!start_up_robot(p_robot)) {
		log_error("start up robot[%s] failed!", m_special_login_id.c_str());
		robot_manager_t::add_robot_failed_connect(p_robot->get_robot_id());
		return false;
	}

	if (!add_robot(p_robot)) {
		log_error("robot[%s] add in list error", m_special_login_id.c_str());
		return false;
	}

	return true;
}

void robot_manager_t::heartbeat()
{
	uint64_t curr_time = common::time_util_t::now_msecs();
	for (robot_groups::const_iterator citr = m_robot_group_vec[m_step_done].begin();
		citr != m_robot_group_vec[m_step_done].end(); ++citr) {
		robot_ptr p_robot = (*citr).lock();
		if (p_robot) {
			p_robot->heartbeat(curr_time);
		}
	}

	//延迟高的时候提示
	uint64_t delay_time = common::time_util_t::now_msecs() - curr_time;
	if (delay_time > env::robot_config->frame_time + 100) { 
		log_warn("robot delay_time[%lu] > frame_time[%u]", delay_time, env::robot_config->frame_time);
	}

	m_step_done++;
	m_step_done = m_step_done % env::robot_config->heart_group;
}

bool robot_manager_t::start_up_robot(const robot_ptr& p_robot)
{
    if (NULL == p_robot)
    {
        log_error("robot is NULL!");
        return false;
    }

    uint32_t rand_index = rand() % env::server->m_account_ip_list.size();
    env::robot_config->account_server_ip = env::server->m_account_ip_list[rand_index];
    env::robot_config->account_server_port = boost::lexical_cast<uint32_t>(env::server->m_account_port_list[rand_index]);
    if (!p_robot->connect(env::robot_config->account_server_ip, env::robot_config->account_server_port)) 
    {
		log_error("robot[%lu] connect to account_server[%s:%d] failed!", p_robot->get_robot_id(), env::robot_config->account_server_ip.c_str(), env::robot_config->account_server_port);
		return false;
    }

	return true;
}

robot_ptr robot_manager_t::get_robot(uint64_t uid)
{
    robot_list_t::iterator itr = m_robot_list.find(uid);
    if (itr != m_robot_list.end())
    {
        return itr->second;
    }

    return NULL;
}

bool robot_manager_t::add_robot(robot_ptr& p_robot)
{
	if (p_robot == NULL) {
		log_error("p_robot null error");
		return false;
	}

	std::pair<robot_list_t::iterator, bool> res = m_robot_list.insert(std::pair<uint64_t, robot_ptr>(p_robot->get_robot_id(), p_robot));
	if (!res.second) {
		log_error("robot[%lu] has exist at list!", p_robot->get_robot_id());
		return false;
	}

	p_robot->set_open_lua(env::robot_config->lua_open);
	p_robot->set_login_start_time(common::time_util_t::now_msecs());

	m_robot_group_vec[m_step_add].push_back(p_robot);
	m_step_add++;
	m_step_add = m_step_add % env::robot_config->heart_group;

	return true;
}


