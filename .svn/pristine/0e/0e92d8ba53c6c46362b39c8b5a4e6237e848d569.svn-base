#include "general_event_msg_handler.hpp"
#include "general_info_manager.hpp"
#include "general_event_manager.hpp"

bool general_event_msg_handler_t::init_game_msg_handler()
{
	bool bresult = true;

	bresult &= game_msg_handler_t::m_game_msg_handle.register_func(op_cmd::ge_push_general_event_request, handle_ge_push_general_event_request);

	return bresult;
}


bool general_event_msg_handler_t::handle_ge_push_general_event_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::ge_push_general_event_request);

	//统计
	general_info_manager_t::add_num(msg.index());

	//处理统计时间
	general_event_manager_t::on_event_process(msg.index(), msg.value(), msg.name(), msg.role_uid()) ;

	return true;
}