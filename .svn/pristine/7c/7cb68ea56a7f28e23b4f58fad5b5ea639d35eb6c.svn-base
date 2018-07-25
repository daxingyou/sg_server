#include "business_activity_msg_handler.hpp"
#include "business_activity_manager.hpp"
#include "tblh/errcode_enum.hpp"


bool business_activity_msg_handler_t::init_msg_handler()
{
	gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_get_money_tree_request, handle_cg_get_money_tree_request);
	gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_shake_money_tree_request, handle_cg_shake_money_tree_request);
	gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_levelup_money_tree_request, handle_cg_levelup_money_tree_request);

	gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_get_7d_login_request, handle_cg_get_7d_login_request);
	gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_get_7d_login_prize_request, handle_cg_get_7d_login_prize_request);

	gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_get_month_card_info_request, handle_cg_get_month_card_info_request);
	gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_buy_month_card_request, handle_cg_buy_month_card_request);
	gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_get_month_card_prize_request, handle_cg_get_month_card_prize_request);

	gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_get_level_fund_info_request, handle_cg_get_level_fund_info_request);
	gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_get_level_fund_prize_request, handle_cg_get_level_fund_prize_request);
	gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_buy_level_fund_request, handle_cg_buy_level_fund_prize_request);

	gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_get_exchange_info_request, handle_cg_get_exchange_info_request);
	gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_get_exchange_prize_request, handle_cg_get_exchange_prize_request);

	gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_get_limit_buy_info_request, handle_cg_get_limit_buy_info_request);
	gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_buy_limit_request, handle_cg_buy_limit_request);

	gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_get_sd_target_info_request, handle_cg_get_sd_target_info_request);
	gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_get_sd_target_red_request, handle_cg_get_sd_target_red_request);
	gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_get_sd_target_prize_request, handle_cg_get_sd_target_prize_request);

	gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_recharge_total_data_request, handle_cg_recharge_total_data_request);
	gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_recharge_total_award_request, handle_cg_recharge_total_award_request);
	
	gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_gold_luckydraw_data_request, handle_cg_gold_luckydraw_data_request);
	gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_gold_luckydraw_award_request, handle_cg_gold_luckydraw_award_request);

	gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_activity_shop_get_info_request, handle_cg_activity_shop_get_info_request);
	gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_activity_shop_buy_request, handle_cg_activity_shop_buy_request);

	return true;
}

/*Ò¡Ç®Ê÷*/
bool business_activity_msg_handler_t::handle_cg_get_money_tree_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_get_money_tree_request);
	role_ptr p_role = role_manager_t::find_role(uid);
	if (p_role == NULL)
	{
		log_error("p_role is null [%lu]", uid);
		return false;
	}

	proto::client::gc_get_money_tree_reply reply;
	business_activity_manager_t::get_money_tree_info(p_role, reply);
	p_role->send_msg_to_client(op_cmd::gc_get_money_tree_reply, reply);
	return true;
}

bool business_activity_msg_handler_t::handle_cg_shake_money_tree_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_shake_money_tree_request);
	role_ptr p_role = role_manager_t::find_role(uid);
	if (p_role == NULL)
	{
		log_error("p_role is null [%lu]", uid);
		return false;
	}

	proto::client::gc_shake_money_tree_reply reply;
	uint32_t reply_code = business_activity_manager_t::shake_money_tree(p_role, reply);
	reply.set_reply_code(reply_code);

	proto::common::role_change_data* p_rcd = reply.mutable_rcd();
	if (p_rcd) {
		p_role->get_personal_info(p_rcd->mutable_per_info());
	}

	p_role->send_msg_to_client(op_cmd::gc_shake_money_tree_reply, reply);
	return true;
}

bool business_activity_msg_handler_t::handle_cg_levelup_money_tree_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_levelup_money_tree_request);
	role_ptr p_role = role_manager_t::find_role(uid);
	if (p_role == NULL)
	{
		log_error("p_role is null [%lu]", uid);
		return false;
	}

	proto::client::gc_levelup_money_tree_reply reply;
	uint32_t reply_code = business_activity_manager_t::level_up_money_tree(p_role, reply);
	reply.set_reply_code(reply_code);
	proto::common::role_change_data* p_rcd = reply.mutable_rcd();
	if (p_rcd) {
		p_role->get_personal_info(p_rcd->mutable_per_info());
	}

	p_role->send_msg_to_client(op_cmd::gc_levelup_money_tree_reply, reply);
	return true;
}


