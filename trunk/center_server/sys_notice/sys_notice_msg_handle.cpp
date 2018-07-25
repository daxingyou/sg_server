#include "sys_notice_msg_handle.hpp"
#include "game/game_msg_handler.hpp"
#include "main/center_server_fwd.hpp"
#include "sys_notice_manager.hpp"

USING_NS_NETWORK;
USING_NS_COMMON;

bool sys_notice_msg_handle_t::init_msg_handler()
{
	bool ret = true;
	ret &= game_msg_handler_t::m_game_msg_handle.register_func(op_cmd::ge_check_sys_notice_request, handle_ge_check_sys_notice_request);
	ret &= game_msg_handler_t::m_game_msg_handle.register_func(op_cmd::ge_gm_clear_notice_request, handle_ge_gm_clear_notice_request);
	return ret;
}

bool sys_notice_msg_handle_t::handle_ge_check_sys_notice_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::ge_check_sys_notice_request);
	const proto::common::sys_notity_data& data_list = msg.sys_notice_list();
	if (0 == data_list.sys_notity_data_list_size())
	{
		return true;
	}
	proto::server::eg_check_sys_notice_reply msg_reply;
	proto::common::sys_notity_data* p_data = msg_reply.mutable_sys_notice_list();
	for (int i = 0; i < data_list.sys_notity_data_list_size(); ++i)
	{
		const proto::common::sys_notity_single& single = data_list.sys_notity_data_list(i);
		if (!sys_notice_manager_t::update_limit_count(single))
		{
			continue;
		}
		sys_notice_ptr p_sys_notice = sys_notice_manager_t::get_sys_notice(single.type(), single.param());
		if (NULL == p_sys_notice)
		{
			log_warn("NULL == p_sys_notice type[%d] param[%d]", single.type(), single.param());
			continue;
		}
		proto::common::sys_notity_single* p_single = p_data->add_sys_notity_data_list();
		p_sys_notice->peek_data(p_single);
	}
	
	if (0 != p_data->sys_notity_data_list_size())
	{
		env::server->send_msg_to_all_games(op_cmd::eg_check_sys_notice_reply, 0, msg_reply);
	}

	return true;
}

bool sys_notice_msg_handle_t::handle_ge_gm_clear_notice_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::ge_gm_clear_notice_request);
	sys_notice_manager_t::clear_notice_list();
	proto::server::eg_gm_clear_notice_notify re_msg;
	env::server->send_msg_to_all_games(op_cmd::eg_gm_clear_notice_notify, 0, re_msg);
	return true;
}
