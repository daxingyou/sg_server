#include "skill_manager.hpp"
#include "fight_hero.hpp"
#include "config_mgr.h"

//查找技能位置
int32_t skill_manager_t::get_skill_pos( uint32_t skill_id ) {
	auto iter_f = m_skill_pos.find(skill_id);
	if (iter_f == m_skill_pos.end()) {
		return -1;
	}
	return iter_f->second;
}

void skill_manager_t::add_skill(uint32_t  turn_level, skill_data_t &data, uint32_t skill_pos ) {
	//记录技能索引位置
	m_skill_pos[data.skill_id] = skill_pos;

	skill_info_t *p_skill_info = get_skill_info_by_pos(skill_pos);
	if (p_skill_info) {
		p_skill_info->m_skill_data_list.push_back(data);
		return;
	}
	else {
		skill_info_t skill_info;
		skill_info.m_public_cd = 0;
		if (turn_level == 0) {
			skill_info.m_skill_level = data.skill_level;
		}
		skill_info.m_skill_data_list.push_back(data);
		m_skill_list.push_back(skill_info);
	}
}

//获取当前技能列表
std::vector<skill_info_t> &skill_manager_t::get_cur_skill_list() {
	return m_skill_list;
}

skill_info_t *skill_manager_t::get_skill_info(uint32_t skill_id) {
	int32_t skill_pos = get_skill_pos(skill_id);
	if (skill_pos < 0 || (size_t)skill_pos >= m_skill_list.size() ) {
		return NULL;
	}
	return &m_skill_list[skill_pos];
}

skill_info_t *skill_manager_t::get_skill_info_by_pos(uint32_t pos) {
	if (pos >= m_skill_list.size()) {
		return NULL;
	}
	return &m_skill_list[pos];
}

skill_data_t *skill_manager_t::get_skill_data(uint32_t skill_id) {
	skill_info_t *p_skill_info = get_skill_info(skill_id);
	if (NULL == p_skill_info) {
		return NULL;
	}
	return p_skill_info->get_skill_data_by_skill_id(skill_id);
}

bool skill_manager_t::is_single_attack( SkillEffectTable *p_conf) {
	if (NULL == p_conf) {
		return false;
	}
	switch (p_conf->target_type()) {
		case skill_attack_target_type_select:
		case skill_attack_target_type_default:
		case skill_attack_target_type_min_hp:
			return true;
	}
	return false;
}

// 获取默认技能
int32_t skill_manager_t::get_default_skill() {
	for (auto &skill_info : m_skill_list) {
		const skill_data_t *p_skill_data = skill_info.get_skill_data_by_turn_level(m_turn_level);
		if (NULL == p_skill_data) {
			log_error("get_default_skill is null at turn level:[%u]", m_turn_level);
			continue;
		}
		if (p_skill_data->type == proto::common::skill_type_atk)
			return p_skill_data->skill_id;
	}
	return 0;
}

// 获取反击技能
int32_t skill_manager_t::get_counter_skill() {
	return get_default_skill();
}

//获取技能CD
uint32_t skill_manager_t::get_skill_cd(uint32_t skill_id ) {
	skill_info_t *p_skill_info = get_skill_info(skill_id);
	if (NULL == p_skill_info) {
		//log_error("get_skill skill_id [%u] NULL", skill_id);	有太多放公立技能的了，这里只好屏蔽
		return -1;
	}

	return p_skill_info->get_skill_cd(skill_id);
}

void skill_manager_t::sub_all_skill_cd(uint32_t cd) {
	for (auto &skill_info : m_skill_list) {
		for (auto &skill_data : skill_info.m_skill_data_list) {
			uint32_t change_cd = skill_data.skill_cd > cd ? skill_data.skill_cd - cd : 0;
			skill_data.set_skill_cd(change_cd);
		}
		uint32_t change_cd = skill_info.m_public_cd > cd ? skill_info.m_public_cd - cd : 0;
		skill_info.set_pub_skill_cd(change_cd);
		sync_skill_cd(get_self_ptr(), &skill_info);
	}
	add_skill_cd_notify();
}

//减指定技能CD
void skill_manager_t::sub_skill_cd(uint32_t skill_id, uint32_t cd) {
	skill_info_t *p_skill_info = get_skill_info(skill_id);
	if (NULL == p_skill_info) {
		log_error("get_skill skill_id [%u] NULL", skill_id);
		return;
	}

	p_skill_info->sub_skill_cd(skill_id, cd);

	add_skill_cd_notify();
}

// 加技能cd
void skill_manager_t::add_skill_cd(uint32_t skill_id) {
	skill_info_t *p_skill_info = get_skill_info(skill_id);
	if (NULL == p_skill_info) {
		log_error("get_skill skill_id [%u] NULL", skill_id);
		return;
	}
	
	p_skill_info->add_skill_cd(get_self_ptr(), skill_id);

	//通知客户端玩家技能CD变化
	add_skill_cd_notify();
}

void skill_manager_t::add_skill_cd_ex(uint32_t skill_id, uint32_t cd) {
	skill_info_t *p_skill_info = get_skill_info(skill_id);
	if (NULL == p_skill_info) {
		log_error("get_skill skill_id [%u] NULL", skill_id);
		return;
	}

	p_skill_info->add_skill_cd_ex(skill_id,cd);

	//通知客户端玩家技能CD变化
	add_skill_cd_notify();
}


