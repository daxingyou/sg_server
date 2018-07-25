#ifndef __CENTER_CENTER_SERVER_HPP__
#define __CENTER_CENTER_SERVER_HPP__

#include "common/server_base.hpp"
#include "common/zdb_ex.hpp"
#include "network/protobuf.hpp"
#include "network/network.hpp"
#include "gate/gate_listener.hpp"
#include "gate/gate_manager.hpp"
#include "game/game_listener.hpp"
#include "game/game_manager.hpp"
#include "log/log_connector.hpp"
#include "db/db_connector.hpp"
#include "gm/gm_listener.hpp"
#include "cmd_desc.hpp"
#include "chat/chat_listener.hpp"
#include "transfer/transfer_connector.hpp"
#include "transfer/transfer_manager.hpp"
#include "protocpp/protos_fwd.hpp"

class center_server_t
    : public common::server_base_t
{
public:
    center_server_t();
    virtual ~center_server_t();

public:
    virtual const char* server_name() { return "center"; }
    virtual bool on_init_server();
    virtual bool on_close_server();
    virtual void on_run_server();

private:
    bool load_xml();
    
    bool load_channel_xml();
    void close_xml();

    bool init_zdb();
    void close_zdb();

    bool init_redis();
    void close_redis();

    bool init_network();
	void stop_network();
    void close_network();

    bool init_game_logic();
    void clear_game_logic();

public:
    bool load_server_xml(bool reload = false);
	void post_network_send_msg(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& buf);

    void post_network_close_socket(const network::tcp_socket_ptr& s) { m_network.post_close_socket(s); }

public:
    /*
    ****gate 的消息处理
    */
    void on_add_gate(const network::tcp_socket_ptr& s);
    void on_del_gate(const network::tcp_socket_ptr& s);
    void on_gate_msg(const network::tcp_socket_ptr& s, uint64_t uid, uint16_t cmd, const network::msg_buf_ptr& msg_buf);

    template<typename T_MSG>
    void send_msg_to_gate(const network::tcp_socket_ptr& s, uint16_t cmd, uint64_t uid, const T_MSG& msg)
    {
        if (NULL == s)
        {
            log_error("send uid[%lu] cmd[%s:%d] to gate failed: NULL == s", uid, CMD_DESC(cmd), cmd);
            return;
        }

        network::msg_buf_ptr send_buf = network::wrap_msg<T_MSG>(cmd, uid, msg);
        if (NULL == send_buf)
        {
            log_error("send uid[%lu] cmd[%s:%d] to gate failed: NULL == send_buf", uid, CMD_DESC(cmd), cmd);
            return;
        }
        //m_gate_listener.send_msg_to_gate(s, send_buf);
		post_network_send_msg(s, send_buf);

        switch (cmd)
        {
        case op_cmd::ea_broadcast_notify:
            break;
        default:
            log_trace("send user[%lu] cmd[%s:%d] to gate[%d]", uid, CMD_DESC(cmd), cmd, gate_manager_t::get_gate_id_by_socket(s));
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
                log_error("send uid[%lu] cmd[%s:%d] to all gates failed: NULL == p_gate", uid, CMD_DESC(cmd), cmd);
                continue;
            }
            
            send_msg_to_gate(p_gate->get_socket(), cmd, uid, msg);
        }
    }

	template<typename T_MSG>
	void send_msg_to_one_gate(uint16_t cmd, uint64_t uid, const T_MSG& msg)
	{
		for (auto p_gate : gate_manager_t::get_gate_list())
		{
			if (NULL == p_gate)
			{
				log_error("send role[%lu] cmd[%s:%d] to gate failed: NULL == p_gate", uid, CMD_DESC(cmd), cmd);
				continue;
			}
			send_msg_to_gate(p_gate->get_socket(), cmd, uid, msg);
			break;
		}
	}

    template<typename T_BUF>
    void send_msg_from_gm_to_gate(const network::tcp_socket_ptr& s, uint16_t cmd, uint64_t uid, const T_BUF& msg_buf)
    {
        network::msg_buf_ptr send_buf = network::re_wrap_msg_s<T_BUF>(cmd, msg_buf);
        if (NULL == send_buf)
        {
            log_error("send uid[%lu] cmd[%s:%d] from gm to gate failed: NULL == send_buf", uid, CMD_DESC(cmd), cmd);
            return;
        }
		//m_gate_listener.send_msg_to_gate(s, send_buf);
		post_network_send_msg(s, send_buf);
        log_trace("send user[%lu] cmd[%s:%d] from gm to gate[%d]", uid, CMD_DESC(cmd), cmd, gate_manager_t::get_gate_id_by_socket(s));
    }

    template<typename T_BUF>
    void send_msg_from_gm_to_all_gates(uint16_t cmd, const T_BUF& msg_buf)
    {
        network::msg_buf_ptr send_buf = network::re_wrap_msg_s<T_BUF>(cmd, msg_buf);
        if (NULL == send_buf)
        {
            log_error("send cmd[%s:%d] from gm to all gates failed: NULL == send_buf", CMD_DESC(cmd), cmd);
            return;
        }
        for (auto p_gate : gate_manager_t::get_gate_list())
        {
            if (NULL == p_gate->get_socket())
            {
                log_error("send cmd[%s:%d] from gm to all gates failed: NULL == p_gate->get_socket()", CMD_DESC(cmd), cmd);
                continue;
            }

			// m_gate_listener.send_msg_to_gate(p_gate->get_socket(), send_buf);
			post_network_send_msg(p_gate->get_socket(), send_buf);
            log_trace("send cmd[%s:%d] from gm to gate[%d]", CMD_DESC(cmd), cmd, p_gate->get_id());
        }
    }

