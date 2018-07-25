#include "shop_manager.hpp"

#include "log.hpp"
#include "utility.hpp"
#include "config_mgr.h"
#include "tblh/errcode_enum.hpp"
#include "tblh/ShopTable.tbls.h"
#include "tblh/Comprehensive.tbls.h"
#include "tblh/Item.tbls.h"
#include "item/item_manager.hpp"
#include "role/money_manager.hpp"
#include "log/log_wrapper.hpp"
#include "achieve/achieve_common.hpp"

uint32_t shop_manager_t::check_user_buy_goods(role_ptr p_role, const proto::common::user_buy_goods& info, em_goods_check_type type, proto::common::role_change_data* p_data)
{
	if (p_role == NULL)
	{
		log_error("p_role null error");
		return common::errcode_enum::notice_unknown;
	}

	if (p_data == NULL)
	{
		log_error("p_data null error");
		return common::errcode_enum::notice_unknown;
	}

	uint32_t goods_id = info.goods_id();
	uint32_t goods_num = info.goods_num();

	if (goods_id == 0 || goods_num == 0)
	{
		log_error("user[%lu] goods id == 0 or number == 0", p_role->get_uid());
		return common::errcode_enum::notice_shop_error11;
	}

	ShopTable* p_conf = GET_CONF(ShopTable, goods_id);
	// 商品id错误
	if (NULL == p_conf)
	{
		log_error("user[%lu] ShopTable id[%u] not exsit", p_role->get_uid(), goods_id);
		return common::errcode_enum::notice_shop_error9;
	}

	// 限购商品game不能处理
	if (type == em_goods_check_type::goods_check_type_game && p_conf->refresh_type() != proto::common::shop_refresh_type_none)
	{
		log_error("user[%lu] ShopTable id[%u] is limit goods", p_role->get_object_id(), goods_id);
		return common::errcode_enum::notice_shop_error10;
	}

	// 判断购买条件
	const std::vector<std::pair<uint32_t, uint32_t>>& conds = p_conf->purchase_condition();
	for (const auto& item : conds) {
		switch (item.first) {
			case proto::common::shop_buy_condition_level: {
				if (p_role->get_level() < item.second) {
					SHOP_LOG("user[%lu] ShopTable id[%u] level insufficient", p_role->get_object_id(), goods_id);
					return common::errcode_enum::notice_shop_error12;
				}
				break;
			}
			case proto::common::shop_buy_condition_pvp: {
				if (p_role->get_arena().get_level() < item.second) {
					SHOP_LOG("user[%lu] ShopTable id[%u] pvp level insufficient", p_role->get_object_id(), goods_id);
					return common::errcode_enum::notice_shop_error13;
				}
				break;
			}
			case proto::common::shop_buy_condition_task: {
				task_mgr_ptr p_task_mgr_ptr = p_role->get_task_mgr();
				if (p_task_mgr_ptr == NULL || !p_task_mgr_ptr->is_done(item.second))
				{
					SHOP_LOG("user[%lu] ShopTable id[%u] task not done", p_role->get_object_id(), goods_id, item.second);
					return common::errcode_enum::notice_shop_error16;
				}
				break;
			}
			default: {
				log_error("user[%lu] ShopTable id[%u] buy_condition[%u] not exsit", p_role->get_object_id(), goods_id, item.first);
				return common::errcode_enum::notice_unknown;
			}
		}
	}

	money_ptr p_money = p_role->get_money();
	if (p_money == NULL)
	{
		log_error("user[%lu] p_money null error", p_role->get_object_id());
		return common::errcode_enum::notice_unknown;
	}

	// 判断钱够不够
	uint32_t money = p_money->get_money((proto::common::MONEY_TYPE)p_conf->money_type());
	uint32_t cost = p_conf->price() * goods_num;
	if (money < cost)
	{
		SHOP_LOG("user[%lu] money_type[%u] money_value[%u] ShopTable id[%u] money[%u] count[%u] insufficient",
			p_role->get_object_id(), p_conf->money_type(), money, goods_id, p_conf->price(), goods_num);

		switch (p_conf->money_type())
		{
		case proto::common::MONEY_TYPE_YUANBAO:
			{
				return common::errcode_enum::notice_shop_error2;
			}
			break;
		case proto::common::MONEY_TYPE_SILVER:
			{
				return common::errcode_enum::notice_shop_error3;
			}
			break;
		case proto::common::MONEY_TYPE_COPPER:
			{
				return common::errcode_enum::notice_shop_error4;
			}
			break;
		default:
			{
				return common::errcode_enum::notice_unknown;
			}
			break;
		}
	}

	// 判断背包能不能放的下
	std::map<uint32_t, uint32_t> item_map;
	item_map.insert(std::make_pair(p_conf->item_id(), goods_num));

	if (!item_manager_t::check_add_items(p_role, item_map))
	{
		SHOP_LOG("role[%lu] backpack is not enough", p_role->get_object_id());
		return common::errcode_enum::notice_shop_error8;
	}

	// 如果不是限购的购买直接扣钱给东西
	if (!money_manager_t::use_money(p_role, (proto::common::MONEY_TYPE)p_conf->money_type(), cost, log_enum::source_type_shop_buy,
		goods_id, false, p_data))
	{
		log_error("role[%lu] use_money money error", p_role->get_object_id());
		return common::errcode_enum::notice_unknown;
	}

	// 如果不是限购的购买直接扣钱给东西
	if (type == em_goods_check_type::goods_check_type_game)
	{
		item_manager_t::add_items(p_role, item_map, log_enum::source_type_shop_buy, goods_id, p_data);

		shop_manager_t::role_buy_one_goods(p_role->get_uid(), p_conf->shop_type(), goods_id, goods_num, p_conf->money_type(), cost);

		log_wrapper_t::send_user_shop_log(p_role->get_uid(), goods_id, goods_num, p_conf->money_type(), cost, p_conf->item_id(), goods_num, log_enum::source_type_buy_no_limit, 0);

		SHOP_LOG("role[%lu] buy shop_type[%u] goods[%u:%u] cost money[%u:%u]", p_role->get_uid(), p_conf->shop_type(), goods_id, goods_num, p_conf->money_type(), cost);
	}

	return common::errcode_enum::error_ok;
}

