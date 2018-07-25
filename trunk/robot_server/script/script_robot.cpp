#include "script_robot.h"
#include "common/ScriptManager.hpp"
#include "common/log.hpp"

script_robot_t::script_robot_t(robot_t* owner)
	: ScriptProxy<robot_t>(owner, this, ScriptManager::getInstancePtr(), "script_robot_t" )
{
}

script_robot_t::~script_robot_t()
{
}

void script_robot_t::onInit(uint64_t robot_id)
{
	this->callRTVoid("onInit", robot_id);
}

void script_robot_t::onReset(uint64_t robot_id)
{
	this->callRTVoid("onReset", robot_id);
}

void script_robot_t::onFinalize(uint64_t robot_id)
{
	this->callRTVoid("onFinalize", robot_id);
}

void script_robot_t::onThink(uint64_t robot_id, uint32_t uTime)
{
	this->callRTVoid("onThink", robot_id, uTime);
}

void script_robot_t::onError(uint64_t robot_id)
{
	this->callRTVoid("onError", robot_id);
}

void script_robot_t::c_test_robot(const int index)
{
    log_debug("script_robot_t::c_test_robot %d", index);
}

void script_robot_t::c_move_to(float x, float z)
{
	getOwner()->move_to_postion(x, z);
}

void script_robot_t::c_fast_move(float x, float z)
{
	getOwner()->fast_move(x, z);
}

bool script_robot_t::c_move_arrive(float x, float z)
{
	return getOwner()->move_arrive(x, z);
}

bool script_robot_t::c_move_around(float x, float z, float dist)
{
	return getOwner()->move_arrive(x, z, dist);
}

int script_robot_t::c_get_scene()
{
	return getOwner()->get_robot_info().map_id;
}

void script_robot_t::c_robot_done()
{
	getOwner()->task_done();
}

float script_robot_t::c_get_currX()
{
	return getOwner()->get_currX();
}

float script_robot_t::c_get_currY()
{
	return getOwner()->get_currY();
}

void script_robot_t::c_talk_about(int index, const char* str)
{
	getOwner()->talk_about((uint32_t)index, str);
}

void script_robot_t::c_jump_scene(int id)
{
	getOwner()->jump_scene(id);
}

void script_robot_t::c_jump_id(int id)
{
	getOwner()->jump_id(id);
}

void script_robot_t::c_gm_command(const char* str)
{
	getOwner()->opt_gm_cmd(str);
}

void script_robot_t::c_toggle_mount(int state)
{
	getOwner()->toggle_mount(state);
}

void script_robot_t::c_get_mail_list()
{
	getOwner()->get_mail_list();
}

bool script_robot_t::c_have_new_mail()
{
	return getOwner()->have_new_mail();
}

void script_robot_t::c_open_one_mail()
{
	getOwner()->open_one_mail();
}

void script_robot_t::c_random_mail(int mail_num)
{
	getOwner()->random_mail_send_self(mail_num);
}

int script_robot_t::c_get_mail_num()
{
	return getOwner()->get_mail_size();
}

void script_robot_t::c_del_one_mail()
{
	getOwner()->del_one_mail();
}

int script_robot_t::c_get_addenda_mail_index()
{
	return getOwner()->get_addenda_mail_index();
}

void script_robot_t::c_get_mail_addenda(int index)
{
	getOwner()->get_mail_addenda(index);
}

void script_robot_t::c_one_key_del_all()
{
	getOwner()->one_key_del_all();
}

void script_robot_t::c_try_open_dungeon(int dungeon_id)
{
	getOwner()->try_open_dungeon(dungeon_id);
}

bool script_robot_t::c_is_in_dungeon(int dungeon_id)
{
	return getOwner()->is_in_dungeon(dungeon_id);
}

void script_robot_t::c_leave_dungeon()
{
	return getOwner()->try_leave_dungeon();
}

bool script_robot_t::c_try_enter_fight()
{
	return getOwner()->try_enter_fight();
}

bool script_robot_t::c_is_fighting()
{
	return getOwner()->get_fight_id() != 0;
}

void script_robot_t::c_try_auto_fight()
{
	getOwner()->try_auto_fight();
}

bool script_robot_t::c_is_all_killed()
{
	return getOwner()->is_all_killed();
}

int script_robot_t::c_get_fight_index()
{
	return getOwner()->get_fight_index();
}

void script_robot_t::c_dungeon_move_to(int index)
{
	getOwner()->dungeon_fast_move_to(index);
}

bool script_robot_t::c_is_near_dungeon_target(int index)
{
	return getOwner()->is_near_dungeon_target(index);
}

void script_robot_t::c_request_boss_info(int id)
{
	getOwner()->request_boss_info(id);
}

int script_robot_t::c_get_boss_monster_id()
{
	return getOwner()->get_boss_monster_id();
}

int script_robot_t::c_get_boss_map_id()
{
	return getOwner()->get_boss_map_id();
}

int script_robot_t::c_get_boss_pos_x()
{
	return getOwner()->get_boss_pos_x();
}

int script_robot_t::c_get_boss_pos_y()
{
	return getOwner()->get_boss_pos_y();
}

bool script_robot_t::c_try_attack_field_boss(int monster_id)
{
	return getOwner()->attack_field_boss(monster_id);
}

char script_robot_t::c_get_field_boss_exist_flag()
{
	return getOwner()->get_field_boss_exist_flag();
}

void script_robot_t::c_clear_field_boss_exist_flag()
{
	return getOwner()->clear_field_boss_exist_flag();
}

int script_robot_t::c_random_boss_id()
{
	return getOwner()->random_field_boss_id();
}

bool script_robot_t::c_is_join_arena()
{
	return getOwner()->is_join_arena();
}

void script_robot_t::c_join_arena()
{
	getOwner()->arena_join();
}

void script_robot_t::c_arena_match()
{
	getOwner()->arena_match();
}

bool script_robot_t::c_is_arena_matched()
{
	return getOwner()->is_arena_matched();
}

void script_robot_t::c_buy_month_card(int id)
{
	return getOwner()->buy_month_card(id);
}
