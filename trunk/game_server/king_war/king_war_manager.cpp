#include "king_war_manager.hpp"
#include "log.hpp"
#include "tblh/errcode_enum.hpp"
#include "monster/monster_manager.hpp"
#include "monster/monster.hpp"
#include "long_pulse.hpp"
#include "config_mgr.h"
#include "tblh/Comprehensive.tbls.h"
#include <vector>
#include "king_war/king_war_scene.hpp"
#include "achieve/achieve_common.hpp"
#include "tblh/Country.tbls.h"
#include "tblh/family_enum.hpp"
#include "tblh/KingWarPersonal.tbls.h"
#include "tblh/KingWarRank.tbls.h"
#include "tblh/sys_notice_enum.hpp"
#include "sys_notice/sys_notice_manager.hpp"

country_scene_map king_war_manager_t::m_country_scene_map;

bool king_war_manager_t::close_clear()
{
	m_country_scene_map.clear();

	return true;
}

bool king_war_manager_t::add_country_scene(uint32_t country_id, uint64_t scene_id)
{
	country_scene_info_ptr p_info = get_country_info_by_id(country_id);
	if (p_info == NULL)
	{
		p_info = country_scene_info_ptr(new country_scene_info_t());

		if (p_info == NULL)
		{
			log_error("p_info null error");
			return false;
		}

		p_info->m_country_id = country_id;

		m_country_scene_map.insert(std::make_pair(country_id, p_info));
	}

	p_info->m_scene_id = scene_id;

	return true;
}

void king_war_manager_t::update_king_war_state(uint32_t country_id, const proto::server::eg_king_war_state_notify& notify)
{
	country_scene_info_ptr p_info = get_country_info_by_id(country_id);
	if (p_info == NULL)
	{
		log_error("country[%u] not find info", country_id);
		return;
	}

	// 清理上次的数据
	p_info->reset_all();

	p_info->m_king_war_info.m_fight_family[king_war_side_attack].m_family_id = notify.att_family_id();
	p_info->m_king_war_info.m_fight_family[king_war_side_attack].m_family_name = notify.att_family_name();
	if (notify.att_help_family() != 0) {
		p_info->m_king_war_info.m_fight_family[king_war_side_attack].m_help_family = notify.att_help_family();
		p_info->m_king_war_info.m_fight_family[king_war_side_attack].m_helper_name = notify.att_help_family_name();
	}

	p_info->m_king_war_info.m_fight_family[king_war_side_defence].m_family_id = notify.def_family_id();
	p_info->m_king_war_info.m_fight_family[king_war_side_defence].m_family_name = notify.def_family_name();
	if (notify.def_help_family() != 0) {
		p_info->m_king_war_info.m_fight_family[king_war_side_defence].m_help_family = notify.def_help_family();
		p_info->m_king_war_info.m_fight_family[king_war_side_defence].m_helper_name = notify.def_help_family_name();
	}

	p_info->m_king_war_state = notify.state();

	const scene_ptr& p_country_scene = scene_manager_t::find_scene(p_info->m_scene_id);
	if (NULL == p_country_scene)
	{
		log_error("country[%u] scene[%lu] can not find", country_id, p_info->m_scene_id);
		return;
	}

	if (!p_country_scene->is_king_war_city())
	{
		log_error("scene[%lu] is not king country", p_info->m_scene_id);
		return;
	}

	king_war_scene_ptr p_king_scene = p_country_scene->get_king_war_scene_ptr();
	if (p_king_scene == NULL)
	{
		log_error("scene[%lu]is not king country", p_info->m_scene_id);
		return;
	}

	p_king_scene->set_king_war_family(king_war_side_defence, p_info->m_king_war_info.m_fight_family[king_war_side_defence]);
	p_king_scene->set_king_war_family(king_war_side_attack, p_info->m_king_war_info.m_fight_family[king_war_side_attack]);

	switch (p_info->m_king_war_state)
	{
		case proto::common::king_war_state_prepare:	// 进入准备期
		{
			p_king_scene->ready_king_war();
		}
		break;
		case proto::common::king_war_state_start:	// 进入开战期
		{
			p_king_scene->start_king_war();
		}
		break;
		default:
		{
			log_error("country[%u] center notify state[%u] error", country_id, notify.state());
		}
		break;
	}

	KINGWAR_LOG("country[%u] m_king_war_state[%u]", country_id, notify.state());
}

void king_war_manager_t::king_war_ending(uint32_t country_id)
{
	// 结算
	king_war_settlement(country_id);

	KINGWAR_LOG("center notify country[%u] king war end", country_id);
}

