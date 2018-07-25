#include "task.hpp"
#include "log.hpp"
#include "role/role_manager.hpp"
#include <sstream>
#include "utility.hpp"
#include "scene/scene.hpp"
#include "op_cmd.hpp"
#include "server.pb.h"
#include "role/role.hpp"
#include "role/role_unify_save.hpp"
#include "cache_key.hpp"
#include "hero/hero_manager.hpp"
#include "item/item_manager.hpp"
#include "Garrison.pb.h"
#include "GarrisonCitylist.pb.h"
#include "tblh/errcode_enum.hpp"
#include "log/log_wrapper.hpp"
#include "fight/fight_manager.hpp"
#include "hero/cultivate_manager.hpp"
#include "role/money_manager.hpp"
#include "tblh/Task.tbls.h"
#include "tblh/HeroGet.tbls.h"
#include "global_id.hpp"
#include "tblh/Comprehensive.tbls.h"
#include "tblh/TaskCircle.tbls.h"
#include "tblh/TaskShilian.tbls.h"
#include "tblh/Item.tbls.h"
#include "bounty_manager.hpp"
#include "mail/mail_manager.hpp"
#include "lifeforce/lifeforce_manager.hpp"
#include "page/page_manager.hpp"
#include "achieve/achieve_common.hpp"
#include "item/drop_manager.hpp"
#include "user_troop/user_troop_mgr.hpp"
#include "tblh/Activities.tbls.h"

task_circle_map task_manager_t::s_circle_map;
std::map<uint32_t, uint32_t> task_manager_t::s_prop_task_coeff;
std::vector<uint32_t> task_manager_t::s_shilian_seq;
std::map<uint32_t, uint32_t> task_manager_t::s_quick_task_list;

USING_NS_COMMON;

task_manager_t::task_manager_t(uint64_t role_uid)
	:m_role_uid(role_uid)
{
	m_key = cache_key_t::create(m_role_uid, cache_name::shilian);
	init();
}

uint64_t task_manager_t::gen_task_uid()
{
	uint64_t uid = common::g_id_allocator.gen_new_id(common::ID_ALLOCATOR_TYPE_TASK);
	return uid;
}

bool task_manager_t::is_activated(uint32_t id)
{
	task_ptr p_task = get_task(id);
	if (NULL == p_task)
	{
		return false;
	}
	return p_task->get_state() != TASK_STATE_NOT_ACCEPT;
}


task_ptr task_manager_t::get_task_by_uid(uint64_t uid)
{
	std::map<uint64_t, task_ptr>::iterator it = m_task_map.find(uid);
	if (it == m_task_map.end())
	{
		return task_ptr(NULL);
	}
	return it->second;
}

task_ptr task_manager_t::get_task(uint32_t task_id)
{
	for (auto it : m_task_map)
	{
		task_ptr p_task = it.second;
		if (NULL == p_task)
		{
			continue;
		}
		if (p_task->get_tid() == task_id)
		{
			return p_task;
		}
	}
	return task_ptr(NULL);
}

task_ptr task_manager_t::get_circle_task(uint32_t task_type)
{
	std::map<uint32_t, task_ptr>::iterator it = m_circle_task_map.find(task_type);
	if (it == m_circle_task_map.end())
	{
		return task_ptr(NULL);
	}
	return it->second;
}

bool task_manager_t::is_done(uint32_t id)
{
	task_ptr p_task = get_task(id);
	if (NULL == p_task)
	{
		return false;
	}
	return p_task->get_state() == TASK_STATE_FINISH;
}

bool task_manager_t::is_doing(uint32_t id)
{
	task_ptr p_task = get_task(id);
	if (NULL == p_task)
	{
		return false;
	}
	if (p_task->get_state() == TASK_STATE_DOING || p_task->get_state() == TASK_STATE_CAN_FINISH)
		return true;
	return false;
}

bool task_manager_t::active_task_by_id(uint32_t id, accept_task_type type, proto::common::role_change_data& rcd, bool is_notify/* = true*/)
{
	auto p_role = role_manager_t::find_role(m_role_uid);
	if (NULL == p_role)
	{
		log_error("task owner[%lu] not valid", m_role_uid);
		return false;
	}

	auto p_conf = GET_CONF(Task, id);
	if (NULL == p_conf)
	{
		log_error("NULL == p_conf[%d]", id);
		return false;
	}

	uint32_t task_type = p_conf->type();

	if (task_type == TASK_TYPE_ZHUOGUI)
	{
		//次数超过限制
		if (get_task_type_count(TASK_TYPE_ZHUOGUI) >= task_manager_t::get_max_zhuogui_num())
		{
			return false;
		}
	}

	//检测背包空间
	uint32_t access_item_id = p_conf->access_item().first;
	uint32_t access_item_count = p_conf->access_item().second;
	errcode_enum::error_code ec = errcode_enum::error_ok;
	if (type != ACCEPT_TASK_BY_GMCMD)
	{
		if (!can_accept_by_type(id, type))
		{
			//log_error("task owner[%lu] can_accept_by_type[%d]", m_role_uid, id);
			return false;
		}

		if (access_item_id > 0)
		{
			if (type == ACCEPT_TASK_BY_MANUAL)
			{
				if (!item_manager_t::check_add_item(p_role, access_item_id, access_item_count))
				{
					log_error("role[%lu] check_add_item failed", p_role->get_uid());
					proto::client::gc_accept_task_reply reply;
					reply.set_reply_code(errcode_enum::task_error_accept_failed);
					p_role->send_msg_to_client(op_cmd::gc_accept_task_reply, reply);
					return false;
				}
			}
			else if (type == ACCEPT_TASK_BY_SYSTEM)
			{
				if (!item_manager_t::check_add_item(p_role, access_item_id, access_item_count))
				{
					ec = errcode_enum::task_bag_full_mail;
				}
			}
		}
	}
	
	task_ptr p_new_task;
	if (is_circle_task(task_type))
	{
		task_ptr p_circle_task = get_circle_task(task_type);
		if (NULL == p_circle_task)
		{
			log_error("NULL == p_circle_task[%d]", task_type);
			return false;
		}
		p_new_task = p_circle_task;
		p_new_task->set_tid(id);
		if (task_type == TASK_TYPE_SHILIAN)
		{
			uint32_t index = m_shilian_count % s_shilian_seq.size();
			if (index == 0)
			{
				make_seq();
			}
			uint32_t star = m_shilian_seq[index];
			if (0 == star)
			{
				log_error("0 == star  role[%lu]", p_role->get_uid());
				return false;
			}
			m_shilian_count += 1;
			save_self();
			p_new_task->set_star(star);
		}
	}
	else if (task_type == TASK_TYPE_TREASURE)
	{
		task_ptr p_treasure_task = get_circle_task(task_type);
		if (NULL == p_treasure_task)
		{
			log_error("NULL == p_treasure_task[%d]", task_type);
			return false;
		}
		p_new_task = p_treasure_task;
		p_new_task->set_tid(id);
	}
	else
	{
		p_new_task = get_task(id);
		if (NULL == p_new_task)
		{
			uint64_t uid = gen_task_uid();
			p_new_task.reset(new task_t(uid, id));
		}
	}
	p_new_task->set_type(task_type);
	p_new_task->set_accept_level(p_role->get_level());
	p_new_task->set_progress("");
	p_new_task->clear_progress();
	p_new_task->change_state(TASK_STATE_DOING);
	task_target_type target_type = (task_target_type)p_conf->get_target_type();
	switch (target_type)
	{
	case TASK_TARGET_TYPE_KILL_MONSTER:
		break;
	case TASK_TARGET_TYPE_DIALOGUE:
		{
			if (p_conf->complete_cond().size() == 1)
			{
				p_new_task->change_state(TASK_STATE_CAN_FINISH);
				task_target* target = p_conf->get_target(0);
				if (NULL != target)
				{
					uint32_t target_id = std::get<1>(*target);
					p_new_task->advance(TASK_TARGET_TYPE_DIALOGUE, target_id, 1, p_role);
				}
			}
		}
		break;
	case TASK_TARGET_TYPE_COLLECT:
		break;
	case TASK_TARGET_TYPE_RUN:
		break;
	case TASK_TARGET_TYPE_USE_PROP:
		break;
	case TASK_TARGET_TYPE_LEVEL:
		p_new_task->advance(TASK_TARGET_TYPE_LEVEL, 1, p_role->get_level(), p_role, false);
		break;
	case TASK_TARGET_TYPE_KILL_MONSTER_COLLECT:
		break;
	case TASK_TARGET_TYPE_COMMIT_PROP:
		break;
	case TASK_TARGET_TYPE_KILL_USER:
		break;
	case TASK_TARGET_TYPE_CHOISE:
		break;
	case TASK_TARGET_TYPE_KILL_MONSTER2:
		break;
	case TASK_TARGET_TYPE_KILL_MONSTER3:
		break;
	case TASK_TARGET_TYPE_JOIN_COUNTRY:
		if (p_role->get_country_id() > 0)
		{
			p_new_task->advance(TASK_TARGET_TYPE_JOIN_COUNTRY, 1, 1, p_role);
		}
		break;
	case TASK_TARGET_TYPE_RECRUIT:
		{
			if (p_conf->complete_cond().size() == 1)
			{
				task_target* target = p_conf->get_target(0);
				if (NULL != target)
				{
					uint32_t target_id = std::get<1>(*target);
					if (hero_manager_t::get_hero(p_role, target_id) != NULL)
					{
						p_new_task->advance(TASK_TARGET_TYPE_RECRUIT, target_id, 1, p_role);
					}
				}
			}
		}
		break;
	case TASK_TARGET_TYPE_JOIN_FAMILY:
	{
		if (p_role->get_family_id() > 0)
		{
			p_new_task->advance(TASK_TARGET_TYPE_JOIN_FAMILY, 1, 1, p_role);
		}
	}
	break;
	case TASK_TARGET_TYPE_HERO_FORM:
	{
		uint32_t hero_count = p_role->m_formation_mgr.get_cur_hero_count();
		p_new_task->advance(TASK_TARGET_TYPE_HERO_FORM, 1, hero_count, p_role, false);
	}
	break;
	case TASK_TARGET_TYPE_HERO_EQUIP:
	{
		uint32_t hero_count = hero_manager_t::get_hero_full_equip_count(p_role);
		p_new_task->advance(TASK_TARGET_TYPE_HERO_EQUIP, 1, hero_count, p_role, false);
	}
	break;
	case TASK_TARGET_TYPE_EQUIP_STRENGTHEN_5:
	{
		uint32_t equip_count = item_manager_t::get_equip_strengthen_count(p_role, 5);
		p_new_task->advance(TASK_TARGET_TYPE_EQUIP_STRENGTHEN_5, 1, equip_count, p_role, false);
	}
	break;
	case TASK_TARGET_TYPE_EQUIP_STRENGTHEN_10:
	{
		uint32_t equip_count = item_manager_t::get_equip_strengthen_count(p_role, 10);
		p_new_task->advance(TASK_TARGET_TYPE_EQUIP_STRENGTHEN_10, 1, equip_count, p_role, false);
	}
	break;
	default:
		break;
	}

	TASK_LOG("role[%lu] active task[%d]", p_role->get_uid(), id);
	
	add_task(p_new_task);
	p_new_task->save_self(p_role->get_uid(), true);
	p_role->script_event_task_accept(id);

	if (access_item_id > 0)
	{
		if (ec == errcode_enum::error_ok)
		{
			item_manager_t::add_item(p_role, access_item_id, access_item_count, log_enum::source_type_interior_task_active_item, id, &rcd);
		}
		else
		{
			std::map<uint32_t, uint32_t> items_id_count;
			items_id_count[access_item_id] = access_item_count;
			mail_manager_t::send_mail(p_role->get_uid(), proto::common::MAIL_TYPE_SYS, 7, items_id_count);
		}
	}
	
	if (is_notify)
		notify_client_taskstate<proto::client::gc_accept_task_reply>(p_role, p_new_task, op_cmd::gc_accept_task_reply, rcd, ec);

	if (p_new_task->can_finished())
	{
		p_new_task->change_state(TASK_STATE_CAN_FINISH);
		if (p_conf->auto_complete() == true)
		{
			finish_task(id, rcd);
		}
		else
		{
			if (is_notify)
			{
				std::vector<task_ptr> tasks;
				tasks.push_back(p_new_task);
				notify_client_tasklist_changed(tasks);
			}
		}
	}
	p_role->get_personal_info(rcd.mutable_per_info());
	return true;
}

