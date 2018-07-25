#include "skill_data.hpp"
#include "config_mgr.h"
#include "fight_hero.hpp"
#include "protocpp/common.pb.h"

void skill_data_t::set_skill_cd(uint32_t cd) {
	skill_cd = cd;
}

////////////////////////////////
void skill_info_t::set_pub_skill_cd(uint32_t cd) {
	m_public_cd = cd;
}

void skill_info_t::set_skill_cd(uint32_t skill_id, uint32_t cd){
	skill_data_t *p_skill_data = get_skill_data_by_skill_id(skill_id);
	if (NULL == p_skill_data) {
		return;
	}
	
	if (p_skill_data->is_use_public_cd == 0) {
		set_pub_skill_cd(cd);
		return;
	}
	p_skill_data->set_skill_cd(cd);
}

void skill_info_t::add_skill_cd_ex(uint32_t skill_id, uint32_t cd) {
	skill_data_t *p_skill_data = get_skill_data_by_skill_id(skill_id);
	if (NULL == p_skill_data) {
		log_error("can't find skill_id[%u] skill", skill_id);
		return;
	}

	SkillEffectTable* p_skill_conf = GET_CONF(SkillEffectTable, hash_util_t::hash_to_uint32_move_7(p_skill_data->skill_id, p_skill_data->skill_level));
	if (NULL == p_skill_conf) {
		log_error("add_skill_cd error, skill_tid:%d, level:%d", p_skill_data->skill_id, p_skill_data->skill_level);
		return;
	}
	
	//公共技能
	if (p_skill_data->is_use_public_cd == 0) {
		m_public_cd += cd;
		if ((uint32_t)m_public_cd > p_skill_conf->max_cd()) {
			m_public_cd = p_skill_conf->max_cd();
		}
		return;
	}

	p_skill_data->skill_cd += cd;
	if ((uint32_t)m_public_cd > p_skill_conf->max_cd()) {
		p_skill_data->skill_cd = p_skill_conf->max_cd();
	}
}

void skill_info_t::add_skill_cd( fight_hero_ptr m_owner, uint32_t skill_id, uint32_t cd) {
	if ( NULL == m_owner) {
		return;
	}

	skill_data_t *p_skill_data = get_skill_data_by_skill_id(skill_id);
	if (NULL == p_skill_data) {
		log_error("can't find skill_id[%u] skill", skill_id);
		return;
	}
	
	SkillEffectTable* p_skill_conf = GET_CONF(SkillEffectTable, hash_util_t::hash_to_uint32_move_7(p_skill_data->skill_id, p_skill_data->skill_level));
	if (NULL == p_skill_conf) {
		log_error("add_skill_cd error, skill_tid:%d, level:%d", p_skill_data->skill_id, p_skill_data->skill_level);
		return;
	}

	//无CD
	if (p_skill_conf->cd() == 0) {
		return;
	}

	//判断是否免CD
	int exempt_cd = m_owner->get_attr(attrtype_enum::exempt_cd);
	if (random_util_t::selectByTenTh(exempt_cd))
	{
		m_owner->add_combat_act( proto::common::combat_act_type_syn_cd, p_skill_data->skill_id, m_owner, m_owner, get_skill_cd(skill_id) );
		return;
	}

	// 技能CD 的额外变化
	int ex_cd = m_owner->get_attr(attrtype_enum::skill_cd_change);
	m_owner->set_attr(attrtype_enum::skill_cd_change, 0);

	uint32_t change_cd = (cd + p_skill_data->skill_cd) > p_skill_conf->cd() ? p_skill_conf->cd() : (cd + p_skill_data->skill_cd);
	change_cd += ex_cd;

	//公共技能
	if (p_skill_data->is_use_public_cd == 0 ) {
		set_pub_skill_cd(change_cd);
		return;
	}

	//单技能
	p_skill_data->set_skill_cd(change_cd);
}

void skill_info_t::sub_skill_cd(uint32_t skill_id, uint32_t cd) {
	skill_data_t *p_skill_data = get_skill_data_by_skill_id(skill_id);
	if (NULL == p_skill_data) {
		return;
	}

	SkillEffectTable* p_skill_conf = GET_CONF(SkillEffectTable, hash_util_t::hash_to_uint32_move_7(p_skill_data->skill_id, p_skill_data->skill_level));
	if (NULL == p_skill_conf) {
		log_error("skill[%u] skill_conf is null", p_skill_data->skill_id);
		return;
	}

	// 判断是不是不受减CD影响
	if ((p_skill_conf->special_sign() & skill_special_sign_immune_dec_cd) != 0)
		return;

	//公共技能
	if (p_skill_data->is_use_public_cd == 0) {
		uint32_t change_cd = m_public_cd > cd ? m_public_cd - cd : 0;
		set_pub_skill_cd(change_cd);
		return;
	}

	//单技能
	uint32_t change_cd = p_skill_data->skill_cd > cd ? p_skill_data->skill_cd - cd : 0;
	p_skill_data->set_skill_cd(change_cd);

}

uint32_t skill_info_t::get_skill_cd( uint32_t skill_id ) {
	skill_data_t *p_skill_data = get_skill_data_by_skill_id(skill_id);
	if (NULL == p_skill_data) {
		return -1;
	}

	if (p_skill_data->is_use_public_cd == 0) {
		return m_public_cd;
	}
	return p_skill_data->skill_cd;
}

uint32_t skill_info_t::get_skill_id(uint32_t turn_level) {
	const skill_data_t *p_skill_data = get_skill_data_by_turn_level(turn_level);
	if (NULL == p_skill_data) {
		return 0;
	}
	return p_skill_data->skill_id;
}

uint32_t skill_info_t::get_skill_level(uint32_t turn_level) {
	return m_skill_level;
}


//获取技能信息
skill_data_t *skill_info_t::get_skill_data_by_turn_level(uint32_t turn_level){
	if (m_skill_data_list.size() == 0) {
		return NULL;
	}

	if (turn_level == 0 || turn_level >= m_skill_data_list.size()) {
		return &m_skill_data_list[0];
	}

	return &m_skill_data_list[turn_level];
}

skill_data_t *skill_info_t::get_skill_data_by_skill_id(uint32_t skill_id){
	for (std::vector<skill_data_t>::iterator iter = m_skill_data_list.begin(); iter != m_skill_data_list.end(); ++iter) {
		skill_data_t &skill_data = *iter;
		if (skill_data.skill_id == skill_id) {
			return &skill_data;
		}
	}
	return NULL;
}

