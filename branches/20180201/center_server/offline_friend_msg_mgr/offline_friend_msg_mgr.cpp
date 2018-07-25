#include "offline_friend_msg_mgr.hpp"
#include "log.hpp"
#include "friend/friend_manager.hpp"


offline_friend_msg_mgr_t::offline_friend_msg_map offline_friend_msg_mgr_t::m_offline_friends_msg;

void offline_friend_msg_mgr_t::add_offline_friend_msg(uint64_t uid, const proto::client::ec_personal_chat_notify& msg)
{
	offline_msg_list_ptr p_list_ptr = get_msg_list_by_uid(uid);
	
	// 列表中没有，新加一个列表
	if (p_list_ptr == NULL)
	{
		p_list_ptr = offline_msg_list_ptr(new offline_msg_list());

		if (p_list_ptr == NULL)
		{
			log_error("role[%lu] add p_list_ptr null error", uid);
			return;
		}

		m_offline_friends_msg.insert(std::make_pair(uid, p_list_ptr));
	}

	if (p_list_ptr != NULL)
	{
		p_list_ptr->m_msg_list.push_back(msg);
		p_list_ptr->m_list_size++;

		// check size
		if (p_list_ptr->m_list_size > friend_manager_t::get_chat_max_content())
		{
			p_list_ptr->m_msg_list.pop_front();

			if (p_list_ptr->m_list_size > 0)
				p_list_ptr->m_list_size--;
		}
	}
}

void offline_friend_msg_mgr_t::close_clear()
{
	m_offline_friends_msg.clear();
}

offline_friend_msg_mgr_t::offline_msg_list_ptr offline_friend_msg_mgr_t::get_msg_list_by_uid(uint64_t uid)
{
	offline_friend_msg_map::iterator itr = m_offline_friends_msg.find(uid);
	if (itr != m_offline_friends_msg.end())
	{
		return itr->second;
	}

	return NULL;
}


bool offline_friend_msg_mgr_t::is_have_offile_chat_msg(uint64_t uid)
{
	offline_friend_msg_map::iterator itr = m_offline_friends_msg.find(uid);

	if (itr != m_offline_friends_msg.end())
	{
		return true;
	}

	return false;
}

void offline_friend_msg_mgr_t::get_offline_chat_and_delete(uint64_t uid, proto::client::ec_batch_personal_chat_notify& msg)
{
	offline_friend_msg_map::iterator itr = m_offline_friends_msg.find(uid);

	if (itr != m_offline_friends_msg.end())
	{
		offline_msg_list_ptr p_msg_list = itr->second;
		if (p_msg_list != NULL && p_msg_list->m_list_size > 0)
		{
			uint32_t max_save_time = friend_manager_t::get_max_save_time();
			uint32_t time_now = common::time_util_t::now_time();

			for (std::list<proto::client::ec_personal_chat_notify>::iterator list_itr = p_msg_list->m_msg_list.begin();
				list_itr != p_msg_list->m_msg_list.end(); ++list_itr)
			{
				const proto::client::ec_personal_chat_notify& notify = *list_itr;

				// 已经超过最大保存时间的，直接跳过
				if (notify.contact_time() + max_save_time < time_now)
					continue;

				proto::client::ec_personal_chat_notify* p_new_msg = msg.add_chat_msgs();
				if (p_new_msg != NULL)
				{
					p_new_msg->CopyFrom(notify);
				}
			}
		}

		m_offline_friends_msg.erase(itr);
	}
}
