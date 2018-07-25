#ifndef __GAME_FRIEND_MANAGER_H_
#define __GAME_FRIEND_MANAGER_H_

#include "server.pb.h"
#include "macros.hpp"

namespace proto
{

namespace common
{
	class role_change_data;
}

}

class friend_manager_t
{
private:
	static bool m_close;
public:
	static int32_t handle_friend_point_operation(uint64_t role_id, proto::server::friend_point_operation op, int32_t parameter1, int32_t parameter2, proto::common::role_change_data* change_data, int32_t& info1, int32_t& info2, std::string* p_info3, int32_t& info4);
	static void close(){m_close = true;}
	static void open(){m_close = false;}
	static bool isClose(){return m_close;}
};

#endif