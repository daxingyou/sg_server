#include "page_manager.hpp"
#include "tblh/errcode_enum.hpp"
#include "role/role_manager.hpp"
#include "role/role.hpp"
#include "tblh/Task.tbls.h"
#include "common/config_mgr.h"
#include "item/drop_manager.hpp"
#include "tblh/Section.tbls.h"
#include "tblh/SectionReward.tbls.h"
#include <set>
#include <algorithm>
#include <vector>
#include "red_point/red_point_manager.hpp"
#include "tblh/SectionElite.tbls.h"
#include "role/money_manager.hpp"
#include "tblh/BuyPermission.tbls.h"


void page_manager_t::init(uint64_t role_uid)
{
	check_page_elite(role_uid);
	check_reward_status(role_uid);
}

void page_manager_t::check_first_page_elite_open(uint64_t role_uid)
{
	role_ptr p_role = role_manager_t::find_role(role_uid);
	if (NULL == p_role) {
		log_error("NULL == p_role");
		return;
	}

	///是否已经存在于开启列表
	std::set<uint32_t>& open_list = p_role->get_page_elite_open_list();
	if (0 != open_list.count(1))
		return;
	
	SectionElite* p_config = GET_CONF(SectionElite,1);
	if (NULL == p_config){
		log_error("NULL == p_config SectionElite [%d]",1);
		return;
	}

	const std::map<uint32_t, uint32_t>& cond_list = p_config->cond_list();
	for (auto& it : cond_list)
	{
		if (it.first != PAGE_OPEN_COND_ENUM::PAGE_OPEN_COND_LEVEL)
			continue;
		if (p_role->get_level() < it.second)
			return;
		open_list.insert(1);
		red_point_manager_t::red_point_notify(role_uid, proto::common::RED_POINT_TYPE::RED_POINT_PAGE_ELITE, 1);
		return;
	}
}

void page_manager_t::check_page_elite(uint64_t role_uid)
{
	role_ptr p_role = role_manager_t::find_role(role_uid);
	if (NULL == p_role) {
		log_error("NULL == p_role");
		return;
	}

	std::set<uint32_t>& open_list = p_role->get_page_elite_open_list();
	std::map<uint32_t, uint32_t>& elite_list = p_role->get_page_elite_map();
	///从开启列表清除已经通关的章节精英关卡
	for (auto it : elite_list)
	{
		if (0 != open_list.count(it.first))
			open_list.erase(it.first);
	}
	if (elite_list.empty()) {
		SectionElite* p_config = GET_CONF(SectionElite, 1);
		if (NULL == p_config) {
			log_error("NULL == p_config SectionElite id[%d]", 1);
			return;
		}
		const std::map<uint32_t, uint32_t>& cond_list = p_config->cond_list();
		for (auto it : cond_list)
		{
			if (it.first == PAGE_OPEN_COND_ENUM::PAGE_OPEN_COND_LEVEL) {
				if (p_role->get_level() >= it.second)
					open_list.insert(1);
			}
			///以后可能会有其他条件
		}

		if (!open_list.empty())
			red_point_manager_t::red_point_notify(role_uid, proto::common::RED_POINT_TYPE::RED_POINT_PAGE_ELITE, 1);
	}
	else {
		uint32_t pass_elite_count = elite_list.size();
		SectionElite* p_config = GET_CONF(SectionElite, pass_elite_count + 1);
		SectionElite* p_prev_config = GET_CONF(SectionElite, pass_elite_count);
		if (NULL == p_config) {
			log_error("NULL == p_config SectionElite id[%d]", pass_elite_count + 1);
			return;
		}

		if (NULL == p_prev_config) {
			log_error("NULL == p_config SectionElite id[%d]", pass_elite_count);
			return;
		}

		do {
			if (0 != open_list.count(pass_elite_count + 1))
				break;	///已经存在于开启列表

			const std::map<uint32_t, uint32_t>& cond_list = p_config->cond_list();
			bool is_break = false;

			for (auto it : cond_list)
			{
				switch (it.first)
				{
				case PAGE_OPEN_COND_ENUM::PAGE_OPEN_COND_ELITE_ID:
				{
					if (0 == elite_list.count(it.second))
						is_break = true;
				}break;
				case PAGE_OPEN_COND_ENUM::PAGE_OPEN_COND_LEVEL:
				{
					if (p_role->get_level() < it.second)
						is_break = true;
				}break;
				default:
					break;	///找不到类型当作没有条件
				}
				if (is_break)
					break;
			}

			if (!is_break)
				open_list.insert(pass_elite_count + 1);
		} while (false);

		if (!open_list.empty() && p_config->page_id() != p_prev_config->page_id())
			red_point_manager_t::red_point_notify(role_uid, proto::common::RED_POINT_TYPE::RED_POINT_PAGE_ELITE, pass_elite_count + 1);
	}
}

