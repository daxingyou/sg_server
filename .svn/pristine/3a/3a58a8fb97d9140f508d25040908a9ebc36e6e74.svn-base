#include "hero.hpp"
#include "cache_key.hpp"
#include "role/role_unify_save.hpp"
#include "protos_fwd.hpp"
#include "formation/formation.hpp"
#include "config/config_manager.hpp"
#include "fight/fight_manager.hpp"
#include "equip/equip_manager.hpp"
#include "tblh/EquipSuit.tbls.h"
#include "tblh/EquipSpecial.tbls.h"
#include "tblh/EquipStrengthenMaster.tbls.h"
#include "tblh/SkillScore.tbls.h"
#include "tblh/TalentScore.tbls.h"
#include "tblh/HeroStar.tbls.h"
#include "tblh/Hero.tbls.h"
#include "tblh/attr_enum.hpp"
#include "tblh/HeroPlugin.tbls.h"
#include "achieve/achieve_common.hpp"
hero_t::hero_t(uint64_t role_uid)
{
	m_role_uid = role_uid;
}

hero_t::hero_t(uint32_t hero_id, uint64_t unique_id, uint64_t role_uid) : object_attr_t(hero_id)
{
	m_role_uid = role_uid;
	m_hero_id = hero_id;
	set_unique_id(unique_id);
	set_key(cache_key_t::create(unique_id, cache_name::heros));
}

hero_t::hero_t()
{

}

hero_t::~hero_t()
{
}

void hero_t::load_data(const proto::common::hero_single& single)
{
	const proto::common::base_attr& ba = single.base_attr();
	set_base_attr(single.base_attr());

	m_hero_id = single.base_attr().id();
	for (int32_t i = 0; i < ba.equips_size(); ++i)
	{
		const proto::common::hero_equip& he = ba.equips(i);
		uint32_t pos = he.pos();
		uint64_t equip_uid = string_util_t::string_to_uint64(he.equip_uid());
		m_equip[pos] = equip_uid;
	}
	for (int32_t i = 0; i < ba.fate_info_size(); ++i)
	{
		const proto::common::fate_pair& fp = ba.fate_info(i);
		uint32_t fate_id = fp.fate_tid();
		int32_t fate_level = fp.fate_level();
		m_fate_map.insert(std::make_pair(fate_id, fate_level));
	}
	set_key(cache_key_t::create(get_unique_id(), cache_name::heros));
	load_talent_data(single.talent_data());
//	calc_fighting();	wys 注释 2017.6.15 放在这有漏洞 此时英雄还没有添加到容器中 无法更新战斗力
}

void hero_t::peek_data(proto::common::hero_single* p_single, bool fight)
{
	if (NULL == p_single)
	{
		log_error("NULL == p_single");
		return;
	}
	
	proto::common::base_attr* p_base_attr = p_single->mutable_base_attr();
	for (uint32_t i = 0; i < equip_pos_end; ++i)
	{
		if (m_equip[i] > 0)
		{
			proto::common::hero_equip* p_hero_equip = p_base_attr->add_equips();
			p_hero_equip->set_pos(i);
			p_hero_equip->set_equip_uid(string_util_t::uint64_to_string(m_equip[i]));
		}
	}
	for (auto it : m_fate_map)
	{
		proto::common::fate_pair* p_fate_pair = p_base_attr->add_fate_info();
		p_fate_pair->set_fate_tid(it.first);
		p_fate_pair->set_fate_level(it.second);
	}
	//HERO_LOG("fate_num[%d]", get_base_attr().fate_info_size());
	peek_base_attr(p_base_attr);
	
	//p_single->mutable_fight_attr()->CopyFrom(get_fight_attr());
	peek_talent_data(p_single->mutable_talent_data());

	attr_map attrs;
	fight_manager_t::calc_attr(shared_from_this(), attrs, p_single, fight);
	fight_manager_t::fill_hero_skill(shared_from_this(), p_single);
	p_single->mutable_fight_attr()->set_fighting(m_fighting);
}

void hero_t::save_self(uint64_t role_uid, bool is_new/* = false*/)
{
	calc_fighting();
	
	proto::common::hero_single single;
	peek_data(&single);
	role_unify_save::add_task(get_key(), single, is_new, false, role_uid);
}

