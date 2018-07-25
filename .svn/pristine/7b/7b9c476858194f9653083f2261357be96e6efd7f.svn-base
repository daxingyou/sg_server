#include "equip_manager.hpp"
#include "log.hpp"
#include "global_id.hpp"
#include "log/log_wrapper.hpp"
#include "tblh/Hero.tbls.h"
#include "role/role_manager.hpp"
#include "tblh/EquipExchange.tbls.h"
#include "tblh/EquipExattrMake.tbls.h"
#include "tblh/EquipExattrRemake.tbls.h"
#include "tblh/EquipStar.tbls.h"
#include "tblh/EquipSmelt.tbls.h"
#include "tblh/EquipStrengthen.tbls.h"
#include "tblh/EquipStrengthenMaster.tbls.h"
#include "tblh/Item.tbls.h"
#include "hero/hero_manager.hpp"
#include "item/item_manager.hpp"
#include "tblh/Comprehensive.tbls.h"
#include "utility.hpp"
#include "role/money_manager.hpp"
#include "tblh/errcode_enum.hpp"
#include "tblh/attr_enum.hpp"
#include "tblh/ShopTable.tbls.h"
#include "tblh/HeroGrade.tbls.h"
#include "item/drop_manager.hpp"
#include "achieve/achieve_common.hpp"

USING_NS_COMMON;
USING_NS_CONFIG;

EquipExattrPoolMap equip_manager_t::equip_exattr_pool_map;
EquipExchangePoolMap equip_manager_t::equip_exchange_pool_map;
EquipNamePoolMap equip_manager_t::equip_name_pool_map;
EquipSpecialPoolMap equip_manager_t::equip_special_pool_map;
uint32_t equip_manager_t::equip_remake_stone_gold;
std::map<uint32_t, uint32_t> equip_manager_t::s_box_info;

extern common::id_allocator_t common::g_id_allocator;

item_ptr equip_manager_t::get_equip_by_uid(role_ptr p_role, uint64_t uid)
{
	item_ptr p_item = item_manager_t::get_item_by_uid(p_role, uid);
    if (NULL == p_item)
    {
        log_error("p_item is NULL!");
        return item_ptr();
    }
    if (!p_item->is_equip())
    {
		log_error("p_item[%llu] is not equip!", uid);
		return item_ptr();
    }
    return p_item;
}

bool equip_manager_t::make_equip(role_ptr p_role, item_ptr p_equip)
{
	if (NULL == p_role)
	{
		log_error("p_role is NULL!");
		return false;
	}
	if (NULL == p_equip)
	{
		log_error("NULL == p_equip");
		return false;
	}
	if (!p_equip->is_equip())
	{
		log_error("p_item[%llu:%d] is not equip!", p_equip->get_uid(), p_equip->get_tid());
		return false;
	}
	auto p_equip_config = GET_CONF(Equip, p_equip->get_tid());
	if (NULL == p_equip_config)
	{
		log_error("Equip[%d] FindCnf not found", p_equip->get_tid());
		return false;
	}
	auto p_item_config = GET_CONF(Item, p_equip->get_tid());
	if (NULL == p_item_config)
	{
		log_error("Item[%d] FindCnf not found", p_equip->get_tid());
		return false;
	}
	auto p_equip_exattr_make_config = GET_CONF(EquipExattrMake, p_item_config->quality());
	if (NULL == p_equip_exattr_make_config)
	{
		log_error("equip exattr make quality[%d] FindCnf not found", p_item_config->quality());
		return false;
	}
	p_equip->set_hero_uid(0);
	//生成基础属性
	EQUIP_LOG("role[%lu] equip[%llu:%d] make_attr", p_role->get_uid(), p_equip->get_uid(), p_equip->get_tid());
	std::map<uint32_t, uint32_t> attr_map;
	make_attr(p_equip_config, attr_map);
	p_equip->set_attr(attr_map);
	//生成附加属性
	EQUIP_LOG("role[%lu] equip[%llu:%d] make_exattr", p_role->get_uid(), p_equip->get_uid(), p_equip->get_tid());
	std::vector<uint32_t> exattr_vec;
	make_exattr(p_equip_exattr_make_config->count(), p_equip_config->exattr_pool(), exattr_vec);
	p_equip->set_exattr(exattr_vec);
	//生成词缀
	if (p_equip_config->name_pool().first > 0)
	{
		uint32_t name_id = make_name(p_equip_config->name_pool().first, p_equip_config->name_pool().second);
		p_equip->set_name(name_id);
	}
	//生成特技
	if (p_equip_config->special_pool().first > 0)
	{
		uint32_t special_id = make_special(p_equip_config->special_pool().first, p_equip_config->special_pool().second);
		p_equip->set_special(special_id);
	}
	p_equip->check_rarity();
	p_equip->calc_score();
	p_equip->save_self(p_role->get_uid(), true);
	
	EQUIP_LOG("role[%lu] create new equip[%lu:%d] success!", p_role->get_uid(), p_equip->get_uid(), p_equip->get_tid());

	//achieve_method_t::achieve_exattr(p_role->get_uid(),proto::common::Achieve_Event_Type::ACHIEVE_EQUIP_EXATTR_NUMBER_STAR,exattr_vec);

	return true;
}

