#include "role_random_shop.hpp"
#include "cache_key.hpp"
#include "log.hpp"
#include "role/role_unify_save.hpp"
#include "config/config_manager.hpp"
#include "time_manager.hpp"
#include "tblh/ShopItemTable.tbls.h"
#include "item/item_manager.hpp"
#include "role/money_manager.hpp"
#include "log/log_wrapper.hpp"
#include "role/role.hpp"
#include "tblh/errcode_enum.hpp"
#include "log/log_wrapper.hpp"
#include "achieve/achieve_common.hpp"

role_random_shop_t::role_random_shop_t(uint64_t uid)
	: m_role_uid(uid)
{
	m_key = cache_key_t::create(m_role_uid, cache_name::random_shop);
}

role_random_shop_t::~role_random_shop_t()
{

}

void role_random_shop_t::load_data(const proto::common::role_random_shop_data& data)
{
	m_random_shop_map.clear();

	for (int32_t i = 0; i < data.datas_size(); ++i)
	{
		m_random_shop_map[data.datas(i).shop_id()] = data.datas(i);
	}
}

void role_random_shop_t::save_data(const proto::common::random_shop_data& data)
{
	role_unify_save::add_task(m_key, data, false, false, m_role_uid);
}

void role_random_shop_t::get_random_shop_info(const role_ptr& p_role, uint32_t shop_id, proto::client::gc_random_shop_info_reply& reply)
{
	if (NULL == p_role)
	{
		log_error("p_role null error");
		reply.set_reply_code(common::errcode_enum::notice_unknown);
		return;
	}

	RandomShopTable* p_conf = GET_CONF(RandomShopTable, shop_id);
	if (NULL == p_conf)
	{
		log_error("role[%lu] random_shop[%u] not find", m_role_uid, shop_id);
		reply.set_reply_code(common::errcode_enum::random_shop_error_not_find);
		return;
	}

	random_shop_map::iterator itr = m_random_shop_map.find(shop_id);
	if (itr == m_random_shop_map.end())
	{
		proto::common::random_shop_data new_shop_data;
		uint32_t back_code = refresh_random_shop(p_role, p_conf, &new_shop_data);

		if (back_code != common::errcode_enum::error_ok)
		{
			log_error("role[%u] create random_shop_data[%u] error", m_role_uid, shop_id);
			reply.set_reply_code(back_code);
			return;
		}

		std::pair<random_shop_map::iterator, bool> res = m_random_shop_map.insert(std::make_pair(shop_id, new_shop_data));
		if (!res.second)
		{
			log_error("role[%u] create random_shop_data[%u] error, it is exsit", m_role_uid, shop_id);
			reply.set_reply_code(common::errcode_enum::notice_unknown);
			return;
		}

		log_wrapper_t::send_random_shop_log(p_role->get_uid(), shop_id, 0, 0, 0, 0, 0, 0, 0, log_enum::source_type_random_shop_time_refresh);

		itr = res.first;
	}

	if (itr == m_random_shop_map.end())
	{
		log_error("role[%u] get random_shop_data error", m_role_uid);
		reply.set_reply_code(common::errcode_enum::notice_unknown);
		return;
	}

	if (common::time_util_t::now_time() > itr->second.next_refresh_time())
	{
		uint32_t back_code = refresh_random_shop(p_role, p_conf, &itr->second);

		if (back_code != common::errcode_enum::error_ok)
		{
			log_error("role[%u] refresh random_shop_data[%u] error", m_role_uid, shop_id);
			reply.set_reply_code(back_code);
			return;
		}

		log_wrapper_t::send_random_shop_log(p_role->get_uid(), shop_id, 0, 0, 0, 0, 0, 0, 0, log_enum::source_type_random_shop_time_refresh);
	}

	proto::common::random_shop_data* p_data = reply.mutable_info();
	if (NULL != p_data)
	{
		p_data->CopyFrom(itr->second);
	}

	reply.set_reply_code(common::errcode_enum::error_ok);
}

