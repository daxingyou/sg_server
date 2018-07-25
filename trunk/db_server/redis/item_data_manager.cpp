#include "item_data_manager.hpp"
#include "redis_client.hpp"
#include "config/config_manager.hpp"
#include "tblh/Item.tbls.h"

item_data_manager::item_data_manager()
{
}

item_data_manager::~item_data_manager()
{
}

bool item_data_manager::save_item_data_to_redis(uint64_t uid, const proto::common::item_single& single)
{
	redis_client_t::set_uint32("item", "tid", single.uid(), single.tid());
	redis_client_t::set_uint64("item", "role_uid", single.uid(), uid);
	redis_client_t::set_uint32("item", "num", single.uid(), single.num());
	redis_client_t::set_uint32("item", "package", single.uid(), single.package());
	redis_client_t::set_uint32("item", "param", single.uid(), single.param());
	redis_client_t::set_uint32("item", "param2", single.uid(), single.param2());
	redis_client_t::set_string("item", "find_role", single.uid(), single.find_role());
	redis_client_t::set_uint32("item", "state", single.uid(), single.state());
	redis_client_t::set_uint32("item", "expired_time", single.uid(), single.expired_time());
	if (single.has_equip_info())
	{
		const proto::common::equip_single& es = single.equip_info();
		redis_client_t::set_uint64("item", "hero_uid", single.uid(), string_util_t::string_to_uint64(es.hero_uid()));

		std::ostringstream ss;
		//基础属性
		for (auto equip_attr : es.attr())
		{
			ss << equip_attr.type() << ":" << equip_attr.value() << "$";
		}
		std::string attr = ss.str().substr(0, ss.str().length() - 1);
		redis_client_t::set_string("item", "attr", single.uid(), attr);
		
		ss.str("");
		ss.clear();
		//判词
		for (int i = 0; i < es.exattr_size(); ++i)
		{
			ss << es.exattr(i) << "$";
		}
		std::string exattr = ss.str().substr(0, ss.str().length() - 1);
		redis_client_t::set_string("item", "exattr", single.uid(), exattr);
		
		ss.str("");
		ss.clear();
		//判词重铸
		for (int i = 0; i < es.remake_exattr_size(); ++i)
		{
			ss << es.remake_exattr(i) << "$";
		}
		std::string remake_exattr = ss.str().substr(0, ss.str().length() - 1);
		redis_client_t::set_string("item", "remake_exattr", single.uid(), remake_exattr);
		
		ss.str("");
		ss.clear();

		redis_client_t::set_uint32("item", "remake_exattr_bonus", single.uid(), es.remake_exattr_bonus());
		redis_client_t::set_uint32("item", "name", single.uid(), es.name());
		redis_client_t::set_uint32("item", "special", single.uid(), es.special());
		redis_client_t::set_uint32("item", "strengthen_level", single.uid(), es.strengthen_level());
		redis_client_t::set_uint32("item", "strmaster_level", single.uid(), es.strmaster_level());
		redis_client_t::set_uint32("item", "remake_material", single.uid(), es.remake_material());
		redis_client_t::set_uint32("item", "lock_material", single.uid(), es.lock_material());
		redis_client_t::set_uint32("item", "strengthen_material", single.uid(), es.strengthen_material());
		redis_client_t::set_uint32("item", "strengthen_material_ex", single.uid(), es.strengthen_material_ex());
		redis_client_t::set_uint32("item", "strengthen_money", single.uid(), es.strengthen_money());
		redis_client_t::set_uint32("item", "remain_remake_count", single.uid(), es.remain_remake_count());
		redis_client_t::set_uint32("item", "add_remake_count", single.uid(), es.add_remake_count());
	}
	
	return true;
}

