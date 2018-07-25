#include "expedition.hpp"
#include "main/game_server_fwd.hpp"
#include "main/game_server.hpp"
#include "cache_key.hpp"
#include "role/role_unify_save.hpp"
#include "role/role_manager.hpp"
#include "tblh/Comprehensive.tbls.h"
#include "tblh/Monster.tbls.h"
#include "tblh/Expedition.tbls.h"
#include "tblh/ExpeditionBox.tbls.h"
#include "tblh/ExpeditionShop.tbls.h"
#include "config/config_manager.hpp"
#include "errcode_enum.hpp"
#include "fight/fight_manager.hpp"
#include "item/drop_manager.hpp"
#include "user_troop/user_troop_mgr.hpp"
#include "mail/mail_manager.hpp"
#include "role/money_manager.hpp"
#include "hero/hero_manager.hpp"
#include "log/log_wrapper.hpp"
#include "achieve/achieve_common.hpp"
#include "item/item_manager.hpp"

USING_NS_COMMON;

expedition_t::expedition_t(uint64_t uid)
	: m_owner(uid)
{
	m_key = cache_key_t::create(m_owner, cache_name::expedition);
}

expedition_t::~expedition_t()
{

}

void expedition_t::load_data(const proto::common::expedition_data& ed)
{
	m_stage_map.clear();
	m_pass_num = ed.pass_num();
	m_refresh_count = ed.refresh_count();
	for (int32_t i = 0; i < ed.stage_list_size(); ++i)
	{
		expedition_stage_ptr p_stage = expedition_stage_ptr(new expedition_stage_t);
		p_stage->load_data(ed.stage_list(i));
		m_stage_map.insert(std::make_pair(p_stage->get_tid(), p_stage));
	}
	if (ed.has_save_data())
	{
		const proto::common::fight_save_data& fsd = ed.save_data();
		for (int32_t i = 0; i < fsd.hero_list_size(); ++i)
		{
			hero_save_data_ptr p_hero = hero_save_data_ptr(new hero_save_data_t);
			p_hero->load_data(fsd.hero_list(i));
			m_hero_map.insert(std::make_pair(p_hero->get_uid(), p_hero));
		}
	}
	m_help_count = ed.help_count();
	m_call_help_count = ed.call_help_count();
	m_glory = ed.glory();
	m_liveness = ed.liveness();
	m_shop_id = ed.shop_id();
	m_is_selled = ed.is_selled();
	for (int32_t i = 0; i < ed.box_list_size(); ++i)
	{
		const proto::common::liveness_box& lb = ed.box_list(i);
		if (lb.state() < 2)
		{
			continue;
		}
		m_box_vec.push_back(lb.liveness());
	}
}

void expedition_t::peek_data(proto::common::expedition_data* p_data)
{
	if (NULL == p_data)
	{
		log_error("NULL == p_data");
		return;
	}
	p_data->set_pass_num(m_pass_num);
	p_data->set_refresh_count(m_refresh_count);
	for (auto it : m_stage_map)
	{
		expedition_stage_ptr p_stage = it.second;
		if (NULL == p_stage)
		{
			continue;
		}
		proto::common::expedition_stage_single* p_single = p_data->add_stage_list();
		p_stage->peek_data(p_single);
	}
	for (auto it : m_hero_map)
	{
		hero_save_data_ptr p_hero = it.second;
		if (NULL == p_hero)
		{
			continue;
		}
		proto::common::hero_save_data* p_single = p_data->mutable_save_data()->add_hero_list();
		p_hero->peek_data(p_single);
	}
	p_data->set_help_count(m_help_count);
	p_data->set_call_help_count(m_call_help_count);
	p_data->set_glory(m_glory);
	p_data->set_liveness(m_liveness);
	p_data->set_shop_id(m_shop_id);
	p_data->set_is_selled(m_is_selled);

	std::map<uint32_t, ExpeditionBox*> confs;
	GET_ALL_CONF(ExpeditionBox, confs);
	for (auto it : confs)
	{
		uint32_t glory = it.first;
		proto::common::liveness_box* p_box = p_data->add_box_list();
		p_box->set_liveness(glory);
		std::vector<uint32_t>::iterator it_vec = std::find(m_box_vec.begin(), m_box_vec.end(), glory);
		if (it_vec != m_box_vec.end())
		{
			p_box->set_state(star_box_state_over);
		}
		else
		{
			if (m_liveness >= glory)
			{
				p_box->set_state(star_box_state_finish);
			}
			else
			{
				p_box->set_state(star_box_state_none);
			}
		}
	}
}


