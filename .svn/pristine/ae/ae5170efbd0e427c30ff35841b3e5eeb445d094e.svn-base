//运营活动-黄金十连系统

#ifndef _GOLD_LUCKYDRAW_
#define _GOLD_LUCKYDRAW_

#include "macros.hpp"
#include "protos_fwd.hpp"
#include "base_business_active.h"
#include <set>
#include <string>


class role_t;
typedef boost::shared_ptr<role_t> role_ptr;

class gold_luckydraw_t : public base_business_active_t
{
public:
	bool save_data();
	bool peek_data(  proto::common::gold_luckydraw_data &data );
	bool load_data( const proto::common::gold_luckydraw_data &data);
	void peek_client_data(proto::common::gold_luckydraw_client_data &data);
public:	
	gold_luckydraw_t(uint64_t role_id);

	uint32_t on_login();
	
	uint32_t award(uint32_t id);

	//凌晨跨日处理
	void one_day();

	void clear_lucky();

	uint32_t get_expired_time();

	bool is_in_time();
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
	virtual void set_active_start( uint32_t start_time );

	//设置活动结束
	virtual void set_active_stop();

	//获取红点
	virtual uint32_t get_red_num();

private:
	std::string m_key;

	uint64_t m_role_uid;

	uint32_t m_start_time;

	std::vector<uint32_t> m_award_vec;		//已经领取的奖励
};


#endif
