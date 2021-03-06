#include "combat.hpp"
#include "fight_hero.hpp"
#include "common/config_mgr.h"
#include "common/func_time.hpp"
#include "fight_manager.hpp"
#include "tblh/Tactic.tbls.h"
#include "tblh/attr_enum.hpp"
#include "tblh/Comprehensive.tbls.h"
#include "tblh/comprehensive_common.hpp"
#include "tblh/SkillTargetAI.tbls.h"
#include "tblh/GuideFightTable.tbls.h"
#include "tblh/FightTypeTable.tbls.h"
#include "user_troop/user_troop_mgr.hpp"
#include "combat_log.hpp"
#include "script/script_fight_hero.h"
#include "object/object_manager.hpp"

// static const uint32_t MAX_ROUND = 20;
// static const uint32_t MAX_LOADING_TIME = 16;
// static const uint32_t MAX_PREPARE_TIME = 31;
// static const uint32_t MAX_SP = 600;
// static const uint32_t RECOVER_SP = 200;

uint32_t combat_t::s_sync_tick_msec = 1000;
uint32_t combat_t::init_fight_constant = 0;

combat_t::combat_t(uint64_t fight_uid)
	: m_fight_uid(fight_uid)
{

}

combat_t::~combat_t()
{
	close();
}

void combat_t::close()
{
	m_timer.stop();
	if (NULL != m_end_timer)
	{
		m_end_timer->cancel();
	}
	m_camp_data.clear();
	m_form.clear();
	m_process.clear();
	m_p_att_fight_hero = NULL;

	for (auto pair : m_heros)
	{
		if (pair.second != NULL)
			pair.second->close();
	}
	for (auto pair : m_hide_heros)
	{
		if (pair.second != NULL)
			pair.second->close();
	}
	for (auto pair : m_objs)
	{
		if (pair.second != NULL)
			pair.second->close();
	}

	m_first_att_hero_list.clear();
	m_alive_hero_list.clear();
	m_heros.clear();
	m_hide_heros.clear();
	m_objs.clear();
	m_watching_roles.clear();
}

bool combat_t::init(proto::common::fight_data& fd, bool can_form_before_fight, combat_type type)
{

	auto p_conf = GET_CONF(Comprehensive, comprehensive_common::boss_sync_hp_time);
	if ( NULL != p_conf )
		s_sync_tick_msec = GET_COMPREHENSIVE_VALUE_1(p_conf);


	auto p_conf_constant = GET_CONF(Comprehensive, comprehensive_common::fight_constant);
	if ( NULL != p_conf_constant)
		init_fight_constant = GET_COMPREHENSIVE_VALUE_1(p_conf_constant);

	proto::common::fight_camp* camp1 = fd.mutable_self();
	proto::common::fight_camp* camp2 = fd.mutable_enemy();

	if (!init_fight_obj(camp1->mutable_objs()))
	{
		log_error("combat_t::init combat_camp_1 obj init fail!");
		return false;
	}

	if (!init_fight_obj(camp2->mutable_objs()))
	{
		log_error("combat_t::init combat_camp_2 obj init fail!");
		return false;
	}
	
	if (!init_loading_fight_hero(camp1->mutable_heroes()))
	{
		log_error("combat_t::init combat_camp_1 hero init fail!");
		return false;
	}

	if (!init_loading_fight_hero(camp2->mutable_heroes()))
	{
		log_error("combat_t::init combat_camp_2 hero init fail!");
		return false;
	}

	if (!init_loading_fight_hero(camp1->mutable_help_heroes()))
	{
		log_error("combat_t::init combat_camp_1 help_hero init fail!");
		return false;
	}

	if (!init_loading_fight_hero(camp2->mutable_help_heroes()))
	{
		log_error("combat_t::init combat_camp_2 help_hero init fail!");
		return false;
	}

	m_fight_data.CopyFrom(fd);

	m_combat_type = type;

	set_is_replay(get_fight_type());
//fight_hero_on_init();
// 	if (check_tactic_valid(proto::common::combat_camp_1))
// 	{
// 		m_fight_data.mutable_self()->set_tactic_switch(1);
// 		do_tactic_buff(proto::common::combat_camp_1, true);
// 	}
// 
// 	if (check_tactic_valid(proto::common::combat_camp_2))
// 	{
// 		m_fight_data.mutable_enemy()->set_tactic_switch(1);
// 		do_tactic_buff(proto::common::combat_camp_2, true);
// 	}
	loading_start();
	
	m_can_form_before_fight = can_form_before_fight;
	m_start_time = common::time_util_t::now_time();

	auto conf = GET_CONF(Comprehensive, comprehensive_common::battle_auto_close_time);
	uint32_t auto_close_time = GET_COMPREHENSIVE_VALUE_1(conf);

	m_end_timer.reset(new boost::asio::deadline_timer(env::server->get_ios()));
	m_end_timer->expires_from_now(boost::posix_time::seconds(auto_close_time));
	m_end_timer->async_wait(boost::bind(&combat_t::on_end_timer, boost::weak_ptr<combat_t>(shared_from_this()), boost::asio::placeholders::error));

	return true;
}

void combat_t::set_is_replay(uint32_t  type){
	//设置是否要回放	
	FightTypeTable *p_conf = GET_CONF(FightTypeTable, type);
	if (NULL == p_conf) {
		log_error("combat[%u] replay set is not found", type);
		return;
	}

	m_is_replay = false;
	if (p_conf->is_replay() == 1) {
		m_is_replay = true;
	}
}

formation_ptr combat_t::get_formation(uint32_t camp)
{
	fight_obj_ptr p_obj = fight_obj_ptr();
	role_ptr p_role = role_ptr();
	for (auto iter : m_objs)
	{
		p_obj = iter.second;
		if (p_obj != NULL)
		{
			if (p_obj->get_type() == proto::common::SCENEOBJECT_USER && 
				p_obj->get_camp() == camp)
			{
				p_role = role_manager_t::find_role(p_obj->get_uid());
				if (NULL == p_role)
				{
					log_error("combat_t get_formation p_role NULL uid[%lu]", p_obj->get_uid());
					return NULL;
				}
				switch (m_combat_type)
				{
				case combat_type_single_role:
					{
						if(get_fight_type() == proto::common::fight_type_expedition)
							return p_role->m_formation_mgr.get_formation(proto::common::formation_type_expedition);
						else if (get_fight_type() == proto::common::fight_type_family_war || 
								 get_fight_type() == proto::common::fight_type_family_pk)
							return p_role->m_formation_mgr.get_formation(proto::common::formation_type_family);
						else
							return p_role->m_formation_mgr.get_cur_formation();
					}
					break;
				case combat_type_troop:
					{
						troop_ptr p_troop = game_troop_mgr_t::get_troop(p_role->get_troop_id());
						if (NULL == p_troop)
						{
							log_error("combat get_formation p_troop NULL id[%lu]", p_role->get_troop_id());
							return NULL;
						}
						return p_troop->get_formation_mgr().get_cur_formation();
					}
					break;
				default:
					break;
				}
			}
		}
	}
	return NULL;
}


// bool combat_t::change_tactic(uint64_t role_uid, uint32_t tactic_id)
// {
// 	fight_obj_ptr p_obj = find_obj(role_uid);
// 	if (NULL == p_obj)
// 	{
// 		log_error("combat_t p_obj NULL uid[%lu]", role_uid);
// 		return false;
// 	}
// 
// 	Tactic* tactic_conf = GET_CONF(Tactic, tactic_id);
// 	if (NULL == tactic_conf)
// 	{
// 		log_error("combat_t tactic_conf NULL id[%d]", tactic_id);
// 		return false;
// 	}
// 
// 	uint32_t camp = p_obj->get_camp();
// 
// 	if (camp == proto::common::combat_camp_1)
// 	{
// 		m_fight_data.mutable_self()->set_tactic(tactic_id);
// 	}
// 	else
// 	{
// 		m_fight_data.mutable_enemy()->set_tactic(tactic_id);
// 	}
// 	return true;
// }
// 
// bool combat_t::change_form(formation_ptr form)
// {
// 	if (NULL == form)
// 	{
// 		log_error("combat_t form NULL");
// 		return false;
// 	}
// 	return true;

void combat_t::fight_hero_on_init()
{
	for (auto hero : m_heros)
	{
		if( NULL == hero.second )
			continue;

		hero.second->on_init();
	}
}

bool combat_t::init_fight_obj(proto::common::obj_data* od)
{
	if (NULL == od)
	{
		log_error("od is null");
		return false;
	}

	for (int i = 0; i < od->obj_size(); ++i)
	{
		proto::common::obj_single* os = od->mutable_obj(i);
		fight_obj_ptr obj(new fight_obj_t());
		if (!obj->init(os))
		{
			log_error("combat_t::init_fight_obj fail %s", os->object_uid().c_str());
			continue;
		}
		
		if (obj->get_type() != proto::common::SCENEOBJECT_USER )
		{
			obj->set_state(proto::common::combat_obj_state_loading_end);
		}

		m_objs.insert(std::make_pair(string_util_t::string_to_uint64(os->object_uid()), obj));
		m_camp_data[obj->get_camp()].camp_obj_type = obj->get_type();
	}

	return true;
}

bool combat_t::init_loading_fight_hero(proto::common::hero_data* hd)
{
	if (NULL == hd)
	{
		log_error("hd is null");
		return false;
	}

	int hp = 0;
	int max_hp = 0;

	for (int i = 0; i < hd->hero_list_size(); ++i)
	{
		proto::common::hero_single* hs = hd->mutable_hero_list(i);
		
		for (const proto::common::attr_single& attr : hs->fight_attr().attr().attrs())
		{
			if (attr.type() == attrtype_enum::hp)
			{
				hp = attr.value();
				break;
			}
			else if (attr.type() == attrtype_enum::max_hp)
			{
				hp = attr.value();
				max_hp = attr.value();
				break;
			}
		}

		proto::common::attr_single* as = hs->mutable_fight_attr()->mutable_attr()->add_attrs();
		if (NULL != as)
		{
			as->set_type(attrtype_enum::hp);
			if (hs->fight_attr().start().pos() == hs->fight_attr().pos())
			{
				as->set_value(hs->fight_attr().start().hp());
			}
			else
			{
				as->set_value(hp);
			}
			
			if (hs->has_save_hp())
			{
				if (hs->save_hp() >= 0)
				{
					uint32_t cur_hp = hs->save_hp() * max_hp / 100;
					as->set_value(cur_hp);
				}
			}
			FIGHT_LOG("[%lu]fight_hero_t loading: tid[%u] [%u][%d]", get_fight_uid(),  hs->base_attr().id(), as->type(), as->value());
		}
	}

	return true;
}

bool combat_t::init_fight_hero(proto::common::hero_data* hd)
{
	if (NULL == hd)
	{
		log_error("hd is null");
		return false;
	}

	for (int i = 0; i < hd->hero_list_size(); ++i)
	{
		proto::common::hero_single* hs = hd->mutable_hero_list(i);
		uint64_t fight_hero_uid = string_util_t::string_to_uint64(hs->base_attr().fight_hero_uid());
		fight_hero_ptr hero(new fight_hero_t());
		if (!hero->init(shared_from_this(), hs))
		{
			continue;
		}

// 		proto::common::attr_single* as = hs->mutable_fight_attr()->mutable_attr()->add_attrs();
// 		if (NULL != as)
// 		{
// 			as->set_type(attrtype_enum::hp);
// 			as->set_value(hero->get_attr(attrtype_enum::hp));
// 		}
		// 后续波次的先不加入当前战斗武将列表 加入等待列表
		if (hero->get_wave_num() > 1)
		{
			m_wave_heros.insert(std::make_pair(fight_hero_uid, hero));
			continue;
		}

		m_heros.insert(std::make_pair(fight_hero_uid, hero));
		m_form[hs->fight_attr().camp()].insert(std::make_pair(hs->fight_attr().pos(), string_util_t::string_to_uint64(hs->base_attr().fight_hero_uid())));
	}

	return true;
}

bool combat_t::init_fight_help_hero(proto::common::hero_data* hd)
{
	if (NULL == hd)
	{
		log_error("hd is null");
		return false;
	}

	for (int i = 0; i < hd->hero_list_size(); ++i)
	{
		proto::common::hero_single* hs = hd->mutable_hero_list(i);
		uint64_t fight_hero_uid = string_util_t::string_to_uint64(hs->base_attr().fight_hero_uid());
		fight_hero_ptr hero(new fight_hero_t());
		if (!hero->init(shared_from_this(), hs))
		{
			log_error("combat_t::init_fight_hero fail %lu", fight_hero_uid);
			continue;
		}
		hero->set_is_story(true);

		proto::common::attr_single* as = hs->mutable_fight_attr()->mutable_attr()->add_attrs();
		if (NULL != as)
		{
			as->set_type(attrtype_enum::hp);
			as->set_value(hero->get_attr(attrtype_enum::hp));
		}

		m_hide_heros.insert(std::make_pair(fight_hero_uid, hero));
	}

	return true;
}

bool combat_t::loading_start()
{
	auto max_loading_time_conf = GET_CONF(Comprehensive, comprehensive_common::MAX_LOADING_TIME);
	uint32_t max_loading_time = GET_COMPREHENSIVE_VALUE_1(max_loading_time_conf);
	max_loading_time += 1;	// 服务器时间要比客户端大一点

	m_timer.start(max_loading_time, boost::weak_ptr<combat_t>(shared_from_this()), &combat_t::on_loading_timer, env::server->get_ios());
	m_end_time = time_util_t::now_time() + max_loading_time;

	proto::client::gc_combat_start_loading_ntf ntf;

	FightAgainst *p_conf = GET_CONF(FightAgainst, get_against_id());
	if (NULL != p_conf) {
		proto::common::combat_end_ahead *p_cond = ntf.mutable_cond();
		p_cond->set_cond_type(p_conf->get_end_fight_cond_type() );
		const EndFightCond &end_cond = p_conf->get_end_fight_cond_value( p_conf->get_end_fight_cond_type() );

		switch (p_conf->get_end_fight_cond_type()) {
			case END_FIGHT_END_DEFAULT:
			break;
		case END_FIGHT_END_MONST_ID: 
			p_cond->set_param1(end_cond.param1);
			break;
		case END_FIGHT_END_ROUND: 
			p_cond->set_param1(end_cond.param1);
			break;
		}
	}

	fill_combat_data(ntf.mutable_combat(), NULL, true);
	send_msg_to_all(op_cmd::gc_combat_start_loading_ntf, ntf);
	auto_loading();
	syn_combat_obj_state();
	return true;
}

bool combat_t::is_auto_ignore_lineup(uint64_t uid)
{
	role_ptr p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
		return false;

	// 判断这个地方是否可以跳过布阵
	// 如果不可以布阵则自动跳过
	if (!m_can_form_before_fight)
		return true;

	return p_role->is_auto_ignore_lineup(get_fight_type());
}

void combat_t::auto_ignore_lineup(uint64_t uid)
{
	if(is_auto_ignore_lineup(uid))
		combat_before_form_end(uid);
}

bool combat_t::loading_end(uint64_t uid)
{
	if (m_step == proto::common::combat_step_end)
	{
		log_error("combat is end");
		return false;
	}

	fight_obj_ptr obj = find_obj(uid);
	if (NULL == obj)
	{
		log_error("loading_end can not find obj %lu", uid);
		return false;
	}

    if (m_step != proto::common::combat_step_loading)
    {
        log_error("loading_end step error %lu %d", uid, (int)m_step);
        return false;
    }

    if ( obj->is_monster() == false && obj->get_state() != proto::common::combat_obj_state_loading )
    {
        log_error("loading_end obj state error %lu %d", uid, (int)obj->get_state());
        return false;
    }

    obj->set_state(proto::common::combat_obj_state_loading_end);
    if (!check_loading_finish())
    {
        auto_ignore_lineup(uid);
        return true;
    }
	
	bool b_result = false;
	if (m_can_form_before_fight)
	{
		// 开始布阵
		b_result = combat_before_form_start();
		auto_ignore_lineup(uid);
	}
	else
		b_result = round_start_before();
	return b_result;
}

bool combat_t::combat_before_form_start()
{
	m_step = proto::common::combat_step_form;

	auto single_pve_form_time_conf = GET_CONF(Comprehensive, comprehensive_common::fight_single_pve_prewar_array_time);
	auto form_time_conf = GET_CONF(Comprehensive, comprehensive_common::fight_non_single_pve_prewar_array_time);
	

	// 服务器时间比客户端大一点
	uint32_t form_max_time = 0;
	uint32_t server_add_time = 1000;

	if (is_single_pve())
	{
		form_max_time = GET_COMPREHENSIVE_VALUE_1(single_pve_form_time_conf);
	}
	else
	{
		form_max_time = GET_COMPREHENSIVE_VALUE_1(form_time_conf);
	}

	m_end_time = time_util_t::now_time() + form_max_time;
	form_max_time += server_add_time;

	syn_combat_data();

	m_timer.start(form_max_time, boost::weak_ptr<combat_t>(shared_from_this()), &combat_t::on_form_timer, env::server->get_ios());
	m_end_time = time_util_t::now_time() + form_max_time;

	return true;
}

bool combat_t::check_all_form_end()
{
	fight_obj_ptr p_obj = fight_obj_ptr();
	fight_obj_map::iterator iter;
	// 有一个未准备的 就继续等待 
	iter = m_objs.begin();
	for (; iter != m_objs.end(); ++iter)
	{
		p_obj = iter->second;
		if (NULL == p_obj) {
			continue;
		}
		// 非玩家不处理
		if (!p_obj->is_object_type())
			continue;
		if (p_obj != NULL && !p_obj->is_ready())
			return false;
	}
	return true;
}

void combat_t::do_form_end(uint64_t uid)
{
	fight_obj_ptr p_obj = fight_obj_ptr();
	fight_obj_map::iterator iter = m_objs.find(uid);
	if (iter == m_objs.end())
		return;
	
	p_obj = iter->second;
	if (p_obj == NULL)
		return;

	// 非玩家不处理
	if (!p_obj->is_object_type())
		return;
	if (p_obj->is_ready())
		return;
	
	p_obj->ready();
	proto::client::gc_combat_before_form_ready_reply reply;
	reply.set_fight_uid(string_util_t::uint64_to_string(m_fight_uid));
	reply.set_uid(string_util_t::uint64_to_string(uid));

	send_msg_to_all(op_cmd::gc_combat_before_form_ready_reply, reply);
	
}

bool combat_t::combat_before_form_end(uint64_t uid)
{
	// 布阵结束
	do_form_end(uid);

	// 有一个未准备的 就继续等待 
	if (!check_all_form_end())
		return false;

	if (m_step != proto::common::combat_step_form)
		return false;
	// 全部准备后重新设置本场战斗的两房阵营英雄数据
	reset_all_camp_form_data();

	auto conf = GET_CONF(Comprehensive, comprehensive_common::fight_deploy_finish_ani_time);
	uint32_t deploy_finish_ani_time = GET_COMPREHENSIVE_VALUE_1(conf);

	if (0 == deploy_finish_ani_time)
	{
		log_error("combat_t deploy_finish_ani_time = 0");
		return false;
	}

	m_end_time = time_util_t::now_time() + deploy_finish_ani_time;

	round_start_before();
	return true;
}

bool combat_t::reset_all_camp_form_data()
{
	//role_ptr p_role = role_ptr();
	//m_fight_data.mutable_self()->mutable_heroes()->Clear();
	//m_fight_data.mutable_common()->mutable_param()->mutable_start()->mutable_self()->Clear();
	//m_fight_data.mutable_enemy()->mutable_heroes()->Clear();
	//m_fight_data.mutable_common()->mutable_param()->mutable_start()->mutable_enemy()->Clear();


	reset_single_camp_form_data(proto::common::combat_camp_1);
	reset_single_camp_form_data(proto::common::combat_camp_2);

	proto::client::gc_combat_reset_form_notify ntf;

	fill_combat_data(ntf.mutable_combat(), NULL, true);
	send_msg_to_all(op_cmd::gc_combat_reset_form_notify, ntf);

	return true;
}

bool combat_t::reset_single_camp_form_data(uint32_t camp)
{
	/*formation_ptr form = get_formation(camp);
	if (form == NULL)
	{
		return false;
	}

	switch (camp)
	{
	case proto::common::combat_camp_1:
		{
			m_fight_data.mutable_self()->mutable_heroes()->Clear();
			m_fight_data.mutable_self()->set_tactic(form->get_tactic_id());
		}
		break;
	case proto::common::combat_camp_2:
		{
			m_fight_data.mutable_enemy()->mutable_heroes()->Clear();
			m_fight_data.mutable_enemy()->set_tactic(form->get_tactic_id());
		}
		break;
	default:
		break;
	}*/
	
	object_base_ptr p_role = object_base_ptr();

	for (auto objpair : m_objs)
	{
		if (NULL == objpair.second)
		{
			continue;
		}
		// 非玩家不处理
		if ( !objpair.second->is_object_type()|| objpair.second->get_camp() != camp)
			continue;

		p_role = object_manager_t::find_object(objpair.second->get_object_id_type());
		if (NULL == p_role)
			continue;

		switch (camp)
		{
		case proto::common::combat_camp_1:
			{
				p_role->fill_hero_data(
					proto::common::combat_camp_1, 
					p_role->get_camp_unique_offset(proto::common::combat_camp_1),
					objpair.second->get_team_member_size(),
					get_fight_type(), 
					m_fight_data.mutable_self()->mutable_heroes(),
					m_fight_data.mutable_common()->mutable_param()->mutable_start()->mutable_self(),
					objpair.second->get_team_member_pos());

				if (!init_loading_fight_hero(m_fight_data.mutable_self()->mutable_heroes()))
				{
					log_error("combat_t::reset_single_camp_form_data combat_camp_1 hero init fail!");
					return false;
				}
			}
			break;
		case proto::common::combat_camp_2:
			{
				p_role->fill_hero_data(
					proto::common::combat_camp_2, 
					p_role->get_camp_unique_offset(proto::common::combat_camp_2),
					objpair.second->get_team_member_size(),
					get_fight_type(), 
					m_fight_data.mutable_enemy()->mutable_heroes(),
					m_fight_data.mutable_common()->mutable_param()->mutable_start()->mutable_enemy(),
					objpair.second->get_team_member_pos());

				if (!init_loading_fight_hero(m_fight_data.mutable_enemy()->mutable_heroes()))
				{
					log_error("combat_t::reset_single_camp_form_data combat_camp_2 hero init fail!");
					return false;
				}
			}
			break;
		default:
			break;
		}
	}
	return false;
}

bool combat_t::round_start_before()
{
	proto::common::fight_camp* camp1 = m_fight_data.mutable_self();
	proto::common::fight_camp* camp2 = m_fight_data.mutable_enemy();

	if (!init_fight_hero(camp1->mutable_heroes()))
	{
		log_error("combat_t::init combat_camp_1 hero init fail!");
		return false;
	}

	if (!init_fight_hero(camp2->mutable_heroes()))
	{
		log_error("combat_t::init combat_camp_2 hero init fail!");
		return false;
	}

	if (!init_fight_help_hero(camp1->mutable_help_heroes()))
	{
		log_error("combat_t::init combat_camp_1 help_hero init fail!");
		return false;
	}

	if (!init_fight_help_hero(camp2->mutable_help_heroes()))
	{
		log_error("combat_t::init combat_camp_2 help_hero init fail!");
		return false;
	}

	check_event(combat_event_step_type_combat_start);
// 	if (check_tactic_valid(proto::common::combat_camp_1))
// 	{
// 		m_fight_data.mutable_self()->set_tactic_switch(1);
// 		do_tactic_buff(proto::common::combat_camp_1, true);
// 	}
// 
// 	if (check_tactic_valid(proto::common::combat_camp_2))
// 	{
// 		m_fight_data.mutable_enemy()->set_tactic_switch(1);
// 		do_tactic_buff(proto::common::combat_camp_2, true);
// 	}
	fight_hero_on_init();
	//释放光环
	distribute_special_buff();

	m_step = proto::common::combat_step_round;
	m_round = 1;
	fight_start_event();
	syn_combat_data(&(m_process[m_hero_att_round]));
	m_hero_att_round = 1;

	return round_start();
}


