#include "cross_fight_test.hpp"
#include "main/area_server.hpp"
#include "main/area_server_fwd.hpp"

std::list<proto::common::role_cross_data> cross_fight_test_t::m_wait_list;

cross_fight_test_t::cross_fight_test_t()
{
}

cross_fight_test_t::~cross_fight_test_t()
{
}

void cross_fight_test_t::on_cross_fight_test_notify(proto::server::gr_cross_fight_test_notify ntf)
{
	m_wait_list.push_back(ntf.user());
	check_start_fight();
}

void cross_fight_test_t::check_start_fight()
{
// 	while (m_wait_list.size() > 1)
// 	{
// 		proto::server::rs_cross_fight_test_notify ntf;
// 		ntf.mutable_fight()->set_type(proto::common::fight_type_cross_test);
// 		ntf.mutable_user1()->CopyFrom(m_wait_list.front());
// 		m_wait_list.pop_front();
// 		ntf.mutable_user2()->CopyFrom(m_wait_list.front());
// 		m_wait_list.pop_front();
// 
// 		env::server->send_msg_to_cross(op_cmd::rs_cross_fight_test_notify, 0, ntf);
// 	}
}
