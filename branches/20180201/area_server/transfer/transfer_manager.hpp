#pragma once

#include "transfer.hpp"
#include <vector>
#include "tcp_socket.hpp"
#include "io_base.hpp"

typedef std::vector<transfer_ptr> transfer_vector;

class transfer_manager_t
{
public:
    static bool add_transfer(const network::tcp_socket_ptr& s);
    static void del_transfer(const network::tcp_socket_ptr& s);
	static void clear_transfer();
	static const transfer_vector& get_transfer_list() { return m_transfer_list; }

	static transfer_ptr get_transfer(const network::tcp_socket_ptr& s);
	static transfer_ptr get_transfer(uint32_t server_id);
	static uint32_t get_transfer_id_by_socket(const network::tcp_socket_ptr& s);
    static const network::tcp_socket_ptr get_transfer_socket(uint32_t server_id);
    static void set_transfer_server_info(const network::tcp_socket_ptr& s, uint32_t server_id);
	
	static const network::tcp_socket_ptr get_avg_transfer_socket();
public:
	static transfer_vector m_transfer_list;
	static size_t m_cur_index;
};