public:
    /*
    ****game 的消息处理
    */
    void on_add_game(const network::tcp_socket_ptr& s);
    void on_del_game(const network::tcp_socket_ptr& s);
    void on_game_msg(const network::tcp_socket_ptr& s, uint64_t uid, uint16_t cmd, const network::msg_buf_ptr& msg_buf);

    template<typename T_MSG>
    void send_msg_to_game(const network::tcp_socket_ptr& s, uint16_t cmd, uint64_t uid, const T_MSG& msg)
    {
        if (NULL == s)
        {
            log_error("send uid[%lu] cmd[%s:%d] to game failed: NULL == s", uid, CMD_DESC(cmd), cmd);
            return;
        }
        network::msg_buf_ptr send_buf = network::wrap_msg<T_MSG>(cmd, uid, msg);
        if (NULL == send_buf)
        {
            log_error("send uid[%lu] cmd[%s:%d] to game failed: NULL == send_buf", uid, CMD_DESC(cmd), cmd);
            return;
        }
		//m_game_listener.send_msg_to_game(s, send_buf);
		post_network_send_msg(s, send_buf);
        log_trace("send user[%lu] cmd[%s:%d] to game[%d]", uid, CMD_DESC(cmd), cmd, game_manager_t::get_game_id_by_socket(s));
    }

    template<typename T_MSG>
    void send_msg_to_game(uint32_t server_id, uint16_t cmd, uint64_t uid, const T_MSG& msg)
    {
        const network::tcp_socket_ptr s = game_manager_t::get_game_socket_by_id(server_id);
        if (NULL == s)
        {
            log_error("send uid[%lu] cmd[%s:%d] to game failed: NULL == s", uid, CMD_DESC(cmd), cmd);
            return;
        }
        network::msg_buf_ptr send_buf = network::wrap_msg<T_MSG>(cmd, uid, msg);
        if (NULL == send_buf)
        {
            log_error("send uid[%lu] cmd[%s:%d] to game failed: NULL == send_buf", uid, CMD_DESC(cmd), cmd);
            return;
        }
		//m_game_listener.send_msg_to_game(s, send_buf);
		post_network_send_msg(s, send_buf);
        log_trace("send user[%lu] cmd[%s:%d] to game[%d]", uid, CMD_DESC(cmd), cmd, game_manager_t::get_game_id_by_socket(s));
    }

    template<typename T_MSG>
    void send_msg_to_all_games(uint16_t cmd, uint64_t uid, const T_MSG& msg)
    {
        network::msg_buf_ptr send_buf = network::wrap_msg<T_MSG>(cmd, uid, msg);
        if (NULL == send_buf)
        {
            log_error("send uid[%lu] cmd[%s:%d] to all games failed: NULL == send_buf", uid, CMD_DESC(cmd), cmd);
            return;
        }

        for (auto p_game : game_manager_t::get_game_list())
        {
            if (NULL == p_game->get_socket())
            {
                log_error("send uid[%lu] cmd[%s:%d] to all games failed: NULL == p_game->get_socket()", uid, CMD_DESC(cmd), cmd);
                continue;
            }

			//m_game_listener.send_msg_to_game(p_game->get_socket(), send_buf);
			post_network_send_msg(p_game->get_socket(), send_buf);
            log_trace("send uid[%lu] cmd[%s:%d] to game[%d]", uid, CMD_DESC(cmd), cmd, p_game->get_id());
        }
    }

    template<typename T_BUF>
    void send_msg_from_gm_to_game(const network::tcp_socket_ptr& s, uint16_t cmd, uint64_t uid, const T_BUF& msg_buf)
    {
        network::msg_buf_ptr send_buf = network::re_wrap_msg_s<T_BUF>(cmd, msg_buf);
        if (NULL == send_buf)
        {
            log_error("send uid[%lu] cmd[%s:%d] from gm to game failed: NULL == send_buf", uid, CMD_DESC(cmd), cmd);
            return;
        }
		//m_game_listener.send_msg_to_game(s, send_buf);
		post_network_send_msg(s, send_buf);
        log_trace("send user[%lu] cmd[%s:%d] from gm to game[%d]", uid, CMD_DESC(cmd), cmd, game_manager_t::get_game_id_by_socket(s));
    }

    template<typename T_BUF>
    void send_msg_from_gm_to_all_games(uint16_t cmd, uint64_t uid,const T_BUF& msg_buf)
    {
        network::msg_buf_ptr send_buf = network::re_wrap_msg_s<T_BUF>(cmd, msg_buf);
        if (NULL == send_buf)
        {
            log_error("send role[%lu] cmd[%s:%d] from gm to all games failed: NULL == send_buf", uid,CMD_DESC(cmd), cmd);
            return;
        }
        for (auto p_game : game_manager_t::get_game_list())
        {
            if (NULL == p_game->get_socket())
            {
               continue;
            }

			//m_game_listener.send_msg_to_game(p_game->get_socket(), send_buf);
			post_network_send_msg(p_game->get_socket(), send_buf);
            log_trace("send role[%lu] cmd[%s:%d] from gm to game[%d]", uid,CMD_DESC(cmd), cmd, p_game->get_id());
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
            log_error("send uid[%lu] cmd[%s:%d] to db failed: NULL == send_buf", uid, CMD_DESC(cmd), cmd);
            return;
        }
		//m_db_connector.send_msg_to_db(send_buf);
		post_network_send_msg(m_db_connector.get_socket(), send_buf);

		switch (cmd)
        {
        case op_cmd::eo_batch_save_logs_notify:
            break;
        default:
            log_trace("send user[%lu] cmd[%s:%d] to db", uid, CMD_DESC(cmd), cmd);
            break;
        }
    }

