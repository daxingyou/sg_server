#pragma once

#include <boost/asio/deadline_timer.hpp>
#include <boost/system/error_code.hpp>

#include "tcp_socket.hpp"
#include "country.hpp"
#include "user/global_user.hpp"
#include "client.pb.h"
#include "server.pb.h"

class country_mgr_t
{
public:
	typedef std::map<uint32_t, country_ptr> country_map;

public:
	/**
	 * \brief init
	 */
	static bool init();
	/**
	 * \brief 关服清理
	 */
	static void close();

	/**
	 * \brief 跨天处理
	 */
	static void one_day();

public:
	static country_ptr get_country(uint32_t id);
	static void get_country_common(uint32_t id, proto::common::country_common* p_base);

public:
	/**
	 * \brief 国家时期
	 */
	static uint32_t get_country_period() { return m_global_country_data.period_type(); }

	/**
	 * \brief 建国倒计时
	 */
	static uint32_t get_create_countdown() { return m_global_country_data.period_time(); }

	/**
	 * \brief 启动建国定时器
	 */
	static void start_create_countdown(uint32_t left_time);
	static void countdown_callback(const boost::system::error_code& ec);

	/**
	 * \brief 天下大势倒计时
	 */
	static void open_establish_callback(uint32_t id, bool is_in_time);

	/**
	 * \brief 开启国家系统倒计时
	 */
	static void open_country_system(const std::string& family_name, bool is_notify = true);

public:
	/**
	 * \brief 王城战报名
	 */
	static void start_king_war_sign_up(uint32_t start_time, uint32_t state_end_time);

	/**
	 * \brief 王城战报名结束
	 */
	static void end_king_war_sign_up();

	/**
	 * \brief  王城战备战开始
	 */
	static void start_king_war_prepare(uint32_t state_end_time);

	 /**
	 * \brief  王城战备战结束
	 */
	static void end_king_war_prepare();

	/**
	* \brief  王城战战斗开始
	*/
	static void start_king_war_fight(uint32_t state_end_time);

	/**
	* \brief  王城战战斗结束
	*/
	static void end_king_war_fight();

	/**
	 * \brief 国家名称是否重复
	 */
	static bool is_name_repeate(const std::string& name, uint32_t except);

public:
	// 国家同步到game
	static void sync_country_data(const network::tcp_socket_ptr& s);
	// 国家预览
	static void get_country_preview(proto::client::ec_country_preview_reply& reply);

	// 国家信息
	static void get_country_info(uint32_t country_id, proto::client::ec_country_info_reply& reply);

	// 国家同盟请求
	static void ask_country_alliance(const global_user_ptr& p_user, uint32_t country_id, proto::client::ec_country_alliance_reply& reply);

	// 解除国家同盟
	static uint32_t quit_country_alliance(const global_user_ptr& p_user);

	// 修改年号
	static uint32_t modify_year_name(const global_user_ptr& p_user, const std::string& name);

	// game扣钱返回
	static void modify_country_year_name_back(const global_user_ptr& p_user, const proto::server::ge_country_year_name_reply& reply);

	// 修改公告
	static uint32_t modify_country_notice(const global_user_ptr& p_user, const std::string& notice);

	// 获得国家任命列表
	static void open_appoint_request(const global_user_ptr& p_user, uint32_t officer_id, proto::client::ec_open_appoint_reply& reply);

	// 任命官员
	static void appoint_officer_request(const global_user_ptr& p_user, uint64_t target_uid, uint32_t officer_id, proto::client::ec_appoint_officer_reply& reply);

	// 罢免官员
	static void fire_officer_request(const global_user_ptr& p_user, uint32_t officer_id, proto::client::ec_fire_officer_reply& reply);

	// 辞官
	static void resign_officer_request(const global_user_ptr& p_user, proto::client::ec_resign_officer_reply& reply);

	// 获得国家旗帜列表
	static void get_country_flag_list(const global_user_ptr& p_user, proto::client::ec_country_flag_list_reply& reply);

	// 修改国家的旗帜和名字
	static uint32_t modify_country_name_flag(const global_user_ptr& p_user, uint32_t flag, const std::string& name);
	
	// game扣钱返回
	static void modify_country_name_flag_back(const global_user_ptr& p_user, const proto::server::ge_country_name_change_reply& reply);

protected:
	static country_map m_country_map;
	static proto::server::global_data_country_data m_global_country_data;
	static boost::shared_ptr<boost::asio::deadline_timer> m_open_timer;
};