void page_manager_t::check_reward_status(uint64_t role_uid)
{
	role_ptr p_role = role_manager_t::find_role(role_uid);
	if (NULL == p_role) {
		log_error("NULL == p_role");
		return;
	}

	proto::client::gc_section_online_reward_notify notify;
	proto::common::role_change_data* p_data = notify.mutable_rcd();
	std::map<uint32_t, uint32_t>& page_task_list = p_role->get_page_task_map();
	std::map<uint32_t, uint32_t>& page_elite_list = p_role->get_page_elite_map();
	std::set<uint32_t> reward_task_list,reward_elite_list;
	for (auto& it : page_task_list)
	{
		if (0 != it.second)
			continue;

		Section* p_config = GET_CONF(Section, it.first);
		if (NULL == p_config){
			log_error("NULL == p_config Section id[%d]", it.first);
			continue;
		}
		bool result = drop_manager_t::drop(p_role, p_config->drop_id(), log_enum::source_type_page_reward_login_auto_send, proto::common::drop_sys_type_page, it.first, p_data);
		if (!result)
			continue;
		reward_task_list.insert(it.first);
	}

	for (auto& it : page_elite_list)
	{
		if (0 != it.second)
			continue;

		SectionElite* p_config = GET_CONF(SectionElite, it.first);
		if (NULL == p_config){
			log_error("NULL == p_config SectionElite id[%d]", it.first);
			continue;
		}
		bool result = drop_manager_t::drop(p_role, p_config->drop_id(), log_enum::source_type_page_reward_login_auto_send, proto::common::drop_sys_type_page, it.first, p_data);
		if (!result)
			continue;

		reward_elite_list.insert(it.first);
	}

	for (auto& it : reward_task_list)
	{
		if(0 == page_task_list.count(it))
			continue;
		page_task_list[it] = 1;
	}

	for (auto& it : reward_elite_list)
	{
		if (0 == page_elite_list.count(it))
			continue;
		page_elite_list[it] = 1;
	}
	p_role->save_self(true);

	if (notify.has_rcd())
	{
		p_role->send_msg_to_client(op_cmd::gc_section_online_reward_notify, notify);
	}
}

