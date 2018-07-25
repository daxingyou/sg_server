#include "expedition_data_manager.hpp"
#include "redis_client.hpp"
#include "utility.hpp"
USING_NS_COMMON;
expedition_data_manager::expedition_data_manager()
{
}

expedition_data_manager::~expedition_data_manager()
{
}

bool expedition_data_manager::save_expedition_data_to_redis(uint64_t uid, const proto::common::expedition_data& ed)
{
    redis_client_t::set_protobuf("expedition", "expedition_data", uid, ed);
	return true;
}

bool expedition_data_manager::load_expedition_data_from_redis(uint64_t uid, proto::common::expedition_data* p_data)
{
	if (NULL == p_data)
	{
		return false;
	}
    redis_client_t::get_protobuf("expedition", "expedition_data", uid, *p_data);
	return true;
}
