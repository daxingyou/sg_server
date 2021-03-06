#include "hero_data_manager.hpp"
#include "redis_client.hpp"
#include "config_mgr.h"
#include "tblh/Hero.tbls.h"

hero_data_manager::hero_data_manager()
{
}

hero_data_manager::~hero_data_manager()
{
}

bool hero_data_manager::save_hero_data_to_redis(uint64_t role_uid, const proto::common::hero_single& single)
{
	const proto::common::base_attr& attr = single.base_attr();
	uint64_t uid = string_util_t::string_to_uint64(attr.unique_id());
	
	redis_client_t::set_uint64("heroes", "role_uid", uid, role_uid);
	redis_client_t::set_uint32("heroes", "hero_id", uid, attr.id());
	redis_client_t::set_uint32("heroes", "level", uid, attr.level());
	redis_client_t::set_uint32("heroes", "exp", uid, attr.exp());
	redis_client_t::set_uint32("heroes", "species", uid, attr.species());
	redis_client_t::set_uint32("heroes", "country", uid, attr.country());
	redis_client_t::set_uint32("heroes", "grade", uid, attr.grade());
	redis_client_t::set_uint32("heroes", "talent_level", uid, attr.talent_level());
	redis_client_t::set_uint32("heroes", "star", uid, attr.star());
	redis_client_t::set_uint32("heroes", "degree_point", uid, attr.degree());
	redis_client_t::set_uint32("heroes", "talent_count", uid, attr.talent_count());
	redis_client_t::set_uint32("heroes", "exp_today", uid, attr.exp_today());
	redis_client_t::set_uint32("heroes", "talent_use", uid, attr.talent_use());
	redis_client_t::set_uint32("heroes", "wakeup", uid, attr.wakeup());
	redis_client_t::set_uint32("heroes", "active_talent_use", uid, attr.active_talent_use());
	redis_client_t::set_uint32("heroes", "hero_state", uid, attr.hero_state());
	redis_client_t::set_uint32("heroes", "delete_flag", uid, attr.delete_flag());

	std::ostringstream ss;
	//ss.str("");
	//ss.clear();
	//for (auto equip_info : attr.equips())
	//{
	//	ss << equip_info.pos() << ":" << equip_info.equip_uid() << "$";
	//}
	//std::string equip = ss.str().substr(0, ss.str().length() - 1);
	//redis_client_t::set_string("heroes", "equip", uid, equip);

	ss.str("");
	ss.clear();
	for (auto plugin : attr.plugins())
	{
		if (!ss.str().empty())
		{
			ss << "$";
		}

		ss << plugin;
	}

	redis_client_t::set_string("heroes", "plugins", uid, ss.str());
	redis_client_t::set_uint32("heroes", "current_plugin", uid, attr.current_plugin());
	redis_client_t::set_string("heroes", "nick_name", uid, attr.nick_name());
	redis_client_t::set_uint64("heroes", "last_equip_plan", uid, string_util_t::string_to_uint64(attr.last_equip_plan()));
	//skill talent
	redis_client_t::set_protobuf("heroes", "skill_data", uid, attr.skill_data());
	redis_client_t::set_protobuf("heroes", "talent_data", uid, single.talent_data());

	ss.str("");
	ss.clear();
	for (auto famous : attr.famous())
	{
		if (!ss.str().empty())
		{
			ss << "$";
		}

		ss << famous;
	}

	redis_client_t::set_string("heroes", "famous", uid, ss.str());
	return true;
}

