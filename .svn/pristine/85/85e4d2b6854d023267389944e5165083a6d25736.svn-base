#ifndef __EQUIP_PLAN_HPP__
#define __EQUIP_PLAN_HPP__

#include "protos_fwd.hpp"
#include "macros.hpp"

class equip_plan_t 
{
public:
	equip_plan_t(){}
	equip_plan_t(uint64_t uid);
	~equip_plan_t();
public:
	uint64_t get_uid() { return m_uid; }

	const std::string& get_name() { return m_name; }
	void set_name(const std::string& val) { m_name = val; }
	uint64_t get_weapon() { return m_weapon; }
	void set_weapon(uint64_t val) { m_weapon = val; }
	uint64_t get_armor() { return m_armor; }
	void set_armor(uint64_t val) { m_armor = val; }
	uint64_t get_talisman() { return m_talisman; }
	void set_talisman(uint64_t val) { m_talisman = val; }
	uint64_t get_hero_uid() { return m_hero_uid; }
	void set_hero_uid(uint64_t val) { m_hero_uid = val; }
public:
	void peek_data(proto::common::equip_plan_single* p_data);
	void load_data(const proto::common::equip_plan_single& eps);
private:
	uint64_t m_uid = 0;
	std::string m_name = "";
	uint64_t m_weapon = 0;
	uint64_t m_armor = 0;
	uint64_t m_talisman = 0;
	uint64_t m_hero_uid = 0;
};
typedef boost::shared_ptr<equip_plan_t> equip_plan_ptr;
#endif