void expedition_t::init_new_data()
{
	m_refresh_count = 1;
	m_pass_num = 0;
	make_data();
	m_help_count = 0;
	m_call_help_count = 1;
	m_glory = 0;
	reset_shop();
}


void expedition_t::reset_shop()
{
	uint32_t rate = 0;
	std::map<uint32_t, ExpeditionShop*> all_confs;
	GET_ALL_CONF(ExpeditionShop, all_confs);
	for (auto it : all_confs)
	{
		auto p_conf = it.second;
		if (NULL == p_conf)
		{
			continue;
		}
		rate += p_conf->rate();
	}
	int32_t rand_val = random_util_t::randMin(0, rate);
	if (-1 == rand_val)
	{
		log_error("ExpeditionShop rand_max invalid");
		return;
	}
	for (auto it : all_confs)
	{
		auto p_conf = it.second;
		if (NULL == p_conf)
		{
			continue;
		}
		if ((uint32_t)rand_val < p_conf->rate())
		{
			m_shop_id = p_conf->id();
			EXPE_LOG("role[%lu] set shop_id[%d]", m_owner, m_shop_id);
			break;
		}
		rand_val -= p_conf->rate();
	}
	m_is_selled = 0;
}

void expedition_t::one_day()
{
	if (0 == m_refresh_count)
	{
		m_refresh_count = 1;
	}
	m_help_count = 1;
	m_glory = 0;
	save_self();
}

void expedition_t::save_self()
{
	proto::common::expedition_data expedition_data;
	peek_data(&expedition_data);
	role_unify_save::add_task(m_owner, m_key, expedition_data);
}

void expedition_t::load_fight_data(const proto::server::eg_expedition_fight_data_reply& reply)
{
	m_stage_map.clear();
	
	for (int32_t i = 0; i < reply.stage_list_size(); ++i)
	{
		expedition_stage_ptr p_stage = expedition_stage_ptr(new expedition_stage_t);
		p_stage->load_data(reply.stage_list(i));
		p_stage->fill_data();
		m_stage_map.insert(std::make_pair(p_stage->get_tid(), p_stage));
	}
	save_self();
}


uint32_t expedition_t::fight(uint32_t stage_id)
{
	if (m_pass_num >= get_max_pass_num())
	{
		log_error("m_pass_num >= get_max_pass_num");
		return errcode_enum::notice_unknown;
	}
	auto it = m_stage_map.find(stage_id);
	if (it == m_stage_map.end())
	{
		log_error("stage[%d] invalid", stage_id);
		return errcode_enum::notice_unknown;
	}
	auto p_stage = it->second;
	if (NULL == p_stage)
	{
		log_error("stage[%d] invalid", stage_id);
		return errcode_enum::notice_unknown;
	}
	if (p_stage->get_pass_num() >= get_stage_pass_num())
	{
		log_error("p_stage->get_pass_num >= get_stage_pass_num");
		return errcode_enum::notice_unknown;
	}
	role_ptr p_role = role_manager_t::find_role(m_owner);
	if (NULL == p_role)
	{
		log_error("NULL == p_role[%lu]", m_owner);
		return errcode_enum::notice_role_null;
	}
	if (!has_hero())
	{
		log_error("p_role[%lu] no hero", m_owner);
		return errcode_enum::notice_expedition_not_enough_hero;
	}
	uint64_t troop_id = p_role->get_troop_id();
	if (troop_id > 0)
	{
		troop_ptr p_troop = game_troop_mgr_t::get_troop(troop_id);
		if (NULL == p_troop)
		{
			log_error("NULL == p_troop[%lu]", troop_id);
			return errcode_enum::user_troop_err_code_not_exsit;
		}
		if (p_troop->get_member_size() > 1)
		{
			if (!p_troop->check_family(p_role->get_family_id()))
			{
				log_error("role[%lu] troop has diff family member", m_owner);
				return errcode_enum::notice_expedition_single_only;
			}
			if (m_call_help_count <= 0)
			{
				log_error("role[%lu] call_help_count not enough", m_owner);
				return errcode_enum::notice_expedition_not_enough_appeal;
			}
			--m_call_help_count;
			log_wrapper_t::send_expedition_log(m_owner, p_role->get_level(), log_enum::source_type_expedition_call_help, stage_id);
		}
	}
	return p_stage->fight(m_owner);
}

