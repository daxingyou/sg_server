#include "dungeon_msg_handle.hpp"

#include "center/center_msg_handler.hpp"
#include "gate/gate_msg_handler.hpp"
#include "dungeon_manager.hpp"
#include "tblh/DungeonTable.tbls.h"
#include "user_troop/user_troop_mgr.hpp"
#include "role/role_manager.hpp"
#include "tblh/errcode_enum.hpp"
#include "scene/scene_manager.hpp"
#include "role/money_manager.hpp"

bool dungeon_msg_handle_t::init_gate_msg_handler()
{
	bool bresult = true;

	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_role_dungeon_info_request, handle_cg_role_dungeon_info_request);
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_open_dungeon_request, handle_cg_open_dungeon_request);
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_dungeon_leave_request, handle_cg_dungeon_leave_request);

	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_open_dugeon_team_check_reply, handle_cg_open_dugeon_team_check_reply);
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_ask_team_check_reuqest, handle_cg_ask_team_check_reuqest);

	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_dungeon_fight_again_request, handle_cg_dungeon_fight_again_request);

	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_dungeon_event_request, handle_cg_dungeon_event_request);
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_dungeon_buy_request, handle_cg_dungeon_buy_request);
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_dungeon_times_request, handle_cg_dungeon_times_request);

	return bresult;
}

bool dungeon_msg_handle_t::init_center_msg_handler()
{
	bool bresult = true;

	bresult &= center_msg_handler_t::m_center_msg_handle.register_func(op_cmd::eg_create_dugeon_request, handle_eg_create_dugeon_request);
	bresult &= center_msg_handler_t::m_center_msg_handle.register_func(op_cmd::eg_open_dungeon_reply, handle_eg_open_dungeon_reply);

	return bresult;
}

bool dungeon_msg_handle_t::handle_cg_role_dungeon_info_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_role_dungeon_info_request);

	const role_ptr& p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("p_role[%lu] is null ", uid);
		return false;
	}

	proto::client::gc_role_dungeon_info_reply reply;
	p_role->get_dungeon_data().peek_data(p_role, reply.mutable_dungeon_data());
	p_role->send_msg_to_client(op_cmd::gc_role_dungeon_info_reply, reply);

	return true;
}

bool dungeon_msg_handle_t::handle_cg_open_dungeon_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_open_dungeon_request);

	const role_ptr& p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("p_role[%lu] is null ", uid);
		return false;
	}

	DungeonTable* p_dungeon_table = GET_CONF(DungeonTable, msg.dungeon_id());
	if (NULL == p_dungeon_table)
	{
		log_error("role[%lu] dungeon_table id[%u] not find", p_role->get_uid(), msg.dungeon_id());
		return false;
	}

	if (dungeon_manager_t::check_dungeon(p_role, msg.dungeon_id()))
	{
		// 是否立即开启
		bool is_team_check = true;

		do 
		{
			// 不需要就为确认
			if (p_dungeon_table->team_check() != dungeon::team_need_team_check)
			{
				is_team_check = false;
				break;
			}

			if (p_role->get_troop_id() == 0)
			{
				is_team_check = false;
				break;
			}

			troop_ptr p_troop = game_troop_mgr_t::get_troop(p_role->get_troop_id());
			if (p_troop == NULL)
			{
				is_team_check = false;
				break;
			}

			// 队伍中只有一个人
			if (p_troop->get_role_size() <= 1)
			{
				is_team_check = false;
				break;
			}

			// 发起确认就位
			p_troop->start_team_check(msg.dungeon_id());

		} while (0);

		// 需要确认就位
		if (!is_team_check)
		{
			dungeon_manager_t::open_dungeon(p_role, msg.dungeon_id());
		}
	}

	return true;
}

bool dungeon_msg_handle_t::handle_cg_dungeon_leave_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_dungeon_leave_request);

	role_ptr p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("p_role[%lu] is null ", uid);
		return false;
	}

	if (!p_role->is_in_dungeon())
	{
		log_error("role[%lu] is not in dungeon", uid);
		return false;
	}

	uint32_t back_code = scene_manager_t::role_back_scene(p_role, proto::server::user_change_scene);
	if (back_code != common::errcode_enum::error_ok)
	{
		proto::client::gc_dungeon_leave_reply reply;
		reply.set_reply_code(back_code);
		p_role->send_msg_to_client(op_cmd::gc_dungeon_leave_reply, reply);
	}

	return true;
}

bool dungeon_msg_handle_t::handle_cg_open_dugeon_team_check_reply(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_open_dugeon_team_check_reply);

	const role_ptr& p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("p_role[%lu] is null ", uid);
		return false;
	}

	if (p_role->get_troop_id() == 0)
	{
		log_error("role[%lu] troop id == 0", uid);
		return true;
	}

	troop_ptr p_troop = game_troop_mgr_t::get_troop(p_role->get_troop_id());
	if (p_troop == NULL)
	{
		log_error("role[%lu] troop id[%lu] null error", uid, p_role->get_troop_id());
		return true;
	}

	p_troop->role_team_check(uid, msg.type());

	return true;
}

