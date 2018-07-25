#include "trade.hpp"
#include "trade_item.hpp"
#include "trade_manager.hpp"
#include "config_mgr.h"
#include "tblh/Item.tbls.h"
#include "tblh/Trade.tbls.h"
#include "tblh/MapTable.tbls.h"
#include "tblh/TradeGoodsType.tbls.h"
#include "tblh/Comprehensive.tbls.h"
#include "city/city_manager.hpp"
#include "main/game_server_fwd.hpp"
#include "main/game_server.hpp"

trade_t::trade_t(uint32_t trade_id,uint32_t scene_id)
	:m_id(trade_id)
	,m_scene_id(scene_id)
{

}

void trade_t::init()
{
	Trade* p_config_trade = GET_CONF(Trade, m_id);
	if (NULL == p_config_trade)
	{
		log_error("init trade fail trade id[%d] config error!",m_id);
		return;
	}

	Item* p_config_item = NULL;
	uint32_t item_base_price = 0;
	TradeGoodsType* p_config_trade_goods = NULL;
	trade_item_ptr p_trade_item = trade_item_ptr();
	trade_item_type_ptr p_trade_item_type = trade_item_type_ptr();

	std::map<uint32_t, TradeGoodsType*> confs;
	GET_ALL_CONF(TradeGoodsType,confs);
	for (auto& it : confs)
	{
		p_config_trade_goods = it.second;
		if (NULL == p_config_trade_goods)
		{
			log_error("NULL == p_config_trade_goods goods type[%d]",it.first);
			return;
		}
		p_trade_item_type.reset(new trade_item_type_t(m_id,it.first));
		if (NULL == p_trade_item_type)
		{
			log_error("new trade_item_type NULL == p_trade_item_type");
			return;
		}

		///m_trade_item_type_list.insert(std::make_pair(it.first,p_trade_item_type));
		for (uint32_t i = 0; i < p_config_trade_goods->goods_id_size();++i)
		{
			uint32_t item_tid = p_config_trade_goods->goods_id(i);
			p_config_item = GET_CONF(Item, item_tid);
			if (NULL == p_config_item)
			{
				log_error("NULL == p_config_item item_tid[%d]",item_tid);
				return;
			}
			item_base_price = p_config_item->get_trade_price(m_id);
			//TRADE_LOG("base_price[%d]", item_base_price);
			p_trade_item.reset(new trade_item_t(m_id,item_tid,it.first,item_base_price,item_base_price));
			if (NULL == p_trade_item)
			{
				log_error("new p_trade_item NULL");
				return;
			}
			p_trade_item->set_market(p_config_trade_goods->market_base_value());
			m_trade_item_list.insert(std::make_pair(item_tid,p_trade_item));

			//TRADE_LOG("id[%d] item[%d] base_price[%d]", m_id, item_tid, item_base_price);
		}
	}
	//TRADE_LOG("m_trade_item_list.size[%d]", m_trade_item_list.size());
	///初始化跑商玩家可购买物品数据

	for (uint32_t i = 0; i < p_config_trade->sell_goods_size(); ++i)
	{
		auto iter = m_trade_item_list.find(p_config_trade->sell_goods(i));
		if (iter != m_trade_item_list.end())
		{
			p_trade_item = iter->second;
			if (NULL == p_trade_item)
			{
				log_error("NULL == p_trade_item item_tid[%d]", p_config_trade->sell_goods(i));
				return;
			}
			p_trade_item->set_can_buy();
			p_trade_item->reset_base_value(p_trade_item->get_type(), p_config_trade->sell_cnt(i), p_config_trade->unlock_para(i), p_config_trade->unlock_type(i));
			//TRADE_LOG("trade_id[%d] item can buy [%d]",p_trade_item->get_trade_id(), p_trade_item->get_id());
		}
	}

	market_on_time();
	reset_buy_num_on_time();

	m_timer_event.reset(new boost::asio::deadline_timer(env::server->get_ios()));
}

void trade_t::close()
{
	///TODO
	///清楚定时器
	cancel_event_timer();
}

trade_item_ptr trade_t::get_trade_item(uint32_t trade_item_tid)
{
	auto iter = m_trade_item_list.find(trade_item_tid);
	if (iter != m_trade_item_list.end())
	{
		return iter->second;
	}
	return trade_item_ptr();
}