void expedition_t::fight_result(uint32_t stage_id, bool is_win, const proto::common::fight_save_data& fsd,
	const proto::common::fight_save_data& esd, const std::vector<uint64_t>& helpers)
{
	role_ptr p_role = role_manager_t::find_role(m_owner);
	if (NULL == p_role)
	{
		log_error("NULL == p_role[%lu]", m_owner);
		return;
	}

	auto it = m_stage_map.find(stage_id);
	if (it == m_stage_map.end())
	{
		log_error("stage[%d] invalid", stage_id);
		return;
	}
	auto p_stage = it->second;
	if (NULL == p_stage)
	{
		log_error("stage[%d] invalid", stage_id);
		return;
	}
	auto p_conf = GET_CONF(Expedition, stage_id);
	if (NULL == p_conf)
	{
		log_error("NULL == p_stage_conf[%d]", stage_id);
		return;
	}
	save_data(fsd);
	p_stage->save_data(esd);
	if (is_win)
	{
		++m_pass_num;
		m_liveness += p_conf->honor();
		m_glory += p_conf->honor();
		notify_glory_to_center();
		p_stage->set_award_num(p_stage->get_award_num() + 1);
		EXPE_LOG("role[%lu] win stage[%d] award_num[%d]", m_owner, stage_id, p_stage->get_award_num());
		p_stage->set_pass_num(p_stage->get_pass_num() + 1);

		log_wrapper_t::send_expedition_log(m_owner, p_role->get_level(), log_enum::source_type_expedition_win, stage_id);
		for (auto helper_uid : helpers)
		{
			role_ptr p_helper = role_manager_t::find_role(helper_uid);
			if (NULL == p_helper)
			{
				log_error("NULL == p_helper[%lu]", helper_uid);
				continue;
			}
			//帮手奖励
			expedition_ptr p_expedition = p_helper->get_expedition();
			if (NULL == p_expedition)
			{
				log_error("NULL == p_expedition[%lu]", helper_uid);
				continue;
			}
			p_expedition->set_help_count(p_expedition->get_help_count() + 1);
			achieve_common_t::notify_progress_state(helper_uid, proto::common::ACHIEVE_EXPEDITION_HELP_OTHERS);	///累计统计
			if (p_expedition->get_help_count() > get_max_help_count())
			{
				continue;
			}
			mail_manager_t::send_mail(helper_uid, proto::common::MAIL_TYPE_SYS, get_help_reward());
			log_wrapper_t::send_expedition_log(helper_uid, p_helper->get_level(), log_enum::source_type_expedition_help, stage_id);
		}
		
		achieve_common_t::notify_progress_state(m_owner, proto::common::ACHIEVE_EXPEDITION_SINGLE_POINT, 0, m_liveness);
		if (m_pass_num >= get_max_pass_num())
		{
			achieve_common_t::notify_progress_state(m_owner, proto::common::ACHIEVE_EXPEDITION_CHALLENGE_PASS_NUMBER);
		}
		achieve_common_t::notify_progress_state(m_owner, proto::common::ACHIEVE_EXPEDITION_WIN_NUMBER, 0);

		activity_mgr_ptr p_activity_mgr = p_role->get_activity_mgr();
		if (NULL != p_activity_mgr)
		{
			p_activity_mgr->activity_done(activity_expedition);
		}
		if (p_stage->get_pass_num() < get_stage_pass_num())
		{
			//没有达到攻打上限请求刷新
			proto::server::ge_expedition_refresh_stage_request req;
			req.set_level(p_role->get_level());
			req.set_stage_id(stage_id);
			env::server->send_msg_to_center(op_cmd::ge_expedition_refresh_stage_request, m_owner, req);
		}
	}
	save_self();
	proto::client::gc_expedition_fight_reply reply;
	reply.set_reply_code(errcode_enum::error_ok);
	reply.set_stage_id(stage_id);
	reply.set_pass_num(p_stage->get_pass_num());
	p_role->send_msg_to_client(op_cmd::gc_expedition_fight_reply, reply);

	proto::server::ge_troop_dissolve_notify ntf;
	env::server->send_msg_to_center(op_cmd::ge_troop_dissolve_notify, m_owner, ntf);

}

