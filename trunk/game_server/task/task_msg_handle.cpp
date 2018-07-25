#include "task_msg_handle.hpp"
#include "role/role_manager.hpp"
#include "role/role.hpp"
#include "common/log_enum.hpp"
#include "gm_cmd/gm_cmd.hpp"
#include "task_manager.hpp"
#include "config/config_manager.hpp"
#include "gate/gate_msg_handler.hpp"
#include "tblh/errcode_enum.hpp"

bool task_msg_handle_t::init_msg_handler()
{
	gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_accept_task_request, handle_cg_accept_task_request);
	gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_commit_task_request, handle_cg_commit_task_request);
	gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_drop_task_request, handle_cg_drop_task_request);
	gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_help_task_request, handle_cg_help_task_request);
	gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_kill_task_monster_request, handle_cg_kill_task_monster_request);
	gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_task_refresh_shilian_request, handle_cg_task_refresh_shilian_request);

	gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_country_bounty_data_request, handle_cg_country_bounty_data_request);
	gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_country_bounty_accept_request, handle_cg_country_bounty_accept_request);
	//gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_country_bounty_drop_request, handle_cg_country_bounty_drop_request);
	gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_country_bounty_refresh_request, handle_cg_country_bounty_refresh_request);
	gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_country_bounty_box_request, handle_cg_country_bounty_box_request);

	gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_task_quick_data_request, handle_cg_task_quick_data_request);
	gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_task_quick_complete_request, handle_cg_task_quick_complete_request);
	gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_auto_complete_task_request, handle_cg_auto_complete_task_request);
	gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_task_event_award_request, handle_cg_task_event_award_request);

	return true;
}

bool task_msg_handle_t::handle_cg_accept_task_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_accept_task_request);
    auto p_role = role_manager_t::find_role(uid);
    if( NULL == p_role)
    {
		log_error("NULL == p_role[%lu]", uid);
        return false;
    }
    auto p_task_mgr = p_role->get_task_mgr();
    if( NULL == p_task_mgr)
    {
		log_error("NULL == p_task_mgr[%lu]", uid);
        return false;
    }
	proto::common::role_change_data rcd;
	if (msg.has_task_type())
	{
		p_task_mgr->active_circle_task(msg.task_type(), 0, ACCEPT_TASK_BY_MANUAL, rcd);
	}
	return true;
}

bool task_msg_handle_t::handle_cg_commit_task_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_commit_task_request);
    auto p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role[%lu]", uid);
		return false;
	}
	auto p_task_mgr = p_role->get_task_mgr();
	if (NULL == p_task_mgr)
	{
		log_error("NULL == p_task_mgr[%lu]", uid);
		return false;
	}
	
	return p_task_mgr->request_commit_task(msg);
}

bool task_msg_handle_t::handle_cg_drop_task_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
    PRE_S2S_MSG(proto::client::cg_drop_task_request);

	auto p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role[%lu]", uid);
		return false;
	}
	auto p_task_mgr = p_role->get_task_mgr();
	if (NULL == p_task_mgr)
	{
		log_error("NULL == p_task_mgr[%lu]", uid);
		return false;
	}
	p_task_mgr->drop_task(msg.task_id());

    return true;
}

bool task_msg_handle_t::handle_cg_help_task_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_help_task_request);

	uint64_t dst_role_uid = string_util_t::string_to_uint64(msg.help_role_uid());
	uint32_t help_task_tid = msg.help_task_tid();

	if (dst_role_uid == uid)
	{
		log_error("role [%lu] dst_role_uid == uid", uid);
		return false;
	}

	auto p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role[%lu]", uid);
		return false;
	}
	auto p_task_mgr = p_role->get_task_mgr();
	if (NULL == p_task_mgr)
	{
		log_error("NULL == p_task_mgr[%lu]", uid);
		return false;
	}

	proto::client::gc_help_task_reply reply;
	uint32_t reply_code = 1;
	auto p_dst_role = role_manager_t::find_role(dst_role_uid);
	if (NULL == p_dst_role)
	{
		log_error("NULL == p_dst_role [%lu]", dst_role_uid);
	}
	else
	{
		reply_code = p_task_mgr->help_finish_task(uid, help_task_tid);
	}
	reply.set_reply_code(reply_code);
	p_role->send_msg_to_client(op_cmd::gc_help_task_reply, reply);

	return true;
}

