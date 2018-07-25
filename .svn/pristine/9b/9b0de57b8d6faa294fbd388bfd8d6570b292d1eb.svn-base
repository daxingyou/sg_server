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

	if (p_info->m_king_war_state != proto::common::king_war_state_prepare && p_info->m_king_war_state != proto::common::king_war_state_start)
	{
		// 清理上次的数据
		p_info->reset();

		p_info->m_king_war_info.m_family_id[king_war_side_attack] = notify.att_family_id();
		p_info->m_king_war_info.m_family_name[king_war_side_attack] = notify.att_family_name();
		p_info->m_king_war_info.m_family_id[king_war_side_defence] = notify.def_family_id();
		p_info->m_king_war_info.m_family_name[king_war_side_defence] = notify.def_family_name();
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

	p_king_scene->set_king_war_family(king_war_side_defence, p_info->m_king_war_info.m_family_id[king_war_side_defence]);
	p_king_scene->set_king_war_family(king_war_side_attack, p_info->m_king_war_info.m_family_id[king_war_side_attack]);

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

	// 给每个玩家一个战功排名
	std::vector<king_war_role_ptr> all_role;
	for (king_war_role_map::iterator itr = p_info->m_king_war_role_map.begin();
		itr != p_info->m_king_war_role_map.end(); ++itr)
	{
		if (itr->second != NULL)
			all_role.push_back(itr->second);
	}

	// TODO:qhj 排序有待商确
	//std::sort(all_role.begin(), all_role.end(), king_war_manager_t::exploit_rank_sort);
	if (all_role.size() > 1)
	{
		common::Sort::Qsort(all_role, 0, all_role.size() - 1, king_war_manager_t::exploit_rank_sort);
	}

	uint32_t rank = 0;
	for (std::vector<king_war_role_ptr>::iterator itr = all_role.begin();
		itr != all_role.end(); ++itr)
	{
		king_war_role_ptr p_king_war_role = *itr;
		if (p_king_war_role != NULL)
		{
			p_king_war_role->m_rank = ++rank;

			///成就项统计
			achieve_common_t::notify_progress_state(p_king_war_role->m_uid, proto::common::ACHIEVE_KING_WAR_NUMBER);
			///achieve_common_t::notify_progress_state(p_king_war_role->m_uid, proto::common::ACHIEVE_KING_WAR_QUICK_REVIVE,1,p_king_war_role->m_free_revive_count);
			///achieve_common_t::notify_progress_state(p_king_war_role->m_uid, proto::common::ACHIEVE_KING_WAR_KILL_PLAYER_NUMBER,1,p_king_war_role->m_all_kills);
		}
	}

	// 排序结束
	all_role.clear();

	// 设置王城战结束
	p_info->m_king_war_state = proto::common::king_war_state_end;

	// TODO:根据战功发奖励

	// 发消息给center哪个家族赢了
	if (NULL == env::server)
	{
		log_error("env::server null error");
		return;
	}

	proto::server::ge_king_war_result_notify notify_center;
	notify_center.set_country_id(country_id);
	if (p_info->m_king_war_info.m_family_long_gas[king_war_side_attack] > p_info->m_king_war_info.m_family_long_gas[king_war_side_defence])
	{
		notify_center.set_win_family_id(p_info->m_king_war_info.m_family_id[king_war_side_attack]);
	}
	else
	{
		notify_center.set_win_family_id(p_info->m_king_war_info.m_family_id[king_war_side_defence]);
	}

	env::server->send_msg_to_center(op_cmd::ge_king_war_result_notify, 0, notify_center);
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

king_war_role_ptr king_war_manager_t::get_king_war_role_ex(uint32_t country_id, uint64_t uid)
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

	king_war_role_ptr p_king_war_role = king_war_role_ptr(new king_war_role_t());
	if (p_king_war_role == NULL)
	{
		log_error("p_king_war_role null error");
		return NULL;
	}

	p_king_war_role->m_uid = uid;
	p_king_war_role->m_warfare = WARFARE_DEFAULT;
	auto p_conf = GET_CONF(Comprehensive, comprehensive_common::king_revive_free_cnt);
	p_king_war_role->m_free_revive_count = GET_COMPREHENSIVE_VALUE_1(p_conf);

	p_info->m_king_war_role_map.insert(std::make_pair(uid, p_king_war_role));

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

	// TODO:目前game不知道职位

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

void king_war_manager_t::get_king_war_family_num(uint32_t country_id, proto::client::gc_king_war_scene_num_reply& notify)
{
	country_scene_info_ptr p_info = get_country_info_by_id(country_id);
	if (p_info == NULL)
	{
		log_error("country[%u] not find info", country_id);
		notify.set_reply_code(errcode_enum::country_error_invalid_country);
		return;
	}

	const scene_ptr& p_country_scene = scene_manager_t::find_scene(p_info->m_scene_id);
	if (NULL == p_country_scene)
	{
		log_error("country[%u] scene[%lu] can not find", country_id, p_info->m_scene_id);
		notify.set_reply_code(errcode_enum::notice_unknown);
		return;
	}

	if (!p_country_scene->is_king_war_city())
	{
		log_error("scene[%lu] is not king scene", p_info->m_scene_id);
		notify.set_reply_code(errcode_enum::notice_unknown);
		return;
	}

	king_war_scene_ptr p_king_scene = p_country_scene->get_king_war_scene_ptr();
	if (p_king_scene == NULL)
	{
		log_error("scene[%lu] is not king scene", p_info->m_scene_id);
		notify.set_reply_code(errcode_enum::notice_unknown);
		return;
	}

	notify.set_reply_code(errcode_enum::error_ok);
	notify.set_att_family_num(p_king_scene->get_scene_family_num(king_war_side_attack));
	notify.set_def_family_num(p_king_scene->get_scene_family_num(king_war_side_defence));
}

uint32_t king_war_manager_t::ask_enter_king_war_scene(role_ptr p_role)
{
	if (NULL == p_role)
	{
		log_error("p_role null error");
		return errcode_enum::notice_unknown;
	}

	country_scene_info_ptr p_info = get_country_info_by_id(p_role->get_country_id());
	if (p_info == NULL)
	{
		log_error("role[%lu] country[%u] not find info", p_role->get_object_id(), p_role->get_country_id());
		return errcode_enum::country_error_invalid_country;
	}

	// 不在备战期和战斗开始不让进
	if (p_info->m_king_war_state < proto::common::king_war_state_prepare)
	{
		log_error("role[%lu] country[%u] country_scene_info_t::m_king_war_state < proto::common::king_war_state_prepare", p_role->get_object_id(), p_role->get_country_id());
		return errcode_enum::notice_king_war_error11;
	}

	// 不是参战家族
	if (p_role->get_family_id() != p_info->m_king_war_info.m_family_id[king_war_side_attack] && p_role->get_family_id() != p_info->m_king_war_info.m_family_id[king_war_side_defence])
	{
		log_error("role[%lu] family_id[%lu] is not king war family", p_role->get_object_id(), p_role->get_family_id());
		return errcode_enum::notice_king_war_error12;
	}

	const scene_ptr& p_country_scene = scene_manager_t::find_scene(p_info->m_scene_id);
	if (NULL == p_country_scene)
	{
		log_error("role[%lu] country[%u] scene[%lu] can not find", p_role->get_object_id(), p_role->get_country_id(), p_info->m_scene_id);
		return errcode_enum::notice_unknown;
	}

	if (!p_country_scene->is_king_war_city())
	{
		log_error("role[%lu] scene[%lu] is not king country", p_role->get_object_id(), p_info->m_scene_id);
		return errcode_enum::notice_unknown;
	}

	king_war_scene_ptr p_king_scene = p_country_scene->get_king_war_scene_ptr();
	if (p_king_scene == NULL)
	{
		log_error("role[%lu] scene[%lu] is not king country", p_role->get_object_id(), p_info->m_scene_id);
		return errcode_enum::notice_unknown;
	}

	return p_king_scene->king_war_scene_process(p_role);
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

void king_war_manager_t::user_artifacts(role_ptr p_role, uint32_t index)
{
	if (NULL == p_role)
	{
		log_error("role null error");
		return;
	}
	country_scene_info_ptr p_info = get_country_info_by_id(p_role->get_country_id());
	if (p_info == NULL)
	{
		log_error("role[%lu] country[%u] not find info", p_role->get_object_id(), p_role->get_country_id());
		return;
	}

	if (p_info->m_king_war_state != proto::common::king_war_state_start)
	{
		log_error("country[%u] country_scene_info_t::m_king_war_state != proto::common::king_war_state_start", p_role->get_country_id());
		return;
	}

	const scene_ptr& p_country_scene = scene_manager_t::find_scene(p_info->m_scene_id);
	if (NULL == p_country_scene)
	{
		log_error("role[%lu] scene[%lu] can not find", p_role->get_object_id(), p_info->m_scene_id);
		return;
	}

	if (!p_country_scene->is_king_war_city())
	{
		log_error("role[%lu] scene[%lu] is not king scene", p_role->get_object_id(), p_info->m_scene_id);
		return;
	}

	king_war_scene_ptr p_king_scene = p_country_scene->get_king_war_scene_ptr();
	if (p_king_scene == NULL)
	{
		log_error("role[%lu] scene[%lu] is not king scene", p_role->get_object_id(), p_info->m_scene_id);
		return;
	}

	if (p_king_scene->role_user_artifacts(p_role, index))
	{
		KINGWAR_LOG("role[%lu] user artifacts index[%u]", p_role->get_object_id(), index);

		// 获得自己的信息，没有就增加
		king_war_role_ptr p_king_war_role = get_king_war_role_ex(p_role->get_country_id(), p_role->get_object_id());
		if (p_king_war_role == NULL)
		{
			log_error("role[%lu] add king war role info null error", p_role->get_object_id());
			return;
		}

		auto p_conf = GET_CONF(Comprehensive, comprehensive_common::king_start_weapon_merit);
		uint32_t add_exploit = GET_COMPREHENSIVE_VALUE_1(p_conf);
		p_king_war_role->m_exploit += add_exploit;

		KINGWAR_LOG("role[%lu] user artifacts, add exploit[%u]", p_role->get_object_id(), add_exploit);

		achieve_common_t::notify_progress_state(p_role->get_uid(), proto::common::ACHIEVE_KING_WAR_START_WEAPON);
	}
}

void king_war_manager_t::ask_king_war_result(role_ptr p_role, proto::client::gc_king_war_result_reply& reply)
{
	if (NULL == p_role)
	{
		log_error("role null error");
		reply.set_reply_code(errcode_enum::notice_unknown);
		return;
	}

	country_scene_info_ptr p_info = get_country_info_by_id(p_role->get_country_id());
	if (p_info == NULL)
	{
		log_error("role[%lu] country[%u] not find info", p_role->get_object_id(), p_role->get_country_id());
		reply.set_reply_code(errcode_enum::notice_unknown);
		return;
	}

	if (p_info->m_king_war_state != proto::common::king_war_state_end)
	{
		log_error("country[%u] country_scene_info_t::m_king_war_state != proto::common::king_war_state_end", p_role->get_country_id());
		reply.set_reply_code(errcode_enum::notice_unknown);
		return;
	}

	uint64_t family_id = p_role->get_family_id();
	if (family_id != p_info->m_king_war_info.m_family_id[king_war_side_attack] && family_id != p_info->m_king_war_info.m_family_id[king_war_side_defence])
	{
		log_error("role[%lu] is not king war family", p_role->get_object_id());
		reply.set_reply_code(errcode_enum::notice_king_war_error12);
		return;
	}

	king_war_role_map::const_iterator itr = p_info->m_king_war_role_map.find(p_role->get_object_id());
	if (itr == p_info->m_king_war_role_map.end())
	{
		log_error("role[%lu] is not in m_king_war_role_map", p_role->get_object_id());
		reply.set_reply_code(errcode_enum::notice_king_war_error16);
		return;
	}

	const king_war_role_ptr& p_king_war_role = itr->second;
	if (p_king_war_role == NULL)
	{
		reply.set_reply_code(errcode_enum::notice_unknown);
		return;
	}

	reply.set_def_name(p_info->m_king_war_info.m_family_name[king_war_side_defence]);
	reply.set_def_score(p_info->m_king_war_info.m_family_long_gas[king_war_side_defence]);
	reply.set_att_name(p_info->m_king_war_info.m_family_name[king_war_side_attack]);
	reply.set_att_score(p_info->m_king_war_info.m_family_long_gas[king_war_side_attack]);
	if (p_info->m_king_war_info.m_family_long_gas[king_war_side_attack] > p_info->m_king_war_info.m_family_long_gas[king_war_side_defence])
	{
		reply.set_win_family(common::string_util_t::uint64_to_string(p_info->m_king_war_info.m_family_id[king_war_side_attack]));
	}
	else
	{
		reply.set_win_family(common::string_util_t::uint64_to_string(p_info->m_king_war_info.m_family_id[king_war_side_defence]));
	}
	reply.set_rank(p_king_war_role->m_rank);
	reply.set_kill_count(p_king_war_role->m_all_kills);
	reply.set_continued_kill(p_king_war_role->m_max_kills);
	reply.set_death_count(p_king_war_role->m_death_count);
	reply.set_exploits(p_king_war_role->m_exploit);

	reply.set_reply_code(errcode_enum::error_ok);
}

void king_war_manager_t::add_long_gas(uint32_t country_id, uint64_t family_id, uint32_t val)
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

	if (p_info->m_king_war_info.m_family_id[king_war_side_attack] == family_id)
	{
		p_info->m_king_war_info.m_family_long_gas[king_war_side_attack] += val;

		KINGWAR_LOG("country[%u] family[%lu] add long gas[%u], total[%u]", country_id, family_id, val, p_info->m_king_war_info.m_family_long_gas[king_war_side_attack]);
	}
	else if (p_info->m_king_war_info.m_family_id[king_war_side_defence] == family_id)
	{
		p_info->m_king_war_info.m_family_long_gas[king_war_side_defence] += val;

		KINGWAR_LOG("country[%u] family[%lu] add long gas[%u], total[%u]", country_id, family_id, val, p_info->m_king_war_info.m_family_long_gas[king_war_side_defence]);
	}
	else
	{
		log_error("country[%u] family[%lu] is not king war family", country_id, family_id);
	}

	// 超过4000 提前结束
	auto p_conf = GET_CONF(Comprehensive, comprehensive_common::king_score_max);
	uint32_t max_gas = GET_COMPREHENSIVE_VALUE_1(p_conf);
	if (p_info->m_king_war_info.m_family_long_gas[king_war_side_attack] > max_gas || p_info->m_king_war_info.m_family_long_gas[king_war_side_defence] > max_gas)
	{
		king_war_settlement(country_id);

		KINGWAR_LOG("country[%u] family[%lu] an early closure", country_id, family_id);
	}
}

void king_war_manager_t::role_kill_other(role_ptr p_role, role_ptr p_enemy)
{
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

	// 获得自己的信息，没有就增加
	king_war_role_ptr p_king_war_role = get_king_war_role_ex(p_role->get_country_id(), p_role->get_object_id());
	if (p_king_war_role == NULL)
	{
		log_error("role[%lu] add king war role info null error", p_role->get_object_id());
		return;
	}

	// 敌方的信息
	em_kill_title enemy_title = em_kill_title_0;
	king_war_role_ptr p_king_war_enemy = get_king_war_role(p_enemy->get_country_id(), p_enemy->get_object_id());
	if (p_king_war_enemy != NULL)
	{
		enemy_title = p_king_war_enemy->m_kill_title;
	}

	// 连击杀数增加
	p_king_war_role->m_kills += 1;
	if (p_king_war_role->m_kills > p_king_war_role->m_max_kills)
	{
		p_king_war_role->m_max_kills = p_king_war_role->m_kills;
	}

	p_king_war_role->m_kill_title = get_kill_title(p_king_war_role->m_kills);
	p_king_war_role->m_all_kills += 1;

	// 胜利基础龙气增加
	auto p_conf = GET_CONF(Comprehensive, comprehensive_common::king_fight_base_score1);
	uint32_t base_win_add_gas = GET_COMPREHENSIVE_VALUE_1(p_conf);
	p_conf = GET_CONF(Comprehensive, comprehensive_common::king_fight_base_merit1);
	uint32_t base_win_add_exploit = GET_COMPREHENSIVE_VALUE_1(p_conf);

	// TODO 目前战斗力数值未定 直接增加基础就行
	uint32_t add_gas = base_win_add_gas;
	uint32_t add_exploit = base_win_add_exploit;

	// 自己的称号奖励
	std::pair<uint32_t, uint32_t> self_title_rwd = get_reward_by_self_title(p_king_war_role->m_kill_title);
	add_gas += self_title_rwd.first;
	add_exploit += self_title_rwd.second;

	// 终结的称号奖励
	std::pair<uint32_t, uint32_t> enemy_title_rwd = get_reward_by_other_title(enemy_title);
	add_gas += enemy_title_rwd.first;
	add_exploit += enemy_title_rwd.second;

	// TODO 击杀官员奖励
	//std::pair<uint32_t, uint32_t> offer_title_rwd = get_reward_by_other_offer(0);
	//add_gas += enemy_title_rwd.first;
	//add_exploit += enemy_title_rwd.second;

	p_king_war_role->m_exploit += add_exploit;

	add_long_gas(p_role->get_country_id(), p_role->get_family_id(), add_gas);

	KINGWAR_LOG("role[%lu] win country[%u] family[%lu] add long gas[%u], add exploit[%u]", p_role->get_object_id(), p_role->get_country_id(), p_role->get_family_id(), add_gas, add_exploit);

	achieve_common_t::notify_progress_state(p_role->get_uid(), proto::common::ACHIEVE_KING_WAR_KILL_PLAYER_NUMBER);
}

void king_war_manager_t::other_kill_self(role_ptr p_role, role_ptr p_enemy)
{
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

	// 获得自己的信息，没有就增加
	king_war_role_ptr p_king_war_role = get_king_war_role_ex(p_role->get_country_id(), p_role->get_object_id());
	if (p_king_war_role == NULL)
	{
		log_error("role[%lu] add king war role info null error", p_role->get_object_id());
		return;
	}

	// 击杀数增加
	p_king_war_role->m_kills = 0;
	p_king_war_role->m_kill_title = em_kill_title_0;

	if (p_king_war_role->m_warfare > 0)
	{
		p_king_war_role->m_warfare -= 1;
	}

	p_role->set_king_war_warfare(p_king_war_role->m_warfare);

	p_king_war_role->m_death_count += 1;

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
		if (p_role->get_pk_mode() != proto::common::PK_PEACE && p_role->change_pk_mode(proto::common::PK_PEACE))
		{
			proto::client::gc_role_change_data_notify notify;
			proto::common::role_change_data* p_data = notify.mutable_change_data();
			if (p_data != NULL)
			{
				p_role->get_personal_info(p_data->mutable_per_info());
			}

			p_role->send_msg_to_client(op_cmd::gc_role_change_data_notify, notify);
		}
	}

	// 失败基础龙气增加
	auto p_conf = GET_CONF(Comprehensive, comprehensive_common::king_fight_base_score2);
	uint32_t base_failed_add_gas = GET_COMPREHENSIVE_VALUE_1(p_conf);
	p_conf = GET_CONF(Comprehensive, comprehensive_common::king_fight_base_merit2);
	uint32_t base_failed_add_exploit = GET_COMPREHENSIVE_VALUE_1(p_conf);

	// TODO 目前战斗力数值未定 直接增加基础就行
	uint32_t add_gas = base_failed_add_gas;
	uint32_t add_exploit = base_failed_add_exploit;

	p_king_war_role->m_exploit += add_exploit;

	add_long_gas(p_role->get_country_id(), p_role->get_family_id(), add_gas);

	KINGWAR_LOG("role[%lu] failed country[%u] family[%lu] add long gas[%u], add exploit[%u]", p_role->get_object_id(), p_role->get_country_id(), p_role->get_family_id(), add_gas, add_exploit);
}

