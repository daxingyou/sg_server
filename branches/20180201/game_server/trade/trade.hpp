#pragma once

#include "macros.hpp"
#include "common.pb.h"
#include "trade_item.hpp"
#include <vector>
#include <unordered_map>
#include <boost/system/error_code.hpp>
#include <boost/asio/deadline_timer.hpp>

typedef std::map<uint32_t,trade_item_ptr> trade_item_map;

class trade_t
{
public:
	trade_t(uint32_t trade_id,uint32_t scene_id);
	~trade_t() {}

public:
	void init();
	void close();

public:
	///获取跑商ID
	uint32_t get_id() { return m_id; }

	///获取场景ID
	uint32_t get_scene_id() { return m_scene_id; }

	///获取贸易事件开始时间
	uint32_t get_event_start_time() { return m_event_start_time; }

	///获取贸易事件参数
	uint32_t get_event_param() { return m_event_param; }

	///获取贸易事件行情
	uint32_t get_event_market() { return m_event_market; }

	///获取贸易事件统计
	uint32_t get_event_count() { return m_event_count; }

	///获取贸易事件
	uint32_t get_event_type() { return m_event_type; }

public:
	///获取贸易物品
	trade_item_ptr			get_trade_item(uint32_t trade_item_tid);

	///获取贸易物品种类
	trade_item_type_ptr		get_trade_item_type(uint32_t trade_item_type);

	///获取全部贸易物品列表
	const trade_item_map&	get_trade_item_list() { return m_trade_item_list; }

	///购买城市贸易的物品
	bool					trade_buy_item(trade_item_ptr p_trade_item, uint32_t item_num);
	///出售城市贸易的物品
	bool					trade_sell_item(trade_item_ptr p_trade_item, uint32_t item_num);
	///检测城市贸易事件是否结束
	bool					check_trade_event_is_end();

	///获取繁荣度
	uint32_t get_prosperity();
	
	///获取占有度
	uint32_t get_hold();

	///获取繁荣度等级
	uint32_t get_prosperity_level();
public:
	///城市贸易行情到时处理
	bool	market_on_time();

	///城市贸易重置可购买数量到时处理
	bool	reset_buy_num_on_time();

	///城市贸易事件重置到时处理
	bool	event_reset_on_time(proto::common::trade_event_type event_type);

	///城市贸易事件到时处理
	void	event_on_time();

	///贸易事件停止
	void	event_stop();

	///开始贸易事件定时器
	void	start_event_timer();

	///取消贸易事件定时器
	void	cancel_event_timer();

private:
	uint32_t m_id = 0;						///跑商ID
	uint32_t m_scene_id = 0;				///sceneID
	uint32_t m_event_start_time = 0;		///贸易事件开始时间
	uint32_t m_event_param = 0;				///贸易事件的参数
	uint32_t m_event_market = 10000;		///贸易事件的行情
	uint32_t m_event_count = 0;				///贸易事件统计
	uint32_t m_event_type = 0;				///贸易事件

private:
	boost::shared_ptr<boost::asio::deadline_timer> m_timer_event;	///事件持续时间

	///trade_item_type_map				m_trade_item_type_list;			///跑商物品类型列表
	trade_item_map					m_trade_item_list;				///跑商物品列表
	std::vector<trade_item_ptr>		m_event_item_list;				///贸易事件所有商品
};

typedef boost::shared_ptr<trade_t>	trade_ptr;