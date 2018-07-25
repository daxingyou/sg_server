#include "item_msg_handler.hpp"
#include "item/item_manager.hpp"
#include "game/game_msg_handler.hpp"

USING_NS_NETWORK;
USING_NS_COMMON;

bool item_msg_handler_t::init_msg_handler()
{
	bool ret = true;
	ret &= game_msg_handler_t::m_game_msg_handle.register_func(op_cmd::ge_check_drop_items_request, handle_ge_check_drop_items_request);
	
	return ret;
}

bool item_msg_handler_t::handle_ge_check_drop_items_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::ge_check_drop_items_request);

	proto::server::eg_check_drop_items_reply msg_reply;
	proto::common::item_limit_data *p_item_limit_single = NULL;
	item_limit_ptr p_item_limit = item_limit_ptr();

	for (int i = 0; i < msg.item_list_size(); ++i)
	{
		p_item_limit = item_manager_t::give_item(msg.item_list(i).item_tid(), msg.item_list(i).item_num());
		if (NULL == p_item_limit)
		{
			log_error("item_limit not find item_limit[%d]", msg.item_list(i).item_tid());
			continue;
		}
		p_item_limit_single = msg_reply.add_item_list();
		if (NULL == p_item_limit_single)
		{
			log_error("[wys] eg_check_drop_items_reply add_item_list fail pos[%d]", i);
			return false;
		}
		p_item_limit_single->set_item_tid(p_item_limit->get_item_tid());
		p_item_limit_single->set_item_num(p_item_limit->get_count());
	}
	env::server->send_msg_to_all_games(op_cmd::eg_check_drop_items_reply, uid, msg_reply);

	return true;
}
