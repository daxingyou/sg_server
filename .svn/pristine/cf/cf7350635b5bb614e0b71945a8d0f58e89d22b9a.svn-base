#include "rank_criminal.hpp"
#include "redis_client.hpp"

rank_criminal_t::rank_criminal_t(common::rank_common::rank_type type, const char* name)
	: rank_sort_t(type, name)
{

}

void rank_criminal_t::update_rank_from_game(const proto::server::ge_update_rank_notify& notify)
{
	if (!update_sort(notify.owner(), notify.key())) { //  ß∞‹±Ì æŒ¥»Î∞Ò
		return;
	}

	redis_client_t::set_uint32(m_rank_name.c_str(), "save_time", notify.owner(), common::time_util_t::now_time());
}