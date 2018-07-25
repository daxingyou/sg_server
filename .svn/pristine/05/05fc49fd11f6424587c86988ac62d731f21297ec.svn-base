#include "scene_manager.hpp"

#include "scene.hpp"
#include "king_war/king_war_scene.hpp"
#include "role/role_manager.hpp"
#include "role/role.hpp"
#include "tblh/JumpTable.tbls.h"
#include "tblh/errcode_enum.hpp"
#include "log.hpp"
#include "main/game_server_fwd.hpp"
#include "main/game_server.hpp"
#include "dungeon/dungeon_scene.hpp"
#include "global_id.hpp"
#include "user_troop/user_troop_mgr.hpp"
#include "family/family_war_scene.hpp"
#include "tblh/Comprehensive.tbls.h"

scene_manager_t::scene_map scene_manager_t::m_scenes;

bool scene_manager_t::m_is_inited = false;

scene_manager_t::map_map scene_manager_t::m_maps;

scene_manager_t::scene_safe_map scene_manager_t::m_scene_safes;

bool scene_manager_t::init()
{
	if (env::server == NULL)
	{
		log_error("env::server null error");
		return false;
	}

	std::map<uint32_t, MapTable*> confs;
	GET_ALL_CONF(MapTable, confs);
	for (std::map<uint32_t, MapTable*>::iterator itr = confs.begin(); 
		itr != confs.end(); ++itr)
	{
		MapTable* p_maptable = itr->second;
		if (NULL == p_maptable)
		{
			log_error("map[%u] null error", itr->first);
			continue;
		}

		if (p_maptable->scene_type() == proto::server::map_type_none || p_maptable->scene_type() > proto::server::em_map_type_MAX)
		{
			//log_error("map[%u] scene type[%u] error", itr->first, p_maptable->scene_type());
			continue;
		}

		// 是否是本服务器的地图
		// 对于不是本服务器的地图不加载9屏信息
		bool is_this_game = p_maptable->game_id() == env::server->get_id();

		// 对于是本场景加载的地图
		if (is_this_game)
		{
			switch (env::server->get_server_type())
			{
			case common::GAME_SERVER_STATIC:
				{
					if (p_maptable->scene_type() == proto::server::map_type_dynamic)
					{
						log_error("game[%u] [GAME_SERVER_STATIC] load map[%u] scene type is [map_type_dynamic] error", env::server->get_id(), itr->first);
						return false;
					}
				}
				break;
			case common::GAME_SERVER_DYNAMIC:
				{
					if (p_maptable->scene_type() == proto::server::map_type_static)
					{
						log_error("game[%u] [GAME_SERVER_DYNAMIC] load map[%u] scene type is [map_type_static] error", env::server->get_id(), itr->first);
						return false;
					}
				}
				break;
			default:
				break;
			}
		}

		map_ptr pMap(new map_t(itr->first, (proto::server::em_map_type)p_maptable->scene_type()));

		if (!pMap->load(p_maptable, is_this_game))
		{
			log_error("load map[%d] failed!", pMap->get_map_tid());
			return false;
		}

		m_maps.insert(std::make_pair(itr->first, pMap));

		// 注册本服务器所有静态地图
		if (is_this_game && p_maptable->scene_type() == proto::server::map_type_static)
		{
			// 静态地图sceneid = map_tid
			scene_ptr p_scene = create_scene(itr->first, itr->first, (proto::server::em_scene_type)p_maptable->get_scene_attr_type(), p_maptable->get_scene_attr_type_param());
			if (p_scene == NULL)
			{
				log_error("scene[%lu] map[%u] load error", itr->first, itr->first);
				return false;
			}
		}
	}

	return true;
}

map_ptr scene_manager_t::find_map_by_id(uint32_t map_id)
{
	map_map::iterator ptr = m_maps.find(map_id);
	if (ptr != m_maps.end())
	{
		return ptr->second;
	}

	return NULL;
}

