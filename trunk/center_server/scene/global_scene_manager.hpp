#ifndef __GLOBAL_SCENE_MANAGER_H__
#define __GLOBAL_SCENE_MANAGER_H__

#include "global_scene.hpp"
#include "global_dungeon.hpp"
#include <map>
#include <set>
#include "server.pb.h"

class global_scene_manager_t
{
public:
	static bool init();
	static bool close();

	// game 被删除
	static bool on_del_game(uint32_t game_id);
	// game 场景注册
	static void on_add_game(const network::tcp_socket_ptr& s, const proto::server::ge_game_scene_reg_request& msg);

public:
	// 添加一个场景
	static bool add_one_scene(uint32_t game_id, const ::proto::server::game_scene_reg_info& info);
	static bool add_one_dungeon(uint32_t game_id, const proto::server::ge_create_dugeon_reply& reply);
	static bool add_damily_dungeon(uint32_t game_id, uint32_t war_id, const proto::server::game_scene_reg_info& info);

	static void del_scene(uint64_t scene_id);

	static global_scene_ptr get_scene_by_id(uint64_t id);
	// 通过mapid来查找场景
	static global_scene_ptr get_scene_by_map_tid(uint32_t map_id);

public:
	static uint32_t ask_open_dungeon(const proto::server::dungeon_create_data& data);

public:
	typedef std::set<uint64_t> scene_id_list;
	typedef std::map<uint32_t, scene_id_list> game_scene_list;

	typedef std::map<uint64_t, global_scene_ptr> global_scene_map;

	typedef std::map<uint32_t, scene_id_list> diversion_scene_list; // 静态地图分流场景列表(不包括主场景)

protected:
	static game_scene_list m_game_scene_list;
	static global_scene_map m_global_scene_map;
	static diversion_scene_list m_diversion_scene_list;
};

#endif
