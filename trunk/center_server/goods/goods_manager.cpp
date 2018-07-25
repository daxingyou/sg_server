#include "goods_manager.hpp"

#include "main/center_server.hpp"
#include "main/center_server_fwd.hpp"
#include "user/global_user_manager.hpp"
#include "client.pb.h"
#include "global_id.hpp"
#include "redis_client.hpp"
#include "config_mgr.h"
#include "utility.hpp"
#include "tblh/errcode_enum.hpp"
#include "log/log_wrapper.hpp"
#include "tblh/Comprehensive.tbls.h"
#include "red_point/red_point_manager.hpp"
#include "redis/global_user_data_mgr.hpp"

USING_NS_COMMON;

goods_map goods_manager_t::m_goods_map;
goods_map_by_tid goods_manager_t::m_selling_goods_map_by_tid;
goods_map_by_tid goods_manager_t::m_selling_rarity_map_by_tid;
goods_map_by_tid goods_manager_t::m_public_goods_map_by_tid;
goods_user_map goods_manager_t::m_user_sell_goods;
goods_user_care_map goods_manager_t::m_user_care_goods;
deal_price_map goods_manager_t::m_deal_price_map;
goods_item_by_type goods_manager_t::m_goods_conf;
std::map<uint32_t, Goods*> goods_manager_t::m_goods_conf_by_tid;
std::map<uint32_t, goods_vec> goods_manager_t::m_goods_search_map;


uint64_t goods_manager_t::get_new_goods_uid()
{
	return g_id_allocator.gen_new_id(ID_ALLOCATOR_TYPE_GOODS);
}

uint64_t goods_manager_t::get_new_care_uid()
{
	return g_id_allocator.gen_new_id(ID_ALLOCATOR_TYPE_GOODS_CARE);
}

void goods_manager_t::make_conf()
{
	m_goods_conf.clear();
	std::map<uint32_t, Goods*> goods_map;
	GET_ALL_CONF(Goods, goods_map);
	
	for (auto it : goods_map)
	{
		auto p_conf = it.second;
		if (NULL == p_conf)
		{
			continue;
		}
		uint32_t item_tid = p_conf->item_tid();
		m_goods_conf_by_tid[item_tid] = p_conf;
		for (auto type : p_conf->type())
		{
			m_goods_conf[type].push_back(item_tid);
		}
	}
}

void goods_manager_t::clear_conf()
{
	m_goods_conf.clear();
}

void goods_manager_t::load_data()
{
	//读取所有寄售行物品
	std::vector<uint64_t> goods_uids;
	redis_client_t::get_all_keys<uint64_t, std::vector<uint64_t>>("goods", goods_uids);

	for (auto goods_uid : goods_uids)
	{
		goods_ptr p_goods = goods_ptr(new goods_t);
		p_goods->set_uid(goods_uid);
		p_goods->set_seller_uid(redis_client_t::get_uint64("goods", "seller_uid", goods_uid));
		p_goods->set_item_tid(redis_client_t::get_uint32("goods", "item_tid", goods_uid));
		p_goods->set_item_num(redis_client_t::get_uint32("goods", "item_num", goods_uid));
		proto::common::equip_single es;
		redis_client_t::get_protobuf("goods", "equip_info", goods_uid, es);
		p_goods->load_equip_data(es);
		p_goods->set_rarity(redis_client_t::get_uint32("goods", "rarity", goods_uid));
		p_goods->set_item_price(redis_client_t::get_uint32("goods", "item_price", goods_uid));
		p_goods->set_selled_num(redis_client_t::get_uint32("goods", "selled_num", goods_uid));
		p_goods->set_profit(redis_client_t::get_uint32("goods", "profit", goods_uid));
		p_goods->set_state(redis_client_t::get_uint32("goods", "state", goods_uid));
		p_goods->set_expired_time(redis_client_t::get_uint32("goods", "expired_time", goods_uid));
		p_goods->set_heat(redis_client_t::get_uint32("goods", "heat", goods_uid));

		add_goods(p_goods);
	}

	//读取珍品出售记录
	std::vector<std::uint32_t> rarity_price_uids;
	redis_client_t::get_all_keys<uint32_t, std::vector<std::uint32_t>>("rarity_price", rarity_price_uids);
	for (auto rarity_price_uid : rarity_price_uids)
	{
		//最近成交价格
		std::string deal_price = redis_client_t::get_string("rarity_price", "deal_price", rarity_price_uid);
		std::vector<uint32_t> deal_price_vec;
		string_util_t::split<uint32_t>(deal_price, deal_price_vec, "$");
		
		m_deal_price_map.insert(std::make_pair(rarity_price_uid, deal_price_vec));
	}

	//读取玩家关注数据
	std::vector<std::uint64_t> goods_care_uids;
	redis_client_t::get_all_keys<uint64_t, std::vector<std::uint64_t>>("goods_care", goods_care_uids);
	for (auto goods_care_uid : goods_care_uids)
	{
		uint64_t role_uid = redis_client_t::get_uint64("goods_care", "role_uid", goods_care_uid);
		uint64_t goods_uid = redis_client_t::get_uint64("goods_care", "goods_uid", goods_care_uid);
		if (role_uid == 0 || goods_uid == 0)
		{
			continue;
		}
		m_user_care_goods[role_uid][goods_uid] = goods_care_uid;
	}
}

void goods_manager_t::goods_timeout(uint64_t goods_uid, const boost::system::error_code& ec)
{
	GOODS_LOG("goods[%lu] timeout", goods_uid);
	if (ec == boost::asio::error::operation_aborted)
	{
		
		return;
	}
	else if (ec)
	{
		log_error("Timer error: %s", ec.message().c_str());
		return;
	}
	goods_ptr p_goods = get_goods_by_uid(goods_uid);
	if (NULL == p_goods)
	{
		log_error("NULL == p_goods[%lu]", goods_uid);
		return;
	}
	if (p_goods->get_state() == proto::common::goods_state_selling)
	{
		p_goods->set_state(proto::common::goods_state_cancel);
		del_selling_goods(p_goods);
		GOODS_LOG("goods[%lu] selling timeout, auto cancel", goods_uid);
	}
	else if(p_goods->get_state() == proto::common::goods_state_public)
	{
		p_goods->set_state(proto::common::goods_state_selling);
		p_goods->set_expired_time(common::time_util_t::now_time() + get_rarity_sell_time());
		p_goods->start_timer();
		del_public_goods(p_goods);
		if (is_equip(p_goods->get_item_tid()))
		{
			m_selling_rarity_map_by_tid[p_goods->get_item_tid()].push_back(p_goods->get_uid());
		}
		else
		{
			m_selling_goods_map_by_tid[p_goods->get_item_tid()].push_back(p_goods->get_uid());
		}
		GOODS_LOG("goods[%lu] public timeout, auto selling", goods_uid);
	}
	p_goods->save_self();
}