scene_ptr scene_manager_t::create_scene(uint32_t map_id, uint64_t scene_id, proto::server::em_scene_type scene_type, uint32_t type_param)
{
	scene_ptr p_scene = NULL;

	switch (scene_type)
	{
	case proto::server::em_scene_type_emperor:
	case proto::server::em_scene_type_normal:
	case proto::server::em_scene_type_prison:
	case proto::server::em_scene_type_arena:
		{
			p_scene = scene_ptr(new scene_t(scene_id, map_id, scene_type, type_param));
		}
		break;
	case proto::server::em_scene_type_king:
		{
			p_scene = king_war_scene_ptr(new king_war_scene_t(scene_id, map_id, scene_type, type_param));
		}
		break;
	default:
		{
			log_error("map[%u] scene_type[%u] not find", map_id, scene_type);
			return NULL;
		}
		break;
	}

	if (p_scene == NULL)
	{
		log_error("create scene[%lu] by map[%u] null error", scene_id, map_id);
		return NULL;
	}

	if (!p_scene->init())
	{
		log_error("create scene[%lu] by map[%u] init error", scene_id, map_id);
		return NULL;
	}
	
	std::pair<scene_map::iterator, bool> res = m_scenes.insert(std::make_pair(scene_id, p_scene));
	if (!res.second)
	{
		log_error("create scene[%lu] by map[%u] insert error, it is exist", scene_id, map_id);
		return NULL;
	}

	return p_scene;
}

dungeon_scene_ptr scene_manager_t::create_dungeon_scene(uint32_t map_id, uint64_t scene_id, uint32_t dungeon_id, uint32_t scene_param)
{
	dungeon_scene_ptr p_dungeon_scene(new dungeon_scene_t(scene_id, map_id, proto::server::em_scene_type_dugeon, scene_param));
	if (p_dungeon_scene == NULL)
	{
		log_error("create dungeon[%u] by map[%u] null error", dungeon_id, map_id);
		return NULL;
	}

	if (!p_dungeon_scene->init_dugeon(dungeon_id))
	{
		log_error("dungeon[%u] init_dugeon error", dungeon_id);
		return NULL;
	}

	if (!p_dungeon_scene->init())
	{
		log_error("dungeon[%u] init error", dungeon_id);
		return NULL;
	}

	std::pair<scene_map::iterator, bool> res = m_scenes.insert(std::make_pair(scene_id, p_dungeon_scene));
	if (!res.second)
	{
		log_error("create dungeon[%lu] by map[%u] insert error, it is exist", scene_id, map_id);
		return NULL;
	}

	return p_dungeon_scene;
}

family_war_scene_ptr scene_manager_t::create_family_war_scene(uint32_t map_id, uint64_t scene_id)
{
	family_war_scene_ptr p_family_war_scene(new family_war_scene_t(scene_id, map_id, proto::server::em_scene_type_family_war, 0));
	if (p_family_war_scene == NULL)
	{
		log_error("create family war scene by map[%u] null error", map_id);
		return NULL;
	}

	if (!p_family_war_scene->init())
	{
		log_error("family war scene[%lu] map[%u] init error", scene_id, map_id);
		return NULL;
	}

	std::pair<scene_map::iterator, bool> res = m_scenes.insert(std::make_pair(scene_id, p_family_war_scene));
	if (!res.second)
	{
		log_error("create family war scene[%lu] by map[%u] insert error, it is exist", scene_id, map_id);
		return NULL;
	}

	return p_family_war_scene;
}

void scene_manager_t::unload_scene(uint64_t scene_id, bool notify /*= true*/)
{
	scene_map::iterator itr = m_scenes.find(scene_id);
	if (itr == m_scenes.end())
	{
		log_error("scene[%lu] is not find when unload", scene_id);
		return;
	}

	if (notify)
	{
		// 通知center删除
		proto::server::ge_game_scene_unload_notify notify;
		notify.set_scene_id(scene_id);

		if (env::server != NULL)
		{
			env::server->send_msg_to_center(op_cmd::ge_game_scene_unload_notify, 0, notify);
		}
	}

	m_scenes.erase(itr);
}

scene_ptr scene_manager_t::find_scene(uint64_t scene_id)
{
	scene_map::iterator itr = m_scenes.find(scene_id);
	if (itr != m_scenes.end())
	{
		return itr->second;
	}

	return NULL;
}

