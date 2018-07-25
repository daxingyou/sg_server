#include "global_scene_manager.hpp"

#include "global_scene.hpp"
#include "tblh/MapTable.tbls.h"
#include "tblh/DungeonTable.tbls.h"
#include "config/config_manager.hpp"
#include "tblh/errcode_enum.hpp"
#include "family_dungeon.hpp"

global_scene_manager_t::game_scene_list global_scene_manager_t::m_game_scene_list;

global_scene_manager_t::global_scene_map global_scene_manager_t::m_global_scene_map;

global_scene_manager_t::diversion_scene_list global_scene_manager_t::m_diversion_scene_list;

bool global_scene_manager_t::init()
{
	return true;
}

bool global_scene_manager_t::close()
{
	m_global_scene_map.clear();
	m_game_scene_list.clear();
	m_diversion_scene_list.clear();

	return true;
}

bool global_scene_manager_t::on_del_game(uint32_t game_id)
{	
	game_scene_list::iterator itr = m_game_scene_list.find(game_id);
	if (itr != m_game_scene_list.end())
	{
		const scene_id_list& lists = itr->second;

		for (const auto& scene_id : lists)
		{
			global_scene_map::iterator sitr = m_global_scene_map.find(scene_id);
			if (sitr != m_global_scene_map.end())
			{
				if (sitr->second != NULL) {
					if (sitr->second->is_diversion()) {
						m_diversion_scene_list[sitr->second->get_map_tid()].erase(scene_id);
					}
				}
				m_global_scene_map.erase(sitr);
			}
		}

		SCENE_LOG("game[%u] del , clear scene num[%u]", game_id, (int32_t)lists.size());
		itr->second.clear();
	}

	return true;
}

void global_scene_manager_t::on_add_game(const network::tcp_socket_ptr& s, const proto::server::ge_game_scene_reg_request& msg)
{
	uint32_t game_id = game_manager_t::get_game_id_by_socket(s);

	proto::server::eg_game_scene_reg_reply reply;
	reply.set_game_id(game_id);

	uint32_t success_num = 0;
	for (int32_t i = 0; i < msg.scene_infos_size(); ++i)
	{
		const ::proto::server::game_scene_reg_info& info = msg.scene_infos(i);

		proto::server::game_scene_reg_ret* p_ret = reply.add_rets();
		if (p_ret == NULL)
		{
			log_error("p_ret null error");
			continue;
		}

		p_ret->set_scene_id(info.scene_id());

		const global_scene_ptr& p_global_scene = global_scene_manager_t::get_scene_by_id(info.scene_id());
		if (p_global_scene != NULL)
		{
			log_error("scene[%lu] is already exsit", info.scene_id());
			p_ret->set_is_success(false);
			continue;
		}

		bool is_success = global_scene_manager_t::add_one_scene(game_id, info);

		p_ret->set_is_success(is_success);

		if (is_success)
			success_num++;
	}

	SCENE_LOG("game[%u] scene registered over, success[%u] failed[%u]", game_id, success_num, msg.scene_infos_size() - success_num);

	if (env::server)
		env::server->send_msg_to_game(s, op_cmd::eg_game_scene_reg_reply, 0, reply);
}

global_scene_ptr global_scene_manager_t::get_scene_by_id(uint64_t id)
{
	global_scene_map::iterator itr = m_global_scene_map.find(id);
	if (itr != m_global_scene_map.end())
	{
		return itr->second;
	}

	return global_scene_ptr();
}

global_scene_ptr global_scene_manager_t::get_scene_by_map_tid(uint32_t map_id)
{
	global_scene_ptr p_main_scene = get_scene_by_id(map_id);
	if (p_main_scene) {
		if (p_main_scene->get_max_user() == 0) { // 主场景没有人数限制
			return p_main_scene;
		} else {
			if (p_main_scene->get_user_count() < p_main_scene->get_max_user()) { // 主场景没满就去主场景
				return p_main_scene;
			}
		}
	}

	diversion_scene_list::const_iterator citr = m_diversion_scene_list.find(map_id);
	if (citr != m_diversion_scene_list.end()) {
		const scene_id_list& lists = citr->second;
		for (const auto& scene_id : lists) {
			global_scene_ptr p_diversion_scene = get_scene_by_id(scene_id);
			if (p_diversion_scene && p_diversion_scene->get_user_count() < p_diversion_scene->get_max_user()) {
				return p_diversion_scene;
			}
		}
	}

	// 没的去就去主场景
	return p_main_scene;
}

