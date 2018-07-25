#include "business_activity_manager.hpp"
#include "role/role.hpp"
#include "tblh/errcode_enum.hpp"
#include "role/role_manager.hpp"


business_activity_manager_t::business_activity_manager_t(uint64_t role_id)
	:m_role_uid(role_id)
	,m_money_tree(role_id)
	,m_sevenday_login(role_id)
{

}

bool business_activity_manager_t::load_data( const proto::common::bussiness_act_data &data)
{
	m_money_tree.load_data( data.money_tree_info() );
	m_sevenday_login.load_data(data.sevenday_data());
	return true;
}


uint32_t business_activity_manager_t::get_money_tree_info(role_ptr p_role, proto::client::gc_get_money_tree_reply &msg)
{
	if (NULL == p_role)
	{
		log_error("NULL == p_role");
		return errcode_enum::notice_unknown;
	}

	auto &business_mgr = p_role->get_business_act_mgr();
	return business_mgr.m_money_tree.get_money_tree_info(msg);
}

uint32_t business_activity_manager_t::shake_money_tree(role_ptr p_role, proto::client::gc_shake_money_tree_reply &msg)
{
	if (NULL == p_role)
	{
		log_error("NULL == p_role");
		return errcode_enum::notice_unknown;
	}

	auto &business_mgr = p_role->get_business_act_mgr();
	return business_mgr.m_money_tree.shake_money_tree(msg);
}

uint32_t business_activity_manager_t::level_up_money_tree(role_ptr p_role, proto::client::gc_levelup_money_tree_reply &msg)
{
	if (NULL == p_role)
	{
		log_error("NULL == p_role");
		return errcode_enum::notice_unknown;
	}

	auto &business_mgr = p_role->get_business_act_mgr();
	return business_mgr.m_money_tree.level_up_money_tree(msg);
}


//ÆßÈÕµÇÂ¼
uint32_t business_activity_manager_t::get_sevenday_login_info(role_ptr p_role, proto::client::gc_get_7d_login_reply &msg)
{
	if (NULL == p_role)
	{
		log_error("NULL == p_role");
		return errcode_enum::notice_unknown;
	}

	auto &business_mgr = p_role->get_business_act_mgr();
	return business_mgr.m_sevenday_login.get_sevenday_list_info(msg);
}

uint32_t business_activity_manager_t::get_sevenday_login_prize(role_ptr p_role, uint32_t index, proto::client::gc_get_7d_login_prize_reply &msg)
{
	if (NULL == p_role)
	{
		log_error("NULL == p_role");
		return errcode_enum::notice_unknown;
	}

	auto &business_mgr = p_role->get_business_act_mgr();
	return business_mgr.m_sevenday_login.get_sevenday_list_prize(index, msg);
}

void business_activity_manager_t::oneday()
{
	m_money_tree.oneday();
	m_sevenday_login.oneday();
}

void business_activity_manager_t::init_newbie()
{
	m_money_tree.init_newbie();
	m_sevenday_login.init_newbie();
}

void business_activity_manager_t::on_user_login()
{
	role_ptr p_role = role_manager_t::find_role( m_role_uid );
	if (NULL == p_role)
	{
		log_error("p_role[%lu] is NULL", m_role_uid);
		return;
	}

	auto &business_mgr = p_role->get_business_act_mgr();
	business_mgr.m_sevenday_login.on_login();
}


