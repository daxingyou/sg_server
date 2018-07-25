#include "dungeon_data_manager.hpp"
#include "utility.hpp"
#include "redis_client.hpp"
#include "log.hpp"

bool dungeon_data_manager::save_dungeon_data_to_redis(uint64_t uid, const proto::common::role_dungeon_data& data)
{
	std::ostringstream key;
	key << uid << data.dungeon_id();

	std::ostringstream ss;
	for (int32_t i = 0; i < data.monsters_size(); ++i)
	{
		if (!ss.str().empty())
		{
			ss << "$";
		}

		ss << data.monsters(i).monster_tid() << ":" << data.monsters(i).num();
	}

	REDIS_BATCH_START
	redis_client_t::set_uint64("role_dungeon", "role_uid", key.str(), uid);
	redis_client_t::set_uint32("role_dungeon", "dungeon_id", key.str(), data.dungeon_id());
	redis_client_t::set_uint32("role_dungeon", "total_pass_time", key.str(), data.total_pass_time());
	redis_client_t::set_string("role_dungeon", "rwd_list", key.str(), ss.str());
	redis_client_t::set_uint32("role_dungeon", "refresh_time", key.str(), data.refresh_time());
	redis_client_t::set_uint32("role_dungeon", "best_score", key.str(), data.best_score());
	redis_client_t::set_uint32("role_dungeon", "best_pass_time", key.str(), data.best_pass_time());
	redis_client_t::set_uint32("role_dungeon", "best_death_value", key.str(), data.best_death_value());
	REDIS_BATCH_DONE

	return true;
}

bool dungeon_data_manager::load_dungeon_data_from_redis(uint64_t uid, proto::common::role_all_dungeon_data* data)
{
	if (data == NULL)
	{
		log_error("load_dungeon_data_from_redis [%lu] data == null", uid);
		return false;
	}

	std::vector<std::string> dungeon_keys;
	redis_client_t::get_keys_by_field_condition<std::string, uint64_t, std::vector<std::string>>("role_dungeon", "role_uid", uid, dungeon_keys);

	for (const auto& key : dungeon_keys)
	{
		proto::common::role_dungeon_data* p_dungeon_data = data->add_datas();
		if (p_dungeon_data == NULL)
		{
			continue;
		}

		uint32_t dungeon_id = redis_client_t::get_uint32("role_dungeon", "dungeon_id", key);
		p_dungeon_data->set_dungeon_id(dungeon_id);
		p_dungeon_data->set_total_pass_time(redis_client_t::get_uint32("role_dungeon", "total_pass_time", key));
		p_dungeon_data->set_refresh_time(redis_client_t::get_uint32("role_dungeon", "refresh_time", key));
		p_dungeon_data->set_best_score(redis_client_t::get_uint32("role_dungeon", "best_score", key));
		p_dungeon_data->set_best_pass_time(redis_client_t::get_uint32("role_dungeon", "best_pass_time", key));
		p_dungeon_data->set_best_death_value(redis_client_t::get_uint32("role_dungeon", "best_death_value", key));

		std::string rwd_list = redis_client_t::get_string("role_dungeon", "rwd_list", key);
		if (!rwd_list.empty())
		{
			std::vector<std::string> rwds;
			std::vector<uint32_t> rwd_items;
			common::string_util_t::split<std::string>(rwd_list, rwds, "$");

			for (const auto& rwd : rwds)
			{
				// rwd_items.clear(); string_util_t::split »áclear 
				common::string_util_t::split<uint32_t>(rwd, rwd_items, ":");

				if (rwd_items.size() != 2)
				{
					log_error("role[%lu] load dungeon_data error, dungeon_id[%u]", uid, dungeon_id);
					continue;
				}

				proto::common::role_dungeon_monster* p_dungeon_monster = p_dungeon_data->add_monsters();
				if (p_dungeon_monster != NULL)
				{
					p_dungeon_monster->set_monster_tid(rwd_items[0]);
					p_dungeon_monster->set_num(rwd_items[1]);
				}
			}
		}
	}

	return true;
}

bool dungeon_data_manager::save_dungeon_times_data_to_redis(uint64_t uid, const proto::common::role_all_dungeon_times_data& data)
{
	redis_client_t::set_protobuf("role_dungeon_times", "times_data", uid, data);
	return true;
}

bool dungeon_data_manager::load_dungeon_times_data_from_redis(uint64_t uid, proto::common::role_all_dungeon_times_data* data)
{
	if (data == NULL)
	{
		log_error("load_dungeon_times_data_from_redis [%lu] data == null", uid);
		return false;
	}
	redis_client_t::get_protobuf("role_dungeon_times", "times_data", uid, *data);
	return true;
}
