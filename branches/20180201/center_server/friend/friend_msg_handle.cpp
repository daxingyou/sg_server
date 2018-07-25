#include "friend_msg_handle.hpp"
#include "log.hpp"
#include "config/config_manager.hpp"
#include "gate/gate_msg_handler.hpp"
#include "client.pb.h"
#include "user/global_user.hpp"
#include "user/global_user_manager.hpp"
#include "tblh/errcode_enum.hpp"
#include "game/game_msg_handler.hpp"
#include "utility.hpp"
#include "friend_manager.hpp"
#include "redis/friend_data_mgr.hpp"
#include "redis/global_user_data_mgr.hpp"

bool friend_msg_handle_t::init_msg_handler()
{
    gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::ce_get_friends_request,handle_ce_get_friends_request_s);
    gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::ce_get_recent_friends_request,handle_ce_get_recent_friends_request_s);
    gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::ce_get_black_friends_request,handle_ce_get_black_friends_request_s);
    gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::ce_add_friend_request,handle_ce_add_friend_request_s);
    gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::ce_approve_add_friend_request,handle_ce_approve_add_friend_request_s);
    gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::ce_remove_friend_request,handle_ce_remove_friend_request_s);
    gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::ce_black_friend_request,handle_ce_black_friend_request_s);
    gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::ce_friend_search_user_by_id_request,handle_ce_friend_search_user_by_id_request_s);
    gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::ce_friend_search_user_by_name_request,handle_ce_friend_search_user_by_name_request_s);
    gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::ce_refresh_recommend_friends_list_request,handle_ce_refresh_recommend_friends_list_request_s);
    gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::ce_top_friend_request,handle_ce_top_friend_request_s);
    gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::ce_cancel_top_friend_request,handle_ce_cancel_top_friend_request_s);
    gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::ce_clear_friends_request,handle_ce_clear_friends_request_s);
    gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::ce_cancel_black_friend_request,handle_ce_cancel_black_friend_request_s);
    gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::ce_clear_recent_friends_request,handle_ce_clear_recent_friends_request_s);
    gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::ce_clear_black_friends_request,handle_ce_clear_black_friends_request_s);
    gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::ce_clear_friend_appliers_request,handle_ce_clear_friend_appliers_request_s);
    gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::ce_get_friend_applier_list_request, handle_ce_get_friend_applier_list_request_s);

	return true;
}

bool friend_msg_handle_t::handle_ce_get_friends_request_s(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::ce_get_friends_request);

	global_user_ptr p_user = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL == p_user)
	{
		log_error("user[%lu] null error", uid);
		return false;
	}

	proto::client::ec_get_friends_reply reply;
	p_user->get_friend_mgr().get_friend_list(reply);
	p_user->send_msg_to_client(op_cmd::ec_get_friends_reply, reply);

	return true;
}

bool friend_msg_handle_t::handle_ce_add_friend_request_s(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::ce_add_friend_request);

	global_user_ptr p_user = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL == p_user)
	{
		log_error("user[%lu] null error", uid);
		return false;
	}

	uint64_t friend_uid = string_util_t::string_to_uint64(msg.friend_uid());
	if (friend_uid == 0)
	{
		log_error("user[%lu] friend_uid == 0", uid);
		return false;
	}

	proto::client::ec_add_friend_reply reply;
	p_user->get_friend_mgr().add_friend_by_uid(friend_uid, reply);
	// 错误给个名字，用于提示
	if (reply.reply_code() != errcode_enum::error_ok)
	{
		// 可能不在线，所以直接去redis读
		reply.set_error_name(global_user_data_mgr_t::get_name(friend_uid));
	}

	p_user->send_msg_to_client(op_cmd::ec_add_friend_reply, reply);

	return true;
}

bool friend_msg_handle_t::handle_ce_remove_friend_request_s(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::ce_remove_friend_request);

	global_user_ptr p_user = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL == p_user)
	{
		log_error("user[%lu] null error", uid);
		return false;
	}

	uint64_t friend_uid = string_util_t::string_to_uint64(msg.friend_uid());
	if (friend_uid == 0)
	{
		log_error("user[%lu] friend_uid == 0", uid);
		return false;
	}

	proto::client::ec_remove_friend_reply reply;
	p_user->get_friend_mgr().delete_friend_by_uid(friend_uid, reply);
	p_user->send_msg_to_client(op_cmd::ec_remove_friend_reply, reply);

	return true;
}

bool friend_msg_handle_t::handle_ce_black_friend_request_s(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::ce_black_friend_request);

	global_user_ptr p_user = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL == p_user)
	{
		log_error("user[%lu] null error", uid);
		return false;
	}

	uint64_t friend_uid = string_util_t::string_to_uint64(msg.friend_uid());
	if (friend_uid == 0)
	{
		log_error("user[%lu] friend_uid == 0", uid);
		return false;
	}

	proto::client::ec_black_friend_reply reply;
	p_user->get_friend_mgr().black_friend_by_uid(friend_uid, reply);
	p_user->send_msg_to_client(op_cmd::ec_black_friend_reply, reply);

	return true;
}

