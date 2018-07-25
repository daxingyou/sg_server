#include "item_manager.hpp"
#include "log.hpp"
#include "global_id.hpp"
#include "log/log_wrapper.hpp"
#include "role/role_manager.hpp"
//#include "config/config_manager.hpp"
#include "tblh/errcode_enum.hpp"
#include "task/task_manager.hpp"
#include "tblh/Item.tbls.h"
#include "role/money_manager.hpp"
#include "drop_manager.hpp"
#include "user_troop/user_troop_mgr.hpp"
#include "tblh/Treasure.tbls.h"
#include "tblh/TreasureMake.tbls.h"
#include "tblh/TreasureEvent.tbls.h"
#include "tblh/Comprehensive.tbls.h"
#include "treasure/treasure_manager.hpp"
#include "mount/mount_manager.hpp"
#include "equip/equip_manager.hpp"
#include "tblh/HeroPlugin.tbls.h"
#include "hero/hero_manager.hpp"
#include "achieve/achieve_common.hpp"
#include "multi_time/multi_time_manager.hpp"
#include "tblh/ItemMerageTable.tbls.h"
#include "role/role_unify_save.hpp"
#include "cache_key.hpp"
#include "tblh/FashionMerageTable.tbls.h"

USING_NS_COMMON;

extern common::id_allocator_t common::g_id_allocator;
item_manager_t::item_limit_map item_manager_t::m_item_limit_map;

item_manager_t::item_manager_t()
{

}

item_manager_t::~item_manager_t()
{

}

void item_manager_t::load_all_datas(role_ptr p_role, const proto::common::item_data& data)
{
    if (NULL == p_role)
    {
        log_error("item p_role is NULL!");
        return;
    }

    for (int32_t i = 0; i < data.item_list_size(); ++i)
    {
        proto::common::item_single single = data.item_list(i);
        add_item(p_role, single);
    }
    //ITEM_LOG("[wsy item] role[%lu] load all items from db: size[%d]", p_role->get_uid(), data.item_list_size());
}

void item_manager_t::peek_all_datas(role_ptr p_role, proto::common::item_data* p_data)
{
    if (NULL == p_role)
    {
        log_error("item p_role NULL!");
        return;
    }
    if (NULL == p_data)
    {
        log_error("item p_data NULL");
        return;
    }

	p_role->m_package_manager.peek_all_datas(p_data);
    //ITEM_LOG("role[%lu] send all items to client: size[%d]", p_role->get_uid(), p_data->item_list_size());
}

void item_manager_t::peek_hero_equip_datas(role_ptr p_role, proto::common::item_data* p_data)
{
	if (NULL == p_role)
	{
		log_error("item p_role NULL!");
		return;
	}
	if (NULL == p_data)
	{
		log_error("item p_data NULL");
		return;
	}
	p_role->m_package_manager.peek_hero_equip_datas(p_data);
}


void item_manager_t::save_all_datas(role_ptr p_role)
{
    if (NULL == p_role)
    {
        log_error("item p_role NULL!");
        return;
	}
	p_role->m_package_manager.save_all_packages();
}

item_limit_ptr item_manager_t::get_item_limit(uint32_t item_tid)
{
	item_limit_map::iterator iter = m_item_limit_map.find(item_tid);
	if (iter != m_item_limit_map.end())
		return iter->second;
	return item_limit_ptr();
}


bool item_manager_t::is_need_check(uint32_t item_tid)
{
	if (NULL == get_item_limit(item_tid))
		return false;
	return true;
}

bool item_manager_t::check_limit_item(uint32_t item_tid, uint32_t item_num)
{
	// 在限制列表里的判断是否可以给 不在限制里的认为可以给 不统计数量
	item_limit_ptr p_item_limit = get_item_limit(item_tid);
	if (p_item_limit)
	{
		bool bresult = p_item_limit->can_add(item_num);
		if (bresult)
		{
			p_item_limit->set_count(p_item_limit->get_count() + 1);
			return true;
		}
	}
	return false;
}


void item_manager_t::update_item_limit(uint32_t item_tid, uint32_t item_num, uint32_t last_give_time)
{
	item_limit_ptr p_item_limit = get_item_limit(item_tid);
	if (p_item_limit)
	{
		p_item_limit->set_count(item_num);
		p_item_limit->set_last_give_time(last_give_time);
	}
}

