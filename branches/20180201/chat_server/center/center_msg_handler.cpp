#include "center_msg_handler.hpp"
#include "main/chat_server.hpp"
#include "main/chat_server_fwd.hpp"
#include "network/network.hpp"
#include "network/msg_handler.hpp"
#include "config/config_manager.hpp"
#include "family/family_manager.hpp"
#include "country/country_manager.hpp"


USING_NS_NETWORK;
USING_NS_COMMON;

server_msg_handler_t<op_cmd::MH_BEGIN, op_cmd::EH_END, center_msg_handler_t::msg_handler_func> center_msg_handler_t::m_center_msg_handle;

bool center_msg_handler_t::init_msg_handler()
{
    bool ret = true;

    ret &= m_center_msg_handle.register_func(op_cmd::eh_test_request,     handle_eh_test_request);
	ret &= m_center_msg_handle.register_func(op_cmd::mh_reload_config_notify, handle_mh_reload_config_notify);
	ret &= m_center_msg_handle.register_func(op_cmd::eh_country_member_change_notify, handle_eh_country_member_change_notify);
	ret &= m_center_msg_handle.register_func(op_cmd::eh_family_member_change_notify, handle_eh_family_member_change_notify);

    return ret;
}

bool center_msg_handler_t::handle_eh_test_request(const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::eh_test_request);
	//log_warn("%lu chat recv [eh_test_request] from center", uid);

	proto::server::he_test_reply rep;
	env::server->send_msg_to_center(op_cmd::he_test_reply, uid, rep);

	return true;
}

bool center_msg_handler_t::handle_mh_reload_config_notify(const network::msg_buf_ptr& msg_buf)
{
	if (!load_config())
	{
		log_error("reload config fail");
	}
	else
	{
		log_info("reload config success");
	}

	return true;
}

bool center_msg_handler_t::handle_eh_country_member_change_notify(const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::eh_country_member_change_notify);

	uint32_t country_id = msg.country_id();

	if (country_id != 0)
	{
		if (msg.change_type() == 0)
		{
			country_manager_t::earse_user(country_id, uid);
		}
		else
		{
			country_manager_t::add_user(country_id, uid);
		}
	}


	return true;
}

bool center_msg_handler_t::handle_eh_family_member_change_notify(const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::eh_family_member_change_notify);

	uint64_t family_id = msg.family_id();
	if (family_id != 0)
	{
		if (msg.change_type() == 0)
		{
			family_manager_t::earse_user(family_id, uid);
		}
		else
		{
			family_manager_t::add_user(family_id, uid);
		}
	}

	return true;
}