uint32_t scene_manager_t::get_pk_jump_time(proto::common::EM_PK_MODE mode)
{
	uint32_t mode_id = 0;
	switch (mode) {
		case proto::common::PK_PEACE: {
			mode_id = comprehensive_common::jump_time_pk_peace;
			break;
		}
		case proto::common::PK_SLAUGHTER: {
			mode_id = comprehensive_common::jump_time_pk_slaughter;
			break;
		}
		case proto::common::PK_FAMILY: {
			mode_id = comprehensive_common::jump_time_pk_family;
			break;
		}
		case proto::common::PK_COUNTRY: {
			mode_id = comprehensive_common::jump_time_pk_country;
			break;
		}
		default: {
			log_error("pk mode[%u] not define", mode);
			mode_id = comprehensive_common::jump_time_pk_peace;
		}
	}

	auto p_conf = GET_CONF(Comprehensive, mode_id);
	uint32_t time = GET_COMPREHENSIVE_VALUE_1(p_conf);
	return time / 1000;
}

uint32_t scene_manager_t::role_jump_scene(role_ptr p_role, proto::server::scene_change_type change_type, uint64_t scene_id, float des_x/* = -1*/, float des_y /*= -1*/, float des_dir /*= 0.f*/, bool is_teleport /*= true*/)
{
	if (NULL == p_role)
	{
		log_error("p_role is null");
		return errcode_enum::notice_jump_error;
	}

	// 传送读条检测
	if (change_type == proto::server::user_change_scene && p_role->get_pk_mode() != proto::common::PK_PEACE && is_teleport) {
		if (p_role->get_transmission_type() != proto::common::transmission_type_user) {
			log_error("role[%lu] pk_mode[%u], jump scene not reading", p_role->get_uid(), p_role->get_pk_mode());
			return errcode_enum::notice_pk_mode_reading_error;
		}

		uint32_t reading_time = get_pk_jump_time(p_role->get_pk_mode());
		uint32_t start_time = p_role->get_object_state_param();
		// 模糊判断
		if (start_time + reading_time - 2 > common::time_util_t::now_time() || common::time_util_t::now_time() > start_time + reading_time + 2) {
			log_error("role[%lu] jump scene reading time error", p_role->get_uid());
			// 重置状态
			p_role->cancel_transmission();
			return errcode_enum::notice_pk_mode_reading_error;
		}

		// 重置状态
		p_role->cancel_transmission();
	}

	// 监狱中不能传送
	if (p_role->get_pk_punish_time() > 0)
	{
		SCENE_LOG("role[%lu] pk_punish can not jump scene", p_role->get_uid());
		return common::errcode_enum::notice_pk_punishment_error;
	}

	// 判断是否有跑商的
	if (is_teleport && p_role->get_trade_info().get_trade_bag_weight() > 0)
	{
		SCENE_LOG("role[%lu] trade can not jump scene", p_role->get_uid());
		return common::errcode_enum::notice_jump_trade_error;
	}
	// 匹配中不可切换场景
	if (p_role->get_object_state() == proto::common::object_state_arena_match)
	{
		SCENE_LOG("role[%lu] arena match can not jump scene", p_role->get_uid());
		return common::errcode_enum::notice_jump_arena_match_error;
	}

	//判断组队
	if (p_role->get_troop_id() > 0)
	{
		troop_ptr p_troop = game_troop_mgr_t::get_troop(p_role->get_troop_id());

		// 只处理能找到这个队伍的情况,不能找到队伍信息说明这个队伍不在本服，所有队员都是暂离状态
		if (NULL != p_troop)
		{
			if (p_role->is_leader())
			{
				if (is_teleport && p_troop->is_member_trade())
				{
					SCENE_LOG("role[%lu] member trade can not jump scene", p_role->get_uid());
					return common::errcode_enum::notice_jump_member_trade_error;
				}

				// 队长先判断能否跳
				jump_result_type result = do_jump_scene(p_role, change_type, scene_id, des_x, des_y, des_dir);
				switch (result)
				{
				case jump_result_failed: // 失败处理
					{
						log_error("p_role[%lu] is jump scene[%lu] do_jump_scene error", p_role->get_uid(), scene_id);
						return errcode_enum::notice_jump_error;
					}
					break;
				case jump_result_success:// 成功队员做相同处理
					{
						// 队员跳
						p_troop->follow_jump_scene(change_type, scene_id, des_x, des_y, des_dir);
					}
					break;
				case jump_result_center_check: // 中心服验证的也算成功
					break;
				}

				return errcode_enum::error_ok;
			}
			else
			{
				// 有队伍不是暂离不能移动
				if (p_role->get_troop_state() == 0)
				{
					SCENE_LOG("following member[%lu] cannot jump", p_role->get_uid());
					return errcode_enum::user_troop_err_member_can_not_jump;
				}
			}
		}
	}
	
	if (do_jump_scene(p_role, change_type, scene_id, des_x, des_y, des_dir) == jump_result_failed)
	{
		log_error("p_role[%lu] is jump scene[%lu] do_jump_scene error", p_role->get_uid(), scene_id);
		return errcode_enum::notice_jump_error;
	}

	return errcode_enum::error_ok;
}

