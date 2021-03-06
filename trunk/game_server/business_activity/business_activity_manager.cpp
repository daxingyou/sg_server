#include "business_activity_manager.hpp"
#include "role/role.hpp"
#include "tblh/errcode_enum.hpp"
#include "role/role_manager.hpp"
#include "config/config_manager.hpp"
#include "common/time_manager.hpp"
#include "tblh/TimeTable.tbls.h"
#include "log.hpp"
#include "role/role_unify_save.hpp"
#include "tblh/Comprehensive.tbls.h"
#include "tblh/WsTreasuryTable.tbls.h"
#include "tblh/WsScoreTreasuryTable.tbls.h"
#include "tblh/GoldLuckydraw.tbls.h"
#include "tblh/TimeRank.tbls.h"
#include "item/item_manager.hpp"
#include "role/money_manager.hpp"
#include "log/log_wrapper.hpp"
#include "item/drop_manager.hpp"
#include "sys_notice/sys_notice_manager.hpp"

business_activity_manager_t::business_activity_manager_t(uint64_t role_id)
	:m_role_uid(role_id)
	, m_money_tree(role_id)
	, m_sevenday_login(role_id)
	, m_month_card(role_id)
	, m_level_fund(role_id)
	, m_exchange_item(role_id)
	, m_limit_buy(role_id)
	, m_sevenday_target(role_id)
	, m_recharge_total(role_id)
	, m_activity_shop(role_id)
	, m_gold_luckydraw(role_id)
	, m_time_rank(role_id)
{
	m_active_list[proto::common::SEVEN_DAY_LOGIN] = &m_sevenday_login;
	m_active_list[proto::common::LEVEL_FUND] = &m_level_fund;
	m_active_list[proto::common::EXCHANGE_ITEM] = &m_exchange_item;
	m_active_list[proto::common::ED_BUY_LIMIT] = &m_limit_buy;
	m_active_list[proto::common::LOAD_OF_NMS] = &m_sevenday_target;
	m_active_list[proto::common::RECHARGE_TOTAL] = &m_recharge_total;
	m_active_list[proto::common::WUSHENG_BOX] = &m_activity_shop;
	m_active_list[proto::common::GOLD_LUCKYDRAW] = &m_gold_luckydraw;
	m_active_list[proto::common::TIME_RANK] = &m_time_rank;
}

bool business_activity_manager_t::load_data( const proto::common::bussiness_act_data &data)
{
	m_money_tree.load_data( data.money_tree_info() );
	m_sevenday_login.load_data( data.sevenday_data() );
	m_month_card.load_data( data.month_card_data() );
	m_level_fund.load_data( data.level_fund_data() );
	m_limit_buy.load_data( data.limit_buy_data() );
	m_exchange_item.load_data( data.exchange_data() );
	m_sevenday_target.load_data(data.sevenday_target_data() );
	m_recharge_total.load_data(data.recharge_total());
	//m_activity_shop的load_data在load_role_data里的set_personal_info里
	m_gold_luckydraw.load_data(data.gold_luckydraw());
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


//七日登录
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
	m_month_card.one_day();
	m_limit_buy.one_day();
	m_sevenday_target.one_day();
	m_recharge_total.one_day();
	m_activity_shop.one_day();

	//日切也要初始化一次
	init_active_info();
}

void business_activity_manager_t::init_newbie()
{
	m_money_tree.init_newbie();
	m_sevenday_login.init_newbie();
	m_limit_buy.init_newbie();
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
	business_mgr.m_month_card.on_login();
	business_mgr.m_recharge_total.on_login();
	business_mgr.m_gold_luckydraw.on_login();
	business_mgr.m_time_rank.on_login();
	init_active_info();
}


activity_shop& business_activity_manager_t::get_activity_shop()
{
	return m_activity_shop;
}

////////////////月卡////////////////////////
uint32_t business_activity_manager_t::get_month_card_info(role_ptr &p_role, proto::client::gc_get_month_card_info_reply &reply)
{
	if (NULL == p_role)
	{
		log_error("p_role is NULL");
		return errcode_enum::notice_unknown;
	}

	auto &business_mgr = p_role->get_business_act_mgr();
	return business_mgr.m_month_card.get_month_card_info(reply);
}

