#ifndef __CENTER_FIGHT_BOSS_MANAGER_H__
#define __CENTER_FIGHT_BOSS_MANAGER_H__

#include <map>
#include <vector>

#include "field_boss.hpp"
#include "server.pb.h"
#include "user/global_user.hpp"

class field_boss_manager_t
{
public:
	typedef std::vector<filed_boss_ptr> filed_boss_vec;
	typedef std::map<uint32_t, filed_boss_vec> filed_boss_id_map;

	static bool init();
	static void close_clear();
	static void reload();

	//////////////////////////////////////////////////////////////////////////
	// 查找boss
	static filed_boss_ptr find_field_boss(uint32_t id, uint64_t object_id);
	// 清除所有
	static void clear_all();

	// 跨天
	static void new_day(uint32_t cur_time);
	// 加载今天数据
	static void load_day(uint32_t cur_time);

public:
	// 当天boss信息
	static void get_all_boss_info(proto::client::ec_field_boss_info_notify& notify);
	// 召唤boss返回
	static bool summon_boss_result(uint32_t id, uint64_t object_id, bool result);
	// boss被击杀
	static void field_boss_killed(uint32_t id, uint64_t object_id, const std::string& killer);
	// boss战奖励发放
	static void field_boss_rank_reward(const proto::server::ge_field_boss_rank_notify& ntf);

public:
	// gm召唤一个boss
	static void gm_summon_field_boss(uint32_t id);

protected:
	static filed_boss_id_map m_filed_boss_id_map;
};

#endif