public:
    /*
    ****gm 的消息处理
    */
    void on_add_gm(const network::tcp_socket_ptr& s);
    void on_del_gm(const network::tcp_socket_ptr& s);
    void on_gm_msg(const network::tcp_socket_ptr& sock, uint64_t uid, uint16_t cmd, const network::msg_buf_ptr& msg_buf);

    void send_msg_from_gate_to_gm(uint16_t cmd, uint64_t uid, const network::msg_buf_ptr& msg_buf)
    {
		//m_gm_listener.send_msg_to_gm(msg_buf);
		post_network_send_msg(m_gm_listener.get_socket(), msg_buf);
        log_trace("send user[%lu] cmd[%s:%d] from gate to gm", uid, CMD_DESC(cmd), cmd);
    }

	void send_msg_from_game_to_gm(uint16_t cmd, uint64_t uid, const network::msg_buf_ptr& msg_buf)
	{
		//m_gm_listener.send_msg_to_gm(msg_buf);
		post_network_send_msg(m_gm_listener.get_socket(), msg_buf);
		log_trace("send user[%lu] cmd[%s:%d] from game to gm", uid, CMD_DESC(cmd), cmd);
	}

	template<typename T_BUF>
	void send_msg_to_gm(uint16_t cmd, uint64_t uid, const T_BUF& msg_buf)
	{
		network::msg_buf_ptr send_buf = network::wrap_msg<T_BUF>(cmd, uid, msg_buf);
		if (NULL == send_buf)
		{
			log_error("send uid[%lu] cmd[%s:%d] from gm to gm failed: NULL == send_buf", uid, CMD_DESC(cmd), cmd);
			return;
		}

		//m_gm_listener.send_msg_to_chat(send_buf);
		post_network_send_msg(m_gm_listener.get_socket(), send_buf);
		log_trace("send user[%lu] cmd[%s:%d] from game to gm", uid, CMD_DESC(cmd), cmd);
	}


    void send_msg_from_chat_to_gm(uint16_t cmd, uint64_t uid, const network::msg_buf_ptr& msg_buf)
    {
		//m_gm_listener.send_msg_to_gm(msg_buf);
		post_network_send_msg(m_gm_listener.get_socket(), msg_buf);
        log_trace("send user[%lu] cmd[%s:%d] from chat to gm", uid, CMD_DESC(cmd), cmd);
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
            log_error("send cmd[%s:%d] to log failed: NULL == send_buf", CMD_DESC(cmd), cmd);
            return;
        }
		//m_log_connector.send_msg_to_log(send_buf);
		post_network_send_msg(m_log_connector.get_socket(), send_buf);
        if (op_cmd::eo_batch_save_logs_notify != cmd)
        {
            log_trace("send cmd[%s:%d] to log", CMD_DESC(cmd), cmd);
        }
    }