uint32_t business_activity_manager_t::get_month_card_ed_prize(role_ptr &p_role, uint32_t card_id, proto::common::role_change_data *rcd)
{
	if (NULL == p_role)
	{
		log_error("p_role is NULL");
		return errcode_enum::notice_unknown;
	}

	auto &business_mgr = p_role->get_business_act_mgr();
	return business_mgr.m_month_card.get_month_card_ed_prize( card_id, rcd );
}

uint32_t business_activity_manager_t::buy_month_card(role_ptr &p_role, uint32_t card_id)
{
	if (NULL == p_role)
	{
		log_error("p_role is NULL");
		return errcode_enum::notice_unknown;
	}

	auto &business_mgr = p_role->get_business_act_mgr();
	return business_mgr.m_month_card.buy_month_card(card_id);
}

uint32_t business_activity_manager_t::on_buy_month_card_callback(role_ptr &p_role, uint32_t card_id, proto::common::role_change_data *rcd)
{
	if (NULL == p_role)
	{
		log_error("p_role is NULL");
		return errcode_enum::notice_unknown;
	}

	auto &business_mgr = p_role->get_business_act_mgr();
	return business_mgr.m_month_card.on_buy_month_card_callback(card_id,rcd);
}

uint32_t business_activity_manager_t::get_month_card_id() {
	role_ptr p_role = role_manager_t::find_role(m_role_uid);
	if (NULL == p_role)
	{
		log_error("p_role[%lu] is NULL", m_role_uid);
		return 0;
	}
	auto &business_mgr = p_role->get_business_act_mgr();
	return business_mgr.m_month_card.get_month_card_id();
}

//上线检测,运营活动时间管理
void business_activity_manager_t::check_all_yyhd()
{
	role_ptr p_role = role_manager_t::find_role(m_role_uid);
	if (!p_role)
	{
		log_error("p_role[%lu] is NULL", m_role_uid);
		return;
	}

	proto::client::gc_init_active_info_reply reply;
	for (auto it : m_active_list) 
	{
		BusinessActiveTable *p_conf = GET_CONF(BusinessActiveTable, it.first);
		if (NULL == p_conf)
			continue;

		//检测结束条件
		base_business_active_t *p_active = it.second;
		if (NULL == p_active)
			continue;

		//检测是否在活动中
		if ( p_active->is_active() == false)
			continue;

		//检测开放条件
		if (check_open_cond( p_role, p_conf , it.second ) == false)
			continue;

		//检测是否结束显示
		if ( check_end_cond( p_role,p_conf, it.second ) == false )
			continue;

		proto::common::business_active_info *p_info = reply.add_active_list();

		p_info->set_active_id( it.first );
		p_info->set_red_num( p_active->get_red_num() );
		p_info->set_start_time( p_active->get_active_start_time() );
		p_info->set_end_time( p_active->get_active_end_time() );
	}

	//发送显示列表给客户端
	p_role->send_msg_to_client(op_cmd::gc_init_active_info_reply, reply);
}

//时间回调
void business_activity_manager_t::on_timer(uint32_t id, bool is_active)
{
	//遍历	
	std::map<uint32_t, BusinessActiveTable*> conf_map;
	GET_ALL_CONF(BusinessActiveTable, conf_map);

	for (auto it : conf_map)
	{
		BusinessActiveTable *p_conf = it.second;
		if (NULL == p_conf)
			continue;

		if (p_conf->time_id() == id)
		{
			if (is_active == true)
			{
				uint32_t start_time = time_manager_t::get_timer_start_time(id);
				set_active_start( p_conf->id(), start_time );
			}
			else 
			{
				set_active_stop( p_conf->id() );
			}
		}
	}
	std::map<uint32_t, GoldLuckydraw*> conf_map1;
	GET_ALL_CONF(GoldLuckydraw, conf_map1);
	for (auto it : conf_map1)
	{
		if (it.first == id)
		{
			if (!is_active)
			{
				m_gold_luckydraw.clear_lucky();
				set_active_stop(proto::common::GOLD_LUCKYDRAW);
			}
			else
			{
				uint32_t start_time = time_manager_t::get_timer_start_time(id);
				set_active_start(proto::common::GOLD_LUCKYDRAW, start_time);
			}
		}
	}

	std::map<uint32_t, TimeRank*> conf_map2;
	GET_ALL_CONF(TimeRank, conf_map2);
	for (auto it : conf_map2)
	{
		if (it.first == id)
		{
			if (!is_active)
			{
				set_active_stop(proto::common::TIME_RANK);
			}
			else
			{
				uint32_t start_time = time_manager_t::get_timer_start_time(id);
				set_active_start(proto::common::TIME_RANK, start_time);
			}
		}
	}
}

