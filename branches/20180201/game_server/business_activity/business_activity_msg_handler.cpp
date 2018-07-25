#include "business_activity_msg_handler.hpp"
#include "business_activity_manager.hpp"


bool business_activity_msg_handler_t::init_msg_handler()
{
	gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_get_money_tree_request, handle_cg_get_money_tree_request);
	gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_shake_money_tree_request, handle_cg_shake_money_tree_request);
	gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_levelup_money_tree_request, handle_cg_levelup_money_tree_request);

	gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_get_7d_login_request, handle_cg_get_7d_login_request);
	gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_get_7d_login_prize_request, handle_cg_get_7d_login_prize_request);

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

