#ifndef __ACCOUNT_SERVER_HPP__
#define __ACCOUNT_SERVER_HPP__

#include "server_base.hpp"
#include <zdb.h>
#include "protobuf.hpp"
#include "network.hpp"
#include "user/user_listener.hpp"
#include "login/login_listener.hpp"
#include "db/db_listener.hpp"
#include "log/log_connector.hpp"
#include "cmd_desc.hpp"
#include "msg_count.hpp"
#include "account_server_fwd.hpp"

class account_server_t
    : public common::server_base_t
{
public:
	account_server_t();
    virtual ~account_server_t();

    virtual const char* server_name() { return "account"; }
    virtual bool on_init_server();
    virtual bool on_close_server();
    virtual void on_run_server();

private:
    bool load_xml();
    bool load_global_server_xml();
	bool load_server_xml();
    bool load_channel_xml();
    bool load_config_xml();
    void close_xml();

	bool open_channel();
	void close_channel();
    
    bool init_zdb();
    void close_zdb();

    bool init_network();
    void close_network();
	void stop_network();

	bool load_server_info();	//从web读取,登录服列表信息

public:
    template<typename T_MSG>
    void post_network_send_msg(const network::tcp_socket_ptr& s, uint16_t cmd, const T_MSG& msg)
    {
        network::msg_buf_ptr send_buf = network::wrap_msg<T_MSG>(cmd, msg);
        if (NULL == send_buf)
        {
            log_error("send cmd[%s:%d] msg failed: NULL == send_buf", CMD_DESC(cmd), cmd);
            return;
		}
		msg_count_t::push_msg_send_info(send_buf, env::xml->is_log_msg_count);
        m_network.post_send_msg(s, send_buf);
    }

    void post_network_send_msg(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& buf) 
	{
		msg_count_t::push_msg_send_info(buf, env::xml->is_log_msg_count);
		m_network.post_send_msg(s, buf); 
	}

    void post_network_close_socket(const network::tcp_socket_ptr& s) { m_network.post_close_socket(s); }

public:
    /*
    ****client 的消息处理
    */
	void on_add_client(const network::tcp_socket_ptr& s);
	void on_del_client(const network::tcp_socket_ptr& s);
	void on_client_msg(const network::tcp_socket_ptr& s, uint64_t uid, uint16_t cmd, const network::msg_buf_ptr& msg_buf);

    template<typename T_MSG>
    void send_msg_to_client(const network::tcp_socket_ptr& s, uint16_t cmd, const T_MSG& msg)
    {
        if (NULL == s)
        {
            log_error("send cmd[%s:%d] to client failed: NULL == s", CMD_DESC(cmd), cmd);
            return;
        }
#ifdef __ENCRYPT__
        network::msg_buf_ptr send_buf = network::wrap_msg_encrypt<T_MSG>(cmd, msg, SG_SERVER_TO_CLIENT_KEY);
#else
        network::msg_buf_ptr send_buf = network::wrap_msg_to_client<T_MSG>(cmd, msg);
#endif
        if (NULL == send_buf)
        {
            log_error("send socket[%d] cmd[%s:%d] to client failed: NULL == send_buf", s->socket().native(), CMD_DESC(cmd), cmd);
            return;
        }
        post_network_send_msg(s, send_buf);
        log_trace("send socket[%d] cmd[%s:%d] message to client", s->socket().native(), CMD_DESC(cmd), cmd);
    }

public:
    /*
    ****login 的消息处理
    */
	void on_add_login(const network::tcp_socket_ptr& s);
	void on_del_login(const network::tcp_socket_ptr& s);
	void on_login_msg(const network::tcp_socket_ptr& s, uint64_t uid, uint16_t cmd, const network::msg_buf_ptr& msg_buf);

    template<typename T_MSG>
    void send_msg_to_login(const network::tcp_socket_ptr& s, uint16_t cmd, uint64_t uid, const T_MSG& msg)
    {
        if (NULL == s)
        {
            log_error("send uid[%lu] cmd[%s:%d] to db failed: NULL == s", uid, CMD_DESC(cmd), cmd);
            return;
        }
        network::msg_buf_ptr send_buf = network::wrap_msg<T_MSG>(cmd, uid, msg);
        if (NULL == send_buf)
        {
            log_error("send uid[%lu] cmd[%s:%d] to db failed: NULL == send_buf", uid, CMD_DESC(cmd), cmd);
            return;
        }
        post_network_send_msg(s, send_buf);
        log_trace("send uid[%lu] cmd[%s:%d] message to login", uid, CMD_DESC(cmd), cmd);
    }

public:
    /*
    ****db 的消息处理
    */
	void on_add_db(const network::tcp_socket_ptr& s);
	void on_del_db(const network::tcp_socket_ptr& s);
	void on_db_msg(const network::tcp_socket_ptr& s, uint64_t uid, uint16_t cmd, const network::msg_buf_ptr& msg_buf);

	template<typename T_MSG>
	void send_msg_to_db(const network::tcp_socket_ptr& s, uint16_t cmd, uint64_t uid, const T_MSG& msg)
	{
		if (NULL == s)
		{
            log_error("send uid[%lu] cmd[%s:%d] to db failed: NULL == s", uid, CMD_DESC(cmd), cmd);
            return;
		}
		network::msg_buf_ptr send_buf = network::wrap_msg<T_MSG>(cmd, uid, msg);
        if (NULL == send_buf)
        {
            log_error("send uid[%lu] cmd[%s:%d] to db failed: NULL == send_buf", uid, CMD_DESC(cmd), cmd);
            return;
        }
		post_network_send_msg(s, send_buf);
		log_trace("send uid[%lu] cmd[%s:%d] message to db", uid, CMD_DESC(cmd), cmd);
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
        post_network_send_msg(m_log_connector.get_socket(), send_buf);
        if (op_cmd::no_batch_save_logs_notify != cmd)
        {
            log_trace("send cmd[%s:%d] to log", CMD_DESC(cmd), cmd);
        }
    }

public:
    uint32_t get_id() const { return m_id; }
    void set_id(uint32_t val) { m_id = val; }

private:
    uint32_t m_id = 0;
    URL_T m_db_url;
    network::network_t m_network;
    user_listener_t m_user_listener;
    login_listener_t m_login_listener;
	db_listener_t m_db_listener;
    log_connector_t m_log_connector;
};

#endif