void goods_manager_t::add_goods(goods_ptr p_goods)
{
	if (NULL == p_goods)
	{
		log_error("NULL == p_goods");
		return;
	}
	m_goods_map.insert(std::make_pair(p_goods->get_uid(), p_goods));
	if (common::time_util_t::now_time() >= p_goods->get_expired_time())
	{
		if (p_goods->get_state() == proto::common::goods_state_selling)
		{
			p_goods->set_state(proto::common::goods_state_cancel);
		}
		else if (p_goods->get_state() == proto::common::goods_state_public)
		{
			if (common::time_util_t::now_time() - p_goods->get_expired_time() >= get_rarity_sell_time())
			{
				p_goods->set_state(proto::common::goods_state_cancel);
			}
			else
			{
				p_goods->set_state(proto::common::goods_state_selling);
				p_goods->set_expired_time(p_goods->get_expired_time() + get_rarity_sell_time());
			}
		}
	}
	p_goods->start_timer();

	if (p_goods->get_state() == proto::common::goods_state_selling)
	{
		if (is_equip(p_goods->get_item_tid()) && p_goods->get_rarity() > 0)
		{
			m_selling_rarity_map_by_tid[p_goods->get_item_tid()].push_back(p_goods->get_uid());
		}
		else
		{
			m_selling_goods_map_by_tid[p_goods->get_item_tid()].push_back(p_goods->get_uid());
		}
	}
	else if(p_goods->get_state() == proto::common::goods_state_public)
	{
		m_public_goods_map_by_tid[p_goods->get_item_tid()].push_back(p_goods->get_uid());
	}
	uint64_t seller_uid = p_goods->get_seller_uid();
	if (p_goods->get_state() != proto::common::goods_state_return)
	{
		m_user_sell_goods[seller_uid].push_back(p_goods->get_uid());
	}
	
	p_goods->save_self();

	add_search_goods( p_goods );
}



void goods_manager_t::del_selling_goods(goods_ptr p_goods)
{
	if (NULL == p_goods)
	{
		log_error("NULL == p_goods");
		return;
	}

	if (is_equip(p_goods->get_item_tid()) && p_goods->get_rarity() > 0)
	{
		auto it = m_selling_rarity_map_by_tid.find(p_goods->get_item_tid());
		if (it == m_selling_rarity_map_by_tid.end())
		{
			log_error("m_selling_rarity_map_by_tid[%d] empty", p_goods->get_item_tid());
			return;
		}
		goods_vec& gv = it->second;
		auto it_vec = std::find(gv.begin(), gv.end(), p_goods->get_uid());
		if (it_vec == gv.end())
		{
			log_error("m_selling_rarity_map_by_tid[%d] goods[%lu] not found", p_goods->get_item_tid(), p_goods->get_uid());
			return;
		}
		gv.erase(it_vec);
		if (gv.empty())
		{
			m_selling_rarity_map_by_tid.erase(it);
		}
	}
	else
	{
		auto it = m_selling_goods_map_by_tid.find(p_goods->get_item_tid());
		if (it == m_selling_goods_map_by_tid.end())
		{
			log_error("m_selling_goods_map_by_tid[%d] empty", p_goods->get_item_tid());
			return;
		}
		goods_vec& gv = it->second;
		auto it_vec = std::find(gv.begin(), gv.end(), p_goods->get_uid());
		if (it_vec == gv.end())
		{
			log_error("m_selling_goods_map_by_tid[%d] goods[%lu] not found", p_goods->get_item_tid(), p_goods->get_uid());
			return;
		}
		gv.erase(it_vec);
		if (gv.empty())
		{
			m_selling_goods_map_by_tid.erase(it);
		}
	}

	del_search_goods(p_goods);
}

void goods_manager_t::del_public_goods(goods_ptr p_goods)
{
	if (NULL == p_goods)
	{
		log_error("NULL == p_goods");
		return;
	}
	auto it = m_public_goods_map_by_tid.find(p_goods->get_item_tid());
	if (it == m_public_goods_map_by_tid.end())
	{
		log_error("m_public_goods_map_by_tid[%d] empty", p_goods->get_item_tid());
		return;
	}
	goods_vec& gv = it->second;
	auto it_vec = std::find(gv.begin(), gv.end(), p_goods->get_uid());
	if (it_vec == gv.end())
	{
		log_error("m_public_goods_map_by_tid[%d] goods[%lu] not found", p_goods->get_item_tid(), p_goods->get_uid());
		return;
	}
	gv.erase(it_vec);
	if (gv.empty())
	{
		m_public_goods_map_by_tid.erase(it);
	}
}

void goods_manager_t::clear_user_sell_goods(goods_ptr p_goods)
{
	if (NULL == p_goods)
	{
		log_error("NULL == p_goods");
		return;
	}
	if (p_goods->get_state() != proto::common::goods_state_return && p_goods->get_state() != proto::common::goods_state_selled)
	{
		return;
	}
	if (p_goods->get_profit() > 0)
	{
		return;
	}
	uint64_t seller_uid = p_goods->get_seller_uid();
	uint64_t goods_uid = p_goods->get_uid();
	auto it = m_user_sell_goods.find(seller_uid);
	if (it == m_user_sell_goods.end())
	{
		log_error("user[%lu] has not sell goods[%lu]", seller_uid, goods_uid);
		return;
	}
	goods_vec& gv = it->second;
	auto it_vec = std::find(gv.begin(), gv.end(), goods_uid);
	if (it_vec == gv.end())
	{
		log_error("user[%lu] has not sell goods[%lu]", seller_uid, goods_uid);
		return;
	}
	gv.erase(it_vec);
	if (gv.empty())
	{
		m_user_sell_goods.erase(it);
	}
}

