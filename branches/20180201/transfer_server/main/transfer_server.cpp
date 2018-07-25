#include "transfer_server.hpp"
#include "transfer_server_fwd.hpp"
#include "log.hpp"
#include "chat/chat_msg_handler.hpp"
#include "chat/chat_manager.hpp"
#include "area/area_msg_handler.hpp"
#include "channel/chat_channel_manager.hpp"
#include "config/config_manager.hpp"
#include "center/center_msg_handler.hpp"
#include "msg_count.hpp"

USING_NS_NETWORK;
USING_NS_COMMON;

bool transfer_server_t::on_init_server()
{
    CMD_INIT;

    // xml
    if (!load_xml())
    {
        close_xml();
        COMMON_ASSERT(false);
        return false;
    }

    // log
    if (!g_log.open(env::xml->log_path, server_name(), env::xml->is_log_trace, env::xml->is_log_debug, env::xml->is_log_info, env::xml->is_new_logfile_per_hour))
    {
        COMMON_ASSERT(false);
        return false;
    }

    // config
    if (!load_config())
    {
        log_error("load config failed!");
        COMMON_ASSERT(false);
        return false;
    }

    // game logic
    if (!init_game_logic())
    {
        log_error("load game logic failed!");
        COMMON_ASSERT(false);
        return false;
    }

    // network
    if (!init_network())
    {
        COMMON_ASSERT(false);
        return false;
    }

    return true;
}

bool transfer_server_t::on_close_server()
{
    {
        release_config();
    }

    close_network();

    close_xml();

    g_log.close();

    CMD_CLOSE;

    return true;
}

void transfer_server_t::on_run_server()
{
    log_info("transfer[%d] server running...", env::server->get_id());
}

//----------------------------------------------------------------------------------
bool transfer_server_t::load_xml()
{
    if (!load_config_xml())
    {
        printf("\033[31mload [%s] failed!\033[0m\n", TRANSFER_XML_PATH);
        return false;
    }
    printf("load [%s] success!\n", TRANSFER_XML_PATH);

    if (!load_global_server_xml())
    {
        printf("\033[31mload [%s] failed!\033[0m\n", GLOBAL_SERVER_XML_PATH);
        return false;
    }
    printf("load [%s] success!\n", GLOBAL_SERVER_XML_PATH);

    return true;
}
bool transfer_server_t::load_config_xml()
{
    try
    {
        boost::property_tree::ptree pt_config;
        boost::property_tree::xml_parser::read_xml(TRANSFER_XML_PATH, pt_config);

        uint32_t id = pt_config.get<uint32_t>("server.transfer_server.id");
        env::server->set_id(id);
    }
    catch (boost::property_tree::ptree_error& e)
    {
        printf("\033[31mthrow error[%s]\033[0m\n", e.what());
        return false;
    }

    return true;
}
bool transfer_server_t::load_global_server_xml()
{
    try
    {
        boost::property_tree::ptree pt;
        boost::property_tree::xml_parser::read_xml(GLOBAL_SERVER_XML_PATH, pt);

		env::xml = new transfer_xml_t;

        env::xml->thread_num.network = pt.get<uint32_t>("server.transfer_server.thread_num.network");

        char buf[256];

        //listen at chat
        snprintf(buf, sizeof(buf), "server.transfer_server.transfer%d.listen.chat.ip", env::server->get_id());
        env::xml->listen_at_chat.ip = pt.get<std::string>(buf);

        snprintf(buf, sizeof(buf), "server.transfer_server.transfer%d.listen.chat.port", env::server->get_id());
        env::xml->listen_at_chat.port = pt.get<uint32_t>(buf);
        CHECK_PORT(env::xml->listen_at_chat.port);

        //listen at center
        snprintf(buf, sizeof(buf), "server.transfer_server.transfer%d.listen.center.ip", env::server->get_id());
        env::xml->listen_at_center.ip = pt.get<std::string>(buf);

        snprintf(buf, sizeof(buf), "server.transfer_server.transfer%d.listen.center.port", env::server->get_id());
        env::xml->listen_at_center.port = pt.get<uint32_t>(buf);
        CHECK_PORT(env::xml->listen_at_center.port);

        //connect to area
		env::xml->connect_to_area.ip = pt.get<std::string>("server.area_server.listen.transfer.ip");
		env::xml->connect_to_area.port = pt.get<uint32_t>("server.area_server.listen.transfer.port");
		CHECK_PORT(env::xml->connect_to_area.port);

        std::string str_ip_white_list = pt.get<std::string>("server.common.ip_white_list");
        string_util_t::split(str_ip_white_list, env::xml->ip_white_list, ";");
        if (env::xml->ip_white_list.empty())
        {
            printf("\033[31mip_white_list is empty!\033[0m\n");
            return false;
        }

        env::xml->log_path = pt.get<std::string>("server.common.log_path");
        env::xml->is_log_trace = pt.get<bool>("server.common.log_trace");
        env::xml->is_log_debug = pt.get<bool>("server.common.log_debug");
		env::xml->is_log_info = pt.get<bool>("server.common.log_info");
		env::xml->is_log_msg_count = pt.get<bool>("server.common.log_msg_count");
		env::xml->is_new_logfile_per_hour = pt.get<bool>("server.common.new_logfile_per_hour");
		env::xml->config_path = pt.get<std::string>("server.common.path.config");
		env::xml->reconnect_interval_time = pt.get<uint32_t>("server.common.reconnect_interval_time");
    }
    catch (boost::property_tree::ptree_error& e)
    {
        printf("\033[31mthrow error[%s]\033[0m\n", e.what());
        return false;
    }

    if (NULL == env::xml)
    {
        printf("\033[31menv::xml is NULL!\033[0m\n");
        return false;
    }

    return true;
}

