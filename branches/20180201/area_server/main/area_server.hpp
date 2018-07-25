#ifndef __AREA_SERVER_HPP__
#define __AREA_SERVER_HPP__

#include "common/server_base.hpp"
#include "network/protobuf.hpp"
#include "network/network.hpp"
#include "cmd_desc.hpp"
#include "common/zdb_ex.hpp"
#include "transfer/transfer_listener.hpp"
#include "transfer/transfer_manager.hpp"
#include "cross/cross_listener.hpp"
#include "cross/cross_manager.hpp"

class area_server_t
    : public common::server_base_t
{
public:
    area_server_t();
    virtual ~area_server_t();

    virtual const char* server_name() { return "area"; }
    virtual bool on_init_server();
    virtual bool on_close_server();
    virtual void on_run_server();

private:
    bool load_xml();
    bool load_global_server_xml();
    void close_xml();

    bool init_network();
    void close_network();

    bool init_zdb();
    void close_zdb();

    bool init_redis();
    void close_redis();

    bool init_game_logic();
    void clear_game_logic();

public:
	void post_network_send_msg(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& buf);
    
    void post_network_close_socket(const network::tcp_socket_ptr& s) { m_network.post_close_socket(s); }

public:
    /*
    ****transfer 的消息处理
    */
	void on_add_transfer(const network::tcp_socket_ptr& s);
	void on_del_transfer(const network::tcp_socket_ptr& s);
	void on_transfer_msg(const network::tcp_socket_ptr& s, uint64_t uid, uint16_t cmd, const network::msg_buf_ptr& msg_buf);

    template<typename T_MSG>
    void send_msg_to_transfer(const network::tcp_socket_ptr& s, uint16_t cmd, uint64_t uid, const T_MSG& msg)
    {
        if (NULL == s)
        {
            log_error("send uid[%lu] cmd[%s:%d] to transfer failed: NULL == s", uid, CMD_DESC(cmd), cmd);
            return;
        }
        const transfer_ptr p_transfer = transfer_manager_t::get_transfer(s);
        if (NULL == p_transfer)
        {
            log_error("send uid[%lu] cmd[%s:%d] to transfer failed: NULL == p_transfer", uid, CMD_DESC(cmd), cmd);
            return;
        }
        network::msg_buf_ptr send_buf = network::wrap_msg<T_MSG>(cmd, uid, msg);
        if (NULL == send_buf)
        {
            log_error("send uid[%lu] cmd[%s:%d] to transfer[%d] failed: NULL == send_buf", uid, CMD_DESC(cmd), cmd, p_transfer->get_id());
            return;
        }

        post_network_send_msg(s, send_buf);
        log_trace("send uid[%lu] cmd[%s:%d] to transfer[%d]", uid, CMD_DESC(cmd), cmd, p_transfer->get_id());
    }

    template<typename T_MSG>
    void send_msg_to_transfer(uint32_t server_id, uint16_t cmd, uint64_t uid, const T_MSG& msg)
    {
        const network::tcp_socket_ptr& s = transfer_manager_t::get_transfer_socket(server_id);
        if (NULL == s)
        {
            log_error("send uid[%lu] cmd[%s:%d] to transfer[%d] failed: NULL == s", uid, CMD_DESC(cmd), cmd, server_id);
            return;
        }
        network::msg_buf_ptr send_buf = network::wrap_msg<T_MSG>(cmd, uid, msg);
        if (NULL == send_buf)
        {
            log_error("send uid[%lu] cmd[%s:%d] to transfer[%d] failed: NULL == send_buf", uid, CMD_DESC(cmd), cmd, server_id);
            return;
        }
        post_network_send_msg(s, send_buf);
        log_trace("send uid[%lu] cmd[%s:%d] to transfer[%d]", CMD_DESC(cmd), uid, cmd, server_id);
    }

    template<typename T_MSG>
    void send_msg_to_all_transfer(uint16_t cmd, uint64_t uid, const T_MSG& msg)
    {
        network::msg_buf_ptr send_buf = network::wrap_msg<T_MSG>(cmd, uid, msg);
        if (NULL == send_buf)
        {
            log_error("send uid[%lu] cmd[%s:%d] to all transfers failed: NULL == send_buf", uid, CMD_DESC(cmd), cmd);
            return;
        }
        for (auto p_tansfer : transfer_manager_t::get_transfer_list())
        {
            if (NULL == p_tansfer->get_socket())
            {
                log_error("send uid[%lu] cmd[%s:%d] to all transfers failed: p_tansfer->get_socket()", uid, CMD_DESC(cmd), cmd);
                continue;
            }

            post_network_send_msg(p_tansfer->get_socket(), send_buf);
            log_trace("send uid[%lu] cmd[%s:%d] to transfer[%d]", CMD_DESC(cmd), uid, cmd, p_tansfer->get_id());
        }
    }

public:
    /*
    ****game 的消息处理：area --> transfer --> center --> game
    */
    template<typename T_MSG>
    void send_msg_to_transfer(uint16_t cmd, uint64_t uid, const T_MSG& msg)
    {
        const network::tcp_socket_ptr& s = transfer_manager_t::get_avg_transfer_socket();
        if (NULL == s)
		{
			log_error("send uid[%lu] cmd[%s:%d] to transfer failed: NULL == s", uid, CMD_DESC(cmd), cmd);
            return;
        }
		uint32_t transfer_id = transfer_manager_t::get_transfer_id_by_socket(s);
        network::msg_buf_ptr send_buf = network::wrap_msg<T_MSG>(cmd, uid, msg);
        if (NULL == send_buf)
		{
			log_error("send uid[%lu] cmd[%s:%d] to transfer[%d] failed: NULL == send_buf", uid, CMD_DESC(cmd), cmd, transfer_id);
            return;
        }
        post_network_send_msg(s, send_buf);
        log_trace("send user[%lu] cmd[%s:%d] to transfer[%d]", uid, CMD_DESC(cmd), cmd, transfer_id);
    }

public:
    /*
    ****cross 的消息处理
    */
    void on_add_cross(const network::tcp_socket_ptr& s);
    void on_del_cross(const network::tcp_socket_ptr& s);
    void on_cross_msg(const network::tcp_socket_ptr& s, uint64_t uid, uint16_t cmd, const network::msg_buf_ptr& msg_buf);

	template<typename T_MSG>
	void send_msg_to_cross(uint16_t cmd, uint64_t uid, const T_MSG& msg)
	{
		const network::tcp_socket_ptr& s = cross_manager_t::get_avg_cross_socket();
		if (NULL == s)
		{
            log_error("send uid[%lu] cmd[%s:%d] to cross failed: NULL == s", uid, CMD_DESC(cmd), cmd);
            return;
		}

		network::msg_buf_ptr send_buf = network::wrap_msg<T_MSG>(cmd, uid, msg);
		if (NULL == send_buf)
		{
            log_error("send uid[%lu] cmd[%s:%d] to cross failed: NULL == send_buf", uid, CMD_DESC(cmd), cmd);
            return;
		}

		//m_cross_listener.send_msg_to_cross(s, send_buf);
		post_network_send_msg(s, send_buf);
        log_trace("send uid[%lu] cmd[%s:%d] to cross", uid, CMD_DESC(cmd), cmd);
	}

	template<typename T_MSG>
	void send_msg_to_cross(const network::tcp_socket_ptr& s, uint16_t cmd, uint64_t uid, const T_MSG& msg)
	{
		if (NULL == s)
		{
            log_error("send uid[%lu] cmd[%s:%d] to cross failed: NULL == s", uid, CMD_DESC(cmd), cmd);
            return;
		}

		network::msg_buf_ptr send_buf = network::wrap_msg<T_MSG>(cmd, uid, msg);
		if (NULL == send_buf)
		{
            log_error("send uid[%lu] cmd[%s:%d] to cross[%d] failed: NULL == send_buf", uid, CMD_DESC(cmd), cmd, cross_manager_t::get_cross_id_by_socket(s));
            return;
		}

		//m_cross_listener.send_msg_to_cross(s, send_buf);
		post_network_send_msg(s, send_buf);
		log_trace("send user[%lu] cmd[%s:%d] to cross[%d]", uid, CMD_DESC(cmd), cmd, cross_manager_t::get_cross_id_by_socket(s));
	}

public:
    static void one_day(uint32_t id, bool is_in_time);

private:
    URL_T                   m_db_url;
    network::network_t      m_network;
	transfer_listener_t		m_transfer_listener;
	cross_listener_t		m_cross_listener;
};

#endif