goods_ptr goods_manager_t::get_goods_by_uid(uint64_t goods_uid)
{
	auto it = m_goods_map.find(goods_uid);
	if (it == m_goods_map.end())
	{
		return goods_ptr(NULL);
	}
	return it->second;
}

void goods_manager_t::stop_timer()
{
	for (auto it : m_goods_map)
	{
		goods_ptr p_goods = it.second;
		if (NULL != p_goods)
		{
			p_goods->stop_timer();
		}
	}
}

void goods_manager_t::get_selling_list(uint32_t tab, uint32_t type, proto::client::ec_goods_selling_list_reply& reply)
{
	reply.set_tab(tab);
	reply.set_item_type(type);
	if (0 == tab)
	{
		if (1 == type)
		{
			get_selling_goods_list(type, reply, m_selling_rarity_map_by_tid);
		}
		else
		{
			get_selling_goods_list(type, reply, m_selling_goods_map_by_tid);
		}
	}
	else
	{
		if (type != 4)
		{
			get_selling_goods_list(type, reply, m_public_goods_map_by_tid);
		}
	}
}

void goods_manager_t::get_selling_data(uint64_t user_uid, uint32_t tab, uint32_t rarity, uint32_t item_tid, proto::client::ec_goods_selling_data_reply& reply)
{
	reply.set_tab(tab);
	reply.set_rarity(rarity);
	reply.set_item_tid(item_tid);
	if (0 == tab)
	{
		if (0 == rarity)
		{
			get_selling_goods_data(user_uid, item_tid, reply, m_selling_goods_map_by_tid);
		}
		else
		{
			get_selling_goods_data(user_uid, item_tid, reply, m_selling_rarity_map_by_tid);
		}
	}
	else
	{
		get_selling_goods_data(user_uid, item_tid, reply, m_public_goods_map_by_tid);
	}
}

void goods_manager_t::get_care_goods(uint64_t user_uid, uint32_t tab, proto::client::ec_goods_care_reply& reply)
{
	reply.set_tab(tab);
	auto it = m_user_care_goods.find(user_uid);
	if (it == m_user_care_goods.end())
	{
		return;
	}
	const goods_care_map& gcm = it->second;
	for (auto it_care : gcm)
	{
		uint64_t goods_uid = it_care.first;
		goods_ptr p_goods = get_goods_by_uid(goods_uid);
		if (NULL == p_goods)
		{
			log_error("NULL == p_goods[%lu]", goods_uid);
			continue;
		}
		if (0 == tab)
		{
			if (p_goods->get_state() == proto::common::goods_state_public)
			{
				continue;
			}
		}
		else
		{
			if (p_goods->get_state() != proto::common::goods_state_public)
			{
				continue;
			}
		}
		proto::common::goods_single* p_goods_single = reply.add_goods_self();
		p_goods->peek_data(p_goods_single);
		p_goods_single->set_care(1);
	}
	reply.set_care_num(get_care_num(user_uid));
}

void goods_manager_t::get_self_goods(uint64_t user_uid, proto::client::ec_goods_self_reply& reply)
{
	auto it = m_user_sell_goods.find(user_uid);
	if (it == m_user_sell_goods.end())
	{
		return;
	}
	const goods_vec& gv = it->second;
	for (auto goods_uid : gv)
	{
		goods_ptr p_goods = get_goods_by_uid(goods_uid);
		if (NULL == p_goods)
		{
			log_error("NULL == p_goods[%lu]", goods_uid);
			continue;
		}
		if (p_goods->get_state() == proto::common::goods_state_return && p_goods->get_profit() == 0)
		{
			continue;
		}
		if (p_goods->get_state() == proto::common::goods_state_selled && p_goods->get_profit() == 0)
		{
			continue;
		}
		proto::common::goods_single* p_goods_single = reply.add_goods_self();
		p_goods->peek_data(p_goods_single);
		p_goods_single->set_care(is_care(goods_uid, user_uid));
	}
}

uint32_t goods_manager_t::buy_goods(uint64_t user_uid, const std::string& user_name, uint64_t goods_uid, uint32_t item_tid, uint32_t buy_count, uint32_t price, proto::common::goods_item* p_item)
{
	goods_ptr p_goods = get_goods_by_uid(goods_uid);
	if (NULL == p_goods)
	{
		log_error("NULL == p_goods[%lu]", goods_uid);
		return errcode_enum::notice_unknown;
	}

	if ( p_goods->get_destine_uid() != 0 && p_goods->get_destine_uid() != user_uid)
	{
		return errcode_enum::trading_items_miss;
	}

	if (p_goods->get_state() != proto::common::goods_state_selling)
	{
		log_error("p_goods[%lu] isnot selling", goods_uid);
		return errcode_enum::trading_items_miss;
	}
	if (p_goods->get_item_tid() != item_tid)
	{
		log_error("p_goods[%lu:%d] item_tid[%d] invalid", goods_uid, p_goods->get_item_tid(), item_tid);
		return errcode_enum::notice_unknown;
	}
	if (p_goods->get_item_price() != price)
	{
		log_error("p_goods[%lu:%d] price[%d] invalid", goods_uid, p_goods->get_item_price(), price);
		return errcode_enum::notice_unknown;
	}
	if (buy_count > p_goods->get_item_num())
	{
		log_error("p_goods[%lu:%d:%d] count[%d] invalid", goods_uid, p_goods->get_item_tid(), p_goods->get_item_num(), buy_count);
		return errcode_enum::notice_unknown;
	}
	p_goods->set_selled_num(p_goods->get_selled_num() + buy_count);
	p_goods->set_profit(p_goods->get_profit() + buy_count*price);
	p_goods->set_item_num(p_goods->get_item_num() - buy_count);
	if (0 == p_goods->get_item_num())
	{
		p_goods->set_state(proto::common::goods_state_selled);
		del_selling_goods(p_goods);
	}
	if (p_goods->get_rarity() > 0)
	{
		add_deal_price(item_tid, price);
	}
	p_item->set_item_tid(item_tid);
	p_item->set_item_num(buy_count);
	p_goods->peek_equip_data(p_item->mutable_equip_info());
	GOODS_LOG("name[%d] special[%d]", p_item->equip_info().name(), p_item->equip_info().special());
	p_goods->save_self();
	GOODS_LOG("user[%lu] buy goods[%lu] item[%d] buy_count[%d] item_num[%d] profit[%d]",
		user_uid, goods_uid, p_goods->get_item_tid(), buy_count, p_goods->get_item_num(), p_goods->get_profit());
	send_goods_log(user_uid, user_name, p_goods, log_enum::source_type_goods_buy);
	//通知卖家红点
	red_point_manager_t::red_point_notify(p_goods->get_seller_uid(), proto::common::RED_POINT_GOODS);
	return errcode_enum::error_ok;
}