void king_war_manager_t::king_war_settlement(uint32_t country_id)
{
	country_scene_info_ptr p_info = get_country_info_by_id(country_id);
	if (p_info == NULL)
	{
		log_error("country[%u] not find info", country_id);
		return;
	}

	if (p_info->m_king_war_state != proto::common::king_war_state_start)
	{
		log_error("country[%u] country_scene_info_t::m_king_war_state != proto::common::king_war_state_start", country_id);
		return;
	}

	const scene_ptr& p_country_scene = scene_manager_t::find_scene(p_info->m_scene_id);
	if (NULL == p_country_scene)
	{
		log_error("country[%u] scene[%lu] can not find", country_id, p_info->m_scene_id);
		return;
	}

	if (!p_country_scene->is_king_war_city())
	{
		log_error("scene[%lu] is not king country", p_info->m_scene_id);
		return;
	}

	king_war_scene_ptr p_king_scene = p_country_scene->get_king_war_scene_ptr();
	if (p_king_scene == NULL)
	{
		log_error("scene[%lu] is not king country", p_info->m_scene_id);
		return;
	}

	p_king_scene->end_king_war();

	// 最后排一次名
	p_info->sort_rank_list();
	p_info->update_rank_list();

	proto::server::ge_king_war_result_notify notify_center;
	notify_center.set_country_id(country_id);

	proto::client::gc_king_war_reward_notify notify_role;
	notify_role.set_def_name(p_info->m_king_war_info.m_fight_family[king_war_side_defence].m_family_name);
	notify_role.set_def_help(p_info->m_king_war_info.m_fight_family[king_war_side_defence].m_helper_name);
	notify_role.set_def_score(p_info->m_king_war_info.m_family_long_gas[king_war_side_defence]);
	notify_role.set_att_name(p_info->m_king_war_info.m_fight_family[king_war_side_attack].m_family_name);
	notify_role.set_att_help(p_info->m_king_war_info.m_fight_family[king_war_side_attack].m_helper_name);
	notify_role.set_att_score(p_info->m_king_war_info.m_family_long_gas[king_war_side_attack]);

	if (p_info->m_king_war_info.m_family_long_gas[king_war_side_attack] > p_info->m_king_war_info.m_family_long_gas[king_war_side_defence]) {
		notify_center.set_win_family_id(p_info->m_king_war_info.m_fight_family[king_war_side_attack].m_family_id);
		notify_role.set_win_family(p_info->m_king_war_info.m_fight_family[king_war_side_attack].m_family_name);
		notify_role.set_win_help_family(p_info->m_king_war_info.m_fight_family[king_war_side_attack].m_helper_name);
	} else {
		notify_center.set_win_family_id(p_info->m_king_war_info.m_fight_family[king_war_side_defence].m_family_id);
		notify_role.set_win_family(p_info->m_king_war_info.m_fight_family[king_war_side_defence].m_family_name);
		notify_role.set_win_help_family(p_info->m_king_war_info.m_fight_family[king_war_side_defence].m_helper_name);
	}

	king_war_rank_list::const_iterator rank_list_citr = p_info->m_king_war_rank_list.begin();

	// 个人奖励表
	std::map<uint32_t, KingWarPersonal*> personal_confs;
	GET_ALL_CONF(KingWarPersonal, personal_confs);
	std::map<uint32_t, KingWarPersonal*>::iterator personal_conf_itr = personal_confs.begin();

	uint32_t index = 0;
	std::map<uint32_t, KingWarRank*> rank_confs;
	GET_ALL_CONF(KingWarRank, rank_confs);
	for (std::map<uint32_t, KingWarRank*>::iterator itr = rank_confs.begin();
		itr != rank_confs.end(); ++itr)
	{
		if (!itr->second) continue;

		for (; index < itr->second->rank(); ++index) {
			if (rank_list_citr != p_info->m_king_war_rank_list.end()) {
				const king_war_role_ptr& p_king_war_role = *rank_list_citr;
				if (!p_king_war_role) {
					log_error("p_king_war_role null error");
					++rank_list_citr;
					continue;
				}

				bool is_notify_role = false;
				role_ptr p_role = role_manager_t::find_role(p_king_war_role->m_uid);
				if (p_role && p_role->get_scene_id() == p_king_scene->get_scene_id()) {
					is_notify_role = true;
				}

				proto::server::king_war_personal* p_personal = notify_center.add_all_roles();
				if (p_personal) {
					p_personal->set_uid(p_king_war_role->m_uid);
					p_personal->set_exploit(p_king_war_role->m_exploit);
					p_personal->set_rank(index + 1);
					p_personal->set_rank_level(itr->first);
					p_personal->set_exploit_level(0);
				}

				if (is_notify_role) {
					proto::common::king_war_rank_single* p_king_war_rank_single = notify_role.mutable_rank_info();
					if (p_king_war_rank_single) {
						p_king_war_rank_single->CopyFrom(p_king_war_role->m_rank_single);
					}
					notify_role.set_rank(index + 1);
					notify_role.set_rank_level(itr->first);
					notify_role.set_exploit_level(0);
				}

				if (personal_conf_itr != personal_confs.end()) {
					if (!personal_conf_itr->second || p_king_war_role->m_exploit < personal_conf_itr->second->exploit()) {
						for (++personal_conf_itr; personal_conf_itr != personal_confs.end(); ++personal_conf_itr) {
							if (personal_conf_itr->second && p_king_war_role->m_exploit >= personal_conf_itr->second->exploit() ) {
								break;
							}
						}
					}

					if (personal_conf_itr != personal_confs.end()) {
						if (p_personal) {
							p_personal->set_exploit_level(personal_conf_itr->first);
						}
							
						if (is_notify_role) {
							notify_role.set_exploit_level(personal_conf_itr->first);
						}
					}
				}

				if (is_notify_role && p_role) {
					p_role->send_msg_to_client(op_cmd::gc_king_war_reward_notify, notify_role);
				}

				///成就项统计
				achieve_common_t::notify_progress_state(p_king_war_role->m_uid, proto::common::ACHIEVE_KING_WAR_NUMBER);
				///achieve_common_t::notify_progress_state(p_king_war_role->m_uid, proto::common::ACHIEVE_KING_WAR_QUICK_REVIVE,1,p_king_war_role->m_free_revive_count);
				///achieve_common_t::notify_progress_state(p_king_war_role->m_uid, proto::common::ACHIEVE_KING_WAR_KILL_PLAYER_NUMBER,1,p_king_war_role->m_all_kills);
				
				++rank_list_citr;
			}

			if (rank_list_citr == p_info->m_king_war_rank_list.end()) {
				break;
			}
		}

		if (rank_list_citr == p_info->m_king_war_rank_list.end()) {
			break;
		}
	}
 
 	env::server->send_msg_to_center(op_cmd::ge_king_war_result_notify, 0, notify_center);

	for (; rank_list_citr != p_info->m_king_war_rank_list.end(); ++rank_list_citr, ++index) {
		const king_war_role_ptr& p_king_war_role = *rank_list_citr;
		if (p_king_war_role) {
			role_ptr p_role = role_manager_t::find_role(p_king_war_role->m_uid);
			if (p_role && p_role->get_scene_id() == p_king_scene->get_scene_id()) {
				notify_role.set_rank(index + 1);
				notify_role.set_rank_level(0);
				notify_role.set_exploit_level(0);
				p_role->send_msg_to_client(op_cmd::gc_king_war_reward_notify, notify_role);
			}
		}
	}

	// 清理王城战信息
	p_info->king_war_end();
}

country_scene_info_ptr king_war_manager_t::get_country_info_by_id(uint32_t country_id)
{
	country_scene_map::iterator itr = m_country_scene_map.find(country_id);
	if (itr != m_country_scene_map.end())
	{
		return itr->second;
	}

	return NULL;
}