bool task_manager_t::can_accept_by_type(uint32_t task_tid, accept_task_type type)
{
	auto p_role = role_manager_t::find_role(m_role_uid);
	if (NULL == p_role)
	{
		log_error("task owner[%lu] not valid", m_role_uid);
		return false;
	}

	if (is_activated(task_tid))
	{
		//log_error("task owner[%lu] check task[%d] active failed", m_role_uid, task_tid);
		return false;
	}

	auto p_conf = GET_CONF(Task, task_tid);
	if (NULL == p_conf)
	{
		log_error("task tid[%u] invalid", task_tid);
		return false;
	}

	if (!check_task_type_count((task_type)p_conf->type()))
	{
		//log_error("task type[%u] count limit", conf->type());
		return false;
	}

	accept_task_type accept_type = p_conf->accept_npc() == 0 ? ACCEPT_TASK_BY_SYSTEM : ACCEPT_TASK_BY_MANUAL;
	if (accept_type != type)
	{
		//log_error("task[%d] accept_type check failed", task_tid);
		return false;
	}

	for (auto pre_task : p_conf->pre_task())
	{
		if (0 != pre_task && !is_done(pre_task))
		{
			//log_error("task owner[%lu] check task[%d] pre_task[%d] failed", m_role_uid, task_tid, pre_task);
			return false;
		}
	}

	if (p_conf->level() != 0 && p_conf->level() > p_role->get_level())
	{
		return false;
	}
	for (auto cond : p_conf->accept_cond())
	{
		uint32_t cond_type = cond.first;
		uint32_t cond_param1 = cond.second;
		uint32_t cond_param2 = cond.third;
		//log_warn("task[%d] accept_cond[%d:%d:%d]", task_tid, cond_type, cond_param1, cond_param2);
		switch (cond_type)
		{
		case task_accept_cond_lifestar:
		{
			if (p_role->get_lifestate_level() < cond_param1)
			{
				return false;
			}
			if (p_role->get_lifestar_level() < cond_param2)
			{
				return false;
			}
		}
			break;
		case task_accept_cond_family:
		{
			if (p_role->get_family_id() == 0)
			{
				//log_error("task owner[%lu] check task[%d] get_family_id failed", m_role_uid, task_tid);
				return false;
			}
		}
		break;
		default:
			break;
		}
	}
	return true;
}

void task_manager_t::active_task_by_cond(proto::common::role_change_data& rcd, bool isNotify/* = false*/)
{
	auto p_role = role_manager_t::find_role(m_role_uid);
	if (NULL == p_role)
	{
		log_error("task owner[%lu] not valid", m_role_uid);
		return;
	}

	std::map<uint32_t, Task*> confs;
	GET_ALL_CONF(Task, confs);
	std::vector<task_ptr> task_set;

	for (auto it : confs)
	{
		auto p_conf = it.second;
		if (NULL == p_conf)
		{
			continue;
		}
		if (is_circle_task(p_conf->type()))
		{
			continue;
		}
		if (p_conf->type() == TASK_TYPE_TREASURE)
		{
			continue;
		}
		active_task_by_id(it.first, ACCEPT_TASK_BY_SYSTEM, rcd);
		//if (active_task_by_id(it.first, ACCEPT_TASK_BY_SYSTEM, rcd))
		//{
		//	task_ptr p_task_state = get_task(it.first);
		//	if (NULL != p_task_state)
		//	{
		//		task_set.push_back(p_task_state);
		//	}
		//}
	}

	if (isNotify)
	{
		notify_client_tasklist_changed(task_set, &rcd);
	}

	p_role->notify_save_data();
}

template<typename MessageType> void task_manager_t::notify_client_taskstate(role_ptr p_role, task_ptr p_task, uint16_t cmd, const proto::common::role_change_data& rcd, uint32_t reply_code)
{
    if (NULL == p_role)
    {
        log_error("task owner[%lu] not valid", m_role_uid);
        return;
    }

    if (NULL == p_task)
    {
        log_error("NULL == p_task");
        return;
    }

    MessageType msg;
	msg.set_reply_code(reply_code);
    proto::common::task_state* p_state = msg.mutable_task();
	p_task->peek_data(p_state);
    proto::common::role_change_data *p_data = msg.mutable_rcd();
    p_data->CopyFrom(rcd);

    p_role->send_msg_to_client(cmd, msg);
}

void task_manager_t::peek_all_datas(proto::common::task_data* msg)
{
	auto p_role = role_manager_t::find_role(m_role_uid);
	if (NULL == p_role)
	{
		log_error("task owner[%lu] not valid", m_role_uid);
		return;
	}

	if (msg == NULL)
		return;

	for (auto it : m_task_map)
	{
		task_ptr p_task = it.second;
		if (NULL == p_task)
		{
			continue;
		}
		if (p_task->get_state() != TASK_STATE_FINISH)
		{
			auto p_state = msg->add_tasks();
			p_task->peek_data(p_state);
		}
	}

	std::vector<uint32_t> task_can_do_manual;
	get_task_can_do_manul(task_can_do_manual);
	for (auto task_id : task_can_do_manual)
	{
		auto p_state = msg->add_tasks();
		p_state->set_id(task_id);
		p_state->set_progress("");
		p_state->set_state(TASK_STATE_NOT_ACCEPT);
	}

	// 寻访任务
	peek_recruit_task(msg);
}

void task_manager_t::peek_recruit_task(proto::common::task_data* msg)
{
	auto p_role = role_manager_t::find_role(m_role_uid);
	if (NULL == p_role)
	{
		log_error("task owner[%lu] not valid", m_role_uid);
		return;
	}

	if (msg == NULL)
		return;

	std::vector<uint32_t> hero_tids;
	cultivate_manager_t::get_unrecruit_hero(p_role, hero_tids);
	for (auto hero_tid : hero_tids)
	{
		auto p_recruit_conf = GET_CONF(HeroGet, hero_tid);
		if (p_recruit_conf == NULL)
		{
			continue;
		}
		const std::vector<uint32_t>& visit_quest_list = p_recruit_conf->visit_quest();
		for (auto task_id : visit_quest_list)
		{
			task_ptr p_task = get_task(task_id);
			if (NULL == p_task)
			{
				continue;
			}

			if (TASK_STATE_FINISH != p_task->get_state())
			{
				continue;
			}

			auto p_state = msg->add_tasks();
			p_task->peek_data(p_state);
		}
	}
}

void task_manager_t::notify_client_tasklist_changed(std::vector<task_ptr>& task_changed, proto::common::role_change_data* rcd)
{
	auto p_role = role_manager_t::find_role(m_role_uid);
	if (NULL == p_role)
	{
		log_error("task owner[%lu] not valid", m_role_uid);
		return;
	}

	proto::client::gc_task_change_ntf msg;
    if( NULL != rcd)
    {
        msg.mutable_rcd()->CopyFrom(*rcd);
    }
	for (auto p_task : task_changed)
	{
		if (NULL != p_task)
		{
			auto p_state = msg.add_task_changed();
			p_task->peek_data(p_state);
		}
	}
    std::vector<uint32_t> task_can_do_manual;
    get_task_can_do_manul(task_can_do_manual);
	for (auto task_id : task_can_do_manual)
	{
		auto p_state = msg.add_task_changed();
		p_state->set_id(task_id);
		p_state->set_progress("");
		p_state->set_state(TASK_STATE_NOT_ACCEPT);
	}

	p_role->send_msg_to_client(op_cmd::gc_task_change_ntf, msg);
}

void task_manager_t::reset_task_by_type(task_type type, proto::common::task_data& reply)
{
	auto p_role = role_manager_t::find_role(m_role_uid);
	if (NULL == p_role)
	{
		log_error("task owner[%lu] not valid", m_role_uid);
		return;
	}

	for (auto it : m_task_map)
	{
		auto p_conf = GET_CONF(Task, it.first);
		if (NULL == p_conf)
		{
			continue;
		}
		task_ptr p_task = it.second;
		if (NULL == p_task)
		{
			continue;
		}

		if (p_conf->type() == type)
		{
			p_task->set_progress("");
			p_task->set_state(TASK_STATE_NOT_ACCEPT);
			p_task->set_circle_count(0);
			p_task->save_self(p_role->get_uid());
			auto p_state = reply.add_tasks();
			p_task->peek_data(p_state);
		}
	}

	m_task_type_count.erase(type);

	p_role->notify_save_data();
}

void task_manager_t::do_after_finish_task(uint32_t task_id)
{
	auto p_role = role_manager_t::find_role(m_role_uid);
	if (NULL == p_role)
	{
		log_error("task owner[%lu] not valid", m_role_uid);
		return;
	}

	auto p_conf = GET_CONF(Task, task_id);
	if (NULL == p_conf)
	{
		log_error("task owner[%lu] p_conf[%d] == NULL ", m_role_uid, task_id);
		return;
	}
	proto::common::role_change_data rcd;
	if (is_circle_task(p_conf->type()))
	{
		auto p_task = get_task(task_id);
		if (NULL == p_task)
		{
			log_error("task owner[%lu] NULL == p_task[%d] ", m_role_uid, task_id);
			return;
		}
		p_task->set_tid(0);
		p_task->save_self(p_role->get_uid());
		
		task_mgr_ptr task_mgr = p_role->get_task_mgr();
		if (task_mgr == NULL)
		{
			log_error("task_mgr[%lu] is NULL", m_role_uid );
			return;
		}

		if (p_task->get_circle_count() < p_task->get_max_circle_count())
		{
			//抓鬼队员不会自动接任务，走任务通知
			if (p_task->get_type() == TASK_TYPE_ZHUOGUI)
			{
				//抓鬼任务不按环数也能结束		
				if (get_task_type_count(TASK_TYPE_ZHUOGUI) == task_manager_t::get_max_zhuogui_num())
				{
					proto::client::gc_zhuagui_task_over_reply msg;
					msg.set_count(get_task_type_count(TASK_TYPE_ZHUOGUI));
					msg.set_type(1);
					p_role->send_msg_to_client(op_cmd::gc_zhuagui_task_over_reply, msg);
				}

				if (p_role->is_leader() == false)
				{
					p_task->set_circle_count(0);
					p_task->set_max_circle_count(0);
					p_task->set_circle_id(0);
					p_task->set_star(0);
					p_task->save_self(p_role->get_uid());
					return;
				}
			}
			active_circle_task(p_task->get_type(), p_task->get_circle_id(), ACCEPT_TASK_BY_SYSTEM, rcd);
		}
		else
		{
			if (p_conf->type() == TASK_TYPE_MASTER)
			{
				auto p_act_mgr = p_role->get_activity_mgr();
				if (NULL != p_act_mgr)
				{
					p_act_mgr->activity_done(activity_jingyan);
					achieve_common_t::notify_progress_state(m_role_uid, proto::common::Achieve_Event_Type::ACHIEVE_TASK_FINISH_EXP_CIRCLE_NUMBER);
				}
				if (task_mgr->is_finish_all_circle( TASK_TYPE_MASTER, p_role->get_level(), activity_jingyan))
				{
					task_mgr->unlock_task_sd(TASK_TYPE_MASTER);
				}
			}
			else if (p_conf->type() == TASK_TYPE_SHILIAN)
			{
				auto p_act_mgr = p_role->get_activity_mgr();
				if (NULL != p_act_mgr)
				{
					p_act_mgr->activity_done(activity_shilian);
					achieve_common_t::notify_progress_state(m_role_uid, proto::common::Achieve_Event_Type::ACHIEVE_TASK_FINISH_TRIAL_NUMBER);
					///achieve_common_t::notify_progress_state(m_role_uid,proto::common::ACHIEVE_TASK_FINISH_TRIAL_NUMBER_STAR,p_task->get_star());
				}
				if (task_mgr->is_finish_all_circle( TASK_TYPE_SHILIAN , p_role->get_level(), activity_shilian))
				{
					task_mgr->unlock_task_sd(TASK_TYPE_SHILIAN);
				}
			}
			else if (p_conf->type() == TASK_TYPE_ZHUOGUI)
			{
				auto p_act_mgr = p_role->get_activity_mgr();
				if (NULL != p_act_mgr)
				{
					p_act_mgr->activity_done(activity_zhuogui);
					achieve_common_t::notify_progress_state(m_role_uid, proto::common::Achieve_Event_Type::ACHIEVE_TASK_FINISH_CHASE_GHOST_NUMBER);
				}

				if (task_mgr->is_finish_all_circle( TASK_TYPE_ZHUOGUI, p_role->get_level(), activity_zhuogui))
				{
					task_mgr->unlock_task_sd(TASK_TYPE_ZHUOGUI);
				}

				//如果满了30次则通知结束
				proto::client::gc_zhuagui_task_over_reply msg;
				msg.set_count(get_task_type_count(TASK_TYPE_ZHUOGUI));
				if (msg.count() >= task_manager_t::get_max_zhuogui_num() )
					msg.set_type(1);
				else 
					msg.set_type(0);

				if ( msg.type() == 1 ){
					p_role->send_msg_to_client(op_cmd::gc_zhuagui_task_over_reply, msg);
				}
				else if ( p_role->is_leader() )
				{
					p_role->send_msg_to_client(op_cmd::gc_zhuagui_task_over_reply, msg);
				}
			}
			else if (p_conf->type() == TASK_TYPE_BOUNTY)
			{
				bounty_mgr_ptr p_bounty_mgr = p_role->get_bounty_mgr();
				if (NULL != p_bounty_mgr)
				{
					p_bounty_mgr->finish_task(p_task->get_circle_id());
				}

				if (task_mgr->is_finish_all_circle( TASK_TYPE_BOUNTY, p_role->get_level(), activity_country_bounty))
				{
					task_mgr->unlock_task_sd(TASK_TYPE_BOUNTY);
				}
			}
			p_task->set_circle_count(0);
			p_task->set_max_circle_count(0);
			p_task->set_circle_id(0);
			p_task->set_star(0);
			p_task->save_self(p_role->get_uid());
		}
	}
	else if (p_conf->type() == TASK_TYPE_TREASURE)
	{
		auto p_task = get_task(task_id);
		if (NULL == p_task)
		{
			log_error("task owner[%lu] NULL == p_task[%d] ", m_role_uid, task_id);
			return;
		}
		p_task->set_tid(0);
		p_task->set_circle_id(0);
		p_task->save_self(p_role->get_uid());
		if (p_conf->type() == TASK_TYPE_MASTER)
		{
			auto p_act_mgr = p_role->get_activity_mgr();
			if (NULL != p_act_mgr)
			{
				p_act_mgr->activity_done(activity_treasure);
			}
		}

		achieve_common_t::notify_progress_state(m_role_uid, proto::common::Achieve_Event_Type::ACHIEVE_TREASURE_ACTIVITY_NUMBER);
	}
	else
	{
		if (p_conf->get_target_type() != TASK_TARGET_TYPE_CHOISE)
		{
			active_task_by_cond(rcd, true);
		}
	}
}

