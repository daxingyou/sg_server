#include "bounty_manager.hpp"
#include "cache_key.hpp"
#include "tblh/Bounty.tbls.h"
#include "tblh/Comprehensive.tbls.h"
#include "config_mgr.h"
#include "role/role_unify_save.hpp"
#include "role/role.hpp"
#include "role/role_manager.hpp"
#include "item/drop_manager.hpp"
#include "hero/hero_manager.hpp"
#include "role/money_manager.hpp"
#include "activity/activity_manager.hpp"
#include "center/center_msg_handler.hpp"
#include "tblh/Activities.tbls.h"
#include "achieve/achieve_common.hpp"
#include "tblh/errcode_enum.hpp"

USING_NS_COMMON;
uint32_t bounty_manager_t::s_max_accept_count;
uint32_t bounty_manager_t::s_refresh_count;
uint32_t bounty_manager_t::s_min_level;
std::vector<uint32_t> bounty_manager_t::s_first_prob;
std::vector<uint32_t> bounty_manager_t::s_second_prob;
std::vector<uint32_t> bounty_manager_t::s_third_prob;
std::map<uint32_t, uint32_t> bounty_manager_t::s_box_info;
std::map<uint32_t, std::vector<uint32_t>> bounty_manager_t::s_bounty_map_by_star;

bounty_manager_t::bounty_manager_t(uint64_t role_uid)
	: m_role_uid(role_uid)
{
	m_key = cache_key_t::create(m_role_uid, cache_name::bounty);
}

void bounty_manager_t::init()
{
	role_ptr p_role = role_manager_t::find_role(m_role_uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role[%lu]", m_role_uid);
		return;
	}
	m_bounty_vec.clear();
	m_box_vec.clear();
	m_refresh_count = s_max_accept_count;
	m_star_count = 0;
	make_bounty(s_min_level);
	save_self();
}

void bounty_manager_t::load_data(const proto::common::country_bounty_data& country_bounty_data)
{
	m_bounty_vec.clear();
	m_box_vec.clear();
	for (int i = 0; i < country_bounty_data.bounty_data_size(); ++i)
	{
		bounty_ptr p_bounty = bounty_ptr(new bounty_t());
		p_bounty->load_data(country_bounty_data.bounty_data(i));
		calc_star(p_bounty);
		m_bounty_vec.push_back(p_bounty);
	}
	
	for (int i = 0; i < country_bounty_data.box_list_size(); ++i)
	{
		m_box_vec.push_back(country_bounty_data.box_list(i).star());
	}
	m_refresh_count = country_bounty_data.refresh_count();
	m_star_count = country_bounty_data.star_count();
	m_accept_level = country_bounty_data.accept_level();
}

void bounty_manager_t::peek_data(proto::common::country_bounty_data* p_country_bounty_data)
{
	if (NULL == p_country_bounty_data)
	{
		return;
	}
	role_ptr p_role = role_manager_t::find_role(m_role_uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role[%lu]", m_role_uid);
		return;
	}
	activity_mgr_ptr p_activity_mgr = p_role->get_activity_mgr();
	if (NULL == p_activity_mgr)
	{
		log_error("NULL == p_activity_mgr[%lu]", m_role_uid);
		return;
	}
	for (uint32_t i = 0; i < m_bounty_vec.size(); ++i)
	{
		bounty_ptr p_bounty = m_bounty_vec[i];
		if (NULL == p_bounty)
		{
			log_error("NULL == p_bounty");
			continue;
		}
		proto::common::bounty_single* p_single = p_country_bounty_data->add_bounty_data();
		p_bounty->peek_data(p_single);
	}
	
	p_country_bounty_data->set_accept_count(p_activity_mgr->get_activity_count(activity_country_bounty));
	p_country_bounty_data->set_max_accept_count(s_max_accept_count);
	p_country_bounty_data->set_refresh_count(m_refresh_count);
	p_country_bounty_data->set_star_count(m_star_count);
	p_country_bounty_data->set_accept_level(m_accept_level);

	for (auto it : s_box_info)
	{
		uint32_t star = it.first;
		proto::common::star_box* p_star_box = p_country_bounty_data->add_box_list();
		p_star_box->set_star(star);
		std::vector<uint32_t>::iterator it_vec = std::find(m_box_vec.begin(), m_box_vec.end(), star);
		if (it_vec != m_box_vec.end())
		{
			p_star_box->set_state(star_box_state_over);
		}
		else
		{
			if (m_star_count >= star)
			{
				p_star_box->set_state(star_box_state_finish);
			}
			else
			{
				p_star_box->set_state(star_box_state_none);
			}
		}
	}
}

