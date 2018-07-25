#include "trade_item.hpp"
#include "tblh/TradeGoodsType.tbls.h"
#include "tblh/TradeFloat.tbls.h"
#include "config_mgr.h"
#include "tblh/Comprehensive.tbls.h"
#include "main/game_server_fwd.hpp"

trade_item_t::trade_item_t(uint32_t trade_id,uint32_t item_tid,uint32_t item_trade_type,uint32_t item_base_sell_price,uint32_t item_base_buy_price)
	:m_trade_id(trade_id)
	,m_id(item_tid)
	,m_type(item_trade_type)
	,m_base_sell_price(item_base_sell_price)
	,m_base_buy_price(item_base_buy_price)
{
	//TODO
}

trade_item_t::~trade_item_t()
{

}

void trade_item_t::set_market(uint32_t market)
{
	m_market = market;
}

void trade_item_t::calc_market()
{
	TradeGoodsType* p_config_trade_type = GET_CONF(TradeGoodsType,m_type);
	if (NULL == p_config_trade_type)
	{
		log_error("not find trade_type[%d]", m_type);
		return;
	}
	const uint32_t market_max = p_config_trade_type->market_upper();
	const uint32_t market_min = p_config_trade_type->market_lower();
	if (m_market >= market_max)
	{
		m_market_type = trade_market_type_down_reset;
	}
	else if(m_market <= market_min)
	{
		m_market_type = trade_market_type_up_reset;
	}
	/*TRADE_LOG("trade_id[%d] item_id[%d] market_min/market_max [%d/%d] m_market[%d] m_market_type[%d]", 
	get_trade_id(), get_id(), market_min, market_max, m_market, m_market_type);*/

	///非向上/下重置行情
	if (trade_market_type_none == m_market_type)
	{
		std::map<uint32_t, TradeFloat*> confs;
		GET_ALL_CONF(TradeFloat, confs);
		TradeFloat* p_config_trade_float = NULL;
		for (auto& it : confs)
		{
			TradeFloat* p_config = it.second;
			if (NULL == p_config)
			{
				log_error("not trade float config");
				break;
			}
			if (m_item_sell_total >= p_config->sell_cnt())
			{
				p_config_trade_float = p_config;
				break;
			}
		}

		if (NULL == p_config_trade_float)
		{
			log_error("NULL == p_config_trade_float");
			return;
		}
		m_market_stay_prob = p_config_trade_float->stay_prob();
		///const uint32_t random_value = rate_util_t::ranged_random(1, 10000);
		const uint32_t random_value = random_util_t::randBetween(1, 10000);
		/*TRADE_LOG("trade_id[%d] item_id[%d] sell_toal[%d] random_value[%d] down_prob[%d]",
			get_trade_id(), get_id(), m_item_sell_total, random_value, p_config_trade_float->down_prob());*/

		if (random_value <= p_config_trade_float->down_prob())
		{
			m_market_type = trade_market_type_change_down;
		}
		else
		{
			m_market_type = trade_market_type_change_up;
		}
	}
	m_last_market = m_market;
	uint32_t market_base_value  = p_config_trade_type->market_base_value();
	uint32_t market_float = p_config_trade_type->market_float();
	switch (m_market_type)
	{
	case trade_market_type_down_reset:
	{
		m_market -= market_float;
		if (market_base_value >= m_market)
		{
			m_market_type = trade_market_type_none;
		}
	}break;
	case trade_market_type_up_reset:
	{
		m_market += market_float;
		if (market_base_value <= m_market)
		{
			m_market_type = trade_market_type_none;
		}
	}break;
	case trade_market_type_change_up:
	{
		///uint32_t random_stay = rate_util_t::ranged_random(1, 10000);
		uint32_t random_stay = random_util_t::randBetween(1, 10000);
		if (random_stay > m_market_stay_prob)
		{
			m_market += market_float;
		
		}
		/*TRADE_LOG("trade_id[%d] item_id[%d] trade_market_type_change_up random_stay[%d] m_market_stay_prob[%d]",
			get_trade_id(), get_id(), random_stay, m_market_stay_prob);*/

		m_market_type = trade_market_type_none;
		
	}break;
	case trade_market_type_change_down:
	{
		///uint32_t random_stay = rate_util_t::ranged_random(1, 10000);
		uint32_t random_stay = random_util_t::randBetween(1, 10000);
		if (random_stay > m_market_stay_prob)
		{
			m_market -= market_float;
		}
		/*TRADE_LOG("trade_id[%d] item_id[%d] trade_market_type_change_down random_stay[%d] m_market_stay_prob[%d]",
			get_trade_id(), get_id(), random_stay, m_market_stay_prob);*/

		m_market_type = trade_market_type_none;
	}break;
	default:
		break;
	}
	m_item_sell_total = 0;
	m_market = m_market > market_max ? market_max : m_market;
	m_market = m_market < market_min ? market_min : m_market;
	if (0 == m_market)
	{
		log_error("trade_id[%d] item[%d] market[%d] max_market[%d] min_market[%d]",m_trade_id,m_id,m_market, market_max, market_min);
	}
	/*TRADE_LOG("trade_id[%d] item_id[%d] new_market_type[%d]  m_market_stay_prob[%d] new_market[%d]",
		get_trade_id(), get_id(), m_market_type, m_market_stay_prob, m_market);*/

}

