#include "liandan_manager.hpp"
#include "main/game_server_fwd.hpp"
#include "main/game_server.hpp"
#include "cache_key.hpp"
#include "role/role_unify_save.hpp"
#include "tblh/Comprehensive.tbls.h"
#include "config/config_manager.hpp"
#include "common/utility.hpp"
#include "log/log_wrapper.hpp"
#include "item/item_manager.hpp"
#include "tblh/errcode_enum.hpp"
#include "item/drop_manager.hpp"
#include "role/money_manager.hpp"
#include "tblh/LiandanTable.tbls.h"
#include "tblh/LiandanluTable.tbls.h"
#include "role/role_manager.hpp"

liandan_manager_t::liandan_manager_t(uint64_t uid)
	: m_owner(uid)
{
	m_key = cache_key_t::create(m_owner, cache_name::liandan);
}

liandan_manager_t::~liandan_manager_t()
{

}

void liandan_manager_t::load_data(const proto::common::liandan_data& ld)
{
	for (int32_t i = 0; i < ld.liandan_info_size(); ++i)
	{
		liandan_ptr p_liandan = liandan_ptr(new liandan_t);
		p_liandan->load_data(ld.liandan_info(i));
		m_liandan_vec.push_back(p_liandan);
	}
}

void liandan_manager_t::peek_data(proto::common::liandan_data* p_data)
{
	if (NULL == p_data)
	{
		log_error("NULL == p_data");
		return;
	}
	for (uint32_t i = 0; i < m_liandan_vec.size(); ++i)
	{
		liandan_ptr p_liandan = m_liandan_vec[i];
		if (NULL == p_liandan)
		{
			log_error("NULL == p_liandan role[%lu]", m_owner);
			continue;
		}
		proto::common::liandan_single* p_single = p_data->add_liandan_info();
		p_liandan->peek_data(p_single);
	}
}

void liandan_manager_t::save_self()
{
	proto::common::liandan_data liandan_data;
	peek_data(&liandan_data);
	role_unify_save::add_task(m_owner, m_key, liandan_data);
}

uint32_t liandan_manager_t::unlock(proto::common::role_change_data* p_data)
{
	if (NULL == p_data)
	{
		log_error("NULL == p_data");
		return errcode_enum::notice_unknown;
	}
	auto p_role = role_manager_t::find_role(m_owner);
	if (NULL == p_role)
	{
		log_error("NULL == p_role[%lu]", m_owner);
		return errcode_enum::notice_role_null;
	}
	//获取下一槽位的编号
	uint32_t index = m_liandan_vec.size() + 1;
	auto p_conf = GET_CONF(LiandanluTable, index);
	if (NULL == p_conf)
	{
		log_error("role[%lu] NULL == p_conf LiandanluTable[%d]", m_owner, index);
		return errcode_enum::notice_unknown;
	}
	//检测消耗物品
	if (errcode_enum::error_ok != item_manager_t::remove_items(p_role, p_conf->material(), log_enum::source_type_liandan_unlock, index, p_data))
	{
		log_error("role[%lu] liandan[%d] unlock item not enough", m_owner, index);
		return errcode_enum::notice_item_not_enough;
	}
	proto::common::personal_info* p_info = p_data->mutable_per_info();
	p_role->get_personal_info(p_info);
	//增加新的槽位
	liandan_ptr p_liandan = liandan_ptr(new liandan_t);
	p_liandan->set_state(proto::common::liandan_state_free);
	m_liandan_vec.push_back(p_liandan);
	LIANDAN_LOG("role[%lu] unlock liandan[%d] success", m_owner, index);
	return errcode_enum::error_ok;
}

