#ifndef __ACHIEVE_MSG_HANDLE_H__
#define __ACHIEVE_MSG_HANDLE_H__

#include "message.hpp"
#include "network/tcp_socket.hpp"
#include "msg_handler.hpp"
#include <map>
#include "achieve.hpp"



template<uint16_t type_begin, uint16_t type_end, typename event_msg_handler_func>
class achieve_bind_func_t
{
public:
	typedef std::map<uint32_t, event_msg_handler_func> msg_handler_func_map;
	bool register_func(uint32_t type, event_msg_handler_func func)
	{
		if (type <= type_begin || type >= type_end || NULL == func)
		{
			log_error("register func for type[%d] error", type);
			return false;
		}
		if (NULL != find_func(type))
		{
			log_error("duplicate register func for type[%d]", type);
			return false;
		}
		m_funcs[type] = func;
		return true;
	}

	event_msg_handler_func find_func(uint32_t type)
	{
		if (type <= type_begin || type >= type_end)
		{
			log_error("find func for type[%d] error", type);
			return NULL;
		}
		auto itr = m_funcs.find(type);
		if (itr != m_funcs.end())
		{
			return itr->second;
		}
		else
		{
			return NULL;
		}
	}

private:
	msg_handler_func_map m_funcs;
};

class achieve_msg_handle_t
{
public:
	typedef std::function<void (const uint64_t, const proto::common::Achieve_Event_Type, uint32_t, uint32_t)> event_msg_handler_func;
public:
	static bool init_msg_handler();

	static bool init_event_handler();

	static bool init_center_msg_handler();

	static void handle_event_msg(const uint64_t uid, const proto::common::Achieve_Event_Type type, uint32_t param1, uint32_t param2)
	{
		event_msg_handler_func func = m_event_msg_handle.find_func(type);
		if (NULL == func)
		{
			log_error("no handler func for msg type[%d]", type);
			return;
		}

		func(uid, type, param1, param2);
	};

	//from client
	static bool handle_cg_get_achieve_infolist_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_cg_get_achieve_record_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_cg_get_achieve_reward_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_cg_get_active_reward_list_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_cg_get_active_reward_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);

	static bool handle_eg_notify_progress_state_notify(const network::msg_buf_ptr& msg_buf);
public:
	static achieve_bind_func_t<proto::common::Achieve_Event_Type::ACHIEVE_EVENT_TYPE_BEGIN, proto::common::Achieve_Event_Type::ACHIEVE_EVENT_TYPE_END, event_msg_handler_func> m_event_msg_handle;
};


#endif	// __ACHIEVE_MSG_HANDLE_H__