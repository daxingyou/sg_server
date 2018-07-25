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
	 * \brief �ط�����
	 */
	static void close();

	/**
	 * \brief ���촦��
	 */
	static void one_day();

public:
	static country_ptr get_country(uint32_t id);
	static void get_country_common(uint32_t id, proto::common::country_common* p_base);

public:
	/**
	 * \brief ����ʱ��
	 */
	static uint32_t get_country_period() { return m_global_country_data.period_type(); }

	/**
	 * \brief ��������ʱ
	 */
	static uint32_t get_create_countdown() { return m_global_country_data.period_time(); }

	/**
	 * \brief ����������ʱ��
	 */
	static void start_create_countdown(uint32_t left_time);
	static void countdown_callback(const boost::system::error_code& ec);

	/**
	 * \brief ���´��Ƶ���ʱ
	 */
	static void open_establish_callback(uint32_t id, bool is_in_time);

	/**
	 * \brief ��������ϵͳ����ʱ
	 */
	static void open_country_system(const std::string& family_name, bool is_notify = true);

public:
	/**
	 * \brief ����ս����
	 */
	static void start_king_war_sign_up(uint32_t start_time, uint32_t state_end_time);

	/**
	 * \brief ����ս��������
	 */
	static void end_king_war_sign_up();

	/**
	 * \brief  ����ս��ս��ʼ
	 */
	static void start_king_war_prepare(uint32_t state_end_time);

	 /**
	 * \brief  ����ս��ս����
	 */
	static void end_king_war_prepare();

	/**
	* \brief  ����սս����ʼ
	*/
	static void start_king_war_fight(uint32_t state_end_time);

	/**
	* \brief  ����սս������
	*/
	static void end_king_war_fight();

	/**
	 * \brief ���������Ƿ��ظ�
	 */
	static bool is_name_repeate(const std::string& name, uint32_t except);

public:
	// ����ͬ����game
	static void sync_country_data(const network::tcp_socket_ptr& s);
	// ����Ԥ��
	static void get_country_preview(proto::client::ec_country_preview_reply& reply);

	// ������Ϣ
	static void get_country_info(uint32_t country_id, proto::client::ec_country_info_reply& reply);

	// ����ͬ������
	static void ask_country_alliance(const global_user_ptr& p_user, uint32_t country_id, proto::client::ec_country_alliance_reply& reply);

	// �������ͬ��
	static uint32_t quit_country_alliance(const global_user_ptr& p_user);

	// �޸����
	static uint32_t modify_year_name(const global_user_ptr& p_user, const std::string& name);

	// game��Ǯ����
	static void modify_country_year_name_back(const global_user_ptr& p_user, const proto::server::ge_country_year_name_reply& reply);

	// �޸Ĺ���
	static uint32_t modify_country_notice(const global_user_ptr& p_user, const std::string& notice);

	// ��ù��������б�
	static void open_appoint_request(const global_user_ptr& p_user, uint32_t officer_id, proto::client::ec_open_appoint_reply& reply);

	// ������Ա
	static void appoint_officer_request(const global_user_ptr& p_user, uint64_t target_uid, uint32_t officer_id, proto::client::ec_appoint_officer_reply& reply);

	// �����Ա
	static void fire_officer_request(const global_user_ptr& p_user, uint32_t officer_id, proto::client::ec_fire_officer_reply& reply);

	// �ǹ�
	static void resign_officer_request(const global_user_ptr& p_user, proto::client::ec_resign_officer_reply& reply);

	// ��ù��������б�
	static void get_country_flag_list(const global_user_ptr& p_user, proto::client::ec_country_flag_list_reply& reply);

	// �޸Ĺ��ҵ����ĺ�����
	static uint32_t modify_country_name_flag(const global_user_ptr& p_user, uint32_t flag, const std::string& name);
	
	// game��Ǯ����
	static void modify_country_name_flag_back(const global_user_ptr& p_user, const proto::server::ge_country_name_change_reply& reply);

protected:
	static country_map m_country_map;
	static proto::server::global_data_country_data m_global_country_data;
	static boost::shared_ptr<boost::asio::deadline_timer> m_open_timer;
};
