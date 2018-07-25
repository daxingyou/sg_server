#include "family_war_long.hpp"
#include "config_mgr.h"
#include "tblh/Comprehensive.tbls.h"
#include "tblh/FightAgainst.tbls.h"
#include "tblh/MonsterHero.tbls.h"
#include "tblh/attr_enum.hpp"
#include "fight/fight_manager.hpp"

family_war_long_t::~family_war_long_t()
{

}

void family_war_long_t::get_object_info(proto::common::object_common_info* object_info)
{
	if (NULL == object_info)
	{
		log_error("object_info null error");
		return;
	}

	monster_t::get_object_info(object_info);

	if (m_object_state == proto::common::object_state_death)
	{
		proto::common::object_state_info* object_state = object_info->mutable_object_state();
		if (object_state != NULL)
		{
			object_state->set_alert_time(m_revive_time);
		}
	}

	object_info->set_current_hp(m_curr_hp);
	object_info->set_max_hp(m_max_hp);
	object_info->set_family_id(common::string_util_t::uint64_to_string(m_occupy_family));
}

bool family_war_long_t::init_long(uint32_t monster_tid, const monster_common::monster_define_t& define, Monster* monster)
{
	if (NULL == monster)
	{
		log_error("npc null error");
		return false;
	}

	// 基类
	if (!monster_t::init_monster(define, monster))
	{
		log_error("family_war_long[%u] null error", monster_tid);
		return false;
	}

	m_monster_tid = monster_tid;

	uint32_t against_id = monster->get_fight_id();
	if (against_id == 0)
	{
		log_error("family_war_long[%u] against_id not find", m_monster_tid);
		return false;
	}

	FightAgainst* against_ids = GET_CONF(FightAgainst, against_id);
	if (NULL == against_ids)
	{
		log_error("against_id[%u] null error", against_id);
		return false;
	}

	for (const auto& tt : against_ids->get_against_ids())
	{
		uint32_t id = tt.second;
		MonsterHero* hero = GET_CONF(MonsterHero, id);
		if (NULL == hero)
		{
			log_error("load MonsterHero[%u] error", id);
			continue;
		}

		uint32_t hp = hero->get_attr(attrtype_enum::max_hp);

		m_hero_map.insert(std::make_pair(tt.first, long_hero_info(id, hp, hp)));

		m_max_hp += hp;
	}

	m_curr_hp = m_max_hp;

	return true;
}

void family_war_long_t::set_family_war_family(family_war_side_type type, uint64_t family_id)
{
	if (type >= family_war_side_max)
	{
		log_error("family type[%u] error", type);
		return;
	}

	m_war_family[type] = family_id;
}

family_war_side_type family_war_long_t::get_family_war_side_type(uint64_t family_id)
{
	if (family_id == m_war_family[family_war_side_left])
	{
		return family_war_side_left;
	}
	else if (family_id == m_war_family[family_war_side_right])
	{
		return family_war_side_right;
	}

	return family_war_side_none;
}

bool family_war_long_t::get_hero_state_data(proto::common::hero_state_data* data)
{
	if (NULL == data)
	{
		log_error("hero_state_data null error!");
		return false;
	}

	if (m_hero_map.empty())
	{
		log_error("boos[%u] m_hero_map empty", get_monster_id());
		return false;
	}

	for (long_hero_map::iterator iter = m_hero_map.begin();
		iter != m_hero_map.end(); ++iter)
	{
		proto::common::hero_state* hero = data->add_states();
		if (NULL != hero)
		{
			hero->set_pos(iter->first);
			hero->set_hp(iter->second.m_curr_hp);

			//printf("\033[31m pos[%u] curr hp:%u max hp:%u \033[0m\n", iter->first, iter->second.m_curr_hp, iter->second.m_max_hp);
		}
	}

	return true;
}

void family_war_long_t::add_combat(uint64_t id)
{
	if (m_is_fight_end) return;

	m_fight_uids.insert(id);
}

void family_war_long_t::del_combat(uint64_t id)
{
	if (m_is_fight_end) return;

	m_fight_uids.erase(id);
}

void family_war_long_t::set_all_fight_failed()
{
	m_is_fight_end = true;

	// 所有战斗强制失败
	for (std::set<uint64_t>::const_iterator citr = m_fight_uids.begin();
		citr != m_fight_uids.end(); ++citr)
	{
		// 击杀场景内玩家
		combat_ptr p_combat = fight_manager_t::get_combat(*citr);
		if (p_combat != NULL)
		{
			p_combat->all_fail();
		}
		else
		{
			log_error("combat_ptr[%lu] not find when set_all_fight_failed", *citr);
		}
	}

	m_fight_uids.clear();
}

