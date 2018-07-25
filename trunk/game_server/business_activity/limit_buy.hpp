//7日限购
#ifndef _LIMIT_BUY_HPP_
#define _LIMIT_BUY_HPP_

#include "macros.hpp"
#include "protos_fwd.hpp"
#include "base_business_active.h"
#include <set>
#include <string>

class role_t;
typedef boost::shared_ptr<role_t> role_ptr;

class limit_buy_t: public base_business_active_t
{
public:
	bool load_data(const proto::common::limit_buy_save_data &data);
	bool peek_data(proto::common::limit_buy_save_data &data);
	bool save_data();
public:
	limit_buy_t(uint64_t role_id);

	uint32_t get_limit_buy_info(proto::client::gc_get_limit_buy_info_reply &reply);

	uint32_t get_prize_status(uint32_t index);

	uint32_t buy_limit( uint32_t index, proto::common::role_change_data *rcd);

	uint32_t get_buy_num(uint32_t index);

	void init_newbie() { m_day = 1; save_data(); }

	void one_day();
public:
	//是否在活动中
	virtual bool is_active();

	//是否领取了所有奖励
	virtual bool is_get_all_prize();

	//是否是结束时间了
	virtual bool is_end_show_time();

	//获取活动开始时间, 如果是0表示未参加过，也不需要显示
	virtual uint32_t get_active_start_time(uint32_t start_time);

	//获取活动结束时间,放在模块做，方便扩展
	virtual uint32_t get_active_end_time();

	//设置活动开始
	virtual void set_active_start();

	//设置活动结束
	virtual void set_active_stop();

	//获取红点
	virtual uint32_t get_red_num();
private:
	std::string m_key = "";

	uint64_t m_role_id = 0;

	uint32_t m_day = 1;						//登录天数

	uint32_t m_red = 0;

	std::map<uint32_t, uint32_t> m_prize_list;	//奖励列表
};
#endif 