// 计算奖励
void task_manager_t::calc_reward(uint32_t task_id, proto::common::role_change_data& rcd, uint32_t reply_code, uint32_t coeff)
{
	auto p_role = role_manager_t::find_role(m_role_uid);
	if (NULL == p_role)
	{
		log_error("task owner[%lu] not valid", m_role_uid);
		return;
	}

	auto p_conf = GET_CONF(Task, task_id);
	if (NULL != p_conf)
	{
		for (auto it : p_conf->coin())
		{
			uint32_t coin_type = it.first;
			uint32_t coin_count = it.second * coeff;
			if (coin_count > 0)
			{
				money_manager_t::add_money(p_role, proto::common::MONEY_TYPE(coin_type), coin_count, log_enum::source_type_task_finish, task_id);
			}
		}
		
		if (p_conf->itemtype() == 1)
		{
			//全部获取
			std::map<uint32_t, uint32_t> items_id_count;
			for (auto it : p_conf->item())
			{
				auto item_id = it.first;
				auto item_count = it.second;
				items_id_count[item_id] = item_count;
				TASK_LOG("role[%lu] finish task[%d] reward item[%d] count[%d]", p_role->get_uid(), task_id, item_id, item_count);
			}
			if (reply_code == 0)
			{
				item_manager_t::add_items(p_role, items_id_count, log_enum::source_type_task_finish, task_id, &rcd);
			}
			else
			{
				mail_manager_t::send_mail(p_role->get_uid(), proto::common::MAIL_TYPE_SYS, 7, items_id_count);
			}
		}
		else if(p_conf->itemtype() == 3)
		{
			if (p_conf->item().empty())
			{
				log_error("task p_conf[%d] item invalid", task_id);
				return;
			}
			//随机获取
			int32_t rand_index = random_util_t::randMin(0, p_conf->item().size());
			if (-1 == rand_index)
			{
				log_error("task p_conf[%d] rand_max invalid", task_id);
				return;
			}
			auto item_id = p_conf->item().at(rand_index).first;
			auto item_count = p_conf->item().at(rand_index).second;
			std::map<uint32_t, uint32_t> items_id_count;
			items_id_count[item_id] = item_count;
			if (reply_code == 0)
			{
				item_manager_t::add_item(p_role, item_id, item_count, log_enum::source_type_task_finish, task_id, &rcd);
			}
			else
			{
				mail_manager_t::send_mail(p_role->get_uid(), proto::common::MAIL_TYPE_SYS, 7, items_id_count);
			}
			TASK_LOG("role[%lu] finish task[%d] reward item[%d] count[%d]", p_role->get_uid(), task_id, item_id, item_count);
		}

		if (p_conf->hero() > 0)
		{
			TASK_LOG("role[%lu] finish task[%d] addhero[%d] ", p_role->get_uid(), task_id, p_conf->hero());
			hero_manager_t::add_hero(p_role, p_conf->hero(), log_enum::source_type_task_finish, &rcd);
		}
		if (p_conf->buff() > 0)
		{
			TASK_LOG("role[%lu] finish task[%d] addbuff[%d] ", p_role->get_uid(), task_id, p_conf->buff());
			role_buff_mgr_ptr p_role_buff_mgr = p_role->get_role_buff_mgr();
			if (NULL != p_role_buff_mgr)
			{
				p_role_buff_mgr->add_role_buff(p_conf->buff(), log_enum::source_type_task_finish, task_id);
			}
		}
		if (p_conf->mail() > 0)
		{
			TASK_LOG("role[%lu] finish task[%d] sendmail[%d] ", p_role->get_uid(), task_id, p_conf->mail());
			mail_manager_t::send_mail(p_role->get_uid(), proto::common::MAIL_TYPE_SYS, p_conf->mail());
		}
		proto::common::personal_info* p_info = rcd.mutable_per_info();
		p_role->get_personal_info(p_info);
	}
}

void task_manager_t::try_finish_pay_task(uint32_t task_id, uint64_t prop_id, proto::common::role_change_data& rcd)
{
	auto p_role = role_manager_t::find_role(m_role_uid);
	if (NULL == p_role)
	{
		log_error("task owner[%lu] not valid", m_role_uid);
		return;
	}
	auto p_task = get_task(task_id);
	if (NULL == p_task)
	{
		log_error("task[%d] == NULL", task_id);
		return;
	}
	if (!p_task->can_finished())
	{
		log_error("task[%d] can not finish", task_id);
		return;
	}
	item_ptr p_item = item_manager_t::get_item_by_uid(p_role, prop_id);
	if (NULL == p_item)
	{
		log_error("NULL == p_item");
		return;
	}
	Item* p_item_conf = GET_CONF(Item, p_item->get_tid());
	if (NULL == p_item_conf)
	{
		log_error("NULL == p_item_conf[%d]", p_item->get_tid());
		return;
	}
	//if (!p_task->have_target(TASK_TARGET_TYPE_COMMIT_PROP, p_item->get_tid()))
	if (!p_task->have_target(TASK_TARGET_TYPE_COMMIT_PROP, p_item_conf->search_id()))
	{
		log_error("task[%d] target invalid", task_id);
		return;
	}
	
	uint32_t quality = p_item_conf->quality();
	if (quality >= s_prop_task_coeff.size())
	{
		log_error("item[%d] quality invalid", p_item->get_tid());
		return;
	}
	uint32_t coeff = s_prop_task_coeff[quality];
	if (errcode_enum::error_ok == item_manager_t::remove_item(p_role, prop_id, 1, log_enum::source_type_task_finish, task_id, &rcd))
	{
		finish_task(task_id, rcd, coeff);
		achieve_common_t::notify_progress_state(m_role_uid, proto::common::ACHIEVE_TASK_EXP_CIRCLE_ITEM_QUALITY_NUMBER, quality);
	}
}

bool task_manager_t::finish_choice_task(uint32_t task_id, uint32_t choice, proto::common::role_change_data& rcd)
{
	auto p_task = get_task(task_id);
	if (NULL != p_task)
	{
		p_task->change_state(TASK_STATE_CAN_FINISH);
	}
	auto ret = finish_task(task_id, rcd);
	active_task_by_id(choice, ACCEPT_TASK_BY_SYSTEM, rcd);
	return ret;
}