uint32_t goods_manager_t::sell_goods(uint64_t user_uid, const std::string& user_name, uint32_t price, const proto::common::goods_item& item_info)
{
	if (check_sell_full(user_uid))
	{
		log_error("user[%lu] sell_count full", user_uid);
		return errcode_enum::function_jump_map_not_open;
	}
	uint32_t item_tid = item_info.item_tid();
	uint32_t rarity = item_info.rarity();
	if (!check_price(item_tid, rarity, price))
	{
		log_error("user[%lu] item[%d] check_price[%d] failed", user_uid, item_tid, price);
		return errcode_enum::notice_unknown;
	}
	goods_ptr p_goods = goods_ptr(new goods_t);
	p_goods->set_uid(get_new_goods_uid());
	p_goods->set_seller_uid(user_uid);
	p_goods->set_item_tid(item_tid);
	p_goods->set_item_num(item_info.item_num());
	if (is_equip(item_tid))
	{
		p_goods->load_equip_data(item_info.equip_info());
		GOODS_LOG("name[%d] special[%d]", p_goods->get_name(), p_goods->get_special());
	}
	p_goods->set_rarity(rarity);
	p_goods->set_item_price(price);
	p_goods->set_profit(0);
	p_goods->set_heat(0);
	if (0 == rarity)
	{
		p_goods->set_state(proto::common::goods_state_selling);
		p_goods->set_expired_time(get_goods_sell_time() + common::time_util_t::now_time());
	}
	else
	{
		p_goods->set_state(proto::common::goods_state_public);
		p_goods->set_expired_time(get_public_time() + common::time_util_t::now_time());
	}
	
	add_goods(p_goods);
	GOODS_LOG("role[%lu] sell item[%d] num[%d] price[%d] goods[%lu]",
		user_uid, item_tid, p_goods->get_item_num(), price, p_goods->get_uid());
	send_goods_log(user_uid, user_name, p_goods, log_enum::source_type_goods_sell);
	return errcode_enum::error_ok;
}

uint32_t goods_manager_t::return_goods(uint64_t user_uid, const std::string& user_name, uint64_t goods_uid, proto::common::goods_item* p_item)
{
	if (NULL == p_item)
	{
		log_error("NULL == p_item");
		return errcode_enum::notice_unknown;
	}
	goods_ptr p_goods = get_goods_by_uid(goods_uid);
	if (NULL == p_goods)
	{
		log_error("NULL == p_goods[%lu]", goods_uid);
		return errcode_enum::notice_unknown;
	}
	if (p_goods->get_seller_uid() != user_uid)
	{
		log_error("p_goods[%lu] seller[%lu] not user_uid[%lu]", goods_uid, p_goods->get_seller_uid(), user_uid);
		return errcode_enum::notice_unknown;
	}
	if (p_goods->get_state() == proto::common::goods_state_selling)
	{
		del_selling_goods(p_goods);
	}
	else if(p_goods->get_state() == proto::common::goods_state_public)
	{
		del_public_goods(p_goods);
	}
	else if (p_goods->get_state() != proto::common::goods_state_cancel)
	{
		log_error("p_goods[%lu] state[%d] error", goods_uid, p_goods->get_state());
		return errcode_enum::notice_unknown;
	}
	p_goods->set_state(proto::common::goods_state_return);
	p_goods->stop_timer();
	p_goods->save_self();
	clear_user_sell_goods(p_goods);
	p_item->set_item_tid(p_goods->get_item_tid());
	p_item->set_item_num(p_goods->get_item_num());
	p_goods->peek_equip_data(p_item->mutable_equip_info());
	GOODS_LOG("user[%lu] return goods[%lu] item[%d] item_num[%d] profit[%d]",
		user_uid, goods_uid, p_goods->get_item_tid(), p_goods->get_item_num(), p_goods->get_profit());
	send_goods_log(user_uid, user_name, p_goods, log_enum::source_type_goods_return);
	return errcode_enum::error_ok;
}

uint32_t goods_manager_t::resell_goods(uint64_t user_uid, const std::string& user_name, uint64_t goods_uid, uint32_t sell_count, uint32_t price)
{
	goods_ptr p_goods = get_goods_by_uid(goods_uid);
	if (NULL == p_goods)
	{
		log_error("NULL == p_goods[%lu]", goods_uid);
		return errcode_enum::notice_unknown;
	}
	if (p_goods->get_seller_uid() != user_uid)
	{
		log_error("p_goods[%lu] seller[%lu] not user_uid[%lu]", goods_uid, p_goods->get_seller_uid(), user_uid);
		return errcode_enum::notice_unknown;
	}
	
	uint32_t item_tid = p_goods->get_item_tid();
	uint32_t rarity = p_goods->get_rarity();
	if (!check_price(item_tid, rarity, price))
	{
		log_error("user[%lu] item[%d] check_price[%d] failed", user_uid, item_tid, price);
		return errcode_enum::notice_unknown;
	}
	if (p_goods->get_state() == proto::common::goods_state_selling)
	{
		if (price == p_goods->get_item_price())
		{
			log_error("p_goods[%lu] same price", goods_uid);
			return errcode_enum::notice_unknown;
		}
		del_selling_goods(p_goods);
	}
	else if (p_goods->get_state() == proto::common::goods_state_public)
	{
		if (price == p_goods->get_item_price())
		{
			log_error("p_goods[%lu] same price", goods_uid);
			return errcode_enum::notice_unknown;
		}
		del_public_goods(p_goods);
	}
	else if (p_goods->get_state() != proto::common::goods_state_cancel)
	{
		log_error("p_goods[%lu] state[%d] error", goods_uid, p_goods->get_state());
		return errcode_enum::trading_operation_items_miss;
	}
	p_goods->set_state(proto::common::goods_state_return);
	p_goods->stop_timer();
	p_goods->save_self();
	clear_user_sell_goods(p_goods);
	send_goods_log(user_uid, user_name, p_goods, log_enum::source_type_goods_resell);

	goods_ptr p_new_goods = goods_ptr(new goods_t);
	p_new_goods->set_uid(get_new_goods_uid());
	p_new_goods->set_seller_uid(user_uid);
	p_new_goods->set_item_tid(item_tid);
	p_new_goods->set_item_num(p_goods->get_item_num());
	if (is_equip(item_tid))
	{
		proto::common::equip_single es;
		p_goods->peek_equip_data(&es);
		p_new_goods->load_equip_data(es);
	}
	p_new_goods->set_rarity(rarity);
	p_new_goods->set_item_price(price);
	p_new_goods->set_profit(0);
	p_new_goods->set_heat(0);
	if (0 == rarity)
	{
		p_new_goods->set_state(proto::common::goods_state_selling);
		p_new_goods->set_expired_time(get_goods_sell_time() + common::time_util_t::now_time());
	}
	else
	{
		p_new_goods->set_state(proto::common::goods_state_public);
		p_new_goods->set_expired_time(get_public_time() + common::time_util_t::now_time());
	}

	add_goods(p_new_goods);
	GOODS_LOG("user[%lu] resell goods[%lu] item[%d] item_num[%d] price[%d] new goods[%lu] price[%d]",
		user_uid, goods_uid, p_goods->get_item_tid(), p_goods->get_item_num(), p_goods->get_item_price(),
		p_new_goods->get_uid(), price);
	send_goods_log(user_uid, user_name, p_new_goods, log_enum::source_type_goods_resell);
	return errcode_enum::error_ok;
}

