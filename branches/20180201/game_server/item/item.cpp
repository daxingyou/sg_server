#include <boost/lexical_cast.hpp>

#include "item.hpp"
#include "cache_key.hpp"
#include "role/role_unify_save.hpp"
#include "item_manager.hpp"
#include "tblh/Item.tbls.h"
#include "tblh/Equip.tbls.h"
#include "tblh/EquipExattr.tbls.h"
#include "tblh/EquipAttrScore.tbls.h"
#include "tblh/EquipName.tbls.h"
#include "tblh/EquipSpecial.tbls.h"
#include "tblh/EquipSuit.tbls.h"
#include "tblh/SkillScore.tbls.h"
#include "tblh/attr_enum.hpp"
#include "tblh/Comprehensive.tbls.h"

item_t::item_t()
{

}

item_t::item_t(uint64_t uid, uint32_t tid, int32_t cur_num, const std::string& role_name, uint32_t param)
    : m_uid(uid) 
    , m_tid(tid)
    , m_cur_num(cur_num)
	, m_find_role(role_name)
	, m_param(param)
{
    m_key = cache_key_t::create(m_uid, cache_name::items);
}

item_t::~item_t()
{

}

void item_t::load_data(const proto::common::item_single& single)
{
    m_uid = string_util_t::string_to_uint64(single.uid());
    m_tid = single.tid();
    m_cur_num = single.num();
    m_key = cache_key_t::create(m_uid, cache_name::items);
	m_package = single.package();
	m_param = single.param();
	m_param2 = single.param2();
	m_find_role = single.find_role();
	m_state = single.state();
	m_expired_time = single.expired_time();
	check_state();
	
	if (is_equip() && single.has_equip_info())
	{
		const proto::common::equip_single& es = single.equip_info();
		for (int32_t i = 0; i < es.attr_size(); ++i)
		{
			const proto::common::equip_attr& ea = es.attr(i);
			uint32_t attr_type = ea.type();
			uint32_t attr_value = ea.value();
			m_attrs.insert(std::make_pair(attr_type, attr_value));
		}
		for (int32_t i = 0; i < es.exattr_size(); ++i)
		{
			m_exattrs.push_back(es.exattr(i));
		}
		for (int32_t i = 0; i < es.remake_exattr_size(); ++i)
		{
			m_remake_exattrs.push_back(es.remake_exattr(i));
		}
		m_remake_exattr_bonus = es.remake_exattr_bonus();
		m_hero_uid = string_util_t::string_to_uint64(es.hero_uid());
		m_score = es.score();
		m_name = es.name();
		m_special = es.special();
		m_strengthen_level = es.strengthen_level();
		m_strmaster_level = es.strmaster_level();
		m_remake_material = es.remake_material();
		m_lock_material = es.lock_material();
		m_strengthen_material = es.strengthen_material();
		m_strengthen_material_ex = es.strengthen_material_ex();
		m_strengthen_money = es.strengthen_money();
		calc_score();
		calc_remake_score();
	}
	check_rarity();
}

void item_t::peek_data(proto::common::item_single* p_single)
{
	check_state();
	if (p_single)
	{
		p_single->set_uid(string_util_t::uint64_to_string(m_uid));
		p_single->set_tid(m_tid);
		p_single->set_num(m_cur_num);
		p_single->set_package(m_package);
		p_single->set_param(m_param);
		p_single->set_param2(m_param2);
		p_single->set_find_role(m_find_role);
		p_single->set_rarity(m_rarity);
		p_single->set_state(m_state);
		p_single->set_expired_time(m_expired_time);
		if (is_equip())
		{
			proto::common::equip_single* p_equip = p_single->mutable_equip_info();
			for (auto it : m_attrs)
			{
				proto::common::equip_attr* p_equip_attr = p_equip->add_attr();
				p_equip_attr->set_type(it.first);
				p_equip_attr->set_value(it.second);
			}
			for (auto exattr : m_exattrs)
			{
				p_equip->add_exattr(exattr);
			}
			for (auto exattr : m_remake_exattrs)
			{
				p_equip->add_remake_exattr(exattr);
			}
			p_equip->set_hero_uid(string_util_t::uint64_to_string(m_hero_uid));
			p_equip->set_remake_exattr_bonus(m_remake_exattr_bonus);
			p_equip->set_score(m_score);
			p_equip->set_remake_score(m_remake_score);
			p_equip->set_name(m_name);
			p_equip->set_special(m_special);
			p_equip->set_strengthen_level(m_strengthen_level);
			p_equip->set_strmaster_level(m_strmaster_level);
			p_equip->set_remake_material(m_remake_material);
			p_equip->set_lock_material(m_lock_material);
			p_equip->set_strengthen_material(m_strengthen_material);
			p_equip->set_strengthen_material_ex(m_strengthen_material_ex);
			p_equip->set_strengthen_money(m_strengthen_money);
		}
	}
}

