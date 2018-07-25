#ifndef __RECHARGE_SERVER_HPP__
#define __RECHARGE_SERVER_HPP__

#include "server_base.hpp"
#include <zdb.h>
#include "protobuf.hpp"
#include "network.hpp"
#include "log/log_connector.hpp"
#include "cmd_desc.hpp"
#include "msg_count.hpp"
#include "recharge_server_fwd.hpp"

class recharge_server_t
    : public common::server_base_t
{
public:
	recharge_server_t();
    virtual ~recharge_server_t();

    virtual const char* server_name() { return "recharge"; }
    virtual bool on_init_server();
    virtual bool on_close_server();
    virtual void on_run_server();

private:
    bool load_xml();
	bool load_server_xml();
    void close_xml();

    bool init_zdb();
    void close_zdb();

    bool init_network();
    void close_network();
	void stop_network();
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
	**** center的消息处理	
	*/
	void on_add_center(const network::tcp_socket_ptr& s);
	void on_del_center(const network::tcp_socket_ptr& s);

	template<typename T_MSG>
	void send_msg_to_center(uint16_t cmd, const T_MSG& msg)
	{
		network::msg_buf_ptr send_buf = network::wrap_msg<T_MSG>(cmd, msg);
		if (NULL == send_buf)
		{
			log_error("send cmd[%s:%d] to log failed: NULL == send_buf", CMD_DESC(cmd), cmd);
			return;
		}
		post_network_send_msg(m_center_listener.get_socket(), send_buf);
		if (op_cmd::no_batch_save_logs_notify != cmd)
		{
			log_trace("send cmd[%s:%d] to log", CMD_DESC(cmd), cmd);
		}
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
	log_connector_t m_log_connector;
	center_listener_t m_center_listener;
};

#endif
