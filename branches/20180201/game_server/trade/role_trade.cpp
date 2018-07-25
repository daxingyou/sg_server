#include "role_trade.hpp"
#include "cache_key.hpp"
#include "role/role_unify_save.hpp"
#include "config_mgr.h"
#include "tblh/Comprehensive.tbls.h"
#include "role/money_manager.hpp"
#include "trade/trade_manager.hpp"
#include "role/role_manager.hpp"
#include "city/city_manager.hpp"
#include <tuple>
#include "trade/trade.hpp"
#include "log/log_wrapper.hpp"

role_trade_t::role_trade_t(uint64_t role_uid)
	: m_role_uid(role_uid)
{
	m_key = cache_key_t::create(m_role_uid, cache_name::role_trade_item);
}

role_trade_t::~role_trade_t()
{

}

void role_trade_t::init_trade()
{
	auto conf = GET_CONF(Comprehensive, comprehensive_common::trade_point_daily);
	uint32_t init_trade_point = GET_COMPREHENSIVE_VALUE_1(conf);
	set_trade_point(init_trade_point, log_enum::source_type_trade_init_trade_point);
}

bool role_trade_t::buy_trade_item(uint32_t item_tid, uint32_t item_buy_price, uint32_t item_buy_num, uint32_t item_buy_max_num,
	proto::common::role_trade_item_single *p_change_item_single)
{
	if (NULL == p_change_item_single)
		return false;

	uint32_t new_buy_num = 0;
	uint32_t new_buy_price = 0;
	uint32_t can_buy_num = 0;

	trade_item_info_map::iterator it = m_trade_item_map.find(item_tid);
	role_trade_item_info_ptr p_role_trade_item_ptr = role_trade_item_info_ptr();
	if (it != m_trade_item_map.end())
	{
		p_role_trade_item_ptr = it->second;
		if (NULL == p_role_trade_item_ptr)
		{
			log_error("[wys] trade p_role_trade_item_ptr NULL");
			return false;
		}

		new_buy_num = p_role_trade_item_ptr->get_item_buy_num() + item_buy_num;
		///超过系统可购买数量 要计算附加可购买数量
		if (item_buy_num > p_role_trade_item_ptr->get_item_max_buy_num())
		{
			if (p_role_trade_item_ptr->get_item_max_buy_num_addi() > 0)
			{
				uint32_t can_buy_num_addi = p_role_trade_item_ptr->get_item_max_buy_num_addi() - (item_buy_num - p_role_trade_item_ptr->get_item_max_buy_num());
				p_role_trade_item_ptr->set_item_max_buy_num_addi(can_buy_num_addi);
				can_buy_num = can_buy_num_addi;
			}
			p_role_trade_item_ptr->set_item_max_buy_num(0);
		}
		else
		{
			can_buy_num = p_role_trade_item_ptr->get_item_max_buy_num() - item_buy_num;
			p_role_trade_item_ptr->set_item_max_buy_num(can_buy_num);
		}
		new_buy_price = p_role_trade_item_ptr->get_item_buy_price() +  item_buy_price;
		p_role_trade_item_ptr->set_item_buy_num(new_buy_num);
		p_role_trade_item_ptr->set_item_buy_price(new_buy_price);
	}
	else
	{
		can_buy_num = item_buy_max_num - item_buy_num;			///物品最大可购买数量(配置表配置) - 物品当次购买数量 
		role_trade_item_info_ptr trade_item_info = role_trade_item_info_ptr(new role_trade_item_info_t(item_tid,
			item_buy_price, item_buy_num, can_buy_num, 0));

		m_trade_item_map.insert(std::make_pair(item_tid, trade_item_info));

		new_buy_num = item_buy_num;
		new_buy_price = item_buy_price;
	}

	calc_trade_bag();

	p_change_item_single->set_item_tid(item_tid);
	p_change_item_single->set_item_buy_num(new_buy_num);
	p_change_item_single->set_item_max_buy_num(can_buy_num);
	// 	p_change_item_single->set_item_max_buy_num_addi(max_buy_num_addi);
	p_change_item_single->set_item_buy_price(new_buy_price);

	return true;
}

