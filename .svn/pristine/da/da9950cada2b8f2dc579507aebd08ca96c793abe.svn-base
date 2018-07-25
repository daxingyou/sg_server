#include "fight_study_msg_handle.hpp"
#include "tblh/errcode_enum.hpp"

bool fight_study_msg_handle_t::init_msg_handler()
{
	gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_fight_study_request, handle_fight_study_request);
	gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_fight_study_cancel_notify, handle_fight_study_cancel_notify);
	gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_fight_study_answer_request, handle_fight_study_answer_request);

	return true;
}

bool fight_study_msg_handle_t::handle_fight_study_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_fight_study_request);
	role_ptr p_role = role_manager_t::find_role(uid);
	if (p_role == NULL)
	{
		log_error("p_role is null [%lu]", uid);
		return false;
	}

	return p_role->m_fight_study_mgr.fight_study_request(p_role, string_util_t::string_to_uint64(msg.target_uid()));
}

bool fight_study_msg_handle_t::handle_fight_study_cancel_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_fight_study_cancel_notify);
	role_ptr p_role = role_manager_t::find_role(uid);
	if (p_role == NULL)
	{
		log_error("p_role is null [%lu]", uid);
		return false;
	}

	return p_role->m_fight_study_mgr.fight_study_cancel(p_role);
}

bool fight_study_msg_handle_t::handle_fight_study_answer_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_fight_study_answer_request);
	role_ptr p_role = role_manager_t::find_role(uid);
	if (p_role == NULL)
	{
		log_error("p_role is null [%lu]", uid);
		return false;
	}

	uint32_t reply = 0;
	do 
	{
		proto::common::role_fight_data& rfd = p_role->get_role_fight_data();
		uint64_t attacker_uid = string_util_t::string_to_uint64(msg.attacker_uid());
		if (attacker_uid != rfd.study().attacker())
		{
			reply = errcode_enum::fight_study_already_cancel;
			break;
		}

		role_ptr attacker = role_manager_t::find_role(attacker_uid);
		if (NULL == attacker)
		{
			reply = errcode_enum::fight_study_already_cancel;
			break;
		}

		proto::common::role_fight_data& attacker_rfd = attacker->get_role_fight_data();
		if (attacker_rfd.study().target() != p_role->get_uid())
		{
			reply = errcode_enum::fight_study_already_cancel;
			break;
		}

		return attacker->m_fight_study_mgr.fight_study_answer_request(p_role, attacker, msg.answer());

	} while (0);

	p_role->m_fight_study_mgr.cancel_match(p_role);
	p_role->m_fight_study_mgr.fight_study_answer_reply(p_role, reply);
	return true;
}