trade_item_type_ptr trade_t::get_trade_item_type(uint32_t trade_item_type)
{
	return trade_item_type_ptr();
}

bool trade_t::trade_buy_item(trade_item_ptr p_trade_item, uint32_t item_num)
{
	return true;
}

bool trade_t::trade_sell_item(trade_item_ptr p_trade_item, uint32_t item_num)
{
	if (NULL == p_trade_item)
	{
		log_error("NULL == p_trade_item");
		return false;
	}
	p_trade_item->add_item_sell_total(item_num);

	uint32_t event_count_max = 0;
	switch (m_event_type)
	{
	case proto::common::trade_event_type_popular:
	{
		if (m_event_param == p_trade_item->get_id())
		{
			auto conf = GET_CONF(Comprehensive, comprehensive_common::trade_event2_cnt_limt);
			event_count_max = GET_COMPREHENSIVE_VALUE_1(conf);
			m_event_count += item_num;
		}
	}
	break;
	case proto::common::trade_event_type_black:
	{
		auto conf = GET_CONF(Comprehensive, comprehensive_common::trade_event1_cnt_limt);
		event_count_max = GET_COMPREHENSIVE_VALUE_1(conf);
		m_event_count += item_num;
	}
	break;
	default:
		break;
	}
	TRADE_LOG("event_count[%d] event_config_max[%d]", m_event_count, event_count_max);
	if (m_event_count >= event_count_max)
	{
		event_stop();
	}
		
	return true;
}

bool trade_t::check_trade_event_is_end()
{
	if (m_event_type == proto::common::trade_event_type_normal)
		return true;

	return false;
}

uint32_t trade_t::get_prosperity()
{
	Trade* p_config_trade = GET_CONF(Trade, m_id);
	if (NULL == p_config_trade)
	{
		log_error("NULL == p_config_trade trade id[%d]", m_id);
		return 0;
	}
	city_ptr p_city = city_manager_t::get_city(p_config_trade->scene());
	if (NULL == p_city)
	{
		log_error("NULL == p_city[%u]", p_config_trade->scene());
		return 0;
	}

	return p_city->get_prosperity_value();
}

uint32_t trade_t::get_hold()
{
	Trade* p_config_trade = GET_CONF(Trade, m_id);
	if (NULL == p_config_trade)
	{
		log_error("NULL == p_config_trade trade id[%d]", m_id);
		return 0;
	}
	city_ptr p_city = city_manager_t::get_city(p_config_trade->scene());
	if (NULL == p_city)
	{
		return 0;
	}

	return p_city->get_hold_value();
}

uint32_t trade_t::get_prosperity_level()
{
	Trade* p_config_trade = GET_CONF(Trade,m_id);
	if (NULL == p_config_trade)
	{
		log_error("NULL == p_config_trade trade id[%d]",m_id);
		return 1;
	}
	city_ptr p_city = city_manager_t::get_city(p_config_trade->scene());
	if (NULL == p_city)
	{
		return 1;
	}

	return p_city->get_prosperity_level();
}

bool trade_t::market_on_time()
{
	trade_item_ptr p_item = trade_item_ptr();
	for (auto& it : m_trade_item_list)
	{
		p_item = it.second;
		if (NULL == p_item)
		{
			log_error("NULL == p_item");
			continue;
		}
		p_item->calc_market();
		p_item->recalc_item_price(get_prosperity_level(),m_event_market);
	}
	return true;
}

bool trade_t::reset_buy_num_on_time()
{
	trade_item_ptr p_item = trade_item_ptr();
	for (auto& it : m_trade_item_list)
	{
		p_item = it.second;
		if (NULL == p_item)
		{
			log_error("NULL == p_item");
			continue;
		}
		if (!p_item->is_can_buy())
		{
			continue;
		}
		///log_warn("trade_id[%d] item_id[%d]",p_item->get_trade_id(),p_item->get_id());
		p_item->recalc_buy_num(get_hold());
		///log_warn("trade_id[%d] item_id[%d] can_buy_num[%d]", p_item->get_trade_id(), p_item->get_id(),p_item->get_can_buy_num());
	}
	return true;
}

