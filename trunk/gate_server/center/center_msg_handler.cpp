#include "center_msg_handler.hpp"
#include "network.hpp"
#include "common/config_mgr.h"
#include "user/user_msg_checker.hpp"
#include "config/config_manager.hpp"
#include "user/user_manager.hpp"

USING_NS_NETWORK;
USING_NS_COMMON;

server_msg_handler_t<op_cmd::EA_BEGIN, op_cmd::EA_END, center_msg_handler_t::msg_handler_func> center_msg_handler_t::m_center_msg_handle;
server_msg_handler_t<op_cmd::MA_BEGIN, op_cmd::MA_END, center_msg_handler_t::msg_handler_func> center_msg_handler_t::m_gm_msg_handle;

bool center_msg_handler_t::init_msg_handler()
{
    bool ret = true;
	// center => gate
    m_center_msg_handle.register_func(op_cmd::ea_broadcast_notify, handle_ea_broadcast_notify);
	m_center_msg_handle.register_func(op_cmd::ea_unreg_user_center_notify, handle_ea_unreg_user_center_notify);
	m_center_msg_handle.register_func(op_cmd::ea_create_role_reply, handle_ea_create_role_reply);
	m_center_msg_handle.register_func(op_cmd::ea_send_gm_msg_to_account_request, handle_ea_send_gm_msg_to_account_request);
	m_center_msg_handle.register_func(op_cmd::ea_kick_all_user_notify, handle_ea_kick_all_user_notify);

	// gm => gate
    m_gm_msg_handle.register_func(op_cmd::ma_masked_msg_notify,     handle_ma_masked_msg_notify);
    m_gm_msg_handle.register_func(op_cmd::ma_reload_config_notify,  handle_ma_reload_config_notify);
    return ret;
}

// center => gate---------------------------------------------------------------------------------------------------


#define BROADCAST_ALL_MSG(msg_type) \
	msg_type msg_to_client; \
    network::unwrap_msg(msg_to_client, msg_data); \
	user_manager_t::send_to_all_users(msg_cmd, msg_to_client);

#define BROADCAST_UID_MSG(msg_type) \
	msg_type msg_to_client; \
    network::unwrap_msg(msg_to_client, msg_data); \
	for (int32_t i = 0; i < msg.role_list_size(); ++i) \
	{ \
		const user_ptr& p_user = user_manager_t::get_user(msg.role_list(i)); \
		if (NULL == p_user || !p_user->is_play_state()) \
		{ \
			continue; \
		} \
		p_user->send_msg_to_client(msg_cmd, msg_to_client); \
	}

bool center_msg_handler_t::handle_ea_broadcast_notify(uint16_t cmd, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::ea_broadcast_notify);

	uint16_t msg_cmd = (uint16_t)msg.cmd();
	const std::string& msg_data = msg.data();

	switch (msg_cmd)
	{
	case op_cmd::gc_sys_notice_notify:
		{
			if (msg.role_list_size() > 0)
			{
				BROADCAST_UID_MSG(proto::client::gc_sys_notice_notify);
			} 
			else
			{
				BROADCAST_ALL_MSG(proto::client::gc_sys_notice_notify);
			}
		}
		break;
	case op_cmd::ec_update_country_notify:
		{
			BROADCAST_ALL_MSG(proto::client::ec_update_country_notify);
		}
		break;
	case op_cmd::ec_king_war_notice_notify:
		{
			BROADCAST_UID_MSG(proto::client::ec_king_war_notice_notify);
		}
		break;
	case op_cmd::ec_family_members_in_notify:
		{
			BROADCAST_UID_MSG(proto::client::ec_family_members_in_notify);
		}
		break;
	case op_cmd::ec_family_members_out_notify:
		{
			BROADCAST_UID_MSG(proto::client::ec_family_members_out_notify);
		}
		break;
	case op_cmd::ec_family_money_change_notify:
		{
			BROADCAST_UID_MSG(proto::client::ec_family_money_change_notify);
		}
		break;
	case op_cmd::ec_family_notification_change_notify:
		{
			BROADCAST_UID_MSG(proto::client::ec_family_notification_change_notify);
		}
		break;
	case op_cmd::ec_family_declaration_change_notify:
		{
			BROADCAST_UID_MSG(proto::client::ec_family_declaration_change_notify);
		}
		break;
	case op_cmd::ec_redbag_notify:
		{
			BROADCAST_UID_MSG(proto::client::ec_redbag_notify);
		}
		break;
	case op_cmd::ec_country_alliance_notify:
		{
			BROADCAST_UID_MSG(proto::client::ec_country_alliance_notify);
		}
		break;
	case op_cmd::ec_family_officer_changed_notify:
		{
			BROADCAST_UID_MSG(proto::client::ec_family_officer_changed_notify);
		}
		break;
	case op_cmd::ec_family_war_state_notify:
		{
			BROADCAST_UID_MSG(proto::client::ec_family_war_state_notify);
		}
		break;
	default:
		{
			log_error("cmd[%s:%d] error!", CMD_DESC(msg_cmd), msg_cmd);
			return false;
		}
		break;
	}

	return true;
}


