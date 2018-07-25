#include "recharge.hpp"
#include "main/game_server_fwd.hpp"
#include "main/game_server.hpp"
#include "cache_key.hpp"
#include "role/role_unify_save.hpp"
#include "tblh/Comprehensive.tbls.h"
#include "config/config_manager.hpp"
#include "tblh/VIP.tbls.h"
#include <bitset>
#include "role/role_manager.hpp"
#include "log/log_wrapper.hpp"
#include "tblh/errcode_enum.hpp"
#include "role/money_manager.hpp"
#include "item/drop_manager.hpp"
#include "tblh/Recharge.tbls.h"
#include "tblh/RechargeCheap.tbls.h"
#include "global_id.hpp"

USING_NS_COMMON;

recharge_t::recharge_t(uint64_t uid)
	: m_owner(uid)
{
	m_key = cache_key_t::create(m_owner, cache_name::recharge);
}

recharge_t::~recharge_t()
{

}

void recharge_t::load_data(const proto::common::recharge_data& rd)
{
	m_total_cny = rd.total_cny();
	m_vip_level = rd.vip_level();
	m_recharge_count = rd.recharge_flag();
	m_recharge_reward_flag = rd.recharge_reward_flag();
	for (int32_t i = 0; i < rd.cheap_day_size(); ++i)
	{
		uint32_t cheap_id = rd.cheap_day(i).id();
		uint32_t count = rd.cheap_day(i).count();
		m_cheap_count[cheap_id] = count;
	}
}

void recharge_t::one_day()
{
	m_cheap_count.clear();
	save_self();
}


uint32_t recharge_t::get_red_num()
{
	uint32_t red_num = 0;
	std::map<uint32_t, RechargeCheap*> conf_map;
	GET_ALL_CONF(RechargeCheap, conf_map);
	for (auto it : conf_map)
	{
		RechargeCheap *p_conf = it.second;
		if (NULL == p_conf)
			continue;
		if (get_cheap_count(p_conf->id()) < p_conf->limit())
		{
			++red_num;
		}
	}
	return red_num;
}

void recharge_t::peek_data(proto::common::recharge_data* p_data)
{
	if (NULL == p_data)
	{
		log_error("NULL == p_data");
		return;
	}
	p_data->set_recharge_flag(m_recharge_count);
	p_data->set_recharge_reward_flag(m_recharge_reward_flag);
	p_data->set_total_cny(m_total_cny);
	p_data->set_vip_level(m_vip_level);
	for (auto it : m_cheap_count)
	{
		proto::common::cheap_day_single* p_single = p_data->add_cheap_day();
		p_single->set_id(it.first);
		p_single->set_count(it.second);
	}
}

void recharge_t::save_self()
{
	proto::common::recharge_data rd;
	peek_data(&rd);
	role_unify_save::add_task(m_owner, m_key, rd);
}


void recharge_t::add_total_cny(uint32_t val, proto::common::role_change_data* p_data)
{
	if (m_total_cny + val > std::numeric_limits<unsigned int>::max())
	{
		m_total_cny = std::numeric_limits<unsigned int>::max();
	}
	else
	{
		m_total_cny += val;
	}
	std::map<uint32_t, VIP*> confs;
	GET_ALL_CONF(VIP, confs);
	for (auto& it : confs)
	{
		VIP* p_config = it.second;
		if (NULL == p_config)
		{
			log_error("NULL == p_config VIP table");
			return;
		}
		if (m_total_cny > p_config->CNY())
			continue;
		if (0 == it.first)
			return;
		if (m_total_cny == p_config->CNY())
		{
			set_vip_level(it.first, p_data);
		}
		else if (1 < it.first)
		{
			set_vip_level(it.first - 1, p_data);
		}
		break;
	}
}