public:
    /*
    ****chat 的消息处理
    */
	void on_add_chat(const network::tcp_socket_ptr& s);
	void on_del_chat(const network::tcp_socket_ptr& s);
	void on_chat_msg(const network::tcp_socket_ptr& sock, uint64_t uid, uint16_t cmd, const network::msg_buf_ptr& msg_buf);

    template<typename T_BUF>
    void send_msg_from_gm_to_chat(const network::tcp_socket_ptr& s, uint16_t cmd, uint64_t uid, const T_BUF& msg_buf)
    {
        network::msg_buf_ptr send_buf = network::re_wrap_msg_s<T_BUF>(cmd, msg_buf);
        if (NULL == send_buf)
        {
            log_error("send uid[%lu] cmd[%s:%d] from gm to chat failed: NULL == send_buf", uid, CMD_DESC(cmd), cmd);
            return;
        }

		//m_chat_listener.send_msg_to_chat(send_buf);
		post_network_send_msg(m_chat_listener.get_socket(), send_buf);
        log_trace("send user[%lu] cmd[%s:%d] from gm to chat", uid, CMD_DESC(cmd), cmd);
    }

    template<typename T_MSG>
    void send_msg_to_chat(uint16_t cmd, uint64_t uid, const T_MSG& msg)
    {
        network::msg_buf_ptr send_buf = network::wrap_msg<T_MSG>(cmd, uid, msg);
        if (NULL == send_buf)
        {
            log_error("send uid[%lu] cmd[%s:%d] to chat failed: NULL == send_buf", uid, CMD_DESC(cmd), cmd);
            return;
        }
		//m_chat_listener.send_msg_to_chat(send_buf);
		post_network_send_msg(m_chat_listener.get_socket(), send_buf);
        log_trace("send user[%lu] cmd[%s:%d] to chat", uid, CMD_DESC(cmd), cmd);
    }