//ÆßÈÕµÇÂ¼
bool business_activity_msg_handler_t::handle_cg_get_7d_login_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_get_7d_login_request);
	role_ptr p_role = role_manager_t::find_role(uid);
	if (p_role == NULL)
	{
		log_error("p_role is null [%lu]", uid);
		return false;
	}

	proto::client::gc_get_7d_login_reply reply;
	uint32_t reply_code = business_activity_manager_t::get_sevenday_login_info(p_role, reply);
	reply.set_reply_code(reply_code);
	
	p_role->send_msg_to_client(op_cmd::gc_get_7d_login_reply, reply);
	return true;

}
bool business_activity_msg_handler_t::handle_cg_get_7d_login_prize_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_get_7d_login_prize_request);
	role_ptr p_role = role_manager_t::find_role(uid);
	if (p_role == NULL)
	{
		log_error("p_role is null [%lu]", uid);
		return false;
	}

	proto::client::gc_get_7d_login_prize_reply reply;
	uint32_t reply_code = business_activity_manager_t::get_sevenday_login_prize(p_role, msg.index(), reply);
	reply.set_reply_code(reply_code);
	reply.set_index(msg.index());
	proto::common::role_change_data* p_rcd = reply.mutable_rcd();
	if (p_rcd) {
		p_role->get_personal_info(p_rcd->mutable_per_info());
	}

	p_role->send_msg_to_client(op_cmd::gc_get_7d_login_prize_reply, reply);
	return true;
}


//ÔÂ¿¨
bool business_activity_msg_handler_t::handle_cg_get_month_card_info_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_get_month_card_info_request);
	role_ptr p_role = role_manager_t::find_role(uid);
	if (p_role == NULL)
	{
		log_error("p_role is null [%lu]", uid);
		return false;
	}

	proto::client::gc_get_month_card_info_reply reply;
	uint32_t reply_code = business_activity_manager_t::get_month_card_info(p_role, reply);
	reply.set_reply_code(reply_code);
	p_role->send_msg_to_client(op_cmd::gc_get_month_card_info_reply, reply);

	return true;
}

bool business_activity_msg_handler_t::handle_cg_buy_month_card_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_buy_month_card_request);
	role_ptr p_role = role_manager_t::find_role(uid);
	if (p_role == NULL)
	{
		log_error("p_role is null [%lu]", uid);
		return false;
	}

	proto::client::gc_buy_month_card_reply reply;
	business_activity_manager_t::buy_month_card(p_role, msg.card_id() );
	p_role->send_msg_to_client(op_cmd::gc_buy_month_card_reply, reply);
	return true;
}

bool business_activity_msg_handler_t::handle_cg_get_month_card_prize_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_get_month_card_prize_request);
	role_ptr p_role = role_manager_t::find_role(uid);
	if (p_role == NULL)
	{
		log_error("p_role is null [%lu]", uid);
		return false;
	}

	proto::client::gc_get_month_card_prize_reply reply;
	uint32_t reply_code = business_activity_manager_t::get_month_card_ed_prize(p_role, msg.card_id(),  reply.mutable_rcd() );
	reply.set_reply_code(reply_code);
	reply.set_card_id(msg.card_id());
	proto::common::role_change_data* p_rcd = reply.mutable_rcd();
	if (p_rcd) {
		p_role->get_personal_info(p_rcd->mutable_per_info());
	}
	p_role->send_msg_to_client(op_cmd::gc_get_month_card_prize_reply, reply);
	return true;
}

//»ù½ð
bool business_activity_msg_handler_t::handle_cg_get_level_fund_info_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_get_level_fund_info_request);
	role_ptr p_role = role_manager_t::find_role(uid);
	if (p_role == NULL)
	{
		log_error("p_role is null [%lu]", uid);
		return false;
	}

	proto::client::gc_get_level_fund_info_reply reply;
	business_activity_manager_t::get_level_fund_info(p_role, reply );
	p_role->send_msg_to_client(op_cmd::gc_get_level_fund_info_reply, reply);
	return true;
}

bool business_activity_msg_handler_t::handle_cg_get_level_fund_prize_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_get_level_fund_prize_request);
	role_ptr p_role = role_manager_t::find_role(uid);
	if (p_role == NULL)
	{
		log_error("p_role is null [%lu]", uid);
		return false;
	}

	proto::client::gc_get_level_fund_prize_reply  reply;
	uint32_t reply_code = business_activity_manager_t::get_level_fund_prize(p_role, msg.id(), reply.mutable_rcd() );
	reply.set_reply_code(reply_code);
	reply.set_id(msg.id());
	proto::common::role_change_data* p_rcd = reply.mutable_rcd();
	if (p_rcd) {
		p_role->get_personal_info(p_rcd->mutable_per_info());
	}
	p_role->send_msg_to_client(op_cmd::gc_get_level_fund_prize_reply, reply);
	return true;
}