king_war_role_ptr king_war_manager_t::get_king_war_role_ex(uint32_t country_id, const role_ptr& p_role)
{
	if (p_role == NULL) {
		log_error("role null error");
		return NULL;
	}

	country_scene_info_ptr p_info = get_country_info_by_id(country_id);
	if (p_info == NULL)
	{
		log_error("role[%lu] country[%u] not find info", p_role->get_uid(), country_id);
		return NULL;
	}

	king_war_role_map::iterator itr = p_info->m_king_war_role_map.find(p_role->get_uid());
	if (itr != p_info->m_king_war_role_map.end())
	{
		return itr->second;
	}

	uint32_t king_war_side = p_info->m_king_war_info.get_king_war_side_type(p_role->get_family_id());
	if (king_war_side == king_war_side_none) {
		log_error("role[%lu] not country[%u] king war family", p_role->get_uid(), country_id);
		return NULL;
	}

	king_war_role_ptr p_king_war_role = king_war_role_ptr(new king_war_role_t(p_role->get_uid()));
	if (p_king_war_role == NULL)
	{
		log_error("p_king_war_role null error");
		return NULL;
	}

	auto p_king_fight = GET_CONF(Comprehensive, comprehensive_common::king_fight_number);
	uint32_t king_fight_number = GET_COMPREHENSIVE_VALUE_1(p_king_fight);

	// p_king_war_role->m_uid = uid;
	p_king_war_role->m_rank_single.set_role_name(p_role->get_name());
	p_king_war_role->m_rank_single.set_family_name(p_role->get_family_name());
	p_king_war_role->m_rank_single.set_kill_count(0);
	p_king_war_role->m_rank_single.set_death_count(0);
	p_king_war_role->m_rank_single.set_continued_kill(0);
	p_king_war_role->m_rank_single.set_exploits(0);
	p_king_war_role->m_warfare = king_fight_number;
	auto p_conf = GET_CONF(Comprehensive, comprehensive_common::king_revive_free_cnt);
	p_king_war_role->m_free_revive_count = GET_COMPREHENSIVE_VALUE_1(p_conf);

	// 加入两个列表
	p_info->m_king_war_role_map.insert(std::make_pair(p_role->get_uid(), p_king_war_role));
	p_info->m_king_war_rank_list.push_back(p_king_war_role);

	return p_king_war_role;
}

king_war_role_ptr king_war_manager_t::get_king_war_role(uint32_t country_id, uint64_t uid)
{
	country_scene_info_ptr p_info = get_country_info_by_id(country_id);
	if (p_info == NULL)
	{
		log_error("role[%lu] country[%u] not find info", uid, country_id);
		return NULL;
	}

	king_war_role_map::iterator itr = p_info->m_king_war_role_map.find(uid);
	if (itr != p_info->m_king_war_role_map.end())
	{
		return itr->second;
	}

	return NULL;
}

std::pair<uint32_t, uint32_t> king_war_manager_t::get_reward_by_self_title(em_kill_title title)
{
	uint32_t add_gas = 0;
	uint32_t add_exploit = 0;
	switch (title)
	{
	case em_kill_title_0:
		break;
	case em_kill_title_1:
		{
			auto p_conf = GET_CONF(Comprehensive, comprehensive_common::king_fight_kill_score1);
			add_gas = GET_COMPREHENSIVE_VALUE_1(p_conf);
			p_conf = GET_CONF(Comprehensive, comprehensive_common::king_fight_kill_merit1);
			add_exploit = GET_COMPREHENSIVE_VALUE_1(p_conf);
		}
		break;
	case em_kill_title_2:
		{
			auto p_conf = GET_CONF(Comprehensive, comprehensive_common::king_fight_kill_score2);
			add_gas = GET_COMPREHENSIVE_VALUE_1(p_conf);
			p_conf = GET_CONF(Comprehensive, comprehensive_common::king_fight_kill_merit2);
			add_exploit = GET_COMPREHENSIVE_VALUE_1(p_conf);
		}
		break;
	case em_kill_title_3:
		{
			auto p_conf = GET_CONF(Comprehensive, comprehensive_common::king_fight_kill_score3);
			add_gas = GET_COMPREHENSIVE_VALUE_1(p_conf);
			p_conf = GET_CONF(Comprehensive, comprehensive_common::king_fight_kill_merit3);
			add_exploit = GET_COMPREHENSIVE_VALUE_1(p_conf);
		}
		break;
	case em_kill_title_4:
		{
			auto p_conf = GET_CONF(Comprehensive, comprehensive_common::king_fight_kill_score4);
			add_gas = GET_COMPREHENSIVE_VALUE_1(p_conf);
			p_conf = GET_CONF(Comprehensive, comprehensive_common::king_fight_kill_merit4);
			add_exploit = GET_COMPREHENSIVE_VALUE_1(p_conf);
		}
		break;
	default:
		{
			log_error("title[%u] not find", title);
		}
		break;
	}

	return std::pair<uint32_t, uint32_t>(add_gas, add_exploit);
}

std::pair<uint32_t, uint32_t> king_war_manager_t::get_reward_by_other_title(em_kill_title title)
{
	uint32_t add_gas = 0;
	uint32_t add_exploit = 0;
	switch (title)
	{
	case em_kill_title_0:
		break;
	case em_kill_title_1:
	{
		auto p_conf = GET_CONF(Comprehensive, comprehensive_common::king_fight_end_kill_score1);
		add_gas = GET_COMPREHENSIVE_VALUE_1(p_conf);
		p_conf = GET_CONF(Comprehensive, comprehensive_common::king_fight_end_kill_merit1);
		add_exploit = GET_COMPREHENSIVE_VALUE_1(p_conf);
	}
	break;
	case em_kill_title_2:
	{
		auto p_conf = GET_CONF(Comprehensive, comprehensive_common::king_fight_end_kill_score2);
		add_gas = GET_COMPREHENSIVE_VALUE_1(p_conf);
		p_conf = GET_CONF(Comprehensive, comprehensive_common::king_fight_end_kill_merit2);
		add_exploit = GET_COMPREHENSIVE_VALUE_1(p_conf);
	}
	break;
	case em_kill_title_3:
	{
		auto p_conf = GET_CONF(Comprehensive, comprehensive_common::king_fight_end_kill_score3);
		add_gas = GET_COMPREHENSIVE_VALUE_1(p_conf);
		p_conf = GET_CONF(Comprehensive, comprehensive_common::king_fight_end_kill_merit3);
		add_exploit = GET_COMPREHENSIVE_VALUE_1(p_conf);
	}
	break;
	case em_kill_title_4:
	{
		auto p_conf = GET_CONF(Comprehensive, comprehensive_common::king_fight_end_kill_score4);
		add_gas = GET_COMPREHENSIVE_VALUE_1(p_conf);
		p_conf = GET_CONF(Comprehensive, comprehensive_common::king_fight_end_kill_merit4);
		add_exploit = GET_COMPREHENSIVE_VALUE_1(p_conf);
	}
	break;
	default:
	{
		log_error("title[%u] not find", title);
	}
	break;
	}

	return std::pair<uint32_t, uint32_t>(add_gas, add_exploit);
}