void item_t::save_self(uint64_t role_uid, bool is_new, bool is_right_now)
{
    proto::common::item_single single;
	peek_data(&single);
	role_unify_save::add_task(m_key, single, is_new, is_right_now, role_uid);
}

uint32_t item_t::get_need_grid()
{
	Item *p_item_config = GET_CONF(Item, get_tid());
	if (NULL == p_item_config)
	{
		log_error("item p_item_config NULL tid[%d]", get_tid());
		return 0;
	}

	uint32_t overlap = p_item_config->overlap_count();
	overlap = overlap == 0 ? 1 : overlap;

	uint32_t grid = get_cur_num() / overlap;
	uint32_t left = get_cur_num() % overlap;

	if (left > 0)
	{
		grid += 1;
	}

	return grid;
}


void item_t::set_attr(const std::map<uint32_t, uint32_t>& attr_map)
{
	m_attrs.clear();
	m_attrs = attr_map;
}

void item_t::set_exattr(const std::vector<uint32_t>& exattr_vec)
{
	m_exattrs.clear();
	m_exattrs = exattr_vec;
}

uint32_t item_t::get_exattr(uint32_t index)
{
	if (index >= m_exattrs.size())
	{
		return 0;
	}
	return m_exattrs.at(index);
}

void item_t::set_remake_exattr(const std::vector<uint32_t>& exattr_vec)
{
	m_remake_exattrs.clear();
	m_remake_exattrs = exattr_vec;
	calc_remake_score();
}

void item_t::replace_exattr()
{
	m_exattrs.clear();
	m_exattrs = m_remake_exattrs;
	m_remake_exattrs.clear();
	calc_remake_score();
}

void item_t::calc_score()
{
// 老的装备评分
// 	auto p_equip_conf = GET_CONF(Equip, m_tid);
// 	if (NULL == p_equip_conf)
// 	{
// 		log_error("NULL == p_equip_conf[%d]", m_tid);
// 		return;
// 	}
// 	uint32_t score = 0;
// 	std::map<uint32_t, int> attrs;
// 	calc_attr(attrs);
// 	//for (auto it : attrs)
// 	//{
// 	//	log_warn("attr[%d:%d]", it.first, it.second);
// 	//}
// 	uint32_t special_score = calc_special_score();
// 	//EQUIP_LOG("special_score[%d]", special_score);
// 	uint32_t suit_score = calc_suit_score();
// 	//EQUIP_LOG("suit_score[%d]", suit_score);
// 	uint32_t speed_score = attrs[attrtype_enum::speed] * get_speed_switch_power();
// 	//EQUIP_LOG("speed_score[%d]", speed_score);
// 	uint32_t atk_score = get_atk_score(attrs);
// 	//EQUIP_LOG("atk_score[%d]", atk_score);
// 	uint32_t hp_score = get_hp_score(attrs);
// 	//EQUIP_LOG("hp_score[%d]", hp_score);
// 	score = atk_score + hp_score + special_score + suit_score + speed_score;
// 	//EQUIP_LOG("score[%d]", score);
// 	set_score(score);

	/* 新装备评分
	装备总评分=装备评分*（100+装备强化评分系数*强化等级）/ 100+
	词缀评分*（100+词缀强化评分系数*向下取整（强化等级/5））/100+
	判词评分（有几条判词就加几次）+
	特技评分*/

	Equip* p_equip_conf = GET_CONF(Equip, m_tid);
	if (!p_equip_conf) {
		log_error("NULL == p_equip_conf[%d]", m_tid);
		return;
	}

	auto p_conf = GET_CONF(Comprehensive, comprehensive_common::equipscore_1);
	uint32_t factor1 = GET_COMPREHENSIVE_VALUE_1(p_conf);

	p_conf = GET_CONF(Comprehensive, comprehensive_common::equipscore_2);
	uint32_t factor2 = GET_COMPREHENSIVE_VALUE_1(p_conf);

	uint32_t base_score = p_equip_conf->score() * (100 + factor1 * m_strengthen_level) / 100;

	uint32_t name_score = 0;
	if (m_name > 0) {
		EquipName* p_equip_name_conf = GET_CONF(EquipName, m_name);
		if (p_equip_name_conf) {
			name_score = p_equip_name_conf->score() * (100 + factor2 * (m_strengthen_level / 5)) / 100;
		}
	}

	uint32_t exattr_score = 0;
	if (!m_exattrs.empty()) {
		for (auto exattr_id : m_exattrs) {
			EquipExattr* p_equip_exattr_conf = GET_CONF(EquipExattr, exattr_id);
			if (p_equip_exattr_conf) {
				exattr_score += p_equip_exattr_conf->score();
			}
		}
	}

	uint32_t special_score = 0;
	EquipSpecial* p_special_conf = GET_CONF(EquipSpecial, m_special);
	if (p_special_conf) {
		special_score = p_special_conf->score();
	}

	uint32_t score = base_score + name_score + exattr_score + special_score;
	set_score(score);
}