void combat_t::fight_start_event()
{
	fight_hero_ptr p_hero = fight_hero_ptr();
	fight_hero_map::iterator it;
	for (it = m_heros.begin(); it != m_heros.end(); ++it)
	{
		p_hero = it->second;
		if (NULL == p_hero)
		{
			log_error("fight_start_event p_hero NULL");
			continue;
		}
		//it->second->get_script_proxy()->on_event(event_on_fight_begin);
		p_hero->on_event(event_on_fight_begin, p_hero);
	}
}

bool combat_t::round_start()
{
	// 回合开始阶段检查事件
	check_event(combat_event_step_type_round_start);

	// 第一次出手 判断主动阵营
	if (m_hero_att_round == 1)
	{
		//先获取攻击顺序
		reset_max_speed();
	}

	find_cur_att_hero();

	if (NULL == m_p_att_fight_hero)
	{
		log_error("combat_t m_p_att_fight_hero NULL");
		return false;
	}

	m_state = proto::common::combat_state_prepare;

	for (auto objpair : m_objs)
	{
		if (NULL == objpair.second)
		{
			continue;
		}

		objpair.second->on_round_start_imp(get_attack_obj_uid());
	}

	// 判断是否在这里触发了 BUFF事件
	if (m_p_att_fight_hero->on_round_start_imp())
	{
		// 触发了BUFF事件，需要将这个武将的信息重新分发
		// 判断这个武将是不是死亡了
		// add by hy 这里貌似不用通知,强制通知 可能倒是很多事件客户端重复触发
		//syn_combat_data(&(m_process[m_hero_att_round]));
	}
	
	//如果部署时死亡了(中毒)
	if (m_p_att_fight_hero->is_dead()) {
		m_p_att_fight_hero->on_dead(NULL, NULL);
	}
	return prepare_start();
}

void combat_t::find_cur_att_hero()
{
	// 先获取攻击顺序
	m_alive_hero_list.clear();
	get_alive_attack_order(m_alive_hero_list);

	fight_hero_ptr p_fight_hero = fight_hero_ptr();
	fight_hero_ptr p_min_count_fight_hero = fight_hero_ptr();
	int32_t speed_dec = 0;
	int32_t speed_remain = 0;
	uint32_t temp_add_speed_count = 0;
	uint32_t final_add_speed_count = 99999;
	bool b_find_first_att_hero = false;
	m_p_att_fight_hero = NULL;

	if (m_first_att_hero_list.size() > 0)
	{
		for (uint32_t i = 0; i < m_first_att_hero_list.size(); ++i)
		{
			m_p_att_fight_hero = m_first_att_hero_list[0];
			if (NULL == m_p_att_fight_hero)
			{
				log_error("combat_t m_p_att_fight_hero NULL");
				return;
			}
			if (!m_p_att_fight_hero->is_dead())
			{
				b_find_first_att_hero = true;
				return;
			}
		}
	}

	if(!b_find_first_att_hero)
	{
		// 循环查找 需要增加的速度最低次数 及武将
		for (uint32_t i = 0; i < m_alive_hero_list.size(); ++i)
		{
			p_fight_hero = m_alive_hero_list[i];
			if (NULL == p_fight_hero)
			{
				log_error("combat_t p_fight_hero NULL");
				continue;
			}
			if (p_fight_hero->is_in_mirage())//处于幻境下的武将不增加行动条也不参与出手行动条计算
			{
				continue;
			}

			uint32_t att_speed = p_fight_hero->get_att_speed();
			if (att_speed == 0) {
				//log_error("find_cur_att_hero speed = 0 hero[%lu][%d]", p_fight_hero->get_uid(), p_fight_hero->get_tid());
				continue;
			}

			uint32_t base_speed = p_fight_hero->get_speed();
			if (0 == base_speed) {
				log_error("find_fight_hero[%u] base_speed is 0", p_fight_hero->get_tid());
				continue;
			}
			speed_dec = m_max_att_speed - att_speed;
			speed_remain = (speed_dec % att_speed) > 0 ? 1 : 0;//speed_remain其实就是用于向上取整
			temp_add_speed_count = speed_dec / base_speed + speed_remain;
			if (temp_add_speed_count < final_add_speed_count)
			{
				final_add_speed_count = temp_add_speed_count;
				p_min_count_fight_hero = p_fight_hero;
			}
			//log_error("before add_attr_speed hero_uid[%lu] max_att_speed[%u], speed_dec[%u], temp_add_speed_count[%u], speed_remain[%u], speed[%u]", p_fight_hero->get_uid(), m_max_att_speed, speed_dec, temp_add_speed_count, speed_remain, att_speed);
		}

		if (final_add_speed_count == 0)
		{
			log_error("combat_t final_add_speed_count = 0 hero_tid[%d] hero_uid[%lu] att_speed[%d] order_att_speed[%d]",
			p_min_count_fight_hero->get_tid(), p_min_count_fight_hero->get_uid(), p_min_count_fight_hero->get_att_speed(),
			p_min_count_fight_hero->get_order_att_speed());
			return;
		}

		// 出手速度的加成次数 全体存活武将按自己的速度值*次数增加出手速度值
		int32_t add_speed_value = 0;
		for (uint32_t i = 0; i < m_alive_hero_list.size(); ++i)
		{
			p_fight_hero = m_alive_hero_list[i];
			if (NULL == p_fight_hero)
			{
				log_error("combat_t p_fight_hero NULL");
				continue;
			}
			if (p_fight_hero->is_in_mirage())//处于幻境状态下的武将不增加行动条
			{
				//log_warn("mirage hero att speed is   %u", p_fight_hero->get_att_speed());
				continue;
			}

			add_speed_value = p_fight_hero->get_speed() * final_add_speed_count;
			p_fight_hero->set_att_speed(add_speed_value + p_fight_hero->get_att_speed());
			//log_error(" after add_attr_speed hero_uid[%lu] total:[%u], round[%u], base[%u], add_value[%u]", p_fight_hero->get_uid(), p_fight_hero->get_att_speed(), final_add_speed_count, p_fight_hero->get_speed(), add_speed_value);
		}

		m_alive_hero_list.clear();
		// 加完再排序
		get_alive_attack_order(m_alive_hero_list);
		if(m_alive_hero_list.size() > 0)
			m_p_att_fight_hero = m_alive_hero_list[0];

		//重新添加first_attr_speed	
		for (auto iter : m_alive_hero_list) {
			if (NULL == iter) {
				continue;
			}
			if (iter->get_att_speed() >= m_max_att_speed ) {
				add_first_att_hero_list( iter );
				iter->set_full_speed(0);
			}
		}
	}

	//log，正式代码需要注释掉！
//	for (uint32_t i = 0; i < m_alive_hero_list.size(); ++i)
//	{
//		p_fight_hero = m_alive_hero_list[i];
//		log_error(" after add_attr_speed hero_uid[%lu] total:[%u], round[%u], base[%u]", p_fight_hero->get_uid(), p_fight_hero->get_att_speed(), final_add_speed_count, p_fight_hero->get_speed());
//	}
}


bool combat_t::prepare_start()
{
	auto prepare_time_no_single = GET_CONF(Comprehensive, comprehensive_common::MAX_PREPARE_TIME_NON_SINGLE);
	auto prepare_time_single = GET_CONF(Comprehensive, comprehensive_common::MAX_PREPARE_TIME_SINGLE);

	// 服务器时间比客户端大一点
	uint32_t prepare_max_time = m_fight_data.common().param().prepare_time();
	uint32_t server_add_time = 1000;

	if (prepare_max_time == 0)
	{
		if (is_single_pve())
		{
			prepare_max_time = GET_COMPREHENSIVE_VALUE_1(prepare_time_single);
			prepare_max_time += server_add_time;
		}
		else
		{
			prepare_max_time = GET_COMPREHENSIVE_VALUE_1(prepare_time_no_single);
			prepare_max_time += server_add_time;
		}
	}


	if (NULL == m_p_att_fight_hero)
	{
		log_error("combat_t m_p_att_fight_hero NULL");
		return false;
	}

	// 在这里有可能出现死亡状态，因为这个武将在回合开始的时候被扣血了
	if (m_p_att_fight_hero->is_dead() ||
		m_p_att_fight_hero->is_in_control() || 
		m_p_att_fight_hero->get_attr(attrtype_enum::taunt) > 0 ||
		m_p_att_fight_hero->get_attr(attrtype_enum::confused) > 0)
	{
		auto control_wait_time_conf = GET_CONF(Comprehensive, comprehensive_common::hero_control_wait_time);
		uint32_t hero_control_wait_time = GET_COMPREHENSIVE_VALUE_1(control_wait_time_conf);
		prepare_max_time = hero_control_wait_time;
	}

	m_timer.start(prepare_max_time, boost::weak_ptr<combat_t>(shared_from_this()), &combat_t::on_prepare_timer, env::server->get_ios());
	m_end_time = time_util_t::now_time() + prepare_max_time;
	syn_combat_data(&(m_process[m_hero_att_round]));
	syn_combat_obj_state();
	auto_disposition();

	return true;
}

bool combat_t::prepare_end()
{
	return play_start();
}

fight_hero_ptr combat_t::get_att_fight_hero()
{
	return m_p_att_fight_hero;
}

void combat_t::on_round_end()
{
	for (auto iter_t : m_heros ) {
		fight_hero_ptr p_hero = iter_t.second;
		if (p_hero == NULL)
			continue;
		p_hero->get_count_data().resetByOnceRound();
		p_hero->clear_phase_fight_info();
	}
	m_beattack_hero_list.clear();
}

void combat_t::add_beattack_hero(fight_hero_ptr fight_hero)
{
	if (NULL == fight_hero)
		return;
	m_beattack_hero_list[fight_hero->get_uid()] = fight_hero;
}


bool combat_t::play_start()
{
	m_state = proto::common::combat_state_play;
	auto fault_tolerant_conf = GET_CONF(Comprehensive, comprehensive_common::fight_fault_tolerant_time);
	uint32_t fight_fault_tolerant_time = GET_COMPREHENSIVE_VALUE_1(fault_tolerant_conf);
	
	if (NULL == m_p_att_fight_hero)
	{
		log_error("combat_t m_p_att_fight_hero NULL");
		return false;
	}

	// 播放时移除出先手队列
	remove_first_att_hero_list(m_p_att_fight_hero);

	//add_combat_act(proto::common::combat_act_type_hero_attack, 0, m_p_att_fight_hero->get_uid(), 0);
	m_p_att_fight_hero->on_round_start();


	uint32_t hero_att_num = 0;
	if (!m_p_att_fight_hero->is_dead())
	{
		m_p_att_fight_hero->do_attack();
	}

	fight_manager_t::on_combat_after_attack(shared_from_this());
	
	hero_att_num = m_p_att_fight_hero->get_do_attack_num();
	m_p_att_fight_hero->on_round_end();

	deal_round_use_skill_list();
	remove_all_round_use_skill_list();

	if ( m_p_att_fight_hero->is_dead() ) {
		m_p_att_fight_hero->on_dead(NULL, NULL);
	}

	bool b_change_wave = check_wave_change();

	m_state = proto::common::combat_state_fight;
	// 预留给客户端的时间 服务器时间不能太及时 客户端一卡就懵逼了, 这里要加上在游戏里转移目标的时间
	uint32_t play_time = ceil( get_fight_play_time(m_hero_att_round) * hero_att_num ) + fight_fault_tolerant_time;
	if (m_story_addi_time != 0)
	{
		play_time += m_story_addi_time;
		m_story_addi_time = 0;
	}

	//重置波数记录，用于play_end中重置速度
	m_is_new_wave = false;
	if (b_change_wave)
	{
		auto wave_wait_conf = GET_CONF(Comprehensive, comprehensive_common::wait_time_every_stage);
		uint32_t wave_wait_time = GET_COMPREHENSIVE_VALUE_1(wave_wait_conf);
		play_time += wave_wait_time;
		m_is_new_wave = true;
	}

	//TODO test
	//play_time = 3;
	m_timer.start(play_time, boost::weak_ptr<combat_t>(shared_from_this()), &combat_t::on_play_timer, env::server->get_ios());
	m_end_time = time_util_t::now_time() + play_time / 1000;
	syn_combat_data(&(m_process[m_hero_att_round]));
	// 同步完数据后增加波次值
	if (b_change_wave)
	{
		++m_wave;
	}

	for (auto objpair : m_objs)
	{
		if (NULL == objpair.second)
		{
			continue;
		}
		objpair.second->on_play_start();
	}

	auto_play();

	syn_combat_obj_state();

	on_round_end();

	return true;
}

bool combat_t::play_end()
{
	if (NULL == m_p_att_fight_hero)
	{
		log_error("combat_t m_p_att_fight_hero NULL");
		return false;
	}

	// 回合结束阶段检查事件
	check_event(combat_evnet_step_type_play_end);

	fight_hero_ptr p_fight_hero = fight_hero_ptr();
	std::vector<fight_hero_ptr> temp_speed_key_heroes;
	std::vector<fight_hero_ptr>::iterator iter = m_alive_hero_list.begin();
	
	// 遍历存活武将 给除了攻击武将的其他武将增加出手速度
	for (; iter != m_alive_hero_list.end(); ++iter)
	{
		p_fight_hero = *iter;
		if (p_fight_hero == NULL)
		{
			log_error("combat_t p_fight_hero NULL");
			continue;
		}

		if (p_fight_hero->is_dead())
		{
			remove_first_att_hero_list(p_fight_hero);
			continue;
		}

		//如果有立即获得新回合
		if (p_fight_hero->get_count_data().is_get_new_round == true) {
			temp_speed_key_heroes.clear();
			temp_speed_key_heroes.push_back(p_fight_hero);
			p_fight_hero->get_count_data().is_get_new_round = false;
			break;
		}

		// 超过阀值的加入先手队列
		if (p_fight_hero->get_att_speed() >= m_max_att_speed)
		{
			add_first_att_hero_list(p_fight_hero);
		}
		// 小于阀值的看是不是在队列里 在的话移除
		else if (p_fight_hero->get_att_speed() < m_max_att_speed)
		{
			remove_first_att_hero_list(p_fight_hero);
		}

		// 本次攻击武将不参与计算
		/*
		if (p_fight_hero->get_uid() == m_p_att_fight_hero->get_uid())
		{
			continue;
		}
		*/
		// 加自身速度值
		if (!p_fight_hero->is_in_mirage())//幻境状态武将不增加出手速度
		{
			p_fight_hero->set_att_speed(p_fight_hero->get_att_speed() + p_fight_hero->get_speed());
		}

		// 加完速度值后大于阀值扔到临时队列
		if (p_fight_hero->get_att_speed() >= m_max_att_speed)
		{
			temp_speed_key_heroes.push_back(p_fight_hero);
		}
	}

	//20180515，侍剑要求切波次以后，重新计算速
	if ( m_is_new_wave == true ) {
		reset_max_speed();
	}

	// 排序临时队列
	sort_att_speed_list(temp_speed_key_heroes);
	// 顺序加到队列
	for (uint32_t i = 0; i < temp_speed_key_heroes.size(); ++i)
	{
		add_first_att_hero_list(temp_speed_key_heroes[i]);
	}

	return round_end();
}

bool combat_t::round_end()
{
	if (NULL == m_p_att_fight_hero)
	{
		log_error("combat_t m_p_att_fight_hero NULL");
		return false;
	}
	
	m_p_att_fight_hero->on_round_end_imp();

	for (auto objpair : m_objs)
	{
		if (NULL == objpair.second)
		{
			continue;
		}

		objpair.second->on_round_end_imp();
	}

	m_round_add_buff_count = 0;

	// 回合结束阶段检查事件
	check_event(combat_event_step_type_round_end);


	if (check_combat_end())
	{
		return combat_end();
	}

	++m_hero_att_round;
	m_round += 1;
	return round_start();
}

bool combat_t::combat_end()
{
	if (NULL != m_end_timer)
	{
		m_end_timer->cancel();
	}

	m_step = proto::common::combat_step_end;

	// 结算
	proto::client::gc_combat_result_ntf ntf;
	fill_combat_data(ntf.mutable_combat());
	fill_count_data(ntf.mutable_count_data());
	ntf.mutable_result()->CopyFrom(m_combat_result);
	fight_manager_t::on_combat_result(m_combat_result, shared_from_this(), ntf);
	
	for (auto objpair : m_objs)
	{
		if (NULL == objpair.second)
			continue;

		if (objpair.second->is_escape_end())
			continue;

		objpair.second->on_combat_end(shared_from_this());
	}

	remove_all_watching_roles();

	return true;
}

// 添加先手队列
void combat_t::add_first_att_hero_list(fight_hero_ptr p_fight_hero)
{
	if (NULL == p_fight_hero)
	{
		log_error("add_first_att_hero_list p_hero NULL");
		return;
	}

	// 不在队列 扔队尾
	int32_t order_pos = p_fight_hero->get_first_att_order_pos();
	if (order_pos == -1)
	{
		m_first_att_hero_list.push_back(p_fight_hero);
		order_pos = m_first_att_hero_list.size() - 1;
		p_fight_hero->set_first_att_order_pos(order_pos);
	}
	// 没人和只有一个的情况不处理
	if (m_first_att_hero_list.size() <= 1)
		return;

	// 在第一个位置 也不处理
	if (order_pos == 0)
		return;

	// 有拉条 遍历插队
	if (p_fight_hero->get_att_more_speed() > 0)
	{
		// 异常
		if ((uint32_t)order_pos >= m_first_att_hero_list.size())
		{
			log_error("add_first_att_hero_list order_pos[%d] >= order_list[%d]", order_pos, (int32_t)m_first_att_hero_list.size());
			return;
		}
		// 自己
		fight_hero_ptr p_self = m_first_att_hero_list[order_pos];
		if (NULL == p_self)
		{
			log_error("add_first_att_hero_list p_self NULL pos[%d]", order_pos);
			return;
		}

		fight_hero_ptr p_compare_hero = fight_hero_ptr();
		for (int32_t i = order_pos - 1; i >= 0; --i)
		{
			p_compare_hero = m_first_att_hero_list[i];
			if (NULL == p_compare_hero)
			{
				log_error("add_first_att_hero_list p_compare_hero NULL");
				continue;
			}
			if (!fight_hero_t::hero_att_more_speed_sort(p_self, p_compare_hero))
			{
				m_first_att_hero_list[i] = p_self;
				m_first_att_hero_list[i + 1] = p_compare_hero;
				// 更新位置
				p_self->set_first_att_order_pos(i);
				p_compare_hero->set_first_att_order_pos(i+1);
			}
			else
				break;
		}
	}
}

void combat_t::remove_first_att_hero_list(fight_hero_ptr p_fight_hero)
{
	if (NULL == p_fight_hero)
	{
		log_error("remove_first_att_hero_list p_hero NULL");
		return;
	}
	int32_t order_pos = p_fight_hero->get_first_att_order_pos();
	if (order_pos == -1)
		return;

	if ((uint32_t)order_pos >= m_first_att_hero_list.size())
	{
		log_error("remove_first_att_hero_list order_pos[%d] >= order_size[%d]", order_pos, (int32_t)m_first_att_hero_list.size());
		return;
	}

	p_fight_hero->set_first_att_order_pos(-1);
	m_first_att_hero_list.erase(m_first_att_hero_list.begin() + order_pos);
	
	// 更新后续队位置
	for (uint32_t i = order_pos; i < m_first_att_hero_list.size(); ++i)
	{
		if (m_first_att_hero_list[i] != NULL)
			m_first_att_hero_list[i]->set_first_att_order_pos(i);
	}
}

void combat_t::sort_first_att_hero_list()
{
	if (m_first_att_hero_list.size() <= 1)
		return;
	common::Sort::Qsort(m_first_att_hero_list, 0, m_first_att_hero_list.size() - 1, fight_hero_t::hero_att_more_speed_sort);
}

void combat_t::sort_att_speed_list(std::vector<fight_hero_ptr> &res)
{
	if (res.size() <= 1)
		return;
	common::Sort::Qsort(res, 0, res.size() - 1, fight_hero_t::hero_speed_sort);
}

void combat_t::check_event(combat_event_step_type type)
{
	check_hide_event(type);
}

void combat_t::clear_enemy_camp_death_heros()
{
	std::vector<uint64_t> remove_list;
	const pos_hero* camp_heros = get_camp_pos_hero(proto::common::combat_camp_2);
	if (NULL == camp_heros)
	{
		return;
	}

	for (auto pospair : *camp_heros)
	{
		fight_hero_ptr hero = find_hero(pospair.second);
		if (NULL == hero)
		{
			continue;
		}
		if (!hero->is_dead())
			continue;
		remove_list.push_back(pospair.second);
	}

	m_form[proto::common::combat_camp_2].clear();

	fight_hero_map::iterator it;
	for (int i = 0; i < (int)remove_list.size(); ++i)
	{
		 it = m_heros.find(remove_list[i]);
		 if (it == m_heros.end())
			 continue;
		 m_heros.erase(it);
	}
}

bool combat_t::check_wave_change()
{
	if (!check_camp_all_dead(proto::common::combat_camp_2))
		return false;

	if (m_wave_heros.size() == 0)
		return false;
	
	// 这暂时不可以增加当前波次值 待同步完数据后++
	uint32_t new_wave = m_wave + 1;
	add_combat_act(proto::common::combat_act_type_syn_wave, 1, 0, new_wave);

	std::vector<fight_hero_ptr> result;
	fight_hero_ptr p_hero = NULL;
	get_camp_alive_heros(result, proto::common::combat_camp_1);

	for (uint32_t i = 0; i < result.size(); ++i)
	{
		p_hero = result[i];
		if (p_hero != NULL)
		{
			p_hero->reset_wave_change_data();
		}
	}

	clear_enemy_camp_death_heros();

	fight_hero_map::iterator iter = m_wave_heros.begin();
	for (; iter != m_wave_heros.end();)
	{
		p_hero = iter->second;
		if (p_hero == NULL)
		{
			log_error("check_wave_change p_hero[%lu] NULL", iter->first);
			++iter;
			continue;
		}
		if (p_hero->get_wave_num() == new_wave)
		{
			//新的波数怪物也要清空速度条数据
			p_hero->clear_att_speed();
			m_heros.insert(std::make_pair(iter->first, iter->second));
			m_form[proto::common::combat_camp_2].insert(std::make_pair(p_hero->get_pos(), p_hero->get_uid()));
			iter = m_wave_heros.erase(iter);
		}
		else
		{
			++iter;
		}
	}

	distribute_special_buff();
	// 清空先手队列
	m_first_att_hero_list.clear();
	// 清空存活武将列表
	m_alive_hero_list.clear();
	return true;
}

uint32_t combat_t::get_team_member_size(uint64_t uid)
{
	fight_obj_ptr role = find_obj(uid);
	if (NULL == role)
		return 0;
	return get_obj_count( role->get_camp());
}