bool business_activity_msg_handler_t::handle_cg_buy_level_fund_prize_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_buy_level_fund_request);
	role_ptr p_role = role_manager_t::find_role(uid);
	if (p_role == NULL)
	{
		log_error("p_role is null [%lu]", uid);
		return false;
	}

	proto::client::gc_buy_level_fund_reply reply;
	uint32_t reply_code = business_activity_manager_t::buy_level_fund(p_role, reply.mutable_rcd());
	reply.set_reply_code(reply_code);
	proto::common::role_change_data* p_rcd = reply.mutable_rcd();
	if (p_rcd) {
		p_role->get_personal_info(p_rcd->mutable_per_info());
	}
	p_role->send_msg_to_client(op_cmd::gc_buy_level_fund_reply, reply);
	return true;
}

bool business_activity_msg_handler_t::handle_cg_get_exchange_info_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_get_exchange_info_request);
	role_ptr p_role = role_manager_t::find_role(uid);
	if (p_role == NULL)
	{
		log_error("p_role is null [%lu]", uid);
		return false;
	}

	proto::client::gc_get_exchange_info_reply reply;
	business_activity_manager_t::get_exchange_item_info(p_role, reply);
	p_role->send_msg_to_client(op_cmd::gc_get_exchange_info_reply, reply);
	return true;
}

bool business_activity_msg_handler_t::handle_cg_get_exchange_prize_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_get_exchange_prize_request);
	role_ptr p_role = role_manager_t::find_role(uid);
	if (p_role == NULL)
	{
		log_error("p_role is null [%lu]", uid);
		return false;
	}

	proto::client::gc_get_exchange_prize_reply reply;
	uint32_t reply_code = business_activity_manager_t::get_exchange_item_prize(p_role, msg.id(), reply.mutable_rcd());
	reply.set_reply_code(reply_code);
	reply.set_id(msg.id());
	proto::common::role_change_data* p_rcd = reply.mutable_rcd();
	if (p_rcd) {
		p_role->get_personal_info(p_rcd->mutable_per_info());
	}
	p_role->send_msg_to_client(op_cmd::gc_get_exchange_prize_reply, reply);
	return true;
}


bool business_activity_msg_handler_t::handle_cg_get_limit_buy_info_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_get_limit_buy_info_request);
	role_ptr p_role = role_manager_t::find_role(uid);
	if (p_role == NULL)
	{
		log_error("p_role is null [%lu]", uid);
		return false;
	}

	proto::client::gc_get_limit_buy_info_reply reply;
	business_activity_manager_t::get_limit_buy_info(p_role, reply);
	p_role->send_msg_to_client(op_cmd::gc_get_limit_buy_info_reply, reply);
	return true;
}


bool business_activity_msg_handler_t::handle_cg_buy_limit_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_buy_limit_request);
	role_ptr p_role = role_manager_t::find_role(uid);
	if (p_role == NULL)
	{
		log_error("p_role is null [%lu]", uid);
		return false;
	}

	proto::client::gc_buy_limit_reply reply;
	uint32_t reply_code = business_activity_manager_t::buy_limit(p_role, msg.id(), reply.mutable_rcd());
	reply.set_reply_code(reply_code);
	reply.set_id(msg.id());
	proto::common::role_change_data* p_rcd = reply.mutable_rcd();
	if (p_rcd) {
		p_role->get_personal_info(p_rcd->mutable_per_info());
	}
	p_role->send_msg_to_client(op_cmd::gc_buy_limit_reply, reply);
	return true;
}


bool business_activity_msg_handler_t::handle_cg_get_sd_target_info_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_get_sd_target_info_request);
	role_ptr p_role = role_manager_t::find_role(uid);
	if (p_role == NULL)
	{
		log_error("p_role is null [%lu]", uid);
		return false;
	}

	proto::client::gc_get_sd_target_info_reply reply;
	business_activity_manager_t::get_sevenday_target_info(p_role, msg.day(), reply);
	p_role->send_msg_to_client(op_cmd::gc_get_sd_target_info_reply, reply);
	return true;
}


bool business_activity_msg_handler_t::handle_cg_get_sd_target_red_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_get_sd_target_red_request);
	role_ptr p_role = role_manager_t::find_role(uid);
	if (p_role == NULL)
	{
		log_error("p_role is null [%lu]", uid);
		return false;
	}

	proto::client::gc_get_sd_target_red_reply reply;
	business_activity_manager_t::get_sevenday_red_num(p_role, reply );
	p_role->send_msg_to_client(op_cmd::gc_get_sd_target_red_reply, reply);
	return true;
}