bool friend_msg_handle_t::handle_ce_friend_search_user_by_id_request_s(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::ce_friend_search_user_by_id_request);

	global_user_ptr p_user = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL == p_user)
	{
		log_error("user[%lu] null error", uid);
		return false;
	}

	uint64_t search_uid = string_util_t::string_to_uint64(msg.uid());
	if (search_uid == 0)
	{
		log_error("user[%lu] search_uid == 0", uid);
		return false;
	}

	proto::client::ec_friend_search_user_by_id_reply reply;
	if (!friend_data_mgr_t::search_user_info_by_id(search_uid, reply.mutable_info()))
	{
		reply.set_reply_code(errcode_enum::friend_error_id);
	}
	else
	{
		reply.set_reply_code(errcode_enum::error_ok);
	}

	p_user->send_msg_to_client(op_cmd::ec_friend_search_user_by_id_reply, reply);

	return true;
}

bool friend_msg_handle_t::handle_ce_friend_search_user_by_name_request_s(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::ce_friend_search_user_by_name_request);

	global_user_ptr p_user = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL == p_user)
	{
		log_error("user[%lu] null error", uid);
		return false;
	}

	proto::client::ec_friend_search_user_by_name_reply reply;
	if (!friend_data_mgr_t::search_user_info_by_name(msg.name(), reply.mutable_info()))
	{
		reply.set_reply_code(errcode_enum::friend_error_id);
	}
	else
	{
		reply.set_reply_code(errcode_enum::error_ok);
	}

	p_user->send_msg_to_client(op_cmd::ec_friend_search_user_by_name_reply, reply);

	return true;
}

bool friend_msg_handle_t::handle_ce_refresh_recommend_friends_list_request_s(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::ce_refresh_recommend_friends_list_request);

	global_user_ptr p_user = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL == p_user)
	{
		log_error("user[%lu] null error", uid);
		return false;
	}

	proto::client::ec_refresh_recommend_friends_list_reply reply;

	do 
	{
		// 私聊未关注 组队好友
		p_user->get_friend_mgr().get_recommend_friends(reply);

		// 数量够了吗
		if ((uint32_t)reply.recommend_friends_size() >= friend_manager_t::get_friend_cnt6())
			break;

		// 排行榜

		// 在线等级相近
		const global_user_manager_t::global_user_map_type& all_global_user_map = global_user_manager_t::get_all_global_users();
		uint32_t level_max = p_user->get_level() + friend_recommand_level_near;
		uint32_t level_min = p_user->get_level() >= friend_recommand_level_near ? p_user->get_level() - friend_recommand_level_near : 0;

		for (global_user_manager_t::global_user_map_type::const_iterator itr = all_global_user_map.begin();
			itr != all_global_user_map.end(); ++itr)
		{
			global_user_ptr p_temp_user = itr->second;
			if (NULL != p_temp_user && level_min <= p_temp_user->get_level() && p_temp_user->get_level() <= level_max)
			{
				// 自己除外
				if (itr->first == uid)
				{
					continue;
				}

				// 判断对方是否在我社会关系中，在我社会关系中直接返回，已经在之前做过判断了
				if (p_user->get_friend_mgr().get_friend_type_by_uid(itr->first) != proto::common::friend_type_none)
				{
					continue;
				}

				proto::common::recommend_member* p_recommend_member = reply.add_recommend_friends();
				if (p_recommend_member != NULL)
				{
					p_recommend_member->set_src(proto::common::recommend_source_online_same_level);

					proto::common::user_info* p_user_info = p_recommend_member->mutable_info();
					if (p_user_info != NULL)
					{
						friend_data_mgr_t::search_user_info_by_id(itr->first, p_user_info);
					}
				}

				// 数量够了吗
				if ((uint32_t)reply.recommend_friends_size() >= friend_manager_t::get_friend_cnt6())
					break;
			}
		}

	} while (false);

	reply.set_reply_code(errcode_enum::error_ok);
	p_user->send_msg_to_client(op_cmd::ec_refresh_recommend_friends_list_reply, reply);

	return true;
}

bool friend_msg_handle_t::handle_ce_top_friend_request_s(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::ce_top_friend_request);

	global_user_ptr p_user = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL == p_user)
	{
		log_error("user[%lu] null error", uid);
		return false;
	}

	uint64_t friend_uid = string_util_t::string_to_uint64(msg.friend_uid());
	if (friend_uid == 0)
	{
		log_error("user[%lu] friend_uid == 0", uid);
		return false;
	}

	proto::client::ec_top_friend_reply reply;
	p_user->get_friend_mgr().top_friend_by_uid(friend_uid, reply);
	p_user->send_msg_to_client(op_cmd::ec_top_friend_reply, reply);
	return true;
}