void trade_item_t::add_item_buy_total(uint32_t buy_num)
{
	m_item_buy_total += buy_num;
}

void trade_item_t::add_item_sell_total(uint32_t sell_num)
{
	m_item_sell_total += sell_num;
}


void trade_item_t::reset_base_value(uint32_t type,uint32_t base_num,uint32_t cond_param,bool is_need_prosperity)
{
	m_type = type;
	m_base_num = base_num;
	m_cond_param = cond_param;
	m_is_need_prosperity = is_need_prosperity;
}

bool trade_item_t::check_can_buy(uint32_t prosperity)
{
	if (m_is_need_prosperity && prosperity < m_cond_param)
	{
		return false;
	}
	return true;
}

void trade_item_t::recalc_item_price(uint32_t prosperity_level, uint32_t event_market)
{
	///获取繁荣度加成系数
	///auto conf_p = GET_CONF(Comprehensive,comprehensive_common::trade_prosperity_add_rate);
	///uint32_t prosperity_rate = GET_COMPREHENSIVE_VALUE_1(conf_p);
	if (0 == event_market)
	{
		event_market = 10000;
	}

	const uint32_t market = (uint32_t)((float)m_market * ((float)event_market / 10000));
	if (0 == market)
	{
		log_error("market == 0 m_market[%d] event_market[%d] prosperity_level[%d]", m_market, event_market, prosperity_level);
	}
	///购买单价调整为 货物购买单价 = 货物基础价格
	///m_buy_price = (uint32_t)((float)m_base_buy_price * (10000 - prosperity_level * prosperity_rate) / 10000);
	///m_sell_price = (uint32_t)(float(m_base_sell_price * market) / 10000 * (10000 + prosperity_level * prosperity_rate) / 10000 );
	m_buy_price = m_base_buy_price ;
	m_sell_price = uint32_t(float(m_base_sell_price * market) / 10000);
	
	if (0 == m_buy_price ||
		0 == m_sell_price)
	{
		log_error("trade_id[%d] item[%d] base_price[%d] sell_price[%d]  buy_price[%d] market[%d]",
			m_trade_id, m_id, m_base_sell_price, m_sell_price, m_buy_price, market);
	}

}

void trade_item_t::recalc_buy_num(uint32_t hold_value)
{
	auto conf_h = GET_CONF(Comprehensive, comprehensive_common::trade_sell_cnt_occupancy);
	uint32_t hold_rate = GET_COMPREHENSIVE_VALUE_1(conf_h);

	m_can_buy_num = (uint32_t)(m_base_num  * (1 + (float)(hold_value * hold_rate) / 10000));
	if (0 == m_can_buy_num)
	{
		log_error("trade_id[%d] item[%d] base_num[%d] hold_value[%d] hold_rate[%d]",
			m_trade_id,m_id, m_base_num, hold_value, hold_rate);
	}
}

trade_item_type_t::trade_item_type_t(uint32_t trade_id,uint32_t type)
	:m_trade_id(trade_id)
	,m_type(type)
{
	//TODO
}
