#ifndef __CHAT_SERVER_HPP__
#define __CHAT_SERVER_HPP__

#include "server_base.hpp"
#include "protobuf.hpp"
#include "network.hpp"
#include "gate/gate_manager.hpp"
#include "gate/gate_listener.hpp"
#include "game/game_manager.hpp"
#include "game/game_listener.hpp"
#include "transfer/transfer_connector.hpp"
#include "transfer/transfer_manager.hpp"
#include "cmd_desc.hpp"
#include "center/center_connector.hpp"
#include "protocpp/protos_fwd.hpp"
#include "chat_server_fwd.hpp"
#include "video/video_connector.hpp"

class chat_server_t
    : public common::server_base_t
{
public:
    chat_server_t();
    virtual ~chat_server_t();

public:
    virtual const char* server_name() { return "chat"; }
    virtual bool on_init_server();
    virtual bool on_close_server();
    virtual void on_run_server();

private:
    bool load_xml();
    bool load_server_xml();
    void close_xml();

    bool init_network();
    void close_network();
	void stop_network();

    bool init_logic();

public:
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
            log_error("send uid[%lu] cmd[%s:%d] to gate[%d] failed: NULL == send_buf", uid, CMD_DESC(cmd), cmd, gate_manager_t::get_gate_id_by_socket(s));
            return;
        }
        //m_gate_listener.send_msg_to_gate(s, send_buf);
		post_network_send_msg(s, send_buf);
        log_trace("send role[%lu] cmd[%s:%d] to gate[%d]", uid, CMD_DESC(cmd), cmd, gate_manager_t::get_gate_id_by_socket(s));
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
            log_error("send uid[%lu] cmd[%s:%d] to game[%d] failed: NULL == send_buf", uid, CMD_DESC(cmd), cmd, game_manager_t::get_game_id_by_socket(s));
            return;
        }
		//m_game_listener.send_msg_to_game(s, send_buf);
		post_network_send_msg(s, send_buf);
        log_trace("send role[%lu] cmd[%s:%d] to game[%d]", uid, CMD_DESC(cmd), cmd, game_manager_t::get_game_id_by_socket(s));
    }

    template<typename T_MSG>
    void send_msg_to_all_games(uint16_t cmd, const T_MSG& msg)
    {
        network::msg_buf_ptr send_buf = network::wrap_msg<T_MSG>(cmd, 0, msg);
        if (NULL == send_buf)
        {
            log_error("send cmd[%s:%d] to all games failed: NULL == send_buf", CMD_DESC(cmd), cmd);
            return;
        }
        for (auto p_game : game_manager_t::get_game_list())
        {
            if (NULL == p_game)
            {
                log_error("send cmd[%s:%d] to all games failed: NULL == p_game", CMD_DESC(cmd), cmd);
                return;
            }
            if (NULL == p_game->get_socket())
            {
                log_error("send cmd[%s:%d] to all games failed: NULL == p_game->get_socket()", CMD_DESC(cmd), cmd);
                return;
            }
			//m_game_listener.send_msg_to_game(p_game->get_socket(), send_buf);
			post_network_send_msg(p_game->get_socket(), send_buf);
            log_trace("send cmd[%s:%d] to game[%d]", CMD_DESC(cmd), cmd, p_game->get_id());
        }
    }

