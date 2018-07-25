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
	static void apply_king_war_request(global_user_ptr p_user, uint32_t country_id, proto::client::ec_king_war_sign_up_reply& reply);

	/**
	* \brief 申请竞价
	*/
	static void bid_king_war_request(global_user_ptr p_user, uint32_t country_id, uint32_t money, proto::client::ec_king_war_offer_money_reply& reply);

	/**
	* \brief 申请列表
	*/
	static void get_king_war_apply_list(global_user_ptr p_user, uint32_t country_id, proto::client::ec_king_war_sign_up_list_reply& reply);

	/**
	 * \brief 撤销报名
	 */
	static void sudo_king_war_apply(const global_user_ptr& p_user, proto::client::ec_king_war_undo_sign_up_reply& reply);

	/**
	 * \brief 编辑战前宣言
	 */
	static void update_king_war_notice(const global_user_ptr& p_user, uint32_t country_id, const std::string& notice, proto::client::ec_king_war_notice_reply& reply);

	/**
	* \brief 点赞
	*/
	static void update_king_war_like(const global_user_ptr& p_user, uint32_t country_id, uint64_t family_id, uint32_t like, proto::client::ec_king_war_like_reply& reply);

	/**
	 * \brief 获得场景人数
	 */
	static void get_king_war_scene_num(uint32_t country_id, proto::client::ec_king_war_scene_num_reply& reply);

	/**
	 * \brief 请求助战家族列表
	 */
	static void get_king_war_help_family_list(const global_user_ptr& p_user, uint32_t page, proto::client::ec_king_war_help_family_list_reply& reply);

	/**
	 * \brief 请求助战家族列表(模糊查询)
	 */
	static void get_king_war_help_family_fuzzy(const global_user_ptr& p_user, const std::string& name, proto::client::ec_king_war_help_family_fuzzy_reply& reply);

	/**
	 * \brief 邀请家族助战
	 */
	static uint32_t invite_king_war_help_family(const global_user_ptr& p_user, uint64_t invite_family);

	/**
	 * \brief 邀请助战处理
	 */
	static uint32_t king_war_process_invited(const global_user_ptr& p_user, uint64_t invited_family_id, uint32_t process);

	/**
	 * \brief 取消邀请助战
	 */
	static void king_war_cancel_invited(const global_user_ptr& p_user, proto::client::ec_cencel_king_war_invite_reply& reply);

	/**
	 * \brief 取消助战邀请
	 */
	static void king_war_cancel_help(const global_user_ptr& p_user, proto::client::ec_cencel_king_war_help_reply& reply);
};

#endif