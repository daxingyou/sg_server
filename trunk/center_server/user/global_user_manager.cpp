#include "global_user_manager.hpp"
#include "main/center_server.hpp"
#include "redis_client.hpp"
#include "redis/global_user_data_mgr.hpp"
#include "country/country_mgr.hpp"
#include "tblh/errcode_enum.hpp"
#include "scene/global_scene_manager.hpp"
#include "user_troop/user_troop_mgr.hpp"
USING_NS_NETWORK;
USING_NS_COMMON;

global_user_manager_t::global_user_map_type global_user_manager_t::m_global_user_map;

void global_user_manager_t::close_clear()
{
	m_global_user_map.clear();
}

void global_user_manager_t::ready_enter_scene(uint64_t uid)
{
    const global_user_ptr& p_user = get_global_user_by_uid(uid);
    if (NULL == p_user)
    {
        log_error("NULL == p_user");
        return;
    }

    p_user->ready_enter_scene();
}

bool global_user_manager_t::add_global_user( global_user_ptr p_user)
{
    if (NULL == p_user)
    {
        log_error("NULL == p_user");
        return false;
    }
    
	std::pair<global_user_map_type::iterator, bool> ret = m_global_user_map.insert(global_user_map_type::value_type(p_user->get_uid(), p_user));
	if (!ret.second)
	{
		log_error("user[%lu] insert error, it is exist", p_user->get_uid());
		return false;
	}

	return true;
}

void global_user_manager_t::kick_global_user(uint64_t user_uid, uint32_t kick_reason)
{
    global_user_map_type::iterator itr = m_global_user_map.find(user_uid);
    if (itr != m_global_user_map.end())
    {
        const global_user_ptr& p_user = itr->second;
        if (NULL != p_user)
        {
			p_user->on_logout();

			ROLE_LOG("delete global_user[%lu] gate_id[%d] game_id[%d], reason:%u", user_uid, p_user->get_gate_id(), p_user->get_game_id(), kick_reason);
        }

		m_global_user_map.erase(itr);
    }
}

void global_user_manager_t::kick_game_all_users(uint32_t game_id)
{
    global_user_map_type::iterator itr = m_global_user_map.begin();
    global_user_map_type::iterator itr_end = m_global_user_map.end();
    for (; itr != itr_end;)
    {
        const global_user_ptr& p_user = itr->second;
        if (NULL != p_user && p_user->get_game_id() == game_id)
        {
			p_user->on_logout();

            itr = m_global_user_map.erase(itr);
        }
        else
        { 
            ++itr;
        }
    }

	ROLE_LOG("delete game[%d] all global users, reason:%u", game_id, common::KICK_ROLE_REASON_GAME_SERVER_CLOSED);
}

void global_user_manager_t::kick_gate_all_users(uint32_t gate_id)
{
	global_user_map_type::iterator itr = m_global_user_map.begin();
	global_user_map_type::iterator itr_end = m_global_user_map.end();
	for (; itr != itr_end;)
	{
		const global_user_ptr& p_user = itr->second;
		if (NULL != p_user && p_user->get_gate_id() == gate_id)
		{
			p_user->on_logout();

			itr = m_global_user_map.erase(itr);
		}
		else
		{
			++itr;
		}
	}

	ROLE_LOG("delete gate[%d] all global users, reason:%u", gate_id, common::KICK_ROLE_REASON_GATE_SERVER_CLOSED);
}

void global_user_manager_t::kick_all_global_users(uint32_t reason)
{
	// 直接删除所有玩家
	m_global_user_map.clear();

	// 删除所有组队
	global_troop_manager::del_all_troop();

	ROLE_LOG("delete all global users, reason:%u", reason);
}

global_user_ptr global_user_manager_t::get_global_user_by_uid(uint64_t user_uid)
{
    global_user_map_type::iterator itr = m_global_user_map.find(user_uid);
    if (itr != m_global_user_map.end())
    {
        return (itr->second);
    }
    return global_user_ptr();
}

void global_user_manager_t::one_day(uint32_t curr_time)
{
	for (global_user_map_type::iterator itr = m_global_user_map.begin(); 
		itr != m_global_user_map.end(); ++itr) {
		if (itr->second != NULL) {
			itr->second->one_day(curr_time);
		}
	}
}

