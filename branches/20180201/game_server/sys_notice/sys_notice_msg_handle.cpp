#include "sys_notice_msg_handle.hpp"
#include "sys_notice_manager.hpp"
#include "center/center_msg_handler.hpp"

USING_NS_NETWORK;
USING_NS_COMMON;

bool sys_notice_msg_handle_t::init_msg_handler()
{
	bool ret = true;
	ret &= center_msg_handler_t::m_center_msg_handle.register_func(op_cmd::eg_check_sys_notice_reply, handle_eg_check_sys_notice_reply);
	ret &= center_msg_handler_t::m_center_msg_handle.register_func(op_cmd::eg_syn_sys_notice_notify, handle_eg_syn_sys_notice);
	ret &= center_msg_handler_t::m_center_msg_handle.register_func(op_cmd::eg_gm_clear_notice_notify, handle_eg_gm_clear_notice_notify);
	return ret;
}

bool sys_notice_msg_handle_t::handle_eg_check_sys_notice_reply(const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::eg_check_sys_notice_reply);
	const proto::common::sys_notity_data& data_list = msg.sys_notice_list();
	for (int i = 0; i < data_list.sys_notity_data_list_size(); ++i)
	{
		sys_notice_manager_t::update_sys_notify_from_center(data_list.sys_notity_data_list(i));
	}
	
	return true;
}


bool sys_notice_msg_handle_t::handle_eg_syn_sys_notice(const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::eg_syn_sys_notice_notify);
	sys_notice_manager_t::load_sys_notify_from_center(msg.sys_notice_list());
	return true;
}

bool sys_notice_msg_handle_t::handle_eg_gm_clear_notice_notify(const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::eg_gm_clear_notice_notify);
	sys_notice_manager_t::clear_notice_list();
	return true;
}
