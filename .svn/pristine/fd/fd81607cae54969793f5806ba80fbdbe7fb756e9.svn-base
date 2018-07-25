#include "world_cup_manager.hpp"
#include "main/area_server.hpp"
#include "main/area_server_fwd.hpp"
#include "config/config_manager.hpp"
#include "role/arena_role_manager.hpp"
#include "tblh/errcode_enum.hpp"
#include "global_id.hpp"
#include "common/redis_client.hpp"
#include "arena/arena_score_rank_mgr.hpp"
USING_NS_NETWORK;
USING_NS_COMMON;

world_cup_battle_map world_cup_manager_t::battle_map;
cross_role_map world_cup_manager_t::player_map;
uint32_t world_cup_manager_t::current_round;
boost::shared_ptr<boost::asio::deadline_timer> world_cup_manager_t::m_timer;
boost::shared_ptr<boost::asio::deadline_timer> world_cup_manager_t::m_interval_timer;

void world_cup_manager_t::init()
{
	uint32_t season = 1;
	current_round = 0;

	std::vector<std::uint64_t> uids;
	redis_client_t::get_keys_by_field_condition<uint64_t, uint32_t, std::vector<std::uint64_t>>("world_cup", "season", season, uids);

	for (auto& uid : uids)
	{
		world_cup_battle_ptr p_battle = world_cup_battle_ptr(new world_cup_battle_t);
		p_battle->set_uid(uid);
		p_battle->set_round(redis_client_t::get_uint32("world_cup", "round", uid));
		p_battle->set_battle_id(redis_client_t::get_uint32("world_cup", "battle_id", uid));
		p_battle->set_player_a_uid(redis_client_t::get_uint64("world_cup", "player_a", uid));
		p_battle->set_player_b_uid(redis_client_t::get_uint64("world_cup", "player_b", uid));
		p_battle->set_player_a_score(redis_client_t::get_uint32("world_cup", "score_a", uid));
		p_battle->set_player_b_score(redis_client_t::get_uint32("world_cup", "score_b", uid));
		p_battle->set_winner_uid(redis_client_t::get_uint64("world_cup", "winner", uid));
		p_battle->set_state(redis_client_t::get_uint32("world_cup", "state", uid));
		p_battle->set_odds_a(redis_client_t::get_uint32("world_cup", "odds_a", uid));
		p_battle->set_odds_b(redis_client_t::get_uint32("world_cup", "odds_b", uid));
		p_battle->set_bet_a(redis_client_t::get_uint32("world_cup", "bet_a", uid));
		p_battle->set_bet_b(redis_client_t::get_uint32("world_cup", "bet_b", uid));
		p_battle->set_vote_a(redis_client_t::get_uint32("world_cup", "vote_a", uid));
		p_battle->set_vote_b(redis_client_t::get_uint32("world_cup", "vote_b", uid));
		battle_map.insert(std::make_pair(uid, p_battle));
		if (p_battle->get_round() > current_round && p_battle->get_state() == proto::common::world_cup_battle_state_end)
		{
			current_round = p_battle->get_round();
		}
		p_battle->load_bets();
	}
	//make_battle();
}

uint64_t world_cup_manager_t::get_new_battle_uid()
{
	return g_id_allocator.gen_new_id(ID_ALLOCATOR_TYPE_WORLD_CUP);
}

void world_cup_manager_t::peek_data(proto::common::world_cup_data* p_data)
{
	if (NULL == p_data)
	{
		log_error("NULL == p_data");
		return;
	}
	for (auto it : battle_map)
	{
		world_cup_battle_ptr p_battle = it.second;
		if (NULL == p_battle)
		{
			continue;
		}
		proto::common::world_cup_battle* p_single = p_data->add_battle_list();
		p_battle->peek_data(p_single);
	}
}

void world_cup_manager_t::peek_bet_data(proto::server::re_world_cup_bet_data_reply& reply, uint64_t player_uid)
{
	for (auto it : battle_map)
	{
		world_cup_battle_ptr p_battle = it.second;
		if (NULL == p_battle)
		{
			continue;
		}
		proto::common::world_cup_bet* p_bet_single = reply.add_bet_data();
		p_battle->peek_bet(player_uid, p_bet_single);
	}
}

void world_cup_manager_t::peek_odds_data(proto::common::world_cup_odds* p_data, uint64_t battle_uid)
{
	auto it = battle_map.find(battle_uid);
	if (it == battle_map.end())
	{
		log_error("battle[%lu] is null", battle_uid);
		return;
	}
	auto p_battle = it->second;
	if (NULL == p_battle)
	{
		log_error("battle[%lu] is null", battle_uid);
		return;
	}
	p_battle->peek_odds_data(p_data);
}

