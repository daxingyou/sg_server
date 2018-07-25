#include "holiday_manager.hpp"
#include "config_mgr.h"
#include "tblh/Holiday.tbls.h"
#include "time_manager.hpp"

USING_NS_COMMON;

std::map<uint32_t, uint32_t> holiday_manager_t::holiday_drop;
bool holiday_manager_t::init()
{
	holiday_drop.clear();
	std::map<uint32_t, Holiday*> all_holidays;
	GET_ALL_CONF(Holiday, all_holidays);
	for (auto it : all_holidays)
	{
		time_manager_t::register_func(it.first, holiday_manager_t::on_holiday);
	}
	return true;
}

void holiday_manager_t::on_holiday(uint32_t id, bool is_in_time)
{
	if (is_in_time)
	{
		auto p_conf = GET_CONF(Holiday, id);
		if (NULL == p_conf)
		{
			log_error("NULL == p_conf Holiday[%d]", id);
			return;
		}
		uint32_t drop_id = p_conf->drop_id();
		holiday_drop.insert(std::make_pair(id, drop_id));
		ACTIVITY_LOG("holiday[%d] open", id);
	}
	else
	{
		auto it = holiday_drop.find(id);
		if (it != holiday_drop.end())
		{
			holiday_drop.erase(it);
			ACTIVITY_LOG("holiday[%d] close", id);
		}
	}
}

void holiday_manager_t::drop(role_ptr p_role, std::map<uint32_t, drop_info> &drop_all_result)
{
	if (holiday_drop.empty())
	{
		return;
	}
	if (NULL == p_role)
	{
		log_error("holiday_manager drop p_role NULL");
		return;
	}
	std::set<uint32_t> drop_ids;
	for (auto it : holiday_drop)
	{
		uint32_t drop_bag_id = it.second;
		DropBagTable *p_drop_bag = GET_CONF(DropBagTable, drop_bag_id);
		if (NULL == p_drop_bag)
		{
			log_error("drop[%d] p_drop_bag NULL", drop_bag_id);
			continue;
		}
		for (uint32_t i = 0; i < p_drop_bag->drop_ids_size(); ++i)
		{
			drop_ids.clear();
			DropTableConfig_t::get_drop_ids_by_group_id(p_drop_bag->drop_ids(i), drop_ids);
			if (!drop_manager_t::get_drop_list(p_role, p_drop_bag, drop_ids, drop_all_result))
			{
				log_error("drop get_drop_list error drop_ids[%s] drop_result_size[%d]", p_drop_bag->base_drop_ids().c_str(), (uint32_t)drop_all_result.size());
				continue;
			}
			ITEM_LOG("role[%lu] start drop: drop_bag_id[%d] drop_group[%s]", p_role->get_uid(), p_drop_bag->id(), p_drop_bag->base_drop_ids().c_str());
		}
	}
}
