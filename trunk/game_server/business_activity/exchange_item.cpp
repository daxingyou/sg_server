#include "exchange_item.hpp"
#include "config/config_manager.hpp"
#include "role/role.hpp"
#include "role/role_manager.hpp"
#include "tblh/errcode_enum.hpp"
#include "cache_key.hpp"
#include "item/drop_manager.hpp"
#include "role/role_unify_save.hpp"
#include "tblh/Comprehensive.tbls.h"
#include "role/money_manager.hpp"
#include "tblh/BAExchangeItem.tbls.h"
#include "item/item_manager.hpp"
#include "common/time_manager.hpp"
#include "red_point/red_point_manager.hpp"

exchange_item_t::exchange_item_t(uint64_t role_id) : m_role_uid( role_id ), m_start_time(0)
{
	m_key = cache_key_t::create(m_role_uid, cache_name::exchange_item);
}

uint32_t exchange_item_t::do_exchange_item(uint32_t id, proto::common::role_change_data *rcd )
{
	role_ptr p_role = role_manager_t::find_role(m_role_uid);
	if (NULL == p_role)
	{
		log_error("[%lu] p_role is NULL", m_role_uid);
		return errcode_enum::notice_unknown;
	}

	BAExchangeItem *p_conf = GET_CONF( BAExchangeItem, id );
	if (NULL == p_conf)
	{
		log_error("[%lu] BAExchangeItme Confg[%u] is NULL", m_role_uid, id );
		return errcode_enum::notice_unknown;
	}

	if (get_cur_num( id ) >= p_conf->max_num())
	{
		log_error("[%lu] cur_num[%u] >= max_num[%u] ", m_role_uid, get_cur_num( id ), p_conf->max_num());
		return errcode_enum::notice_unknown;
	}
		
	if (errcode_enum::error_ok != item_manager_t::remove_items(p_role, p_conf->get_need_item(), log_enum::source_type_bussiness_exchang_item, id, rcd))
	{
		log_error("[%lu] exchange item[%u] , need_item is not enought ", m_role_uid, id );
		return errcode_enum::notice_unknown;
	}

	if (errcode_enum::error_ok != item_manager_t::add_items(p_role, p_conf->get_make_item(), log_enum::source_type_bussiness_exchang_item, id, rcd))
	{
		log_error("[%lu] exchange item[%u] , add_make_item Faild", m_role_uid, id );
		return errcode_enum::notice_unknown;
	}

	m_exchange_map[id] += 1;
	
	save_data();

	proto::client::gc_active_update_reply reply;
	reply.set_id(proto::common::BUSINESS_ACTIVE_TYPE::EXCHANGE_ITEM);
	reply.set_red_num(get_red_num());
	p_role->send_msg_to_client(op_cmd::gc_active_update_reply, reply);
	
	return errcode_enum::error_ok;
}

uint32_t exchange_item_t::get_exchange_info( proto::client::gc_get_exchange_info_reply &reply)
{
	std::map<uint32_t, BAExchangeItem*> conf_map;
	GET_ALL_CONF(BAExchangeItem, conf_map);

	uint32_t now = time_util_t::now_time();
	uint32_t time_left = get_active_end_time() > now ? ( get_active_end_time() - now ) : 0;
	reply.set_time_left( time_left );

	for ( auto it : conf_map )
	{
		BAExchangeItem *p_conf = it.second;
		if (NULL == p_conf)
			continue;

		proto::common::exchange_item_data *p_data = reply.add_data();
		p_data->set_id(it.first);
		p_data->set_cur_num(get_cur_num(it.first));
		p_data->set_max_num(p_conf->max_num());
		p_data->set_make_item(p_conf->make_item());
		p_data->set_need_item(p_conf->need_item());
	}
	return errcode_enum::error_ok;
}

uint32_t exchange_item_t::on_login()
{
	//判断活动是否重开
	BusinessActiveTable *p_conf = GET_CONF(BusinessActiveTable, proto::common::BUSINESS_ACTIVE_TYPE::EXCHANGE_ITEM);
	if (NULL == p_conf)
		return false;
	
	if (time_manager_t::is_in_time(p_conf->time_id()))
	{
		uint32_t start_time = time_manager_t::get_timer_start_time(p_conf->time_id());
		if (m_start_time != start_time)
			set_active_start(start_time);
	}

	if (get_red_num() > 0)
	{
		red_point_manager_t::red_point_notify(m_role_uid, proto::common::RED_POINT_TYPE::RED_POINT_TYPE_FULI, 1);
	}
	return 0;
}

//是否在活动中
bool exchange_item_t::is_active()
{
	BusinessActiveTable *p_conf = GET_CONF(BusinessActiveTable, proto::common::BUSINESS_ACTIVE_TYPE::EXCHANGE_ITEM);
	if (NULL == p_conf)
		return false;

	//新增如果时间ID为0 表示永久开放
	if (p_conf->time_id() == 0) {
		return true;
	}

	return time_manager_t::is_in_time(p_conf->time_id());
}