uint32_t equip_manager_t::exchange_equip(role_ptr p_role, uint32_t type, uint32_t exchange_id, proto::common::role_change_data* p_data)
{
	if (NULL == p_role)
	{
		log_error("NULL == p_role");
		return errcode_enum::notice_role_null;
	}
	if (NULL == p_data)
	{
		log_error("NULL == p_data");
		return errcode_enum::notice_unknown;
	}
	EQUIP_LOG("role[%lu] exchange_equip exchange_id[%d] type[%d]", p_role->get_uid(), exchange_id, type);
	auto p_exchange_conf = GET_CONF(EquipExchange, exchange_id);
	if (NULL == p_exchange_conf)
	{
		log_error("EquipExchange[%d] conf not found", exchange_id);
		return errcode_enum::notice_unknown;
	}
	if (!item_manager_t::has_items(p_role, p_exchange_conf->material()))
	{
		log_error("role[%lu] exchange_id[%d] material not enough", p_role->get_uid(), exchange_id);
		return errcode_enum::notice_item_not_enough;
	}
	if (money_manager_t::get_money(p_role, proto::common::MONEY_TYPE_COPPER) < p_exchange_conf->copper())
	{
		log_error("role[%lu] exchange_id[%d] copper not enough", p_role->get_uid(), exchange_id);
		return errcode_enum::notice_copper_money_not_enough;
	}
	bool b_lucky = false;
	uint32_t equip_lucky = p_role->get_equip_lucky();
	EQUIP_LOG("equip_lucky[%d]", equip_lucky);
	if (random_util_t::selectByTenTh(equip_lucky))
	{
		EQUIP_LOG("lucky");
		b_lucky = true;
	}
	uint32_t equip_exchange_pool = 0;
	if (0 == type)
	{
		if (b_lucky)
		{
			//普通幸运打造
			equip_exchange_pool = p_exchange_conf->exchange_lucky_pool();
		}
		else
		{
			//普通打造
			equip_exchange_pool = p_exchange_conf->exchange_pool();
		}
	}
	else
	{
		if (errcode_enum::error_ok != item_manager_t::remove_items(p_role, p_exchange_conf->ex_material(), log_enum::source_type_equip_exchange, exchange_id, p_data))
		{
			log_error("role[%lu] exchange_id[%d] ex_material not enough", p_role->get_uid(), exchange_id);
			return errcode_enum::notice_item_not_enough;
		}
		if (b_lucky)
		{
			//强化幸运打造
			equip_exchange_pool = p_exchange_conf->ex_exchange_lucky_pool();
		}
		else
		{
			//强化打造
			equip_exchange_pool = p_exchange_conf->ex_exchange_pool();
		}
	}
	item_manager_t::remove_items(p_role, p_exchange_conf->material(), log_enum::source_type_equip_exchange, exchange_id, p_data);
	money_manager_t::use_money(p_role, proto::common::MONEY_TYPE_COPPER, p_exchange_conf->copper(), log_enum::source_type_equip_exchange, exchange_id, true, p_data);
	EQUIP_LOG("role[%lu] get equip_exchange_pool[%d]", p_role->get_uid(), equip_exchange_pool);
	uint32_t target_equip_id = get_equip_id_by_exchange_pool(equip_exchange_pool);
	EQUIP_LOG("role[%lu] get target_equip_id[%d]", p_role->get_uid(), target_equip_id);
	item_ptr p_equip = item_manager_t::add_item(p_role, target_equip_id, 1, log_enum::source_type_equip_exchange, exchange_id, p_data);
	if (NULL == p_equip)
	{
		log_error("NULL == p_equip");
		return errcode_enum::notice_unknown;
	}
	//幸运值处理
	if (b_lucky)
	{
		//触发幸运打造清零
		p_role->set_equip_lucky(0);
		EQUIP_LOG("p_role->set_equip_lucky(0)");
	}
	else
	{
		//没有触发幸运打造
		if (p_equip->is_clear_equip_lucky())
		{
			//满足清零条件清零
			p_role->set_equip_lucky(0);
			EQUIP_LOG("p_equip->is_clear_equip_lucky()");
		}
		else
		{
			//增加幸运值
			uint32_t add_lucky = p_equip->get_equip_lucky();
			equip_lucky += add_lucky;
			if (equip_lucky > get_equip_make_lucky_max())
			{
				equip_lucky = get_equip_make_lucky_max();
			}
			p_role->set_equip_lucky(equip_lucky);
			EQUIP_LOG("add_lucky[%d] set_equip_lucky[%d]", add_lucky, equip_lucky);
		}
	}
	proto::common::personal_info* p_info = p_data->mutable_per_info();
	p_role->get_personal_info(p_info);
	return errcode_enum::error_ok;
}

uint32_t equip_manager_t::equip_on(role_ptr p_role, uint64_t equip_uid, uint64_t hero_uid, proto::common::role_change_data* p_data)
{
	if (NULL == p_role)
	{
		log_error("NULL == p_role");
		return errcode_enum::notice_role_null;
	}
	if (NULL == p_data)
	{
		log_error("NULL == p_data");
		return errcode_enum::notice_unknown;
	}
	item_ptr p_equip = get_equip_by_uid(p_role, equip_uid);
	if (NULL == p_equip)
	{
		log_error("p_role[%lu] NULL == p_equip[%lu]", p_role->get_uid(), equip_uid);
		return errcode_enum::notice_item_null;
	}
	auto p_item_conf = GET_CONF(Item, p_equip->get_tid());
	if (NULL == p_item_conf)
	{
		log_error("NULL == p_item_conf[%d]", p_equip->get_tid());
		return errcode_enum::notice_unknown;
	}
	auto p_equip_conf = GET_CONF(Equip, p_equip->get_tid());
	if (NULL == p_equip_conf)
	{
		log_error("NULL == p_equip_conf[%d]", p_equip->get_tid());
		return errcode_enum::notice_unknown;
	}
	hero_ptr p_hero = hero_manager_t::get_hero_by_unique_id(p_role, hero_uid);
	if (NULL == p_hero)
	{
		log_error("role[%lu] NULL == p_hero[%lu]", p_role->get_uid(), hero_uid);
		return errcode_enum::notice_hero_null;
	}
	auto p_hero_config = GET_CONF(Hero, p_hero->get_tid());
	if (NULL == p_hero_config)
	{
		log_error("p_hero_config[%d] not found!!", p_hero->get_tid());
		return errcode_enum::notice_unknown;
	}
	if (p_role->get_level() < p_equip_conf->need_level())
	{
		log_error("role[%lu] hero[%d] user_level[%d] not enough p_equip_conf[%d] need_level[%d]", p_role->get_uid(), 
			p_hero->get_tid(), p_role->get_level(), p_equip->get_tid(), p_equip_conf->need_level());
		return errcode_enum::notice_talent_tips5;
	}
	//const std::vector<uint32_t>& job_vec = p_equip_conf->job();
	//uint32_t profession = p_hero_config->profession();
	//if (std::find(job_vec.begin(), job_vec.end(), profession) == job_vec.end())
	//{
	//	log_error("p_hero[%llu] profession[%d] invalid, p_equip_conf[%d]", hero_uid, profession, p_equip->get_tid());
	//	return errcode_enum::notice_talent_tips5;
	//}
	uint32_t pos = p_equip_conf->equip_type();
	uint64_t exist_equip_uid = p_hero->get_equip(pos);
	EQUIP_LOG("role[%lu] hero[%d] exist_equip_uid[%llu]", p_role->get_uid(), p_hero->get_tid(), exist_equip_uid);
	if (exist_equip_uid > 0)
	{
		item_ptr p_exist_equip = get_equip_by_uid(p_role, exist_equip_uid);
		if (NULL == p_exist_equip)
		{
			log_error("role[%lu] p_exist_equip[%lu] not found", p_role->get_uid(), exist_equip_uid);
			return errcode_enum::notice_army_not_exist;
		}
		p_exist_equip->set_hero_uid(0);
		p_hero->equip_off(pos);

		proto::common::item_data* p_item_data = p_data->mutable_item();
		proto::common::item_single* p_item_single = p_item_data->add_item_list();
		p_exist_equip->peek_data(p_item_single);
		EQUIP_LOG("role[%lu] hero[%d] equip_off[%llu]", p_role->get_uid(), p_hero->get_tid(), exist_equip_uid);
		log_wrapper_t::send_equip_log(p_role->get_uid(), p_role->get_level(), p_exist_equip, log_enum::opt_type_set, log_enum::source_type_equip_off, 0);
	}
	p_equip->set_hero_uid(hero_uid);
	p_equip->set_state(item_state_bind);
	p_equip->save_self(p_role->get_uid());
	proto::common::item_data* p_item_data = p_data->mutable_item();
	proto::common::item_single* p_item_single = p_item_data->add_item_list();
	p_equip->peek_data(p_item_single);
	log_wrapper_t::send_equip_log(p_role->get_uid(), p_role->get_level(), p_equip, log_enum::opt_type_set, log_enum::source_type_equip_on, 0);
	
	p_hero->equip_on(pos, equip_uid);
	p_hero->save_self(p_role->get_uid());
	proto::common::hero_data* p_hero_data = p_data->mutable_hero();
	proto::common::hero_single* p_hero_single = p_hero_data->add_hero_list();
	p_hero->peek_data(p_hero_single);
	auto p_task_mgr = p_role->get_task_mgr();
	if (NULL != p_task_mgr)
	{
		uint32_t hero_count = hero_manager_t::get_hero_full_equip_count(p_role);
		p_task_mgr->try_change_task_state(TASK_TARGET_TYPE_HERO_EQUIP, *p_data, 1, hero_count, false);
	}
	EQUIP_LOG("role[%lu] equip_on[%llu:%d] hero[%d]", p_role->get_uid(), equip_uid, p_equip->get_tid(), p_hero->get_tid());

	if (p_role->m_formation_mgr.is_in_cur_form(hero_uid))
	{
		p_role->update_role_info_to_center(proto::common::simple_info_type);
	}
	return errcode_enum::error_ok;
}


