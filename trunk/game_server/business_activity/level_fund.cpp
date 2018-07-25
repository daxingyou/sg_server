#include "level_fund.hpp"
#include "tblh/LevelFundTable.tbls.h"
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

level_fund_t::level_fund_t(uint64_t role_id) : m_role_id( role_id )
{
	m_key = cache_key_t::create(m_role_id, cache_name::level_fund);
}

//领取奖励
uint32_t level_fund_t::get_level_fund_prize(uint32_t index, proto::common::role_change_data *rcd )
{
	role_ptr p_role = role_manager_t::find_role(m_role_id);
	if (NULL == p_role)
	{
		log_error("user[%lu] is NULL", m_role_id);
		return errcode_enum::notice_unknown;
	}

	if (m_buy_flag == 0)
	{
		log_error("user[%lu] get prize but buy_flag is 0", m_role_id);
		return errcode_enum::notice_unknown;
	}

	//判断是否已经领取
	auto iter_f = m_prize_list.find(index);
	if (iter_f != m_prize_list.end())
	{
		log_error("user[%lu] prize[%u] has get",m_role_id, index );
		return errcode_enum::notice_unknown;
	}

	//判断是否已经达成条件	
	LevelFundTable *p_conf = GET_CONF(LevelFundTable, index);
	if (NULL == p_conf)
	{
		log_error("user[%lu] [%u] config is NULL", m_role_id, index );
		return errcode_enum::notice_unknown;
	}

	if (p_conf->level() > p_role->get_level())
	{
		log_error("user[%lu] get prize[%d], but cond not ok", m_role_id, index );
		return errcode_enum::notice_unknown;
	}

	bool ret = drop_manager_t::drop(p_role, p_conf->prize_id(), log_enum:: source_type_bussiness_level_fund , proto::common::drop_sys_type_level_fund, index, rcd);
	if (!ret)
	{
		log_error("user[%lu] [%u] level_fund drop faild", m_role_id, index);
		return errcode_enum::notice_unknown;
	}

	m_prize_list.insert(index);

	save_data();

	proto::client::gc_active_update_reply reply;
	reply.set_id(proto::common::BUSINESS_ACTIVE_TYPE::LEVEL_FUND);
	reply.set_red_num(get_red_num());
	p_role->send_msg_to_client(op_cmd::gc_active_update_reply, reply);

	if (is_end_show_time())
	{
		business_activity_manager_t::del_icon_info(p_role, proto::common::LEVEL_FUND);
	}

	return errcode_enum::error_ok;
}

uint32_t level_fund_t::get_level_fund_info( proto::client::gc_get_level_fund_info_reply &reply)
{
	role_ptr p_role = role_manager_t::find_role(m_role_id);
	if (NULL == p_role)
	{
		log_error("user[%lu] is NULL", m_role_id);
		return errcode_enum::notice_unknown;
	}
	
	auto p_conf1 = GET_CONF(Comprehensive, comprehensive_common::kaifujijin_1);
	uint32_t money_type = GET_COMPREHENSIVE_VALUE_1(p_conf1);
	uint32_t money      = GET_COMPREHENSIVE_VALUE_2(p_conf1);

	auto p_conf2 = GET_CONF(Comprehensive, comprehensive_common::kaifujijin_2);
	uint32_t rate = GET_COMPREHENSIVE_VALUE_1(p_conf2);
	uint32_t total_money = GET_COMPREHENSIVE_VALUE_2(p_conf2);
	uint32_t limit_prize = GET_COMPREHENSIVE_VALUE_3(p_conf2);

	reply.set_rate(rate);
	reply.set_money_type(money_type);
	reply.set_buy_prize(money);
	reply.set_buy_flag(m_buy_flag);
	reply.set_limit_prize(limit_prize);
	reply.set_total_money(total_money);

	std::map<uint32_t, LevelFundTable*> conf_map;
	GET_ALL_CONF(LevelFundTable, conf_map);

	for (auto it : conf_map)
	{
		LevelFundTable *p_conf = it.second;
		if ( NULL == p_conf )
			continue;

		proto::common::level_fund_info *p_new_info = reply.add_info_list();
		p_new_info->set_id(it.first);
		p_new_info->set_prize_state(get_prize_status(it.first, p_role->get_level() ) );
		p_new_info->set_level(p_conf->level());
		p_new_info->set_des(common::string_util_t::convert_to_utf8( p_conf->desc().c_str()) );
		p_new_info->set_show_prize(p_conf->show_prize());
	}

	return errcode_enum::error_ok;
}

uint32_t level_fund_t::get_prize_status(uint32_t index, uint32_t cur_level )
{
	if (m_buy_flag == 0)
		return proto::common::PRIZE_STATUS::PRIZE_STATUS_NONE;

	auto iter_f = m_prize_list.find(index);
	if (iter_f != m_prize_list.end())
		return proto::common::PRIZE_STATUS::PRIZE_STATUS_HAS;

	LevelFundTable *p_conf = GET_CONF(LevelFundTable, index);
	if (NULL == p_conf)
		return proto::common::PRIZE_STATUS::PRIZE_STATUS_NONE;

	//未达成条件
	if( p_conf->level() > cur_level )
		return proto::common::PRIZE_STATUS::PRIZE_STATUS_NONE;
	
	return proto::common::PRIZE_STATUS::PRIZE_STATUS_CAN;
}

