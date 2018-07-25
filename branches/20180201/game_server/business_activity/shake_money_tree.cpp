#include "shake_money_tree.hpp"
#include "tblh/MoneyTreeTable.tbls.h"
#include "tblh/MoneyTreeReward.tbls.h"
#include "tblh/MoneyTreeShake.tbls.h"
#include "cache_key.hpp"
#include "tblh/errcode_enum.hpp"
#include "role/role_unify_save.hpp"
#include "role/role_manager.hpp"
#include "config/config_manager.hpp"
#include "role/money_manager.hpp"
#include "item/item_manager.hpp"
#include "common/log_enum.hpp"

USING_NS_COMMON;

shake_money_tree_t::shake_money_tree_t(uint64_t uid):m_role_uid(uid)
{
	m_key = cache_key_t::create(m_role_uid, cache_name::shake_money_tree);
}

void shake_money_tree_t::load_data(const proto::common::money_tree_data& data)
{
	m_shake_num  = data.count();
	m_tree_level = data.level();

	if (m_tree_level == 0)
	{
		m_tree_level = 1;
		save_data();
	}
}

void shake_money_tree_t::peek_data(proto::common::money_tree_data &data)
{
	data.set_count(m_shake_num);
	data.set_level(m_tree_level);
}

void shake_money_tree_t::save_data()
{
	proto::common::money_tree_data p_data;
	peek_data(p_data);
	role_unify_save::add_task(m_key, p_data, false, false, m_role_uid);
}

uint32_t shake_money_tree_t::get_money_tree_info(proto::client::gc_get_money_tree_reply &msg)
{
	msg.set_level(m_tree_level);
	msg.set_count(m_shake_num);
	return errcode_enum::error_ok;
}

uint32_t shake_money_tree_t::shake_money_tree(proto::client::gc_shake_money_tree_reply &msg)
{
	auto p_role = role_manager_t::find_role(m_role_uid);
	if (NULL == p_role)
	{
		log_error("user[%lu] is NULL", m_role_uid);
		return false;
	}

	auto p_tree_conf = GET_CONF(MoneyTreeTable, m_tree_level);
	if (NULL == p_tree_conf)
	{
		log_error("user:[%lu] MoneyTreeTable level[%u] config not found", m_role_uid, m_tree_level);
		return errcode_enum::notice_unknown;
	}

	auto p_reward_conf = GET_CONF(MoneyTreeReward, p_role->get_level());
	if (NULL == p_reward_conf)
	{
		log_error("user:[%lu] MoneyTreeReward level[%u] config not found", m_role_uid, p_role->get_level());
		return errcode_enum::notice_unknown;
	}

	auto p_shake_conf = GET_CONF(MoneyTreeShake, m_shake_num + 1);
	if (NULL == p_shake_conf)
	{
		log_error("user:[%lu] p_reward_conf shake_num[%u] config not found", m_role_uid, m_shake_num + 1);
		return errcode_enum::notice_unknown;
	}

	//计算花费
	uint32_t cost_yb = p_shake_conf->get_cost_money(m_tree_level);
	if ( cost_yb != 0 && !money_manager_t::use_money(p_role, proto::common::MONEY_TYPE_YUANBAO, cost_yb, log_enum::source_type_bussiness_money_tree, 0, true, msg.mutable_rcd()) )
	{
		log_error("user:[%lu] shake_money_tree yb[%u] not enough", m_role_uid, cost_yb);
		return errcode_enum::notice_gold_money_not_enough;
	}

	//暴击等级
	uint32_t crit_type = get_crit_type( m_tree_level );
	uint32_t money_num = p_reward_conf->get_consum_coin(m_tree_level);
	uint32_t ex_money = p_tree_conf->exGet();
	uint32_t add_money = money_num + (money_num * ex_money) / 10000.0f;

	money_manager_t::add_money(p_role, proto::common::MONEY_TYPE::MONEY_TYPE_COPPER, add_money * crit_type, log_enum::source_type_bussiness_money_tree, 0, true,  msg.mutable_rcd());
	msg.set_crit_type(crit_type);

	m_shake_num += 1;
	save_data();

	return errcode_enum::error_ok;
}

uint32_t shake_money_tree_t::level_up_money_tree(proto::client::gc_levelup_money_tree_reply &msg)
{
	auto p_role = role_manager_t::find_role(m_role_uid);
	if (NULL == p_role)
	{
		log_error("user[%lu] is NULL", m_role_uid);
		return false;
	}

	uint32_t new_level = m_tree_level + 1;
	auto p_newtree_conf = GET_CONF(MoneyTreeTable, new_level);
	if (NULL == p_newtree_conf)
	{
		log_error("user:[%lu] MoneyTreeTable level[%u] config not found", m_role_uid, new_level);
		return errcode_enum::notice_unknown;
	}

	auto p_tree_conf = GET_CONF(MoneyTreeTable, m_tree_level);
	if (NULL == p_tree_conf)
	{
		log_error("user:[%lu] MoneyTreeTable level[%u] config not found", m_role_uid, m_tree_level);
		return errcode_enum::notice_unknown;
	}

	if ( errcode_enum::error_ok  != item_manager_t::remove_items( p_role, p_tree_conf->get_levelup_cost_map(), log_enum::source_type_bussiness_money_tree, new_level, msg.mutable_rcd()) )
	{
		log_error("user:[%lu] levelup tree[%u] , item not found", m_role_uid, new_level);
		return errcode_enum::notice_item_not_enough;
	}

	m_tree_level = new_level;

	save_data();

	return errcode_enum::error_ok;
}

uint32_t shake_money_tree_t::get_crit_type(uint32_t tree_level)
{
	auto p_tree_conf = GET_CONF(MoneyTreeTable, tree_level);
	if (NULL == p_tree_conf)
	{
		log_error("MoneyTreeTable level[%u] config not found", tree_level);
			return errcode_enum::notice_unknown;
	}

	uint32_t crit_type = 1;
	auto crit_map = p_tree_conf->get_crit_chance_map();
	rand_weighting(crit_map, crit_type);

	return crit_type;
}


bool shake_money_tree_t::rand_weighting(std::map<uint32_t, uint32_t> &weighting_map, uint32_t &key)
{
	if (weighting_map.empty()) 
	{
		return false;
	}

	uint32_t max_weight = 0;
	for (auto it : weighting_map)
	{
		max_weight += it.second;
	}

	uint32_t rand_value = random_util_t::randBetween(1, max_weight);
	uint32_t rand = 0;
	for (auto it : weighting_map)
	{
		rand += it.second;
		if ( rand >= rand_value ) 
		{
			key = it.first;
			break;
		}
	}
	return true;
}

void shake_money_tree_t::oneday() 
{
	m_shake_num = 0;
	save_data();
}

void shake_money_tree_t::init_newbie()
{
	m_tree_level = 1;
	save_data();
}


