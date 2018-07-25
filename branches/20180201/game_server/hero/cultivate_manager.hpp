#ifndef __GAME_CULTIVATE_MANAGER_H__
#define __GAME_CULTIVATE_MANAGER_H__
#include <map>
#include "Singleton.h"
#include "role/role.hpp"
#include "hero.hpp"

#define FATE_NUM 4
#define RESET_TALENT_FREE_LEVEL 50
#define RESET_TALENT_CONSUME_GOLD 100
				
enum RECRUIT_COND_TYPE
{
	RECRUIT_COND_TYPE_ROLELEVEL = 3,
	RECRUIT_COND_TYPE_PREHERO = 4,
};

class cultivate_manager_t
{

public:
	static bool init();
	static uint32_t get_star_item();
	static uint32_t get_talent_stone();
	static uint32_t get_init_talent_count();
	static uint32_t get_plugin_star();
	// 武将养成
	static uint32_t cultivate_hero(role_ptr p_role, const proto::client::cg_hero_cultivate_request& msg, uint32_t& out, proto::common::role_change_data* p_data);
	static uint32_t levelup_hero(role_ptr p_role, hero_ptr p_hero, uint32_t item_id, uint32_t item_num, proto::common::role_change_data* p_data);
	static uint32_t starup_hero(role_ptr p_role, hero_ptr p_hero, proto::common::role_change_data* p_data);
	static uint32_t advance_hero(role_ptr p_role, hero_ptr p_hero, proto::common::role_change_data* p_data);
	static uint32_t levelup_skill(role_ptr p_role, hero_ptr p_hero, uint32_t skill_id, proto::common::role_change_data* p_data);
	static uint32_t wakeup_hero(role_ptr p_role, hero_ptr p_hero, proto::common::role_change_data* p_data);
	static uint32_t unlock_skill(role_ptr p_role, hero_ptr p_hero, uint32_t skill_id, proto::common::role_change_data* p_data);
	static uint32_t replace_skill(role_ptr p_role, hero_ptr p_hero, uint32_t old_skill_id, uint32_t new_skill_id, proto::common::role_change_data* p_data);

	static uint32_t recruit_hero(role_ptr p_role, uint32_t hero_tid, proto::common::role_change_data* p_data);
	// 获取未招募的武将
	static void get_unrecruit_hero(role_ptr p_role, std::vector<uint32_t>& tids);
private:
	static std::map<uint32_t, uint32_t> levelup_item;
};

#endif // !__GAME_CULTIVATE_MANAGER_H__