//设置技能CD
void skill_manager_t::set_skill_cd(uint32_t skill_id, uint32_t cd) {
	skill_info_t *p_skill_info = get_skill_info(skill_id);
	if (NULL == p_skill_info) {
		log_error("get_skill skill_id [%u] NULL", skill_id);
		return;
	}

	p_skill_info->set_skill_cd(skill_id,cd);

	//通知客户端玩家技能CD变化
	add_skill_cd_notify();
}

// 添加所有技能CD 
void skill_manager_t::be_inc_all_skill_cd(fight_hero_ptr attacker, uint32_t cd) {
	fight_hero_ptr p_owner = get_self_ptr();
	if (NULL == p_owner) {
		log_error("fight_hero is NULL");
		return;
	}

	for (auto &skill_info : m_skill_list) {
		uint32_t skill_id = skill_info.get_skill_id(m_turn_level);
		skill_info.add_skill_cd(p_owner, skill_id, cd);
		sync_skill_cd(attacker, &skill_info);
	}
}

// 减少所有技能CD
void skill_manager_t::be_dec_all_skill_cd(fight_hero_ptr attacker, uint32_t cd) {
	for (auto &skill_info : m_skill_list) {
		uint32_t skill_id = skill_info.get_skill_id(m_turn_level);
		skill_info.sub_skill_cd(skill_id, cd);
		sync_skill_cd(attacker, &skill_info);
	}
}
// 随机添加技能CD 
void skill_manager_t::be_inc_random_skill_cd(fight_hero_ptr attacker, uint32_t cd) {
	fight_hero_ptr p_owner = get_self_ptr();
	if (NULL == p_owner) {
		log_error("fight_hero is NULL");
		return;
	}

	std::vector<uint32_t> skills;
	for (auto &skill_info : m_skill_list) {
		uint32_t skill_id = skill_info.get_skill_id(m_turn_level);
		SkillEffectTable *p_skill_conf = GET_CONF(SkillEffectTable, hash_util_t::hash_to_uint32_move_7(skill_id, skill_info.get_skill_level(m_turn_level)));
		if (NULL == p_skill_conf) {
			log_error("skill[%u] skill_conf is null", skill_id);
			continue;
		}

		if (p_skill_conf->cd() == 0)
			continue;

		if (skill_info.get_skill_cd(skill_id) >= p_skill_conf->cd())
			continue;

		skills.push_back(skill_id);
	}

	if (skills.empty())
		return;
	
	int rand = random_util_t::randMin(0, skills.size());
	if (rand == -1) {
		log_error("rand error skill_size[%d]", (int32_t)skills.size());
		return;
	}
	
	skill_info_t *p_skill_info = get_skill_info(skills[rand]);
	if (NULL == p_skill_info) {
		return;
	}

	p_skill_info->add_skill_cd(p_owner, m_turn_level, cd);

	sync_skill_cd(attacker, p_skill_info);
}

// 随机减少技能CD
void skill_manager_t::be_dec_random_skill_cd(fight_hero_ptr attacker, uint32_t cd) {
	fight_hero_ptr p_owner = get_self_ptr();
	if (NULL == p_owner) {
		log_error("fight_hero is NULL");
		return;
	}

	std::vector<uint32_t> skills;
	for (auto &skill_info : m_skill_list) {
		uint32_t skill_id = skill_info.get_skill_id(m_turn_level);
		SkillEffectTable *p_skill_conf = GET_CONF(SkillEffectTable, hash_util_t::hash_to_uint32_move_7(skill_id, skill_info.get_skill_level(m_turn_level)));
		if (NULL == p_skill_conf) {
			log_error("skill[%u] skill_conf is null", skill_id);
			continue;
		}

		if (p_skill_conf->type() == proto::common::skill_type_passive)
			continue;

		if (p_skill_conf->cd() == 0)
			continue;

		if (skill_info.get_skill_cd(skill_id) == 0 )
			continue;

		skills.push_back(skill_id);
	}

	if (skills.empty())
		return;

	int rand = random_util_t::randMin(0, skills.size());
	if (rand == -1) {
		log_error("rand error skill_size[%d]", (int32_t)skills.size());
		return;
	}

	skill_info_t *p_skill_info = get_skill_info(skills[rand]);
	if (NULL == p_skill_info) {
		return;
	}

	p_skill_info->add_skill_cd(p_owner, skills[rand], cd);;
	sync_skill_cd(attacker, p_skill_info);
}

void skill_manager_t::add_skill_cd_notify() {
	fight_hero_ptr p_owner = get_self_ptr();
	if (NULL == p_owner) {
		log_error("fight_hero is null");
		return;
	}

	combat_ptr p_combat = p_owner->get_combat();
	if (p_combat) {
		p_combat->set_hero_skill_cd(p_owner);
	}
}

void skill_manager_t::sync_skill_cd( fight_hero_ptr attacker, skill_info_t *p_skill_info ) {
	fight_hero_ptr p_owner = get_self_ptr();
	if (NULL == p_owner) {
		log_error("fight_hero is null");
		return;
	}

	if (NULL == p_skill_info) {
		return;
	}

	uint32_t skill_id = p_skill_info->get_skill_id(m_turn_level);

	p_owner->add_combat_act(proto::common::combat_act_type_syn_cd, skill_id , p_owner, attacker, p_skill_info->get_skill_cd(skill_id));
}

uint32_t skill_manager_t::get_skill_level(uint32_t skill_id) {
	skill_info_t *p_skill_info = get_skill_info(skill_id);
	if (NULL == p_skill_info) {
		log_error("skill[%u] not found", skill_id );
		return 0;
	}

	return p_skill_info->get_skill_level(skill_id);
}