void combat_t::check_hide_event(combat_event_step_type type)
{
	if (m_hide_heros.size() == 0)
		return;

	fight_hero_map::iterator iter = m_hide_heros.begin();
	MonsterHero* conf = NULL;
	fight_hero_ptr p_hero = fight_hero_ptr();
	fight_hero_map remove_map;
	
	for (; iter != m_hide_heros.end(); ++iter)
	{
		p_hero = iter->second;
		if (NULL == p_hero)
		{
			log_error("combat_t p_hero NULL");
			continue;
		}
		conf = GET_CONF(MonsterHero, p_hero->get_tid());
		if (NULL == conf)
		{
			log_error("combat_t conf NULL tid[%d]", p_hero->get_tid());
			continue;
		}
		switch (conf->show_cond())
		{
		case show_cond_type_round_start:
			{
				if (type != combat_event_step_type_round_start)
					return;
				if (get_hero_att_round() >= conf->show_cond_param(0))
				{
					add_hide_hero_to_combat(p_hero);
					remove_map.insert(std::make_pair(p_hero->get_uid(), p_hero));
				}
			}
			break;
		case show_cond_type_hero_die:
			{
				if (type != combat_event_step_type_round_start)
					return;
				fight_hero_ptr p_find_hero = get_hero_by_camp_and_tid(conf->show_cond_param(0), conf->show_cond_param(1));
				if (NULL == p_find_hero)
				{
					log_error("combat_t p_hero NULL cond_param[%d %d]", conf->show_cond_param(0), conf->show_cond_param(1));
				}
				else
				{
					if (p_find_hero->is_dead())
					{
						add_hide_hero_to_combat(p_hero);
						remove_map.insert(std::make_pair(p_hero->get_uid(), p_hero));
					}
				}
			}
			break;
		case show_cond_type_hero_hp:
			{
				if (type != combat_event_step_type_round_start)
					return;
				fight_hero_ptr p_find_hero = get_hero_by_camp_and_tid(conf->show_cond_param(0), conf->show_cond_param(1));
				if (NULL == p_find_hero)
				{
					log_error("combat_t p_hero NULL cond_param[%d %d]", conf->show_cond_param(0), conf->show_cond_param(1));
				}
				else
				{
					if (p_find_hero->get_hp() <= conf->show_cond_param(2))
					{
						add_hide_hero_to_combat(p_hero);
						remove_map.insert(std::make_pair(p_hero->get_uid(), p_hero));
					}
				}
			}
			break;
		case show_cond_type_round_end:
			{
				if (type != combat_event_step_type_round_end)
					return;
				if (get_hero_att_round() >= conf->show_cond_param(0))
				{
					add_hide_hero_to_combat(p_hero);
					remove_map.insert(std::make_pair(p_hero->get_uid(), p_hero));
				}
			}
			break;
		case show_cond_type_combat_start:
			{
				if (type != combat_event_step_type_combat_start)
					return;
				add_hide_hero_to_combat(p_hero);
				remove_map.insert(std::make_pair(p_hero->get_uid(), p_hero));
			}
			break;
		case show_cond_type_play_end:
		{
			if (type != combat_evnet_step_type_play_end)
				return;
			if (get_hero_att_round() >= conf->show_cond_param(0))
			{
				add_hide_hero_to_combat(p_hero);
				remove_map.insert(std::make_pair(p_hero->get_uid(), p_hero));
			}
		}
		break;
		default:
			break;
		}
	}

	uint64_t temp_uid = 0;
	for (auto it : remove_map)
	{
		temp_uid = it.first;
		m_hide_heros.erase(temp_uid);
	}
}

void combat_t::add_hide_hero_to_combat(fight_hero_ptr p_hero)
{
	if (NULL == p_hero)
	{
		log_error("p_hero NULL");
		return;
	}
	uint32_t pos = p_hero->get_pos();
	if (p_hero->get_camp() == proto::common::combat_camp_1)
	{
		if (get_hero_by_camp_and_pos(p_hero->get_camp(), pos) != NULL)
			pos = get_empty_pos(p_hero->get_camp());
	}

	m_heros.insert(std::make_pair(p_hero->get_uid(), p_hero));
	m_alive_hero_list.push_back(p_hero);
	m_form[p_hero->get_camp()].insert(std::make_pair(pos, p_hero->get_uid()));

	p_hero->on_init();
	m_story_addi_time = battle_story_addi_time;
}

void combat_t::set_win_info(proto::common::combat_camp win_camp){
	m_combat_result.set_win_camp(win_camp);
	m_combat_result.set_result(proto::common::combat_fight_result_win);
	if (get_initiative_camp() == win_camp)
		m_combat_result.mutable_win_camp_tasks()->add_finish_tasks()->set_finish_task(proto::common::combat_finish_task_type_first_attack);
	m_combat_result.mutable_win_camp_tasks()->add_finish_tasks()->set_finish_task(proto::common::combat_finish_task_type_enemy_all_die);
	m_combat_result.mutable_win_camp_tasks()->add_finish_tasks()->set_finish_task(proto::common::combat_finish_task_type_alive_more);
}

bool combat_t::check_end_fight_ahead() {
	FightAgainst *p_conf = GET_CONF(FightAgainst, get_against_id());
	if (NULL == p_conf) {
		log_error("combat FightAginst [%u] is NULL", get_against_id());
		return false;
	}

	const EndFightCond &end_cond = p_conf->get_end_fight_cond_value( p_conf->get_end_fight_cond_type() );

	proto::common::combat_camp win_camp = proto::common::combat_camp_1;
	switch ( p_conf->get_end_fight_cond_type() ){
		case END_FIGHT_END_DEFAULT:
			return false;
		case END_FIGHT_END_MONST_ID: {
			if ( is_last_wave() == false || check_camp_hero_dead(proto::common::combat_camp_2, end_cond.param1 ) == false) {
				return false;
			}
			//如果打死了赢的是本方阵营
			win_camp = proto::common::combat_camp_1;
		}
			break;
		case END_FIGHT_END_ROUND: {
			//如果超出回合赢的是对方阵营,其他走默认规则
			if ( end_cond.param1 >= get_hero_att_round() ) {
				return false;
			}
			win_camp = proto::common::combat_camp_2;
		}
			break;
		default:
			return false;
	}
	set_win_info(win_camp);
	return true;
}

bool combat_t::check_combat_end()
{
	//检测提前结束战斗条件
	if (check_end_fight_ahead() == true) {
		return true;
	}

	uint32_t max_round = m_fight_data.common().param().max_rounds();
	if (check_camp_all_dead(proto::common::combat_camp_1) && check_camp_all_dead(proto::common::combat_camp_2)) // 同时死亡
	{
		uint32_t npc_camp = 0;
		if (is_pve(npc_camp))
		{
			m_combat_result.set_win_camp(npc_camp);
			m_combat_result.set_result(proto::common::combat_fight_result_win);
		}
		else
		{
			m_combat_result.set_win_camp(get_initiative_camp());
			m_combat_result.set_result(proto::common::combat_fight_result_win);
			m_combat_result.mutable_win_camp_tasks()->add_finish_tasks()->set_finish_task(proto::common::combat_finish_task_type_first_attack);
			m_combat_result.mutable_win_camp_tasks()->add_finish_tasks()->set_finish_task(proto::common::combat_finish_task_type_enemy_all_die);
			m_combat_result.mutable_win_camp_tasks()->add_finish_tasks()->set_finish_task(proto::common::combat_finish_task_type_alive_more);
		}

		return true;
	}
	else if (check_camp_all_dead(proto::common::combat_camp_1)) // 阵营1死亡
	{
		m_combat_result.set_win_camp(proto::common::combat_camp_2);
		m_combat_result.set_result(proto::common::combat_fight_result_win);
		if(get_initiative_camp() == proto::common::combat_camp_2)
			m_combat_result.mutable_win_camp_tasks()->add_finish_tasks()->set_finish_task(proto::common::combat_finish_task_type_first_attack);
		m_combat_result.mutable_win_camp_tasks()->add_finish_tasks()->set_finish_task(proto::common::combat_finish_task_type_enemy_all_die);
		m_combat_result.mutable_win_camp_tasks()->add_finish_tasks()->set_finish_task(proto::common::combat_finish_task_type_alive_more);
		return true;
	}
	else if (check_camp_all_dead(proto::common::combat_camp_2)) // 阵营2死亡
	{
		m_combat_result.set_win_camp(proto::common::combat_camp_1);
		m_combat_result.set_result(proto::common::combat_fight_result_win);
		if (get_initiative_camp() == proto::common::combat_camp_1)
			m_combat_result.mutable_win_camp_tasks()->add_finish_tasks()->set_finish_task(proto::common::combat_finish_task_type_first_attack);
		m_combat_result.mutable_win_camp_tasks()->add_finish_tasks()->set_finish_task(proto::common::combat_finish_task_type_enemy_all_die);
		m_combat_result.mutable_win_camp_tasks()->add_finish_tasks()->set_finish_task(proto::common::combat_finish_task_type_alive_more);
		return true;
	}
	else if (get_hero_att_round() >= max_round) // 达到最大回合
	{
		uint32_t npc_camp = 0;
		if (is_pve(npc_camp))
		{
			m_combat_result.set_win_camp(npc_camp);
			m_combat_result.set_result(proto::common::combat_fight_result_win);
		}
		else
		{
			uint32_t alive_num_1 = get_alive_hero_num(proto::common::combat_camp_1);
			uint32_t alive_num_2 = get_alive_hero_num(proto::common::combat_camp_2);
			if (alive_num_1 == alive_num_2)
			{
				m_combat_result.set_win_camp(get_initiative_camp());
				m_combat_result.set_result(proto::common::combat_fight_result_win);
				m_combat_result.mutable_win_camp_tasks()->add_finish_tasks()->set_finish_task(proto::common::combat_finish_task_type_first_attack);
			}
			else if (alive_num_1 > alive_num_2)
			{
				m_combat_result.set_win_camp(proto::common::combat_camp_1);
				m_combat_result.set_result(proto::common::combat_fight_result_win);
				m_combat_result.mutable_win_camp_tasks()->add_finish_tasks()->set_finish_task(proto::common::combat_finish_task_type_alive_more);
			}
			else
			{
				m_combat_result.set_win_camp(proto::common::combat_camp_2);
				m_combat_result.set_result(proto::common::combat_fight_result_win);
				m_combat_result.mutable_win_camp_tasks()->add_finish_tasks()->set_finish_task(proto::common::combat_finish_task_type_alive_more);
			}
			
		}
		return true;
	}

	return false;
}

void combat_t::fill_combat_data(
	proto::common::combat_data* data, 
	const proto::common::combat_action_data* process, 
	bool syn_fight_data)
{
	if (NULL == data)
	{
		log_error("data is null");
		return;
	}

	data->set_fight_uid(string_util_t::uint64_to_string(m_fight_uid));
	data->set_step(m_step);
//	data->set_round(m_round);
	data->set_hero_att_round(m_hero_att_round);
	data->set_state(m_state);
	data->mutable_data()->mutable_common()->mutable_param()->set_type(m_fight_data.common().param().type());
	data->set_watching_role_num(get_watching_role_num());
	data->set_cur_wave_num(m_wave);
	fight_hero_ptr p_hero = get_attack_hero();
	if (p_hero != NULL)
	{
		data->set_cur_att_hero_uid(string_util_t::uint64_to_string(p_hero->get_uid()));
	}

	//同步本回合技能CD
	for (auto iter : m_hero_skill_cd_list) {
		fight_hero_ptr p_cd_hero = iter.second;
		if (NULL == p_cd_hero) {
			log_error("p_cd_hero is NULL:[%lu]", iter.first);
			continue;
		}
		p_cd_hero->peek_fight_skill_data(data->mutable_skill_data());
	}
	m_hero_skill_cd_list.clear();
	

	uint32_t now = time_util_t::now_time();
	uint32_t remain_time = 0;
	if (m_end_time > now)
	{
		remain_time  = m_end_time - now;
	}

	if (remain_time > 0)
	{
		remain_time -= 1;
	}

	data->set_remain_time(m_end_time);
	FIGHT_LOG("[%lu]fill_combat_data %d, %u", get_fight_uid(), (int)m_state, remain_time);
	
	if (NULL != process)
	{
		data->mutable_action_data()->CopyFrom(*process);
	}

	if (syn_fight_data)
	{
		data->mutable_data()->CopyFrom(m_fight_data);

		for (int i = 0; i < data->data().self().objs().obj_size(); ++i)
		{
			fight_obj_ptr obj = find_obj(string_util_t::string_to_uint64(data->data().self().objs().obj(i).object_uid()));
			if (NULL == obj)
			{
				continue;
			}

			data->mutable_data()->mutable_self()->mutable_objs()->mutable_obj(i)->set_auto_fight(obj->is_auto_fight() ? 1 : 0);
			data->mutable_data()->mutable_self()->mutable_objs()->mutable_obj(i)->set_sp(obj->get_sp());
			data->mutable_data()->mutable_self()->mutable_objs()->mutable_obj(i)->set_name(obj->m_obj_data.name());
		}

		for (int i = 0; i < data->data().enemy().objs().obj_size(); ++i)
		{
			fight_obj_ptr obj = find_obj(string_util_t::string_to_uint64(data->data().enemy().objs().obj(i).object_uid()));
			if (NULL == obj)
			{
				continue;
			}

			data->mutable_data()->mutable_enemy()->mutable_objs()->mutable_obj(i)->set_auto_fight(obj->is_auto_fight() ? 1 : 0);
			data->mutable_data()->mutable_enemy()->mutable_objs()->mutable_obj(i)->set_sp(obj->get_sp());
			data->mutable_data()->mutable_enemy()->mutable_objs()->mutable_obj(i)->set_name(obj->m_obj_data.name());
		}
	}
}

void combat_t::fill_count_data(proto::common::combat_count_data* data)
{
	if (NULL == data)
	{
		log_error("combat_t combat_count_data NULL");
		return;
	}

	fight_obj_map::iterator iter_obj = m_objs.begin();
	fight_obj_ptr p_fight_obj = fight_obj_ptr();
	fight_hero_ptr p_fight_hero = fight_hero_ptr();

	proto::common::combat_count_data_heroes_obj *p_proto_obj = NULL;
	proto::common::combat_count_data_single_hero *p_proto_single_hero = NULL;

	for (; iter_obj != m_objs.end(); ++iter_obj)
	{
		p_fight_obj = iter_obj->second;
		if (NULL == p_fight_obj)
		{
			log_error("combat_t p_fight_obj NULL");
			continue;
		}
		p_proto_obj = data->add_obj_list();
		p_proto_obj->set_obj_uid(p_fight_obj->get_str_uid());
		
		for (fight_hero_map::iterator iter_hero = m_heros.begin(); iter_hero != m_heros.end(); ++iter_hero)
		{
			p_fight_hero = iter_hero->second;
			if (NULL == p_fight_hero)
			{
				log_error("combat_t p_fight_hero NULL");
				continue;
			}
			if (p_fight_hero->get_master() == p_fight_obj->get_uid())
			{
				p_proto_single_hero = p_proto_obj->add_hero_list();
				p_proto_single_hero->set_hero_tid(p_fight_hero->get_tid());
				p_proto_single_hero->set_plugin(p_fight_hero->get_plugin());
				p_proto_single_hero->set_pos(p_fight_hero->get_pos());
				p_proto_single_hero->set_heal(p_fight_hero->get_stat_data().get_heal_count());
				p_proto_single_hero->set_damage(p_fight_hero->get_stat_data().get_damage_count());
				p_proto_single_hero->set_accept_damage(p_fight_hero->get_stat_data().get_accept_damage_count());
			}
		}
	}
}

void combat_t::fill_combat_obj_state(proto::common::fight_obj_state_data* data)
{
	if (NULL == data)
	{
		log_error("data is null");
		return;
	}

	for (auto objpair : m_objs)
	{
		if (NULL == objpair.second)
		{
			continue;
		}

		proto::common::fight_obj_state* state = data->add_states();
		if (NULL == state)
		{
			continue;
		}

		state->set_object_uid(objpair.second->get_str_uid());
		state->set_state(objpair.second->get_state());
	}
}

void combat_t::fill_combat_hero_att_order(proto::common::combat_hero_att_order* data, const std::vector<fight_hero_ptr>& res)
{
	if (NULL == data)
	{
		log_error("combat_t fill_combat_hero_att_order data NULL");
		return;
	}

	fight_hero_ptr p_hero = fight_hero_ptr();
	proto::common::combat_hero_att_order_single *p_hero_att_order_single;

	for (uint32_t i = 0; i < res.size(); ++i)
	{
		p_hero = res[i];
		if (NULL == p_hero)
		{
			log_error("combat_t p_hero NULL pos[%d]", i);
			continue;
		}
		p_hero_att_order_single = data->add_hero_list();
		p_hero_att_order_single->set_hero_uid(p_hero->get_str_uid());
		p_hero_att_order_single->set_att_speed(p_hero->get_att_speed());
		p_hero_att_order_single->set_att_more_speed(p_hero->get_att_more_speed());
	}
}

void combat_t::fill_combat_recovery_data(proto::common::fight_recovery_data* data)
{
	if (NULL == data)
	{
		log_error("combat_t fill_combat_recovery_data data NULL");
		return;
	}
	proto::common::fight_recovery_obj_single *obj_single = NULL;
	proto::common::fight_recovery_hero_single_data *hero_single = NULL;

	fight_obj_ptr p_obj = fight_obj_ptr();
	fight_hero_ptr p_hero = fight_hero_ptr();

	for (auto pair : m_objs)
	{
		p_obj = pair.second;
		if (NULL == p_obj)
		{
			log_error("combat_t fill_combat_recovery_data p_obj NULL");
			continue;
		}
		obj_single = data->mutable_objs()->add_objs();
		obj_single->set_id(p_obj->get_str_uid());
		obj_single->set_sp(p_obj->get_sp());
	}
	for (auto pair : m_heros)
	{
		p_hero = pair.second;
		if (NULL == p_hero)
		{
			log_error("combat_t fill_combat_recovery_data p_hero NULL");
			continue;
		}

		hero_single = data->mutable_heroes()->add_hero_list();
		p_hero->peek_recovery_data(hero_single);
	}

	formation_ptr self_form = get_formation(proto::common::combat_camp_1);
	formation_ptr enemy_form = get_formation(proto::common::combat_camp_2);

	if (self_form != NULL)
	{
		proto::common::formation_single *formation_single = data->mutable_self_formation_data();
		self_form->peek_data(formation_single);
	}
	if (enemy_form != NULL)
	{
		proto::common::formation_single *formation_single = data->mutable_enemy_formation_data();
		enemy_form->peek_data(formation_single);
	}
}

bool combat_t::check_loading_finish()
{
	for (auto objpair : m_objs)
	{
		if (NULL == objpair.second)
			continue;

		if (objpair.second->get_state() == proto::common::combat_obj_state_loading)
		{
			if (objpair.second->get_type() != proto::common::SCENEOBJECT_USER || objpair.second->is_escape_end() )
			{
				objpair.second->set_state(proto::common::combat_obj_state_loading_end);
			}
			else
			{
				return false;
			}
		}
	}

	return true;
}

void combat_t::syn_combat_data(const proto::common::combat_action_data* process, uint64_t uid, bool syn_fight_data)
{
	proto::client::gc_combat_data_ntf ntf;
	fill_combat_data(ntf.mutable_data(), process, syn_fight_data);
	if (m_state == proto::common::combat_state_prepare)
	{
		fill_combat_hero_att_order(ntf.mutable_data()->mutable_att_order(), m_alive_hero_list);
	}
	fill_combat_hero_att_order(ntf.mutable_data()->mutable_first_att_order(), m_first_att_hero_list);
	if (uid != 0)
	{
		fight_obj_ptr obj = find_obj(uid);
		if (NULL != obj )
		{
			obj->send_msg_to_client(op_cmd::gc_combat_data_ntf, ntf);
		}
	}
	else
	{
		send_msg_to_all(op_cmd::gc_combat_data_ntf, ntf);
	}
}

void combat_t::syn_combat_obj_state(uint64_t uid /*= 0*/)
{
	proto::client::gc_combat_obj_state_ntf ntf;
	fill_combat_obj_state(ntf.mutable_data());
	if (uid != 0)
	{
		fight_obj_ptr obj = find_obj(uid);
		if (NULL != obj)
		{
			obj->send_msg_to_client(op_cmd::gc_combat_obj_state_ntf, ntf);
		}
	}
	else
	{
		send_msg_to_all(op_cmd::gc_combat_obj_state_ntf, ntf);
	}
}

void combat_t::syn_combat_recovery_data(uint64_t uid)
{
	proto::client::gc_combat_recovery_notify msg;
	msg.set_result(1);

	fill_combat_recovery_data(msg.mutable_recovery_data());

	if (uid != 0)
	{
		fight_obj_ptr obj = find_obj(uid);
		if (NULL != obj)
		{
			obj->send_msg_to_client(op_cmd::gc_combat_recovery_notify, msg);
		}
	}
	else
	{
		send_msg_to_all(op_cmd::gc_combat_recovery_notify, msg);
	}
}

bool combat_t::disposition(uint64_t uid, const proto::common::combat_form_data& data)
{
	if (m_step == proto::common::combat_step_end)
	{
		log_error("combat is end");
		return false;
	}

	fight_obj_ptr obj = find_obj(uid);
	if (NULL == obj)
	{
		log_error("disposition can not find obj %lu", uid);
		return false;
	}

	if (m_step != proto::common::combat_step_round)
	{
		log_error("disposition step error %lu %d", uid, (int)m_step);
		return false;
	}

	if (m_state != proto::common::combat_state_prepare)
	{
		log_error("disposition state error %lu %d", uid, (int)m_state);
		return false;
	}

// 	if (obj->get_state() != proto::common::combat_obj_state_prepare)
// 	{
// 		log_error("disposition obj state error %lu %d", uid, (int)obj->get_state());
// 		return false;
// 	}

	for (const proto::common::combat_form_single& action_t : data.forms())
	{
		proto::common::combat_form_single action(action_t);

		uint64_t attacker_id = string_util_t::string_to_uint64(action.attacker());
		fight_hero_ptr attacker = find_hero(attacker_id);
		if (NULL == attacker)
		{
			log_error("disposition can not find attacker %lu", attacker_id);
			continue;
		}

		if (attacker->get_master() != uid)
		{
			log_error("disposition hero master error %lu %lu", uid, attacker_id);
			continue;
		}

		SkillEffectTable* skill_effect_conf = attacker->get_skill_effect(action.skill());
		if (NULL == skill_effect_conf)
		{
			log_error("skill_effect_conf is null %u", action.skill());
			continue;
		}

// 		if (!attacker->check_start_skill_cd(skill_effect_conf))
// 		{
// 			log_error("disposition skill start_cd error %u", action.skill());
// 			continue;
// 		}

		uint32_t skill = action.skill();
		if (!attacker->check_skill_exist(skill))//武将没有该技能
		{
			int32_t skill_id = attacker->get_default_skill();
			if (skill_id == -1) {
				log_error("hero[%u] get_default skill -1", attacker->get_tid());
				continue;
			}
			action.set_skill(skill_id);
		}

		uint64_t target_id = string_util_t::string_to_uint64(action.target());
		fight_hero_ptr target = find_hero(target_id);
		if (NULL == target)
		{
			log_error("disposition can not find target %lu", target_id);
			continue;
		}

		SkillEffectTable* skill_conf = attacker->get_skill_effect(action.skill());
		if (NULL == skill_conf)
		{
			log_error("skill_conf is null %u", action.skill());
			continue;
		}
		if (skill_conf->type() == proto::common::skill_type_passive)
		{
			log_error("skill type = skill_type_passive");
			continue;
		}

		if (!check_skill_target_type(skill_conf, attacker, target, true))
		{
			log_error("disposition skill %u target type error attacker %lu target %lu", action.skill(), attacker_id, target_id);
			continue;
		}

		if (attacker->get_speed() <= 0)
		{
			log_error("disposition attacker[%lu %d] speed[%d] error", attacker->get_uid(), attacker->get_tid(), attacker->get_speed());
			continue;
		}

// 		if (skill_effect_conf->consum_type() == skill_consum_type_hp)
// 		{
// 			int cur_hp = attacker->get_attr(attrtype_enum::hp);
// 			if ((uint32_t)cur_hp < skill_effect_conf->consum_value())
// 			{
// 				log_error("disposition skill consum hp error %u %d", action.skill(), cur_hp);
// 				continue;
// 			}
// 		}
// 		else
// 		{
// 			uint32_t cur_sp = obj->get_disposition_sp();
// 			if (cur_sp < skill_effect_conf->consum_value())
// 			{
// 				log_error("disposition skill consum sp error %u %u", action.skill(), cur_sp);
// 				continue;
// 			}
// 
// 			obj->set_disposition_sp(cur_sp - skill_effect_conf->consum_value());
// 		}

		attacker->set_cur_action(action);
	}

	obj->set_state(proto::common::combat_obj_state_prepare_end);

	m_timer.stop();
	return true;
}

