#include "limit_buy.hpp"
#include "tblh/LimitBuyTable.tbls.h"
#include "config/config_manager.hpp"
#include "role/role.hpp"
#include "role/role_manager.hpp"
#include "tblh/errcode_enum.hpp"
#include "cache_key.hpp"
#include "item/drop_manager.hpp"
#include "role/role_unify_save.hpp"
#include "tblh/Comprehensive.tbls.h"
#include "role/money_manager.hpp"
#include "red_point/red_point_manager.hpp"

limit_buy_t::limit_buy_t(uint64_t role_id) : m_role_id(role_id)
{
	m_key = cache_key_t::create(m_role_id, cache_name::limit_buy);
}

uint32_t limit_buy_t::get_limit_buy_info(proto::client::gc_get_limit_buy_info_reply &reply)
{
	role_ptr p_role = role_manager_t::find_role(m_role_id);
	if (NULL == p_role)
	{
		log_error("user[%lu] is NULL", m_role_id);
		return errcode_enum::notice_unknown;
	}
	
	reply.set_day( m_day );
	std::map<uint32_t, LimitBuyTable*> conf_map;
	GET_ALL_CONF(LimitBuyTable, conf_map);
	for (auto it : conf_map)
	{
		LimitBuyTable *p_conf = it.second;
		if (NULL == p_conf)
			continue;

		proto::common::limit_buy_client_data *p_data = reply.add_data();
		p_data->set_id( it.first );
		p_data->set_show_item_id(p_conf->show_item_id());
		p_data->set_show_item_num(p_conf->show_item_num());
		p_data->set_buy_money_type(p_conf->money_type());
		p_data->set_buy_money(p_conf->money());
		p_data->set_buy_num(p_conf->max_num());
		p_data->set_desc(common::string_util_t::convert_to_utf8(p_conf->show_prize().c_str()));
		p_data->set_src_buy_money( p_conf->show_moeny() );
		p_data->set_is_buy_num(get_buy_num(it.first));
	}

	m_red += 1;
	if (m_red == 1)
	{
		save_data();
	}

	return errcode_enum::error_ok;
}

uint32_t limit_buy_t::get_prize_status(uint32_t index)
{
	LimitBuyTable *p_conf = GET_CONF(LimitBuyTable, index);
	if (NULL == p_conf)
		return proto::common::PRIZE_STATUS::PRIZE_STATUS_NONE;

	if( p_conf->id() > m_day )
		return proto::common::PRIZE_STATUS::PRIZE_STATUS_NONE;

	uint32_t num = get_buy_num(index);

	if( num >= p_conf->max_num() )
		return proto::common::PRIZE_STATUS::PRIZE_STATUS_HAS;

	return proto::common::PRIZE_STATUS::PRIZE_STATUS_CAN;
}

bool limit_buy_t::load_data(const proto::common::limit_buy_save_data &data)
{
	m_day = data.day();
	m_red = data.red();
	for (int i = 0; i < data.info_size(); ++i)
	{
		const proto::common::limit_buy_save_info &p_info = data.info(i);
		m_prize_list.insert(std::make_pair(p_info.id(), p_info.num()));
	}
	
	//新角色load后变0
	if (m_day == 0)
		m_day = 1;

	return true;
}

bool limit_buy_t::peek_data(proto::common::limit_buy_save_data &data)
{
	data.set_day(m_day);
	data.set_red(m_red);

	for (auto it : m_prize_list)
	{
		proto::common::limit_buy_save_info *p_info = data.add_info();
		p_info->set_id(it.first);
		p_info->set_num(it.second);
	}
	return true;
}

bool limit_buy_t::save_data()
{
	proto::common::limit_buy_save_data data;
	peek_data(data);
	role_unify_save::add_task(m_role_id, m_key, data);
	return true;
}

//是否在活动中
bool limit_buy_t::is_active()
{
	return (is_get_all_prize() == false);
}

//获取购买次数
uint32_t limit_buy_t::get_buy_num(uint32_t index)
{
	auto iter_f = m_prize_list.find(index);
	if (iter_f == m_prize_list.end())
		return 0;
	
	return iter_f->second;
}

