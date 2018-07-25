#include "login_msg_handler.hpp"
#include "main/gate_server.hpp"
#include "main/gate_server_fwd.hpp"
#include "network.hpp"
#include "user/user_manager.hpp"
#include "common/common_struct.hpp"
#include "errcode_enum.hpp"
#include "game/game_manager.hpp"
#include "common/random_uuid.hpp"

USING_NS_NETWORK;
USING_NS_COMMON;

server_msg_handler_t<op_cmd::LA_BEGIN, op_cmd::LA_END, login_msg_handler_t::msg_handler_func> login_msg_handler_t::m_login_msg_handle;

bool login_msg_handler_t::init_msg_handler()
{
    m_login_msg_handle.register_func(op_cmd::la_login_request,                  handle_la_login_request);

    return true;
}

bool login_msg_handler_t::handle_la_login_request(const msg_buf_ptr& msg_buf)
{
    PRE_S2S_MSG(proto::server::la_login_request);
 
	bool rlt = false;
 	user_ptr p_gate_user = user_manager_t::get_user(uid);
 	if (NULL != p_gate_user)
 	{
		switch (p_gate_user->get_game_state())
		{
		case common::ROLE_GAME_STATE_INIT:		// 对于一个还未绑定任何socket的客户端重置下超时定时器
			{
				ROLE_LOG("user[%lu] state[%u] restart login gate", uid, p_gate_user->get_game_state());

				p_gate_user->start_pingpong_timer();
			}
			break;
		case common::ROLE_GAME_STATE_GATE:		// 对于在登陆到gate，被顶下线
			{
				log_warn("user[%lu] was kicked by someone, state[%u]!", uid, p_gate_user->get_game_state());

				p_gate_user->set_kick_reason(KICK_ROLE_REASON_BY_OTHER);

				// 通知客户端下线
				proto::client::ac_kick_role_notify ac_kick;
				ac_kick.set_kick_reason(errcode_enum::accounts_error_enter_login);
				p_gate_user->send_msg_to_client(op_cmd::ac_kick_role_notify, ac_kick);

				// 删除原来的连接但是不删除user，还要复用
				user_manager_t::del_socket(p_gate_user->get_client_socket(), false);

				// 重置下超时定时器
				p_gate_user->set_init_state();
			}
			break;
		case common::ROLE_GAME_STATE_REGATE:	// 等于重登，被顶下线
			{
				log_warn("user[%lu] was kicked by someone, state[%u]!", uid, p_gate_user->get_game_state());

				p_gate_user->set_kick_reason(KICK_ROLE_REASON_BY_OTHER);

				// 通知客户端下线
				proto::client::ac_kick_role_notify ac_kick;
				ac_kick.set_kick_reason(errcode_enum::accounts_error_enter_login);
				p_gate_user->send_msg_to_client(op_cmd::ac_kick_role_notify, ac_kick);

				// 删除原来的连接但是不删除user，还要复用
				user_manager_t::del_socket(p_gate_user->get_client_socket(), false);

				// 设置继续离线状态
				p_gate_user->set_game_offline();
			}
			break;
		case common::ROLE_GAME_STATE_LOGIN:
		case common::ROLE_GAME_STATE_SCENE:
		case common::ROLE_GAME_STATE_WAIT:
			{
				log_warn("user[%lu] was kicked by someone, state[%u]!", uid, p_gate_user->get_game_state());

				p_gate_user->set_kick_reason(KICK_ROLE_REASON_BY_OTHER);

				// 对于在线玩家，通知下线
				if (p_gate_user->is_scene_state())
				{
					user_manager_t::notify_others_unreg_user(p_gate_user, common::UNREGUSER_RET_OFFLINE);
				}

				// 通知客户端下线
				proto::client::ac_kick_role_notify ac_kick;
				ac_kick.set_kick_reason(errcode_enum::accounts_error_enter_login);
				p_gate_user->send_msg_to_client(op_cmd::ac_kick_role_notify, ac_kick);

				// 删除原来的连接但是不删除user，还要复用
				user_manager_t::del_socket(p_gate_user->get_client_socket(), false);

				// 设置离线状态，启用重登流程
				p_gate_user->set_game_offline();
			}
			break;
		default:	// 对于离线和注销状态不用处理
			{
				ROLE_LOG("user[%lu] state[%u] restart login gate", uid, p_gate_user->get_game_state());
			}
			break;
		}

 		// 重置guid
 		p_gate_user->set_guid(msg.guid());

		rlt = true;
 	}
 	else
 	{
		p_gate_user.reset(new user_t(uid, msg.guid()));

		if (user_manager_t::add_user(p_gate_user))
		{
			rlt = true;

			// 这里pingpang当作这个user的管理时间
			p_gate_user->start_pingpong_timer();
		}
 	}

	proto::server::al_login_reply rep;
	if (rlt && NULL != p_gate_user)
	{
		//不管是顶号还是重登，这里统一生成唯一验证登录串
		p_gate_user->set_session_id(common::uuid_t::random_uuid());

		rep.set_reply_code(0);
		rep.set_guid(p_gate_user->get_guid());
		rep.set_gate_ip(env::xml->listen_at_client.out_ip);
		rep.set_gate_port(env::xml->listen_at_client.out_port);
		rep.set_session_id(p_gate_user->get_session_id());
	}
	else
	{
		rep.set_reply_code(1);
	}

	env::server->send_msg_to_login(op_cmd::al_login_reply, uid, rep);

    return true;
}