uint32_t scene_manager_t::role_fast_move(role_ptr p_role, uint32_t move_type, float des_x, float des_y, float des_dir)
{
	if (NULL == p_role)
	{
		log_error("p_role is null");
		return errcode_enum::notice_jump_error;
	}

	const scene_ptr& p_scene = scene_manager_t::find_scene(p_role->get_scene_id());
	if (NULL == p_scene)
	{
		log_error("role[%lu] nullptr == pScene", p_role->get_uid());
		return errcode_enum::notice_jump_error;
	}

	// 对于不是瞬移的广播9屏
	proto::client::gc_object_scene_jump_notify notify;
	notify.set_uid(common::string_util_t::uint64_to_string(p_role->get_object_id()));
	notify.set_object_type(p_role->get_object_type());
	notify.set_move_type(move_type);
	notify.set_dest_x(des_x);
	notify.set_dest_y(des_y);
	notify.set_dest_dir(des_dir);

	if (move_type == 0)
	{
		p_role->send_msg_to_client(op_cmd::gc_object_scene_jump_notify, notify);
	}
	else
	{
		p_role->send_msg_to_nine(op_cmd::gc_object_scene_jump_notify, notify);
	}

	p_scene->on_move_s(p_role, des_x, des_y, des_dir, 0);

	if (p_role->get_troop_id() > 0 && p_role->is_leader())
	{
		troop_ptr p_troop = game_troop_mgr_t::get_troop(p_role->get_troop_id());
		if (p_troop != NULL)
		{
			p_troop->member_fast_move(p_scene, move_type, des_x, des_y, des_dir);
		}
	}

	return errcode_enum::error_ok;
}

void scene_manager_t::role_jump_dungeon(role_ptr p_role, uint64_t scene_id, uint32_t map_tid)
{
	if (NULL == p_role)
	{
		log_error("p_role is null");
		return;
	}

	const scene_ptr& p_scene = scene_manager_t::find_scene(p_role->get_scene_id());
	if (NULL == p_scene)
	{
		log_error("role[%lu] scene[%lu] null error", p_role->get_uid(), p_role->get_scene_id());
		return;
	}

	map_ptr p_dungeon_map = scene_manager_t::find_map_by_id(map_tid);
	if (NULL == p_dungeon_map)
	{
		log_error("role[%lu] map_ptr[%u] null error", p_role->get_uid(), map_tid);
		return;
	}

	// 同场景组
	if (p_scene->get_map_group_id() == p_dungeon_map->get_map_group_id())
	{
		role_jump_scene(p_role, proto::server::user_enter_dungeon, scene_id, p_role->get_current_x(), p_role->get_current_y(), p_role->get_direction());
	}
	else
	{
		role_jump_scene(p_role, proto::server::user_enter_dungeon, scene_id);
	}
}