bool dungeon_msg_handle_t::handle_cg_ask_team_check_reuqest(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_ask_team_check_reuqest);

	const role_ptr& p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("p_role[%lu] is null ", uid);
		return false;
	}

	if (p_role->get_troop_id() == 0)
	{
		log_error("role[%lu] troop id == 0", uid);
		return true;
	}

	troop_ptr p_troop = game_troop_mgr_t::get_troop(p_role->get_troop_id());
	if (p_troop == NULL)
	{
		log_error("role[%lu] troop id[%lu] null error", uid, p_role->get_troop_id());
		return true;
	}

	// 又没有未确认的玩家
	proto::client::gc_ask_team_check_reply reply;
	p_troop->get_no_confirm_member(reply);
	p_role->send_msg_to_client(op_cmd::gc_ask_team_check_reply, reply);

	return true;
}

bool dungeon_msg_handle_t::handle_cg_dungeon_fight_again_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_dungeon_fight_again_request);

	const role_ptr& p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("p_role[%lu] is null ", uid);
		return false;
	}

	// 检查能否再次挑战
	if (dungeon_manager_t::check_fight_again(p_role))
	{
		const scene_ptr& p_scene = scene_manager_t::find_scene(p_role->get_scene_id());
		if (p_scene == NULL)
		{
			log_error("role[%lu] scene[%lu] not find", p_role->get_uid(), p_role->get_scene_id());
			return false;
		}

		dungeon_scene_ptr p_dungeon_scene = p_scene->get_dungeon_scene_ptr();
		if (p_dungeon_scene == NULL)
		{
			log_error("role[%lu] scene[%lu] is not dungeon scene", p_role->get_uid(), p_role->get_scene_id());
			return false;
		}

		if (!p_dungeon_scene->is_can_restart())
		{
			log_error("role[%lu] dungeon[%lu] can't restart", p_role->get_uid(), p_role->get_scene_id());
			return false;
		}

		// 先把这个消息给客户端，做表现
		proto::client::gc_dungeon_fight_again_reply reply;
		reply.set_reply_code(common::errcode_enum::error_ok);
		p_role->send_msg_to_client(op_cmd::gc_dungeon_fight_again_reply, reply);

		p_dungeon_scene->restart_dungeon();
	}

	return true;
}

bool dungeon_msg_handle_t::handle_cg_dungeon_event_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_dungeon_event_request);

	const role_ptr& p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("p_role[%lu] is null ", uid);
		return false;
	}

	if (!p_role->is_in_dungeon())
	{
		log_error("role[%lu] is not in dungeon", uid);
		return false;
	}

	const scene_ptr& p_scene = scene_manager_t::find_scene(p_role->get_scene_id());
	if (p_scene == NULL)
	{
		log_error("role[%lu] scene[%lu] not find", p_role->get_uid(), p_role->get_scene_id());
		return false;
	}

	dungeon_scene_ptr p_dungeon_scene = p_scene->get_dungeon_scene_ptr();
	if (p_dungeon_scene == NULL)
	{
		log_error("role[%lu] scene[%lu] is not dungeon scene", p_role->get_uid(), p_role->get_scene_id());
		return false;
	}

	p_dungeon_scene->ask_do_event(msg.event_id());

	return true;
}

bool dungeon_msg_handle_t::handle_cg_dungeon_buy_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_dungeon_buy_request);

	const role_ptr& p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("p_role[%lu] is null ", uid);
		return false;
	}

	proto::client::gc_dungeon_buy_reply reply;
	uint32_t reply_code = dungeon_manager_t::buy_times(p_role, msg.dungeon_type(), reply.mutable_dungeon_times_data(), reply.mutable_rcd());
	reply.set_reply_code(reply_code);
	p_role->send_msg_to_client(op_cmd::gc_dungeon_buy_reply, reply);
	return true;
}

bool dungeon_msg_handle_t::handle_cg_dungeon_times_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_dungeon_times_request);

	const role_ptr& p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("p_role[%lu] is null ", uid);
		return false;
	}

	dungeon_manager_t::ask_dungeon_times(p_role);
	return true;
}

bool dungeon_msg_handle_t::handle_eg_create_dugeon_request(const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::eg_create_dugeon_request);

	proto::server::ge_create_dugeon_reply reply;
	dungeon_manager_t::create_dungeon(msg.data(), reply);

	if (NULL == env::server)
	{
		log_error("env::server null error");
		return false;
	}

	env::server->send_msg_to_center(op_cmd::ge_create_dugeon_reply, 0, reply);

	return true;
}

