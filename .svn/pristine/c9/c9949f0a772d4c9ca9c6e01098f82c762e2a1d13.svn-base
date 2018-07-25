#include "luckydraw_data_manager.hpp"
#include "redis_client.hpp"
#include "utility.hpp"
#include "log.hpp"
USING_NS_COMMON;
luckydraw_data_manager::luckydraw_data_manager()
{
}

luckydraw_data_manager::~luckydraw_data_manager()
{
}

//count  type$count;type$count
//seq type:level$level;type:level$level;
bool luckydraw_data_manager::save_luckydraw_data_to_redis(uint64_t uid, const proto::common::luckydraw_data& ld)
{
	redis_client_t::set_protobuf("luckydraw", "luckydraw_data", uid, ld);
	return true;
}

bool luckydraw_data_manager::load_luckydraw_data_from_redis(uint64_t uid, proto::common::luckydraw_data* p_data)
{
	if (NULL == p_data)
	{
		return false;
	}
	redis_client_t::get_protobuf("luckydraw", "luckydraw_data", uid, *p_data);
	return true;
}