uint32_t equip_manager_t::equip_off(role_ptr p_role, uint64_t equip_uid, uint64_t hero_uid, proto::common::role_change_data* p_data)
{
	if (NULL == p_role)
	{
		log_error("NULL == p_role");
		return errcode_enum::notice_role_null;
	}
	if (NULL == p_data)
	{
		log_error("NULL == p_data");
		return errcode_enum::notice_unknown;
	}
	item_ptr p_equip = get_equip_by_uid(p_role, equip_uid);
	if (NULL == p_equip)
	{
		log_error("role[%lu] p_equip[%lu] not found", p_role->get_uid(), equip_uid);
		return errcode_enum::notice_item_null;
	}
	auto p_equip_conf = GET_CONF(Equip, p_equip->get_tid());
	if (NULL == p_equip_conf)
	{
		log_error("NULL == p_equip_conf[%d]", p_equip->get_tid());
		return errcode_enum::notice_unknown;
	}
	hero_ptr p_hero = hero_manager_t::get_hero_by_unique_id(p_role, hero_uid);
	if (NULL == p_hero)
	{
		log_error("role[%lu] NULL == p_hero[%lu]", p_role->get_uid(), hero_uid);
		return errcode_enum::notice_hero_null;
	}
	uint32_t pos = p_equip_conf->equip_type();
	uint64_t exist_equip_uid = p_hero->get_equip(pos);
	if (exist_equip_uid != equip_uid)
	{
		log_error("role[%lu] hero[%d] exist_equip_uid[%llu] != equip_uid[%llu]", p_role->get_uid(), p_hero->get_tid(), exist_equip_uid, equip_uid);
		return errcode_enum::notice_army_not_exist;
	}
	p_equip->set_hero_uid(0);
	p_equip->save_self(p_role->get_uid());
	proto::common::item_data* p_item_data = p_data->mutable_item();
	proto::common::item_single* p_item_single = p_item_data->add_item_list();
	p_equip->peek_data(p_item_single);
	log_wrapper_t::send_equip_log(p_role->get_uid(), p_role->get_level(), p_equip, log_enum::opt_type_set, log_enum::source_type_equip_off, 0);

	p_hero->equip_off(pos);
	p_hero->save_self(p_role->get_uid());
	proto::common::hero_data* p_hero_data = p_data->mutable_hero();
	proto::common::hero_single* p_hero_single = p_hero_data->add_hero_list();
	p_hero->peek_data(p_hero_single);
	EQUIP_LOG("role[%lu] hero[%d] equip_off[%llu:%d] success", p_role->get_uid(), p_hero->get_tid(), equip_uid, p_equip->get_tid());
	return errcode_enum::error_ok;
}


uint32_t equip_manager_t::remake_attr(role_ptr p_role, uint64_t equip_uid, uint32_t gold_flag, proto::common::role_change_data* p_data)
{
	if (NULL == p_role)
	{
		log_error("NULL == p_role");
		return errcode_enum::notice_role_null;
	}
	if (NULL == p_data)
	{
		log_error("NULL == p_data");
		return errcode_enum::notice_unknown;
	}
	item_ptr p_equip = get_equip_by_uid(p_role, equip_uid);
	if (NULL == p_equip)
	{
		log_error("role[%lu] p_equip[%lu] not found", p_role->get_uid(), equip_uid);
		return errcode_enum::notice_item_null;
	}
	auto p_equip_conf = GET_CONF(Equip, p_equip->get_tid());
	if (NULL == p_equip_conf)
	{
		log_error("NULL == p_equip_conf[%d]", p_equip->get_tid());
		return errcode_enum::notice_unknown;
	}
	if (p_equip_conf->can_remake() > 0)
	{
		log_error("role[%lu] p_equip[%lu:%d] cannot remake", p_role->get_uid(), equip_uid, p_equip->get_tid());
		return errcode_enum::notice_unknown;
	}
	std::map<uint32_t, uint32_t> attr_map;
	make_attr(p_equip_conf, attr_map);
	p_equip->set_attr(attr_map);
	p_equip->calc_score();
	p_equip->check_rarity();
	p_equip->save_self(p_role->get_uid());
	uint64_t hero_uid = p_equip->get_hero_uid();
	if (hero_uid > 0)
	{
		hero_ptr p_hero = hero_manager_t::get_hero_by_unique_id(p_role, hero_uid);
		if (NULL != p_hero)
		{
			p_hero->calc_fighting();
		}
	}
	log_wrapper_t::send_equip_log(p_role->get_uid(), p_role->get_level(), p_equip, log_enum::opt_type_set, log_enum::source_type_equip_remake_attr, 0);
	proto::common::item_data* p_item_data = p_data->mutable_item();
	proto::common::item_single* p_item_single = p_item_data->add_item_list();
	p_equip->peek_data(p_item_single);
	proto::common::personal_info* p_info = p_data->mutable_per_info();
	p_role->get_personal_info(p_info);
	EQUIP_LOG("role[%lu] remake_attr[%llu:%d] success", p_role->get_uid(), equip_uid, p_equip->get_tid());

	//成就通知
	achieve_common_t::notify_progress_state(p_role->get_uid(), proto::common::Achieve_Event_Type::ACHIEVE_EQUIP_RECAST);
	return errcode_enum::error_ok;
}

