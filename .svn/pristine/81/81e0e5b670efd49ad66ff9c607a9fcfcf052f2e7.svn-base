#ifndef __CENTER_OFFLINE_ROLE_HPP__
#define __CENTER_OFFLINE_ROLE_HPP__

#include "main/center_server_fwd.hpp"
#include "main/center_server.hpp"
#include "network/tcp_socket.hpp"
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include "common.pb.h"
#include "server.pb.h"

struct tower_trigger_data
{
	std::string uid;
	uint32_t tid;
	uint32_t expired_time;
	std::string finder_name;
};

class offline_role_t : public boost::enable_shared_from_this<offline_role_t>
{
public:
	offline_role_t(uint64_t uid);
    ~offline_role_t();

public:
	void online();
	void offline(const proto::common::offline_role_data& ord);
	void add_other_trigger(const proto::common::tower_trigger& tower_trigger);
	void add_other_trigger(const proto::server::ge_tower_trigger_notify& ntf);
	void notify_self_trigger_to_friend(uint64_t friend_uid);
private:
	void save_self();
private:
	uint64_t m_uid = 0;
	std::vector<tower_trigger_data> m_self_trigger_vec;
	std::vector<tower_trigger_data> m_other_trigger_vec;
};

typedef boost::shared_ptr<offline_role_t> offline_role_ptr;
typedef std::map<uint64_t, offline_role_ptr> offline_role_map;
#endif