uint32_t global_scene_manager_t::ask_open_dungeon(const proto::server::dungeon_create_data& data)
{
	DungeonTable* p_dungeon_table = GET_CONF(DungeonTable, data.dungeon_id());
	if (NULL == p_dungeon_table)
	{
		log_error("dungeon_table id[%u] not find", data.dungeon_id());
		return common::errcode_enum::dungeon_id_not_find;
	}

	MapTable* p_map_table = GET_CONF(MapTable, p_dungeon_table->map_tid());
	if (NULL == p_map_table)
	{
		log_error("dungeon[%u] map_table[%u] null error", data.dungeon_id(), p_dungeon_table->map_tid());
		return common::errcode_enum::dungeon_create_error;
	}

	const network::tcp_socket_ptr s = game_manager_t::get_game_socket_by_id(p_map_table->main_game().first);
	if (NULL == s)
	{
		log_error("game[%u] not open!!!!!!!!!!!!!", p_map_table->main_game().first);
		return common::errcode_enum::dungeon_server_not_open;
	}

	proto::server::eg_create_dugeon_request request;
	proto::server::dungeon_create_data* p_dungeon_data = request.mutable_data();
	if (p_dungeon_data != NULL)
	{
		p_dungeon_data->CopyFrom(data);
	}

	if (env::server)
		env::server->send_msg_to_game(s, op_cmd::eg_create_dugeon_request, 0, request);

	return common::errcode_enum::error_ok;
}

bool global_scene_manager_t::add_one_scene(uint32_t game_id, const ::proto::server::game_scene_reg_info& info)
{
	global_scene_ptr p_global_scene(new global_scene_t(info.scene_id(), info.map_tid()));
	if (p_global_scene == NULL)
	{
		log_error("p_global_scene null error");
		return false;
	}

	if (!p_global_scene->init())
	{
		log_error("scene[%lu] map_tid[%u] init error", info.scene_id(), info.map_tid());
		return false;
	}

	// 设置最大玩家数量
	p_global_scene->set_max_user(info.num());
	p_global_scene->set_game_id(game_id);
	p_global_scene->set_scene_state(SCENE_STATE_NORMAL);

	std::pair<global_scene_map::iterator, bool> ret = m_global_scene_map.insert(std::make_pair(p_global_scene->get_scene_id(), p_global_scene));
	if (!ret.second)
	{
		log_error("scene[%lu] insert error, it is exist", p_global_scene->get_scene_id());
		return false;
	}

	m_game_scene_list[game_id].insert(info.scene_id());

	// 增加分流地图
	if (info.is_diversion()) {
		p_global_scene->set_is_diversion(true);
		m_diversion_scene_list[info.map_tid()].insert(info.scene_id());
	}

	return true;
}

bool global_scene_manager_t::add_one_dungeon(uint32_t game_id, const proto::server::ge_create_dugeon_reply& reply)
{
	const ::proto::server::game_scene_reg_info& info = reply.info();
	global_dungeon_ptr p_global_dungeon(new global_dungeon_t(info.scene_id(), info.map_tid(), reply.data()));
	if (p_global_dungeon == NULL)
	{
		log_error("p_global_dungeon null error");
		return false;
	}

	if (!p_global_dungeon->init())
	{
		log_error("dungeon[%lu] map_tid[%u] init error", info.scene_id(), info.map_tid());
		return false;
	}

	p_global_dungeon->set_game_id(game_id);
	p_global_dungeon->set_scene_state(SCENE_STATE_NORMAL);

	std::pair<global_scene_map::iterator, bool> ret = m_global_scene_map.insert(std::make_pair(p_global_dungeon->get_scene_id(), p_global_dungeon));
	if (!ret.second)
	{
		log_error("dungeon scene[%lu] insert error, it is exist", p_global_dungeon->get_scene_id());
		return false;
	}

	m_game_scene_list[game_id].insert(info.scene_id());

	return true;
}

bool global_scene_manager_t::add_damily_dungeon(uint32_t game_id, uint32_t war_id, const proto::server::game_scene_reg_info& info)
{
	family_dungeon_ptr p_family_dungeon(new family_dungeon_t(info.scene_id(), info.map_tid()));
	if (p_family_dungeon == NULL)
	{
		log_error("p_family_dungeon null error");
		return false;
	}

	if (!p_family_dungeon->init())
	{
		log_error("family_dungeon[%lu] map_tid[%u] init error", info.scene_id(), info.map_tid());
		return false;
	}

	p_family_dungeon->set_game_id(game_id);
	p_family_dungeon->set_scene_state(SCENE_STATE_NORMAL);

	std::pair<global_scene_map::iterator, bool> ret = m_global_scene_map.insert(std::make_pair(p_family_dungeon->get_scene_id(), p_family_dungeon));
	if (!ret.second)
	{
		log_error("family_dungeon scene[%lu] insert error, it is exist", p_family_dungeon->get_scene_id());
		return false;
	}

	p_family_dungeon->set_family_war_id(war_id);

	m_game_scene_list[game_id].insert(info.scene_id());

	return true;
}

void global_scene_manager_t::del_scene(uint64_t scene_id)
{
	global_scene_map::iterator itr = m_global_scene_map.find(scene_id);
	if (itr == m_global_scene_map.end())
	{
		log_error("scene[%lu] not find when del scene", scene_id);
		return;
	}

	const global_scene_ptr& p_global_scene = itr->second;
	if (p_global_scene != NULL)
	{
		if (p_global_scene->is_diversion()) {
			m_diversion_scene_list[p_global_scene->get_map_tid()].erase(p_global_scene->get_scene_id());
		}

		m_game_scene_list[p_global_scene->get_game_id()].erase(p_global_scene->get_scene_id());
	}

	SCENE_LOG("del one scene[%lu]!!!!!!!!!", scene_id);

	m_global_scene_map.erase(itr);
}

