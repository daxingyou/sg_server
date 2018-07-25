#ifndef __TRANSFER_CHAT_HPP__
#define __TRANSFER_CHAT_HPP__
#include <map>
#include <boost/shared_ptr.hpp>
#include "tcp_socket.hpp"
#include "protos_fwd.hpp"

class chat_user_t;
typedef boost::shared_ptr<chat_user_t> chat_user_ptr;
typedef std::map<uint64_t, chat_user_ptr> chat_user_map;

class chat_t
{
public:
    chat_t(const network::tcp_socket_ptr& sokcet)
        : m_socket(sokcet)
    {}
	chat_t() {}
    ~chat_t() {}

public:
	void add_chat_user(chat_user_ptr p_user);
	void erase_chat_user(uint64_t user_uid);
	chat_user_ptr get_user(uint64_t user_uid);

public:
    uint32_t get_server_id() const { return m_server_id; }
    void set_server_id(uint32_t val) { m_server_id = val; }

	uint32_t get_channel_id() const { return m_channel_id; }
	void set_channel_id(uint32_t val) { m_channel_id = val; }

	std::string get_server_name() { return m_server_name; }
	void set_server_name(const std::string& server_name) { m_server_name = server_name; }

    network::tcp_socket_ptr get_socket() const { return m_socket.lock(); }
    void set_socket(const network::tcp_socket_ptr& val) { m_socket = val; }

private:
	uint32_t m_server_id = 0;
	uint32_t m_channel_id = 0;
	std::string m_server_name = "";
	network::tcp_socket_wptr m_socket;
	chat_user_map m_user_list;
};
typedef boost::shared_ptr<chat_t> chat_ptr;

#endif