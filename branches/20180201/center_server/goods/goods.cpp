#include "goods.hpp"
#include "log.hpp"
#include "utility.hpp"
#include "main/center_server.hpp"
#include "main/center_server_fwd.hpp"
#include "goods_manager.hpp"
#include "common/redis_client.hpp"
USING_NS_COMMON;

void goods_t::peek_data(proto::common::goods_single* p_info)
{
	if (NULL == p_info)
	{
		log_error("NULL == p_info");
		return;
	}
	p_info->set_uid(string_util_t::uint64_to_string(m_uid));
	p_info->set_seller_uid(string_util_t::uint64_to_string(m_seller_uid));
	p_info->set_seller_name(m_seller_name);
	p_info->set_item_tid(m_item_tid);
	p_info->set_item_num(m_item_num);
	proto::common::equip_single* p_equip = p_info->mutable_equip_info();
	peek_equip_data(p_equip);
	p_info->set_rarity(m_rarity);
	p_info->set_item_price(m_item_price);
	p_info->set_selled_num(m_selled_num);
	p_info->set_profit(m_profit);
	p_info->set_state(m_state);
	p_info->set_expired_time(m_expired_time);
	p_info->set_heat(m_heat);
}

void goods_t::peek_equip_data(proto::common::equip_single* p_equip)
{
	if (NULL == p_equip)
	{
		log_error("NULL == p_equip");
		return;
	}
	for (auto it : m_attrs)
	{
		proto::common::equip_attr* p_equip_attr = p_equip->add_attr();
		p_equip_attr->set_type(it.first);
		p_equip_attr->set_value(it.second);
	}
	for (auto exattr : m_exattrs)
	{
		p_equip->add_exattr(exattr);
	}
	p_equip->set_score(m_score);
	p_equip->set_name(m_name);
	p_equip->set_special(m_special);
	p_equip->set_strengthen_level(m_strengthen_level);
	p_equip->set_strmaster_level(m_strmaster_level);
	p_equip->set_remake_material(m_remake_material);
	p_equip->set_lock_material(m_lock_material);
	p_equip->set_strengthen_material(m_strengthen_material);
	p_equip->set_strengthen_material_ex(m_strengthen_material_ex);
	p_equip->set_strengthen_money(m_strengthen_money);
}

void goods_t::start_timer()
{
	uint32_t now_time = common::time_util_t::now_time();
	if (now_time >= get_expired_time())
	{
		return;
	}
	if (m_state != proto::common::goods_state_selling && m_state != proto::common::goods_state_public)
	{
		return;
	}
	if (NULL == m_timer)
	{
		m_timer.reset(new boost::asio::deadline_timer(env::server->get_ios()));
	}
	m_timer->cancel();
	m_timer->expires_from_now(boost::posix_time::seconds(get_expired_time() - now_time));
	m_timer->async_wait(boost::bind(&goods_manager_t::goods_timeout, get_uid(), boost::asio::placeholders::error));

}

void goods_t::load_equip_data(const proto::common::equip_single& es)
{
	for (int32_t i = 0; i < es.attr_size(); ++i)
	{
		const proto::common::equip_attr& ea = es.attr(i);
		uint32_t attr_type = ea.type();
		uint32_t attr_value = ea.value();
		m_attrs.insert(std::make_pair(attr_type, attr_value));
	}
	for (int32_t i = 0; i < es.exattr_size(); ++i)
	{
		m_exattrs.push_back(es.exattr(i));
	}
	m_score = es.score();
	m_name = es.name();
	m_special = es.special();
	m_strengthen_level = es.strengthen_level();
	m_strmaster_level = es.strmaster_level();
	m_remake_material = es.remake_material();
	m_lock_material = es.lock_material();
	m_strengthen_material = es.strengthen_material();
	m_strengthen_material_ex = es.strengthen_material_ex();
	m_strengthen_money = es.strengthen_money();
}

void goods_t::save_self()
{
	redis_client_t::set_uint64("goods", "seller_uid", m_uid, m_seller_uid);
	redis_client_t::set_string("goods", "seller_name", m_uid, m_seller_name);
	redis_client_t::set_uint32("goods", "item_tid", m_uid, m_item_tid);
	redis_client_t::set_uint32("goods", "item_num", m_uid, m_item_num);
	redis_client_t::set_uint32("goods", "rarity", m_uid, m_rarity);
	redis_client_t::set_uint32("goods", "item_price", m_uid, m_item_price);
	redis_client_t::set_uint32("goods", "selled_num", m_uid, m_selled_num);
	redis_client_t::set_uint32("goods", "profit", m_uid, m_profit);
	redis_client_t::set_uint32("goods", "state", m_uid, m_state);
	redis_client_t::set_uint32("goods", "expired_time", m_uid, m_expired_time);
	redis_client_t::set_uint32("goods", "heat", m_uid, m_heat);
	proto::common::equip_single es;
	peek_equip_data(&es);
	redis_client_t::set_protobuf("goods", "equip_info", m_uid, es);
}

void goods_t::stop_timer()
{
	if (NULL != m_timer)
	{
		m_timer->cancel();
	}
}


