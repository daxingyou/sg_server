#ifndef __CENTER_OFFLINE_ROLE_MANAGER_HPP__
#define __CENTER_OFFLINE_ROLE_MANAGER_HPP__

#include "offline_role.hpp"
#include "protos_fwd.hpp"
#include <map>

#include "main/center_server_fwd.hpp"
#include "main/center_server.hpp"

class offline_role_manager_t
{
public:
	offline_role_manager_t() {}
	~offline_role_manager_t() {}

public:
	static void online(uint64_t role_uid);

	static void offline(uint64_t role_uid, const proto::common::offline_role_data& ord);

	static void add_other_trigger(uint64_t role_uid, const proto::common::tower_trigger& tower_trigger);

	static void add_other_trigger(uint64_t role_uid, const proto::server::ge_tower_trigger_notify& ntf);

	static void notify_self_trigger_to_friend(uint64_t self_uid, uint64_t friend_uid);
private:
    static offline_role_map m_offline_role_map;
};

#endif
