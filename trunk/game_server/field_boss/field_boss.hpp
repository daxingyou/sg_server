#ifndef __GAME_WORLD_BOSS_H__
#define __GAME_WORLD_BOSS_H__

#include "monster/monster.hpp"

#include <map>
#include <vector>
#include <boost/asio/deadline_timer.hpp>
#include "server.pb.h"
#include "fight/combat.hpp"

class field_boss_t : public monster_t
{
public:
	field_boss_t();
	virtual ~field_boss_t();

	// ����
	virtual void del_clear();

	//�Ƿ���Ұ��boss
	virtual bool is_field_boss() { return true; }

	// ��ȡ����ָ��
	virtual field_boss_ptr get_field_boss() { return boost::dynamic_pointer_cast<field_boss_t>(shared_from_this()); }

	// ��ȡ���󳡾�����
	virtual void get_object_info(proto::common::object_common_info* object_info);

public:
	// ���������Ұ
	virtual void other_object_into_vision(const object_id_type& object);
	// ֪ͨ�뿪��Ұ
	virtual void other_object_out_of_vision(const object_id_type& object);
	// ���볡��
	virtual void on_enter_scene(scene_ptr p_scene);
	// �뿪����
	virtual void on_leave_scene(scene_ptr p_scene);

public:
	struct boss_role_info
	{
		uint64_t role_id = 0;			// ����id
		std::string role_name;			// ����
		uint32_t hurt_time = 0;			// ��ɵ��˺�ʱ��
		uint32_t hurt = 0;				// �˺�
		uint32_t leave_fight_time = 0;	// �뿪ս��ʱ��

		// ����true������ǰ��
		bool operator< (const boss_role_info& item) const
		{
			if (this->hurt > item.hurt) {
				return false;
			}

			if (this->hurt == item.hurt) {
				if (this->hurt_time != item.hurt_time) {
					return this->hurt_time > item.hurt_time;
				} else {
					return this->role_id < item.role_id;
				}
			}
			return true;
		}
	};
	// 2������������ָ�룬ɾ����ʱ��һ��ɾ��
	typedef boost::shared_ptr<boss_role_info> boss_role_info_ptr;
	typedef std::vector<boss_role_info_ptr> boss_role_vec;	// ����������
	typedef std::map<uint64_t, boss_role_info_ptr> boss_role_info_map;

	struct monster_hero_info
	{
		uint32_t m_id;
		uint32_t m_curr_hp;	// ��ǰѪ��
		uint32_t m_max_hp;	// ���Ѫ��

		monster_hero_info() : m_id(0), m_curr_hp(0), m_max_hp(0) {};
		monster_hero_info(uint32_t id, uint32_t curr, uint32_t max) : m_id(id), m_curr_hp(curr), m_max_hp(max) {};
	};

	typedef std::map<uint32_t, monster_hero_info> monster_hero_map;
	// ��ǰ�Ͳμ�bossս��id
	typedef std::set<uint64_t> field_boss_combat;
	// �����˺�����
	typedef std::map<uint64_t, uint32_t> family_hurt_map;

public:
	// �����ʼ��
	virtual bool init_monster(const monster_common::monster_define_t& define, Monster* monster, uint64_t object_id = 0);
	virtual uint64_t fight_with_role(role_ptr p_role) { return 0; };
	// ��սidʵʱ����
	virtual uint32_t get_against_form_id();

	// ��ȡ���Ѫ��
	virtual uint32_t get_max_hp() const { return m_boss_max_hp; }
	virtual void set_max_hp(uint32_t value) { m_boss_max_hp = value; }
	// ��ȡѪ��
	virtual uint32_t get_current_hp() const { return m_boss_curr_hp; }
	virtual void set_current_hp(uint32_t value) { m_boss_curr_hp = value; }

	virtual void on_tick(uint64_t msecs);

public:
	// �������ñ�id
	void set_id(uint32_t id) { m_id = id; }
	uint32_t get_id() const { return m_id; }

	// �õ�һ����ҵ��˺���Ϣ
	boss_role_info_ptr get_boss_role_info(uint64_t uid);
	boss_role_info_ptr add_boss_role_info(uint64_t uid);

	// boss �˺�����
	void fight_role_hurt(uint64_t fight_id, const proto::common::fight_camp& self_camp, const proto::common::fight_camp& enemy_camp, const combat_ptr& combat);
	void update_all_combat(uint64_t exception = 0);
	bool get_hero_state_data(proto::common::hero_state_data* data);
	bool sync_all_boss_hp( uint64_t fight_id,  const proto::common::hero_state_data& data);

	// ����
	void calc_persion_rank(proto::server::EM_FIELD_MODE mode, proto::server::ge_field_boss_rank_notify& ntf);
	// ��������
	void calc_family_rank(proto::server::EM_FIELD_MODE mode, proto::server::ge_field_boss_rank_notify& ntf);

	// ����boss��ȴ
	uint32_t get_fight_cd_time(uint64_t uid);

	void add_combat(uint64_t id);
	void del_combat(uint64_t id);
	void leave_fight(uint64_t fight_id, const proto::common::fight_camp& self_camp);
	// ��������ս��ʧ��
	void set_all_fight_failed(uint64_t exception = 0);
	//��������ս���ɹ�
	void set_all_fight_success(uint64_t exception = 0);
	// ���û����
	void field_boss_end(proto::server::EM_FIELD_MODE mode);
	// ���Ž���
	void start_settlement(proto::server::EM_FIELD_MODE mode);

protected:
	uint32_t m_id = 0;		// ���ñ�id
	uint32_t m_against_form_id = 0;
	monster_hero_map m_hero_map;
	uint32_t m_boss_curr_hp = 0;
	uint32_t m_boss_max_hp = 0;

	boss_role_vec m_boss_role_vec;
	boss_role_info_map m_boss_role_map;
	field_boss_combat m_field_boss_combat;
	family_hurt_map m_family_hurt_map;
	std::vector<uint64_t> m_killers;	// ��ɱ�߿��ܶ������Ϊ���
	std::string m_killer_name;

	// ����ս���Ƿ����
	bool m_is_fight_end = false;
protected:
	// ��ʱ��
	bool m_is_need_sync = false;
	common::cTimer m_sync_sec;
};

#endif