bool center_msg_handler_t::handle_ea_unreg_user_center_notify(uint16_t cmd, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::ea_unreg_user_center_notify);

	const user_ptr& p_user = user_manager_t::get_user(uid);
	if (NULL != p_user)
	{
		switch (msg.unreg_code())
		{
		case common::UNREGUSER_RET_LOGOUT:		// 让gate与client断开，并且设置为离线状态
			{
				if (p_user->is_login_game())	// 登陆状态
				{
					p_user->set_kick_reason(KICK_ROLE_REASON_CENTER_UNREG);

					// 通知客户端下线
					proto::client::ac_kick_role_notify ac_kick;
					ac_kick.set_kick_reason(common::errcode_enum::notice_account_error_relogin);
					p_user->send_msg_to_client(op_cmd::ac_kick_role_notify, ac_kick);

					// 删除原来的连接但是不删除user
					user_manager_t::del_socket(p_user->get_client_socket(), false);

					// 设置离线状态，启用重登流程
					p_user->set_game_offline();
				}
				else if (p_user->is_offline_state()) // 已经下线或者注销
				{
					log_warn("user[%lu] is already offline or unreg", uid);
				}
				else
				{
					log_error("user[%lu] is unreg[%u] error, state[%u]", uid, msg.unreg_code(), p_user->get_game_state());

					if (NULL != p_user->get_client_socket())
					{
						user_manager_t::del_socket(p_user->get_client_socket(), true);
					}
					else
					{
						user_manager_t::del_user(uid);
					}
				}
			}
			break;
		case common::UNREGUSER_RET_ERROR:		// 发生错误，直接与客户端断开，并且下线user
			{
				if (p_user->is_offline_state()) // 已经下线或者注销
				{
					log_warn("user[%lu] is already offline or unreg", uid);
				}
				else
				{
					log_error("user[%lu] is unreg[%u] error, state[%u]", uid, msg.unreg_code(), p_user->get_game_state());

					if (NULL != p_user->get_client_socket())
					{
						user_manager_t::del_socket(p_user->get_client_socket(), true);
					}
					else
					{
						user_manager_t::del_user(uid);
					}
				}
			}
			break;
		case common::UNREGUSER_RET_NO_ROLE:		// 没有创建角色错误
			{
				if (p_user->is_login_state())
				{
					proto::client::ac_login_game_reply reply;
					reply.set_reply_code(common::errcode_enum::notice_account_no_role);
					p_user->send_msg_to_client(op_cmd::ac_login_game_reply, reply);
				}
				else if (p_user->is_offline_state()) // 已经下线或者注销
				{
					log_warn("user[%lu] is already offline or unreg", uid);
				}
				else
				{
					log_error("user[%lu] is unreg[%u] error, state[%u]", uid, msg.unreg_code(), p_user->get_game_state());

					if (NULL != p_user->get_client_socket())
					{
						user_manager_t::del_socket(p_user->get_client_socket(), true);
					}
					else
					{
						user_manager_t::del_user(uid);
					}
				}
			}
			break;
		case common::UNREGUSER_RET_BAN:		// 被封号踢人
			{
				log_warn("user[%lu] was kicked by ban, state[%u]!", uid, p_user->get_game_state());
				
				p_user->set_kick_reason(KICK_ROLE_REASON_BAN);

				// 通知客户端下线
				proto::client::ac_kick_role_notify ac_kick;
				ac_kick.set_kick_reason(common::errcode_enum::notice_account_ban);
				ac_kick.set_kick_param(msg.code_param());
				p_user->send_msg_to_client(op_cmd::ac_kick_role_notify, ac_kick);

				// 对于在线玩家，通知下线
				if (p_user->is_scene_state())
				{
					user_manager_t::notify_others_unreg_user(p_user, common::UNREGUSER_RET_LOGOUT);
				}

				user_manager_t::del_socket(p_user->get_client_socket(), true);
			}
			break;
		default:
			log_error("user[%lu] error unreg_code[%u]", uid, msg.unreg_code());
			break;
		}
	}
	else
	{
		log_warn("user[%lu] unreg not find in gate[%u]", uid, env::server->get_id());
	}

	return true;
}


bool center_msg_handler_t::handle_ea_create_role_reply(uint16_t cmd, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::ea_create_role_reply);

	const user_ptr& p_user = user_manager_t::get_user(uid);
	if (NULL != p_user)
	{
		p_user->clear_create_state();

		if (p_user->is_login_state())
		{
			proto::client::ac_create_role_reply reply;
			reply.set_reply_code(msg.reply_code());
			p_user->send_msg_to_client(op_cmd::ac_create_role_reply, reply);
		}
		else
		{
			log_warn("user[%lu] state[%u] error, when recv ea_create_role_reply", uid, p_user->get_game_state());
		}
	}
	else
	{
		log_warn("user[%lu] unreg not find in gate[%u]", uid, env::server->get_id());
	}

	return true;
}


bool center_msg_handler_t::handle_ea_kick_all_user_notify(uint16_t cmd, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::ea_kick_all_user_notify);

	user_manager_t::clear_all_users_and_socket(true, msg.reason());

	log_error("gate_server[%u] del all user, reason:%u", env::server->get_id(), msg.reason());
	return true;
}

// gm => gate---------------------------------------------------------------------------------------------------
bool center_msg_handler_t::handle_ma_masked_msg_notify(uint16_t cmd, const network::msg_buf_ptr& msg_buf)
{
    PRE_S2S_MSG(proto::gm::ma_masked_msg_notify);

    for (auto cmd : msg.masked_list())
    {
        if (msg.masked_flag() == 0)
        {
            user_msg_checker_t::remove_masked_msg(cmd);
        }
        else
        {
            user_msg_checker_t::add_masked_msg(cmd);
        }
    }

    return true;
}

bool center_msg_handler_t::handle_ma_reload_config_notify(uint16_t cmd, const network::msg_buf_ptr& msg_buf)
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

bool center_msg_handler_t::handle_ea_send_gm_msg_to_account_request(uint16_t cmd, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::ea_send_gm_msg_to_account_request);

	proto::server::al_send_gm_msg_to_account_requst request;
	request.set_cmd_type(msg.cmd_type());
	request.mutable_msg()->CopyFrom(msg.msg());
	env::server->send_msg_to_login(op_cmd::al_send_gm_msg_to_account_requst, 0, request );
	return true;
}

