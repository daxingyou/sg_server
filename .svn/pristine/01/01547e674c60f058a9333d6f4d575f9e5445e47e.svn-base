#ifndef __TRANSFER_CHAT_USER_HPP__
#define __TRANSFER_CHAT_USER_HPP__
#include <boost/shared_ptr.hpp>
#include <map>
#include "main/transfer_server.hpp"
#include "main/transfer_server_fwd.hpp"

class chat_user_t
{
public:
	chat_user_t() {}
	~chat_user_t() {}

public:
	void init_chat_usser(uint64_t uid, std::string user_name, uint32_t server_id)
	{
		m_user_uid = uid;
		m_user_name = user_name;
		m_server_id = server_id;
	}

	template<typename T_MSG>
	int send_chat_msg(uint16_t cmd, const T_MSG& msg)
	{
		env::server->send_msg_to_chat(m_server_id, cmd, m_user_uid, msg);
		return 0;
	}

public:
	uint64_t get_uid() { return m_user_uid; }
	std::string get_user_name() { return m_user_name; }

	uint32_t get_server_id() { return m_server_id; }

	void set_channel_id(uint32_t channel_id) { m_channel_id = channel_id; }
	uint32_t get_channel_id() { return m_channel_id; }

private:
	uint64_t m_user_uid = 0;
	std::string m_user_name = "";
	uint32_t m_server_id = 0;
	uint32_t m_channel_id = 0;
};
typedef boost::shared_ptr<chat_user_t> chat_user_ptr;
typedef std::map<uint64_t, chat_user_ptr> chat_user_map;
#endif // !__TRANSFER_CHAT_USER_HPP__
