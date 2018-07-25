#include "chat_msg_handler.hpp"
#include "network.hpp"
#include "role/role_manager.hpp"
#include "item/item_manager.hpp"
#include "scene/scene.hpp"
#include "task/task_manager.hpp"
#include "hero/hero_manager.hpp"
#include "gate/gate_manager.hpp"
#include "tblh/errcode_enum.hpp"
#include "user_troop/user_troop_mgr.hpp"
#include "chat_msg_manager.hpp"
#include "achieve/achieve_common.hpp"

USING_NS_NETWORK;
USING_NS_COMMON;

server_msg_handler_t<op_cmd::HG_BEGIN, op_cmd::HG_END, chat_msg_handler_t::msg_handler_func_1> chat_msg_handler_t::m_chat_msg_handle_1;
server_msg_handler_t<op_cmd::HG_BEGIN, op_cmd::HG_END, chat_msg_handler_t::msg_handler_func_2> chat_msg_handler_t::m_chat_msg_handle_2;

bool chat_msg_handler_t::init_msg_handler()
{
    bool ret = true;

	ret &= m_chat_msg_handle_1.register_func(op_cmd::hg_test_notify,					handle_hg_test_notify);
	ret &= m_chat_msg_handle_1.register_func(op_cmd::hg_get_nearby_roles_request,		handle_hg_get_nearby_roles_request);
	ret &= m_chat_msg_handle_1.register_func(op_cmd::hg_syn_online_role_list_request,	handle_hg_syn_online_role_list_request);
	ret &= m_chat_msg_handle_1.register_func(op_cmd::hg_user_channel_notify,			handle_hg_user_channel_notify);
	ret &= m_chat_msg_handle_1.register_func(op_cmd::hg_chat_get_role_info_request,		handle_hg_chat_get_role_info_request);
	ret &= m_chat_msg_handle_1.register_func(op_cmd::hg_notify_progress_state_notify,	handle_hg_notify_progress_state_notify);

    return ret;
}

bool chat_msg_handler_t::handle_hg_test_notify(role_ptr p_role, const network::msg_buf_ptr& msg_buf)
{
    PRE_S2S_MSG(proto::server::hg_test_notify);
    //log_warn("%lu game recv [hg_test_notify] from chat", uid);

    return true;
}


bool chat_msg_handler_t::handle_hg_get_nearby_roles_request(role_ptr p_role, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::hg_get_nearby_roles_request);

	proto::server::gh_get_nearby_roles_reply reply;
	uint32_t reply_code = 0;
	if (p_role == nullptr)
	{
		log_error("role [%lu] is not exist!", uid);
		return errcode_enum::notice_role_not_exist;
	}
	switch (msg.chat_channel())
	{
	case proto::common::chat_channel_vicinity:
		{
			std::vector<object_id_type> nearby_roles;
			p_role->get_nearby_objects(nearby_roles, true);
			for (object_id_type id_type : nearby_roles)
			{
				reply.add_role_uids(id_type.first);
				if (reply.role_uids_size() == 100)
				{
					break;
				}
			}
			reply_code = 0;
		}
		break;
	case proto::common::chat_channel_team:
		{
			troop_ptr p_troop = game_troop_mgr_t::get_troop(p_role->get_troop_id());
			if (p_troop)
			{
				uint64_t role_uid = 0;
				const troop_t::troop_member_vec& members = p_troop->get_members();
				for (auto p_member : members)
				{
					if (NULL == p_member)
					{
						continue;
					}
					role_uid = p_member->get_uid();
					reply.add_role_uids(role_uid);
					if (reply.role_uids_size() == 100)
					{
						break;
					}
				}
			}
		}
		break;
	default:
		break;
	}

	reply.set_chat_info(msg.chat_info());
	reply.set_chat_channel(msg.chat_channel());
	reply.set_reply_code(reply_code);
	reply.set_head_icon_id(msg.head_icon_id());
	reply.set_role_level(msg.role_level());
	reply.set_param(msg.param());
	proto::common::chat_common_role_info* p_data = reply.mutable_chat_role_info();
	p_data->CopyFrom(msg.chat_role_info());
	env::server->send_msg_to_chat(op_cmd::gh_get_nearby_roles_reply, uid, reply);

	return true;
}

