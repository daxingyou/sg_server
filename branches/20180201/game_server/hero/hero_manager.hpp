#ifndef __GAME_HERO_MANAGER_H__
#define __GAME_HERO_MANAGER_H__
#include "role/role.hpp"
#include "hero.hpp"

enum unlock_hero_plugin_type : uint32_t
{
	unlock_plugin_default = 0,
	unlock_plugin_wakeup  = 1,
	unlock_plugin_shop    = 2,
};

// 武将类型
enum hero_type
{
	hero_type_normal = 0,			// 普通
	hero_type_story = 1,			// 剧情
};

class hero_manager_t
{
	// about db
public:
	// load datas from db
	static void load_all_datas(role_ptr p_role, const proto::common::hero_data& data);
	// send datas to client
	static void peek_all_datas(role_ptr p_role, proto::common::hero_data* p_data);
	// save datas to db
	static void save_all_datas(role_ptr p_role);

	static void create_hero(role_ptr p_role, const proto::common::hero_single& single);
public:
	// 添加主角武将
	static void add_role_hero(role_ptr p_role, uint32_t hero_job, bool is_notify = true);
	// 添加武将
	static void add_hero(role_ptr p_role, uint32_t hero_tid, uint32_t source_type, proto::common::role_change_data* p_data = NULL, uint32_t type = 1, uint32_t state = 0, bool is_notify = true, proto::common::luckydraw_award* la = NULL);
	// 添加武将
	static void add_story_hero(role_ptr p_role, uint32_t hero_tid, uint32_t level);
	// 添加武将
	static void add_hero(role_ptr p_role, hero_ptr p_hero, uint32_t source_type, proto::common::role_change_data* p_data, bool is_new = false);

	// 获取武将
	static hero_ptr get_hero(role_ptr p_role, uint32_t hero_id);
	// 获取武将
	static hero_ptr get_hero_by_unique_id(role_ptr p_role, uint64_t unique_id);
    //获取主角武将
	static hero_ptr get_role_hero(role_ptr p_role);
    //获取所有武将
	static void get_all_heroes(role_ptr p_role, std::vector<hero_ptr>& res);
	//获取所有武将(包含隐藏武将)
	static void get_all_heroes_include_hide(role_ptr p_role, std::vector<hero_ptr>& res);
	//获取所有武将(战力排行)
	static void get_all_heroes_by_fight_value(role_ptr p_role, std::vector<hero_ptr>& res);
	// 战力s排序
	static bool hero_fight_value_sort(const hero_ptr& h1, const hero_ptr& h2);
	// 删除武将(目前只有剧情用) wys 2017.10.19
	static void remove_story_hero(role_ptr p_role, uint32_t hero_tid);
	static void remove_hero(role_ptr p_role, uint32_t hero_tid, uint32_t source_type, proto::common::role_change_data* p_data = NULL);

	// 获取武将唯一Id
	static uint64_t gen_hero_uid();

	// 是否有武将
	static bool has_hero(role_ptr p_role, uint64_t uid);

	// 是否拥有该plugin
	static bool has_plugin(role_ptr p_role,uint32_t hero_id,uint32_t plugin_id);

	static uint32_t change_hero_plugin(role_ptr p_role, uint32_t hero_id, uint32_t plugin_id, proto::common::role_change_data* p_data = NULL);

	static uint32_t get_hero_full_equip_count(role_ptr p_role);

	static void check_hero_wakeup_add_plugin(const uint64_t role_uid,const uint32_t hero_id);

	// 添加经验
	static bool add_exp(role_ptr p_role, uint64_t hero_uid, uint32_t exp, uint32_t source_type, bool isNotify, proto::common::role_change_data* p_data = NULL);
	// PVE获得经验
	static void add_pve_exp(role_ptr p_role, uint64_t hero_uid, uint32_t exp, proto::client::gc_combat_result_ntf* p_data = NULL);
	// 解锁主角武将
	static uint32_t unlock_main_hero(role_ptr p_role, uint32_t hero_tid, proto::common::role_change_data* p_data);
	// 切换主角
	static void change_role_main_hero(role_ptr p_role, uint32_t hero_tid);
public:
	static void calc_fate_info(role_ptr p_role, hero_ptr p_hero);
	static void do_next_day(role_ptr p_role);

public:
	static uint32_t get_hero_level_limit();
};

#endif // !__GAME_HERO_MANAGER_H__