bool business_activity_msg_handler_t::handle_cg_get_sd_target_prize_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_get_sd_target_prize_request);
	role_ptr p_role = role_manager_t::find_role(uid);
	if (p_role == NULL)
	{
		log_error("p_role is null [%lu]", uid);
		return false;
	}

	proto::client::gc_get_sd_target_prize_reply reply;
	uint32_t reply_code = business_activity_manager_t::get_sevenday_target_prize(p_role, msg.index(), reply.mutable_rcd());
	reply.set_index(msg.index());
	reply.set_reply_code(reply_code);
	p_role->send_msg_to_client(op_cmd::gc_get_sd_target_prize_reply, reply);
	return true;
}

bool business_activity_msg_handler_t::handle_cg_recharge_total_data_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_recharge_total_data_request);
	role_ptr p_role = role_manager_t::find_role(uid);
	if (p_role == NULL)
	{
		log_error("p_role is null [%lu]", uid);
		return false;
	}

	proto::client::gc_recharge_total_data_reply reply;
	business_activity_manager_t::peek_recharge_total_client_data(p_role, reply.mutable_recharge_total());
	p_role->send_msg_to_client(op_cmd::gc_recharge_total_data_reply, reply);
	return true;
}

bool business_activity_msg_handler_t::handle_cg_recharge_total_award_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_recharge_total_award_request);
	role_ptr p_role = role_manager_t::find_role(uid);
	if (p_role == NULL)
	{
		log_error("p_role is null [%lu]", uid);
		return false;
	}

	proto::client::gc_recharge_total_award_reply reply;
	uint32_t reply_code = business_activity_manager_t::recharge_total_award(p_role, msg.recharge_total_id(), reply.mutable_rcd());
	reply.set_recharge_total_id(msg.recharge_total_id());
	reply.set_reply_code(reply_code);
	p_role->send_msg_to_client(op_cmd::gc_recharge_total_award_reply, reply);
	return true;
}

bool business_activity_msg_handler_t::handle_cg_gold_luckydraw_data_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_gold_luckydraw_data_request);
	role_ptr p_role = role_manager_t::find_role(uid);
	if (p_role == NULL)
	{
		log_error("p_role is null [%lu]", uid);
		return false;
	}

	proto::client::gc_gold_luckydraw_data_reply reply;
	business_activity_manager_t::peek_gold_luckydraw_client_data(p_role, reply.mutable_gold_luckydraw());
	p_role->send_msg_to_client(op_cmd::gc_gold_luckydraw_data_reply, reply);
	return true;
}

bool business_activity_msg_handler_t::handle_cg_gold_luckydraw_award_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_gold_luckydraw_award_request);
	role_ptr p_role = role_manager_t::find_role(uid);
	if (p_role == NULL)
	{
		log_error("p_role is null [%lu]", uid);
		return false;
	}

	proto::client::gc_gold_luckydraw_award_reply reply;
	uint32_t reply_code = business_activity_manager_t::gold_luckydraw_award(p_role, msg.gold_luckydraw_id());
	reply.set_gold_luckydraw_id(msg.gold_luckydraw_id());
	reply.set_reply_code(reply_code);
	p_role->send_msg_to_client(op_cmd::gc_gold_luckydraw_award_reply, reply);
	return true;
}

bool business_activity_msg_handler_t::handle_cg_activity_shop_get_info_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_activity_shop_get_info_request);

	role_ptr p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("role[%lu] p_role is NULL", uid);
		return false;
	}

	proto::client::gc_activity_shop_get_info_reply reply;
	business_activity_manager_t::get_activity_shop_data(p_role, msg.shop_type(), reply);
	p_role->send_msg_to_client(op_cmd::gc_activity_shop_get_info_reply, reply);

	return true;
}

bool business_activity_msg_handler_t::handle_cg_activity_shop_buy_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_activity_shop_buy_request);

	role_ptr p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("role[%lu] p_role is NULL", uid);
		return false;
	}

	proto::client::gc_activity_shop_buy_reply reply;

	uint32_t reply_code = business_activity_manager_t::buy_activity_shop(p_role, msg.shop_type(), msg.buy_item_pos(), msg.buy_item_num(), reply);
	reply.set_shop_type(msg.shop_type());
	reply.set_error_code(reply_code);
	p_role->send_msg_to_client(op_cmd::gc_activity_shop_buy_reply, reply);

	return true;
}