uint32_t world_cup_manager_t::play(uint64_t player_id, const proto::common::role_cross_data& rcd)
{
	if (current_round != proto::common::world_cup_round_16th_final)
	{
		log_error("current_round[%d] invalid", current_round);
		return errcode_enum::notice_unknown;
	}
	if (player_map.find(player_id) != player_map.end())
	{
		log_error("player[%llu] already play", player_id);
		return errcode_enum::notice_unknown;
	}
	bool bfind = false;
	for (auto it : battle_map)
	{
		world_cup_battle_ptr p_battle = it.second;
		if (NULL == p_battle)
		{
			continue;
		}
		if (p_battle->get_state() != proto::common::world_cup_battle_state_none)
		{
			continue;
		}
		if (p_battle->get_round() != current_round)
		{
			continue;
		}
		uint64_t player_a_uid = p_battle->get_player_a_uid();
		uint64_t player_b_uid = p_battle->get_player_b_uid();
		if (player_a_uid == player_id || player_b_uid == player_id)
		{
			bfind = true;
			break;
		}
	}
	if (!bfind)
	{
		log_error("player[%llu] invalid", player_id);
		return errcode_enum::notice_unknown;
	}
	player_map.insert(std::make_pair(player_id, rcd));
	return errcode_enum::error_ok;
}

void world_cup_manager_t::bet(uint64_t player_id, uint64_t battle_uid, uint64_t winner, uint32_t odds, uint32_t bet, proto::server::rg_world_cup_bet_reply& reply)
{
	auto it = battle_map.find(battle_uid);
	if (it == battle_map.end())
	{
		log_error("battle[%lu] is null", battle_uid);
		reply.set_reply_code(errcode_enum::notice_unknown);
		return;
	}
	auto p_battle = it->second;
	if (NULL == p_battle)
	{
		log_error("battle[%lu] is null", battle_uid);
		reply.set_reply_code(errcode_enum::notice_unknown);
		return;
	}
	p_battle->bet(player_id, winner, odds, bet, reply);
}

uint32_t world_cup_manager_t::bet_reward(uint64_t player_id, uint64_t battle_uid, uint32_t& reward_bet)
{
	auto it = battle_map.find(battle_uid);
	if (it == battle_map.end())
	{
		log_error("battle[%lu] is null", battle_uid);
		return errcode_enum::notice_unknown;
	}
	auto p_battle = it->second;
	if (NULL == p_battle)
	{
		log_error("battle[%lu] is null", battle_uid);
		return errcode_enum::notice_unknown;
	}
	return p_battle->bet_reward(player_id, reward_bet);
}

