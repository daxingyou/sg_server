#include "trade_manager.hpp"
#include "main/game_server_fwd.hpp"
#include "main/game_server.hpp"
#include "config_mgr.h"
#include "tblh/Comprehensive.tbls.h"
#include "trade.hpp"
#include "tblh/Trade.tbls.h"
#include "user_troop/user_troop_mgr.hpp"
#include "tblh/errcode_enum.hpp"

trade_manager_t::trade_map trade_manager_t::m_trade_map;
trade_manager_t::sug_trade_info trade_manager_t::m_sug_trade_info;
trade_manager_t::trade_hot_map trade_manager_t::m_hot_map;
boost::shared_ptr<boost::asio::deadline_timer> trade_manager_t::m_market_timer;
boost::shared_ptr<boost::asio::deadline_timer> trade_manager_t::m_buy_num_timer;
boost::shared_ptr<boost::asio::deadline_timer> trade_manager_t::m_event_timer;
uint32_t trade_manager_t::m_market_reset_time = 0;
uint32_t trade_manager_t::m_buy_num_reset_time = 0;
uint32_t trade_manager_t::m_event_reset_time = 0;

trade_manager_t::trade_manager_t()
{

}

trade_manager_t::~trade_manager_t()
{
	m_trade_map.clear();
}

void trade_manager_t::close()
{
	for (auto& it : m_trade_map)
	{
		if (it.second)
			it.second->close();
	}
	cancel_buy_num_timer();
	cancel_market_timer();
	cancel_event_timer();
}

bool trade_manager_t::init()
{
	auto conf_m_1 = GET_CONF(Comprehensive, comprehensive_common::trade_market_float_time1);
	auto conf_m_2 = GET_CONF(Comprehensive, comprehensive_common::trade_market_float_time2);
	auto conf_g_1 = GET_CONF(Comprehensive, comprehensive_common::trade_goods_add_time1);
	auto conf_g_2 = GET_CONF(Comprehensive, comprehensive_common::trade_goods_add_time2);
	auto conf_e_1 = GET_CONF(Comprehensive, comprehensive_common::trade_event_time1);
	auto conf_e_2 = GET_CONF(Comprehensive, comprehensive_common::trade_event_time2);

	uint32_t market_start_time = GET_COMPREHENSIVE_VALUE_1(conf_m_1);
	uint32_t trade_market_refresh_time = GET_COMPREHENSIVE_VALUE_1(conf_m_2);

	uint32_t buy_num_start_time = GET_COMPREHENSIVE_VALUE_1(conf_g_1);
	uint32_t trade_buy_num_refresh_time = GET_COMPREHENSIVE_VALUE_1(conf_g_2);

	uint32_t event_start_time = GET_COMPREHENSIVE_VALUE_1(conf_e_1);
	uint32_t trade_event_refresh_time = GET_COMPREHENSIVE_VALUE_1(conf_e_2);

	if (trade_market_refresh_time == 0 || trade_buy_num_refresh_time == 0 || trade_event_refresh_time == 0)
	{
		log_error("[wys] init city_manager failed! market_refresh_time:%d buy_num_refresh_time:%d event_refresh_time:%d",
			trade_market_refresh_time, trade_buy_num_refresh_time, trade_event_refresh_time);
		return false;
	}

	///计算正式开始时间(开服)
	uint32_t now_time = time_util_t::get_minutes() + 1;
	uint32_t market_next_time = 0;
	uint32_t buy_num_next_time = 0;
	uint32_t event_next_time = 0;

	if (now_time < market_start_time)
		market_next_time = (market_start_time - now_time) * 60;
	else if (now_time > market_start_time)
		market_next_time = (trade_market_refresh_time - (now_time - market_start_time)) * 60;

	if (now_time < buy_num_start_time)
		buy_num_next_time = (buy_num_start_time - now_time) * 60;
	else if (now_time > buy_num_start_time)
		buy_num_next_time = (trade_buy_num_refresh_time - (now_time - buy_num_start_time)) * 60;

	if (now_time < event_start_time)
		event_next_time = (event_start_time - now_time) * 60;
	else if (now_time > event_start_time)
		event_next_time = (trade_event_refresh_time - (now_time - event_start_time)) * 60;

	m_market_timer.reset(new boost::asio::deadline_timer(env::server->get_ios()));
	m_buy_num_timer.reset(new boost::asio::deadline_timer(env::server->get_ios()));
	m_event_timer.reset(new boost::asio::deadline_timer(env::server->get_ios()));

	if (NULL == m_market_timer ||
		NULL == m_buy_num_timer ||
		NULL == m_event_timer)
	{
		return false;
	}
		
	///注释 此处的next_time是算出来的 可能为0 故不做判断
	m_market_timer->expires_from_now(boost::posix_time::seconds(market_next_time));
	m_market_timer->async_wait(boost::bind(&trade_manager_t::market_on_time, boost::asio::placeholders::error));

	m_buy_num_timer->expires_from_now(boost::posix_time::seconds(buy_num_next_time));
	m_buy_num_timer->async_wait(boost::bind(&trade_manager_t::buy_num_on_time, boost::asio::placeholders::error));

	m_event_timer->expires_from_now(boost::posix_time::seconds(event_next_time));
	m_event_timer->async_wait(boost::bind(&trade_manager_t::event_reset_on_time, boost::asio::placeholders::error));

	///开服会重新计算贸易行情 要记时间
	m_market_reset_time = common::time_util_t::now_time();
	m_buy_num_reset_time = common::time_util_t::now_time();
	m_event_reset_time = common::time_util_t::now_time();

	return true;
}