void item_t::calc_remake_score()
{
// 	auto p_equip_conf = GET_CONF(Equip, m_tid);
// 	if (NULL == p_equip_conf)
// 	{
// 		log_error("NULL == p_equip_conf[%d]", m_tid);
// 		return;
// 	}
// 	if (m_remake_exattrs.empty())
// 	{
// 		set_remake_score(m_score);
// 		return;
// 	}
// 	uint32_t score = 0;
// 	std::map<uint32_t, int> attrs;
// 	calc_remake_attr(attrs);
// 
// 	uint32_t special_score = calc_special_score();
// 	EQUIP_LOG("special_score[%d]", special_score);
// 	uint32_t suit_score = calc_suit_score();
// 	EQUIP_LOG("suit_score[%d]", suit_score);
// 	//先攻值评分
// 	uint32_t speed_score = attrs[attrtype_enum::speed] * get_speed_switch_power();
// 	EQUIP_LOG("speed_score[%d]", speed_score);
// 	uint32_t base_score = 0;
// 	switch (p_equip_conf->equip_type())
// 	{
// 	case equip_pos_weapon:
// 	{
// 		base_score = get_atk_score(attrs);
// 	}
// 	break;
// 	//护甲和护符规则同，不是写错了
// 	case equip_pos_armor:
// 	case equip_pos_talisman:
// 	{
// 		base_score = get_hp_score(attrs);
// 	}
// 	break;
// 	default:
// 		break;
// 	}
// 	score = base_score + special_score + suit_score + speed_score;
// 	EQUIP_LOG("score[%d]", score);
// 	set_remake_score(score);

	/* 新装备评分
	装备总评分=装备评分*（100+装备强化评分系数*强化等级）/ 100+
	词缀评分*（100+词缀强化评分系数*向下取整（强化等级/5））/100+
	判词评分（有几条判词就加几次）+
	特技评分*/

	Equip* p_equip_conf = GET_CONF(Equip, m_tid);
	if (!p_equip_conf) {
		log_error("NULL == p_equip_conf[%d]", m_tid);
		return;
	}

	if (m_remake_exattrs.empty())
	{
		set_remake_score(m_score);
		return;
	}

	auto p_conf = GET_CONF(Comprehensive, comprehensive_common::equipscore_1);
	uint32_t factor1 = GET_COMPREHENSIVE_VALUE_1(p_conf);

	p_conf = GET_CONF(Comprehensive, comprehensive_common::equipscore_2);
	uint32_t factor2 = GET_COMPREHENSIVE_VALUE_1(p_conf);

	uint32_t base_score = p_equip_conf->score() * (100 + factor1 * m_strengthen_level) / 100;

	uint32_t name_score = 0;
	if (m_name > 0) {
		EquipName* p_equip_name_conf = GET_CONF(EquipName, m_name);
		if (p_equip_name_conf) {
			name_score = p_equip_name_conf->score() * (100 + factor2 * (m_strengthen_level / 5)) / 100;
		}
	}

	uint32_t exattr_score = 0;
	if (!m_exattrs.empty()) {
		for (auto exattr_id : m_remake_exattrs) {
			EquipExattr* p_equip_exattr_conf = GET_CONF(EquipExattr, exattr_id);
			if (p_equip_exattr_conf) {
				exattr_score += p_equip_exattr_conf->score();
			}
		}
	}

	uint32_t special_score = 0;
	EquipSpecial* p_special_conf = GET_CONF(EquipSpecial, m_special);
	if (p_special_conf) {
		special_score = p_special_conf->score();
	}

	uint32_t score = base_score + name_score + exattr_score + special_score;
	set_remake_score(score);
}

