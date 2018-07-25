#include "recharge_data_manager.hpp"
#include "redis_client.hpp"
#include "utility.hpp"
USING_NS_COMMON;
recharge_data_manager::recharge_data_manager()
{
}

recharge_data_manager::~recharge_data_manager()
{
}

bool recharge_data_manager::save_recharge_data_to_redis(uint64_t uid, const proto::common::recharge_data& rd)
{
	REDIS_BATCH_START
	redis_client_t::set_uint32("recharge", "recharge_flag", uid, rd.recharge_flag());
	redis_client_t::set_uint32("recharge", "total_cny", uid, rd.total_cny());
	redis_client_t::set_uint32("recharge", "vip_level", uid, rd.vip_level());
	redis_client_t::set_uint32("recharge", "recharge_reward_flag", uid, rd.recharge_reward_flag());

	std::ostringstream ss;
	ss.str("");
	ss.clear();
	for (auto cheap : rd.cheap_day())
	{
		ss << cheap.id() << ":" << cheap.count() << "$";
	}
	std::string cheap = ss.str().substr(0, ss.str().length() - 1);
	redis_client_t::set_string("recharge", "cheap_day", uid, cheap);
	REDIS_BATCH_DONE
	return true;
}

bool recharge_data_manager::load_recharge_data_from_redis(uint64_t uid, proto::common::recharge_data* p_data)
{
	if (NULL == p_data)
	{
		return false;
	}

	p_data->set_recharge_flag(redis_client_t::get_uint32("recharge", "recharge_flag", uid));
	p_data->set_total_cny(redis_client_t::get_uint32("recharge", "total_cny", uid));
	p_data->set_vip_level(redis_client_t::get_uint32("recharge", "vip_level", uid));
	p_data->set_recharge_reward_flag(redis_client_t::get_uint32("recharge", "recharge_reward_flag", uid));
	std::string cheap = redis_client_t::get_string("recharge", "cheap_day", uid);
	std::vector<uint32_t> cheap_vec;
	string_util_t::split<uint32_t>(cheap, cheap_vec, ":$");
	if (cheap_vec.size() % 2 != 0)
	{
		log_error("user[%lu] load recharge error", uid);
		return false;
	}
	for (size_t i = 0; i < cheap_vec.size();)
	{
		proto::common::cheap_day_single* cds = p_data->add_cheap_day();
		cds->set_id(cheap_vec[i++]);
		cds->set_count(cheap_vec[i++]);
	}
	return true;
}
