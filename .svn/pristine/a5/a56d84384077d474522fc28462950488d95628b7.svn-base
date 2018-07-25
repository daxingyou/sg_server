#ifndef __CENTER_KING_WAR_MAN_H__
#define __CENTER_KING_WAR_MAN_H__

#include "country/country_common.hpp"
#include "user/global_user.hpp"

#include "client.pb.h"

class king_war_manager_t
{
public:
	// 初始化
	static bool init();

	// 活动时间回调
public:
	// 报名回调
	static void sign_up_callback(uint32_t id, bool is_in_time);
	// 备战回调
	static void prepare_callback(uint32_t id, bool is_in_time);
	// 开战回调
	static void start_callback(uint32_t id, bool is_in_time);

public:
	/**
	* \brief 申请报名
	*/
	static void apply_king_war_request(global_user_ptr p_user, proto::client::ec_king_war_sign_up_reply& reply);

	/**
	* \brief 申请竞价
	*/
	static void bid_king_war_request(global_user_ptr p_user, uint32_t money, proto::client::ec_king_war_offer_money_reply& reply);

	/**
	* \brief 申请列表
	*/
	static void get_king_war_apply_list(global_user_ptr p_user, proto::client::ec_king_war_sign_up_list_reply& reply);
};

#endif