bool combat_t::initiative_disposition(uint64_t uid, const proto::common::combat_form_data& data)
{
	change_role_auto_fight(false, uid);
	if(disposition(uid, data))
	{
		syn_combat_obj_state();
		return check_disposition_finish();
	}
	return false;
}

bool combat_t::check_disposition_finish()
{
	// 如果当前武将属于自动控制部分则直接部署结束
	/*if (m_p_att_fight_hero != NULL && m_p_att_fight_hero->get_control_by_system)
	{
		return prepare_end();
	}*/

	for (auto objpair : m_objs)
	{
		if (NULL == objpair.second)
			continue;
		
		if (objpair.second->get_state() != proto::common::combat_obj_state_prepare_end)
		{
			if (objpair.second->get_type() != proto::common::SCENEOBJECT_USER || objpair.second->is_escape_end())
			{
				objpair.second->set_state(proto::common::combat_obj_state_prepare_end);
			}
			else
			{
				return false;
			}
		}
	}
	
	return prepare_end();
}

bool combat_t::fight_play_end(uint64_t uid, uint32_t hero_att_round)
{
	if (m_step == proto::common::combat_step_end)
	{
		log_error("combat is end");
		return false;
	}

	if (m_step != proto::common::combat_step_round)
	{
		log_error("play_end step error %lu %d", uid, (int)m_step);
		return false;
	}

	if (m_state != proto::common::combat_state_fight)
	{
		log_error("play_end state error %lu %d", uid, (int)m_state);
		return false;
	}
	// 屏蔽因消息延迟导致的客户端消息在时机结束后发送过来导致错误
	if (hero_att_round != m_hero_att_round)
		return false;

	fight_obj_ptr obj = find_obj(uid);
	if (NULL == obj)
	{
		log_error("fight_play_end can not find obj %lu", uid);
		return false;
	}

	if (obj->get_state() != proto::common::combat_obj_state_play)
	{
		log_error("play_end obj state error %lu %d", uid, (int)obj->get_state());
		return false;
	}

	obj->set_state(proto::common::combat_obj_state_play_end);
	
	if (!check_play_finish())
	{
		return true;
	}

	m_timer.stop();
	return play_end();
}

bool combat_t::check_play_finish()
{
	for (auto objpair : m_objs)
	{
		if (NULL == objpair.second) {
			continue;
		}

		if (objpair.second->get_state() != proto::common::combat_obj_state_play_end)
		{
			if (objpair.second->get_type() != proto::common::SCENEOBJECT_USER || objpair.second->is_escape_end())
			{
				objpair.second->set_state(proto::common::combat_obj_state_play_end);
			}
			else
			{
				return false;
			}
		}
	}
	return true;
}

bool combat_t::get_alive_attack_order(std::vector<fight_hero_ptr>& res)
{
	for (auto heropair : m_heros)
	{
		if (NULL == heropair.second)
		{
			continue;
		}

		if (heropair.second->is_dead())
		{
			continue;
		}

		res.push_back(heropair.second);
	}

	common::Sort::Qsort(res, 0, res.size() - 1, fight_hero_t::hero_speed_sort);
	return true;
}

bool combat_t::get_all_attack_order(std::vector<fight_hero_ptr>& res)
{
	for (auto heropair : m_heros)
	{
		if (NULL == heropair.second)
		{
			continue;
		}
		
		res.push_back(heropair.second);
	}

	common::Sort::Qsort(res, 0, res.size() - 1, fight_hero_t::hero_speed_sort);
	//std::sort(res.begin(), res.end(), fight_hero_t::hero_speed_sort);

	std::map<uint32_t, uint32_t> temp_fight_hero_list;
	std::map<uint32_t, uint32_t>::iterator temp_iter;

	fight_hero_ptr p_hero = fight_hero_ptr();
	uint32_t key = 0;
	for (uint32_t i = 0; i < res.size(); ++i)
	{
		p_hero = res[i];
		if (NULL == p_hero)
			continue;
		key = p_hero->get_pos() * 1000000 + p_hero->get_speed();
		temp_iter = temp_fight_hero_list.find(key);
		if (temp_iter != temp_fight_hero_list.end())
		{
			int32_t rand_value = random_util_t::randBetween(0, 1);
			if (rand_value == 1)
			{
				fight_hero_ptr temp_hero = res[temp_iter->second];

				res[temp_iter->second] = p_hero;
				temp_fight_hero_list.insert(std::make_pair(key, temp_iter->second));

				res[i] = temp_hero;
				temp_iter->second = i;
			}
		}
		else
		{
			temp_fight_hero_list.insert(std::make_pair(key, i));
		}
	}
	return true;
}


bool combat_t::check_camp_all_dead(uint32_t camp)
{
	pos_hero::iterator it = m_form[camp].begin();
	for (; it != m_form[camp].end(); ++it)
	{
		fight_hero_ptr hero = find_hero(it->second);
		if (NULL == hero)
		{
			log_error("check_camp_all_dead can not find hero %d %u %lu", (int)camp, it->first, it->second);
			return true;
		}

		// 如果处于灵魂状态则不做判断
		if (hero->get_attr(attrtype_enum::soul_condition) > 0)
			continue;

		// 幻境状态在判定战斗结束时不作为存活
		if (hero->is_in_mirage())
		{
			continue;
		}

		if (!hero->is_dead())
		{
			return false;
		}
	}

	return true;
}

//检测英雄死亡
bool combat_t::check_camp_hero_dead(uint32_t camp, uint32_t hero_id) {
	if (hero_id > 0) {//指定怪物
		fight_hero_ptr p_hero = find_hero(hero_id);
		if (NULL == p_hero) {
			log_error("check_camp_all_dead can not find hero %d %u", (int)camp, hero_id);
			return true;
		}
	
		if (p_hero->is_dead()) {
			return true;
		}
	}
	else {//boss怪物
		pos_hero::iterator it = m_form[camp].begin();
		for (; it != m_form[camp].end(); ++it)
		{
			fight_hero_ptr p_hero = find_hero(it->second);
			if (NULL == p_hero) {
				log_error("check_camp_all_dead can not find hero %d %u %lu", (int)camp, it->first, it->second);
				continue;
			}
			//有一个BOSS 没死就不算死
			if (p_hero->is_boss() && !p_hero->is_dead()) {
				return false;
			}
		}
		return true;
	}
	return false;
}
//获得主角英雄血量
uint32_t combat_t::get_camp_owner_hp_rate(uint32_t camp){
	pos_hero::iterator it = m_form[camp].begin();
	for (; it != m_form[camp].end(); ++it)
	{
		fight_hero_ptr p_hero = find_hero(it->second);
		if (NULL == p_hero) {
			log_error("check_camp_all_dead can not find hero %d %u %lu", (int)camp, it->first, it->second);
			continue;
		}
		if ( p_hero->is_player() ) {
			return p_hero->get_hp() / (p_hero->get_max_hp() * 1.0f);
		}
	}
	return 0;
}

//获得全队英雄平均血量
uint32_t combat_t::get_camp_team_avage_hp_reate(uint32_t camp) {
	uint32_t cur_team_hp = 0;
	uint32_t max_team_hp = 0;
	pos_hero::iterator it = m_form[camp].begin();
	for (; it != m_form[camp].end(); ++it)
	{
		fight_hero_ptr p_hero = find_hero(it->second);
		if (NULL == p_hero) {
			log_error("check_camp_all_dead can not find hero %d %u %lu", (int)camp, it->first, it->second);
			continue;
		}
		cur_team_hp += p_hero->get_hp();
		max_team_hp += p_hero->get_max_hp();
	}
	
	if (max_team_hp == 0) {
		return 0;
	}
	return cur_team_hp / (max_team_hp * 1.0f);
}

//获得阵营2,1号位血量
uint32_t combat_t::get_camp_hero_pos_hp_rate( uint32_t camp, uint32_t pos ){
	pos_hero &hero_list = m_form[camp];
	if (hero_list.size() > 0) {
		fight_hero_ptr p_hero = find_hero( hero_list[pos] );
		if (NULL == p_hero) {
			log_error(" combat fight pos[%u] hero is NULL", pos);
			return 0;
		}

		if (p_hero->get_max_hp() == 0) {
			return 0;
		}

		return p_hero->get_hp() / (p_hero->get_max_hp() * 1.0f);
	}
	return 0;
}

fight_hero_ptr combat_t::summon_hero(proto::common::hero_data* hd)
{
	if (NULL == hd)
	{
		log_error("hd is null");
		return NULL;
	}
	fight_hero_ptr ret;
	for (int i = 0; i < hd->hero_list_size(); ++i)
	{
		proto::common::hero_single* hs = hd->mutable_hero_list(i);
		uint64_t fight_hero_uid = string_util_t::string_to_uint64(hs->base_attr().fight_hero_uid());
		fight_hero_ptr hero(new fight_hero_t());
		if (!hero->init(shared_from_this(), hs))
		{
			continue;
		}
		ret = hero;

		m_heros[fight_hero_uid] = hero;
		uint32_t camp = hs->fight_attr().camp();
		uint32_t pos = hs->fight_attr().pos();
		m_form[camp][pos] = string_util_t::string_to_uint64(hs->base_attr().fight_hero_uid());
	}
	return ret;
}

void combat_t::on_master_die(uint32_t camp, uint64_t master)
{
	pos_hero::iterator it = m_form[camp].begin();
	for (; it != m_form[camp].end(); ++it)
	{
		fight_hero_ptr hero = find_hero(it->second);
		if (NULL == hero)
			continue;

		if (hero->is_dead())
			continue;

		if (!hero->is_summon())
			continue;

		if (hero->get_death_with_master() == 0)
			continue;

		hero->to_die();
	}
}

bool combat_t::get_summon_pos(uint32_t camp, uint32_t& pos)
{
	uint32_t num = 0;
	pos = fight_manager_t::get_max_camp_hero_count();
	pos_hero::iterator it = m_form[camp].begin();
	for (; it != m_form[camp].end(); ++it)
	{
		fight_hero_ptr hero = find_hero(it->second);
		if (NULL == hero)
		{
			continue;
		}

		if (hero->is_dead())
		{
			continue;
		}

		if (hero->is_summon())
		{
			++num;
			if (num >= fight_manager_t::get_max_summon_count())
			{
				return false;
			}
		}

		if (hero->get_pos() == pos)
		{
			pos = hero->get_pos() + 1;
		}
	}

	return true;
}

uint32_t combat_t::get_alive_hero_num(uint32_t camp)
{
	uint32_t num = 0;
	pos_hero::iterator it = m_form[camp].begin();
	for (; it != m_form[camp].end(); ++it)
	{
		fight_hero_ptr hero = find_hero(it->second);
		if (NULL == hero)
		{
			continue;
		}

		if (hero->is_dead())
		{
			continue;
		}

		++num;
	}

	return num;
}

fight_obj_ptr combat_t::find_obj(uint64_t uid)
{
	fight_obj_map::iterator it = m_objs.find(uid);
	if (it != m_objs.end())
	{
		return it->second;
	}

	return fight_obj_ptr();
}

fight_hero_ptr combat_t::find_hero(uint64_t uid)
{
	fight_hero_map::iterator it = m_heros.find(uid);
	if (it != m_heros.end())
	{
		return it->second;
	}

	return fight_hero_ptr();
}

bool combat_t::is_friend_target(uint32_t type)
{
	if (type == skill_target_alive_friend
		|| type == skill_target_alive_teammate
		|| type == skill_target_alive_or_dead_friend
		|| type == skill_target_teammate
		|| type == skill_target_dead_teammate
		|| type == skill_target_dead_friend)
	{
		return true;
	}
	return false;
}

bool combat_t::is_enemy_target(uint32_t type)
{
	if (type == skill_target_alive_enemy
		|| type == skill_target_dead_enemy
		|| type == skill_target_enemy)
	{
		return true;
	}
	return false;
}

const pos_hero* combat_t::get_skill_target_camp_pos_hero(fight_hero_ptr attaker, SkillEffectTable* skill_conf)
{
	if (NULL == attaker)
	{
		log_error("attaker is null");
		return NULL;
	}

	if (NULL == skill_conf)
	{
		log_error("skill_conf is null");
		return NULL;
	}

	const pos_hero* camp_heros = NULL;
	if (combat_t::is_friend_target( skill_conf->target_type() ) )
	{
		camp_heros = get_camp_pos_hero(attaker->get_camp());
	}
	else if (combat_t::is_enemy_target( skill_conf->target_type() ))
	{
		camp_heros = get_camp_pos_hero(attaker->get_enemy_camp());
	}

	return camp_heros;
}

void combat_t::get_skill_target_heroes(fight_hero_ptr attaker, SkillEffectTable* skill_conf, std::vector<fight_hero_ptr>& result)
{
	if (NULL == attaker)
	{
		log_error("attaker is null");
		return;
	}

	if (NULL == skill_conf)
	{
		log_error("skill_conf is null");
		return;
	}

	if (combat_t::is_friend_target(skill_conf->target_type()))
	{
		get_camp_heros(result, attaker->get_camp());
	}
	else if (combat_t::is_enemy_target(skill_conf->target_type()))
	{
		get_camp_heros(result, attaker->get_enemy_camp());
	}

}

bool combat_t::check_skill_target_type(SkillEffectTable* skill_conf, fight_hero_ptr attacker, fight_hero_ptr target, bool disposition)
{
	if (NULL == skill_conf)
	{
		log_error("skill_conf is null");
		return false;
	}

	if (NULL == attacker)
	{
		log_error("attacker is null");
		return false;
	}

	if (NULL == target)
	{
		log_error("target is null");
		return false;
	}
	// 目标处于灵魂或幻境状态，则不可以作为目标
	if (target->get_attr(attrtype_enum::soul_condition) > 0 || target->is_in_mirage())
		return false;

	uint32_t target_type = skill_conf->target_type();

	// 混乱状态转换目标类型
	if (attacker->get_attr(attrtype_enum::confused))
		target_type = skill_target_alive_friend;

	// 阵营
	if (target_type == skill_target_alive_or_dead_friend ||
		target_type == skill_target_teammate || 
		target_type == skill_target_alive_friend || 
		target_type == skill_target_alive_teammate ||
		target_type == skill_target_dead_teammate ||
		target_type == skill_target_dead_friend )
	{
		if (attacker->get_camp() != target->get_camp())
		{
			if (disposition)
			{
				log_error("skill %u target type error attacker %lu target %lu is not same camp", skill_conf->id(), attacker->get_uid(), target->get_uid());
			}
			
			return false;
		}

		// 是否包括自己
		if (target_type != skill_target_alive_or_dead_friend
			&& target_type != skill_target_alive_friend
			&& target_type != skill_target_dead_friend )
		{
			if (attacker->get_uid() == target->get_uid())
			{
				if (disposition)
				{
					log_error("skill %u target type error attacker %lu target %lu can not select self", skill_conf->id(), attacker->get_uid(), target->get_uid());
				}
				
				return false;
			}
		}
	}
	else
	{
		if (attacker->get_camp() == target->get_camp())
		{
			if (disposition)
			{
				log_error("skill %u target type error attacker %lu target %lu is same camp", skill_conf->id(), attacker->get_uid(), target->get_uid());
			}
			
			return false;
		}
	}


	// 死亡状态
	if (target_type == skill_target_alive_or_dead_friend
		|| target_type == skill_target_teammate
		|| target_type == skill_target_enemy)
	{
		return true;
	}
	else if (target_type == skill_target_dead_teammate
		|| target_type == skill_target_dead_enemy
		|| target_type == skill_target_dead_friend)
	{
		if (!target->is_dead())
		{
			if (disposition)
			{
				log_error("skill %u target type error attacker %lu target %lu is not dead", skill_conf->id(), attacker->get_uid(), target->get_uid());
			}
			
			return false;
		}
	}
	else
	{
		if (target->is_dead())
		{
			if (disposition)
			{
				log_error("skill %u target type error attacker %lu target %lu is dead", skill_conf->id(), attacker->get_uid(), target->get_uid());
			}
			
			return false;
		}
	}

	return true;
}

const pos_hero* combat_t::get_camp_pos_hero(uint32_t camp)
{
	if (m_form.find(camp) == m_form.end())
	{
		return NULL;
	}

	return &(m_form[camp]);
}

fight_hero_ptr combat_t::get_hero_by_camp_and_pos(uint32_t camp, uint32_t pos)
{
	const pos_hero* camp_heros = get_camp_pos_hero(camp);
	if (NULL == camp_heros)
	{
		return fight_hero_ptr();
	}

	const pos_hero::const_iterator iter = (*camp_heros).find(pos);
	if (iter != (*camp_heros).end())
	{
		return find_hero(iter->second);
	}
// 	for (auto pospair : *camp_heros)
// 	{
// 		if (pospair.first == pos)
// 		{
// 			return find_hero(pospair.second);
// 		}
// 	}

	return fight_hero_ptr();
}

fight_hero_ptr combat_t::get_hero_by_camp_and_tid(uint32_t camp, uint32_t tid)
{
	const pos_hero* camp_heros = get_camp_pos_hero(camp);
	if (NULL == camp_heros)
	{
		return fight_hero_ptr();
	}

	fight_hero_ptr p_hero = fight_hero_ptr();
	for (auto pospair : *camp_heros)
	{
		p_hero = find_hero(pospair.second);
		if (NULL == p_hero)
		{
			continue;
		}
		if (p_hero->get_tid() == tid)
			return p_hero;
	}

	return fight_hero_ptr();
}

bool combat_t::get_camp_heros(std::vector<fight_hero_ptr>& result, uint32_t camp, uint32_t except_pos)
{
	const pos_hero* camp_heros = get_camp_pos_hero(camp);
	if (NULL == camp_heros)
	{
		return false;
	}

	for (auto pospair : *camp_heros)
	{
		if (pospair.first == except_pos)
		{
			continue;
		}

		fight_hero_ptr hero = find_hero(pospair.second);
		if (NULL == hero)
		{
			continue;
		}

		result.push_back(hero);
	}

	return true;
}

fight_hero_ptr combat_t::get_random_alive_hero_by_camp(uint32_t camp)
{
	std::vector<fight_hero_ptr> heros;
	get_camp_alive_heros(heros, camp, 0);
	if (heros.empty())
	{
		log_error("all hero dead of camp[%u]", camp);
		return NULL;
	}

	return heros[random_util_t::randBetween(0, heros.size() - 1)];
}

bool combat_t::get_camp_alive_heros(std::vector<fight_hero_ptr>& result, uint32_t camp, uint32_t except_pos)
{
	const pos_hero* camp_heros = get_camp_pos_hero(camp);
	if (NULL == camp_heros)
	{
		return false;
	}

	for (auto pospair : *camp_heros)
	{
		if (pospair.first == except_pos)
		{
			continue;
		}

		fight_hero_ptr hero = find_hero(pospair.second);
		if (NULL == hero)
		{
			continue;
		}
		if(hero->is_dead())
			continue;
		// 灵魂状态不可以被选择
		if (hero->get_attr(attrtype_enum::soul_condition) > 0)
			continue;
		result.push_back(hero);
	}

	return true;
}

bool combat_t::is_escape_camp(uint32_t camp)
{
	uint32_t escape_num = 0;
	uint32_t all_num = 0;
	fight_obj_ptr p_obj = fight_obj_ptr();
	for (auto pair : m_objs)
	{
		p_obj = pair.second;
		if (NULL == p_obj)
			continue;
		if (p_obj->get_type() != proto::common::SCENEOBJECT_USER || p_obj->get_camp() != camp)
			continue;
		if (p_obj->is_escape())
			++escape_num;
		++all_num;
	}
	if (all_num == 1)
	{
		if (escape_num == 1)
			return true;
	}
	else
	{
		if (all_num - escape_num <= 1)
			return true;
	}
	return false;
}
fight_hero_ptr combat_t::find_alive_target_of_not_mirage(fight_hero_ptr attacker, SkillEffectTable* skill_conf)
{
	if (NULL == attacker)
	{
		log_error("attacker is null");
		return fight_hero_ptr(NULL);
	}

	if (NULL == skill_conf)
	{
		log_error("skill_conf is null");
		return fight_hero_ptr(NULL);
	}

	// 如果技能目标是自己 就没必要选择默认目标了
	if (skill_conf->attack_target() == skill_attack_target_type_self)
		return fight_hero_ptr(NULL);

	// 根据技能目标类型筛选出阵营
	std::vector<fight_hero_ptr> result;
	get_skill_target_heroes(attacker, skill_conf, result);
	if (result.size() == 0)
	{
		log_error("find_default_target result_size = 0");
		return fight_hero_ptr(NULL);
	}

	for (auto fight_hero : result) {
		if (NULL == fight_hero) {
			continue;
		}

		//幻境状态武将不作为技能施放目标
		if (fight_hero->is_dead() || fight_hero->is_relive_cur_round() || fight_hero->is_in_mirage()) {
			continue;
		}

			return fight_hero;
	}
	return NULL;
}