uint32_t equip_manager_t::remake_exattr(role_ptr p_role, uint64_t equip_uid, const std::vector<uint32_t>& lock_vec, proto::common::role_change_data* p_data)
{
	if (NULL == p_role)
	{
		log_error("NULL == p_role");
		return errcode_enum::notice_role_null;
	}
	if (NULL == p_data)
	{
		log_error("NULL == p_data");
		return errcode_enum::notice_unknown;
	}
	item_ptr p_equip = get_equip_by_uid(p_role, equip_uid);
	if (NULL == p_equip)
	{
		log_error("role[%lu] p_equip[%lu] not found", p_role->get_uid(), equip_uid);
		return errcode_enum::notice_item_null;
	}
	auto p_equip_conf = GET_CONF(Equip, p_equip->get_tid());
	if (NULL == p_equip_conf)
	{
		log_error("NULL == p_equip_conf[%d]", p_equip->get_tid());
		return errcode_enum::notice_unknown;
	}
	if (p_equip_conf->can_remake() > 0)
	{
		log_error("role[%lu] p_equip[%lu:%d] cannot remake", p_role->get_uid(), equip_uid, p_equip->get_tid());
		return errcode_enum::notice_unknown;
	}
	auto p_item_conf = GET_CONF(Item, p_equip->get_tid());
	if (NULL == p_item_conf)
	{
		log_error("NULL == p_item_conf[%d]", p_equip->get_tid());
		return errcode_enum::notice_unknown;
	}
	auto p_equip_exattr_make_config = GET_CONF(EquipExattrMake, p_item_conf->quality());
	if (NULL == p_equip_exattr_make_config)
	{
		log_error("EquipExattrMake quality[%d] FindCnf not found", p_item_conf->quality());
		return errcode_enum::notice_unknown;
	}
	if (lock_vec.size() >= p_equip_exattr_make_config->count())
	{
		log_error("role[%lu] lock_vec_size[%d] invalid, >=[%d]", p_role->get_uid(), lock_vec.size(), p_equip_exattr_make_config->count());
		return errcode_enum::notice_unknown;
	}
	uint32_t count = p_equip_exattr_make_config->count() - lock_vec.size();
	for (auto index : lock_vec)
	{
		if (index >= p_equip_exattr_make_config->count())
		{
			log_error("role[%lu] lock_vec has invalid index[%d], >=[%d]", p_role->get_uid(), index, p_equip_exattr_make_config->count());
			return errcode_enum::notice_unknown;
		}
	}
	uint32_t remake_stone = get_remake_stone();
	uint32_t lock_material = get_lock_material();
	auto p_equip_exattr_remake_config = GET_CONF(EquipExattrRemake, lock_vec.size());
	if (NULL == p_equip_exattr_remake_config)
	{
		log_error("EquipExattrRemake[%d] FindCnf not found", lock_vec.size());
		return errcode_enum::notice_unknown;
	}
	std::map<uint32_t, uint32_t> item_id_counts;
	if (p_equip_exattr_remake_config->material_count() > 0)
	{
		item_id_counts[remake_stone] = p_equip_exattr_remake_config->material_count();
	}
	if (p_equip_exattr_remake_config->lock_count() > 0)
	{
		item_id_counts[lock_material] = p_equip_exattr_remake_config->lock_count();
	}
	uint32_t cost_money = p_equip_exattr_remake_config->money();
	
	if (!item_manager_t::has_items(p_role, item_id_counts))
	{
		log_error("p_role[%llu] equip_remake_exattr item is not enough!!", p_role->get_uid());
		return errcode_enum::notice_item_not_enough;
	}
	if (!money_manager_t::use_money(p_role, proto::common::MONEY_TYPE_COPPER, cost_money, log_enum::source_type_equip_remake_exattr, 0, true, p_data))
	{
		log_error("p_role[%llu] equip_remake_exattr copper[%d] not enough", p_role->get_uid(), cost_money);
		return errcode_enum::notice_copper_money_not_enough;
	}
	item_manager_t::remove_items(p_role, item_id_counts, log_enum::source_type_equip_remake_exattr, p_equip->get_tid(), p_data);
	
	std::vector<uint32_t> tmp_vec;
	make_exattr(count, p_equip_conf->exattr_pool(), tmp_vec);
	
	if (lock_vec.empty())
	{
		p_equip->set_remake_exattr(tmp_vec);
	}
	else
	{
		std::vector<uint32_t> exattr_vec;
		uint32_t index = 0;
		for (uint32_t i = 0; i < p_equip_exattr_make_config->count(); ++i)
		{
			if (std::find(lock_vec.begin(), lock_vec.end(), i) == lock_vec.end())
			{
				if (index < tmp_vec.size())
				{
					exattr_vec.push_back(tmp_vec.at(index++));
				}
			}
			else
			{
				exattr_vec.push_back(p_equip->get_exattr(i));
			}
		}
		p_equip->set_remake_exattr(exattr_vec);
	}
	p_equip->set_remake_material(p_equip->get_remake_material() + p_equip_exattr_remake_config->material_count());
	p_equip->set_lock_material(p_equip->get_lock_material() + p_equip_exattr_remake_config->lock_count());
	p_equip->save_self(p_role->get_uid());
	log_wrapper_t::send_equip_log(p_role->get_uid(), p_role->get_level(), p_equip, log_enum::opt_type_set, log_enum::source_type_equip_remake_exattr, 0);
	proto::common::item_data* p_item_data = p_data->mutable_item();
	proto::common::item_single* p_item_single = p_item_data->add_item_list();
	p_equip->peek_data(p_item_single);
	proto::common::personal_info* p_info = p_data->mutable_per_info();
	p_role->get_personal_info(p_info);
	EQUIP_LOG("role[%lu] remake_exattr[%llu:%d] success", p_role->get_uid(), equip_uid, p_equip->get_tid());

	//成就通知
	achieve_common_t::notify_progress_state(p_role->get_uid(), proto::common::Achieve_Event_Type::ACHIEVE_EQUIP_RECAST);
	return errcode_enum::error_ok;
}

uint32_t equip_manager_t::replace_exattr(role_ptr p_role, uint64_t equip_uid, proto::common::role_change_data* p_data)
{
	if (NULL == p_role)
	{
		log_error("NULL == p_role");
		return errcode_enum::notice_role_null;
	}
	if (NULL == p_data)
	{
		log_error("NULL == p_data");
		return errcode_enum::notice_unknown;
	}
	item_ptr p_equip = get_equip_by_uid(p_role, equip_uid);
	if (NULL == p_equip)
	{
		log_error("role[%lu] p_equip[%lu] not found", p_role->get_uid(), equip_uid);
		return errcode_enum::notice_item_null;
	}
	p_equip->replace_exattr();
	p_equip->calc_score();
	p_equip->save_self(p_role->get_uid());
	uint64_t hero_uid = p_equip->get_hero_uid();
	if (hero_uid > 0)
	{
		hero_ptr p_hero = hero_manager_t::get_hero_by_unique_id(p_role, hero_uid);
		if (NULL != p_hero)
		{
			p_hero->calc_fighting();
		}
	}
	log_wrapper_t::send_equip_log(p_role->get_uid(), p_role->get_level(), p_equip, log_enum::opt_type_set, log_enum::source_type_equip_replace_exattr, 0);
	proto::common::item_data* p_item_data = p_data->mutable_item();
	proto::common::item_single* p_item_single = p_item_data->add_item_list();
	p_equip->peek_data(p_item_single);
	EQUIP_LOG("role[%lu] replace_exattr[%llu:%d] success", p_role->get_uid(), equip_uid, p_equip->get_tid());
	return errcode_enum::error_ok;
}