bool item_t::is_equip()
{
	return m_tid / 10000 == equip_prefix;
}

void item_t::calc_attr(std::map<uint32_t, int>& attrs)
{
	//基础属性
	for (auto it : m_attrs)
	{
		uint32_t attr_type = it.first;
		uint32_t attr_value = it.second;
		attrs[attr_type] += attr_value;
	}
	//附加属性
	for (auto exattr_id : m_exattrs)
	{
		auto p_equip_exattr_conf = GET_CONF(EquipExattr, exattr_id);
		if (NULL == p_equip_exattr_conf)
		{
			log_error("NULL == p_equip_exattr_conf[%d]", exattr_id);
			continue;
		}
		uint32_t attr_type = p_equip_exattr_conf->attr_type();
		uint32_t attr_value = p_equip_exattr_conf->attr_value();
		attrs[attr_type] += attr_value;
	}
	//强化属性
	if (m_strengthen_level > 0)
	{
		auto p_equip_conf = GET_CONF(Equip, m_tid);
		if (NULL == p_equip_conf)
		{
			log_error("NULL == p_equip_conf[%d]", m_tid);
			return;
		}
		for (auto it : p_equip_conf->attr_growth())
		{
			uint32_t attr_type = it.first;
			uint32_t attr_value = it.second;
			attrs[attr_type] += attr_value * m_strengthen_level/10000;
		}
	}
	//词缀属性
	if (m_name > 0)
	{
		auto p_equip_name_conf = GET_CONF(EquipName, m_name);
		if (NULL == p_equip_name_conf)
		{
			log_error("NULL == p_equip_name_conf[%d]", m_name);
			return;
		}
		for (auto it : p_equip_name_conf->attr())
		{
			uint32_t attr_type = it.first;
			uint32_t base_value = it.second;
			uint32_t growth_value = it.third;
			attrs[attr_type] += base_value + growth_value * (m_strengthen_level/ p_equip_name_conf->level());
		}
	}
}

void item_t::calc_remake_attr(std::map<uint32_t, int>& attrs)
{
	//基础属性
	for (auto it : m_attrs)
	{
		uint32_t attr_type = it.first;
		uint32_t attr_value = it.second;
		attrs[attr_type] += attr_value;
	}
	//附加属性
	for (auto exattr_id : m_remake_exattrs)
	{
		auto p_equip_exattr_conf = GET_CONF(EquipExattr, exattr_id);
		if (NULL == p_equip_exattr_conf)
		{
			log_error("NULL == p_equip_exattr_conf[%d]", exattr_id);
			continue;
		}
		uint32_t attr_type = p_equip_exattr_conf->attr_type();
		uint32_t attr_value = p_equip_exattr_conf->attr_value();
		attrs[attr_type] += attr_value;
	}
	//强化属性
	if (m_strengthen_level > 0)
	{
		auto p_equip_conf = GET_CONF(Equip, m_tid);
		if (NULL == p_equip_conf)
		{
			log_error("NULL == p_equip_conf[%d]", m_tid);
			return;
		}
		for (auto it : p_equip_conf->attr_growth())
		{
			uint32_t attr_type = it.first;
			uint32_t attr_value = it.second;
			attrs[attr_type] += attr_value * m_strengthen_level/10000;
		}
	}
	//词缀属性
	if (m_name > 0)
	{
		auto p_equip_name_conf = GET_CONF(EquipName, m_name);
		if (NULL == p_equip_name_conf)
		{
			log_error("NULL == p_equip_name_conf[%d]", m_name);
			return;
		}
		if (0 == p_equip_name_conf->level())
		{
			log_error("0 == p_equip_name_conf->level()[%d]", m_name);
			return;
		}
		for (auto it : p_equip_name_conf->attr())
		{
			uint32_t attr_type = it.first;
			uint32_t base_value = it.second;
			uint32_t growth_value = it.third;
			attrs[attr_type] += base_value + growth_value * (m_strengthen_level / p_equip_name_conf->level());
		}
	}
}

