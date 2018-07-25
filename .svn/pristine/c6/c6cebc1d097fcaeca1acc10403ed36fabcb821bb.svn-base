#pragma once

#include <boost/shared_ptr.hpp>

enum trade_market_type : uint32_t
{
	trade_market_type_none = 0,
	trade_market_type_up_reset = 1,
	trade_market_type_down_reset = 2,
	trade_market_type_change_up = 3,
	trade_market_type_change_down = 4,

	trade_market_type_count
};
class trade_item_t
{
public:
	trade_item_t(uint32_t trade_id,uint32_t item_tid,uint32_t item_trade_type,uint32_t item_base_sell_price,uint32_t item_base_buy_price);
	~trade_item_t();

public:
	///获取物品ID
	uint32_t get_id() { return m_id; }

	///获取跑商ID
	uint32_t get_trade_id() { return m_trade_id; }

	///获取物品贸易类型
	uint32_t get_type() { return m_type; }

	///获取物品基础数量
	uint32_t get_base_num() { return m_base_num; }

	///获取物品基础出售数量
	uint32_t get_base_sell_num() { return m_base_num; }

	///获取物品基础购买数量
	uint32_t get_base_buy_num() { return m_base_num; }

	///获取物品能购买数量
	uint32_t get_can_buy_num() { return m_can_buy_num; }

	///获取基础购买价格
	uint32_t get_base_buy_price() { return m_base_buy_price; }

	///获取基础出售价格
	uint32_t get_base_sell_price() { return m_base_sell_price; }

	///获取购买价格
	uint32_t get_buy_price() { return m_buy_price; }

	///获取出售价格
	uint32_t get_sell_price() { return m_sell_price; }

	///是否能购买
	bool	 is_can_buy() { return m_is_can_buy; }

	///设置是否能购买
	void	 set_can_buy() { m_is_can_buy = true; }

	///获取条件参数
	uint32_t get_cond_param() { return m_cond_param; }

public:
	///获取上次行情
	int32_t	get_last_market() { return m_last_market; }

	///获取行情
	int32_t get_market() { return m_market; }

	///设置行情
	void	set_market(uint32_t market);

	///获取上次行情该物品购买总数
	uint32_t get_last_market_buy_total() { return m_item_last_buy_total; }

	///获取上次行情物品出售总数
	uint32_t get_last_market_sell_total() { return m_item_last_sell_total; }

	///获取本次行情该物品购买总数
	uint32_t get_market_buy_total() { return m_item_buy_total; }

	///获取本次行情物品出售总数
	uint32_t get_market_sell_total() { return m_item_sell_total; }

	///重新计算行情
	void	calc_market();

	///增加物品购买数量
	void	add_item_buy_total(uint32_t buy_num);

	///增加物品出售数量
	void	add_item_sell_total(uint32_t sell_num);

public:
	///重置基础数据(配置表读取)(贸易类型 基础数量 条件参数 是否需要繁荣度)
	void	 reset_base_value(uint32_t type,uint32_t base_num,uint32_t cond_param,bool is_need_prosperity);

	///检测是否可以购买(开启条件)
	bool	 check_can_buy(uint32_t prosperity);
		
	///重新计算物品价格(参数 繁荣度等级) 
	void	 recalc_item_price(uint32_t prosperity_level, uint32_t event_market = 10000);
	
	///重新计算物品可购买数量(参数 占有度)
	void	 recalc_buy_num(uint32_t hold_value);

private:
	int32_t	 m_last_market = 0;					///上次的贸易行情
	uint32_t m_market = 10000;					///当前贸易行情(万分比)
	uint32_t m_item_last_buy_total = 0;			///上一次行情的物品购买总数
	uint32_t m_item_last_sell_total = 0;		///上一次行情的物品出售总数
	uint32_t m_item_buy_total = 0;				///本次行情的物品购买总数
	uint32_t m_item_sell_total = 0;				///本次行情的物品出售总数
	uint32_t m_market_stay_prob = 0;			///行情不变概率
	uint32_t m_market_type = 0;					///当前贸易行情类型
private:
	uint32_t m_id = 0;							///物品ID
	uint32_t m_trade_id = 0;					///跑商ID
	uint32_t m_type = 0;						///物品贸易类型
	uint32_t m_base_num = 0;					///物品基础数量
	uint32_t m_can_buy_num = 0;					///物品可购买数量
	uint32_t m_base_sell_price = 0;				///物品基础出售价格
	uint32_t m_base_buy_price = 0;				///物品基础购买价格
	uint32_t m_buy_price = 0;					///物品购买价格
	uint32_t m_sell_price = 0;					///物品出售价格
	uint32_t m_cond_param = 0;					///物品条件参数
	bool	 m_is_can_buy = false;				///物品是否可以购买(与可出售物品做区分)
	bool	 m_is_need_prosperity = false;		///是否需要城市繁荣度
};

typedef boost::shared_ptr<trade_item_t> trade_item_ptr;


class trade_item_type_t
{
public:
	trade_item_type_t(uint32_t trade_id,uint32_t type);
	~trade_item_type_t(){}

private:
	uint32_t m_trade_id = 0;			///跑商ID
	uint32_t m_type = 0;				///物品类型
};

typedef boost::shared_ptr<trade_item_type_t> trade_item_type_ptr;

