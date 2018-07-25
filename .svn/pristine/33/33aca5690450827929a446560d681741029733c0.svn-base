#ifndef __MONTH_CARD_MANAGER_HPP__
#define __MONTH_CARD_MANAGER_HPP__

#include "macros.hpp"
#include "protocpp/protos_fwd.hpp"
#include <map>
#include <set>

class role_t;
typedef boost::shared_ptr<role_t> role_ptr;

class month_card_t 
{
public:
	month_card_t(uint64_t uid);

	bool load_data(const proto::common::month_card_save_data &data );
	bool peek_data(proto::common::month_card_save_data &data );
	bool save_data();

	void one_day();
public:
	uint32_t get_month_card_info(proto::client::gc_get_month_card_info_reply &reply);
	uint32_t get_month_card_ed_prize(uint32_t card_id, proto::common::role_change_data *rcd);
	uint32_t buy_month_card(uint32_t card_id);
	uint32_t on_buy_month_card_callback( uint32_t card_id, proto::common::role_change_data *rcd );

	uint32_t get_month_card_info( proto::client::gc_buy_month_card_reply &reply);
public:
	uint32_t get_month_card_end_time(uint32_t card_id);

	uint32_t get_month_card_time_left(uint32_t card_id);

	uint32_t get_month_card_ed_prize_status(uint32_t card_id);

	//如果购买多个，只返回最高级的那个
	uint32_t get_month_card_id();

	void on_login();
	
	uint32_t get_red_num();
private:
	uint64_t m_owner;
	std::string m_key;
	std::map<uint32_t, uint32_t> m_month_card_time_map;		//月卡时间管理
	std::set<uint32_t> m_month_card_ed_prize_map;	//月卡每日奖励领取状态
};


#endif