void item_t::fill_equip_log(proto::log::go_equip_log_notify& msg)
{
	msg.set_item_uid(get_uid());
	msg.set_item_tid(get_tid());
	msg.set_hero_uid(m_hero_uid);
	for (auto it : m_attrs)
	{
		proto::common::equip_attr* ea = msg.add_attr();
		ea->set_type(it.first);
		ea->set_value(it.second);
	}
	for (auto exattr : m_exattrs)
	{
		msg.add_exattr(exattr);
	}
	for (auto exattr : m_remake_exattrs)
	{
		msg.add_remake_exattr(exattr);
	}
}

uint32_t item_t::get_base_block_rate()
{
	auto block_rate = GET_CONF(Comprehensive, comprehensive_common::parry_base);
	uint32_t val = GET_COMPREHENSIVE_VALUE_1(block_rate);
	return val;
}

uint32_t item_t::get_base_block_effect()
{
	auto block_effect = GET_CONF(Comprehensive, comprehensive_common::parry_add_base);
	uint32_t val = GET_COMPREHENSIVE_VALUE_1(block_effect);
	return val;
}

uint32_t item_t::get_base_block_effect_reduce_max()
{
	auto block_effect_reduce_max = GET_CONF(Comprehensive, comprehensive_common::parry_add_base_max);
	uint32_t val = GET_COMPREHENSIVE_VALUE_1(block_effect_reduce_max);
	return val;
}

float item_t::get_effect_hit_number()
{
	auto effect_hit_number = GET_CONF(Comprehensive, comprehensive_common::effect_hit_number);
	uint32_t val = GET_COMPREHENSIVE_VALUE_1(effect_hit_number);
	return val/10000.0;
}

float item_t::get_speed_switch_power()
{
	auto speed_switch_power = GET_CONF(Comprehensive, comprehensive_common::speed_switch_power);
	uint32_t val = GET_COMPREHENSIVE_VALUE_1(speed_switch_power);
	return val / 10000.0;
}

uint32_t item_t::get_base_crit_rate()
{
	auto critical_base = GET_CONF(Comprehensive, comprehensive_common::critical_base);
	uint32_t val = GET_COMPREHENSIVE_VALUE_1(critical_base);
	return val;
}

uint32_t item_t::get_base_crit_damage()
{
	auto critical_multiple_base = GET_CONF(Comprehensive, comprehensive_common::critical_multiple_base);
	uint32_t val = GET_COMPREHENSIVE_VALUE_1(critical_multiple_base);
	return val;
}

uint32_t item_t::get_critical_fight_max()
{
	auto critical_fight_max = GET_CONF(Comprehensive, comprehensive_common::critical_fight_max);
	uint32_t val = GET_COMPREHENSIVE_VALUE_1(critical_fight_max);
	return val;
}

uint32_t item_t::get_parry_base_max()
{
	auto parry_base_max = GET_CONF(Comprehensive, comprehensive_common::parry_base_max);
	uint32_t val = GET_COMPREHENSIVE_VALUE_1(parry_base_max);
	return val;
}

uint32_t item_t::get_parry_add_base_max()
{
	auto parry_add_base_max = GET_CONF(Comprehensive, comprehensive_common::parry_add_base_max);
	uint32_t val = GET_COMPREHENSIVE_VALUE_1(parry_add_base_max);
	return val;
}

uint32_t item_t::get_duck_max()
{
	auto duck_max = GET_CONF(Comprehensive, comprehensive_common::duck_max);
	uint32_t val = GET_COMPREHENSIVE_VALUE_1(duck_max);
	return val;
}

uint32_t item_t::get_effect_duck_max()
{
	auto effect_duck_max = GET_CONF(Comprehensive, comprehensive_common::effect_duck_max);
	uint32_t val = GET_COMPREHENSIVE_VALUE_1(effect_duck_max);
	return val;
}

float item_t::get_effect_duck_number()
{
	auto effect_duck_number = GET_CONF(Comprehensive, comprehensive_common::effect_duck_number);
	uint32_t val = GET_COMPREHENSIVE_VALUE_1(effect_duck_number);
	return val/10000.0;
}

float item_t::get_defence_switch_hp()
{
	auto defence_switch_hp = GET_CONF(Comprehensive, comprehensive_common::defence_switch_hp);
	uint32_t val = GET_COMPREHENSIVE_VALUE_1(defence_switch_hp);
	return val/10000.0;
}

