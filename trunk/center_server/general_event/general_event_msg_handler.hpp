#ifndef _GENERAL_EVENT_MSG_HANDLER_HPP_
#define _GENERAL_EVENT_MSG_HANDLER_HPP_

#include "network/tcp_socket.hpp"
#include "message.hpp"
#include "game/game_msg_handler.hpp"

class general_event_msg_handler_t 
{
public:
	static bool init_game_msg_handler();

	static bool handle_ge_push_general_event_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
};

#endif
