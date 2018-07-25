#ifndef __GAME_KING_WAR_COMMON_H__
#define __GAME_KING_WAR_COMMON_H__

#include "macros.hpp"

#include "common.pb.h"

#include <boost/shared_ptr.hpp>
#include <list>
#include "utility.hpp"

enum king_war_side_type : uint32_t
{
	king_war_side_attack = 0,	// 进攻方
	king_war_side_defence = 1,	// 防守方

	king_war_side_max,		// 最大值
	king_war_side_none = king_war_side_max,	// 不是参战方
};

enum king_war_obj_type : uint32_t
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
	uint32_t m_scene_id = 0;
};

// 场景中龙脉信息
struct king_war_long_pulse
{
	uint64_t m_object_id = 0;
	uint32_t m_area_id = 0;
	king_war_obj_type m_long_type = king_war_obj_type_none;
};

// 战斗双方
struct king_war_family
{
	uint64_t m_family_id = 0;
	std::string m_family_name;
	uint64_t m_help_family = 0;
	std::string m_helper_name;

	void reset() {
		m_family_id = 0;
		m_family_name.clear();
		m_help_family = 0;
		m_helper_name.clear();
	}

	king_war_family& operator =(const king_war_family& rf)
	{
		this->m_family_id = rf.m_family_id;
		this->m_family_name = rf.m_family_name;
		this->m_help_family = rf.m_help_family;
		this->m_helper_name = rf.m_helper_name;

		return *this;
	}
};

struct king_war_scene_info_t
{
	king_war_family m_fight_family[king_war_side_max];
	uint32_t m_family_long_gas[king_war_side_max] = { 0 };

	void reset()
	{
		for (uint32_t i = 0; i < king_war_side_max; ++i)
		{
			m_fight_family[i].reset();
			m_family_long_gas[i] = 0;
		}
	}

	// 是否是参战家族
	uint32_t get_king_war_side_type(uint64_t family_id)
	{
		for (uint32_t i = king_war_side_attack; i < king_war_side_max; ++i) {
			if (family_id == m_fight_family[i].m_family_id || family_id == m_fight_family[i].m_help_family) {
				return i;
			}
		}

		return king_war_side_none;
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

struct king_war_role_t
{
	explicit king_war_role_t(uint64_t uid) : m_uid(uid) {};

	uint64_t m_uid = 0;								// uid
	uint32_t m_exploit = 0;							// 战功
	uint32_t m_kill_count = 0;						// 击杀数
	uint32_t m_kills = 0;							// 连续击杀数
	em_kill_title m_kill_title = em_kill_title_0;	// 没称号
	uint32_t m_warfare = 0;							// 战意
	uint32_t m_free_revive_count = 0;				// 免费的立即复活次数
	uint32_t m_rank = 0;							// 排名

	uint32_t get_exploit() const { return m_exploit; }
	void add_exploit(uint32_t value) 
	{ 
		m_exploit += value;
		m_rank_single.set_exploits(m_exploit);
	}

	uint32_t get_kill_count() const { return m_kill_count; }
	void add_kill_count() 
	{ 
		m_kill_count += 1;
		m_rank_single.set_kill_count(m_kill_count);
	}

	uint32_t get_death_count() const { return m_rank_single.death_count(); }
	void add_death_count() { m_rank_single.set_death_count(m_rank_single.death_count() + 1); }

	uint32_t get_free_revive_count() const { return m_free_revive_count; }

	uint32_t get_continued_kill() const { return m_kills; }
	void clear_continued_kill() { m_kills = 0; }
	void add_continued_kill() 
	{
		m_kills += 1;

		if (m_kills > m_rank_single.continued_kill()) {
			m_rank_single.set_continued_kill(m_kills);
		}
	}

	proto::common::king_war_rank_single m_rank_single;
};

typedef boost::shared_ptr<king_war_role_t> king_war_role_ptr;
typedef std::map<uint64_t, king_war_role_ptr> king_war_role_map;
typedef std::list<king_war_role_ptr> king_war_rank_list;

// 王城战信息
struct country_scene_info_t
{
	uint32_t m_country_id = 0;
	uint64_t m_scene_id = 0;			// 王城战场景
	proto::common::king_war_state m_king_war_state = proto::common::king_war_state_end;
	king_war_scene_info_t m_king_war_info;
	king_war_role_map m_king_war_role_map;
	king_war_rank_list m_king_war_rank_list;
	uint32_t m_next_sort_time = 0;
	bool m_is_old_rank_list = true;
	proto::common::king_war_rank_list m_rank_list;

	void king_war_end()
	{
		m_king_war_state = proto::common::king_war_state_end;
		m_king_war_info.reset();
		m_king_war_rank_list.clear();
		m_king_war_role_map.clear();
		m_next_sort_time = 0;
	}

	void reset_all()
	{
		king_war_end();
		m_is_old_rank_list = true;
		m_rank_list.Clear();
	}

	void sort_rank_list()
	{
		if (!m_king_war_rank_list.empty()) {
			m_king_war_rank_list.sort([](const king_war_role_ptr& h1, const king_war_role_ptr& h2) {
				if (NULL == h1 || NULL == h2) {
					return false;
				}

				if (h1->m_exploit > h2->m_exploit) {
					return true;
				} else if (h1->m_exploit == h2->m_exploit) {
					if (h1->m_kill_count != h2->m_kill_count) {
						return h1->m_kill_count > h2->m_kill_count;
					} else {
						return h1->m_uid > h2->m_uid;
					}
				}

				return false;
			});
		}

		m_next_sort_time = common::time_util_t::now_time() + 5;
		m_is_old_rank_list = true;
	}

	const king_war_rank_list& get_rank_list()
	{
		if (common::time_util_t::now_time() > m_next_sort_time) {
			sort_rank_list();
		}

		return m_king_war_rank_list;
	}

	void update_rank_list()
	{
		m_rank_list.Clear();

		for (king_war_rank_list::const_iterator citr = m_king_war_rank_list.begin();
			citr != m_king_war_rank_list.end(); ++citr) {
			const king_war_role_ptr& p_king_war_role = *citr;
			if (p_king_war_role) {
				proto::common::king_war_rank_single* p_single = m_rank_list.add_ranks();
				if (p_single) {
					p_single->CopyFrom(p_king_war_role->m_rank_single);
				}
			}
		}

		m_is_old_rank_list = false;
	}

	const proto::common::king_war_rank_list& get_king_war_ranks()
	{
		if (m_is_old_rank_list) {
			update_rank_list();
		}

		return m_rank_list;
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
	uint32_t m_buff_tid = 0;		// buff tid
	uint32_t m_king_war_side = king_war_side_none;

	em_artifacts_state m_state = em_artifacts_none;

	uint32_t m_next_time = 0;		// 下一个神器时间
	uint32_t m_start_left = 0;		// 神器刷新前一分钟

	void reset()
	{
		m_artifacts_tid = 0;	// tid
		m_artifacts_id = 0;	// obj id
		m_buff_tid = 0;	// role id
		m_king_war_side = king_war_side_none;

		m_state = em_artifacts_none;

		m_next_time = 0;		// 下一个神器时间
		m_start_left = 0;
	}
};

#endif
