#include "country_msg_handle.hpp"
#include "log.hpp"
#include "config/config_manager.hpp"
#include "gate/gate_msg_handler.hpp"
#include "role/role_manager.hpp"
#include "client.pb.h"
#include "global_id.hpp"
#include "scene/scene_manager.hpp"
#include "center/center_msg_handler.hpp"
#include "role/money_manager.hpp"
#include "tblh/errcode_enum.hpp"
#include "country_manager.hpp"

bool country_msg_handle_t::init_gate_msg_handler()
{
	return true;
}

bool country_msg_handle_t::init_center_msg_handler()
{
	bool ret = true;

	ret &= center_msg_handler_t::m_center_msg_handle.register_func(op_cmd::eg_update_country_notify, handle_eg_update_country_notify);
	ret &= center_msg_handler_t::m_center_msg_handle.register_func(op_cmd::eg_country_name_change_request, handle_eg_country_name_change_request);
	ret &= center_msg_handler_t::m_center_msg_handle.register_func(op_cmd::eg_country_year_name_request, handle_eg_country_year_name_request);

	return ret;
}


bool country_msg_handle_t::handle_eg_update_country_notify(const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::eg_update_country_notify);

	country_manager_t::update_country_common(msg);

	return true;
}

bool country_msg_handle_t::handle_eg_country_name_change_request(const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::eg_country_name_change_request);

	const role_ptr& p_role = role_manager_t::find_role(uid);
	if (NULL == p_role) {
		log_error("NULL == p_role[%lu]", uid);
		return false;
	}

	uint32_t back_code = country_manager_t::change_country_name_flag(p_role, msg.name(), msg.flag_id());
	if (back_code != common::errcode_enum::error_ok) {
		proto::client::gc_change_country_name_reply reply;
		reply.set_reply_code(back_code);
		p_role->send_msg_to_client(op_cmd::gc_change_country_name_reply, reply);
	}

	return true;
}

bool country_msg_handle_t::handle_eg_country_year_name_request(const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::eg_country_name_change_request);

	const role_ptr& p_role = role_manager_t::find_role(uid);
	if (NULL == p_role) {
		log_error("NULL == p_role[%lu]", uid);
		return false;
	}

	uint32_t back_code = country_manager_t::change_country_year_name(p_role, msg.name());
	if (back_code != common::errcode_enum::error_ok) {
		proto::client::gc_modify_year_name_reply reply;
		reply.set_reply_code(back_code);
		p_role->send_msg_to_client(op_cmd::gc_modify_year_name_reply, reply);
	}

	return true;
}
