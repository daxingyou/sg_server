#pragma once
#include <map>
#include <boost/shared_ptr.hpp>
#include "tcp_socket.hpp"
#include "protos_fwd.hpp"

class transfer_t
{
public:
	transfer_t(const network::tcp_socket_ptr& sokcet)
        : m_socket(sokcet)
    {}
	transfer_t() {}
    ~transfer_t() {}

public:
    uint32_t get_id() const { return m_id; }
    void set_id(uint32_t val) { m_id = val; }

	uint32_t get_channel_id() const { return m_channel_id; }
	void set_channel_id(uint32_t val) { m_channel_id = val; }

    network::tcp_socket_ptr get_socket() const { return m_socket.lock(); }
    void set_socket(const network::tcp_socket_ptr& val) { m_socket = val; }

private:
	uint32_t m_id = 0;
	uint32_t m_channel_id = 0;
	network::tcp_socket_wptr m_socket;
};
typedef boost::shared_ptr<transfer_t> transfer_ptr;