fight_hero_ptr combat_t::find_default_target(fight_hero_ptr attacker, SkillEffectTable* skill_conf)
{
 	if (NULL == attacker)
 	{
 		log_error("attacker is null");
 		return fight_hero_ptr();
 	}
 
 	if (NULL == skill_conf)
 	{
 		log_error("skill_conf is null");
 		return fight_hero_ptr();
 	}

	// 如果技能目标是自己 就没必要选择默认目标了
	if (skill_conf->attack_target() == skill_attack_target_type_self)
		return fight_hero_ptr();
 
	// 根据技能目标类型筛选出阵营
 	std::vector<fight_hero_ptr> result;
 	get_skill_target_heroes(attacker, skill_conf, result);
 	if (result.size() == 0)
 	{
 		log_error("find_default_target result_size = 0");
 		return fight_hero_ptr();
 	}

	uint32_t ai_id = skill_conf->ai_id();
	SkillTargetAI* p_conf = GET_CONF(SkillTargetAI, ai_id);
	if (NULL == p_conf)
	{
		log_error("find_default_target p_conf NULL id[%d]", ai_id);
		return NULL;
	}

	fight_hero_ptr p_fight_hero = NULL;
	fight_hero_ptr p_find_hero = NULL;
 	std::map<uint32_t, uint32_t> skill_target_ai_type_list;
	std::vector<fight_hero_ptr>::iterator iter_hero = result.begin();

	fight_hero_ptr p_min_hp_hero = select_min_hp_target(attacker->get_camp());
	fight_hero_ptr p_min_att_speed_hero = select_min_att_speed_target(attacker->get_camp());
	fight_hero_ptr p_max_atk_att_hero = select_hero_by_attr(attacker->get_camp(), 0, attrtype_enum::atk, attacker, skill_conf, true);

	int32_t max_skill_target_ai_rate = 0;				// 技能目标AI权重最大值
	int32_t skill_target_ai_rate = 0;					// 技能目标AI权重
	int32_t all_skill_target_ai_rate = 0;				// 总技能目标AI权重值
	const int32_t skill_ai_rate_max_value = 100;		// 技能目标ai权重的阀值
	std::vector<uint32_t> skill_target_ai_rate_list;
	std::map<uint32_t, uint32_t>::iterator iter_ai;

 	// 计算出出现的全部事件
	for (; iter_hero != result.end(); ++iter_hero)
 	{
 		p_fight_hero = *iter_hero;
 		if (NULL == p_fight_hero)
 		{
 			log_error("find_default_target p_fight_hero NULL");
 			continue;
 		}
		skill_target_ai_type_list.clear();
 		if (attacker->get_camp() == p_fight_hero->get_camp())
 		{
			// 附带大类型1、2、3、4的buff
 			if (p_fight_hero->m_buff_mgr.get_count_by_big_type(1) > 0)
				skill_target_ai_type_list[target_ai_enemy_big_type1_rate] = 1;
			if (p_fight_hero->m_buff_mgr.get_count_by_big_type(2) > 0)
				skill_target_ai_type_list[target_ai_enemy_big_type2_rate] = 1;
			if (p_fight_hero->m_buff_mgr.get_count_by_big_type(3) > 0)
				skill_target_ai_type_list[target_ai_enemy_big_type3_rate] = 1;
			if (p_fight_hero->m_buff_mgr.get_count_by_big_type(4) > 0)
				skill_target_ai_type_list[target_ai_enemy_big_type4_rate] = 1;
			if (p_min_hp_hero != NULL && p_min_hp_hero->get_uid() == p_fight_hero->get_uid())
				skill_target_ai_type_list[target_ai_friend_min_hp_rate] = 1;
			if (p_min_att_speed_hero != NULL && p_min_att_speed_hero->get_uid() == p_fight_hero->get_uid())
				skill_target_ai_type_list[target_ai_friend_min_att_speed_rate] = 1;
			if (p_fight_hero->is_dead())
				skill_target_ai_type_list[target_ai_friend_dead_rate] = 1;
			if (p_max_atk_att_hero != NULL && p_max_atk_att_hero->get_uid() == p_fight_hero->get_uid()) {
				skill_target_ai_type_list[target_ai_friend_max_att_atk_rate] = 1;
			}

 		}
 		else
		{
			uint32_t percent = p_fight_hero->get_hp_percent();
			uint32_t terry_relation = attacker->get_hero_attr_relation_type(p_fight_hero);

			if (p_fight_hero->get_mark_id() > 0)
				skill_target_ai_type_list[target_ai_all_att_target_rate] = 1;
 			if (!p_fight_hero->is_dead() && percent <= 2000)
 				skill_target_ai_type_list[target_ai_enemy_hp_20_rate] = 1;
 			if (p_fight_hero->get_attr(attrtype_enum::def_break) > 0)
				skill_target_ai_type_list[target_ai_enemy_def_break_rate] = 1;
 			if (terry_relation == terry_relation_type_restraint)
				skill_target_ai_type_list[target_ai_enemy_restrain_rate] = 1;
			if (terry_relation == terry_relation_type_none)
				skill_target_ai_type_list[target_ai_enemy_no_restrain_rate] = 1;
			if (terry_relation == terry_relation_type_be_restraint)
				skill_target_ai_type_list[target_ai_enemy_be_restrain_rate] = 1;
			if (p_fight_hero->get_attr(attrtype_enum::shield_points) > 0)
				skill_target_ai_type_list[target_ai_enemy_shield_rate] = 1;
			if (p_fight_hero->get_attr(attrtype_enum::invincibility) > 0)
				skill_target_ai_type_list[target_ai_enemy_invincibility_rate] = 1;
		}
		// 默认权重至少为1
		skill_target_ai_rate = 1;
		// 根据选出的事件加权重
		for (iter_ai = skill_target_ai_type_list.begin(); iter_ai != skill_target_ai_type_list.end(); ++iter_ai)
		{
			skill_target_ai_rate += p_conf->get_target_ai_rate(iter_ai->first)/* * iter_ai->second*/;
		}
		// 如果该目标AI的权重超过阀值 就直接取该目标 不再roll
		if (skill_target_ai_rate >= skill_ai_rate_max_value && skill_target_ai_rate > max_skill_target_ai_rate)
		{
			max_skill_target_ai_rate = skill_target_ai_rate;
			p_find_hero = p_fight_hero;
		}
		else
		{
			if(skill_target_ai_rate < 0)
				continue;

			skill_target_ai_rate_list.push_back(skill_target_ai_rate);
			all_skill_target_ai_rate += skill_target_ai_rate;
		}
 	}
	
	// 如果已经找到就不再roll了
	if (p_find_hero != NULL)
	{
		return p_find_hero;
	}

	if (all_skill_target_ai_rate <= 0)
	{
		int32_t random_value = random_util_t::randMin(0, result.size());
		return result[random_value];
	}
	else
	{
		int32_t random_value = random_util_t::randBetween(1, all_skill_target_ai_rate);
		if (random_value == -1 ) {
			log_error("dispel_buff rand error min[%d] > max[%d]", 1, all_skill_target_ai_rate);
			return 0;
		}

		uint32_t compare_value = 0;
		for (uint32_t i = 0; i < skill_target_ai_rate_list.size(); ++i)
		{
			compare_value += skill_target_ai_rate_list[i];
			// roll到了一个
			if ((uint32_t)random_value <= compare_value)
			{
				// 安全校验 避免数组越界 skill_target_ai_rate_list的顺序跟result的顺序是一致的
				if (i < result.size())
				{
					return result[i];
				}
				else
				{
					log_error("find_default_target find_pos[%d] >= skill_ai_list_size[%d]", i, (int32_t)result.size());
					return 0;
				}
			}
		}
	}

	log_error("find_default_target not find compare ai all_ai_rate[%d]", all_skill_target_ai_rate);
	return NULL;
}

// 查找目标
bool combat_t::find_target_by_skill_conf(
	fight_hero_ptr attaker,
	SkillEffectTable* skill_conf,
	std::vector<fight_hero_ptr>& targets,
	std::vector<fight_hero_ptr>& spurting_targets,
	fight_hero_ptr target,
	proto::common::combat_act_type type )
{
	if (NULL == skill_conf) {
		return false;
	}

	switch (skill_conf->attack_target())
	{
	case skill_attack_target_type_random:
	{
		return find_random_target(attaker, skill_conf, targets, target, skill_conf->target_count());
	}
	break;
	case skill_attack_target_type_all:
	{
		return find_all_target(attaker, skill_conf, targets, target);
	}
	break;
	case skill_attack_target_type_some_min_hp:
	{
		return find_some_min_hp_target(attaker, skill_conf, targets, target);
	}
	break;
	case skill_attack_target_type_min_hp:
	{
		return find_min_hp_target(attaker, skill_conf, targets, target);
	}
	break;
	case skill_attack_target_type_spurting:
	{
		return find_spurting_target(attaker, skill_conf, targets, spurting_targets, target, skill_conf->target_count());
	}
	break;
	case skill_attack_target_type_self:
	{
		targets.push_back(attaker);
		return true;
	}
	break;
	case skill_attack_target_type_hp_greater:
	{
		return find_some_grater_hp_target(attaker, skill_conf, targets, target);
	}
	break;
	case skill_attack_target_type_hurt_me_most:
	{
		fight_hero_ptr p_hero = attaker->get_hurt_most_alive_enemy(attaker, skill_conf);
		if (p_hero)
		{
			targets.push_back(p_hero);
			return true;
		}
		else
		{
			log_error("find hurt most target null");
		}
	}
	break;
	default:
	{
		if (target != NULL)
		{
			if ( include_death_act_type(type) || check_skill_target_type(skill_conf, attaker, target))
			{
				targets.push_back(target);
			}
		}
	}
	break;
	}

	if (!targets.empty())
	{
		return true;
	}

	return find_random_target(attaker, skill_conf, targets, target);
}

bool combat_t::include_death_target(uint32_t type)
{
	if (type == skill_target_dead_teammate)
		return true;
	if (type == skill_target_dead_enemy)
		return true;
	if (type == skill_target_dead_friend)
		return true;
	// 这个类型不论死亡与否都可以
	if (type == skill_target_teammate)
		return true;
	// 这个类型不论死亡与否都可以
	if (type == skill_target_enemy)
		return true;
	// 这个类型不论死亡与否都可以
	if (type == skill_target_alive_or_dead_friend)
		return true;
	return false;
}

bool combat_t::include_death_act_type(proto::common::combat_act_type type) 
{
	switch ((uint32_t)type) {
		case proto::common::combat_act_type_counter:
		case proto::common::combat_act_type_pincer_attack:
		case proto::common::combat_act_type_follow:
		return true;
	}
	return false;
}


bool combat_t::find_target(
	fight_hero_ptr attaker, 
	SkillEffectTable* skill_conf,
	std::vector<fight_hero_ptr>& targets, 
	std::vector<fight_hero_ptr>& spurting_targets,
	proto::common::combat_act_type type,
	fight_hero_ptr select)
{
	if (NULL == attaker)
	{
		log_error("attaker is null");
		return false;
	}

	if (NULL == skill_conf)
	{
		log_error("skill_conf is null");
		return false;
	}

	fight_hero_ptr target = select;
	if (target == NULL ) {
		target = find_default_target(attaker, skill_conf);
	}

	if (target != NULL && ( target->is_dead() || target->is_relive_cur_round() == true || target->is_in_mirage())) {
		// 如果不是复活类技能 则重新寻找
		if (combat_t::include_death_target(skill_conf->target_type())==false && combat_t::include_death_act_type( type ) == false ) {
			//add by hy 找一个活的hero，非幻境状态
			target = find_alive_target_of_not_mirage(attaker, skill_conf);
		}
	}
	
	return find_target_by_skill_conf(attaker, skill_conf, targets, spurting_targets, target, type );
}

bool combat_t::find_random_target(
	fight_hero_ptr attaker, 
	SkillEffectTable* skill_conf,
	std::vector<fight_hero_ptr>& targets, 
	fight_hero_ptr select,
	uint32_t num,
	bool b_has_select)
{
	if (NULL == select)
	{
		log_error("select target is null");
		return false;
	}

	if (NULL == attaker)
	{
		log_error("attaker is null");
		return false;
	}

	if (NULL == skill_conf)
	{
		log_error("skill_conf is null");
		return false;
	}

	const pos_hero* camp_heros = get_camp_pos_hero(select->get_camp());
	if (NULL == camp_heros)
	{
		log_error("camp_heros is null");
		return false;
	}

	if (b_has_select)
	{
		if (check_skill_target_type(skill_conf, attaker, select))
		{
			targets.push_back(select);
		}

// 		if (targets.size() >= num)
// 		{
// 			return true;
// 		}
	}

	std::vector<fight_hero_ptr> heros;
	for (auto pospair : *camp_heros)
	{
		if (pospair.first == select->get_pos())
		{
			continue;
		}

		fight_hero_ptr hero = find_hero(pospair.second);
		if (NULL == hero)
		{
			continue;
		}

		if (!check_skill_target_type(skill_conf, attaker, hero))
		{
			continue;
		}

		heros.push_back(hero);
	}

	if (heros.empty())
	{
		return true;
	}

	std::random_shuffle(heros.begin(), heros.end());
	for (auto h : heros)
	{
		if (NULL == h) {
			continue;	
		}

		targets.push_back(h);
		if (targets.size() >= num)
		{
			break;
		}
	}

	return true;
}

bool combat_t::find_spurting_target( 
	fight_hero_ptr attaker, 
	SkillEffectTable* skill_conf, 
	std::vector<fight_hero_ptr>& targets, 
	std::vector<fight_hero_ptr>& spurting_heroes, 
	fight_hero_ptr select, 
	uint32_t num)
{
	if (check_skill_target_type(skill_conf, attaker, select))
	{
		targets.push_back(select);
	}

	find_random_target(attaker, skill_conf, spurting_heroes, select, num, false);
	return true;
}

uint32_t combat_t::get_empty_pos(uint32_t camp)
{
	const pos_hero* camp_heros = get_camp_pos_hero(camp);
	if (NULL == camp_heros)
	{
		log_error("camp_heros is null");
		return 1;
	}

	pos_hero::const_iterator iter;
	for (int i = 1; i <= battle_max_grid; ++i)
	{
		iter = camp_heros->find(i);
		if (iter == camp_heros->end())
			return i;
	}
	return 1;
}

// bool combat_t::find_vertical_target(
// 	fight_hero_ptr attaker, 
// 	SkillEffectTable* skill_conf,
// 	std::vector<fight_hero_ptr>& targets, 
// 	fight_hero_ptr select)
// {
// 	if (NULL == select)
// 	{
// 		log_error("select target is null");
// 		return false;
// 	}
// 
// 	if (NULL == attaker)
// 	{
// 		log_error("attaker is null");
// 		return false;
// 	}
// 
// 	if (NULL == skill_conf)
// 	{
// 		log_error("skill_conf is null");
// 		return false;
// 	}
// 
// 	if (check_skill_target_type(skill_conf, attaker, select))
// 	{
// 		targets.push_back(select);
// 	}
// 
// 	uint32_t vertical_index = VERTICAL_INDEX[select->get_pos()];
// 	for (auto pos : VERTICAL_MAP[vertical_index])
// 	{
// 		if (pos == select->get_pos())
// 		{
// 			continue;
// 		}
// 
// 		fight_hero_ptr hero = get_hero_by_camp_and_pos(select->get_camp(), pos);
// 		if (NULL == hero)
// 		{
// 			continue;
// 		}
// 
// 		if (!check_skill_target_type(skill_conf, attaker, hero))
// 		{
// 			continue;
// 		}
// 
// 		targets.push_back(hero);
// 	}
// 
// 	return true;
// }

// bool combat_t::find_horizon_target(
// 	fight_hero_ptr attaker, 
// 	SkillEffectTable* skill_conf,
// 	std::vector<fight_hero_ptr>& targets, 
// 	fight_hero_ptr select)
// {
// 	if (NULL == select)
// 	{
// 		log_error("select target is null");
// 		return false;
// 	}
// 
// 	if (NULL == attaker)
// 	{
// 		log_error("attaker is null");
// 		return false;
// 	}
// 
// 	if (NULL == skill_conf)
// 	{
// 		log_error("skill_conf is null");
// 		return false;
// 	}
// 
// 	if (check_skill_target_type(skill_conf, attaker, select))
// 	{
// 		targets.push_back(select);
// 	}
// 
// 	uint32_t horizon_index = HORIZON_INDEX[select->get_pos()];
// 	for (auto pos : HORIZON_MAP[horizon_index])
// 	{
// 		if (pos == select->get_pos())
// 		{
// 			continue;
// 		}
// 
// 		fight_hero_ptr hero = get_hero_by_camp_and_pos(select->get_camp(), pos);
// 		if (NULL == hero)
// 		{
// 			continue;
// 		}
// 
// 		if (!check_skill_target_type(skill_conf, attaker, hero))
// 		{
// 			continue;
// 		}
// 
// 		targets.push_back(hero);
// 	}
// 
// 	return true;
// }

bool combat_t::find_all_target(
	fight_hero_ptr attaker, 
	SkillEffectTable* skill_conf,
	std::vector<fight_hero_ptr>& targets, 
	fight_hero_ptr select)
{
	if (NULL == select)
	{
		log_error("select target is null");
		return false;
	}

	if (NULL == attaker)
	{
		log_error("attaker is null");
		return false;
	}

	if (NULL == skill_conf)
	{
		log_error("skill_conf is null");
		return false;
	}

	const pos_hero* camp_heros = get_camp_pos_hero(select->get_camp());
	if (NULL == camp_heros)
	{
		log_error("camp_heros is null");
		return false;
	}

	if (check_skill_target_type(skill_conf, attaker, select))
	{
		targets.push_back(select);
	}

	for (auto pospair : *camp_heros)
	{
		if (pospair.first == select->get_pos())
		{
			continue;
		}

		fight_hero_ptr hero = find_hero(pospair.second);
		if (NULL == hero)
		{
			continue;
		}

		if (!check_skill_target_type(skill_conf, attaker, hero))
		{
			continue;
		}

		targets.push_back(hero);
	}
	
	return true;
}

bool combat_t::find_min_hp_target(
	fight_hero_ptr attaker, 
	SkillEffectTable* skill_conf,
	std::vector<fight_hero_ptr>& targets, 
	fight_hero_ptr select)
{
	if (NULL == attaker)
	{
		log_error("attaker is null");
		return false;
	}

	if (NULL == skill_conf)
	{
		log_error("skill_conf is null");
		return false;
	}

	std::vector<fight_hero_ptr> temp_targets;
	if(!select_min_hp_target(attaker, skill_conf, temp_targets))
	{
		return false;
	}
	if (temp_targets.size() == 0)
		return true;

	int32_t random_pos = random_util_t::randMin(0, temp_targets.size());
	if (random_pos == -1)
	{
		log_error("find_min_hp_target rand error min[%d] max[%d]", 0, (int32_t)temp_targets.size());
		return false;
	}

	fight_hero_ptr p_final = temp_targets[random_pos];
	targets.push_back(p_final);
	return true;
}

bool combat_t::find_some_min_hp_target(
	fight_hero_ptr attaker,
	SkillEffectTable* skill_conf,
	std::vector<fight_hero_ptr>& targets,
	fight_hero_ptr select)
{
	if (NULL == attaker)
	{
		log_error("attaker is null");
		return false;
	}

	if (NULL == skill_conf)
	{
		log_error("skill_conf is null");
		return false;
	}

	if (!select_min_hp_target(attaker, skill_conf, targets))
	{
		return false;
	}
	return true;
}

fight_hero_ptr combat_t::select_confused_target(fight_hero_ptr attaker)
{
	if (NULL == attaker)
	{
		log_error("attaker is null");
		return NULL;
	}
	
	std::vector<fight_hero_ptr> heros;
	get_camp_alive_heros(heros, attaker->get_camp(), attaker->get_pos());
	if (heros.empty())
	{
		return NULL;
	}

	int32_t rand_num = random_util_t::randMin(0, heros.size());
	if (rand_num == -1)
	{
		log_error("select_confused_target rand error min[%d] max[%d]", 0, (int32_t)heros.size());
		return NULL;
	}
	return heros[rand_num];
}

void combat_t::on_loading_timer(const boost::weak_ptr<combat_t> pw_combat, const boost::system::error_code& ec)
{
	if (ec == boost::asio::error::operation_aborted)
	{
		
		return;
	}
	else if (ec)
	{
		log_error("Timer error: %s", ec.message().c_str());
		return;
	}

	combat_ptr p_combat(pw_combat.lock());
	if (NULL == p_combat)
	{
		log_error("on_loading_timer p_combat NULL");
		return;
	}
	p_combat->deal_on_loading_timer();
}

void combat_t::on_form_timer(const boost::weak_ptr<combat_t> pw_combat, const boost::system::error_code& ec)
{
	if (ec == boost::asio::error::operation_aborted)
	{
		
		return;
	}
	else if (ec)
	{
		log_error("Timer error: %s", ec.message().c_str());
		return;
	}
	
	combat_ptr p_combat(pw_combat.lock());
	if (NULL == p_combat)
	{
		log_error("on_form_timer p_combat NULL");
		return;
	}
	p_combat->deal_on_form_timer();
}

void combat_t::on_form_ready_timer(const boost::weak_ptr<combat_t> pw_combat, const boost::system::error_code& ec)
{
	if (ec == boost::asio::error::operation_aborted)
	{
		
		return;
	}
	else if (ec)
	{
		log_error("Timer error: %s", ec.message().c_str());
		return;
	}

	combat_ptr p_combat(pw_combat.lock());
	if (NULL == p_combat)
	{
		log_error("on_form_ready_timer p_combat NULL");
		return;
	}
	p_combat->deal_on_form_ready_timer();
}

void combat_t::on_disposition_ready_timer(const boost::weak_ptr<combat_t> pw_combat, const boost::system::error_code& ec)
{
	if (ec == boost::asio::error::operation_aborted)
	{
		
		return;
	}
	else if (ec)
	{
		log_error("Timer error: %s", ec.message().c_str());
		return;
	}

	combat_ptr p_combat(pw_combat.lock());
	if (NULL == p_combat)
	{
		log_error("on_disposition_ready_timer p_combat NULL");
		return;
	}
	p_combat->deal_on_disposition_ready_timer();
}

void combat_t::on_prepare_timer(const boost::weak_ptr<combat_t> pw_combat, const boost::system::error_code& ec)
{
	if (ec == boost::asio::error::operation_aborted)
	{
		
		return;
	}
	else if (ec)
	{
		log_error("Timer error: %s", ec.message().c_str());
		return;
	}

	combat_ptr p_combat(pw_combat.lock());
	if (NULL == p_combat)
	{
		log_error("on_prepare_timer p_combat NULL");
		return;
	}
	p_combat->deal_on_prepare_timer();
}

void combat_t::on_play_timer(const boost::weak_ptr<combat_t> pw_combat, const boost::system::error_code& ec)
{
	if (ec == boost::asio::error::operation_aborted)
	{
		
		return;
	}
	else if (ec)
	{
		log_error("Timer error: %s", ec.message().c_str());
		return;
	}

	combat_ptr p_combat(pw_combat.lock());
	if (NULL == p_combat)
	{
		log_error("on_play_timer p_combat NULL");
		return;
	}
	p_combat->deal_on_play_timer();
}


void combat_t::on_check_auto_dis_timer(const boost::weak_ptr<combat_t> pw_combat, const boost::system::error_code& ec)
{
	if (ec == boost::asio::error::operation_aborted)
	{
		return;
	}
	else if (ec)
	{
		log_error("Timer error: %s", ec.message().c_str());
		return;
	}

	combat_ptr p_combat(pw_combat.lock());
	if (NULL == p_combat)
	{
		log_error("on_play_timer p_combat NULL");
		return;
	}
	p_combat->deal_on_check_auto_dis_timer();
}

void combat_t::deal_on_check_auto_dis_timer()
{
	m_dis_timer.stop();
	
	check_disposition_finish();
}




void combat_t::deal_on_loading_timer()
{
	m_timer.stop();

	for (auto objpair : m_objs)
	{
		if (NULL == objpair.second)
		{
			continue;
		}

		if (objpair.second->get_state() != proto::common::combat_obj_state_loading)
		{
			continue;
		}

		loading_end(objpair.second->get_uid());
	}	
}

void combat_t::deal_on_form_timer()
{
	m_timer.stop();

	if (m_step != proto::common::combat_step_form)
		return;

	for (auto objpair : m_objs)
	{
		if (NULL == objpair.second)
		{
			continue;
		}

		combat_before_form_end(objpair.second->get_uid());
	}
}

void combat_t::deal_on_prepare_timer()
{
//	for (auto objpair : m_objs)
// 	{
// 		if (NULL == objpair.second)
// 		{
// 			continue;
// 		}

// 		if (objpair.second->get_state() != proto::common::combat_obj_state_prepare)
// 		{
// 			continue;
// 		}

		proto::common::combat_form_data action;
//		disposition(objpair.second->get_uid(), action);
		disposition(get_attack_obj_uid(), action);

		fight_hero_ptr attacker = get_attack_hero();
		if (NULL == attacker)
		{
			log_error("combat_t attacker NULL");
			return;
		}

		if ( !(attacker->is_in_control() ||
			  attacker->get_attr(attrtype_enum::silent) > 0 ||
			  attacker->get_attr(attrtype_enum::taunt) > 0 || 
			  attacker->get_attr(attrtype_enum::confused) > 0 ||
			  attacker->is_dead()
			 )
		   ) 
		{
			change_role_auto_fight(true, get_attack_obj_uid());
		}

		check_disposition_finish();
//	}
}