bool task_manager_t::finish_task(uint32_t task_id, proto::common::role_change_data& rcd, uint32_t coeff)
{
	auto p_role = role_manager_t::find_role(m_role_uid);
	if (NULL == p_role)
	{
		log_error("task owner[%lu] not valid", m_role_uid);
		return false;
	}
	auto p_conf = GET_CONF(Task, task_id);
	if (NULL == p_conf)
	{
		log_error("task[%d]  conf not found ", task_id);
		return false;
	}

	task_ptr p_task = get_task(task_id);
	if (NULL == p_task)
	{
		log_error("owner[%lu] task[%d]  not found ", m_role_uid, task_id);
		return false;
	}
	if (p_task->get_state() != TASK_STATE_CAN_FINISH)
	{
		log_error("owner[%lu] task[%d]  state[%d] invalid ", m_role_uid, task_id, p_task->get_state());
		return false;
	}
	errcode_enum::error_code ec = errcode_enum::error_ok;
	if (p_conf->itemtype() == 1)
	{
		//全部获取
		std::map<uint32_t, uint32_t> items_id_count;
		for (auto it : p_conf->item())
		{
			auto item_id = it.first;
			auto item_count = it.second;
			items_id_count[item_id] = item_count;
		}
		if (!item_manager_t::check_add_items(p_role, items_id_count))
		{
			if (p_conf->auto_complete())
			{
				ec = errcode_enum::task_bag_full_mail;
			}
			else
			{
				log_error("role[%lu] check_add_item failed", p_role->get_uid());
				proto::client::gc_commit_task_reply reply;
				reply.set_reply_code(errcode_enum::task_error_deliver_failed);
				p_role->send_msg_to_client(op_cmd::gc_commit_task_reply, reply);
				return false;
			}
		}
	}
	else if (p_conf->itemtype() == 3)
	{
		package_ptr p_main_pkg = item_manager_t::get_package(p_role);
		if (NULL == p_main_pkg)
		{
			log_error("role[%lu] NULL == p_main_pkg", p_role->get_uid());
			return false;
		}
		if (p_main_pkg->get_bag_can_use_num() < 1)
		{
			if (p_conf->auto_complete())
			{
				ec = errcode_enum::task_bag_full_mail;
			}
			else
			{
				log_error("role[%lu] check_add_item failed", p_role->get_uid());
				proto::client::gc_commit_task_reply reply;
				reply.set_reply_code(errcode_enum::task_error_deliver_failed);
				p_role->send_msg_to_client(op_cmd::gc_commit_task_reply, reply);
				return false;
			}
		}
	}
	uint32_t add_exp = 0;
	uint32_t task_type = p_conf->type();
	if (is_circle_task(task_type))
	{
		task_circle_map::iterator it = s_circle_map.find(p_task->get_accept_level());
		if (it == s_circle_map.end())
		{
			log_error("s_circle_map[level:%d] not found", p_task->get_accept_level());
			return false;
		}
		const task_circle_map_by_level& tcmbl = it->second;
		uint32_t circle_id = p_task->get_circle_id();
		uint32_t key = make_circle_key(task_type, circle_id);
		task_circle_map_by_level::const_iterator it_level = tcmbl.find(key);
		if (it_level == tcmbl.end())
		{
			log_error("task_type[%d] circle_id[%d] not found", task_type, circle_id);
			return false;
		}
		task_circle_ptr p_task_circle = it_level->second;
		if (NULL == p_task_circle)
		{
			log_error("task_type[%d] circle_id[%d] not found", task_type, circle_id);
			return false;
		}
		uint32_t circle_count = p_task->get_circle_count();
		if (circle_count <= 0)
		{
			log_error("circle_count <= 0");
			return false;
		}
		if (circle_count > p_task_circle->circle_single_vec.size())
		{
			log_error("circle_count error");
			return false;
		}
		task_circle_single_ptr p_single = p_task_circle->circle_single_vec[circle_count - 1];
		if (NULL == p_single)
		{
			log_error("NULL == p_single");
			return false;
		}
		add_exp = p_conf->exp() * p_single->exp_coeff;
		if (p_single->award > 0)
		{
			drop_manager_t::drop(p_role, p_single->award, log_enum::source_type_task_finish, proto::common::drop_sys_type_task, task_id, &rcd);
		}
		if (task_type == TASK_TYPE_SHILIAN)
		{
			uint32_t star = p_task->get_star();
			auto p_shilian_conf = GET_CONF(TaskShilian, star);
			if (NULL == p_shilian_conf)
			{
				log_error("NULL == p_shilian_conf[%d]", star);
				return false;
			}
			coeff = p_shilian_conf->coeff();
			achieve_common_t::notify_progress_state(m_role_uid, proto::common::ACHIEVE_TASK_FINISH_STAR_TRIAL_NUMBER, star);
		}else if (task_type == TASK_TYPE_ZHUOGUI)
		{
			//如果是队长额外奖励一个道具
			if (p_role->is_leader())
			{
				uint32_t drop_id = task_manager_t::get_zhuogui_leader_drop();
				if (drop_id != 0) 
				{
					drop_manager_t::drop(p_role, drop_id, log_enum::source_type_task_zhuogui_leader, proto::common::drop_sys_type_task, task_id, &rcd);
				}
			}
			achieve_common_t::notify_progress_state(m_role_uid, proto::common::ACHIEVE_CHASE_GHOST_CIRCLE_NUMBER);
		}
	}
	else
	{
		add_exp = p_conf->exp();
	}
	
	p_task->set_state(TASK_STATE_FINISH);
	p_task->save_self(p_role->get_uid());
	TASK_LOG("role[%lu] finish task[%d]", m_role_uid, task_id);
	add_task_type_count(task_type, 1);

	// 结算奖励
	if (add_exp > 0)
	{
		p_role->add_exp(add_exp * coeff, log_enum::source_type_task_finish, true, &rcd);
	}

	//增加家族资金
	if (p_conf->family_gold() != 0 && p_role->get_family_id() != 0)
	{
		role_t::add_family_money(p_role, p_conf->family_gold());
	}
	//增加国家资金
	if (p_conf->country_gold() != 0 && p_role->get_country_id() != 0) 
	{
		role_t::add_country_money(p_role, p_conf->country_gold());
	}
	calc_reward(task_id, rcd, ec, coeff);

	notify_client_taskstate<proto::client::gc_commit_task_reply>(p_role, p_task, op_cmd::gc_commit_task_reply, rcd, ec);
	p_role->script_event_task_finish(task_id);
	do_after_finish_task(task_id);

	log_wrapper_t::send_task_log(m_role_uid, p_task->get_tid(), p_task->get_state(), p_task->get_progress(),
		p_task->get_accept_time(), log_enum::source_type_task_finish);


	achieve_common_t::notify_progress_state(m_role_uid, proto::common::Achieve_Event_Type::ACHIEVE_TASK_FINISH_NUMBER);
	switch (p_conf->type())
	{
	case TASK_TYPE_FAMILY:
	{
		achieve_common_t::notify_progress_state(m_role_uid, proto::common::Achieve_Event_Type::ACHIEVE_TASK_FINISH_FAMILY_NUMBER);
	}break;
	case TASK_TYPE_COUNTRY_DAILY:
	case TASK_TYPE_COUNTRY_WEEKLY:
	{
		achieve_common_t::notify_progress_state(m_role_uid, proto::common::Achieve_Event_Type::ACHIEVE_TASK_FINISH_COUNTRY_NUMBER);
	}break;
	case TASK_TYPE_BREAK:
	{
		//检查是否是当前境界突破所需要完成的任务 
		lifeforce_manager_t::check_break_task_is_finish(m_role_uid, task_id);
	}break;
	default:
		break;
	}
	
	//if (p_conf->type() == TASK_TYPE_BREAK)
	//{
	//	//检查是否是当前境界突破所需要完成的任务 
	//	lifeforce_manager_t::check_break_task_is_finish(m_role_uid, task_id);
	//}
	///检测章节是否通关
	page_manager_t::check_page_task_status(m_role_uid, task_id);
	return true;
}

bool task_manager_t::request_commit_task(const proto::client::cg_commit_task_request& msg)
{
	TASK_LOG("role[%lu] request_commit_task[%u] param_size[%d]", m_role_uid, msg.task_id(), msg.param_size());
	auto p_role = role_manager_t::find_role(m_role_uid);
	if (NULL == p_role)
	{
		log_error("task owner[%lu] not valid", m_role_uid);
		return false;
	}

	auto p_conf = GET_CONF(Task, msg.task_id());
	if (NULL == p_conf)
	{
		return false;
	}

	proto::common::role_change_data rcd;
	if (p_conf->get_target_type() == TASK_TARGET_TYPE_COMMIT_PROP)//交付任务
	{
		uint64_t prop_id = 0;
		if (msg.param_size() > 0)
		{
			prop_id = string_util_t::string_to_uint64(msg.param(0));
		}
		
		try_finish_pay_task(msg.task_id(), prop_id, rcd);
	}
	else if (p_conf->get_target_type() == TASK_TARGET_TYPE_CHOISE)//答题任务
	{
		uint32_t choice = 0;
		if (msg.param_size() > 0)
		{
			choice = string_util_t::string_to_uint32(msg.param(0));
		}

		finish_choice_task(msg.task_id(), choice, rcd);
	}
	else if (p_conf->get_target_type() == TASK_TARGET_TYPE_KILL_MONSTER3)//boss任务
	{
		finish_task(msg.task_id(), rcd);
	}
	else
	{
		finish_task(msg.task_id(), rcd);
	}

	return true;
}

bool task_manager_t::try_kill_task_monster(uint32_t task_id)
{
	auto p_role = role_manager_t::find_role(m_role_uid);
	if (NULL == p_role)
	{
		log_error("task owner[%lu] not valid", m_role_uid);
		return false;
	}

	task_ptr p_task = get_task(task_id);
	if (NULL == p_task)
	{
		log_error("p_task null error");
		return false;
	}

	if (p_task->get_state() != TASK_STATE_DOING)
	{
		log_error("task[%u] is not TASK_STATE_DOING ", task_id);
		return false;
	}

	auto p_conf = GET_CONF(Task, task_id);
	if (NULL == p_conf)
	{
		log_error("p_conf[%u] null error", task_id);
		return false;
	}

	if ((p_conf->type() == TASK_TYPE_ZHUOGUI) && !p_role->is_leader() )
	{
		log_error("user:[%lu] is not leader, try kill zhuogui monster[%u]", p_role->get_uid(), task_id);
		return false;
	}

	if (TASK_TARGET_TYPE_KILL_MONSTER != p_conf->get_target_type())
	{
		log_error("task[%u] is not TASK_TARGET_TYPE_KILL_MONSTER ", task_id);
		return false;
	}

	uint32_t against_id = 0;
	task_target* target = p_conf->get_target(0);
	if (NULL != target)
	{
		against_id = std::get<1>(*target);
	}

	if (0 == against_id)
	{
		log_error("task[%u] have no against_id", task_id);
		return false;
	}

	proto::common::fight_param fp;
	fp.set_type(proto::common::fight_type_task);
	fight_manager_t::fight_pve(p_role, against_id, &fp);
	return true;
}


uint16_t task_manager_t::gm_finish_task(uint32_t task_id)
{
	auto p_role = role_manager_t::find_role(m_role_uid);
	if (NULL == p_role)
	{
		log_error("task owner[%lu] not valid", m_role_uid);
		return false;
	}
	task_ptr p_task = get_task(task_id);
	if (NULL == p_task)
	{
		return false;
	}
	
	p_task->set_state(TASK_STATE_FINISH);
	p_task->save_self(p_role->get_uid());
	proto::common::role_change_data rcd;
	////notify client;
	auto p_conf = GET_CONF(Task, task_id);
	if (NULL != p_conf)
	{
		p_role->add_exp(p_conf->exp(), log_enum::source_type_gm_finish_task, true);
		calc_reward(task_id, rcd);
		notify_client_taskstate<proto::client::gc_commit_task_reply>(p_role, p_task, op_cmd::gc_commit_task_reply, rcd);
		do_after_finish_task(task_id);
	}

	return true;
}

void task_manager_t::get_specified_state_tasks_by_target_type(task_state state, task_target_type target_type, std::vector<uint64_t>& task_uids, uint32_t target_id)
{
	for (auto it : m_task_map)
	{
		task_ptr p_task = it.second;
		if (NULL == p_task)
		{
			continue;
		}
		auto p_conf = GET_CONF(Task, p_task->get_tid());
		if (NULL == p_conf)
		{
			continue;
		}
		if (p_task->get_state() != state)
		{
			continue;
		}
			
		for (auto type_id_cnt : p_conf->complete_cond())
		{
			if (std::get<0>(type_id_cnt) == target_type && (std::get<1>(type_id_cnt) == target_id || std::get<1>(type_id_cnt) == 0))
			{
				if (p_conf->is_ordered())
				{
					if (p_task->is_target_before_done(target_type, std::get<1>(type_id_cnt)))
					{
						task_uids.push_back(it.first);
					}
				}
				else
				{
					task_uids.push_back(it.first);
				}

			}
		}
	}
}

void task_manager_t::try_change_task_state(task_target_type target_type, proto::common::role_change_data& rcd, uint32_t target_id, uint32_t add_count, bool is_add)
{
	auto p_role = role_manager_t::find_role(m_role_uid);
	if (NULL == p_role)
	{
		log_error("task owner[%lu] not valid", m_role_uid);
		return;
	}

	std::vector<uint64_t> task_uids;
	std::vector<task_ptr> tasks;
	get_specified_state_tasks_by_target_type(TASK_STATE_DOING, target_type, task_uids, target_id);
	if (task_uids.empty())
	{
		return;
	}

	if (add_count <= 0)
	{
		return;
	}
	for (auto task_uid : task_uids)
	{
		task_ptr p_task = get_task_by_uid(task_uid);
		if (NULL == p_task)
		{
			continue;
		}
		
		p_task->advance(target_type, target_id, add_count, p_role, is_add);

		//check is task can finished

		if (p_task->can_finished())
		{
			uint32_t task_tid = p_task->get_tid();
			auto p_conf = GET_CONF(Task, task_tid);
			p_task->change_state(TASK_STATE_CAN_FINISH);

			// 更新区域暗雷怪列表
			switch (p_conf->get_target_type())
			{
			case TASK_TARGET_TYPE_MAP_DARK_MONSTER:
			case TASK_TARGET_TYPE_SPECIAL_MONSTER:
				{
					p_role->update_area_trigger();
				}
				break;
			}

			if (NULL != p_conf && p_conf->auto_complete() == true)
			{
				finish_task(task_tid, rcd);
			}
			else
			{
				tasks.push_back(p_task);
			}
		}
		else
		{
			tasks.push_back(p_task);
		}
		p_task->save_self(p_role->get_uid());
		
	}
	////notify client task change
	notify_client_tasklist_changed(tasks);

}

void  task_manager_t::add_task(task_ptr p_task, bool by_load)
{
	if (NULL == p_task)
	{
		log_error("p_task null error");
		return;
	}

	// 新设置任务接取时间
	if (!by_load)
	{
		p_task->set_accept_time(time_util_t::now_time());

		log_wrapper_t::send_task_log(m_role_uid, p_task->get_tid(), p_task->get_state(), p_task->get_progress(), p_task->get_accept_time(),
			log_enum::source_type_task_accept);
	}

	m_task_map.insert(std::make_pair(p_task->get_uid(), p_task));

	if (p_task->get_state() == TASK_STATE_DOING)
	{
		add_task_trigger(p_task->get_tid());
	}

	uint32_t task_type = p_task->get_type();
	if (is_circle_task(task_type) || task_type == TASK_TYPE_TREASURE)
	{
		task_ptr p_circle_task = get_circle_task(task_type);
		if (NULL == p_circle_task)
		{
			m_circle_task_map.insert(std::make_pair(task_type, p_task));
		}
	}
}