public:
    /*
    ****transfer 的消息处理
    */
    void on_add_transfer(const network::tcp_socket_ptr& s);
    void on_del_transfer(const network::tcp_socket_ptr& s);
    void on_transfer_msg(const network::tcp_socket_ptr& s,uint64_t uid, uint16_t cmd, const network::msg_buf_ptr& msg_buf);

    template<typename T_MSG>
    void send_msg_to_transfer(uint16_t cmd, uint64_t uid, const T_MSG& msg)
    {
        transfer_ptr p_transfer = transfer_manager_t::get_transfer_by_rand();
        if (NULL == p_transfer)
        {
            log_error("NULL == p_transfer");
            return;
        }

        if (NULL == p_transfer->get_socket())
        {
            log_error("NULL == transfer[%d] socket", p_transfer->get_id());
            return;
        }

        network::msg_buf_ptr send_buf = network::wrap_msg<T_MSG>(cmd, uid, msg);
        if (NULL == send_buf)
        {
            log_error("send uid[%lu] cmd[%s:%d] to transfer[%d] failed: NULL == send_buf", uid, CMD_DESC(cmd), cmd, p_transfer->get_id());
            return;
        }

		post_network_send_msg(p_transfer->get_socket(), send_buf);
        log_trace("send uid[%lu] cmd[%s:%d] to transfer[%d]", uid, CMD_DESC(cmd), cmd, p_transfer->get_id());
    }

    template<typename T_MSG>
    void send_msg_to_transfer(const network::tcp_socket_ptr& s, uint16_t cmd, uint64_t uid, const T_MSG& msg)
    {
        transfer_ptr p_transfer = transfer_manager_t::get_transfer_by_socket(s);
        if (NULL == p_transfer)
        {
            log_error("NULL == p_transfer");
            return;
        }

        network::msg_buf_ptr send_buf = network::wrap_msg<T_MSG>(cmd, uid, msg);
        if (NULL == send_buf)
        {
            log_error("send uid[%lu] cmd[%s:%d] to transfer[%d] failed: NULL == send_buf", uid, CMD_DESC(cmd), cmd, p_transfer->get_id());
            return;
        }

        post_network_send_msg(p_transfer->get_socket(), send_buf);
        log_trace("send uid[%lu] cmd[%s:%d] to transfer[%d]", uid, CMD_DESC(cmd), cmd, p_transfer->get_id());
	}

	template<typename T_BUF>
	void send_msg_from_game_to_transfer(uint16_t cmd, uint64_t uid, const T_BUF& msg_buf)
	{
        transfer_ptr p_transfer = transfer_manager_t::get_transfer_by_rand();
        if (NULL == p_transfer)
        {
            log_error("NULL == p_transfer");
            return;
        }

        if (NULL == p_transfer->get_socket())
        {
            log_error("NULL == transfer[%d] socket", p_transfer->get_id());
            return;
        }

        network::msg_buf_ptr send_buf = network::re_wrap_msg_s<T_BUF>(cmd, msg_buf);
        if (NULL == send_buf)
        {
            log_error("send role[%lu] cmd[%s:%d] from game to transfer[%d] failed: NULL == send_buf", uid, CMD_DESC(cmd), cmd, p_transfer->get_id());
            return;
        }

        post_network_send_msg(p_transfer->get_socket(), send_buf);
        log_trace("send uid[%lu] cmd[%s:%d] from game to transfer[%d]", uid, CMD_DESC(cmd), cmd, p_transfer->get_id());
	}

public:
    /*
    ****center 的消息处理
    */
	void on_add_center(const network::tcp_socket_ptr& s);
	void on_del_center(const network::tcp_socket_ptr& s);
	void on_center_msg(const network::tcp_socket_ptr& s, uint64_t uid, uint16_t cmd, const network::msg_buf_ptr& msg_buf);

	template<typename T_MSG>
	void send_msg_to_center(uint16_t cmd, uint64_t uid, const T_MSG& msg)
	{
		network::msg_buf_ptr send_buf = network::wrap_msg<T_MSG>(cmd, uid, msg);
		if (NULL == send_buf)
		{
            log_error("send uid[%lu] cmd[%s:%d] to center failed: NULL == send_buf", uid, CMD_DESC(cmd), cmd);
            return;
		}
		//m_center_connector.send_msg_to_center(send_buf);
		post_network_send_msg(m_center_connector.get_socket(), send_buf);
		log_trace("send uid[%lu] cmd[%s:%d] to center", uid, CMD_DESC(cmd), cmd);
	}

public:
    /*
    ****video 的消息处理
    */
    void on_add_video(const network::tcp_socket_ptr& s);
    void on_del_video(const network::tcp_socket_ptr& s);

	template<typename T_BUF>
	void send_msg_from_game_to_video(uint16_t cmd, uint64_t uid, const T_BUF& msg_buf)
	{
		network::msg_buf_ptr send_buf = network::re_wrap_msg_s<T_BUF>(cmd, msg_buf);
		if (NULL == send_buf)
		{
			log_error("send role[%lu] cmd[%s:%d] from game to video failed: NULL == send_buf", uid, CMD_DESC(cmd), cmd);
			return;
		}

		post_network_send_msg(m_video_connector.get_socket(), send_buf);
		log_trace("send uid[%lu] cmd[%s:%d] from game to video", uid, CMD_DESC(cmd), cmd);
	}
	
private:
    network::network_t      m_network;
    gate_listener_t         m_gate_listener;
    game_listener_t         m_game_listener;
	center_connector_t		m_center_connector;
	std::vector<transfer_connector_t> m_transfer_connector_list;
	video_connector_t		m_video_connector;
};

#endif
