#ifndef __ACCOUNT_LOGIN_HPP__
#define __ACCOUNT_LOGIN_HPP__

#include <boost/shared_ptr.hpp>
#include "tcp_socket.hpp"
#include "protocpp/common.pb.h" 

class login_t
{
public:
	login_t() {}
    ~login_t() {}

    uint32_t get_id() const{ return m_id; }
    void set_id(uint32_t val) { m_id = val; }

    uint32_t get_user_num() const { return m_user_num; }
    void set_user_num(uint32_t val) { m_user_num = val; }

    const std::string& get_ip() const { return m_ip; }
    void set_ip(std::string val) { m_ip = val; }

    uint32_t get_port() const { return m_port; }
    void set_port(uint32_t val) { m_port = val; }

	proto::common::SERVER_STATE get_state() const { return m_state; }
	void set_state(proto::common::SERVER_STATE val) { m_state = val; }

	const std::string& get_name() const { return m_name; }
	void set_name(std::string val) { m_name = val; }

	uint32_t get_is_new() const { return m_is_new; }
	void set_is_new(uint32_t val) { m_is_new = val; }

	uint32_t get_can_create_new_role() { return m_can_create_new_role; }
	void set_can_create_new_role(uint32_t val) { m_can_create_new_role = val; }

    network::tcp_socket_ptr get_socket() { return m_socket.lock(); }
    network::tcp_socket_ptr reset_socket(network::tcp_socket_ptr const& s);

private:
    uint32_t m_id        = 0;
    uint32_t m_user_num  = 0;
    std::string m_ip     = "";
    uint32_t m_port      = 0;
	proto::common::SERVER_STATE m_state  = proto::common::SERVER_STATE::SERVER_STATE_CLOSE;
	std::string m_name   = "";
	uint32_t m_is_new    = 1;
	uint32_t m_can_create_new_role = 0;
    network::tcp_socket_wptr m_socket;
};
typedef boost::shared_ptr<login_t> login_ptr;

struct login_socket_data_t
{
	login_ptr p_login = NULL;
};
#endif