uint32_t equip_manager_t::equip_smelt(role_ptr p_role, const std::vector<uint64_t>& equip_list, proto::common::role_change_data* p_data)
{
	if (NULL == p_role)
	{
		log_error("NULL == p_role");
		return errcode_enum::notice_role_null;
	}
	if (NULL == p_data)
	{
		log_error("NULL == p_data");
		return errcode_enum::notice_unknown;
	}
	EQUIP_LOG("role[%lu] equip_smelt num[%d]", p_role->get_uid(), equip_list.size());
	std::map<uint32_t, uint32_t> item_id_count;
	for (auto equip_uid : equip_list)
	{
		item_ptr p_equip = get_equip_by_uid(p_role, equip_uid);
		if (NULL == p_equip)
		{
			log_error("role[%lu] NULL == p_equip[%lu]", p_role->get_uid(), equip_uid);
			return errcode_enum::notice_item_null;
		}
		auto p_equip_conf = GET_CONF(Equip, p_equip->get_tid());
		if (NULL == p_equip_conf)
		{
			log_error("NULL == p_equip_conf[%d]", p_equip->get_tid());
			return errcode_enum::notice_unknown;
		}
		if (p_equip_conf->can_smelt() > 0)
		{
			log_error("role[%lu] p_equip[%lu:%d] cannot smelt", p_role->get_uid(), equip_uid, p_equip->get_tid());
			return errcode_enum::notice_unknown;
		}
		uint32_t smelt_id = p_equip_conf->smelt_id();
		auto p_equip_smelt_conf = GET_CONF(EquipSmelt, smelt_id);
		if (NULL == p_equip_smelt_conf)
		{
			log_error("NULL == p_equip_smelt_conf[%d]", smelt_id);
			return errcode_enum::notice_unknown;
		}
		auto p_equip_star_conf = GET_CONF(EquipStar, p_equip_conf->star());
		if (NULL == p_equip_star_conf)
		{
			log_error("NULL == p_equip_star_conf[%d]", p_equip_conf->star());
			return errcode_enum::notice_unknown;
		}
		std::map<uint32_t, uint32_t> item_id_count;
		uint32_t strengthen_material = get_strengthen_material();
		uint32_t strengthen_material_ex = get_strengthen_material_ex();
		uint32_t strengthen_material_count = p_equip->get_strengthen_material() * p_equip_star_conf->strengthen_limit() / 10000;
		uint32_t strengthen_material_ex_count = p_equip->get_strengthen_material_ex() * p_equip_star_conf->strengthen_limit() / 10000;
		uint32_t strengthen_money = p_equip->get_strengthen_money() * p_equip_star_conf->strengthen_limit() / 10000;
		item_id_count[strengthen_material] += strengthen_material_count;
		item_id_count[strengthen_material_ex] += strengthen_material_ex_count;
		uint32_t remake_stone = get_remake_stone();
		uint32_t lock_material = get_lock_material();
		uint32_t remake_material_count = p_equip->get_remake_material() * p_equip_star_conf->remake_limit() / 10000;
		uint32_t lock_material_count = p_equip->get_lock_material() * p_equip_star_conf->remake_limit() / 10000;
		item_id_count[remake_stone] += remake_material_count;
		item_id_count[lock_material] += lock_material_count;

		uint32_t reply_code = item_manager_t::remove_item(p_role, equip_uid, 1, log_enum::source_type_equip_smelt, 0, p_data);
		if (reply_code != errcode_enum::error_ok)
		{
			log_error("role[%lu] remove_item[%lu] error_code[%d]", p_role->get_uid(), equip_uid, reply_code);
			return reply_code;
		}
		item_manager_t::add_items(p_role, item_id_count, log_enum::source_type_equip_smelt, 0, p_data);
		money_manager_t::add_money(p_role, proto::common::MONEY_TYPE_COPPER, strengthen_money, log_enum::source_type_equip_smelt, 0, true, p_data);
		drop_manager_t::drop(p_role, p_equip_smelt_conf->drop_id(), log_enum::source_type_equip_smelt_box, proto::common::drop_sys_type_equip, p_equip->get_tid(), p_data);
		EQUIP_LOG("role[%lu] smelt equip[%lu:%d] success", p_role->get_uid(), equip_uid, p_equip_conf->id());
	}
	
	p_role->add_equip_smelt_value(equip_list.size());
	p_role->save_self();
	proto::common::personal_info* p_info = p_data->mutable_per_info();
	p_role->get_personal_info(p_info);

	proto::client::gc_equip_smelt_data_reply reply;
	peek_equip_smelt_data(p_role, reply);
	p_role->send_msg_to_client(op_cmd::gc_equip_smelt_data_reply, reply);
	EQUIP_LOG("role[%lu] equip_smelt num[%d] success", p_role->get_uid(), equip_list.size());
	return errcode_enum::error_ok;
}


void equip_manager_t::peek_equip_smelt_data(role_ptr p_role, proto::client::gc_equip_smelt_data_reply& reply)
{
	if (NULL == p_role)
	{
		log_error("NULL == p_role");
		return;
	}
	uint32_t equip_smelt_value = p_role->get_equip_smelt_value();
	reply.set_equip_smelt_value(equip_smelt_value);
	const std::vector<uint32_t> equip_smelt_box_vec = p_role->get_equip_smelt_box_vec();
	for (auto it : s_box_info)
	{
		uint32_t value = it.first;
		proto::common::equip_smelt_box* p_equip_smelt_box = reply.add_box_list();
		p_equip_smelt_box->set_equip_smelt_value(value);
		std::vector<uint32_t>::const_iterator it_vec = std::find(equip_smelt_box_vec.begin(), equip_smelt_box_vec.end(), value);
		if (it_vec != equip_smelt_box_vec.end())
		{
			p_equip_smelt_box->set_state(equip_smelt_box_state_over);
		}
		else
		{
			if (equip_smelt_value >= value)
			{
				p_equip_smelt_box->set_state(equip_smelt_box_state_finish);
			}
			else
			{
				p_equip_smelt_box->set_state(equip_smelt_box_state_none);
			}
		}
	}
}


uint32_t equip_manager_t::equip_smelt_box(role_ptr p_role, uint32_t box_id, proto::common::role_change_data* p_data)
{
	if (NULL == p_role)
	{
		log_error("NULL == p_role");
		return errcode_enum::notice_role_null;
	}
	if (NULL == p_data)
	{
		log_error("NULL == p_data");
		return errcode_enum::notice_unknown;
	}
	uint32_t equip_smelt_value = p_role->get_equip_smelt_value();
	if (equip_smelt_value < box_id)
	{
		log_error("role[%llu] equip_smelt_value[%d] not enough need[%d]", p_role->get_uid(), p_role->get_equip_smelt_value(), box_id);
		return errcode_enum::notice_unknown;
	}
	const std::vector<uint32_t>& equip_smelt_box_vec = p_role->get_equip_smelt_box_vec();
	std::vector<uint32_t>::const_iterator it_vec = std::find(equip_smelt_box_vec.begin(), equip_smelt_box_vec.end(), box_id);
	if (it_vec != equip_smelt_box_vec.end())
	{
		log_error("role[%lu] equip_smelt_box[%d] already get", p_role->get_uid(), box_id);
		return errcode_enum::notice_unknown;
	}
	std::map<uint32_t, uint32_t>::iterator it_map = s_box_info.find(box_id);
	if (it_map == s_box_info.end())
	{
		log_error("role[%lu] equip_smelt_box[%d] invalid", p_role->get_uid(), box_id);
		return errcode_enum::notice_unknown;
	}
	
	p_role->add_equip_smelt_box(box_id);
	uint32_t drop_id = it_map->second;
	drop_manager_t::drop(p_role, drop_id, log_enum::source_type_equip_smelt_box, proto::common::drop_sys_type_equip, box_id, p_data);

	p_role->save_self();
	proto::common::personal_info* p_info = p_data->mutable_per_info();
	p_role->get_personal_info(p_info);

	proto::client::gc_equip_smelt_data_reply reply;
	peek_equip_smelt_data(p_role, reply);
	p_role->send_msg_to_client(op_cmd::gc_equip_smelt_data_reply, reply);
	EQUIP_LOG("role[%lu] equip_smelt_box[%d] success", p_role->get_uid(), box_id);
	return errcode_enum::error_ok;
}