std::pair<uint32_t, uint32_t> king_war_manager_t::get_reward_by_other_offer(uint32_t offer)
{
	uint32_t add_gas = 0;
	uint32_t add_exploit = 0;

	switch (offer) {
		case family_officer_type_general: {
			auto p_conf = GET_CONF(Comprehensive, comprehensive_common::king_fight_kill_official_score1);
			add_gas = GET_COMPREHENSIVE_VALUE_1(p_conf);
			p_conf = GET_CONF(Comprehensive, comprehensive_common::king_fight_kill_official_merit1);
			add_exploit = GET_COMPREHENSIVE_VALUE_1(p_conf);
			break;
		}
		case family_officer_type_vice_general: {
			auto p_conf = GET_CONF(Comprehensive, comprehensive_common::king_fight_kill_official_score2);
			add_gas = GET_COMPREHENSIVE_VALUE_1(p_conf);
			p_conf = GET_CONF(Comprehensive, comprehensive_common::king_fight_kill_official_merit2);
			add_exploit = GET_COMPREHENSIVE_VALUE_1(p_conf);
			break;
		}
		case family_officer_type_elit: {
			auto p_conf = GET_CONF(Comprehensive, comprehensive_common::king_fight_kill_official_score3);
			add_gas = GET_COMPREHENSIVE_VALUE_1(p_conf);
			p_conf = GET_CONF(Comprehensive, comprehensive_common::king_fight_kill_official_merit3);
			add_exploit = GET_COMPREHENSIVE_VALUE_1(p_conf);
			break;
		}
		default:
			break;
	}

	return std::pair<uint32_t, uint32_t>(add_gas, add_exploit);
}

em_kill_title king_war_manager_t::get_kill_title(uint32_t kill_num)
{
	auto p_conf = GET_CONF(Comprehensive, comprehensive_common::king_fight_kill_title4);
	uint32_t need_num = GET_COMPREHENSIVE_VALUE_1(p_conf);
	if (kill_num >= need_num)
	{
		return em_kill_title_4;
	}

	p_conf = GET_CONF(Comprehensive, comprehensive_common::king_fight_kill_title3);
	need_num = GET_COMPREHENSIVE_VALUE_1(p_conf);
	if (kill_num >= need_num)
	{
		return em_kill_title_3;
	}

	p_conf = GET_CONF(Comprehensive, comprehensive_common::king_fight_kill_title2);
	need_num = GET_COMPREHENSIVE_VALUE_1(p_conf);
	if (kill_num >= need_num)
	{
		return em_kill_title_2;
	}

	p_conf = GET_CONF(Comprehensive, comprehensive_common::king_fight_kill_title1);
	need_num = GET_COMPREHENSIVE_VALUE_1(p_conf);
	if (kill_num >= need_num)
	{
		return em_kill_title_1;
	}

	return em_kill_title_0;
}

uint32_t king_war_manager_t::ask_enter_king_war_scene(const role_ptr& p_role, uint32_t country_id)
{
	if (NULL == p_role)	{
		log_error("p_role null error");
		return errcode_enum::notice_unknown;
	}

	Country* p_conf = GET_CONF(Country, country_id);
	if (!p_conf) {
		log_error("role[%lu] country[%u] not define", p_role->get_uid(), country_id);
		return errcode_enum::notice_unknown;
	}

	return scene_manager_t::role_jump_scene(p_role, proto::server::user_change_scene, p_conf->capital());
}

uint32_t king_war_manager_t::ask_defence_long(role_ptr p_role, uint64_t object_id)
{
	if (NULL == p_role)
	{
		log_error("p_role null error");
		return errcode_enum::notice_unknown;
	}

	// 死亡提醒
	if (p_role->is_die())
	{
		log_error("p_role[%lu] obj_state[%u] error", p_role->get_object_id(), p_role->get_object_state());
		return errcode_enum::sys_notice_you_dead;
	}

	// 战意值为0
	if (p_role->get_king_war_warfare() == 0)
	{
		log_error("p_role[%lu] king_war_warfare[%u] == 0", p_role->get_object_id(), p_role->get_king_war_warfare());
		return errcode_enum::notice_king_war_error14;
	}

	// 自己状态
	if (p_role->get_object_state() != proto::common::object_state_normal)
	{
		log_error("p_role[%lu] object_state[%u] error", p_role->get_object_id(), p_role->get_object_state());
		return errcode_enum::notice_king_war_error_state;
	}

	monster_ptr p_monster = monster_manager_t::find_monster(object_id);
	if (NULL == p_monster)
	{
		log_error("can not find p_monster[%lu]", object_id);
		return errcode_enum::sys_notice_object_not_exsit;
	}

	if (!p_monster->is_long_pulse())
	{
		log_error("object[%lu] is not long", object_id);
		return errcode_enum::notice_unknown;
	}

	long_pulse_ptr p_long_monster = p_monster->get_long_pulse();
	if (NULL == p_long_monster)
	{
		log_error("object[%lu] is not long", object_id);
		return errcode_enum::notice_unknown;
	}

	return p_long_monster->ask_add_defence_list(p_role);
}

uint32_t king_war_manager_t::ask_attack_long(role_ptr p_role, uint64_t object_id)
{
	if (NULL == p_role)
	{
		log_error("p_role null error");
		return errcode_enum::notice_unknown;
	}

	// 死亡提醒
	if (p_role->is_die())
	{
		log_error("p_role[%lu] obj_state[%u] error", p_role->get_object_id(), p_role->get_object_state());
		return errcode_enum::sys_notice_you_dead;
	}

	// 战意值为0
	if (p_role->get_king_war_warfare() == 0)
	{
		log_error("p_role[%lu] king_war_warfare[%u] == 0", p_role->get_object_id(), p_role->get_king_war_warfare());
		return errcode_enum::notice_king_war_error14;
	}

	// 自己状态
	//if (p_role->get_object_state() != proto::common::object_state_normal)
	//{
	//	log_error("p_role[%lu] object_state[%u] error", p_role->get_object_id(), p_role->get_object_state());
	//	return errcode_enum::notice_king_war_error_state;
	//}

	monster_ptr p_monster = monster_manager_t::find_monster(object_id);
	if (NULL == p_monster)
	{
		log_error("can not find p_monster[%lu]", object_id);
		return errcode_enum::sys_notice_object_not_exsit;
	}

	if (!p_monster->is_long_pulse())
	{
		log_error("object[%lu] is not long", object_id);
		return errcode_enum::notice_unknown;
	}

	long_pulse_ptr p_long_monster = p_monster->get_long_pulse();
	if (NULL == p_long_monster)
	{
		log_error("object[%lu] is not long", object_id);
		return errcode_enum::notice_unknown;
	}

	return p_long_monster->ask_add_attack_list(p_role);
}

