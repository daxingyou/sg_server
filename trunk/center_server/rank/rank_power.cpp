#include "rank_power.hpp"
#include "redis_client.hpp"

rank_power_t::rank_power_t(common::rank_common::rank_type type, const char* name)
	: rank_sort_t(type, name)
{

}

void rank_power_t::update_power(uint64_t uid, uint32_t val)
{
	if (!update_sort(uid, val)) { //  ß∞‹±Ì æŒ¥»Î∞Ò
		return;
	}

	redis_client_t::set_uint32(m_rank_name.c_str(), "save_time", uid, common::time_util_t::now_time());
}
