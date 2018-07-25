#include "office_data_manager.hpp"
#include "redis_client.hpp"
#include "utility.hpp"
USING_NS_COMMON;
office_data_manager::office_data_manager()
{
}

office_data_manager::~office_data_manager()
{
}

bool office_data_manager::save_office_data_to_redis(uint64_t uid, const proto::common::office_data& rd)
{
	REDIS_BATCH_START
	redis_client_t::set_uint32("office", "level", uid, rd.level());
	redis_client_t::set_uint32("office", "exp", uid, rd.exp());
	redis_client_t::set_uint32("office", "award", uid, rd.award());
	REDIS_BATCH_DONE
	return true;
}

bool office_data_manager::load_office_data_from_redis(uint64_t uid, proto::common::office_data* p_data)
{
	if (NULL == p_data)
	{
		return false;
	}

	p_data->set_level(redis_client_t::get_uint32("office", "level", uid));
	p_data->set_exp(redis_client_t::get_uint32("office", "exp", uid));
	p_data->set_award(redis_client_t::get_uint32("office", "award", uid));
	return true;
}