void task_manager_t::drop_task(uint32_t task_id)
{
	auto p_role = role_manager_t::find_role(m_role_uid);
	if (NULL == p_role)
	{
		log_error("task owner[%lu] not valid", m_role_uid);
		return;
	}
	auto p_task = get_task(task_id);
	if (NULL == p_task)
	{
		log_error("task owner[%lu] task[%d] not found", m_role_uid, task_id);
		return;
	}
	uint32_t task_state = p_task->get_state();
	if (task_state != TASK_STATE_DOING && task_state != TASK_STATE_CAN_FINISH)
	{
		log_error("task owner[%lu] task[%d] state[%d] inivalid", m_role_uid, task_id, task_state);
		return;
	}
	auto p_conf = GET_CONF(Task, task_id);
	//system auto accept task can't be droped;
	if (NULL == p_conf)
	{
		log_error("NULL == p_conf task[%d]", task_id);
		return;
	}
	if (p_conf->type() == TASK_TYPE_BOUNTY)
	{
		bounty_mgr_ptr p_bounty_mgr = p_role->get_bounty_mgr();
		if (NULL != p_bounty_mgr)
		{
			p_bounty_mgr->drop_task(p_task->get_circle_id());
		}

		p_task->set_state(TASK_STATE_NOT_ACCEPT);
		p_task->set_tid(0);
		p_task->set_progress("");
		p_task->set_circle_id(0);
		p_task->set_circle_count(0);
		p_task->set_max_circle_count(0);
		p_task->save_self(p_role->get_uid());

		proto::client::gc_drop_task_reply msg;
		auto p_state = msg.mutable_task()->add_tasks();
		p_task->peek_data(p_state);
		p_state->set_id(task_id);
		p_role->send_msg_to_client(op_cmd::gc_drop_task_reply, msg);
	}
	else if (p_conf->type() == TASK_TYPE_TREASURE)
	{
		p_task->set_state(TASK_STATE_NOT_ACCEPT);
		p_task->set_tid(0);
		p_task->set_progress("");
		p_task->set_circle_id(0);
		p_task->save_self(p_role->get_uid());

		proto::client::gc_drop_task_reply msg;
		auto p_state = msg.mutable_task()->add_tasks();
		p_task->peek_data(p_state);
		p_state->set_id(task_id);
		p_role->send_msg_to_client(op_cmd::gc_drop_task_reply, msg);
	}
	else 
	{
		if (p_conf->accept_npc() == 0)
		{
			log_error("task owner[%lu] task[%d] can't drop", m_role_uid, task_id);
			return;
		}
		if (p_conf->drop() == 0)
		{
			log_error("task owner[%lu] task[%d] can't drop", m_role_uid, task_id);
			return;
		}
		p_task->set_state(TASK_STATE_NOT_ACCEPT);
		p_task->set_progress("");
		p_task->save_self(p_role->get_uid());
		TASK_LOG("role[%lu] drop task[%d]", m_role_uid, task_id);
		proto::client::gc_drop_task_reply msg;
		auto p_state = msg.mutable_task()->add_tasks();
		p_task->peek_data(p_state);
		p_role->send_msg_to_client(op_cmd::gc_drop_task_reply, msg);
	}
}


void task_manager_t::get_task_can_do_manul(std::vector<uint32_t>& task_can_do_manual)
{
    task_can_do_manual.clear();

	struct ExecTask : public execEntry< Task >
	{
		task_mgr_ptr p_task_mgr;
		std::vector<uint32_t>& task_vec;
		ExecTask(task_mgr_ptr p_mgr, std::vector<uint32_t>& vec) : p_task_mgr(p_mgr), task_vec(vec) {};

		bool exec(Task* entry)
		{
			if (NULL == p_task_mgr)
			{
				return false;
			}
			if (NULL == entry)
			{
				return true;
			}
			if (is_circle_task(entry->type()))
			{
				return true;
			}
			task_ptr p_task = p_task_mgr->get_task(entry->id());
			if (NULL != p_task)
			{
				return true;
			}
			if (p_task_mgr->can_accept_by_type(entry->id(), ACCEPT_TASK_BY_MANUAL))
			{
				task_vec.push_back(entry->id());
			}

			return true;
		}

	}exec(shared_from_this(), task_can_do_manual);
	if (NULL == TaskManager::getInstancePtr())
	{
		log_error("NULL == TaskManager::getInstancePtr()");
		return;
	}
	TaskManager::getInstancePtr()->execEveryData(exec);
}

void task_manager_t::update_state_to_db(uint32_t task_id)
{
	task_ptr p_task = get_task(task_id);
	if (NULL != p_task)
	{
		p_task->save_self(m_role_uid, true);
	}
}

void task_manager_t::load_all_task_data(const proto::common::task_data& task_data)
{
	auto p_role = role_manager_t::find_role(m_role_uid);
	if (NULL == p_role)
	{
		log_error("task owner[%lu] not valid", m_role_uid);
		return;
	}

	for (auto task : task_data.tasks())
	{
		task_ptr p_task = task_ptr(new task_t());
		p_task->load_data(task);
		add_task(p_task, true);
	}
	m_shilian_seq.clear();
	m_shilian_count = task_data.shilian_count();
	for (int i = 0; i < task_data.shilian_seq_size(); ++i)
	{
		m_shilian_seq.push_back(task_data.shilian_seq(i));
	}

	m_circle_task_complate_list.clear();
	for (int i = 0; i < task_data.circle_task_comp_list_size(); ++i)
	{
		m_circle_task_complate_list.insert( task_data.circle_task_comp_list(i));
	}
		
}

void task_manager_t::peek_all_task_data(proto::common::task_data* task_data)
{
	if (NULL == task_data)
	{
		return;
	}
	for (auto it : m_task_map)
	{
		task_ptr p_task = it.second;
		if (NULL == p_task)
		{
			continue;
		}
		p_task->peek_data(task_data->add_tasks());
	}
	task_data->set_shilian_count(m_shilian_count);
	for (auto star : m_shilian_seq)
	{
		task_data->add_shilian_seq(star);
	}
}

void task_manager_t::save_all_task_data()
{
	for (auto it : m_task_map)
	{
		task_ptr p_task = it.second;
		if (NULL == p_task)
		{
			continue;
		}
		p_task->save_self(m_role_uid);
	}
	save_self();
}

void task_manager_t::save_self()
{
	proto::common::task_data task_data;
	task_data.set_shilian_count(m_shilian_count);
	for (auto star : m_shilian_seq)
	{
		task_data.add_shilian_seq(star);
	}

	for (auto it : m_circle_task_complate_list)
	{
		task_data.add_circle_task_comp_list(it);
	}
	role_unify_save::add_task(m_key, task_data, false, false, m_role_uid);
}

void task_manager_t::task_seek_help(uint32_t task_tid)
{
	task_ptr p_task = get_task(task_tid);
	if (NULL != p_task)
	{
		p_task->seek_help();
	}
}

// 帮助完成任务
uint32_t task_manager_t::help_finish_task(uint64_t help_role_uid, uint32_t task_tid)
{
	auto p_role = role_manager_t::find_role(m_role_uid);
	if (NULL == p_role)
	{
		log_error("task owner[%lu] not valid", m_role_uid);
		return errcode_enum::notice_role_not_exist;
	}

	task_ptr p_task = get_task(task_tid);
	if (NULL == p_task)
	{
		return errcode_enum::notice_task_id_not_exist;
	}

	if (p_task->get_state() != TASK_STATE_DOING)
	{
		return errcode_enum::notice_task_id_not_exist;
	}

	uint32_t item_tid = p_task->get_commit_item_tid();
	uint32_t item_count = p_task->get_commit_item_count();
	proto::common::role_change_data rcd;
	if (errcode_enum::error_ok != item_manager_t::remove_item(help_role_uid, item_tid, item_count, log_enum::source_type_task_help_finish, task_tid, &rcd))
	{
		log_error("[sjw task]role [%lu] remove item [%u] count [%u] fail !", help_role_uid, item_tid, item_count);
		return errcode_enum::notice_item_not_enough;
	}

	p_task->set_state(TASK_STATE_FINISH);
	p_task->save_self(p_role->get_uid());
	
	auto p_conf = GET_CONF(Task, task_tid);
	if (NULL != p_conf)
	{
		//do_after_finish_task(task_tid, rcd, help_role_uid);
        notify_client_taskstate<proto::client::gc_commit_task_reply>(p_role, p_task, op_cmd::gc_commit_task_reply, rcd);
	}

    return 0;
}

void task_manager_t::on_finish_pve(uint64_t against_id, proto::common::role_change_data& rcd)
{
	try_change_task_state(TASK_TARGET_TYPE_KILL_MONSTER, rcd, against_id, 1);
}

void task_manager_t::load_task_type_data(const proto::common::task_type_data& ttd)
{
	m_task_type_count.clear();
	for (auto single : ttd.type_count())
	{
		m_task_type_count.insert(std::make_pair(single.type(), single.count()));
	}
}

void task_manager_t::peek_task_type_data(proto::common::task_type_data* ttd)
{
	if (NULL == ttd)
	{
		return;
	}

	for (auto data : m_task_type_count)
	{
		proto::common::task_type_count* ttc = ttd->add_type_count();
		if (NULL == ttc)
		{
			continue;
		}

		ttc->set_type(data.first);
		ttc->set_count(data.second);
	}
}

void task_manager_t::one_day()
{
	auto p_role = role_manager_t::find_role(m_role_uid);
	if (NULL == p_role)
	{
		log_error("task owner[%lu] not valid", m_role_uid);
		return;
	}

	proto::common::task_data drop;
	reset_task_by_type(TASK_TYPE_DAILY, drop);
	reset_task_by_type(TASK_TYPE_FAMILY, drop);
	reset_task_by_type(TASK_TYPE_ZHUOGUI, drop);
	reset_task_by_type(TASK_TYPE_SHILIAN, drop);
	reset_task_by_type(TASK_TYPE_BOUNTY, drop);

	if (drop.tasks_size() > 0)
	{
		proto::client::gc_drop_task_reply msg;
		msg.mutable_task()->CopyFrom(drop);
		p_role->send_msg_to_client(op_cmd::gc_drop_task_reply, msg);
	}
}

void task_manager_t::one_week()
{
	auto p_role = role_manager_t::find_role(m_role_uid);
	if (NULL == p_role)
	{
		log_error("task owner[%lu] not valid", m_role_uid);
		return;
	}
	//if (NULL == m_p_circle_task)
	//{
	//	return;
	//}
	proto::common::task_data drop;
	//reset_task_by_type(TASK_TYPE_CIRCLE, drop);
	reset_task_by_type(TASK_TYPE_MASTER, drop);
	
	if (drop.tasks_size() > 0)
	{
		proto::client::gc_drop_task_reply msg;
		msg.mutable_task()->CopyFrom(drop);
		p_role->send_msg_to_client(op_cmd::gc_drop_task_reply, msg);
	}
}

void task_manager_t::add_task_type_count(uint32_t type, uint32_t count)
{
	auto p_role = role_manager_t::find_role(m_role_uid);
	if (NULL == p_role)
	{
		log_error("task owner[%lu] not valid", m_role_uid);
		return;
	}

	m_task_type_count[type] += count;

	p_role->notify_save_data();
}

uint32_t task_manager_t::get_task_type_count(uint32_t type)
{
	if (m_task_type_count.find(type) == m_task_type_count.end())
	{
		return 0;
	}

	return m_task_type_count[type];
}

uint32_t task_manager_t::get_task_type_max(uint32_t type)
{
	if (type == TASK_TYPE_FAMILY)
	{
		auto conf = GET_CONF(Comprehensive, comprehensive_common::family_task1);
		uint32_t val = GET_COMPREHENSIVE_VALUE_1(conf);
		return val;
	}

	return 0;
}

bool task_manager_t::check_task_type_count(uint32_t type)
{
	uint32_t max_count = get_task_type_max(type);
	if (max_count && get_task_type_count(type) >= max_count)
	{
		return false;
	}

	return true;
}

void task_manager_t::leave_family()
{
	auto p_role = role_manager_t::find_role(m_role_uid);
	if (NULL == p_role)
	{
		log_error("task role[%lu] not valid", m_role_uid);
		return;
	}

	proto::common::task_data drop;
	reset_task_by_type(TASK_TYPE_FAMILY, drop);

	if (drop.tasks_size() > 0)
	{
		proto::client::gc_drop_task_reply msg;
		msg.mutable_task()->CopyFrom(drop);
		p_role->send_msg_to_client(op_cmd::gc_drop_task_reply, msg);
	}
}

