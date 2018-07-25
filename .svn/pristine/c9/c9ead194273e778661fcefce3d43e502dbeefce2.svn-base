#pragma once

#include "ScriptProxy.h"
#include "robot/robot.hpp"

class script_robot_t : public ScriptProxy<robot_t>{//tolua_export

public:
	script_robot_t(robot_t* owner);

	void onInit(uint64_t robot_id);
	void onReset(uint64_t robot_id);
	void onFinalize(uint64_t robot_id);
	void onThink(uint64_t robot_id, uint32_t uTime);
	void onError(uint64_t robot_id);

public:
	//tolua_begin
	~script_robot_t(void);
	void c_test_robot(const int index);
	void c_move_to(float x, float z);
	void c_fast_move(float x, float z);
	bool c_move_arrive(float x, float z);
	int c_get_scene();
	void c_robot_done();
	float c_get_currX();
	float c_get_currY();
	void c_talk_about(int index, const char* str);
	void c_jump_scene(int id);
	void c_jump_id(int id);
	void c_gm_command(const char* str);
	void c_toggle_mount(int state);

	// mail
	void c_get_mail_list();
	bool c_have_new_mail();
	void c_open_one_mail();
	void c_random_mail(int mail_num);
	int c_get_mail_num();
	void c_del_one_mail();
	int c_get_addenda_mail_index();
	void c_get_mail_addenda(int index);
	void c_one_key_del_all();

	// arena
	bool c_is_join_arena();
	void c_join_arena();
	void c_arena_match();
	bool c_is_arena_fighting();

	//dungeon
	void c_try_open_dungeon(int dungeon_id);
	bool c_is_in_dungeon(int dungeon_id);
	void c_leave_dungeon();
	bool c_is_all_killed();
	int c_get_fight_index();
	void c_dungeon_move_to(int index);
	bool c_is_near_dungeon_target(int index);
	bool c_try_enter_fight();
	bool c_is_fighting();
	void c_try_auto_fight();
private:

};

//tolua_end
