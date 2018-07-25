#ifndef __ARENA_SCORE_RANK_MGR_HPP__
#define __ARENA_SCORE_RANK_MGR_HPP__

#include <map>
#include "common.pb.h"
#include "macros.hpp"
#include <boost/shared_ptr.hpp>
#include <list>
#include "MyTuple.h"
class arena_rank_info_t;
typedef boost::shared_ptr<arena_rank_info_t> arena_rank_info_ptr;

// 武将攻略展示数量
#define hero_strategy_show_num 5

class arena_rank_info_t
{
public:
	arena_rank_info_t();
	~arena_rank_info_t();
public:
	void set_role_uid(uint64_t role_uid) { m_role_uid = role_uid; }
	uint64_t get_role_uid() { return m_role_uid; }

	void set_score(uint32_t score) { m_score = score; }
	uint32_t get_score() { return m_score; }

	void set_stage(uint32_t val) { m_stage = val; }
	uint32_t get_stage() { return m_stage; }

	void set_time(uint32_t val) { m_time = val; }
	uint32_t get_time() { return m_time; }

	void set_server_id(uint32_t val) { m_server_id = val; }
	uint32_t get_server_id() { return m_server_id; }

	void set_name(const std::string& val) { m_name = val; }
	const std::string& get_name() { return m_name; }

	void set_server_name(const std::string& val) { m_server_name = val; }
	const std::string& get_server_name() { return m_server_name; }

	void set_fighting_value(uint32_t val) { m_fighting_value = val; }
	uint32_t get_fighting_value() { return m_fighting_value; }

	bool has_hero(uint32_t hero_tid);

	void set_hero_data(const proto::common::hero_data& hero_data);
	void set_hero_equip(const proto::common::item_data& equip_data);
	const proto::common::hero_data& get_hero_data() { return m_fight_heroes; }
	const proto::common::item_data& get_hero_equip() { return m_hero_equips; }
	void peek_hero_strategy_info(uint32_t hero_tid, uint32_t role_rank, proto::common::hero_strategy_single *p_hero_strategy_info);
// 	void clear_fight_hero();
// 	void add_fight_hero(uint32_t hero_type, uint32_t hero_id, uint32_t hero_grade);
public:
	void save_self();
	void peek_data(proto::common::arena_role_rank_info* p_data);
private:
	uint64_t m_role_uid = 0; 		//玩家uid
	uint32_t m_score = 0;			//玩家竞技积分
	uint32_t m_stage = 0;			//玩家段位
	uint32_t m_time = 0;			//最近积分更新时间
	uint32_t m_server_id = 0;		//玩家所在服id
	std::string m_server_name = "";	//玩家所在服名称
	std::string m_name = "";		//玩家名称
//	std::vector<Dawn::triple<uint32_t,uint32_t,uint32_t>> m_fight_heroes;	//战斗的武将
	proto::common::hero_data m_fight_heroes;	//战斗的武将
	proto::common::item_data m_hero_equips;		//武将的装备
	uint32_t m_fighting_value = 0;	// 战斗力
};

struct rank_sort
{
	bool operator()(const arena_rank_info_ptr p1, const arena_rank_info_ptr p2)
	{
		if (NULL == p1 || NULL == p2)
		{
			return true;
		}
		if (p1->get_score() > p2->get_score())
		{
			return true;
		}
		else if (p1->get_score() == p2->get_score())
		{
			return p1->get_role_uid() < p2->get_role_uid();
		}
		return false;
	}
};

typedef std::list<arena_rank_info_ptr> rank_list;
typedef std::map<uint64_t, arena_rank_info_ptr> rank_map;

class arena_score_rank_mgr_t
{
public:
    static uint32_t update_arena_score_rank(uint64_t role_uid, uint32_t score, uint32_t stage, uint32_t server_id, 
										const char *server_name, const char *name, uint32_t fighting_value, 
										const proto::common::hero_data &hero_info,
										const proto::common::item_data &hero_equip);
    static void save_history_score_rank();
    static arena_rank_info_ptr find_user_arena_info(uint64_t role_uid);
    static uint32_t get_current_num();
    static uint32_t get_rank(uint64_t role_uid);
    static bool load_from_redis(uint32_t time_id);
	static bool get_rank_data(uint32_t rank_start_pos, uint32_t server_id, uint64_t uid);
	static void get_16th_final_player(std::vector<uint64_t>& player_vec);
	static uint32_t reset_stage(uint32_t& stage);
	static void reset_stage();
	static rank_list& get_arena_score_rank() { return m_arena_score_rank_list; }
private:
    static rank_list m_arena_score_rank_list;
	static rank_map m_arena_score_rank_map;
};

#endif