uint32_t item_t::get_hp_score(std::map<uint32_t, int>& attrs)
{
	//生命值
	float hp_value = attrs[attrtype_enum::max_hp];
	//EQUIP_LOG("hp_value[%.3f]", hp_value);
	//物防
	float phydef_value = attrs[attrtype_enum::def_physical];
	//EQUIP_LOG("phydef_value[%.3f]", phydef_value);
	
	//抗暴生存能力
	uint32_t crit_rate = get_base_crit_rate();
	uint32_t crit_damage = get_base_crit_damage();
	uint32_t crit_damage_reduce = attrs[attrtype_enum::crit_damage_reduce];
	if (crit_damage_reduce > get_critical_fight_max())
	{
		crit_damage_reduce = get_critical_fight_max();
	}
	float crit_damage_reduce_value = (1.0f + crit_rate / 10000.0) * ((crit_damage - crit_damage_reduce) / 10000.0);
	//EQUIP_LOG("crit_damage_reduce_value[%.3f]", crit_damage_reduce_value);
	//闪避生存能力
	uint32_t dodge = attrs[attrtype_enum::dodge];
	if (dodge > get_duck_max())
	{
		dodge = get_duck_max();
	}
	float dodge_value = 1.0f - (dodge / 10000.0);
	//EQUIP_LOG("dodge_value[%.3f]", dodge_value);
	//效果闪避生存能力
	uint32_t effect_dodge_rate = attrs[attrtype_enum::effect_dodge_rate];
	if (effect_dodge_rate > get_effect_duck_max())
	{
		effect_dodge_rate = get_effect_duck_max();
	}
	float effect_dodge_value = (1.0f - (effect_dodge_rate / 10000.0)) * get_effect_duck_number();
	//EQUIP_LOG("effect_dodge_value[%.3f]", effect_dodge_value);
	if (0 == (uint32_t)(crit_damage_reduce_value * dodge_value * effect_dodge_value))
	{
		log_error("attr invalid");
		return 0;
	}
	//数值评分
	uint32_t base_score = (hp_value + get_defence_switch_hp() * phydef_value ) / \
		(crit_damage_reduce_value * dodge_value * effect_dodge_value);
	//EQUIP_LOG("base_score[%d]", base_score);
	return base_score;
}

uint32_t item_t::get_atk_score(std::map<uint32_t, int>& attrs)
{
	//攻击
	float atk_value = attrs[attrtype_enum::atk];
	EQUIP_LOG("atk_value[%.3f]", atk_value);
	//暴击输出
	float crit_value = 1.0f + ((get_weapon_crit_rate() + attrs[attrtype_enum::crit_rate]) / 10000.0)* ((get_weapon_crit_damage() + attrs[attrtype_enum::crit_damage]) / 10000.0 - 1.0f);
	EQUIP_LOG("crit_value[%.3f]", crit_value);
	//效果命中输出
	float effect_value = 1.0f + (attrs[attrtype_enum::effect_hit_rate] / 10000.0) * get_effect_hit_number();
	EQUIP_LOG("effect_value[%.3f]", effect_value);
	//数值评分
	uint32_t base_score = atk_value * crit_value * effect_value;
	EQUIP_LOG("base_score[%d]", base_score);
	return base_score;
}

uint32_t item_t::get_atk_score_without_weapon(std::map<uint32_t, int>& attrs)
{
	//攻击
	float atk_value = attrs[attrtype_enum::atk];
	//EQUIP_LOG("atk_value[%.3f]", atk_value);
	//暴击输出
	float crit_value = 1.0f + (attrs[attrtype_enum::crit_rate] / 10000.0)* (attrs[attrtype_enum::crit_damage] / 10000.0 - 1.0f);
	//EQUIP_LOG("crit_value[%.3f]", crit_value);
	//效果命中输出
	float effect_value = 1.0f + (attrs[attrtype_enum::effect_hit_rate] / 10000.0) * get_effect_hit_number();
	//EQUIP_LOG("effect_value[%.3f]", effect_value);
	//数值评分
	uint32_t base_score = atk_value * crit_value * effect_value;
	//EQUIP_LOG("base_score[%d]", base_score);
	return base_score;
}

float item_t::get_hp_effective()
{
	auto hp_effective = GET_CONF(Comprehensive, comprehensive_common::hp_effective);
	uint32_t val = GET_COMPREHENSIVE_VALUE_1(hp_effective);
	return val/10000.0;
}

float item_t::get_ad_effective()
{
	auto ad_effective = GET_CONF(Comprehensive, comprehensive_common::ad_effective);
	uint32_t val = GET_COMPREHENSIVE_VALUE_1(ad_effective);
	return val/10000.0;
}