bool trade_manager_t::init_all_trade()
{
	trade_ptr p_trade = trade_ptr();
	Trade* p_config = NULL;
	std::map<uint32_t,Trade*> confs;
	GET_ALL_CONF(Trade,confs);
	for (auto& it : confs)
	{
		p_config = it.second;
		if (NULL == p_config)
		{
			log_error("NULL == p_config");
			return false;
		}
		p_trade.reset(new trade_t(it.first,p_config->scene()));
		if (NULL == p_trade)
		{
			log_error("new fail NULL == p_trade,id[%d]",it.first);
			return false;
		}
		m_trade_map.insert(std::make_pair(it.first,p_trade));
	}

	for (auto& it : m_trade_map)
	{
		p_trade = it.second;
		if (NULL == p_trade)
		{
			log_error("NULL == p_trade id[%d]",it.first);
			continue;
		}
		p_trade->init();
	}
	
	push_trade();
	return true;
}

trade_ptr trade_manager_t::get_trade(uint32_t trade_id)
{
	auto iter = m_trade_map.find(trade_id);
	if (iter != m_trade_map.end())
	{
		return iter->second;
	}
	return trade_ptr();
}

void trade_manager_t::market_on_time(const boost::system::error_code& ec)
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
	trade_ptr p_trade = trade_ptr();
	for (auto& it : m_trade_map)
	{
		p_trade = it.second;
		if (NULL == p_trade)
		{
			log_error("NULL == p_trade");
			continue;
		}
		p_trade->market_on_time();
	}
	start_market_timer();
	m_market_reset_time = common::time_util_t::now_time();
	push_trade();
}

void trade_manager_t::buy_num_on_time(const boost::system::error_code& ec)
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

	trade_ptr p_trade = trade_ptr();
	for (auto& it : m_trade_map)
	{
		p_trade = it.second;
		if (NULL == p_trade)
		{
			log_error("NULL == p_trade");
			continue;
		}
		p_trade->reset_buy_num_on_time();
	}
	start_buy_num_timer();
	m_buy_num_reset_time = common::time_util_t::now_time();
}