uint32_t equip_manager_t::equip_strengthen(role_ptr p_role, uint64_t equip_uid, uint32_t count, proto::common::role_change_data* p_data)
{
	if (NULL == p_role)
	{
		log_error("NULL == p_role");
		return errcode_enum::notice_role_null;
	}
	if (NULL == p_data)
	{
		log_error("NULL == p_data");
		return errcode_enum::notice_unknown;
	}
	item_ptr p_equip = get_equip_by_uid(p_role, equip_uid);
	if (NULL == p_equip)
	{
		log_error("role[%lu] p_equip[%lu] not found", p_role->get_uid(), equip_uid);
		return errcode_enum::notice_item_null;
	}
	EQUIP_LOG("role[%lu] equip_strengthen[%llu:%d] count[%d]", p_role->get_uid(), equip_uid, p_equip->get_tid(), count);
	uint32_t old_level = p_equip->get_strengthen_level();
	uint32_t real_count = 0;
	uint32_t reply_code = errcode_enum::error_ok;
	for (uint32_t i = 0; i < count; ++i)
	{
		reply_code = equip_strengthen_single(p_role, p_equip, p_data);
		if (errcode_enum::error_ok == reply_code)
		{
			++real_count;
		}
		else
		{
			break;
		}
	}
	if (real_count > 0)
	{
		reply_code = errcode_enum::error_ok;
		p_equip->set_state(item_state_bind);
		p_equip->calc_score();
		p_equip->check_rarity();
		p_equip->save_self(p_role->get_uid());
		uint64_t hero_uid = p_equip->get_hero_uid();
		if (hero_uid > 0)
		{
			hero_ptr p_hero = hero_manager_t::get_hero_by_unique_id(p_role, hero_uid);
			if (NULL != p_hero)
			{
				p_hero->calc_fighting();
				proto::common::hero_data* p_hero_data = p_data->mutable_hero();
				proto::common::hero_single* p_hero_single = p_hero_data->add_hero_list();
				p_hero->peek_data(p_hero_single);
			}
		}
		proto::common::item_data* p_item_data = p_data->mutable_item();
		proto::common::item_single* p_item_single = p_item_data->add_item_list();
		p_equip->peek_data(p_item_single);
		log_wrapper_t::send_equip_log(p_role->get_uid(), p_role->get_level(), p_equip, log_enum::opt_type_set, log_enum::source_type_equip_strengthen, 0);
		auto p_task_mgr = p_role->get_task_mgr();
		if (NULL != p_task_mgr)
		{
			uint32_t equip_count = item_manager_t::get_equip_strengthen_count(p_role, 5);
			p_task_mgr->try_change_task_state(TASK_TARGET_TYPE_EQUIP_STRENGTHEN_5, *p_data, 1, equip_count, false);
			equip_count = item_manager_t::get_equip_strengthen_count(p_role, 10);
			p_task_mgr->try_change_task_state(TASK_TARGET_TYPE_EQUIP_STRENGTHEN_10, *p_data, 1, equip_count, false);
		}
		EQUIP_LOG("role[%lu] equip_strengthen[%llu:%d] real_count[%d] strengthen_level from[%d] to [%d]", 
			p_role->get_uid(), equip_uid, p_equip->get_tid(), real_count, old_level, p_equip->get_strengthen_level());
	}
	return reply_code;
}

void equip_manager_t::make_attr(Equip* p_equip_conf, std::map<uint32_t, uint32_t>& attr_map)
{
	if (NULL == p_equip_conf)
	{
		return;
	}
	//EQUIP_LOG("equip[%d] make_attr begin", p_equip_conf->id());
	attr_map.insert(std::make_pair(attrtype_enum::atk, p_equip_conf->base_atk()));
	//EQUIP_LOG("atk[%d]", p_equip_conf->base_atk());
	attr_map.insert(std::make_pair(attrtype_enum::def_physical, p_equip_conf->base_def()));
	//EQUIP_LOG("def[%d]", p_equip_conf->base_def());
	attr_map.insert(std::make_pair(attrtype_enum::max_hp, p_equip_conf->base_hp()));
	//EQUIP_LOG("hp[%d]", p_equip_conf->base_hp());
	//EQUIP_LOG("equip[%d] make_attr end", p_equip_conf->id());
}

void equip_manager_t::make_exattr(uint32_t count, uint32_t exattr_pool, std::vector<uint32_t>& exattr_vec)
{
	//EQUIP_LOG("exattr_pool[%d] make_exattr begin", exattr_pool);

	EquipExattrPoolMap::iterator it = equip_exattr_pool_map.find(exattr_pool);
	if (it == equip_exattr_pool_map.end())
	{
		log_error("NULL == p_equip_exattr_conf[%d]", exattr_pool);
		return;
	}
	const EquipExattrVec& eev = it->second;
	uint32_t rand_max = 0;
	for (auto p_exattr_conf : eev)
	{
		if (NULL == p_exattr_conf)
		{
			continue;
		}
		rand_max += p_exattr_conf->prob();
	}
	if (0 == rand_max)
	{
		log_error("exattr_pool[%d] 0 == rand_max", exattr_pool);
		return;
	}
	for (uint32_t i = 0; i < count; ++i)
	{
		int32_t rand_val = random_util_t::randMin(0, rand_max);
		if (-1 == rand_val)
		{
			log_error("exattr_pool[%d] rand_max invalid", exattr_pool);
			return;
		}
		for (auto p_exattr_conf : eev)
		{
			if (NULL == p_exattr_conf)
			{
				continue;
			}
			if ((uint32_t)rand_val < p_exattr_conf->prob())
			{
				//EQUIP_LOG("exattr[%d]", p_exattr_conf->id());
				exattr_vec.push_back(p_exattr_conf->id());
				break;
			}
			else
			{
				rand_val -= p_exattr_conf->prob();
			}
		}
	}
	//EQUIP_LOG("exattr_pool[%d] make_exattr end", exattr_pool);
}


uint32_t equip_manager_t::make_name(uint32_t name_pool, uint32_t prob)
{
	//EQUIP_LOG("name_pool[%d] make_name begin", name_pool);
	uint32_t name_id = 0;
	if (!random_util_t::selectByTenTh(prob))
	{
		return name_id;
	}
	EquipNamePoolMap::iterator it = equip_name_pool_map.find(name_pool);
	if (it == equip_name_pool_map.end())
	{
		log_error("NULL == p_equip_name_conf[%d]", name_pool);
		return name_id;
	}
	const EquipNameVec& env = it->second;
	uint32_t rand_max = 0;
	for (auto p_name_conf : env)
	{
		if (NULL == p_name_conf)
		{
			continue;
		}
		rand_max += p_name_conf->prob();
	}
	if (0 == rand_max)
	{
		log_error("name_pool[%d] 0 == rand_max", name_pool);
		return name_id;
	}
	int32_t rand_val = random_util_t::randMin(0, rand_max);
	if (-1 == rand_val)
	{
		log_error("name_pool[%d] rand_max invalid", name_pool);
		return name_id;
	}
	for (auto p_name_conf : env)
	{
		if (NULL == p_name_conf)
		{
			continue;
		}
		if ((uint32_t)rand_val < p_name_conf->prob())
		{
			//EQUIP_LOG("name[%d]", p_name_conf->id());
			name_id = p_name_conf->id();
			break;
		}
		else
		{
			rand_val -= p_name_conf->prob();
		}
	}
	//EQUIP_LOG("name_pool[%d] make_name end", name_pool);
	return name_id;
}

uint32_t equip_manager_t::make_special(uint32_t special_pool, uint32_t prob)
{
	//EQUIP_LOG("special_pool[%d] make_special begin", special_pool);
	uint32_t special_id = 0;
	if (!random_util_t::selectByTenTh(prob))
	{
		return special_id;
	}
	EquipSpecialPoolMap::iterator it = equip_special_pool_map.find(special_pool);
	if (it == equip_special_pool_map.end())
	{
		log_error("NULL == p_equip_special_conf[%d]", special_pool);
		return special_id;
	}
	const EquipSpecialVec& esv = it->second;
	uint32_t rand_max = 0;
	for (auto p_special_conf : esv)
	{
		if (NULL == p_special_conf)
		{
			continue;
		}
		rand_max += p_special_conf->prob();
	}
	if (0 == rand_max)
	{
		log_error("special_pool[%d] 0 == rand_max", special_pool);
		return special_id;
	}
	int32_t rand_val = random_util_t::randMin(0, rand_max);
	if (-1 == rand_val)
	{
		log_error("special_pool[%d] rand_max invalid", special_pool);
		return special_id;
	}
	for (auto p_special_conf : esv)
	{
		if (NULL == p_special_conf)
		{
			continue;
		}
		if ((uint32_t)rand_val < p_special_conf->prob())
		{
			//EQUIP_LOG("special[%d]", p_special_conf->id());
			special_id = p_special_conf->id();
			break;
		}
		else
		{
			rand_val -= p_special_conf->prob();
		}
	}
	//EQUIP_LOG("special_pool[%d] make_special end", special_pool);
	return special_id;
}