bool item_data_manager::load_item_data_from_redis(uint64_t uid, proto::common::item_data* data)
{
	if (data == NULL)
	{
		log_error("load_item_data_from_redis [%lu] data == null", uid);
		return false;
	}

	std::vector<uint64_t> item_uids;
	redis_client_t::get_keys_by_field_condition<uint64_t, uint64_t, std::vector<uint64_t>>("item", "role_uid", uid, item_uids);

	for (auto item_uid : item_uids)
	{
		uint32_t tid = redis_client_t::get_uint32("item", "tid", item_uid);
		uint32_t num = redis_client_t::get_uint32("item", "num", item_uid);
		if (NULL == GET_CONF(Item, tid))
		{
			continue;
		}
		//if (is_equip(tid) && num == 0)
		//所有道具为0 都不允许重新load
		if (num == 0)
		{
			continue;
		}
		proto::common::item_single* single = data->add_item_list();
		if (single == NULL)
			continue;

		single->set_uid(string_util_t::uint64_to_string(item_uid));
		single->set_tid(tid);
		single->set_num(num);
		single->set_package(redis_client_t::get_uint32("item", "package", single->uid()));
		single->set_param(redis_client_t::get_uint32("item", "param", single->uid()));
		single->set_param2(redis_client_t::get_uint32("item", "param2", single->uid()));
		single->set_find_role(redis_client_t::get_string("item", "find_role", single->uid()));
		single->set_state(redis_client_t::get_uint32("item", "state", single->uid()));
		single->set_expired_time(redis_client_t::get_uint32("item", "expired_time", single->uid()));

		proto::common::equip_single* es = single->mutable_equip_info();
		if (es == NULL)
			continue;

		es->set_hero_uid(string_util_t::uint64_to_string(redis_client_t::get_uint64("item", "hero_uid", item_uid)));
		//基础属性
		std::string attr = redis_client_t::get_string("item", "attr", item_uid);
		std::vector<int32_t> attr_vec;
		string_util_t::split<int32_t>(attr, attr_vec, ":$");
		if (attr_vec.size() % 2 != 0)
		{
			log_error("equip[%lu] load attr error ", item_uid);
			continue;
		}
		for (size_t i = 0; i < attr_vec.size();)
		{
			proto::common::equip_attr* ea = es->add_attr();
			ea->set_type(attr_vec[i++]);
			ea->set_value(attr_vec[i++]);
		}
		//判词
		std::string exattr = redis_client_t::get_string("item", "exattr", item_uid);
		std::vector<int32_t> exattr_vec;
		string_util_t::split<int32_t>(exattr, exattr_vec, "$");
		for (uint16_t i = 0; i < exattr_vec.size(); ++i)
		{
			es->add_exattr(exattr_vec[i]);
		}
		//判词重铸
		std::string remake_exattr = redis_client_t::get_string("item", "remake_exattr", item_uid);
		std::vector<int32_t> remake_exattr_vec;
		string_util_t::split<int32_t>(remake_exattr, remake_exattr_vec, "$");
		for (uint16_t i = 0; i < remake_exattr_vec.size(); ++i)
		{
			es->add_remake_exattr(remake_exattr_vec[i]);
		}

		es->set_remake_exattr_bonus(redis_client_t::get_uint32("item", "remake_exattr_bonus", item_uid));
		es->set_name(redis_client_t::get_uint32("item", "name", item_uid));
		es->set_special(redis_client_t::get_uint32("item", "special", item_uid));
		es->set_strengthen_level(redis_client_t::get_uint32("item", "strengthen_level", item_uid));
		es->set_strmaster_level(redis_client_t::get_uint32("item", "strmaster_level", item_uid));
		es->set_remake_material(redis_client_t::get_uint32("item", "remake_material", item_uid));
		es->set_lock_material(redis_client_t::get_uint32("item", "lock_material", item_uid));
		es->set_strengthen_material(redis_client_t::get_uint32("item", "strengthen_material", item_uid));
		es->set_strengthen_material_ex(redis_client_t::get_uint32("item", "strengthen_material_ex", item_uid));
		es->set_strengthen_money(redis_client_t::get_uint32("item", "strengthen_money", item_uid));
		es->set_remain_remake_count(redis_client_t::get_uint32("item", "remain_remake_count", item_uid));
		es->set_add_remake_count(redis_client_t::get_uint32("item", "add_remake_count", item_uid));
	}

	return true;
}

bool item_data_manager::is_equip(uint32_t tid)
{
	return tid / 10000 == equip_prefix;
}
