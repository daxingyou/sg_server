#ifndef __GAME_KING_WAR_COMMON_H__
#define __GAME_KING_WAR_COMMON_H__

#include "macros.hpp"

#include "common.pb.h"

#include <boost/shared_ptr.hpp>

enum king_war_side_type
{
	king_war_side_attack = 0,	// 进攻方
	king_war_side_defence = 1,	// 防守方

	king_war_side_max,		// 最大值
	king_war_side_none = king_war_side_max,	// 不是参战方
};

enum king_war_obj_type
{
	king_war_obj_long = 0,		// 龙王
	king_war_obj_long1 = 1,		// 囚牛
	king_war_obj_long2 = 2,		// 蒲牢

	king_war_obj_type_max,		// 最大值
	king_war_obj_type_none = king_war_obj_type_max,	// 无效类型
};

// 复活点
struct king_war_relive_side
{
	uint32_t m_area_id = 0;
	uint32_t m_jump_id = 0;
};

// 场景中龙脉信息
struct king_war_long_pulse
{
	uint64_t m_object_id = 0;
	uint32_t m_area_id = 0;
	king_war_obj_type m_long_type = king_war_obj_type_none;
};

struct king_war_scene_info_t
{
	uint64_t m_family_id[king_war_side_max] = { 0 };
	std::string m_family_name[king_war_side_max];
	uint32_t m_family_long_gas[king_war_side_max] = { 0 };

	void reset()
	{
		for (uint32_t i = 0; i < king_war_side_max; ++i)
		{
			m_family_id[i] = 0;
			m_family_name[i].clear();
			m_family_long_gas[0] = 0;
		}
	}
};

enum em_kill_title
{
	em_kill_title_0 = 0,	// 没有
	em_kill_title_1	= 1,	// 大杀特杀
	em_kill_title_2 = 2,	// 所向披靡
	em_kill_title_3 = 3,	// 无人能挡
	em_kill_title_4 = 4,	// 万夫莫敌
};

#define WARFARE_DEFAULT		20	// 默认战意

struct king_war_role_t
{
	uint64_t m_uid = 0;								// uid
	uint32_t m_exploit = 0;							// 战功
	uint32_t m_kills = 0;							// 连续击杀数
	uint32_t m_max_kills = 0;						// 最大连杀
	uint32_t m_all_kills = 0;						// 总的击杀数
	em_kill_title m_kill_title = em_kill_title_0;	// 没称号
	uint32_t m_warfare = 0;							// 战意
	uint32_t m_death_count = 0;						// 死亡次数
	uint32_t m_free_revive_count = 0;				// 免费的立即复活次数
	uint32_t m_rank = 0;							// 排名
};

typedef boost::shared_ptr<king_war_role_t> king_war_role_ptr;
typedef std::map<uint64_t, king_war_role_ptr> king_war_role_map;

// 王城战信息
struct country_scene_info_t
{
	uint32_t m_country_id = 0;
	uint64_t m_scene_id = 0;			// 王城战场景
	proto::common::king_war_state m_king_war_state = proto::common::king_war_state_end;
	king_war_scene_info_t m_king_war_info;
	king_war_role_map m_king_war_role_map;

	void reset()
	{
		m_king_war_state = proto::common::king_war_state_end;
		m_king_war_info.reset();
		m_king_war_role_map.clear();
	}
};

// 神器阶段
enum em_artifacts_state
{
	em_artifacts_none	= 0,	// 默认阶段
	em_artifacts_ready	= 1,	// 5分钟后召唤申请
	em_artifacts_pre	= 2,	// 刷新前一分钟
	em_artifacts_appear = 3,	// 出现
	em_artifacts_collected = 4,	// 被采集
	em_artifacts_start = 5,		// 启动时间
};

// 神器信息
struct artifacts_info_t
{
	uint32_t m_artifacts_tid = 0;	// tid
	uint64_t m_artifacts_id = 0;	// obj id
	uint64_t m_role_artifacts = 0;	// role id
	uint32_t m_area_id = 0;			// 神器区域

	em_artifacts_state m_state = em_artifacts_none;

	uint32_t m_next_time = 0;		// 下一个神器时间

	void reset()
	{
		m_artifacts_tid = 0;	// tid
		m_artifacts_id = 0;	// obj id
		m_role_artifacts = 0;	// role id
		m_area_id = 0;

		m_state = em_artifacts_none;

		m_next_time = 0;		// 下一个神器时间
	}
};

#endif
