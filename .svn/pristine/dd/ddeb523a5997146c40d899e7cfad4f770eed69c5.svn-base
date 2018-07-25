#include "dungeon_manager.hpp"
#include "main/game_server.hpp"
#include "main/game_server_fwd.hpp"
#include "scene/scene_manager.hpp"
#include "config/config_manager.hpp"
#include "tblh/errcode_enum.hpp"
#include "tblh/DungeonTable.tbls.h"
#include "tblh/MapTable.tbls.h"
#include "user_troop/user_troop_mgr.hpp"
#include "role/role_manager.hpp"
#include "role/role.hpp"
#include "log/log_wrapper.hpp"
#include "family/family_war_scene.hpp"
#include "role/money_manager.hpp"

std::vector<dungeon_manager_t::dungeon_set> dungeon_manager_t::m_dungeon_group;

uint32_t dungeon_manager_t::m_group_add = 0;

uint32_t dungeon_manager_t::m_group_step = 0;

boost::shared_ptr<boost::asio::deadline_timer> dungeon_manager_t::m_group_timer = NULL;

bool dungeon_manager_t::init()
{
	if (DUNGEON_MAX_GROUP == 0)
	{
		log_error("DUNGEON_MAX_GROUP size 0 error");
		return false;
	}

	m_dungeon_group.resize(DUNGEON_MAX_GROUP);

	// 定时器初始化
	if (NULL == env::server)
	{
		log_error("env::server null error");
		return false;
	}

	m_group_timer.reset(new boost::asio::deadline_timer(env::server->get_ios()));
	if (NULL == m_group_timer)
	{
		log_error("m_group_timer null error");
		return false;
	}

	// 每250毫秒执行一次 一个轮回刚好是1秒
	m_group_timer->expires_from_now(boost::posix_time::millisec(DUNGEON_PRE_TIME));
	m_group_timer->async_wait(boost::bind(&dungeon_manager_t::update, boost::asio::placeholders::error));

	return true;
}

void dungeon_manager_t::close()
{
	// 定时器清除
	if (NULL != m_group_timer)
	{
		m_group_timer->cancel();
	}
}

void dungeon_manager_t::update(const boost::system::error_code& ec)
{
	if (ec == boost::asio::error::operation_aborted)
	{
		
		return;
	}
	else if (ec)
	{
		log_error("Timer error: %s", ec.message().c_str());
		return;
	}

	if (m_group_timer != NULL)
	{
		m_group_timer->expires_from_now(boost::posix_time::millisec(DUNGEON_PRE_TIME));
		m_group_timer->async_wait(boost::bind(&dungeon_manager_t::update, boost::asio::placeholders::error));
	}

	uint32_t curr_time = common::time_util_t::now_time();

	for (dungeon_set::iterator itr = m_dungeon_group[m_group_step].begin();
		itr != m_dungeon_group[m_group_step].end();)
	{
		const scene_ptr& p_scene = scene_manager_t::find_scene(*itr);
		if (p_scene == NULL || p_scene->get_scene_state() == SCENE_STATE_UNLOADED)
		{
			if (p_scene != NULL)
			{
				scene_manager_t::unload_scene(*itr);
			}
			
			m_dungeon_group[m_group_step].erase(itr++);
		}
		else
		{
			p_scene->update(curr_time);

			++itr;
		}
	}

	m_group_step++;
	m_group_step = m_group_step % DUNGEON_MAX_GROUP;
}

void dungeon_manager_t::add_dungeon_id(uint64_t id)
{
	if (m_dungeon_group.size() < DUNGEON_MAX_GROUP)
	{
		log_error("m_dungeon_group size error");
		return;
	}

	m_dungeon_group[m_group_add].insert(id);

	m_group_add++;
	m_group_add = m_group_add % DUNGEON_MAX_GROUP;
}