void combat_t::deal_on_form_ready_timer()
{
	round_start_before();	
}

void combat_t::deal_on_disposition_ready_timer()
{
	play_start();
}

void combat_t::deal_on_play_timer()
{
	for (auto objpair : m_objs)
	{
		if (NULL == objpair.second)
		{
			continue;
		}

		if (objpair.second->get_state() != proto::common::combat_obj_state_play)
		{
			continue;
		}

		fight_play_end(objpair.second->get_uid(), m_hero_att_round);
	}
}

bool combat_t::auto_disposition(uint64_t obj_id)
{
	uint64_t fight_obj_id = obj_id;

	proto::common::combat_form_data action;
	// 判断下 如果当前的英雄是由系统控制的则自动部署
	if (m_p_att_fight_hero != NULL && m_p_att_fight_hero->get_control_by_system() != 0)
	{
		fight_obj_id = get_attack_obj_uid();
		disposition(fight_obj_id, action);
	}
	else
	{
		if (fight_obj_id == 0)
		{
			fight_obj_id = get_attack_obj_uid();
			fight_obj_ptr p_att_obj = find_obj(fight_obj_id);
			// 自动战斗状态则自动部署
			if (NULL != p_att_obj)
			{
				if (!p_att_obj->is_auto_fight())
				{
					m_is_pass_dis_timer = true;//非自动战斗没有定时器，也需要设置为true
					return true;
				}
				disposition(fight_obj_id, action);
			}
		}
		else
		{
			if (disposition(fight_obj_id, action))
			{
				syn_combat_obj_state();
			}
		}
	}

	//这些代码只能靠补丁维护了...
	if (obj_id > 0) {
		//如果已经设置过dis定时器，切dis定时器已经停止， 则这时候的自动操作要设置检测,否则会导致服务端一直没有触发技能时机
		if (m_is_pass_dis_timer == true && m_dis_timer.is_stop()) {
			return check_disposition_finish();
		}
		return true;
	}
	
	//第一回合的自动检测要延迟
	if (m_round == 1) {
		auto conf = GET_CONF(Comprehensive, comprehensive_common::cancel_auto_fight_time);
		uint32_t cancel_auto_fight_time = GET_COMPREHENSIVE_VALUE_1(conf);
		m_dis_timer.start(cancel_auto_fight_time, boost::weak_ptr<combat_t>(shared_from_this()), &combat_t::on_check_auto_dis_timer, env::server->get_ios());
		m_is_pass_dis_timer = true;
	}
	else {
		return check_disposition_finish();
	}
	return true;
}

bool combat_t::auto_loading()
{
	for (auto objpair : m_objs)
	{
		if (NULL == objpair.second)
		{
			continue;
		}

		if (!objpair.second->is_monster())
			continue;

		loading_end(objpair.second->get_uid());;
	}

	return true;
}

bool combat_t::auto_play()
{
	for (auto objpair : m_objs)
	{
		if (NULL == objpair.second) {
			continue;
		}

		//如果是人且在线才不走auto_play
		if ( !objpair.second->is_monster() ) {
			continue;
		}

		fight_play_end(objpair.second->get_uid(), m_hero_att_round);
	}

	return true;
}

bool combat_t::is_end()
{
	return m_step == proto::common::combat_step_end;
}

bool combat_t::is_single_pve()
{
	if (get_combat_type() == combat_type_single_role)
	{
		switch (m_fight_data.common().param().type())
		{
		case proto::common::fight_type_field_boss:
		case proto::common::fight_type_family_war:
		case proto::common::fight_type_task:
		case proto::common::fight_type_dark_monster:
		case proto::common::fight_type_light_monster:
		case proto::common::fight_type_page_pass_pk:
		case proto::common::fight_type_tower:
		case proto::common::fight_type_expedition:
		case proto::common::fight_type_against:
			return true;
		default:
			return false;
		}
	}
	return false;
}

bool combat_t::is_arena_pvp()
{
	switch (m_fight_data.common().param().type())
	{
	case proto::common::fight_type_cross_arena_rank:
	case proto::common::fight_type_cross_arena_train:
		return true;
	default:
		return false;
	}

	return false;
}

bool combat_t::is_un_arena_pvp()
{
	if (get_combat_type() == combat_type_single_role)
	{
		switch (m_fight_data.common().param().type())
		{
		case proto::common::fight_type_pk:
		case proto::common::fight_type_challenge:
			return true;
		default:
			return false;
		}
	}
	return false;
}

bool combat_t::on_role_login(uint64_t uid)
{
	proto::common::combat_action_data* process = NULL;
	if (m_state == proto::common::combat_state_fight)
	{
		if (m_process.find(m_hero_att_round) != m_process.end())
		{
			process = &(m_process[m_hero_att_round]);
		}
	}

	open_sync_user_combat_data(uid);

	syn_combat_recovery_data(uid);
	syn_combat_data(process, uid, true);
	syn_combat_obj_state(uid);
	return true;
}

bool combat_t::on_role_logout(uint64_t uid)
{
	// wys 注释 2017.5.15 断线重连要恢复战斗 取消直接结束战斗的做法
// 	fight_obj_ptr obj = find_obj(uid);
// 	if (NULL == obj)
// 	{
// 		return false;
// 	}
	return true;
// 	m_combat_result.set_win_camp(obj->get_enemy_camp());
// 	m_combat_result.set_result(proto::common::combat_fight_result_enemy_escape);
// 	return cancel();
}

void combat_t::all_escape(uint32_t camp)
{
	fight_obj_ptr p_obj = NULL;
	for (auto pair : m_objs)
	{
		p_obj = pair.second;
		if (NULL == p_obj)
			continue;
		if (p_obj->get_camp() != camp)
			continue;
		p_obj->escape();
	}
}

uint32_t combat_t::get_escape_num(uint32_t camp, uint32_t& all_num)
{
	uint32_t escape_num = 0;
	fight_obj_ptr p_obj = NULL;
	role_ptr p_role = NULL;
	for (auto pair : m_objs)
	{
		p_obj = pair.second;
		if (NULL == p_obj)
			continue;
		if (p_obj->get_type() != proto::common::SCENEOBJECT_USER || p_obj->get_camp() != camp)
			continue;
		p_role = role_manager_t::find_role(p_obj->get_uid());
		if (NULL == p_role)
		{
			log_error("not find role[%lu]", p_obj->get_uid());
			continue;
		}
		if (p_obj->is_escape()
			|| p_role->get_online_state() == common::ROLE_ONLINE_STATE_OFFLINE
			|| p_role->get_online_state() == common::ROLE_ONLINE_STATE_LOGOUT)
			++escape_num;
		++all_num;
	}
	return escape_num;
}

bool combat_t::request_escape(uint64_t uid)
{
	fight_obj_ptr p_escape_obj = find_obj(uid);
	if (NULL == p_escape_obj)
	{
		return false;
	}
	p_escape_obj->escape();
	
	

	uint32_t escape_num = 0;
	uint32_t all_num = 0;
	role_ptr p_role = NULL;

	// 判断是否队长，如果是队长则所有人一起逃跑
	p_role = role_manager_t::find_role(uid);
	if (NULL != p_role && p_role->is_team_leader() )
	{
		all_escape(p_escape_obj->get_camp());
	}
	else
	{
		//组队情况,队伍中还有人
		escape_num = get_escape_num(p_escape_obj->get_camp(), all_num);
		if (all_num - escape_num > 0)
		{
			escape_end(uid);
			return true;
		}
	}

	// 结束战斗
	m_combat_result.set_win_camp(p_escape_obj->get_enemy_camp());
	m_combat_result.set_result(proto::common::combat_fight_result_enemy_escape);
	return cancel();
}

void combat_t::escape_end(uint64_t uid)
{
	fight_obj_ptr p_escape_obj = find_obj(uid);
	if (NULL == p_escape_obj)
		return;

	bool is_auto_fight = p_escape_obj->is_auto_fight();

	p_escape_obj->set_auto_fight(1);
	p_escape_obj->escape_end();

	//如果是布阵状态,逃跑的人直接准备		
	if (!p_escape_obj->is_ready())
	{
		combat_before_form_end(uid);
	}

	// 发送给自己逃离信息
	role_ptr p_role = role_manager_t::find_role(uid);
	if (p_role != NULL)
	{
		// 设置战斗ID结束
		p_role->set_fight_uid(0);
		p_role->change_object_state(proto::common::object_state_normal);

		proto::client::gc_combat_result_ntf ntf;
		ntf.mutable_result()->set_win_camp(p_escape_obj->get_enemy_camp());
		ntf.mutable_result()->set_result(proto::common::combat_fight_result_none);
		p_role->send_msg_to_client(op_cmd::gc_combat_result_ntf, ntf);
	}

	// 判断当前是不是自己正在不是，如果是的话则自己主动部署
	if (!is_auto_fight && get_attack_obj_uid() == uid)
	{
		proto::common::combat_form_data action;
		
		disposition(get_attack_obj_uid(), action);

		check_disposition_finish();
	}
	

	if (NULL == p_role) {
		return;
	}

	// 组队相关操作
	if (p_role != NULL && p_role->get_troop_id() == 0)
		return;

	troop_ptr p_troop = game_troop_mgr_t::get_troop(p_role->get_troop_id());
	if (NULL == p_troop)
		return;
	p_troop->notify_event(proto::common::troop_event_escape, p_role->get_name());

	proto::server::ge_troop_escape_notify msg_to_center;
	env::server->send_msg_to_center(op_cmd::ge_troop_escape_notify, p_role->get_uid(), msg_to_center);
}

bool combat_t::change_role_auto_fight(bool is_auto, uint64_t uid, const proto::client::hero_auto_fight_data* hero_data)
{
	fight_obj_ptr p_fight_obj = fight_obj_ptr();
	if (uid != 0)
	{
		p_fight_obj = find_obj(uid);
		if (NULL == p_fight_obj)
		{
			log_error("combat_t p_fight_obj[%lu] NULL", uid);
			return false;
		}
		role_ptr p_role = role_manager_t::find_role(uid);
		if (NULL == p_role)
		{
			log_error("combat_t p_role NULL uid[%lu]", uid);
			return false;
		}
		// 检查是否能开启自动战斗
		if (!p_role->is_function_open(proto::common::function_code_type_combat_auto))
			return false;

// 		if (p_fight_obj->is_auto_fight() == is_auto)
// 			return true;
		p_fight_obj->set_auto_fight((uint32_t)is_auto);

		proto::client::gc_combat_auto_fight_reply reply;
		reply.set_need_change_hero(0);

		if (hero_data != NULL)
		{
			change_hero_auto_fight(hero_data);
			reply.mutable_hero_data()->CopyFrom(*hero_data);
			reply.set_need_change_hero(1);
		}

		reply.set_auto_fight((uint32_t)is_auto);
		p_fight_obj->send_msg_to_client(op_cmd::gc_combat_auto_fight_reply, reply);
	}
// 	else
// 	{
// 		for (auto pair : m_objs)
// 		{
// 			p_fight_obj = pair.second;
// 			if (NULL == p_fight_obj)
// 				continue;
// 			if (p_fight_obj->get_type() != proto::common::SCENEOBJECT_USER)
// 				continue;
// 			if (p_fight_obj->is_auto_fight() == is_auto)
// 				continue;
// 			p_fight_obj->set_auto_fight((uint32_t)is_auto);
// 
// 			proto::client::gc_combat_auto_fight_reply reply;
// 			reply.set_auto_fight((uint32_t)is_auto);
// 			p_fight_obj->send_msg_to_client(op_cmd::gc_combat_auto_fight_reply, reply);
// 		}
// 	}

	return true;
}

bool combat_t::change_hero_auto_fight(const proto::client::hero_auto_fight_data* hero_data)
{
	if (NULL == hero_data)
		return false;

	uint32_t size = hero_data->hero_list_size();
	fight_hero_ptr p_hero = fight_hero_ptr();
	uint64_t uid = 0;

	for (uint32_t i = 0; i < size; ++i)
	{
		const proto::client::hero_auto_fight_data_single &hero_single = hero_data->hero_list(i);
		uid = string_util_t::string_to_uint64(hero_single.hero_uid());
		p_hero = find_hero(uid);
		if (NULL == p_hero)
		{
			log_error("combat_t p_hero NULL uid[%lu]", uid);
			continue;
		}
		p_hero->change_auto_fight(hero_single.fight_mode());
	}
	return true;
}


uint32_t combat_t::get_fight_play_time(uint32_t round)
{
	auto fight_two_speed_conf = GET_CONF(Comprehensive, comprehensive_common::fight_three_speed_rate);
	uint32_t fight_two_speed_rate = GET_COMPREHENSIVE_VALUE_1(fight_two_speed_conf);

	uint32_t time = 1000;
	process_map::const_iterator it = m_process.find(round);
	if (it == m_process.end())
	{
		return time;
	}

	auto conf = GET_CONF(Comprehensive, comprehensive_common::fight_every_role_act_space_time);
	auto control_wait_time_conf = GET_CONF(Comprehensive, comprehensive_common::hero_control_wait_time);
	uint32_t act_space_time = GET_COMPREHENSIVE_VALUE_1(conf);
	uint32_t hero_control_wait_time = GET_COMPREHENSIVE_VALUE_1(control_wait_time_conf);

	fight_hero_ptr p_fight_hero = fight_hero_ptr();
	const proto::common::combat_action_data& action_data = it->second;
	uint32_t skill_id = 0;
	uint32_t play_time = 0;
	
	for (const proto::common::combat_act& action : action_data.actions())
	{
		if ( (action.act_type() != proto::common::combat_act_type_skill) &&
			 (action.act_type() != proto::common::combat_act_type_counter) &&
			 (action.act_type() != proto::common::combat_act_type_combo) &&
			 (action.act_type() != proto::common::combat_act_type_follow) &&
			 (action.act_type() != proto::common::combat_act_type_pincer_attack) &&
			 (action.act_type() != proto::common::combat_act_type_camera) &&
			 (action.act_type() != proto::common::combat_act_type_story_play) &&
			 (action.act_type() != proto::common::combat_act_type_trigger_skill))
		{
			continue;
		}

		p_fight_hero = find_hero(string_util_t::string_to_uint64(action.attacker()));
		if (NULL == p_fight_hero)
		{
			log_error("combat_t p_fight_hero[%s] NULL", action.attacker().c_str());
			continue;
		}

		if (p_fight_hero->is_in_control())
		{
			time += hero_control_wait_time;
		}

		skill_id = action.act_value();
		SkillEffectTable* skill_conf = p_fight_hero->get_skill_effect(skill_id);
		if (NULL == skill_conf)
		{
			log_error("get_fight_play_time skill[%d] skill_conf NULL", skill_id);
			continue;
		}

		if (action.act_type() == proto::common::combat_act_type_camera)
		{
			play_time += skill_conf->camera_skill_play_time();
		}
		else if (action.act_type() == proto::common::combat_act_type_story_play)
		{
			uint32_t story_play_time = 0;
			GuideFightTable* guide_conf = GET_CONF(GuideFightTable, action.act_value());
			if (NULL == guide_conf)
				log_error("get_fight_play_time skill[%d] skill_conf NULL", skill_id);
			else
				story_play_time = guide_conf->play_time();
			play_time += story_play_time;
		}
		else
		{
			play_time = skill_conf->play_time();
			m_total_time += play_time;
		}

		time += (play_time + act_space_time);
	}

	// PVP战斗加速 wys 2017.6.10 暂时所有战斗都是加速处理
// 	if (m_fight_data.common().param().type() == proto::common::fight_type_cross_arena_rank ||
// 		m_fight_data.common().param().type() == proto::common::fight_type_cross_arena_train ||
// 		m_fight_data.common().param().type() == proto::common::fight_type_pk ||
// 		m_combat_type == combat_type_troop)
	if (fight_two_speed_rate == 0)
	{
		log_error("get_fight_play_time fight_two_speed_rate = 0");
		return time;
	}
	
	time = (uint32_t)((float)time / ((float)fight_two_speed_rate / 10000.f));
	
	return time;
}

fight_hero_ptr combat_t::get_attack_hero()
{
	return m_p_att_fight_hero;
}

uint64_t combat_t::get_attack_obj_uid()
{
	fight_hero_ptr p_hero = get_attack_hero();
	if (NULL == p_hero)
	{
		log_error("p_hero NULL");
		return 0;
	}
	return p_hero->get_master();
}

bool combat_t::check_tactic_valid(uint32_t camp)
{
	uint32_t tactic_id = 0;
	if (camp == proto::common::combat_camp_1)
	{
		tactic_id = m_fight_data.self().tactic();
	}
	else
	{
		tactic_id = m_fight_data.enemy().tactic();
	}

	Tactic* tactic_conf = GET_CONF(Tactic, tactic_id);
	if (NULL == tactic_conf)
	{
		return false;
	}

	const pos_hero* camp_heros = get_camp_pos_hero(camp);
	if (NULL == camp_heros)
	{
		return false;
	}

	size_t cur_count = 0;
	for (auto eye : tactic_conf->eye())
	{
		pos_hero::const_iterator it = camp_heros->find(eye);
		if (it == camp_heros->end())
		{
			continue;
		}

		fight_hero_ptr hero = find_hero(it->second);
		if (NULL == hero)
		{
			continue;
		}

		if (hero->is_dead())
		{
			continue;
		}

		++cur_count;
	}

	if (cur_count > 0)
	{
		return true;
	}

	return false;
}

bool combat_t::on_fight_hero_dead(fight_hero_ptr hero)
{
	if (NULL == hero)
	{
		return false;
	}

// 	if (!check_tactic_valid(hero->get_camp()))
// 	{
// 		uint32_t tactic_id = 0;
// 		if (hero->get_camp() == proto::common::combat_camp_1)
// 		{
// 			if (m_fight_data.self().tactic_switch() == 0)
// 			{
// 				return false;
// 			}
// 
// 			m_fight_data.mutable_self()->set_tactic_switch(0);
// 			tactic_id = m_fight_data.self().tactic();
// 		}
// 		else
// 		{
// 			if (m_fight_data.enemy().tactic_switch() == 0)
// 			{
// 				return false;
// 			}
// 
// 			m_fight_data.mutable_enemy()->set_tactic_switch(0);
// 			tactic_id = m_fight_data.enemy().tactic();
// 		}
// 
// 		do_tactic_buff(hero->get_camp(), false);
// 		add_combat_act(proto::common::combat_act_type_tactic_off, tactic_id, hero->get_uid(), 0);
// 		return true;
// 	}

	return false;
}

bool combat_t::do_tactic_buff(uint32_t camp, bool is_add)
{
	uint32_t tactic_id = 0;
	if (camp == proto::common::combat_camp_1)
	{
		tactic_id = m_fight_data.self().tactic();
	}
	else
	{
		tactic_id = m_fight_data.enemy().tactic();
	}

	Tactic* tactic_conf = GET_CONF(Tactic, tactic_id);
	if (NULL == tactic_conf)
	{
		return false;
	}

	const pos_hero* camp_heros = get_camp_pos_hero(camp);
	if (NULL == camp_heros)
	{
		return false;
	}

	for (auto pos_buff : tactic_conf->attr_buff())
	{
		pos_hero::const_iterator it = camp_heros->find(pos_buff.first);
		if (it == camp_heros->end())
		{
			continue;
		}

		fight_hero_ptr hero = find_hero(it->second);
		if (NULL == hero)
		{
			continue;
		}

		if (is_add)
		{
			hero->m_buff_mgr.add_buff(pos_buff.second);
		}
		else
		{
			hero->m_buff_mgr.remove_buff(pos_buff.second);
		}
	}

	return true;
}

uint64_t combat_t::get_fight_uid()
{
	return m_fight_uid;
}

uint32_t combat_t::get_round()
{
	return m_round;
}

uint32_t combat_t::get_hero_att_round()
{
	return m_hero_att_round;
}

uint32_t combat_t::get_self_death_hero_num()
{
	uint32_t dead_num = 0;
	for (auto pair : m_heros)
	{
		if(pair.second == NULL)
			continue;
		if (pair.second->get_camp() == proto::common::combat_camp_1)
		{
			if (pair.second->is_dead())
			{
				++dead_num;
			}
		}
	}
	return dead_num;
}

uint32_t combat_t::get_hero_injured(uint64_t hero_uid)
{
	fight_hero_ptr hero = find_hero(hero_uid);
	if (NULL == hero)
	{
		return 0;
	}

	return hero->get_count_data().once_injured;
}

const proto::common::fight_data& combat_t::get_fight_data()
{
	return m_fight_data;
}

bool combat_t::update_fight(const proto::common::fight_state& state)
{
	bool ret = false;
	ret &= update_fight(proto::common::combat_camp_1, state.self());
	ret &= update_fight(proto::common::combat_camp_2, state.enemy());
	return ret;
}

bool combat_t::update_fight(uint32_t camp, const proto::common::hero_state_data& state)
{
	for (int i = 0; i < state.states_size(); ++i)
	{
		const proto::common::hero_state& hs = state.states(i);
		fight_hero_ptr hero = get_hero_by_camp_and_pos(camp, hs.pos());
		if (NULL == hero)
		{
			continue;
		}

		hero->update_fight(hs);
	}

	return true;
}

bool combat_t::sync_enemy_hp(const proto::common::hero_state_data& state)
{
	uint32_t now = common::time_util_t::now_msecs();

	if ( now >= m_sync_hurt_time + s_sync_tick_msec )
	{
		m_sync_hurt_time = now;
		proto::client::gc_sync_enemy_hurt_hp ntf;
		proto::common::hero_state_data *p_data = ntf.mutable_hero_data();
		for (int i = 0; i < state.states_size(); ++i)
		{
			const proto::common::hero_state& hs = state.states(i);
			fight_hero_ptr hero = get_hero_by_camp_and_pos(proto::common::combat_camp_2, hs.pos());
			if (NULL == hero)
				continue;

			hero->sync_hp(hs.hp());

			proto::common::hero_state *p_hurt_data = p_data->add_states();
			p_hurt_data->set_hp(hero->get_hp());
			p_hurt_data->set_pos(hero->get_pos());
			p_hurt_data->set_hero_uid( common::string_util_t::uint64_to_string(hero->get_uid()));
			log_error("sync  hurt_hp[%u] hero_hp left[%u]", hs.hp(), hero->get_hp());
		}

		//同步扣血数据给自己
		send_msg_to_all(op_cmd::gc_sync_enemy_hurt_hp, ntf);
		m_hurt_hp_map.clear();
	}

	return true;
}

proto::common::combat_action_data* combat_t::get_cur_process()
{
	return &m_process[m_hero_att_round];
}

bool combat_t::cancel()
{
	m_timer.stop();
	return combat_end();
}

bool combat_t::all_fail()
{
	m_combat_result.set_win_camp(proto::common::combat_camp_none);
	m_combat_result.set_result(proto::common::combat_fight_result_all_lose);
	return cancel();
}

bool combat_t::all_success()
{
	m_combat_result.set_win_camp(proto::common::combat_camp_1);
	m_combat_result.set_result(proto::common::combat_fight_result_win);
	return cancel();
}


uint32_t combat_t::get_sp(uint64_t obj_id)
{
	fight_obj_ptr obj = find_obj(obj_id);
	if (NULL == obj)
	{
		return 0;
	}

	return obj->get_sp();
}

void combat_t::set_sp(uint64_t obj_id, uint32_t value)
{
	fight_obj_ptr obj = find_obj(obj_id);
	if (NULL == obj)
	{
		return;
	}

	obj->set_sp(value);
}