void role_random_shop_t::buy_random_shop(const role_ptr& p_role, uint32_t shop_id, uint32_t index, proto::client::gc_random_shop_buy_reply& reply)
{
	if (NULL == p_role)
	{
		log_error("p_role null error");
		reply.set_reply_code(common::errcode_enum::notice_unknown);
		return;
	}

	random_shop_map::iterator itr = m_random_shop_map.find(shop_id);
	if (itr == m_random_shop_map.end())
	{
		log_error("role[%lu] random_shop[%u] not find", m_role_uid, shop_id);
		reply.set_reply_code(common::errcode_enum::random_shop_error_not_shop);
		return;
	}

	if ((int32_t)index >= itr->second.items_size())
	{
		log_error("role[%lu] random_shop[%u] index[%u] not find", m_role_uid, shop_id, index);
		reply.set_reply_code(common::errcode_enum::random_shop_error_not_item);
		return;
	}

	proto::common::random_shop_item* p_shop_item = itr->second.mutable_items(index);
	if (p_shop_item == NULL || p_shop_item->state() == proto::common::random_shop_state_none)
	{
		log_error("role[%lu] random_shop[%u] index[%u] state error", m_role_uid, shop_id, index);
		reply.set_reply_code(common::errcode_enum::random_shop_error_item_state);
		return;
	}

	if (p_shop_item->state() == proto::common::random_shop_state_sold)
	{
		SHOP_LOG("role[%lu] random_shop[%u] item[%u] is sold", m_role_uid, shop_id, p_shop_item->shop_item());
		reply.set_reply_code(common::errcode_enum::random_shop_error_item_sold);
		return;
	}

	ShopItemTable* p_ShopItem = GET_CONF(ShopItemTable, p_shop_item->shop_item());
	if (NULL == p_ShopItem)
	{
		log_error("role[%lu] random_shop_item[%u] not find", m_role_uid, p_shop_item->shop_item());
		reply.set_reply_code(common::errcode_enum::random_shop_error_item_not_find);
		return;
	}

	money_ptr p_money = p_role->get_money();
	if (p_money == NULL)
	{
		log_error("role[%lu] p_money null error", m_role_uid);
		reply.set_reply_code(common::errcode_enum::notice_unknown);
		return;
	}

	// 判断钱够不够
	uint32_t money = p_money->get_money((proto::common::MONEY_TYPE)p_ShopItem->money_type());
	if (money < p_ShopItem->price())
	{
		SHOP_LOG("role[%lu] money_type[%u] money_value[%u] random_shop_item_id[%u] money[%u] insufficient",
			m_role_uid, p_ShopItem->money_type(), money, p_shop_item->shop_item(), p_ShopItem->price());

		switch (p_ShopItem->money_type())
		{
		case proto::common::MONEY_TYPE_YUANBAO:
			{
				reply.set_reply_code(common::errcode_enum::notice_shop_error2);
				return;
			}
			break;
		case proto::common::MONEY_TYPE_SILVER:
			{
				reply.set_reply_code(common::errcode_enum::notice_shop_error3);
				return;
			}
			break;
		case proto::common::MONEY_TYPE_COPPER:
			{
				reply.set_reply_code(common::errcode_enum::notice_shop_error4);
				return;
			}
			break;
		default:
			{
				reply.set_reply_code(common::errcode_enum::notice_shop_error5);
				return;
			}
			break;
		}
	}

	// 判断背包能不能放的下
	std::map<uint32_t, uint32_t> item_map;
	item_map.insert(std::make_pair(p_ShopItem->item_id(), p_ShopItem->item_val()));

	if (!item_manager_t::check_add_items(p_role, item_map))
	{
		SHOP_LOG("role[%lu] backpack is not enough", m_role_uid);
		reply.set_reply_code(common::errcode_enum::notice_shop_error8);
		return;
	}

	// 直接扣钱给东西
	if (!money_manager_t::use_money(p_role, (proto::common::MONEY_TYPE)p_ShopItem->money_type(), p_ShopItem->price(), log_enum::source_type_random_shop_buy,
		p_shop_item->shop_item(), false, reply.mutable_change_data()))
	{
		log_error("role[%lu] use_money money error", m_role_uid);
		reply.set_reply_code(common::errcode_enum::notice_shop_error5);
		return;
	}

	item_manager_t::add_items(p_role, item_map, log_enum::source_type_random_shop_item, p_shop_item->shop_item(), reply.mutable_change_data());

	if (reply.mutable_change_data()) p_role->get_personal_info(reply.mutable_change_data()->mutable_per_info());

	p_shop_item->set_state(proto::common::random_shop_state_sold);

	save_data(itr->second);

	reply.set_reply_code(common::errcode_enum::error_ok);
	reply.set_shop_id(shop_id);
	reply.set_item_index(index);

	log_wrapper_t::send_random_shop_log(p_role->get_uid(), shop_id, p_shop_item->shop_item(), p_ShopItem->money_type(), p_ShopItem->price(), p_ShopItem->item_id(), p_ShopItem->item_val(), 1, 0, log_enum::source_type_random_shop_buy);
	achieve_common_t::notify_progress_state(m_role_uid, proto::common::ACHIEVE_SHOP_BUY_NUMBER, proto::common::shop_type_exchange);
}

