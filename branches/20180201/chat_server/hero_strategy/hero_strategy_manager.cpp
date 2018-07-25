#include "hero_strategy_manager.hpp"
#include "main/chat_server_fwd.hpp"
#include "main/chat_server.hpp"

all_hero_strategy_map hero_strategy_manager_t::m_all_hero_strategy_map;
boost::shared_ptr<boost::asio::deadline_timer> hero_strategy_manager_t::m_init_timer;
bool hero_strategy_manager_t::m_b_init = false;

bool hero_strategy_manager_t::init()
{
	m_init_timer.reset(new boost::asio::deadline_timer(env::server->get_ios()));
	if (NULL == m_init_timer)
	{
		log_error("hero_strategy_manager::init() m_init_timer NULL");
		return false;
	}
	m_init_timer->expires_from_now(boost::posix_time::seconds(10));
	m_init_timer->async_wait(boost::bind(&hero_strategy_manager_t::init_on_time, boost::asio::placeholders::error));
	return true;
}

void hero_strategy_manager_t::close()
{
	cancel_init_timer();
}

void hero_strategy_manager_t::get_hero_strategy_info_from_area()
{
	proto::server::hr_get_hero_strategy_info_request msg;
	msg.set_server_id(env::xml->srv_group.group_id);
	env::server->send_msg_to_transfer(op_cmd::hr_get_hero_strategy_info_request, 0, msg);
	m_b_init = false;
}

void hero_strategy_manager_t::load_hero_strategy_info(const proto::server::rh_get_hero_strategy_info_reply& reply)
{
	if (!m_b_init)
	{
		m_all_hero_strategy_map.clear();
	}
	m_b_init = true;
	cancel_init_timer();

	proto::common::hero_strategy_info hero_info;
	for (int32_t i = 0; i < reply.hero_info_size(); ++i)
	{
		hero_info.CopyFrom(reply.hero_info(i));
		m_all_hero_strategy_map.insert(std::make_pair(hero_info.hero_tid(), hero_info));
	}
}

void hero_strategy_manager_t::get_hero_strategy_info(user_ptr p_user, uint32_t hero_tid)
{
	if (NULL == p_user)
	{
		log_error("p_user NULL");
		return;
	}
	proto::client::hc_hero_strategy_hero_info_reply reply;
	reply.set_error_code(0);
	all_hero_strategy_map::iterator iter = m_all_hero_strategy_map.find(hero_tid);
	if (iter != m_all_hero_strategy_map.end())
	{
		reply.set_error_code(1);
		reply.mutable_hero_info()->CopyFrom(iter->second);
	}
	p_user->send_msg_to_client(op_cmd::hc_hero_strategy_hero_info_reply, reply);
}

void hero_strategy_manager_t::init_on_time(const boost::system::error_code& ec)
{
	if (ec == boost::asio::error::operation_aborted)
	{

		return;
	}
	else if (ec)
	{
		log_error("Timer error: %s", ec.message().c_str());
		return;
	}
	hero_strategy_manager_t::get_hero_strategy_info_from_area();

	m_init_timer->expires_from_now(boost::posix_time::seconds(10));
	m_init_timer->async_wait(boost::bind(&hero_strategy_manager_t::init_on_time, boost::asio::placeholders::error));
}

void hero_strategy_manager_t::cancel_init_timer()
{
	if (m_init_timer != NULL)
		m_init_timer->cancel();
}

void hero_strategy_manager_t::chat_get_arena_data_one_day(uint32_t id, bool is_in_time)
{
	if (!is_in_time)
		return;
	hero_strategy_manager_t::get_hero_strategy_info_from_area();
}
