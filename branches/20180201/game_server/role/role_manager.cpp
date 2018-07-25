#include "role_manager.hpp"
#include "log/log_wrapper.hpp"
#include "main/game_server.hpp"
#include "tblh/Comprehensive.tbls.h"
#include "config/config_manager.hpp"
#include "role_buff/role_buff_manager.hpp"
#include "tblh/errcode_enum.hpp"
#include "scene/scene_manager.hpp"
#include "user_troop/user_troop_mgr.hpp"

USING_NS_NETWORK;
USING_NS_COMMON;

role_map role_manager_t::m_role_list;

role_manager_t::~role_manager_t()
{
}

role_ptr role_manager_t::find_role(uint64_t uid)
{
    role_map::iterator itr = m_role_list.find(uid);
    if (itr != m_role_list.end())
        return itr->second;

    return NULL;
}

bool role_manager_t::add_role(role_ptr p_role)
{
    std::pair<role_map::iterator, bool> res = m_role_list.insert(std::make_pair(p_role->get_uid(), p_role));
    if (res.second)
    {
        return true;
    }
    return false;
}

void role_manager_t::del_role(uint64_t uid, uint32_t kick_reason)
{
    role_map::iterator itr = m_role_list.find(uid);
    if (itr == m_role_list.end())
    {
        log_warn("role[%lu] was not exsit when deleted!", uid);
        return;
    }

	const role_ptr& p_role = itr->second;
	do 
	{
		if (NULL == p_role)
		{
			log_error("NULL == p_role");
			break;
		}

		if (env::server->get_type() == game_server_type_normal && ROLE_STATE_OK == p_role->get_state())
		{
			if (p_role->get_online_time() <= 0)
			{
				ROLE_LOG("user[%lu] kick_reason[%d] has not logout_time and log!", uid, kick_reason);
				p_role->set_last_logout_time(common::time_util_t::now_time());
				log_wrapper_t::send_role_logout_log(p_role->get_uid(), p_role->get_online_time(), p_role->get_gate_id(), env::server->get_id(), kick_reason);
			}

			// 登出场景
			p_role->leave_game();

			// 回写档
			p_role->save_all_datas(true);

			// 通知其他服务器自己登出
			p_role->notify_the_other_servers_del_role();
		}

		p_role->del_clear();

	} while (0);

	m_role_list.erase(itr);

	ROLE_LOG("delete role[%lu], reason: %u", uid, kick_reason);
}

void role_manager_t::del_role(uint32_t gate_id)
{
    KICK_ROLE_REASON kick_reason = KICK_ROLE_REASON_GATE_SERVER_CLOSED;

    role_map::iterator itr = m_role_list.begin();
    for (; itr != m_role_list.end();)
    {
        const role_ptr& p_role = itr->second;
        if (NULL != p_role && p_role->get_gate_id() == gate_id)
        {
			if (env::server->get_type() == game_server_type_normal && ROLE_STATE_OK == p_role->get_state())
			{
				if (p_role->get_online_time() <= 0)
				{
					p_role->set_last_logout_time(common::time_util_t::now_time());
					log_wrapper_t::send_role_logout_log(p_role->get_uid(), p_role->get_online_time(), p_role->get_gate_id(), env::server->get_id(), kick_reason);
				}

				// 登出场景
				p_role->leave_game();

				// 回写档
				p_role->save_all_datas(true);

				// 通知其他服务器自己登出
				p_role->notify_the_other_servers_del_role();
			}

			p_role->del_clear();

			ROLE_LOG("delete role[%lu] by gate[%u], reason: %u", p_role->get_uid(), gate_id, kick_reason);

			itr = m_role_list.erase(itr);
        }
        else
        {
            ++itr;
        }
    }
}