bool shop_manager_t::back_role_money(role_ptr p_role, const proto::common::user_buy_goods& info)
{
	if (p_role == NULL)
	{
		log_error("p_role null error");
		return false;
	}

	uint32_t goods_id = info.goods_id();
	uint32_t goods_num = info.goods_num();

	if (goods_id == 0 || goods_num == 0)
	{
		log_error("user[%lu] goods id == 0 or number == 0", p_role->get_uid());
		return false;
	}

	ShopTable* p_conf = GET_CONF(ShopTable, goods_id);
	// 商品id错误
	if (NULL == p_conf)
	{
		log_error("user[%lu] ShopTable id[%u] not exsit", p_role->get_uid(), goods_id);
		return false;
	}

	uint32_t cost = p_conf->price() * goods_num;
	
	// 把钱换给玩家
	money_manager_t::add_money(p_role, (proto::common::MONEY_TYPE)p_conf->money_type(), cost, log_enum::source_type_shop_buy_center_failed_back,
		goods_id, false, NULL);

	SHOP_LOG("role[%lu] buy goods[%u:%u] failed back money[%u:%u]", p_role->get_uid(), goods_id, goods_num, p_conf->money_type(), cost);

	return true;
}

uint32_t shop_manager_t::give_role_item(role_ptr p_role, const proto::common::user_buy_goods& info, proto::common::role_change_data* p_data)
{
	if (p_role == NULL)
	{
		log_error("p_role null error");
		return common::errcode_enum::notice_unknown;
	}

	if (p_data == NULL)
	{
		log_error("p_data null error");
		return common::errcode_enum::notice_unknown;
	}

	uint32_t goods_id = info.goods_id();
	uint32_t goods_num = info.goods_num();

	if (goods_id == 0 || goods_num == 0)
	{
		log_error("user[%lu] goods id == 0 or number == 0", p_role->get_uid());
		return common::errcode_enum::notice_shop_error11;
	}

	ShopTable* p_conf = GET_CONF(ShopTable, goods_id);
	// 商品id错误
	if (NULL == p_conf)
	{
		log_error("user[%lu] ShopTable id[%u] not exsit", p_role->get_uid(), goods_id);
		return common::errcode_enum::notice_shop_error9;
	}

	// 判断背包能不能放的下
	std::map<uint32_t, uint32_t> item_map;
	item_map.insert(std::make_pair(p_conf->item_id(), goods_num));

	if (!item_manager_t::check_add_items(p_role, item_map))
	{
		SHOP_LOG("role[%lu] backpack is not enough", p_role->get_object_id());
		return common::errcode_enum::notice_shop_error8;
	}

	item_manager_t::add_items(p_role, item_map, log_enum::source_type_shop_buy, goods_id, p_data);

	uint32_t cost = p_conf->price() * goods_num;

	shop_manager_t::role_buy_one_goods(p_role->get_uid(), p_conf->shop_type(), goods_id, goods_num, p_conf->money_type(), cost);

	log_wrapper_t::send_user_shop_log(p_role->get_uid(), goods_id, goods_num, p_conf->money_type(), cost, p_conf->item_id(), goods_num, log_enum::source_type_buy_limit, 0);

	SHOP_LOG("role[%lu] buy shop_type[%u] goods[%u:%u] cost money[%u:%u]", p_role->get_uid(), p_conf->shop_type(), goods_id, goods_num, p_conf->money_type(), cost);

	return common::errcode_enum::error_ok;
}

