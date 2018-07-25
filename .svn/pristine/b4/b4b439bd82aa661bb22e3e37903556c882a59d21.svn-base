#include "page_msg_handle.hpp"
#include "page/page_manager.hpp"
#include "gate/gate_msg_handler.hpp"
#include "fight/fight_manager.hpp"
#include "tblh/SectionElite.tbls.h"
#include "tblh/errcode_enum.hpp"
#include "common/config_mgr.h"
#include "tblh/SectionReward.tbls.h"
#include "user_troop/user_troop_mgr.hpp"
USING_NS_NETWORK;
USING_NS_COMMON;


bool page_msg_handler_t::init_msg_handler()
{
	bool bresult = true;
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_section_request, handle_cg_section_request);
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_section_reward_request, handle_cg_section_reward_request);
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_section_pass_pve_request, handle_cg_section_pass_pve_request);
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_quick_fight_elite_page_request, handle_cg_quick_fight_elite_page_request);
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_reset_quick_fight_elite_page_request, handle_cg_reset_quick_fight_elite_page_request);

	return bresult;
}

bool page_msg_handler_t::handle_cg_section_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_section_request);
	auto p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("trade_msg p_role NULL");
		return false;
	}
	proto::client::gc_section_reply reply;
	uint32_t reply_code = page_manager_t::get_page_list(uid, reply);
	reply.set_reply_code(reply_code);
	p_role->send_msg_to_client(op_cmd::gc_section_reply, reply);

	return true;
}

bool page_msg_handler_t::handle_cg_section_reward_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_section_reward_request);
	role_ptr p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role rile[%lu]",uid);
		return false;
	}
	proto::client::gc_section_reward_reply reply;
	uint32_t reply_code = page_manager_t::get_page_pass_reward(msg.get_type(),uid,msg.id(), reply);
	reply.set_reply_code(reply_code);
	p_role->send_msg_to_client(op_cmd::gc_section_reward_reply, reply);
	return true;
}

bool page_msg_handler_t::handle_cg_section_pass_pve_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_section_pass_pve_request);
	role_ptr p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role rile[%lu]", uid);
		return false;
	}
	std::set<uint32_t>& open_list = p_role->get_page_elite_open_list();
	uint32_t reply_code = 0;
	do 
	{
		if (0 != p_role->get_troop_id()){
			troop_ptr p_troop = game_troop_mgr_t::get_troop(p_role->get_troop_id());
			if (NULL == p_troop)
			{
				log_error("role[%lu] troop_id[%lu] null error", p_role->get_uid(), p_role->get_troop_id());
				reply_code = common::errcode_enum::notice_unknown;
				break;
			}

			if (proto::common::troop_state_leave != p_role->get_troop_state())
			{
				log_error("role[%lu] troop_id[%lu] but not leave state. cur state[%d],not enter page pass pve fight", uid, p_role->get_troop_id(), p_role->get_troop_state());
				reply_code = errcode_enum::team_cannot_enter_elitefight;
				break;
			}
		}
	
		if (0 == msg.order_id()){
			log_error("role[%lu] page elite pve param order_id[%d] error!!!",uid,msg.order_id());
			reply_code = errcode_enum::notice_unknown;
			break;
		}

		SectionElite* p_config = GET_CONF(SectionElite, msg.order_id());
		if (NULL == p_config){
			log_error("SectionElite id[%d] null", msg.order_id());
			reply_code = errcode_enum::notice_config_error;
			break;
		}

		if (p_config->page_id() != msg.page_id()){
			log_error("role[%lu] page id[%d] order_id[%d] no match!!!",uid,msg.page_id(),msg.order_id());
			reply_code = errcode_enum::notice_unknown;
			break;
		}

		if (p_config->fight_id() != msg.fight_id()){
			log_error("role[%lu] SectionElite table order_id[%d] fight[%d] with msg fight[%d]  no match!!!", uid,msg.order_id(), p_config->fight_id(), msg.fight_id());
			reply_code = errcode_enum::notice_unknown;
			break;
		}

		if (open_list.empty() || 0 == open_list.count(msg.order_id()))
		{
			log_error("SectionElite id[%d] null", msg.order_id());
			reply_code = errcode_enum::notice_checkpoint_elite_complete_Pre;
			break;
		}
	} while (false);
	
	if (reply_code == errcode_enum::error_ok){
		proto::common::fight_param fp;
		fp.set_type(proto::common::fight_type_page_pass_pk);
		fp.set_page_id(msg.order_id());
		fight_manager_t::fight_pve(p_role, msg.fight_id(), &fp);
	}
	
	proto::client::gc_section_pass_pve_reply reply;
	reply.set_reply_code(reply_code);
	p_role->send_msg_to_client(op_cmd::gc_section_pass_pve_reply, reply);
	return true;
}

bool page_msg_handler_t::handle_cg_quick_fight_elite_page_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_quick_fight_elite_page_request);
	role_ptr p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role rile[%lu]", uid);
		return false;
	}
	proto::client::gc_quick_fight_elite_page_reply reply;
	uint32_t reply_code = page_manager_t::quick_fight_elite_page( uid,  msg.id(), msg.type(), reply);
	reply.set_reply_code(reply_code);
	proto::common::role_change_data* p_rcd = reply.mutable_rcd();
	if (p_rcd) {
		p_role->get_personal_info(p_rcd->mutable_per_info());
	}

	p_role->send_msg_to_client(op_cmd::gc_quick_fight_elite_page_reply, reply);
	return true;
}


bool page_msg_handler_t::handle_cg_reset_quick_fight_elite_page_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_reset_quick_fight_elite_page_request);
	role_ptr p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role rile[%lu]", uid);
		return false;
	}
	proto::client::gc_reset_quick_fight_elite_page_reply reply;
	uint32_t reply_code = page_manager_t::reset_quick_fight_elite_page( uid, msg.id(), reply);
	reply.set_reply_code(reply_code);
	reply.set_id(msg.id());
	proto::common::role_change_data* p_rcd = reply.mutable_rcd();
	if (p_rcd) {
		p_role->get_personal_info(p_rcd->mutable_per_info());
	}

	p_role->send_msg_to_client(op_cmd::gc_reset_quick_fight_elite_page_reply, reply);
	return true;
}
