#ifndef __GAME_HERO_H__
#define __GAME_HERO_H__
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include "object/object_attr.hpp"
#include <map>
#include "talent.h"
#include <vector>

typedef std::map<uint32_t, int> attr_map;
typedef std::pair<uint64_t, proto::common::EM_SCENEOBJECT_TYPE> object_id_type;

enum hero_atk_type
{
	hero_atk_type_physics	= 1,
	hero_atk_type_magic		= 2,
};

enum hero_atk_mode
{
	hero_atk_mode_melee		= 1,
	hero_atk_mode_ranged	= 2,
};

class formation_t;
enum hero_type_t
{
	// ����
    hero_type_role = 0,
	// Ӣ��
    hero_type_hero = 1,
	// ����
    hero_type_monster = 2,
	// �ٻ���
	hero_type_summon = 3,
};

enum equip_pos
{
	equip_pos_weapon = 0,	//����
	equip_pos_armor = 1,	//����
	equip_pos_talisman = 2,	//�����
	equip_pos_end = 3,
};

class hero_t : public object_attr_t, public boost::enable_shared_from_this<hero_t>
{
public:
	hero_t();
	hero_t(object_id_type role_uid);
	hero_t(uint32_t hero_id, uint64_t unique_id, object_id_type role_uid);

	virtual ~hero_t();

	// �洢���ȡ
public:
	void load_data(const proto::common::hero_single& single);
	void peek_data(proto::common::hero_single* p_single, bool fight = false, bool b_isdb = false);
	void load_talent_data(const proto::common::talent_data& td);
	void peek_talent_data(proto::common::talent_data* p_data);
	void save_self(uint64_t role_uid);
    
public:
	object_id_type get_role_uid() { return m_role_uid; }
	uint32_t get_fighting() { return m_fighting; }
	// get set
	void add_talent(talent_ptr p_talent);
	talent_ptr add_talent(uint32_t tid, uint32_t level);
	talent_ptr get_talent(uint32_t tid);
	void reset_talent();

	bool equip_on(uint32_t pos, uint64_t equip_id);
	bool equip_off(uint32_t pos);
	uint64_t get_equip(uint32_t pos);

	bool is_main_hero();

	//����
	bool can_reset();

	void reset();

	uint32_t get_hero_type() const { return m_hero_type; }
public:
	uint32_t get_tid() { return m_hero_id; }

public:
	int get_attr(uint32_t type);
	void calc_fighting();
	void calc_equip_attr(attr_map& attrs);
	uint32_t get_equip_count();
	//����ս����
	uint32_t get_skill_power();
	//�츳ս����
	uint32_t get_talent_power();
	//��ȡװ������
	const std::vector<uint32_t>& get_equip_skill() { return m_equip_skill_vec; }
	// ��ȡ��������
	bool find_hero_skill(uint32_t skill_id, hero_skill& hero_skill_data);
	// �޸ļ��ܵȼ�
	bool set_hero_skill_level(uint32_t skill_id, uint32_t level);
	// �޸ļ���λ��
	bool set_hero_skill_pos(uint32_t skill_id, uint32_t pos);
	//��ȡ�����б�
	void get_fight_skill_list(std::vector<hero_skill>& skill_vec);

	///ʱװ����
	void calc_plugin_attr(attr_map& attrs);
	///��ʱbuff����
	void calc_role_buff_attr(attr_map& attrs);
	//�츳����
	void calc_talent_attr(attr_map& attrs);
	//��������
	void calc_famous_attr(attr_map& attrs);
	//���弼������
	void calc_family_skill_attr(attr_map& attrs);
	//��������
	void calc_lifestar_attr(attr_map& attrs);
	//�ռ�ͼ������
	void calc_handbook_attr(attr_map& attrs);
public:
	talent_map m_talent_map;
private:
	object_id_type m_role_uid;
	uint32_t m_hero_id = 0;
	uint32_t m_hero_type = 0;
	uint64_t m_equip[equip_pos_end] = {0};
	uint32_t m_fighting = 0;
	std::vector<uint32_t> m_equip_skill_vec;
};

typedef boost::shared_ptr<hero_t> hero_ptr;
typedef std::map<uint64_t, hero_ptr> hero_map;
#endif // !__GAME_HERO_H__
