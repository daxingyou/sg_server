//运营活动-道具兑换系统

#ifndef _SEVENDAY_TARGET_HPP_
#define _SEVENDAY_TARGET_HPP_

#include "macros.hpp"
#include "protos_fwd.hpp"
#include "base_business_active.h"
#include <set>
#include <string>
#include "tblh/SDTargetCountTable.tbls.h"

class role_t;
typedef boost::shared_ptr<role_t> role_ptr;

class sevenday_target_t : public base_business_active_t
{
public:
	bool save_data();
	bool peek_data(proto::common::sd_target_save_data &data);
	bool load_data(const proto::common::sd_target_save_data &data);
public:
	sevenday_target_t(uint64_t role_id);

	uint32_t get_sevenday_target_info( uint32_t id, proto::client::gc_get_sd_target_info_reply &reply);

	uint32_t get_sevenday_target_prize(uint32_t id, proto::common::role_change_data *rcd);

	uint32_t get_sevenday_target_red_num(proto::client::gc_get_sd_target_red_reply &reply );

	uint32_t on_login();

	//获取当前进度
	uint32_t get_cur_num(uint32_t id);

	uint32_t get_prize_status(uint32_t id, SDTargetCountTable *p_conf , uint32_t count);

	void one_day();

	void check_red_num();
public:
	//是否在活动中
	virtual bool is_active();

	//是否领取了所有奖励
	virtual bool is_get_all_prize();

	//是否是结束时间了
	virtual bool is_end_show_time();

	//设置活动开始
	virtual void set_active_start(uint32_t start_time) {}

	//设置活动结束
	virtual void set_active_stop(){}

	//获取红点
	virtual uint32_t get_red_num();
private:
	std::string m_key;

	uint64_t m_role_uid;

	uint32_t m_day;

	uint32_t m_last_red_num = 0;

	std::set<uint32_t> m_prize_map;		//奖励信息
};


#endif
