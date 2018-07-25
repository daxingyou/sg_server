#include "month_card.hpp"
#include "tblh/MonthCard.tbls.h"
#include "config/config_manager.hpp"
#include "tblh/errcode_enum.hpp"
#include "role/role_manager.hpp"
#include "item/drop_manager.hpp"
#include "common/cache_key.hpp"
#include "role/role.hpp"
#include "role/role_unify_save.hpp"
#include "common/utility.hpp"
#include "red_point/red_point_manager.hpp"
#include "tblh/Recharge.tbls.h"
#include "role/money_manager.hpp"

month_card_t::month_card_t(uint64_t uid):m_owner(uid)
{
	m_key = cache_key_t::create(m_owner, cache_name::month_card);
}


bool month_card_t::load_data( const proto::common::month_card_save_data &data)
{
	m_month_card_ed_prize_map.clear();
	m_month_card_time_map.clear();

	for (int i = 0; i < data.card_info_size(); ++i )
	{ 
		const proto::common::month_card_info p_card = data.card_info(i);
		m_month_card_time_map.insert(std::make_pair(p_card.month_card_id(), p_card.end_time()));

		//如果状态已经领取，就插入领取map
		if (p_card.ed_prize() == proto::common::PRIZE_STATUS::PRIZE_STATUS_HAS)
		{
			m_month_card_ed_prize_map.insert(p_card.month_card_id());
		}
	}
	return true;
}

bool month_card_t::peek_data(proto::common::month_card_save_data &data)
{
	for (auto it : m_month_card_time_map)
	{
		proto::common::month_card_info *p_info = data.add_card_info();
		p_info->set_month_card_id(it.first);
		p_info->set_end_time(get_month_card_end_time(it.first));
		p_info->set_ed_prize(get_month_card_ed_prize_status(it.first));
	}
	return true;
}

bool month_card_t::save_data()
{
	proto::common::month_card_save_data data;
	peek_data(data);
	role_unify_save::add_task( m_owner, m_key, data);
	return true;
}

uint32_t month_card_t::get_month_card_info(proto::client::gc_get_month_card_info_reply &reply)
{
	std::map<uint32_t, MonthCard*> month_card_map;
	GET_ALL_CONF(MonthCard, month_card_map);
	for (auto it : month_card_map)
	{
		MonthCard *p_conf = it.second;
		if (NULL == p_conf)
			continue;

		proto::common::month_card_info *p_info = reply.add_month_card_list();
		p_info->set_month_card_id(it.first);
		p_info->set_end_time(get_month_card_end_time(it.first));
		p_info->set_ed_prize(get_month_card_ed_prize_status(it.first));

		proto::common::month_card_show_info *p_show_info = p_info->mutable_show_info();
		p_show_info->set_max_day(p_conf->add_day());
		p_show_info->set_mingshi(p_conf->show_mingshi());
		p_show_info->set_ed_yuanbao(p_conf->show_yuanbao());
		p_show_info->set_show_gift(p_conf->show_gift());
		p_show_info->set_can_buy_day(p_conf->add_limit());
		p_show_info->set_cz_id(p_conf->cz_id());
	}
	return errcode_enum::error_ok;
}

uint32_t month_card_t::get_month_card_ed_prize(uint32_t card_id, proto::common::role_change_data *rcd)
{
	role_ptr p_role = role_manager_t::find_role(m_owner);
	if ( NULL == p_role )
	{ 
		log_error("user[%lu], user is NULL", m_owner);
		return errcode_enum::notice_unknown;
	}

	if ( get_month_card_ed_prize_status(card_id) != proto::common::PRIZE_STATUS::PRIZE_STATUS_CAN )
		return errcode_enum::notice_unknown;

	MonthCard *p_conf = GET_CONF(MonthCard, card_id);
	if (NULL == p_conf)
	{
		log_error("user[%lu] [%u] MonthCard conf is NULL", m_owner, card_id);
		return errcode_enum::notice_unknown;
	}

	bool ret = drop_manager_t::drop(p_role, p_conf->ed_prize(), log_enum::source_type_bussiness_month_card_get_prize, proto::common::drop_sys_type_month_card_prize, card_id, rcd);
	if (!ret)
	{
		log_error("user[%lu] [%u] MonthCard drop faild", m_owner, card_id);
		return errcode_enum::notice_unknown;
	}

	m_month_card_ed_prize_map.insert(card_id);

	save_data();

	return errcode_enum::error_ok;
}