void world_cup_manager_t::make_battle()
{
	WORLDCUP_LOG("make_battle round[%d]", current_round);
	//获取参战玩家列表
	std::vector<uint64_t> player_vec;
	if (current_round == 0)
	{
		//第一轮从赛季排行获取
		arena_score_rank_mgr_t::get_16th_final_player(player_vec);
		if (player_vec.size() != 32)
		{
			log_error("get_16th_final_player error, player_vec.size()[%d] != 32", player_vec.size());
			return;
		}
		////测试代码 随机添加一个在线参赛者
		//proto::common::role_cross_data rcd;
		//uint32_t rand_index = rand() % 32;
		//player_map.insert(std::make_pair(player_vec[rand_index], rcd));
		//第一轮需要打乱顺序
		std::random_shuffle(player_vec.begin(), player_vec.end());
	}
	else
	{
		//获取下一轮参加的玩家数量
		uint32_t player_count = 32 >> current_round;
		if (player_count == 1)
		{
			//结束了，冠军已经产生
			WORLDCUP_LOG("world_cup is over");
			restart();
			return;
		}

		player_vec.assign(player_count, 0);
		for (auto it : battle_map)
		{
			world_cup_battle_ptr p_battle = it.second;
			if (NULL == p_battle)
			{
				continue;
			}
			if (p_battle->get_round() != current_round)
			{
				continue;
			}
			if (p_battle->get_state() != proto::common::world_cup_battle_state_end)
			{
				WORLDCUP_LOG("p_battle[%llu] is not end, round[%d]", p_battle->get_uid(), current_round);
				continue;
			}
			uint32_t battle_id = p_battle->get_battle_id();
			if (battle_id >= player_count)
			{
				log_error("round[%d] make_battle error, battle_id[%d] invalid", current_round+1, battle_id);
				return;
			}
			player_vec[battle_id] = p_battle->get_winner_uid();
		}
	}
	//根据玩家列表生成对阵
	for (uint32_t i = 0; i < player_vec.size()/2; ++i)
	{
		world_cup_battle_ptr p_battle = world_cup_battle_ptr(new world_cup_battle_t);
		uint64_t uid = get_new_battle_uid();
		p_battle->set_uid(uid);
		p_battle->set_round(current_round + 1);
		p_battle->set_battle_id(i);
		p_battle->set_player_a_uid(player_vec[2*i]);
		p_battle->set_player_b_uid(player_vec[2 * i + 1]);
		p_battle->set_state(proto::common::world_cup_battle_state_bet);
		p_battle->set_odds_a(10);
		p_battle->set_odds_b(10);
		battle_map.insert(std::make_pair(uid, p_battle));
		WORLDCUP_LOG("make battle[%llu] player_a[%llu] player_b[%llu]", uid, p_battle->get_player_a_uid(), p_battle->get_player_b_uid());
		arena_rank_info_ptr p_role_a = arena_score_rank_mgr_t::find_user_arena_info(p_battle->get_player_a_uid());
		arena_rank_info_ptr p_role_b = arena_score_rank_mgr_t::find_user_arena_info(p_battle->get_player_b_uid());
		if (NULL != p_role_a && NULL != p_role_b)
		{
			proto::server::re_world_cup_mail_notify ntf_a;
			ntf_a.set_player_name(p_role_b->get_name());
			ntf_a.set_server_id(p_role_a->get_server_id());
			ntf_a.set_round(p_battle->get_round());
			env::server->send_msg_to_transfer(op_cmd::re_world_cup_mail_notify, p_battle->get_player_a_uid(), ntf_a);

			proto::server::re_world_cup_mail_notify ntf_b;
			ntf_b.set_player_name(p_role_a->get_name());
			ntf_b.set_server_id(p_role_b->get_server_id());
			ntf_b.set_round(p_battle->get_round());
			env::server->send_msg_to_transfer(op_cmd::re_world_cup_mail_notify, p_battle->get_player_b_uid(), ntf_b);
		}
	}
	WORLDCUP_LOG("battle_map size[%d]", battle_map.size());
	if (NULL == m_timer)
	{
		m_timer.reset(new boost::asio::deadline_timer(env::server->get_ios()));
	}
	m_timer->cancel();
	if (current_round == 0)
	{
		m_timer->expires_from_now(boost::posix_time::seconds(get_before_play_time()));
		m_timer->async_wait(boost::bind(&world_cup_manager_t::comming_quarter, boost::asio::placeholders::error));
	}
	else
	{
		m_timer->expires_from_now(boost::posix_time::seconds(get_interval_time()));
		m_timer->async_wait(boost::bind(&world_cup_manager_t::ready, boost::asio::placeholders::error));
	}
	++current_round;

	//广播到所有center
	proto::server::re_world_cup_data_reply reply;
	reply.set_server_id(0);
	peek_data(reply.mutable_world_cup_info());
	env::server->send_msg_to_transfer(op_cmd::re_world_cup_data_reply, 0, reply);
}

void world_cup_manager_t::next_round(const boost::system::error_code& ec)
{
	if (ec != 0)
	{
		return;
	}
	WORLDCUP_LOG("next_round");
	if (NULL != m_interval_timer)
	{
		m_interval_timer->cancel();
	}
	if (NULL != m_timer)
	{
		m_timer->cancel();
	}

	if (check_over())
	{
		make_battle();
	}
	else
	{
		if (NULL == m_timer)
		{
			m_timer.reset(new boost::asio::deadline_timer(env::server->get_ios()));
		}
		m_timer->expires_from_now(boost::posix_time::seconds(get_wait_time()));
		m_timer->async_wait(boost::bind(&world_cup_manager_t::next_round, boost::asio::placeholders::error));
	}
}

void world_cup_manager_t::comming_quarter(const boost::system::error_code& ec)
{
	if (ec != 0)
	{
		return;
	}
	WORLDCUP_LOG("comming_quarter");
	if (NULL != m_timer)
	{
		m_timer->cancel();
	}
	
	//通知所有参加玩家
	for (auto it : battle_map)
	{
		world_cup_battle_ptr p_battle = it.second;
		if (NULL == p_battle)
		{
			continue;
		}
		if (p_battle->get_state() != proto::common::world_cup_battle_state_none)
		{
			continue;
		}
		if (p_battle->get_round() != current_round)
		{
			continue;
		}
		uint64_t player_a_uid = p_battle->get_player_a_uid();
		arena_rank_info_ptr p_role_a = arena_score_rank_mgr_t::find_user_arena_info(player_a_uid);
		if (NULL != p_role_a)
		{
			proto::server::re_world_cup_coming_quarter_notify ntf_a;
			ntf_a.set_server_id(p_role_a->get_server_id());
			ntf_a.set_time_remain(get_comming_quarter_time());
			env::server->send_msg_to_transfer(op_cmd::re_world_cup_coming_quarter_notify, player_a_uid, ntf_a);
		}

		uint64_t player_b_uid = p_battle->get_player_b_uid();
		arena_rank_info_ptr p_role_b = arena_score_rank_mgr_t::find_user_arena_info(player_b_uid);
		if (NULL != p_role_b)
		{
			proto::server::re_world_cup_coming_quarter_notify ntf_b;
			ntf_b.set_server_id(p_role_b->get_server_id());
			ntf_b.set_time_remain(get_comming_quarter_time());
			env::server->send_msg_to_transfer(op_cmd::re_world_cup_coming_quarter_notify, player_b_uid, ntf_b);
		}
	}
	if (NULL == m_interval_timer)
	{
		m_interval_timer.reset(new boost::asio::deadline_timer(env::server->get_ios()));
	}
	m_interval_timer->expires_from_now(boost::posix_time::seconds(get_interval_time()));
	m_interval_timer->async_wait(boost::bind(&world_cup_manager_t::comming_quarter_interval, boost::asio::placeholders::error));
	if (NULL == m_timer)
	{
		m_timer.reset(new boost::asio::deadline_timer(env::server->get_ios()));
	}
	m_timer->expires_from_now(boost::posix_time::seconds(get_comming_quarter_time()));
	m_timer->async_wait(boost::bind(&world_cup_manager_t::comming_soon, boost::asio::placeholders::error));
}

