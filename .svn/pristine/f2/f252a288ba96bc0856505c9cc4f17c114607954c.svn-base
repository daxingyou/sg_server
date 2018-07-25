#include "activity_data_manager.hpp"
#include "redis_client.hpp"
#include "utility.hpp"
#include "log.hpp"
USING_NS_COMMON;
activity_data_manager::activity_data_manager()
{
}

activity_data_manager::~activity_data_manager()
{
}

bool activity_data_manager::save_activity_data_to_redis(uint64_t uid, const proto::common::activities_data& activities_data)
{
	redis_client_t::set_protobuf("activity", "activities_data", uid, activities_data);
	return true;
}

bool activity_data_manager::load_activity_data_from_redis(uint64_t uid, proto::common::activities_data* p_activities_data)
{
	if (NULL == p_activities_data)
	{
		return false;
	}
	redis_client_t::get_protobuf("activity", "activities_data", uid, *p_activities_data);
	return true;
}