void trade_manager_t::event_reset_on_time(const boost::system::error_code& ec)
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
	auto conf_1 = GET_CONF(Comprehensive, comprehensive_common::trade_event1_city_cnt);
	auto conf_2 = GET_CONF(Comprehensive, comprehensive_common::trade_event2_city_cnt);

	uint32_t event_black_num = GET_COMPREHENSIVE_VALUE_1(conf_1);
	uint32_t event_popular_num = GET_COMPREHENSIVE_VALUE_1(conf_2);

	std::deque<uint32_t>	popular_list;
	std::deque<uint32_t>	black_list;
	std::vector<std::pair<uint32_t, uint32_t> >	trade_list;

	Trade* p_config_trade = NULL;
	trade_ptr	p_trade = trade_ptr();

	for (auto& it : m_trade_map)
	{
		p_trade = it.second;
		if (NULL == p_trade)
		{
			log_error("NULL == p_trade");
			continue;
		}
		p_config_trade = GET_CONF(Trade,it.first);
		if (NULL == p_config_trade)
		{
			log_error("NULL == p_config_trade");
			continue;
		}
		switch (p_config_trade->type())
		{
		case proto::common::trade_event_type_popular:
		{
			popular_list.push_back(it.first);
		}break;
		case proto::common::trade_event_type_black:
		{
			black_list.push_back(it.first);
		}break;
		default:
			break;
		}	
	}

	if (event_popular_num >= popular_list.size())
	{
		for (auto& it : popular_list)
		{
			trade_list.push_back(std::make_pair((uint32_t)proto::common::trade_event_type_popular, it));
		}
	}
	else
	{
		std::set<uint32_t> trade_id_set;
		do 
		{
			if (popular_list.empty())
				break;

			///uint32_t rand = rate_util_t::ranged_random(0, popular_list.size() - 1);
			int32_t rand = random_util_t::randBetween(0, popular_list.size() - 1);
			if (-1 == rand)
			{
				log_error("trade_event_type_popular min[%d]  max[%d]",0, (int32_t)popular_list.size() - 1);
				break;
			}
			if (rand >= (int32_t)popular_list.size())
			{
				log_error("rand[%d] popular_list.size()[%d]",rand, (int32_t)popular_list.size());
				break;
			}
			if (trade_id_set.count(popular_list.at(rand)))
				continue;
			trade_id_set.insert(popular_list.at(rand));
			if (rand < (int32_t)popular_list.size())
			{
				popular_list.erase(popular_list.begin() + rand);
			}
			
		} while (trade_id_set.size() < event_popular_num);


		for (auto& it : trade_id_set)
		{
			TRADE_LOG("popular trade_id_set trade_id[%d]", it);
			trade_list.push_back(std::make_pair((uint32_t)proto::common::trade_event_type_popular, it));
		}
		
	}
	
	if (event_black_num >= black_list.size())
	{
		for (auto& it : black_list)
		{
			trade_list.push_back(std::make_pair((uint32_t)proto::common::trade_event_type_black, it));
		}
	}
	else
	{
		std::set<uint32_t> trade_id_set;
		do
		{
			if (black_list.empty())
				break;
			///uint32_t rand = rate_util_t::ranged_random(0, black_list.size() - 1);
			int32_t rand = random_util_t::randBetween(0, black_list.size() - 1);
			if (-1 == rand)
			{
				log_error("trade_event_type_black min[%d]  max[%d]", 0, (int32_t)black_list.size() - 1);
				break;
			}
			if (rand >= (int32_t)black_list.size())
			{
				log_error("rand[%d] black_list.size()[%d]", rand, (int32_t)black_list.size());
				break;
			}
			
			if (trade_id_set.count(black_list.at(rand)))
				continue;
			trade_id_set.insert(black_list.at(rand));
			if (rand < (int32_t)black_list.size())
			{
				black_list.erase(black_list.begin() + rand);
			}
		} while (trade_id_set.size() < event_black_num);

		for (auto& it : trade_id_set)
		{
			TRADE_LOG("black trade_id_set trade_id[%d]",it);
			trade_list.push_back(std::make_pair((uint32_t)proto::common::trade_event_type_black, it));
		}
	}
	
	for (auto& it : trade_list)
	{
		trade_ptr p_trade  = get_trade(it.second);
		if (NULL == p_trade)
		{
			log_error("NULL == p_trade id[%d]", it.second);
			continue;
		}
		//TRADE_LOG("trade_id[%d] event_type[%d]",it.second,it.first);
		p_trade->event_reset_on_time((proto::common::trade_event_type)(it.first));
	}
	start_event_timer();
	m_event_reset_time = common::time_util_t::now_time();
	push_trade();
}