uint32_t month_card_t::buy_month_card(uint32_t card_id)
{
	role_ptr p_role = role_manager_t::find_role(m_owner);
	if (NULL == p_role)
	{
		log_error("user[%lu], user is NULL", m_owner);
		return errcode_enum::notice_unknown;
	}

	MonthCard *p_conf = GET_CONF(MonthCard, card_id);
	if (NULL == p_conf)
	{
		log_error("user[%lu] [%u] MonthCard conf is NULL", m_owner, card_id);
		return errcode_enum::notice_unknown;
	}

	uint32_t time_left = get_month_card_end_time(card_id);
	if ( time_left > time_util_t::get_zero_time() + p_conf->add_limit() )
	{
		log_error("user[%lu] buy month card faild time_left[%u], time_limit[%u]", m_owner, time_left, p_conf->add_limit() );
		return errcode_enum::notice_unknown;
	}

	//TODO 特殊代码，用于20150509测试
	uint32_t cost = 300;
	if (card_id == 1) {
		cost = 300;
	}
	else {
		cost = 980;
	}

	if (money_manager_t::use_money(p_role, proto::common::MONEY_TYPE_YUANBAO, cost, log_enum::source_type_bussiness_buy_month_card, card_id) == false) {
		log_error("user[%lu] buy month card faild not enohgh money ", m_owner);

		proto::client::gc_buy_month_card_reply reply;
		get_month_card_info(reply);
		reply.set_reply_code(errcode_enum::notice_gold_money_not_enough);
		p_role->send_msg_to_client(op_cmd::gc_buy_month_card_reply, reply);
		return errcode_enum::notice_gold_money_not_enough;
	}

	proto::common::role_change_data rcd;
	on_buy_month_card_callback(card_id, &rcd);
	return errcode_enum::error_ok;
}

uint32_t month_card_t::on_buy_month_card_callback(uint32_t card_id, proto::common::role_change_data *rcd)
{
	role_ptr p_role = role_manager_t::find_role(m_owner);
	if (NULL == p_role)
	{
		log_error("user[%lu], user is NULL", m_owner);
		return errcode_enum::notice_unknown;
	}

	MonthCard *p_conf = GET_CONF(MonthCard, card_id);
	if (NULL == p_conf)
	{
		log_error("user[%lu] [%u] MonthCard conf is NULL", m_owner, card_id);
		return errcode_enum::notice_unknown;
	}

	auto iter_f = m_month_card_time_map.find(card_id);
	if (iter_f == m_month_card_time_map.end())
	{
		m_month_card_time_map.insert( std::make_pair( card_id, time_util_t::get_zero_time() + ( p_conf->add_day() - 10) ) );
	}
	else
	{
		iter_f->second += p_conf->add_day();
	}

	save_data();

	//加送命石
	Recharge *p_recharge_conf = GET_CONF(Recharge, p_conf->cz_id());
	if (NULL == p_recharge_conf) {
		log_error("user[%lu] [%u] cz_id config is not found", m_owner, card_id);
		return errcode_enum::notice_unknown;
	}
	money_manager_t::add_money( p_role, proto::common::MONEY_TYPE_LIFE_STONE, p_recharge_conf->money(), log_enum::source_type_bussiness_buy_month_card, p_conf->cz_id() );
	proto::common::personal_info *persion = rcd->mutable_per_info();
	persion->set_month_card_id(get_month_card_id());
	proto::client::gc_buy_month_card_reply reply;
	uint32_t reply_code = get_month_card_info( reply );
	reply.mutable_rcd()->CopyFrom( *rcd );
	reply.set_reply_code( reply_code );
	proto::common::role_change_data* p_rcd = reply.mutable_rcd();
	if (p_rcd) {
		p_role->get_personal_info(p_rcd->mutable_per_info());
	}
	p_role->send_msg_to_client(op_cmd::gc_buy_month_card_reply, reply);

	if (get_red_num() > 0)
	{
		red_point_manager_t::red_point_notify(m_owner, proto::common::RED_POINT_TYPE::RED_POINT_TYPE_MONTH_CARD);
	}

	return errcode_enum::error_ok;
}

