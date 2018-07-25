#ifndef __GAME_SERVER_HPP__
#define __GAME_SERVER_HPP__

#include "server_base.hpp"
#include "protobuf.hpp"
#include "network.hpp"
#include "cmd_desc.hpp"
#include "game_server_fwd.hpp"
#include "gate/gate_listener.hpp"
#include "gate/gate_manager.hpp"
#include "db/db_connector.hpp"
#include "center/center_connector.hpp"
#include "log/log_connector.hpp"
#include "chat/chat_connector.hpp"
#include "area/area_connector.hpp"
#include "gate/gate_connector.hpp"

enum game_server_type
{
	game_server_type_normal = 0,	// 普通
	game_server_type_cross = 1,		// 跨服
};

class game_server_t
    : public common::server_base_t
{
public:
    game_server_t();
    virtual ~game_server_t();

public:
    virtual const char* server_name() { return "game"; }
    virtual bool on_init_server();
    virtual bool on_close_server();
    virtual void on_run_server();

    void really_save_data_to_db();

private:
    bool load_xml();
    bool load_global_server_xml();
    bool load_server_xml();
    bool load_channel_xml() { return true; }
    bool load_config_xml();
    void close_xml();

    bool init_network();
    void close_network();

	bool init_logic();
    bool init_game_logic();
    void clear_game_logic();

	bool init_cross_logic();
	void clear_cross_logic();
public:
	void post_network_send_msg(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& buf);

    void post_network_close_socket(const network::tcp_socket_ptr& s) { m_network.post_close_socket(s); }

public:
    /*
    ****gate 的消息处理
    */
    void on_add_gate(const network::tcp_socket_ptr& s);
    void on_del_gate(const network::tcp_socket_ptr& s);
    void on_gate_msg(const network::tcp_socket_ptr& s, uint16_t cmd, const network::msg_buf_ptr& msg_buf, uint64_t uid);

    template<typename T_MSG>
    void send_msg_to_gate(const network::tcp_socket_ptr& s, uint16_t cmd, uint64_t uid, const T_MSG& msg)
    {
        if (NULL == s)
        {
            log_error("send uid[%lu] cmd[%s:%d] to gate failed: NULL == s", uid, CMD_DESC(cmd), cmd);
            return;
        }
        gate_ptr p_gate = gate_manager_t::get_gate_by_socket(s);
        if (NULL == p_gate)
        {
            log_error("send uid[%lu] cmd[%s:%d] to gate failed: NULL == p_gate", uid, CMD_DESC(cmd), cmd);
            return;
        }

        uint32_t gate_id = p_gate->get_id();
        uint32_t server_id = p_gate->get_server_id();

        network::msg_buf_ptr send_buf = network::wrap_msg<T_MSG>(cmd, uid, msg);
        if (NULL == send_buf)
        {
            log_error("send uid[%lu] cmd[%s:%d] to gate[server_id=%d: gate_id=%d] failed: NULL == send_buf", uid, CMD_DESC(cmd), cmd, server_id, gate_id);
            return;
        }

        post_network_send_msg(s, send_buf);

        switch (cmd)
        {
        case op_cmd::gc_role_outo_vision_notify:
        case op_cmd::gc_role_into_vision_notify:
        case op_cmd::ga_role_move_notify:
        case op_cmd::ga_role_into_vision_notify:
        case op_cmd::ga_role_out_vision_notify:
        case op_cmd::ga_broadcast_notify:
            break;
        default:
            log_trace("send role[%lu] cmd[%s:%d] to gate[server_id=%d: gate_id=%d]", uid, CMD_DESC(cmd), cmd, server_id, gate_id);
            break;
        }
    }

    template<typename T_MSG>
    void send_msg_to_all_gate(uint16_t cmd, uint64_t uid, const T_MSG& msg)
    {
        for (auto p_gate : gate_manager_t::get_gate_list())
        {
            if (NULL == p_gate)
            {
                log_error("send role[%lu] cmd[%s:%d] to gate failed: NULL == p_gate", uid, CMD_DESC(cmd), cmd);
                continue;
            }
            send_msg_to_gate(p_gate->get_socket(), cmd, uid, msg);
        }
    }

    template<typename T_BUF>
    void send_msg_from_gm_to_gate(uint16_t cmd, uint64_t uid, const T_BUF& msg_buf)
    {
        network::msg_buf_ptr send_buf = network::re_wrap_msg_s<T_BUF>(cmd, msg_buf);
        if (NULL == send_buf)
        {
            log_error("send role[%lu] cmd[%s:%d] to gate failed: NULL == send_buf", uid, CMD_DESC(cmd), cmd);
            return;
        }

        for (auto p_gate : gate_manager_t::get_gate_list())
        {
            if (NULL == p_gate)
            {
                log_error("send uid[%lu] cmd[%s:%d] from gm to gate failed: NULL == p_gate", uid, CMD_DESC(cmd), cmd);
                continue;
            }
            post_network_send_msg(p_gate->get_socket(), send_buf);
            log_trace("send uid[%lu] cmd[%s:%d] from gm to gate[server_id=%d: gate_id=%d]", uid, CMD_DESC(cmd), cmd, p_gate->get_server_id(), p_gate->get_id());
        }
    }

public:
    /*
    ****db 的消息处理
    */
    void on_add_db(const network::tcp_socket_ptr& s);
    void on_del_db(const network::tcp_socket_ptr& s);
    void on_db_msg(uint64_t uid, uint16_t cmd, const network::msg_buf_ptr& msg_buf);

    template<typename T_MSG>
    void send_msg_to_db(uint16_t cmd, uint64_t uid, const T_MSG& msg)
    {
        network::msg_buf_ptr send_buf = network::wrap_msg<T_MSG>(cmd, uid, msg);
        if (NULL == send_buf)
        {
            log_error("send uid[%lu] cmd[%s:%d] from gm to db failed: NULL == send_buf", uid, CMD_DESC(cmd), cmd);
            return;
        }
		post_network_send_msg(m_db_connector.get_socket(), send_buf);
       // m_db_connector.send_msg_to_db(send_buf);

        switch (cmd)
        {
        case op_cmd::gd_batch_save_to_db_notify:
            break;
        default:
            log_trace("send role[%lu] cmd[%s:%d] to db", uid, CMD_DESC(cmd), cmd);
            break;
        }
    }

    template<typename T_MSG>
    void send_msg_to_db(uint16_t cmd, const T_MSG& msg)
    {
        network::msg_buf_ptr send_buf = network::wrap_msg<T_MSG>(cmd, msg);
        if (NULL == send_buf)
        {
            log_error("send cmd[%s:%d] from gm to db failed: NULL == send_buf", CMD_DESC(cmd), cmd);
            return;
		}
		post_network_send_msg(m_db_connector.get_socket(), send_buf);
        //m_db_connector.send_msg_to_db(send_buf);

        switch (cmd)
        {
        case op_cmd::gd_batch_save_to_db_notify:
            break;
        default:
            log_trace("send cmd[%s:%d] to db", CMD_DESC(cmd), cmd);
            break;
        }
    }

public:
    /*
    ****center 的消息处理
    */
    void on_add_center(const network::tcp_socket_ptr& s);
    void on_del_center(const network::tcp_socket_ptr& s);
    void on_center_msg(uint64_t uid, uint16_t cmd, const network::msg_buf_ptr& msg_buf);

    template<typename T_MSG>
    void send_msg_to_center(uint16_t cmd, uint64_t uid, const T_MSG& msg)
    {
        network::msg_buf_ptr send_buf = network::wrap_msg<T_MSG>(cmd, uid, msg);
        if (NULL == send_buf)
        {
            log_error("send uid[%lu] cmd[%s:%d] to center failed: NULL == send_buf", uid, CMD_DESC(cmd), cmd);
            return;
        }
		// m_center_connector.send_msg_to_center(send_buf);
		post_network_send_msg(m_center_connector.get_socket(), send_buf);
        log_trace("send role[%lu] cmd[%s:%d] to center", uid, CMD_DESC(cmd), cmd);
    }

public:
    /*
    ****chat 的消息处理
    */
    void on_add_chat(const network::tcp_socket_ptr& s);
    void on_del_chat(const network::tcp_socket_ptr& s);
    void on_chat_msg(uint64_t uid, uint16_t cmd, const network::msg_buf_ptr& msg_buf);
    void on_chat_msg(uint16_t cmd, const network::msg_buf_ptr& msg_buf);

    template<typename T_MSG>
    void send_msg_to_chat(uint16_t cmd, uint64_t uid, const T_MSG& msg)
    {
        network::msg_buf_ptr send_buf = network::wrap_msg<T_MSG>(cmd, uid, msg);
        if (NULL == send_buf)
        {
            log_error("send uid[%lu] cmd[%s:%d] to chat failed: NULL == send_buf", uid, CMD_DESC(cmd), cmd);
            return;
        }
		//m_chat_connector.send_msg_to_chat(send_buf);
		post_network_send_msg(m_chat_connector.get_socket(), send_buf);
        log_trace("send role[%lu] cmd[%s:%d] to chat", uid, CMD_DESC(cmd), cmd);
    }

public:
    /*
    ****log 的消息处理
    */
    void on_add_log(const network::tcp_socket_ptr& s);
    void on_del_log(const network::tcp_socket_ptr& s);

    template<typename T_MSG>
    void send_msg_to_log(uint16_t cmd, const T_MSG& msg)
    {
        network::msg_buf_ptr send_buf = network::wrap_msg<T_MSG>(cmd, msg);
        if (NULL == send_buf)
        {
            log_error("send cmd[%s:%d] to log: NULL == send_buf", CMD_DESC(cmd), cmd);
            return;
        }
		//m_log_connector.send_msg_to_log(send_buf);
		post_network_send_msg(m_log_connector.get_socket(), send_buf);
        if (op_cmd::go_batch_save_logs_notify != cmd)
        {
            log_trace("send cmd[%s:%d] to log", CMD_DESC(cmd), cmd);
        }
    }

public:
    /*
    ****area 的消息处理
    */
	void on_add_area(const network::tcp_socket_ptr& s);
	void on_del_area(const network::tcp_socket_ptr& s);
	void on_area_msg(const network::tcp_socket_ptr& s, uint16_t cmd, const network::msg_buf_ptr& msg_buf, uint64_t uid);

	template<typename T_MSG>
	void send_msg_to_area(uint16_t cmd, uint64_t uid, const T_MSG& msg)
	{
		if (env::server->get_type() == game_server_type_cross)
		{
			network::msg_buf_ptr send_buf = network::wrap_msg<T_MSG>(cmd, uid, msg);
			if (NULL == send_buf)
			{
                log_error("send uid[%lu] cmd[%s:%d] to area: NULL == send_buf", uid, CMD_DESC(cmd), cmd);
                return;
			}
			//m_area_connector.send_msg_to_area(send_buf);
			post_network_send_msg(m_area_connector.get_socket(), send_buf);
			log_trace("send role[%lu] cmd[%s:%d] to area", uid, CMD_DESC(cmd), cmd);
		}
		else
		{
			send_msg_to_center(cmd, uid, msg);
		}
	}

public:
    static void one_day(uint32_t id, bool is_in_time);

public:
    uint32_t get_id() const { return m_id; }
    void set_id(uint32_t val) { m_id = val; }

	uint32_t get_type() const { return m_type; }
	void set_type(uint32_t val) { m_type = val; }

	common::GAME_SERVER_TYPE get_server_type() const { return m_server_type; }
	void set_server_type(common::GAME_SERVER_TYPE type) { m_server_type = type; }

private:
    uint32_t m_id = 0;
	uint32_t m_type = 0;
	common::GAME_SERVER_TYPE m_server_type = common::GAME_SERVER_NONE;
    network::network_t  m_network;
    gate_listener_t		 m_gate_listener;
    std::vector<gate_connector_t> m_gate_connector_list;
    db_connector_t      m_db_connector;
    center_connector_t  m_center_connector;
    chat_connector_t     m_chat_connector;
	area_connector_t      m_area_connector;
    log_connector_t      m_log_connector;
};

#endif
