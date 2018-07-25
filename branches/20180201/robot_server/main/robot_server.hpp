#ifndef __ROBOT_SERVER_HPP__
#define __ROBOT_SERVER_HPP__

#include "common/server_base.hpp"
#include "main/robot_server_fwd.hpp"
#include "network/network.hpp"
#include "network/protobuf.hpp"

#include <boost/asio/deadline_timer.hpp>

class robot_server_t : public common::server_base_t
{
public:
    robot_server_t();
    ~robot_server_t();

    virtual const char* server_name();
    virtual bool  on_init_server();
    virtual bool  on_close_server();
    virtual void  on_run_server();

    bool load_ini(const std::string& ini_path);

    bool connect(const std::string& ip, uint16_t port, network::connector_callback_t* connector_cb);
    bool disconnect(const network::tcp_socket_ptr& s);

    bool send_msg(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& buf);

	void start_robot_heart();	/// 定时回调
	void stop_robot_heart();

	void start_robot_login();
	void stop_robot_login();



private:
	void on_robot_heart(const boost::system::error_code& ec);
	void on_robot_login(const boost::system::error_code& ec);

private:
    network::network_t              m_network;
	boost::asio::deadline_timer     m_heart_timer;
    boost::asio::deadline_timer     m_login_timer;
};


template<typename T_MSG>
bool send_msg(const network::tcp_socket_ptr& s, uint16_t cmd, const T_MSG& msg)
{
    network::msg_buf_ptr send_buf = network::wrap_msg(cmd, msg);
    if (NULL == send_buf)
    {
        return false;
    }
    return env::server->send_msg(s, send_buf);
}


#endif  //__ROBOT_SERVER_HPP__