uint32_t scene_manager_t::role_jump_point(role_ptr p_role, uint32_t jump_id)
{
	if (NULL == p_role)
	{
		log_error("p_role is null");
		return errcode_enum::notice_jump_error;
	}

	JumpTable* p_jump_info = GET_CONF(JumpTable, jump_id);
	if (NULL == p_jump_info)
	{
		log_error("role[%lu] can not find jump id[%u]", p_role->get_object_id(), jump_id);
		return errcode_enum::notice_jump_error;
	}

	uint32_t from_map = p_jump_info->from_scene();
	uint32_t to_map = p_jump_info->to_scene();

	if (0 == to_map)
	{
		log_error("role[%lu] jump id[%u] to_map[%u] is zero", p_role->get_object_id(), jump_id, to_map);
		return errcode_enum::notice_jump_error;
	}

	if (from_map != 0 && p_role->get_map_tid() != from_map)
	{
		log_error("role[%lu] jump id[%u] from_map[%d] != self_map[%u]", p_role->get_object_id(), jump_id, from_map, p_role->get_map_tid());
		return errcode_enum::notice_jump_error;
	}

	int32_t pos_x = p_jump_info->to_position_x();
	int32_t pos_y = p_jump_info->to_position_y();

	if (pos_x < 0 || pos_y < 0)
	{
		log_error("role[%lu] jump id[%u] position[%d, %d] error ", p_role->get_object_id(), jump_id, pos_x, pos_y);
		pos_x = 0;
		pos_y = 0;
	}

	if (to_map == p_role->get_map_tid())
	{
		return role_fast_move(p_role, p_jump_info->move_type(), pos_x, pos_y, p_jump_info->to_angle());
	}
	else
	{
		return role_jump_scene(p_role, proto::server::user_change_scene, to_map, pos_x, pos_y, p_jump_info->to_angle(), false);
	}

	return errcode_enum::error_ok;
}

uint32_t scene_manager_t::role_back_scene(role_ptr p_role, proto::server::scene_change_type change_type)
{
	if (NULL == p_role)
	{
		log_error("p_role is null");
		return errcode_enum::notice_jump_error;
	}

	do
	{
		uint32_t last_map_tid = p_role->get_role_scene_info().get_last_map_tid();

		if (last_map_tid == 0)
			break;

		const scene_ptr& p_scene = scene_manager_t::find_scene(p_role->get_scene_id());
		if (NULL == p_scene)
		{
			log_error("role[%lu] scene[%lu] null error", p_role->get_uid(), p_role->get_scene_id());
			break;
		}

		map_ptr p_back_map = scene_manager_t::find_map_by_id(last_map_tid);
		if (NULL == p_back_map)
		{
			log_error("role[%lu] map_ptr[%u] null error", p_role->get_uid(), last_map_tid);
			break;
		}

		// 同场景组
		if (p_scene->get_map_group_id() == p_back_map->get_map_group_id())
		{
			return role_jump_scene(p_role, change_type, last_map_tid, p_role->get_current_x(), p_role->get_current_y(), p_role->get_direction());
		}
		else
		{
			return role_jump_scene(p_role, change_type, last_map_tid, p_role->get_role_scene_info().get_last_pos_x(), p_role->get_role_scene_info().get_last_pos_y(), p_role->get_role_scene_info().get_last_dir());
		}
	} while (0);

	// 发送任何错误去默认地图
	auto p_conf = GET_CONF(Comprehensive, comprehensive_common::born_map_tid);
	uint32_t born_map_tid = GET_COMPREHENSIVE_VALUE_1(p_conf);
	return role_jump_scene(p_role, change_type, born_map_tid);
}

void scene_manager_t::gm_jump_scene(role_ptr p_role, uint64_t scene_id, float des_x /*= -1*/, float des_y /*= -1*/, float des_dir /*= 0.f*/)
{
	if (p_role == NULL)
	{
		log_error("p_role null error");
		return;
	}

	if (p_role->get_scene_id() == scene_id)
	{
		role_fast_move(p_role, 0, des_x, des_y, des_dir);
	}
	else
	{
		do_jump_scene(p_role, proto::server::user_change_scene, scene_id, des_x, des_y, des_dir);
	}
}

void scene_manager_t::center_cross_check_reply(role_ptr p_role, const proto::server::cross_server_data& data)
{
	if (p_role == NULL)
	{
		log_error("p_role null error");
		return;
	}

	// 有队伍的判断
	if (p_role->get_troop_id() > 0)
	{
		troop_ptr p_troop = game_troop_mgr_t::get_troop(p_role->get_troop_id());

		if (NULL != p_troop)
		{
			if (p_role->is_leader())
			{
				// 把队员先拉过，队长殿后
				p_troop->follow_cross_server(data);
			}
			else
			{
				if (p_role->get_troop_state() == 0)
				{
					SCENE_LOG("following member[%lu] cannot cross jump", p_role->get_uid());
					return;
				}
			}
		}
	}

	proto::server::ga_user_cross_server_notify notify;
	proto::server::cross_server_data* p_data = notify.mutable_data();
	if (p_data) p_data->CopyFrom(data);
	p_role->send_msg_to_gate(op_cmd::ga_user_cross_server_notify, notify);

	role_manager_t::del_role_by_cross_server(p_role->get_uid(), data.type());

	SCENE_LOG("role[%lu] cross server jump scene[%lu], change type[%u]!", p_role->get_uid(), data.scene_id(), data.type());
}

