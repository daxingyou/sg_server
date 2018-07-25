#ifndef __GM_ADMIN_HPP__
#define __GM_ADMIN_HPP__

#include "common/common_fwd.hpp"
#include "network/tcp_socket.hpp"
#include "main/gm_server.hpp"
#include "main/gm_server_fwd.hpp"
#include "protobuf.hpp"
#include "cmd_desc.hpp"

class admin_t : public boost::enable_shared_from_this<admin_t>, private boost::noncopyable
{
public:
    template<typename T_MSG>
    void send_msg_to_center(uint16_t cmd, const T_MSG& msg)
    {
        env::server->send_msg_to_center(cmd, m_admin_id, msg);
        log_trace("send admin[%lu] cmd[%s:%d] to center", m_admin_id, CMD_DESC(cmd), cmd);
    }

    template<typename T_MSG>
    void send_msg_to_gm_client(uint16_t cmd, const T_MSG& msg)
    {
        network::msg_buf_ptr send_buf = network::wrap_msg<T_MSG>(cmd, m_admin_id, msg);
        if (NULL == send_buf)
        {
            return;
        }
        env::server->post_network_send_msg(get_socket(), send_buf);
        log_trace("send admin[%lu] cmd[%s:%d] to gm client", m_admin_id, CMD_DESC(cmd), cmd);
    }

    void disconnect_with_gm_client();

public:
    admin_t(const uint32_t admin_id, const std::string admin_name);
    admin_t(const uint32_t admin_id, const std::string admin_name, const network::tcp_socket_ptr& s);
    ~admin_t();

    void set_admin_id(uint32_t id) { m_admin_id = id; }
    uint32_t get_admin_id() { return m_admin_id; }

    void set_admin_name(std::string name) { m_admin_name = name; }
    std::string get_admin_name() { return m_admin_name; }

    void set_socket(network::tcp_socket_ptr socket) { m_socket = socket; }
    network::tcp_socket_ptr get_socket() { return m_socket.lock(); }

private:
    uint32_t        m_admin_id;
    std::string     m_admin_name;
    network::tcp_socket_wptr  m_socket;
};
typedef boost::shared_ptr<admin_t> admin_ptr;

#endif //__GM_ADMIN_HPP__