uint32_t expedition_t::refresh_request()
{
	role_ptr p_role = role_manager_t::find_role(m_owner);
	if (NULL == p_role)
	{
		log_error("NULL == p_role[%lu]", m_owner);
		return errcode_enum::notice_role_null;
	}
	uint32_t level = p_role->get_level();
	if (level < get_min_level())
	{
		log_error("role[%lu] level not enough", m_owner);
		return errcode_enum::notice_role_levle_not_enough;
	}
	if (m_refresh_count == 0)
	{
		log_error("role[%lu] refresh_count not enough", m_owner);
		return errcode_enum::notice_unknown;
	}
	--m_refresh_count;
	proto::server::ge_expedition_refresh_request msg;
	msg.set_level(level);
	env::server->send_msg_to_center(op_cmd::ge_expedition_refresh_request, m_owner, msg);
	return errcode_enum::error_ok;
}


void expedition_t::refresh_reply(const proto::server::eg_expedition_refresh_reply& reply)
{
	m_pass_num = 0;
	m_call_help_count = 1;
	m_liveness = 0;
	m_box_vec.clear();
	m_stage_map.clear();
	m_hero_map.clear();
	reset_shop();

	for (int32_t i = 0; i < reply.stage_list_size(); ++i)
	{
		expedition_stage_ptr p_stage = expedition_stage_ptr(new expedition_stage_t);
		p_stage->load_data(reply.stage_list(i));
		p_stage->fill_data();
		m_stage_map.insert(std::make_pair(p_stage->get_tid(), p_stage));
	}
	save_self();

	role_ptr p_role = role_manager_t::find_role(m_owner);
	if (NULL == p_role)
	{
		log_error("NULL == p_role[%lu]", m_owner);
		return;
	}
	proto::client::gc_expedition_refresh_reply msg;
	msg.set_reply_code(errcode_enum::error_ok);
	peek_data(msg.mutable_expedition_data());
	p_role->send_msg_to_client(op_cmd::gc_expedition_refresh_reply, msg);

	log_wrapper_t::send_expedition_log(m_owner, p_role->get_level(), log_enum::source_type_expedition_refresh, 0);
}


void expedition_t::refresh_stage_reply(const proto::server::eg_expedition_refresh_stage_reply& reply)
{
	const proto::common::expedition_stage_single& ess = reply.stage_data();
	uint32_t stage_id = ess.stage_id();
	auto it = m_stage_map.find(stage_id);
	if (it == m_stage_map.end())
	{
		log_error("role[%lu] stage[%d] null", m_owner, stage_id);
		return;
	}
	expedition_stage_ptr p_stage = it->second;
	if (NULL == p_stage)
	{
		log_error("role[%lu] stage[%d] null", m_owner, stage_id);
		return;
	}
	p_stage->load_fight_data(ess);
	p_stage->fill_data();
}

uint32_t expedition_t::help_request(const std::vector<uint64_t>& helper_list)
{
	role_ptr p_role = role_manager_t::find_role(m_owner);
	if (NULL == p_role)
	{
		log_error("NULL == p_role[%lu]", m_owner);
		return errcode_enum::notice_role_null;
	}
	
	if (m_call_help_count <= 0)
	{
		log_error("role[%lu] call_help_count not enough", m_owner);
		return errcode_enum::notice_unknown;
	}
	
	proto::server::ge_expedition_help_request msg;
	for (auto helper_uid : helper_list)
	{
		msg.add_helper_list(helper_uid);
	}
	env::server->send_msg_to_center(op_cmd::ge_expedition_help_request, m_owner, msg);
	return errcode_enum::error_ok;
}