bool chat_msg_handler_t::handle_hg_task_seek_help_request(role_ptr p_role, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::hg_task_seek_help_request);

	proto::server::gh_task_seek_help_reply reply;
	uint32_t reply_code = 0;
	if (p_role == nullptr)
	{
		reply_code = 1;
		log_error("role [%lu] is not exist!", uid);
	}
	else
	{
		task_mgr_ptr p_task_mgr = p_role->get_task_mgr();
		if (NULL == p_task_mgr)
		{
			reply_code = 1;
		}
		p_task_mgr->task_seek_help(msg.task_id());
	}
	reply.set_reply_code(reply_code);
	env::server->send_msg_to_chat(op_cmd::gh_get_nearby_roles_reply, uid, reply);

	return true;
}

bool chat_msg_handler_t::handle_hg_syn_online_role_list_request(role_ptr p_role, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::hg_syn_online_role_list_request);
	proto::server::gh_syn_online_role_list_notify reply;

	const role_map& role_list = role_manager_t::get_all_role();
	uint32_t send_count = 0;
	for (auto it : role_list)
	{
		role_ptr p_role = it.second;
		if (p_role != nullptr)
		{
			proto::common::chat_syn_user_info* p_sys_infos = reply.add_user_list();
			p_sys_infos->set_gate_id(p_role->get_gate_id());
			p_sys_infos->set_uid(p_role->get_object_id());
			p_sys_infos->set_country_id(p_role->get_country_id());
			p_sys_infos->set_family_id(p_role->get_family_id());
			p_sys_infos->set_nickname(p_role->get_name());
			p_sys_infos->set_channel_id(p_role->get_chat_channel());
			send_count++;
			if (100 == send_count)
			{
				env::server->send_msg_to_chat(op_cmd::gh_syn_online_role_list_notify, 0, reply);
				reply.clear_user_list();
				send_count = 0;
			}
		}
	}
	env::server->send_msg_to_chat(op_cmd::gh_syn_online_role_list_notify, 0, reply);
	return true;
}

bool chat_msg_handler_t::handle_hg_user_channel_notify(role_ptr p_role, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::hg_user_channel_notify);
	if (p_role != nullptr)
	{
		p_role->set_chat_channel(msg.channel_id());
	}
	return true;
}

bool chat_msg_handler_t::handle_hg_chat_get_role_info_request(role_ptr p_role, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::hg_chat_get_role_info_request);

	proto::server::gh_chat_get_role_info_reply reply;
	proto::common::chat_common_data* p_chat_notify = reply.mutable_chat_notify();
	p_chat_notify->CopyFrom(msg.chat_notify());
	uint32_t reply_code = 1;
	if (NULL == p_role)
	{
		log_error("role [%lu] is not exist!", uid);
	}
	else
	{
		std::vector<proto::common::chat_param> param_vec;
		for (int i = 0; i < msg.param_list_size(); ++i)
		{
			param_vec.push_back(msg.param_list(i));
		}
		proto::common::chat_common_role_info* p_data = reply.mutable_chat_role_info();
		if (!chat_msg_manager_t::chat_get_role_info(uid, p_data, param_vec))
		{
			log_error("role[%lu] chat Query rolo info fail!!!",uid);
			return true;
		}	
		reply_code = errcode_enum::error_ok;
	}
	///log_warn("item_size[%d] hero_size[%d]", reply.chat_role_info().item_single_size(),reply.chat_role_info().hero_single_size());
	reply.set_reply_code(reply_code);
	///log_warn("recode[%d]",reply_code);
	env::server->send_msg_to_chat(op_cmd::gh_chat_get_role_info_reply, uid, reply);
	achieve_common_t::notify_progress_state(uid, proto::common::ACHIEVE_CHANNEL_CHAT, msg.chat_notify().chat_channel());
	return true;
}

bool chat_msg_handler_t::handle_hg_notify_progress_state_notify(role_ptr p_role, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::hg_notify_progress_state_notify);
	achieve_common_t::notify_progress_state(uid, proto::common::ACHIEVE_CHANNEL_CHAT, msg.param1());
	return true;
}