bool business_activity_manager_t::init()
{
	std::map<uint32_t, BusinessActiveTable*> conf_map;
	GET_ALL_CONF(BusinessActiveTable, conf_map);
	for (auto it : conf_map)
	{
		BusinessActiveTable *p_conf = it.second;
		if (p_conf == NULL)
			continue;

		if (p_conf->time_id() == 0)
			continue;

		time_manager_t::register_func(p_conf->time_id(), business_activity_manager_t::on_timer_callback);
	}
	std::map<uint32_t, GoldLuckydraw*> conf_map1;
	GET_ALL_CONF(GoldLuckydraw, conf_map1);
	for (auto it : conf_map1)
	{
		time_manager_t::register_func(it.first, business_activity_manager_t::on_timer_callback);
	}
	std::map<uint32_t, TimeRank*> conf_map2;
	GET_ALL_CONF(TimeRank, conf_map2);
	for (auto it : conf_map2)
	{
		time_manager_t::register_func(it.first, business_activity_manager_t::on_timer_callback);
	}
	return true;
}

//所有人的时间管理
void business_activity_manager_t::on_timer_callback(uint32_t id, bool is_in_time)
{
	TimeTable *p_conf = GET_CONF(TimeTable, id);
	if (NULL == p_conf)
		return;

	if (p_conf->time_flag() != 1)
		return; 

	//不是运营类活动，直接无视
	if (is_in_time)
		role_manager_t::exec_all_user( business_activity_manager_t::exec_all_user_start, id);
	else
		role_manager_t::exec_all_user( business_activity_manager_t::exec_all_user_stop, id);
}


//检测开始条件
//true表示显示
bool business_activity_manager_t::check_open_cond( role_ptr p_role, BusinessActiveTable *p_conf, base_business_active_t *p_active )
{
	if (NULL == p_role) 
		return false;

	if (NULL == p_conf)
		return false;

	switch (p_conf->get_open_cond())
	{
		case BA_OPEN_TYPE::TYPE_LEVEL: 
		{
			if (p_conf->get_open_value() > p_role->get_level())
				return false;
		}
		break;
		default:
			return false;
	}
	return true;
}


//true 表示显示
bool business_activity_manager_t::check_end_cond(role_ptr p_role, BusinessActiveTable *p_conf, base_business_active_t *p_active)
{
	if (NULL == p_role || NULL == p_conf || NULL == p_active)
		return false;
	
	uint32_t time_id = p_conf->time_id();

	//永久显示
	if (p_conf->remove_cond() == BA_DEL_TYPE::TYPE_NONE)
		return true;

	//如果在活动中
	if (  time_id != 0 && time_manager_t::is_in_time(time_id) )
		return true;

	//如果不在活动中，奖励没有领完
	if (p_active->is_get_all_prize() == false)
		return true;

	//奖励领完了
	//uint32_t now = time_util_t::now_time();
	const uint32_t end_time   = time_manager_t::get_timer_end_time( time_id );
	switch (p_conf->remove_cond())
	{
		case BA_DEL_TYPE::TYPE_ZERO:		//5点删除
		{
			//如果结束时间小于次日日切时间
			uint32_t next_zero_time = time_util_t::get_zero_time() + DAY_SECOND;	//次日日切时间
			if ( end_time > next_zero_time )
				return false;

			return true;
		}
		break;
		default:
			return false;
	}
	return false;
}

void business_activity_manager_t::exec_all_user_start(role_ptr p_role, uint32_t ex_data)
{
	if (NULL == p_role)
		return;
	
	auto &business_mgr = p_role->get_business_act_mgr();
	return business_mgr.on_timer(ex_data, true);
}

void business_activity_manager_t::exec_all_user_stop(role_ptr p_role, uint32_t ex_data)
{
	if (NULL == p_role)
		return;
	
	auto &business_mgr = p_role->get_business_act_mgr();
	return business_mgr.on_timer(ex_data, false);
}

