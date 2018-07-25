#include "fight_obj.hpp"
#include "tblh/Comprehensive.tbls.h"
#include "config/config_manager.hpp"
#include "combat.hpp"
static const uint32_t INIT_SP = 200;

fight_obj_t::fight_obj_t()
{
}

fight_obj_t::~fight_obj_t()
{
}

void fight_obj_t::close()
{

}

bool fight_obj_t::is_object_type()
{
	if (get_type() == proto::common::SCENEOBJECT_USER ||
		get_type() == proto::common::SCENEOBJECT_TUTOR ||
		get_type() == proto::common::SCENEOBJECT_USERSHADOW)
		return true;
	return false;
}

bool fight_obj_t::init(proto::common::obj_single* os)
{
	if (NULL == os)
	{
		return false;
	}

	auto conf = GET_CONF(Comprehensive, comprehensive_common::init_fire);
	uint32_t init_sp = GET_COMPREHENSIVE_VALUE_1(conf);
	
	os->set_sp(init_sp);
	m_obj_data.CopyFrom(*os);
	m_state = proto::common::combat_obj_state_loading;
	return true;
}

uint64_t fight_obj_t::get_uid()
{
	return string_util_t::string_to_uint64(m_obj_data.object_uid());
}

const std::string& fight_obj_t::get_str_uid()
{
	return m_obj_data.object_uid();
}

proto::common::EM_SCENEOBJECT_TYPE fight_obj_t::get_type()
{
	return m_obj_data.object_type();
}

uint32_t fight_obj_t::get_camp()
{
	return m_obj_data.camp();
}

uint32_t fight_obj_t::get_enemy_camp()
{
	if (get_camp() == proto::common::combat_camp_1)
	{
		return proto::common::combat_camp_2;
	}
	else
	{
		return proto::common::combat_camp_1;
	}
}

uint32_t fight_obj_t::get_level()
{
	return m_obj_data.level();
}

void fight_obj_t::set_state(proto::common::combat_obj_state state)
{
	m_state = state;
}

proto::common::combat_obj_state fight_obj_t::get_state()
{
	return m_state;
}

void fight_obj_t::on_round_start_imp(uint64_t attacker_uid)
{
	if (get_uid() != attacker_uid)
		set_state(proto::common::combat_obj_state_prepare_end);
	else
		set_state(proto::common::combat_obj_state_prepare);
//	set_disposition_sp(get_sp());
}

void fight_obj_t::on_round_end_imp()
{

}

void fight_obj_t::on_play_start()
{
	set_state(proto::common::combat_obj_state_play);
}

void fight_obj_t::on_combat_end(combat_ptr combat)
{
	role_ptr role = role_manager_t::find_role(get_uid());
	if (NULL == role)
	{
		return;
	}

	role->set_fight_uid(0);
	// if(combat != NULL)
	// 	role->script_event_combat_end(combat->get_fight_uid());
}

void fight_obj_t::set_team_shield(int32_t value)
{
	m_combat_count_data.m_team_shield = value;
	if (m_combat_count_data.m_team_shield < 0)
		m_combat_count_data.m_team_shield = 0;
}

bool fight_obj_t::is_monster()
{
	return get_type() == proto::common::SCENEOBJECT_MONSTER;
}

void fight_obj_t::set_auto_fight(uint32_t auto_fight)
{
	m_obj_data.set_auto_fight(auto_fight);
}

bool fight_obj_t::is_auto_fight()
{
	return (m_obj_data.auto_fight() > 0);
}

uint32_t fight_obj_t::get_sp()
{
	return m_obj_data.sp();
}

uint32_t fight_obj_t::get_team_member_size()
{
	return m_obj_data.team_member_size();
}

int32_t fight_obj_t::get_team_member_pos()
{
	return m_obj_data.team_member_pos();
}

void fight_obj_t::set_sp(uint32_t value)
{
	m_obj_data.set_sp(value);
}

void fight_obj_t::set_acttack_count(uint32_t value)
{
	m_attack_count = value;
}

uint32_t fight_obj_t::get_acttack_count()
{
	return m_attack_count;
}

void fight_obj_t::set_disposition_sp(uint32_t value)
{
	m_disposition_sp = value;
}

uint32_t fight_obj_t::get_disposition_sp()
{
	return m_disposition_sp;
}

bool fight_obj_t::is_offline()
{
	role_ptr p_role = role_manager_t::find_role(get_uid());
	if (NULL == p_role)
		return true;

	return (p_role->get_online_state() != common::ROLE_ONLINE_STATE_ONLINE);
}
