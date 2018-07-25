#ifndef __LOGIN_SERVER_HPP__
#define __LOGIN_SERVER_HPP__

#include "server_base.hpp"
#include "common/zdb_ex.hpp"
#include "protobuf.hpp"
#include "network.hpp"
#include "user/user_listener.hpp"
#include "gate/gate_listener.hpp"
#include "gate/gate_manager.hpp"
#include "account/account_connector.hpp"
#include <boost/asio/deadline_timer.hpp>
#include "cmd_desc.hpp"
#include "msg_count.hpp"
#include "login_server_fwd.hpp"

enum LOGIN_SERVER_STATE
{
	LOGIN_SERVER_STATE_NONE,
	LOGIN_SERVER_STATE_START,
	LOGIN_SERVER_STATE_RUNNING,
};

class login_server_t
    : public common::server_base_t
{
public:
    login_server_t();
    virtual ~login_server_t();

    virtual const char* server_name() { return "login"; }
    virtual bool on_init_server();
    virtual bool on_close_server();
    virtual void on_run_server();

private:
    bool load_xml();
    bool load_server_xml();
    void close_xml();

    bool init_zdb();
    void close_zdb();

    bool init_redis();
    void close_redis();

    bool init_network();
    void close_network();

    void start_server_run_timer();
    void server_run_timeout_callback(const boost::system::error_code& ec);
    void cancel_server_run_timer();

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

    template<typename T_MSG>
    void post_network_send_msg(const network::tcp_socket_ptr& s, uint16_t cmd, uint64_t uid, const T_MSG& msg)
    {
        network::msg_buf_ptr send_buf = network::wrap_msg<T_MSG>(cmd, uid, msg);
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
    void send_msg_to_client(const network::tcp_socket_ptr& s, uint64_t uid, uint16_t cmd, const T_MSG& msg)
    {
        if (NULL == s)
        {
            log_error("send user[%lu] cmd[%s:%d] to client failed: NULL == s", uid, CMD_DESC(cmd), cmd);
            return;
        }
#ifdef __ENCRYPT__
        network::msg_buf_ptr send_buf = network::wrap_msg_encrypt<T_MSG>(cmd, msg, SG_SERVER_TO_CLIENT_KEY);
#else
        network::msg_buf_ptr send_buf = network::wrap_msg_to_client<T_MSG>(cmd, msg);
#endif
        if (NULL == send_buf)
        {
            log_error("send user[%lu: %d] cmd[%s:%d] to client failed: NULL == send_buf", uid, s->socket().native(), CMD_DESC(cmd), cmd);
            return;
        }
        post_network_send_msg(s, send_buf);
        log_trace("send user[%lu: %d] cmd[%s:%d] message to client", uid, s->socket().native(), CMD_DESC(cmd), cmd);
    }

public:
    /*
    ****account 的消息处理
    */
	void on_add_account(const network::tcp_socket_ptr& s);
	void on_del_account(const network::tcp_socket_ptr& s);
	void on_account_msg(uint64_t uid,uint16_t cmd, const network::msg_buf_ptr& msg_buf);

    template<typename T_MSG>
    void send_msg_to_account(uint16_t cmd, const T_MSG& msg)
    {
        network::msg_buf_ptr send_buf = network::wrap_msg<T_MSG>(cmd, msg);
        if (NULL == send_buf)
        {
            log_error("send cmd[%s:%d] to account failed: NULL == send_buf", CMD_DESC(cmd), cmd);
            return;
        }
		//m_account_connector.send_msg_to_account(send_buf);
		post_network_send_msg(m_account_connector.get_socket(), send_buf);
        log_trace("send cmd[%s:%d] message to account", CMD_DESC(cmd), cmd);
    }

public:
    /*
    ****gate 的消息处理
    */
    void on_add_gate(const network::tcp_socket_ptr& s);
    void on_del_gate(const network::tcp_socket_ptr& s);
    void on_gate_msg(const network::tcp_socket_ptr& s, uint64_t uid, uint16_t cmd, const network::msg_buf_ptr& msg_buf);

    template<typename T_MSG>
    void send_msg_to_gate(const network::tcp_socket_ptr& p_socket, uint16_t cmd, const T_MSG& msg)
    {
        if (NULL == p_socket)
        {
            log_error("send cmd[%s:%d] to gate failed: NULL == p_socket", CMD_DESC(cmd), cmd);
            return;
        }
        network::msg_buf_ptr send_buf = network::wrap_msg<T_MSG>(cmd, msg);
        if (NULL == send_buf)
        {
            log_error("send cmd[%s:%d] to gate[%d] failed: NULL == send_buf", CMD_DESC(cmd), cmd, gate_manager_t::get_gate_id_by_socket(p_socket));
            return;
        }
		//m_gate_listener.send_msg_to_gate(p_socket, send_buf);
		post_network_send_msg(p_socket, send_buf);
        log_trace("send cmd[%s:%d] to gate[%d]", CMD_DESC(cmd), cmd, gate_manager_t::get_gate_id_by_socket(p_socket));
    }

    template<typename T_MSG>
    void send_msg_to_gate(uint32_t gate_id, uint16_t cmd, const T_MSG& msg)
    {
        if (0 == gate_id)
        {
            log_error("send cmd[%s:%d] to gate failed: 0 == gate_id", CMD_DESC(cmd), cmd);
            return;
        }
        const network::tcp_socket_ptr& p_socket = gate_manager_t::get_gate_socket_by_id(gate_id);
        if (NULL == p_socket)
        {
            log_error("send cmd[%s:%d] to gate[%d] failed: NULL == p_socket", CMD_DESC(cmd), cmd, gate_id);
            return;
        }
        network::msg_buf_ptr send_buf = network::wrap_msg<T_MSG>(cmd, msg);
        if (NULL == send_buf)
        {
            log_error("send cmd[%s:%d] to gate[%d] failed: NULL == send_buf", CMD_DESC(cmd), cmd, gate_id);
            return;
        }
		//m_gate_listener.send_msg_to_gate(p_socket, send_buf);
		post_network_send_msg(p_socket, send_buf);
        log_trace("send cmd[%s:%d] to gate[%d]", CMD_DESC(cmd), cmd, gate_id);
    }

    template<typename T_MSG>
    void send_msg_to_gate(uint16_t cmd, uint64_t uid, const T_MSG& msg)
    {
        if (0 == uid)
        {
            log_error("send cmd[%s:%d] to gate failed: 0 == uid", CMD_DESC(cmd), cmd);
            return;
        }
        const gate_ptr& p_gate = gate_manager_t::get_gate_by_user_uid(uid);
        if (NULL == p_gate)
        {
            log_error("send uid[%lu] cmd[%s:%d] to gate failed: NULL == p_gate", uid, CMD_DESC(cmd), cmd);
            return;
        }
        const network::tcp_socket_ptr& p_socket = p_gate->get_socket();
        if (NULL == p_socket)
        {
            log_error("send uid[%lu] cmd[%s:%d] to gate[%d] failed: NULL == p_socket", uid, CMD_DESC(cmd), cmd, p_gate->get_id());
            return;
        }
        network::msg_buf_ptr send_buf = network::wrap_msg<T_MSG>(cmd, uid, msg);
        if (NULL == send_buf)
        {
            log_error("send uid[%lu] cmd[%s:%d] to gate[%d] failed: NULL == send_buf", uid, CMD_DESC(cmd), cmd, p_gate->get_id());
            return;
        }
		//m_gate_listener.send_msg_to_gate(p_socket, send_buf);
		post_network_send_msg(p_socket, send_buf);
		log_trace("send user[%lu] cmd[%s:%d] to gate[%d]", uid, CMD_DESC(cmd), cmd, p_gate->get_id());

    }

private:
    URL_T m_db_url;
    network::network_t m_network;
    user_listener_t m_user_listener;
    gate_listener_t m_gate_listener;
	account_connector_t m_account_connector;
    int32_t m_start_time = 0;
    boost::shared_ptr<boost::asio::deadline_timer> m_server_run_timer = NULL;
	LOGIN_SERVER_STATE m_state = LOGIN_SERVER_STATE_NONE;
};

#endif