void shop_manager_t::role_buy_one_goods(uint64_t uid, uint32_t shop_type, uint32_t goods_id, uint32_t goods_num, uint32_t cost_type, uint32_t cost_money)
{
	achieve_common_t::notify_progress_state(uid, proto::common::Achieve_Event_Type::ACHIEVE_SHOP_BUY_NUMBER, shop_type);
}

uint32_t shop_manager_t::goods_buy_request(role_ptr p_role, uint64_t goods_uid, uint32_t item_tid, uint32_t buy_count, uint32_t price)
{
	if (NULL == p_role)
	{
		log_error("NULL == p_role");
		return errcode_enum::notice_role_null;
	}
	if (!item_manager_t::check_add_item(p_role, item_tid, buy_count))
	{
		log_error("p_role[%lu] check_add_item failed", p_role->get_uid());
		return errcode_enum::notice_main_bag_max_size;
	}
	uint32_t total_price = buy_count * price;
	if (!money_manager_t::use_money(p_role, proto::common::MONEY_TYPE_SILVER, total_price, log_enum::source_type_goods_buy, 0))
	{
		log_error("p_role[%lu] silver not enough", p_role->get_uid());
		return errcode_enum::notice_silver_money_not_enough;
	}
	proto::server::ge_goods_buy_request msg;
	msg.set_goods_uid(goods_uid);
	msg.set_item_tid(item_tid);
	msg.set_buy_count(buy_count);
	msg.set_price(price);
	env::server->send_msg_to_center(op_cmd::ge_goods_buy_request, p_role->get_uid(), msg);
	GOODS_LOG("role[%lu] request buy goods[%lu] item_tid[%d] buy_count[%d]",
		p_role->get_uid(), goods_uid, item_tid, buy_count);
	return common::errcode_enum::error_ok;
}