uint32_t role_random_shop_t::refresh_random_shop(const role_ptr& p_role, RandomShopTable* p_conf, proto::common::random_shop_data* p_data, bool is_auto/* = true*/)
{
	if (NULL == p_role)
	{
		log_error("p_role null error");
		return common::errcode_enum::notice_unknown;
	}

	if (NULL == p_conf)
	{
		log_error("p_conf null error");
		return common::errcode_enum::notice_unknown;
	}

	if (NULL == p_data)
	{
		log_error("p_data null error");
		return common::errcode_enum::notice_unknown;
	}

	p_data->set_shop_id(p_conf->id());

	if (is_auto)
	{
		uint32_t refresh_time = time_manager_t::get_next_refresh_time(p_conf->refresh_time());
		if (refresh_time == 0)
		{
			log_error("random_shop_id[%lu] refresh_time[%u] not find", p_conf->id(), p_conf->refresh_time());
			return common::errcode_enum::notice_unknown;
		}

		p_data->set_next_refresh_time(refresh_time);

		p_data->set_free_refresh(p_conf->free_refresh() != -1 ? p_conf->free_refresh() : 0);
		p_data->set_pay_refresh(p_conf->pay_refresh() != -1 ? p_conf->pay_refresh() : 0);
	}

	p_data->clear_items();

	const RandomShopTable::grid_vec& all_grids = p_conf->get_all_grids();

	for (uint32_t i = 0; i < all_grids.size(); ++i)
	{
		proto::common::random_shop_item* p_shop_item = p_data->add_items();

		if (p_shop_item != NULL)
		{
			uint32_t item_id = RandomShopConfig_t::get_random_item(p_role->get_level(), all_grids[i]);
			p_shop_item->set_shop_item(item_id);
			if (item_id == 0)
			{
				log_error("shop_id[%u] grid_index[%u] get item error", p_conf->id(), i);
				p_shop_item->set_state(proto::common::random_shop_state_none);
			}
			else
			{
				p_shop_item->set_state(proto::common::random_shop_state_normal);
			}
		}
	}

	save_data(*p_data);

	return common::errcode_enum::error_ok;
}

