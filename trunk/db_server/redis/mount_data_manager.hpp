#pragma once
#include "common/macros.hpp"
#include "protos_fwd.hpp"

class mount_data_manager
{
public:
	mount_data_manager();
	~mount_data_manager();

	static bool save_mount_data_to_redis(uint64_t uid, const proto::common::mount_data& mount_data);
	static bool load_mount_data_from_redis(uint64_t uid, proto::common::mount_data* mount_data);
};