#pragma once
#include "global_scene.hpp"

class family_dungeon_t : public global_scene_t
{
public:
	family_dungeon_t(uint64_t scene_id, uint32_t map_tid);
	virtual ~family_dungeon_t() {};

public:
	// 是否是副本
	virtual bool is_family_dungeon() const { return true; }
	// 获取副本场景指针
	virtual family_dungeon_ptr get_family_dungeon_ptr() { return boost::dynamic_pointer_cast<family_dungeon_t>(shared_from_this()); }

	// 设置家族战id
	void set_family_war_id(uint32_t id) { m_family_war_id = id; }
	uint32_t get_family_war_id() const { return m_family_war_id; }

	virtual void on_role_enter(const global_user_ptr& p_user);

protected:
	uint32_t m_family_war_id = 0;
};