bool role_trade_t::sell_trade_item(uint32_t item_tid, uint32_t item_sell_num,
	proto::common::role_trade_item_single *p_change_item_single)
{
	if (NULL == p_change_item_single)
		return false;

	uint32_t new_buy_num = 0;
	uint32_t new_buy_price = 0;

	trade_item_info_map::iterator it = m_trade_item_map.find(item_tid);
	if (it == m_trade_item_map.end())
	{
		return false;
	}
	else
	{
		role_trade_item_info_ptr p_role_trade_item = it->second;
		if (NULL == p_role_trade_item)
		{
			log_error("[wys] trade p_role_trade_item NULL");
			return false;
		}
		if (p_role_trade_item->get_item_buy_num() == 0)
		{
			log_error("[wys] trade item_buy_num = 0");
			return false;
		}
		if (p_role_trade_item->get_item_buy_num() < item_sell_num)
		{
			log_error("[wys] trade item_buy_num < item_sell_num");
			return false;
		}

		new_buy_num = p_role_trade_item->get_item_buy_num() - item_sell_num;
		uint32_t price_single = 0;
		if (p_role_trade_item->get_item_buy_num() == 0)
			price_single = p_role_trade_item->get_item_buy_price();
		else
			price_single = p_role_trade_item->get_item_buy_price() / p_role_trade_item->get_item_buy_num();
		new_buy_price = price_single * new_buy_num;

		p_role_trade_item->set_item_buy_num(new_buy_num);
		p_role_trade_item->set_item_buy_price(new_buy_price);

		p_change_item_single->set_item_tid(item_tid);
		p_change_item_single->set_item_buy_num(new_buy_num);
		p_change_item_single->set_item_buy_price(new_buy_price);
	}
	calc_trade_bag();

	return true;
}

bool role_trade_t::supplement_trade_item(uint32_t item_tid, uint32_t item_buy_max_num, uint32_t item_num_addi)
{
	trade_item_info_map::iterator it = m_trade_item_map.find(item_tid);
	if (it == m_trade_item_map.end())
	{
		role_trade_item_info_ptr trade_item_info = role_trade_item_info_ptr(new role_trade_item_info_t(item_tid,
			0, 0, item_buy_max_num, item_num_addi));

		m_trade_item_map.insert(std::make_pair(item_tid, trade_item_info));
	}
	else
	{
		role_trade_item_info_ptr p_role_trade_item_ptr = it->second;
		if (NULL == p_role_trade_item_ptr)
		{
			log_error("[wys] trade p_role_trade_item_ptr NULL");
			return false;
		}
		uint32_t new_num = p_role_trade_item_ptr->get_item_max_buy_num_addi() + item_num_addi;
		p_role_trade_item_ptr->set_item_max_buy_num_addi(new_num);
	}
	return true;
}

void role_trade_t::set_trade_point(uint32_t trade_point, uint32_t source_type)
{
	if (trade_point == m_trade_point)
	{
		TRADE_LOG("trade_point == m_trade_point role[%lu]", m_role_uid);
		return;
	}

	role_ptr p_role = role_manager_t::find_role(m_role_uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role role[%lu]",m_role_uid);
		return;
	}

	auto conf = GET_CONF(Comprehensive, comprehensive_common::trade_point_limit);
	uint32_t max_trade_point = GET_COMPREHENSIVE_VALUE_1(conf);
	const uint32_t new_value = trade_point > max_trade_point ? max_trade_point : trade_point;
	uint32_t change_value = 0 , old_value = m_trade_point;
	uint32_t op_type = log_enum::opt_type_none;
	if (new_value < m_trade_point)
	{
		op_type = log_enum::opt_type_del;
		change_value = m_trade_point - new_value;
	} 
	else if(new_value > m_trade_point)
	{
		op_type = log_enum::opt_type_add;
		change_value = new_value - m_trade_point;
	}
	TRADE_LOG("role[%lu] trade_point[%d] max_trade_point[%d] cur_trade_point[%d]",m_role_uid, trade_point, max_trade_point, m_trade_point);
	m_trade_point = new_value;
	TRADE_LOG("role[%lu] new_trade_point[%d]", m_role_uid, m_trade_point);
	log_wrapper_t::send_money_log(m_role_uid, p_role->get_level(), old_value, change_value, new_value, 
		proto::common::MONEY_TYPE_TRADE_POINT, op_type, source_type,0);
}

uint32_t role_trade_t::get_trade_point()
{
	return m_trade_point;
}