void king_war_manager_t::attack_long_end(role_ptr p_role, uint64_t object_id)
{
	if (NULL == p_role)
	{
		log_error("p_role null error");
		return;
	}

	// 先把人的状态设置回来
	//if (p_role->get_object_state() != proto::common::object_state_attack)
	//{
	//	log_warn("p_role[%lu] is already object_state[%u] != proto::common::role_sys_state_attack", p_role->get_object_id(), p_role->get_object_state());
	//	return;
	//}

	// 设置为normal状态
	p_role->change_object_state(proto::common::object_state_normal);

	monster_ptr p_monster = monster_manager_t::find_monster(object_id);
	if (NULL == p_monster)
	{
		log_error("can not find p_monster[%lu]", object_id);
		return;
	}

	// 死亡
	if (p_monster->is_die())
	{
		KINGWAR_LOG("object[%lu] is dead", object_id);
		return;
	}

	if (!p_monster->is_long_pulse())
	{
		log_error("object[%lu] is not long", object_id);
		return;
	}

	long_pulse_ptr p_long_monster = p_monster->get_long_pulse();
	if (NULL == p_long_monster)
	{
		log_error("object[%lu] is not long", object_id);
		return;
	}

	p_long_monster->ask_end_attack(p_role->get_object_id());
}

void king_war_manager_t::ask_hurt_long(role_ptr p_role, uint64_t object_id, proto::client::gc_king_war_attack_end_reply& reply)
{
	if (NULL == p_role)
	{
		log_error("p_role null error");
		reply.set_reply_code(errcode_enum::notice_unknown);
		return;
	}

	// 战意值为0
	if (p_role->get_king_war_warfare() == 0)
	{
		log_error("p_role[%lu] king_war_warfare[%u] == 0", p_role->get_object_id(), p_role->get_king_war_warfare());
		reply.set_reply_code(errcode_enum::notice_king_war_error14);
		return;
	}

	// 先判断状态对不对
	if (p_role->get_object_state() != proto::common::object_state_attack)
	{
		KINGWAR_LOG("p_role[%lu] object_state[%u] is already not proto::common::role_sys_state_attack", p_role->get_object_id(), p_role->get_object_state());
		reply.set_reply_code(errcode_enum::notice_king_war_error_state);
		return;
	}

	// 判断怪物对不对
	if (p_role->get_object_state_param() != object_id)
	{
		log_error("p_role[%lu] hurt param error", p_role->get_object_id());
		reply.set_reply_code(errcode_enum::notice_unknown);
		return;
	}

	monster_ptr p_monster = monster_manager_t::find_monster(object_id);
	if (NULL == p_monster)
	{
		log_error("can not find p_monster[%lu]", object_id);
		reply.set_reply_code(errcode_enum::sys_notice_object_not_exsit);
		return;
	}

	if (!p_monster->is_long_pulse())
	{
		log_error("object[%lu] is not long", object_id);
		reply.set_reply_code(errcode_enum::notice_unknown);
		return;
	}

	long_pulse_ptr p_long_monster = p_monster->get_long_pulse();
	if (NULL == p_long_monster)
	{
		log_error("object[%lu] is not long", object_id);
		reply.set_reply_code(errcode_enum::notice_unknown);
		return;
	}

	// 死亡
	if (p_long_monster->is_die())
	{
		KINGWAR_LOG("object[%lu] is dead", object_id);
		reply.set_reply_code(errcode_enum::sys_notice_object_dead);

		// 把人的状态设置回来，设置为normal状态
		p_role->change_object_state(proto::common::object_state_normal);
		return;
	}

	p_long_monster->ask_hurt_long(p_role, reply);
}

void king_war_manager_t::add_long_gas(uint32_t country_id, uint32_t side, uint32_t val)
{
	country_scene_info_ptr p_info = get_country_info_by_id(country_id);
	if (p_info == NULL)
	{
		log_error("country[%u] not find info", country_id);
		return;
	}

	// 结束了不增加
	if (p_info->m_king_war_state == proto::common::king_war_state_end)
	{
		return;
	}

	if (king_war_side_attack == side)
	{
		p_info->m_king_war_info.m_family_long_gas[king_war_side_attack] += val;

		KINGWAR_LOG("country[%u] side[%u] add long gas[%u], total[%u]", country_id, side, val, p_info->m_king_war_info.m_family_long_gas[king_war_side_attack]);
	}
	else if (king_war_side_defence == side)
	{
		p_info->m_king_war_info.m_family_long_gas[king_war_side_defence] += val;

		KINGWAR_LOG("country[%u] side[%u] add long gas[%u], total[%u]", country_id, side, val, p_info->m_king_war_info.m_family_long_gas[king_war_side_defence]);
	}
	else
	{
		log_error("country[%u] side[%u] is not king war family", country_id, side);
	}

	// 超过4000 提前结束
	auto p_conf = GET_CONF(Comprehensive, comprehensive_common::king_score_max);
	uint32_t max_gas = GET_COMPREHENSIVE_VALUE_1(p_conf);
	if (p_info->m_king_war_info.m_family_long_gas[king_war_side_attack] > max_gas || p_info->m_king_war_info.m_family_long_gas[king_war_side_defence] > max_gas)
	{
		king_war_settlement(country_id);

		KINGWAR_LOG("country[%u] side[%u] an early closure", country_id, side);
	}
}

