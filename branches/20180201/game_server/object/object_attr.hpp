#ifndef __GAME_OBJECT_ATTR_H__
#define __GAME_OBJECT_ATTR_H__

#include "protos_fwd.hpp"
#include <boost/shared_ptr.hpp>
#include "utility.hpp"

USING_NS_COMMON;

struct hero_skill
{
	uint32_t skill_id = 0;
	uint32_t skill_level = 0;
	uint32_t skill_pos = 0;
	uint32_t skill_source = 0;
};

typedef std::map<uint32_t, hero_skill> HeroSkillMap;

class object_attr_t
{
public:
	object_attr_t();
	object_attr_t(uint32_t hero_id);
	virtual ~object_attr_t();

public:
	// 初始化武将
	void init_hero(uint32_t hero_id);
	// 计算属性
	void init_from_conf(uint32_t hero_id);
	
	bool is_role() { return m_type == 0; }

	// get set
public:
	uint64_t get_unique_id() { return m_uid; }
	void set_unique_id(uint64_t unique_id) { m_uid = unique_id; }

	void set_type(uint32_t type) { m_type = type; }
	uint32_t get_type() { return m_type; }

	// 经验
	uint32_t get_current_exp() { return m_exp; }
	void set_current_exp(uint32_t val) { m_exp = val; }
	uint32_t get_exp_today() { return m_exp_today; }
	void set_exp_today(uint32_t val) { m_exp_today = val; }
	// 等级
	uint32_t get_current_level() { return m_level; }
	void set_current_level(uint32_t val) { m_level = val; }

	void set_star(uint32_t val) { m_star = val; }
	uint32_t get_star() { return m_star; }
	void set_degree(uint32_t val) { m_degree = val; }
	uint32_t get_degree() { return m_degree; }
	void set_grade(uint32_t val) { m_grade = val; }
	uint32_t get_grade() { return m_grade; }
	void set_talent_count(uint32_t val) { m_talent_count = val; }
	uint32_t get_talent_count() { return m_talent_count; }
	void set_talent_level(uint32_t val) { m_talent_level = val; }
	uint32_t get_talent_level() { return m_talent_level; }
	void set_talent_point(uint32_t val) { m_talent_point = val; }
	uint32_t get_talent_point() { return m_talent_point; }
	void set_talent_use(uint32_t val) { m_talent_use = val; }
	uint32_t get_talent_use() { return m_talent_use; }
	void set_active_talent_use(uint32_t val) { m_active_talent_use = val; }
	uint32_t get_active_talent_use() { return m_active_talent_use; }
	
	uint32_t get_wakeup() { return m_wakeup; }
	void set_wakeup(uint32_t val) { m_wakeup = val; }

	void add_plugin(uint32_t plugin);
	bool set_current_plugin(uint32_t plugin);
	uint32_t get_current_plugin() { return m_current_plugin; }
	const std::vector<uint32_t>& get_plugin_list() { return m_plugins; }

	void set_add_attr(uint32_t attr_type, int32_t attr_value);
	void reset_add_attr();

	void set_hero_state(uint32_t state) { m_hero_state = state; }	// state:proto::common::hero_state_type
	uint32_t get_hero_state() { return m_hero_state; }

	void set_delete_flag(uint32_t flag) { m_delete_flag = flag; }
	uint32_t get_delete_flag() { return m_delete_flag; }

	void set_key(std::string key) { m_key = key; }
	std::string get_key() { return m_key; }

	void set_base_attr(const proto::common::base_attr& attr);
	void peek_base_attr(proto::common::base_attr* p_attr);

protected:
	std::string m_key = "";
	uint64_t m_uid = 0;
	uint32_t m_tid = 0;
	uint32_t m_type = 0;
	uint32_t m_exp = 0;
	uint32_t m_exp_today = 0;
	uint32_t m_level = 0;
	uint32_t m_star = 0;
	uint32_t m_degree = 0;
	uint32_t m_grade = 0;
	uint32_t m_talent_count = 0;
	uint32_t m_talent_level = 0;
	uint32_t m_talent_point = 0;
	uint32_t m_talent_use = 0;
	uint32_t m_active_talent_use = 0;
	uint32_t m_wakeup = 0;
	uint32_t m_current_plugin = 0;

	uint32_t m_absorb_damage = 0;
	uint32_t m_absorb_damage_percent = 0;
	uint32_t m_damage = 0;
	uint32_t m_damage_dec = 0;
	uint32_t m_reflect_damage = 0;
	uint32_t m_reflect_damage_percent = 0;
	uint32_t m_gender = 0;
	uint32_t m_hero_state = 0;
	uint32_t m_delete_flag = 0;
	std::vector<uint32_t>	m_plugins;
	std::vector<uint32_t>	m_fate_ids;
	std::map<uint32_t, int32_t>	m_add_attrs;
	HeroSkillMap m_hero_skills;
};
typedef boost::shared_ptr<object_attr_t> object_attr_ptr;

#endif // !__GAME_OBJECT_ATTR_H__
