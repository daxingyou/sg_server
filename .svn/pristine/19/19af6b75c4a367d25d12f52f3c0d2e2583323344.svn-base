#ifndef __GAME_SCENE_MANAGER_H__
#define __GAME_SCENE_MANAGER_H__
// #include <Types.h>
#include <map>
#include <set>

#include "map.hpp"
#include "scene.hpp"
#include "object/object_position.hpp"
#include "server.pb.h"

class scene_manager_t
{
public:
	static bool init();
	static void close();

	// 生成一个
	static uint64_t get_new_dungeon_id();

	// 向center注册本服务器所有场景
	static void reg_game_scene_to_center(proto::server::ge_game_scene_reg_request* scene_reg_request);

	// 向center注册本服务器所有场景返回
	static void reg_game_scene_back(const proto::server::eg_game_scene_reg_reply& reply);

public:
	static map_ptr find_map_by_id(uint32_t map_id);

	static scene_ptr find_scene(uint64_t scene_id);

	// 是否是安全区
	static bool is_safe_area(uint32_t id);
	static void add_safe_area_id(uint32_t id) { m_scene_safes.insert(id); }

	// 跳场景
public:
	// 跳场景返回值
	enum jump_result_type
	{
		jump_result_failed = 0,
		jump_result_success = 1,
		jump_result_center_check = 2,
	};

	static uint32_t get_pk_jump_time(proto::common::EM_PK_MODE mode);

	/**
	 * \brief 场景跳转接口 返回值 0 表示成功 否则返回错误码
	 * \param 场景传送（会把队员一起拉过去）
	 * \param is_teleport 默认瞬移
	 */
	static uint32_t role_jump_scene(role_ptr p_role, proto::server::scene_change_type change_type, uint64_t scene_id, float des_x = -1, float des_y = -1, float des_dir = 0.f, bool is_teleport = true);
	// 同场景传送
	static uint32_t role_fast_move(role_ptr p_role, uint32_t move_type, float des_x, float des_y, float des_dir);
	// 副本传送（只会拉自己）
	static void role_jump_dungeon(role_ptr p_role, uint64_t scene_id, uint32_t map_tid);
	// 踩场景跳转点（可能是同场景）
	static uint32_t role_jump_point(role_ptr p_role, uint32_t jump_id);
	// 返回原场景
	static uint32_t role_back_scene(role_ptr p_role, proto::server::scene_change_type change_type);

	// gm 命令
	static void gm_jump_scene(role_ptr p_role, uint64_t scene_id, float des_x = -1, float des_y = -1, float des_dir = 0.f);

	// 中心服返回跨场景信息
	static void center_cross_check_reply(role_ptr p_role, const proto::server::cross_server_data& data);

	// 队伍拉人请求
	static void troop_do_cross_server(role_ptr p_role);

public:
	// 玩家跳场景
	static jump_result_type do_jump_scene(role_ptr p_role, proto::server::scene_change_type change_type, uint64_t scene_id, float des_x = -1, float des_y = -1, float des_dir = 0.f);

public:
	static scene_ptr create_scene(uint32_t map_id, uint64_t scene_id, proto::server::em_scene_type scene_type, uint32_t type_param);
	static dungeon_scene_ptr create_dungeon_scene(uint32_t map_id, uint64_t scene_id, uint32_t dungeon_id, uint32_t scene_param);
	static family_war_scene_ptr create_family_war_scene(uint32_t map_id, uint64_t scene_id);
	static void unload_scene(uint64_t scene_id, bool notify = true);

public:
	typedef std::map<uint64_t, scene_ptr> scene_map;
	static scene_map m_scenes;

	static bool m_is_inited;

	typedef std::map<uint32_t, map_ptr> map_map;
	static map_map m_maps;

	typedef std::set<uint32_t> scene_safe_map;
	static scene_safe_map m_scene_safes;	// 全世界安全区
};

#endif // !__GAME_SCENE_MANAGER_H__