uint32_t expedition_t::shop_request(uint32_t buy_style, proto::client::gc_expedition_shop_reply& reply)
{
	role_ptr p_role = role_manager_t::find_role(m_owner);
	if (NULL == p_role)
	{
		log_error("NULL == p_role[%lu]", m_owner);
		return errcode_enum::notice_role_null;
	}
	if (m_is_selled > 0)
	{
		log_error("role[%lu] already buyed", m_owner);
		return errcode_enum::notice_unknown;
	}
	auto p_shop_conf = GET_CONF(ExpeditionShop, m_shop_id);
	if (NULL == p_shop_conf)
	{
		log_error("NULL == p_shop_conf[%d]", m_shop_id);
		return errcode_enum::notice_unknown;
	}

	if (buy_style == 0)
	{
		if (!money_manager_t::use_money(p_role, proto::common::MONEY_TYPE_YUANBAO, p_shop_conf->gold(), log_enum::source_type_expedition_shop, m_shop_id, true, reply.mutable_rcd()))
		{
			log_error("role[%lu] gold not enough", m_owner);
			return errcode_enum::notice_gold_money_not_enough;
		}
	}
	else
	{
		if (!money_manager_t::use_money(p_role, proto::common::MONEY_TYPE_CONTRIBUTION, p_shop_conf->cont(), log_enum::source_type_expedition_shop, m_shop_id, true, reply.mutable_rcd()))
		{
			log_error("role[%lu] CONTRIBUTION not enough", m_owner);
			return errcode_enum::notice_expedition_not_enough_glory;
		}
		//achieve_common_t::notify_progress_state(m_owner, proto::common::ACHIEVE_EXPEDITION_STORE_TOTAL_COST_HONOUR, 0, p_shop_conf->glory());
	}
	m_is_selled = 1;
	item_manager_t::add_item(p_role, p_shop_conf->item_id_count().first, p_shop_conf->item_id_count().second, log_enum::source_type_expedition_shop, m_shop_id, reply.mutable_rcd());
	log_wrapper_t::send_expedition_log(m_owner, p_role->get_level(), log_enum::source_type_expedition_shop, m_shop_id);
	proto::common::personal_info* p_info = reply.mutable_rcd()->mutable_per_info();
	p_role->get_personal_info(p_info);
	save_self();
	achieve_common_t::notify_progress_state(m_owner, proto::common::ACHIEVE_EXPEDITION_STORE_BUY_NUMBER);
	return errcode_enum::error_ok;
}

uint32_t expedition_t::award(uint32_t stage_id, proto::common::role_change_data* p_data)
{
	auto it = m_stage_map.find(stage_id);
	if (it == m_stage_map.end())
	{
		log_error("stage[%d] invalid", stage_id);
		return errcode_enum::notice_unknown;
	}
	auto p_stage = it->second;
	if (NULL == p_stage)
	{
		log_error("stage[%d] invalid", stage_id);
		return errcode_enum::notice_unknown;
	}
	auto p_conf = GET_CONF(Expedition, stage_id);
	if (NULL == p_conf)
	{
		log_error("NULL == p_stage_conf[%d]", stage_id);
		return errcode_enum::notice_unknown;
	}
	role_ptr p_role = role_manager_t::find_role(m_owner);
	if (NULL == p_role)
	{
		log_error("NULL == p_role[%lu]", m_owner);
		return errcode_enum::notice_role_null;
	}
	if (p_stage->get_award_num() == 0)
	{
		log_error("stage[%d] no award", stage_id);
		return errcode_enum::notice_role_null;
	}
	for (uint32_t i = 0; i < p_stage->get_award_num(); ++i)
	{
		drop_manager_t::drop(p_role, p_conf->drop_id(), log_enum::source_type_expedition_award, proto::common::drop_sys_type_expedition, stage_id, p_data);
		EXPE_LOG("role[%lu] stage[%d] award", m_owner, stage_id);
	}
	p_stage->set_award_num(0);
	save_self();
	log_wrapper_t::send_expedition_log(m_owner, p_role->get_level(), log_enum::source_type_expedition_award, stage_id);

	proto::common::personal_info* p_info = p_data->mutable_per_info();
	p_role->get_personal_info(p_info);
	return errcode_enum::error_ok;
}

