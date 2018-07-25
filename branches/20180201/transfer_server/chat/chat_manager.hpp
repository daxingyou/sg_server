#ifndef __TRANSFER_CHAT_MANAGER_HPP__
#define __TRANSFER_CHAT_MANAGER_HPP__

#include "chat.hpp"
#include <vector>
#include "tcp_socket.hpp"

typedef std::vector<chat_ptr> chat_vector;

class chat_manager_t
{
public:
	static void init_chat();
    static void add_chat(const network::tcp_socket_ptr& sokcet);
    static void del_chat(const network::tcp_socket_ptr& sokcet);
	static void clear_chat();
    static const chat_vector& get_chat_list() { return m_chat_list; }

	static chat_ptr get_chat(const network::tcp_socket_ptr& socket);
	static chat_ptr get_chat(uint32_t server_id);
    static network::tcp_socket_ptr get_chat_socket(uint32_t server_id);
    static void set_chat_server_info(const network::tcp_socket_ptr& socket, uint32_t server_id, const std::string& server_name);

public:
    static chat_vector m_chat_list;
};

#endif
