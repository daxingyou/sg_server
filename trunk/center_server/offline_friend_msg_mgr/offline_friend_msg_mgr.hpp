#ifndef __OFFLINE_FRIEND_MSG_MGR_H__
#define __OFFLINE_FRIEND_MSG_MGR_H__

#include <boost/shared_ptr.hpp>
#include <unordered_map>
#include <list>

#include "macros.hpp"
#include "client.pb.h"

class offline_friend_msg_mgr_t
{
public:
	struct offline_msg_list
	{
		std::list<proto::client::ec_personal_chat_notify> m_msg_list;
		uint32_t m_list_size;

		offline_msg_list() : m_list_size(0) {};
	};

	typedef boost::shared_ptr<offline_msg_list> offline_msg_list_ptr;

	typedef std::unordered_map<uint64_t, offline_msg_list_ptr> offline_friend_msg_map;

public:
	static void close_clear();

	static offline_msg_list_ptr get_msg_list_by_uid(uint64_t uid);

	static bool is_have_offile_chat_msg(uint64_t uid);

	static void get_offline_chat_and_delete(uint64_t uid, proto::client::ec_batch_personal_chat_notify& msg);

	static void add_offline_friend_msg(uint64_t uid, const proto::client::ec_personal_chat_notify& msg);

protected:
	static offline_friend_msg_map m_offline_friends_msg;
};

#endif
