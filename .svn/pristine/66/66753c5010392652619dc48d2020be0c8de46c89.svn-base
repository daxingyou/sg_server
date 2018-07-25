#include "gate_server.hpp"
#include "log.hpp"
#include "login/login_msg_handler.hpp"
#include "game/game_msg_handler.hpp"
#include "chat/chat_msg_handler.hpp"
#include "center/center_msg_handler.hpp"
#include "user/user_msg_handler.hpp"
#include "user/user_manager.hpp"
#include "gate_server_fwd.hpp"
#include "config/config_manager.hpp"
#include "msg_count.hpp"

USING_NS_NETWORK;
USING_NS_COMMON;

gate_server_t::gate_server_t()
{
}

gate_server_t::~gate_server_t()
{
}

//--------------------------------------------------------------------
bool gate_server_t::on_init_server()
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

bool gate_server_t::on_close_server()
{
    {
        user_manager_t::cancel_sync_user_num_to_login();
        release_config();
    }

    close_network();

    close_xml();

    g_log.close();

	CMD_CLOSE;

    return true;
}

void gate_server_t::on_run_server()
{
    log_info("gate[%d] server running...", env::server->get_id());
    //log_info("network thread num: %d", env::xml->thread_num.network);
}

//----------------------------------------------------------------------------------
bool gate_server_t::load_xml()
{
    if (!load_config_xml())
    {
        printf("\033[31mload [%s] failed!\033[0m\n", GATE_XML_PATH);
        return false;
    }
    printf("load [%s] success!\n", GATE_XML_PATH);

    if (!load_server_xml())
    {
        printf("\033[31mload [%s] failed!\033[0m\n", SERVER_XML_PATH);
        return false;
    }
    printf("load [%s] success!\n", SERVER_XML_PATH);

    return true;
}
bool gate_server_t::load_config_xml()
{
    if (env::server->get_id() > 0)
    {
        return true;
    }

    try
    {
        boost::property_tree::ptree pt_config;
        boost::property_tree::xml_parser::read_xml(GATE_XML_PATH, pt_config);
        uint32_t id = pt_config.get<uint32_t>("gate_server.id");
        env::server->set_id(id);
    }
    catch (boost::property_tree::ptree_error& e)
    {
        printf("\033[31mthrow error[%s]\033[0m\n", e.what());
        return false;
    }

    return true;
}
bool gate_server_t::load_server_xml()
{
    try
    {
        boost::property_tree::ptree pt;
        boost::property_tree::xml_parser::read_xml(SERVER_XML_PATH, pt);

        env::xml = new gate_xml_t;

        env::xml->srv_group.group_id = pt.get<uint32_t>("server.server_id");
        env::xml->srv_group.start_time = pt.get<std::string>("server.start_time");
        
        env::xml->thread_num.network = pt.get<int32_t>("server.gate_server.thread_num.network");
        //int32_t thread_num_network = sysconf(_SC_NPROCESSORS_CONF) * 2 + 1;
        //if (-1 == env::xml->thread_num.network || thread_num_network < env::xml->thread_num.network)
        //{
        //    env::xml->thread_num.network = thread_num_network;
        //}

        env::xml->timeout.notify_login_time = pt.get<uint32_t>("server.gate_server.timeout.notify_login_time");
        if (env::xml->timeout.notify_login_time <= 0)
        {
            printf("\033[31mnot open notify_login_client_num!\033[0m\n");
        }

		game_msg_handler_t::set_is_batch_move(pt.get<uint32_t>("server.gate_server.move_batch", 0));

        env::xml->timeout.session = pt.get<uint32_t>("server.gate_server.timeout.session");
        if (env::xml->timeout.session <= 0)
        {
            printf("\033[31mnot open client session timeout!\033[0m\n");
        }
        env::xml->timeout.alive = pt.get<uint32_t>("server.gate_server.timeout.alive");
        if (env::xml->timeout.alive <= 0)
        {
            printf("\033[31mnot open client cache alive!\033[0m\n");
        }
        env::xml->timeout.pingpong = pt.get<uint32_t>("server.gate_server.timeout.pingpong");
        if (env::xml->timeout.pingpong <= 0)
        {
            printf("\033[31mnot open client pingpong!\033[0m\n");
        }
        env::xml->timeout.reconnect = pt.get<uint32_t>("server.gate_server.timeout.reconnect");
        if (env::xml->timeout.reconnect <= 0)
        {
            printf("\033[31mnot open client reconnect!\033[0m\n");
        }
        env::xml->timeout.msg_bomb_interval = pt.get<uint32_t>("server.gate_server.timeout.msg_bomb_interval");
        env::xml->timeout.msg_bomb_count = pt.get<uint32_t>("server.gate_server.timeout.msg_bomb_count");

        char buf[256];
        snprintf(buf, sizeof(buf), "server.gate_server.gate%d.listen.client.ip", env::server->get_id());
        env::xml->listen_at_client.ip = pt.get<std::string>(buf);

        snprintf(buf, sizeof(buf), "server.gate_server.gate%d.listen.client.port", env::server->get_id());
        env::xml->listen_at_client.port = pt.get<uint32_t>(buf);
        CHECK_PORT(env::xml->listen_at_client.port);

        snprintf(buf, sizeof(buf), "server.gate_server.gate%d.listen.client.out_ip", env::server->get_id());
        env::xml->listen_at_client.out_ip = pt.get<std::string>(buf);

        snprintf(buf, sizeof(buf), "server.gate_server.gate%d.listen.client.out_port", env::server->get_id());
        env::xml->listen_at_client.out_port = pt.get<uint32_t>(buf);
        CHECK_PORT(env::xml->listen_at_client.out_port);

        //listen at game
        snprintf(buf, sizeof(buf), "server.gate_server.gate%d.listen.game.ip", env::server->get_id());
        env::xml->listen_at_game.ip = pt.get<std::string>(buf);

        snprintf(buf, sizeof(buf), "server.gate_server.gate%d.listen.game.port", env::server->get_id());
        env::xml->listen_at_game.port = pt.get<uint32_t>(buf);
        CHECK_PORT(env::xml->listen_at_game.port);
        
        //connect to cross(game copy)`
        {
            std::string str_ip_list = pt.get<std::string>("server.gate_server.connect.cross.ip");
            std::vector<std::string> ip_list;
            string_util_t::split(str_ip_list, ip_list, ";");
            if (ip_list.empty())
            {
                printf("\033[31mconnect to cross: ip_list is empty!\033[0m\n");
                return false;
            }

            std::string str_port_list = pt.get<std::string>("server.gate_server.connect.cross.port");
            std::vector<std::string> port_list;
            string_util_t::split(str_port_list, port_list, ";");
            if (port_list.empty())
            {
                printf("\033[31mconnect to cross: port_list is empty!\033[0m\n");
                return false;
            }

            if (ip_list.size() != port_list.size())
            {
                printf("\033[31mconnect to cross: ip_size[%d] != port_size[%d]\033[0m\n", uint32_t(ip_list.size()), uint32_t(port_list.size()));
                return false;
            }

            for (uint32_t i = 0; i < ip_list.size(); ++i)
            {
                connect_t game_connect;
                game_connect.ip = ip_list[i];
                game_connect.port = boost::lexical_cast<uint32_t>(port_list[i]);
                CHECK_PORT(game_connect.port);
                env::xml->connect_to_cross_list.push_back(game_connect);
            }
            m_cross_connector_list.resize(env::xml->connect_to_cross_list.size());
        }
        

        env::xml->connect_to_login.ip = pt.get<std::string>("server.login_server.listen.gate.ip");
        env::xml->connect_to_login.port = pt.get<uint32_t>("server.login_server.listen.gate.port");
        CHECK_PORT(env::xml->connect_to_login.port);

        env::xml->connect_to_chat.ip = pt.get<std::string>("server.chat_server.listen.gate.ip");
        env::xml->connect_to_chat.port = pt.get<uint32_t>("server.chat_server.listen.gate.port");
        CHECK_PORT(env::xml->connect_to_chat.port);

        env::xml->connect_to_center.ip = pt.get<std::string>("server.center_server.listen.gate.ip");
        env::xml->connect_to_center.port = pt.get<uint32_t>("server.center_server.listen.gate.port");
        CHECK_PORT(env::xml->connect_to_center.port);

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
        env::xml->reconnect_interval_time = pt.get<uint32_t>("server.common.reconnect_interval_time");
        env::xml->config_path = pt.get<std::string>("server.common.path.config");

		env::xml->log_debug.role = pt.get<bool>("server.common.log.role");
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
void gate_server_t::close_xml()
{
    SAFE_DELETE_P(env::xml);
    log_info("close xml success!");
}

//--------------------------------------------------------------------
bool gate_server_t::init_network()
{
    if (!m_network.init(env::xml->thread_num.network, env::xml->ip_white_list))
    {
        log_error("init network failed!");
        return false;
    }

    // listen for client
    if (!m_network.listen_at(env::xml->listen_at_client.ip, env::xml->listen_at_client.port, &m_user_listener))
    {
        return false;
    }
    else
    {
        if (!user_msg_handler_t::init_msg_handler())
        {
            log_error("init client msg handler failed!");
            return false;
        }
    }

    // connect to login
    if (!m_network.connect_to(env::xml->connect_to_login.ip, env::xml->connect_to_login.port, &m_login_connector, env::xml->reconnect_interval_time))
    {
        return false;
    }
    else
    {
        if (!login_msg_handler_t::init_msg_handler())
        {
            log_error("init login msg handler failed!");
            return false;
        }
    }

    // connect to chat
    if (!m_network.connect_to(env::xml->connect_to_chat.ip, env::xml->connect_to_chat.port, &m_chat_connector, env::xml->reconnect_interval_time))
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

    // connect to center
    if (!m_network.connect_to(env::xml->connect_to_center.ip, env::xml->connect_to_center.port, &m_center_connector, env::xml->reconnect_interval_time))
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

    // listen for game
    if (!m_network.listen_at(env::xml->listen_at_game.ip, env::xml->listen_at_game.port, &m_game_listener))
    {
        return false;
    }
    // connect to cross
    for (size_t index = 0; index < env::xml->connect_to_cross_list.size(); ++index)
    {
        connect_t connector = env::xml->connect_to_cross_list.at(index);
        if (!m_network.connect_to(connector.ip, connector.port, &m_cross_connector_list[index], env::xml->reconnect_interval_time))
        {
            return false;
        }
    }
    // game cross共用一套消息回调接口
    if (!game_msg_handler_t::init_msg_handler())
    {
        log_error("init game|cross msg handler failed!");
        return false;
    }

    return true;
}
void gate_server_t::close_network()
{
	msg_count_t::write_msg_recv_count_data_file(env::xml->is_log_msg_count);
	msg_count_t::write_msg_send_count_data_file(env::xml->is_log_msg_count);

	//TODO:qhj login 断开自己能侦测到(但是login那里貌似没做相应的处理)
    user_manager_t::clear_all_users_and_socket(false, KICK_ROLE_REASON_GATE_SERVER_CLOSED);

	game_manager_t::clear_all_game();

    m_network.close();
}

//--------------------------------------------------------------------------------
void gate_server_t::on_add_client(const tcp_socket_ptr& s)
{
    user_manager_t::add_socket(s);
}
//由客户端主动关闭连接
void gate_server_t::on_del_client(const tcp_socket_ptr& s)
{
    const user_ptr& p_user = user_manager_t::get_user(s);
    if (NULL == p_user)
    {
		// 删除socket
		user_manager_t::del_socket(s, true);
        return;
    }

	user_manager_t::kick_user(s, p_user, common::KICK_ROLE_REASON_NORMAL);
}

void gate_server_t::on_client_msg(const tcp_socket_ptr& s,uint64_t uid, uint16_t cmd, const msg_buf_ptr& msg_buf)
{
	if (NULL == s)
	{
		log_error("NULL == s");
		return;
	}

	socket_data_t* p_socket_data = (socket_data_t*)s->get_bind_data();
	if (NULL == p_socket_data)
	{
		log_error("NULL == p_socket_data");
		return;
	}
	const user_ptr& p_user = p_socket_data->get_user();
	// 检查是否为消息轰炸
	if (!p_socket_data->check_msg_bomb(cmd))
	{
		if (NULL != p_user)
		{
			log_error("msg cmd[%s:%d] bomb, and kick the user[%lu] !!!!!!!!!!", CMD_DESC(cmd), cmd, p_user->get_uid());

			user_manager_t::kick_user(s, p_user, common::KICK_ROLE_REASON_MSG_BOMB);

		}
		return;
	}

	msg_count_t::push_msg_recv_info(msg_buf, cmd, env::xml->is_log_msg_count);

	
	// 当没有绑定user，只接收ca_connect_request其他一律不处理
	// 当用户已经登陆gate，还未登陆场景
	if (NULL == p_user || !p_user->is_play_state())
	{
		// 不是游戏状态抛弃任何除gate以外的消息
		if (cmd > op_cmd::CA_BEGIN && cmd < op_cmd::CA_END)
		{
			user_msg_handler_t::m_user_msg_handle.handle_client_msg(s, uid,cmd, msg_buf);
		}
		else
		{
            try
            {
                log_warn("recv role[%lu] socket[%d] abandon the message[%s:%d] , it's not in play state", uid, s->socket().native(), CMD_DESC(cmd), cmd);
            }
            catch (boost::system::system_error &ec)
            {
                log_warn("recv role[%lu] abandon the message[%s:%d], it's not in play state", uid, CMD_DESC(cmd), cmd);
            }
		}
	}
	else
	{
		if (cmd > op_cmd::CG_BEGIN && cmd < op_cmd::CG_END)
		{
			send_msg_from_client_to_game(p_user, cmd, msg_buf);
		}
		else if (cmd > op_cmd::CH_BEGIN && cmd < op_cmd::CH_END)
		{
			send_msg_from_client_to_chat(p_user, cmd, msg_buf);
		}
		else if (cmd > op_cmd::CE_BEGIN && cmd < op_cmd::CE_END)
		{
			send_msg_from_client_to_center(p_user, cmd, msg_buf);
		}
		else if (cmd > op_cmd::CA_BEGIN && cmd < op_cmd::CA_END)
		{
			user_msg_handler_t::m_user_msg_handle.handle_client_msg(s,uid, cmd, msg_buf);
		}
		else
		{
			log_error("user[%lu] error cmd[%d]", p_user->get_uid(), cmd);
		}
	}
}

//--------------------------------------------------------------------------------
void gate_server_t::on_add_login(const tcp_socket_ptr& s)
{
    m_login_connector.set_socket(s);

    syn_gate_info_to_login();
    
    user_manager_t::start_sync_user_num_to_login_timer();
}
void gate_server_t::on_del_login(const tcp_socket_ptr& s)
{
    user_manager_t::cancel_sync_user_num_to_login();

    env::server->post_network_close_socket(s);

    m_login_connector.close_socket();
}
void gate_server_t::on_login_msg(uint64_t uid,uint16_t cmd, const msg_buf_ptr& msg_buf)
{
	msg_count_t::push_msg_recv_info(msg_buf, cmd, env::xml->is_log_msg_count);
    login_msg_handler_t::handle_server_msg(uid,cmd, msg_buf);
}
//同步gate信息给login服务器
void gate_server_t::syn_gate_info_to_login()
{
    proto::server::al_syn_gate_info_notify noti;
    noti.set_gate_id(env::server->get_id());
    user_map::iterator itr = user_manager_t::m_user_list.begin();
    user_map::iterator itr_end = user_manager_t::m_user_list.end();
    for (; itr != itr_end; ++itr)
    {
        user_ptr p_user = itr->second;
        if (NULL != p_user)
        {
            noti.add_user_list(p_user->get_uid());
        }
    }
    env::server->send_msg_to_login(op_cmd::al_syn_gate_info_notify, 0, noti);
}
//--------------------------------------------------------------------------------
void gate_server_t::on_add_game(const tcp_socket_ptr& s)
{
    game_manager_t::add_game(s);

    // 同步gate信息给game
    proto::server::ag_syn_gate_info_notify syn_msg;
    syn_msg.set_gate_id(env::server->get_id());
	syn_msg.set_server_id(env::xml->srv_group.group_id);
    env::server->send_msg_to_game(s, op_cmd::ag_syn_gate_info_notify, 0, syn_msg);
}
void gate_server_t::on_del_game(const tcp_socket_ptr& s)
{
    game_manager_t::del_game(s);
}
void gate_server_t::on_game_msg(const tcp_socket_ptr& s, uint16_t cmd, const msg_buf_ptr& msg_buf, uint64_t uid)
{
	msg_count_t::push_msg_recv_info(msg_buf, cmd, env::xml->is_log_msg_count);
    if (cmd > op_cmd::GC_BEGIN && cmd < op_cmd::GC_END)
    {
        send_msg_to_client(cmd, msg_buf, uid);
    }
    else
    {
        game_msg_handler_t::handle_server_msg(s, uid, cmd, msg_buf);
    }
}

//--------------------------------------------------------------------------------
void gate_server_t::on_add_chat(const tcp_socket_ptr& s)
{
    m_chat_connector.set_socket(s);

    // 同步gate信息给chat
    proto::server::ah_syn_gate_info_notify syn_msg;
    syn_msg.set_gate_id(env::server->get_id());
    env::server->send_msg_to_chat(op_cmd::ah_syn_gate_info_notify, 0, syn_msg);
}

void gate_server_t::on_del_chat(const tcp_socket_ptr& s)
{
    env::server->post_network_close_socket(s);
    m_chat_connector.close_socket();
}
void gate_server_t::on_chat_msg(uint16_t cmd, const msg_buf_ptr& msg_buf, uint64_t uid)
{
	msg_count_t::push_msg_recv_info(msg_buf, cmd, env::xml->is_log_msg_count);
    if (cmd > op_cmd::HC_BEGIN && cmd < op_cmd::HC_END)
    {
        send_msg_to_client(cmd, msg_buf, uid);
    }
    else
    {
        chat_msg_handler_t::handle_server_msg(uid,cmd, msg_buf);
    }
}

//--------------------------------------------------------------------------------
void gate_server_t::on_add_center(const tcp_socket_ptr& s)
{
    m_center_connector.set_socket(s);

    // 同步gate信息给center
    proto::server::ae_syn_gate_info_notify syn_msg;
    syn_msg.set_gate_id(env::server->get_id());
    env::server->send_msg_to_center(op_cmd::ae_syn_gate_info_notify, 0, syn_msg);
}
void gate_server_t::on_del_center(const tcp_socket_ptr& s)
{
    env::server->post_network_close_socket(s);
    m_center_connector.close_socket();

	user_manager_t::clear_all_users_and_socket(true, KICK_ROLE_REASON_CENTER_SERVER_CLOSED);
}
void gate_server_t::on_center_msg(uint16_t cmd, const msg_buf_ptr& msg_buf, uint64_t uid)
{
	msg_count_t::push_msg_recv_info(msg_buf, cmd, env::xml->is_log_msg_count);
    if (cmd > op_cmd::EC_BEGIN && cmd < op_cmd::EC_END)
    {
        send_msg_to_client(cmd, msg_buf, uid);
    }
    else
    {
        center_msg_handler_t::handle_server_msg(cmd, msg_buf, uid);
    }
}

void gate_server_t::post_network_send_msg(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& buf)
{
	msg_count_t::push_msg_send_info(buf, env::xml->is_log_msg_count);
	m_network.post_send_msg(s, buf);
}