void trade_manager_t::push_trade()
{
	uint32_t sug_trade_id = 0, sug_item_tid = 0, sug_sell_trade_id, max_gain = 0; ///推荐跑商ID、货物ID 货物出售跑商ID 最大的利润
	std::vector<trade_item_ptr> item_vec;
	///找出所有的跑商的可出售物品
	trade_ptr p_trade = trade_ptr();
	for (auto& it : m_trade_map)
	{
		p_trade = it.second;
		if (NULL == p_trade)
		{
			log_error("NULL == p_trade");
			continue;
		}
		const trade_item_map& item_list = p_trade->get_trade_item_list();
		for (auto& item : item_list)
		{
			if (!item.second)
				continue;
			if (!item.second->is_can_buy())
				continue;
			item_vec.push_back(item.second);
		}
	}
	if (item_vec.empty())
	{
		///初始化没有物品列表
		//TRADE_LOG("item_vec.empty()");
		return;
	}

	std::map<uint32_t, std::tuple<uint32_t, uint32_t,uint32_t>> gain_list;		///item_tid , trade_id , gain sell_trade
	///循环运算
	for (auto& item : item_vec)
	{
		if (NULL == item)
			continue;

		uint32_t trade_id = 0, gain = 0, scene_id = 0 ,sour_trade_id = 0;
		for (auto& trade : m_trade_map)
		{
			p_trade = trade.second;
			if (NULL == p_trade)
			{
				log_error("NULL == p_trade");
				continue;
			}
			const trade_item_map& item_list = p_trade->get_trade_item_list();
			auto iter = item_list.find(item->get_id());
			if (iter == item_list.end())
				continue;
			trade_item_ptr p_item = iter->second;
			if (NULL == p_item)
			{
				continue;
			}
			if (p_item->is_can_buy())
			{
				continue;
			}
			if (p_item->get_sell_price() <= item->get_buy_price())
			{
				/*		gain = 0;
						trade_id = trade.first;
						scene_id = p_trade->get_scene_id();
						sour_trade_id = item->get_trade_id();*/
				continue;
			}
			uint32_t temp_gain = p_item->get_sell_price() - item->get_buy_price();
			if (gain >= temp_gain)
				continue;
			///TRADE_LOG("item_id[%d] market[%d]",p_item->get_id(), p_item->get_market());
			gain = temp_gain;
			trade_id = trade.first;
			scene_id = p_trade->get_scene_id();
			sour_trade_id = item->get_trade_id();
		}
		if (0 == gain && 0 == trade_id && 0 == scene_id && 0 == sour_trade_id)
		{
			TRADE_LOG("item[%d] 0 == gain[%d] && 0 == trade_id[%d] && 0 == sour_trade_id[%d]", item->get_id(),gain,trade_id,sour_trade_id);
			continue;
		}
		//TRADE_LOG("item[%d] gain[%d]  trade_id[%d] sour_trade_id[%d]", item->get_id(), gain, trade_id, sour_trade_id);
		m_hot_map.insert(std::make_pair(item->get_id(), std::make_tuple(trade_id, scene_id,sour_trade_id)));
		gain_list.insert(std::make_pair(item->get_id(), std::make_tuple(trade_id, gain,item->get_trade_id())));
		trade_id = 0, gain = 0;
	}
	if (gain_list.empty())
	{
		log_error("gain_list.empty()");
		return;
	}
	if (m_hot_map.empty() )
	{
		log_error("m_hot_map.empty()");
	}
	for (auto& gain_iter : gain_list)
	{
		uint32_t temp_gain = std::get<1>(gain_iter.second);
		///TRADE_LOG("item_id[%d] trade_id[%d] gain[%d]", gain_iter.first, std::get<0>(gain_iter.second), std::get<1>(gain_iter.second));
		if (max_gain < temp_gain)
		{
			max_gain = temp_gain;
			sug_item_tid = gain_iter.first;
			sug_trade_id = std::get<0>(gain_iter.second);
			sug_sell_trade_id = std::get<2>(gain_iter.second);
		}
	}
	///TRADE_LOG("sug_trade_id[%d] sug_item_tid[%d]", sug_trade_id, sug_item_tid);
	if (0 != sug_trade_id && 0 != sug_item_tid && 0 != sug_sell_trade_id)
	{
		m_sug_trade_info = std::make_tuple(0, 0, 0);
		m_sug_trade_info = std::make_tuple(sug_trade_id, sug_item_tid, sug_sell_trade_id);
	}
	else
	{
		log_error("sug_trade_id[%d] sug_item_tid[%d] sug_sell_trade_id[%d]", sug_trade_id, sug_item_tid, sug_sell_trade_id);
	}
}

void trade_manager_t::start_market_timer()
{
	if (NULL == m_market_timer)
		return;

	auto conf = GET_CONF(Comprehensive, comprehensive_common::trade_market_float_time2);
	uint32_t market_refresh_time = GET_COMPREHENSIVE_VALUE_1(conf);

	if (0 == market_refresh_time)
	{
		log_error("market_refresh_time = 0");
		return;
	}

	m_market_timer->expires_from_now(boost::posix_time::seconds(market_refresh_time * 60));
	m_market_timer->async_wait(boost::bind(&trade_manager_t::market_on_time, boost::asio::placeholders::error));
}