void role_trade_t::reset_buy_num()
{
	m_last_reset_buy_num_time = common::time_util_t::now_time();
	const trade_manager_t::trade_map& trade_list = trade_manager_t::get_all_trade();
	std::map<uint32_t, trade_item_ptr> item_list;
	if (trade_list.empty())
	{
		log_error("trade_list.empty() role_uid[%lu]", m_role_uid);
		return;
	}
	for (const auto& it : trade_list)
	{
		trade_ptr p_trade = it.second;
		if (NULL == p_trade)
		{
			log_error("NULL == p_trade id[%d]",it.first);
			continue;
		}
		const trade_item_map&  list = p_trade->get_trade_item_list();
		for (const auto& item : list)
		{
			trade_item_ptr p_item = item.second;
			if (NULL == p_item)
			{
				log_error("NULL == p_item id[%d]",item.first);
				continue;
			}
			if (p_item->is_can_buy())
			{
				item_list.insert(std::make_pair(item.first,p_item));
			}
		}
	}
	if (item_list.empty())
	{
		log_error("item_list.empty() role_uid[%lu]",m_role_uid);
		return;
	}
	for (auto& role_item : m_trade_item_map)
	{
		role_trade_item_info_ptr p_item = role_item.second;
		if (NULL == p_item)
		{
			log_error("NULL == p_item role_uid[%lu] id[%d]",m_role_uid,role_item.first);
			continue;
		}
		auto iter = item_list.find(role_item.first);
		if (iter == item_list.end())
		{
			log_error("role[%lu]  item[%d] reset buy num fail!!!",m_role_uid, role_item.first);
			continue;
		}
		trade_item_ptr p_trade_item = iter->second;
		if (NULL == p_trade_item)
		{
			log_error("NULL == p_trade_item");
			continue;
		}
		p_item->set_item_max_buy_num(p_trade_item->get_can_buy_num());
	}
}

role_trade_item_info_ptr role_trade_t::get_role_trade_item_info(uint32_t item_tid)
{
	trade_item_info_map::iterator it = m_trade_item_map.find(item_tid);

	if (it != m_trade_item_map.end())
	{
		return it->second;
	}

	return role_trade_item_info_ptr();
}

uint32_t role_trade_t::calc_trade_bag()
{
	m_trade_bag_weight = 0;
	role_trade_item_info_ptr p_role_trade_item_ptr = role_trade_item_info_ptr();
	for (auto& it : m_trade_item_map)
	{
		p_role_trade_item_ptr = it.second;
		if (NULL == p_role_trade_item_ptr)
		{
			log_error("[wys] trade p_role_trade_item_ptr NULL");
			continue;
		}
		m_trade_bag_weight += p_role_trade_item_ptr->get_item_buy_num();
	}
	TRADE_LOG("role[%lu] trade bag weight[%d]",m_role_uid ,m_trade_bag_weight);
	return m_trade_bag_weight;
}

void role_trade_t::peek_data(proto::common::role_trade_info* trade_data)
{
	if (NULL == trade_data)
	{
		log_error("trade trade_data NULL");
		return;
	}
	trade_data->set_trade_bag_weight(m_trade_bag_weight);
	trade_data->set_trade_point(m_trade_point);
	trade_data->set_last_reset_buy_num_time(m_last_reset_buy_num_time);
	trade_data->set_total_week_gain(m_trade_week_total_gain);
}

void role_trade_t::peek_trade_item_data(proto::common::role_trade_item* trade_item_data,bool is_update)
{
	proto::common::role_trade_item_single *trade_item_single = NULL;
	role_trade_item_info_ptr p_role_trade_item_ptr = role_trade_item_info_ptr();
	for (auto& it : m_trade_item_map)
	{
		p_role_trade_item_ptr = it.second;
		if (NULL == p_role_trade_item_ptr)
		{
			log_error("[wys] trade p_role_trade_item_ptr NULL");
			continue;
		}
		if (!is_update)
		{
			if (0 == p_role_trade_item_ptr->get_item_buy_num() &&
				0 == p_role_trade_item_ptr->get_item_buy_price())
			{
				continue;
			}
		}
		
		trade_item_single = trade_item_data->add_trade_item_list();
		if (NULL == trade_item_single)
		{
			log_error("trade peek_trade_item_data add_trade_item_list fail");
			break;
		}
		trade_item_single->set_item_tid(p_role_trade_item_ptr->get_item_tid());
		trade_item_single->set_item_buy_num(p_role_trade_item_ptr->get_item_buy_num());
		trade_item_single->set_item_max_buy_num(p_role_trade_item_ptr->get_item_max_buy_num());
		trade_item_single->set_item_max_buy_num_addi(p_role_trade_item_ptr->get_item_max_buy_num_addi());
		trade_item_single->set_item_buy_price(p_role_trade_item_ptr->get_item_buy_price());
	}
}

void role_trade_t::save_trade_item_data(bool is_right_now, uint64_t uid)
{
	proto::common::role_trade_item trade_items;
	peek_trade_item_data(&trade_items,true);
	role_unify_save::add_task(m_key, trade_items, false, is_right_now, uid);
}

