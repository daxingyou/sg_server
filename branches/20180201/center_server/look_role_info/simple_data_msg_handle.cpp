#include "simple_data_msg_handle.hpp"

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
#include "simple_data_manager.hpp"
#include "tblh/errcode_enum.hpp"
#include "tblh/Comprehensive.tbls.h"

USING_NS_COMMON;
bool simple_data_msg_handle_t::init_client_msg_handler()
{
	bool bresult = true;
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::ce_other_role_info_request, handle_ce_other_role_info_request);
	return bresult;
}

bool simple_data_msg_handle_t::init_game_msg_handler()
{
	bool bresult = true;
	bresult &= game_msg_handler_t::m_game_msg_handle.register_func(op_cmd::ge_role_simple_data_notify, handle_ge_role_simple_data_notify);
	return bresult;
}

bool simple_data_msg_handle_t::handle_ce_other_role_info_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::ce_other_role_info_request);
	global_user_ptr p_user = global_user_manager_t::get_global_user_by_uid(uid);

	if (NULL == p_user)
	{
		log_error("user[%lu] null error", uid);
		return false;
	}

	const uint64_t other_uid = string_util_t::string_to_uint64(msg.uid());

	return simple_data_manager_t::get_other_role_info(uid, other_uid);
}

bool simple_data_msg_handle_t::handle_ge_role_simple_data_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::ge_role_simple_data_notify);
	global_user_ptr p_user = global_user_manager_t::get_global_user_by_uid(uid);

	if (NULL == p_user)
	{
		log_error("user[%lu] null error", uid);
		return false;
	}
	
	return simple_data_manager_t::update_role_simle_info(uid,msg.simple_data(),msg.type());
}