void transfer_server_t::close_xml()
{
    SAFE_DELETE_P(env::xml);
    log_info("close xml success!");
}

//----------------------------------------------------------------------------------
bool transfer_server_t::init_network()
{
    if (!m_network.init(env::xml->thread_num.network, env::xml->ip_white_list))
    {
        log_error("init network failed!");
        return false;
    }

    // listen for chat
    if (!m_network.listen_at(env::xml->listen_at_chat.ip, env::xml->listen_at_chat.port, &m_chat_listener))
    {
        return false;
    }
    else
    {
        if (!chat_msg_handler_t::init_msg_handler())
        {
            log_error("init chat msg handler failed!");
            return false;
        }
    }

    // listen for center
    if (!m_network.listen_at(env::xml->listen_at_center.ip, env::xml->listen_at_center.port, &m_center_listener))
    {
        return false;
    }
    else
    {
        if (!center_msg_handler_t::init_msg_handler())
        {
            log_error("init center msg handler failed!");
            return false;
        }
    }

    // connect to area
    if (!m_network.connect_to(env::xml->connect_to_area.ip, env::xml->connect_to_area.port, &m_area_connector, env::xml->reconnect_interval_time))
    {
        return false;
    }
    else
    {
        if (!area_msg_handler_t::init_msg_handler())
        {
            log_error("init area msg handler failed!");
            return false;
        }
    }

    return true;
}

void transfer_server_t::close_network()
{
	msg_count_t::write_msg_recv_count_data_file(env::xml->is_log_msg_count);
	msg_count_t::write_msg_send_count_data_file(env::xml->is_log_msg_count);

    chat_manager_t::clear_chat();
    m_network.close();
}

//---------------------------------------------------------------------------------------------
bool transfer_server_t::init_game_logic()
{
    chat_channel_manager_t::init_channel();
    return true;
}