void hero_t::add_talent(talent_ptr p_talent)
{
	if (NULL == p_talent)
	{
		log_error("hero[%lu] NULL == p_talent", get_unique_id());
		return;
	}
	uint32_t tid = p_talent->get_tid();
	if (m_talent_map.find(tid) != m_talent_map.end())
	{
		log_error("hero[%lu] add talent[%d] exist!!", get_unique_id(), tid);
		return;
	}
	m_talent_map.insert(std::pair<uint32_t, talent_ptr>(tid, p_talent));
}

talent_ptr hero_t::add_talent(uint32_t tid, uint32_t level, uint32_t wake)
{
	talent_ptr p_talent(new talent_t(tid, level, wake));
	add_talent(p_talent);
	return p_talent;
}

talent_ptr hero_t::get_talent(uint32_t tid)
{
	talent_map::iterator it = m_talent_map.find(tid);
	if (it == m_talent_map.end())
	{
		return talent_ptr(NULL);
	}
	return it->second;
}

void hero_t::reset_talent()
{
	talent_map::iterator it = m_talent_map.begin();
	talent_map::iterator it_end = m_talent_map.end();
	talent_ptr p_talent;
	for (; it != it_end; ++it)
	{
		p_talent = it->second;
		if (NULL != p_talent)
		{
			p_talent->set_level(0);
		}
	}
}

void hero_t::add_fate(uint32_t fate_id)
{
	//HERO_LOG("hero[%d] add_fate[%d]!!", m_hero_id, fate_id);

	if (m_fate_map.find(fate_id) != m_fate_map.end())
	{
		log_error("fate_id[%d] already exist!!", fate_id);
		return;
	}
	
	m_fate_map.insert(std::make_pair(fate_id, -1));
}

bool hero_t::has_fate(uint32_t fate_id, int32_t& fate_level)
{
	auto it = m_fate_map.find(fate_id);
	if (it != m_fate_map.end())
	{
		fate_level = it->second;
		return true;
	}
	return false;
}

void hero_t::levelup_fate(uint32_t fate_id, int32_t fate_level)
{
	auto it = m_fate_map.find(fate_id);
	if (it != m_fate_map.end())
	{
		it->second = fate_level;
	}
}

void hero_t::load_talent_data(const proto::common::talent_data& td)
{
	for (uint16_t i = 0; i < td.talent_list_size(); ++i)
	{
		const proto::common::talent_single& ts = td.talent_list(i);
		talent_ptr p_talent(new talent_t(ts.tid(), ts.level(), ts.wake()));
		add_talent(p_talent);
	}
}

void hero_t::peek_talent_data(proto::common::talent_data* p_data)
{
	if (NULL == p_data)
	{
		log_error("hero[%lu] NULL == p_data", get_unique_id());
		return;
	}
	talent_map::iterator it = m_talent_map.begin();
	talent_map::iterator it_end = m_talent_map.end();
	talent_ptr p_talent;
	for (; it != it_end; ++it)
	{
		p_talent = it->second;
		if (NULL != p_talent)
		{
			proto::common::talent_single* p_single = p_data->add_talent_list();
			p_single->set_tid(p_talent->get_tid());
			p_single->set_level(p_talent->get_level());
			p_single->set_wake(p_talent->get_wake());
		}
	}
}

int hero_t::get_attr(uint32_t type)
{
	attr_map attrs;
	fight_manager_t::calc_attr(shared_from_this(), attrs, NULL);

	if (attrs.find(type) == attrs.end())
	{
		return 0;
	}

	return attrs[type];
}

void hero_t::equip_on(uint32_t pos, uint64_t equip_id)
{
	if (pos >= equip_pos_end)
	{
		log_error("invalid pos");
		return;
	}
	m_equip[pos] = equip_id;
}

void hero_t::equip_off(uint32_t pos)
{
	if (pos >= equip_pos_end)
	{
		log_error("invalid pos");
		return;
	}
	m_equip[pos] = 0;
}

uint64_t hero_t::get_equip(uint32_t pos)
{
	if (pos >= equip_pos_end)
	{
		log_error("invalid pos");
		return 0;
	}
	return m_equip[pos];
}