void shop_manager_t::goods_buy_reply(role_ptr p_role, const proto::server::eg_goods_buy_reply& msg, proto::client::gc_goods_buy_reply& reply)
{
	if (NULL == p_role)
	{
		log_error("NULL == p_role");
		return;
	}
	reply.set_reply_code(msg.reply_code());
	if (msg.reply_code() != errcode_enum::error_ok)
	{
		uint32_t back_money = msg.buy_count()*msg.price();
		money_manager_t::add_money(p_role, proto::common::MONEY_TYPE_SILVER, back_money, log_enum::source_type_goods_buy, 0);
		GOODS_LOG("role[%lu] buy goods failed, return money", p_role->get_uid());
		return;
	}
	const proto::common::goods_item& gi = msg.item_info();
	item_ptr p_item = item_manager_t::add_item(p_role, gi.item_tid(), gi.item_num(), log_enum::source_type_goods_buy, 0);
	if (NULL == p_item)
	{
		reply.set_reply_code(errcode_enum::notice_unknown);
		log_error("NULL == p_item");
		return;
	}
	auto p_item_conf = GET_CONF(Item, p_item->get_tid());
	if (NULL == p_item_conf)
	{
		reply.set_reply_code(errcode_enum::notice_unknown);
		log_error("NULL == p_item_conf[%d]", p_item->get_tid());
		return;
	}
	proto::common::role_change_data* p_data = reply.mutable_rcd();
	if (p_item->is_equip())
	{
		const proto::common::equip_single& es = gi.equip_info();
		std::map<uint32_t, uint32_t> attr_map;
		for (int32_t i = 0; i < es.attr_size(); ++i)
		{
			const proto::common::equip_attr& ea = es.attr(i);
			attr_map.insert(std::make_pair(ea.type(), ea.value()));
		}
		p_item->set_attr(attr_map);

		std::vector<uint32_t> exattr;
		for (int32_t i = 0; i < es.exattr_size(); ++i)
		{
			exattr.push_back(es.exattr(i));
		}
		p_item->set_exattr(exattr);
		p_item->set_name(es.name());
		p_item->set_special(es.special());
		p_item->set_strengthen_level(es.strengthen_level());
		p_item->set_strmaster_level(es.strmaster_level());
		p_item->set_remake_material(es.remake_material());
		p_item->set_lock_material(es.lock_material());
		p_item->set_strengthen_material(es.strengthen_material());
		p_item->set_strengthen_material_ex(es.strengthen_material_ex());
		p_item->set_strengthen_money(es.strengthen_money());
		p_item->set_score(es.score());
		p_item->check_rarity();
		auto p_task_mgr = p_role->get_task_mgr();
		if (NULL != p_task_mgr)
		{
			if (p_item->get_strengthen_level() >= 5)
			{
				uint32_t equip_count = item_manager_t::get_equip_strengthen_count(p_role, 5);
				p_task_mgr->try_change_task_state(TASK_TARGET_TYPE_EQUIP_STRENGTHEN_5, *p_data, 1, equip_count, false);
			}
			if (p_item->get_strengthen_level() >= 10)
			{
				uint32_t equip_count = item_manager_t::get_equip_strengthen_count(p_role, 10);
				p_task_mgr->try_change_task_state(TASK_TARGET_TYPE_EQUIP_STRENGTHEN_10, *p_data, 1, equip_count, false);
			}
		}
	}

	if (p_item_conf->bind_type() == bind_type_goods)
	{
		p_item->set_state(item_state_bind);
	}
	else
	{
		if (p_item->get_rarity() > 0)
		{
			p_item->set_state(item_state_cooldown);
			p_item->set_expired_time(time_util_t::now_time() + get_rarity_cooldown());
		}
	}
	p_item->save_self(p_role->get_uid());
	
	proto::common::item_data* p_item_data = p_data->mutable_item();
	proto::common::item_single *p_item_single = p_item_data->add_item_list();
	p_item->peek_data(p_item_single);
	p_item_single->set_up_num(gi.item_num());
	proto::common::personal_info* p_info = p_data->mutable_per_info();
	p_role->get_personal_info(p_info);
	GOODS_LOG("role[%lu] success buy item_tid[%d] buy_count[%d]", p_role->get_uid(), gi.item_tid(), gi.item_num());
}