void world_cup_manager_t::comming_quarter_interval(const boost::system::error_code& ec)
{
	if (ec != 0)
	{
		return;
	}
	WORLDCUP_LOG("comming_quarter_interval");
	if (NULL != m_interval_timer)
	{
		m_interval_timer->cancel();
	}
	
	//通知所有参加玩家
	for (auto it : battle_map)
	{
		world_cup_battle_ptr p_battle = it.second;
		if (NULL == p_battle)
		{
			continue;
		}
		if (p_battle->get_state() != proto::common::world_cup_battle_state_none)
		{
			continue;
		}
		if (p_battle->get_round() != current_round)
		{
			continue;
		}
		uint64_t player_a_uid = p_battle->get_player_a_uid();
		arena_rank_info_ptr p_role_a = arena_score_rank_mgr_t::find_user_arena_info(player_a_uid);
		if (NULL != p_role_a)
		{
			proto::server::re_world_cup_coming_quarter_notify ntf_a;
			ntf_a.set_server_id(p_role_a->get_server_id());
			ntf_a.set_time_remain(time_util_t::get_remain_time(m_timer));
			env::server->send_msg_to_transfer(op_cmd::re_world_cup_coming_quarter_notify, player_a_uid, ntf_a);
		}

		uint64_t player_b_uid = p_battle->get_player_b_uid();
		arena_rank_info_ptr p_role_b = arena_score_rank_mgr_t::find_user_arena_info(player_b_uid);
		if (NULL != p_role_b)
		{
			proto::server::re_world_cup_coming_quarter_notify ntf_b;
			ntf_b.set_server_id(p_role_b->get_server_id());
			ntf_b.set_time_remain(time_util_t::get_remain_time(m_timer));
			env::server->send_msg_to_transfer(op_cmd::re_world_cup_coming_quarter_notify, player_b_uid, ntf_b);
		}
	}
	if (NULL == m_interval_timer)
	{
		m_interval_timer.reset(new boost::asio::deadline_timer(env::server->get_ios()));
	}
	m_interval_timer->expires_from_now(boost::posix_time::seconds(get_interval_time()));
	m_interval_timer->async_wait(boost::bind(&world_cup_manager_t::comming_quarter_interval, boost::asio::placeholders::error));
}

void world_cup_manager_t::comming_soon(const boost::system::error_code& ec)
{
	if (ec != 0)
	{
		return;
	}
	WORLDCUP_LOG("comming_soon");
	if (NULL != m_interval_timer)
	{
		m_interval_timer->cancel();
	}
	if (NULL != m_timer)
	{
		m_timer->cancel();
	}
	
	//通知所有参加玩家
	for (auto it : battle_map)
	{
		world_cup_battle_ptr p_battle = it.second;
		if (NULL == p_battle)
		{
			continue;
		}
		if (p_battle->get_state() != proto::common::world_cup_battle_state_none)
		{
			continue;
		}
		if (p_battle->get_round() != current_round)
		{
			continue;
		}
		uint64_t player_a_uid = p_battle->get_player_a_uid();
		arena_rank_info_ptr p_role_a = arena_score_rank_mgr_t::find_user_arena_info(player_a_uid);
		if (NULL != p_role_a)
		{
			proto::server::re_world_cup_coming_soon_notify ntf_a;
			ntf_a.set_server_id(p_role_a->get_server_id());
			ntf_a.set_time_remain(get_comming_soon_time());
			env::server->send_msg_to_transfer(op_cmd::re_world_cup_coming_soon_notify, player_a_uid, ntf_a);
		}

		uint64_t player_b_uid = p_battle->get_player_b_uid();
		arena_rank_info_ptr p_role_b = arena_score_rank_mgr_t::find_user_arena_info(player_b_uid);
		if (NULL != p_role_b)
		{
			proto::server::re_world_cup_coming_soon_notify ntf_b;
			ntf_b.set_server_id(p_role_b->get_server_id());
			ntf_b.set_time_remain(get_comming_soon_time());
			env::server->send_msg_to_transfer(op_cmd::re_world_cup_coming_soon_notify, player_b_uid, ntf_b);
		}
	}
	if (NULL == m_interval_timer)
	{
		m_interval_timer.reset(new boost::asio::deadline_timer(env::server->get_ios()));
	}
	m_interval_timer->expires_from_now(boost::posix_time::seconds(get_interval_time()));
	m_interval_timer->async_wait(boost::bind(&world_cup_manager_t::comming_soon_interval, boost::asio::placeholders::error));

	if (NULL == m_timer)
	{
		m_timer.reset(new boost::asio::deadline_timer(env::server->get_ios()));
	}
	m_timer->expires_from_now(boost::posix_time::seconds(get_comming_soon_time()));
	m_timer->async_wait(boost::bind(&world_cup_manager_t::ready, boost::asio::placeholders::error));
}

