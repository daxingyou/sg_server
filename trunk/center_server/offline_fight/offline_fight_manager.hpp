#ifndef __CENTER_OFFLINE_FIGHT_MANAGER_H__
#define __CENTER_OFFLINE_FIGHT_MANAGER_H__

#include "macros.hpp"

#include "server.pb.h"
#include "common.pb.h"
#include <boost/enable_shared_from_this.hpp>
#include <map>
#include <vector>
#include "tblh/Match.tbls.h"
#include "user/global_user.hpp"

enum stage_level
{
	stage_level_b = 1,
	stage_level_a = 2,
	stage_level_s = 3,
	stage_level_ss = 4,
	stage_level_sss = 5,
};

typedef std::map<uint32_t, std::vector<uint64_t>> offline_fight_map;
class offline_fight_manager_t
{
public:
	offline_fight_manager_t(){}
	~offline_fight_manager_t(){}

	static void load_data();
	static void one_day();

	static void update_fight_data(uint64_t role_uid, uint32_t fight_value, proto::common::mirror_fight_data* p_fight_data);
	static void make_conf();
	static void clear_conf();
	static void get_friend_recommend(uint32_t fighting, std::vector<uint64_t>& uids);
public:
	static void make_fight_data(uint32_t level, proto::server::eg_expedition_fight_data_reply& reply);
	static void refresh_fight_data(uint32_t level, proto::server::eg_expedition_refresh_reply& reply);
	static void refresh_stage_data(uint32_t level, uint32_t stage_id, proto::server::eg_expedition_refresh_stage_reply& reply);
	static uint32_t expedition_help(global_user_ptr p_user, const std::vector<uint64_t>& helper_list);
private:
	static void make_stage_data(const std::vector<uint32_t>& fight_val, proto::common::expedition_stage_single* p_data);
	static uint32_t get_stage_enemy_count();
private:
	static offline_fight_map m_offline_fight_map;
	static std::map<uint32_t, Match*> m_match_confs;
	static std::map<uint32_t, uint32_t> m_match_monster;
};

#endif