public:
    /*
    ****transfer 的消息处理
    */
	void on_add_transfer(const network::tcp_socket_ptr& s);
	void on_del_transfer(const network::tcp_socket_ptr& s);
	void on_transfer_msg(const network::tcp_socket_ptr& s, uint64_t uid, uint16_t cmd, const network::msg_buf_ptr& msg_buf);

	template<typename T_MSG>
	void send_msg_to_transfer(uint16_t cmd, uint64_t uid, const T_MSG& msg)
	{
		network::msg_buf_ptr send_buf = network::wrap_msg<T_MSG>(cmd, uid, msg);
		if (NULL == send_buf)
		{
            log_error("send uid[%lu] cmd[%s:%d] to transfer failed: NULL == send_buf", uid, CMD_DESC(cmd), cmd);
            return;
		}

        transfer_ptr p_transfer = transfer_manager_t::get_transfer_by_rand();
        if (NULL == p_transfer)
        {
            log_error("NULL == p_transfer");
            return;
        }

        post_network_send_msg(p_transfer->get_socket(), send_buf);
        log_trace("send role[%lu] cmd[%s:%d] to transfer[%d]", uid, CMD_DESC(cmd), cmd, p_transfer->get_id());
	}

	template<typename T_MSG>
	void send_msg_to_transfer(const network::tcp_socket_ptr& s, uint16_t cmd, uint64_t uid, const T_MSG& msg)
	{
		network::msg_buf_ptr send_buf = network::wrap_msg<T_MSG>(cmd, uid, msg);
		if (NULL == send_buf)
		{
            log_error("send uid[%lu] cmd[%s:%d] to transfer failed: NULL == send_buf", uid, CMD_DESC(cmd), cmd);
            return;
		}

        transfer_ptr p_transfer = transfer_manager_t::get_transfer_by_socket(s);
        if (NULL == p_transfer)
        {
            log_error("NULL == p_transfer");
            return;
        }

        post_network_send_msg(p_transfer->get_socket(), send_buf);
		log_trace("send role[%lu] cmd[%s:%d] to transfer[%d]", uid,CMD_DESC(cmd), cmd, p_transfer->get_id());
	}

    template<typename T_BUF>
    void send_msg_from_game_to_transfer(uint16_t cmd, uint64_t uid, const T_BUF& msg_buf)
    {
        network::msg_buf_ptr send_buf = network::re_wrap_msg_s<T_BUF>(cmd, msg_buf);
        if (NULL == send_buf)
        {
            log_error("send role[%lu] cmd[%s:%d] from game to transfer failed: NULL == send_buf", uid,CMD_DESC(cmd), cmd);
            return;
        }

        transfer_ptr p_transfer = transfer_manager_t::get_transfer_by_rand();
        if (NULL == p_transfer)
        {
            log_error("NULL == p_transfer");
            return;
        }

		post_network_send_msg(p_transfer->get_socket(), send_buf);
        log_trace("send role[%lu] cmd[%s:%d] from game to transfer[%d]", uid, CMD_DESC(cmd), cmd, p_transfer->get_id());
    }

public:
    static void one_day(uint32_t id, bool is_in_time);
	// 开启服务器，已经过了跨天时间
	// 这里放跨天逻辑操作
	// 对于只是做清理功能的不需要处理
	static void open_update_one_day();

private:
    URL_T m_db_url;
	uint32_t			m_server_id = 0;
    network::network_t m_network;
    gate_listener_t m_gate_listener;
    game_listener_t m_game_listener;
    gm_listener_t m_gm_listener;
    log_connector_t m_log_connector;
    db_connector_t m_db_connector;
	chat_listener_t m_chat_listener;
	std::vector<transfer_connector_t> m_transfer_connector_list;
};


#endif