//是否领取了所有奖励
bool limit_buy_t::is_get_all_prize()
{
	bool ret = true;
	std::map<uint32_t, LimitBuyTable*> conf_map;
	GET_ALL_CONF(LimitBuyTable, conf_map);

	for (auto it : conf_map)
	{
		LimitBuyTable *p_conf = it.second;
		if (NULL == p_conf)
			continue;

		if (get_prize_status(it.first) != proto::common::PRIZE_STATUS_HAS)
		{
			ret = false;
			break;
		}
	}
	return ret;
}

//是否是结束时间了
bool limit_buy_t::is_end_show_time()
{
	return is_get_all_prize();
}

//获取活动开始时间, 如果是0表示永不结束
uint32_t limit_buy_t::get_active_start_time(uint32_t start_time)
{
	return 0;
}

//获取活动结束时间,放在模块做，方便扩展
uint32_t limit_buy_t::get_active_end_time()
{
	return 0;
}

//设置活动开始,本活动不需要重复开启，属于默认开启状态
void limit_buy_t::set_active_start()
{
	return;
}

//设置活动结束
void limit_buy_t::set_active_stop()
{
	return;
}

//获取红点
uint32_t limit_buy_t::get_red_num()
{
	if (m_red > 0)
		return 0;

	uint32_t count = 0;

	std::map<uint32_t, LimitBuyTable*> conf_map;
	GET_ALL_CONF(LimitBuyTable, conf_map);

	for (auto it : conf_map)
	{
		LimitBuyTable *p_conf = it.second;
		if (NULL == p_conf)
			continue;

		if (get_prize_status(it.first) == proto::common::PRIZE_STATUS_CAN)
			count++;
	}
	return count;
}

uint32_t limit_buy_t::buy_limit( uint32_t index, proto::common::role_change_data *rcd)
{
	role_ptr p_role = role_manager_t::find_role(m_role_id);
	if (NULL == p_role)
	{
		log_error("user[%lu] is NULL", m_role_id);
		return errcode_enum::notice_unknown;
	}

	LimitBuyTable *p_conf = GET_CONF(LimitBuyTable, index);
	if (NULL == p_conf)
	{
		log_error("user[%lu] limitbuytable[%u] conf is null", m_role_id, index);
		return errcode_enum::notice_unknown;
	}

	if (p_conf->get_id() > m_day)
	{
		log_error("user[%lu] buy day[%u] is > cur_day[%u]", m_role_id, p_conf->get_id(), m_day);
		return errcode_enum::notice_unknown;
	}

	uint32_t num = get_buy_num(index);
	if (num >= p_conf->max_num())
	{
		log_error("user[%lu] buy num[%u] is > max_num[%u]", m_role_id, num, p_conf->max_num() );
		return errcode_enum::notice_unknown;
	}
	
	if (!money_manager_t::use_money(p_role, proto::common::MONEY_TYPE(p_conf->money_type()), p_conf->money(), log_enum::source_type_bussiness_buy_limit, 0, true, rcd))
	{
		log_error("user[%lu] not enough!!!", m_role_id);
		return errcode_enum::notice_gold_money_not_enough;
	}

	if (drop_manager_t::drop(p_role, p_conf->drop_id(), log_enum::source_type_bussiness_buy_limit, proto::common::drop_sys_type_limit_buy, index, rcd ) == false)
	{
		log_error("user[%lu] drop[%u] faild!!!", m_role_id, p_conf->drop_id() );
		return errcode_enum::notice_unknown;
	}

	m_prize_list[index] += 1;

	save_data();

	proto::client::gc_active_update_reply reply;
	reply.set_id(proto::common::BUSINESS_ACTIVE_TYPE::ED_BUY_LIMIT);
	reply.set_red_num( get_red_num() );
	p_role->send_msg_to_client(op_cmd::gc_active_update_reply, reply);

	return errcode_enum::error_ok;
}

void limit_buy_t::one_day() 
{
	 m_day += 1;
	 m_red = 0;
	 save_data();

	 if( get_red_num() > 0 )
		red_point_manager_t::red_point_notify(m_role_id, proto::common::RED_POINT_TYPE::RED_POINT_TYPE_FULI, 1);
}

