#ifndef __GAME_ROLE_SCENE_H__
#define __GAME_ROLE_SCENE_H__

#include "macros.hpp"
#include "server.pb.h"

#include <vector>
#include <boost/smart_ptr/shared_ptr.hpp>

class role_t;
typedef boost::shared_ptr<role_t> role_ptr;

class scene_t;
typedef boost::shared_ptr<scene_t> scene_ptr;

class role_scene_t
{
public:
	role_scene_t(uint64_t owner);
	~role_scene_t() {};

public:
	void set_last_scene_info(uint32_t last_map_tid, float last_pos_x, float last_pos_y, float last_dir);
	void set_last_scene_info(const proto::common::last_static_map& info);

	void get_last_scene_info(proto::common::last_static_map* info);

	uint32_t get_last_map_tid() const { return m_last_map_tid; }
	float get_last_pos_x() const { return m_last_x; }
	float get_last_pos_y() const { return m_last_y; }
	float get_last_dir() const { return m_last_dir; }

public:
	// 进入的处理
	void role_enter_scene(const role_ptr& p_role, const scene_ptr& p_scene);
	// 重登的处理
	void role_reenter_scene(const role_ptr& p_role, const scene_ptr& p_scene);
	void role_leave_scene();

	uint32_t get_map_id() { return m_map_tid; }

	void update_area(role_ptr p_role, scene_ptr p_scene);
	uint32_t get_current_area_tid() { return m_current_area_tid; }
	bool is_in_safe_area() const;

	// 更新下任务暗雷触发
	void update_area_trigger(const role_ptr& p_role);

	bool get_scene_trigger_status();

	// 每次调用计算暗雷id
	uint32_t get_trigger_monster_id();

	void clear_area_trrigger_info();

	// 设置巡逻参数
	bool set_patrol_state(bool is_patrol);
	bool is_patrol_state() const { return m_is_patrol; }

	// 更新区域出发时间
	void reset_area_trigger_time();

	// 暗雷触发时间
	void reset_trigger_time();
	uint32_t trigger_time_pass();

	void reset_patrol_trigger_time();

	// 得到任务id，以判断这个暗雷是否由任务暗雷触发
	uint32_t get_task_id() { return m_task_id; }

	// 副本id
	uint32_t get_dungeon_id() const { return m_dungeon_id; }

public:
	// 设置预加载场景id
	void set_pre_scene_info(const proto::server::cross_server_data& scene_data) { m_pre_scene_data.CopyFrom(scene_data); }
	const proto::server::cross_server_data& get_pre_scene_info() const { return m_pre_scene_data; }

private:
	uint64_t m_owner = 0;
	uint32_t m_map_tid = 0;
	// 副本id
	uint32_t m_dungeon_id = 0;

	uint32_t m_last_map_tid = 0;
	float m_last_x = 0.f;
	float m_last_y = 0.f;
	float m_last_dir = 0.f;

	// 区域id
	uint32_t m_current_area_tid = 0; // 场景中的区域信息

	// 暗雷信息
	bool m_monster_trigger = false;
	uint32_t m_trigger_time = 0;

	uint32_t m_trigger_min_time = 0;
	uint32_t m_trigger_max_time = 0;

	uint32_t m_task_id = 0;
	uint32_t m_task_trigger_monster = 0;

	// 是否在巡逻状态
	bool m_is_patrol = false;
	uint32_t m_patrol_time = 0;
	// 巡逻遇怪时间
	uint32_t m_patrol_min_time = 0;
	uint32_t m_patrol_max_time = 0;

	// 中心服下发的场景信息
	proto::server::cross_server_data m_pre_scene_data;
};

#endif