uint32_t goods_manager_t::profit_goods(uint64_t user_uid, const std::string& user_name, uint32_t& profit)
{
	auto it = m_user_sell_goods.find(user_uid);
	if (it == m_user_sell_goods.end())
	{
		log_error("user[%lu] has no profit", user_uid);
		return errcode_enum::notice_unknown;
	}
	goods_vec gv = it->second;
	for (auto goods_uid : gv)
	{
		goods_ptr p_goods = get_goods_by_uid(goods_uid);
		if (NULL == p_goods)
		{
			log_error("NULL == p_goods[%lu]", goods_uid);
			continue;
		}
		if (p_goods->get_profit() == 0)
		{
			continue;
		}
		profit += p_goods->get_profit();
		p_goods->set_profit(0);
		p_goods->set_selled_num(0);
		p_goods->save_self();
		clear_user_sell_goods(p_goods);
		send_goods_log(user_uid, user_name, p_goods, log_enum::source_type_goods_profit);
	}
	if (0 == profit)
	{
		log_error("user[%lu] has no profit", user_uid);
		return errcode_enum::notice_unknown;
	}
	GOODS_LOG("user[%lu] profit[%d]", user_uid, profit);
	return errcode_enum::error_ok;
}

uint32_t goods_manager_t::care_goods(uint64_t user_uid, uint64_t goods_uid, uint32_t operation, uint32_t& heat)
{
	goods_ptr p_goods = get_goods_by_uid(goods_uid);
	if (NULL == p_goods)
	{
		log_error("NULL == p_goods[%lu]", goods_uid);
		return errcode_enum::notice_unknown;
	}
	uint32_t reply_code = errcode_enum::error_ok;
	if (0 == operation)
	{
		reply_code = del_care_goods(user_uid, p_goods);
	}
	else
	{
		if (p_goods->get_state() != proto::common::goods_state_public && p_goods->get_state() != proto::common::goods_state_selling)
		{
			log_error("p_goods[%lu] state[%d] error", goods_uid, p_goods->get_state());
			return errcode_enum::notice_unknown;
		}
		if (check_care_full(user_uid))
		{
			log_error("user[%lu] check_care_full failed", user_uid);
			return errcode_enum::trading_items_max;
		}
		reply_code = add_care_goods(user_uid, p_goods);
	}
	heat = p_goods->get_heat();
	return reply_code;
}

void goods_manager_t::clear_care(uint64_t user_uid, proto::client::ec_goods_clear_care_reply& reply)
{
	reply.set_reply_code(errcode_enum::error_ok);
	auto it = m_user_care_goods.find(user_uid);
	if (it == m_user_care_goods.end())
	{
		return;
	}
	std::vector<goods_ptr> wait_del_care_goods;
	const goods_care_map& gcm = it->second;
	for (auto it_care : gcm)
	{
		uint64_t goods_uid = it_care.first;
		goods_ptr p_goods = get_goods_by_uid(goods_uid);
		if (NULL == p_goods)
		{
			log_error("NULL == p_goods[%lu]", goods_uid);
			continue;
		}
		if (p_goods->get_state() == proto::common::goods_state_public)
		{
			continue;
		}
		if (p_goods->get_state() == proto::common::goods_state_selling)
		{
			proto::common::goods_single* p_goods_single = reply.add_goods_self();
			p_goods->peek_data(p_goods_single);
			p_goods_single->set_care(1);
			log_warn("add_goods_self");
		}
		else
		{
			wait_del_care_goods.push_back(p_goods);
		}
	}
	for (auto p_goods : wait_del_care_goods)
	{
		del_care_goods(user_uid, p_goods);
	}
}

