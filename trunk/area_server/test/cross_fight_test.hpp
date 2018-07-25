#pragma once
#include <list>
#include "server.pb.h"
#include "protos_fwd.hpp"
#include "tcp_socket.hpp"
#include "cmd_desc.hpp"
class cross_fight_test_t
{
public:
	cross_fight_test_t();
	~cross_fight_test_t();

	static void on_cross_fight_test_notify(proto::server::gr_cross_fight_test_notify ntf);

protected:

	static void check_start_fight();

private:

	static std::list<proto::common::role_cross_data> m_wait_list;

};