void hero_t::calc_equip_attr(attr_map& attrs)
{
	role_ptr p_role = role_manager_t::find_role(m_role_uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role");
		return;
	}
	m_equip_skill_vec.clear();
	std::map<uint32_t, uint32_t> suit_map;
	std::map<uint32_t, uint32_t> special_map;
	std::vector<uint32_t> strmaster_vec;
	for (uint32_t i = 0; i < equip_pos_end; ++i)
	{
		uint64_t equip_uid = m_equip[i];
		if (equip_uid == 0)
		{
			continue;
		}
		item_ptr p_equip = equip_manager_t::get_equip_by_uid(p_role, equip_uid);
		if (NULL == p_equip)
		{
			log_error("NULL == p_equip[%llu]", equip_uid);
			continue;
		}
		p_equip->calc_attr(attrs);
		//for (auto it : attrs)
		//{
		//	log_warn("attr[%d:%d]", it.first, it.second);
		//}
		auto p_equip_conf = GET_CONF(Equip, p_equip->get_tid());
		if (NULL == p_equip_conf)
		{
			log_error("NULL == p_equip_conf[%d]", p_equip->get_tid());
			continue;
		}
		uint32_t suit_id = p_equip_conf->suit_id();
		if (suit_id > 0)
		{
			suit_map[suit_id] += 1;
		}
		if (p_equip->get_special() > 0)
		{
			special_map[p_equip->get_special()] += 1;
		}
		if (p_equip->get_strmaster_level() > 0)
		{
			strmaster_vec.push_back(p_equip->get_strmaster_level());
		}
	}
	//套装属性
	for (auto it : suit_map)
	{
		uint32_t suit_id = it.first;
		uint32_t suit_count = it.second;
		EQUIP_LOG("suit_id[%d] count[%d]", suit_id, suit_count);
		auto p_equip_suit_conf = GET_CONF(EquipSuit, suit_id);
		if (NULL == p_equip_suit_conf)
		{
			log_error("NULL == p_equip_suit_conf[%d]", suit_id);
			continue;
		}
		//两件套属性
		if (suit_count >= 2)
		{
			for (auto attr : p_equip_suit_conf->two_attr())
			{
				uint32_t attr_type = attr.first;
				uint32_t attr_value = attr.second;
				attrs[attr_type] += attr_value;
				EQUIP_LOG("suit_id[%d] two_attr[%d:%d]", suit_id, attr_type, attr_value);
			}
		}
		//三件套技能
		if (suit_count >= 3)
		{
			for (auto skill_id : p_equip_suit_conf->three_attr())
			{
				m_equip_skill_vec.push_back(skill_id);
				EQUIP_LOG("suit_id[%d] three_attr skill[%d]", suit_id, skill_id);
			}
		}
		//achieve_manager_t::notify_achieve_state(m_role_uid, Achieve_Event_Type::ACHIEVE_EQUIP_TYPE_SAME, suit_id);
	}
	//for (auto it : attrs)
	//{
	//	log_warn("attr[%d:%d]", it.first, it.second);
	//}
	//特技属性
	for (auto it : special_map)
	{
		uint32_t special_id = it.first;
		uint32_t special_count = it.second;
		if (special_count >= 1)
		{
			auto p_equip_special_conf = GET_CONF(EquipSpecial, special_id);
			if (NULL == p_equip_special_conf)
			{
				log_error("NULL == p_equip_special_conf[%d]", special_id);
				continue;
			}
			for (auto skill_id : p_equip_special_conf->attr())
			{
				m_equip_skill_vec.push_back(skill_id);
				EQUIP_LOG("special_id[%d] attr skill[%d]", special_id, skill_id);
			}
		}
	}
	//强化大师属性
	if (strmaster_vec.size() >= 3)
	{
		uint32_t min_strmaster_level = strmaster_vec[0];
		for (auto strmaster_level : strmaster_vec)
		{
			if (strmaster_level < min_strmaster_level)
			{
				min_strmaster_level = strmaster_level;
			}
		}
		auto p_equip_strengthen_master_conf = GET_CONF(EquipStrengthenMaster, min_strmaster_level);
		if (NULL == p_equip_strengthen_master_conf)
		{
			log_error("NULL == p_equip_strengthen_master_conf[%d]", min_strmaster_level);
			return;
		}
		
		for (auto attr : p_equip_strengthen_master_conf->award())
		{
			uint32_t attr_type = attr.first;
			uint32_t attr_value = attr.second;
			attrs[attr_type] += attr_value;
			EQUIP_LOG("strmaster_level[%d] attr[%d:%d]", min_strmaster_level, attr_type, attr_value);
		}
		
	}
}

