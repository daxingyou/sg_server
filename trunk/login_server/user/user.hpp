#ifndef __LOGIN_USER_HPP__
#define __LOGIN_USER_HPP__

#include <boost/shared_ptr.hpp>
#include <boost/asio/deadline_timer.hpp>
#include <map>
#include "tcp_socket.hpp"
#include "log.hpp"
#include "protobuf.hpp"
#include "cmd_desc.hpp"

class user_t
{
public:
    user_t(uint64_t uid, std::string const& guid);

public:
    bool send_msg(const network::msg_buf_ptr& buf);

    template<typename T_MSG>
    void send_msg_to_client(uint16_t cmd, const T_MSG& msg)
    {
#ifdef __ENCRYPT__
        network::msg_buf_ptr send_buf = network::wrap_msg_encrypt<T_MSG>(cmd, msg, SG_SERVER_TO_CLIENT_KEY);
#else
        network::msg_buf_ptr send_buf = network::wrap_msg_to_client<T_MSG>(cmd, msg);
#endif
        if (NULL == send_buf)
        {
            return;
        }
        if (!send_msg(send_buf))
        {
            return;
        }
        log_trace("send user[%lu] cmd[%s:%d] message to client", m_uid, CMD_DESC(cmd), cmd);
    }

    void send_msg_to_client(uint16_t cmd)
    {
#ifdef __ENCRYPT__
        network::msg_buf_ptr send_buf = network::wrap_msg_encrypt(cmd, SG_SERVER_TO_CLIENT_KEY);
#else
        network::msg_buf_ptr send_buf = network::wrap_msg_to_client(cmd);
#endif
        if (NULL == send_buf)
        {
            return;
        }
        if (!send_msg(send_buf))
        {
            return;
        }
        log_trace("send user[%lu] cmd[%s:%d] message to client", m_uid, CMD_DESC(cmd), cmd);
    }

public:
    uint64_t get_uid() const{ return m_uid; }
    void set_uid(uint64_t val) { m_uid = val; }

    std::string get_guid() const{ return m_guid; }
    void set_guid(std::string const& val){ m_guid = val; }

    //uint32_t get_ch_type() const { return m_ch_type; }
    //void set_ch_type(uint32_t val) { m_ch_type = val; }

    network::tcp_socket_ptr get_client_socket(){ return m_socket.lock(); }
    network::tcp_socket_ptr reset_client_socket(network::tcp_socket_ptr const& s);

	// 防止点击多次，自己踢自己
	bool check_last_login();

private:
    uint64_t        m_uid;
    //uint32_t        m_ch_type;
    std::string     m_guid;
    network::tcp_socket_wptr  m_socket;
	uint32_t		m_last_login;
};
typedef boost::shared_ptr<user_t> user_ptr;

class socket_data_t
{
public:
	socket_data_t() {};

    bool check_msg_bomb(uint32_t cmd);
    void reset_msg_bomb()
    {
        m_last_msg_time = 0;
        m_msg_bomb_count = 0;
    }

public:
    user_ptr p_user = NULL;
    boost::shared_ptr<boost::asio::deadline_timer> p_session_timer = NULL;

private:
	uint64_t					m_last_msg_time = 0;
	uint32_t					m_msg_bomb_count = 0;
    //uint32_t					m_last_cmd = 0;
};

#endif