uint32_t expedition_t::box(uint32_t box_id, proto::client::gc_expedition_box_reply& reply)
{
	auto p_conf = GET_CONF(ExpeditionBox, box_id);
	if (NULL == p_conf)
	{
		log_error("NULL == p_box_conf[%d]", box_id);
		return errcode_enum::notice_unknown;
	}
	if (m_liveness < p_conf->glory())
	{
		log_error("role[%lu] liveness not enough", m_owner);
		return errcode_enum::notice_unknown;
	}
	std::vector<uint32_t>::iterator it_vec = std::find(m_box_vec.begin(), m_box_vec.end(), box_id);
	if (it_vec != m_box_vec.end())
	{
		log_error("role[%lu] glory_box[%d] already get", m_owner, box_id);
		return errcode_enum::notice_unknown;
	}
	
	role_ptr p_role = role_manager_t::find_role(m_owner);
	if (NULL == p_role)
	{
		log_error("NULL == p_role[%lu]", m_owner);
		return errcode_enum::notice_role_null;
	}
	m_box_vec.push_back(box_id);
	save_self();
	uint32_t drop_id = p_conf->reward();
	drop_manager_t::drop(p_role, drop_id, log_enum::source_type_expedition_box, proto::common::drop_sys_type_expedition, box_id, reply.mutable_rcd());
	log_wrapper_t::send_expedition_log(m_owner, p_role->get_level(), log_enum::source_type_expedition_box, box_id);
	
	proto::common::personal_info* p_info = reply.mutable_rcd()->mutable_per_info();
	p_role->get_personal_info(p_info);

	std::map<uint32_t, ExpeditionBox*> confs;
	GET_ALL_CONF(ExpeditionBox, confs);
	for (auto it : confs)
	{
		uint32_t glory = it.first;
		proto::common::liveness_box* p_box = reply.add_box_list();
		p_box->set_liveness(glory);
		std::vector<uint32_t>::iterator it_vec = std::find(m_box_vec.begin(), m_box_vec.end(), glory);
		if (it_vec != m_box_vec.end())
		{
			p_box->set_state(star_box_state_over);
		}
		else
		{
			if (m_liveness >= glory)
			{
				p_box->set_state(star_box_state_finish);
			}
			else
			{
				p_box->set_state(star_box_state_none);
			}
		}
	}
	return errcode_enum::error_ok;
}


void expedition_t::notify_glory_to_center()
{
	role_ptr p_role = role_manager_t::find_role(m_owner);
	if (NULL == p_role)
	{
		log_error("NULL == p_role[%lu]", m_owner);
		return;
	}
	proto::server::ge_family_update_glory_notify ntf;
	ntf.set_glory(m_glory);
	ntf.set_family_id(p_role->get_family_id());
	env::server->send_msg_to_center(op_cmd::ge_family_update_glory_notify, m_owner, ntf);
}


uint32_t expedition_t::get_stage_pass_num()
{
	return 4;
}

uint32_t expedition_t::get_max_pass_num()
{
	auto max_pass_num = GET_CONF(Comprehensive, comprehensive_common::glory_max_times);
	uint32_t val = GET_COMPREHENSIVE_VALUE_1(max_pass_num);
	return val;
}


uint32_t expedition_t::get_min_level()
{
	return 30;
}


uint32_t expedition_t::get_max_help_count()
{
	auto max_help_count = GET_CONF(Comprehensive, comprehensive_common::max_appeal_reward_number);
	uint32_t val = GET_COMPREHENSIVE_VALUE_1(max_help_count);
	return val;
}

uint32_t expedition_t::get_help_reward()
{
	auto help_reward = GET_CONF(Comprehensive, comprehensive_common::appeal_reward_id);
	uint32_t val = GET_COMPREHENSIVE_VALUE_1(help_reward);
	return val;
}


uint32_t expedition_t::get_hero_min_level()
{
	auto hero_min_level = GET_CONF(Comprehensive, comprehensive_common::glory_hero_limit_level);
	uint32_t val = GET_COMPREHENSIVE_VALUE_1(hero_min_level);
	return val;
}

int32_t expedition_t::get_hp(uint64_t hero_uid)
{
	auto it = m_hero_map.find(hero_uid);
	if (it == m_hero_map.end())
	{
		return 100;
	}
	hero_save_data_ptr p_hero = it->second;
	if (NULL == p_hero)
	{
		return 100;
	}
	return (int32_t)p_hero->get_hp();
}

int32_t expedition_t::get_skill_cd(uint64_t hero_uid, uint32_t skill_id)
{
	auto it = m_hero_map.find(hero_uid);
	if (it == m_hero_map.end())
	{
		return -1;
	}
	hero_save_data_ptr p_hero = it->second;
	if (NULL == p_hero)
	{
		return -1;
	}
	return p_hero->get_skill_cd(skill_id);
}


