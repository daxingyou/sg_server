#ifndef __TRANSFER_CHAT_CHANNEL_HPP__
#define __TRANSFER_CHAT_CHANNEL_HPP__
#include "user/chat_user.hpp"
#include "protos_fwd.hpp"

typedef std::map<uint32_t, chat_user_map> server_channel_user_map;
class chat_channel_t
{
public:
	chat_channel_t() {}
	~chat_channel_t() {}

public:
	void init_channel(uint32_t channel_id, std::string channel_name, proto::common::channel_type channel_type);

	bool add_user(chat_user_ptr p_user);
	void earse_user(uint64_t uid);
	chat_user_ptr get_user(uint64_t uid);
	bool is_hot() { return m_max_user_count / 2 < m_user_list.size(); }

	void peek_data(proto::common::server_channel_single* p_single);

	void peek_role_list(uint32_t server_id, proto::common::fh_channel_role_list* role_list);

	template<typename T_MSG>
	int channel_send_chat_msg(uint16_t cmd, uint64_t user_uid, const T_MSG& msg, proto::common::fh_channel_role_list* role_list)
	{
		server_channel_user_map::iterator iter_server = m_server_user_list.begin();
		uint32_t server_id = 0;
		for (; iter_server != m_server_user_list.end(); ++iter_server)
		{
			server_id = iter_server->first;
			peek_role_list(server_id, role_list);
			env::server->send_msg_to_chat(server_id, cmd, user_uid, msg);
		}
		return 0;
	}

public:
	uint32_t get_channel_id() { return m_uid; }
	std::string get_channel_name() { return m_name; }
	uint64_t get_channel_users() { return m_user_list.size(); }

private:
	uint32_t m_uid = 0;
	std::string m_name = "";
	proto::common::channel_type m_channel_type = proto::common::channel_type_public;
	uint32_t m_chat_id = 0;
	uint32_t m_max_user_count = 0;

public:
	chat_user_map m_user_list;
	server_channel_user_map m_server_user_list;
};
typedef boost::shared_ptr<chat_channel_t> chat_channel_ptr;
typedef std::vector<chat_channel_ptr> composite_channel;
typedef std::map<uint64_t, chat_channel_ptr> chat_channel_map;
typedef std::map<std::string, uint64_t> chat_channel_name;
#endif // !__TRANSFER_CHAT_CHANNEL_HPP__
