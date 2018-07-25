#include "chat.hpp"
#include "main/transfer_server_fwd.hpp"
#include "main/transfer_server.hpp"
#include "user/chat_user.hpp"

void chat_t::add_chat_user(chat_user_ptr p_user)
{
	m_user_list.insert(std::make_pair(p_user->get_uid(), p_user));
}

void chat_t::erase_chat_user(uint64_t user_uid)
{
	m_user_list.erase(user_uid);
}

chat_user_ptr chat_t::get_user(uint64_t user_uid)
{
	auto it = m_user_list.find(user_uid);
	if (it != m_user_list.end())
	{
		return it->second;
	}
	return chat_user_ptr();
}