bool task_msg_handle_t::handle_cg_kill_task_monster_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_kill_task_monster_request);
	auto p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role[%lu]", uid);
		return false;
	}
	auto p_task_mgr = p_role->get_task_mgr();
	if (NULL == p_task_mgr)
	{
		log_error("NULL == p_task_mgr[%lu]", uid);
		return false;
	}

	proto::client::gc_kill_task_monster_reply reply;
	uint32_t reply_code = 0;
	if (p_task_mgr->try_kill_task_monster(msg.task_id()))
	{
		reply_code = 1;
	}
	reply.set_reply_code(reply_code);
	p_role->send_msg_to_client(op_cmd::gc_kill_task_monster_reply, reply);
	return true;
}

bool task_msg_handle_t::handle_cg_task_refresh_shilian_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_task_refresh_shilian_request);
	auto p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role[%lu]", uid);
		return false;
	}
	auto p_task_mgr = p_role->get_task_mgr();
	if (NULL == p_task_mgr)
	{
		log_error("NULL == p_task_mgr[%lu]", uid);
		return false;
	}

	proto::client::gc_task_refresh_shilian_reply reply;
	uint32_t reply_code = p_task_mgr->refresh_shilian(reply.mutable_rcd());
	reply.set_reply_code(reply_code);
	p_role->send_msg_to_client(op_cmd::gc_task_refresh_shilian_reply, reply);
	return true;
}

bool task_msg_handle_t::handle_cg_country_bounty_data_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_country_bounty_data_request);
	auto p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role[%lu]", uid);
		return false;
	}
	//if (0 == p_role->get_country_id())
	//{
	//	log_error("role[%lu] have no country", uid);
	//	return false;
	//}
	auto p_bounty_mgr = p_role->get_bounty_mgr();
	if (NULL == p_bounty_mgr)
	{
		log_error("NULL == p_bounty_mgr[%lu]", uid);
		return false;
	}
	proto::client::gc_country_bounty_data_reply reply;
	p_bounty_mgr->peek_data(reply.mutable_country_bounty());
	p_role->send_msg_to_client(op_cmd::gc_country_bounty_data_reply, reply);
	return true;
}

bool task_msg_handle_t::handle_cg_country_bounty_accept_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_country_bounty_accept_request);
	auto p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role[%lu]", uid);
		return false;
	}
	//if (0 == p_role->get_country_id())
	//{
	//	log_error("role[%lu] have no country", uid);
	//	return false;
	//}
	auto p_bounty_mgr = p_role->get_bounty_mgr();
	if (NULL == p_bounty_mgr)
	{
		log_error("NULL == p_bounty_mgr[%lu]", uid);
		return false;
	}
	proto::client::gc_country_bounty_accept_reply reply;
	uint32_t reply_code = 0;
	if (!p_bounty_mgr->accept_bounty(msg.bounty_id()))
	{
		reply_code = 1;
	}
	else
	{
		p_bounty_mgr->peek_data(reply.mutable_country_bounty());
	}
	reply.set_bounty_id(msg.bounty_id());
	reply.set_reply_code(reply_code);
	p_role->send_msg_to_client(op_cmd::gc_country_bounty_accept_reply, reply);
	return true;
}
//
//bool task_msg_handle_t::handle_cg_country_bounty_drop_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
//{
//	PRE_S2S_MSG(proto::client::cg_country_bounty_drop_request);
//	auto p_role = role_manager_t::find_role(uid);
//	if (NULL == p_role)
//	{
//		log_error("NULL == p_role[%lu]", uid);
//		return false;
//	}
//	if (0 == p_role->get_country_id())
//	{
//		log_error("role[%lu] have no country", uid);
//		return false;
//	}
//	auto p_bounty_mgr = p_role->get_bounty_mgr();
//	if (NULL == p_bounty_mgr)
//	{
//		log_error("NULL == p_bounty_mgr[%lu]", uid);
//		return false;
//	}
//	proto::client::gc_country_bounty_drop_reply reply;
//	uint32_t reply_code = 0;
//	if (!p_bounty_mgr->drop_bounty(msg.bounty_id()))
//	{
//		reply_code = 1;
//	}
//	reply.set_bounty_id(msg.bounty_id());
//	reply.set_reply_code(reply_code);
//	p_role->send_msg_to_client(op_cmd::gc_country_bounty_drop_reply, reply);
//	return true;
//}

