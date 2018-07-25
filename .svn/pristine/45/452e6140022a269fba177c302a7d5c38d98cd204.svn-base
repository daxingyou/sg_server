#include "script_role.h"
#include "common/ScriptManager.hpp"
#include "common/log.hpp"
#include "hero/hero_manager.hpp"
#include "item/item_manager.hpp"
//#include "equip/equip_manager.hpp"
#include "role/money_manager.hpp"
#include "mail/mail_manager.hpp"
#include "scene/scene_manager.hpp"
#include "fight/fight_manager.hpp"

script_role_t::script_role_t(role_t* owner)
	: ScriptProxy<role_t>(owner, this, ScriptManager::getInstancePtr(), "script_role_t" )
{
}

script_role_t::~script_role_t(void)
{
}

void script_role_t::on_event(const int event, const int param /*= 0*/)
{
	this->callRTVoid("on_event");
}

void script_role_t::init_new_role_data()
{
	this->callRTVoid("init_new_role_data");
}

void script_role_t::on_login()
{
	this->callRTVoid("on_login");
}

void script_role_t::on_level_up(unsigned int level)
{
    this->callRTVoid("on_level_up", level);
}

void script_role_t::on_create_item(unsigned int item_tid)
{
    this->callRTVoid("on_create_item", item_tid);
}

void script_role_t::on_enter_area(unsigned int area_tid)
{
    this->callRTVoid("on_enter_area", area_tid);
}

void script_role_t::on_enter_fight(unsigned int fight_tid, unsigned int round_tid)
{
	this->callRTVoid("on_enter_fight", fight_tid, round_tid);
}

void script_role_t::on_ready_fight(unsigned int fight_tid, unsigned int round_tid)
{
	this->callRTVoid("on_ready_fight", fight_tid, round_tid);
}

void script_role_t::on_end_fight(unsigned int fight_tid)
{
	this->callRTVoid("on_end_fight", fight_tid);
}

void script_role_t::on_task_accept(unsigned int task_id)
{
	this->callRTVoid("on_task_accept", task_id);
}

void script_role_t::on_task_finish(unsigned int task_id)
{
	this->callRTVoid("on_task_finish", task_id);
}

void script_role_t::on_use_item(unsigned int item_tid)
{
	this->callRTVoid("on_use_item", item_tid);
}

void script_role_t::on_first_enter_scene()
{
	this->callRTVoid("on_first_enter_scene");
}

void script_role_t::on_finish_new_guide_touch(unsigned int guide_tid)
{
	this->callRTVoid("on_finish_new_guide_touch", guide_tid);
}

void script_role_t::c_add_hero(const int hero_index, const int level, const int source_type, const int source_param, bool is_notify)
{
	if (NULL == getOwner())
	{
		return;
	}

	hero_manager_t::add_story_hero(getOwner()->get_role(), hero_index, level);
}

void script_role_t::c_del_hero(const int hero_index, const int source_type, const int source_param, bool is_notify)
{
	if (NULL == getOwner())
	{
		return;
	}

	hero_manager_t::remove_story_hero(getOwner()->get_role(), hero_index);
}

void script_role_t::c_add_item(const int item_tid, const int add_num, const int source_type, const int source_param)
{
	if (NULL == getOwner())
	{
		return;
	}

	item_manager_t::add_item(getOwner()->get_role(), item_tid, add_num, source_type, source_param);
}

void script_role_t::c_add_equip(const int equip_tid, const int source_type, const int source_param)
{
	if (NULL == getOwner())
	{
		return;
	}

	//equip_manager_t::add_equip(getOwner()->get_role(), equip_tid, source_type);
}

void script_role_t::c_add_money(const int type, const int count, const int source_type, const int source_param,bool is_notify)
{
	if (NULL == getOwner())
	{
		return;
	}

	money_manager_t::add_money(getOwner()->get_role(), (proto::common::MONEY_TYPE)type, count, source_type, source_param,true,NULL, is_notify);
}

void script_role_t::c_init_new_role_guide(const int guide_tid, bool is_not_waiting,const int event_type)
{
    if (NULL == getOwner())
    {
        return;
    }
    
    getOwner()->init_new_role_guide(guide_tid, is_not_waiting, event_type);
}

void script_role_t::c_finish_new_role_guide(const int guide_tid)
{
    if (NULL == getOwner())
    {
        return;
    }
    
    getOwner()->finish_new_role_guide_from_script(guide_tid);
}

void script_role_t::c_open_function(const int func_id)
{
	if (NULL == getOwner())
	{
		return;
	}
	
	getOwner()->open_function(func_id);
}

void script_role_t::c_close_function(const int func_id)
{
	if (NULL == getOwner())
	{
		return;
	}

	getOwner()->close_function(func_id);
}

bool script_role_t::c_is_open_function(const int func_id)
{
	if (NULL == getOwner())
	{
		return false;
	}

	return getOwner()->is_function_open(func_id);
}

bool script_role_t::c_has_task(const int task_id)
{
	if (NULL == getOwner())
	{
		return false;
	}

	task_mgr_ptr p_task = getOwner()->get_task_mgr();
	if (NULL == p_task)
	{
		return false;
	}

	return p_task->is_doing(task_id);
}

void script_role_t::c_send_mail(const int mail_id)
{
	if (NULL == getOwner())
	{
		return;
	}

	mail_manager_t::send_mail(getOwner()->get_uid(), proto::common::MAIL_TYPE_SYS, mail_id);
}

void script_role_t::c_do_jump_point(const int jump_id)
{
	if (NULL == getOwner())
	{
		return;
	}

	scene_manager_t::role_jump_point(getOwner()->get_role(), jump_id);
}

void script_role_t::c_set_npc_code(const int func)
{
	if (NULL == getOwner())
	{
		return;
	}

	getOwner()->set_npc_code(func);
}

void script_role_t::c_set_collect_code(const int func)
{
	if (NULL == getOwner())
	{
		return;
	}

	getOwner()->set_collect_code(func);
}

void script_role_t::c_new_fight(const int against)
{
	proto::common::fight_param param;
	param.set_type(proto::common::fight_type_against);
	fight_manager_t::fight_pve(getOwner()->get_role(), against, &param);
}

void script_role_t::c_cancel_fight()
{
	role_ptr p_role = getOwner()->get_role();
	if (p_role != NULL)
	{
		combat_ptr p_combat = fight_manager_t::check_combat(p_role, p_role->get_fight_uid());
		if (p_combat != NULL)
		{
			p_combat->all_fail();
		}
	}
}