bool king_war_manager_t::role_fast_revive(role_ptr p_role)
{
	if (NULL == p_role)
	{
		log_error("p_role null error");
		return false;
	}

	king_war_role_ptr p_king_war_role = get_king_war_role(p_role->get_country_id(), p_role->get_object_id());
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
	p_king_war_role->m_exploit += GET_COMPREHENSIVE_VALUE_1(p_conf);

	KINGWAR_LOG("role[%lu] fast revive ,curr free_revive_count[%u]", p_role->get_object_id(), p_king_war_role->m_free_revive_count);

	return true;
}

void king_war_manager_t::attack_long_add(role_ptr p_role)
{
	if (NULL == p_role)
	{
		log_error("p_role null error");
		return;
	}

	// 获得自己的信息，没有就增加
	king_war_role_ptr p_king_war_role = get_king_war_role_ex(p_role->get_country_id(), p_role->get_object_id());
	if (p_king_war_role == NULL)
	{
		log_error("role[%lu] add king war role info null error", p_role->get_object_id());
		return;
	}

	auto p_conf = GET_CONF(Comprehensive, comprehensive_common::king_attack_dragon_merit);
	uint32_t add_exploit = GET_COMPREHENSIVE_VALUE_1(p_conf);
	p_king_war_role->m_exploit += add_exploit;

	KINGWAR_LOG("role[%lu] hurt long, add exploit[%u], total[%u]", p_role->get_object_id(), add_exploit, p_king_war_role->m_exploit);
	achieve_common_t::notify_progress_state(p_role->get_uid(), proto::common::ACHIEVE_KING_WAR_DRAGON_HARM,0,add_exploit);
}