void king_war_manager_t::role_kill_other(king_war_scene_ptr p_country, const role_ptr& p_role, const role_ptr& p_enemy)
{
	if (NULL == p_country) {
		log_error("p_country null error");
		return;
	}

	if (NULL == p_role)
	{
		log_error("p_role null error");
		return;
	}

	if (NULL == p_enemy)
	{
		log_error("p_enemy null error");
		return;
	}

	uint32_t king_war_side = p_country->get_king_war_side_type(p_role->get_family_id());
	if (king_war_side == king_war_side_none) {
		log_error("role[%lu] not country[%u] king war family", p_role->get_uid(), p_country->get_country_id());
		return;
	}

	// 获得自己的信息，没有就增加
	king_war_role_ptr p_king_war_role = get_king_war_role_ex(p_country->get_country_id(), p_role);
	if (p_king_war_role == NULL)
	{
		log_error("role[%lu] add king war role info null error", p_role->get_object_id());
		return;
	}

	// 敌方的信息
	em_kill_title enemy_title = em_kill_title_0;
	king_war_role_ptr p_king_war_enemy = get_king_war_role(p_country->get_country_id(), p_enemy->get_object_id());
	if (p_king_war_enemy != NULL)
	{
		enemy_title = p_king_war_enemy->m_kill_title;
	}

	// 连击杀数增加
	p_king_war_role->add_continued_kill();

	p_king_war_role->m_kill_title = get_kill_title(p_king_war_role->m_kills);
	p_king_war_role->add_kill_count();

	uint32_t sys_notice_kill_id = sys_notice_enum::sys_notice_25;
	switch (p_king_war_role->m_kill_title) {
		case em_kill_title_4: {
			sys_notice_kill_id = sys_notice_enum::sys_notice_29;
			break;
		}
		case em_kill_title_3: {
			sys_notice_kill_id = sys_notice_enum::sys_notice_28;
			break;
		}
		case em_kill_title_2: {
			sys_notice_kill_id = sys_notice_enum::sys_notice_27;
			break;
		}
		case em_kill_title_1: {
			sys_notice_kill_id = sys_notice_enum::sys_notice_26;
			break;
		}
		default: {
			sys_notice_kill_id = sys_notice_enum::sys_notice_25;
			break;
		}
	}

	if (sys_notice_kill_id != 0) {
		BROADCAST_SCENE_NOTICE(p_country, sys_notice_kill_id, 0, 0, 0, p_role->get_family_name().c_str(), p_role->get_name().c_str(), p_enemy->get_family_name().c_str(), p_enemy->get_name().c_str());
	}

	// 胜利基础龙气增加
	auto p_conf = GET_CONF(Comprehensive, comprehensive_common::king_fight_base_score1);
	uint32_t base_win_add_gas = GET_COMPREHENSIVE_VALUE_1(p_conf);
	p_conf = GET_CONF(Comprehensive, comprehensive_common::king_fight_base_merit1);
	uint32_t base_win_add_exploit = GET_COMPREHENSIVE_VALUE_1(p_conf);

	float fight_precent = 1.0f;
	if (p_role->get_fighting() != 0) {
		fight_precent = (float)p_enemy->get_fighting() / (float)p_role->get_fighting();
	}

	uint32_t add_gas = base_win_add_gas * fight_precent;
	uint32_t add_exploit = base_win_add_exploit * fight_precent;

	// 自己的称号奖励
	std::pair<uint32_t, uint32_t> self_title_rwd = get_reward_by_self_title(p_king_war_role->m_kill_title);
	add_gas += self_title_rwd.first;
	add_exploit += self_title_rwd.second;

	// 终结的称号奖励
	std::pair<uint32_t, uint32_t> enemy_title_rwd = get_reward_by_other_title(enemy_title);
	add_gas += enemy_title_rwd.first;
	add_exploit += enemy_title_rwd.second;

	// 击杀官员奖励
	std::pair<uint32_t, uint32_t> offer_title_rwd = get_reward_by_other_offer(p_enemy->get_family_officer());
	add_gas += offer_title_rwd.first;
	add_exploit += offer_title_rwd.second;

	p_king_war_role->add_exploit(add_exploit);

	add_long_gas(p_country->get_country_id(), king_war_side, add_gas);

	KINGWAR_LOG("role[%lu] win country[%u] family[%lu] add long gas[%u], add exploit[%u]", p_role->get_object_id(), p_country->get_country_id(), p_role->get_family_id(), add_gas, add_exploit);

	achieve_common_t::notify_progress_state(p_role->get_uid(), proto::common::ACHIEVE_KING_WAR_KILL_PLAYER_NUMBER);
}

void king_war_manager_t::other_kill_self(king_war_scene_ptr p_country, const role_ptr& p_role, const role_ptr& p_enemy)
{
	if (NULL == p_country) {
		log_error("p_country null error");
		return;
	}

	if (NULL == p_role)
	{
		log_error("p_role null error");
		return;
	}

	if (NULL == p_enemy)
	{
		log_error("p_enemy null error");
		return;
	}

	uint32_t king_war_side = p_country->get_king_war_side_type(p_role->get_family_id());
	if (king_war_side == king_war_side_none) {
		log_error("role[%lu] not country[%u] king war family", p_role->get_uid(), p_country->get_country_id());
		return;
	}

	// 获得自己的信息，没有就增加
	king_war_role_ptr p_king_war_role = get_king_war_role_ex(p_country->get_country_id(), p_role);
	if (p_king_war_role == NULL)
	{
		log_error("role[%lu] add king war role info null error", p_role->get_object_id());
		return;
	}

	uint32_t sys_notice_kill_id = 0;
	switch (p_king_war_role->m_kill_title) {
		case em_kill_title_4: {
			sys_notice_kill_id = sys_notice_enum::sys_notice_33;
			break;
		}
		case em_kill_title_3: {
			sys_notice_kill_id = sys_notice_enum::sys_notice_32;
			break;
		}
		case em_kill_title_2: {
			sys_notice_kill_id = sys_notice_enum::sys_notice_31;
			break;
		}
		case em_kill_title_1: {
			sys_notice_kill_id = sys_notice_enum::sys_notice_30;
			break;
		}
		default: {
			break;
		}
	}

	if (sys_notice_kill_id != 0) {
		BROADCAST_SCENE_NOTICE(p_country, sys_notice_kill_id, 0, 0, 0, p_enemy->get_family_name().c_str(), p_enemy->get_name().c_str(), p_role->get_family_name().c_str(), p_role->get_name().c_str());
	}

	// 击杀数增加
	p_king_war_role->clear_continued_kill();
	p_king_war_role->m_kill_title = em_kill_title_0;

	if (p_king_war_role->m_warfare > 0)
	{
		p_king_war_role->m_warfare -= 1;
	}

	p_role->set_king_war_warfare(p_king_war_role->m_warfare);

	p_king_war_role->add_death_count();

	// 通知死亡次数
	proto::client::gc_role_battle_value_notify battle_value_notify;
	battle_value_notify.set_state(1);
	battle_value_notify.set_value(p_king_war_role->m_warfare);
	p_role->send_msg_to_client(op_cmd::gc_role_battle_value_notify, battle_value_notify);

	// 死亡超过配置数量
	if (p_king_war_role->m_warfare == 0)
	{
		proto::client::gc_king_war_icon_notify notify;
		notify.set_state(0);
		p_role->send_msg_to_client(op_cmd::gc_king_war_icon_notify, notify);

		// 设置pk模式
		if (p_role->get_pk_mode() != proto::common::PK_PEACE)
		{
			p_role->change_pk_mode(proto::common::PK_PEACE);
		}
	}

	// 失败基础龙气增加
	auto p_conf = GET_CONF(Comprehensive, comprehensive_common::king_fight_base_score2);
	uint32_t base_failed_add_gas = GET_COMPREHENSIVE_VALUE_1(p_conf);
	p_conf = GET_CONF(Comprehensive, comprehensive_common::king_fight_base_merit2);
	uint32_t base_failed_add_exploit = GET_COMPREHENSIVE_VALUE_1(p_conf);

	float fight_precent = 1.0f;
	if (p_role->get_fighting() != 0) {
		fight_precent = (float)p_enemy->get_fighting() / (float)p_role->get_fighting();
	}

	uint32_t add_gas = base_failed_add_gas * fight_precent;
	uint32_t add_exploit = base_failed_add_exploit * fight_precent;

	p_king_war_role->add_exploit(add_exploit);

	add_long_gas(p_country->get_country_id(), king_war_side, add_gas);

	KINGWAR_LOG("role[%lu] failed country[%u] family[%lu] add long gas[%u], add exploit[%u]", p_role->get_object_id(), p_country->get_country_id(), p_role->get_family_id(), add_gas, add_exploit);
}