void business_activity_manager_t::set_active_start(uint32_t id, uint32_t start_time )
{
	auto iter_f = m_active_list.find(id);
	if (iter_f == m_active_list.end())
		return;

	base_business_active_t *p_active = iter_f->second;
	if (p_active == NULL)
		return;

	p_active->set_active_start( start_time);
}

void business_activity_manager_t::set_active_stop(uint32_t id)
{
	auto iter_f = m_active_list.find(id);
	if (iter_f == m_active_list.end())
		return;

	base_business_active_t *p_active = iter_f->second;
	if (p_active == NULL)
		return;

	p_active->set_active_stop();
}

void business_activity_manager_t::init_active_info()
{
	check_all_yyhd();
}

//通知添加单个活动信息
void business_activity_manager_t::add_icon_info( role_ptr p_role, uint32_t id, uint32_t red_num, uint32_t start_time, uint32_t end_time)
{
	if (NULL == p_role)
	{
		log_error("p_role is NULL");
		return;
	}
	
	proto::client::gc_active_start_reply  reply;
	proto::common::business_active_info *p_info = reply.mutable_info();
	p_info->set_active_id(id);
	p_info->set_red_num(red_num);
	p_info->set_start_time(start_time);
	p_info->set_end_time(end_time);
	p_role->send_msg_to_client(op_cmd::gc_active_start_reply, reply);
}

//删除
void business_activity_manager_t::del_icon_info( role_ptr p_role, uint32_t id )
{
	if (NULL == p_role)
	{
		log_error("p_role is NULL");
		return;
	}

	proto::client::gc_active_stop_reply  reply;
	reply.set_active_id(id);
	p_role->send_msg_to_client(op_cmd::gc_active_stop_reply, reply);
}

//升级后要同步一次
void business_activity_manager_t::on_level_up()
{
	check_all_yyhd();
}


//---------------------------基金-----------------------
uint32_t business_activity_manager_t::get_level_fund_info(role_ptr &p_role, proto::client::gc_get_level_fund_info_reply &reply)
{
	if (NULL == p_role)
	{
		log_error("p_role is NULL");
		return errcode_enum::notice_unknown;
	}

	auto &business_mgr = p_role->get_business_act_mgr();
	return business_mgr.m_level_fund.get_level_fund_info(reply);
}

uint32_t business_activity_manager_t::buy_level_fund(role_ptr &p_role, proto::common::role_change_data *rcd)
{
	if (NULL == p_role)
	{
		log_error("p_role is NULL");
		return errcode_enum::notice_unknown;
	}
	auto &business_mgr = p_role->get_business_act_mgr();
	return business_mgr.m_level_fund.buy_level_fund(rcd);
}

uint32_t business_activity_manager_t::get_level_fund_prize(role_ptr &p_role, uint32_t index, proto::common::role_change_data *rcd)
{
	if (NULL == p_role)
	{
		log_error("p_role is NULL");
		return errcode_enum::notice_unknown;
	}

	auto &business_mgr = p_role->get_business_act_mgr();
	return business_mgr.m_level_fund.get_level_fund_prize(index, rcd);
}

//兑换
uint32_t business_activity_manager_t::get_exchange_item_info(role_ptr &p_role, proto::client::gc_get_exchange_info_reply &reply)
{
	if (NULL == p_role)
	{
		log_error("p_role is NULL");
		return errcode_enum::notice_unknown;
	}

	auto &business_mgr = p_role->get_business_act_mgr();
	return business_mgr.m_exchange_item.get_exchange_info(reply);
}

uint32_t business_activity_manager_t::get_exchange_item_prize(role_ptr &p_role, uint32_t id, proto::common::role_change_data *rcd)
{
	if (NULL == p_role)
	{
		log_error("p_role is NULL");
		return errcode_enum::notice_unknown;
	}

	auto &business_mgr = p_role->get_business_act_mgr();
	return business_mgr.m_exchange_item.do_exchange_item( id, rcd);
}

//限购
uint32_t business_activity_manager_t::get_limit_buy_info(role_ptr &p_role, proto::client::gc_get_limit_buy_info_reply &reply)
{
	if (NULL == p_role)
	{
		log_error("p_role is NULL");
		return errcode_enum::notice_unknown;
	}

	auto &business_mgr = p_role->get_business_act_mgr();
	return business_mgr.m_limit_buy.get_limit_buy_info(reply);
}

