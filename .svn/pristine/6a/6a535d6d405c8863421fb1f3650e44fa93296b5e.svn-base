#include "global_data_mgr.hpp"
#include "common/redis_client.hpp"

uint32_t global_data_mgr_t::load_day_update_time()
{
	proto::server::global_data_day_update_time data;
	redis_client_t::get_protobuf("global_server_data", "data", (uint32_t)proto::server::global_data_type_one_day, data);
	return data.update_time();
}

void global_data_mgr_t::save_day_update_time(uint32_t update_time)
{
	proto::server::global_data_day_update_time data;
	data.set_update_time(update_time);
	redis_client_t::set_protobuf("global_server_data", "data", (uint32_t)proto::server::global_data_type_one_day, data);
}

void global_data_mgr_t::load_family_war_data(proto::server::global_data_type type, proto::server::global_data_family_war& data)
{
	redis_client_t::get_protobuf("global_server_data", "data", (uint32_t)type, data);
}

void global_data_mgr_t::save_family_war_data(proto::server::global_data_type type, const proto::server::global_data_family_war& data)
{
	redis_client_t::set_protobuf("global_server_data", "data", (uint32_t)type, data);
}

void global_data_mgr_t::load_country_data(proto::server::global_data_type type, proto::server::global_data_country_data& data)
{
	redis_client_t::get_protobuf("global_server_data", "data", (uint32_t)type, data);
}

void global_data_mgr_t::save_country_data(proto::server::global_data_type type, const proto::server::global_data_country_data& data)
{
	redis_client_t::set_protobuf("global_server_data", "data", (uint32_t)type, data);
}
