#include "city_msg_handler.hpp"
#include "city_manager.hpp"
#include "center/center_msg_handler.hpp"

USING_NS_NETWORK;
USING_NS_COMMON;

bool city_msg_handler_t::init_msg_handler()
{
	bool ret = true;
	ret &= center_msg_handler_t::m_center_msg_handle.register_func(op_cmd::eg_get_city_list_reply, handle_eg_get_city_list_reply);

	return ret;
}

bool city_msg_handler_t::handle_eg_get_city_list_reply(const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::eg_get_city_list_reply);

	city_manager_t::load_from_center(msg);

	return true;
}
