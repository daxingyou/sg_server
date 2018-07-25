#include "gate_msg_handler.hpp"
#include "country/country_msg_handle.hpp"
#include "field_boss/field_boss_msg_handle.hpp"
#include "friend/friend_msg_handle.hpp"
#include "mail/mail_msg_handle.hpp"
#include "chat/chat_msg_handler.hpp"
#include "challenge/challenge_msg_handle.hpp"
#include "king_war/king_war_msg_handle.hpp"
#include "shop/shop_msg_handle.hpp"
#include "user/global_user_msg_handle.hpp"
#include "world_cup/world_cup_msg_handle.hpp"
#include "user_troop/user_troop_msg_handler.hpp"
#include "scene/global_scene_msg_handle.hpp"
#include "goods/goods_msg_handle.hpp"
#include "redbag/redbag_msg_handle.hpp"
#include "family/family_msg_handle.hpp"
#include "look_role_info/simple_data_msg_handle.hpp"
#include "offline_arena/offline_arena_msg_handle.hpp"
#include "luckydraw/luckydraw_msg_handle.hpp"
#include "cdkey/cdkey_msg_handler.hpp"

USING_NS_NETWORK;
USING_NS_COMMON;

server_msg_handler_t<op_cmd::AE_BEGIN, op_cmd::AE_END, gate_msg_handler_t::msg_handler_func> gate_msg_handler_t::m_gate_msg_handle;

server_msg_handler_t<op_cmd::CE_BEGIN, op_cmd::CE_END, gate_msg_handler_t::msg_handler_func> gate_msg_handler_t::m_gate_msg_handle_from_client;

bool gate_msg_handler_t::init_msg_handler()
{
    bool ret = true;
    ret &= m_gate_msg_handle.register_func(op_cmd::ae_syn_gate_info_notify, handle_ae_syn_gate_info_notify);
	ret &= m_gate_msg_handle.register_func(op_cmd::ae_reg_user_center_request, handle_ae_reg_user_center_request);
	ret &= m_gate_msg_handle.register_func(op_cmd::ae_create_role_request, handle_ae_create_role_request);
	ret &= m_gate_msg_handle.register_func(op_cmd::ae_unreg_user_scene_notify, handle_ae_unreg_user_scene_notify);
	ret &= m_gate_msg_handle.register_func(op_cmd::ae_user_cross_server_notify, handle_ae_user_cross_server_notify);

	// scene messgae
	if (!global_scene_msg_handle_t::init_gate_msg_handler())
	{
		log_error("init global_scene msg handler failed!");
		return false;
	}

    // country message
	if (!country_msg_handle_t::init_gate_msg_handler())
	{
		log_error("init country gate msg handler failed!");
		return false;
	}

	// family message
	if (!family_msg_handle::init_gate_msg_handler())
	{
		log_error("init family gate msg handler failed!");
		return false;
	}

	// boss messgae
	if (!field_boss_msg_handle_t::init_gate_msg_handler())
	{
		log_error("init field_boss msg handler failed!");
		return false;
	}

	// friend messgae
	if (!friend_msg_handle_t::init_msg_handler())
	{
		log_error("init friend msg handler failed!");
		return false;
	}

	// mail messgae
	if (!mail_msg_handle_t::init_gate_msg_handler())
	{
		log_error("init mail msg handler failed!");
		return false;
	}

	// chat message
	if (!chat_msg_handler_t::init_client_msg_handler())
	{
		log_error("init chat msg handler failed!");
		return false;
	}

	// challenge message
	if (!challenge_msg_handle_t::init_client_msg_handler())
	{
		log_error("init challenge msg handler failed!");
		return false;
	}

	// challenge message
	if (!user_troop_msg_handle_t::init_msg_handler())
	{
		log_error("init troop msg handler failed!");
		return false;
	}

	// king war messgae
	if (!king_war_msg_handle_t::init_gate_msg_handler())
	{
		log_error("init king_war msg handler failed!");
		return false;
	}

	// shop message
	if (!shop_msg_handle_t::init_gate_msg_handler())
	{
		log_error("init shop msg handler failed!");
		return false;
	}

	// rank message
	if (!global_user_msg_handle_t::init_client_msg_handler())
	{
		log_error("init rank msg handler failed!");
		return false;
	}

	// world_cup message
	if (!world_cup_msg_handle_t::init_client_msg_handler())
	{
		log_error("init world_cup msg handler failed!");
		return false;
	}

	// goods message
	if (!goods_msg_handle_t::init_client_msg_handler())
	{
		log_error("init goods msg handler failed!");
		return false;
	}

	// redbag message
	if (!redbag_msg_handle_t::init_client_msg_handler())
	{
		log_error("init redbag msg handler failed!");
		return false;
	}

	// look_role_info message
	if (!simple_data_msg_handle_t::init_client_msg_handler())
	{
		log_error("init look_role_info msg handler failed!");
		return false;
	}

	// offline_arena message
	if (!offline_arena_msg_handle_t::init_client_msg_handler())
	{
		log_error("init offline_arena msg handler failed!");
		return false;
	}

	if (!luckydraw_msg_handle_t::init_gate_msg_handler())
	{
		log_error("init luckydraw msg handler failed!");
		return false;
	}
	
	if ( !cdkey_msg_handler_t::init_gate_msg_handler() )
	{ 
		log_error("init gate msg handler failed!");
		return false;
	}

    return ret;
}

bool gate_msg_handler_t::handle_ae_syn_gate_info_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
    PRE_S2S_MSG(proto::server::ae_syn_gate_info_notify);

    gate_manager_t::set_gate(s, msg.gate_id());

    return true;
}

bool gate_msg_handler_t::handle_ae_reg_user_center_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::ae_reg_user_center_request);

	global_user_manager_t::reg_global_user(s, uid);

	return true;
}

bool gate_msg_handler_t::handle_ae_create_role_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::ae_create_role_request);

	global_user_manager_t::create_global_user(s, uid, msg.nickname(), msg.role_type());
	
	return true;
}

bool gate_msg_handler_t::handle_ae_unreg_user_scene_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::ae_unreg_user_scene_notify);

	global_user_manager_t::unreg_global_user(uid, msg.unreg_code(), msg.kick_reason());

	return true;
}

bool gate_msg_handler_t::handle_ae_user_cross_server_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::ae_user_cross_server_notify);

	global_user_manager_t::user_cross_server(uid, msg.data());

	return true;
}
