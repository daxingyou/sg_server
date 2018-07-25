#include "mount_data_manager.hpp"
#include "redis_client.hpp"
#include "common/config_mgr.h"
#include "tblh/MountTable.tbls.h"

mount_data_manager::mount_data_manager()
{
}

mount_data_manager::~mount_data_manager()
{
}

bool mount_data_manager::save_mount_data_to_redis(uint64_t uid, const proto::common::mount_data& mount_data)
{
	std::ostringstream ss;

	for (int i = 0; i < mount_data.mount_data_list_size(); ++i)
	{
		ss.str("");
		ss << uid << mount_data.mount_data_list(i).mount_tid();

		redis_client_t::set_uint64("role_mount", "role_uid", ss.str(), uid);
		redis_client_t::set_uint32("role_mount", "mount_tid", ss.str(), mount_data.mount_data_list(i).mount_tid());
		redis_client_t::set_uint32("role_mount", "mount_state", ss.str(), mount_data.mount_data_list(i).mount_state());
		
	}

	return true;
}

bool mount_data_manager::load_mount_data_from_redis(uint64_t uid, proto::common::mount_data* mount_data)
{
	if (mount_data == NULL)
	{
		log_error("load_mount_data_from_redis [%lu] data == null", uid);
		return false;
	}

	std::vector<std::string> mount_tids;
	redis_client_t::get_keys_by_field_condition<std::string, uint64_t, std::vector<std::string>>("role_mount", "role_uid", uid, mount_tids);

	for (auto mount_tid : mount_tids)
	{
		uint32_t tid = redis_client_t::get_uint32("role_mount", "mount_tid", mount_tid);
		if (NULL == GET_CONF(MountTable, tid))
		{
			log_warn("[wys] not find this mount:%d", tid);
			continue;
		}
		proto::common::mount_data_single* single = mount_data->add_mount_data_list();
		if (single == NULL)
			continue;

		single->set_mount_tid(tid);
		uint32_t state = redis_client_t::get_uint32("role_mount", "mount_state", mount_tid);
		single->set_mount_state((proto::common::mount_state_type)state);

	}

	return true;
}
