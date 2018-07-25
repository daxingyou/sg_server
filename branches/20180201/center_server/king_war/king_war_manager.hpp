#ifndef __CENTER_KING_WAR_MAN_H__
#define __CENTER_KING_WAR_MAN_H__

#include "country/country_common.hpp"
#include "user/global_user.hpp"

#include "client.pb.h"

class king_war_manager_t
{
public:
	// ��ʼ��
	static bool init();

	// �ʱ��ص�
public:
	// �����ص�
	static void sign_up_callback(uint32_t id, bool is_in_time);
	// ��ս�ص�
	static void prepare_callback(uint32_t id, bool is_in_time);
	// ��ս�ص�
	static void start_callback(uint32_t id, bool is_in_time);

public:
	/**
	* \brief ���뱨��
	*/
	static void apply_king_war_request(global_user_ptr p_user, proto::client::ec_king_war_sign_up_reply& reply);

	/**
	* \brief ���뾺��
	*/
	static void bid_king_war_request(global_user_ptr p_user, uint32_t money, proto::client::ec_king_war_offer_money_reply& reply);

	/**
	* \brief �����б�
	*/
	static void get_king_war_apply_list(global_user_ptr p_user, proto::client::ec_king_war_sign_up_list_reply& reply);
};

#endif