uint32_t shop_manager_t::goods_sell_request(role_ptr p_role, uint64_t item_uid, uint32_t sell_count, uint32_t price)
{
	if (NULL == p_role)
	{
		log_error("NULL == p_role");
		return errcode_enum::notice_role_null;
	}
	item_ptr p_item = item_manager_t::get_item_by_uid(p_role, item_uid);
	if (NULL == p_item)
	{
		log_error("p_role[%lu] item[%lu] not found", p_role->get_uid(), item_uid);
		return errcode_enum::notice_item_null;
	}
	auto p_item_conf = GET_CONF(Item, p_item->get_tid());
	if (NULL == p_item_conf)
	{
		log_error("p_item_conf item[%d] not found", p_item->get_tid());
		return errcode_enum::notice_item_null;
	}
	if (0 == p_item_conf->can_sell())
	{
		log_error("p_role[%lu] item[%lu] tid[%d] cannot sell", p_role->get_uid(), item_uid, p_item->get_tid());
		return errcode_enum::notice_item_null;
	}
	p_item->check_state();
	if (item_state_none != p_item->get_state())
	{
		log_error("p_role[%lu] item[%lu] tid[%d] cannot sell state[%d]", p_role->get_uid(), item_uid, p_item->get_tid(), p_item->get_state());
		return errcode_enum::notice_item_null;
	}
	if (p_item->get_cur_num() < (int32_t)sell_count)
	{
		log_error("p_role[%lu] item[%lu] not enough", p_role->get_uid(), item_uid);
		return errcode_enum::notice_item_not_enough;
	}
	uint32_t sell_cost = get_sell_cost(sell_count, price);
	if (!money_manager_t::use_money(p_role, proto::common::MONEY_TYPE_COPPER, sell_cost, log_enum::source_type_goods_sell, 0))
	{
		log_error("p_role[%lu] copper not enough", p_role->get_uid());
		return errcode_enum::notice_copper_money_not_enough;
	}
	item_manager_t::remove_item(p_role, item_uid, sell_count, log_enum::source_type_goods_sell, 0);
	proto::server::ge_goods_sell_request msg;
	msg.set_item_uid(item_uid);
	msg.set_price(price);
	proto::common::goods_item* p_goods_item = msg.mutable_item_info();
	p_item->peek_goods_item(p_goods_item);
	p_goods_item->set_item_num(sell_count);
	env::server->send_msg_to_center(op_cmd::ge_goods_sell_request, p_role->get_uid(), msg);
	GOODS_LOG("role[%lu] request sell item[%lu] tid[%d] sell_count[%d]", 
		p_role->get_uid(), item_uid, p_item->get_tid(), sell_count);
	return common::errcode_enum::error_ok;
}

void shop_manager_t::goods_sell_reply(role_ptr p_role, const proto::server::eg_goods_sell_reply& msg, proto::client::gc_goods_sell_reply& reply)
{
	if (NULL == p_role)
	{
		log_error("NULL == p_role");
		return;
	}
	reply.set_reply_code(msg.reply_code());
	const proto::common::goods_item& gi = msg.item_info();
	if (msg.reply_code() != errcode_enum::error_ok)
	{
		uint32_t back_money = get_sell_cost(gi.item_num(), msg.price());
		money_manager_t::add_money(p_role, proto::common::MONEY_TYPE_COPPER, back_money, log_enum::source_type_goods_sell, 0);
		item_manager_t::add_item(p_role, gi.item_tid(), gi.item_num(), log_enum::source_type_goods_sell, 0);
		GOODS_LOG("role[%lu] failed sell goods, return money and item", p_role->get_uid());
	}
	else
	{
		item_ptr p_item = item_manager_t::get_item_by_uid(p_role, msg.item_uid());
		if (NULL == p_item)
		{
			log_error("p_role[%lu] item[%lu] not found", p_role->get_uid(), msg.item_uid());
			return;
		}
		proto::common::role_change_data* p_data = reply.mutable_rcd();
		proto::common::item_data* p_item_data = p_data->mutable_item();
		proto::common::item_single *p_item_single = p_item_data->add_item_list();
		p_item->peek_data(p_item_single);
		p_item_single->set_up_num(-1 * gi.item_num());
		proto::common::personal_info* p_info = p_data->mutable_per_info();
		p_role->get_personal_info(p_info);
		GOODS_LOG("role[%lu] success sell item[%lu] tid[%d] sell_count[%d]",
			p_role->get_uid(), p_item->get_uid(), p_item->get_tid(), gi.item_num());
	}
}