void role_manager_t::del_all_roles(KICK_ROLE_REASON kick_reason)
{
    for (role_map::iterator citr = m_role_list.begin();
		citr != m_role_list.end(); ++citr)
    {
		const role_ptr& p_role = citr->second;
        if (NULL != p_role)
        {
			if (env::server->get_type() == game_server_type_normal && ROLE_STATE_OK == p_role->get_state())
			{
				if (p_role->get_online_time() <= 0)
				{
					p_role->set_last_logout_time(common::time_util_t::now_time());
					log_wrapper_t::send_role_logout_log(p_role->get_uid(), p_role->get_online_time(), p_role->get_gate_id(), env::server->get_id(), kick_reason);
				}

				// 登出场景
				p_role->leave_game();

				// 回写档
				p_role->save_all_datas(true);

				// 通知其他服务器自己登出
				p_role->notify_the_other_servers_del_role();
			}

			p_role->del_clear();
        }
    }

    m_role_list.clear();
}

void role_manager_t::del_role_by_cross_server(uint64_t uid, proto::server::scene_change_type type)
{
	role_map::iterator itr = m_role_list.find(uid);
	if (itr == m_role_list.end())
	{
		log_error("role[%lu] was not exsit when deleted!", uid);
		return;
	}

	const role_ptr& p_role = itr->second;
	if (p_role)
	{
		if (p_role->get_online_time() <= 0)
		{
			p_role->set_last_logout_time(common::time_util_t::now_time());

			log_wrapper_t::send_role_logout_log(p_role->get_uid(), p_role->get_online_time(), p_role->get_gate_id(), env::server->get_id(), KICK_ROLE_REASON_CROSS_SCENE);
		}

		// 登出场景
		p_role->leave_game();

		// 回写档
		p_role->save_all_datas(true);

		// 通知其他服务器自己登出
		p_role->notify_the_other_servers_del_role();

		p_role->del_clear();
	}

	m_role_list.erase(itr);

	ROLE_LOG("delete role[%lu] by cross server jump scene, type:%u", uid, type);
}

void role_manager_t::reset_arena()
{
	uint32_t now_time = time_util_t::now_time();
	for (auto it : m_role_list)
	{
		role_ptr p_role = it.second;
		if (NULL == p_role)
		{
			continue;
		}
		arena_t& arena = p_role->get_arena();
		arena.reset(now_time);
	}
}

void role_manager_t::award_arena()
{
	for (auto it : m_role_list)
	{
		role_ptr p_role = it.second;
		if (NULL == p_role)
		{
			continue;
		}
		arena_t& arena = p_role->get_arena();
		arena.award();
	}
}