float item_t::get_base_attack_range()
{
	auto attack_range_equip_default = GET_CONF(Comprehensive, comprehensive_common::attack_range_equip_default);
	uint32_t val = GET_COMPREHENSIVE_VALUE_1(attack_range_equip_default);
	return val / 10000.0;
}

uint32_t item_t::get_block_param1()
{
	auto block_param1 = GET_CONF(Comprehensive, comprehensive_common::wreck_switch_1);
	uint32_t val = GET_COMPREHENSIVE_VALUE_1(block_param1);
	return val;
}

uint32_t item_t::get_block_param2()
{
	auto block_param2 = GET_CONF(Comprehensive, comprehensive_common::wreck_switch_2);
	uint32_t val = GET_COMPREHENSIVE_VALUE_1(block_param2);
	return val;
}

uint32_t item_t::get_weapon_crit_rate()
{
	auto weapon_crit_rate = GET_CONF(Comprehensive, comprehensive_common::weapon_crit);
	uint32_t val = GET_COMPREHENSIVE_VALUE_1(weapon_crit_rate);
	return val;
}

uint32_t item_t::get_weapon_crit_damage()
{
	auto weapon_crit_damage = GET_CONF(Comprehensive, comprehensive_common::weapon_crit_damage);
	uint32_t val = GET_COMPREHENSIVE_VALUE_1(weapon_crit_damage);
	return val;
}

uint32_t item_t::get_min_rarity_level()
{
	auto min_rarity_level = GET_CONF(Comprehensive, comprehensive_common::treasure_equip_level);
	uint32_t val = GET_COMPREHENSIVE_VALUE_1(min_rarity_level);
	return val;
}

void item_t::peek_goods_item(proto::common::goods_item* p_goods_item)
{
	if (NULL == p_goods_item)
	{
		log_error("NULL == p_goods_item");
		return;
	}
	p_goods_item->set_item_tid(get_tid());
	p_goods_item->set_rarity(m_rarity);
	if (is_equip())
	{
		proto::common::equip_single* p_equip = p_goods_item->mutable_equip_info();
		for (auto it : m_attrs)
		{
			proto::common::equip_attr* p_equip_attr = p_equip->add_attr();
			p_equip_attr->set_type(it.first);
			p_equip_attr->set_value(it.second);
		}
		for (auto exattr : m_exattrs)
		{
			p_equip->add_exattr(exattr);
		}
		p_equip->set_score(m_score);
		p_equip->set_name(m_name);
		p_equip->set_special(m_special);
		p_equip->set_strengthen_level(m_strengthen_level);
		p_equip->set_strmaster_level(m_strmaster_level);
		p_equip->set_remake_material(m_remake_material);
		p_equip->set_lock_material(m_lock_material);
		p_equip->set_strengthen_material(m_strengthen_material);
		p_equip->set_strengthen_material_ex(m_strengthen_material_ex);
		p_equip->set_strengthen_money(m_strengthen_money);
	}
}

void item_t::check_rarity()
{
	auto p_item_conf = GET_CONF(Item, get_tid());
	if (NULL == p_item_conf)
	{
		log_error("NULL == p_item_conf[%d]", get_tid());
		return;
	}
	m_rarity = 0;
	if (is_equip())
	{
		//log_warn("quality[%d] special[%d] min_special_quality[%d] name[%d] min_name_quality[%d]", 
			//p_item_conf->quality(), get_special(), get_min_special_quality(), get_name(), get_min_name_quality());
		if (get_special() > 0 && p_item_conf->quality() >= get_min_special_quality())
		{
			m_rarity = 1;
			return;
		}
		if (get_name() > 0 && p_item_conf->quality() >= get_min_name_quality())
		{
			m_rarity = 1;
			return;
		}
	}
	else
	{
		if (p_item_conf->rarity() > 0)
		{
			m_rarity = 1;
		}
	}
}

void item_t::check_state()
{
	if (m_state != item_state_cooldown)
	{
		return;
	}
	if (m_expired_time > 0 && time_util_t::now_time() >= m_expired_time)
	{
		m_state = 0;
		m_expired_time = 0;
	}
}

uint32_t item_t::get_min_special_quality()
{
	auto min_special_quality = GET_CONF(Comprehensive, comprehensive_common::quality_special);
	uint32_t val = GET_COMPREHENSIVE_VALUE_1(min_special_quality);
	return val;
}

uint32_t item_t::get_min_name_quality()
{
	auto min_name_quality = GET_CONF(Comprehensive, comprehensive_common::quality_prefix);
	uint32_t val = GET_COMPREHENSIVE_VALUE_1(min_name_quality);
	return val;
}