uint32_t liandan_manager_t::doing(uint32_t liandan_tid, uint32_t count, proto::common::role_change_data* p_data)
{
	if (NULL == p_data)
	{
		log_error("NULL == p_data");
		return errcode_enum::notice_unknown;
	}
	auto p_role = role_manager_t::find_role(m_owner);
	if (NULL == p_role)
	{
		log_error("NULL == p_role[%lu]", m_owner);
		return errcode_enum::notice_role_null;
	}
	auto p_conf = GET_CONF(LiandanTable, liandan_tid);
	if (NULL == p_conf)
	{
		log_error("role[%lu] NULL == p_conf LiandanTable[%d]", m_owner, liandan_tid);
		return errcode_enum::notice_unknown;
	}
	////检测目标是否已经在炼制
	//liandan_ptr p_liandan = get_liandan_by_tid(liandan_tid);
	//if (NULL != p_liandan)
	//{
	//	log_error("role[%lu] liandan[%d] already doing", m_owner, liandan_tid);
	//	return errcode_enum::notice_unknown;
	//}
	//检测是否还有空闲槽位
	liandan_ptr p_liandan = get_free_liandan();
	if (NULL == p_liandan)
	{
		log_error("role[%lu] liandan full", m_owner);
		return errcode_enum::exchange_tips7;
	}
	//检测炼丹数量上限
	if (p_conf->max_count() > 0 && count > p_conf->max_count())
	{
		log_error("role[%lu] liandan[%d] count[%d] max_count[%d]", m_owner, liandan_tid, count, p_conf->max_count());
		return errcode_enum::notice_unknown;
	}
	//检测消耗物品
	for (auto it : p_conf->money())
	{
		if (money_manager_t::get_money(p_role, (proto::common::MONEY_TYPE)it.first) < it.second*count)
		{
			log_error("role[%lu] liandan[%d] count[%d] money not enough", m_owner, liandan_tid, count);
			return errcode_enum::notice_copper_money_not_enough;
		}
	}
	
	std::map<uint32_t, uint32_t> item_id_count;
	for (auto it : p_conf->material())
	{
		item_id_count[it.first] = it.second * count;
	}
	if (errcode_enum::error_ok != item_manager_t::remove_items(p_role, item_id_count, log_enum::source_type_liandan_doing, liandan_tid, p_data))
	{
		log_error("role[%lu] liandan[%d] count[%d] item not enough", m_owner, liandan_tid, count);
		return errcode_enum::notice_item_not_enough;
	}
	for (auto it : p_conf->money())
	{
		money_manager_t::use_money(p_role, (proto::common::MONEY_TYPE)it.first, it.second*count, log_enum::source_type_liandan_doing, liandan_tid, true, p_data);
	}
	proto::common::personal_info* p_info = p_data->mutable_per_info();
	p_role->get_personal_info(p_info);
	//设置槽位状态
	p_liandan->set_state(proto::common::liandan_state_doing);
	p_liandan->set_liandan_tid(liandan_tid);
	p_liandan->set_count(count);
	uint32_t free_time = time_util_t::now_time() + p_conf->time() * count;
	p_liandan->set_free_time(free_time);
	LIANDAN_LOG("role[%lu] liandan[%d] count[%d] success", m_owner, liandan_tid, count);
	return errcode_enum::error_ok;
}

uint32_t liandan_manager_t::done(uint32_t liandan_index, proto::common::role_change_data* p_data)
{
	if (NULL == p_data)
	{
		log_error("NULL == p_data");
		return errcode_enum::notice_unknown;
	}
	auto p_role = role_manager_t::find_role(m_owner);
	if (NULL == p_role)
	{
		log_error("NULL == p_role[%lu]", m_owner);
		return errcode_enum::notice_role_null;
	}
	
	//检测目标是否在炼制
	liandan_ptr p_liandan = get_liandan_by_index(liandan_index);
	if (NULL == p_liandan)
	{
		log_error("role[%lu] liandan[%d] not exist", m_owner, liandan_index);
		return errcode_enum::notice_unknown;
	}
	//检测目标是否已经炼丹完成
	p_liandan->check_doing();
	if (p_liandan->get_state() != proto::common::liandan_state_doing)
	{
		log_error("role[%lu] liandan[%d] state[%d] error", m_owner, liandan_index, p_liandan->get_state());
		return errcode_enum::notice_unknown;
	}
	uint32_t liandan_tid = p_liandan->get_liandan_tid();
	auto p_conf = GET_CONF(LiandanTable, liandan_tid);
	if (NULL == p_conf)
	{
		log_error("role[%lu] NULL == p_conf LiandanTable[%d]", m_owner, liandan_tid);
		return errcode_enum::notice_unknown;
	}
	//计算并检测消耗货币
	uint32_t expired_time = p_liandan->get_free_time() - time_util_t::now_time();
	uint32_t interval_time = p_conf->done_cost().first;
	if (0 == interval_time)
	{
		log_error("role[%lu] 0 == interval_time LiandanTable[%d]", m_owner, liandan_tid);
		return errcode_enum::notice_unknown;
	}
	uint32_t cost_type = p_conf->done_cost().second;
	uint32_t cost_count = p_conf->done_cost().third;
	uint32_t interval = expired_time / interval_time;
	if (expired_time % interval_time != 0)
	{
		interval += 1;
	}
	cost_count *= interval;
	if (!money_manager_t::use_money(p_role, (proto::common::MONEY_TYPE)cost_type, cost_count, log_enum::source_type_liandan_done, liandan_tid))
	{
		log_error("role[%lu] liandan[%d] done money not enough", m_owner, liandan_tid);
		return errcode_enum::notice_gold_money_not_enough;
	}
	proto::common::personal_info* p_info = p_data->mutable_per_info();
	p_role->get_personal_info(p_info);
	//改变槽位状态
	p_liandan->set_state(proto::common::liandan_state_done);
	p_liandan->set_free_time(0);
	LIANDAN_LOG("role[%lu] liandan[%d:%d] done success", m_owner, liandan_index, liandan_tid);
	return errcode_enum::error_ok;
}