void page_manager_t::check_page_task_status(uint64_t role_uid,uint32_t task_id)
{
	role_ptr p_role = role_manager_t::find_role(role_uid);
	if (NULL == p_role){
		log_error("NULL == p_role");
		return;
	}

	task_mgr_ptr p_task_mgr = p_role->get_task_mgr();
	if (NULL == p_task_mgr){
		log_error("NULL == p_task_mgr");
		return;
	}

	if (p_task_mgr->is_done(task_id)){

		std::map<uint32_t, Section*> confs;
		GET_ALL_CONF(Section, confs);
		if (confs.empty()){
			log_error("Section table empty");
			return;
		}
		uint32_t section_id = 0 ,drop_id = 0;
		for (auto& it : confs)
		{
			Section* p_config = it.second;
			if (NULL == p_config){
				log_error("NULL == p_config");
				return;
			}

			if (task_id == p_config->task_id()){
				section_id = p_config->id();
				drop_id = p_config->drop_id();
				break;
			}
		}

		if (0 != drop_id){
			std::map<uint32_t, uint32_t>& page_task_list = p_role->get_page_task_map();
			if (page_task_list.empty() || 0 == page_task_list.count(section_id)){

				page_task_list.insert(std::make_pair(section_id, 0));
				p_role->save_self(true);
				red_point_manager_t::red_point_notify(role_uid, proto::common::RED_POINT_TYPE::RED_POINT_PAGE, 0);
			}
		}	
		page_task_finish_notify(role_uid);
	}

	std::set<uint32_t>& pass_page_list = p_role->get_page_list();
	const uint32_t cur_page = pass_page_list.size();		///当前已完成章节数
	SectionReward* p_reward_config = GET_CONF(SectionReward,cur_page + 1);

	if (NULL == p_reward_config){
		log_error("NULL == p_reward_config page_id[%d]",cur_page + 1);
		return;
	}

	std::set<uint32_t> task_id_list;
	const std::vector<uint32_t>& list = p_reward_config->page_vec();
	for (auto& it : list)
	{
		Section* p_page_config = GET_CONF(Section, it);
		if (NULL == p_page_config){
			log_error("NULL == p_page_config page[%d]",it);
			return;
		}
		///log_warn("p_page_config->task_id() [%d]", p_page_config->task_id());
		task_id_list.insert(p_page_config->task_id());
	}
	if (0 == task_id_list.count(task_id))
		return;
	
	for (auto& it : task_id_list)
	{
		if (!p_task_mgr->is_done(it)){
			PAGE_LOG("page[%d] task[%d] not finish ", cur_page + 1,it);
			return;
		}
	}
	///完成章节
	pass_page_list.insert(cur_page + 1);
	p_role->save_self(true);

	/*std::map<uint32_t, uint32_t>& elite_list = p_role->get_page_elite_map();
	if (elite_list.empty())
		page_manager_t::check_page_elite(role_uid);*/
}

void page_manager_t::check_page_elite_task_status(uint64_t role_uid,uint32_t page_elite_id, uint32_t against_id)
{
	role_ptr p_role = role_manager_t::find_role(role_uid);
	if (NULL == p_role){
		log_error("NULL == p_role");
		return;
	}

	SectionElite* p_elite_config = GET_CONF(SectionElite, page_elite_id);
	if (NULL == p_elite_config){
		log_error("NULL == p_elite_config SectionElite id[%d]", page_elite_id);
		return;
	}

	if (p_elite_config->fight_id() != against_id) {
		log_error("page_elite_id[%d] fight_id[%d] with against_id[%d] no match",page_elite_id, p_elite_config->fight_id(), against_id);
		return;
	}

	std::map<uint32_t, uint32_t>& elite_list = p_role->get_page_elite_map();
	if (0 != elite_list.count(page_elite_id))
		return;

	elite_list.insert(std::make_pair(page_elite_id,0));
	p_role->save_self(true);
	check_page_elite(role_uid);
}


