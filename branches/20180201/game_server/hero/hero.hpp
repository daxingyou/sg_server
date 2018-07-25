#ifndef __GAME_HERO_H__
#define __GAME_HERO_H__
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include "object/object_attr.hpp"
#include <map>
#include "talent.h"
#include <vector>

typedef std::map<uint32_t, int> attr_map;

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
    hero_type_role = 0,
    hero_type_hero = 1,
    hero_type_monster = 2
};

enum equip_pos
{
	equip_pos_weapon = 0,	//武器
	equip_pos_armor = 1,	//盔甲
	equip_pos_talisman = 2,	//护身符
	equip_pos_end = 3,
};

class hero_t : public object_attr_t, public boost::enable_shared_from_this<hero_t>
{
public:
	hero_t();
	hero_t(uint64_t role_uid);
	hero_t(uint32_t hero_id, uint64_t unique_id, uint64_t role_uid);

	virtual ~hero_t();

	// 存储与读取
public:
	void load_data(const proto::common::hero_single& single);
	void peek_data(proto::common::hero_single* p_single, bool fight = false);
	void load_talent_data(const proto::common::talent_data& td);
	void peek_talent_data(proto::common::talent_data* p_data);
	void save_self(uint64_t role_uid, bool is_new = false);
    
public:
	uint64_t get_role_uid() { return m_role_uid; }
	uint32_t get_fighting() { return m_fighting; }
	// get set
	void add_talent(talent_ptr p_talent);
	talent_ptr add_talent(uint32_t tid, uint32_t level, uint32_t wake);
	talent_ptr get_talent(uint32_t tid);
	void reset_talent();

	void add_fate(uint32_t fate_id);
	bool has_fate(uint32_t fate_id, int32_t& fate_level);

	void levelup_fate(uint32_t fate_id, int32_t fate_level);

	void equip_on(uint32_t pos, uint64_t equip_id);
	void equip_off(uint32_t pos);
	uint64_t get_equip(uint32_t pos);
public:
	uint32_t get_tid() { return m_hero_id; }

public:
	int get_attr(uint32_t type);
	void calc_fighting();
	void calc_equip_attr(attr_map& attrs);
	uint32_t get_equip_count();
	//技能战斗力
	uint32_t get_skill_power();
	//天赋战斗力
	uint32_t get_talent_power();
	//获取装备技能
	const std::vector<uint32_t>& get_equip_skill() { return m_equip_skill_vec; }
	// 获取技能数据
	bool find_hero_skill(uint32_t skill_id, hero_skill& hero_skill_data);
	// 修改技能等级
	bool set_hero_skill_level(uint32_t skill_id, uint32_t level);
	// 修改技能位置
	bool set_hero_skill_pos(uint32_t skill_id, uint32_t pos);
	// 添加技能
	bool add_hero_skill(uint32_t skill_id, uint32_t level = 1);
	// 移除技能
	bool remove_hero_skill(uint32_t skill_id);
	// 替换技能
	bool replace_hero_skill(uint32_t old_skill_id, uint32_t new_skill_id);
	//获取技能列表
	void get_fight_skill_list(std::vector<hero_skill>& skill_vec);

	///时装属性
	void calc_plugin_attr(attr_map& attrs);
	///临时buff属性
	void calc_role_buff_attr(attr_map& attrs);
public:
	talent_map m_talent_map;
private:
	uint64_t m_role_uid = 0;
	uint32_t m_hero_id = 0;
	uint64_t m_equip[equip_pos_end] = {0};
	uint32_t m_fighting = 0;
	std::map<uint32_t, int32_t> m_fate_map;
	std::vector<uint32_t> m_equip_skill_vec;
};

typedef boost::shared_ptr<hero_t> hero_ptr;
typedef std::map<uint64_t, hero_ptr> hero_map;
#endif // !__GAME_HERO_H__