uint32_t business_activity_manager_t::buy_limit(role_ptr &p_role, uint32_t id, proto::common::role_change_data *rcd)
{
	if (NULL == p_role)
	{
		log_error("p_role is NULL");
		return errcode_enum::notice_unknown;
	}

	auto &business_mgr = p_role->get_business_act_mgr();
	return business_mgr.m_limit_buy.buy_limit(id, rcd);
}

//七日目标
uint32_t business_activity_manager_t::get_sevenday_target_info(role_ptr &p_role, uint32_t day, proto::client::gc_get_sd_target_info_reply &reply)
{
	if (NULL == p_role)
	{
		log_error("p_role is NULL");
		return errcode_enum::notice_unknown;
	}

	auto &business_mgr = p_role->get_business_act_mgr();
	return business_mgr.m_sevenday_target.get_sevenday_target_info(day, reply);
}

uint32_t business_activity_manager_t::get_sevenday_target_prize(role_ptr &p_role, uint32_t id, proto::common::role_change_data *rcd)
{
	if (NULL == p_role)
	{
		log_error("p_role is NULL");
		return errcode_enum::notice_unknown;
	}

	auto &business_mgr = p_role->get_business_act_mgr();
	return business_mgr.m_sevenday_target.get_sevenday_target_prize( id, rcd );
}

uint32_t business_activity_manager_t::get_sevenday_red_num(role_ptr &p_role, proto::client::gc_get_sd_target_red_reply &reply)
{
	if (NULL == p_role)
	{
		log_error("p_role is NULL");
		return errcode_enum::notice_unknown;
	}

	auto &business_mgr = p_role->get_business_act_mgr();
	return business_mgr.m_sevenday_target.get_sevenday_target_red_num(reply);
}

void business_activity_manager_t::peek_recharge_total_client_data(role_ptr &p_role, proto::common::recharge_total_client_data* p_data)
{
	if (NULL == p_role)
	{
		log_error("p_role is NULL");
		return;
	}
	if (NULL == p_data)
	{
		log_error("p_data is NULL");
		return;
	}
	auto &business_mgr = p_role->get_business_act_mgr();
	business_mgr.m_recharge_total.peek_client_data(*p_data);
}

uint32_t business_activity_manager_t::recharge_total_award(role_ptr &p_role, uint32_t id, proto::common::role_change_data *p_data)
{
	if (NULL == p_role)
	{
		log_error("p_role is NULL");
		return errcode_enum::notice_unknown;
	}

	auto &business_mgr = p_role->get_business_act_mgr();
	return business_mgr.m_recharge_total.award(id, p_data);
}

void business_activity_manager_t::peek_gold_luckydraw_client_data(role_ptr &p_role, proto::common::gold_luckydraw_client_data* p_data)
{
	if (NULL == p_role)
	{
		log_error("p_role is NULL");
		return;
	}
	if (NULL == p_data)
	{
		log_error("p_data is NULL");
		return;
	}
	auto &business_mgr = p_role->get_business_act_mgr();
	business_mgr.m_gold_luckydraw.peek_client_data(*p_data);
}

uint32_t business_activity_manager_t::gold_luckydraw_award(role_ptr &p_role, uint32_t id)
{
	if (NULL == p_role)
	{
		log_error("p_role is NULL");
		return errcode_enum::notice_unknown;
	}

	auto &business_mgr = p_role->get_business_act_mgr();
	return business_mgr.m_gold_luckydraw.award(id);
}

uint32_t business_activity_manager_t::get_gold_luckydraw_expired_time(role_ptr &p_role)
{
	if (NULL == p_role)
	{
		log_error("p_role is NULL");
		return 0;
	}

	auto &business_mgr = p_role->get_business_act_mgr();
	return business_mgr.m_gold_luckydraw.get_expired_time();
}

void business_activity_manager_t::check_seven_day_target(role_ptr p_role)
{
	if (NULL == p_role)
	{
		log_error("p_role is NULL");
		return;
	}

	auto &business_mgr = p_role->get_business_act_mgr();
	business_mgr.m_sevenday_target.check_red_num();
}