void world_cup_manager_t::comming_soon_interval(const boost::system::error_code& ec)
{
	if (ec != 0)
	{
		return;
	}
	WORLDCUP_LOG("comming_soon_interval");
	if (NULL != m_interval_timer)
	{
		m_interval_timer->cancel();
	}
	
	//通知所有参加玩家
	for (auto it : battle_map)
	{
		world_cup_battle_ptr p_battle = it.second;
		if (NULL == p_battle)
		{
			continue;
		}
		if (p_battle->get_state() != proto::common::world_cup_battle_state_none)
		{
			continue;
		}
		if (p_battle->get_round() != current_round)
		{
			continue;
		}
		uint64_t player_a_uid = p_battle->get_player_a_uid();
		arena_rank_info_ptr p_role_a = arena_score_rank_mgr_t::find_user_arena_info(player_a_uid);
		if (NULL != p_role_a)
		{
			proto::server::re_world_cup_coming_soon_notify ntf_a;
			ntf_a.set_server_id(p_role_a->get_server_id());
			ntf_a.set_time_remain(time_util_t::get_remain_time(m_timer));
			env::server->send_msg_to_transfer(op_cmd::re_world_cup_coming_soon_notify, player_a_uid, ntf_a);
		}

		uint64_t player_b_uid = p_battle->get_player_b_uid();
		arena_rank_info_ptr p_role_b = arena_score_rank_mgr_t::find_user_arena_info(player_b_uid);
		if (NULL != p_role_b)
		{
			proto::server::re_world_cup_coming_soon_notify ntf_b;
			ntf_b.set_server_id(p_role_b->get_server_id());
			ntf_b.set_time_remain(time_util_t::get_remain_time(m_timer));
			env::server->send_msg_to_transfer(op_cmd::re_world_cup_coming_soon_notify, player_b_uid, ntf_b);
		}
	}
	if (NULL == m_interval_timer)
	{
		m_interval_timer.reset(new boost::asio::deadline_timer(env::server->get_ios()));
	}
	m_interval_timer->expires_from_now(boost::posix_time::seconds(get_interval_time()));
	m_interval_timer->async_wait(boost::bind(&world_cup_manager_t::comming_soon_interval, boost::asio::placeholders::error));
}

void world_cup_manager_t::ready(const boost::system::error_code& ec)
{
	if (ec != 0)
	{
		return;
	}
	WORLDCUP_LOG("ready");
	if (NULL != m_interval_timer)
	{
		m_interval_timer->cancel();
	}
	if (NULL != m_timer)
	{
		m_timer->cancel();
	}
	//通知所有参加玩家
	//通知所有参加玩家
	for (auto it : battle_map)
	{
		world_cup_battle_ptr p_battle = it.second;
		if (NULL == p_battle)
		{
			continue;
		}
		if (p_battle->get_state() != proto::common::world_cup_battle_state_bet)
		{
			continue;
		}
		if (p_battle->get_round() != current_round)
		{
			continue;
		}
		p_battle->set_state(proto::common::world_cup_battle_state_playing);
		uint64_t player_a_uid = p_battle->get_player_a_uid();
		arena_rank_info_ptr p_role_a = arena_score_rank_mgr_t::find_user_arena_info(player_a_uid);
		if (NULL != p_role_a)
		{
			proto::server::re_world_cup_ready_notify ntf_a;
			ntf_a.set_server_id(p_role_a->get_server_id());
			p_battle->peek_data(ntf_a.mutable_battle_info());
			ntf_a.set_time_remain(get_ready_time());
			env::server->send_msg_to_transfer(op_cmd::re_world_cup_ready_notify, player_a_uid, ntf_a);
		}

		uint64_t player_b_uid = p_battle->get_player_b_uid();
		arena_rank_info_ptr p_role_b = arena_score_rank_mgr_t::find_user_arena_info(player_b_uid);
		if (NULL != p_role_b)
		{
			proto::server::re_world_cup_ready_notify ntf_b;
			ntf_b.set_server_id(p_role_b->get_server_id());
			p_battle->peek_data(ntf_b.mutable_battle_info());
			ntf_b.set_time_remain(get_ready_time());
			env::server->send_msg_to_transfer(op_cmd::re_world_cup_ready_notify, player_b_uid, ntf_b);
		}
	}
	if (NULL == m_timer)
	{
		m_timer.reset(new boost::asio::deadline_timer(env::server->get_ios()));
	}
	m_timer->expires_from_now(boost::posix_time::seconds(get_ready_time()));
	m_timer->async_wait(boost::bind(&world_cup_manager_t::fight, boost::asio::placeholders::error));
}

