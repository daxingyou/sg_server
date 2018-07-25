#ifndef __GLOBAL_USER_MSG_HANDLE_H__
#define __GLOBAL_USER_MSG_HANDLE_H__

#include "network/tcp_socket.hpp"
#include "message.hpp"
#include "msg_handler.hpp"
#include "protos_fwd.hpp"
#include "global_user_manager.hpp"

class global_user_msg_handle_t
{
public:
	typedef bool(*data_sync_func)(uint64_t uid, std::string val);

public:
	static bool init_game_msg_handler();

	static bool init_data_syncers();

	static bool init_client_msg_handler();
public:
	static bool handle_ce_rank_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_ce_rank_time_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
public:

    static bool handle_ge_syn_game_info_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_ge_update_rank_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_ge_sync_data_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_ge_reload_config(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_ge_ready_enter_scene_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
    static bool handle_ge_pk_beat_criminal_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_ge_unreg_user_scene_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_ge_reg_user_scene_reply(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_ge_chat_get_role_info_reply(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_ge_user_cross_scene_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_ge_tower_trigger_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_ge_offline_data_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	// 数据同步
public:
	template<typename valtype>
	static void sync_data_to_game(uint64_t uid, proto::server::data_sync_type type, valtype val, bool is_sync_now = false)
	{
		if (m_sync_uid != 0 && m_sync_uid != uid)
		{
			global_user_ptr p_user = global_user_manager_t::get_global_user_by_uid(m_sync_uid);
			if (p_user != NULL)
			{
				p_user->send_msg_to_game(op_cmd::eg_sync_data_request, m_sync_datas);
			}
			proto::server::eg_sync_data_request swap_data;
			m_sync_datas.Swap(&swap_data);
			m_sync_uid = 0;
		}

		m_sync_uid = uid;

		proto::server::data_sync_item* p_item = m_sync_datas.add_items();
		if (NULL != p_item)
		{
			p_item->set_type(type);
			p_item->set_value(boost::lexical_cast<std::string>(val));
		}

		if (is_sync_now)
		{
			global_user_ptr p_user = global_user_manager_t::get_global_user_by_uid(m_sync_uid);
			if (p_user != NULL)
			{
				p_user->send_msg_to_game(op_cmd::eg_sync_data_request, m_sync_datas);
			}
			proto::server::eg_sync_data_request swap_data;
			m_sync_datas.Swap(&swap_data);
			m_sync_uid = 0;
		}
	}

	static bool on_sync_level(uint64_t uid, std::string val);
	static bool on_sync_fighting_value(uint64_t uid, std::string val);
	static bool on_country_money_change(uint64_t uid, std::string val);
	static bool on_sync_server_time(uint64_t uid, std::string val);
	static bool on_gm_add_family_money(uint64_t uid, std::string val);
	static bool on_sync_family_contribution(uint64_t uid, std::string val);
	static bool on_gm_summon_boss(uint64_t uid, std::string val);
	static bool on_sync_head_frame(uint64_t uid, std::string val);
	static bool on_sync_main_plugin(uint64_t uid, std::string val);

public:
	static network::server_msg_handler_t<proto::server::data_sync_type_begin, proto::server::data_sync_type_end, data_sync_func> m_data_syncer;
	static proto::server::eg_sync_data_request m_sync_datas;
	static uint64_t m_sync_uid;
};

#endif // !__GLOBAL_USER_MSG_HANDLE_H__