bool king_war_manager_t::role_fast_revive(uint32_t country_id, const role_ptr& p_role)
{
	if (NULL == p_role)
	{
		log_error("p_role null error");
		return false;
	}

	king_war_role_ptr p_king_war_role = get_king_war_role(country_id, p_role->get_object_id());
	if (p_king_war_role == NULL)
	{
		log_error("role[%lu] have no king war info", p_role->get_object_id());
		return false;
	}

	if (p_king_war_role->m_free_revive_count == 0)
	{
		log_error("role[%lu] have no free_revive_count", p_role->get_object_id());
		return false;
	}

	p_king_war_role->m_free_revive_count -= 1;

	auto p_conf = GET_CONF(Comprehensive, comprehensive_common::king_revive_merit);
	uint32_t add_val = GET_COMPREHENSIVE_VALUE_1(p_conf);
	p_king_war_role->add_exploit(add_val);

	KINGWAR_LOG("role[%lu] fast revive ,curr free_revive_count[%u]", p_role->get_object_id(), p_king_war_role->m_free_revive_count);

	return true;
}

void king_war_manager_t::attack_long_add(uint32_t country_id, const role_ptr& p_role)
{
	if (NULL == p_role)
	{
		log_error("p_role null error");
		return;
	}

	// 获得自己的信息，没有就增加
	king_war_role_ptr p_king_war_role = get_king_war_role_ex(country_id, p_role);
	if (p_king_war_role == NULL)
	{
		log_error("role[%lu] add king war role info null error", p_role->get_object_id());
		return;
	}

	auto p_conf = GET_CONF(Comprehensive, comprehensive_common::king_attack_dragon_merit);
	uint32_t add_exploit = GET_COMPREHENSIVE_VALUE_1(p_conf);
	p_king_war_role->add_exploit(add_exploit);

	KINGWAR_LOG("role[%lu] hurt long, add exploit[%u], total[%u]", p_role->get_object_id(), add_exploit, p_king_war_role->get_exploit());
	achieve_common_t::notify_progress_state(p_role->get_uid(), proto::common::ACHIEVE_KING_WAR_DRAGON_HARM,0,add_exploit);
}

void king_war_manager_t::gm_add_exploit(const role_ptr& p_role, uint32_t val)
{
	if (NULL == p_role)
	{
		log_error("p_role null error");
		return;
	}

	const scene_ptr& p_country_scene = scene_manager_t::find_scene(p_role->get_scene_id());
	if (NULL == p_country_scene) {
		log_error("role[%lu] scene null error", p_role->get_uid());
		return;
	}

	if (!p_country_scene->is_king_war_city()) {
		log_error("role[%lu] not in king war scene", p_role->get_uid());
		return;
	}

	king_war_scene_ptr p_king_scene = p_country_scene->get_king_war_scene_ptr();
	if (p_king_scene == NULL) {
		log_error("role[%lu] not in king war scene", p_role->get_uid());
		return;
	}

	// 获得自己的信息，没有就增加
	king_war_role_ptr p_king_war_role = get_king_war_role_ex(p_king_scene->get_country_id(), p_role);
	if (p_king_war_role == NULL)
	{
		log_error("role[%lu] add king war role info null error", p_role->get_object_id());
		return;
	}

	p_king_war_role->add_exploit(val);

	KINGWAR_LOG("role[%lu] gm add exploit[%u], total[%u]", p_role->get_object_id(), val, p_king_war_role->get_exploit());
}