void scene_manager_t::troop_do_cross_server(role_ptr p_role)
{
	if (p_role == NULL) {
		log_error("p_role null error");
		return;
	}

	proto::server::ga_user_cross_server_notify notify;
	proto::server::cross_server_data* p_data = notify.mutable_data();
	if (p_data) {
		p_data->set_type(proto::server::user_troop_jump);
	}

	p_role->send_msg_to_gate(op_cmd::ga_user_cross_server_notify, notify);

	role_manager_t::del_role_by_cross_server(p_role->get_uid(), proto::server::user_troop_jump);
}

scene_manager_t::jump_result_type scene_manager_t::do_jump_scene(role_ptr p_role, proto::server::scene_change_type change_type, uint64_t scene_id, float des_x /*= -1*/, float des_y /*= -1*/, float des_dir /*= 0.f*/)
{
	if (p_role == NULL)
	{
		log_error("p_role null error");
		return jump_result_failed;
	}

	if (p_role->get_scene_id() == scene_id)
	{
		log_error("role[%lu] jump same scene[%lu]", p_role->get_uid(), scene_id);
		return jump_result_failed;
	}

	const scene_ptr& p_scene = scene_manager_t::find_scene(scene_id);
	if (p_scene != NULL)
	{
		// 同服场景跳转
		if (p_scene->get_scene_state() != SCENE_STATE_NORMAL)
		{
			log_error("role[%lu] scene[%lu] is not running", p_role->get_uid(), scene_id);
			return jump_result_failed;
		}

		if (des_x < 0.f && des_y < 0.f)
		{
			sPos born_pos = p_scene->get_scene_born_position(p_role);
			if (born_pos.is_zero())
			{
				log_error("role[%lu] scene[%lu:%u] get born position error", p_role->get_uid(), scene_id, p_scene->get_map_tid());
				return jump_result_failed;
			}

			des_x = born_pos.m_x;
			des_y = born_pos.m_y;
			des_dir = born_pos.m_dir;
		}

		// 通知客户端加载场景，缓存之后的消息
		proto::client::gc_enter_scene_notify notify;
		notify.set_scene_id(common::string_util_t::uint64_to_string(scene_id));
		notify.set_map_tid(p_scene->get_map_tid());
		notify.set_pos_x(des_x);
		notify.set_pos_y(des_y);
		notify.set_pos_dir(des_dir);
		p_role->send_msg_to_client(op_cmd::gc_enter_scene_notify, notify);

		SCENE_LOG("role[%lu] notify client enter scene[%lu]", p_role->get_uid(), scene_id);

		// 离开现在场景
		p_role->leave_scene();

		if (!p_scene->on_enter_s(p_role, des_x, des_y, des_dir))
		{
			log_error("role[%lu] enter scene[%lu:%u] failed", p_role->get_uid(), scene_id, p_scene->get_map_tid());
			return jump_result_failed;
		}

		SCENE_LOG("role[%lu] enter scene[%lu:%u] success", p_role->get_uid(), scene_id, p_scene->get_map_tid());
	}
	else
	{
		switch (change_type)
		{
		case proto::server::user_change_scene:
			{
				proto::server::ge_user_cross_scene_request request;
				proto::server::cross_server_data* p_data = request.mutable_data();
				if (p_data)
				{
					p_data->set_type(change_type);
					p_data->set_scene_id(scene_id);
					p_data->set_des_x(des_x);
					p_data->set_des_y(des_y);
					p_data->set_des_dir(des_dir);
				}
				env::server->send_msg_to_center(op_cmd::ge_user_cross_scene_request, p_role->get_uid(), request);

				SCENE_LOG("role[%lu] cross server jump scene[%lu], start center check!", p_role->get_uid(), scene_id);

				// 中心服确认
				return jump_result_center_check;
			}
			break;
		case proto::server::user_enter_dungeon:
		case proto::server::user_troop_jump:
		case proto::server::forced_change_scene:
		case proto::server::center_change_scene:
			{
				SCENE_LOG("role[%lu] cross server jump scene[%lu], change type[%u]!", p_role->get_uid(), scene_id, change_type);

				proto::server::ga_user_cross_server_notify notify;
				proto::server::cross_server_data* p_data = notify.mutable_data();
				if (p_data)
				{
					p_data->set_type(change_type);
					p_data->set_scene_id(scene_id);
					p_data->set_des_x(des_x);
					p_data->set_des_y(des_y);
					p_data->set_des_dir(des_dir);
				}

				p_role->send_msg_to_gate(op_cmd::ga_user_cross_server_notify, notify);

				role_manager_t::del_role_by_cross_server(p_role->get_uid(), change_type);
			}
			break;
		default:
			{
				log_error("role[%lu] do_jump_scene change_type[%u] error", p_role->get_uid(), change_type);
				return jump_result_failed;
			}
			break;
		}

	}

	return jump_result_success;
}