void expedition_t::save_data(const proto::common::fight_save_data& fsd)
{
	role_ptr p_role = role_manager_t::find_role(m_owner);
	if (NULL == p_role)
	{
		log_error("NULL == p_role[%lu]", m_owner);
		return;
	}
	for (int32_t i = 0; i < fsd.hero_list_size(); ++i)
	{
		const proto::common::hero_save_data& hsd = fsd.hero_list(i);
		if (hsd.hero_uid() == p_role->get_main_hero_uid())
		{
			continue;
		}
		auto it = m_hero_map.find(hsd.hero_uid());
		if (it == m_hero_map.end())
		{
			hero_save_data_ptr p_hero = hero_save_data_ptr(new hero_save_data_t);
			p_hero->load_data(hsd);
			m_hero_map.insert(std::make_pair(p_hero->get_uid(), p_hero));
		}
		else
		{
			hero_save_data_ptr p_hero = it->second;
			if (NULL != p_hero)
			{
				p_hero->save_data(hsd);
			}
		}
	}
}


uint32_t expedition_t::shop_hp(uint64_t hero_uid)
{
	auto it = m_hero_map.find(hero_uid);
	if (it == m_hero_map.end())
	{
		log_error("hero[%lu] hp full", hero_uid);
		return errcode_enum::notice_unknown;
	}
	hero_save_data_ptr p_hero = it->second;
	if (NULL == p_hero)
	{
		log_error("hero[%lu] hp full", hero_uid);
		return errcode_enum::notice_unknown;
	}
	if (p_hero->get_hp() >= 100)
	{
		log_error("hero[%lu] hp full", hero_uid);
		return errcode_enum::notice_unknown;
	}
	if (p_hero->get_hp() == 0)
	{
		log_error("hero[%lu] dead", hero_uid);
		return errcode_enum::notice_unknown;
	}
	p_hero->set_hp(100);
	return errcode_enum::error_ok;
}


uint32_t expedition_t::shop_cd(uint64_t hero_uid)
{
	auto it = m_hero_map.find(hero_uid);
	if (it == m_hero_map.end())
	{
		log_error("hero[%lu] cd empty", hero_uid);
		return errcode_enum::notice_unknown;
	}
	hero_save_data_ptr p_hero = it->second;
	if (NULL == p_hero)
	{
		log_error("hero[%lu] cd empty", hero_uid);
		return errcode_enum::notice_unknown;
	}
	if (!p_hero->has_cd())
	{
		log_error("hero[%lu] cd empty", hero_uid);
		return errcode_enum::notice_unknown;
	}
	p_hero->clear_cd();
	return errcode_enum::error_ok;
}

uint32_t expedition_t::shop_all(uint64_t hero_uid)
{
	auto it = m_hero_map.find(hero_uid);
	if (it == m_hero_map.end())
	{
		log_error("hero[%lu] state full", hero_uid);
		return errcode_enum::notice_unknown;
	}
	hero_save_data_ptr p_hero = it->second;
	if (NULL == p_hero)
	{
		log_error("hero[%lu] state full", hero_uid);
		return errcode_enum::notice_unknown;
	}
	if (p_hero->get_hp() >= 100 && !p_hero->has_cd())
	{
		log_error("hero[%lu] state full", hero_uid);
		return errcode_enum::notice_unknown;
	}
	
	p_hero->set_hp(100);
	p_hero->clear_cd();
	return errcode_enum::error_ok;
}


bool expedition_t::has_hero()
{
	role_ptr p_role = role_manager_t::find_role(m_owner);
	if (NULL == p_role)
	{
		log_error("NULL == p_role[%lu]", m_owner);
		return false;
	}
	uint64_t main_hero_uid = p_role->get_main_hero_uid();
	uint32_t hero_min_level = get_hero_min_level();

	std::vector<hero_ptr> hero_list;
	hero_manager_t::get_all_heroes(p_role, hero_list);
	
	for (uint32_t i = 0; i < hero_list.size(); ++i)
	{
		hero_ptr p_hero = hero_list[i];
		if (NULL == p_hero)
		{
			continue;
		}
		uint64_t hero_uid = p_hero->get_unique_id();
		if (hero_uid == main_hero_uid)
		{
			continue;
		}
		if (p_hero->get_current_level() < hero_min_level)
		{
			continue;
		}
		auto it_save = m_hero_map.find(hero_uid);
		if (it_save == m_hero_map.end())
		{
			return true;
		}
		hero_save_data_ptr p_data = it_save->second;
		if (NULL == p_data)
		{
			continue;
		}
		if (p_data->get_hp() > 0)
		{
			return true;
		}
	}
	return false;
}