uint32_t liandan_manager_t::return_back(uint32_t liandan_index, proto::common::role_change_data* p_data)
{
	if (NULL == p_data)
	{
		log_error("NULL == p_data");
		return errcode_enum::notice_unknown;
	}
	auto p_role = role_manager_t::find_role(m_owner);
	if (NULL == p_role)
	{
		log_error("NULL == p_role[%lu]", m_owner);
		return errcode_enum::notice_role_null;
	}
	
	//检测目标是否在炼制
	liandan_ptr p_liandan = get_liandan_by_index(liandan_index);
	if (NULL == p_liandan)
	{
		log_error("role[%lu] liandan[%d] not exist", m_owner, liandan_index);
		return errcode_enum::notice_unknown;
	}
	//检测目标是否已经炼丹完成
	p_liandan->check_doing();
	if (p_liandan->get_state() != proto::common::liandan_state_done)
	{
		log_error("role[%lu] liandan[%d] state[%d] error", m_owner, liandan_index, p_liandan->get_state());
		return errcode_enum::notice_unknown;
	}
	uint32_t liandan_tid = p_liandan->get_liandan_tid();
	auto p_conf = GET_CONF(LiandanTable, liandan_tid);
	if (NULL == p_conf)
	{
		log_error("role[%lu] NULL == p_conf LiandanTable[%d]", m_owner, liandan_tid);
		return errcode_enum::notice_unknown;
	}
	//获取收益
	drop_manager_t::drop(p_role, p_conf->drop_id(), log_enum::source_type_liandan_return, proto::common::drop_sys_type_liandan_return, liandan_tid, p_data, true, p_liandan->get_count());
	proto::common::personal_info* p_info = p_data->mutable_per_info();
	p_role->get_personal_info(p_info);
	//改变槽位状态
	p_liandan->set_state(proto::common::liandan_state_free);
	p_liandan->set_free_time(0);
	p_liandan->set_liandan_tid(0);
	p_liandan->set_count(0);
	LIANDAN_LOG("role[%lu] liandan[%d:%d] return success", m_owner, liandan_index, liandan_tid);
	return errcode_enum::error_ok;
}

liandan_ptr liandan_manager_t::get_liandan_by_index(uint32_t liandan_index)
{
	if (liandan_index >= m_liandan_vec.size())
	{
		return NULL;
	}
	return m_liandan_vec[liandan_index];
}

liandan_ptr liandan_manager_t::get_free_liandan()
{
	for (uint32_t i = 0; i < m_liandan_vec.size(); ++i)
	{
		liandan_ptr p_liandan = m_liandan_vec[i];
		if (NULL == p_liandan)
		{
			log_error("NULL == p_liandan role[%lu]", m_owner);
			continue;
		}
		if (p_liandan->get_state() == proto::common::liandan_state_free)
		{
			return p_liandan;
		}
	}
	return NULL;
}
