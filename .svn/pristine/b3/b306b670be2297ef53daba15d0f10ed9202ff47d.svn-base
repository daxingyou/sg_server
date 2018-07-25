#ifndef __GAME_VIRTUAL_NOTICE_MANAGER_H__
#define __GAME_VIRTUAL_NOTICE_MANAGER_H__

#include "macros.hpp"

#include <boost/shared_ptr.hpp>
#include <boost/asio/deadline_timer.hpp>
#include <boost/system/error_code.hpp>

#include "common.pb.h"
#include "client.pb.h"
#include "server.pb.h"

class virtual_notice_manager_t
{
public:
	static bool init();
	static void close();

	// 定时器调用
	static void on_timer(const boost::system::error_code& ec);
	static void broadcast_virtual_notice();
	static void random_virtual_notice(proto::common::sys_notice_data* data);

protected:
	static boost::shared_ptr<boost::asio::deadline_timer> m_timer;
};

#endif
