#ifndef __WORLD_CUP_BATTLE_HPP__
#define __WORLD_CUP_BATTLE_HPP__

#include "server.pb.h"
#include "macros.hpp"
#include <boost/shared_ptr.hpp>
#include "world_cup_bet.hpp"
typedef std::map<uint64_t, world_cup_bet_ptr> world_cup_bet_map;

class world_cup_battle_t
{

public:
	world_cup_battle_t(){}
public:
	void load_data(const proto::common::world_cup_battle& battle_single);
	void peek_data(proto::common::world_cup_battle* p_battle_single);
	void load_bets();
	void peek_bet(uint64_t player_uid, proto::common::world_cup_bet* p_bet_single);
	void peek_odds_data(proto::common::world_cup_odds* p_data);
	// ����һ��ΨһID
	uint64_t get_new_bet_uid();
public:
	//Ͷע
	void bet(uint64_t player_id, uint64_t winner, uint32_t odds, uint32_t bet, proto::server::rg_world_cup_bet_reply& reply);
	//��ȡͶע����
	uint32_t bet_reward(uint64_t player_id, uint32_t& reward_bet);
	//Ͷע����
	void bet_result();
private:
	void calc_odds();
public:
	void set_uid(uint64_t val) { m_uid = val; }
	uint64_t get_uid() { return m_uid; }

	void set_round(uint32_t val) { m_round = val; }
	uint32_t get_round() { return m_round; }

	void set_battle_id(uint32_t val) { m_battle_id = val; }
	uint32_t get_battle_id() { return m_battle_id; }

	void set_player_a_uid(uint64_t val) { m_player_a_uid = val; }
	uint64_t get_player_a_uid() { return m_player_a_uid; }

	void set_player_b_uid(uint64_t val) { m_player_b_uid = val; }
	uint64_t get_player_b_uid() { return m_player_b_uid; }

	void set_player_a_score(uint32_t val) { m_player_a_score = val; }
	uint32_t get_player_a_score() { return m_player_a_score; }

	void set_player_b_score(uint32_t val) { m_player_b_score = val; }
	uint32_t get_player_b_score() { return m_player_b_score; }

	void set_winner_uid(uint64_t val) { m_winner_uid = val; }
	uint64_t get_winner_uid() { return m_winner_uid; }

	void set_state(uint32_t val) { m_state = val; }
	uint32_t get_state() { return m_state; }

	void set_odds_a(uint32_t val) { m_odds_a = val; }
	uint32_t get_odds_a() { return m_odds_a; }

	void set_odds_b(uint32_t val) { m_odds_b = val; }
	uint32_t set_odds_b() { return m_odds_b; }

	void set_bet_a(uint32_t val) { m_bet_a = val; }
	uint32_t set_bet_a() { return m_bet_a; }

	void set_bet_b(uint32_t val) { m_bet_b = val; }
	uint32_t get_bet_b() { return m_bet_b; }

	void set_vote_a(uint32_t val) { m_vote_a = val; }
	uint32_t set_vote_a() { return m_vote_a; }

	void set_vote_b(uint32_t val) { m_vote_b = val; }
	uint32_t get_vote_b() { return m_vote_b; }
public:
	//��СͶע
	uint32_t get_min_bet();
	//���Ͷע
	uint32_t get_max_bet();
	//����Ͷע������������ʵģ�
	uint32_t get_delta_bet();
private:
	uint64_t m_uid = 0;					//ΨһID
	uint32_t m_round = 0;				//�ִ� world_cup_round
	uint32_t m_battle_id = 0;			//��ս���(��һ��������Ե�)
	uint64_t m_player_a_uid = 0;		//���1
	uint64_t m_player_b_uid = 0;		//���2
	uint32_t m_player_a_score = 0;		//���1 �ķ���
	uint32_t m_player_b_score = 0;		//���2 �ķ���
	uint64_t m_winner_uid = 0;			//ʤ����
	uint32_t m_state = 0;				//world_cup_battle_state
	uint32_t m_odds_a = 0;				//Ͷע���1������
	uint32_t m_odds_b = 0;				//Ͷע���2������
	uint32_t m_bet_a = 0;				//Ͷע���1����ע
	uint32_t m_bet_b = 0;				//Ͷע���2����ע
	uint32_t m_vote_a = 0;				//Ͷע���1��������
	uint32_t m_vote_b = 0;				//Ͷע���2��������
	world_cup_bet_map m_bet_map;	
};
typedef boost::shared_ptr<world_cup_battle_t> world_cup_battle_ptr;
#endif