uint32_t page_manager_t::get_page_list(uint64_t role_uid,proto::client::gc_section_reply& reply)
{
	uint32_t reply_code = errcode_enum::error_ok;
	std::vector<uint32_t> task_id_vec;
	do 
	{
		role_ptr p_role = role_manager_t::find_role(role_uid);
		if (NULL == p_role){
			log_error("NULL == p_role");
			reply_code = errcode_enum::notice_unknown;
			break;;
		}

		task_mgr_ptr p_task_mgr	= p_role->get_task_mgr();
		if (NULL == p_task_mgr){
			log_error("NULL == p_task_mgr role[%lu]",role_uid);
			reply_code = errcode_enum::notice_unknown;
			break;
		}

		proto::common::page_data* p_page_data = reply.mutable_page_list();
		std::set<uint32_t>& page_list = p_role->get_page_list();
		std::map<uint32_t, uint32_t>& page_task_list = p_role->get_page_task_map();
		std::map<uint32_t, uint32_t>& page_elite_list = p_role->get_page_elite_map();
		std::set<uint32_t>& open_list = p_role->get_page_elite_open_list();
		std::map<uint32_t, uint32_t>& quick_fight_list = p_role->get_quick_elite_page_map();
		std::map<uint32_t, uint32_t>& reset_list = p_role->get_reset_quick_elite_page_map();
		for (auto& it : page_list)
		{
			p_page_data->add_pass_page_list(it);
			PAGE_LOG("page_list role[%lu] id[%d]", role_uid, it);
		}

		for (auto& it : page_task_list)
		{
			proto::common::page_task_single* p_page_task_single = p_page_data->add_task_info();
			p_page_task_single->set_id(it.first);
			p_page_task_single->set_status(it.second);
			PAGE_LOG("page_task_list role[%lu] id[%d] status[%d]", role_uid, it.first,it.second);
		}

		proto::common::page_elite_data* p_elite_data = p_page_data->mutable_elite_data();
		for (auto& it : page_elite_list)
		{
			proto::common::page_elite_single* p_page_elite_single = p_elite_data->add_pass_elite_list();
			p_page_elite_single->set_id(it.first);
			p_page_elite_single->set_status(it.second);
			p_page_elite_single->set_num(quick_fight_list[it.first]);	//此处两个列表都可以任意插数据， 所以用这种写法，代码比较整洁
			p_page_elite_single->set_reset(reset_list[it.first]);

			PAGE_LOG("page_elite_list role[%lu] id[%d] status[%d]", role_uid, it.first, it.second);
		}

		for (auto& it : open_list)
		{
			p_elite_data->add_can_fight_list(it);
			PAGE_LOG("open_list role[%lu] id[%d]", role_uid, it);
		}

		const std::map<uint64_t, task_ptr>& task_list = p_task_mgr->get_all_task();
	
		for (auto& it : task_list)
		{
			task_ptr p_task = it.second;
			if (NULL == p_task){
				log_error("NULL == p_task");
				continue;
			}

			if (task_type::TASK_TYPE_TRUNK != p_task->get_type()){
				PAGE_LOG("task[%d] task type not trunk type[%d]",p_task->get_tid() ,p_task->get_type());
				continue;
			}

			if (task_state::TASK_STATE_FINISH != p_task->get_state()){
				PAGE_LOG("task[%d] task state [%d]", p_task->get_tid(), p_task->get_state());
				continue;
			}

			Task* p_config = GET_CONF(Task,p_task->get_tid());
			if (NULL == p_config){
				log_error("NULL == p_config task_id[%d]",p_task->get_tid());
				reply_code = errcode_enum::notice_config_error;
				break;
			}

			if (task_target_type::TASK_TARGET_TYPE_KILL_MONSTER != p_config->get_target_type()){
				PAGE_LOG("target type[%d]", p_config->get_target_type());
				continue;
			}

			task_id_vec.push_back(p_task->get_tid());
		}

	} while (false);
	///PAGE_LOG("task_id_vec[%d]", task_id_vec.size());
	std::sort(task_id_vec.begin(), task_id_vec.end(),std::less<uint32_t>());
	for (auto& it : task_id_vec )
	{
		reply.add_task_id(it);
		PAGE_LOG("role[%lu] task_id[%d]", it);
	}
	///PAGE_LOG("task_id_size[%d]", reply.task_id_size());
	/*for (int32_t i = 0; i < reply.task_id_size(); ++i)
	{
		PAGE_LOG("task_id[%d]", reply.task_id(i));
	}*/
	return reply_code;
}

uint32_t page_manager_t::get_page_pass_reward(uint32_t type,uint64_t role_uid,uint32_t id ,proto::client::gc_section_reward_reply& reply)
{
	uint32_t reply_code = errcode_enum::error_ok;
	do 
	{
		role_ptr p_role = role_manager_t::find_role(role_uid);
		if (NULL == p_role)
		{
			log_error("NULL == p_role");
			reply_code = errcode_enum::notice_unknown;
			break;
		}
		uint32_t drop_id = 0;
		proto::common::role_change_data* p_data = reply.mutable_rcd();
		std::map<uint32_t, uint32_t>& page_task_list = p_role->get_page_task_map();
		std::map<uint32_t, uint32_t>& page_elite_list = p_role->get_page_elite_map();
		if (1 == type){
			if (0 == page_task_list.count(id)){
				log_error("role[%lu] page_task[%d] not pass", role_uid, id);
				reply_code = errcode_enum::notice_page_task_not_finish;
				break;
			}

			if (1 == page_task_list[id]){
				log_error("role[%lu] page_task[%d] already get", role_uid, id);
				reply_code = errcode_enum::notice_page_task_reward_already_get;
				break;
			}

			Section* p_config = GET_CONF(Section, id);
			if (NULL == p_config)
			{
				log_error("NULL == p_config Section id[%d]", id);
				reply_code = errcode_enum::notice_config_error;
				break;
			}

			page_task_list[id] = 1;
			drop_id = p_config->drop_id();
			drop_manager_t::drop(p_role, drop_id, log_enum::source_type_page_task_finish, proto::common::drop_sys_type_page, id, p_data);
			p_role->save_self(true);
		} 
		else if(2 == type)
		{
			if (0 == page_elite_list.count(id))
			{
				log_error("role[%lu] page_elite[%d] not pass", role_uid, id);
				reply_code = errcode_enum::notice_page_task_not_pass;
				break;
			}
			if (1 == page_elite_list[id])
			{
				log_error("role[%lu] page_elite[%d] reward already get", role_uid, id);
				reply_code = errcode_enum::notice_page_reward_already_get;
				break;
			}
			SectionElite* p_config = GET_CONF(SectionElite, id);
			if (NULL == p_config)
			{
				log_error("NULL == p_config SectionElite id[%d]", id);
				reply_code = errcode_enum::notice_config_error;
				break;
			}

			page_elite_list[id] = 1;
			drop_id = p_config->drop_id();
			drop_manager_t::drop(p_role, drop_id, log_enum::source_type_page_pass, proto::common::drop_sys_type_page, id, p_data);
			p_role->save_self(true);
		}
	} while (false);

	return reply_code;
}



