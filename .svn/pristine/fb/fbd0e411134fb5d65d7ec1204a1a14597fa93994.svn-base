#include "patrol_hire_data_manager.hpp"
#include "redis_client.hpp"
#include "utility.hpp"
#include "log.hpp"
USING_NS_COMMON;

bool patrol_hire_data_manager::save_patrol_hire_data_to_redis(uint64_t uid, const proto::common::patrol_hire_data& td)
{
REDIS_BATCH_START
	redis_client_t::set_uint32("patrol_hire", "map_id", uid, td.map_id());
	redis_client_t::set_uint32("patrol_hire", "begin_time", uid, td.begin_time());
	redis_client_t::set_uint32("patrol_hire", "fast_patrol_count", uid, td.fast_patrol_count());
REDIS_BATCH_DONE
	return true;
}

bool patrol_hire_data_manager::load_patrol_hire_data_from_redis(uint64_t uid, proto::common::patrol_hire_data* p_data)
{
	if (NULL == p_data)
	{
		return false;
	}

 	p_data->set_map_id(redis_client_t::get_uint32("patrol_hire", "map_id", uid));
	p_data->set_begin_time(redis_client_t::get_uint32("patrol_hire", "begin_time", uid));
	p_data->set_fast_patrol_count(redis_client_t::get_uint32("patrol_hire", "fast_patrol_count", uid));
	return true;
}

