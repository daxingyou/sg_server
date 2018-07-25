#include "shop_user_data.hpp"

#include "log.hpp"
#include "utility.hpp"
#include "config_mgr.h"
#include "time_manager.hpp"
#include "tblh/TimeTable.tbls.h"
#include "redis/shop_data_mgr.hpp"
#include "tblh/errcode_enum.hpp"

shop_user_data_t::shop_user_data_t(uint64_t role_uid)
	: m_role_uid(role_uid)
{

}

void shop_user_data_t::on_login_load()
{
	clear();

	shop_data_mgr_t::load_user_goods(m_role_uid, m_shop_user_map);
}

void shop_user_data_t::clear()
{
	m_shop_user_map.clear();
}

bool shop_user_data_t::get_shop_list_by_type(uint32_t type, proto::client::ec_shop_goods_reply& reply)
{
	uint32_t time_now = common::time_util_t::now_time();

	for (shop_user_map::iterator itr = m_shop_user_map.begin();
		itr != m_shop_user_map.end(); ++itr)
	{
		shop_user_ptr p_goods = itr->second;
		if (p_goods == NULL)
		{
			log_error("p_goods null error");
			continue;
		}

		//log_warn("shop goods[%u] num[%u] type[%u] time now[%u] last[%u]", p_goods->m_user_goods.goods_id(), p_goods->m_user_goods.buy_count(), p_goods->m_shoptype, time_now, p_goods->m_user_goods.last_refresh_time());

		// 把还在限购时间内的数据发给客户端
		if (p_goods->m_shoptype == type && p_goods->m_user_goods.last_refresh_time() >= time_now)
		{
			proto::common::user_goods_list* p_user_goods_list = reply.mutable_user_list();
			if (p_user_goods_list != NULL)
			{
				proto::common::user_goods* p_user_goods = p_user_goods_list->add_goods_list();
				if (p_user_goods != NULL)
				{
					p_user_goods->CopyFrom(p_goods->m_user_goods);
				}
			}
		}
	}

	return true;
}

shop_user_data_t::shop_user_ptr shop_user_data_t::get_presonal_limit_info(uint32_t id)
{
	shop_user_map::iterator itr = m_shop_user_map.find(id);
	if (itr != m_shop_user_map.end())
	{
		return itr->second;
	}

	return NULL;
}

uint32_t shop_user_data_t::add_shop_goods_num(uint32_t goods_id, uint32_t goods_num, ShopTable* p_shop_conf, proto::server::eg_give_user_goods_notify& notify)
{
	if (p_shop_conf == NULL)
	{
		log_error("user[%lu] p_shop_conf null error", m_role_uid);
		return common::errcode_enum::notice_unknown;
	}

	shop_user_ptr person_limit_data = get_presonal_limit_info(goods_id);

	uint32_t time_now = common::time_util_t::now_time();

	// 为null 说明还没这个记录，可以购买
	if (person_limit_data == NULL)
	{
		person_limit_data = shop_user_ptr(new shop_user_t());

		if (NULL == person_limit_data)
		{
			log_error("user[%lu] person_limit_data null error", m_role_uid);
			return common::errcode_enum::notice_unknown;
		}

		person_limit_data->m_shoptype = p_shop_conf->shop_type();	// 记录下类型方便查找
		person_limit_data->m_user_goods.set_goods_id(goods_id);
		person_limit_data->m_user_goods.set_last_refresh_time(0);
		person_limit_data->m_user_goods.set_buy_count(0);

		m_shop_user_map.insert(std::make_pair(goods_id, person_limit_data));
	}

	// 已经超过限购时间了，重新刷新
	if (time_now >= person_limit_data->m_user_goods.last_refresh_time())
	{
		uint32_t next_time = shop_manager_t::get_next_refresh_time((proto::common::shop_refresh_type)p_shop_conf->refresh_type());
		// 发生错误
		if (next_time == 0)
		{
			log_error("role[%lu] goods[%u] get_next_refresh_time time 0 error", m_role_uid, goods_id);
			return common::errcode_enum::notice_unknown;
		}

		person_limit_data->m_user_goods.set_buy_count(0);
		person_limit_data->m_user_goods.set_last_refresh_time(next_time);
	}

	uint32_t curr_count = person_limit_data->m_user_goods.buy_count();

	if (curr_count >= p_shop_conf->presonal_limit())
	{
		SHOP_LOG("user[%lu] goods[%u] is already max presonal limit number", m_role_uid, goods_id);
		return common::errcode_enum::notice_shop_error6;
	}

	if (curr_count + goods_num > p_shop_conf->presonal_limit())
	{
		SHOP_LOG("user[%lu] goods[%u] curr_num + user_num > presonal_limit", m_role_uid, goods_id);
		return common::errcode_enum::notice_shop_error11;
	}

	person_limit_data->m_user_goods.set_buy_count(curr_count + goods_num);

	shop_data_mgr_t::save_user_goods(m_role_uid, goods_id, person_limit_data);

	// 赋值告诉客户端当前限购数量
	proto::common::user_goods* user_goods_data = notify.mutable_user_goods_data();
	if (user_goods_data != NULL)
	{
		user_goods_data->CopyFrom(person_limit_data->m_user_goods);
	}

	return common::errcode_enum::error_ok;
}

