#include "redbag_msg_handle.hpp"

#include "gate/gate_msg_handler.hpp"
#include "game/game_msg_handler.hpp"

#include "common.pb.h"
#include "client.pb.h"
#include "server.pb.h"

#include "user/global_user_manager.hpp"
#include "utility.hpp"
#include "main/center_server_fwd.hpp"
#include "main/center_server.hpp"
#include "user/global_user.hpp"
#include "redbag_manager.hpp"
#include "tblh/errcode_enum.hpp"

USING_NS_COMMON;

bool redbag_msg_handle_t::init_client_msg_handler()
{
	bool bresult = true;

	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::ce_redbag_draw_request, handle_ce_redbag_draw_request);
	return bresult;
}

bool redbag_msg_handle_t::init_game_msg_handler()
{
	bool bresult = true;

	bresult &= game_msg_handler_t::m_game_msg_handle.register_func(op_cmd::ge_redbag_send_notify, handle_ge_redbag_send_notify);
	return bresult;
}

bool redbag_msg_handle_t::handle_ce_redbag_draw_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::ce_redbag_draw_request);
	global_user_ptr p_user = global_user_manager_t::get_global_user_by_uid(uid);

	if (NULL == p_user)
	{
		log_error("user[%lu] null error", uid);
		return false;
	}
	uint64_t redbag_uid = string_util_t::string_to_uint64(msg.redbag_uid());
	proto::client::ec_redbag_draw_reply reply;
	uint32_t reply_code = redbag_manager_t::draw(redbag_uid, p_user, reply.mutable_redbag_data());
	reply.set_reply_code(reply_code);
	p_user->send_msg_to_client(op_cmd::ec_redbag_draw_reply, reply);
	return true;
}

bool redbag_msg_handle_t::handle_ge_redbag_send_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::ge_redbag_send_notify);
	global_user_ptr p_user = global_user_manager_t::get_global_user_by_uid(uid);

	if (NULL == p_user)
	{
		log_error("user[%lu] null error", uid);
		return false;
	}
	redbag_manager_t::send(p_user, msg.count(), msg.money());
	
	return true;
}