//快速扫荡
uint32_t page_manager_t::quick_fight_elite_page(uint64_t role_uid, uint32_t id, uint32_t type, proto::client::gc_quick_fight_elite_page_reply &reply)
{
	uint32_t reply_code = errcode_enum::notice_unknown;
	uint32_t need_count = 1;
	if (type > 1)
		need_count = 5;

	do {
		role_ptr p_role = role_manager_t::find_role(role_uid);
		if (NULL == p_role)
		{
			log_error("NULL == p_role");
			reply_code = errcode_enum::notice_unknown;
			break;
		}

		std::map<uint32_t, uint32_t>& page_elite_list = p_role->get_page_elite_map();
		std::map<uint32_t, uint32_t>& quick_elite_page_map = p_role->get_quick_elite_page_map();
		std::map<uint32_t, uint32_t>& reset_quick_elite_page_map = p_role->get_reset_quick_elite_page_map();
		
		auto iter_f = page_elite_list.find(id);
		if (iter_f == page_elite_list.end())
		{
			log_error("role[%lu] page_elite[%d] not pass", role_uid, id);
			reply_code = errcode_enum::notice_page_task_not_pass;
			break;
		}

		SectionElite* p_config = GET_CONF(SectionElite, id);
		if (NULL == p_config)
		{
			log_error("[%lu] NULL == p_config SectionElite id[%d]", role_uid, id);
			reply_code = errcode_enum::notice_config_error;
			break;
		}

		uint32_t quick_num = quick_elite_page_map[id];
		if ( (quick_num + need_count) > p_config->max_sd_count() )  
		{//如果剩余次数小于5次， 则可扫荡次数等于 剩余次数。
			need_count = p_config->max_sd_count() - quick_num;
		}

		if (need_count == 0)
		{
			log_error( "user:[%lu], need_count == 0", role_uid );
			reply_code = errcode_enum::notice_elite_scouring_1;
			break;
		}
	
		if ( !money_manager_t::use_money(p_role, proto::common::MONEY_TYPE_VIGOUR, p_config->sd_cost() * need_count, log_enum::source_type_page_task_quick_finish, 0, true, reply.mutable_rcd()) )
		{
			log_error("user:[%lu] shake_money_tree vigour[%u] not enough", role_uid, p_config->sd_cost());
			reply_code = errcode_enum::notice_elite_scouring_2;
			break;
		}

		quick_elite_page_map[id] = quick_num + need_count;
		
		std::vector<proto::common::item_num_info> item_vec;
		for (uint32_t i = 0; i < need_count; ++i)
		{
			item_vec.clear();
			drop_manager_t::drop_once(p_role, p_config->sd_drop_id(), log_enum::source_type_page_task_quick_finish, proto::common::drop_sys_type_page, id, item_vec, reply.mutable_rcd() );
			proto::common::item_num_info_list *p_item_list = reply.add_item_info_list();

			for (auto it : item_vec)
			{
				auto *p_item = p_item_list->add_item_list();
				p_item->CopyFrom(it);
			}
		}

		uint32_t reset = reset_quick_elite_page_map[id];

		reply_code = errcode_enum::error_ok;
		reply.set_num(quick_num + need_count);
		reply.set_reset( reset );

		p_role->save_self(true);
	} while (0);
	return reply_code;
}

