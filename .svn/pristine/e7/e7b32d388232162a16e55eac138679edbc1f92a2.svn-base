#ifndef __DB_DB_SERVER_HPP__
#define __DB_DB_SERVER_HPP__

#include "server_base.hpp"
#include "common/zdb_ex.hpp"
#include "protobuf.hpp"
#include "network.hpp"
#include "account/account_manager.hpp"
#include "game/game_manager.hpp"
#include "game/game_listener.hpp"
#include "center/center_listener.hpp"
#include "account/account_connector.hpp"
#include "cmd_desc.hpp"

class db_server_t
    : public common::server_base_t
{
public:
    db_server_t();
    virtual ~db_server_t();

    virtual const char* server_name() { return "db"; }
    virtual bool on_init_server();
    virtual bool on_close_server();
    virtual void on_run_server();

	bool check_terminate() const;

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
	void stop_network();

public:
	void post_network_send_msg(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& buf);

    void post_network_close_socket(const network::tcp_socket_ptr& s) { m_network.post_close_socket(s); }

public:
    /*
    ****game 的消息处理
    */
	void on_add_game(const network::tcp_socket_ptr& s);
	void on_del_game(const network::tcp_socket_ptr& s);
    void on_game_msg(const network::tcp_socket_ptr& s, uint64_t uid, uint16_t cmd, const network::msg_buf_ptr& msg_buf);
    ///void on_game_msg(const network::tcp_socket_ptr& s, uint16_t cmd, const network::msg_buf_ptr& msg_buf);

    template<typename T_MSG>
    void send_msg_to_game(const network::tcp_socket_ptr& s, uint16_t cmd, uint64_t uid, const T_MSG& msg)
    {
        uint32_t game_id = 0;
        network::tcp_socket_ptr game_socket = NULL;
        if (NULL == s)
        {
            boost::tie(game_id, game_socket) = game_manager_t::get_game_data_by_uid(uid);
        }
        else
        {
            game_socket = s;
            game_id = game_manager_t::get_game_id_by_socket(s);
        }
        if (NULL == game_socket)
        {
            log_error("send uid[%lu] cmd[%s:%d] to game failed: NULL == game_socket", uid, CMD_DESC(cmd), cmd);
            return;
        }
        network::msg_buf_ptr send_buf = network::wrap_msg<T_MSG>(cmd, uid, msg);
        if (NULL == send_buf)
        {
            log_error("send uid[%lu] cmd[%s:%d] to game[%d] failed: NULL == send_buf", uid, CMD_DESC(cmd), cmd, game_id);
            return;
        }
        //m_game_listener.send_msg_to_game(game_socket, send_buf);
		post_network_send_msg(game_socket, send_buf);
        log_trace("send user[%lu] cmd[%s:%d] to game[%d]", uid, CMD_DESC(cmd), cmd, game_id);
    }

    template<typename T_MSG>
    void send_msg_to_game(const network::tcp_socket_ptr& s, uint16_t cmd, const T_MSG& msg)
    {
        if (NULL == s)
        {
            log_error("send cmd[%s:%d] to game failed: NULL == s", CMD_DESC(cmd), cmd);
            return;
        }
        network::msg_buf_ptr send_buf = network::wrap_msg<T_MSG>(cmd, msg);
        if (NULL == send_buf)
        {
            log_error("send cmd[%s:%d] to game[%d] failed: NULL == send_buf", CMD_DESC(cmd), cmd, game_manager_t::get_game_id_by_socket(s));
            return;
        }
		//m_game_listener.send_msg_to_game(s, send_buf);
		post_network_send_msg(s, send_buf);
        log_trace("send cmd[%s:%d] to game[%d]", CMD_DESC(cmd), cmd, game_manager_t::get_game_id_by_socket(s));
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
		//m_center_listener.send_msg_to_center(send_buf);
		post_network_send_msg(m_center_listener.get_socket(), send_buf);
        log_trace("send role[%lu] cmd[%s:%d] to center", uid, CMD_DESC(cmd), cmd);
    }

public:
    /*
    ****account 的消息处理
    */
	void on_add_account(const network::tcp_socket_ptr& s);
	void on_del_account(const network::tcp_socket_ptr& s);
	void on_account_msg(const network::tcp_socket_ptr& s, uint64_t uid, uint16_t cmd, const network::msg_buf_ptr& msg_buf);

	template<typename T_MSG>
	void send_msg_to_account(uint16_t cmd, uint64_t uid, const T_MSG& msg)
	{
		network::msg_buf_ptr send_buf = network::wrap_msg<T_MSG>(cmd, uid, msg);
        if (NULL == send_buf)
        {
            log_error("send uid[%lu] cmd[%s:%d] to account failed: NULL == send_buf", uid, CMD_DESC(cmd), cmd);
            return;
        }
        account_ptr p_account = account_manager_t::get_account_by_rand();
        if (NULL == p_account)
        {
            log_error("NULL == p_account");
            return;
        }
		post_network_send_msg(p_account->get_socket(), send_buf);
		log_trace("send uid[%lu] cmd[%s:%d] to account[%d]", uid, CMD_DESC(cmd), cmd, p_account->get_id());
	}

    template<typename T_MSG>
    void send_msg_to_all_accounts(uint16_t cmd, uint64_t uid, const T_MSG& msg)
    {
        network::msg_buf_ptr send_buf = network::wrap_msg<T_MSG>(cmd, uid, msg);
        if (NULL == send_buf)
        {
            log_error("send uid[%lu] cmd[%s:%d] to account failed: NULL == send_buf", uid, CMD_DESC(cmd), cmd);
            return;
        }
        for (size_t idx = 0; idx < account_manager_t::get_account_size(); ++idx)
        {
            account_ptr p_account = account_manager_t::get_account_list().at(idx);
            if (NULL != p_account)
            {
                post_network_send_msg(p_account->get_socket(), send_buf);
                log_trace("send cmd[%s:%d] message to account[%d]", CMD_DESC(cmd), cmd, p_account->get_id());
            }
        }
    }

private:
    URL_T m_db_url;
    network::network_t m_network;
    game_listener_t m_game_listener;
    center_listener_t m_center_listener;
    std::vector<account_connector_t> m_account_connector_list;
};
#endif