uint32_t month_card_t::get_month_card_info(proto::client::gc_buy_month_card_reply &reply)
{
	std::map<uint32_t, MonthCard*> month_card_map;
	GET_ALL_CONF(MonthCard, month_card_map);
	for (auto it : month_card_map)
	{
		MonthCard *p_conf = it.second;
		if (NULL == p_conf)
			continue;

		proto::common::month_card_info *p_info = reply.add_month_card_list();
		p_info->set_month_card_id(it.first);
		p_info->set_end_time(get_month_card_end_time(it.first));
		p_info->set_ed_prize(get_month_card_ed_prize_status(it.first));

		proto::common::month_card_show_info *p_show_info = p_info->mutable_show_info();
		p_show_info->set_max_day(p_conf->add_day());
		p_show_info->set_mingshi(p_conf->show_mingshi());
		p_show_info->set_ed_yuanbao(p_conf->show_yuanbao());
		p_show_info->set_show_gift(p_conf->show_gift());
		p_show_info->set_can_buy_day(p_conf->add_limit());
		p_show_info->set_cz_id(p_conf->cz_id());
	}
	return errcode_enum::error_ok;
}

uint32_t month_card_t::get_month_card_end_time(uint32_t card_id)
{
	auto iter_f = m_month_card_time_map.find(card_id);
	if ( iter_f == m_month_card_time_map.end() )
		return 0;
	
	MonthCard *p_conf = GET_CONF(MonthCard, card_id);
	if (NULL == p_conf)
		return 0;

	uint32_t end_time = iter_f->second > time_util_t::now_time() ? iter_f->second : 0;

	return end_time;
}

uint32_t month_card_t::get_month_card_ed_prize_status(uint32_t card_id)
{
	auto iter_f = m_month_card_ed_prize_map.find(card_id);
	if (iter_f != m_month_card_ed_prize_map.end())
		return proto::common::PRIZE_STATUS::PRIZE_STATUS_HAS;

	if (get_month_card_end_time(card_id) >= time_util_t::now_time())
		return proto::common::PRIZE_STATUS::PRIZE_STATUS_CAN;

	return proto::common::PRIZE_STATUS::PRIZE_STATUS_NONE;
}


void month_card_t::one_day()
{
	m_month_card_ed_prize_map.clear();
		
	//查询一下所有月卡如果小于一天的直接结束
	for (auto it : m_month_card_time_map) 
	{
		if (get_month_card_time_left(it.first) < DAY_SECOND)
		{
			it.second = 0;
		}
	}
	save_data();
	
	if (get_red_num() > 0)
	{
		red_point_manager_t::red_point_notify(m_owner, proto::common::RED_POINT_TYPE::RED_POINT_TYPE_MONTH_CARD);
	}
}


uint32_t month_card_t::get_month_card_id()
{
	uint32_t card_id = 0;
	for (auto it : m_month_card_time_map) {
		if (get_month_card_end_time(it.first) < time_util_t::now_time()) {
			continue;
		}
		if (it.first > card_id) {
			card_id = it.first;
		}
	}
	return card_id;
}

uint32_t month_card_t::get_month_card_time_left(uint32_t card_id)
{
	auto iter_f = m_month_card_time_map.find(card_id);
	if (iter_f == m_month_card_time_map.end())
		return 0;

	uint32_t now = time_util_t::now_time();
	uint32_t time_left = iter_f->second > now ? (iter_f->second - now) : 0;
	return time_left;
}

void  month_card_t::on_login()
{
	if (get_red_num() > 0)
	{
		red_point_manager_t::red_point_notify(m_owner, proto::common::RED_POINT_TYPE::RED_POINT_TYPE_MONTH_CARD);
	}
}

uint32_t month_card_t::get_red_num()
{
	uint32_t count = 0;
	for (auto it : m_month_card_time_map )
	{
		if (it.second <= time_util_t::now_time())
			continue;

		if (get_month_card_ed_prize_status(it.first) == proto::common::PRIZE_STATUS::PRIZE_STATUS_CAN)
			count += 1;
	}
	return count;
}