void business_activity_manager_t::get_activity_shop_data(role_ptr p_role, uint32_t shop_type, proto::client::gc_activity_shop_get_info_reply& reply)
{
	if (NULL == p_role)
	{
		log_error("p_role in function get_activity_shop_data is NULL");
		return;
	}
	switch (shop_type)
	{
		case proto::common::activity_shop_type_wusheng_rand:
		{
			get_activity_shop_ws_rand(reply.mutable_shop_item_info());
			break;
		}
		case proto::common::activity_shop_type_wusheng_score:
		{
			get_activity_shop_ws_score(reply.mutable_shop_item_info());
			break;
		}
	}

	reply.set_shop_type(shop_type);
	p_role->get_personal_info(reply.mutable_rcd()->mutable_per_info());
}

uint32_t business_activity_manager_t::buy_activity_shop(role_ptr p_role, uint32_t shop_type, uint32_t item_pos, uint32_t item_num, proto::client::gc_activity_shop_buy_reply &reply)
{
	if (NULL == p_role)
	{
		log_error("p_role in function buy_activity_shop is NULL");
		return errcode_enum::notice_unknown;
	}

	uint32_t reply_code = errcode_enum::error_ok;
	if (!p_role->get_business_act_mgr().get_activity_shop().is_active())
	{
		log_error("role[%lu] buy_activity_shop error, shop not active", p_role->get_uid());
		return errcode_enum::notice_activity_not_open;
	}

	switch (shop_type)
	{
	case proto::common::activity_shop_type_wusheng_rand:
	{
		reply_code = buy_activity_shop_ws_rand(p_role, item_pos, item_num, reply);
	}
	break;
	case proto::common::activity_shop_type_wusheng_score:
	{
		reply_code = buy_activity_shop_ws_score(p_role, item_pos, item_num, reply);
	}
	break;
	}
	if (reply_code == errcode_enum::error_ok)
	{
		p_role->get_personal_info(reply.mutable_rcd()->mutable_per_info());
		p_role->save_self();
	}
	return reply_code;
}

uint32_t business_activity_manager_t::get_activity_shop_ws_rand(proto::common::activity_shop_item_info *item_info)
{
	if (NULL == item_info)
	{
		log_error("item_info NULL");
		return errcode_enum::notice_unknown;
	}

	std::map<uint32_t, WsTreasuryTable*> all_confs;
	GET_ALL_CONF(WsTreasuryTable, all_confs);
	for (auto& it : all_confs)
	{
		auto p_conf = it.second;
		if (NULL != p_conf)
		{
			proto::common::activity_shop_item_info_single *p_single = item_info->add_item_list();
			p_single->set_shop_item_id(p_conf->show_item_id());
			p_single->set_shop_item_num(p_conf->show_item_num());
		}
	}

	return errcode_enum::error_ok;
}

uint32_t business_activity_manager_t::get_activity_shop_ws_score(proto::common::activity_shop_item_info *item_info)
{
	if (NULL == item_info)
	{
		log_error("item_info NULL");
		return errcode_enum::notice_unknown;
	}

	std::map<uint32_t, WsScoreTreasuryTable*> all_confs;
	GET_ALL_CONF(WsScoreTreasuryTable, all_confs);
	for (auto& it : all_confs)
	{
		auto p_conf = it.second;
		if (NULL != p_conf)
		{
			proto::common::activity_shop_item_info_single *p_single = item_info->add_item_list();
			p_single->set_shop_item_id(p_conf->show_item_id());
			p_single->set_shop_item_price(p_conf->cost_score());
		}
	}

	return errcode_enum::error_ok;
}