// uint32_t item_t::calc_special_score()
// {
// 	if (m_special == 0)
// 	{
// 		return 0;
// 	}
// 	// 读星级表
// 	auto* p_special_conf = GET_CONF(EquipSpecial, m_special);
// 	if (p_special_conf == NULL)
// 	{
// 		log_error("p_special_conf [%u] not exists", m_special);
// 		return 0;
// 	}
// 	uint32_t skill_power = 0;
// 	for (auto skill_id : p_special_conf->attr())
// 	{
// 		SkillScore* p_skill_score = GET_CONF(SkillScore, hash_util_t::hash_to_uint32(skill_id, 1));
// 		if (p_skill_score != NULL)
// 		{
// 			skill_power += p_skill_score->score();
// 		}
// 	}
// 	return skill_power;
// }

// uint32_t item_t::calc_suit_score()
// {
// 	auto p_equip_conf = GET_CONF(Equip, get_tid());
// 	if (NULL == p_equip_conf)
// 	{
// 		log_error("NULL == p_equip_conf[%d]", get_tid());
// 		return 0;
// 	}
// 	uint32_t suit_id = p_equip_conf->suit_id();
// 	auto p_equip_suit_conf = GET_CONF(EquipSuit, suit_id);
// 	if (NULL == p_equip_suit_conf)
// 	{
// 		log_error("NULL == p_equip_suit_conf[%d]", suit_id);
// 		return 0;
// 	}
// 	uint32_t skill_power = 0;
// 	for (auto skill_id : p_equip_suit_conf->three_attr())
// 	{
// 		SkillScore* p_skill_score = GET_CONF(SkillScore, hash_util_t::hash_to_uint32(skill_id, 1));
// 		if (p_skill_score != NULL)
// 		{
// 			skill_power += p_skill_score->score();
// 		}
// 	}
// 	return skill_power;
// }

uint32_t item_t::get_equip_lucky()
{
	auto p_item_conf = GET_CONF(Item, get_tid());
	if (NULL == p_item_conf)
	{
		log_error("NULL == p_item_conf[%d]", get_tid());
		return 0;
	}
	if (!is_equip())
	{
		return 0;
	}
	auto p_equip_conf = GET_CONF(Equip, get_tid());
	if (NULL == p_equip_conf)
	{
		log_error("NULL == p_equip_conf[%d]", get_tid());
		return 0;
	}
	uint32_t equip_lucky = 0;
	equip_lucky += p_equip_conf->star() * get_equip_make_star_constant();
	equip_lucky += p_item_conf->quality() * get_equip_make_quality_constant();
	if (get_special() > 0)
	{
		equip_lucky += get_equip_make_special_constant();
	}
	return equip_lucky;
}

bool item_t::is_clear_equip_lucky()
{
	auto p_item_conf = GET_CONF(Item, get_tid());
	if (NULL == p_item_conf)
	{
		log_error("NULL == p_item_conf[%d]", get_tid());
		return false;
	}
	if (!is_equip())
	{
		return false;
	}
	if (get_special() > 0)
	{
		return true;
	}
	if (p_item_conf->quality() >= get_equip_make_lucky_quality())
	{
		return true;
	}
	return false;
}

uint32_t item_t::get_equip_make_star_constant()
{
	auto equip_make_star_constant = GET_CONF(Comprehensive, comprehensive_common::equip_make_star_constant);
	uint32_t val = GET_COMPREHENSIVE_VALUE_1(equip_make_star_constant);
	return val;
}

uint32_t item_t::get_equip_make_quality_constant()
{
	auto equip_make_quality_constant = GET_CONF(Comprehensive, comprehensive_common::equip_make_quality_constant);
	uint32_t val = GET_COMPREHENSIVE_VALUE_1(equip_make_quality_constant);
	return val;
}

uint32_t item_t::get_equip_make_special_constant()
{
	auto equip_make_special_constant = GET_CONF(Comprehensive, comprehensive_common::equip_make_special_constant);
	uint32_t val = GET_COMPREHENSIVE_VALUE_1(equip_make_special_constant);
	return val;
}

uint32_t item_t::get_equip_make_lucky_quality()
{
	auto equip_make_lucky_quality = GET_CONF(Comprehensive, comprehensive_common::equip_make_lucky_quality);
	uint32_t val = GET_COMPREHENSIVE_VALUE_1(equip_make_lucky_quality);
	return val;
}