void bounty_manager_t::save_self()
{
	proto::common::country_bounty_data country_bounty_data;
	peek_data(&country_bounty_data);
	role_unify_save::add_task(m_role_uid, m_key, country_bounty_data);
}

void bounty_manager_t::load_comprehensive()
{
	s_first_prob.clear();
	s_second_prob.clear();
	s_third_prob.clear();
	s_box_info.clear();
	s_bounty_map_by_star.clear();

	auto p_activity_conf = GET_CONF(Activities, activity_country_bounty);
	if (NULL != p_activity_conf)
	{
		s_max_accept_count = p_activity_conf->times();
		s_min_level = p_activity_conf->level();
	}

	auto p_refresh_count = GET_CONF(Comprehensive, comprehensive_common::country_bountytask_refreshfreetimes);
	s_refresh_count = GET_COMPREHENSIVE_VALUE_1(p_refresh_count);

	auto p_first_prob_three = GET_CONF(Comprehensive, comprehensive_common::country_bountytask_minprobabilitythree);
	uint32_t val = GET_COMPREHENSIVE_VALUE_1(p_first_prob_three);
	s_first_prob.push_back(val);
	auto p_first_prob_four = GET_CONF(Comprehensive, comprehensive_common::country_bountytask_minprobabilityfour);
	val = GET_COMPREHENSIVE_VALUE_1(p_first_prob_four);
	s_first_prob.push_back(val);
	auto p_first_prob_five = GET_CONF(Comprehensive, comprehensive_common::country_bountytask_minprobabilityfive);
	val = GET_COMPREHENSIVE_VALUE_1(p_first_prob_five);
	s_first_prob.push_back(val);

	auto p_second_prob_one = GET_CONF(Comprehensive, comprehensive_common::country_bountytask_probabilityone);
	val = GET_COMPREHENSIVE_VALUE_1(p_second_prob_one);
	s_second_prob.push_back(val);
	auto p_second_prob_two = GET_CONF(Comprehensive, comprehensive_common::country_bountytask_probabilitytwo);
	val = GET_COMPREHENSIVE_VALUE_1(p_second_prob_two);
	s_second_prob.push_back(val);
	auto p_second_prob_three = GET_CONF(Comprehensive, comprehensive_common::country_bountytask_probabilitythree);
	val = GET_COMPREHENSIVE_VALUE_1(p_second_prob_three);
	s_second_prob.push_back(val);
	auto p_second_prob_four = GET_CONF(Comprehensive, comprehensive_common::country_bountytask_probabilityfour);
	val = GET_COMPREHENSIVE_VALUE_1(p_second_prob_four);
	s_second_prob.push_back(val);
	auto p_second_prob_five = GET_CONF(Comprehensive, comprehensive_common::country_bountytask_probabilityfive);
	val = GET_COMPREHENSIVE_VALUE_1(p_second_prob_five);
	s_second_prob.push_back(val);

	auto p_third_prob_one = GET_CONF(Comprehensive, comprehensive_common::country_bountytask_probabilityone2);
	val = GET_COMPREHENSIVE_VALUE_1(p_third_prob_one);
	s_third_prob.push_back(val);
	auto p_third_prob_two = GET_CONF(Comprehensive, comprehensive_common::country_bountytask_probabilitytwo2);
	val = GET_COMPREHENSIVE_VALUE_1(p_third_prob_two);
	s_third_prob.push_back(val);
	auto p_third_prob_three = GET_CONF(Comprehensive, comprehensive_common::country_bountytask_probabilitythree2);
	val = GET_COMPREHENSIVE_VALUE_1(p_third_prob_three);
	s_third_prob.push_back(val);
	auto p_third_prob_four = GET_CONF(Comprehensive, comprehensive_common::country_bountytask_probabilityfour2);
	val = GET_COMPREHENSIVE_VALUE_1(p_third_prob_four);
	s_third_prob.push_back(val);
	auto p_third_prob_five = GET_CONF(Comprehensive, comprehensive_common::country_bountytask_probabilityfive2);
	val = GET_COMPREHENSIVE_VALUE_1(p_third_prob_five);
	s_third_prob.push_back(val);

	auto p_first_box = GET_CONF(Comprehensive, comprehensive_common::country_bountytask_boxonestar);
	uint32_t star1 = GET_COMPREHENSIVE_VALUE_1(p_first_box);
	uint32_t drop1 = GET_COMPREHENSIVE_VALUE_2(p_first_box);
	s_box_info.insert(std::make_pair(star1, drop1));
	auto p_second_box = GET_CONF(Comprehensive, comprehensive_common::country_bountytask_boxtwostar);
	uint32_t star2 = GET_COMPREHENSIVE_VALUE_1(p_second_box);
	uint32_t drop2 = GET_COMPREHENSIVE_VALUE_2(p_second_box);
	s_box_info.insert(std::make_pair(star2, drop2));
	auto p_third_box = GET_CONF(Comprehensive, comprehensive_common::country_bountytask_boxthreestar);
	uint32_t star3 = GET_COMPREHENSIVE_VALUE_1(p_third_box);
	uint32_t drop3 = GET_COMPREHENSIVE_VALUE_2(p_third_box);
	s_box_info.insert(std::make_pair(star3, drop3));

	auto p_four_box = GET_CONF(Comprehensive, comprehensive_common::country_bountytask_boxfourstar);
	uint32_t star4 = GET_COMPREHENSIVE_VALUE_1(p_four_box);
	uint32_t drop4 = GET_COMPREHENSIVE_VALUE_2(p_four_box);
	s_box_info.insert(std::make_pair(star4, drop4));


	std::map<uint32_t, Bounty*> bounty_map;
	GET_ALL_CONF(Bounty, bounty_map);
	for (auto it : bounty_map)
	{
		auto p_conf = it.second;
		if (NULL == p_conf)
		{
			continue;
		}
		uint32_t star = p_conf->star();
		std::map<uint32_t, std::vector<uint32_t>>::iterator iter = s_bounty_map_by_star.find(star);
		if (iter != s_bounty_map_by_star.end())
		{
			std::vector<uint32_t>& bounty_vec = iter->second;
			bounty_vec.push_back(p_conf->id());
		}
		else
		{
			std::vector<uint32_t> bounty_vec;
			bounty_vec.push_back(p_conf->id());
			s_bounty_map_by_star.insert(std::make_pair(star, bounty_vec));
		}
	}
}