bool task_manager_t::make_circle_info()
{
	s_circle_map.clear();
	std::map<uint32_t, TaskCircle*> circle_map;
	GET_ALL_CONF(TaskCircle, circle_map);
    uint32_t circle_task_num = 0;
	for (auto circle : circle_map)
	{
		auto p_conf = circle.second;
		if (NULL == p_conf)
		{
			continue;
		}
		uint32_t level_range_size = p_conf->level_range().size();
		if (level_range_size == 1)
		{
			uint32_t level = p_conf->level_range().at(0);
			task_circle_map_by_level& tcmbl = s_circle_map[level];
			make_circle_task_single_level(tcmbl, p_conf);
		}
		else if (level_range_size == 2)
		{
			uint32_t min_level = p_conf->level_range().at(0);
			uint32_t max_level = p_conf->level_range().at(1);
			for (uint32_t i = min_level; i <= max_level; ++i)
			{
				task_circle_map_by_level& tcmbl = s_circle_map[i];
				make_circle_task_single_level(tcmbl, p_conf);
			}
		}
		else
		{
			log_error("make_circle_info failed, circle[%d] level_range_size invalid", p_conf->id());
			return false;
		}

        ++circle_task_num;
	}
	TASK_LOG("load circle task size: %d", circle_task_num);

	//生成快速扫荡任务信息
	std::map<uint32_t, Activities*> quick_task_map;
	GET_ALL_CONF(Activities, quick_task_map);
	for (auto it : quick_task_map)
	{
		if (NULL == it.second)
			continue;

		auto p_conf = it.second;
		if ( p_conf->quick_cost() == 0 )
			continue;

		task_manager_t::s_quick_task_list[p_conf->get_id()] = p_conf->task_type();
	}
	return true;
}

bool task_manager_t::load_circle_task(task_circle_single_ptr p_circle_single, TaskCircle* p_conf, uint32_t index)
{
	if (NULL == p_circle_single)
	{
		log_error("NULL == p_circle");
		return false;
	}
	if (NULL == p_conf)
	{
		log_error("NULL == p_conf");
		return false;
	}
	p_circle_single->type = p_conf->type();
	p_circle_single->exp_coeff = p_conf->exp_coeff();
	p_circle_single->award = p_conf->award();
	if (p_conf->task_id_style() == 1)
	{
		if (p_conf->task_id_range().size() != 2)
		{
			log_error("make_circle_info failed, circle[%d] task_id_range invalid", p_conf->id());
			return false;
		}
		uint32_t min_id = p_conf->task_id_range().at(0);
		uint32_t max_id = p_conf->task_id_range().at(1);
		if (min_id >= max_id)
		{
			log_error("make_circle_info failed, circle[%d] task_id_range invalid", p_conf->id());
			return false;
		}
		for (uint32_t i = min_id; i <= max_id; ++i)
		{
			p_circle_single->task_id_range.push_back(i);
		}
	}
	else if (p_conf->task_id_style() == 2)
	{
		if (p_conf->task_id_range().empty())
		{
			log_error("make_circle_info failed, circle[%d] task_id_range invalid", p_conf->id());
			return false;
		}
		p_circle_single->task_id_range.assign(p_conf->task_id_range().begin(), p_conf->task_id_range().end());
	}
	else if (p_conf->task_id_style() == 3)
	{
		if (index >= p_conf->task_id_range().size())
		{
			log_error("make_circle_info failed, circle[%d] task_id_range invalid", p_conf->id());
			return false;
		}
		p_circle_single->task_id_range.push_back(p_conf->task_id_range().at(index));
	}
	return true;
}

void task_manager_t::make_circle_task_single_level(task_circle_map_by_level& tcmbl, TaskCircle* p_conf)
{
	if (NULL == p_conf)
	{
		return;
	}
	uint32_t type = p_conf->type();
	uint32_t circle_id;
	if (type == TASK_TYPE_MASTER)
	{
		circle_id = 0;
	}
	else if (type == TASK_TYPE_BOUNTY)
	{
		circle_id = p_conf->id();
	}
	else if (type == TASK_TYPE_SHILIAN)
	{
		circle_id = 0;
	}
	else if (type == TASK_TYPE_ZHUOGUI)
	{
		circle_id = 0;
	}
	else
	{
		log_error("make_circle_info failed, circle[%d] circle_type invalid", p_conf->id());
		return;
	}

	task_circle_ptr p_task_circle;
	uint32_t key = make_circle_key(type, circle_id);
	task_circle_map_by_level::iterator it = tcmbl.find(key);
	if (it == tcmbl.end())
	{
		p_task_circle.reset(new task_circle_t);
		tcmbl.insert(std::make_pair(key, p_task_circle));
		p_task_circle->circle_id = circle_id;
		p_task_circle->max_circle_count = 0;
		p_task_circle->circle_single_vec.clear();
	}
	else
	{
		p_task_circle = it->second;
	}
	if (NULL == p_task_circle)
	{
		log_error("make_circle_info failed, circle[type:%d id:%d] NULL == p_task_circle", type, circle_id);
		return;
	}
	task_circle_single_vec& vec = p_task_circle->circle_single_vec;
	uint32_t circle_size = p_conf->circle_range().size();
	if (circle_size == 1)
	{
		uint32_t circle_count = p_conf->circle_range().at(0);

		task_circle_single_ptr p_new_info(new task_circle_single_t());
		p_new_info->circle_count = circle_count;
		vec.push_back(p_new_info);
		if (!load_circle_task(p_new_info, p_conf))
		{
			log_error("make_circle_info failed, circle[%d] load_task failed", p_conf->id());
			return;
		}
		if (p_task_circle->max_circle_count < circle_count)
		{
			p_task_circle->max_circle_count = circle_count;
		}
	}
	else if (circle_size == 2)
	{
		uint32_t min_circle = p_conf->circle_range().at(0);
		uint32_t max_circle = p_conf->circle_range().at(1);
		if (min_circle >= max_circle)
		{
			log_error("make_circle_info failed, circle[%d] task_id_range invalid", p_conf->id());
			return;
		}
		for (uint32_t i = min_circle; i <= max_circle; ++i)
		{
			uint32_t circle_count = i;
			task_circle_single_ptr p_new_info(new task_circle_single_t());
			p_new_info->circle_count = circle_count;
			vec.push_back(p_new_info);
			if (!load_circle_task(p_new_info, p_conf, i - min_circle))
			{
				log_error("make_circle_info failed, circle[%d] load_task failed", p_conf->id());
				return;
			}
		}
		if (p_task_circle->max_circle_count < max_circle)
		{
			p_task_circle->max_circle_count = max_circle;
		}
	}
	else
	{
		log_error("make_circle_info failed, circle[%d] circle_size invalid", p_conf->id());
		return;
	}
}

void task_manager_t::active_circle_task(uint32_t task_type, uint32_t circle_id, accept_task_type type, proto::common::role_change_data& rcd)
{
	auto p_role = role_manager_t::find_role(m_role_uid);
	if (NULL == p_role)
	{
		log_error("task owner[%lu] not valid", m_role_uid);
		return;
	}
	task_circle_map::iterator it = s_circle_map.find(p_role->get_level());
	if (it == s_circle_map.end())
	{
		log_error("s_circle_map[level:%d] not found", p_role->get_level());
		return;
	}
	const task_circle_map_by_level& tcmbl = it->second;
	uint32_t key = make_circle_key(task_type, circle_id);
	task_circle_map_by_level::const_iterator it_level = tcmbl.find(key);
	if (it_level == tcmbl.end())
	{
		log_error("circle_task[type:%d circle_id:%d] not found", task_type, circle_id);
		return;
	}
	task_circle_ptr p_task_circle = it_level->second;
	if (NULL == p_task_circle)
	{
		log_error("circle_task[type:%d circle_id:%d] not found", task_type, circle_id);
		return;
	}
	task_ptr p_circle_task = get_circle_task(task_type);
	if (NULL == p_circle_task)
	{
		uint64_t uid = gen_task_uid();
		p_circle_task.reset(new task_t(uid, 0));
		p_circle_task->set_type(task_type);
		add_task(p_circle_task);
		p_circle_task->set_circle_count(0);
		p_circle_task->set_max_circle_count(p_task_circle->max_circle_count);
		p_circle_task->set_circle_id(circle_id);
		p_circle_task->save_self(m_role_uid, true);
	}
	else
	{
		if (p_circle_task->get_max_circle_count() == 0)
		{
			p_circle_task->set_circle_count(0);
			p_circle_task->set_max_circle_count(p_task_circle->max_circle_count);
			p_circle_task->set_circle_id(circle_id);
			p_circle_task->save_self(m_role_uid);
		}
	}
	if (type == ACCEPT_TASK_BY_MANUAL)
	{
		if (p_circle_task->get_circle_count() > 0)
		{
			log_error("master task can only manual first");
			return;
		}
	}
	p_circle_task->add_circle_count();
	uint32_t task_id = make_circle_task(p_circle_task, p_task_circle);
	if (0 == task_id)
	{
		log_error("make_circle_task[type:%d circle_id:%d] failed", task_type, circle_id);
		return;
	}
	auto p_conf = GET_CONF(Task, task_id);
	if (NULL == p_conf)
	{
		log_error("NULL == p_conf[%d]", task_id);
		return;
	}

	bool is_accept_success = false;
	if (0 == p_conf->accept_npc())
	{
		is_accept_success = active_task_by_id(task_id, ACCEPT_TASK_BY_SYSTEM, rcd);
	}
	else
	{
		is_accept_success = active_task_by_id(task_id, ACCEPT_TASK_BY_MANUAL, rcd);
	}

	//抓鬼任务共享给全队
	if ( p_role->is_leader() && task_type == TASK_TYPE_ZHUOGUI && true == is_accept_success) 
	{
		game_troop_mgr_t::accept_zhuagui_task(p_role, task_id);
	}
}

bool task_manager_t::is_circle_task(uint32_t task_type)
{
	if (task_type == TASK_TYPE_MASTER)
	{
		return true;
	}
	if (task_type == TASK_TYPE_BOUNTY)
	{
		return true;
	}
	if (task_type == TASK_TYPE_SHILIAN)
	{
		return true;
	}
	if (task_type == TASK_TYPE_ZHUOGUI)
	{
		return true;
	}
	return false;
}

uint32_t task_manager_t::make_circle_key(uint32_t task_type, uint32_t circle_id)
{
	return task_type * 1000 + circle_id;
}

uint32_t task_manager_t::get_max_circle_count(uint32_t role_level, uint32_t task_type, uint32_t task_id)
{
	task_circle_map::iterator it = s_circle_map.find(role_level);
	if (it == s_circle_map.end())
	{
		log_error("s_circle_map[level:%d] not found", role_level);
		return 0;
	}
	const task_circle_map_by_level& tcmbl = it->second;
	uint32_t key = make_circle_key(task_type, task_id);
	task_circle_map_by_level::const_iterator it_level = tcmbl.find(key);
	if (it_level == tcmbl.end())
	{
		return 0;
	}
	task_circle_ptr p_task_circle = it_level->second;
	if (NULL == p_task_circle)
	{
		return 0;
	}
	return p_task_circle->max_circle_count;
}

uint32_t task_manager_t::make_circle_task(task_ptr p_circle_task, task_circle_ptr p_task_circle)
{
	if (NULL == p_circle_task)
	{
		log_error("NULL == p_circle_task");
		return 0;
	}
	if (NULL == p_task_circle)
	{
		log_error("NULL == p_task_circle");
		return 0;
	}
	uint32_t circle_count = p_circle_task->get_circle_count();
	uint32_t max_circle_count = p_circle_task->get_max_circle_count();
	//log_error("circle_count[%d]  max_circle_count[%d]", circle_count, max_circle_count);
	if (circle_count == 0)
	{
		log_error("circle_count == 0");
		return 0;
	}
	if (circle_count > max_circle_count)
	{
		log_error("circle_count[%d] > max_circle_count[%d]", circle_count, max_circle_count);
		return 0;
	}
	//log_error("p_task_circle->circle_single_vec.size()[%d]", p_task_circle->circle_single_vec.size());
	if (circle_count > p_task_circle->circle_single_vec.size())
	{
		log_error("circle_count max");
		return 0;
	}
	task_circle_single_ptr p_single = p_task_circle->circle_single_vec[circle_count-1];
	if (NULL == p_single)
	{
		log_error("NULL == p_single");
		return 0;
	}
	//log_error("p_single->task_id_range.size()[%d]", p_single->task_id_range.size());
	if (p_single->task_id_range.empty())
	{
		log_error("task_id_range empty");
		return 0;
	}
	int32_t index = random_util_t::randMin(0, p_single->task_id_range.size());
	
	uint32_t task_id = p_single->task_id_range[index];

	return task_id;
}

