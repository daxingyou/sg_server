#pragma once

#include "trade.hpp"
#include "protos_fwd.hpp"
#include <unordered_map>
#include <boost/system/error_code.hpp>
#include <boost/asio/deadline_timer.hpp>
#include <tuple>


class trade_manager_t
{
public:
	trade_manager_t();
	~trade_manager_t();
public:
	typedef std::map<uint32_t, trade_ptr> trade_map;
	typedef std::tuple<uint32_t, uint32_t,uint32_t> sug_trade_info;
	typedef std::map<uint32_t, std::tuple<uint32_t, uint32_t, uint32_t> > trade_hot_map;
public:
	static void			close();
	static bool			init();
	static bool			init_all_trade();
	static trade_ptr	get_trade(uint32_t trade_id);

	///贸易行情到时处理
	static void			market_on_time(const boost::system::error_code& ec);
	
	///贸易重置可购买数量到时处理
	static void			buy_num_on_time(const boost::system::error_code& ec);
	
	///贸易事件到时处理
	static void			event_reset_on_time(const boost::system::error_code& ec);

	///获取贸易列表
	static const trade_map&	get_all_trade() { return m_trade_map; }


	///推荐贸易
	static void			push_trade();

	///热销列表
	static const trade_hot_map& get_trade_hot_map() { return m_hot_map; }

public:
	///打开定时器
	static void			start_market_timer();
	static void			start_buy_num_timer();
	static void			start_event_timer();

	///关闭定时器
	static void			cancel_market_timer();
	static void			cancel_buy_num_timer();
	static void			cancel_event_timer();

	///获取定时器时间
	static uint32_t		get_market_reset_time() { return m_market_reset_time; }
	static uint32_t		get_buy_num_reset_time() { return m_buy_num_reset_time; }
	static uint32_t		get_event_reset_time() { return m_event_reset_time; }

public:
	///贸易事件到时处理
	static void			event_on_time(const boost::system::error_code& ec, uint32_t trade_id);

public:
	///获取推荐贸易信息
	static void			get_suggest_trade_info(uint32_t& trade_id,uint32_t& item_tid, uint32_t& sell_trade_id);

public:
	///组队跑商界面通知
	static uint32_t		troop_trade_view_notify(const uint64_t role_uid,const uint32_t trade_id);

private:
	static trade_map m_trade_map;												///贸易列表
	static sug_trade_info m_sug_trade_info;										///推荐跑商(跑商ID 物品ID 该物品为所有跑商中最贵的)
	static trade_hot_map  m_hot_map;											///货物热销列表
	static boost::shared_ptr<boost::asio::deadline_timer> m_market_timer;		///贸易行情定时器
	static boost::shared_ptr<boost::asio::deadline_timer> m_buy_num_timer;		///贸易可购买物品数量定时器
	static boost::shared_ptr<boost::asio::deadline_timer> m_event_timer;		///贸易事件定时器
	static uint32_t m_market_reset_time;										///贸易行情重置时间
	static uint32_t m_buy_num_reset_time;										///贸易可购买物品数量重置时间
	static uint32_t m_event_reset_time;											///贸易事件重置时间
};