void bounty_manager_t::make_bounty(uint32_t level)
{
	TASK_LOG("role[%lu] level[%d] make_bounty", m_role_uid, level);
	m_accept_level = level;
	//随机悬赏任务数据
	std::vector<uint32_t> rand_bounty;
	
	//随机第一个
	uint32_t first_star = get_star_index(s_first_prob) + 3;
	uint32_t first_bounty = make_bounty_id(first_star, level, rand_bounty);
	rand_bounty.push_back(first_bounty);

	//随机第二个
	uint32_t second_star = get_star_index(s_second_prob) + 1;
	uint32_t second_bounty = make_bounty_id(second_star, level, rand_bounty);
	rand_bounty.push_back(second_bounty);

	//随机第三个
	uint32_t third_star = get_star_index(s_third_prob) + 1;
	uint32_t third_bounty = make_bounty_id(third_star, level, rand_bounty);
	rand_bounty.push_back(third_bounty);
	std::random_shuffle(rand_bounty.begin(), rand_bounty.end());

	//初始化悬赏任务
	m_bounty_vec.clear();
	for (uint32_t i = 0; i < 3; ++i)
	{
		uint32_t bounty_tid = rand_bounty[i];
		auto p_bounty_conf = GET_CONF(Bounty, bounty_tid);
		if (NULL == p_bounty_conf)
		{
			log_error("NULL == p_bounty_conf, bounty_tid[%d]", bounty_tid);
			return;
		}
		bounty_ptr p_bounty = bounty_ptr(new bounty_t(bounty_tid));
		p_bounty->set_state(0);
		p_bounty->set_star(p_bounty_conf->star());
		calc_star(p_bounty);
		m_bounty_vec.push_back(p_bounty);
	}
	TASK_LOG("role[%lu] level[%d] make_bounty success", m_role_uid, level);
}

