#pragma once

#include "monster/monster.hpp"
#include "family_war_common.hpp"

#include "client.pb.h"
#include "role/role.hpp"

#include <boost/asio/deadline_timer.hpp>
#include <boost/system/error_code.hpp>
#include "fight/combat.hpp"

class family_war_long_t : public monster_t
{
public:
	family_war_long_t();
	virtual ~family_war_long_t();

	// �Ƿ��Ǽ���ս����
	virtual bool is_family_war_long() { return true; }

	// ��ȡ����ָ��
	virtual family_war_long_ptr get_family_war_long() { return boost::dynamic_pointer_cast<family_war_long_t>(shared_from_this()); }

	// ��ȡ���󳡾�����
	virtual void get_object_info(proto::common::object_common_info* object_info);

	// �����佫
public:
	struct long_hero_info
	{
		uint32_t m_id;
		uint32_t m_curr_hp;	// ��ǰѪ��
		uint32_t m_max_hp;	// ���Ѫ��

		long_hero_info() : m_id(0), m_curr_hp(0), m_max_hp(0) {};
		long_hero_info(uint32_t id, uint32_t curr, uint32_t max) : m_id(id), m_curr_hp(curr), m_max_hp(max) {};
	};
	typedef std::map<uint32_t, long_hero_info> long_hero_map;

	typedef std::set<uint64_t> family_war_long_combat;

public:
	// ս�������ܽ���ս��ֱ�ӷ���0
	bool init_long(uint32_t monster_tid, const monster_common::monster_define_t& define, Monster* monster);
	virtual uint64_t fight_with_role(role_ptr p_role) { return 0; }

	// ��ȡ���Ѫ��
	virtual uint32_t get_max_hp() const { return m_max_hp; }
	virtual void set_max_hp(uint32_t value) { m_max_hp = value; }
	// ��ȡѪ��
	virtual uint32_t get_current_hp() const { return m_curr_hp; }
	virtual void set_current_hp(uint32_t value) { m_curr_hp = value; }

	// ���볡��
	virtual void on_enter_scene(scene_ptr p_scene);

	/**
	 * \brief ��������
	 * \param obj_killer:˭ɱ����
	 */
	virtual void set_object_die(uint64_t obj_killer = 0);
	/**
	 * \brief ��������
	 */
	virtual void on_object_die();

	// ���ò�ս����
	void set_family_war_family(family_war_side_type type, uint64_t family_id);

	// �Ƿ��ǲ�ս����
	family_war_side_type get_family_war_side_type(uint64_t family_id);

	// ս������
	bool get_hero_state_data(proto::common::hero_state_data* data);

	void add_combat(uint64_t id);

	void del_combat(uint64_t id);

	// ��������ս��ʧ��
	void set_all_fight_failed(uint64_t exception = 0);

	//��������ս���ɹ�
	void set_all_fight_success(uint64_t exception = 0);

	// ����Ѫ��
	uint32_t update_hero_hp(const scene_ptr& p_scene, family_war_side_type side_type, uint64_t fight_id, const proto::common::fight_camp& enemy_camp, const combat_ptr& p_combat);

	// ͬ������Ѫ��
	void update_all_combat(uint64_t exception = 0);

	//ͬ������ս��Ѫ��
	bool sync_all_boss_hp(uint64_t fight_id, const proto::common::hero_state_data& data);

	// ����
	family_war_side_type get_win_side();

	// �õ��˺��ٷְ�
	float get_hurt_pre(family_war_side_type side_type);

	// �õ��˺�����
	void peek_long_hurt_info(proto::client::gc_long_hurt_info_reply& reply);

	// �Ƿ��Ѿ���ʼ����
	bool is_family_occupied() const { return m_is_fight_end; }

	// ͬ��Ѫ��
	void broadcast_up(const scene_ptr& p_scene);

	void peek_hp_info(proto::client::gc_family_long_info_notify& ntf);

protected:
	long_hero_map m_hero_map;	// �佫��Ϣ
	uint32_t m_curr_hp = 0;
	uint32_t m_max_hp = 0;

	// Ѫ��ͬ����ʱ��
	common::cTimer m_sync_sec;

	family_war_long_combat m_field_boss_combat;

	// ��ս����
	uint64_t m_war_family[family_war_side_max] = { 0 };
	// �˺�
	uint32_t m_hurt_val[family_war_side_max] = { 0 };
	// ռ�����
	uint64_t m_occupy_family = 0;

	uint32_t m_revive_time = 0;

	bool m_is_fight_end = false;

protected:
	std::set<uint64_t> m_fight_uids;
};