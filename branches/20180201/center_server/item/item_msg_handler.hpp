#pragma once

#include "main/center_server.hpp"
#include "main/center_server_fwd.hpp"
#include "msg_handler.hpp"
#include "protos_fwd.hpp"
#include "tcp_socket.hpp"
#include "cmd_desc.hpp"

class item_msg_handler_t
{
public:
	static bool init_msg_handler();
	
public:
	static bool handle_ge_check_drop_items_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
};