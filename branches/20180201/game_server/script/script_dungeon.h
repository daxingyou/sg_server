#pragma once

#include "ScriptProxy.h"
#include "dungeon/dungeon_scene.hpp"

class script_dungeon_t : public ScriptProxy<dungeon_scene_t> {//tolua_export

public:
	script_dungeon_t(dungeon_scene_t* owner);
	
	void on_monster_die(uint32_t dungeon_id, uint32_t monster_tid);
	void on_enter_area(uint32_t dungeon_id, uint32_t area_id);

	uint32_t calc_time_score(uint32_t dungeon_id, uint32_t round_time);
	uint32_t calc_death_score(uint32_t dungeon_id, uint32_t death_value);
	uint32_t calc_score_level(uint32_t dungeon_id, uint32_t score);

	void do_user_event(uint32_t dungeon_id, uint32_t event_id);

public:
	//tolua_begin
	~script_dungeon_t(void);
	void c_summon_monster(const int monster_tid, const int num);
	void c_notify_dungeon_schedule();
	unsigned int c_get_death_count(const int monster_tid);
	bool c_is_event_done(const int id);
	void c_add_event_done(const int id);
	void c_add_schedule_count(const int id, bool is_del);

private:

};

//tolua_end
