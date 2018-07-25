#include "family_skill_data_manager.hpp"
#include "redis_client.hpp"
#include "utility.hpp"
#include "log.hpp"
USING_NS_COMMON;
family_skill_data_manager::family_skill_data_manager()
{
}

family_skill_data_manager::~family_skill_data_manager()
{
}

bool family_skill_data_manager::save_family_skill_data_to_redis(uint64_t uid, const proto::common::family_skill_data& ld)
{
	redis_client_t::set_protobuf("family_skill", "all_family_skill_data", uid, ld);
	return true;
}

bool family_skill_data_manager::load_family_skill_data_from_redis(uint64_t uid, proto::common::family_skill_data* p_data)
{
	if (NULL == p_data)
	{
		return false;
	}

	redis_client_t::get_protobuf("family_skill", "all_family_skill_data", uid, *p_data);
	return true;
}
