#include "robot_server.hpp"

#include "common/assert.hpp"
#include "common/log.hpp"
#include "main/robot_server_fwd.hpp"
#include "robot/robot_manager.hpp"
#include "robot/robot_msg_handler.hpp"
#include "robot_common/robot_common_struct.hpp"
#include "common/ScriptManager.hpp"
#include "script/game_script_func.h"
#include "config/config_manager.hpp"
#include <boost/property_tree/ini_parser.hpp>
#include <boost/property_tree/exceptions.hpp>
#include <boost/property_tree/ptree.hpp>
#include <stdio.h>
#include "msg_count.hpp"

USING_NS_COMMON;

static const std::string config_ini_path("./config.ini");


robot_server_t::robot_server_t() 
	: m_heart_timer(server_base_t::m_ios)
	, m_login_timer(server_base_t::m_ios)
{
}


robot_server_t::~robot_server_t()
{
}


const char* robot_server_t::server_name()
{
    return "robot_server";
}

bool robot_server_t::on_init_server()
{
	CMD_INIT;

    // load config.ini
    if (!load_ini(config_ini_path))
    {
        printf("load ini failed!\n");
        COMMON_ASSERT(false);
        return false;
    }

    // init log
    if (!g_log.open("./logs/", "robot", env::robot_config->wlog_trace, env::robot_config->wlog_debug, env::robot_config->wlog_info))
    {
        printf("init log failed!\n");
        COMMON_ASSERT(false);
        return false;
    }

	if (!load_config())
	{
		printf("load config failed!\n");
		COMMON_ASSERT(false);
		return false;
	}
	// init robotmanager
	if (!robot_manager_t::init_all_robots())
	{
		printf("init robot manager failed!\n");
		COMMON_ASSERT(false);
		return false;
	}

    // init network
    std::vector<std::string> ip_white_list;
    ip_white_list.clear();
    if (!m_network.init(env::robot_config->network, ip_white_list))
    {
        log_error("init network failed!");
        COMMON_ASSERT(false);
        return false;
    }

    // regist robot_msg_handler
    if (!robot_msg_handler_t::init_msg_handler())
    {
        log_error("init robot msg handler failed!");
        COMMON_ASSERT(false);
        return false;
    }

	ScriptManager::getInstance().init(std::string(c_get_root_dir()));

    return true;
}


bool robot_server_t::on_close_server()
{
    log_info("robot server is closing...");

	msg_count_t::write_msg_recv_count_data_file(env::robot_config->wlog_msg_count);
	msg_count_t::write_msg_send_count_data_file(env::robot_config->wlog_msg_count);

	// close network
	m_network.close();

    robot_manager_t::reset_all_robots();

    // stop timer
	stop_robot_login();
	stop_robot_heart();

    release_config();
    log_info("release config success!");

	ScriptManager::getInstance().close();
	ScriptManager::delInstance();

    // close log
    g_log.close();

	CMD_CLOSE;

    return true;
}


void robot_server_t::on_run_server()
{
    log_info("robot server is running...");

	robot_manager_t::on_robot_start_login();

	start_robot_login();
	start_robot_heart();
}