void role_random_shop_t::role_refresh_random_shop(const role_ptr& p_role, uint32_t shop_id, proto::client::gc_random_shop_refresh_reply& reply)
{
	if (NULL == p_role)
	{
		log_error("p_role null error");
		reply.set_reply_code(common::errcode_enum::notice_unknown);
		return;
	}

	RandomShopTable* p_conf = GET_CONF(RandomShopTable, shop_id);
	if (NULL == p_conf)
	{
		log_error("role[%lu] random_shop[%u] not find", m_role_uid, shop_id);
		reply.set_reply_code(common::errcode_enum::random_shop_error_not_find);
		return;
	}

	random_shop_map::iterator itr = m_random_shop_map.find(shop_id);
	if (itr == m_random_shop_map.end())
	{
		log_error("role[%lu] random_shop[%u] not find", m_role_uid, shop_id);
		reply.set_reply_code(common::errcode_enum::random_shop_error_not_shop);
		return;
	}

	uint32_t back_code = common::errcode_enum::error_ok;
	do 
	{
		if (p_conf->free_refresh() == 0 && p_conf->pay_refresh() == 0)
		{
			back_code = common::errcode_enum::random_shop_error_no_refresh;
			break;
		}

		if (p_conf->free_refresh() == -1)
		{
			log_wrapper_t::send_random_shop_log(p_role->get_uid(), shop_id, 0, 0, 0, 0, 0, 0, 0, log_enum::source_type_random_shop_free_refresh);
			break;
		}

		if (p_conf->free_refresh() > 0 && itr->second.free_refresh() > 0)
		{
			uint32_t old_times = itr->second.free_refresh();

			itr->second.set_free_refresh(old_times - 1);

			log_wrapper_t::send_random_shop_log(p_role->get_uid(), shop_id, 0, 0, 0, 0, 0, old_times, old_times - 1, log_enum::source_type_random_shop_free_refresh);

			break;
		}

		if (p_conf->pay_refresh() == -1 || (p_conf->pay_refresh() > 0 && itr->second.pay_refresh() > 0))
		{
			if (p_conf->cost_item() == 0 || p_conf->cost_value() == 0)
			{
				log_error("shop[%u] refresh item[%u:%u] error", shop_id, p_conf->cost_item(), p_conf->cost_value());
				back_code = common::errcode_enum::notice_unknown;
				break;
			}

			if (!item_manager_t::has_item(p_role, p_conf->cost_item(), p_conf->cost_value()))
			{
				SHOP_LOG("role[%lu] refresh shop[%u] item[%u:%u] not enough", m_role_uid, shop_id, p_conf->cost_item(), p_conf->cost_value());
				back_code = common::errcode_enum::random_shop_error_item_not_enough;
				break;
			}

			if (reply.mutable_change_data()) p_role->get_personal_info(reply.mutable_change_data()->mutable_per_info());

			item_manager_t::remove_item(p_role, p_conf->cost_item(), p_conf->cost_value(), log_enum::source_type_random_shop_cost_refresh, 0, reply.mutable_change_data());

			uint32_t old_times = itr->second.pay_refresh();

			log_wrapper_t::send_random_shop_log(p_role->get_uid(), shop_id, 0, 0, 0, p_conf->cost_item(), p_conf->cost_value(), old_times, old_times - 1, log_enum::source_type_random_shop_cost_refresh);

			if (old_times > 0)
			{
				itr->second.set_pay_refresh(old_times - 1);
				break;
			}

			break;
		}

		if (p_conf->free_refresh() > 0 && p_conf->pay_refresh() == 0)
		{
			back_code = common::errcode_enum::random_shop_error_free_deplete;
			break;
		}

		if (p_conf->free_refresh() == 0 && p_conf->pay_refresh() > 0)
		{
			back_code = common::errcode_enum::random_shop_error_pay_deplete;
			break;
		}

		back_code = common::errcode_enum::random_shop_error_both_refresh_deplete;

	} while (0);

	if (back_code != common::errcode_enum::error_ok)
	{
		reply.set_reply_code(back_code);
		return;
	}

	back_code = refresh_random_shop(p_role, p_conf, &itr->second, false);

	if (back_code != common::errcode_enum::error_ok)
	{
		log_error("role[%u] refresh random_shop_data[%u] error", m_role_uid, shop_id);
		reply.set_reply_code(back_code);
		return;
	}

	proto::common::random_shop_data* p_data = reply.mutable_info();
	if (NULL != p_data)
	{
		p_data->CopyFrom(itr->second);
	}

	reply.set_reply_code(common::errcode_enum::error_ok);
}

