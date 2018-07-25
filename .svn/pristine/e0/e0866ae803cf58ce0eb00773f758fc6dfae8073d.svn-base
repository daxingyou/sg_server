#ifndef __TRANSFER_SERVER_HPP__
#define __TRANSFER_SERVER_HPP__

#include "server_base.hpp"
#include "protobuf.hpp"
#include "network.hpp"
#include "chat/chat_listener.hpp"
#include "chat/chat_manager.hpp"
#include "cmd_desc.hpp"
#include "area/area_connector.hpp"
#include "center/center_manager.hpp"
#include "center/center_listener.hpp"

class transfer_server_t
    : public common::server_base_t
{
public:
    transfer_server_t() {}
    virtual ~transfer_server_t() {}

public:
    virtual const char* server_name() { return "transfer"; }
    virtual bool on_init_server();
    virtual bool on_close_server();
    virtual void on_run_server();

private:
    bool load_xml();
    bool load_global_server_xml();
    bool load_config_xml();
    void close_xml();

    bool init_network();
    void close_network();

    bool init_game_logic();

public:
	void post_network_send_msg(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& buf);

    void post_network_close_socket(const network::tcp_socket_ptr& s) { m_network.post_close_socket(s); }

public:
    /*
    ****chat 的消息处理
    */
    void on_add_chat(const network::tcp_socket_ptr& s);
    void on_del_chat(const network::tcp_socket_ptr& s);
    void on_chat_msg(const network::tcp_socket_ptr& s, uint64_t uid, uint16_t cmd, const network::msg_buf_ptr& msg_buf);

    template<typename T_MSG>
    void send_msg_to_chat(const network::tcp_socket_ptr& s, uint16_t cmd, uint64_t uid, const T_MSG& msg)
    {
        if (NULL == s)
        {
            log_error("send uid[%lu] cmd[%s:%d] to chat failed: NULL == s", uid, CMD_DESC(cmd), cmd);
            return;
        }
        chat_ptr p_chat = chat_manager_t::get_chat(s);
        if (NULL == p_chat)
        {
            log_error("send uid[%lu] cmd[%s:%d] to chat failed: NULL == p_chat", uid, CMD_DESC(cmd), cmd);
            return;
        }
        
        network::msg_buf_ptr send_buf = network::wrap_msg<T_MSG>(cmd, uid, msg);
        if (NULL == send_buf)
        {
            log_error("send uid[%lu] cmd[%s:%d] to chat[server_id=%d] failed: NULL == send_buf", uid, CMD_DESC(cmd), cmd, p_chat->get_server_id());
            return;
        }
        post_network_send_msg(s, send_buf);
        log_trace("send uid[%lu] cmd[%s:%d] to chat[server_id=%d]", CMD_DESC(cmd), uid, cmd, p_chat->get_server_id());
    }

    template<typename T_MSG>
    void send_msg_to_chat(uint32_t server_id, uint16_t cmd, uint64_t uid, const T_MSG& msg)
    {
        const network::tcp_socket_ptr& s = chat_manager_t::get_chat_socket(server_id);
        if (NULL == s)
        {
            log_error("send uid[%lu] cmd[%s:%d] to chat[server_id=%d] failed: NULL == s", uid, CMD_DESC(cmd), cmd, server_id);
            return;
        }
        network::msg_buf_ptr send_buf = network::wrap_msg<T_MSG>(cmd, uid, msg);
        if (NULL == send_buf)
        {
            log_error("send uid[%lu] cmd[%s:%d] to chat[server_id=%d] failed: NULL == send_buf", uid, CMD_DESC(cmd), cmd, server_id);
            return;
        }
        post_network_send_msg(s, send_buf);
        log_trace("send uid[%lu] cmd[%s:%d] to chat[server_id=%d]", uid, CMD_DESC(cmd), cmd, server_id);
    }

    template<typename T_MSG>
    void send_msg_to_all_chat(uint16_t cmd, uint64_t uid, const T_MSG& msg)
    {
        network::msg_buf_ptr send_buf = network::wrap_msg<T_MSG>(cmd, uid, msg);
        if (NULL == send_buf)
        {
            log_error("send uid[%lu] cmd[%s:%d] to all chats failed: NULL == send_buf", uid, CMD_DESC(cmd), cmd);
            return;
        }
        for (auto p_chat : chat_manager_t::get_chat_list())
        {
            if (NULL == p_chat->get_socket())
            {
                log_error("send uid[%lu] cmd[%s:%d] to all chats failed: NULL == p_chat->get_socket()", uid, CMD_DESC(cmd), cmd);
                continue;
            }

            post_network_send_msg(p_chat->get_socket(), send_buf);
            log_trace("send uid[%lu] cmd[%s:%d] to chat[server_id=%d]", uid, CMD_DESC(cmd), cmd, p_chat->get_server_id());
        }
    }

public:
    /*
    ****area 的消息处理
    */
	void on_add_area(const network::tcp_socket_ptr& s);
	void on_del_area(const network::tcp_socket_ptr& s);
	void on_area_msg(const network::tcp_socket_ptr& s, uint64_t uid, uint16_t cmd, const network::msg_buf_ptr& msg_buf);

    template<typename T_BUF>
    void send_msg_forward_area(uint16_t cmd, uint64_t uid, const T_BUF& msg_buf)
    {
        network::msg_buf_ptr send_buf = network::re_wrap_msg_s<T_BUF>(cmd, msg_buf);
        if (NULL == send_buf)
        {
            log_error("send uid[%lu] cmd[%s:%d] to area failed: NULL == send_buf", uid, CMD_DESC(cmd), cmd);
            return;
        }
        //m_area_connector.send_msg_to_area(send_buf);
		post_network_send_msg(m_area_connector.get_socket(), send_buf);
        log_trace("send cmd[%s:%d] from game to area", CMD_DESC(cmd), cmd);
    }

    template<typename T_MSG>
    void send_msg_to_area(uint16_t cmd, uint64_t uid, const T_MSG& msg)
    {
        network::msg_buf_ptr send_buf = network::wrap_msg<T_MSG>(cmd, uid, msg);
        if (NULL == send_buf)
        {
            log_error("send uid[%lu] cmd[%s:%d] to area failed: NULL == send_buf", uid, CMD_DESC(cmd), cmd);
            return;
        }
		//m_area_connector.send_msg_to_area(send_buf);
		post_network_send_msg(m_area_connector.get_socket(), send_buf);
        log_trace("send uid[%lu] cmd[%s:%d] to area", uid, CMD_DESC(cmd), cmd);
    }

public:
    /*
    ****center 的消息处理
    */
	void on_add_center(const network::tcp_socket_ptr& s);
	void on_del_center(const network::tcp_socket_ptr& s);
	void on_center_msg(const network::tcp_socket_ptr& s, uint64_t uid, uint16_t cmd, const network::msg_buf_ptr& msg_buf);

	template<typename T_MSG>
	void send_msg_to_center(const network::tcp_socket_ptr& s, uint16_t cmd, uint64_t uid, const T_MSG& msg)
	{
		if (NULL == s)
		{
            log_error("send uid[%lu] cmd[%s:%d] to center failed: NULL == s", uid, CMD_DESC(cmd), cmd);
            return;
		}
        center_ptr p_center = center_manager_t::get_center_by_socket(s);
        if (NULL == p_center)
        {
            log_error("send uid[%lu] cmd[%s:%d] to center failed: NULL == p_center", uid, CMD_DESC(cmd), cmd);
            return;
        }
		network::msg_buf_ptr send_buf = network::wrap_msg<T_MSG>(cmd, uid, msg);
		if (NULL == send_buf)
		{
            log_error("send uid[%lu] cmd[%s:%d] to center[server_id=%d] failed: NULL == send_buf", uid, CMD_DESC(cmd), cmd, p_center->get_id());
            return;
		}
		post_network_send_msg(s, send_buf);
		log_trace("send uid[%lu] cmd[%s:%d] to center[server_id=%d]", uid, CMD_DESC(cmd), cmd, p_center->get_id());
	}

	template<typename T_MSG>
	void send_msg_to_center(uint32_t server_id, uint16_t cmd, uint64_t uid, const T_MSG& msg)
	{
		const network::tcp_socket_ptr& s = center_manager_t::get_center_socket_by_id(server_id);
		if (NULL == s)
		{
            log_error("send uid[%lu] cmd[%s:%d] to center[server_id=%d] failed: NULL == s", uid, CMD_DESC(cmd), cmd, server_id);
            return;
		}
		network::msg_buf_ptr send_buf = network::wrap_msg<T_MSG>(cmd, uid, msg);
		if (NULL == send_buf)
		{
            log_error("send uid[%lu] cmd[%s:%d] to center[server_id=%d] failed: NULL == send_buf", uid, CMD_DESC(cmd), cmd, server_id);
            return;
		}
		post_network_send_msg(s, send_buf);
		log_trace("send uid[%lu] cmd[%s:%d] to center[server_id=%d]", uid, CMD_DESC(cmd), cmd, server_id);
	}

	template<typename T_MSG>
	void send_msg_to_all_center(uint16_t cmd, uint64_t uid, const T_MSG& msg)
	{
		network::msg_buf_ptr send_buf = network::wrap_msg<T_MSG>(cmd, uid, msg);
		if (NULL == send_buf)
		{
            log_error("send uid[%lu] cmd[%s:%d] to all centers failed: NULL == send_buf", uid, CMD_DESC(cmd), cmd);
            return;
		}
		for (auto p_center : center_manager_t::get_center_list())
		{
			if (NULL == p_center->get_socket())
			{
                log_error("send uid[%lu] cmd[%s:%d] to all centers failed: NULL == p_center->get_socket()", uid, CMD_DESC(cmd), cmd);
                continue;
			}
            post_network_send_msg(p_center->get_socket(), send_buf);
            log_trace("send uid[%lu] cmd[%s:%d] to center[server_id=%d]", uid, CMD_DESC(cmd), cmd, p_center->get_id());
		}
	}

public:
    uint32_t get_id() const { return m_id; }
    void set_id(uint32_t val) { m_id = val; }

private:
    uint32_t            m_id = 0;
    network::network_t  m_network;
	chat_listener_t		m_chat_listener;
	center_listener_t	m_center_listener;
	area_connector_t    m_area_connector;
};

#endif
