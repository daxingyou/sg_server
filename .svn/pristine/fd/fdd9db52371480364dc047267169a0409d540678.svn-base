#pragma once

#include "protos_fwd.hpp"
#include <map>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

/*
author: wys
time:2016.11.17
desc:role trade 角色贸易数据
*/

class role_trade_item_info_t
{
public:
	role_trade_item_info_t(uint32_t item_tid, uint32_t item_buy_price, uint32_t item_buy_num, uint32_t item_max_buy_num,
		uint32_t item_max_buy_num_addi)
	{
		m_item_tid = item_tid;
		m_item_buy_price = item_buy_price;
		m_item_buy_num = item_buy_num;
		m_item_max_buy_num = item_max_buy_num;
		m_item_max_buy_num_addi = item_max_buy_num_addi;
	};

	uint32_t	get_item_tid() { return m_item_tid; }
	uint32_t	get_item_buy_price() { return m_item_buy_price; }
	uint32_t	get_item_buy_num() { return m_item_buy_num; }
	uint32_t	get_item_max_buy_num() { return m_item_max_buy_num; }
	uint32_t	get_item_max_buy_num_addi() { return m_item_max_buy_num_addi; }

	uint32_t	get_item_can_buy_num()
	{
		return m_item_max_buy_num + m_item_max_buy_num_addi;
	}

	bool		can_buy(uint32_t buy_num) {
		if (buy_num <= get_item_can_buy_num())
			return true;
		return false;
	}

	void		set_item_buy_price(uint32_t item_buy_price) { m_item_buy_price = item_buy_price; }
	void		set_item_buy_num(uint32_t item_buy_num) { m_item_buy_num = item_buy_num; }
	void		set_item_max_buy_num(uint32_t item_max_buy_num) { m_item_max_buy_num = item_max_buy_num; }
	void		set_item_max_buy_num_addi(uint32_t item_max_buy_num_addi) { m_item_max_buy_num_addi = item_max_buy_num_addi; }
private:
	uint32_t m_item_tid = 0;
	uint32_t m_item_buy_price = 0;						///物品购买价格
	uint32_t m_item_buy_num = 0;						///物品可购买数量
	uint32_t m_item_max_buy_num = 0;					///物品最大购买数量
	uint32_t m_item_max_buy_num_addi = 0;				///物品补货购买数量
};
typedef boost::shared_ptr<role_trade_item_info_t> role_trade_item_info_ptr;


typedef std::map<uint32_t, role_trade_item_info_ptr> trade_item_info_map;

class role_trade_t : public boost::enable_shared_from_this<role_trade_t>
{
public:
	role_trade_t(uint64_t role_uid);
	~role_trade_t();

	void		init_trade();
	void		load_data(const proto::common::role_trade_info &trade_info);
	void		load_trade_item_data(const proto::common::role_trade_item &trade_item_info);

	void		do_next_day(bool b_online_call = false);
	void		do_next_week();

	///从城市购买
	bool		buy_trade_item(uint32_t item_tid, uint32_t item_buy_price, uint32_t item_buy_num, uint32_t item_buy_max_num,
		proto::common::role_trade_item_single *p_change_item_single);
	///在城市出售
	bool		sell_trade_item(uint32_t item_tid, uint32_t item_sell_num,
		proto::common::role_trade_item_single *p_change_item_single);
	///在城市补货
	bool		supplement_trade_item(uint32_t item_tid, uint32_t item_buy_max_num, uint32_t item_num_addi);

	void		set_trade_point(uint32_t trade_point, uint32_t source_type);
	uint32_t	get_trade_point();
	uint32_t	get_trade_bag_weight() { return m_trade_bag_weight; }

	uint32_t	get_trade_max_weight() { return m_trade_bag_max_weight; }
	void		set_trade_bag_max_weight(uint32_t max_weight) { m_trade_bag_max_weight = max_weight; }

	void		reset_buy_num();
	uint32_t	get_last_reset_buy_num_time() { return m_last_reset_buy_num_time; }
	uint32_t	calc_trade_bag();

	role_trade_item_info_ptr	get_role_trade_item_info(uint32_t item_tid);
	trade_item_info_map&		get_trade_item_map() { return m_trade_item_map; }

	void		peek_data(proto::common::role_trade_info* trade_data);
	void		peek_trade_item_data(proto::common::role_trade_item* trade_item_data,bool is_update = false);
	void		save_trade_item_data(bool is_right_now, uint64_t uid);

	bool		notify_trade_item_info(proto::client::gc_trade_info_notify& msg ,bool is_onlogin = false,uint32_t item_tid = 0);

	void		add_trade_week_total_gain(uint32_t gain);
	uint32_t	get_trade_total_week_gain() { return m_trade_week_total_gain; }

	void		notify_trade_info(bool is_inlogin = false);
private:
	trade_item_info_map			m_trade_item_map;
	std::string					m_key = "";
	uint64_t					m_role_uid = 0;
	uint32_t					m_last_reset_buy_num_time = 0;	///上次重置购买数量的时间
	uint32_t					m_trade_point = 0;				///贸易点
	uint32_t					m_trade_bag_weight = 0;			///跑商背包负重
	uint32_t					m_trade_bag_max_weight = 0;		///跑商背包最大负重
	uint32_t					m_trade_week_total_gain = 0;	///周累计盈利
};
typedef boost::shared_ptr<role_trade_t> role_trade_ptr;