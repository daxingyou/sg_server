#include "script_dungeon.h"
#include "common/ScriptManager.hpp"

script_dungeon_t::script_dungeon_t(dungeon_scene_t* owner)
	: ScriptProxy<dungeon_scene_t>(owner, this, ScriptManager::getInstancePtr(), "script_dungeon_t")
{

}

script_dungeon_t::~script_dungeon_t(void)
{

}

void script_dungeon_t::on_monster_die(uint32_t dungeon_id, uint32_t monster_tid)
{
	this->callRTVoid("on_monster_die", dungeon_id, monster_tid);
}

void script_dungeon_t::on_enter_area(uint32_t dungeon_id, uint32_t area_id)
{
	this->callRTVoid("on_enter_area", dungeon_id, area_id);
}

uint32_t script_dungeon_t::calc_time_score(uint32_t dungeon_id, uint32_t round_time)
{
	return this->callRTUINT("on_calc_time_score", dungeon_id, round_time);
}

uint32_t script_dungeon_t::calc_death_score(uint32_t dungeon_id, uint32_t death_value)
{
	return this->callRTUINT("on_calc_death_score", dungeon_id, death_value);
}

uint32_t script_dungeon_t::calc_score_level(uint32_t dungeon_id, uint32_t score)
{
	return this->callRTUINT("on_calc_score_level", dungeon_id, score);
}

void script_dungeon_t::do_user_event(uint32_t dungeon_id, uint32_t event_id)
{
	this->callRTVoid("do_user_event", dungeon_id, event_id);
}

void script_dungeon_t::c_summon_monster(const int monster_tid, const int num)
{
	if (NULL == getOwner())
	{
		return;
	}

	getOwner()->summon_dynamic_monster(monster_tid, num);
}

void script_dungeon_t::c_notify_dungeon_schedule()
{
	if (NULL == getOwner())
	{
		return;
	}

	getOwner()->notify_dungeon_schedule();
}

unsigned int script_dungeon_t::c_get_death_count(const int monster_tid)
{
	if (NULL == getOwner())
	{
		return 0;
	}

	return getOwner()->get_death_count(monster_tid);
}

bool script_dungeon_t::c_is_event_done(const int id)
{
	if (NULL == getOwner())
	{
		return false;
	}

	return getOwner()->is_event_done(id);
}

void script_dungeon_t::c_add_event_done(const int id)
{
	if (NULL == getOwner())
	{
		return;
	}

	getOwner()->add_event_done(id);
}

void script_dungeon_t::c_add_schedule_count(const int id, bool is_del)
{
	if (NULL == getOwner())
	{
		return;
	}

	getOwner()->add_schedule_count(id, is_del);
}