void dungeon_manager_t::create_dungeon(const proto::server::dungeon_create_data& data, proto::server::ge_create_dugeon_reply& reply)
{
	DungeonTable* p_dungeon_table = GET_CONF(DungeonTable, data.dungeon_id());
	if (NULL == p_dungeon_table)
	{
		log_error("dungeon_table id[%u] not find", data.dungeon_id());
		reply.set_reply_code(common::errcode_enum::dungeon_id_not_find);
		return;
	}

	MapTable* p_map_table = GET_CONF(MapTable, p_dungeon_table->map_tid());
	if (NULL == p_map_table)
	{
		log_error("dungeon[%u] map_table[%u] null error", data.dungeon_id(), p_dungeon_table->map_tid());
		reply.set_reply_code(common::errcode_enum::dungeon_create_error);
		return;
	}

	if (p_map_table->scene_type() != proto::server::map_type_dynamic)
	{
		log_error("dungeon[%u] map_table[%u] scene_type[%u] error", data.dungeon_id(), p_dungeon_table->map_tid(), p_map_table->scene_type());
		reply.set_reply_code(common::errcode_enum::dungeon_create_error);
		return;
	}

	if (p_map_table->get_scene_attr_type() != proto::server::em_scene_type_dugeon)
	{
		log_error("dungeon[%u] map_table[%u] scene_attr_type[%u] is not dugeon when create dungeon", data.dungeon_id(), p_dungeon_table->map_tid(), p_map_table->get_scene_attr_type());
		reply.set_reply_code(common::errcode_enum::dungeon_create_error);
		return;
	}

	dungeon_scene_ptr p_dungeon_scene = scene_manager_t::create_dungeon_scene(p_dungeon_table->map_tid(), scene_manager_t::get_new_dungeon_id(), data.dungeon_id(), p_map_table->get_scene_attr_type_param());
	if (p_dungeon_scene == NULL)
	{
		log_error("dungeon[%u] map_table[%u] create_dungeon error", data.dungeon_id(), p_dungeon_table->map_tid());
		reply.set_reply_code(common::errcode_enum::dungeon_create_error);
		return;
	}

	// 设置副本信息
	p_dungeon_scene->set_create_date(data);

	// 直接开启
	p_dungeon_scene->start();

	add_dungeon_id(p_dungeon_scene->get_scene_id());

	proto::server::dungeon_create_data* p_dungeon_data = reply.mutable_data();
	if (p_dungeon_data != NULL)
	{
		p_dungeon_data->CopyFrom(data);
	}

	proto::server::game_scene_reg_info* p_reg_info = reply.mutable_info();
	if (p_reg_info != NULL)
	{
		p_reg_info->set_scene_id(p_dungeon_scene->get_scene_id());
		p_reg_info->set_map_tid(p_dungeon_scene->get_map_tid());
	}

	reply.set_reply_code(common::errcode_enum::error_ok);

	DUNGEON_LOG("creater[%lu] create dungeon[%u] success", data.dungeon_create_id(), data.dungeon_id());
}

family_war_scene_ptr dungeon_manager_t::create_family_war_scene(uint32_t map_tid, const proto::server::family_war_dungeon_data& data)
{
	family_war_scene_ptr p_family_war = scene_manager_t::create_family_war_scene(map_tid, scene_manager_t::get_new_dungeon_id());
	if (p_family_war == NULL)
	{
		log_error("map_table[%u] create family war scene null error", map_tid);
		return NULL;
	}

	p_family_war->set_family_war_date(data);

	// 直接开启
	p_family_war->start();

	add_dungeon_id(p_family_war->get_scene_id());

	DUNGEON_LOG("family_war[%u] create family war scene[%lu] success!", data.id(), p_family_war->get_scene_id());

	return p_family_war;
}

bool dungeon_manager_t::check_fight_again(const role_ptr& p_role)
{
	if (NULL == p_role)
	{
		log_error("NULL == p_role");
		return false;
	}

	if (!p_role->is_in_dungeon())
	{
		log_error("role[%lu] is not in dungeon", p_role->get_uid());
		return false;
	}

	uint32_t dungeon_id = p_role->get_role_scene_info().get_dungeon_id();
	DungeonTable* conf = GET_CONF(DungeonTable, dungeon_id);
	if (NULL == conf)
	{
		log_error("dungeon[%u] null error", dungeon_id);
		return false;
	}

	if (conf->dungeon_type() != dungeon::dungeon_type_daily)
	{
		log_error("dungeon[%u] dungeon_type[%u] can't fight again", dungeon_id, conf->dungeon_type());
		return false;
	}

	if (!check_dungeon(p_role, dungeon_id))
	{
		log_error("role[%lu] check dungeon[%u] error", p_role->get_uid(), dungeon_id);
		return false;
	}

	return true;
}

void dungeon_manager_t::open_dungeon(role_ptr p_role, uint32_t dungeon_id)
{
	if (env::server == NULL)
	{
		log_error("env::server null error");
		return;
	}

	if (p_role == NULL)
	{
		log_error("p_role null error");
		return;
	}

	proto::server::dungeon_create_type create_type = proto::server::dungeon_create_single;
	uint64_t create_id = 0;

	if (p_role->get_troop_id() != 0)
	{
		create_type = proto::server::dungeon_create_team;
		create_id = p_role->get_troop_id();
	}
	else
	{
		create_type = proto::server::dungeon_create_single;
		create_id = p_role->get_object_id();
	}

	proto::server::ge_open_dungeon_request request;
	proto::server::dungeon_create_data* data = request.mutable_data();
	if (data != NULL)
	{
		data->set_game_id(env::server->get_id());
		data->set_dungeon_id(dungeon_id);
		data->set_type(create_type);
		data->set_dungeon_create_id(create_id);
	}

	env::server->send_msg_to_center(op_cmd::ge_open_dungeon_request, 0, request);

	log_wrapper_t::send_dungeon_log(p_role->get_uid(), dungeon_id, create_type, create_id, log_enum::source_type_dungeon_open, 0);

	DUNGEON_LOG("role[%lu] open dungeon[%u]", p_role->get_uid(), dungeon_id);
}

