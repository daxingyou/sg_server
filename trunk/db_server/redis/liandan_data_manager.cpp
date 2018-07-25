#include "liandan_data_manager.hpp"
#include "redis_client.hpp"
#include "utility.hpp"
#include "log.hpp"
USING_NS_COMMON;
liandan_data_manager::liandan_data_manager()
{
}

liandan_data_manager::~liandan_data_manager()
{
}

bool liandan_data_manager::save_liandan_data_to_redis(uint64_t uid, const proto::common::liandan_data& ld)
{
	redis_client_t::set_protobuf("liandan", "all_liandan_data", uid, ld);
	return true;
}

bool liandan_data_manager::load_liandan_data_from_redis(uint64_t uid, proto::common::liandan_data* p_data)
{
	if (NULL == p_data)
	{
		return false;
	}

	redis_client_t::get_protobuf("liandan", "all_liandan_data", uid, *p_data);
	return true;
}
