#include "game_msg_handler.hpp"
#include "main/db_server.hpp"
#include "main/db_server_fwd.hpp"
#include "network.hpp"
#include "center/center_msg_handler.hpp"
#include "redis/redis_data_manager.hpp"
#include "redis/role_data_manager.hpp"
#include "tblh/errcode_enum.hpp"
#include "config/config_manager.hpp"
#include "common/DataManager.h"

USING_NS_NETWORK;
USING_NS_COMMON;

server_msg_handler_t<op_cmd::GD_BEGIN, op_cmd::GD_END, game_msg_handler_t::msg_handler_func> game_msg_handler_t::m_game_msg_handler;

bool game_msg_handler_t::init_msg_handler()
{
    bool ret = true;

    ret &= m_game_msg_handler.register_func(op_cmd::gd_syn_game_info_notify,            handle_gd_syn_game_info_notify);
    ret &= m_game_msg_handler.register_func(op_cmd::gd_role_data_request,               handle_gd_role_data_request);
    ret &= m_game_msg_handler.register_func(op_cmd::gd_batch_save_to_db_notify,         handle_gd_batch_save_to_db_notify);
	ret &= m_game_msg_handler.register_func(op_cmd::gd_reload_config_notify,			handle_gd_reload_config_notify);
	ret &= m_game_msg_handler.register_func(op_cmd::gd_change_name_request,				handle_gd_change_name_request);
	return ret;
}

bool game_msg_handler_t::handle_gd_syn_game_info_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
    PRE_S2S_MSG(proto::server::gd_syn_game_info_notify);

    game_manager_t::set_game(s, msg.game_id());

    return true;
}

bool game_msg_handler_t::handle_gd_role_data_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
    PRE_S2S_MSG(proto::server::gd_role_data_request);
    //log_warn("##### diff time^ %d", time_util_t::now_msecs() - msg.time());

	redis_data_manager_t::handle_get_role_data(uid, s);

    return true;
}

bool game_msg_handler_t::handle_gd_batch_save_to_db_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
    PRE_S2S_MSG_NO_UID(proto::server::gd_batch_save_to_db_notify);
	
    redis_data_manager_t::handle_batch_save_to_db(msg);
    
    return true;
}

bool game_msg_handler_t::handle_gd_reload_config_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	reloadAllDTManager(env::xml->config_path);
	return true;
}

bool game_msg_handler_t::handle_gd_change_name_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::gd_change_name_request);

	proto::server::dg_change_name_reply reply;
	role_data_manager::change_name(uid, msg, reply);
	env::server->send_msg_to_game(s, op_cmd::dg_change_name_reply, uid, reply);
	return true;
}
