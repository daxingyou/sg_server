#include "rank_level.hpp"
#include "redis_client.hpp"

rank_level_t::rank_level_t(common::rank_common::rank_type type, const char* name)
	: rank_sort_t(type, name)
{

}

void rank_level_t::update_level(uint64_t uid, uint32_t val)
{
	if (!update_sort(uid, val)) { // Ê§°Ü±íÊ¾Î´Èë°ñ
		return;
	}

	redis_client_t::set_uint32(m_rank_name.c_str(), "save_time", uid, common::time_util_t::now_time());
}

void rank_level_t::get_rank_user(uint32_t rank, std::vector<uint64_t>& uids)
{
	if (rank == 0) return;

	get_range_by_rank(0, rank - 1, uids);
}