//是否领取了所有奖励
bool exchange_item_t::is_get_all_prize()
{
	return true;
}

//是否是结束时间了
bool exchange_item_t::is_end_show_time()
{
	role_ptr p_role = role_manager_t::find_role(m_role_uid);
	if (NULL == p_role)
	{
		log_error("[%lu] p_role is NULL", m_role_uid);
		return true;
	}

	BusinessActiveTable *p_conf = GET_CONF(BusinessActiveTable, proto::common::BUSINESS_ACTIVE_TYPE::EXCHANGE_ITEM);
	if (NULL == p_conf)
		return true;

	return business_activity_manager_t::check_end_cond(p_role, p_conf, this) == false ;
}

//获取活动开始时间, 如果是0表示未参加过，也不需要显示
uint32_t exchange_item_t::get_active_start_time()
{
	BusinessActiveTable *p_conf = GET_CONF(BusinessActiveTable, proto::common::BUSINESS_ACTIVE_TYPE::EXCHANGE_ITEM);
	if (NULL == p_conf)
		return false;

	return time_manager_t::get_timer_start_time(p_conf->time_id());
}

//获取活动结束时间,放在模块做，方便扩展
uint32_t exchange_item_t::get_active_end_time()
{
	BusinessActiveTable *p_conf = GET_CONF(BusinessActiveTable, proto::common::BUSINESS_ACTIVE_TYPE::EXCHANGE_ITEM);
	if (NULL == p_conf)
		return false;

	return time_manager_t::get_timer_end_time(p_conf->time_id());
}

//设置活动开始
void exchange_item_t::set_active_start( uint32_t start_time )
{
	role_ptr p_role = role_manager_t::find_role(m_role_uid);
	if (NULL == p_role)
	{
		log_error("[%lu] p_role is NULL", m_role_uid);
		return;
	}

	if (m_start_time != start_time)
	{
		m_start_time = start_time;
		m_exchange_map.clear();
		save_data();
	}
	business_activity_manager_t::add_icon_info(p_role, proto::common::BUSINESS_ACTIVE_TYPE::EXCHANGE_ITEM, get_red_num(), start_time, 0 );
}

//设置活动结束
void exchange_item_t::set_active_stop()
{
	role_ptr p_role = role_manager_t::find_role(m_role_uid);
	if (NULL == p_role)
	{
		log_error("[%lu] p_role is NULL", m_role_uid);
		return;
	}

	if (is_end_show_time())
	{
		business_activity_manager_t::del_icon_info(p_role, proto::common::BUSINESS_ACTIVE_TYPE::EXCHANGE_ITEM);
	}
}

//获取红点
uint32_t exchange_item_t::get_red_num()
{
	role_ptr p_role = role_manager_t::find_role(m_role_uid);
	if (NULL == p_role)
	{
		log_error("[%lu] p_role is NULL", m_role_uid);
		return errcode_enum::notice_unknown;
	}

	uint32_t count = 0;
	std::map<uint32_t, BAExchangeItem*> conf_map;
	GET_ALL_CONF(BAExchangeItem, conf_map);
	for (auto it : conf_map)
	{
		BAExchangeItem *p_conf = it.second;
		if (NULL == p_conf)
			continue;

		if (item_manager_t::has_items(p_role, p_conf->get_need_item()))
			count += 1;
	}
	return count;
}

bool exchange_item_t::save_data()
{
	proto::common::exchange_item_save_data data;
	peek_data(data);
	role_unify_save::add_task(m_role_uid, m_key, data);
	return true;
}

bool exchange_item_t::peek_data(proto::common::exchange_item_save_data &data)
{	
	data.set_start_time(m_start_time);
	for (auto it : m_exchange_map)
	{
		proto::common::exchange_item_save_info *p_info = data.add_info();
		p_info->set_id(it.first);
		p_info->set_cur_num(it.second);
	}
	return true;
}

bool exchange_item_t::load_data(const proto::common::exchange_item_save_data &data)
{
	m_exchange_map.clear();
	m_start_time = data.start_time();
	for (int i = 0; i < data.info_size(); ++i)
	{
		const proto::common::exchange_item_save_info &p_info = data.info(i);
		uint32_t id = p_info.id();
		uint32_t num = p_info.cur_num();
		m_exchange_map.insert(std::make_pair(id, num));
	}
	return true;
}

uint32_t exchange_item_t::get_cur_num(uint32_t id)
{
	auto iter_f = m_exchange_map.find(id);
	if (iter_f == m_exchange_map.end())
		return 0;
	
	return m_exchange_map[id];
}