uint32_t family_war_long_t::update_hero_hp(family_war_side_type side_type, uint64_t fight_id, const proto::common::fight_camp& enemy_camp, const combat_ptr& p_combat)
{
	if (p_combat == NULL)
	{
		log_error("p_combat == NULL");
		return 0;
	}

	if (side_type >= family_war_side_max)
	{
		log_error("family_war_side_type[%u] error", side_type);
		return 0;
	}

	uint32_t hurts = 0;
	for (const proto::common::hero_single& hero : enemy_camp.heroes().hero_list())
	{
		long_hero_map::iterator itr = m_hero_map.find(hero.fight_attr().pos());
		if (itr != m_hero_map.end())
		{
			uint32_t hurt = p_combat->get_hero_injured(common::string_util_t::string_to_uint64(hero.base_attr().fight_hero_uid()));
			if (hurt < itr->second.m_curr_hp)
			{
				itr->second.m_curr_hp = itr->second.m_curr_hp - hurt;
				hurts += hurt;
			}
			else
			{
				hurts += itr->second.m_curr_hp;
				itr->second.m_curr_hp = 0;
			}
		}
	}

	if (hurts == 0)
		return 0;

	// 通知除自己之外的所有其他和boss战斗的人
	update_all_combat(fight_id);

	m_hurt_val[side_type] += hurts;	// 记录伤害

	if (m_curr_hp < hurts)
	{
		m_curr_hp = 0;
	}
	else
	{
		m_curr_hp -= hurts;
	}

	// 9屏
	notify_object_common_info_for_nearbies();

	return hurts;
}

void family_war_long_t::update_all_combat(uint64_t exception /*= 0*/)
{
	proto::common::fight_state fight_state;
	if (!get_hero_state_data(fight_state.mutable_enemy()))
	{
		log_error("get family long hero data error");
		return;
	}

	for (const auto& fight_id : m_fight_uids)
	{
		if (exception != 0 && fight_id == exception)
			continue;

		fight_manager_t::update_fight(fight_id, fight_state);
	}
}

void family_war_long_t::set_occupy_side(family_war_side_type side_type)
{
	if (side_type >= family_war_side_max)
	{
		log_error("family_war_side_type[%u] error", side_type);
		return;
	}

	set_all_fight_failed();

	m_occupy_family = m_war_family[side_type];

	notify_object_common_info_for_nearbies();
}

float family_war_long_t::get_hurt_pre(family_war_side_type side_type)
{
	if (side_type >= family_war_side_max)
	{
		log_error("family_war_side_type[%u] error", side_type);
		return 0.f;
	}

	if (m_max_hp == 0)
	{
		log_error("family_war_long max_hp == 0 error");
		return 0.f;
	}

	return (float)m_hurt_val[side_type] / (float)m_max_hp;
}

void family_war_long_t::set_long_revive(uint32_t count_down)
{
	m_revive_time = common::time_util_t::now_time() + count_down;

	set_object_die();
}

void family_war_long_t::peek_long_hurt_info(proto::client::gc_long_hurt_info_reply& reply)
{
	reply.set_max_hp(m_max_hp);

	proto::client::long_hurt_info_data* left_data = reply.mutable_left_side();
	if (left_data)
	{
		left_data->set_family_id(common::string_util_t::uint64_to_string(m_war_family[family_war_side_left]));
		left_data->set_hurt(m_hurt_val[family_war_side_left]);
	}

	proto::client::long_hurt_info_data* right_data = reply.mutable_right_side();
	if (right_data)
	{
		right_data->set_family_id(common::string_util_t::uint64_to_string(m_war_family[family_war_side_right]));
		right_data->set_hurt(m_hurt_val[family_war_side_right]);
	}
}

family_war_side_type family_war_long_t::get_win_side()
{
	if (m_hurt_val[family_war_side_left] > m_hurt_val[family_war_side_right])
	{
		return family_war_side_left;
	}
	
	if (m_hurt_val[family_war_side_left] < m_hurt_val[family_war_side_right])
	{
		return family_war_side_right;
	}
	
	if (m_war_family[family_war_side_left] > m_war_family[family_war_side_right])
	{
		return family_war_side_left;
	}

	if (m_war_family[family_war_side_left] < m_war_family[family_war_side_right])
	{
		return family_war_side_right;
	}

	return family_war_side_none;
}
