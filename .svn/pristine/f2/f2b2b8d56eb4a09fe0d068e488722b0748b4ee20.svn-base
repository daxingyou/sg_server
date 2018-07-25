#pragma once

#include "ScriptProxy.h"
#include "role/role.hpp"

class script_role_t : public ScriptProxy<role_t>{//tolua_export

public:
	script_role_t(role_t* owner);
	void on_event(const int event, const int param = 0);
	void init_new_role_data();
    void on_login();
    void on_level_up(unsigned int level);
    void on_create_item(unsigned int item_tid);
    void on_enter_area(unsigned int area_tid);
	void on_enter_fight(unsigned int fight_tid, unsigned int round_tid);
	void on_ready_fight(unsigned int fight_tid, unsigned int round_tid);
	void on_end_fight(unsigned int fight_tid);
	void on_task_accept(unsigned int task_id);
	void on_task_finish(unsigned int task_id);
	void on_use_item(unsigned int item_tid);
	void on_first_enter_scene();
	void on_finish_new_guide_touch(unsigned int guide_tid);

public:
	//tolua_begin
	~script_role_t(void);
	void c_add_hero(const int hero_index, const int level, const int source_type = 0, const int source_param = 0, bool is_notify = false);
	void c_del_hero(const int hero_index, const int source_type = 0, const int source_param = 0, bool is_notify = false);
	void c_add_item(const int item_tid, const int add_num, const int source_type, const int source_param);
	void c_add_equip(const int equip_tid, const int source_type, const int source_param);
	void c_add_money(const int type, const int count, const int source_type, const int source_param, bool is_notify = false);
    void c_init_new_role_guide(const int guide_tid, bool is_waiting_to_finish,const int event_type);
    void c_finish_new_role_guide(const int guide_tid);
	void c_open_function(const int func_id);
	void c_close_function(const int func_id);
	bool c_is_open_function(const int func_id);
	bool c_has_task(const int task_id);
	void c_send_mail(const int mail_id);
	void c_do_jump_point(const int jump_id);
	void c_set_npc_code(const int func);
	void c_set_collect_code(const int func);
	void c_new_fight(const int against_id);
	void c_cancel_fight();
private:

};

//tolua_end