//---------------------------------------------------------------------------------
void transfer_server_t::on_add_chat(const network::tcp_socket_ptr& s)
{
    chat_manager_t::add_chat(s);

	//同步transfer服務器信息给chat服务器
	proto::server::fh_syn_transfer_info_notify noti;
    noti.set_server_id(env::server->get_id());
	send_msg_to_chat(s, op_cmd::fh_syn_transfer_info_notify, 0, noti);
}
void transfer_server_t::on_del_chat(const network::tcp_socket_ptr& s)
{
	chat_ptr p_chat = chat_manager_t::get_chat(s);
	if (p_chat == nullptr)
	{
		return;
	}
	chat_channel_manager_t::erase_channel(p_chat->get_channel_id());
    chat_manager_t::del_chat(s);

	log_info("delete chat[%u, %s], channel [%d]", p_chat->get_server_id(), p_chat->get_server_name().c_str(), p_chat->get_channel_id());
}
void transfer_server_t::on_chat_msg(const network::tcp_socket_ptr& s, uint64_t uid, uint16_t cmd, const msg_buf_ptr& msg_buf)
{
	msg_count_t::push_msg_recv_info(msg_buf, cmd, env::xml->is_log_msg_count);
	if (cmd > op_cmd::HR_BEGIN && cmd < op_cmd::HR_END)
	{
		send_msg_forward_area(cmd, uid, msg_buf);
	}
	else
	{
		chat_msg_handler_t::handle_server_msg(s, uid, cmd, msg_buf);
	}
}

//---------------------------------------------------------------------------------
void transfer_server_t::on_add_area(const network::tcp_socket_ptr& s)
{
	m_area_connector.set_socket(s);

	//同步transfer服務器信息给area服务器
	proto::server::fr_syn_transfer_info_request req;
    req.set_server_id(env::server->get_id());
	send_msg_to_area(op_cmd::fr_syn_transfer_info_request, 0, req);
}
void transfer_server_t::on_del_area(const network::tcp_socket_ptr& s)
{
	env::server->post_network_close_socket(s);
	m_area_connector.close_socket();
}
void transfer_server_t::on_area_msg(const network::tcp_socket_ptr& s, uint64_t uid, uint16_t cmd, const network::msg_buf_ptr& msg_buf)
{
	msg_count_t::push_msg_recv_info(msg_buf, cmd, env::xml->is_log_msg_count);
	area_msg_handler_t::handle_server_msg(s, uid, cmd, msg_buf);
}

//---------------------------------------------------------------------------------
void transfer_server_t::on_add_center(const network::tcp_socket_ptr& s)
{
	center_manager_t::add_center(s);

	//同步transfer服務器信息给center服务器
	proto::server::fe_syn_transfer_info_notify msg;
    msg.set_server_id(env::server->get_id());
    send_msg_to_center(s, op_cmd::fe_syn_transfer_info_notify, 0, msg);
}
void transfer_server_t::on_del_center(const network::tcp_socket_ptr& s)
{
	center_ptr p_center = center_manager_t::get_center_by_socket(s);
	if (p_center == nullptr)
	{
		return;
	}
	uint32_t server_id = p_center->get_id();
	center_manager_t::del_center(s);

	log_info("delete center[%d]", server_id);
}
void transfer_server_t::on_center_msg(const network::tcp_socket_ptr& s, uint64_t uid, uint16_t cmd, const msg_buf_ptr& msg_buf)
{
	msg_count_t::push_msg_recv_info(msg_buf, cmd, env::xml->is_log_msg_count);
	if ((cmd > op_cmd::GR_BEGIN && cmd < op_cmd::GR_END) || 
		(cmd > op_cmd::ER_BEGIN && cmd < op_cmd::ER_END) ||
		(cmd > op_cmd::HR_BEGIN && cmd < op_cmd::HR_END))
	{
		send_msg_forward_area(cmd, uid, msg_buf);
	}
	else
	{
		center_msg_handler_t::handle_server_msg(s, uid,cmd, msg_buf);
	}
}

void transfer_server_t::post_network_send_msg(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& buf)
{
	msg_count_t::push_msg_send_info(buf, env::xml->is_log_msg_count);
	m_network.post_send_msg(s, buf);
}
