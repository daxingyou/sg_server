#include "rank_data_mgr.hpp"
#include "common/redis_client.hpp"

void rank_data_mgr_t::load_rank_data(uint32_t rank_id, proto::common::rank_data& data)
{
	redis_client_t::get_protobuf("ranklist", "rank_data", rank_id, data);
}

void rank_data_mgr_t::save_rank_data(uint32_t rank_id, const proto::common::rank_data& data)
{
	redis_client_t::set_protobuf("ranklist", "rank_data", rank_id, data);
}