bool task_msg_handle_t::handle_cg_country_bounty_refresh_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_country_bounty_refresh_request);
	auto p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role[%lu]", uid);
		return false;
	}
	//if (0 == p_role->get_country_id())
	//{
	//	log_error("role[%lu] have no country", uid);
	//	return false;
	//}
	auto p_bounty_mgr = p_role->get_bounty_mgr();
	if (NULL == p_bounty_mgr)
	{
		log_error("NULL == p_bounty_mgr[%lu]", uid);
		return false;
	}
	proto::client::gc_country_bounty_refresh_reply reply;
	uint32_t reply_code = p_bounty_mgr->refresh_bounty(reply.mutable_rcd());
	if (reply_code == errcode_enum::error_ok)
	{
		p_bounty_mgr->peek_data(reply.mutable_country_bounty());
	}
	reply.set_reply_code(reply_code);
	p_role->send_msg_to_client(op_cmd::gc_country_bounty_refresh_reply, reply);
	return true;
}

bool task_msg_handle_t::handle_cg_country_bounty_box_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_country_bounty_box_request);
	auto p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role[%lu]", uid);
		return false;
	}
	//if (0 == p_role->get_country_id())
	//{
	//	log_error("role[%lu] have no country", uid);
	//	return false;
	//}
	auto p_bounty_mgr = p_role->get_bounty_mgr();
	if (NULL == p_bounty_mgr)
	{
		log_error("NULL == p_bounty_mgr[%lu]", uid);
		return false;
	}
	proto::client::gc_country_bounty_box_reply reply;
	uint32_t reply_code = 0;
	if (!p_bounty_mgr->bounty_box(msg.box_id(), reply.mutable_rcd()))
	{
		reply_code = 1;
	}
	else
	{
		p_bounty_mgr->peek_data(reply.mutable_country_bounty());
	}
	reply.set_reply_code(reply_code);
	p_role->send_msg_to_client(op_cmd::gc_country_bounty_box_reply, reply);
	return true;
}


bool task_msg_handle_t::handle_cg_task_quick_data_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_task_quick_data_request);
	auto p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role[%lu]", uid);
		return false;
	}
	auto p_task_mgr = p_role->get_task_mgr();
	if (NULL == p_task_mgr)
	{
		log_error("NULL == p_task_mgr[%lu]", uid);
		return false;
	}
	proto::client::gc_task_quick_data_reply reply;
	p_task_mgr->get_quick_task_info( reply );
	p_role->send_msg_to_client(op_cmd::gc_task_quick_data_reply, reply);
	return true;
}


bool task_msg_handle_t::handle_cg_task_quick_complete_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_task_quick_complete_request);
	auto p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role[%lu]", uid);
		return false;
	}
	auto p_task_mgr = p_role->get_task_mgr();
	if (NULL == p_task_mgr)
	{
		log_error("NULL == p_task_mgr[%lu]", uid);
		return false;
	}
	proto::client::gc_task_quick_complete_reply  reply;
	uint32_t reply_code = p_task_mgr->quick_complete_task(msg.activity_id(), msg.count(), reply);
	reply.set_reply_code(reply_code);
	p_role->send_msg_to_client(op_cmd::gc_task_quick_complete_reply, reply);
	return true;
}


bool task_msg_handle_t::handle_cg_auto_complete_task_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_auto_complete_task_request);
	auto p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role[%lu]", uid);
		return false;
	}

	uint64_t item_uid = string_util_t::string_to_uint64( msg.item_uid() );

	proto::client::gc_auto_complete_task_reply reply;
	uint32_t reply_code = task_manager_t::auto_complete_task(p_role, msg.task_id(), item_uid);
	reply.set_reply_code(reply_code);
	reply.set_task_id(msg.task_id());
	p_role->send_msg_to_client(op_cmd::gc_auto_complete_task_reply, reply);
	return true;
}

bool task_msg_handle_t::handle_cg_task_event_award_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_task_event_award_request);
	auto p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role[%lu]", uid);
		return false;
	}
	auto p_task_mgr = p_role->get_task_mgr();
	if (NULL == p_task_mgr)
	{
		log_error("NULL == p_task_mgr[%lu]", uid);
		return false;
	}
	proto::client::gc_task_event_award_reply reply;
	uint32_t reply_code = p_task_mgr->task_event_award(msg.event_id(), reply.mutable_rcd(), reply.mutable_event_data());
	reply.set_reply_code(reply_code);
	p_role->send_msg_to_client(op_cmd::gc_task_event_award_reply, reply);
	return true;
}