void recharge_t::set_vip_level(uint32_t vip_level, proto::common::role_change_data* p_data)
{
	if (NULL == p_data)
	{
		log_error("NULL == p_data, role_uid[%lu]", m_owner);
		return;
	}
	role_ptr p_role = role_manager_t::find_role(m_owner);
	if (NULL == p_role)
	{
		log_error("NULL == p_role, role_uid[%lu]", m_owner);
		return;
	}
	if (m_vip_level == vip_level)
		return;

	uint32_t old_vip_level = m_vip_level;
	m_vip_level = vip_level;
	proto::common::personal_info* p_personal_info = p_data->mutable_per_info();
	p_role->get_personal_info(p_personal_info);
	save_self();
	ROLE_LOG("role[%lu] set_vip_level old_vip_level[%d] new_vip_level[%d]", m_owner, old_vip_level, m_vip_level);
}

bool recharge_t::is_first_recharge()
{
	if (0 == m_recharge_count)
	{
		return true;
	}

	return false;
}

void recharge_t::add_recharge_count()
{
	++m_recharge_count;
}


bool recharge_t::already_get_recharge_gift(uint32_t bit /* = 0 */)
{
	std::bitset<MAX_BIT> bits(m_recharge_reward_flag);
	bool result = bits.test(bit);
	return result;
}

void recharge_t::set_recharge_reward_flag(uint32_t bit /* = 0 */)
{
	if (bit >= MAX_BIT)
	{
		log_error("bit more than MAX_BIT [%d]", bit);
		return;
	}
	std::bitset<MAX_BIT> bits(m_recharge_reward_flag);

	if (bits.test(bit))
	{
		return;
	}
	m_recharge_reward_flag = bits.set(bit).to_ulong();
}

uint32_t recharge_t::get_recharge_gift(proto::common::role_change_data* p_data)
{
	uint32_t reply_code = errcode_enum::error_ok;
	do
	{
		if (NULL == p_data)
		{
			log_error("NULL == p_data");
			reply_code = errcode_enum::notice_unknown;
			break;
		}
		role_ptr p_role = role_manager_t::find_role(m_owner);
		if (NULL == p_role)
		{
			log_error("NULL == p_role, role_uid[%lu]", m_owner);
			reply_code = errcode_enum::notice_unknown;
			break;
		}
		if (already_get_recharge_gift())
		{
			reply_code = errcode_enum::first_recharge_get;
			break;
		}

		///暂时屏蔽********************************
		/*if (0 == get_recharge_count())
		{
		reply_code = errcode_enum::first_recharge_notyet;
		break;
		}*/

		auto gift = GET_CONF(Comprehensive, comprehensive_common::first_recharge);
		uint32_t gift_id = GET_COMPREHENSIVE_VALUE_1(gift);
		bool result = drop_manager_t::drop(p_role, gift_id, log_enum::source_type_recharge_gift, proto::common::drop_sys_type_recharge_gift, 0, p_data);
		if (!result)
		{
			reply_code = errcode_enum::notice_unknown;
			break;
		}
		set_recharge_reward_flag();
		save_self();
		p_role->open_function(proto::common::function_code_type_rechargetotal);
		p_role->close_function(proto::common::function_code_type_firstchange);
	} while (false);
	
	proto::common::recharge_data* p_recharge = p_data->mutable_recharge();
	p_recharge->set_recharge_flag(get_recharge_count());
	p_recharge->set_recharge_reward_flag(get_recharge_reward_flag());
	p_recharge->set_vip_level(get_vip_level());
	p_recharge->set_total_cny(get_total_cny());
	return reply_code;
}

