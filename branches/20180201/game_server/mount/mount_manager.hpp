#pragma once
#include "role/role.hpp"

/*
author:wys
date:2016.12.07
desc:×øÆï¹ÜÀí
*/

class mount_manager_t
{
public:
	mount_manager_t();
	~mount_manager_t();
	
	static void		load_all_mounts(role_ptr role, const proto::common::mount_data &mount_data);
	static uint32_t add_mount(role_ptr role, uint32_t mount_tid, proto::common::role_change_data *rcd);
	static uint32_t use_mount(role_ptr role, uint32_t mount_tid, bool notify=true);
	static uint32_t turn_mount(role_ptr role, uint32_t mount_tid);
	static uint32_t cancel_turn_mount(role_ptr role, uint32_t mount_tid);
};