void expedition_t::make_data()
{
	role_ptr p_role = role_manager_t::find_role(m_owner);
	if (NULL == p_role)
	{
		log_error("NULL == p_role[%lu]", m_owner);
		return;
	}
	uint32_t level = p_role->get_level();
	if (level < get_min_level())
	{
		level = get_min_level();
	}
	proto::server::ge_expedition_fight_data_request msg;
	msg.set_level(level);
	env::server->send_msg_to_center(op_cmd::ge_expedition_fight_data_request, m_owner, msg);
}

expedition_stage_t::expedition_stage_t()
{

}


expedition_stage_t::~expedition_stage_t()
{

}

void expedition_stage_t::load_data(const proto::common::expedition_stage_single& ed)
{
	m_tid = ed.stage_id();
	m_pass_num = ed.pass_num();
	m_award_num = ed.award_num();
	for (int32_t i = 0; i < ed.stage_info_size(); ++i)
	{
		mirror_fight_data_ptr p_fight_data = mirror_fight_data_ptr(new mirror_fight_data_t);
		p_fight_data->load_data(ed.stage_info(i));
		m_fight_data.push_back(p_fight_data);
	}
}


void expedition_stage_t::load_fight_data(const proto::common::expedition_stage_single& ed)
{
	m_fight_data.clear();
	for (int32_t i = 0; i < ed.stage_info_size(); ++i)
	{
		mirror_fight_data_ptr p_fight_data = mirror_fight_data_ptr(new mirror_fight_data_t);
		p_fight_data->load_data(ed.stage_info(i));
		m_fight_data.push_back(p_fight_data);
	}
}

void expedition_stage_t::peek_data(proto::common::expedition_stage_single* p_data)
{
	if (NULL == p_data)
	{
		log_error("NULL == p_data");
		return;
	}
	p_data->set_pass_num(m_pass_num);
	p_data->set_stage_id(m_tid);
	p_data->set_award_num(m_award_num);
	for (auto p_fight_data : m_fight_data)
	{
		if (NULL == p_fight_data)
		{
			continue;
		}
		proto::common::mirror_fight_data* p_single = p_data->add_stage_info();
		p_fight_data->peek_data(p_single);
	}
}


void expedition_stage_t::fill_data()
{
	for (auto p_fight_data : m_fight_data)
	{
		if (NULL == p_fight_data)
		{
			continue;
		}
		p_fight_data->fill_data();
	}
}

uint32_t expedition_stage_t::fight(uint64_t owner)
{
	if (m_fight_data.empty())
	{
		log_error("role[%lu] stage[%d] empty", owner, m_tid);
		return errcode_enum::notice_unknown;
	}
	auto p_fight_data = m_fight_data[0];
	if (NULL == p_fight_data)
	{
		log_error("role[%lu] stage[%d] invalid", owner, m_tid);
		return errcode_enum::notice_unknown;
	}
	role_ptr p_role = role_manager_t::find_role(owner);
	if (NULL == p_role)
	{
		log_error("NULL == p_role[%lu]", owner);
		return errcode_enum::notice_role_null;
	}

	proto::common::mirror_fight_data mfd;
	p_fight_data->peek_data(&mfd);
	proto::common::fight_param fp;
	fp.set_stage_id(m_tid);
	fp.set_type(proto::common::fight_type_expedition);
	fight_manager_t::fight_pve(p_role, mfd, &fp);
	
	return errcode_enum::error_ok;
}

void expedition_stage_t::save_data(const proto::common::fight_save_data& esd)
{
	if (m_fight_data.empty())
	{
		log_error("stage[%d] m_fight_data empty", m_tid);
		return;
	}
	auto p_fight_data = m_fight_data[0];
	if (NULL == p_fight_data)
	{
		log_error("stage[%d] invalid", m_tid);
		return;
	}
	p_fight_data->save_data(esd);
}
