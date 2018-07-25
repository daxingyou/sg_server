//等级基金
//此处暂不考虑扩展，可能存在2中基金并存。为个模块

#ifndef _LEVEL_FUND_
#define _LEVEL_FUND_

#include "macros.hpp"
#include "protos_fwd.hpp"
#include "base_business_active.h"
#include <set>
#include <string>

class role_t;
typedef boost::shared_ptr<role_t> role_ptr;
 
class level_fund_t : public base_business_active_t
{
public:
	bool load_data(const proto::common::level_fund_save_info &data);
	bool peek_data(proto::common::level_fund_save_info &data);
	bool save_data();
public:
	level_fund_t(uint64_t role_id);

	uint32_t get_level_fund_prize(uint32_t index, proto::common::role_change_data *rcd);

	uint32_t get_level_fund_info(proto::client::gc_get_level_fund_info_reply &reply);

	uint32_t get_prize_status( uint32_t index, uint32_t level );
	
	uint32_t buy_level_fund( proto::common::role_change_data *rcd );
	
public:
	//是否在活动中
	virtual bool is_active();

	//是否领取了所有奖励
	virtual bool is_get_all_prize();

	//是否是结束时间了
	virtual bool is_end_show_time();

	//获取活动开始时间, 如果是0表示未参加过，也不需要显示
	virtual uint32_t get_active_start_time();

	//获取活动结束时间,放在模块做，方便扩展
	virtual uint32_t get_active_end_time();

	//设置活动开始
	virtual void set_active_start(uint32_t start_time);

	//设置活动结束
	virtual void set_active_stop();

	//获取红点
	virtual uint32_t get_red_num();

private:
	std::string m_key = "";

	uint64_t m_role_id = 0;

	uint32_t m_buy_flag = 0;			//购买标记
	
	std::set<uint32_t> m_prize_list;	//奖励列表
};

#endif
