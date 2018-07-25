#pragma once
#include "arena_role.hpp"
#include "protos_fwd.hpp"
#include <map>

/*
author:wys
date:2017.2.16
desc:竞技场角色管理器
*/


class arena_role_manager_t
{
public:
	typedef std::map<uint64_t, arena_role_ptr> ARENA_ROLE_MAP;
public:
	static void add_role(arena_role_ptr p_role);
	static arena_role_ptr add_role(uint64_t role_uid, uint32_t score, uint32_t arena_level, const proto::common::role_cross_data &cross_data);
	static arena_role_ptr find_role(uint64_t role_uid);

	static void clear_some_role();
private:
	static ARENA_ROLE_MAP m_role_map;
};