uint32_t hero_t::get_skill_power()
{
	uint32_t skill_power = 0;
	// 读星级表
	Hero* hero_conf = GET_CONF(Hero, get_tid());
	if (hero_conf == NULL)
	{
		log_error("Hero [%u] not exists", get_tid());
		return 0;
	}

	HeroSkillMap::iterator iter = m_hero_skills.begin();
	for (; iter != m_hero_skills.end(); ++iter)
	{
		if(iter->second.skill_pos == 0)
			continue;
		SkillScore* p_skill_score = GET_CONF(SkillScore, hash_util_t::hash_to_uint32(iter->first, iter->second.skill_level));
		if (p_skill_score == NULL)
		{
			log_error("p_skill_score == NULL [%d:%d]", iter->first, iter->second.skill_level);
			return 0;
		}
		skill_power += p_skill_score->score();
	}

	//装备技能
	for (auto skill_id : m_equip_skill_vec)
	{
		SkillScore* p_skill_score = GET_CONF(SkillScore, hash_util_t::hash_to_uint32(skill_id, 1));
		if (p_skill_score == NULL)
		{
			log_error("p_skill_score == NULL [%d:%d]", skill_id, 1);
			return 0;
		}
		skill_power += p_skill_score->score();
		//EQUIP_LOG("equip_skill[%d] power[%d]", skill_id, p_skill_score->score());
	}
	return skill_power;
}

uint32_t hero_t::get_talent_power()
{
	uint32_t talent_power = 0;
	for (auto it : m_talent_map)
	{
		auto p_talent = it.second;
		if (NULL == p_talent)
		{
			continue;
		}
		auto p_talent_score = GET_CONF(TalentScore, hash_util_t::hash_to_uint32(p_talent->get_tid(), p_talent->get_level()));
		if (NULL == p_talent_score)
		{
			continue;
		}
		talent_power += p_talent_score->score();
	}
	return talent_power;
}

void hero_t::calc_fighting()
{
	attr_map attrs;
	attr_map add_attr;
	attr_map percent_attr;
	calc_equip_attr(add_attr);
	calc_plugin_attr(add_attr);
	calc_role_buff_attr(percent_attr);
	fight_manager_t::calc_attr(add_attr, percent_attr, attrs, get_tid(), get_current_level(), get_grade(), get_star(), get_wakeup(), m_role_uid);
	//for (auto it : attrs)
	//{
	//	log_warn("attr[%d:%d]", it.first, it.second);
	//}
	//有效生命
	uint32_t hp_power = item_t::get_hp_score(attrs);
	//CULTI_LOG("hp_power[%d]", hp_power);
	uint32_t atk_power = 0;
	//有效输出
	if (m_equip[equip_pos_weapon] > 0)
	{
		//有武器的情况下
		atk_power = item_t::get_atk_score(attrs);
	}
	else
	{
		//没有武器的情况下
		atk_power = item_t::get_atk_score_without_weapon(attrs);
	}
	//CULTI_LOG("atk_power[%d]", atk_power);
	//技能
	uint32_t skill_power = get_skill_power();
	//CULTI_LOG("skill_power[%d]", skill_power);
	//天赋
	uint32_t talent_power = get_talent_power();
	//CULTI_LOG("talent_power[%d]", talent_power);
	//先攻值评分
	uint32_t speed_power = attrs[attrtype_enum::speed] * item_t::get_speed_switch_power();
	//CULTI_LOG("speed_power[%d]", speed_power);
	//计算战力
	float hp_val = sqrt(item_t::get_hp_effective() * hp_power);
	//CULTI_LOG("hp_val[%.3f]", hp_val);
	float atk_val = sqrt(item_t::get_ad_effective() * atk_power);
	//CULTI_LOG("atk_val[%.3f]", atk_val);
	m_fighting = hp_val * atk_val + skill_power + talent_power + speed_power;
	//CULTI_LOG("m_fighting[%d]", m_fighting);

	role_ptr p_role = role_manager_t::find_role(m_role_uid);
	if (NULL != p_role)
	{
		p_role->check_update_fighting(get_unique_id());
	}
	if (get_tid() == p_role->get_main_hero_tid())
	{
		achieve_common_t::notify_progress_state(m_role_uid, proto::common::ACHIEVE_MAIN_ROLE_FIGHT,0, m_fighting);
	}
}

