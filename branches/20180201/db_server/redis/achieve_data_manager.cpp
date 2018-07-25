#include "achieve_data_manager.hpp"
#include "common/redis_client.hpp"
#include "config_mgr.h"
#include "tblh/Achievelist.tbls.h"

achieve_data_manager::achieve_data_manager()
{
}

achieve_data_manager::~achieve_data_manager()
{
}

bool achieve_data_manager::save_achieve_data_to_redis(uint64_t uid, const proto::common::achieve_data_single& data)
{

	redis_client_t::set_uint32("achieve", "tid", data.uid(), data.tid());
	redis_client_t::set_uint32("achieve", "level", data.uid(), data.level());
	redis_client_t::set_uint32("achieve", "count", data.uid(), data.count());
	redis_client_t::set_uint64("achieve", "role_uid", data.uid(), uid);
	redis_client_t::set_string("achieve", "finish_info_list", data.uid(), data.finish_info_list());
	return true;
}

bool achieve_data_manager::load_achieve_data_from_redis(uint64_t role_uid, proto::common::achieve_data* data)
{
	if (data == NULL)
	{
		log_error("load_achieve_data_from_redis [%lu] data == null", role_uid);
		return false;
	}

	std::vector<uint64_t> achieve_uids;
	redis_client_t::get_keys_by_field_condition<uint64_t, uint64_t, std::vector<uint64_t>>("achieve", "role_uid", role_uid, achieve_uids);

	for (auto& achieve_uid : achieve_uids)
	{
		uint32_t tid = redis_client_t::get_uint32("achieve", "tid", achieve_uid);
		uint32_t level = redis_client_t::get_uint32("achieve", "level", achieve_uid);
		uint32_t find_id = tid + (0 == level ? level + 1 : level);//采用基础值tid + 等级的方式查找表 等级初始为0 会查找不到所以加1
		if (NULL == GET_CONF(Achievelist, find_id))
		{
			log_error("role[%lu] achieve_tid[%d]", role_uid, find_id);
			continue;
		}

		proto::common::achieve_data_single* p_single = data->add_achieve_data_list();
		if (p_single == NULL)
			continue;
		std::string uid = string_util_t::uint64_to_string(achieve_uid);
		uint32_t count = redis_client_t::get_uint32("achieve", "count", achieve_uid);
		std::string finish_info_list = redis_client_t::get_string("achieve", "finish_info_list", achieve_uid);
		p_single->set_uid(uid);
		p_single->set_tid(tid);
		p_single->set_level(level);
		p_single->set_count(count);
		p_single->set_finish_info_list(finish_info_list);
	}
	return true;
}