uint32_t equip_manager_t::get_equip_id_by_exchange_pool(uint32_t pool_id)
{
	EquipExchangePoolMap::iterator it = equip_exchange_pool_map.find(pool_id);
	if (it == equip_exchange_pool_map.end())
	{
		log_error("NULL == p_equip_exchange_pool_conf[%d]", pool_id);
		return 0;
	}
	const EquipExchangePoolVec& eev = it->second;
	
	uint32_t target_equip_id = 0;
	uint32_t rand_max = 0;
	for (auto p_equip_exchange_pool_conf : eev)
	{
		if (NULL == p_equip_exchange_pool_conf)
		{
			continue;
		}
		uint32_t weight = p_equip_exchange_pool_conf->weight();
		rand_max += weight;
	}
	if (0 == rand_max)
	{
		log_error("equip_exchange_pool_id[%d] 0 == rand_max", pool_id);
		return 0;
	}

	int32_t rand_val = random_util_t::randMin(0, rand_max);
	if (-1 == rand_val)
	{
		log_error("equip_exchange_pool_id[%d] rand_max invalid", pool_id);
		return 0;
	}
	for (auto p_equip_exchange_pool_conf : eev)
	{
		if (NULL == p_equip_exchange_pool_conf)
		{
			continue;
		}
		uint32_t equip_id = p_equip_exchange_pool_conf->equip_id();
		uint32_t weight = p_equip_exchange_pool_conf->weight();
		if ((uint32_t)rand_val < weight)
		{
			target_equip_id = equip_id;
			break;
		}
		else
		{
			rand_val -= weight;
		}
	}
	return target_equip_id;
}

uint32_t equip_manager_t::get_remake_stone()
{
	auto remake_stone = GET_CONF(Comprehensive, comprehensive_common::normal_clear_mat);
	uint32_t val = GET_COMPREHENSIVE_VALUE_1(remake_stone);
	return val;
}

uint32_t equip_manager_t::get_lock_material()
{
	 auto lock_material = GET_CONF(Comprehensive, comprehensive_common::lock_clear_mat);
	 uint32_t val = GET_COMPREHENSIVE_VALUE_1(lock_material);
	 return val;
}


uint32_t equip_manager_t::get_strengthen_material()
{
	auto lock_material = GET_CONF(Comprehensive, comprehensive_common::normal_str_mat);
	uint32_t val = GET_COMPREHENSIVE_VALUE_1(lock_material);
	return val;
}


uint32_t equip_manager_t::get_strengthen_material_ex()
{
	auto lock_material = GET_CONF(Comprehensive, comprehensive_common::special_str_mat);
	uint32_t val = GET_COMPREHENSIVE_VALUE_1(lock_material);
	return val;
}

uint32_t equip_manager_t::equip_strengthen_single(role_ptr p_role, item_ptr p_equip, proto::common::role_change_data* p_data)
{
	if (NULL == p_role)
	{
		log_error("NULL == p_role");
		return errcode_enum::notice_role_null;
	}
	if (NULL == p_data)
	{
		log_error("NULL == p_data");
		return errcode_enum::notice_unknown;
	}
	if (NULL == p_equip)
	{
		log_error("role[%lu] NULL == p_equip", p_role->get_uid());
		return errcode_enum::notice_item_null;
	}
	uint32_t strengthen_level = p_equip->get_strengthen_level();
	auto p_equip_conf = GET_CONF(Equip, p_equip->get_tid());
	if (NULL == p_equip_conf)
	{
		log_error("NULL == p_equip_conf[%d]", p_equip->get_tid());
		return errcode_enum::notice_unknown;
	}
	auto p_equip_strengthen_conf = GET_CONF(EquipStrengthen, strengthen_level + 1);
	if (NULL == p_equip_strengthen_conf)
	{
		log_error("NULL == p_equip_strengthen_conf[%d], maybe max", strengthen_level + 1);
		return errcode_enum::notice_unknown;
	}
	uint32_t equip_star = p_equip_conf->star();
	if (0 == equip_star)
	{
		log_error("p_equip_conf[%d] 0 == equip_star", p_equip->get_tid());
		return errcode_enum::notice_unknown;
	}
	auto p_equip_star_conf = GET_CONF(EquipStar, equip_star);
	if (NULL == p_equip_star_conf)
	{
		log_error("NULL == p_equip_star_conf[%d]", equip_star);
		return errcode_enum::notice_unknown;
	}
	if (strengthen_level >= p_equip_star_conf->level_limit())
	{
		log_error("role[%lu] equip[%lu:%d] strengthen_level[%d] star[%d] limit[%d]", p_role->get_uid(), p_equip->get_uid(),
			p_equip->get_tid(), strengthen_level, equip_star, p_equip_star_conf->level_limit());
		return errcode_enum::notice_unknown;
	}
	if (p_equip_strengthen_conf->material().size() < equip_star)
	{
		log_error("p_equip_strengthen_conf[%d] material error", strengthen_level + 1);
		return errcode_enum::notice_unknown;
	}
	uint32_t strengthen_material = get_strengthen_material();
	uint32_t strengthen_material_ex = get_strengthen_material_ex();
	std::map<uint32_t, uint32_t> item_id_counts;
	uint32_t strengthen_material_count = p_equip_strengthen_conf->material().at(equip_star - 1).second;
	uint32_t strengthen_material_ex_count = p_equip_strengthen_conf->material().at(equip_star - 1).third;
	uint32_t cost_money = p_equip_strengthen_conf->material().at(equip_star - 1).fourth;
	if (strengthen_material_count > 0)
	{
		item_id_counts[strengthen_material] = strengthen_material_count;
	}
	if (strengthen_material_ex_count > 0)
	{
		item_id_counts[strengthen_material_ex] = strengthen_material_ex_count;
	}
	if (!item_manager_t::has_items(p_role, item_id_counts))
	{
		log_error("p_role[%llu] equip_strengthen[%lu:%d] material not enough", p_role->get_uid(), p_equip->get_uid(), p_equip->get_tid());
		return errcode_enum::notice_item_not_enough;
	}
	if (!money_manager_t::use_money(p_role, proto::common::MONEY_TYPE_COPPER, cost_money, log_enum::source_type_equip_strengthen, 0, true, p_data))
	{
		log_error("p_role[%llu] equip_strengthen[%lu:%d] copper[%d] not enough", p_role->get_uid(), p_equip->get_uid(), p_equip->get_tid(), cost_money);
		return errcode_enum::notice_copper_money_not_enough;
}	
	item_manager_t::remove_items(p_role, item_id_counts, log_enum::source_type_equip_strengthen, p_equip->get_tid(), p_data);

	p_equip->set_strengthen_level(strengthen_level + 1);
	uint32_t strmaster_level = p_equip->get_strmaster_level();
	auto p_equip_strmaster_conf = GET_CONF(EquipStrengthenMaster, strmaster_level+1);
	if (NULL != p_equip_strmaster_conf)
	{
		if (p_equip->get_strengthen_level() >= p_equip_strmaster_conf->equip_level())
		{
			p_equip->set_strmaster_level(strmaster_level + 1);
		}
	}
	p_equip->set_strengthen_material(p_equip->get_strengthen_material() + strengthen_material_count);
	p_equip->set_strengthen_material_ex(p_equip->get_strengthen_material_ex() + strengthen_material_ex_count);
	p_equip->set_strengthen_money(p_equip->get_strengthen_money() + cost_money);

	achieve_common_t::notify_progress_state(p_role->get_uid(), proto::common::ACHIEVE_STRENGTHEN_EQUIP_NUMBER);
	return errcode_enum::error_ok;
}