void world_cup_manager_t::fight(const boost::system::error_code& ec)
{
	if (ec != 0)
	{
		return;
	}
	if (NULL != m_interval_timer)
	{
		m_interval_timer->cancel();
	}
	if (NULL != m_timer)
	{
		m_timer->cancel();
	}
	WORLDCUP_LOG("fight_battle round[%d]", current_round);
	for (auto it : battle_map)
	{
		world_cup_battle_ptr p_battle = it.second;
		if (NULL == p_battle)
		{
			continue;
		}
		if (p_battle->get_state() != proto::common::world_cup_battle_state_playing)
		{
			continue;
		}
		if (p_battle->get_round() != current_round)
		{
			continue;
		}
		uint64_t player_a_uid = p_battle->get_player_a_uid();
		bool is_a_exist = player_map.find(player_a_uid) != player_map.end();

		uint64_t player_b_uid = p_battle->get_player_b_uid();
		bool is_b_exist = player_map.find(player_b_uid) != player_map.end();
		if (is_a_exist)
		{
			if (is_b_exist)
			{
				//开打
				proto::server::rs_arena_battle_notify msg_cross;
				msg_cross.mutable_fight()->set_type(proto::common::fight_type_world_cup);
				msg_cross.mutable_user1()->CopyFrom(player_map[player_a_uid]);
				msg_cross.mutable_user2()->CopyFrom(player_map[player_b_uid]);
				env::server->send_msg_to_cross(op_cmd::rs_arena_battle_notify, 0, msg_cross);
			}
			else
			{
				//A不战而胜
				p_battle->set_winner_uid(player_a_uid);
				p_battle->set_state(proto::common::world_cup_battle_state_end);
				p_battle->bet_result();
				WORLDCUP_LOG("make battle[%llu] player[%llu] win", p_battle->get_uid(), player_a_uid);
			}
		}
		else
		{
			if (is_b_exist)
			{
				//B不战而胜
				p_battle->set_winner_uid(player_b_uid);
				p_battle->set_state(proto::common::world_cup_battle_state_end);
				p_battle->bet_result();
				WORLDCUP_LOG("make battle[%llu] player[%llu] win", p_battle->get_uid(), player_b_uid);
			}
			else
			{
				//没有胜利者
				p_battle->set_winner_uid(0);
				p_battle->set_state(proto::common::world_cup_battle_state_end);
				p_battle->bet_result();
				WORLDCUP_LOG("make battle[%llu] no winner", p_battle->get_uid());
			}
		}
		proto::server::re_world_cup_update_notify ntf;
		p_battle->peek_data(ntf.mutable_battle_info());
		env::server->send_msg_to_transfer(op_cmd::re_world_cup_update_notify, 0, ntf);
	}
	if (NULL == m_interval_timer)
	{
		m_interval_timer.reset(new boost::asio::deadline_timer(env::server->get_ios()));
	}
	m_interval_timer->expires_from_now(boost::posix_time::seconds(get_battle_interval()));
	m_interval_timer->async_wait(boost::bind(&world_cup_manager_t::next_match, boost::asio::placeholders::error));
	if (NULL == m_timer)
	{
		m_timer.reset(new boost::asio::deadline_timer(env::server->get_ios()));
	}
	m_timer->expires_from_now(boost::posix_time::seconds(get_battle_time()));
	m_timer->async_wait(boost::bind(&world_cup_manager_t::next_round, boost::asio::placeholders::error));
}