void trade_manager_t::start_buy_num_timer()
{
	if (NULL == m_buy_num_timer)
		return;

	auto conf = GET_CONF(Comprehensive, comprehensive_common::trade_goods_add_time2);
	uint32_t buy_num_refresh_time = GET_COMPREHENSIVE_VALUE_1(conf);

	if (0 == buy_num_refresh_time)
	{
		log_error("buy_num_refresh_time = 0");
		return;
	}

	m_buy_num_timer->expires_from_now(boost::posix_time::seconds(buy_num_refresh_time * 60));
	m_buy_num_timer->async_wait(boost::bind(&trade_manager_t::buy_num_on_time, boost::asio::placeholders::error));
}

void trade_manager_t::start_event_timer()
{
	if (NULL == m_event_timer)
		return;

	auto conf = GET_CONF(Comprehensive, comprehensive_common::trade_event_time2);
	uint32_t event_refresh_time = GET_COMPREHENSIVE_VALUE_1(conf);
	if (0 == event_refresh_time)
	{
		log_error("event_refresh_time = 0");
		return;
	}
	m_event_timer->expires_from_now(boost::posix_time::seconds(event_refresh_time * 60));
	m_event_timer->async_wait(boost::bind(&trade_manager_t::event_reset_on_time, boost::asio::placeholders::error));
}

void trade_manager_t::cancel_market_timer()
{
	if (m_market_timer != NULL)
		m_market_timer->cancel();
}

void trade_manager_t::cancel_buy_num_timer()
{
	if (m_buy_num_timer != NULL)
		m_buy_num_timer->cancel();
}

void trade_manager_t::cancel_event_timer()
{
	if (m_event_timer != NULL)
		m_event_timer->cancel();
}

void trade_manager_t::event_on_time(const boost::system::error_code& ec, uint32_t trade_id)
{
	trade_ptr p_trade = get_trade(trade_id);
	if (NULL == p_trade)
	{
		log_error("NULL == p_trade trade id[%d]", trade_id);
		return;
	}
	p_trade->event_on_time();
}


void trade_manager_t::get_suggest_trade_info(uint32_t& trade_id,uint32_t& item_tid, uint32_t& sell_trade_id)
{
	if (0 != std::get<0>(m_sug_trade_info) && 
		0 != std::get<1>(m_sug_trade_info) &&
		0 != std::get<2>(m_sug_trade_info))
	{
		trade_id = std::get<0>(m_sug_trade_info);
		item_tid = std::get<1>(m_sug_trade_info);
		sell_trade_id = std::get<2>(m_sug_trade_info);
	}
	//TRADE_LOG("trade_id[%d] item_tid[%d]",trade_id,item_tid);
}

uint32_t trade_manager_t::troop_trade_view_notify(const uint64_t role_uid,const uint32_t trade_id)
{
	uint32_t reply_code = errcode_enum::error_ok;
	do 
	{
		role_ptr p_role = role_manager_t::find_role(role_uid);
		if (NULL == p_role)
		{
			log_error("NULL == p_role");
			reply_code = errcode_enum::notice_unknown;
			break;
		}

		const uint64_t troop_id = p_role->get_troop_id();
		if (0 == troop_id)
		{
			///没有队伍
			reply_code = errcode_enum::notice_trade_not_troop;
			break;
		}

		if (!p_role->is_leader())
		{
			///不是队长
			reply_code = errcode_enum::notice_trade_not_leader;
			break;
		}
		troop_ptr p_troop = game_troop_mgr_t::get_troop(troop_id);
		if (NULL == p_troop)
		{
			log_error("NULL == p_troop troop_id[%lu]",troop_id);
			break;
		}
		proto::client::gc_troop_trade_notify notify;
		notify.set_reply_code(reply_code);
		notify.set_trade_id(trade_id);
		const troop_t::troop_member_vec& troop_members = p_troop->get_members();
		for (auto& it : troop_members)
		{
			if (role_uid == it->get_uid())
			{
				continue;
			}

			if (it->get_object_type() != proto::common::SCENEOBJECT_USER) {
				continue;
			}

			if (it->get_state() != proto::common::troop_state_normal)
			{
				continue;
			}

			role_ptr role = role_manager_t::find_role(it->get_uid());
			if (NULL == role)
			{
				log_error("NULL == role [%lu]", it->get_uid());
				continue;
			} 
			role->send_msg_to_client(op_cmd::gc_troop_trade_notify, notify);
			///每个玩家发送数据一样 不需要notify.Clear();
		}
	} while (false);
	
	return reply_code;
}