bool role_trade_t::notify_trade_item_info(proto::client::gc_trade_info_notify& msg ,bool is_onlogin,uint32_t item_tid )
{
	role_ptr p_role = role_manager_t::find_role(m_role_uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role role[%lu]", m_role_uid);
		return false;
	}
	const trade_manager_t::trade_hot_map& hot_map = trade_manager_t::get_trade_hot_map();
	if (hot_map.empty())
	{
		log_error("hot_map.empty()");
		return false;
	}

	for (auto& it : m_trade_item_map)
	{
		role_trade_item_info_ptr p_item = it.second;
		if (NULL == p_item)
		{
			log_error("NULL == p_item");
			continue;
		}

		if (0 == p_item->get_item_buy_num() &&
			0 == p_item->get_item_buy_price())
		{
			continue;
		}
		uint32_t item_id = p_item->get_item_tid();
		trade_manager_t::trade_hot_map::const_iterator iter = hot_map.find(item_id);
		if (iter == hot_map.end())
		{
			log_error("role[%d] hot_map not find item[%d] info",m_role_uid, item_id);
			continue;
		}
		uint32_t trade_item_tid = 0 , trade_id = 0, scene_id = 0,sour_trade_id = 0;
		trade_item_tid = iter->first;
		trade_id = std::get<0>(iter->second);
		scene_id = std::get<1>(iter->second);
		sour_trade_id = std::get<2>(iter->second);
		if (0 != trade_item_tid &&
			0 != trade_id &&
			0 != scene_id &&
			0 != sour_trade_id)
		{
			proto::common::trade_item_info* p_data = msg.add_item_info_list();
			p_data->set_item_tid(trade_item_tid);
			p_data->set_scene_id(scene_id);
			p_data->set_trade_id(trade_id);
			p_data->set_sour_trade_id(sour_trade_id);
			TRADE_LOG("tid[%d] scene[%d] trade[%d] sour_trade[%d]", trade_item_tid, scene_id, trade_id,sour_trade_id);
		}
		if (!is_onlogin)
		{
			break;
		}
	}
	TRADE_LOG("role[%lu] gc_trade_info_notify msg size[%d]", m_role_uid,msg.item_info_list_size());
	return true;
}

void role_trade_t::add_trade_week_total_gain(uint32_t gain)
{
	m_trade_week_total_gain += gain;
	//TRADE_LOG("role[%lu] trade week total gain[%d]", m_role_uid, m_trade_week_total_gain);
}

void role_trade_t::notify_trade_info(bool is_inlogin)
{
	role_ptr p_role = role_manager_t::find_role(m_role_uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role role[%lu]", m_role_uid);
		return;
	}

	proto::client::gc_trade_info_notify msg;
	if (notify_trade_item_info(msg, is_inlogin))
	{
		p_role->send_msg_to_client(op_cmd::gc_trade_info_notify, msg);
	}
}

void role_trade_t::load_data(const proto::common::role_trade_info &trade_info)
{
	m_last_reset_buy_num_time = trade_info.last_reset_buy_num_time();
	m_trade_point = trade_info.trade_point();
	m_trade_week_total_gain = trade_info.total_week_gain();
}

void role_trade_t::load_trade_item_data(const proto::common::role_trade_item &trade_item_info)
{
	m_trade_bag_weight = 0;
	for (int i = 0; i < trade_item_info.trade_item_list_size(); ++i)
	{
		const ::proto::common::role_trade_item_single& trade_item_single = trade_item_info.trade_item_list(i);

		role_trade_item_info_ptr trade_item_info = role_trade_item_info_ptr(new role_trade_item_info_t(
			trade_item_single.item_tid(), trade_item_single.item_buy_price(),
			trade_item_single.item_buy_num(), trade_item_single.item_max_buy_num(),
			trade_item_single.item_max_buy_num_addi()));

		m_trade_item_map.insert(std::make_pair(trade_item_single.item_tid(), trade_item_info));
		m_trade_bag_weight += trade_item_single.item_buy_num();

		//TRADE_LOG("[wys] load trade item:%d item_num:%d", trade_item_single.item_tid(), trade_item_single.item_buy_num());
	}
}

void role_trade_t::do_next_day(bool b_online_call)
{
	auto conf = GET_CONF(Comprehensive, comprehensive_common::trade_point_daily);
	uint32_t give_trade_point_day = GET_COMPREHENSIVE_VALUE_1(conf);
	set_trade_point(get_trade_point() + give_trade_point_day, log_enum::source_type_role_every_day_reset);

	///每周一重置最大贸易点
	if (b_online_call && time_util_t::get_weekday() == 1)
	{
		do_next_week();
		m_trade_week_total_gain = 0;
	}
}

void role_trade_t::do_next_week()
{
	auto conf = GET_CONF(Comprehensive, comprehensive_common::trade_point_keep);
	uint32_t give_trade_point_week = GET_COMPREHENSIVE_VALUE_1(conf);
	if (get_trade_point() > give_trade_point_week)
		set_trade_point(give_trade_point_week, log_enum::source_type_role_every_week_reset);
}