//重置类型
uint32_t page_manager_t::reset_quick_fight_elite_page(uint64_t role_uid, uint32_t id, proto::client::gc_reset_quick_fight_elite_page_reply &reply)
{
	uint32_t reply_code = errcode_enum::notice_unknown;
	do {
		role_ptr p_role = role_manager_t::find_role(role_uid);
		if (NULL == p_role)
		{
			log_error("NULL == p_role");
			reply_code = errcode_enum::notice_unknown;
			break;
		}

		std::map<uint32_t, uint32_t>& page_elite_list = p_role->get_page_elite_map();
		std::map<uint32_t, uint32_t>& quick_elite_page_map = p_role->get_quick_elite_page_map();
		std::map<uint32_t, uint32_t>& reset_quick_elite_page_map = p_role->get_reset_quick_elite_page_map();

		auto iter_f = page_elite_list.find(id);
		if (iter_f == page_elite_list.end())
		{
			log_error("role[%lu] page_elite[%d] not pass", role_uid, id);
			reply_code = errcode_enum::notice_page_task_not_pass;
			break;
		}

		SectionElite* p_config = GET_CONF(SectionElite, id);
		if (NULL == p_config)
		{
			log_error("[%lu] NULL == p_config SectionElite id[%d]", role_uid, id);
			reply_code = errcode_enum::notice_config_error;
			break;
		}

		BuyPermission* p_buyPermission = GET_CONF(BuyPermission, 1);
		if (NULL == p_buyPermission)
		{
			log_error("[%lu] NULL == p_buyPermission BuyPermission id[%d]", role_uid, 1);
			reply_code = errcode_enum::notice_config_error;
			break;
		}

		uint32_t quick_num = quick_elite_page_map[id];
		if ( p_config->max_sd_count() > quick_num )
		{
			log_error("[%lu] reset quick_fight[%u] quick_num [%u] > 0 ", role_uid, id, quick_num );
			reply_code = errcode_enum::notice_elite_scouring_5;
			break;
		}

		uint32_t reset_num = reset_quick_elite_page_map[id];
		if (  reset_num >= p_buyPermission->max_buy_count() )
		{
			log_error("[%lu] reset quick_fight[%u] rest_num [%u] > max_num[%u] ", role_uid, id, reset_num, p_buyPermission->max_buy_count() );
			reply_code = errcode_enum::notice_elite_scouring_6;
			break;
		}

		uint32_t price = p_buyPermission->get_price(reset_num);
		if ( !money_manager_t::use_money(p_role, proto::common::MONEY_TYPE_YUANBAO, price, log_enum::source_type_page_task_quick_finish, 0, true, reply.mutable_rcd()) )
		{
			log_error("user:[%lu] reset_quickfight  money[%u] not enough", role_uid, price);
			reply_code = errcode_enum::notice_elite_scouring_4;
			break;
		}
		
		quick_elite_page_map[id] = 0;		
		reset_quick_elite_page_map[id] = reset_num + 1;
		reply.set_num( reset_num + 1 );
		p_role->save_self(true);
		reply_code = errcode_enum::error_ok;
	} while (0);

	return reply_code;
}

void page_manager_t::page_task_finish_notify(uint64_t role_uid)
{
	role_ptr p_role = role_manager_t::find_role(role_uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role");
		return;
	}

	std::set<uint32_t>& page_list = p_role->get_page_list();
	std::map<uint32_t, uint32_t>& page_task_list = p_role->get_page_task_map();

	proto::client::gc_section_task_finish_notify notify;
	for (auto& it : page_list)
	{
		notify.add_pass_page_list(it);
	}

	for (auto& it : page_task_list)
	{
		proto::common::page_task_single* p_single = notify.add_task_info();
		p_single->set_id(it.first);
		p_single->set_status(it.second);
	}
	p_role->send_msg_to_client(op_cmd::gc_section_task_finish_notify, notify);
}

