#ifndef __TRANSFER_CHAT_CHANNEL_MANAGER_HPP__
#define __TRANSFER_CHAT_CHANNEL_MANAGER_HPP__
#include "chat_channel.hpp"

class chat_channel_manager_t
{
public:
	static void init_channel();
	static uint64_t add_channel(std::string channel_name, proto::common::channel_type channel_type);
	static chat_channel_ptr get_channel(uint32_t channel_id);
	static void erase_channel(uint32_t channel_id);

	static bool add_user(chat_user_ptr p_user, uint32_t channel_id);
	static chat_user_ptr get_user(uint64_t uid, uint32_t channel_id);
	static void earse_user(uint64_t uid, uint32_t channel_id);

	static void peek_all_datas(proto::common::server_channel_data* p_data);
	static bool is_need_add_channel();

//	static void notify_all_channel_info();
	static std::string make_first_name(std::string channel_name);
	static std::string make_next_name(std::string channel_name);
private:
	static chat_channel_map m_chat_channels;
	static chat_channel_name m_channel_names;
	static composite_channel m_public_channels;
	static uint32_t m_start_channel_id;
};
#endif // !__TRANSFER_CHAT_CHANNEL_MANAGER_HPP__