bool dungeon_msg_handle_t::handle_eg_open_dungeon_reply(const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::eg_open_dungeon_reply);

	proto::client::gc_open_dungeon_reply reply;
	reply.set_reply_code(msg.reply_code());
	reply.set_dungeon_id(msg.data().dungeon_id());

	if (msg.reply_code() == common::errcode_enum::error_ok)
	{
		DungeonTable* p_dungeon_table = GET_CONF(DungeonTable, msg.data().dungeon_id());
		if (NULL == p_dungeon_table)
		{
			log_error("dungeon_table id[%u] not find", msg.data().dungeon_id());
			return false;
		}

		// 拉人进副本
		switch (msg.data().type())
		{
		case proto::server::dungeon_create_single:
			{
				role_ptr p_role_check = role_manager_t::find_role(msg.data().dungeon_create_id());
				if (p_role_check != NULL)
				{
					// 体力、挑战次数限制，放到副本结算的时候
					//p_role_check->get_dungeon_data().add_fight_times(p_dungeon_table->base_id());
					//// 扣除精力
					//money_manager_t::use_money(p_role_check, proto::common::MONEY_TYPE_VIGOUR, p_dungeon_table->cost_vigour(), log_enum::source_type_vigour_dungeon_reduce, 0);
					//// 单独通知精力变化
					//proto::client::gc_vigour_info_notify ntf;
					//proto::common::role_change_data* p_rcd = ntf.mutable_rcd();
					//if (p_rcd) 
					//{
					//	p_role_check->get_personal_info(p_rcd->mutable_per_info());
					//}
					//p_role_check->send_msg_to_client(op_cmd::gc_vigour_info_notify, ntf);

					p_role_check->send_msg_to_client(op_cmd::gc_open_dungeon_reply, reply);

					scene_manager_t::role_jump_dungeon(p_role_check, msg.info().scene_id(), msg.info().map_tid());
				}
			}
			break;
		case proto::server::dungeon_create_team:
			{
				troop_ptr p_troop = game_troop_mgr_t::get_troop(msg.data().dungeon_create_id());
				if (p_troop != NULL)
				{
					role_ptr p_leader = role_manager_t::find_role(p_troop->get_leader_id());
					if (NULL == p_leader)
					{
						log_error("leader[%lu] not find", p_troop->get_leader_id());
						return false;
					}
					for (const auto& p_member : p_troop->get_members())
					{
						if (NULL != p_member && p_member->get_object_type() == proto::common::SCENEOBJECT_USER)
						{
							if (p_member->get_state() != proto::common::troop_state_normal) 
							{
								DUNGEON_LOG("role[%lu] is troop state:%u", p_member->get_uid(), p_member->get_state());
								continue;
							}
							role_ptr p_role_check = role_manager_t::find_role(p_member->get_uid());
							if (p_role_check != NULL)
							{
								// 体力、挑战次数限制，放到副本结算的时候
								//p_role_check->get_dungeon_data().add_fight_times(p_dungeon_table->base_id());
								//// 扣除精力
								//money_manager_t::use_money(p_role_check, proto::common::MONEY_TYPE_VIGOUR, p_dungeon_table->cost_vigour(), log_enum::source_type_vigour_dungeon_reduce, 0);
								//// 单独通知精力变化
								//proto::client::gc_vigour_info_notify ntf;
								//proto::common::role_change_data* p_rcd = ntf.mutable_rcd();
								//if (p_rcd) 
								//{
								//	p_role_check->get_personal_info(p_rcd->mutable_per_info());
								//}
								//p_role_check->send_msg_to_client(op_cmd::gc_vigour_info_notify, ntf);

								p_role_check->send_msg_to_client(op_cmd::gc_open_dungeon_reply, reply);
							}
						}
					}
					scene_manager_t::role_jump_dungeon(p_leader, msg.info().scene_id(), msg.info().map_tid());
				}
			}
			break;
		}

		DUNGEON_LOG("open dungeon[%u] success!!!!!!!!!", msg.data().dungeon_id());
	}
	else
	{
		uint64_t notify_uid = 0;

		switch (msg.data().type())
		{
		case proto::server::dungeon_create_single:
			{
				notify_uid = msg.data().dungeon_create_id();
			}
			break;
		case proto::server::dungeon_create_team:
			{
				troop_ptr p_troop = game_troop_mgr_t::get_troop(msg.data().dungeon_create_id());
				if (p_troop != NULL)
				{
					notify_uid = p_troop->get_leader_id();
				}
			}
			break;
		}

		const role_ptr& p_role_check = role_manager_t::find_role(notify_uid);
		if (p_role_check == NULL)
		{
			log_error("role[%lu] is not find, when open_dungeon failed", notify_uid);
			return false;
		}

		p_role_check->send_msg_to_client(op_cmd::gc_open_dungeon_reply, reply);

		DUNGEON_LOG("role[%lu] open dungeon[%u] failed", notify_uid, msg.data().dungeon_id());
	}

	return true;
}