uint32_t bounty_manager_t::get_star_index(const std::vector<uint32_t>& prob_vec)
{
	int32_t rand_val = random_util_t::randMin(0, 100);
	if (-1 == rand_val)
	{
		log_error("rand_max invalid");
		return 0;
	}
	for (uint32_t i = 0; i < prob_vec.size(); ++i)
	{
		if ((uint32_t)rand_val < prob_vec[i])
		{
			return i;
		}
		rand_val -= prob_vec[i];
	}
	return 0;
}

bool bounty_manager_t::accept_bounty(uint32_t bounty_id)
{
	role_ptr p_role = role_manager_t::find_role(m_role_uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role[%lu]", m_role_uid);
		return false;
	}
	activity_mgr_ptr p_activity_mgr = p_role->get_activity_mgr();
	if (NULL == p_activity_mgr)
	{
		log_error("NULL == p_activity_mgr[%lu]", m_role_uid);
		return false;
	}
	if (!p_activity_mgr->check_open(activity_country_bounty))
	{
		log_error("role [%lu] check open failed", m_role_uid);
		return false;
	}
	if (is_bounty_doing())
	{
		log_error("role[%lu] bounty state error", m_role_uid);
		return false;
	}
	bounty_ptr p_bounty = get_bounty(bounty_id);
	if (NULL == p_bounty)
	{
		log_error("role[%lu] NULL == p_bounty[%d]", m_role_uid, bounty_id);
		return false;
	}
	if (!p_bounty->is_not_accepted())
	{
		log_error("role[%lu] bounty[%d] state error", m_role_uid, bounty_id);
		return false;
	}
	
	task_mgr_ptr p_task_mgr = p_role->get_task_mgr();
	if (NULL == p_task_mgr)
	{
		log_error("NULL == p_task_mgr[%lu]", m_role_uid);
		return false;
	}
	auto p_bounty_conf = GET_CONF(Bounty, bounty_id);
	if (NULL == p_bounty_conf)
	{
		log_error("NULL == p_bounty_conf, bounty_id[%d]", bounty_id);
		return false;
	}
	proto::common::role_change_data rcd;
	p_task_mgr->active_circle_task(TASK_TYPE_BOUNTY, p_bounty_conf->circle_id(), ACCEPT_TASK_BY_SYSTEM, rcd, m_accept_level);
	p_bounty->set_state(bounty_state_doing);
	
	save_self();
	return true;
}
	
uint32_t bounty_manager_t::refresh_bounty(proto::common::role_change_data* p_data)
{
	if (NULL == p_data)
	{
		log_error("NULL == p_data");
		return errcode_enum::notice_unknown;
	}
	role_ptr p_role = role_manager_t::find_role(m_role_uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role[%lu]", m_role_uid);
		return errcode_enum::notice_role_null;
	}

	if (is_bounty_doing())
	{
		log_error("role[%lu] bounty state error", m_role_uid);
		return errcode_enum::notice_unknown;
	}
	if (m_refresh_count == 0)
	{
		if (!money_manager_t::use_money(p_role, proto::common::MONEY_TYPE_YUANBAO, get_refresh_cost(), log_enum::source_type_bounty_refresh, 0))
		{
			log_error("role[%lu] refresh money not enough", m_role_uid);
			return errcode_enum::notice_gold_money_not_enough;
		}
		proto::common::personal_info* p_info = p_data->mutable_per_info();
		p_role->get_personal_info(p_info);
	}
	else
	{
		--m_refresh_count;
	}
	
	make_bounty(p_role->get_level());

	save_self();
	return errcode_enum::error_ok;
}

