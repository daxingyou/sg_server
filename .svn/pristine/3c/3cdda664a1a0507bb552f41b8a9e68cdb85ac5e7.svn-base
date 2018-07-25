#include "rank_achieve.hpp"
#include "redis_client.hpp"

rank_achieve_t::rank_achieve_t(common::rank_common::rank_type type, const char* name)
	: rank_sort_t(type, name)
{

}

bool rank_achieve_t::peek_data(uint64_t uid, proto::common::rank_single_data* p_data)
{
	if (!p_data) {
		log_error("p_data null error");
		return false;
	}

	if (!rank_sort_t::peek_data(uid, p_data)) {
		return false;
	}

	p_data->add_custom_data(redis_client_t::get_string(m_rank_name.c_str(), "level", uid));

	return true;
}

void rank_achieve_t::update_rank_from_game(const proto::server::ge_update_rank_notify& notify)
{
	if (!update_sort(notify.owner(), notify.key())) { //  ß∞‹±Ì æŒ¥»Î∞Ò
		return;
	}

	REDIS_BATCH_START
	redis_client_t::set_uint32(m_rank_name.c_str(), "level", notify.owner(), notify.extra().achieve().level());
	redis_client_t::set_uint32(m_rank_name.c_str(), "save_time", notify.owner(), common::time_util_t::now_time());
	REDIS_BATCH_DONE
}
