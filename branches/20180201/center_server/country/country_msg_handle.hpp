#ifndef __COUNTRY_MSG_HANDLE_H__
#define __COUNTRY_MSG_HANDLE_H__
#include "network/tcp_socket.hpp"
#include "message.hpp"
class country_msg_handle_t
{
public:
	static bool init_gate_msg_handler();
	static bool init_game_msg_handler();

public:
	static bool handle_ce_country_preview_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_ce_country_alliance_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_ce_dismiss_alliance_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_ce_country_info_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_ce_modify_year_name_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_ce_modify_notice_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_ce_open_appoint_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_ce_appoint_officer_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_ce_fire_officer_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_ce_resign_officer_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_ce_country_family_list_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_ce_country_create_list_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_ce_country_flag_list_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_ce_change_country_name_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);

	static bool handle_ce_get_king_info_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
public:
	static bool handle_ge_gm_set_king_family_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_ge_country_name_change_reply(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_ge_country_year_name_reply(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_ge_modify_country_money_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
};

#endif