bool bounty_manager_t::bounty_box(uint32_t box_id, proto::common::role_change_data* p_data)
{
	if (NULL == p_data)
	{
		log_error("NULL == p_data[%lu]", m_role_uid);
		return false;
	}
	if (m_star_count < box_id)
	{
		log_error("role[%lu] star_count not enough", m_role_uid);
		return false;
	}
	std::vector<uint32_t>::iterator it_vec = std::find(m_box_vec.begin(), m_box_vec.end(), box_id);
	if (it_vec != m_box_vec.end())
	{
		log_error("role[%lu] star_box[%d] already get", m_role_uid, box_id);
		return false;
	}
	std::map<uint32_t, uint32_t>::iterator it_map = s_box_info.find(box_id);
	if (it_map == s_box_info.end())
	{
		log_error("role[%lu] star_box[%d] invalid", m_role_uid, box_id);
		return false;
	}
	role_ptr p_role = role_manager_t::find_role(m_role_uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role[%lu]", m_role_uid);
		return false;
	}
	m_box_vec.push_back(box_id);
	uint32_t drop_id = it_map->second;
	drop_manager_t::drop(p_role, drop_id, log_enum::source_type_bounty_star_box, proto::common::drop_sys_type_bounty, box_id, p_data);

	save_self();
	return true;
}

bounty_ptr bounty_manager_t::get_bounty(uint32_t bounty_id)
{
	for (uint32_t i = 0; i < m_bounty_vec.size(); ++i)
	{
		bounty_ptr p_bounty = m_bounty_vec[i];
		if (NULL == p_bounty)
		{
			continue;
		}
		if (p_bounty->get_tid() == bounty_id)
		{
			return p_bounty;
		}
	}
	return bounty_ptr(NULL);
}

void bounty_manager_t::finish_task(uint32_t circle_id)
{
	role_ptr p_role = role_manager_t::find_role(m_role_uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role[%lu]", m_role_uid);
		return;
	}
	for (uint32_t i = 0; i < m_bounty_vec.size(); ++i)
	{
		bounty_ptr p_bounty = m_bounty_vec[i];
		if (NULL == p_bounty)
		{
			continue;
		}
		if (!p_bounty->is_doing())
		{
			continue;
		}
		
		auto p_bounty_conf = GET_CONF(Bounty, p_bounty->get_tid());
		if (NULL == p_bounty_conf)
		{
			continue;
		}
		if (p_bounty_conf->circle_id() != circle_id)
		{
			continue;
		}
		TASK_LOG("role[%lu] bounty_finish[%d]", m_role_uid, p_bounty->get_tid());
		p_bounty->set_state(bounty_state_finish);

		uint32_t old_star = p_bounty_conf->star();
		uint32_t star = p_bounty->get_star();
		m_star_count += star;
		if (star < old_star)
		{
			log_error("role[%lu] bounty[%d] star error", m_role_uid, p_bounty->get_tid());
			return;
		}
		if (star - old_star >= p_bounty_conf->award().size())
		{
			log_error("role[%lu] bounty[%d] star out of range", m_role_uid, p_bounty->get_tid());
			return;
		}
		proto::client::gc_country_bounty_finish_ntf finish_ntf;
		uint32_t drop_id = p_bounty_conf->award().at(star - old_star);
		drop_manager_t::drop(p_role, drop_id, log_enum::source_type_bounty_finish, proto::common::drop_sys_type_bounty, p_bounty->get_tid(), finish_ntf.mutable_rcd());
		money_manager_t::add_money(p_role, proto::common::MONEY_TYPE_PRESTIGE, p_bounty_conf->prestige_award(), log_enum::source_type_bounty_finish, p_bounty->get_tid());

		center_msg_handler_t::sync_data_to_center(m_role_uid, proto::server::data_sync_type_country_money, p_bounty_conf->country_award(), true);

		//activity_mgr_ptr p_activity_mgr = p_role->get_activity_mgr();
		//if (NULL != p_activity_mgr)
		//{
		//	p_activity_mgr->activity_done(activity_country_bounty);
		//}
		proto::common::personal_info *p_info = finish_ntf.mutable_rcd()->mutable_per_info();
		p_role->get_personal_info(p_info);

		finish_ntf.set_bounty_id(p_bounty->get_tid());
		peek_data(finish_ntf.mutable_country_bounty());
		p_role->send_msg_to_client(op_cmd::gc_country_bounty_finish_ntf, finish_ntf);
		

		achieve_common_t::notify_progress_state(m_role_uid, proto::common::Achieve_Event_Type::ACHIEVE_TASK_FINISH_COUNTRY_NUMBER, 0);
		achieve_common_t::notify_progress_state(m_role_uid, proto::common::Achieve_Event_Type::ACHIEVE_COUNTRY_BOUNTY_STAR_NUMBER, old_star);
		save_self();
		return;
	}
}