uint32_t business_activity_manager_t::buy_activity_shop_ws_rand(role_ptr p_role, uint32_t item_pos, uint32_t item_num, proto::client::gc_activity_shop_buy_reply &reply)
{
	if (NULL == p_role)
	{
		log_error("p_role in function buy_activity_shop_ws_rand is NULL");
		return errcode_enum::notice_unknown;
	}
	uint32_t max_roll_num = 5;
	if (item_num != 1 && item_num != max_roll_num)
	{
		log_error("role[%lu] item_num[%d] error", p_role->get_uid(), item_num);
		return errcode_enum::notice_unknown;
	}

	auto conf1 = GET_CONF(Comprehensive, comprehensive_common::wsbk_1);
	auto conf2 = GET_CONF(Comprehensive, comprehensive_common::wsbk_2);
	auto conf3 = GET_CONF(Comprehensive, comprehensive_common::wsbk_3);
	auto conf4 = GET_CONF(Comprehensive, comprehensive_common::wsbk_4);
	uint32_t cost_item_tid = GET_COMPREHENSIVE_VALUE_1(conf1);
	uint32_t cost_item_num_1 = GET_COMPREHENSIVE_VALUE_2(conf1);
	uint32_t cost_item_num_5 = GET_COMPREHENSIVE_VALUE_3(conf1);
	uint32_t cost_yuanbao_1 = GET_COMPREHENSIVE_VALUE_1(conf2);
	uint32_t cost_yuanbao_5 = GET_COMPREHENSIVE_VALUE_2(conf2);
	uint32_t cost_money_type = GET_COMPREHENSIVE_VALUE_3(conf2);
	int32_t max_lucky = GET_COMPREHENSIVE_VALUE_1(conf3);
	uint32_t day_limit_buy = GET_COMPREHENSIVE_VALUE_1(conf4);

	if (proto::common::MONEY_TYPE_NONE == cost_money_type || !proto::common::MONEY_TYPE_IsValid(cost_money_type)) {
		log_error("role[%lu] buy_activity_shop_ws_rand cost money_type[%u] error", p_role->get_uid(), cost_money_type);
		return errcode_enum::notice_unknown;
	}

	uint32_t today_buy_times = p_role->get_business_act_mgr().get_activity_shop().get_wusheng_today_buy();
	if (today_buy_times + item_num > day_limit_buy) {
		log_error("role[%lu] buy_activity_shop_ws_rand today[%u] + buy[%u] > max[%u]", p_role->get_uid(), today_buy_times, item_num, day_limit_buy);
		return errcode_enum::wsbk_day_buy_limit;
	}

	item_ptr p_item = item_manager_t::get_item_by_tid(p_role, cost_item_tid);
	if (NULL == p_item)
	{
		uint32_t cost_yuanbao = cost_yuanbao_1;
		if (item_num == max_roll_num)
			cost_yuanbao = cost_yuanbao_5;

		if (!money_manager_t::use_money(p_role, (proto::common::MONEY_TYPE)cost_money_type, cost_yuanbao, log_enum::source_type_activity_ws_rand,
			0, false, reply.mutable_rcd()))
		{
			log_error("role[%lu] buy_activity_shop_ws_rand money_not_enough", p_role->get_uid());
			return errcode_enum::notice_life_stone_not_enough;
		}
	}
	else
	{
		int32_t cost_item_num = cost_item_num_1;
		if (item_num == max_roll_num)
			cost_item_num = cost_item_num_5;

		if (p_item->get_cur_num() < cost_item_num)
		{
			log_error("role[%lu] buy_activity_shop_ws_rand item_not_enough", p_role->get_uid());
			return errcode_enum::notice_item_not_enough;
		}
		item_manager_t::remove_item(p_role, cost_item_tid, cost_item_num, log_enum::source_type_activity_ws_rand, 0, reply.mutable_rcd());
	}

	std::map<uint32_t, WsTreasuryTable*> all_confs;
	GET_ALL_CONF(WsTreasuryTable, all_confs);
	std::map<uint32_t, uint32_t> item_rand_info_list;

	//	for (uint32_t i = 0; i < item_num; ++i)
	{
		uint32_t total_rand_percent = 0;
		uint32_t roll_shop_item_id = 0;
		uint32_t role_lucky = p_role->get_business_act_mgr().get_activity_shop().get_wusheng_lucky();
		item_rand_info_list.clear();

		for (auto& it : all_confs)
		{
			auto p_conf = it.second;
			if (NULL == p_conf)
			{
				continue;
			}
			int32_t rand_percent = p_conf->init_percent() + role_lucky * p_conf->lucky_add_percent_val() - p_conf->dec_percent();
			rand_percent = rand_percent < 0 ? 0 : rand_percent;
			total_rand_percent += rand_percent;
			item_rand_info_list.insert(std::make_pair(p_conf->id(), total_rand_percent));
			// 			log_debug("wusheng 道具ID:%d 道具权重:%d-%d 计算[init:%d + lucky:%d * add_value:%d - dec_value:%d]", conf.second->id(),
			// 					rand_percent, total_rand_percent, conf.second->init_percent(), role_lucky, conf.second->lucky_add_percent_val(), 
			// 					conf.second->dec_percent());
		}
		//		log_debug("wusheng 开始抽取 幸运值:%d", p_role->get_activity_shop_data().get_wusheng_lucky());

		int32_t rand_value = random_util_t::randBetween(1, total_rand_percent);
		if (-1 == rand_value)
		{
			log_error("role[%lu] buy_activity_shop_ws_rand rand_value = -1! total_rand_percent[%d]", p_role->get_uid(), total_rand_percent);
		}
		for (auto& pair : item_rand_info_list)
		{
			if ((uint32_t)rand_value < pair.second)
			{
				roll_shop_item_id = pair.first;
				break;
			}
		}
		//		log_debug("wusheng 随机数:%d 抽取到的道具ID:%d", rand_value, roll_shop_item_id);
		WsTreasuryTable *p_conf = GET_CONF(WsTreasuryTable, roll_shop_item_id);
		if (NULL == p_conf)
		{
			log_error("role[%lu] p_conf NULL id:%d", p_role->get_uid(), roll_shop_item_id);
			return errcode_enum::notice_unknown;
		}

		drop_manager_t::drop(p_role, p_conf->drop_id(), log_enum::source_type_activity_ws_rand,
			proto::common::drop_sys_type_activity_ws_rand, 0, reply.mutable_rcd());

		if (p_conf->notice() > 0)
		{
			std::vector<std::string> param_str_list;
			param_str_list.push_back(p_role->get_name());
			param_str_list.push_back(common::string_util_t::uint32_to_string(p_conf->show_item_id()));
			param_str_list.push_back(common::string_util_t::uint32_to_string(p_conf->show_item_num()));
			sys_notice_manager_t::sys_notice_notify(p_role->get_uid(), proto::common::sys_notify_type_wstreasury, 0, param_str_list);

		}

		p_role->get_business_act_mgr().get_activity_shop().add_wusheng_score(p_conf->add_score());
		p_role->get_business_act_mgr().get_activity_shop().add_wusheng_today_buy(item_num);

		int32_t new_lucky = (int32_t)p_role->get_business_act_mgr().get_activity_shop().get_wusheng_lucky() + p_conf->add_lucky() - p_conf->dec_lucky();
		new_lucky = new_lucky < 0 ? 0 : new_lucky;
		new_lucky = new_lucky > max_lucky ? max_lucky : new_lucky;
		p_role->get_business_act_mgr().get_activity_shop().set_lucky(new_lucky);
		//		log_debug("wusheng 抽取结束 幸运值:%d", p_role->get_activity_shop_data().get_wusheng_lucky());

		reply.add_roll_item_pos(roll_shop_item_id);
	}
	return errcode_enum::error_ok;
}