uint32_t hero_t::get_equip_count()
{
	uint32_t count = 0;
	for (uint32_t i = 0; i < equip_pos_end; ++i)
	{
		if (m_equip[i] > 0)
		{
			++count;
		}
	}
	return count;
}

bool hero_t::find_hero_skill(uint32_t skill_id, hero_skill& hero_skill_data)
{
	HeroSkillMap::iterator iter = m_hero_skills.find(skill_id);
	if (iter == m_hero_skills.end())
		return false;

	hero_skill_data = iter->second;
	return true;
}

bool hero_t::set_hero_skill_level(uint32_t skill_id, uint32_t level)
{
	HeroSkillMap::iterator iter = m_hero_skills.find(skill_id);
	if (iter == m_hero_skills.end())
	{
		log_error("not find skill[%d]", skill_id);
		return false;
	}

	iter->second.skill_level = level;
	return true;
}

bool hero_t::set_hero_skill_pos(uint32_t skill_id, uint32_t pos)
{
	HeroSkillMap::iterator iter = m_hero_skills.find(skill_id);
	if (iter == m_hero_skills.end())
	{
		log_error("not find skill[%d]", skill_id);
		return false;
	}

	iter->second.skill_pos = pos;
	return true;
}

bool hero_t::add_hero_skill(uint32_t skill_id, uint32_t level)
{
	if (m_hero_skills.find(skill_id) != m_hero_skills.end())
	{
		log_error("skill[%d] repeat", skill_id);
		return false;
	}
	hero_skill skill_info;
	skill_info.skill_id = skill_id;
	skill_info.skill_level = level;
	m_hero_skills.insert(std::make_pair(skill_id, skill_info));
	return true;
}

bool hero_t::remove_hero_skill(uint32_t skill_id)
{
	HeroSkillMap::iterator iter = m_hero_skills.find(skill_id);
	if (iter == m_hero_skills.end())
	{
		log_error("not find skill[%d]", skill_id);
		return false;
	}
	m_hero_skills.erase(iter);

	return true;
}

bool hero_t::replace_hero_skill(uint32_t old_skill_id, uint32_t new_skill_id)
{
	hero_skill old_hero_skill_data;	
	find_hero_skill(old_skill_id, old_hero_skill_data);
	if (remove_hero_skill(old_skill_id))
	{
		add_hero_skill(new_skill_id);
		set_hero_skill_pos(new_skill_id, old_hero_skill_data.skill_pos);
		return true;
	}
	return false;
}

void hero_t::get_fight_skill_list(std::vector<hero_skill>& skill_vec)
{
	skill_vec.clear();
	
	HeroSkillMap::iterator iter = m_hero_skills.begin();
	for (; iter != m_hero_skills.end(); ++iter)
	{
		if(iter->second.skill_pos == 0)
			continue;
		skill_vec.push_back(iter->second);
	}

	//装备技能
	for (auto skill_id : m_equip_skill_vec)
	{
		hero_skill hs;
		hs.skill_id = skill_id;
		hs.skill_level = 1;
//		hs.skill_source = proto::common::skill_type_equip_skill;
		skill_vec.push_back(hs);
	}
}

void hero_t::calc_plugin_attr(attr_map& attrs)
{
	const std::vector<uint32_t>& plugin_list = get_plugin_list();
	for (auto& it : plugin_list)
	{
		HeroPlugin* p_config = GET_CONF(HeroPlugin, it);
		if (NULL == p_config)
		{
			log_error("NULL == p_config plugin_id[%d]", it);
			continue;
		}
		const std::map<uint32_t, uint32_t>& attr_list = p_config->attr();
		if (attr_list.empty())
		{
			log_error("attr_list.empty() plugin_id[%d]", it);
			continue;
		}
		for (const auto& att : attr_list)
		{
			attrs[att.first] += att.second;
			//HERO_LOG("role[%lu] hero[%d] plugin[%d] type[%d] add_value[%d] value[%d]",m_role_uid,m_hero_id,it, att.first,att.second ,attrs[att.first]);
		}
	}
}

void hero_t::calc_role_buff_attr(attr_map& attrs)
{
	for (auto& it : m_add_attrs)
	{
		attrs[it.first] += it.second;
	}
}
