#ifndef __LOG_SERVER_HPP__
#define __LOG_SERVER_HPP__

#include "server_base.hpp"
#include "common/zdb_ex.hpp"
#include "protobuf.hpp"
#include "network.hpp"
#include "game/game_listener.hpp"
#include "center/center_listener.hpp"
#include "account/account_listener.hpp"

enum log_server_type
{
    log_server_type_global = 0, //全局日志服务器
    log_server_type_single = 1, //单组日志服务器
};

class log_server_t
    : public common::server_base_t
{
public:
    log_server_t();
    virtual ~log_server_t();

    virtual const char* server_name() { return "log"; }
    virtual bool on_init_server();
    virtual bool on_close_server();
    virtual void on_run_server();

private:
    bool load_xml();
    bool load_config_xml();
    bool load_global_server_xml();
    bool load_server_xml();
    void close_xml();

    bool init_zdb();
    void close_zdb();

    bool init_network();
    void close_network();
	void stop_network();

public:
    void post_network_close_socket(const network::tcp_socket_ptr& s) { m_network.post_close_socket(s); }

public:
    /*
    ****game 的消息处理
    */
    void on_add_game(const network::tcp_socket_ptr& s);
    void on_del_game(const network::tcp_socket_ptr& s);
    void on_game_msg(const network::tcp_socket_ptr& s, uint16_t cmd, const network::msg_buf_ptr& msg_buf, uint64_t uid);

public:
    /*
    ****center 的消息处理
    */
    void on_add_center(const network::tcp_socket_ptr& s);
    void on_del_center(const network::tcp_socket_ptr& s);
    void on_center_msg(const network::tcp_socket_ptr& s, uint16_t cmd, const network::msg_buf_ptr& msg_buf, uint64_t uid);
public:
    /*
    ****account 的消息处理
    */
    void on_add_account(const network::tcp_socket_ptr& s);
    void on_del_account(const network::tcp_socket_ptr& s);
    void on_account_msg(const network::tcp_socket_ptr& s, uint16_t cmd, const network::msg_buf_ptr& msg_buf, uint64_t uid);

public:
    uint32_t get_type() const { return m_type; }
    void set_type(uint32_t val) { m_type = val; }

private:
    uint32_t m_type = 0;
    URL_T m_db_url;
    network::network_t m_network;
    game_listener_t m_game_listener;
    center_listener_t m_center_listener;
    account_listener_t m_account_listener;
};

#endif