uint32_t recharge_t::rmb_recharge(uint32_t tid, proto::common::role_change_data* p_data /*= NULL*/)
{
	uint32_t errcode = errcode_enum::error_ok;	///错误码
												//errcode = errcode_enum::function_not_open;
												//return errcode;
	do
	{
		role_ptr p_role = role_manager_t::find_role(m_owner);
		if (NULL == p_role)
		{
			log_error("NULL == p_role role_uid[%lu]", m_owner);
			errcode = errcode_enum::notice_unknown;
			break;
		}
		
		if (NULL == p_data)
		{
			log_error("NULL == p_data role_uid[%lu]", m_owner);
			errcode = errcode_enum::notice_unknown;
			break;
		}

		Recharge* p_config = GET_CONF(Recharge, tid);
		if (NULL == p_config)
		{
			log_error("NULL == p_config tid[%d]", tid);
			errcode = errcode_enum::notice_config_error;
			break;
		}
		uint32_t add_val = p_config->money();
		if (is_first_recharge())
		{
			add_val += p_config->gift_money();
		}
		add_recharge_count();
		money_manager_t::add_money(p_role, proto::common::MONEY_TYPE_LIFE_STONE, add_val, log_enum::source_type_recharge_success, tid, true, p_data);
		add_total_cny(p_config->CNY(), p_data);
		save_self();
		//累计充值红点提示
		p_role->get_business_act_mgr().m_recharge_total.on_login();

		proto::common::recharge_data* p_recharge = p_data->mutable_recharge();
		p_recharge->set_recharge_flag(get_recharge_count());
		p_recharge->set_recharge_reward_flag(get_recharge_reward_flag());

		//订单号 暂时自己创建 后续可能会用渠道给的订单号存储
		uint64_t order_id = g_id_allocator.gen_new_id(ID_ALLOCATOR_TYPE_RECHARGE);
		std::string order_id_str = string_util_t::uint64_to_string(order_id);

		//渠道ID 暂时写为0
		uint32_t channel_id = 0;

		log_wrapper_t::send_recharge_log(m_owner, order_id_str, tid, p_config->CNY(), channel_id);

	} while (false);
	return errcode;
}

void recharge_t::get_recharge_cheap(proto::client::gc_recharge_cheap_reply& reply)
{
	std::map<uint32_t, RechargeCheap*> conf_map;
	GET_ALL_CONF(RechargeCheap, conf_map);
	for (auto it : conf_map)
	{
		RechargeCheap *p_conf = it.second;
		if (NULL == p_conf)
			continue;
		proto::common::cheap_day_client_single* p_single = reply.add_cheap_day();
		p_single->set_id(p_conf->id());
		p_single->set_pay(p_conf->count());
		p_single->set_count(get_cheap_count(p_conf->id()));
		p_single->set_name(p_conf->name());
		p_single->set_desc(p_conf->desc());
		p_single->set_limit(p_conf->limit());
		p_single->set_display(p_conf->display());
		p_single->set_pic(p_conf->pic());
	}
}

uint32_t recharge_t::cheap_buy(uint32_t cheap_id, proto::client::gc_recharge_cheap_buy_reply& reply)
{
	role_ptr p_role = role_manager_t::find_role(m_owner);
	if (NULL == p_role)
	{
		log_error("NULL == p_role[%lu]", m_owner);
		return errcode_enum::notice_unknown;
	}
	RechargeCheap* p_conf = GET_CONF(RechargeCheap, cheap_id);
	if (NULL == p_conf)
	{
		log_error("NULL == p_conf RechargeCheap[%d]", cheap_id);
		return errcode_enum::notice_unknown;
	}
	if (get_cheap_count(cheap_id) >= p_conf->limit())
	{
		log_error("role[%lu] cheap[%d] limit", m_owner, cheap_id);
		return errcode_enum::notice_unknown;
	}
	m_cheap_count[cheap_id] += 1;
	drop_manager_t::drop(p_role, p_conf->drop(), log_enum::source_type_recharge_cheap, proto::common::drop_sys_type_recharge_gift, cheap_id, reply.mutable_rcd());

	std::map<uint32_t, RechargeCheap*> conf_map;
	GET_ALL_CONF(RechargeCheap, conf_map);
	for (auto it : conf_map)
	{
		RechargeCheap *p_conf = it.second;
		if (NULL == p_conf)
			continue;
		proto::common::cheap_day_client_single* p_single = reply.add_cheap_day();
		p_single->set_id(p_conf->id());
		p_single->set_pay(p_conf->count());
		p_single->set_count(get_cheap_count(p_conf->id()));
		p_single->set_name(p_conf->name());
		p_single->set_desc(p_conf->desc());
		p_single->set_limit(p_conf->limit());
		p_single->set_display(p_conf->display());
	}
	return errcode_enum::error_ok;
}

uint32_t recharge_t::get_cheap_count(uint32_t cheap_id)
{
	if (m_cheap_count.find(cheap_id) == m_cheap_count.end())
	{
		return 0;
	}
	return m_cheap_count[cheap_id];
}