bool robot_server_t::load_ini(const std::string& ini_path)
{
    try
    {
        boost::property_tree::ptree tree;
        boost::property_tree::ini_parser::read_ini(ini_path, tree);

        env::robot_config->network = tree.get<uint32_t>("robot.network");

        std::string str_ip_list = tree.get<std::string>("robot.account_server_ip");
        std::vector<std::string> ip_list;
        string_util_t::split(str_ip_list, ip_list, ";");
        if (ip_list.empty())
        {
            printf("\033[31mconnect to account: ip_list is empty!\033[0m\n");
            return false;
        }
        std::string str_port_list = tree.get<std::string>("robot.account_server_port");
        std::vector<std::string> port_list;
        string_util_t::split(str_port_list, port_list, ";");
        if (port_list.empty())
        {
            printf("\033[31mconnect to account: port_list is empty!\033[0m\n");
            return false;
        }
        if (ip_list.size() != port_list.size())
        {
            printf("\033[31mconnect to account: ip_size[%d] != port_size[%d]\033[0m\n", uint32_t(ip_list.size()), uint32_t(port_list.size()));
            return false;
        }
        uint32_t rand_index = rand() % ip_list.size();
        env::robot_config->account_server_ip = ip_list[rand_index];
        env::robot_config->account_server_port = boost::lexical_cast<uint32_t>(port_list[rand_index]);

        env::robot_config->server_group_id = tree.get<uint32_t>("robot.server_group_id");
        env::robot_config->server_list_type = tree.get<std::string>("robot.server_list_type");

		env::robot_config->lua_open			= tree.get<uint32_t>("robot.lua_open");
		env::robot_config->login_interval	= tree.get<uint32_t>("robot.login_interval");
		env::robot_config->lua_heart		= tree.get<uint32_t>("robot.lua_heart");
		env::robot_config->heart_group		= tree.get<uint32_t>("robot.heart_group");
        env::robot_config->script_path      = tree.get<std::string>("robot.script_path");

		env::robot_config->frame_time = env::robot_config->lua_heart / env::robot_config->heart_group;
		// 当帧数低时强制设置为每10ms执行一次
		if (env::robot_config->frame_time < 10) env::robot_config->frame_time = 10;

        env::robot_config->wlog_info        = tree.get<int32_t>("log.info");
        env::robot_config->wlog_trace       = tree.get<int32_t>("log.trace");
		env::robot_config->wlog_debug		= tree.get<int32_t>("log.debug");
		env::robot_config->wlog_msg_count	= tree.get<bool>("log.msg_count");

		env::robot_config->gm_level			= tree.get<uint32_t>("robot.gm_level");

		env::robot_config->robot_name		= tree.get<std::string>("robot.robot_name");
    }
    catch (boost::property_tree::ptree_error& ec)
    {
       fprintf(stderr, "load ini failed! Message:%s", ec.what());
       return false;
    }

    return true; 
}


bool robot_server_t::connect(const std::string& ip, uint16_t port, network::connector_callback_t* connector_cb)
{
    return m_network.connect_to(ip, port, connector_cb, 0, true, false);
}

bool robot_server_t::disconnect(const network::tcp_socket_ptr& s)
{
    m_network.post_close_socket(s);

    return true;
}

bool robot_server_t::send_msg(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& buf)
{
    if (NULL == s)
    {
        log_error("NULL == s");
        return false;
    }
	msg_count_t::push_msg_send_info(buf, env::robot_config->wlog_msg_count);
    m_network.post_send_msg(s, buf);

    return true;
}

void robot_server_t::start_robot_heart()
{
	m_heart_timer.expires_from_now(boost::posix_time::millisec(env::robot_config->frame_time));
	m_heart_timer.async_wait(boost::bind(&robot_server_t::on_robot_heart, this, boost::asio::placeholders::error));
}

void robot_server_t::stop_robot_heart()
{
	m_heart_timer.cancel();
}

void robot_server_t::on_robot_heart(const boost::system::error_code& ec)
{
    if (ec == boost::asio::error::operation_aborted)
    {
        
        return;
    }
    else if (ec)
    {
        log_error("Timer error: %s", ec.message().c_str());
        return;
    }

	start_robot_heart();

	robot_manager_t::heartbeat();
}

void robot_server_t::on_robot_login(const boost::system::error_code& ec)
{
    if (ec == boost::asio::error::operation_aborted)
    {
        
        return;
    }
    else if (ec)
    {
        log_error("Timer error: %s", ec.message().c_str());
        return;
    }

#if 1
	if (robot_manager_t::loop_interval_login())
		start_robot_login();
#else
	if (robot_manager_t::loop_interval_login())
	{
		//start_robot_login();
	}
	else
	{
		if (robot_manager_t::loop_check_robot())
		{
			robot_manager_t::loop_login_out();
		}
	}

	start_robot_login();
#endif

}

void robot_server_t::start_robot_login()
{
	m_login_timer.expires_from_now(boost::posix_time::millisec(env::robot_config->login_interval));
	m_login_timer.async_wait(boost::bind(&robot_server_t::on_robot_login, this, boost::asio::placeholders::error));
}

void robot_server_t::stop_robot_login()
{
	m_login_timer.cancel();
}