void role_manager_t::scene_reg_user(uint64_t uid, const proto::server::eg_reg_user_scene_request& request)
{
	do 
	{
		role_ptr p_role = find_role(uid);
		if (p_role != NULL)
		{
			log_error("user[%lu] is alread exsit, when center reg user in scene", uid);
			role_manager_t::del_role(uid, common::KICK_ROLE_REASON_ERROR);
			break;
		}

		scene_ptr p_scene = NULL;

		switch (request.scene_data().type())
		{
		case proto::server::login_server_scene:
		case proto::server::user_change_scene:
		case proto::server::user_enter_dungeon:
		case proto::server::forced_change_scene:
		case proto::server::center_change_scene:
			{
				p_scene = scene_manager_t::find_scene(request.scene_data().scene_id());
			}
			break;
		case proto::server::user_troop_jump:
			{
				troop_ptr p_troop = game_troop_mgr_t::get_troop(request.scene_data().troop_id());
				if (p_troop != NULL)
				{
					role_ptr p_leader = find_role(p_troop->get_leader_id());
					if (p_leader != NULL)
					{
						p_scene = scene_manager_t::find_scene(p_leader->get_scene_id());

						break;
					}
					else
					{
						log_warn("role[%lu] login server type[user_troop_jump], troop[%lu] leader[%lu] not find", uid, request.scene_data().troop_id(), p_troop->get_leader_id());
					}
				}
				else
				{
					log_warn("role[%lu] login server type[user_troop_jump], troop[%lu] not find", uid, request.scene_data().troop_id());
				}

				// 一个都找不到就用center发来的场景id
				p_scene = scene_manager_t::find_scene(request.scene_data().scene_id());
			}
			break;
		default:
			{
				log_error("user[%lu] login server type[%u] error", uid, request.scene_data().type());
			}
			break;
		}

		if (p_scene == NULL)
		{
			log_error("role[%lu] login server[%lu] not find, type:%u", uid, request.scene_data().scene_id(), request.scene_data().type());
			break;
		}

		p_role.reset(new role_t(uid));

		if (!add_role(p_role))
		{
			log_error("add role[%lu] failed!", uid);
			break;
		}

		p_role->set_gate_id(request.gate_id(), env::xml->srv_group.group_id);

		p_role->set_state(ROLE_STATE_LOADING);

		p_role->set_login_type(request.scene_data().type());

		p_role->set_center_user_data(request);

		// 请求读档
		proto::server::gd_role_data_request udr;
		env::server->send_msg_to_db(op_cmd::gd_role_data_request, uid, udr);

		ROLE_LOG("role[%lu] start to get data from db...", uid);
		return;

	} while (0);

	// 通知中心服注销
	proto::server::ge_unreg_user_scene_notify ret;
	ret.set_uid(uid);
	ret.set_unreg_code(common::UNREGUSER_RET_ERROR);
	ret.set_kick_reason(common::KICK_ROLE_REASON_ERROR);
	env::server->send_msg_to_center(op_cmd::ge_unreg_user_scene_notify, uid, ret);

	// 通知所有网关服注销
	proto::server::ga_unreg_user_scene_notify retgate;
	retgate.set_uid(uid);
	retgate.set_unreg_code(common::UNREGUSER_RET_ERROR);
	env::server->send_msg_to_all_gate(op_cmd::ga_unreg_user_scene_notify, uid, retgate);
}

void role_manager_t::scene_relogin_user(uint64_t uid, uint32_t gate_id)
{
	role_ptr p_role = find_role(uid);
	if (NULL != p_role && p_role->get_state() == ROLE_STATE_OK)
	{
		p_role->set_gate_id(gate_id, env::xml->srv_group.group_id);

		p_role->set_login_type(proto::server::relogin_server_scene);

		scene_ptr p_login_scene = scene_manager_t::find_scene(p_role->get_scene_id());
		if (p_login_scene != NULL)
		{
			if (p_role->do_login_scene(p_login_scene))
			{
				// 通知网关登陆成功
				proto::server::ga_reg_user_success_notify ga_ret;
				ga_ret.set_uid(uid);
				ga_ret.set_gate_id(p_role->get_gate_id());
				p_role->send_msg_to_gate(op_cmd::ga_reg_user_success_notify, ga_ret);

				// 通知中心服场景服务器登陆成功
				proto::server::ge_reg_user_scene_reply ge_ret;
				ge_ret.set_uid(uid);
				p_role->peek_global_user_data(ge_ret.mutable_user_data());
				env::server->send_msg_to_center(op_cmd::ge_reg_user_scene_reply, uid, ge_ret);

				ROLE_LOG("role[%lu] relogin success!", uid);

				return;
			}
			else
			{
				log_error("role[%lu] relogin game failed!", uid);
			}
		}
		else
		{
			log_error("role[%lu] relogin scene[%lu] not find!", uid, p_role->get_scene_id());
		}
	}

	if (NULL != p_role)
	{
		log_error("role[%lu] relogin failed!", uid);
		role_manager_t::del_role(uid, common::KICK_ROLE_REASON_ERROR);
	}
	else
	{
		log_error("role[%lu] is not find when relogin!", uid);
	}

	// 通知中心服注销
	proto::server::ge_unreg_user_scene_notify ret;
	ret.set_uid(uid);
	ret.set_unreg_code(common::UNREGUSER_RET_ERROR);
	ret.set_kick_reason(common::KICK_ROLE_REASON_ERROR);
	env::server->send_msg_to_center(op_cmd::ge_unreg_user_scene_notify, uid, ret);

	// 通知所有网关服注销
	proto::server::ga_unreg_user_scene_notify retgate;
	retgate.set_uid(uid);
	retgate.set_unreg_code(common::UNREGUSER_RET_ERROR);
	env::server->send_msg_to_all_gate(op_cmd::ga_unreg_user_scene_notify, uid, retgate);
}

