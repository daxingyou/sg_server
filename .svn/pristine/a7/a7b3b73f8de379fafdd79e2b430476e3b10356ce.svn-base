#ifndef __CENTER_OFFLINE_ARENA_MANAGER_H__
#define __CENTER_OFFLINE_ARENA_MANAGER_H__

#include "offline_arena.hpp"
#include "tblh/OfflineArenaClass.tbls.h"
#include "tblh/OfflineArenaMatch.tbls.h"
#include "tblh/OfflineArenaAward.tbls.h"

typedef std::map<uint64_t, offline_arena_ptr> offline_arena_map;
typedef std::map<uint32_t, offline_arena_ptr> offline_arena_map_by_rank;
typedef std::map<uint32_t, offline_arena_map_by_rank> offline_arena_map_by_class;
#define MAX_ARENA_CLASS 4

class offline_arena_manager_t
{
public:
	offline_arena_manager_t(){}
	~offline_arena_manager_t(){}
public:
	static void load_data();

	static void one_day();

	static void award(uint32_t id, bool is_in_time);
private:
	static void add_arena(offline_arena_ptr p_arena);
	//新角色加入
	static offline_arena_ptr add_new_arena(uint64_t role_uid);
	//改变排名
	static void change_rank(offline_arena_ptr p_arena, uint32_t cur_class, uint32_t rank);
public:
	//获取竞技数据
	static void peek_data(uint64_t role_uid, proto::client::ec_offline_arena_data_reply& reply);
	//获取玩家阵容
	static void peek_enemy_data(uint64_t role_uid, proto::common::mirror_fight_data* p_data);
	//获取排行榜数据
	static void peek_rank_data(uint64_t role_uid, uint32_t cur_class, proto::client::ec_offline_arena_rank_reply& reply);
	//获取战报数据
	static void peek_record_data(uint64_t role_uid, proto::common::offline_arena_record_data* p_data);
	//请求攻打
	static uint32_t fight(uint64_t self_uid, uint32_t self_rank, uint32_t enemy_type, uint64_t enemy_id, uint32_t enemy_rank);
	//攻打结果
	static void fight_result(uint64_t role_uid, uint32_t is_win);
	//领取历史最高排名奖励
	static void record_award(uint64_t role_uid);
	//购买攻打次数
	static uint32_t buy(uint64_t role_uid, uint32_t money);
	//布阵通知
	static void form_notify(uint64_t role_uid, const proto::common::mirror_fight_data& form_data);
	//玩家等级变化
	static void update_role_level(uint64_t role_uid, uint32_t level);
	//GM命令改变排行
	static void gm_rank(uint64_t role_uid, uint32_t rank);
private:
	//根据等级获取分段配置
	static OfflineArenaClass* get_class_conf_by_level(uint32_t level);
	//根据排名获取匹配配置
	static OfflineArenaMatch* get_match_conf_by_rank(uint32_t rank);
	//获取指定排名的概况信息
	static void peek_general_data(uint32_t cur_class, uint32_t rank, proto::common::offline_arena_general* p_single);
	//获取指定排名的机器人概况信息
	static void peek_robot_general_data(uint32_t cur_class, uint32_t rank, proto::common::offline_arena_general* p_single);
	//获取指定排名的机器人
	static uint32_t get_robot_by_rank(uint32_t cur_class, uint32_t rank);

public:
	//根据分段排名获取奖励配置
	static OfflineArenaAward* get_award_conf_by_rank(uint32_t cur_class, uint32_t rank);
	//最大攻打次数
	static uint32_t get_fight_count();
	//购买的攻打次数
	static uint32_t get_buy_fight_count();
	//挑战前10名的最低排名
	static uint32_t get_fight_top_ten_rank();
private:
	static offline_arena_map m_offline_arena_map;				//所有的数据 以role_uid为KEY
	static offline_arena_map_by_class m_offline_arena_map_by_class;	//所有排名数据 以class为一级key，以rank为二级key
	static std::map<uint32_t, uint32_t> s_buy_info;				//购买信息
};

#endif