#ifndef __GAME_ROLE_DUNGEON_DATA_H__
#define __GAME_ROLE_DUNGEON_DATA_H__

#include "macros.hpp"
#include "protos_fwd.hpp"
#include <boost/smart_ptr/shared_ptr.hpp>
#include "tblh/DungeonTable.tbls.h"
#include <map>

/**
 * \brief 人物副本数据
 */

class role_t;
typedef boost::shared_ptr<role_t> role_ptr;

struct dungeon_times_data_t
{
	uint32_t dungeon_type = 0;
	uint32_t remain_fight_time = 0;
	uint32_t today_buy_time = 0;
};
typedef boost::shared_ptr<dungeon_times_data_t> dungeon_times_data_ptr;
typedef std::map<uint32_t, dungeon_times_data_ptr> dungeon_times_data_map;

class role_dungeon_data_t
{
public:
	role_dungeon_data_t(uint64_t uid);
	~role_dungeon_data_t();

public:
	struct dungeon_data_t
	{
		uint32_t dungeon_id = 0;
		uint32_t total_pass_time = 0;
		uint32_t refresh_time = 0;
		std::map<uint32_t, uint32_t> rwds;
		uint32_t best_score = 0;
		uint32_t best_pass_time = 0;
		uint32_t best_death_value = 0;
	};

	typedef boost::shared_ptr<dungeon_data_t> dungeon_data_ptr;
	typedef std::map<uint32_t, dungeon_data_ptr> dungeon_data_map;

public:
	void load_data(const proto::common::role_all_dungeon_data& data);
	void peek_data(const role_ptr& p_role, proto::common::role_all_client_dungeon_data* p_data);
	void save_data(dungeon_data_ptr p_dungeon_data, bool is_right_now = false);

	void load_times_data(const proto::common::role_all_dungeon_times_data& data);
	void save_times_data();
	void one_day();
public:
	dungeon_data_ptr get_dungeon_data(uint32_t dungeon_id);

	dungeon_times_data_ptr get_dungeon_times_data(uint32_t dungeon_id);
	dungeon_times_data_ptr add_dungeon_times_data(uint32_t dungeon_id);
	void add_fight_times(uint32_t dungeon_type);
	// 通关次数
	uint32_t get_dungeon_pass_time(uint32_t dungeon_id);
	// 增加通关次数
	void add_dungeon_pass(uint32_t dungeon_id);

	// 得到历史最好分数
	uint32_t get_best_score(uint32_t dungeon_id);
	uint32_t get_best_pass_time(uint32_t dungeon_id);
	uint32_t get_best_death_value(uint32_t dungeon_id);

	// 更新历史最好成绩
	void set_best_score(uint32_t dungeon_id, uint32_t best_score);
	void set_best_pass_time(uint32_t dungeon_id, uint32_t best_pass_time);
	void set_best_death_value(uint32_t dungeon_id, uint32_t best_death_value);

	// 副本中怪物被击杀次数
	uint32_t get_monster_kill_times(uint32_t dungeon_id, uint32_t monster_tid);
	void add_one_kill_times(DungeonTable* p_dungeontable, uint32_t monster_tid);

public:
	void gm_pass_all_dungeon();

protected:
	uint64_t m_role_uid = 0;
	dungeon_data_map m_dungeon_data_map;
	std::string	m_times_key = "";
	dungeon_times_data_map m_dungeon_times_data_map;
};

#endif