void task_manager_t::clear_circle_info()
{
	s_circle_map.clear();
}

void task_manager_t::drop_circle_task(uint32_t task_type)
{
	if (task_type != TASK_TYPE_BOUNTY && task_type != TASK_TYPE_ZHUOGUI )
	{
		return;
	}
	task_ptr p_circle_task = get_circle_task(task_type);
	if (NULL == p_circle_task)
	{
		return;
	}

	auto p_role = role_manager_t::find_role(m_role_uid);
	if (NULL == p_role)
	{
		log_error("task owner[%lu] not valid", m_role_uid);
		return;
	}

	uint32_t del_task_id = p_circle_task->get_tid();
	p_circle_task->set_tid(0);
	p_circle_task->set_circle_id(0);
	p_circle_task->set_state(TASK_STATE_NOT_ACCEPT);
	p_circle_task->set_circle_count(0);
	p_circle_task->set_max_circle_count(0);
	p_circle_task->set_progress("");
	p_circle_task->save_self(m_role_uid);

	proto::client::gc_drop_task_reply msg;
	auto p_state = msg.mutable_task()->add_tasks();
	p_circle_task->peek_data(p_state);
	p_state->set_id( del_task_id );
	p_role->send_msg_to_client(op_cmd::gc_drop_task_reply, msg);
	TASK_LOG("role[%lu] drop_circle_task[%d]", m_role_uid, task_type);
}

void task_manager_t::active_treasure_task(uint32_t treasure_id, proto::common::role_change_data& rcd)
{
	task_ptr p_treasure_task = get_circle_task(TASK_TYPE_TREASURE);
	if (NULL == p_treasure_task)
	{
		uint64_t uid = gen_task_uid();
		p_treasure_task.reset(new task_t(uid, 0));
		p_treasure_task->set_type(TASK_TYPE_TREASURE);
		add_task(p_treasure_task);
		p_treasure_task->set_circle_id(treasure_id);
		p_treasure_task->save_self(m_role_uid, true);
	}
	else
	{
		p_treasure_task->set_circle_id(treasure_id);
		p_treasure_task->save_self(m_role_uid);
	}

	uint32_t task_id = get_treasure_task_id();
	auto p_conf = GET_CONF(Task, task_id);
	if (NULL == p_conf)
	{
		log_error("NULL == p_conf[%d]", task_id);
		return;
	}
	if (0 == p_conf->accept_npc())
	{
		active_task_by_id(task_id, ACCEPT_TASK_BY_SYSTEM, rcd);
	}
	else
	{
		active_task_by_id(task_id, ACCEPT_TASK_BY_MANUAL, rcd);
	}
}

void task_manager_t::drop_treasure_task()
{
	drop_task(get_treasure_task_id());
}

uint32_t task_manager_t::get_treasure_task_id()
{
	auto treasure_task = GET_CONF(Comprehensive, comprehensive_common::treasure_task_id);
	uint32_t val = GET_COMPREHENSIVE_VALUE_1(treasure_task);
	return val;
}

bool task_manager_t::make_prop_task_coeff()
{
	auto p_prop_task_coeff = GET_CONF(Comprehensive, comprehensive_common::task_quality_reward);
	if (NULL == p_prop_task_coeff)
	{
		log_error("prop_task_coeff null error");
		return false;
	}
	Dawn::parseMapContainer<uint32_t, uint32_t>(s_prop_task_coeff, p_prop_task_coeff->parameter1().c_str(), ":$");
	if (s_prop_task_coeff.empty())
	{
		log_error("prop_task_coeff null error");
		return false;
	}
	return true;
}

bool task_manager_t::init_conf()
{
	if (!make_prop_task_coeff())
	{
		return false;
	}
	if (!make_shilian_seq())
	{
		return false;
	}
	return true;
}

bool task_manager_t::make_shilian_seq()
{
	std::map<uint32_t, TaskShilian*> shilian_map;
	GET_ALL_CONF(TaskShilian, shilian_map);
	
	for (auto shilian : shilian_map)
	{
		auto p_conf = shilian.second;
		if (NULL == p_conf)
		{
			continue;
		}
		uint32_t star = p_conf->id();
		uint32_t count = p_conf->rate();
		s_shilian_seq.insert(s_shilian_seq.end(), count, star);
	}
	if (s_shilian_seq.empty())
	{
		log_error("s_shilian_seq.empty");
		return false;
	}
	return true;
}

void task_manager_t::make_seq()
{
	m_shilian_seq.clear();
	m_shilian_seq.assign(s_shilian_seq.begin(), s_shilian_seq.end());
	std::random_shuffle(m_shilian_seq.begin(), m_shilian_seq.end());
}

uint32_t task_manager_t::refresh_shilian(proto::common::role_change_data* p_data)
{
	if (NULL == p_data)
	{
		log_error("NULL == p_data");
		return errcode_enum::notice_unknown;
	}
	auto p_task = get_circle_task(TASK_TYPE_SHILIAN);
	if (NULL == p_task)
	{
		log_error("NULL == p_task shilian");
		return errcode_enum::notice_task_not_exist;
	}
	auto p_role = role_manager_t::find_role(m_role_uid);
	if (NULL == p_role)
	{
		log_error("task owner[%lu] not valid", m_role_uid);
		return errcode_enum::notice_role_null;
	}
	if (errcode_enum::error_ok != item_manager_t::remove_item(p_role, get_shilian_refresh_item(), 1, log_enum::source_type_task_refresh_shilian, 0, p_data))
	{
		log_error("task owner[%lu] refresh shilian item not enough", m_role_uid);
		return errcode_enum::notice_item_not_enough;
	}
	uint32_t index = m_shilian_count % s_shilian_seq.size();
	if (index == 0)
	{
		make_seq();
	}
	uint32_t star = m_shilian_seq[index];
	if (0 == star)
	{
		log_error("0 == star  role[%lu]", p_role->get_uid());
		return errcode_enum::notice_unknown;
	}
	m_shilian_count += 1;
	save_self();
	p_task->set_star(star);
	p_task->save_self(m_role_uid);
	proto::common::personal_info* p_info = p_data->mutable_per_info();
	p_role->get_personal_info(p_info);
	//通知客户端
	proto::client::gc_task_change_ntf ntf;
	proto::common::task_state* p_task_state = ntf.add_task_changed();
	p_task->peek_data(p_task_state);
	p_role->send_msg_to_client(op_cmd::gc_task_change_ntf, ntf);

	return errcode_enum::error_ok;
}

uint32_t task_manager_t::get_shilian_refresh_item()
{
	auto shilian_refresh_item = GET_CONF(Comprehensive, comprehensive_common::trial_task_renovat_item);
	uint32_t val = GET_COMPREHENSIVE_VALUE_1(shilian_refresh_item);
	return val;
}

void task_manager_t::add_task_trigger(uint32_t task_id)
{
	auto p_conf = GET_CONF(Task, task_id);
	if (NULL == p_conf)
	{
		log_error("task[%u] is null error", task_id);
		return;
	}

	switch (p_conf->get_target_type())
	{
	case TASK_TARGET_TYPE_MAP_DARK_MONSTER:
		{
			role_ptr p_role = role_manager_t::find_role(m_role_uid);
			if (NULL == p_role)
			{
				log_error("task owner[%lu] not valid", m_role_uid);
				break;
			}

			task_target* target = p_conf->get_target(0);
			if (NULL == target)
			{
				log_error("owner[%lu] task[%u] target not find", m_role_uid, task_id);
				break;
			}

			if (std::get<1>(*target) == p_role->get_map_tid())
			{
				p_role->update_area_trigger();
			}
		}
		break;
	case TASK_TARGET_TYPE_SPECIAL_MONSTER:
		{
			role_ptr p_role = role_manager_t::find_role(m_role_uid);
			if (NULL != p_role)
			{
				p_role->update_area_trigger();
			}
		}
		break;
	default:
		break;
	}
}

bool task_manager_t::is_doing2(uint32_t id)
{
	task_ptr p_task = get_task(id);
	if (NULL == p_task)
	{
		return false;
	}

	if (p_task->get_state() == TASK_STATE_DOING)
		return true;

	return false;
}

void task_manager_t::active_circle_task_by_sync(proto::common::task_state &task_info, proto::common::role_change_data &rcd)
{
	auto p_role = role_manager_t::find_role(m_role_uid);
	if (NULL == p_role)
	{
		log_error("task owner[%lu] not valid", m_role_uid);
		return;
	}
	p_role->accept_sync_task(task_info, rcd);
}

void task_manager_t::del_circle_task(uint32_t task_type) 
{
	task_ptr p_circle_task = get_circle_task(task_type);
	if (NULL == p_circle_task)
	{
		return;
	}

	auto p_role = role_manager_t::find_role(m_role_uid);
	if (NULL == p_role)
	{
		log_error("task owner[%lu] not valid", m_role_uid);
		return;
	}

	uint32_t del_task_id = p_circle_task->get_tid();
	p_circle_task->set_tid(0);
	p_circle_task->set_circle_id(0);
	p_circle_task->set_state(TASK_STATE_NOT_ACCEPT);
	p_circle_task->set_circle_count(0);
	p_circle_task->set_max_circle_count(0);
	p_circle_task->set_progress("");
	p_circle_task->save_self(m_role_uid);

	proto::client::gc_del_task msg;
	msg.set_task_id(del_task_id);
	p_role->send_msg_to_client(op_cmd::gc_del_task, msg);
}

uint32_t task_manager_t::get_max_zhuogui_num()
{
	auto p_com_conf = GET_CONF(Comprehensive, comprehensive_common::zhuogui_task_times );
	return GET_COMPREHENSIVE_VALUE_1(p_com_conf);
}

uint32_t task_manager_t::get_zhuogui_leader_drop() 
{
	auto p_drop_rate_conf = GET_CONF(Comprehensive, comprehensive_common::zhuogui_task_drop_probability);
	uint32_t drop_rate = GET_COMPREHENSIVE_VALUE_1(p_drop_rate_conf);

	if (random_util_t::selectByPercent(drop_rate)) 
	{
		auto p_drop_id_conf = GET_CONF(Comprehensive, comprehensive_common::zhuogui_task_drop_id);
		return  GET_COMPREHENSIVE_VALUE_1(p_drop_id_conf);
	}
	return 0;
}


//获取可以一键完成的任务列表
uint32_t task_manager_t::get_quick_task_info(proto::client::gc_get_quick_task_info_reply &msg)
{
	//纯内用
	enum {
		quick_task_lock = 0,	//未解锁
		quick_task_can  = 1,	//可完成
		quick_task_has  = 2,	//已经完成
	};

	role_ptr p_role = role_manager_t::find_role(m_role_uid);
	if( NULL == p_role )
	{ 
		log_error("user:%lu p_role is null", m_role_uid);
		return errcode_enum::notice_unknown;
	}

	activity_mgr_ptr p_act_mgr = p_role->get_activity_mgr();
	if (NULL == p_act_mgr)
	{
		log_error("NULL == p_act_mgr[%lu]", m_role_uid);
		return errcode_enum::notice_unknown;
	}

	task_mgr_ptr task_mgr = p_role->get_task_mgr();
	for ( auto it : task_manager_t::s_quick_task_list )
	{
		auto p_conf = GET_CONF(Activities, it.first);
		if (NULL == p_conf)
		{
			log_error("role[%lu] Activites config not found", m_role_uid );
			return errcode_enum::notice_unknown;
		}

		auto p_task_info = msg.add_task_list();
		p_task_info->set_id(it.first);

		uint32_t task_has_count = get_circle_task_num( it.second );
		uint32_t max_circle_count = task_manager_t::get_total_circle_count(p_role->get_level(), it.second, 0, it.first);
		
		if ( !task_mgr->is_task_can_sd(it.second) )
		{
			p_task_info->set_status(quick_task_lock);
			p_task_info->set_num(0);
			p_task_info->set_max_num(max_circle_count);
		}else if (task_has_count >= max_circle_count ) 
		{
			p_task_info->set_status(quick_task_has);
			p_task_info->set_num(0);
			p_task_info->set_max_num(max_circle_count);
		} else 
		{
			p_task_info->set_status(quick_task_can);
			p_task_info->set_num( max_circle_count > task_has_count ? max_circle_count - task_has_count : 0 );
			p_task_info->set_max_num(max_circle_count);
		}
	}
	return 	errcode_enum::error_ok;
}

