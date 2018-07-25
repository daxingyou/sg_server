#ifndef __WORLD_CUP_MGR_HPP__
#define __WORLD_CUP_MGR_HPP__

#include <map>
#include "server.pb.h"
#include "macros.hpp"
#include <boost/shared_ptr.hpp>
#include "world_cup_battle.hpp"
#include <boost/asio/deadline_timer.hpp>
typedef std::map<uint64_t, world_cup_battle_ptr> world_cup_battle_map;
typedef std::map<uint64_t, proto::common::role_cross_data> cross_role_map;
class world_cup_manager_t
{

public:
	static void		init();
	// 生成一个唯一ID
	static uint64_t get_new_battle_uid();

public:
	static void peek_data(proto::common::world_cup_data* p_data);
	static void peek_bet_data(proto::server::re_world_cup_bet_data_reply& reply, uint64_t player_uid);
	static void peek_odds_data(proto::common::world_cup_odds* p_data, uint64_t battle_uid);
	//参赛
	static uint32_t play(uint64_t player_id, const proto::common::role_cross_data& rcd);
	//投注
	static void bet(uint64_t player_id, uint64_t battle_uid, uint64_t winner, uint32_t odds, uint32_t bet, proto::server::rg_world_cup_bet_reply& reply);
	//领取投注返还
	static uint32_t bet_reward(uint64_t player_id, uint64_t battle_uid, uint32_t& reward_bet);
	//生成对阵
	static void make_battle();
	//下一轮
	static void next_round(const boost::system::error_code& ec);
	//开赛前十五分钟
	static void comming_quarter(const boost::system::error_code& ec);
	//开赛前十五分钟间隔
	static void comming_quarter_interval(const boost::system::error_code& ec);
	//开赛前五分钟
	static void comming_soon(const boost::system::error_code& ec);
	//开赛前五分钟间隔
	static void comming_soon_interval(const boost::system::error_code& ec);
	//开赛前三十秒
	static void ready(const boost::system::error_code& ec);
	//开打
	static void fight(const boost::system::error_code& ec);
	//下一场比赛
	static void next_match(const boost::system::error_code& ec);
	//战斗结果
	static void battle_result(uint64_t winner_id, uint64_t loser_id);
private:
	static uint32_t get_before_play_time();
	static uint32_t get_comming_quarter_time();
	static uint32_t get_comming_soon_time();
	static uint32_t get_ready_time();
	static uint32_t get_battle_time();
	static uint32_t get_battle_interval();
	static uint32_t get_wait_time();
	static uint32_t get_interval_time();
	static uint32_t get_winner_score();
	static bool check_over();
	static void restart();
private:
	static world_cup_battle_map battle_map;
	static uint32_t current_round;
	static cross_role_map player_map;
	static boost::shared_ptr<boost::asio::deadline_timer> m_timer;
	static boost::shared_ptr<boost::asio::deadline_timer> m_interval_timer;
};
#endif