bool hero_data_manager::load_hero_data_from_redis(uint64_t uid, proto::common::hero_data* data)
{
	if (data == NULL)
	{
		log_error("load_hero_data_from_redis [%lu] data == null", uid);
		return false;
	}
	
	std::vector<uint64_t> hero_uids;
	redis_client_t::get_keys_by_field_condition<uint64_t, uint64_t, std::vector<uint64_t>>("heroes", "role_uid", uid, hero_uids);

	for (auto hero_uid : hero_uids)
	{
		uint32_t hero_id = redis_client_t::get_uint32("heroes", "hero_id", hero_uid);
		if (NULL == GET_CONF(Hero, hero_id))
		{
			log_error("role[%lu] hero_id[%d] invalid!", uid, hero_id);
			continue;
		}

		proto::common::hero_single* single = data->add_hero_list();
		if (single == NULL)
			continue;

		proto::common::base_attr* attr = single->mutable_base_attr();
		attr->set_unique_id(string_util_t::uint64_to_string(hero_uid));
		attr->set_id(hero_id);
		attr->set_level(redis_client_t::get_uint32("heroes", "level", hero_uid));
		attr->set_exp(redis_client_t::get_uint32("heroes", "exp", hero_uid));
		attr->set_species(redis_client_t::get_uint32("heroes", "species", hero_uid));
		attr->set_country(redis_client_t::get_uint32("heroes", "country", hero_uid));
		attr->set_grade(redis_client_t::get_uint32("heroes", "grade", hero_uid));
		attr->set_talent_level(redis_client_t::get_uint32("heroes", "talent_level", hero_uid));
		attr->set_star(redis_client_t::get_uint32("heroes", "star", hero_uid));
		attr->set_degree(redis_client_t::get_uint32("heroes", "degree_point", hero_uid));
		attr->set_talent_count(redis_client_t::get_uint32("heroes", "talent_count", hero_uid));
		attr->set_wakeup(redis_client_t::get_uint32("heroes", "wakeup", hero_uid));
		attr->set_delete_flag(redis_client_t::get_uint32("heroes", "delete_flag", hero_uid));
		attr->set_hero_state(redis_client_t::get_uint32("heroes", "hero_state", hero_uid));
		// 已经删除的不读进来
		if (attr->delete_flag() == 1)
		{
			continue;
		}
		attr->set_exp_today(redis_client_t::get_uint32("heroes", "exp_today", hero_uid));
		attr->set_talent_use(redis_client_t::get_uint32("heroes", "talent_use", hero_uid));
		attr->set_active_talent_use(redis_client_t::get_uint32("heroes", "active_talent_use", hero_uid));
		//std::string equip = redis_client_t::get_string("heroes", "equip", hero_uid);
		//std::vector<std::string> equip_info_vec;
		//string_util_t::split<std::string>(equip, equip_info_vec, ":$");
		//if (equip_info_vec.size() % 2 != 0)
		//{
		//	log_error("user[%lu] load equip error", uid);
		//	continue;
		//}
		//for (size_t i = 0; i < equip_info_vec.size();)
		//{
		//	proto::common::hero_equip* he = attr->add_equips();
		//	uint32_t pos = string_util_t::string_to_uint32(equip_info_vec[i++]);
		//	he->set_pos(pos);
		//	he->set_equip_uid(equip_info_vec[i++]);
		//}
		std::string plugins_info = redis_client_t::get_string("heroes", "plugins", hero_uid);
		if (!plugins_info.empty())
		{
			std::vector<uint32_t> plugins;
			string_util_t::split<uint32_t>(plugins_info, plugins, "$");
			for (auto plugin : plugins)
			{
				attr->add_plugins(plugin);
			}
		}
		attr->set_current_plugin(redis_client_t::get_uint32("heroes", "current_plugin", hero_uid));
		uint64_t last_equip_plan = redis_client_t::get_uint64("heroes", "last_equip_plan", hero_uid);
		attr->set_last_equip_plan(string_util_t::uint64_to_string(last_equip_plan));
		attr->set_nick_name(redis_client_t::get_string("heroes", "nick_name", hero_uid));
		// skill talent
		redis_client_t::get_protobuf("heroes", "skill_data", hero_uid, *attr->mutable_skill_data());
		redis_client_t::get_protobuf("heroes", "talent_data", hero_uid, *single->mutable_talent_data());
		
		std::string famous_info = redis_client_t::get_string("heroes", "famous", hero_uid);
		if (!famous_info.empty())
		{
			std::vector<uint32_t> famous_vec;
			string_util_t::split<uint32_t>(famous_info, famous_vec, "$");
			for (auto famous : famous_vec)
			{
				attr->add_famous(famous);
			}
		}
	}
	
	return true;
}