//一键完成的任务列表
uint32_t task_manager_t::complate_quick_task(role_ptr p_role, proto::client::cg_complate_quick_task_request &msg,  proto::common::role_change_data &rcd)
{
	if (NULL == p_role)
	{
		log_error(" NULL == p_role ");
		return errcode_enum::notice_unknown;
	}
	
	task_mgr_ptr task_mgr = p_role->get_task_mgr();
	if (task_mgr == NULL)
	{
		log_error( "user[%lu] task_mgr is NULL ", p_role->get_uid() );
		return errcode_enum::notice_unknown;
	}

	//判断道具消耗是否足够
	uint32_t max_count = 0;
	for (int32_t i = 0; i < msg.task_list_size(); ++i)
	{ 
		uint32_t id = msg.task_list(i);
		auto iter_f = task_manager_t::s_quick_task_list.find(id);
		if (iter_f == task_manager_t::s_quick_task_list.end())
		{
			log_error("user:[%lu] active_id [%u] not found", p_role->get_uid(), id );
			return errcode_enum::notice_unknown;
		}

		auto p_conf = GET_CONF(Activities, id);
		if (NULL == p_conf)
		{
			log_error("role[%lu] Activites config not found", p_role->get_uid());
			return errcode_enum::notice_unknown;
		}

		uint32_t task_count = task_mgr->get_circle_task_num(iter_f->second);
		uint32_t max_circle_count = task_manager_t::get_total_circle_count(p_role->get_level(), iter_f->second, 0, id);
		uint32_t timeleft = max_circle_count > task_count ? max_circle_count - task_count : 0;
		if (timeleft == 0)
		{
			log_error("role[%lu] count left = 0", p_role->get_uid());
			return errcode_enum::notice_unknown;
		}
		max_count += ( p_conf->quick_cost() );
	}

	auto conf = GET_CONF(Comprehensive, comprehensive_common::daily_task_finish_item_id);
	uint32_t cost_id = GET_COMPREHENSIVE_VALUE_1(conf);

	if (errcode_enum::error_ok != item_manager_t::remove_item(p_role->get_uid(), cost_id, max_count, log_enum::source_type_task_quick_finish, 0, &rcd))
	{
		log_error("user:[%lu] item[%u]:[%u] not enough", p_role->get_uid(), cost_id, max_count );
		return errcode_enum::notice_item_not_enough;
	}

	auto p_act_mgr = p_role->get_activity_mgr();
	for (int32_t i = 0; i < msg.task_list_size(); ++i)
	{
		auto iter_f = task_manager_t::s_quick_task_list.find(msg.task_list(i));
		if (iter_f == task_manager_t::s_quick_task_list.end())
		{
			log_error( "user [%lu], quick finsh task, act_id[%d] not found", p_role->get_uid(), msg.task_list(i));
			return errcode_enum::notice_unknown;
		}

		//随机一任务
		uint32_t task_id = task_manager_t::rand_circle_task_id(p_role, iter_f->second);
		if (task_id == 0) 
		{
			log_error( "user [%lu], quick finsh task, task_type[%d] not found", p_role->get_uid(), iter_f->second);
			return errcode_enum::notice_unknown;
		}

		//删除现有的任务
		task_mgr->del_circle_task(iter_f->second);

		//添加任务奖励
		uint32_t id = msg.task_list(i);
		uint32_t task_count = task_mgr->get_circle_task_num(iter_f->second);
		uint32_t max_circle_count = task_manager_t::get_total_circle_count(p_role->get_level(), iter_f->second, 0, id);
		uint32_t timeleft = max_circle_count > task_count ? max_circle_count - task_count : 0;
		for (uint32_t n = 0; n < timeleft; ++n)
		{
			task_mgr->quick_finish_task(task_id, rcd);
		}

		//设置活动完成
		auto p_conf = GET_CONF(Activities, id);
		if (NULL == p_conf)
		{
			log_error("role[%lu] Activites config not found", p_role->get_uid());
			return errcode_enum::notice_unknown;
		}
		uint32_t max_times = p_conf->times() != 0 ? p_conf->times() : 1;
		for (uint32_t n = 0;  n < (timeleft/max_times + 1) ; n++)
		{
			p_act_mgr->activity_done(id);
				
		}
	}

	proto::client::gc_get_quick_task_info_reply reply;
	uint32_t reply_code = task_mgr->get_quick_task_info(reply);
	reply.set_reply_code(reply_code);
	p_role->send_msg_to_client(op_cmd::gc_get_quick_task_info_reply, reply);

	return errcode_enum::error_ok;
}

//存储完成过的任务环数
bool task_manager_t::save_circle_complate()
{
	return true;
}


//获取环任务剩余次数
uint32_t task_manager_t::get_circle_task_num(uint32_t task_type)
{
	auto iter_f = m_task_type_count.find(task_type);
	if (iter_f == m_task_type_count.end() )
		return 0;

	return iter_f->second;
}


//随机获取一个环任务id
uint32_t task_manager_t::rand_circle_task_id(role_ptr p_role, uint32_t task_type)
{
	if (NULL == p_role)
	{
		log_error("p_role == NULL");
		return 0;
	}


	uint32_t circle_task_id = 0;
	if ( task_type == TASK_TYPE_BOUNTY )
	{
		bounty_mgr_ptr bounty_ptr = p_role->get_bounty_mgr();
		if (NULL == bounty_ptr)
		{
			log_error( "p_role[%lu] rand task_id 0", p_role->get_uid() );
			return 0;
		}
		circle_task_id = bounty_ptr->rand_task_id(p_role->get_level());
	}

	task_circle_map::iterator it = task_manager_t::s_circle_map.find(p_role->get_level());
	if (it == s_circle_map.end())
	{
		log_error("s_circle_map[level:%d] not found", p_role->get_level());
		return 0;
	}
	const task_circle_map_by_level& tcmbl = it->second;
	uint32_t key = make_circle_key(task_type, circle_task_id);
	task_circle_map_by_level::const_iterator it_level = tcmbl.find(key);
	if (it_level == tcmbl.end())
	{
		log_error("circle_task[type:%d circle_id:%d] not found", task_type, 0);
		return 0;
	}
	task_circle_ptr p_task_circle = it_level->second;
	if (NULL == p_task_circle)
	{
		log_error("circle_task[type:%d circle_id:%d] not found", task_type, 0);
		return 0;
	}

	//随机取第一个任务
	task_circle_single_ptr p_single = p_task_circle->circle_single_vec[0];
	if (NULL == p_single)
	{
		log_error("NULL == p_single");
		return 0;
	}

	if (p_single->task_id_range.empty())
	{
		log_error("task_id_range empty");
		return 0;
	}
	int32_t index = random_util_t::randMin(0, p_single->task_id_range.size());

	uint32_t task_id = p_single->task_id_range[index];

	return task_id;
}

bool task_manager_t::quick_finish_task(uint32_t task_id, proto::common::role_change_data& rcd, uint32_t coeff)
{
	auto p_role = role_manager_t::find_role(m_role_uid);
	if (NULL == p_role)
	{
		log_error("task owner[%lu] not valid", m_role_uid);
		return false;
	}
	auto p_conf = GET_CONF(Task, task_id);
	if (NULL == p_conf)
	{
		log_error("task[%d]  conf not found ", task_id);
		return false;
	}

	if (!task_manager_t::is_circle_task( p_conf->type() ))
	{
		log_error("user[%lu] quick finish task[%d] is not circle_task", m_role_uid, task_id);
		return false;
	}

	errcode_enum::error_code ec = errcode_enum::error_ok;

	uint32_t add_exp = 0;
	uint32_t task_type = p_conf->type();
	if (is_circle_task(task_type))
	{
		task_circle_map::iterator it = s_circle_map.find(p_role->get_level());
		if (it == s_circle_map.end())
		{
			log_error("s_circle_map[level:%d] not found", p_role->get_level());
			return false;
		}
		const task_circle_map_by_level& tcmbl = it->second;
		uint32_t circle_id = 0;
		if (task_type == TASK_TYPE_BOUNTY)
		{
			bounty_mgr_ptr bounty_ptr = p_role->get_bounty_mgr();
			if (NULL == bounty_ptr)
			{
				log_error("p_role[%lu] rand task_id 0", p_role->get_uid());
				return 0;
			}
			circle_id = bounty_ptr->rand_task_id(p_role->get_level());
		}

		uint32_t key = make_circle_key(task_type, circle_id);
		task_circle_map_by_level::const_iterator it_level = tcmbl.find(key);
		if (it_level == tcmbl.end())
		{
			log_error("task_type[%d] circle_id[%d] not found", task_type, circle_id);
			return false;
		}
		task_circle_ptr p_task_circle = it_level->second;
		if (NULL == p_task_circle)
		{
			log_error("task_type[%d] circle_id[%d] not found", task_type, circle_id);
			return false;
		}

		uint32_t max_circle_count = 1;

		if (task_type == TASK_TYPE_BOUNTY)
		{
			//auto p_conf = GET_CONF(Activities, it.first);
			max_circle_count = 5;
		}
		else 
		{
			max_circle_count = task_manager_t::get_max_circle_count(p_role->get_level(), task_type, 0);
		}

		uint32_t circle_count = get_task_type_count(task_type) / max_circle_count;
		if (circle_count >= p_task_circle->circle_single_vec.size())
		{
			log_error("circle_count error");
			return false;
		}
		task_circle_single_ptr p_single = p_task_circle->circle_single_vec[circle_count];
		if (NULL == p_single)
		{
			log_error("NULL == p_single");
			return false;
		}
		add_exp = p_conf->exp() * p_single->exp_coeff;
		if (p_single->award > 0)
		{
			drop_manager_t::drop(p_role, p_single->award, log_enum::source_type_task_quick_finish, proto::common::drop_sys_type_task, task_id, &rcd);
		}
		if (task_type == TASK_TYPE_SHILIAN)
		{
			uint32_t star = 2;
			auto p_shilian_conf = GET_CONF(TaskShilian, star);
			if (NULL == p_shilian_conf)
			{
				log_error("NULL == p_shilian_conf[%d]", star);
				return false;
			}
			coeff = p_shilian_conf->coeff();
		}

		if (task_type == TASK_TYPE_BOUNTY)
		{
			bounty_mgr_ptr p_bounty_mgr = p_role->get_bounty_mgr();
			if (NULL != p_bounty_mgr)
			{
				p_bounty_mgr->quick_finish_task( 999, &rcd);
			}
		}
	}
	else 
	{
		return false;
	}
	
	TASK_LOG("role[%lu] quick finish task[%d]", m_role_uid, task_id);
	add_task_type_count(task_type, 1);

	// 结算奖励
	if (add_exp > 0)
	{
		p_role->add_exp(add_exp * coeff, log_enum::source_type_task_quick_finish, false, &rcd);
	}

	//增加家族资金
	if (p_conf->family_gold() != 0 && p_role->get_family_id() != 0)
	{
		role_t::add_family_money(p_role, p_conf->family_gold());
	}
	//增加国家资金
	if (p_conf->country_gold() != 0 && p_role->get_country_id() != 0) 
	{
		role_t::add_country_money(p_role, p_conf->country_gold());
	}
	calc_reward(task_id, rcd, ec, coeff);

	return true;
}


uint32_t task_manager_t::get_total_circle_count(uint32_t role_level, uint32_t task_type, uint32_t task_id, uint32_t act_id)
{
	auto p_conf = GET_CONF( Activities, act_id );
	if (NULL == p_conf)
	{
		log_error("Activites[%u] config not found", act_id);
		return errcode_enum::notice_unknown;
	}

	uint32_t max_circle_count = 0;
	if ( task_type == TASK_TYPE_BOUNTY)
	{
		max_circle_count = p_conf->times();
	}
	else {
		max_circle_count = task_manager_t::get_max_circle_count( role_level, task_type, task_id) * p_conf->times();
	}
	return max_circle_count;
}


//判断是否可解锁扫荡
bool task_manager_t::is_task_can_sd(uint32_t task_type)
{
	auto iter_f = m_circle_task_complate_list.find(task_type);
	if (iter_f == m_circle_task_complate_list.end())
	{
		return 	false;
	}
	return true;
}

void task_manager_t::unlock_task_sd(uint32_t task_type)
{
	m_circle_task_complate_list.insert(task_type);
	save_self();
}

bool task_manager_t::is_finish_all_circle(uint32_t type, uint32_t level, uint32_t aid )
{
	uint32_t task_count = get_circle_task_num(type);
	uint32_t max_circle_count = task_manager_t::get_total_circle_count( level, type, 0, aid);
	if (task_count >= max_circle_count)
		return true;
	return false;
}