uint32_t goods_manager_t::cancel_goods(uint64_t user_uid, const std::string& user_name, uint64_t goods_uid)
{
	goods_ptr p_goods = get_goods_by_uid(goods_uid);
	if (NULL == p_goods)
	{
		log_error("NULL == p_goods[%lu]", goods_uid);
		return errcode_enum::notice_unknown;
	}
	if (p_goods->get_seller_uid() != user_uid)
	{
		log_error("p_goods[%lu] seller[%lu] not user_uid[%lu]", goods_uid, p_goods->get_seller_uid(), user_uid);
		return errcode_enum::notice_unknown;
	}

	if ( p_goods->get_destine_uid() != 0 && p_goods->get_destine_uid() != user_uid)
	{
		return errcode_enum::trading_items_miss;
	}

	if (p_goods->get_state() == proto::common::goods_state_selling)
	{
		del_selling_goods(p_goods);
	}
	else if (p_goods->get_state() == proto::common::goods_state_public)
	{
		del_public_goods(p_goods);
	}
	else if (p_goods->get_state() == proto::common::goods_state_selled)
	{
		log_error("p_goods[%lu] selled", goods_uid);
		return errcode_enum::trading_operation_items_miss;
	}
	else if (p_goods->get_state() == proto::common::goods_state_cancel)
	{
		log_error("p_goods[%lu] cancel", goods_uid);
		return errcode_enum::trading_operation_items_shelves;
	}
	else
	{
		log_error("p_goods[%lu] state[%d] error", goods_uid, p_goods->get_state());
		return errcode_enum::notice_unknown;
	}
	p_goods->set_state(proto::common::goods_state_cancel);
	p_goods->stop_timer();
	p_goods->save_self();
	GOODS_LOG("user[%lu] cancel goods[%lu] item[%d] item_num[%d] profit[%d]",
		user_uid, goods_uid, p_goods->get_item_tid(), p_goods->get_item_num(), p_goods->get_profit());
	send_goods_log(user_uid, user_name, p_goods, log_enum::source_type_goods_cancel);
	return errcode_enum::error_ok;
}

uint32_t goods_manager_t::get_profit(uint64_t user_uid)
{
	auto it = m_user_sell_goods.find(user_uid);
	if (it == m_user_sell_goods.end())
	{
		return 0;
	}
	goods_vec gv = it->second;
	uint32_t profit = 0;
	for (auto goods_uid : gv)
	{
		goods_ptr p_goods = get_goods_by_uid(goods_uid);
		if (NULL == p_goods)
		{
			log_error("NULL == p_goods[%lu]", goods_uid);
			continue;
		}
		if (p_goods->get_profit() == 0)
		{
			continue;
		}
		profit += p_goods->get_profit();
	}
	return profit;
}

void goods_manager_t::get_selling_goods_list(uint32_t type, proto::client::ec_goods_selling_list_reply& reply, const goods_map_by_tid& gmbt)
{
	auto it_type = m_goods_conf.find(type);
	if (it_type == m_goods_conf.end())
	{
		log_error("goods_conf type[%d] invalid", type);
		return;
	}
	const std::vector<uint32_t>& item_list = it_type->second;
	for (auto item_tid : item_list)
	{
		auto it = gmbt.find(item_tid);
		if (it == gmbt.end())
		{
			continue;
		}
		const goods_vec& gv = it->second;
		uint32_t sell_num = 0;
		for (auto goods_uid : gv)
		{
			auto p_goods = get_goods_by_uid(goods_uid);
			if (NULL == p_goods)
			{
				continue;
			}
			sell_num += p_goods->get_item_num();
		}
		proto::common::goods_info* p_goods_info = reply.add_goods_info();
		p_goods_info->set_item_tid(item_tid);
		p_goods_info->set_selling_num(sell_num);
	}
}

void goods_manager_t::get_selling_goods_data(uint64_t user_uid, uint32_t item_tid, proto::client::ec_goods_selling_data_reply& reply, const goods_map_by_tid& gmbt)
{
	auto it = gmbt.find(item_tid);
	if (it == gmbt.end())
	{
		return;
	}
	const goods_vec& gv = it->second;

	for (auto goods_uid : gv)
	{
		goods_ptr p_goods = get_goods_by_uid(goods_uid);
		if (NULL == p_goods)
		{
			log_error("NULL == p_goods[%lu]", goods_uid);
			continue;
		}
		proto::common::goods_single* p_goods_single = reply.add_goods_bottom();
		p_goods->peek_data(p_goods_single);
		p_goods_single->set_care(is_care(goods_uid, user_uid));
	}
	if (gv.size() > get_display_count())
	{
		goods_vec gv_desc = gv;
		common::Sort::Qsort(gv_desc, 0, gv_desc.size() - 1, goods_manager_t::goods_price_sort_desc);
		for (auto goods_uid : gv_desc)
		{
			goods_ptr p_goods = get_goods_by_uid(goods_uid);
			if (NULL == p_goods)
			{
				log_error("NULL == p_goods[%lu]", goods_uid);
				continue;
			}
			proto::common::goods_single* p_goods_single = reply.add_goods_top();
			p_goods->peek_data(p_goods_single);
			p_goods_single->set_care(is_care(goods_uid, user_uid));
		}
	}
}

bool goods_manager_t::is_equip(uint32_t item_tid)
{
	return item_tid / 10000 == equip_prefix;
}

bool goods_manager_t::goods_price_sort_asc(const uint64_t& goods1, const uint64_t& goods2)
{
	goods_ptr p_goods1 = get_goods_by_uid(goods1);
	goods_ptr p_goods2 = get_goods_by_uid(goods2);
	if (NULL == p_goods1 || NULL == p_goods2)
	{
		return goods1 < goods2;
	}
	if (p_goods1->get_item_price() < p_goods2->get_item_price())
	{
		return true;
	}
	else if(p_goods1->get_item_price() > p_goods2->get_item_price())
	{
		return false;
	}
	else
	{
		return goods1 < goods2;
	}
}

bool goods_manager_t::goods_price_sort_desc(const uint64_t& goods1, const uint64_t& goods2)
{
	goods_ptr p_goods1 = get_goods_by_uid(goods1);
	goods_ptr p_goods2 = get_goods_by_uid(goods2);
	if (NULL == p_goods1 || NULL == p_goods2)
	{
		return goods1 < goods2;
	}
	if (p_goods1->get_item_price() < p_goods2->get_item_price())
	{
		return false;
	}
	else if (p_goods1->get_item_price() > p_goods2->get_item_price())
	{
		return true;
	}
	else
	{
		return goods1 < goods2;
	}
}

uint32_t goods_manager_t::get_display_count()
{
	return 30;
}

uint32_t goods_manager_t::get_care_num(uint64_t user_uid)
{
	auto it = m_user_care_goods.find(user_uid);
	if (it == m_user_care_goods.end())
	{
		return 0;
	}
	const goods_care_map& gcm = it->second;
	return gcm.size();
}

uint32_t goods_manager_t::is_care(uint64_t goods_uid, uint64_t user_uid)
{
	auto it_user = m_user_care_goods.find(user_uid);
	if (it_user == m_user_care_goods.end())
	{
		return 0;
	}
	const goods_care_map& gcm = it_user->second;
	auto it = gcm.find(goods_uid);
	if (it == gcm.end())
	{
		return 0;
	}
	return 1;
}