void role_manager_t::on_user_record(uint64_t uid, const proto::server::dg_role_data_reply& rdr)
{
	const role_ptr& p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role");
		return;
	}

	if (common::errcode_enum::error_ok == rdr.reply_code())
	{
		p_role->init_new_guide();
		p_role->load_all_datas(rdr);

		if (p_role->reg_scene())
		{
			ROLE_LOG("role[%lu] read record success!!!!!!!!!!!!!", uid);

			// 通知网关登陆成功
			proto::server::ga_reg_user_success_notify ga_ret;
			ga_ret.set_uid(uid);
			ga_ret.set_gate_id(p_role->get_gate_id());
			p_role->send_msg_to_gate(op_cmd::ga_reg_user_success_notify, ga_ret);

			// 通知中心服场景服务器登陆成功
			proto::server::ge_reg_user_scene_reply ge_ret;
			ge_ret.set_uid(uid);
			p_role->peek_global_user_data(ge_ret.mutable_user_data());
			env::server->send_msg_to_center(op_cmd::ge_reg_user_scene_reply, uid, ge_ret);

			return;
		}
		else
		{
			log_error("role[%lu] reg scene failed!!!!!!!!!!!!", uid);
		}
	}
	else
	{
		log_error("role[%lu] read record failed!!!!!!!!!!!!!", uid);
	}

	// 通知网关服注销
	proto::server::ga_unreg_user_scene_notify err_send;
	err_send.set_uid(uid);
	err_send.set_unreg_code(common::UNREGUSER_RET_ERROR);
	p_role->send_msg_to_gate(op_cmd::ga_unreg_user_scene_notify, err_send);

	// 通知中心服注销
	proto::server::ge_unreg_user_scene_notify err_ret;
	err_ret.set_uid(uid);
	err_ret.set_unreg_code(common::UNREGUSER_RET_ERROR);
	err_ret.set_kick_reason(common::KICK_ROLE_REASON_ERROR);
	env::server->send_msg_to_center(op_cmd::ge_unreg_user_scene_notify, uid, err_ret);

	role_manager_t::del_role(uid, common::KICK_ROLE_REASON_ERROR);
}

void role_manager_t::gate_unreg_user(uint64_t uid, uint32_t unreg_code, uint32_t kick_reason)
{
	const role_ptr& p_role = find_role(uid);
	if (NULL != p_role)
	{
		switch (unreg_code)
		{
		case common::UNREGUSER_RET_OFFLINE:
			{
                ROLE_LOG("role[%lu] UNREGUSER_RET_OFFLINE, reason:%u", uid, kick_reason);
				p_role->offline();
				
				if (env::server->get_type() == game_server_type_normal)
				{
					p_role->set_last_logout_time(common::time_util_t::now_time());
                    log_wrapper_t::send_role_logout_log(p_role->get_uid(), p_role->get_online_time(), p_role->get_gate_id(), env::server->get_id(), kick_reason);
				}

			}
			break;
		case common::UNREGUSER_RET_LOGOUT:
			{
                ROLE_LOG("role[%lu] UNREGUSER_RET_LOGOUT, reason:%u", uid, kick_reason);

				del_role(uid, kick_reason);

				// 通知中心服注销
				proto::server::ge_unreg_user_scene_notify ge_ret;
				ge_ret.set_uid(uid);
				ge_ret.set_unreg_code(unreg_code);
				ge_ret.set_kick_reason(kick_reason);
				env::server->send_msg_to_center(op_cmd::ge_unreg_user_scene_notify, uid, ge_ret);
			}
			break;
		case common::UNREGUSER_RET_ERROR:
			{
                ROLE_LOG("role[%lu] UNREGUSER_RET_ERROR, reason:%u", uid, kick_reason);

				del_role(uid, kick_reason);
			}
			break;
		default:
			log_error("user[%lu] unreg_code[%u] error", uid, unreg_code);
			break;
		}
	}
	else
	{
		log_error("user[%lu] is already logout, when gate unreg role code[%u]", uid, unreg_code);

		// 如果登出消息转发center
		if (unreg_code == common::UNREGUSER_RET_LOGOUT)
		{
			// 通知中心服注销
			proto::server::ge_unreg_user_scene_notify ge_ret;
			ge_ret.set_uid(uid);
			ge_ret.set_unreg_code(unreg_code);
			ge_ret.set_kick_reason(kick_reason);
			env::server->send_msg_to_center(op_cmd::ge_unreg_user_scene_notify, uid, ge_ret);
		}
	}
}