void king_war_manager_t::ask_king_war_fight(const role_ptr& p_role, proto::client::gc_ask_king_war_fight_reply& reply)
{
	if (p_role == NULL) {
		log_error("p_role null error");
		reply.set_reply_code(common::errcode_enum::notice_unknown);
		return;
	}

	const scene_ptr& p_country_scene = scene_manager_t::find_scene(p_role->get_scene_id());
	if (NULL == p_country_scene) {
		log_error("role[%lu] scene null error", p_role->get_uid());
		reply.set_reply_code(common::errcode_enum::notice_unknown);
		return;
	}

	if (!p_country_scene->is_king_war_city()) {
		log_error("role[%lu] not in king war scene", p_role->get_uid());
		reply.set_reply_code(common::errcode_enum::king_war_error_not_in_king_war_scene);
		return;
	}

	king_war_scene_ptr p_king_scene = p_country_scene->get_king_war_scene_ptr();
	if (p_king_scene == NULL) {
		log_error("role[%lu] not in king war scene", p_role->get_uid());
		reply.set_reply_code(common::errcode_enum::king_war_error_not_in_king_war_scene);
		return;
	}

	if (p_king_scene->get_king_war_state() != proto::common::king_war_state_start)
	{
		log_error("country[%u] country_scene_info_t::m_king_war_state != proto::common::king_war_state_start", p_king_scene->get_country_id());
		reply.set_reply_code(errcode_enum::king_war_error_not_start_fight);
		return;
	}

	uint32_t side_type = p_king_scene->get_king_war_side_type(p_role->get_family_id());
	if (side_type == king_war_side_none) {
		log_error("role[%lu] is not king war family", p_role->get_object_id());
		reply.set_reply_code(errcode_enum::notice_king_war_error12);
		return;
	}

	country_scene_info_ptr p_info = get_country_info_by_id(p_king_scene->get_country_id());
	if (p_info == NULL) {
		log_error("role[%lu] country[%u] not find", p_role->get_object_id(), p_king_scene->get_country_id());
		reply.set_reply_code(errcode_enum::notice_unknown);
		return;
	}

	reply.set_reply_code(common::errcode_enum::error_ok);

	p_king_scene->get_king_war_roles_pos(side_type, reply.mutable_self_role());
	
	reply.set_artifact_start_left(p_king_scene->get_artifacts_start_left());

	reply.set_self_family(p_info->m_king_war_info.m_fight_family[side_type].m_family_name);
	reply.set_self_warfare(p_king_scene->get_king_war_warfare(side_type));
	reply.set_self_long_gas(p_info->m_king_war_info.m_family_long_gas[side_type]);

	switch (side_type) {
		case king_war_side_attack: {
			reply.set_enemy_family(p_info->m_king_war_info.m_fight_family[king_war_side_defence].m_family_name);
			reply.set_enemy_warfare(p_king_scene->get_king_war_warfare(king_war_side_defence));
			reply.set_enemy_long_gas(p_info->m_king_war_info.m_family_long_gas[king_war_side_defence]);
			break;
		}
		case king_war_side_defence: {
			reply.set_enemy_family(p_info->m_king_war_info.m_fight_family[king_war_side_attack].m_family_name);
			reply.set_enemy_warfare(p_king_scene->get_king_war_warfare(king_war_side_attack));
			reply.set_enemy_long_gas(p_info->m_king_war_info.m_family_long_gas[king_war_side_attack]);
			break;
		}
		default:
			break;
	}

	const king_war_rank_list& ranklist = p_info->get_rank_list();
	uint32_t index = 0;
	for (king_war_rank_list::const_iterator citr = ranklist.begin();
		citr != ranklist.end() && index < 3; ++citr, ++index) {
		const king_war_role_ptr& p_king_war_role = *citr;
		if (p_king_war_role) {
			proto::common::king_war_rank_single* p_info = reply.add_rank_list();
			if (p_info) {
				p_info->CopyFrom(p_king_war_role->m_rank_single);
			}
		}
	}

	king_war_role_map::const_iterator role_citr = p_info->m_king_war_role_map.find(p_role->get_uid());
	if (role_citr != p_info->m_king_war_role_map.end()) {
		if (role_citr->second) {
			reply.set_self_exploits(role_citr->second->m_exploit);
		}
	}
}

void king_war_manager_t::ask_king_war_fight_rank(const role_ptr& p_role, proto::client::gc_king_war_rank_list_reply& reply)
{
	if (p_role == NULL) {
		log_error("p_role null error");
		reply.set_reply_code(common::errcode_enum::notice_unknown);
		return;
	}

	const scene_ptr& p_country_scene = scene_manager_t::find_scene(p_role->get_scene_id());
	if (NULL == p_country_scene) {
		log_error("role[%lu] scene null error", p_role->get_uid());
		reply.set_reply_code(common::errcode_enum::notice_unknown);
		return;
	}

	if (!p_country_scene->is_king_war_city()) {
		log_error("role[%lu] not in king war scene", p_role->get_uid());
		reply.set_reply_code(common::errcode_enum::king_war_error_not_in_king_war_scene);
		return;
	}

	king_war_scene_ptr p_king_scene = p_country_scene->get_king_war_scene_ptr();
	if (p_king_scene == NULL) {
		log_error("role[%lu] not in king war scene", p_role->get_uid());
		reply.set_reply_code(common::errcode_enum::king_war_error_not_in_king_war_scene);
		return;
	}

// 	if (p_king_scene->get_king_war_state() != proto::common::king_war_state_start)
// 	{
// 		log_error("country[%u] country_scene_info_t::m_king_war_state != proto::common::king_war_state_start", p_king_scene->get_country_id());
// 		reply.set_reply_code(errcode_enum::king_war_error_not_start_fight);
// 		return;
// 	}

	country_scene_info_ptr p_info = get_country_info_by_id(p_king_scene->get_country_id());
	if (p_info == NULL) {
		log_error("role[%lu] country[%u] not find", p_role->get_object_id(), p_king_scene->get_country_id());
		reply.set_reply_code(errcode_enum::notice_unknown);
		return;
	}

	reply.set_reply_code(common::errcode_enum::error_ok);

	proto::common::king_war_rank_list* p_rank_list = reply.mutable_list();
	if (p_rank_list) {
		p_rank_list->CopyFrom(p_info->get_king_war_ranks());
	}
}

uint32_t king_war_manager_t::ask_king_war_enter_scene(const role_ptr& p_role)
{
	if (p_role == NULL) {
		log_error("p_role null error");
		return common::errcode_enum::notice_unknown;
	}

	const scene_ptr& p_country_scene = scene_manager_t::find_scene(p_role->get_scene_id());
	if (NULL == p_country_scene) {
		log_error("role[%lu] scene null error", p_role->get_uid());
		return common::errcode_enum::notice_unknown;
	}

	if (!p_country_scene->is_king_war_city()) {
		log_error("role[%lu] not in king war scene", p_role->get_uid());
		return common::errcode_enum::king_war_error_not_in_king_war_scene;
	}

	king_war_scene_ptr p_king_scene = p_country_scene->get_king_war_scene_ptr();
	if (p_king_scene == NULL) {
		log_error("role[%lu] not in king war scene", p_role->get_uid());
		return common::errcode_enum::king_war_error_not_in_king_war_scene;
	}

// 	if (p_king_scene->get_king_war_state() != proto::common::king_war_state_start) {
// 		log_error("country[%u] country_scene_info_t::m_king_war_state != proto::common::king_war_state_start", p_king_scene->get_country_id());
// 		return errcode_enum::king_war_error_not_start_fight;
// 	}

	return p_king_scene->ask_king_war_enter_scene(p_role);
}