uint32_t shop_manager_t::get_sell_cost(uint32_t sell_count, uint32_t price)
{
	uint32_t sell_cost = sell_count * price;
	auto trading_poundage_min = GET_CONF(Comprehensive, comprehensive_common::trading_poundage_min);
	uint32_t min_cost = GET_COMPREHENSIVE_VALUE_1(trading_poundage_min);
	auto trading_poundage_max = GET_CONF(Comprehensive, comprehensive_common::trading_poundage_max);
	uint32_t max_cost = GET_COMPREHENSIVE_VALUE_1(trading_poundage_max);
	if (sell_cost < min_cost)
	{
		sell_cost = min_cost;
	}
	if (sell_cost > max_cost)
	{
		sell_cost = max_cost;
	}
	return sell_cost;
}

uint32_t shop_manager_t::goods_return_request(role_ptr p_role, uint64_t goods_uid, uint32_t item_tid, uint32_t item_num)
{
	if (!item_manager_t::check_add_item(p_role, item_tid, item_num))
	{
		log_error("p_role[%lu] check_add_item failed", p_role->get_uid());
		return errcode_enum::notice_main_bag_max_size;
	}
	proto::server::ge_goods_return_request msg;
	msg.set_goods_uid(goods_uid);
	env::server->send_msg_to_center(op_cmd::ge_goods_return_request, p_role->get_uid(), msg);
	GOODS_LOG("role[%lu] request return goods[%lu]", p_role->get_uid(), goods_uid);
	return common::errcode_enum::error_ok;
}

void shop_manager_t::goods_return_reply(role_ptr p_role, const proto::common::goods_item& item_info, proto::common::role_change_data* p_data)
{
	if (NULL == p_role)
	{
		log_error("NULL == p_role");
		return;
	}
	if (NULL == p_data)
	{
		log_error("NULL == p_data");
		return;
	}

	item_ptr p_item = item_manager_t::add_item(p_role, item_info.item_tid(), item_info.item_num(), log_enum::source_type_goods_return, 0);
	if (NULL == p_item)
	{
		log_error("NULL == p_item");
		return;
	}
	if (p_item->is_equip())
	{
		const proto::common::equip_single& es = item_info.equip_info();
		std::map<uint32_t, uint32_t> attr_map;
		for (int32_t i = 0; i < es.attr_size(); ++i)
		{
			const proto::common::equip_attr& ea = es.attr(i);
			attr_map.insert(std::make_pair(ea.type(), ea.value()));
		}
		p_item->set_attr(attr_map);

		std::vector<uint32_t> exattr;
		for (int32_t i = 0; i < es.exattr_size(); ++i)
		{
			exattr.push_back(es.exattr(i));
		}
		p_item->set_exattr(exattr);
		p_item->set_name(es.name());
		p_item->set_special(es.special());
		p_item->set_strengthen_level(es.strengthen_level());
		p_item->set_strmaster_level(es.strmaster_level());
		p_item->set_remake_material(es.remake_material());
		p_item->set_lock_material(es.lock_material());
		p_item->set_strengthen_material(es.strengthen_material());
		p_item->set_strengthen_material_ex(es.strengthen_material_ex());
		p_item->set_strengthen_money(es.strengthen_money());
		p_item->set_score(es.score());
		p_item->check_rarity();
		auto p_task_mgr = p_role->get_task_mgr();
		if (NULL != p_task_mgr)
		{
			if (p_item->get_strengthen_level() >= 5)
			{
				uint32_t equip_count = item_manager_t::get_equip_strengthen_count(p_role, 5);
				p_task_mgr->try_change_task_state(TASK_TARGET_TYPE_EQUIP_STRENGTHEN_5, *p_data, 1, equip_count, false);
			}
			if (p_item->get_strengthen_level() >= 10)
			{
				uint32_t equip_count = item_manager_t::get_equip_strengthen_count(p_role, 10);
				p_task_mgr->try_change_task_state(TASK_TARGET_TYPE_EQUIP_STRENGTHEN_10, *p_data, 1, equip_count, false);
			}
		}	
	}

	proto::common::item_data* p_item_data = p_data->mutable_item();
	proto::common::item_single *p_item_single = p_item_data->add_item_list();
	p_item->peek_data(p_item_single);
	p_item_single->set_up_num(item_info.item_num());
	proto::common::personal_info* p_info = p_data->mutable_per_info();
	p_role->get_personal_info(p_info);
	GOODS_LOG("role[%lu] success return item tid[%d] num[%d]",
		p_role->get_uid(), item_info.item_tid(), item_info.item_num());
}