void world_cup_manager_t::next_match(const boost::system::error_code& ec)
{
	if (ec != 0)
	{
		return;
	}
	WORLDCUP_LOG("next_match round[%d]", current_round);
	if (NULL != m_interval_timer)
	{
		m_interval_timer->cancel();
	}

	for (auto it : battle_map)
	{
		world_cup_battle_ptr p_battle = it.second;
		if (NULL == p_battle)
		{
			continue;
		}
		if (p_battle->get_state() != proto::common::world_cup_battle_state_playing)
		{
			continue;
		}
		if (p_battle->get_round() != current_round)
		{
			continue;
		}
		uint64_t player_a_uid = p_battle->get_player_a_uid();
		bool is_a_exist = player_map.find(player_a_uid) != player_map.end();

		uint64_t player_b_uid = p_battle->get_player_b_uid();
		bool is_b_exist = player_map.find(player_b_uid) != player_map.end();
		if (is_a_exist)
		{
			if (is_b_exist)
			{
				//开打
				proto::server::rs_arena_battle_notify msg_cross;
				msg_cross.mutable_fight()->set_type(proto::common::fight_type_world_cup);
				msg_cross.mutable_user1()->CopyFrom(player_map[player_a_uid]);
				msg_cross.mutable_user2()->CopyFrom(player_map[player_b_uid]);
				env::server->send_msg_to_cross(op_cmd::rs_arena_battle_notify, 0, msg_cross);
			}
			else
			{
				//A不战而胜
				p_battle->set_winner_uid(player_a_uid);
				p_battle->set_state(proto::common::world_cup_battle_state_end);
				p_battle->bet_result();
				WORLDCUP_LOG("make battle[%llu] player[%llu] win", p_battle->get_uid(), player_a_uid);
			}
		}
		else
		{
			if (is_b_exist)
			{
				//B不战而胜
				p_battle->set_winner_uid(player_b_uid);
				p_battle->set_state(proto::common::world_cup_battle_state_end);
				p_battle->bet_result();
				WORLDCUP_LOG("make battle[%llu] player[%llu] win", p_battle->get_uid(), player_b_uid);
			}
			else
			{
				//没有胜利者
				p_battle->set_winner_uid(0);
				p_battle->set_state(proto::common::world_cup_battle_state_end);
				p_battle->bet_result();
				WORLDCUP_LOG("make battle[%llu] no winner", p_battle->get_uid());
			}
		}
		proto::server::re_world_cup_update_notify ntf;
		p_battle->peek_data(ntf.mutable_battle_info());
		env::server->send_msg_to_transfer(op_cmd::re_world_cup_update_notify, 0, ntf);
	}
	if (NULL == m_interval_timer)
	{
		m_interval_timer.reset(new boost::asio::deadline_timer(env::server->get_ios()));
	}
	m_interval_timer->expires_from_now(boost::posix_time::seconds(get_battle_interval()));
	m_interval_timer->async_wait(boost::bind(&world_cup_manager_t::next_match, boost::asio::placeholders::error));
}