void king_war_manager_t::gm_add_exploit(role_ptr p_role, uint32_t val)
{
	if (NULL == p_role)
	{
		log_error("p_role null error");
		return;
	}

	// 获得自己的信息，没有就增加
	king_war_role_ptr p_king_war_role = get_king_war_role_ex(p_role->get_country_id(), p_role->get_object_id());
	if (p_king_war_role == NULL)
	{
		log_error("role[%lu] add king war role info null error", p_role->get_object_id());
		return;
	}

	p_king_war_role->m_exploit += val;

	KINGWAR_LOG("role[%lu] gm add exploit[%u], total[%u]", p_role->get_object_id(), val, p_king_war_role->m_exploit);
}

bool king_war_manager_t::exploit_rank_sort(const king_war_role_ptr& h1, const king_war_role_ptr& h2)
{
	if (NULL == h1 || NULL == h2)
	{
		log_error("h1 or h2 null error");
		return false;
	}

	if (h1->m_exploit > h2->m_exploit)
	{
		return true;
	}
	else if (h1->m_exploit == h2->m_exploit)
	{
		if (h1->m_all_kills != h2->m_all_kills)
		{
			return h1->m_all_kills > h2->m_all_kills;
		}
		else
		{
			return h1->m_uid > h2->m_uid;
		}
	}

	return false;
}

void king_war_manager_t::on_long_revive_timer_call_back(const boost::system::error_code& ec, uint64_t object_id)
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

	monster_ptr p_monster = monster_manager_t::find_monster(object_id);
	if (NULL == p_monster)
	{
		log_error("can not find p_monster[%lu]", object_id);
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

	p_long_monster->on_revive_timer_call_back();
}

void king_war_manager_t::on_long_occupied_timer_call_back(const boost::system::error_code& ec, uint64_t object_id)
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

	monster_ptr p_monster = monster_manager_t::find_monster(object_id);
	if (NULL == p_monster)
	{
		log_error("can not find p_monster[%lu]", object_id);
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

	p_long_monster->on_occupied_timer_call_back();
}
