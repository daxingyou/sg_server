#include "rank_family.hpp"
#include "redis_client.hpp"
#include "family/family_manager.hpp"
#include "redis/global_user_data_mgr.hpp"

rank_family_t::rank_family_t(common::rank_common::rank_type type, const char* name)
	: rank_sort_t(type, name)
{

}

void rank_family_t::update_prestige(uint64_t uid, uint32_t val)
{
	if (!update_sort(uid, val)) { // Ê§°Ü±íÊ¾Î´Èë°ñ
		return;
	}

	redis_client_t::set_uint32(m_rank_name.c_str(), "save_time", uid, common::time_util_t::now_time());
}

bool rank_family_t::peek_data(uint64_t uid, proto::common::rank_single_data* p_data)
{
	if (!p_data) {
		log_error("p_data null error");
		return false;
	}

	if (!rank_sort_t::peek_data(uid, p_data)) {
		return false;
	}

	family_ptr p_family = family_manager_t::get_family(uid);
	if (p_family) {
		p_data->add_custom_data(global_user_data_mgr_t::get_name(p_family->get_general_id()));
	}

	return true;
}

void rank_family_t::del_family(uint64_t uid)
{
	rank_sort_t::remove_rank(uid);
}