uint32_t shop_manager_t::goods_resell_request(role_ptr p_role, uint64_t goods_uid, uint32_t sell_count, uint32_t price)
{
	if (NULL == p_role)
	{
		log_error("NULL == p_role");
		return errcode_enum::notice_role_null;
	}
	
	uint32_t sell_cost = get_sell_cost(sell_count, price);
	if (!money_manager_t::use_money(p_role, proto::common::MONEY_TYPE_COPPER, sell_cost, log_enum::source_type_goods_resell, 0))
	{
		log_error("p_role[%lu] copper not enough", p_role->get_uid());
		return errcode_enum::notice_copper_money_not_enough;
	}
	
	proto::server::ge_goods_resell_request msg;
	msg.set_goods_uid(goods_uid);
	msg.set_sell_count(sell_count);
	msg.set_price(price);
	env::server->send_msg_to_center(op_cmd::ge_goods_resell_request, p_role->get_uid(), msg);
	GOODS_LOG("role[%lu] request resell goods[%lu] sell_count[%d] price[%d]", p_role->get_uid(), goods_uid, sell_count, price);
	return common::errcode_enum::error_ok;
}

void shop_manager_t::goods_resell_reply(role_ptr p_role, const proto::server::eg_goods_resell_reply& msg, proto::client::gc_goods_resell_reply& reply)
{
	if (NULL == p_role)
	{
		log_error("NULL == p_role");
		return;
	}
	reply.set_reply_code(msg.reply_code());
	if (msg.reply_code() != errcode_enum::error_ok)
	{
		uint32_t back_money = get_sell_cost(msg.sell_count(), msg.price());
		money_manager_t::add_money(p_role, proto::common::MONEY_TYPE_COPPER, back_money, log_enum::source_type_goods_resell, 0);
		GOODS_LOG("role[%lu] failed resell goods, return money", p_role->get_uid());
	}
	else
	{
		proto::common::role_change_data* p_data = reply.mutable_rcd();
		proto::common::personal_info* p_info = p_data->mutable_per_info();
		p_role->get_personal_info(p_info);
		GOODS_LOG("role[%lu] success resell goods", p_role->get_uid());
	}
}

void shop_manager_t::goods_profit_request(uint64_t uid)
{
	proto::server::ge_goods_profit_request msg;
	env::server->send_msg_to_center(op_cmd::ge_goods_profit_request, uid, msg);
	GOODS_LOG("role[%lu] request profit goods", uid);
}

void shop_manager_t::goods_profit_reply(role_ptr p_role, uint32_t profit, proto::common::role_change_data* p_data)
{
	if (NULL == p_role)
	{
		log_error("NULL == p_role");
		return;
	}
	if (NULL == p_data)
	{
		log_error("NULL == p_data");
		return;
	}

	money_manager_t::add_money(p_role, proto::common::MONEY_TYPE_SILVER, profit, log_enum::source_type_goods_profit, 0);
	proto::common::personal_info* p_info = p_data->mutable_per_info();
	p_role->get_personal_info(p_info);
	GOODS_LOG("role[%lu] success profit[%d]", p_role->get_uid(), profit);
}

uint32_t shop_manager_t::get_item_cooldown()
{
	auto item_cooldown = GET_CONF(Comprehensive, comprehensive_common::trading_freezing_time);
	uint32_t val = GET_COMPREHENSIVE_VALUE_1(item_cooldown);
	return val * 60;
}

uint32_t shop_manager_t::get_rarity_cooldown()
{
	auto rarity_cooldown = GET_CONF(Comprehensive, comprehensive_common::trading_treasures_freezing_time);
	uint32_t val = GET_COMPREHENSIVE_VALUE_1(rarity_cooldown);
	return val * 60;
}