#pragma once

#include "macros.hpp"
#include "config_mgr.h"
#include "protos_fwd.hpp"

enum other_info_type : uint32_t
{
	other_info_base,
	other_info_all,
};


class simple_data_manager_t
{
public:
	static bool get_other_role_info(const uint64_t uid, const uint64_t other_uid,uint32_t type = 0);
	//static bool update_role_simle_info(const uint64_t uid, proto::common::role_blob_data& notify,proto::common::role_data_type type = proto::common::simple_info_type);
	static bool save_simle_info_to_redis(const uint64_t uid, const proto::common::role_blob_data& notify, proto::common::role_data_type type = proto::common::simple_info_type);

	static bool load_simle_info_from_redis(const uint64_t uid, proto::common::role_blob_data& notify,proto::common::role_data_type type = proto::common::simple_info_type);
};