uint32_t goods_manager_t::get_max_sell_count()
{
	auto max_sell_count = GET_CONF(Comprehensive, comprehensive_common::trading_shelves_number);
	uint32_t val = GET_COMPREHENSIVE_VALUE_1(max_sell_count);
	return val;
}

uint32_t goods_manager_t::get_deal_count()
{
	return 10;
}

bool goods_manager_t::check_sell_full(uint64_t user_uid)
{
	auto it = m_user_sell_goods.find(user_uid);
	if (it == m_user_sell_goods.end())
	{
		return false;
	}
	const goods_vec& gv = it->second;
	if (gv.size() < get_max_sell_count())
	{
		return false;
	}
	uint32_t sell_count = 0;
	for (auto goods_uid : gv)
	{
		goods_ptr p_goods = get_goods_by_uid(goods_uid);
		if (NULL == p_goods)
		{
			log_error("NULL == p_goods[%lu]", goods_uid);
			continue;
		}
		if (p_goods->get_state() == proto::common::goods_state_selled || p_goods->get_state() == proto::common::goods_state_return)
		{
			continue;
		}
		++sell_count;
	}
	if (sell_count < get_max_sell_count())
	{
		return false;
	}
	return true;
}

bool goods_manager_t::check_care_full(uint64_t user_uid)
{
	auto it = m_user_care_goods.find(user_uid);
	if (it == m_user_care_goods.end())
	{
		return false;
	}
	const goods_care_map& gcm = it->second;
	if (gcm.size() < get_care_max_num())
	{
		return false;
	}
	return true;
}

bool goods_manager_t::check_price(uint32_t item_tid, uint32_t rarity, uint32_t price)
{
	if (0 == rarity)
	{
		auto it = m_goods_conf_by_tid.find(item_tid);
		if (it == m_goods_conf_by_tid.end())
		{
			log_error("goods_conf[%d] not found", item_tid);
			return false;
		}
		auto p_conf = it->second;
		if (NULL == p_conf)
		{
			log_error("goods_conf[%d] not found", item_tid);
			return false;
		}
		if (price < p_conf->price() / 2 || price > p_conf->price() * 3 / 2)
		{
			log_error(" price[%d] invalid", price);
			return false;
		}
	}
	return true;
}

uint32_t goods_manager_t::get_default_price(uint32_t item_tid)
{
	auto it_conf = m_goods_conf_by_tid.find(item_tid);
	if (it_conf == m_goods_conf_by_tid.end())
	{
		log_error("goods_conf[%d] not found", item_tid);
		return 0;
	}
	auto p_conf = it_conf->second;
	if (NULL == p_conf)
	{
		log_error("goods_conf[%d] not found", item_tid);
		return 0;
	}
	auto it = m_deal_price_map.find(item_tid);
	if (it != m_deal_price_map.end())
	{
		const std::vector<uint32_t> deal_price_vec = it->second;
		if (deal_price_vec.size() >= get_deal_count())
		{
			uint32_t total_price = p_conf->price();
			for (auto deal_price : deal_price_vec)
			{
				total_price += deal_price;
			}
			return total_price / (get_deal_count() + 1);
		}
	}

	return p_conf->price();
}

uint32_t goods_manager_t::get_public_time()
{
	auto public_time = GET_CONF(Comprehensive, comprehensive_common::trading_treasures_publicity_time);
	uint32_t val = GET_COMPREHENSIVE_VALUE_1(public_time);
	return val * 60;
}

uint32_t goods_manager_t::get_goods_sell_time()
{
	auto goods_sell_time = GET_CONF(Comprehensive, comprehensive_common::trading_shelves_time);
	uint32_t val = GET_COMPREHENSIVE_VALUE_1(goods_sell_time);
	return val * 60;
}

uint32_t goods_manager_t::get_rarity_sell_time()
{
	auto rarity_sell_time = GET_CONF(Comprehensive, comprehensive_common::trading_shelves_publicity_time);
	uint32_t val = GET_COMPREHENSIVE_VALUE_1(rarity_sell_time);
	return val * 60;
}

uint32_t goods_manager_t::get_care_max_num()
{
	auto care_max_num = GET_CONF(Comprehensive, comprehensive_common::trading_concern_number);
	uint32_t val = GET_COMPREHENSIVE_VALUE_1(care_max_num);
	return val;
}

uint32_t goods_manager_t::add_care_goods(uint64_t user_uid, goods_ptr p_goods)
{
	if (NULL == p_goods)
	{
		log_error("NULL == p_goods");
		return errcode_enum::notice_unknown;
	}
	uint64_t goods_uid = p_goods->get_uid();
	if (m_user_care_goods[user_uid].find(goods_uid) != m_user_care_goods[user_uid].end())
	{
		log_error("user[%lu] alraedy cared goods[%lu]", user_uid, goods_uid);
		return errcode_enum::notice_unknown;
	}
	uint32_t heat = p_goods->get_heat();
	p_goods->set_heat(heat+1);
	
	uint64_t care_uid = get_new_care_uid();
	m_user_care_goods[user_uid][goods_uid] = care_uid;
	redis_client_t::set_uint64("goods_care", "goods_uid", care_uid, goods_uid);
	redis_client_t::set_uint64("goods_care", "role_uid", care_uid, user_uid);
	return errcode_enum::error_ok;
}

uint32_t goods_manager_t::del_care_goods(uint64_t user_uid, goods_ptr p_goods)
{
	if (NULL == p_goods)
	{
		log_error("NULL == p_goods");
		return errcode_enum::notice_unknown;
	}
	uint64_t goods_uid = p_goods->get_uid();
	auto it_user = m_user_care_goods.find(user_uid);
	if (it_user == m_user_care_goods.end())
	{
		log_error("user[%lu] has not cared goods[%lu]", user_uid, goods_uid);
		return errcode_enum::notice_unknown;
	}
	goods_care_map& gcm = it_user->second;
	auto it = gcm.find(goods_uid);
	if (it == gcm.end())
	{
		log_error("user[%lu] has not cared goods[%lu]", user_uid, goods_uid);
		return errcode_enum::notice_unknown;
	}
	redis_client_t::remove_key("goods_care", it->second);
	gcm.erase(it);
	if (gcm.empty())
	{
		m_user_care_goods.erase(it_user);
	}
	uint32_t heat = p_goods->get_heat();
	if (heat > 0)
	{
		p_goods->set_heat(heat-1);
	}
	return errcode_enum::error_ok;
}

