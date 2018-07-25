//运营活动-道具兑换系统

#ifndef _EXCHANGE_ITEM_
#define _EXCHANGE_ITEM_

#include "macros.hpp"
#include "protos_fwd.hpp"
#include "base_business_active.h"
#include <set>
#include <string>


class role_t;
typedef boost::shared_ptr<role_t> role_ptr;

class exchange_item_t : public base_business_active_t
{
public:
	bool save_data();
	bool peek_data(  proto::common::exchange_item_save_data &data );
	bool load_data( const proto::common::exchange_item_save_data &data);
public:	
	exchange_item_t(uint64_t role_id);

	uint32_t do_exchange_item( uint32_t id, proto::common::role_change_data *rcd );

	uint32_t get_exchange_info( proto::client::gc_get_exchange_info_reply &reply);

	uint32_t on_login();
	
	uint32_t get_cur_num(uint32_t id);
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

	std::map<uint32_t, uint32_t> m_exchange_map;		//兑换的次数
};


#endif
