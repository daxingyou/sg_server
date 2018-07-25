#ifndef __GM_SERVER_HPP__
#define __GM_SERVER_HPP__

#include "server_base.hpp"
#include "protobuf.hpp"
#include "network.hpp"
#include "center/center_connector.hpp"
#include "admin/admin_listener.hpp"
#include "cmd_desc.hpp"
#include "msg_count.hpp"
#include "gm_server_fwd.hpp"
#include "common/zdb_ex.hpp"

class gm_server_t
    : public common::server_base_t
{
public:
    gm_server_t();
    virtual ~gm_server_t();

    virtual const char* server_name() { return "gm"; }
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

public:

    void post_network_close_socket(const network::tcp_socket_ptr& s)
    {
        m_network.post_close_socket(s);
    }

	void post_msg_to_gm(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& buf)
	{
		m_network.post_send_msg(s, buf);
	}

	void post_network_send_msg(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& buf)
	{
		msg_count_t::push_msg_send_info(buf, env::xml->is_log_msg_count);
		m_network.post_send_msg(s, buf);
	}

    template<typename T_MSG>
    void post_network_send_msg(const network::tcp_socket_ptr& s, uint16_t cmd, const T_MSG& msg)
    {
        network::msg_buf_ptr send_buf = network::wrap_msg<T_MSG>(cmd, msg);
        if (NULL == send_buf)
        {
            return;
		}
		msg_count_t::push_msg_send_info(send_buf, env::xml->is_log_msg_count);
        m_network.post_send_msg(s, send_buf);
	}

	template<typename T_MSG>
	void send_msg_to_center(uint16_t cmd, uint64_t uid, const T_MSG& msg)
	{
		network::msg_buf_ptr send_buf = network::wrap_msg<T_MSG>(cmd, uid, msg);
		if (NULL == send_buf)
		{
			return;
		}
		//m_center_connector.send_msg_to_center(send_buf);
		post_network_send_msg(m_center_connector.get_socket(), send_buf);
		log_trace("send user[%lu] cmd[%s:%d] to center", uid, CMD_DESC(cmd), cmd);
	}

    template<typename T_MSG>
    void send_msg_to_gm_tool(uint16_t cmd, const T_MSG& msg)
    {
        network::msg_buf_ptr send_buf = network::wrap_msg<T_MSG>(cmd, msg);
        if (NULL == send_buf)
        {
            return;
        }
		//m_admin_listener.send_msg_to_gm(send_buf);
		post_network_send_msg(m_admin_listener.get_socket(), send_buf);
    }

    void on_add_center(const network::tcp_socket_ptr& s);
    void on_del_center(const network::tcp_socket_ptr& s);
    void on_center_msg(const network::tcp_socket_ptr& s, uint64_t uid, uint16_t cmd, const network::msg_buf_ptr& msg_buf);
    
    void on_add_admin(const network::tcp_socket_ptr& s);
    void on_del_admin(const network::tcp_socket_ptr& s);
    void on_admin_msg(const network::tcp_socket_ptr& s, uint64_t uid, uint16_t cmd, const network::msg_buf_ptr& msg_buf);


private:
    URL_T m_db_url;
    network::network_t m_network;
    center_connector_t m_center_connector;
    admin_listener_t  m_admin_listener;
};

#endif
