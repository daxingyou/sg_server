#include "rank_copper.hpp"
#include "redis_client.hpp"

rank_copper_t::rank_copper_t(common::rank_common::rank_type type, const char* name)
	: rank_sort_t(type, name)
{

}

void rank_copper_t::update_rank_from_game(const proto::server::ge_update_rank_notify& notify)
{
	if (!update_sort_add(notify.owner(), notify.key()))
		return;
	
	REDIS_BATCH_START
		redis_client_t::set_uint32(m_rank_name.c_str(), "save_time", notify.owner(), common::time_util_t::now_time());
	REDIS_BATCH_DONE
}
