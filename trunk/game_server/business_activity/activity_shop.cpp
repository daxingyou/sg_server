#include "activity_shop.hpp"
#include "log.hpp"
#include "log/log_wrapper.hpp"
#include "role/role.hpp"
#include "config/config_manager.hpp"
#include "tblh/WsTreasuryTable.tbls.h"
#include "time_manager.hpp"
#include "role/role_manager.hpp"
#include "tblh/errcode_enum.hpp"
#include "tblh/WsScoreTreasuryTable.tbls.h"

activity_shop::activity_shop(uint64_t uid)
	: m_role_uid(uid)
{
}

activity_shop::~activity_shop()
{

}

void activity_shop::load_data(const proto::common::personal_info & info)
{
	m_wusheng_score = info.wusheng_score();
	m_wusheng_score_today = info.wusheng_score_today();
	m_wusheng_lucky = info.wusheng_lucky();
	m_wusheng_today_buy = info.wusheng_today_buy();
}

void activity_shop::peek_data(proto::common::personal_info *p_info)
{
	if (NULL == p_info)
	{
		log_error("activity_shop peek_data p_info NULL");
		return;
	}
	p_info->set_wusheng_score(m_wusheng_score);
	p_info->set_wusheng_score_today(m_wusheng_score_today);
	p_info->set_wusheng_lucky(m_wusheng_lucky);
	p_info->set_wusheng_today_buy(m_wusheng_today_buy);
}

void activity_shop::one_day()
{
	m_wusheng_score_today = 0;
	m_wusheng_today_buy = 0;
}

void activity_shop::add_wusheng_score(uint32_t score)
{
	m_wusheng_score += score;
	m_wusheng_score_today += score;
}

void activity_shop::dec_wusheng_score(uint32_t score)
{
	if (m_wusheng_score < score)
		m_wusheng_score = 0;
	else
		m_wusheng_score -= score;
}

void activity_shop::add_wusheng_today_buy(uint32_t num)
{
	m_wusheng_today_buy += num;
}

void activity_shop::set_lucky(uint32_t lucky)
{
	m_wusheng_lucky = lucky;
}

// void role_activity_shop_info_t::add_wusheng_lucky(uint32_t lucky)
// {
// 	m_wusheng_lucky += lucky;
// }
// 
// void role_activity_shop_info_t::dec_wusheng_lucky(uint32_t lucky)
// {
// 	if (lucky > m_wusheng_lucky)
// 		m_wusheng_lucky = 0;
// 	else
// 		m_wusheng_lucky -= lucky;
// }

//是否在活动中
bool activity_shop::is_active()
{
	BusinessActiveTable *p_conf = GET_CONF(BusinessActiveTable, proto::common::BUSINESS_ACTIVE_TYPE::WUSHENG_BOX);
	if (NULL == p_conf)
	{
		log_error("role[%lu] activity_shop is_active get BusinessActiveTable WUSHENG_BOX[%d] conf NULL", m_role_uid, proto::common::BUSINESS_ACTIVE_TYPE::WUSHENG_BOX);
		return false;
	}

	//时间表没有id为0的时间，如果配置id为0的时间表示全天active
	if (p_conf->time_id() == 0)
	{
		return true;
	}

	return time_manager_t::is_in_time(p_conf->time_id());
}

//是否领取了所有奖励
bool activity_shop::is_get_all_prize()
{
	return true;
}

//是否是结束时间了
bool activity_shop::is_end_show_time()
{
	role_ptr p_role = role_manager_t::find_role(m_role_uid);
	if (NULL == p_role)
	{
		log_error("role[%lu] p_role is NULL", m_role_uid);
		return false;
	}

	BusinessActiveTable *p_conf = GET_CONF(BusinessActiveTable, proto::common::BUSINESS_ACTIVE_TYPE::WUSHENG_BOX);
	if (NULL == p_conf)
	{
		log_error("role[%lu] activity_shop is_end_show_time get BusinessActiveTable WUSHENG_BOX[%d] conf NULL", m_role_uid, proto::common::BUSINESS_ACTIVE_TYPE::WUSHENG_BOX);
		return false;
	}

	return business_activity_manager_t::check_end_cond(p_role, p_conf, this) == false;
}

//获取活动开始时间, 如果是0表示未参加过，也不需要显示
uint32_t activity_shop::get_active_start_time()
{
	BusinessActiveTable *p_conf = GET_CONF(BusinessActiveTable, proto::common::BUSINESS_ACTIVE_TYPE::WUSHENG_BOX);
	if (NULL == p_conf)
	{
		log_error("role[%lu] activity_shop get_active_start_time get BusinessActiveTable WUSHENG_BOX[%d] conf NULL", m_role_uid, proto::common::BUSINESS_ACTIVE_TYPE::WUSHENG_BOX);
		return false;
	}
	return time_manager_t::get_timer_start_time(p_conf->time_id());
}

//获取活动结束时间,放在模块做，方便扩展
uint32_t activity_shop::get_active_end_time()
{
	BusinessActiveTable *p_conf = GET_CONF(BusinessActiveTable, proto::common::BUSINESS_ACTIVE_TYPE::WUSHENG_BOX);
	if (NULL == p_conf)
	{
		log_error("role[%lu] activity_shop get_active_end_time get BusinessActiveTable WUSHENG_BOX[%d] conf NULL", m_role_uid, proto::common::BUSINESS_ACTIVE_TYPE::WUSHENG_BOX);
		return false;
	}

	return time_manager_t::get_timer_end_time(p_conf->time_id());
}

//设置活动开始
void activity_shop::set_active_start(uint32_t start_time)
{
	role_ptr p_role = role_manager_t::find_role(m_role_uid);
	if (NULL == p_role)
	{
		log_error("role[%lu] p_role is NULL", m_role_uid);
		return;
	}

	ACTIVITY_LOG("notify role[%lu] activity_shop open.", m_role_uid);
	business_activity_manager_t::add_icon_info(p_role, proto::common::BUSINESS_ACTIVE_TYPE::WUSHENG_BOX, get_red_num(), start_time, 0);
}

//设置活动结束
void activity_shop::set_active_stop()
{
	role_ptr p_role = role_manager_t::find_role(m_role_uid);
	if (NULL == p_role)
	{
		log_error("role[%lu] p_role is NULL", m_role_uid);
		return;
	}

	if (is_end_show_time())
	{
		ACTIVITY_LOG("notify role[%lu] activity_shop close.", m_role_uid);
		business_activity_manager_t::del_icon_info(p_role, proto::common::BUSINESS_ACTIVE_TYPE::WUSHENG_BOX);
	}
}

//获取红点
uint32_t activity_shop::get_red_num()
{
	role_ptr p_role = role_manager_t::find_role(m_role_uid);
	if (NULL == p_role)
	{
		log_error("[%lu] p_role is NULL", m_role_uid);
		return 0;
	}

	//获取最小宝箱消耗的积分值
	uint32_t shop_item_id = 1;
	WsScoreTreasuryTable *p_conf = GET_CONF(WsScoreTreasuryTable, shop_item_id);
	if (NULL == p_conf)
	{
		log_error("role[%lu] get WsScoreTreasuryTable p_conf NULL, shop_item_id[%d]", m_role_uid, shop_item_id);
		return 0;
	}
	uint32_t score_cost = p_conf->cost_score();

	uint32_t count = 0;
	if (m_wusheng_score >= score_cost)//大于等于最小宝箱消耗即可以购买
	{
		count = 1;
	}
	return count;
}