bool scene_manager_t::is_safe_area(uint32_t id)
{
	scene_safe_map::const_iterator citr = m_scene_safes.find(id);
	if (citr != m_scene_safes.end())
	{
		return true;
	}

	return false;
}

void scene_manager_t::close()
{
	//log_error("-----------------scene close start-------------------");
	for (scene_map::iterator itr = m_scenes.begin(); itr != m_scenes.end(); ++itr)
	{
		scene_ptr p_scene = itr->second;
		if (p_scene != NULL)
		{
			p_scene->clear();
			//log_error("scene clear");
		}
	}

	//log_error("scene map close start");
	for (map_map::iterator itr = m_maps.begin(); itr != m_maps.end(); ++itr)
	{
		map_ptr p_map = itr->second;
		if (p_map != NULL)
		{
			p_map->clear();
			//log_error("scene map clear");
		}
	}

	//log_error("scene stl clear");
	m_scenes.clear();
	//log_error("scene m_scenes clear");
	m_maps.clear();
	//log_error("scene m_maps clear");
	//log_error("-----------------scene close end-------------------");
}

uint64_t scene_manager_t::get_new_dungeon_id()
{
	return common::g_id_allocator.gen_new_id(common::ID_ALLOCATOR_TYPE_SCENE);
}

void scene_manager_t::reg_game_scene_to_center(proto::server::ge_game_scene_reg_request* scene_reg_request)
{
	if (scene_reg_request == NULL)
	{
		log_error("scene_reg_request null error");
		return;
	}

	for (scene_map::const_iterator citr = m_scenes.begin();
		citr != m_scenes.end(); ++citr)
	{
		const scene_ptr& p_scene_ptr = citr->second;
		if (p_scene_ptr == NULL)
		{
			log_error("p_scene_ptr null error");
			continue;
		}

		proto::server::game_scene_reg_info* info = scene_reg_request->add_scene_infos();
		if (info != NULL)
		{
			info->set_scene_id(citr->first);
			info->set_map_tid(p_scene_ptr->get_map_tid());
		}
	}
}

void scene_manager_t::reg_game_scene_back(const proto::server::eg_game_scene_reg_reply& reply)
{
	if (m_is_inited)
	{
		// 已经注册过了，说明这个是center重启了，game没重启
		log_warn("game scene is already registered");
		return;
	}

	uint32_t failed_num = 0;

	for (int32_t i = 0; i < reply.rets_size(); ++i)
	{
		const proto::server::game_scene_reg_ret& ret = reply.rets(i);

		const scene_ptr& p_scene = find_scene(ret.scene_id());
		if (p_scene == NULL)
		{
			log_error("reg_game_scene_back scene[%lu] not find", ret.scene_id());
			continue;
		}

		if (!ret.is_success())
		{
			log_error("scene[%lu] registered failed!!!!!!!!!!", ret.scene_id());
			unload_scene(ret.scene_id(), false);
			failed_num++;
			continue;
		}

		p_scene->start();
	}

	SCENE_LOG("scene registered over, success[%u] failed[%u]", reply.rets_size() - failed_num, failed_num);

	m_is_inited = true;
}
