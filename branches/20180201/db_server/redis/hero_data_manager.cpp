#include "hero_data_manager.hpp"
#include "common/redis_client.hpp"
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
	std::ostringstream ss;
	for (auto fate_pair : attr.fate_info())
	{
		ss << fate_pair.fate_tid() << ":" << fate_pair.fate_level() << "$";
	}
	std::string fate_info = ss.str().substr(0, ss.str().length() - 1);

	redis_client_t::set_uint64("heroes", "role_uid", uid, role_uid);
	redis_client_t::set_uint32("heroes", "hero_id", uid, attr.id());
	redis_client_t::set_uint32("heroes", "level", uid, attr.level());
	redis_client_t::set_uint32("heroes", "exp", uid, attr.exp());
	redis_client_t::set_uint32("heroes", "species", uid, attr.species());
	redis_client_t::set_uint32("heroes", "country", uid, attr.country());
	redis_client_t::set_uint32("heroes", "grade", uid, attr.grade());
	redis_client_t::set_uint32("heroes", "talent_level", uid, attr.talent_level());
	redis_client_t::set_uint32("heroes", "talent_point", uid, attr.talent_point());
	redis_client_t::set_uint32("heroes", "star", uid, attr.star());
	redis_client_t::set_uint32("heroes", "degree_point", uid, attr.degree());
	redis_client_t::set_uint32("heroes", "talent_count", uid, attr.talent_count());
	redis_client_t::set_string("heroes", "fate_info", uid, fate_info);
	redis_client_t::set_uint32("heroes", "exp_today", uid, attr.exp_today());
	redis_client_t::set_uint32("heroes", "talent_use", uid, attr.talent_use());
	redis_client_t::set_uint32("heroes", "wakeup", uid, attr.wakeup());
	redis_client_t::set_uint32("heroes", "active_talent_use", uid, attr.active_talent_use());
	redis_client_t::set_uint32("heroes", "hero_state", uid, attr.hero_state());
	redis_client_t::set_uint32("heroes", "delete_flag", uid, attr.delete_flag());


	ss.str("");
	ss.clear();
	for (int32_t i = 0; i < attr.skill_data().skill_list_size(); ++i)
	{
		ss << attr.skill_data().skill_list(i).skill_id() << ":" << attr.skill_data().skill_list(i).skill_level() << ":" << attr.skill_data().skill_list(i).skill_pos() << "$";
	}
	std::string skill_data = ss.str().substr(0, ss.str().length() - 1);
	redis_client_t::set_string("heroes", "skill_data", uid, skill_data);

	ss.str("");
	ss.clear();
	for (auto equip_info : attr.equips())
	{
		ss << equip_info.pos() << ":" << equip_info.equip_uid() << "$";
	}
	std::string equip = ss.str().substr(0, ss.str().length() - 1);
	redis_client_t::set_string("heroes", "equip", uid, equip);

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
	// talent
	save_talent_data_to_redis(uid, single.talent_data());
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
		attr->set_talent_point(redis_client_t::get_uint32("heroes", "talent_point", hero_uid));
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

		std::string skill_data = redis_client_t::get_string("heroes", "skill_data", hero_uid);
		std::vector<std::string> skill_info_vec;
		string_util_t::split<std::string>(skill_data, skill_info_vec, ":$");
		if (skill_info_vec.size() % 3 != 0)
		{
			log_error("user[%lu] load skill error", uid);
			continue;
		}
		for (size_t i = 0; i < skill_info_vec.size();)
		{
			proto::common::hero_skill_single* single = attr->mutable_skill_data()->add_skill_list();
			uint32_t skill_id = string_util_t::string_to_uint32(skill_info_vec[i++]);
			uint32_t skill_level = string_util_t::string_to_uint32(skill_info_vec[i++]);
			uint32_t skill_pos = string_util_t::string_to_uint32(skill_info_vec[i++]);
			single->set_skill_id(skill_id);
			single->set_skill_level(skill_level);
			single->set_skill_pos(skill_pos);
		}

		std::string fate_info = redis_client_t::get_string("heroes", "fate_info", hero_uid);
		std::vector<int32_t> fate_info_vec;
		string_util_t::split<int32_t>(fate_info, fate_info_vec, ":$");
		if (fate_info_vec.size() % 2 != 0)
		{
			log_error("user[%lu] load fate info error fate_info is wrong", uid);
			continue;
		}
		for (size_t i = 0; i < fate_info_vec.size();)
		{
			proto::common::fate_pair* fate_pair = attr->add_fate_info();
			fate_pair->set_fate_tid(fate_info_vec[i++]);
			fate_pair->set_fate_level(fate_info_vec[i++]);
		}

		attr->set_exp_today(redis_client_t::get_uint32("heroes", "exp_today", hero_uid));
		attr->set_talent_use(redis_client_t::get_uint32("heroes", "talent_use", hero_uid));
		attr->set_active_talent_use(redis_client_t::get_uint32("heroes", "active_talent_use", hero_uid));
		std::string equip = redis_client_t::get_string("heroes", "equip", hero_uid);
		std::vector<std::string> equip_info_vec;
		string_util_t::split<std::string>(equip, equip_info_vec, ":$");
		if (equip_info_vec.size() % 2 != 0)
		{
			log_error("user[%lu] load equip error", uid);
			continue;
		}
		for (size_t i = 0; i < equip_info_vec.size();)
		{
			proto::common::hero_equip* he = attr->add_equips();
			uint32_t pos = string_util_t::string_to_uint32(equip_info_vec[i++]);
			he->set_pos(pos);
			he->set_equip_uid(equip_info_vec[i++]);
		}
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
		// talent
		load_talent_data_from_redis(hero_uid, single->mutable_talent_data());
	}

	return true;
}

bool hero_data_manager::save_talent_data_to_redis(uint64_t hero_uid, const proto::common::talent_data& td)
{
	for (int i = 0; i < td.talent_list_size(); ++i)
	{
		const proto::common::talent_single& ts = td.talent_list(i);
		std::ostringstream talent_key;
		talent_key << hero_uid << ts.tid();
		redis_client_t::set_uint64("talent", "hero_uid", talent_key.str(), hero_uid);
		redis_client_t::set_uint32("talent", "tid", talent_key.str(), ts.tid());
		redis_client_t::set_uint32("talent", "level", talent_key.str(), ts.level());
		redis_client_t::set_uint32("talent", "wake", talent_key.str(), ts.wake());
	}

	return true;
}

bool hero_data_manager::load_talent_data_from_redis(uint64_t hero_uid, proto::common::talent_data* td)
{
	if (td == NULL)
	{
		log_error("load_talent_data_from_redis [%lu] td == null", hero_uid);
		return false;
	}

	std::vector<std::string> talent_uids;
	redis_client_t::get_keys_by_field_condition<std::string, uint64_t, std::vector<std::string>>("talent", "hero_uid", hero_uid, talent_uids);

	for (auto talent_uid : talent_uids)
	{
		proto::common::talent_single* ts = td->add_talent_list();
		if (ts == NULL)
			continue;

		ts->set_tid(redis_client_t::get_uint32("talent", "tid", talent_uid));
		ts->set_level(redis_client_t::get_uint32("talent", "level", talent_uid));
		ts->set_wake(redis_client_t::get_uint32("talent", "wake", talent_uid));
	}

	return true;
}
