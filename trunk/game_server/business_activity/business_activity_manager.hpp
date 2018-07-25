#ifndef _BUSSINESS_ACTIVITY_MANAGER_HPP_
#define _BUSSINESS_ACTIVITY_MANAGER_HPP_

#include "msg_handler.hpp"
#include "protos_fwd.hpp"
#include "cmd_desc.hpp"
#include "shake_money_tree.hpp"
#include "sevenday_login.hpp"
#include "month_card.hpp"
#include "level_fund.hpp"
#include "tblh/BusinessActiveTable.tbls.h"
#include "exchange_item.hpp"
#include "limit_buy.hpp"
#include "sevenday_target.hpp"
#include "recharge_total.hpp"
#include "activity_shop.hpp"
#include "gold_luckydraw.hpp"
#include "time_rank.hpp"

class role_t;
typedef boost::shared_ptr<role_t> role_ptr;

class business_activity_manager_t
{
public:
	business_activity_manager_t(uint64_t role_id);

	bool load_data( const proto::common::bussiness_act_data &data );

	//上线运营活动时间管理
	void check_all_yyhd();
	//时间回调
	void on_timer(uint32_t id, bool is_active);

	static bool init();
	//时间管理
	static void on_timer_callback( uint32_t id , bool is_in_time );

	//检测开始条件
	static bool check_open_cond( role_ptr p_role, BusinessActiveTable *p_conf, base_business_active_t *p_active );
	//检测结束条件
	static bool check_end_cond( role_ptr p_role, BusinessActiveTable *p_conf , base_business_active_t *p_active );

	static void exec_all_user_start(role_ptr p_role, uint32_t id);
	static void exec_all_user_stop(role_ptr p_role, uint32_t id);

	void set_active_start(uint32_t id, uint32_t start_time );

	void set_active_stop(uint32_t id);

	//通知活动信息
	void init_active_info();

	//通知添加单个活动信息
	static void add_icon_info( role_ptr p_role, uint32_t uid , uint32_t red_num, uint32_t start_time, uint32_t end_time  );
	//删除
	static void del_icon_info( role_ptr p_role, uint32_t uid );
	
	void on_level_up();

	static void check_seven_day_target( role_ptr p_role);
public:
	//摇钱树
	static uint32_t get_money_tree_info(role_ptr p_role, proto::client::gc_get_money_tree_reply &msg);
	static uint32_t shake_money_tree(role_ptr p_role, proto::client::gc_shake_money_tree_reply &msg);
	static uint32_t level_up_money_tree(role_ptr p_role, proto::client::gc_levelup_money_tree_reply &msg);
	
	//七日登录
	static uint32_t get_sevenday_login_info(role_ptr p_role, proto::client::gc_get_7d_login_reply &msg);
	static uint32_t get_sevenday_login_prize(role_ptr p_role, uint32_t index, proto::client::gc_get_7d_login_prize_reply &msg);

	//月卡
	static uint32_t get_month_card_info(role_ptr &p_role, proto::client::gc_get_month_card_info_reply &reply);
	static uint32_t get_month_card_ed_prize(role_ptr &p_role, uint32_t card_id, proto::common::role_change_data *rcd);
	static uint32_t buy_month_card(role_ptr &p_role, uint32_t card_id);
	static uint32_t on_buy_month_card_callback(role_ptr &p_role, uint32_t card_id,proto::common::role_change_data *rcd);

	//基金	
	static uint32_t get_level_fund_info(role_ptr &p_role, proto::client::gc_get_level_fund_info_reply &reply);
	static uint32_t buy_level_fund(role_ptr &p_role,  proto::common::role_change_data *rcd );
	static uint32_t get_level_fund_prize(role_ptr &p_role, uint32_t index, proto::common::role_change_data *rcd);

	//兑换
	static uint32_t get_exchange_item_info(role_ptr &p_role, proto::client::gc_get_exchange_info_reply &reply);
	static uint32_t get_exchange_item_prize(role_ptr &p_role, uint32_t id, proto::common::role_change_data *rcd);

	//限购
	static uint32_t get_limit_buy_info(role_ptr &p_role, proto::client::gc_get_limit_buy_info_reply &reply);
	static uint32_t buy_limit(role_ptr &p_role, uint32_t id, proto::common::role_change_data *rcd);

	//七日目标
	static uint32_t get_sevenday_target_info(role_ptr &p_role, uint32_t day, proto::client::gc_get_sd_target_info_reply &reply);
	static uint32_t get_sevenday_target_prize(role_ptr &p_role, uint32_t id, proto::common::role_change_data *rcd);
	static uint32_t get_sevenday_red_num( role_ptr &p_role, proto::client::gc_get_sd_target_red_reply &reply);
	
	//累充奖励
	static void peek_recharge_total_client_data(role_ptr &p_role, proto::common::recharge_total_client_data* p_data);

	static uint32_t recharge_total_award(role_ptr &p_role, uint32_t id, proto::common::role_change_data *p_data);
	//黄金十连
	static void peek_gold_luckydraw_client_data(role_ptr &p_role, proto::common::gold_luckydraw_client_data* p_data);

	static uint32_t gold_luckydraw_award(role_ptr &p_role, uint32_t id);

	static uint32_t get_gold_luckydraw_expired_time(role_ptr &p_role);
	//登录检测
	void on_user_login();

	//武圣宝库
	activity_shop& get_activity_shop();
	static void get_activity_shop_data(role_ptr p_role, uint32_t shop_type, proto::client::gc_activity_shop_get_info_reply& reply);
	static uint32_t buy_activity_shop(role_ptr p_role, uint32_t shop_type, uint32_t item_pos, uint32_t item_num, proto::client::gc_activity_shop_buy_reply &reply);

public:
	void  oneday();

	void init_newbie();

	uint32_t get_month_card_id();

public:
	std::map<uint32_t, base_business_active_t*>  m_active_list;

public:
	shake_money_tree_t m_money_tree;

	sevenday_login_t  m_sevenday_login;

	month_card_t  m_month_card;

	level_fund_t m_level_fund;

	exchange_item_t m_exchange_item;

	limit_buy_t m_limit_buy;

	sevenday_target_t m_sevenday_target;

	recharge_total_t m_recharge_total;

	gold_luckydraw_t m_gold_luckydraw;

	time_rank_t m_time_rank;
	/*
	*@ logic:  武圣宝库
	*@ author: wys
	*/
public:
	//inline activity_shop& get_activity_shop_data() { return m_role_activity_shop; }

private:
	activity_shop m_activity_shop;

private:
	uint64_t  m_role_uid;

	//武圣宝库
private:
	static uint32_t get_activity_shop_ws_rand(proto::common::activity_shop_item_info *item_info);
	static uint32_t get_activity_shop_ws_score(proto::common::activity_shop_item_info *item_info);
	static uint32_t buy_activity_shop_ws_rand(role_ptr p_role, uint32_t item_pos, uint32_t item_num, proto::client::gc_activity_shop_buy_reply &reply);
	static uint32_t buy_activity_shop_ws_score(role_ptr p_role, uint32_t item_pos, uint32_t item_num, proto::client::gc_activity_shop_buy_reply &reply);
};



#endif