bool friend_msg_handle_t::handle_ce_cancel_top_friend_request_s(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::ce_cancel_top_friend_request);

	global_user_ptr p_user = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL == p_user)
	{
		log_error("user[%lu] null error", uid);
		return false;
	}

	uint64_t friend_uid = string_util_t::string_to_uint64(msg.friend_uid());
	if (friend_uid == 0)
	{
		log_error("user[%lu] friend_uid == 0", uid);
		return false;
	}

	proto::client::ec_cancel_top_friend_reply reply;
	p_user->get_friend_mgr().cancel_top_friend_by_uid(friend_uid, reply);
	p_user->send_msg_to_client(op_cmd::ec_cancel_top_friend_reply, reply);
	return true;
}


bool friend_msg_handle_t::handle_ce_clear_friends_request_s(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::ce_clear_friends_request);

	global_user_ptr p_user = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL == p_user)
	{
		log_error("user[%lu] null error", uid);
		return false;
	}

	proto::client::ec_clear_friends_reply reply;
	p_user->get_friend_mgr().clear_friends_not_login_long_time(reply);
	reply.set_reply_code(errcode_enum::error_ok);
	p_user->send_msg_to_client(op_cmd::ec_clear_friends_reply, reply);

	return true;
}


bool friend_msg_handle_t::handle_ce_clear_recent_friends_request_s(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::ce_clear_recent_friends_request);

	global_user_ptr p_user = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL == p_user)
	{
		log_error("user[%lu] null error", uid);
		return false;
	}

	proto::client::ec_clear_recent_friends_reply reply;
	p_user->get_friend_mgr().clear_recent_contact_friends();
	reply.set_reply_code(errcode_enum::error_ok);
	p_user->send_msg_to_client(op_cmd::ec_clear_recent_friends_reply, reply);

	return true;
}


bool friend_msg_handle_t::handle_ce_clear_black_friends_request_s(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::ce_clear_black_friends_request);

	global_user_ptr p_user = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL == p_user)
	{
		log_error("user[%lu] null error", uid);
		return false;
	}

	proto::client::ec_clear_black_friends_reply reply;
	p_user->get_friend_mgr().clear_all_black_friends();
	reply.set_reply_code(errcode_enum::error_ok);
	p_user->send_msg_to_client(op_cmd::ec_clear_black_friends_reply, reply);

	return true;
}


bool friend_msg_handle_t::handle_ce_cancel_black_friend_request_s(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::ce_cancel_black_friend_request);

	global_user_ptr p_user = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL == p_user)
	{
		log_error("user[%lu] null error", uid);
		return false;
	}

	uint64_t friend_uid = string_util_t::string_to_uint64(msg.friend_uid());
	if (friend_uid == 0)
	{
		log_error("user[%lu] friend_uid == 0", uid);
		return false;
	}

	proto::client::ec_cancel_black_friend_reply reply;
	p_user->get_friend_mgr().cancel_black_friend_by_uid(friend_uid, reply);
	p_user->send_msg_to_client(op_cmd::ec_cancel_black_friend_reply, reply);

	return true;
}


bool friend_msg_handle_t::handle_ce_get_recent_friends_request_s(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::ce_get_recent_friends_request);

	global_user_ptr p_user = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL == p_user)
	{
		log_error("user[%lu] null error", uid);
		return false;
	}

	proto::client::ec_get_recent_friends_reply reply;
	p_user->get_friend_mgr().get_recent_friends_list(reply);
	p_user->send_msg_to_client(op_cmd::ec_get_recent_friends_reply, reply);

	return true;
}


bool friend_msg_handle_t::handle_ce_get_black_friends_request_s(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::ce_get_black_friends_request);

	global_user_ptr p_user = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL == p_user)
	{
		log_error("user[%lu] null error", uid);
		return false;
	}

	proto::client::ec_get_black_friends_reply reply;
	p_user->get_friend_mgr().get_black_friends_list(reply);
	p_user->send_msg_to_client(op_cmd::ec_get_black_friends_reply, reply);

	return true;
}


bool friend_msg_handle_t::handle_ce_approve_add_friend_request_s(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	log_error("ce_approve_add_friend_request is invalid");

	return true;
}

bool friend_msg_handle_t::handle_ce_get_friend_applier_list_request_s(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::ce_get_friend_applier_list_request);

	global_user_ptr p_user = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL == p_user)
	{
		log_error("user[%lu] null error", uid);
		return false;
	}

	proto::client::ec_get_friend_applier_list_reply reply;
	p_user->get_friend_mgr().get_apply_friends_list(reply);
	p_user->send_msg_to_client(op_cmd::ec_get_friend_applier_list_reply, reply);

	return true;
}


bool friend_msg_handle_t::handle_ce_clear_friend_appliers_request_s(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::ce_clear_friend_appliers_request);

	global_user_ptr p_user = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL == p_user)
	{
		log_error("user[%lu] null error", uid);
		return false;
	}

	proto::client::ec_clear_friend_appliers_reply reply;
	p_user->get_friend_mgr().clear_all_apply_friends();
	reply.set_reply_code(errcode_enum::error_ok);
	p_user->send_msg_to_client(op_cmd::ec_clear_friend_appliers_reply, reply);

	return true;
}