void goods_manager_t::add_deal_price(uint32_t item_tid, uint32_t price)
{
	auto it = m_deal_price_map.find(item_tid);
	if (it != m_deal_price_map.end())
	{
		std::vector<uint32_t>& deal_price_vec = it->second;
		if (deal_price_vec.size() >= get_deal_count())
		{
			deal_price_vec.erase(deal_price_vec.begin());
		}
	}
	m_deal_price_map[item_tid].push_back(price);

	std::ostringstream ss;
	for (auto deal_price : m_deal_price_map[item_tid])
	{
		ss << deal_price << "$";
	}
	std::string deal_price_str = ss.str().substr(0, ss.str().length() - 1);
	redis_client_t::set_string("rarity_price", "deal_price", item_tid, deal_price_str);

	ss.str("");
	ss.clear();
}

void goods_manager_t::send_goods_log(uint64_t role_uid, const std::string& role_name, goods_ptr p_goods, common::log_enum::logs_source_type_t type)
{
	if (NULL == p_goods)
	{
		log_error("NULL == p_goods");
		return;
	}
	log_wrapper_t::send_goods_log(role_uid, role_name, p_goods->get_uid(), p_goods->get_item_tid(),
		p_goods->get_item_num(), p_goods->get_item_price(), p_goods->get_selled_num(),
		p_goods->get_profit(), p_goods->get_state(), type);
}


Goods* goods_manager_t::get_goods_conf(uint32_t item_tid)
{
	auto iter_f = m_goods_conf_by_tid.find(item_tid);
	if (iter_f == m_goods_conf_by_tid.end())
	{
		return NULL;
	}
	Goods *p_conf = iter_f->second;
	if (NULL == p_conf)
	{
		return NULL;
	}
	return p_conf;
}

//添加查询映射
void goods_manager_t::add_search_goods( goods_ptr p_goods )
{
	if (NULL == p_goods)
	{
		return;
	}

	//添加到查询ID对应的索引列表(非珍品)
	Goods *p_conf = get_goods_conf(p_goods->get_item_tid());
	if (!p_conf)
	{
		log_error("add goods tid[%u] config is NULL", p_goods->get_item_tid());
		return;
	}

	uint32_t search_id = p_conf->search_id();
	if (p_goods->get_rarity() == 0 && search_id > 0)
	{
		m_goods_search_map[ search_id ].push_back( p_goods->get_uid() );
	}
}

//删除查询映射
void goods_manager_t::del_search_goods(goods_ptr p_goods )
{
	if (NULL == p_goods)
	{
		return;
	}

	//添加到查询ID对应的索引列表(非珍品)
	Goods *p_conf = get_goods_conf(p_goods->get_item_tid());
	if (!p_conf)
	{
		log_error("add goods tid[%u] config is NULL", p_goods->get_item_tid());
		return;
	}

	auto iter_f = m_goods_search_map.find( p_conf->search_id() );
	if ( iter_f == m_goods_search_map.end()) 
		return;
	
	goods_vec& gv = iter_f->second;
	auto it_vec = std::find(gv.begin(), gv.end(), p_goods->get_uid());
	if (it_vec == gv.end())
	{
		log_error("m_goods_search_map[%d] goods[%lu] not found", p_conf->search_id(), p_goods->get_uid());
		return;
	}

	gv.erase(it_vec);

	if (gv.empty())
	{
		m_goods_search_map.erase( iter_f );
	}
}

//预定某个道具	
uint32_t goods_manager_t::destine_goods(uint64_t role_uid, uint32_t search_id, proto::server::eg_destine_item_reply &reply)
{
	auto iter_f = m_goods_search_map.find( search_id );
	if ( iter_f == m_goods_search_map.end() ) 
		return errcode_enum::notice_unknown;

	auto &all_tmp_list = iter_f->second;
	std::vector<uint64_t> tmp_list;
	if (all_tmp_list.size() <= 1000)
	{
		tmp_list.assign(all_tmp_list.begin(), all_tmp_list.end());
	}
	else 
	{
		tmp_list.assign(all_tmp_list.begin(), all_tmp_list.begin() + 1000);
	}
	
	/*
	std::random_shuffle(tmp_list.begin(), tmp_list.end());
	*/
	//策划要求换排序
	std::sort(tmp_list.begin(), tmp_list.end(), goods_manager_t::sort_dents_goods);

	uint64_t p_goods_uid = 0;
	for (std::vector<uint64_t>::iterator iter = tmp_list.begin(); iter != tmp_list.end(); ++iter)
	{
		goods_ptr p_goods = get_goods_by_uid(*iter);
		if (NULL == p_goods)
			continue;

		if (p_goods->get_state() == proto::common::goods_state_selling)
		{
			p_goods->set_destine_uid( role_uid );
			p_goods_uid = p_goods->get_uid();
			reply.set_goods_uid(p_goods_uid);
			reply.set_price(p_goods->get_item_price());
			reply.set_item_id(p_goods->get_item_tid());
			break;
		}
	}
	
	if (p_goods_uid == 0)
		return errcode_enum::notice_unknown;

	return errcode_enum::error_ok;
}

//解除预定某个道具
void goods_manager_t::undestine_goods(uint64_t role_uid, uint64_t goods_uid)
{
	goods_ptr p_goods = get_goods_by_uid( goods_uid );
	if (NULL == p_goods)
	{
		log_error("user[%lu], undestine goods[%lu] is not found", role_uid, goods_uid);
		return;
	}
	p_goods->set_destine_uid(0);
}


bool goods_manager_t::sort_dents_goods(uint64_t auid, uint64_t buid)
{
	goods_ptr p_goods_a = get_goods_by_uid(auid);
	if (NULL == p_goods_a)
		return false;

	goods_ptr p_goods_b = get_goods_by_uid(buid);
	if (NULL == p_goods_b)
		return false;

	return p_goods_a->get_item_price() < p_goods_b->get_item_price();
}

