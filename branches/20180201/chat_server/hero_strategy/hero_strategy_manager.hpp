#pragma once

#include "protos_fwd.hpp"
#include "user/user.hpp"
#include <boost/system/error_code.hpp>
#include <boost/asio/deadline_timer.hpp>

typedef std::map<uint32_t, proto::common::hero_strategy_info> all_hero_strategy_map;

class hero_strategy_manager_t
{
public:
	static bool init();
	static void close();
	static void get_hero_strategy_info_from_area();

	static void load_hero_strategy_info(const proto::server::rh_get_hero_strategy_info_reply& reply);
public:
	static void get_hero_strategy_info(user_ptr p_user, uint32_t hero_tid);
public:
	static void init_on_time(const boost::system::error_code& ec);
	static void cancel_init_timer();
	static void chat_get_arena_data_one_day(uint32_t id, bool is_in_time);
private:
	static all_hero_strategy_map m_all_hero_strategy_map;
	static boost::shared_ptr<boost::asio::deadline_timer> m_init_timer;		///初始化定时器
	static bool m_b_init;

};