void global_user_manager_t::reg_global_user(const network::tcp_socket_ptr& s, uint64_t uid)
{
	if (NULL == s)
	{
		log_error("NULL == s");
		return;
	}

	uint32_t gate_id = gate_manager_t::get_gate_id_by_socket(s);

	const global_user_ptr& p_user = get_global_user_by_uid(uid);
	if (NULL != p_user)
	{
		// 只有离线状态是正确的，其他都注销
		if (/*p_user->is_offline() && */p_user->relogin(gate_id))
		{
			ROLE_LOG("user[%lu] start relogin!!!!!!!!!", uid);
		}
		else
		{
			log_error("user[%lu] is reg failed, state[%u]", uid, p_user->get_online_state());

			// 通知网关错误注销
			proto::server::ea_unreg_user_center_notify ret;
			ret.set_uid(uid);
			ret.set_unreg_code(common::UNREGUSER_RET_ERROR);
			env::server->send_msg_to_gate(s, op_cmd::ea_unreg_user_center_notify, uid, ret);

			// 通知场景错误注销
			proto::server::eg_unreg_user_scene_notify send;
			send.set_uid(uid);
			send.set_unreg_code(common::UNREGUSER_RET_ERROR);
			p_user->send_msg_to_game(op_cmd::eg_unreg_user_scene_notify, send);

			// 删除用户
			kick_global_user(uid, common::KICK_ROLE_REASON_ERROR);
		}
	}
	else
	{
		if (global_user_data_mgr_t::is_user_key_exsit(uid))
		{
			if (!global_user_data_mgr_t::is_user_ban(uid))
			{
				global_user_ptr p_new_user(new global_user_t(uid));
				if (NULL != p_new_user && p_new_user->reg_user(gate_id, false) && global_user_manager_t::add_global_user(p_new_user))
				{
					ROLE_LOG("user[%lu] reg success, start reg to scene", uid);
				} 
				else
				{
					log_error("user[%lu] is reg failed!!!!!!!!!!!!!!!", uid);

					proto::server::ea_unreg_user_center_notify ret;
					ret.set_uid(uid);
					ret.set_unreg_code(common::UNREGUSER_RET_ERROR);
					env::server->send_msg_to_gate(s, op_cmd::ea_unreg_user_center_notify, uid, ret);
				}
			}
			else 
			{
				ROLE_LOG("user[%lu] reg failed, user ban", uid);

				proto::server::ea_unreg_user_center_notify ret;
				ret.set_uid(uid);
				ret.set_unreg_code(common::UNREGUSER_RET_BAN);
				ret.set_code_param(global_user_data_mgr_t::get_user_unblock(uid));
				env::server->send_msg_to_gate(s, op_cmd::ea_unreg_user_center_notify, uid, ret);
			}
		} 
		else 
		{
			proto::server::ea_unreg_user_center_notify ret;
			ret.set_uid(uid);
			ret.set_unreg_code(common::UNREGUSER_RET_NO_ROLE);
			env::server->send_msg_to_gate(s, op_cmd::ea_unreg_user_center_notify, uid, ret);
		}
	}
}

void global_user_manager_t::create_global_user(const network::tcp_socket_ptr& s, uint64_t uid, const std::string& nickname, uint32_t role_type)
{
	if (NULL == s)
	{
		log_error("NULL == s");
		return;
	}

	if (global_user_data_mgr_t::is_user_key_exsit(uid))
	{
		log_error("user[%lu] is already have role", uid);

		proto::server::ea_create_role_reply ret;
		ret.set_uid(uid);
		ret.set_reply_code(common::errcode_enum::notice_account_is_already_have_role);
		env::server->send_msg_to_gate(s, op_cmd::ea_create_role_reply, uid, ret);
		return;
	}

	proto::server::ed_create_role_request ed_create_request;
	ed_create_request.set_uid(uid);
	ed_create_request.set_nickname(nickname);
	ed_create_request.set_role_type(role_type);
    ed_create_request.set_gate_id(gate_manager_t::get_gate_id_by_socket(s));
    ed_create_request.set_time(time_util_t::now_msecs());
	env::server->send_msg_to_db(op_cmd::ed_create_role_request, uid, ed_create_request);
}

void global_user_manager_t::create_user_success(uint32_t gate_id, uint64_t uid, const std::string& nickname, uint32_t create_time, uint32_t role_type, uint32_t channel_id )
{
	global_user_ptr p_new_user(new global_user_t(uid));
	if (NULL != p_new_user && p_new_user->reg_user(gate_id, true) && global_user_manager_t::add_global_user(p_new_user))
	{
		ROLE_LOG("user[%lu] reg success, start reg to scene", uid);
	}
	else
	{
		log_error("user[%lu] is reg failed!!!!!!!!!!!!!!!", uid);

		const network::tcp_socket_ptr& p_gate_socket = gate_manager_t::get_gate_socket_by_id(gate_id);
		if (NULL == p_gate_socket)
		{
			log_error("user[%lu] NULL == p_gate_socket[%u]", uid, gate_id);
			return;
		}

		proto::server::ea_unreg_user_center_notify ret;
		ret.set_uid(uid);
		ret.set_unreg_code(common::UNREGUSER_RET_ERROR);
		env::server->send_msg_to_gate(p_gate_socket, op_cmd::ea_unreg_user_center_notify, uid, ret);
	}
}