bool bounty_manager_t::is_bounty_doing()
{
	for (uint32_t i = 0; i < m_bounty_vec.size(); ++i)
	{
		bounty_ptr p_bounty = m_bounty_vec[i];
		if (NULL == p_bounty)
		{
			continue;
		}
		if (p_bounty->is_doing())
		{
			return true;
		}
	}
	return false;
}

void bounty_manager_t::drop_task(uint32_t circle_id)
{
	role_ptr p_role = role_manager_t::find_role(m_role_uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role[%lu]", m_role_uid);
		return;
	}
	for (uint32_t i = 0; i < m_bounty_vec.size(); ++i)
	{
		bounty_ptr p_bounty = m_bounty_vec[i];
		if (NULL == p_bounty)
		{
			continue;
		}
		if (!p_bounty->is_doing())
		{
			continue;
		}
		auto p_bounty_conf = GET_CONF(Bounty, p_bounty->get_tid());
		if (NULL == p_bounty_conf)
		{
			continue;
		}
		if (p_bounty_conf->circle_id() != circle_id)
		{
			continue;
		}
		p_bounty->set_state(bounty_state_drop);
		
		save_self();
		return;
	}
}

void bounty_manager_t::calc_star(bounty_ptr p_bounty)
{
	if (NULL == p_bounty)
	{
		log_error("NULL == p_bounty");
		return;
	}
	auto p_bounty_conf = GET_CONF(Bounty, p_bounty->get_tid());
	if (NULL == p_bounty_conf)
	{
		log_error("NULL == p_bounty_conf, bounty_tid[%d]", p_bounty->get_tid());
		return;
	}
	role_ptr p_role = role_manager_t::find_role(m_role_uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role[%lu]", m_role_uid);
		return;
	}
	for (auto hero_tid : p_bounty_conf->destiny_id())
	{
		if (NULL != hero_manager_t::get_hero(p_role, hero_tid))
		{
			p_bounty->add_star();
		}
	}
}

void bounty_manager_t::calc_star(uint32_t hero_tid)
{
	bool b_find = false;
	for (uint32_t i = 0; i < m_bounty_vec.size(); ++i)
	{
		bounty_ptr p_bounty = m_bounty_vec[i];
		if (NULL == p_bounty)
		{
			log_error("NULL == p_bounty");
			continue;
		}
		auto p_bounty_conf = GET_CONF(Bounty, p_bounty->get_tid());
		if (NULL == p_bounty_conf)
		{
			log_error("NULL == p_bounty_conf, bounty_tid[%d]", p_bounty->get_tid());
			continue;
		}
		std::vector<uint32_t>::const_iterator it = std::find(p_bounty_conf->destiny_id().begin(), p_bounty_conf->destiny_id().end(), hero_tid);
		if (it != p_bounty_conf->destiny_id().end())
		{
			p_bounty->add_star();
			b_find = true;
		}
	}
	if (b_find)
	{
		role_ptr p_role = role_manager_t::find_role(m_role_uid);
		if (NULL == p_role)
		{
			log_error("NULL == p_role[%lu]", m_role_uid);
			return;
		}
		save_self();
	}
}