bool level_fund_t::load_data(const proto::common::level_fund_save_info &data)
{
	m_buy_flag = data.buy_flag();

	m_prize_list.clear();
	for (int i = 0; i < data.id_size(); ++i)
	{
		m_prize_list.insert(data.id(i));
	}
	return true;
}

bool level_fund_t::peek_data(proto::common::level_fund_save_info &data)
{
	data.set_buy_flag( m_buy_flag );

	for (auto it : m_prize_list )
	{
		data.add_id(it);
	}
	return true;
}

bool level_fund_t::save_data()
{
	proto::common::level_fund_save_info data;
	peek_data( data );
	role_unify_save::add_task(m_role_id, m_key, data);
	return true;
}

//是否在活动中
bool level_fund_t::is_active()
{
	return ( is_get_all_prize() == false );
}

//是否领取了所有奖励
bool level_fund_t::is_get_all_prize()
{
	role_ptr p_role = role_manager_t::find_role(m_role_id);
	if (NULL == p_role)
	{
		log_error("user[%lu] is NULL", m_role_id);
		return errcode_enum::notice_unknown;
	}

	bool ret = true;
	std::map<uint32_t, LevelFundTable*> conf_map;
	GET_ALL_CONF(LevelFundTable, conf_map);

	for (auto it : conf_map)
	{
		LevelFundTable *p_conf = it.second;
		if (NULL == p_conf)
			continue;

		if( get_prize_status( it.first, p_role->get_level() ) != proto::common::PRIZE_STATUS_HAS )
		{
			ret = false;
			break;
		}
	}
	return ret;
}

//是否是结束时间了
bool level_fund_t::is_end_show_time()
{
	return is_get_all_prize();
}

//获取活动开始时间, 如果是0表示永不结束
uint32_t level_fund_t::get_active_start_time()
{
	return 0;
}

//获取活动结束时间,放在模块做，方便扩展
uint32_t level_fund_t::get_active_end_time()
{
	return 0;
}

//设置活动开始,本活动不需要重复开启，属于默认开启状态
void level_fund_t::set_active_start(uint32_t start_time)
{
	return;
}

//设置活动结束
void level_fund_t::set_active_stop()
{
	return;
}

//获取红点
uint32_t level_fund_t::get_red_num()
{
	role_ptr p_role = role_manager_t::find_role(m_role_id);
	if (NULL == p_role)
	{
		log_error("user[%lu] is NULL", m_role_id);
		return errcode_enum::notice_unknown;
	}

	uint32_t count = 0;

	std::map<uint32_t, LevelFundTable*> conf_map;
	GET_ALL_CONF(LevelFundTable, conf_map);

	for (auto it : conf_map)
	{
		LevelFundTable *p_conf = it.second;
		if (NULL == p_conf)
			continue;

		if (get_prize_status(it.first, p_role->get_level()) == proto::common::PRIZE_STATUS_CAN)
			count++;
	}
	return count;
}

uint32_t level_fund_t::buy_level_fund(proto::common::role_change_data *rcd)
{
	role_ptr p_role = role_manager_t::find_role(m_role_id);
	if (NULL == p_role)
	{
		log_error("user[%lu] is NULL", m_role_id);
		return errcode_enum::notice_unknown;
	}

	if (m_buy_flag > 0)
	{
		log_error("%lu buy level_fund , but has buy", m_role_id);
		return errcode_enum::notice_unknown;
	}

	auto p_conf = GET_CONF(Comprehensive, comprehensive_common::kaifujijin_1);
	uint32_t money_type = GET_COMPREHENSIVE_VALUE_1(p_conf);
	uint32_t money      = GET_COMPREHENSIVE_VALUE_2(p_conf);

	if ( !money_manager_t::use_money(p_role, proto::common::MONEY_TYPE(money_type), money, log_enum::source_type_bussiness_level_fund, 0, true, rcd) )
	{
		log_error("user[%lu] not enough!!!", m_role_id);
		return errcode_enum::notice_gold_money_not_enough;
	}

	m_buy_flag = 1;
	
	save_data();

	if (get_red_num() > 0)
		red_point_manager_t::red_point_notify(m_role_id, proto::common::RED_POINT_TYPE::RED_POINT_TYPE_FULI, 1);

	proto::client::gc_active_update_reply reply;
	reply.set_id(proto::common::BUSINESS_ACTIVE_TYPE::LEVEL_FUND);
	reply.set_red_num(get_red_num());
	p_role->send_msg_to_client(op_cmd::gc_active_update_reply, reply);

	return errcode_enum::error_ok;
}
