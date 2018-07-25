#include "city_msg_handler.hpp"
#include "city_manager.hpp"
#include "game/game_msg_handler.hpp"

USING_NS_NETWORK;
USING_NS_COMMON;

bool city_msg_handler_t::init_msg_handler()
{
	bool ret = true;
	ret &= game_msg_handler_t::m_game_msg_handle.register_func(op_cmd::ge_city_trade_add_prosperity_value_notify, handle_ge_city_trade_add_prosperity_value_notify);
	//ret &= game_msg_handler_t::m_game_msg_handle.register_func(op_cmd::ge_get_city_list_request, handle_ge_get_city_list_request);
	
	return ret;
}

bool city_msg_handler_t::handle_ge_city_trade_add_prosperity_value_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::ge_city_trade_add_prosperity_value_notify);

	city_manager_t::add_city_prosperity_value(msg.city_id(), msg.add_prosperity_value());

	return true;
}

bool city_msg_handler_t::handle_ge_get_city_list_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::ge_get_city_list_request);

// 	proto::server::eg_get_city_list_reply reply;
// 	city_manager_t::peek_city_list(reply);
// 
// 	env::server->send_msg_to_game(s, op_cmd::eg_get_city_list_reply, 0, reply);

	return true;
}