uint32_t equip_manager_t::get_equip_make_lucky_max()
{
	auto equip_make_lucky_max = GET_CONF(Comprehensive, comprehensive_common::equip_make_lucky_max);
	uint32_t val = GET_COMPREHENSIVE_VALUE_1(equip_make_lucky_max);
	return val;
}


bool equip_manager_t::make_equip_info()
{
	clear_equip_info();
	if (!make_equip_exattr_pool_map())
	{
		log_error("make_equip_exattr_map error");
		return false;
	}
	if (!make_equip_exchange_pool_map())
	{
		log_error("make_equip_exchange_pool_map error");
		return false;
	}
	if (!make_equip_name_pool_map())
	{
		log_error("make_equip_name_pool_map error");
		return false;
	}
	if (!make_equip_special_pool_map())
	{
		log_error("make_equip_special_pool_map error");
		return false;
	}
	auto remake_stone_gold = GET_CONF(Comprehensive, comprehensive_common::recast_shop_id);
	uint32_t val = GET_COMPREHENSIVE_VALUE_1(remake_stone_gold);

	auto p_shop_conf = GET_CONF(ShopTable, val);
	if (NULL == p_shop_conf)
	{
		log_error("NULL == p_shop_conf[%d]", val);
		return false;
	}
	equip_remake_stone_gold = p_shop_conf->price();
	make_equip_smelt_box();
	return true;
}

void equip_manager_t::clear_equip_info()
{
	equip_exattr_pool_map.clear();
	equip_exchange_pool_map.clear();
	equip_name_pool_map.clear();
	equip_special_pool_map.clear();
	s_box_info.clear();
}


bool equip_manager_t::make_equip_exattr_pool_map()
{
	std::map<uint32_t, EquipExattr*> exattr_map;
	GET_ALL_CONF(EquipExattr, exattr_map);
	for (auto it : exattr_map)
	{
		EquipExattr* p_equip_exattr_conf = it.second;
		if (NULL == p_equip_exattr_conf)
		{
			continue;
		}
		uint32_t pool_id = p_equip_exattr_conf->exattr_pool();
		if (equip_exattr_pool_map.find(pool_id) == equip_exattr_pool_map.end())
		{
			EquipExattrVec eev;
			eev.push_back(p_equip_exattr_conf);
			equip_exattr_pool_map.insert(std::make_pair(pool_id, eev));
		}
		else
		{
			equip_exattr_pool_map[pool_id].push_back(p_equip_exattr_conf);
		}
	}
	EQUIP_LOG("make_equip_exattr_pool_map success");
	return true;
}


bool equip_manager_t::make_equip_exchange_pool_map()
{
	std::map<uint32_t, EquipExchangePool*> exchange_map;
	GET_ALL_CONF(EquipExchangePool, exchange_map);
	for (auto it : exchange_map)
	{
		EquipExchangePool* p_equip_exchange_conf = it.second;
		if (NULL == p_equip_exchange_conf)
		{
			continue;
		}
		uint32_t pool_id = p_equip_exchange_conf->pool();
		if (equip_exchange_pool_map.find(pool_id) == equip_exchange_pool_map.end())
		{
			EquipExchangePoolVec eev;
			eev.push_back(p_equip_exchange_conf);
			equip_exchange_pool_map.insert(std::make_pair(pool_id, eev));
		}
		else
		{
			equip_exchange_pool_map[pool_id].push_back(p_equip_exchange_conf);
		}
	}
	EQUIP_LOG("make_equip_exchange_pool_map success");
	return true;
}


bool equip_manager_t::make_equip_name_pool_map()
{
	std::map<uint32_t, EquipName*> name_map;
	GET_ALL_CONF(EquipName, name_map);
	for (auto it : name_map)
	{
		EquipName* p_equip_name_conf = it.second;
		if (NULL == p_equip_name_conf)
		{
			continue;
		}
		uint32_t pool_id = p_equip_name_conf->pool_id();
		if (equip_name_pool_map.find(pool_id) == equip_name_pool_map.end())
		{
			EquipNameVec env;
			env.push_back(p_equip_name_conf);
			equip_name_pool_map.insert(std::make_pair(pool_id, env));
		}
		else
		{
			equip_name_pool_map[pool_id].push_back(p_equip_name_conf);
		}
	}
	EQUIP_LOG("make_equip_name_pool_map success");
	return true;
}

bool equip_manager_t::make_equip_special_pool_map()
{
	std::map<uint32_t, EquipSpecial*> special_map;
	GET_ALL_CONF(EquipSpecial, special_map);
	for (auto it : special_map)
	{
		EquipSpecial* p_equip_special_conf = it.second;
		if (NULL == p_equip_special_conf)
		{
			continue;
		}
		uint32_t pool_id = p_equip_special_conf->pool_id();
		if (equip_special_pool_map.find(pool_id) == equip_special_pool_map.end())
		{
			EquipSpecialVec esv;
			esv.push_back(p_equip_special_conf);
			equip_special_pool_map.insert(std::make_pair(pool_id, esv));
		}
		else
		{
			equip_special_pool_map[pool_id].push_back(p_equip_special_conf);
		}
	}
	EQUIP_LOG("make_equip_special_pool_map success");
	return true;
}

void equip_manager_t::make_equip_smelt_box()
{
	auto p_first_box = GET_CONF(Comprehensive, comprehensive_common::equip_melt_chest_para1);
	uint32_t value1 = GET_COMPREHENSIVE_VALUE_1(p_first_box);
	uint32_t drop1 = GET_COMPREHENSIVE_VALUE_2(p_first_box);
	s_box_info.insert(std::make_pair(value1, drop1));
	auto p_second_box = GET_CONF(Comprehensive, comprehensive_common::equip_melt_chest_para2);
	uint32_t value2 = GET_COMPREHENSIVE_VALUE_1(p_second_box);
	uint32_t drop2 = GET_COMPREHENSIVE_VALUE_2(p_second_box);
	s_box_info.insert(std::make_pair(value2, drop2));
	auto p_third_box = GET_CONF(Comprehensive, comprehensive_common::equip_melt_chest_para3);
	uint32_t value3 = GET_COMPREHENSIVE_VALUE_1(p_third_box);
	uint32_t drop3 = GET_COMPREHENSIVE_VALUE_2(p_third_box);
	s_box_info.insert(std::make_pair(value3, drop3));
	auto p_fourth_box = GET_CONF(Comprehensive, comprehensive_common::equip_melt_chest_para4);
	uint32_t value4 = GET_COMPREHENSIVE_VALUE_1(p_fourth_box);
	uint32_t drop4 = GET_COMPREHENSIVE_VALUE_2(p_fourth_box);
	s_box_info.insert(std::make_pair(value4, drop4));
}