void bounty_manager_t::one_day()
{
	m_box_vec.clear();
	m_refresh_count = s_refresh_count;
	m_star_count = 0;
	save_self();
}

uint32_t bounty_manager_t::make_bounty_id(uint32_t star, uint32_t level, const std::vector<uint32_t>& exclude_vec)
{
	std::vector<uint32_t> star_vec;
	std::map<uint32_t, std::vector<uint32_t>>::iterator it = s_bounty_map_by_star.find(star);
	if (it == s_bounty_map_by_star.end())
	{
		log_error("star:%d invalid", star);
		return 0;
	}
	const std::vector<uint32_t>& tmp_vec = it->second;
	for (auto bounty_tid : tmp_vec)
	{
		auto p_bounty_conf = GET_CONF(Bounty, bounty_tid);
		if (NULL == p_bounty_conf)
		{
			log_error("NULL == p_bounty_conf, bounty_tid[%d]", bounty_tid);
			continue;
		}
		if (p_bounty_conf->level_range().size() != 2)
		{
			log_error("p_bounty_conf->level_range().size() != 2, bounty_tid[%d]", bounty_tid);
			continue;
		}
		uint32_t min_level = p_bounty_conf->level_range().at(0);
		uint32_t max_level = p_bounty_conf->level_range().at(1);
		if (level < min_level || level > max_level)
		{
			continue;
		}
		star_vec.push_back(bounty_tid);
	}
	uint32_t bounty_id;
	if (!random_util_t::random_from_vec(star_vec, exclude_vec, bounty_id))
	{
		log_error("star_vec invalid");
		return 0;
	}
	return bounty_id;
}

uint32_t bounty_manager_t::get_quick_circle_id(uint32_t star, uint32_t level, uint32_t& bounty_id)
{
	std::vector<uint32_t> rand_bounty;

	bounty_id = make_bounty_id(star, level, rand_bounty);
	
	auto p_bounty_conf = GET_CONF(Bounty, bounty_id);
	if (NULL == p_bounty_conf)
	{
		log_error("NULL == p_bounty_conf, bounty_id[%d]", bounty_id);
		return 0;
	}

	return p_bounty_conf->circle_id();
}

void bounty_manager_t::quick_finish_task(uint32_t bounty_id, std::map<uint32_t, uint32_t>& drop_id_count, std::map<uint32_t, uint32_t>& coin_id_count)
{
	role_ptr p_role = role_manager_t::find_role(m_role_uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role[%lu]", m_role_uid);
		return;
	}

	auto p_bounty_conf = GET_CONF(Bounty, bounty_id);
	if (NULL == p_bounty_conf)
	{
		log_error("user:[%lu] p_bounty_conf[%u] is null", m_role_uid, bounty_id);
		return;
	}

	TASK_LOG("role[%lu] bounty_finish[%d]", m_role_uid, bounty_id);

	uint32_t old_star = p_bounty_conf->star();
	m_star_count += old_star;

	uint32_t drop_id = p_bounty_conf->award().at(0);
	drop_id_count[drop_id] += 1;
	coin_id_count[proto::common::MONEY_TYPE_PRESTIGE] += p_bounty_conf->prestige_award();
	achieve_common_t::notify_progress_state(m_role_uid, proto::common::Achieve_Event_Type::ACHIEVE_TASK_FINISH_COUNTRY_NUMBER,0);
	achieve_common_t::notify_progress_state(m_role_uid, proto::common::Achieve_Event_Type::ACHIEVE_COUNTRY_BOUNTY_STAR_NUMBER, old_star);

	return;
}

uint32_t bounty_manager_t::get_refresh_cost()
{
	auto refresh_cost = GET_CONF(Comprehensive, comprehensive_common::country_bountytask_cost);
	uint32_t val = GET_COMPREHENSIVE_VALUE_1(refresh_cost);
	return val;
}	