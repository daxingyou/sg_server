#ifndef _BUSSINESS_ACTIVITY_MANAGER_HPP_
#define _BUSSINESS_ACTIVITY_MANAGER_HPP_

#include "msg_handler.hpp"
#include "protos_fwd.hpp"
#include "cmd_desc.hpp"
#include "shake_money_tree.hpp"
#include "sevenday_login.hpp"


class role_t;
typedef boost::shared_ptr<role_t> role_ptr;

class business_activity_manager_t
{
public:
	business_activity_manager_t(uint64_t role_id);

	bool load_data( const proto::common::bussiness_act_data &data );
public:
	//Ò¡Ç®Ê÷
	static uint32_t get_money_tree_info(role_ptr p_role, proto::client::gc_get_money_tree_reply &msg);
	static uint32_t shake_money_tree(role_ptr p_role, proto::client::gc_shake_money_tree_reply &msg);
	static uint32_t level_up_money_tree(role_ptr p_role, proto::client::gc_levelup_money_tree_reply &msg);
	
	//ÆßÈÕµÇÂ¼
	static uint32_t get_sevenday_login_info(role_ptr p_role, proto::client::gc_get_7d_login_reply &msg);
	static uint32_t get_sevenday_login_prize(role_ptr p_role, uint32_t index, proto::client::gc_get_7d_login_prize_reply &msg);

	//µÇÂ¼¼ì²â
	void on_user_login();
public:
	void  oneday();

	void init_newbie();
public:
	shake_money_tree_t m_money_tree;

	sevenday_login_t  m_sevenday_login;
private:
	uint64_t  m_role_uid;
};

#endif
