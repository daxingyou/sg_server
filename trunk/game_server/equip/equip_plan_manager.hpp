#ifndef __GAME_EQUIP_PLAN_MANAGER_H__
#define __GAME_EQUIP_PLAN_MANAGER_H__

#include "protos_fwd.hpp"
#include "equip_plan.hpp"

class equip_plan_manager_t
{
public:
	equip_plan_manager_t(uint64_t uid);
	~equip_plan_manager_t();
public:
	void load_data(const proto::common::equip_plan_data& ld);
	void peek_data(proto::common::equip_plan_data* p_data);

	void save_self();

	uint64_t gen_new_equip_plan_id();
public:
	//创建装备方案
	uint32_t create_plan(const std::string& name, uint64_t weapon, uint64_t armor, uint64_t talisman);
	//修改装备方案
	uint32_t modify_plan(uint64_t uid, const std::string& name, uint64_t weapon, uint64_t armor, uint64_t talisman);
	//删除装备方案
	uint32_t delete_plan(uint64_t uid);
	//使用装备方案
	uint32_t use_plan(uint64_t equip_plan_uid, uint64_t hero_uid, proto::common::role_change_data* p_data);
	//检测装备方案
	void check_hero_plan(uint64_t hero_uid);
	//装备销毁后修改对应方案中的数据 由此导致的装备方案内容重复不做删除方案处理
	void del_equip(uint64_t equip_uid);
private:
	equip_plan_ptr get_equip_plan(uint64_t uid);

	void del_plan(uint64_t uid);

	bool has_same_name(const std::string& name);

	bool has_same_plan(uint64_t weapon, uint64_t armor, uint64_t talisman);

	uint32_t get_max_num();

	void use_plan_equip(uint64_t hero_uid, uint32_t equip_pos, uint64_t equip_uid, proto::common::role_change_data* p_data);

	void check_equip_hero(uint64_t equip_uid);

private:
	std::string m_key = "";
	uint64_t m_owner = 0;
	std::map<uint64_t, equip_plan_ptr> m_equip_plan_map;
};
typedef boost::shared_ptr<equip_plan_manager_t> equip_plan_mgr_ptr;
#endif