void combat_t::add_sp(uint64_t obj_id, uint32_t value)
{
	auto max_sp_conf = GET_CONF(Comprehensive, comprehensive_common::MAX_SP);
	uint32_t max_sp = GET_COMPREHENSIVE_VALUE_1(max_sp_conf);

	fight_obj_ptr obj = find_obj(obj_id);
	if (NULL == obj)
	{
		return;
	}

	uint32_t cur_sp = obj->get_sp();
	uint32_t add = value;
	if (cur_sp + add > max_sp)
	{
		add = max_sp - cur_sp;
	}

	if (add != 0)
	{
		obj->set_sp(cur_sp + add);
	}
	
//	add_combat_act(proto::common::combat_act_type_sp_inc, value, obj_id, obj->get_sp());
}

void combat_t::sub_sp(uint64_t obj_id, uint32_t value)
{
	fight_obj_ptr obj = find_obj(obj_id);
	if (NULL == obj)
	{
		return;
	}

	uint32_t cur_sp = obj->get_sp();
	uint32_t sub_value = 0;
	if (cur_sp < value)
	{
		sub_value = cur_sp;
	}
	else
	{
		sub_value = value;
	}

	obj->set_sp(cur_sp - sub_value);
//	add_combat_act(proto::common::combat_act_type_sp_dec, sub_value, obj_id, obj->get_sp());
}

void combat_t::sub_disposition_sp(uint64_t obj_id, uint32_t value)
{
	fight_obj_ptr obj = find_obj(obj_id);
	if (NULL == obj)
	{
		return;
	}
	uint32_t cur_sp = obj->get_disposition_sp();
	uint32_t sub_value = 0;
	if (cur_sp < value)
	{
		sub_value = cur_sp;
	}
	else
	{
		sub_value = value;
	}

	obj->set_disposition_sp(cur_sp - sub_value);
}

uint32_t combat_t::get_disposition_sp(uint64_t obj_id)
{
	fight_obj_ptr obj = find_obj(obj_id);
	if (NULL == obj)
	{
		return 0;
	}

	return obj->get_disposition_sp();
}

void combat_t::add_combat_act(proto::common::combat_act_type type, uint32_t value, uint64_t target, uint32_t remain_value)
{
	proto::common::combat_act* ca = m_process[m_hero_att_round].add_actions();
	if (NULL == ca)
	{
		log_error("ca is null");
		return;
	}

	ca->set_act_type(type);
	ca->set_act_value(value);
	ca->add_target(string_util_t::uint64_to_string(target));
	ca->set_step(m_state);
	ca->set_remain_value(remain_value);

	//如果不用回放，清除上一个回合的数据
	if( m_is_replay == false){
		clear_last_combat_log();
	}
}

fight_hero_ptr combat_t::select_max_atk_target(fight_hero_ptr attaker, SkillEffectTable* skill_conf)
{
	if (NULL == attaker)
	{
		log_error("attaker is null");
		return NULL;
	}

	if (NULL == skill_conf)
	{
		log_error("skill_conf is null");
		return NULL;
	}

	const pos_hero* camp_heros = get_skill_target_camp_pos_hero(attaker, skill_conf);
	if (NULL == camp_heros)
	{
		log_error("camp_heros is null");
		return NULL;
	}

	int cmp_atk = 0;
	fight_hero_ptr find = NULL;
	for (auto pospair : *camp_heros)
	{
		fight_hero_ptr hero = find_hero(pospair.second);
		if (NULL == hero)
		{
			continue;
		}

		if (!check_skill_target_type(skill_conf, attaker, hero))
		{
			continue;
		}

		int hero_atk = hero->get_attr(attrtype_enum::atk);
		if (cmp_atk == 0)
		{
			cmp_atk = hero_atk;
			find = hero;
		}
		else
		{
			if (hero_atk < cmp_atk)
			{
				cmp_atk = hero_atk;
				find = hero;
			}
		}
	}

	return find;
}

fight_hero_ptr combat_t::select_weak_target(fight_hero_ptr attaker, SkillEffectTable* skill_conf)
{
	if (NULL == attaker)
	{
		log_error("attaker is null");
		return NULL;
	}

	if (NULL == skill_conf)
	{
		log_error("skill_conf is null");
		return NULL;
	}

	const pos_hero* camp_heros = get_skill_target_camp_pos_hero(attaker, skill_conf);
	if (NULL == camp_heros)
	{
		log_error("camp_heros is null");
		return NULL;
	}

	fight_hero_ptr find = NULL;
	for (auto pospair : *camp_heros)
	{
		fight_hero_ptr hero = find_hero(pospair.second);
		if (NULL == hero)
		{
			continue;
		}

		if (!check_skill_target_type(skill_conf, attaker, hero))
		{
			continue;
		}

		if (!hero->is_weak_profession())
		{
			continue;
		}

		return hero;
	}

	return NULL;
}

fight_hero_ptr combat_t::select_min_hp_target(fight_hero_ptr attaker, SkillEffectTable* skill_conf)
{
	if (NULL == attaker)
	{
		log_error("attaker is null");
		return NULL;
	}

	if (NULL == skill_conf)
	{
		log_error("skill_conf is null");
		return NULL;
	}

	const pos_hero* camp_heros = get_skill_target_camp_pos_hero(attaker, skill_conf);
	if (NULL == camp_heros)
	{
		log_error("camp_heros is null");
		return NULL;
	}

	int cmp_hp = 0;
	uint32_t max_hp = 0;
	fight_hero_ptr find = NULL;

	for (auto pospair : *camp_heros)
	{
		fight_hero_ptr hero = find_hero(pospair.second);
		if (NULL == hero)
		{
			continue;
		}

		if (!check_skill_target_type(skill_conf, attaker, hero))
		{
			continue;
		}

		max_hp = hero->get_attr(attrtype_enum::max_hp);
		if (max_hp == 0)
		{
			log_error("select_min_hp_target max_hp = 0 hero[%lu][%d]", hero->get_uid(), hero->get_tid());
			continue;
		}
		int hero_hp = (float)hero->get_hp() / (float)max_hp * 10000;
		if (cmp_hp == 0)
		{
			cmp_hp = hero_hp;
			find = hero;
		}
		else
		{
			if (hero_hp < cmp_hp)
			{
				cmp_hp = hero_hp;
				find = hero;
			}
		}
	}

	return find;
}

fight_hero_ptr combat_t::select_min_att_speed_target(uint32_t camp)
{
	std::vector<fight_hero_ptr> hero_list;
	get_camp_alive_heros(hero_list, camp);

	int32_t cmp_att_speed = 0;
	fight_hero_ptr find = NULL;

	for (uint32_t i = 0; i < hero_list.size(); ++i)
	{
		fight_hero_ptr hero = hero_list[i];
		if (NULL == hero)
		{
			continue;
		}

		if (cmp_att_speed == 0)
		{
			cmp_att_speed = hero->get_att_speed();
			find = hero;
		}
		else
		{
			if (hero->get_att_speed() < cmp_att_speed)
			{
				cmp_att_speed = hero->get_att_speed();
				find = hero;
			}
		}
	}

	return find;
}

bool combat_t::select_hero_id_target(std::vector<fight_hero_ptr>& result,uint32_t camp, uint32_t hero_id)
{
	std::vector<fight_hero_ptr> hero_list;
	get_camp_alive_heros(hero_list, camp);
	for (uint32_t i = 0; i < hero_list.size(); ++i) 
	{
		fight_hero_ptr hero = hero_list[i];
		if (NULL == hero)
		{
			continue;
		}
		if (hero->get_tid() == hero_id)
		{
			result.push_back(hero);
		}
	}
	return true;
}


fight_hero_ptr combat_t::select_min_hp_target(uint32_t camp, uint64_t except)
{
	std::vector<fight_hero_ptr> hero_list;
	get_camp_alive_heros(hero_list, camp);

	int cmp_hp = 0;
	uint32_t max_hp = 0;
	fight_hero_ptr find = NULL;

	for (uint32_t i = 0; i < hero_list.size(); ++i)
	{
		fight_hero_ptr hero = hero_list[i];
		if (NULL == hero || except == hero->get_uid())
		{
			continue;
		}

		max_hp = hero->get_attr(attrtype_enum::max_hp);
		if (max_hp == 0)
		{
			log_error("select_min_hp_target max_hp = 0 hero[%lu][%d]", hero->get_uid(), hero->get_tid());
			continue;
		}
		int hero_hp = (float)hero->get_hp() / (float)max_hp * 10000;
		if (cmp_hp == 0)
		{
			cmp_hp = hero_hp;
			find = hero;
		}
		else
		{
			if (hero_hp < cmp_hp)
			{
				cmp_hp = hero_hp;
				find = hero;
			}
		}
	}

	return find;
}

fight_hero_ptr combat_t::select_max_hp_target(uint32_t camp)
{
	std::vector<fight_hero_ptr> hero_list;
	get_camp_alive_heros(hero_list, camp);

	int cmp_hp = 0;
	uint32_t max_hp = 0;
	fight_hero_ptr find = NULL;

	for (uint32_t i = 0; i < hero_list.size(); ++i)
	{
		fight_hero_ptr hero = hero_list[i];
		if (NULL == hero)
		{
			continue;
		}

		max_hp = hero->get_attr(attrtype_enum::max_hp);
		if (max_hp == 0)
		{
			log_error("select_min_hp_target max_hp = 0 hero[%lu][%d]", hero->get_uid(), hero->get_tid());
			continue;
		}
		int hero_hp = (float)hero->get_hp() / (float)max_hp * 10000;
		if (cmp_hp == 0)
		{
			cmp_hp = hero_hp;
			find = hero;
		}
		else
		{
			if (hero_hp > cmp_hp)
			{
				cmp_hp = hero_hp;
				find = hero;
			}
		}
	}

	return find;
}

bool combat_t::select_min_hp_target(fight_hero_ptr attaker, SkillEffectTable* skill_conf, std::vector<fight_hero_ptr>& targets)
{
	if (NULL == attaker)
	{
		log_error("attaker is null");
		return false;
	}

	if (NULL == skill_conf)
	{
		log_error("skill_conf is null");
		return false;
	}

	const pos_hero* camp_heros = get_skill_target_camp_pos_hero(attaker, skill_conf);
	if (NULL == camp_heros)
	{
		log_error("camp_heros is null");
		return false;
	}

	uint32_t max_hp = 0;
	fight_hero_ptr find = NULL;

	std::multimap<uint32_t, fight_hero_ptr> temp_targets;

	for (auto pospair : *camp_heros)
	{
		fight_hero_ptr hero = find_hero(pospair.second);
		if (NULL == hero)
		{
			continue;
		}

		if (!check_skill_target_type(skill_conf, attaker, hero))
		{
			continue;
		}

		max_hp = hero->get_attr(attrtype_enum::max_hp);
		if (max_hp == 0)
		{
			log_error("select_min_hp_target max_hp = 0 hero[%lu][%d]", hero->get_uid(), hero->get_tid());
			continue;
		}
		int hero_hp = (float)hero->get_hp() / (float)max_hp * 10000;
		temp_targets.insert(std::make_pair(hero_hp, hero));
	}

	std::multimap<uint32_t, fight_hero_ptr>::iterator iter = temp_targets.begin();
	uint32_t count = 0;
	for (; iter != temp_targets.end(); ++iter)
	{
		if(count++ > skill_conf->target_count())
			break;
		targets.push_back(iter->second);
	}

	return true;
}

bool combat_t::find_some_grater_hp_target(fight_hero_ptr attaker, SkillEffectTable* skill_conf, std::vector<fight_hero_ptr>& targets, fight_hero_ptr target)
{
	if (NULL == attaker)
	{
		log_error("attaker is null");
		return false;
	}

	if (NULL == skill_conf)
	{
		log_error("skill_conf is null");
		return false;
	}
	
	if( NULL == target){
		log_error("target is null");
		return false;
	}

	if (target->get_attr(attrtype_enum::max_hp) == 0 ) {
		log_error("target[%u] max_hp is zero", target->get_tid() );
		return false;
	}

	//如果指定目标存在
	if (NULL != target) {
		uint32_t hp_value = (target->get_hp() * 10000) / ( target->get_attr(attrtype_enum::max_hp));
		if (hp_value >= skill_conf->hp_condtion()) {
			targets.push_back(target);
			return true;
		}
	}

	const pos_hero* camp_heros = get_skill_target_camp_pos_hero(attaker, skill_conf);
	if (NULL == camp_heros)
	{
		log_error("camp_heros is null");
		return false;
	}

	uint32_t cur_hp_rate = 0;
	fight_hero_ptr find = NULL;

	std::multimap<uint32_t, fight_hero_ptr> temp_targets;

	for (auto pospair : *camp_heros)
	{
		fight_hero_ptr hero = find_hero(pospair.second);
		if (NULL == hero)
		{
			continue;
		}

		if (!check_skill_target_type(skill_conf, attaker, hero))
		{
			continue;
		}
		
		if (hero->get_attr(attrtype_enum::max_hp) == 0) {
			continue;	
		}

		cur_hp_rate = (hero->get_hp() * 10000) % hero->get_attr(attrtype_enum::max_hp);
		if (cur_hp_rate <= skill_conf->hp_condtion()) {
			continue;
		}
		temp_targets.insert(std::make_pair(cur_hp_rate, hero));
	}

	std::multimap<uint32_t, fight_hero_ptr>::iterator iter = temp_targets.begin();
	uint32_t count = 0;
	for (; iter != temp_targets.end(); ++iter)
	{
		if (NULL == iter->second) {
			continue;	
		}

		if (count++ > skill_conf->target_count())
			break;

		targets.push_back(iter->second);
	}

	return true;
}


void combat_t::add_attack_count(uint64_t obj_id, uint32_t value /*= 1*/)
{
	fight_obj_ptr obj = find_obj(obj_id);
	if (NULL == obj)
	{
		return;
	}

	obj->set_acttack_count(obj->get_acttack_count() + value);

	if (obj->get_type() != proto::common::SCENEOBJECT_USER)
	{
		return;
	}

	//add_combat_act(proto::common::combat_act_type_syn_atk_count, value, obj_id, obj->get_acttack_count());

	auto fire_count = GET_CONF(Comprehensive, comprehensive_common::init_fire_count);
	auto fire_regain = GET_CONF(Comprehensive, comprehensive_common::init_fire_regain);
	if (NULL == fire_count || NULL == fire_regain)
	{
		log_error("Comprehensive [%u][%u] config error", comprehensive_common::init_fire_count, comprehensive_common::init_fire_regain);
		return;
	}

	uint32_t count = GET_COMPREHENSIVE_VALUE_1(fire_count);
	uint32_t regain = GET_COMPREHENSIVE_VALUE_1(fire_regain);
	if (count == 0 || regain == 0)
	{
		log_error("Comprehensive [%u][%u] config error", comprehensive_common::init_fire_count, comprehensive_common::init_fire_regain);
		return;
	}

	uint32_t add_count = obj->get_acttack_count() / count;
	if (add_count >= 1)
	{
		uint32_t add_digit = obj->get_acttack_count() % count;
		add_sp(obj_id, add_count * regain);
		obj->set_acttack_count(add_digit);
		//add by hy 老的出手次数不用，只用新的 多段的
		//add_combat_act(proto::common::combat_act_type_syn_atk_count, add_count * count, obj_id, obj->get_acttack_count());
	}
}

bool combat_t::is_pve(uint32_t& npc_camp)
{
	if (m_camp_data[proto::common::combat_camp_1].camp_obj_type != proto::common::SCENEOBJECT_USER)
	{
		npc_camp = proto::common::combat_camp_1;
		return true;
	}

	if (m_camp_data[proto::common::combat_camp_2].camp_obj_type != proto::common::SCENEOBJECT_USER)
	{
		npc_camp = proto::common::combat_camp_2;
		return true;
	}

	return false;
}

bool combat_t::is_pve()
{
	if (m_camp_data[proto::common::combat_camp_1].camp_obj_type != proto::common::SCENEOBJECT_USER)
	{
		return true;
	}

	if (m_camp_data[proto::common::combat_camp_2].camp_obj_type != proto::common::SCENEOBJECT_USER)
	{
		return true;
	}

	return false;
}

uint32_t combat_t::get_initiative_camp()
{
	if (m_camp_data[proto::common::combat_camp_1].initiative)
	{
		return proto::common::combat_camp_1;
	}
	else
	{
		return proto::common::combat_camp_2;
	}
}

void combat_t::get_self_death_hero(uint64_t role_uid, std::set<uint64_t>& death_hero_uids) const
{
	for (const auto& heropair : m_heros)
	{
		if (heropair.second == NULL)
			continue;

		if (heropair.second->get_master() == role_uid)
		{
			if (heropair.second->is_dead())
			{
				death_hero_uids.insert(heropair.second->get_unique_id());
			}
		}
	}
}

int32_t combat_t::get_max_att_speed()
{
	return m_max_att_speed;
}

void combat_t::set_mark_id(uint32_t mark_id, uint64_t fight_hero_uid)
{
	fight_hero_ptr p_fight_hero = NULL;
	fight_hero_map::iterator iter = m_heros.begin();
	for (; iter != m_heros.end(); ++iter)
	{
		p_fight_hero = iter->second;
		if (p_fight_hero != NULL)
		{
			if (p_fight_hero->get_uid() == fight_hero_uid)
				p_fight_hero->set_mark_id(mark_id);
			else
				p_fight_hero->set_mark_id(0);
		}
	}
}

uint32_t combat_t::get_role_camp(uint64_t uid) const
{
	for (const proto::common::obj_single& obj : m_fight_data.self().objs().obj())
	{
		uint64_t role_uid = common::string_util_t::string_to_uint64(obj.object_uid());
		if (uid == role_uid)
		{
			return obj.camp();
		}
	}

	for (const proto::common::obj_single& obj : m_fight_data.enemy().objs().obj())
	{
		uint64_t role_uid = common::string_util_t::string_to_uint64(obj.object_uid());
		if (uid == role_uid)
		{
			return obj.camp();
		}
	}

	return proto::common::combat_camp_none;
}

bool combat_t::fill_initiator_save_data(proto::common::fight_save_data* p_data)
{
	if (NULL == p_data)
	{
		log_error("combat_t p_data NULL");
		return false;
	}
	uint64_t initiator = string_util_t::string_to_uint64(m_fight_data.common().fight_initiator());
	
	fight_hero_ptr p_hero = fight_hero_ptr();
	proto::common::hero_save_data *p_hero_save_data = NULL;

 	for (auto pair : m_heros)
 	{
 		p_hero = pair.second;
 		if(NULL == p_hero)
 			continue;
 		if (p_hero->get_master() == initiator)
		{
			// 主公不存
			if (p_hero->get_type() == 0)
			{
				continue;
			}
 			p_hero_save_data = p_data->add_hero_list();
 			p_hero->peek_save_data(p_hero_save_data);
 		}
 	}
	return true;
}

bool combat_t::fill_enemy_save_data(proto::common::fight_save_data* p_data)
{
	if (NULL == p_data)
	{
		log_error("combat_t p_data NULL");
		return false;
	}

	fight_hero_ptr p_hero = fight_hero_ptr();
	proto::common::hero_save_data *p_hero_save_data = NULL;

	for (auto pair : m_heros)
	{
		p_hero = pair.second;
		if (NULL == p_hero)
			continue;
		if (p_hero->get_camp() == proto::common::combat_camp_2)
		{
			// 主公不存
			if (p_hero->get_type() == 0)
			{
				continue;
			}
			p_hero_save_data = p_data->add_hero_list();
			p_hero->peek_save_data(p_hero_save_data);
		}
	}
	return true;
}

uint32_t combat_t::get_against_id()
{
	return m_fight_data.common().param().against_id();
}

std::pair<uint32_t, uint32_t> combat_t::get_exp(bool is_win)
{
	if (!is_pve())
	{
		return std::make_pair(0,0);
	}
	if (get_fight_type() == proto::common::fight_type_expedition)
	{
		return std::make_pair(0, 0);
	}
	if (get_fight_type() == proto::common::fight_type_cross_arena_rank || get_fight_type() == proto::common::fight_type_cross_arena_train)
	{
		return std::make_pair(0, 0);
	}
	if (get_fight_type() != proto::common::fight_type_field_boss && !is_win)
	{
		return std::make_pair(0, 0);
	}
	uint32_t against_id = get_against_id();
	auto p_conf = GET_CONF(FightAgainst, against_id);
	if (NULL == p_conf)
	{
		log_error("NULL == p_against_conf[%d]", against_id);
		return std::make_pair(0, 0);
	}
	return std::make_pair(p_conf->hero_exp(), p_conf->role_exp());
}

uint32_t combat_t::get_obj_count(uint32_t camp)
{
	

	fight_obj_map::iterator iter = m_objs.begin();
	fight_obj_ptr p_obj = fight_obj_ptr();
	uint32_t count = 0;

	for (; iter != m_objs.end(); ++iter)
	{
		p_obj = iter->second;
		if(NULL == p_obj)
			continue;

		if (/*p_obj->get_type() == proto::common::SCENEOBJECT_USER &&*/ p_obj->get_camp() == camp)
			++count;
	}
	 
	return count;
}

void combat_t::get_role_heros(uint64_t role_uid, std::vector<uint64_t>& hero_list)
{
	fight_hero_map::iterator iter = m_heros.begin();
	fight_hero_ptr p_hero = fight_hero_ptr();

	for (; iter != m_heros.end(); ++iter)
	{
		p_hero = iter->second;
		if(NULL == p_hero)
			continue;

		if (p_hero->get_master() == role_uid)
			hero_list.push_back(p_hero->get_unique_id());
	}
}

void combat_t::get_task_monster(std::vector<uint32_t>& monster_list)
{
	if (!is_pve())
	{
		return;
	}
	fight_hero_map::iterator iter = m_heros.begin();
	fight_hero_ptr p_hero = fight_hero_ptr();

	for (; iter != m_heros.end(); ++iter)
	{
		p_hero = iter->second;
		if (NULL == p_hero)
		{
			log_error("get_task_monster p_hero NULL");
			continue;
		}
		if(p_hero->get_camp() != proto::common::combat_camp_2 || !p_hero->is_dead())
			continue;
		auto p_conf = GET_CONF(MonsterHero, p_hero->get_tid());
		if (NULL == p_conf)
		{
			log_error("NULL == p_conf MonsterHero[%d]", p_hero->get_tid());
			continue;
		}
		monster_list.push_back(p_conf->task_sign());
	}
}

void combat_t::get_camp_fight_obj_uid(uint32_t camp, std::vector<uint64_t>& obj_uid_list)
{
	fight_obj_map::iterator iter = m_objs.begin();
	for (; iter != m_objs.end(); ++iter)
	{
		if (iter->second == NULL)
		{
			log_error("get_camp_fight_obj_uid fight_obj[%lu] NULL", iter->first);
			continue;
		}
		if(iter->second->get_camp() != camp)
			continue;
		obj_uid_list.push_back(iter->first);
	}
}

bool combat_t::ignore_lineup(uint32_t fight_type)
{
	FightTypeTable* conf = GET_CONF(FightTypeTable, fight_type);
	if (NULL == conf)
	{
		return false;
	}
	else
	{
		return conf->ignore_lineup() != 0;
	}
}

bool combat_t::auto_fight_inherit(uint32_t fight_type)
{
	FightTypeTable* conf = GET_CONF(FightTypeTable, fight_type);
	if (NULL == conf)
	{
		return true;
	}
	else
	{
		return conf->auto_fight_inherit() != 0;
	}
}

uint32_t combat_t::get_max_watch_num(uint32_t fight_type)
{
	FightTypeTable* conf = GET_CONF(FightTypeTable, fight_type);
	if (NULL == conf)
	{
		auto conf = GET_CONF(Comprehensive, comprehensive_common::max_normal_combat_watch_num);
		return GET_COMPREHENSIVE_VALUE_1(conf);
	}
	else
	{
		return conf->max_visit();
	}
}