void global_user_manager_t::unreg_global_user(uint64_t uid, uint32_t unreg_code, uint32_t kick_reason)
{
	const global_user_ptr& p_user = get_global_user_by_uid(uid);
	if (NULL != p_user)
	{
		switch (unreg_code)
		{
		case common::UNREGUSER_RET_OFFLINE:
			{
				p_user->online(common::ROLE_ONLINE_STATE_OFFLINE);

				ROLE_LOG("user[%lu] set offline, reason:%u", uid, kick_reason);
			}
			break;
		case common::UNREGUSER_RET_LOGOUT:
		case common::UNREGUSER_RET_ERROR:
			{
				kick_global_user(uid, kick_reason);
			}
			break;
		default:
			log_error("user[%lu] unreg_code[%u] error", uid, unreg_code);
			break;
		}
	}
	else
	{
		log_warn("user[%lu] is already logout", uid);
	}
}

void global_user_manager_t::scene_reg_user_success(const network::tcp_socket_ptr& s, uint64_t uid, const proto::server::global_user_data& data)
{
	const global_user_ptr& p_user = get_global_user_by_uid(uid);
	if (NULL != p_user)
	{
		p_user->sync_role_date(data);
	}
	else
	{
		log_error("user[%lu] is not find when recv[scene_reg_user_success]", uid);

		// 通知网关错误注销
		proto::server::ea_unreg_user_center_notify ret;
		ret.set_uid(uid);
		ret.set_unreg_code(common::UNREGUSER_RET_ERROR);
		env::server->send_msg_to_all_gate(op_cmd::ea_unreg_user_center_notify, uid, ret);

		// 通知场景错误注销
		proto::server::eg_unreg_user_scene_notify send;
		send.set_uid(uid);
		send.set_unreg_code(common::UNREGUSER_RET_ERROR);
		env::server->send_msg_to_game(s, op_cmd::eg_unreg_user_scene_notify, uid, send);
	}
}

uint32_t global_user_manager_t::user_cross_scene_check(const global_user_ptr& p_user, const proto::server::cross_server_data& data)
{
	if (p_user == NULL)
	{
		log_error("p_user null error");
		return common::errcode_enum::notice_unknown;
	}

	if (data.type() != proto::server::user_change_scene)
	{
		log_error("user[%lu] cross scene check type[%u] error", p_user->get_uid(), data.type());
		return common::errcode_enum::notice_unknown;
	}

	const global_scene_ptr& p_cur_scene = global_scene_manager_t::get_scene_by_id(p_user->get_scene_id());
	if (NULL == p_cur_scene)
	{
		log_error("user[%lu] curr scene[%lu] not find when cross server scene check", p_user->get_uid(), p_user->get_scene_id());
		return common::errcode_enum::notice_unknown;
	}

	const global_scene_ptr& p_to_scene = global_scene_manager_t::get_scene_by_id(data.scene_id());
	if (NULL == p_to_scene)
	{
		log_error("user[%lu] cross server scene[%lu] not find", p_user->get_uid(), data.scene_id());
		return common::errcode_enum::notice_unknown;
	}

	if (p_cur_scene->get_game_id() == p_to_scene->get_game_id())
	{
		log_error("user[%lu] cross server scene[%lu] is in same game[%u]", p_user->get_uid(), data.scene_id(), p_to_scene->get_game_id());
		return common::errcode_enum::notice_unknown;
	}

	return p_to_scene->scene_enter_check(p_user);
}

void global_user_manager_t::user_cross_server(uint64_t uid, const proto::server::cross_server_data& data)
{
	global_user_ptr p_user = get_global_user_by_uid(uid);
	if (p_user != NULL)
	{
		uint32_t old_game_id = p_user->get_game_id();

		if (p_user->reg_cross(data))
		{
			SCENE_LOG("user[%lu] start cross server[%lu] type[%u] from game[%u] to game[%u]", uid, data.scene_id(), data.type(), old_game_id, p_user->get_game_id());
			return;
		}

		log_error("user[%lu] cross server[%lu] type[%u] erorr", uid, data.scene_id(), data.type());

		// 删除用户
		kick_global_user(uid, common::KICK_ROLE_REASON_ERROR);
	}
	else
	{
		log_error("user[%lu] can't find when cross server[%lu]", uid, data.scene_id());
	}

	// 通知网关错误注销
	proto::server::ea_unreg_user_center_notify ret;
	ret.set_uid(uid);
	ret.set_unreg_code(common::UNREGUSER_RET_ERROR);
	env::server->send_msg_to_all_gate(op_cmd::ea_unreg_user_center_notify, uid, ret);
}

uint32_t global_user_manager_t::get_delta_level(global_user_ptr p_user, global_user_ptr p_target)
{
	if (NULL == p_user)
	{
		log_error("NULL == p_user");
		return 0;
	}
	if (NULL == p_target)
	{
		log_error("NULL == p_target");
		return 0;
	}
	uint32_t self_level = p_user->get_level();
	uint32_t target_level = p_target->get_level();
	uint32_t delta_level = 0;
	if (self_level > target_level)
	{
		delta_level = self_level - target_level;
	}
	else
	{
		delta_level = target_level - self_level;
	}
	return delta_level;
}