uint32_t business_activity_manager_t::buy_activity_shop_ws_score(role_ptr p_role, uint32_t item_pos, uint32_t item_num, proto::client::gc_activity_shop_buy_reply &reply)
{
	if (NULL == p_role)
	{
		log_error("p_role in function buy_activity_shop_ws_score is NULL");
		return errcode_enum::notice_unknown;
	}

	uint32_t shop_item_id = item_pos;
	WsScoreTreasuryTable *p_conf = GET_CONF(WsScoreTreasuryTable, shop_item_id);
	if (NULL == p_conf)
	{
		log_error("role[%lu] get WsScoreTreasuryTable conf NULL, shop_item_id[%d]", p_role->get_uid(), shop_item_id);
		return errcode_enum::notice_unknown;
	}

	// 判断积分
	if (p_role->get_business_act_mgr().get_activity_shop().get_wusheng_score() < p_conf->cost_score())
	{
		log_error("role[%lu] buy_activity_shop_ws_score score not enough", p_role->get_uid());
		return errcode_enum::activity_ws_score_not_enough;
	}

	p_role->get_business_act_mgr().get_activity_shop().dec_wusheng_score(p_conf->cost_score());
	drop_manager_t::drop(p_role, p_conf->drop_id(), log_enum::source_type_activity_ws_score,
		proto::common::drop_sys_type_activity_ws_score, 0, reply.mutable_rcd());

	return errcode_enum::error_ok;
}
