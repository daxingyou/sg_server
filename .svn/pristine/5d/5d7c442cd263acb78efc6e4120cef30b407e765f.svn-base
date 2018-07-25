#include "lifeforce_msg_handle.hpp"
#include "lifeforce_manager.hpp"
#include "gate/gate_msg_handler.hpp"

bool lifeforce_msg_handle_t::init_msg_handler()
{
	bool bresult = true;
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_activate_life_star_request, handle_cg_activate_life_star_request);

	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_break_life_state_request, handle_cg_break_life_state_request);

	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_up_level_lifelabel_request, handle_cg_up_level_lifelabel_request);
	return bresult;
}

bool lifeforce_msg_handle_t::handle_cg_activate_life_star_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_activate_life_star_request);
	lifeforce_manager_t::level_up_lifestar_level(uid);
	return true;
}

bool lifeforce_msg_handle_t::handle_cg_break_life_state_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_break_life_state_request);
	//lifeforce_manager_t::level_up_lifestate_level(uid);
	return true;
}

bool lifeforce_msg_handle_t::handle_cg_up_level_lifelabel_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_up_level_lifelabel_request);
	lifeforce_manager_t::level_up_lifelabel_level(uid);
	return true;
}



