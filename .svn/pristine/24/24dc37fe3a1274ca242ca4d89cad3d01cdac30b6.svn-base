#include "rank_charm.hpp"
#include "redis_client.hpp"

rank_charm_t::rank_charm_t(common::rank_common::rank_type type, const char* name)
	: rank_sort_t(type, name)
{

}

void rank_charm_t::update_charm(uint64_t uid, uint32_t val)
{
	if (!update_sort(uid, val)) { // Ê§°Ü±íÊ¾Î´Èë°ñ
		return;
	}

	redis_client_t::set_uint32(m_rank_name.c_str(), "save_time", uid, common::time_util_t::now_time());
}
