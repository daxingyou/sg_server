#pragma once

#include "macros.hpp"
#include "protos_fwd.hpp"
#include "base_business_active.h"

/**
 *  人物活动商店数据 wys 2018.3.1
 */

class activity_shop : public base_business_active_t
{
public:
	activity_shop(uint64_t uid);
	~activity_shop();

	void load_data(const proto::common::personal_info & info);
	void peek_data(proto::common::personal_info *p_info);

	void		one_day();

	uint32_t	get_wusheng_score() { return m_wusheng_score; }
	uint32_t	get_wusheng_score_today() { return m_wusheng_score_today; }
	uint32_t	get_wusheng_lucky() { return m_wusheng_lucky; }
	uint32_t	get_wusheng_today_buy() { return m_wusheng_today_buy; }

	void		add_wusheng_score(uint32_t score);
	void		dec_wusheng_score(uint32_t score);
	void		add_wusheng_today_buy(uint32_t num);
// 	void		add_wusheng_lucky(uint32_t lucky);
// 	void		dec_wusheng_lucky(uint32_t lucky);
	void		set_lucky(uint32_t lucky);

	//active相关的接口表示是否该功能激活，show相关的接口表示客户端是否显示，客户端显示的功能不一定在激活状态，关闭激活了的功能也可能会在客户端显示
	//是否在活动中，影响操作（如购买等），但不影响显示
	virtual bool is_active();

	//是否领取了所有奖励
	virtual bool is_get_all_prize();

	//是否是结束时间了，表示客户端界面结束显示的时间
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
	uint64_t	m_role_uid = 0;
	uint32_t	m_wusheng_score = 0;			//武圣宝库积分
	uint32_t	m_wusheng_score_today = 0;		//武圣宝库今日积分
	uint32_t	m_wusheng_lucky = 0;			//武圣宝库幸运值
	uint32_t	m_wusheng_today_buy = 0;		//武圣宝库今日购买次数
};