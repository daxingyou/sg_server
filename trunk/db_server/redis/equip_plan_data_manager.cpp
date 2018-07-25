#include "equip_plan_data_manager.hpp"
#include "redis_client.hpp"
#include "utility.hpp"
#include "log.hpp"
USING_NS_COMMON;
equip_plan_data_manager::equip_plan_data_manager()
{
}

equip_plan_data_manager::~equip_plan_data_manager()
{
}

bool equip_plan_data_manager::save_equip_plan_data_to_redis(uint64_t uid, const proto::common::equip_plan_data& ld)
{
	redis_client_t::set_protobuf("equip_plan", "all_equip_plan_data", uid, ld);
	return true;
}

bool equip_plan_data_manager::load_equip_plan_data_from_redis(uint64_t uid, proto::common::equip_plan_data* p_data)
{
	if (NULL == p_data)
	{
		return false;
	}

	redis_client_t::get_protobuf("equip_plan", "all_equip_plan_data", uid, *p_data);
	return true;
}