bool trade_t::event_reset_on_time(proto::common::trade_event_type event_type)
{
	m_event_start_time = common::time_util_t::now_time();
	m_event_type = event_type;
	m_event_market = 0;
	int32_t random_value = 0;
	uint32_t event_market_add = 10000;
	switch (event_type)
	{
	case proto::common::trade_event_type_popular:
	{
		auto conf = GET_CONF(Comprehensive, comprehensive_common::trade_event2_market_add);
		event_market_add = GET_COMPREHENSIVE_VALUE_1(conf);
		std::vector<trade_item_ptr> item_list;
		trade_item_ptr p_item = trade_item_ptr();
		for (auto& it : m_trade_item_list)
		{
			p_item = it.second;
			if (NULL == p_item)
			{
				log_error("NULL == p_item");
				continue;
			}
			if (p_item->is_can_buy())
				continue;
			item_list.push_back(p_item);
		}
		if (item_list.size() == 0)
		{
			log_error("item_list.size() == 0");
			break;
		}
		///random_value = rate_util_t::ranged_random(0, item_list.size() - 1);
		random_value = random_util_t::randBetween(0, item_list.size() - 1);
		if (-1 == random_value)
		{
			log_error("min[%d]  max[%d]",0 , (int32_t)item_list.size() - 1);
			break;
		}

		if (random_value < (int32_t)item_list.size())
		{
			trade_item_ptr p_trade_item = item_list[random_value];
			if (p_trade_item)
			{
				m_event_param = p_trade_item->get_id();		///记录商品ID
				m_event_item_list.push_back(p_trade_item);
			}
		}	
	}break;
	case proto::common::trade_event_type_black:
	{

	}break;
	default:
		break;
	}
	trade_item_ptr p_item = trade_item_ptr();
	for(uint32_t i = 0; i < m_event_item_list.size(); ++i)
	{
		p_item = m_event_item_list[i];
		if (NULL == p_item)
		{
			log_error("NULL == p_item");
			continue;
		}
		p_item->recalc_item_price(get_prosperity_level(), event_market_add);
	}

	start_event_timer();
	return true;
}

void trade_t::event_on_time()
{
	event_stop();
}

void trade_t::event_stop()
{
	m_event_type = proto::common::trade_event_type_normal;
	m_event_start_time = 0;
	m_event_param = 0;
	m_event_count = 0;
	m_event_market = 10000;
	
	 city_ptr p_city = city_manager_t::get_city(m_scene_id);
	 if (NULL == p_city)
	 {
		 log_error("NULL == p_city,scene_id[%d]", m_scene_id);
		 return;
	 }
	// 重置回原来的价格
	trade_item_ptr p_trade_item = trade_item_ptr();
	for (uint32_t i = 0; i < m_event_item_list.size(); ++i)
	{
		p_trade_item = m_event_item_list[i];
		if (NULL == p_trade_item)
		{
			log_error("NULL == p_trade_item");
			continue;
		}
		p_trade_item->recalc_item_price(p_city->get_prosperity_level(), m_event_market);
	}

	m_event_item_list.clear();
}

void trade_t::start_event_timer()
{
	if (NULL == m_timer_event)
	{
		log_error("NULL == m_timer_event");
		return;
	}
	uint32_t event_time = 0;
	switch (m_event_type)
	{
	case proto::common::trade_event_type_popular:
	{
		auto conf = GET_CONF(Comprehensive, comprehensive_common::trade_event2_time);
		event_time = GET_COMPREHENSIVE_VALUE_1(conf);
	}
	break;
	case proto::common::trade_event_type_black:
	{
		auto conf = GET_CONF(Comprehensive, comprehensive_common::trade_event1_time);
		event_time = GET_COMPREHENSIVE_VALUE_1(conf);
	}
	break;
	default:
		break;
	}

	if (0 == event_time)
	{
		log_error("event_time = 0 event_type[%d]", m_event_type);
		return;
	}

	m_timer_event->expires_from_now(boost::posix_time::seconds(event_time * 60));
	m_timer_event->async_wait(boost::bind(&trade_manager_t::event_on_time, boost::asio::placeholders::error, m_id));
}

void trade_t::cancel_event_timer()
{
	if (m_timer_event != NULL)
		m_timer_event->cancel();
}