void dungeon_manager_t::open_dungeon_by_team(uint64_t leader_id, uint64_t team_id, uint32_t dungeon_id)
{
	if (env::server == NULL)
	{
		log_error("env::server null error");
		return;
	}

	proto::server::ge_open_dungeon_request request;
	proto::server::dungeon_create_data* data = request.mutable_data();
	if (data != NULL)
	{
		data->set_game_id(env::server->get_id());
		data->set_dungeon_id(dungeon_id);

		data->set_type(proto::server::dungeon_create_team);
		data->set_dungeon_create_id(team_id);
	}

	env::server->send_msg_to_center(op_cmd::ge_open_dungeon_request, 0, request);

	log_wrapper_t::send_dungeon_log(leader_id, dungeon_id, proto::server::dungeon_create_team, team_id, log_enum::source_type_dungeon_open, 0);

	DUNGEON_LOG("team[%lu] leader[%lu] open dungeon[%u]", team_id, leader_id, dungeon_id);
}

bool dungeon_manager_t::check_dungeon(role_ptr p_role, uint32_t dungeon_id)
{
	if (p_role == NULL)
	{
		log_error("p_role null error");
		return false;
	}

	uint32_t back_code = common::errcode_enum::error_ok;
	uint64_t error_uid = 0;

	do
	{
		troop_ptr p_troop = NULL;
		if (p_role->get_troop_id() != 0)
		{
			p_troop = game_troop_mgr_t::get_troop(p_role->get_troop_id());

			if (p_troop == NULL)
			{
				log_error("role[%lu] team[%lu] null error", p_role->get_uid(), p_role->get_troop_id());
				back_code = common::errcode_enum::notice_unknown;
				break;
			}
		}

		if (p_troop != NULL && p_troop->get_leader_id() != p_role->get_uid())
		{
			log_error("role[%lu] not leader", p_role->get_uid());
			back_code = common::errcode_enum::dungeon_create_not_leader;
			break;
		}

		DungeonTable* p_dungeon_table = GET_CONF(DungeonTable, dungeon_id);
		if (NULL == p_dungeon_table)
		{
			log_error("role[%lu] dungeon_table id[%u] not find", p_role->get_uid(), dungeon_id);
			back_code = common::errcode_enum::dungeon_id_not_find;
			break;
		}

		// 限时副本是否在开启时间内
		if (p_dungeon_table->is_limit_time_dungeon() && !p_dungeon_table->is_in_open_time(common::time_util_t::now_time()))
		{
			DUNGEON_LOG("role[%lu] dungeon_table id[%u] not in limit time", p_role->get_uid(), dungeon_id);
			back_code = common::errcode_enum::dungeon_time_limit;
			break;
		}

		MapTable* p_map_table = GET_CONF(MapTable, p_dungeon_table->map_tid());
		if (NULL == p_map_table)
		{
			log_error("role[%lu] dungeon[%u] map_table[%u] null error", p_role->get_uid(), dungeon_id, p_dungeon_table->map_tid());
			back_code = common::errcode_enum::dungeon_create_error;
			break;
		}

		if (p_map_table->scene_type() != proto::server::map_type_dynamic)
		{
			log_error("role[%lu] dungeon[%u] map_table[%u] scene_type error", p_role->get_uid(), dungeon_id, p_dungeon_table->map_tid());
			back_code = common::errcode_enum::dungeon_create_error;
			break;
		}

		bool check_team_type = true;
		switch (p_dungeon_table->team_type())
		{
		case dungeon::team_type_none:
			{
				// 不判断
			}
			break;
		case dungeon::team_type_single:
			{
				if (p_troop != NULL)
				{
					DUNGEON_LOG("role[%lu] dungeon[%u] is single dungeon!!!", p_role->get_uid(), dungeon_id);
					back_code = common::errcode_enum::dungeon_create_team_error3;
					check_team_type = false;
				}
			}
			break;
		case dungeon::team_type_team_2:
			{
				if (p_troop == NULL || p_troop->get_members().size() < 2)
				{
					DUNGEON_LOG("role[%lu] dungeon[%u] team_member size < 2", p_role->get_uid(), dungeon_id);
					back_code = common::errcode_enum::dungeon_create_team_error1;
					check_team_type = false;
				}
			}
			break;
		case dungeon::team_type_team_3:
			{
				if (p_troop == NULL || p_troop->get_members().size() < 3)
				{
					DUNGEON_LOG("role[%lu] dungeon[%u] team_member size < 3", p_role->get_uid(), dungeon_id);
					back_code = common::errcode_enum::dungeon_create_team_error2;
					check_team_type = false;
				}
			}
			break;
		default:
			{
				log_error("role[%lu] dungeon[%u] team_type[%u]", p_role->get_uid(), dungeon_id, p_dungeon_table->team_type());
				back_code = common::errcode_enum::notice_unknown;
				check_team_type = false;
			}
			break;
		}

		if (!check_team_type)
			break;

		// 检测有没解锁
		std::vector<uint64_t> check_list;
		bool dungeon_team_limit = true;

		if (p_troop == NULL)
		{
			check_list.push_back(p_role->get_uid());
		}
		else
		{
			for (const auto& p_member : p_troop->get_members())
			{
				if (NULL != p_member)
				{
					if (p_member->get_state() != proto::common::troop_state_normal) {
						DUNGEON_LOG("role[%lu] is troop state:%u", p_member->get_uid(), p_member->get_state());
						back_code = common::errcode_enum::dungeon_troop_member_leave;
						error_uid = p_member->get_uid();
						dungeon_team_limit = false;
						break;
					}

					check_list.push_back(p_member->get_uid());
				}
			}
		}

		if (!dungeon_team_limit)
			break;

		for (const auto& uid : check_list)
		{
			const role_ptr& p_role_check = role_manager_t::find_role(uid);
			if (NULL != p_role_check)
			{
				// 判断是否有跑商的
				if (p_role_check->get_mode_state() == proto::common::role_mode_state_trade)
				{
					DUNGEON_LOG("role[%lu] trade can not jump scene", p_role_check->get_uid());
					back_code = common::errcode_enum::notice_jump_member_trade_error;
					error_uid = uid;
					dungeon_team_limit = false;
					break;
				}

				if (p_dungeon_table->dungeon_type() == dungeon::dungeon_type_plot && p_role_check->get_dungeon_data().get_dungeon_pass_time(dungeon_id) > 0)
				{
					DUNGEON_LOG("role[%lu] plot dungeon[%u] is already pass", p_role_check->get_uid(), dungeon_id);
					back_code = common::errcode_enum::dungeon_team_plot_is_pass;
					error_uid = uid;
					dungeon_team_limit = false;
					break;
				}

				if (p_dungeon_table->level() > 0 && p_role_check->get_level() < p_dungeon_table->level())
				{
					DUNGEON_LOG("role[%lu] dungeon[%u] level error", p_role_check->get_uid(), dungeon_id);
					back_code = common::errcode_enum::dungeon_create_error_level;
					error_uid = uid;
					dungeon_team_limit = false;
					break;
				}

				uint32_t vigour = p_role_check->get_vigour();
				if (vigour < p_dungeon_table->cost_vigour()) {
					DUNGEON_LOG("role[%lu] dungeon[%u] vigour[%u] < %u", p_role_check->get_uid(), dungeon_id, vigour, p_dungeon_table->cost_vigour());
					back_code = common::errcode_enum::dungeon_create_vigour_no_enough;
					error_uid = uid;
					dungeon_team_limit = false;
					break;
				}

				for (const auto& item : p_dungeon_table->open_condition())
				{
					if (proto::common::dungeon_condition_type_task == item.first)
					{
						task_mgr_ptr p_task_mgr_ptr = p_role_check->get_task_mgr();
						if (p_task_mgr_ptr == NULL || !p_task_mgr_ptr->is_done(item.second))
						{
							DUNGEON_LOG("role[%lu] task[%u] not done", p_role_check->get_uid(), item.second);
							back_code = common::errcode_enum::dungeon_create_error_lock;
							error_uid = uid;
							dungeon_team_limit = false;
							break;
						}
					}
					else if (proto::common::dungeon_condition_type_pass == item.first)
					{
						if (p_role_check->get_dungeon_data().get_dungeon_pass_time(item.second) == 0)
						{
							DUNGEON_LOG("role[%lu] dungeon[%u] is not pass", p_role_check->get_uid(), item.second);
							back_code = common::errcode_enum::dungeon_create_error_lock;
							error_uid = uid;
							dungeon_team_limit = false;
							break;
						}
					} else {
						log_error("dungeon[%u] condition type[%u] not define", dungeon_id, item.first);
					}
				}

				if (!dungeon_team_limit)
					break;
			}
		}

		if (!dungeon_team_limit)
			break;

	} while (0);

	if (back_code != common::errcode_enum::error_ok)
	{
		proto::client::gc_open_dungeon_reply reply;
		reply.set_reply_code(back_code);
		reply.set_dungeon_id(dungeon_id);
		reply.add_team_uids(common::string_util_t::uint64_to_string(error_uid));

		p_role->send_msg_to_client(op_cmd::gc_open_dungeon_reply, reply);
	}

	return back_code == common::errcode_enum::error_ok;
}