void world_cup_manager_t::battle_result(uint64_t winner_id, uint64_t loser_id)
{
	for (auto it : battle_map)
	{
		world_cup_battle_ptr p_battle = it.second;
		if (NULL == p_battle)
		{
			continue;
		}
		if (p_battle->get_state() != proto::common::world_cup_battle_state_playing)
		{
			continue;
		}
		if (p_battle->get_round() != current_round)
		{
			continue;
		}
		uint64_t player_a_uid = p_battle->get_player_a_uid();
		uint64_t player_b_uid = p_battle->get_player_b_uid();
		if (player_a_uid == winner_id && player_b_uid == loser_id)
		{
			p_battle->set_player_a_score(p_battle->get_player_a_score()+1);
			WORLDCUP_LOG("battle[%llu] player[%llu] score[%d]", p_battle->get_uid(), player_a_uid, p_battle->get_player_a_score());
			uint32_t winner_score = get_winner_score();
			uint32_t time_remain = time_util_t::get_remain_time(m_interval_timer);
			if (p_battle->get_player_a_score() >= winner_score)
			{
				p_battle->set_winner_uid(player_a_uid);
				p_battle->set_state(proto::common::world_cup_battle_state_end);
				p_battle->bet_result();
				time_remain = time_util_t::get_remain_time(m_timer);
				WORLDCUP_LOG("battle[%llu] player[%llu] win", p_battle->get_uid(), player_a_uid);
			}
			proto::server::re_world_cup_update_notify ntf;
			p_battle->peek_data(ntf.mutable_battle_info());
			env::server->send_msg_to_transfer(op_cmd::re_world_cup_update_notify, 0, ntf);

			arena_rank_info_ptr p_role_a = arena_score_rank_mgr_t::find_user_arena_info(player_a_uid);
			if (NULL != p_role_a)
			{
				proto::server::re_world_cup_ready_notify ntf_a;
				ntf_a.set_server_id(p_role_a->get_server_id());
				p_battle->peek_data(ntf_a.mutable_battle_info());
				ntf_a.set_time_remain(time_remain);
				env::server->send_msg_to_transfer(op_cmd::re_world_cup_ready_notify, player_a_uid, ntf_a);
			}

			arena_rank_info_ptr p_role_b = arena_score_rank_mgr_t::find_user_arena_info(player_b_uid);
			if (NULL != p_role_b)
			{
				proto::server::re_world_cup_ready_notify ntf_b;
				ntf_b.set_server_id(p_role_b->get_server_id());
				p_battle->peek_data(ntf_b.mutable_battle_info());
				ntf_b.set_time_remain(time_remain);
				env::server->send_msg_to_transfer(op_cmd::re_world_cup_ready_notify, player_b_uid, ntf_b);
			}
			break;
		}
		else if (player_a_uid == loser_id && player_b_uid == winner_id)
		{
			p_battle->set_player_b_score(p_battle->get_player_b_score() + 1);
			WORLDCUP_LOG("battle[%llu] player[%llu] score[%d]", p_battle->get_uid(), player_b_uid, p_battle->get_player_b_score());
			uint32_t winner_score = get_winner_score();
			uint32_t time_remain = time_util_t::get_remain_time(m_interval_timer);
			if (p_battle->get_player_b_score() >= winner_score)
			{
				p_battle->set_winner_uid(player_b_uid);
				p_battle->set_state(proto::common::world_cup_battle_state_end);
				p_battle->bet_result();
				time_remain = time_util_t::get_remain_time(m_timer);
				WORLDCUP_LOG("battle[%llu] player[%llu] win", p_battle->get_uid(), player_b_uid);
			}
			proto::server::re_world_cup_update_notify ntf;
			p_battle->peek_data(ntf.mutable_battle_info());
			env::server->send_msg_to_transfer(op_cmd::re_world_cup_update_notify, 0, ntf);

			arena_rank_info_ptr p_role_a = arena_score_rank_mgr_t::find_user_arena_info(player_a_uid);
			if (NULL != p_role_a)
			{
				proto::server::re_world_cup_ready_notify ntf_a;
				ntf_a.set_server_id(p_role_a->get_server_id());
				p_battle->peek_data(ntf_a.mutable_battle_info());
				ntf_a.set_time_remain(time_remain);
				env::server->send_msg_to_transfer(op_cmd::re_world_cup_ready_notify, player_a_uid, ntf_a);
			}

			arena_rank_info_ptr p_role_b = arena_score_rank_mgr_t::find_user_arena_info(player_b_uid);
			if (NULL != p_role_b)
			{
				proto::server::re_world_cup_ready_notify ntf_b;
				ntf_b.set_server_id(p_role_b->get_server_id());
				p_battle->peek_data(ntf_b.mutable_battle_info());
				ntf_b.set_time_remain(time_remain);
				env::server->send_msg_to_transfer(op_cmd::re_world_cup_ready_notify, player_b_uid, ntf_b);
			}
			break;
		}
	}
}

uint32_t world_cup_manager_t::get_before_play_time()
{
	return 60;
}

uint32_t world_cup_manager_t::get_comming_quarter_time()
{
	return 30;
}

uint32_t world_cup_manager_t::get_comming_soon_time()
{
	return 15;
}

uint32_t world_cup_manager_t::get_ready_time()
{
	return 15;
}

uint32_t world_cup_manager_t::get_battle_time()
{
	if (current_round < proto::common::world_cup_round_quarter_final)
	{
		return 30;
	}
	else if (current_round < proto::common::world_cup_round_final)
	{
		return 90;
	}
	else
	{
		return 150;
	}
}

uint32_t world_cup_manager_t::get_battle_interval()
{
	return 60;
}

uint32_t world_cup_manager_t::get_wait_time()
{
	return 10;
}

uint32_t world_cup_manager_t::get_interval_time()
{
	return 120;
}

uint32_t world_cup_manager_t::get_winner_score()
{
	switch (current_round)
	{
	case proto::common::world_cup_round_16th_final:
		return 1;
	case proto::common::world_cup_round_8th_final:
		return 1;
	case proto::common::world_cup_round_quarter_final:
		return 2;
	case proto::common::world_cup_round_semi_final:
		return 2;
	case proto::common::world_cup_round_final:
		return 3;
	default:
		return 3;
	}
}

bool world_cup_manager_t::check_over()
{
	for (auto it : battle_map)
	{
		world_cup_battle_ptr p_battle = it.second;
		if (NULL == p_battle)
		{
			continue;
		}
		if (p_battle->get_round() != current_round)
		{
			continue;
		}
		if (p_battle->get_state() != proto::common::world_cup_battle_state_end)
		{
			WORLDCUP_LOG("p_battle[%llu] is not end, round[%d]", p_battle->get_uid(), current_round);
			return false;
		}
	}
	return true;
}

void world_cup_manager_t::restart()
{
	battle_map.clear();
	player_map.clear();
	current_round = 0;
	make_battle();
}