bool combat_t::can_watch(uint32_t watch_num)
{
	uint32_t max_normal_combat_watch_num = get_max_watch_num(get_fight_type());

	if (get_watching_role_num() + watch_num <= max_normal_combat_watch_num)
		return true;
	return false;
}

void combat_t::remove_all_watching_roles()
{
	std::set<uint64_t>::iterator iter = m_watching_roles.begin();

	for (; iter != m_watching_roles.end(); ++iter)
	{
		role_ptr role = role_manager_t::find_role(*iter);
		if (NULL != role)
		{
			role->set_watching_fight_uid(0, false);
		}
	}
	m_watching_roles.clear();
}

void combat_t::add_round_use_skill_list(const round_use_skill_info& skill_info)
{
	m_round_use_skill_list.push_back(skill_info);
}

void combat_t::deal_round_use_skill_list()
{
	fight_hero_ptr p_fight_hero = NULL;
	for (uint32_t i = 0; i < m_round_use_skill_list.size(); ++i)
	{
		round_use_skill_info& skill_info = m_round_use_skill_list[i];
		
		p_fight_hero = skill_info.attacker;
		if ( p_fight_hero != NULL && !p_fight_hero->is_dead() )
		{
			p_fight_hero->do_attack(skill_info.skill_conf, skill_info.select, skill_info.type, NULL, skill_info.trigger_by_buff, skill_info.can_counter);
		}
	}
}

void combat_t::remove_all_round_use_skill_list()
{
	m_round_use_skill_list.clear();
}

void combat_t::on_end_timer(const boost::weak_ptr<combat_t> pw_combat, const boost::system::error_code& ec)
{
	if (ec == boost::asio::error::operation_aborted)
	{
		return;
	}
	else if (ec)
	{
		log_error("Timer error: %s", ec.message().c_str());
		return;
	}

	combat_ptr p_combat(pw_combat.lock());
	if (NULL == p_combat)
	{
		log_error("on_end_timer p_combat NULL");
		return;
	}
	p_combat->all_fail();
}

bool combat_t::is_allow_reconnect()
{
	uint32_t against_id = get_against_id();
	FightAgainst *p_conf = GET_CONF(FightAgainst, against_id);
	if (NULL == p_conf)
	{
		log_error("NULL == p_against_conf[%d]", against_id);
		return  false;
	}

	if (p_conf->is_allow_reconnect() == 0)
		return true;

	return false;
}

void combat_t::add_hero_event(uint64_t hero_uid, uint32_t event_tmp, uint32_t buff_id )
{
	uint32_t event = get_real_event(event_tmp);
	if (event == 0)
		return;

	std::map<uint64_t, uint32_t> &hero_map = m_hero_event_map[event];
	hero_map[hero_uid] += 1;

   // FIGHT_LOG("hero[%lu] buff_id[%u] add_event:%u, num:%u", hero_uid, buff_id, event, hero_map[hero_uid]);
   log_error("hero[%lu] buff_id[%u] add_event:%u, num:%u", hero_uid, buff_id, event, hero_map[hero_uid]);
}


//这么处理必然保证hero_map中存在hero_uid的内存，所以不考虑 删除临时创建内存的情况 
void combat_t::del_hero_event(uint64_t hero_uid, uint32_t event_tmp, uint32_t buff_id )
{
	uint32_t event = get_real_event(event_tmp);
	if (event == 0)
		return;

	std::map<uint64_t, uint32_t> &hero_map = m_hero_event_map[event];
	if ( hero_map[hero_uid] >= 1 )
		hero_map[hero_uid] -= 1;
	
    //FIGHT_LOG("hero[%lu] buff_id[%u], del_event:%u, num:%u", hero_uid, buff_id, event, hero_map[hero_uid]);
    log_error("hero[%lu] buff_id[%u], del_event:%u, num:%u", hero_uid, buff_id, event, hero_map[hero_uid]);
	/* //保留，不删除，节省内存分配开销
	if (hero_map[hero_uid] == 0)
		hero_map.erase(hero_uid);
	*/
}

void combat_t::add_hero_event(uint64_t hero_uid, const std::set<uint32_t> *event_list, uint32_t buff_id)
{
	if (NULL == event_list)
		return;

	for (auto event_tmp : *event_list) {
		uint32_t event = get_real_event(event_tmp);
		if (event == 0)
			continue;
		std::map<uint64_t, uint32_t> &hero_map = m_hero_event_map[event];
		hero_map[hero_uid] += 1;

        FIGHT_LOG("hero[%lu] buff_id[%u] add_event:%u, num:%u", hero_uid, buff_id, event, hero_map[hero_uid]);
	}
}

void combat_t::del_hero_event(uint64_t hero_uid, const std::set<uint32_t> *event_list, uint32_t buff_id)
{
	if (NULL == event_list)
		return;

	for (auto event_tmp : *event_list) {
		uint32_t event = get_real_event(event_tmp);
		if (event == 0)
			continue;
		std::map<uint64_t, uint32_t> &hero_map = m_hero_event_map[event];
		if (hero_map[hero_uid] >= 1)
			hero_map[hero_uid] -= 1;

        FIGHT_LOG("hero[%lu] buff_id[%u], del_event:%u, num:%u", hero_uid, buff_id, event, hero_map[hero_uid]);
	}
	/* //保留，不删除，节省内存分配开销
	if (hero_map[hero_uid] == 0)
	hero_map.erase(hero_uid);
	*/
}

uint32_t combat_t::get_real_event(uint32_t event) {
	if (event >= 200 && event <= 300) {
		event -= MAGIC_TEAM_EVENT;
	}

	if (event >= 300) {
		event -= MAGIC_ENEMY_EVENT;
	}
	return event;
}


void combat_t::close_sync_user_combat_data(uint64_t uid)
{
	fight_obj_ptr obj = find_obj(uid);
	if (NULL != obj) {
		obj->set_send_combat_flag(false);
	}
}

void combat_t::open_sync_user_combat_data(uint64_t uid)
{
	fight_obj_ptr obj = find_obj(uid);
	if (NULL != obj) {
		obj->set_send_combat_flag(true);
	}
}

void combat_t::reset_max_speed() {
	//初始化所有人的初始进度条
	for (auto heropair : m_heros)
	{
		if (NULL == heropair.second)
			continue;

		if (heropair.second->is_dead())
			continue;

		heropair.second->init_att_speed();
	}

	fight_hero_ptr p_hero  = get_max_speed_attacker();
	if (NULL == p_hero) {
		log_error("round_start error m_alive_hero_list_size = 0");
		combat_end();
		return;
	}

	if (p_hero->get_attr(attrtype_enum::speed) == 0) {
		log_error("combat_t speed error hero[%lu:%d] camp[%d] ", p_hero->get_uid(), p_hero->get_tid(), p_hero->get_camp());
		return;
	}

	m_camp_data[p_hero->get_camp()].initiative = true;

	auto max_att_speed_conf = GET_CONF(Comprehensive, comprehensive_common::fight_speed_max);
	int32_t max_att_speed_rate = GET_COMPREHENSIVE_VALUE_1(max_att_speed_conf);
	m_max_att_speed = p_hero->get_attr( attrtype_enum::speed ) * max_att_speed_rate;
}


void combat_t::clear_last_combat_log() {
	if (m_hero_att_round > 0 && m_process.size() > 1) {
		m_process.erase(m_hero_att_round - 1);
	}

	log_debug("combat[%u]m_prcess size is [%lu]", get_against_id(), m_process.size());
}

void combat_t::gm_clear_all_hero_cd() {
	for (auto hero : m_heros) {
		fight_hero_ptr p_hero = hero.second;
		if (NULL == p_hero)
			continue;
		p_hero->dec_all_skill_cd(p_hero, 99);
		set_hero_skill_cd(p_hero);
	}
	return;
}

void combat_t::gm_win_fight() {
	set_win_info(proto::common::combat_camp_1);
	combat_end();
}

//获取某属性最高的武将
fight_hero_ptr combat_t::select_hero_by_attr( uint32_t camp, uint64_t  except, uint32_t attr_type, fight_hero_ptr attaker, SkillEffectTable *skill_conf, bool is_max ){
	std::vector<fight_hero_ptr> hero_list;
	get_camp_alive_heros(hero_list, camp);

	int32_t attr_value = 0;
	fight_hero_ptr find = NULL;
	for (uint32_t i = 0; i < hero_list.size(); ++i) {
		fight_hero_ptr hero = hero_list[i];
		if (NULL == hero)
			continue;

		if (!check_skill_target_type(skill_conf, attaker, hero))
			continue;

		if (attr_value == 0) {
			attr_value = hero->get_attr( attr_type );
			find = hero;
			continue;
		}

		if (is_max) {
			if (hero->get_attr( attr_type ) > attr_value ) {
				attr_value = hero->get_attr( attr_type );
				find = hero;
			}
		}
		else {
			if (hero->get_attr( attr_type ) < attr_value ) {
				attr_value = hero->get_attr( attr_type );
				find = hero;
			}
		}
	}
	return find;
}


void combat_t::set_hero_skill_cd(fight_hero_ptr p_hero) {
	if (NULL == p_hero) {
		log_error("p_hero is NULL");
		return;
	}
	m_hero_skill_cd_list[p_hero->get_uid()] = p_hero;
}

uint8_t combat_t::get_real_camp_type(uint32_t type, uint32_t user_camp) {
	if (type == 0) {	//全体
		return camp_all;
	}

	switch (user_camp) {
	case proto::common::combat_camp_1: {	//玩家阵营_1 的自己是1 敌方是2
		return type;
	}
	break;
	case proto::common::combat_camp_2: {	//玩家阵营_2 的自己是2 地方是1
		if (type == 1) {
			return camp_2;
		}
		else {
			return camp_1;
		}
	}
	break;
	}
	return camp_all;
}


void combat_t::add_public_buff(uint64_t uid, buff_ptr  p_buff, uint32_t type, uint32_t user_camp) {
	if (NULL == p_buff) {
		log_error("p_buff is NULL");
		return;
	}

	//这里有一层转化关系， 0 全体， 1自己 2敌方
	//到光环这里是   0全体 1:camp_1 2:camp_2
	uint32_t camp_type = get_real_camp_type(type, user_camp);
	
	p_buff->is_public_use = true;
	p_buff->public_use_type = camp_type;

//添加到玩家容器
	buff_map_t &hero_map = hero_buff_map[uid];
	hero_map[p_buff->get_tid()] = p_buff;

//添加到等级容器
	buff_multimap_t  &level_map = level_buff_map[p_buff->get_tid()];
	level_map.insert( std::make_pair(p_buff->get_skill_level(), p_buff) );

//添加事件
	add_public_event(p_buff->get_tid(), p_buff->get_triger_type_list() );
	
//置换到生效容器
	add_camp_buff(p_buff);
}

void combat_t::add_camp_buff(buff_ptr p_buff) {
	if (NULL == p_buff) {
		return;
	}

	buff_multimap_t &level_map = level_buff_map[p_buff->get_tid()];

	//置换到生效容器
	buff_multimap_t::reverse_iterator iter_b = level_map.rbegin();
	if (iter_b == level_map.rend()) {
		return;
	}
	switch (p_buff->public_use_type) {
	case camp_all: {//全体光环
		all_buff[p_buff->get_tid()] = iter_b->second;
		log_error("user[%lu] add_all buff[%u]", p_buff->get_attacker(), p_buff->get_tid());
	}
	break;
	case camp_1: {	//camp_1的光环
		camp1_buff[p_buff->get_tid()] = iter_b->second;
		log_error("user[%lu] add_cmap1 buff[%u]", p_buff->get_attacker(), p_buff->get_tid());
	}
	break;
	case camp_2: {	//camp_2的光环
		camp2_buff[p_buff->get_tid()] = iter_b->second;
		log_error("user[%lu] add_cmap2 buff[%u]", p_buff->get_attacker(), p_buff->get_tid());
	}
	break;
	}
	
}

void combat_t::reset_use_public_buff() {
	camp1_buff.clear();
	camp2_buff.clear();
	all_buff.clear();

	for (auto iter : level_buff_map) {
		buff_multimap_t &buff_map = iter.second;
		for (auto iter_find : buff_map ) {
			buff_ptr p_buff = iter_find.second;
			if ( NULL == p_buff || p_buff->is_public_use == false ) {
				continue;
			}
			add_camp_buff(p_buff);
			break;
		}
	}
	//重置后重新释放技能
	distribute_special_buff();
}

void combat_t::set_public_buff_off(uint64_t uid) {
	auto iter_f = hero_buff_map.find(uid);
	if (iter_f == hero_buff_map.end()) {
		return;
	}

	log_error("set_user [%lu]'s buff off", uid);
	for (auto &iter : iter_f->second ) {
		buff_ptr p_buff = iter.second;
		if (NULL == p_buff) {
			continue;
		}
		//先stop所有旧的
		p_buff->is_public_use = false;
		run_public_buff(p_buff->public_use_type, 0, NULL, NULL, p_buff, buff_step_unload);
	}
	reset_use_public_buff();
}

void combat_t::set_public_buff_on(uint64_t uid) {
	auto iter_f = hero_buff_map.find(uid);
	if (iter_f == hero_buff_map.end()) {
		return;
	}

	log_error("set_user [%lu]'s buff on", uid);
	for (auto &iter : iter_f->second) {
		buff_ptr p_buff = iter.second;
		if (p_buff) {
			p_buff->is_public_use = true;
		}
	}
	reset_use_public_buff();
}

void combat_t::add_public_event(uint32_t buff_tid, const std::set<uint32_t> *event_list) {
	if (NULL == event_list)
		return;

	for (auto event: *event_list) {
		if (event == 0)
			continue;
		
		std::set<uint32_t> &event_set = public_event_list[event];
		event_set.insert(buff_tid);
		log_error("add_public[%u] buff_event[%u]", buff_tid, event);
	}
}

void combat_t::do_public_buff(uint32_t event, fight_hero_ptr p_trigger, fight_hero_ptr p_event_owner, uint32_t camp) {
	std::set<uint32_t> &event_set = public_event_list[event];
	for (auto buff_tid : event_set) {
		//处理公共buff
		auto p_buff_all = all_buff.find(buff_tid);
		if (p_buff_all != all_buff.end()) {
			run_public_buff(camp_all, event, p_trigger, p_event_owner, p_buff_all->second , buff_step_trigger);
		}

		if (camp == proto::common::combat_camp_1) {
			auto p_buff_camp_1 = camp1_buff.find(buff_tid);
			if(p_buff_camp_1 != camp1_buff.end() ){
				run_public_buff(camp_1, event, p_trigger, p_event_owner, p_buff_camp_1->second, buff_step_trigger);
			}
		}

		if (camp == proto::common::combat_camp_2) {
			auto p_buff_camp_2 = camp2_buff.find(buff_tid);
			if (p_buff_camp_2 != camp2_buff.end()) {
				run_public_buff(camp_2, event, p_trigger, p_event_owner, p_buff_camp_2->second, buff_step_trigger);
			}
		}
	}
}

void combat_t::run_public_buff(uint32_t type, uint32_t event, fight_hero_ptr p_trigger, fight_hero_ptr p_event_owner, buff_ptr p_buff, uint32_t trigger) {
	if (NULL == p_buff) {
		return;
	}

	fight_hero_map &hero_map = get_all_heros_ptr();
	for (auto hero_pair : hero_map) {
		fight_hero_ptr p_hero = hero_pair.second;
		if (NULL == p_hero) {
			continue;
		}

		if (type == 1 && p_hero->get_camp() != proto::common::combat_camp_1) {
			continue;
		}
		
		if (type == 2 && p_hero->get_camp() != proto::common::combat_camp_2) {
			continue;
		}

		if (p_trigger != NULL && p_trigger->get_uid() != p_hero->get_uid()) {
			continue;
		}

		if (p_hero->is_immuno_buff(p_buff->get_small_type())) {
			log_error("hero[%lu] immuno buff[%u]", p_hero->get_uid(), p_buff->get_small_type());
			continue;
		}

		p_buff->set_step(trigger);
		p_hero->m_buff_mgr.run_public_buff(p_buff, p_event_owner, event);	
	}
}

//释放所有光环效果(初始化，切波次等时机释放,hero身上的只做添加)
void combat_t::distribute_special_buff() {
	for (auto iter : all_buff) {
		run_public_buff(camp_all, event_on_none, NULL, NULL, iter.second , buff_step_load);	//此处为了代码简洁（支持多次重复run load，在97元素中进行重复处理逻辑）
	}

	for (auto iter : camp1_buff) {
		run_public_buff(camp_1, event_on_none, NULL, NULL, iter.second , buff_step_load);
	}

	for (auto iter : camp2_buff) {
		run_public_buff(camp_2, event_on_none, NULL, NULL, iter.second , buff_step_load);
	}
}

void combat_t::clear_phase_run_info( uint64_t uid ) {
	auto iter_f = hero_buff_map.find(uid);
	if (iter_f == hero_buff_map.end()) {
		return;
	}

	for (auto &iter : iter_f->second) {
		buff_ptr p_buff = iter.second;
		if (p_buff) {
			p_buff->clear_phase_run();
		}
	}
}

fight_hero_ptr combat_t::get_max_speed_attacker() {
	std::vector<fight_hero_ptr> res;
	for (auto heropair : m_heros)
	{
		if (NULL == heropair.second)
		{
			continue;
		}

		if (heropair.second->is_dead())
		{
			continue;
		}

		res.push_back(heropair.second);
	}
	common::Sort::Qsort(res, 0, res.size() - 1, fight_hero_t::hero_max_speed_sort);

	if (res.empty()) {
		return NULL;
	}
	
	return res[0];
}

void combat_t::peek_fight_obj(proto::common::fight_type type, proto::common::video_obj_base* obj1, proto::common::video_obj_base* obj2)
{
	if (!obj1) {
		log_error("obj1 null error");
		return;
	}

	if (!obj2) {
		log_error("obj2 null error");
		return;
	}

	// printf("peek_fight_obj obj1 \n");
	const proto::common::fight_data& fd = get_fight_data();
	for (const proto::common::obj_single& obj : fd.self().objs().obj())
	{
		obj1->set_uid(obj.object_uid());
		obj1->set_object_type(proto::common::SCENEOBJECT_USER);
		obj1->set_name(obj.name());
		obj1->set_head(obj.head_id());
		obj1->set_fighting(obj.fighting());
		obj1->set_level(obj.level());
		// printf("obj1[%s] type[%u] name[%s] head[%u]\n", obj1->uid().c_str(), obj1->object_type(), obj1->name().c_str(), obj1->head());
		switch (type) {
			case proto::common::fight_type_cross_arena_rank: {
				proto::common::video_arena_ex* ex = obj1->mutable_cross_arena();
				const proto::common::cross_arena_param& param = fd.common().param().cross_arena();
				if (ex) {
					ex->set_rank_lv(param.obj1().rank_lv());
					ex->set_score(param.obj1().score());
				}
				break;
			}
			default: {
				log_error("peek_fight_obj type[%u] not define", type);
				break;
			}
		}

		std::vector<fight_hero_ptr> hero_list;
		get_camp_heros(hero_list, proto::common::combat_camp_1, 0);
		for (const fight_hero_ptr& p_hero : hero_list) {
			if (p_hero) {
				const proto::common::hero_single& base_single = p_hero->get_hero_base();
				proto::common::video_hero_base* p_hero_base = obj1->add_heros();
				if (p_hero_base) {
					p_hero_base->set_tid(base_single.base_attr().id());
					p_hero_base->set_resource_id(base_single.base_attr().resource_id());
					p_hero_base->set_lv(base_single.base_attr().level());
					p_hero_base->set_start(base_single.base_attr().star());
					p_hero_base->set_grade(base_single.base_attr().grade());
					p_hero_base->set_quality(base_single.base_attr().quality());
					p_hero_base->set_terry(base_single.base_attr().terry());
					// printf("	tid[%u] resource_id[%u] lv[%u] start[%u] grade[%u] quality[%u] terry[%u]\n", p_hero_base->tid(), p_hero_base->resource_id(), p_hero_base->lv(), p_hero_base->start(), p_hero_base->grade(), p_hero_base->quality(), p_hero_base->terry());
				}
			}
		}
	}

	// printf("peek_fight_obj obj2 \n");
	for (const proto::common::obj_single& obj : fd.enemy().objs().obj())
	{
		obj2->set_uid(obj.object_uid());
		obj2->set_object_type(proto::common::SCENEOBJECT_USER);
		obj2->set_name(obj.name());
		obj2->set_head(obj.head_id());
		obj2->set_fighting(obj.fighting());
		obj2->set_level(obj.level());

		switch (type) {
			case proto::common::fight_type_cross_arena_rank: {
				if (get_fight_data_type() == fight_data_type::fight_data_conf) {
					obj2->set_uid(common::string_util_t::uint32_to_string(fd.common().param().cross_arena().monster_id()));
					obj2->set_object_type(proto::common::SCENEOBJECT_MONSTER);
				}

				proto::common::video_arena_ex* ex = obj2->mutable_cross_arena();
				const proto::common::cross_arena_param& param = fd.common().param().cross_arena();
				if (ex) {
					ex->set_rank_lv(param.obj2().rank_lv());
					ex->set_score(param.obj2().score());
				}
				break;
			}
			default: {
				log_error("peek_fight_obj type[%u] not define", type);
				break;
			}
		}

		// printf("obj2[%s] type[%u] name[%s] head[%u]\n", obj2->uid().c_str(), obj2->object_type(), obj2->name().c_str(), obj2->head());
		std::vector<fight_hero_ptr> hero_list;
		get_camp_heros(hero_list, proto::common::combat_camp_2, 0);
		for (const fight_hero_ptr& p_hero : hero_list) {
			if (p_hero) {
				const proto::common::hero_single& base_single = p_hero->get_hero_base();
				proto::common::video_hero_base* p_hero_base = obj2->add_heros();
				if (p_hero_base) {
					p_hero_base->set_tid(base_single.base_attr().id());
					p_hero_base->set_resource_id(base_single.base_attr().resource_id());
					p_hero_base->set_lv(base_single.base_attr().level());
					p_hero_base->set_start(base_single.base_attr().star());
					p_hero_base->set_grade(base_single.base_attr().grade());
					p_hero_base->set_quality(base_single.base_attr().quality());
					p_hero_base->set_terry(base_single.base_attr().terry());
					// printf("	tid[%u] resource_id[%u] lv[%u] start[%u] grade[%u] quality[%u] terry[%u]\n", p_hero_base->tid(), p_hero_base->resource_id(), p_hero_base->lv(), p_hero_base->start(), p_hero_base->grade(), p_hero_base->quality(), p_hero_base->terry());
				}
			}
		}
	}
}

void combat_t::peek_combat_log(proto::common::fight_video_data& video_data)
{
	if (m_round <= 1) {
		return;
	}

	proto::common::fight_data* fight_data = video_data.mutable_data();
	if (fight_data) {
		fight_data->CopyFrom(m_fight_data);
	}

	fill_count_data(video_data.mutable_result());
	
	process_map::const_iterator iter = m_process.begin();
	process_map::const_iterator iter_end = m_process.end();
	for (; iter != iter_end; ++iter) {
		proto::common::combat_action_data *p_action_data = video_data.mutable_action_data()->add_action_data();
		if (p_action_data) {
			p_action_data->CopyFrom(iter->second);
		}
	}
}

void combat_t::cancel_obj_auto_fight(uint64_t uid) {
	fight_obj_ptr p_obj = find_obj(uid);
	if (NULL == p_obj) {
		log_error("disposition can not find obj %lu", uid);
		return ;
	}
	
	if (m_step != proto::common::combat_step_round) {
		return ;
	}

	if (p_obj->get_state() != proto::common::combat_obj_state_prepare_end) {
		return ;
	}

	p_obj->set_state(proto::common::combat_obj_state_prepare);
}