void item_manager_t::load_item_limit_from_center(proto::server::eg_syn_item_limit_notify &msg)
{
	item_limit_ptr p_item_limit = item_limit_ptr();

	for (int32_t i = 0; i < msg.item_list_size(); ++i)
	{
		const proto::common::item_limit_data &item_limit_single = msg.item_list(i);
		p_item_limit.reset(new item_limit_t(item_limit_single.item_tid(), item_limit_single.item_num(),
											item_limit_single.last_give_time()));
		if (NULL == p_item_limit)
		{
			log_error("[wys] item_manager_t new item_limit_t is null");
			return;
		}
		m_item_limit_map.insert(std::make_pair(item_limit_single.item_tid(), p_item_limit));
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
item_ptr item_manager_t::get_item_by_tid(role_ptr p_role, uint32_t tid, proto::common::package_type package)
{
    if (NULL == p_role)
    {
        log_error("item p_role NULL!");
        return item_ptr();
    }
    if (0 == tid)
    {
        log_error("item tid = 0");
        return item_ptr();
    }

	return p_role->m_package_manager.get_item_by_tid(tid, package);
}

item_ptr item_manager_t::has_same_item(role_ptr p_role, uint32_t tid, proto::common::package_type package /*= proto::common::package_type::package_type_main*/)
{
	if (NULL == p_role)
	{
		log_error("item p_role NULL!");
		return item_ptr();
	}
	if (0 == tid)
	{
		log_error("item tid = 0");
		return item_ptr();
	}
	if (is_equip(tid))
	{
		return item_ptr();
	}
	if (is_special(tid))
	{
		return item_ptr();
	}
	auto p_conf = GET_CONF(Item, tid);
	if (NULL == p_conf)
	{
		return item_ptr();
	}
	if (1 == p_conf->overlap_count())
	{
		return item_ptr();
	}
	return p_role->m_package_manager.has_same_item(tid, package);
}

bool item_manager_t::can_overlap(role_ptr p_role, uint32_t tid, proto::common::package_type package /*= proto::common::package_type::package_type_main*/)
{
	if (NULL == p_role)
	{
		log_error("item p_role NULL!");
		return false;
	}
	if (0 == tid)
	{
		log_error("item tid = 0");
		return false;
	}
	if (is_equip(tid))
	{
		return false;
	}
	if (is_special(tid))
	{
		return false;
	}
	auto p_conf = GET_CONF(Item, tid);
	if (NULL == p_conf)
	{
		return false;
	}
	if (1 == p_conf->overlap_count())
	{
		return false;
	}
	return true;
}

// item_ptr item_manager_t::get_item_by_num(role_ptr p_role, uint32_t tid, int32_t num, proto::common::package_type package)
// {
//     if (NULL == p_role)
//     {
//         log_error("item p_role NULL!");
//         return item_ptr();
//     }
//     if (0 == tid)
//     {
//         log_error("item tid = 0");
//         return item_ptr();
//     }
// 
// 	return p_role->m_package_manager.get_item_by_num(tid, num, package);
// }

item_ptr item_manager_t::get_item_by_uid(object_base_ptr p_role, uint64_t uid, proto::common::package_type package)
{
    if (NULL == p_role)
    {
        log_error("item p_role NULL!");
        return item_ptr();
    }
    if (0 == uid)
    {
        log_error("item uid = 0");
        return item_ptr();
    }

	return p_role->get_item_by_uid(uid, package);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
uint64_t item_manager_t::gen_item_uid()
{
	uint64_t uid = common::g_id_allocator.gen_new_id(common::ID_ALLOCATOR_TYPE_ITEM);
	return uid;
}


bool item_manager_t::check_add_item(role_ptr p_role, uint32_t item_tid, uint32_t add_num, proto::common::package_type package /*= proto::common::package_type::package_type_main*/)
{
	if (NULL == p_role)
	{
		log_error("item p_role NULL!");
		return false;
	}
	package_ptr p_package = get_package(p_role, package);
	if (NULL == p_package)
	{
		log_error("item p_package NULL role_uid[%lu] package_type[%d]", p_role->get_uid(), package);
		return false;
	}
	if (!p_package->check_can_add(item_tid, add_num))
	{
		ITEM_LOG("check_add_item can't add item_id[%d] item_num[%d]", item_tid, add_num);
		return false;
	}
	return true;
}


bool item_manager_t::check_add_items(role_ptr p_role, const std::map<uint32_t, uint32_t> &item_tid_num_map, proto::common::package_type package)
{
	if (NULL == p_role)
	{
		log_error("item p_role NULL!");
		return false;
	}
	package_ptr p_package = get_package(p_role, package);
	if (NULL == p_package)
	{
		log_error("item p_package NULL role_uid[%lu] package_type[%d]", p_role->get_uid(), package);
		return false;
	}
	if (!p_package->check_can_add_items(item_tid_num_map))
	{
		ITEM_LOG("check_add_items role[%lu] fail", p_role->get_uid());
		return false;
	}
	return true;
}

bool item_manager_t::add_item(role_ptr p_role, item_ptr p_item, proto::common::package_type package)
{
    if (NULL == p_role)
    {
        log_error("item p_role NULL!");
        return false;
    }

    if (NULL == p_item)
    {
        log_error("item p_item NULL!");
        return false;
    }

	return p_role->m_package_manager.add_item(p_item, package);
}

void item_manager_t::add_item(role_ptr p_role, const proto::common::item_single& single)
{
    item_ptr p_item(new item_t());
	if (NULL == p_item)
	{
		log_error("item p_item new fail");
		return;
	}
    p_item->load_data(single);
	if (p_item->get_hero_uid() > 0)
	{
		hero_ptr p_hero = p_role->get_hero_by_unique_id(p_item->get_hero_uid());
		if (NULL == p_hero)
		{
			log_error("role[%lu] NULL == p_hero[%lu]", p_role->get_uid(), p_item->get_hero_uid());
			return;
		}
		auto p_equip_conf = GET_CONF(Equip, p_item->get_tid());
		if (NULL == p_equip_conf)
		{
			log_error("NULL == p_equip_conf[%d]", p_item->get_tid());
			return;
		}
		uint32_t pos = p_equip_conf->equip_type();
		if (!p_hero->equip_on(pos, p_item->get_uid()))
		{
			p_item->set_hero_uid(0);
		}
	}
    add_item(p_role, p_item, (proto::common::package_type)p_item->get_package());
	Item *p_conf = GET_CONF(Item, p_item->get_tid());
	if (NULL == p_conf) 
	{
		log_error("Item NULL == p_conf[%d]", p_item->get_tid());
		return;
	}
	//客户端非要服务器判断交付道具任务的完成条件，只能在增加和删除物品的地方检测一下
	//本人不为以下代码的效率负责 add by fangjian
	p_role->m_package_manager.add_search_val(p_conf->search_id(), p_item->get_cur_num());
}

item_ptr item_manager_t::add_item(uint64_t role_uid, uint32_t item_tid, uint32_t add_num, uint32_t source_type, uint32_t source_param, 
								  proto::common::role_change_data* p_data, proto::common::package_type package,
								  const std::string& creater_name, uint32_t param)
{
    role_ptr p_role = role_manager_t::find_role(role_uid);
    if (NULL == p_role) {
        log_error("role_uid[%lu] null error, when add item", role_uid);
        return NULL;
    }

    return add_item(p_role, item_tid, add_num, source_type, source_param, p_data, package, creater_name, param);
}

item_ptr item_manager_t::add_item(role_ptr p_role, uint32_t item_tid, uint32_t add_num, uint32_t source_type, uint32_t source_param,
	proto::common::role_change_data* p_data, proto::common::package_type package,
	const std::string& creater_name, uint32_t param)
{
	if (NULL == p_role) {
		log_error("item p_role NULL!");
		return NULL;
	}

	Item *p_conf = GET_CONF(Item, item_tid);
	if (!p_conf) {
		log_error("role[%lu] add item tid[%d] not found!", p_role->get_uid(), item_tid);
		return NULL;
	}

	if (0 == add_num) {
		log_error("role[%lu] add item tid[%d] num == 0!", p_role->get_uid(), p_conf->id());
		return NULL;
	}

	if (p_conf->overlap_count() == 1)
	{
		item_ptr p_item = NULL;
		for (uint32_t i = 0; i < add_num; ++i)
		{
			p_item = add_item(p_role, p_conf, 1, source_type, source_param, p_data, package, creater_name, param);
		}
		return p_item;
	}
	else
	{
		return add_item(p_role, p_conf, add_num, source_type, source_param, p_data, package, creater_name, param);
	}
}

item_ptr item_manager_t::add_item(role_ptr p_role, Item* p_conf, uint32_t add_num, uint32_t source_type, uint32_t source_param, 
	proto::common::role_change_data* p_data /*= NULL*/, 
	proto::common::package_type package /*= proto::common::package_type::package_type_main*/, 
	const std::string& creater_name /*= ""*/, 
	uint32_t param /*= 0*/)
{
	if (NULL == p_role) {
		log_error("item p_role NULL!");
		return NULL;
	}

	if (NULL == p_conf) {
		log_error("role[%lu] add item not found!", p_role->get_uid());
		return NULL;
	}

	if (p_conf->big_type() == item_big_type_money) { // 钱这个类型的特殊处理
		log_error("role[%lu] item[%d] big_type is money, so can't add item!", p_role->get_uid(), p_conf->id());
		return NULL;
	}

	uint32_t old_num = 0;
	uint32_t new_num = 0;
	item_ptr p_item = has_same_item(p_role, p_conf->id(), package);
	if (NULL == p_item) {
		p_item.reset(new item_t(gen_item_uid(), p_conf->id(), add_num, creater_name, param));
		if (p_conf->use_type() == proto::common::item_use_type_note_closed) p_item->set_find_role(p_role->get_name());
		if (p_conf->bind_type() == bind_type_get) p_item->set_state(item_state_bind);

		if (!add_item(p_role, p_item, package)) {
			log_error("role[%lu] add item tid[%d] insert error", p_role->get_uid(), p_conf->id());
			return NULL;
		}

		old_num = 0;
		new_num = p_item->get_cur_num();
	} else {
		old_num = p_item->get_cur_num();
		new_num = old_num + add_num;
		p_item->set_cur_num(new_num);
	}

	if (p_item->is_equip()) {
		if (!equip_manager_t::make_equip(p_role, p_item)) {
			log_error("role[%lu] add item tid[%d] make equip error", p_role->get_uid(), p_conf->id());
			return NULL;
		}

		log_wrapper_t::send_equip_log(p_role->get_uid(), p_role->get_level(), p_item, log_enum::opt_type_add, source_type, source_param);
		ITEM_LOG("role[%lu] create new equip[%d:%lu] success!", p_role->get_uid(), p_item->get_tid(), p_item->get_uid());

		achieve_common_t::notify_progress_state(p_role->get_uid(), proto::common::Achieve_Event_Type::ACHIEVE_EQUIP_TYPE_NUMBER, p_conf->quality(), add_num);
	} else {
		p_item->check_rarity();
		log_wrapper_t::send_item_log(p_role->get_uid(), p_role->get_level(), p_item->get_uid(), p_item->get_tid(), old_num, add_num, new_num, log_enum::opt_type_add, source_type, source_param);

		ITEM_LOG("role[%lu] create new item[%d:%lu] param[%d] add num[%d] from [%d] to [%d] success!", p_role->get_uid(), p_item->get_tid(), p_item->get_uid(), param, add_num, old_num, new_num);
	}

	p_item->save_self(p_role->get_uid());

	if (p_data) {
		proto::common::item_data* p_item_data = p_data->mutable_item();
		proto::common::item_single *p_item_single = p_item_data->add_item_list();
		p_item->peek_data(p_item_single);
		p_item_single->set_up_num(add_num);
	}

	p_role->script_event_create_item(p_item->get_tid());

	//客户端非要服务器判断交付道具任务的完成条件，只能在增加和删除物品的地方检测一下
	//本人不为以下代码的效率负责 add by fangjian
	p_role->m_package_manager.add_search_val(p_conf->search_id(), add_num);

	return p_item;
}

bool item_manager_t::add_item_ex(role_ptr p_role, uint32_t item_tid, uint32_t add_num, uint32_t source_type, uint32_t source_param,
	proto::common::role_change_data* p_data /*= NULL*/,
	proto::common::package_type package /*= proto::common::package_type::package_type_main*/,
	const std::string& creater_name /*= ""*/,
	uint32_t param /*= 0*/)
{
	if (NULL == p_role) {
		log_error("item p_role NULL!");
		return false;
	}

	Item *p_conf = GET_CONF(Item, item_tid);
	if (!p_conf) {
		log_error("role[%lu] add item tid[%d] not found!", p_role->get_uid(), item_tid);
		return false;
	}

	if (0 == add_num) {
		log_error("role[%lu] add item tid[%d] num == 0!", p_role->get_uid(), p_conf->id());
		return false;
	}

	if (p_conf->big_type() == item_big_type_money) { // 钱这个类型的特殊处理
		if (!proto::common::MONEY_TYPE_IsValid(p_conf->type())) {
			log_error("role[%lu] add item[%u] money_type[%u] invalid", p_role->get_uid(), item_tid, p_conf->type());
			return false;
		}

		money_manager_t::add_money(p_role, (proto::common::MONEY_TYPE)p_conf->type(), add_num, source_type, source_param);

		if (p_data) p_role->get_personal_info(p_data->mutable_per_info());

		return true;
	}
	else {
		return add_item(p_role, item_tid, add_num, source_type, source_param, p_data, package, creater_name, param) != NULL;
	}
}

uint32_t item_manager_t::add_items(role_ptr p_role, const std::map<uint32_t, uint32_t> &item_tid_num_map,
								   uint32_t source_type, uint32_t source_param, proto::common::role_change_data* p_data, 
								   proto::common::package_type package)
{
	for (const auto& it : item_tid_num_map) {
		if (!item_manager_t::add_item_ex(p_role, it.first, it.second, source_type, source_param, p_data, package)) {
			return errcode_enum::notice_main_bag_max_size;
		}
	}

	return errcode_enum::error_ok;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////


uint32_t item_manager_t::use_item(role_ptr p_role, uint64_t item_uid, uint32_t item_num, uint32_t &item_tid, proto::common::role_change_data *p_data, proto::common::package_type package)
{
	if (NULL == p_role)
	{
		log_error("item p_role NULL");
		return errcode_enum::notice_unknown;
	}
	uint32_t reply_code = errcode_enum::notice_item_type_not_match;

	item_ptr use_item = get_item_by_uid(p_role, item_uid, package);
	if (NULL == use_item)
	{
		log_error("item use_item NULL item_uid[%lu]", item_uid);
		return errcode_enum::notice_role_not_exist;
	}

	item_tid = use_item->get_tid();

	Item *config = GET_CONF(Item, use_item->get_tid());
	if (NULL == config)
	{
		log_error("item config NULL item_uid[%lu] item_tid[%d]", item_uid, use_item->get_tid());																						
		return errcode_enum::notice_item_null;
	}
    if (config->big_type() == item_big_type_money)
    {
        log_error("item[%d] big_type is money, so can't use item!", item_tid);
        return errcode_enum::notice_item_type_not_match;
    }
	switch (config->use_cond_type())
	{
	case use_item_cond_type_level:
		{
			if (p_role->get_level() < config->use_cond_param(0))
			{
				log_error("role[%lu] use item[%d] level not enough!", p_role->get_uid(), item_tid);
				return errcode_enum::notice_role_levle_not_enough;
			}
		}
	case use_item_cond_type_money:
		{
			log_warn("use_cond_param[%s]", config->use_cond_param().c_str());
			uint32_t money_type = config->use_cond_param(0);
			uint32_t money_count = config->use_cond_param(1);
			log_warn("money_type[%d]", money_type);
			log_warn("money_count[%d]", money_count);
			if (0 == money_type || 0 == money_count)
			{
				log_error("item[%d] use_cond_param invalid!", item_tid);
				return errcode_enum::notice_item_type_not_match;
			}
			if (!money_manager_t::use_money(p_role, (proto::common::MONEY_TYPE)money_type, money_count, log_enum::source_type_role_use_item_key_gift, item_tid))
			{
				log_error("role[%lu] use item[%d] money not enough!", p_role->get_uid(), item_tid);
				return errcode_enum::notice_copper_money_not_enough;
			}
		}
		break;
	default:
		break;
	}
	if (config->use_pos().first != 0)
	{
		if (!p_role->is_nearby(config->use_pos().first, config->use_pos().second, config->use_pos().third, get_item_use_range()))
		{
			log_error("p_role[%lu] use item[%d] failed, not in pos", p_role->get_uid(),item_tid);
			return errcode_enum::notice_item_wrong_pos;
		}
	}
	
	switch (config->use_type())
	{
	case proto::common::item_use_type_task:
		{
			reply_code = use_item_task(p_role, use_item, item_num, config, p_data);
		}
		break;
	case proto::common::item_use_type_drop:
		{
			for (uint32_t i = 0; i < item_num; ++i)
			{
				reply_code = use_item_gift_package(p_role, use_item, config, p_data);
			}			
		}
		break;
	case proto::common::item_use_type_mount:
		{
			 reply_code = use_item_mount(p_role, use_item, config, item_num, p_data);
		}
		break;
	case proto::common::item_use_type_treasure_item:
		{
			reply_code = errcode_enum::notice_item_type_not_match;
			treasure_manager_t::use_item_treasure(p_role, use_item, p_data);
		}
		break;
	case proto::common::item_use_type_note_closed:
		{
			reply_code = treasure_manager_t::use_item_note_closed(p_role, use_item, config, p_data);
		}
		break;
	case proto::common::item_use_type_note_opened:
		{
			reply_code = treasure_manager_t::use_item_note_opened(p_role, use_item, p_data);
		}
		break;
	case proto::common::item_use_type_reduce_pk_value:
		{
			reply_code = use_item_reduce_pk_value(p_role, use_item, config, item_num, p_data);
		}
		break;
	case proto::common::item_use_type_story:
		{
            reply_code = remove_item(p_role, item_uid, item_num, log_enum::source_type_role_use_item_story, 0, p_data, proto::common::package_type_main);
		}
		break;
	case proto::common::item_use_type_plugin:
		{
			reply_code = use_item_plugin(p_role, use_item, config, item_num, p_data);
		}
		break;
	case proto::common::item_use_type_change_plugin:
	{
		reply_code = use_item_change_plugin(p_role, use_item, config, item_num, p_data);
	}
	break;
	case proto::common::item_use_type_source_package:
		{
			for (uint32_t i = 0; i < item_num; ++i)
			{
				reply_code = use_item_source_package(p_role, use_item, config, p_data);
			}
		}
		break;
	case proto::common::item_use_type_multi_time:
		{
			reply_code = use_item_multi_time(p_role, use_item, config, item_num, p_data);
		}
		break;
	case proto::common::item_use_type_vigour_pill:
		{
			reply_code = user_vigour_pill(p_role, use_item, config, item_num, p_data);
		}
		break;
	case proto::common::item_use_type_add_role_buff:
		{
			reply_code = user_item_add_attr(p_role, use_item, config, item_num, p_data);
		}
		break;
	case proto::common::item_use_type_add_title:
		{
			reply_code = use_item_add_title(p_role, use_item, config, item_num, p_data);
		}
		break;
	case proto::common::item_use_type_key_gift:
		{
			reply_code = use_item_key_gift(p_role, use_item, config, p_data);
		}
		break;
	case proto::common::item_use_type_add_activity:
	{
		reply_code = use_item_add_activity(p_role, use_item, config, p_data);
	}
	break;
	default:
		break;
	}
	proto::common::personal_info *p_info = p_data->mutable_per_info();
	p_role->get_personal_info(p_info);
	return reply_code;
}


uint32_t item_manager_t::use_item_task(role_ptr p_role, item_ptr use_item, uint32_t item_num, Item *config, proto::common::role_change_data *p_data)
{
	if (NULL == p_role)
	{
		log_error("item p_role NULL");
		return errcode_enum::notice_unknown;
	}
	if (NULL == config)
	{
		log_error("item config NULL");
		return errcode_enum::notice_unknown;
	}
	if (NULL == use_item)
	{
		log_error("item use_item NULL");
		return errcode_enum::notice_unknown;
	}

	uint32_t reply_code = errcode_enum::error_ok;
	auto p_task_mgr = p_role->get_task_mgr();
	if (NULL == p_task_mgr)
	{
		log_error("item p_task_mgr NULL");
		reply_code = errcode_enum::notice_task_not_exist;
		return reply_code;
	}
	reply_code = item_manager_t::remove_item(p_role->get_uid(), use_item->get_uid(), item_num, log_enum::source_type_role_use_item_task, 0, p_data);
	p_task_mgr->try_change_task_state(TASK_TARGET_TYPE_USE_PROP, *p_data, use_item->get_tid(), item_num);
	return reply_code;
}


uint32_t item_manager_t::use_item_equip(role_ptr p_role, item_ptr use_item, uint32_t item_num)
{
	return errcode_enum::error_ok;
}


uint32_t item_manager_t::use_item_gift_package(role_ptr p_role, item_ptr use_item, Item *config, proto::common::role_change_data *p_data)
{
	if (NULL == p_role)
	{
		log_error("item p_role NULL");
		return errcode_enum::notice_unknown;
	}
	if (NULL == config)
	{
		log_error("item config NULL");
		return errcode_enum::notice_unknown;
	}
	if (NULL == use_item)
	{
		log_error("item use_item NULL");
		return errcode_enum::notice_unknown;
	}

	// 检测掉落所需格子
	if (!drop_manager_t::check_bag_num(p_role, config->param(0)))
	{
		ITEM_LOG("use_item_gift_package role[%lu] drop_num[%d] free package num not enough", p_role->get_uid(), config->param(0));
		return errcode_enum::notice_main_bag_max_size;
	}

	uint32_t reply_code = errcode_enum::error_ok;

	reply_code = remove_item(p_role, use_item->get_uid(), 1, log_enum::source_type_role_use_item_gift_package, 0, p_data,
							 proto::common::package_type_main);
	
	if (reply_code != errcode_enum::error_ok)
	{
		ITEM_LOG("use_item_gift_package remove_item fail role[%lu] code[%d]", p_role->get_uid(), reply_code);
		return reply_code;
	}

	bool b_result = drop_manager_t::drop(p_role, config->param(0), log_enum::source_type_role_use_item_gift_package,
									     proto::common::drop_sys_type_luck_package, use_item->get_tid(), p_data,false);

	if (!b_result)
	{
		log_error("item use_item_gift_package fail role[%lu] item_uid[%lu] tid[%d]", p_role->get_uid(), use_item->get_uid(), use_item->get_tid());
		return errcode_enum::notice_unknown;
	}

	return reply_code;
}

uint32_t item_manager_t::use_item_source_package(role_ptr p_role, item_ptr use_item, Item *config, proto::common::role_change_data *p_data)
{
	if (NULL == p_role)
	{
		log_error("item p_role NULL");
		return errcode_enum::notice_unknown;
	}
	if (NULL == config)
	{
		log_error("item config NULL");
		return errcode_enum::notice_unknown;
	}
	if (NULL == use_item)
	{
		log_error("item use_item NULL");
		return errcode_enum::notice_unknown;
	}
	
	uint32_t reply_code = errcode_enum::error_ok;

	reply_code = remove_item(p_role, use_item->get_uid(), 1, log_enum::source_type_role_use_item_source_package, 0, p_data,
		proto::common::package_type_main);

	if (reply_code != errcode_enum::error_ok)
	{
		ITEM_LOG("use_item_source_package remove_item fail role[%lu] code[%d]", p_role->get_uid(), reply_code);
		return reply_code;
	}
	
	bool b_result = drop_manager_t::drop(p_role, config->param(0), log_enum::source_type_role_use_item_source_package,
										 proto::common::drop_sys_type_luck_package, use_item->get_tid(), p_data, false);

	if (!b_result)
	{
		log_error("item use_item_source_package fail role[%lu] item_uid[%lu] tid[%d]", p_role->get_uid(), use_item->get_uid(), use_item->get_tid());
		return errcode_enum::notice_unknown;
	}

	return reply_code;
}

uint32_t item_manager_t::use_item_mount(role_ptr p_role, item_ptr use_item, Item *config, uint32_t item_num, proto::common::role_change_data *p_data)
{
	if (NULL == p_role)
	{
		log_error("item p_role NULL");
		return errcode_enum::notice_unknown;
	}
	if (NULL == config)
	{
		log_error("item config NULL");
		return errcode_enum::notice_unknown;
	}
	if (NULL == use_item)
	{
		log_error("item use_item NULL");
		return errcode_enum::notice_unknown;
	}
	if (p_role->get_mount_data()->get_mount(config->param(0)) != NULL)
	{
		ITEM_LOG("use_item_mount role[%lu] has_mount[%d]", p_role->get_uid(), config->param(0));
		return errcode_enum::notice_mount_has_one;
	}

	uint32_t reply_code = errcode_enum::error_ok;

	reply_code = remove_item(p_role, use_item->get_uid(), 1, log_enum::source_type_role_use_item_mount, 0, p_data,
		proto::common::package_type_main);

	if (reply_code != errcode_enum::error_ok)
	{
		ITEM_LOG("use_item_mount remove_item fail role[%lu] code[%d]", p_role->get_uid(), reply_code);
		return reply_code;
	}

	return mount_manager_t::add_mount(p_role, config->param(0), p_data);
}


uint32_t item_manager_t::use_item_reduce_pk_value(role_ptr p_role, item_ptr use_item, Item *config, uint32_t item_num, proto::common::role_change_data *p_data)
{
	if (NULL == p_role)
	{
		log_error("item p_role NULL");
		return errcode_enum::notice_unknown;
	}
	if (NULL == config)
	{
		log_error("item config NULL");
		return errcode_enum::notice_unknown;
	}
	if (NULL == use_item)
	{
		log_error("item use_item NULL");
		return errcode_enum::notice_unknown;
	}
	uint32_t cur_pk_value = p_role->get_pk_value();
	if (cur_pk_value == 0)
	{
		log_error("p_role[%lu] don't need reduce pk_value", p_role->get_uid());
		return errcode_enum::pk_cue_10;
	}

	uint32_t reply_code = errcode_enum::error_ok;

	reply_code = remove_item(p_role, use_item->get_uid(), item_num, log_enum::source_type_role_use_item_reduce_pk_value, 0, p_data,
		proto::common::package_type_main);

	if (reply_code != errcode_enum::error_ok)
	{
		ITEM_LOG("use_item_reduce_pk_value remove_item fail role[%lu] code[%d]", p_role->get_uid(), reply_code);
		return reply_code;
	}
	
	p_role->del_pk_value(config->param(0) * item_num, log_enum::source_type_role_use_item_reduce_pk_value, use_item->get_tid());
	proto::common::personal_info *p_info = p_data->mutable_per_info();
	p_role->get_personal_info(p_info);
	return errcode_enum::error_ok;
}


bool item_manager_t::is_equip(uint32_t item_tid)
{
	return item_tid / 10000 == equip_prefix;
}


bool item_manager_t::is_special(uint32_t item_tid)
{
	Item* p_config = GET_CONF(Item,item_tid);
	if (NULL == p_config)
	{
		log_error("NULL == p_config item[%d]",item_tid);
		return false;
	}
	if (proto::common::item_use_type_note_closed == p_config->use_type() ||
		proto::common::item_use_type_note_opened == p_config->use_type())
	{
		return true;
	}

	return false;
}

uint32_t item_manager_t::use_item_plugin(role_ptr p_role, item_ptr use_item, Item *config, uint32_t item_num, proto::common::role_change_data *p_data)
{
	uint32_t reply_code = errcode_enum::error_ok;
	hero_ptr p_hero = hero_ptr();
	uint32_t add_plugin_id = 0;
	do 
	{
		if (NULL == p_role)
		{
			log_error("item p_role NULL");
			reply_code = errcode_enum::notice_unknown;
			break;
		}

		if (NULL == config)
		{
			log_error("item config NULL");
			reply_code = errcode_enum::notice_unknown;
			break;
		}
		if (NULL == use_item)
		{
			log_error("item use_item NULL");
			reply_code = errcode_enum::notice_unknown;
			break;
		}

		HeroPlugin* p_config = GET_CONF(HeroPlugin, config->param(0));
		if (NULL == p_config)
		{
			log_error("p_config NULL plugin_id[%d]",config->param(0));
			reply_code = errcode_enum::notice_config_error;//配置错误
			break;
		}
		add_plugin_id = p_config->id();
		
		//是否拥有该武将
		p_hero = hero_manager_t::get_hero(p_role,p_config->hero_id());
		if (NULL == p_hero)
		{
			log_error("NULL == p_hero role[%lu] not exist hero[%d]",p_role->get_uid(),p_config->hero_id());
			reply_code = errcode_enum::notice_hero_null;//该英雄没有招募
			break;
		}

		//该武将是否已经拥有该皮肤(目前不能使用道具获取同一皮肤 后面可能会可以重复使用获得同一皮肤)
		bool is_has_plugin = false;
		const std::vector<uint32_t>& plugin_list = p_hero->get_plugin_list();
		for (auto& it : plugin_list)
		{
			if (it != p_config->id())
				continue;
			is_has_plugin = true;
			break;
		}

		if (is_has_plugin)
		{
			reply_code = errcode_enum::notice_already_have_plugin;//已经拥有该皮肤
			break;
		}

		reply_code = remove_item(p_role, use_item->get_uid(), 1, log_enum::source_type_role_use_item_plugin, 0, p_data,
			proto::common::package_type_main);	
	} while (false);

	if (reply_code == errcode_enum::error_ok)
	{
		if (NULL == p_hero)
		{
			log_error("NULL == p_hero");
			return errcode_enum::notice_hero_null;
		}
		p_hero->add_plugin(add_plugin_id);
		ITEM_LOG("role[%lu] hero[%d] add plugin[%d]", p_role->get_uid(), p_hero->get_tid(),add_plugin_id);
		proto::common::hero_data* p_hero_data = p_data->mutable_hero();
		proto::common::hero_single* p_hero_single = p_hero_data->add_hero_list();
		proto::common::base_attr* p_base_attr = p_hero_single->mutable_base_attr();
		p_base_attr->set_id(p_hero->get_tid());
		p_base_attr->set_current_plugin(p_hero->get_current_plugin());
		const std::vector<uint32_t>& plugin_list = p_hero->get_plugin_list();
		for (auto& it : plugin_list)
		{
			p_base_attr->add_plugins(it);
		}
	}
	return reply_code;
}


uint32_t item_manager_t::use_item_change_plugin(role_ptr p_role, item_ptr use_item, Item *config, uint32_t item_num, proto::common::role_change_data *p_data)
{
	if (NULL == p_role) {
		log_error("p_role null error");
		return common::errcode_enum::notice_unknown;
	}

	if (NULL == config) {
		log_error("item config NULL");
		return common::errcode_enum::notice_unknown;
	}

	if (NULL == use_item) {
		log_error("item use_item NULL");
		return common::errcode_enum::notice_unknown;
	}

	uint32_t reply_code = remove_item(p_role, use_item->get_uid(), item_num, log_enum::source_type_role_use_item_change_plugin, 0, p_data,
		proto::common::package_type_main);

	if (reply_code != common::errcode_enum::error_ok) {
		ITEM_LOG("use_item_change_plugin remove_item fail role[%lu] code[%d]", p_role->get_uid(), reply_code);
		return reply_code;
	}
	role_buff_mgr_ptr p_role_buff_mgr = p_role->get_role_buff_mgr();
	if (NULL != p_role_buff_mgr)
	{
		for (uint32_t i = 0; i < config->param_size(); ++i)
		{
			p_role_buff_mgr->add_role_buff(config->param(i), log_enum::source_type_role_use_item_change_plugin, config->id());
		}
		p_role_buff_mgr->notify_to_client();
	}

	return common::errcode_enum::error_ok;
}

uint32_t item_manager_t::use_item_multi_time(role_ptr p_role, item_ptr use_item, Item *config, uint32_t item_num, proto::common::role_change_data *p_data)
{
	uint32_t reply_code = errcode_enum::error_ok;
	do 
	{
		if (NULL == p_role)
		{
			log_error("item p_role NULL");
			reply_code = errcode_enum::notice_unknown;
			break;
		}
		if (NULL == config)
		{
			log_error("item config NULL");
			reply_code = errcode_enum::notice_unknown;
			break;
		}
		if (NULL == use_item)
		{
			log_error("item use_item NULL");
			reply_code = errcode_enum::notice_unknown;
			break;
		}

		if (!multi_time_manager_t::is_exist_add_multi_time(use_item->get_tid()))
		{
			log_error("item[%d] not use add multi_time",use_item->get_tid());
			reply_code = errcode_enum::notice_item_type_not_match;
			break;
		}
		reply_code = remove_item(p_role, use_item->get_uid(), item_num, log_enum::source_type_role_use_item_add_multi_time, 0, p_data,
			proto::common::package_type_main);
	} while (false);
	
	if (reply_code != errcode_enum::error_ok)
	{
		ITEM_LOG("use_item_multi_time remove_item fail role[%lu] code[%d]", p_role->get_uid(), reply_code);
		return reply_code;
	}

	reply_code = multi_time_manager_t::push_to_multi_list(p_role->get_uid(), use_item->get_tid(),p_data);
	return reply_code;
}


uint32_t item_manager_t::user_vigour_pill(const role_ptr& p_role, item_ptr use_item, Item *config, uint32_t item_num, proto::common::role_change_data *p_data)
{
	if (NULL == p_role) {
		log_error("p_role null error");
		return common::errcode_enum::notice_unknown;
	}

	if (NULL == config) {
		log_error("item config NULL");
		return common::errcode_enum::notice_unknown;
	}

	if (NULL == use_item) {
		log_error("item use_item NULL");
		return common::errcode_enum::notice_unknown;
	}

	uint32_t vigour_max_limit = p_role->get_vigour_max_limit();
	uint32_t curr_vigour = p_role->get_vigour();
	uint32_t add_value = config->param(0) * item_num;
	if (curr_vigour >= vigour_max_limit) {
		ITEM_LOG("role[%lu] vigour[%u] add[%u] can't get more", p_role->get_uid(), curr_vigour, add_value);
		return common::errcode_enum::notice_vigour_too_much;
	}

	uint32_t reply_code = remove_item(p_role, use_item->get_uid(), item_num, log_enum::source_type_role_vigour_pill, 0, p_data,
		proto::common::package_type_main);

	if (reply_code != common::errcode_enum::error_ok) {
		ITEM_LOG("user_vigour_pill remove_item fail role[%lu] code[%d]", p_role->get_uid(), reply_code);
		return reply_code;
	}

	money_manager_t::add_money(p_role, proto::common::MONEY_TYPE_VIGOUR, add_value, common::log_enum::source_type_vigour_pill_add, 0, true, p_data);

	return common::errcode_enum::error_ok;
}


uint32_t item_manager_t::user_item_add_attr(const role_ptr& p_role, item_ptr use_item, Item *config, uint32_t item_num, proto::common::role_change_data *p_data)
{
	if (NULL == p_role) {
		log_error("p_role null error");
		return common::errcode_enum::notice_unknown;
	}

	if (NULL == config) {
		log_error("item config NULL");
		return common::errcode_enum::notice_unknown;
	}

	if (NULL == use_item) {
		log_error("item use_item NULL");
		return common::errcode_enum::notice_unknown;
	}

	uint32_t reply_code = remove_item(p_role, use_item->get_uid(), item_num, log_enum::source_type_role_use_item_add_rolebuff, 0, p_data,
		proto::common::package_type_main);

	if (reply_code != common::errcode_enum::error_ok) {
		ITEM_LOG("user_item_add_attr remove_item fail role[%lu] code[%d]", p_role->get_uid(), reply_code);
		return reply_code;
	}
	role_buff_mgr_ptr p_role_buff_mgr = p_role->get_role_buff_mgr();
	if (NULL != p_role_buff_mgr)
	{
		p_role_buff_mgr->add_role_buff(config->param(0), log_enum::source_type_role_use_item_add_rolebuff, config->id());
		p_role_buff_mgr->notify_to_client();
	}
	hero_manager_t::peek_all_datas(p_role, p_data->mutable_hero());

	return common::errcode_enum::error_ok;
}


uint32_t item_manager_t::use_item_add_title(role_ptr p_role, item_ptr use_item, Item *config, uint32_t item_num, proto::common::role_change_data *p_data)
{
	if (NULL == p_role) {
		log_error("p_role null error");
		return common::errcode_enum::notice_unknown;
	}

	if (NULL == config) {
		log_error("item config NULL");
		return common::errcode_enum::notice_unknown;
	}

	if (NULL == use_item) {
		log_error("item use_item NULL");
		return common::errcode_enum::notice_unknown;
	}

	uint32_t reply_code = remove_item(p_role, use_item->get_uid(), item_num, log_enum::source_type_role_use_item_add_title, 0, p_data,
		proto::common::package_type_main);

	if (reply_code != common::errcode_enum::error_ok) {
		ITEM_LOG("user_item_add_title remove_item fail role[%lu] code[%d]", p_role->get_uid(), reply_code);
		return reply_code;
	}

	p_role->add_title(config->param(0));
	p_role->get_personal_info(p_data->mutable_per_info());

	return common::errcode_enum::error_ok;
}


uint32_t item_manager_t::use_item_key_gift(role_ptr p_role, item_ptr use_item, Item *config, proto::common::role_change_data *p_data)
{
	if (NULL == p_role) {
		log_error("p_role null error");
		return common::errcode_enum::notice_unknown;
	}

	if (NULL == config) {
		log_error("item config NULL");
		return common::errcode_enum::notice_unknown;
	}

	if (NULL == use_item) {
		log_error("item use_item NULL");
		return common::errcode_enum::notice_unknown;
	}

	uint32_t reply_code = remove_item(p_role, use_item->get_uid(), 1, log_enum::source_type_role_use_item_key_gift, 0, p_data,
		proto::common::package_type_main);

	if (reply_code != common::errcode_enum::error_ok) {
		ITEM_LOG("use_item_key_gift remove_item fail role[%lu] code[%d]", p_role->get_uid(), reply_code);
		return reply_code;
	}

	bool b_result = drop_manager_t::drop(p_role, config->param(0), log_enum::source_type_role_use_item_key_gift,
		proto::common::drop_sys_type_luck_package, use_item->get_tid(), p_data, false);

	if (!b_result)
	{
		log_error("item use_item_key_gift fail role[%lu] item_uid[%lu] tid[%d]", p_role->get_uid(), use_item->get_uid(), use_item->get_tid());
		return errcode_enum::notice_unknown;
	}

	return reply_code;
}


uint32_t item_manager_t::use_item_add_activity(role_ptr p_role, item_ptr use_item, Item *config, proto::common::role_change_data *p_data)
{
	if (NULL == p_role) {
		log_error("p_role null error");
		return common::errcode_enum::notice_unknown;
	}

	if (NULL == config) {
		log_error("item config NULL");
		return common::errcode_enum::notice_unknown;
	}

	if (NULL == use_item) {
		log_error("item use_item NULL");
		return common::errcode_enum::notice_unknown;
	}
	activity_mgr_ptr p_act_mgr = p_role->get_activity_mgr();
	if (NULL != p_act_mgr)
	{
		if (!p_act_mgr->add_activity_count(config->param(0), config->param(1)))
		{
			log_error("role[%lu] use_item_add_activity activity[%d] count[%d] failed", p_role->get_uid(), config->param(0), config->param(1));
			return common::errcode_enum::notice_unknown;
		}
	}

	uint32_t reply_code = remove_item(p_role, use_item->get_uid(), 1, log_enum::source_type_role_use_item_add_activity, 0, p_data,
		proto::common::package_type_main);

	if (reply_code != common::errcode_enum::error_ok) {
		log_error("use_item_add_activity remove_item fail role[%lu] code[%d]", p_role->get_uid(), reply_code);
		return reply_code;
	}

	return reply_code;
}

uint32_t item_manager_t::get_item_use_range()
{
	auto item_use_range = GET_CONF(Comprehensive, comprehensive_common::item_use_range);
	uint32_t val = GET_COMPREHENSIVE_VALUE_1(item_use_range);
	return val;
}

bool item_manager_t::has_item(role_ptr p_role, uint32_t item_tid, uint32_t item_num, proto::common::package_type package)
{
	if (NULL == p_role)
	{
		log_error("item p_role NULL!");
		return false;
	}
	if (is_equip(item_tid))
	{
		return false;
	}
	if (is_special(item_tid))
	{
		return false;
	}
	if (get_item_count(p_role, item_tid, package) < item_num)
	{
		return false;
	}
	return true;
}

bool item_manager_t::has_items(role_ptr p_role, const std::map<uint32_t, uint32_t> &item_tid_num_map, proto::common::package_type package)
{
	if (NULL == p_role)
	{
		log_error("item p_role NULL");
		return false;
	}
	std::map<uint32_t, uint32_t>::const_iterator it = item_tid_num_map.begin();
	std::map<uint32_t, uint32_t>::const_iterator it_end = item_tid_num_map.end();
	for (; it != it_end; ++it)
	{
		uint32_t count = get_item_count(p_role, it->first, package);
		if (count < it->second)
		{
			log_error("item[%d] count[%d] not enough, need[%d]", it->first, count, it->second);
			return false;
		}
	}
	return true;
}

uint32_t item_manager_t::get_item_count(role_ptr p_role, uint32_t item_tid, proto::common::package_type package)
{
	if (NULL == p_role)
	{
		log_error("item p_role NULL");
		return 0;
	}
	item_ptr p_item = get_item_by_tid(p_role, item_tid, package);
	if (NULL == p_item)
	{
		//log_error("get_item_count p_item NULL role[%lu] item_id[%d] package[%d]", p_role->get_uid(), item_tid, package);
		return 0;
	}
	return p_item->get_cur_num();
}

uint32_t item_manager_t::remove_item(role_ptr p_role, uint32_t item_tid, uint32_t item_num, uint32_t source_type, uint32_t source_param, proto::common::role_change_data* p_data, proto::common::package_type package)
{
	if (NULL == p_role)
	{
		log_error("item p_role NULL");
		return errcode_enum::notice_unknown;
	}
	if (0 == item_num)
	{
		log_error("item item_num = 0 role_uid[%lu] item_tid[%d]", p_role->get_uid(), item_tid);
		return errcode_enum::notice_item_not_enough;
	}

	Item *p_item_config = GET_CONF(Item, item_tid);
	if (NULL == p_item_config)
	{
		log_error("item p_item_config NULL item_tid[%d]", item_tid);
		return errcode_enum::notice_item_null;
	}
    if (p_item_config->big_type() == item_big_type_money)
    {
        log_error("item[%d] big_type is money, so can't use item!", item_tid);
        return errcode_enum::notice_item_type_not_match;
    }
	item_ptr p_item = get_item_by_tid(p_role, item_tid, package);
	if (NULL == p_item)
	{
		log_error("item p_item NULL item_tid[%d] package[%d]", item_tid, package);
		return errcode_enum::notice_item_null;
	}
	uint32_t cur_num = p_item->get_cur_num();
	if (cur_num < item_num)
	{
		log_error("item cur_num[%d] < item_num[%d] item_tid[%d] package[%d]", cur_num, item_num, item_tid, package);
		return errcode_enum::notice_item_not_enough;
	}
	p_item->set_cur_num(cur_num - item_num);
	p_item->save_self(p_role->get_uid());
	
	log_wrapper_t::send_item_log(p_role->get_uid(), p_role->get_level(), p_item->get_uid(), p_item->get_tid(), cur_num, item_num, cur_num - item_num, log_enum::opt_type_del, source_type, source_param);
	if (NULL != p_data)
	{
		proto::common::item_data* p_item_data = p_data->mutable_item();
		proto::common::item_single *p_item_single = p_item_data->add_item_list();
		p_item->peek_data(p_item_single);
		p_item_single->set_up_num(-(int32_t)item_num);
	}
	if (p_item->is_equip())
	{
		log_wrapper_t::send_equip_log(p_role->get_uid(), p_role->get_level(), p_item, log_enum::opt_type_del, source_type, source_param);
		ITEM_LOG("role[%lu] remove equip[%d:%lu] success!", p_role->get_uid(), p_item->get_tid(), p_item->get_uid());
		auto p_task_mgr = p_role->get_task_mgr();
		if (NULL != p_task_mgr)
		{
			if (p_item->get_strengthen_level() >= 5)
			{
				uint32_t equip_count = item_manager_t::get_equip_strengthen_count(p_role, 5);
				if (p_data) {
					p_task_mgr->try_change_task_state(TASK_TARGET_TYPE_EQUIP_STRENGTHEN_5, *p_data, 1, equip_count, false);
				} else {
					proto::common::role_change_data rcd;
					p_task_mgr->try_change_task_state(TASK_TARGET_TYPE_EQUIP_STRENGTHEN_5, rcd, 1, equip_count, false);
				}
			}
			if (p_item->get_strengthen_level() >= 10)
			{
				uint32_t equip_count = item_manager_t::get_equip_strengthen_count(p_role, 10);
				if (p_data) {
					p_task_mgr->try_change_task_state(TASK_TARGET_TYPE_EQUIP_STRENGTHEN_10, *p_data, 1, equip_count, false);
				} else {
					proto::common::role_change_data rcd;
					p_task_mgr->try_change_task_state(TASK_TARGET_TYPE_EQUIP_STRENGTHEN_10, rcd, 1, equip_count, false);	
				}
			}
		}
	}
	ITEM_LOG("role[%lu] remove item[%d:%lu] remove num[%d] from [%d] to [%d] success!", p_role->get_uid(), p_item->get_tid(), p_item->get_uid(), item_num, cur_num, cur_num - item_num);
	//客户端非要服务器判断交付道具任务的完成条件，只能在增加和删除物品的地方检测一下
	//本人不为以下代码的效率负责 add by fangjian
	p_role->m_package_manager.del_search_val(p_item_config->search_id(), item_num);
	return errcode_enum::error_ok;
}

uint32_t item_manager_t::remove_item(role_ptr p_role, uint64_t item_uid, uint32_t item_num, uint32_t source_type, uint32_t source_param, proto::common::role_change_data* p_data, proto::common::package_type package)
{
	if (NULL == p_role)
	{
		log_error("item p_role NULL");
		return errcode_enum::notice_unknown;
	}
	if (0 == item_num)
	{
		log_error("item item_num = 0 role_uid[%lu]", p_role->get_uid());
		return errcode_enum::notice_item_not_enough;
	}
	
	item_ptr p_item = get_item_by_uid(p_role, item_uid, package);
	if (NULL == p_item)
	{
		log_error("item p_item NULL item_uid[%lu] package[%d]", item_uid, package);
		return errcode_enum::notice_item_null;
	}

	uint32_t cur_num = p_item->get_cur_num();
	if (cur_num < item_num)
	{
		log_error("item cur_num[%d] < item_num[%d] item_uid[%lu] package[%d]", cur_num, item_num, item_uid, package);
		return errcode_enum::notice_item_not_enough;
	}

	Item *p_item_config = GET_CONF(Item, p_item->get_tid());
	if (NULL == p_item_config)
	{
		log_error("item p_item_config NULL item_tid[%d]", p_item->get_tid());
		return errcode_enum::notice_item_null;
	}
    if (p_item_config->big_type() == item_big_type_money)
    {
        log_error("item[%d] big_type is money, so can't use item!", p_item->get_tid());
        return errcode_enum::notice_item_type_not_match;
    }
	p_item->set_cur_num(cur_num - item_num);
	p_item->save_self(p_role->get_uid());
	
	if (p_item->is_equip())
	{
		log_wrapper_t::send_equip_log(p_role->get_uid(), p_role->get_level(), p_item, log_enum::opt_type_del, source_type, source_param);
		ITEM_LOG("role[%lu] remove equip[%d:%lu] success!", p_role->get_uid(), p_item->get_tid(), p_item->get_uid());
		equip_plan_mgr_ptr p_equip_plan_mgr = p_role->get_equip_plan_mgr();
		if (NULL != p_equip_plan_mgr)
		{
			p_equip_plan_mgr->del_equip(item_uid);
		}
		auto p_task_mgr = p_role->get_task_mgr();
		if (NULL != p_task_mgr)
		{
			if (p_item->get_strengthen_level() >= 5)
			{
				uint32_t equip_count = item_manager_t::get_equip_strengthen_count(p_role, 5);
				if (p_data) {
					p_task_mgr->try_change_task_state(TASK_TARGET_TYPE_EQUIP_STRENGTHEN_5, *p_data, 1, equip_count, false);
				} else {
					proto::common::role_change_data rcd;
					p_task_mgr->try_change_task_state(TASK_TARGET_TYPE_EQUIP_STRENGTHEN_5, rcd, 1, equip_count, false);
				}
			}
			if (p_item->get_strengthen_level() >= 10)
			{
				uint32_t equip_count = item_manager_t::get_equip_strengthen_count(p_role, 10);
				if (p_data) {
					p_task_mgr->try_change_task_state(TASK_TARGET_TYPE_EQUIP_STRENGTHEN_10, *p_data, 1, equip_count, false);
				} else {
					proto::common::role_change_data rcd;
					p_task_mgr->try_change_task_state(TASK_TARGET_TYPE_EQUIP_STRENGTHEN_10, rcd, 1, equip_count, false);	
				}
			}
		}
	}
	else
	{
		log_wrapper_t::send_item_log(p_role->get_uid(), p_role->get_level(), p_item->get_uid(), p_item->get_tid(), cur_num, item_num, cur_num - item_num, log_enum::opt_type_del, source_type, source_param);
		ITEM_LOG("role[%lu] remove item[%d:%lu] remove num[%d] from [%d] to [%d] success!", p_role->get_uid(), p_item->get_tid(), p_item->get_uid(), item_num, cur_num, cur_num - item_num);
	}
	
	if (NULL != p_data)
	{
		proto::common::item_data* p_item_data = p_data->mutable_item();
		proto::common::item_single *p_item_single = p_item_data->add_item_list();
		p_item->peek_data(p_item_single);
		p_item_single->set_up_num(-(int32_t)item_num);
	}
	//客户端非要服务器判断交付道具任务的完成条件，只能在增加和删除物品的地方检测一下
	//本人不为以下代码的效率负责 add by fangjian
	p_role->m_package_manager.del_search_val(p_item_config->search_id(), item_num);
	return errcode_enum::error_ok;
}

uint32_t item_manager_t::remove_item(uint64_t role_uid, uint32_t item_tid, uint32_t item_num, uint32_t source_type, uint32_t source_param, proto::common::role_change_data* p_data, proto::common::package_type package)
{
	role_ptr p_role = role_manager_t::find_role(role_uid);
	if (NULL == p_role)
	{
		log_error("item p_role NULL role_uid[%lu]", role_uid);
		return errcode_enum::notice_unknown;
	}
	return remove_item(p_role, item_tid, item_num, source_type, source_param, p_data, package);
}

uint32_t item_manager_t::remove_items(role_ptr p_role, std::map<uint32_t, uint32_t> item_tid_num_map, uint32_t source_type, uint32_t source_param, proto::common::role_change_data* p_data, proto::common::package_type package)
{
	if (NULL == p_role)
	{
		log_error("item p_role NULL");
		return errcode_enum::notice_unknown;
	}
	if (!has_items(p_role, item_tid_num_map, package))
	{
		ITEM_LOG("remove_items item not enough role[%lu]", p_role->get_uid());
		return errcode_enum::notice_item_not_enough;
	}

	uint32_t reply_code = errcode_enum::error_ok;
	for (auto item : item_tid_num_map)
	{
		reply_code = remove_item(p_role, item.first, item.second, source_type, source_param, p_data, package);
		if (reply_code != errcode_enum::error_ok)
		{
			ITEM_LOG("remove_items remove_item fail role[%lu] code[%d]", p_role->get_uid(), reply_code);
			return reply_code;
		}
	}
	return errcode_enum::error_ok;
}


uint32_t item_manager_t::remove_items(role_ptr p_role, const std::vector<uint64_t>& item_list, uint32_t source_type, uint32_t source_param, proto::common::role_change_data* p_data /*= NULL*/, proto::common::package_type package /*= proto::common::package_type::package_type_main*/)
{
	if (NULL == p_role)
	{
		log_error("item p_role NULL");
		return errcode_enum::notice_unknown;
	}
	for (auto item_uid : item_list)
	{
		uint32_t reply_code = remove_item(p_role, item_uid, 1, source_type, source_param, p_data, package);
		if (reply_code != errcode_enum::error_ok)
		{
			ITEM_LOG("remove_items remove_item fail role[%lu] code[%d]", p_role->get_uid(), reply_code);
			return reply_code;
		}
	}

	ITEM_LOG("role[%lu] remove items[%d] success!", p_role->get_uid(), (int32_t)item_list.size());
	return errcode_enum::error_ok;
}

package_ptr item_manager_t::get_package(role_ptr p_role, proto::common::package_type package /*= proto::common::package_type::package_type_main*/)
{
	if (NULL == p_role)
	{
		log_error("item p_role NULL");
		return package_ptr();
	}

	return p_role->m_package_manager.get_package(package);
}

uint32_t item_manager_t::sell_bag_item(role_ptr p_role, uint64_t item_uid, uint32_t item_num, proto::common::role_change_data* p_data)
{
	if (NULL == p_role)
	{
		log_error("item p_role NULL");
		return errcode_enum::notice_unknown;
	}
	if (NULL == p_data)
	{
		log_error("item p_data NULL");
		return errcode_enum::notice_unknown;
	}
	if (0 == item_num)
	{
		log_error("item item_num = 0 role[%lu] item_uid[%lu]", p_role->get_uid(), item_uid);
		return errcode_enum::notice_item_not_enough;
	}

	item_ptr use_item = get_item_by_uid(p_role, item_uid, proto::common::package_type_main);
	if (NULL == use_item)
	{
		log_error("item use_item NULL item_uid[%lu] package[%d]", item_uid, proto::common::package_type_main);
		return errcode_enum::notice_role_not_exist;
	}
	if (use_item->get_hero_uid() > 0)
	{
		log_error("role[%lu] item_uid[%lu] equiped cannot sell", p_role->get_uid(), item_uid);
		return errcode_enum::equip_item_cannot_refer;
	}
	Item *config = GET_CONF(Item, use_item->get_tid());
	if (NULL == config)
	{
		log_error("item config NULL item_uid[%lu] item_tid[%d]", item_uid, use_item->get_tid());
		return errcode_enum::notice_item_null;
	}
    if (config->big_type() == item_big_type_money)
    {
        log_error("item[%d] big_type is money, so can't use item!", use_item->get_tid());
        return errcode_enum::notice_item_type_not_match;
    }
	uint32_t reply_code = errcode_enum::error_ok;

	reply_code = remove_item(p_role, item_uid, item_num, log_enum::source_type_role_sell_bag_item, 0, p_data, proto::common::package_type_main);
	if (reply_code != errcode_enum::error_ok)
	{
		ITEM_LOG("sell_bag_item remove_item fail role[%lu] code[%d]", p_role->get_uid(), reply_code);
		return reply_code;
	}

	uint32_t sell_price = config->sell_price() * item_num;
	money_manager_t::add_money(p_role, proto::common::MONEY_TYPE_COPPER, sell_price, log_enum::source_type_role_sell_bag_item, 0, false, p_data);

	return reply_code;
}


uint32_t item_manager_t::get_equip_strengthen_count(role_ptr p_role, uint32_t level)
{
	if (NULL == p_role)
	{
		log_error("item p_role NULL");
		return 0;
	}
	auto p_package = get_package(p_role);
	if (NULL == p_package)
	{
		log_error("item p_package NULL");
		return 0;
	}
	return p_package->get_equip_strengthen_count(level);
}

proto::common::MONEY_TYPE item_manager_t::is_money_item(uint32_t tid)
{
    Item *p_item_config = GET_CONF(Item, tid);
    if (NULL == p_item_config)
    {
        log_error("item item_tid[%d] not found!", tid);
        return proto::common::MONEY_TYPE_NONE;
    }

    if (p_item_config->big_type() == item_big_type_money)
    {
		return proto::common::MONEY_TYPE(p_item_config->type());

    }
    return proto::common::MONEY_TYPE(proto::common::MONEY_TYPE_END + 1);
}

//道具合成
uint32_t item_manager_t::merage_item(role_ptr p_role, uint32_t item_tid, uint32_t merage_num, proto::common::role_change_data* p_data)
{
	if (NULL == p_role)
	{
		log_error("item_prole is NULL");
		return common::errcode_enum::notice_unknown;
	}

	ItemMerageTable *p_conf = GET_CONF(ItemMerageTable, item_tid);
	if (NULL == p_conf) 
	{
		log_error("role[%lu] item_tid[%u] not find", p_role->get_uid(), item_tid);
		return common::errcode_enum::notice_unknown;
	}

	//判断货币是否足够,目前只支持一种货币
	auto &money_cost_map = p_conf->money_const_map();
	uint32_t money_type = 0;
	uint32_t money_cost = 0;
	if (!money_cost_map.empty()) 
	{
		auto iter_cost = money_cost_map.begin();
		money_type = iter_cost->first;
		money_cost = iter_cost->second;
		if ( (money_cost * merage_num) > money_manager_t::get_money(p_role, proto::common::MONEY_TYPE(money_type)) ){
			log_error("role[%lu] merge_item_tid[%u] money_type[%u], money_cost[%u] not enough", p_role->get_uid(), item_tid, money_type, money_cost );
			return common::errcode_enum::notice_gold_money_not_enough;
		}
	}

	//判断合成道具是否足够
	auto &item_cond_map = p_conf->item_condition_map();
	for (auto iter : item_cond_map) 
	{
		if ( !item_manager_t::has_item(p_role, iter.first, iter.second * merage_num ) ) 
		{
			log_error("role[%lu] merge_item_tid[%u] item_cond_id[%u], num[%u] not enough", p_role->get_uid(), item_tid, iter.first, iter.second);
			return common::errcode_enum::notice_item_not_enough;
		}
	}

	//扣除货币
	if ( money_cost > 0 )
	{
		money_manager_t::use_money(p_role, (proto::common::MONEY_TYPE)money_type, money_cost * merage_num, log_enum::source_type_role_merage_item, item_tid, false,p_data);
	}

	//扣除道具
	for (auto iter : item_cond_map) {
		item_manager_t::remove_item(p_role, iter.first, iter.second * merage_num, log_enum::source_type_role_merage_item, item_tid, p_data);
	}

	//添加新道具
	item_ptr p_item = item_manager_t::add_item(p_role, item_tid, 1 * merage_num, log_enum::source_type_role_merage_item, item_tid, p_data);
	if (NULL == p_item)
	{
		log_error("role[%lu] merage_item_tid[%u], num[%u] new_item is NULL", p_role->get_uid(),item_tid, merage_num );
		return errcode_enum::notice_unknown;
	}

	return errcode_enum::error_ok;
}


uint32_t item_manager_t::decompose_item(role_ptr p_role, uint64_t item_uid, uint32_t item_num, proto::common::role_change_data* p_data)
{
	if (NULL == p_role)
	{
		log_error("item p_role NULL");
		return errcode_enum::notice_unknown;
	}
	if (NULL == p_data)
	{
		log_error("item p_data NULL");
		return errcode_enum::notice_unknown;
	}
	if (0 == item_num)
	{
		log_error("item item_num = 0 role[%lu] item_uid[%lu]", p_role->get_uid(), item_uid);
		return errcode_enum::notice_item_not_enough;
	}

	item_ptr use_item = get_item_by_uid(p_role, item_uid, proto::common::package_type_main);
	if (NULL == use_item)
	{
		log_error("item use_item NULL item_uid[%lu] package[%d]", item_uid, proto::common::package_type_main);
		return errcode_enum::notice_role_not_exist;
	}

	Item *config = GET_CONF(Item, use_item->get_tid());
	if (NULL == config)
	{
		log_error("item config NULL item_uid[%lu] item_tid[%d]", item_uid, use_item->get_tid());
		return errcode_enum::notice_item_null;
	}
    if (config->big_type() == item_big_type_money)
    {
        log_error("item[%d] big_type is money, so can't use item!", use_item->get_tid());
        return errcode_enum::notice_item_type_not_match;
    }
	uint32_t reply_code = errcode_enum::error_ok;

	reply_code = remove_item(p_role, item_uid, item_num, log_enum::source_type_role_decompose_item, 0, p_data, proto::common::package_type_main);
	if (reply_code != errcode_enum::error_ok)
	{
		ITEM_LOG("sell_bag_item remove_item fail role[%lu] code[%d]", p_role->get_uid(), reply_code);
		return reply_code;
	}

	uint32_t sell_price = config->resolve_soul() * item_num;
	money_manager_t::add_money(p_role, proto::common::MONEY_TYPE_HEROSOUL, sell_price, log_enum::source_type_role_decompose_item, 0, false, p_data);

	return reply_code;
}

//多选道具礼包使用
uint32_t item_manager_t::use_multiple_chose_item(role_ptr p_role, uint64_t item_uid, const std::set<uint32_t> chosen_list, proto::common::role_change_data *p_data, proto::common::package_type package)
{
	if (NULL == p_role)
	{
		log_error("item p_role NULL");
		return errcode_enum::notice_unknown;
	}

	uint32_t reply_code = errcode_enum::notice_item_type_not_match;

	item_ptr use_item = get_item_by_uid(p_role, item_uid, package);
	if (NULL == use_item)
	{
		log_error("item use_item NULL item_uid[%lu]", item_uid);
		return errcode_enum::notice_item_null;
	}

	Item *config = GET_CONF(Item, use_item->get_tid());
	if (NULL == config)
	{
		log_error("item config NULL item_uid[%lu] item_tid[%d]", item_uid, use_item->get_tid());
		return errcode_enum::notice_item_null;
	}

    if (chosen_list.size() != config->package_choose_num())
    {
        log_error("user[%lu] chose itemlist size[%u] not match config size[%u]", p_role->get_uid(), (int32_t)chosen_list.size(), config->package_choose_num());
        return errcode_enum::notice_unknown;
    }

	reply_code = remove_item(p_role, use_item->get_uid(), 1, log_enum::source_type_role_use_item_gift_package, 0, p_data, proto::common::package_type_main);

	for (auto it : chosen_list)
	{
		if (it > config->param_size())
		{
			log_error("item use_item_gift_package fail role[%lu], index[%u] > cond_size[%u]", p_role->get_uid(), it, config->param_size());
			continue;
		}

		bool b_result = drop_manager_t::drop(p_role, config->param(it), log_enum::source_type_role_use_item_gift_package,
			proto::common::drop_sys_type_luck_package, use_item->get_tid(), p_data);

		if (!b_result)
		{
			log_error("item use_item_gift_package fail role[%lu] item_uid[%lu] tid[%d]", p_role->get_uid(), use_item->get_uid(), use_item->get_tid());
			continue;
		}

	}
	return reply_code;
}

//道具合成
uint32_t item_manager_t::merage_fashion(role_ptr p_role, uint32_t item_tid, uint32_t merage_num, proto::common::role_change_data* p_data)
{
	if (NULL == p_role)
	{
		log_error("item_prole is NULL");
		return common::errcode_enum::notice_unknown;
	}

	FashionMerageTable *p_conf = GET_CONF(FashionMerageTable, item_tid);
	if (NULL == p_conf)
	{
		log_error("role[%lu] item_tid[%u] not find", p_role->get_uid(), item_tid);
		return common::errcode_enum::notice_unknown;
	}

	//判断货币是否足够,目前只支持一种货币
	auto &money_cost_map = p_conf->money_const_map();
	uint32_t money_type = 0;
	uint32_t money_cost = 0;
	if (!money_cost_map.empty())
	{
		auto iter_cost = money_cost_map.begin();
		money_type = iter_cost->first;
		money_cost = iter_cost->second;
		if ((money_cost * merage_num) > money_manager_t::get_money(p_role, proto::common::MONEY_TYPE(money_type))) {
			log_error("role[%lu] merge_item_tid[%u] money_type[%u], money_cost[%u] not enough", p_role->get_uid(), item_tid, money_type, money_cost);
			return common::errcode_enum::notice_gold_money_not_enough;
		}
	}

	//判断合成道具是否足够
	auto &item_cond_map = p_conf->item_condition_map();
	for (auto iter : item_cond_map)
	{
		if (!item_manager_t::has_item(p_role, iter.first, iter.second * merage_num))
		{
			log_error("role[%lu] merge_item_tid[%u] item_cond_id[%u], num[%u] not enough", p_role->get_uid(), item_tid, iter.first, iter.second);
			return common::errcode_enum::notice_item_not_enough;
		}
	}

	//扣除货币
	if (money_cost > 0)
	{
		money_manager_t::use_money(p_role, (proto::common::MONEY_TYPE)money_type, money_cost * merage_num, log_enum::source_type_role_merage_item, item_tid, false, p_data);
	}

	//扣除道具
	for (auto iter : item_cond_map) {
		item_manager_t::remove_item(p_role, iter.first, iter.second * merage_num, log_enum::source_type_role_merage_item, item_tid, p_data);
	}

	//添加新道具
	item_ptr p_item = item_manager_t::add_item(p_role, item_tid, 1 * merage_num, log_enum::source_type_role_merage_item, item_tid, p_data);
	if (NULL == p_item)
	{
		log_error("role[%lu] merage_item_tid[%u], num[%u] new_item is NULL", p_role->get_uid(), item_tid, merage_num);
		return errcode_enum::notice_unknown;
	}

	return errcode_enum::error_ok;
}


