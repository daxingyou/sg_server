#include "rank_data_mgr.hpp"
#include "redis_client.hpp"

bool rank_data_mgr_t::is_rank_exsit(const std::string& rank, uint64_t uid)
{
	return redis_client_t::is_hash_exist(rank.c_str(), uid);
}

void rank_data_mgr_t::del_rank_sort(const std::string& rank, uint64_t uid)
{
	if (!is_rank_exsit(rank, uid)) {
		return;
	}

	redis_client_t::remove_key(rank.c_str(), uid);
}