void role_manager_t::center_unreg_user(uint64_t uid, uint32_t unreg_code)
{
	log_error("role[%lu] center unreg role code[%u]", uid, unreg_code);

	del_role(uid, common::KICK_ROLE_REASON_ERROR);
}

void role_manager_t::enter_game(const network::tcp_socket_ptr& s, uint64_t uid)
{
    uint32_t gate_id = gate_manager_t::get_gate_id_by_socket(s);
	uint32_t server_id = gate_manager_t::get_server_id_by_gate_socket(s);

    role_ptr p_role = find_role(uid);
    if (NULL != p_role)
    {
        p_role->set_gate_id(gate_id, server_id);

		ROLE_STATE state = p_role->get_state();
		if (env::server->get_type() == game_server_type_cross)
		{
			if (ROLE_STATE_LOADING == state)
			{
			}
			else if (ROLE_STATE_OK == state)
			{
				p_role->set_online_state(ROLE_ONLINE_STATE_ONLINE);
				ROLE_LOG("role[%lu] enter game success!", uid);
			}
			else
			{
				log_error("role[%lu] state[%d] error!", uid, state);
				return;
			}
		}
    }
    else
    {
        role_ptr p_new_role(new role_t(uid));
        bool res = add_role(p_new_role);
        if (!res)
        {
            log_error("add role[%lu] failed!", uid);
            return;
        }
       
        p_new_role->set_gate_id(gate_id, server_id);

		if (env::server->get_type() == game_server_type_cross)
		{
			p_new_role->set_online_state(ROLE_ONLINE_STATE_ONLINE);
			p_new_role->set_state(ROLE_STATE_OK);
			ROLE_LOG("role[%lu] enter_game...", uid);
		}
    }
}

void role_manager_t::one_day(uint32_t curr_time, bool pass_week)
{
	for (const auto& citr : m_role_list) {
		if (NULL != citr.second) citr.second->on_next_day(curr_time, pass_week);
	}
}

void role_manager_t::on_del_buff(uint64_t role_uid, uint32_t buff_id, const boost::system::error_code& ec)
{
	//ROLE_LOG("role_manager_t::on_del_buff");
	if (ec == boost::asio::error::operation_aborted)
	{
		
		return;
	}
	else if (ec)
	{
		log_error("Timer error: %s", ec.message().c_str());
		return;
	}
	role_ptr p_role = find_role(role_uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role[%lu]", role_uid);
		return;
	}
	role_buff_mgr_ptr p_role_buf_mgr = p_role->get_role_buff_mgr();
	if (NULL == p_role_buf_mgr)
	{
		log_error("NULL == p_role[%lu]", role_uid);
		return;
	}
	p_role_buf_mgr->del_role_buff(buff_id, log_enum::source_type_role_buff_timeout, 0, true);
}
