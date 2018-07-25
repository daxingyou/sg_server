#ifndef __FIGHT_OBJ_HPP__
#define __FIGHT_OBJ_HPP__

#include "role/role_manager.hpp"
#include "common.pb.h"

struct combat_count_data
{
	int32_t		m_team_shield = 0;						// 队伍护盾
	uint32_t	m_team_shield_buff_type = 0;			// 队伍护盾buff类型
	uint64_t	m_team_shield_hero_uid = 0;				// 队伍护盾的挂载武将
	uint64_t	m_team_damage_turn_hero_uid = 0;		// 队伍伤害转换的挂载武将
};


class fight_obj_t
{
public:
	fight_obj_t();
	~fight_obj_t();

	void close();

	bool init(proto::common::obj_single* os);

	template<typename T_MSG>
	void send_msg_to_client(uint16_t cmd, const T_MSG& protobuf_msg);

	uint64_t get_uid();
	const std::string& get_str_uid();
	proto::common::EM_SCENEOBJECT_TYPE get_type();
	uint32_t get_camp();
	uint32_t get_enemy_camp();
	uint32_t get_level();

	void set_state(proto::common::combat_obj_state state);
	proto::common::combat_obj_state get_state();

	bool is_monster();

	void set_auto_fight(uint32_t auto_fight);
	bool is_auto_fight();

	uint32_t get_sp();
	void set_sp(uint32_t value);

	void set_acttack_count(uint32_t value);
	uint32_t get_acttack_count();

	void set_disposition_sp(uint32_t value);
	uint32_t get_disposition_sp();

	void ready() { m_is_ready = true; }
	bool is_ready() { return m_is_ready; }
	void escape() { m_is_escape = true; }
	bool is_escape() { return m_is_escape; }
public:
	void on_round_start_imp(uint64_t attacker_uid);
	void on_round_end_imp();
	void on_play_start();
	void on_combat_end();

public:
	void set_team_shield(int32_t value);
	int32_t get_team_shield() { return m_combat_count_data.m_team_shield; }
	void set_team_shield_buff_type(uint32_t buff_type) { m_combat_count_data.m_team_shield_buff_type = buff_type; }
	int32_t get_team_shield_buff_type() { return m_combat_count_data.m_team_shield_buff_type; }
	void set_team_shield_hero_uid(uint64_t hero_uid) { m_combat_count_data.m_team_shield_hero_uid = hero_uid; }
	uint64_t get_team_shield_hero_uid() { return m_combat_count_data.m_team_shield_hero_uid; }
	void set_team_damage_turn_hero_uid(uint64_t hero_uid) { m_combat_count_data.m_team_damage_turn_hero_uid = hero_uid; }
	uint64_t get_team_damage_turn_hero_uid() { return m_combat_count_data.m_team_damage_turn_hero_uid; }

public:

	proto::common::obj_single m_obj_data;
	proto::common::combat_obj_state m_state = proto::common::combat_obj_state_none;
	combat_count_data m_combat_count_data;
	uint32_t m_disposition_sp = 0;
	uint32_t m_attack_count = 0;

	bool	 m_is_ready = false;
	bool	 m_is_escape = false;
};

typedef boost::shared_ptr<fight_obj_t> fight_obj_ptr;
//typedef fight_obj_t* fight_obj_ptr;

template<typename T_MSG>
void fight_obj_t::send_msg_to_client(uint16_t cmd, const T_MSG& protobuf_msg)
{
	if (get_type() == proto::common::SCENEOBJECT_MONSTER)
		return;
	role_ptr role = role_manager_t::find_role(get_uid());
	if (NULL == role)
	{
		return;
	}

	role->send_msg_to_client(cmd, protobuf_msg);
}

#endif//__FIGHT_OBJ_HPP__

