#include "game_msg_handler.hpp"
#include "user/global_user_msg_handle.hpp"
#include "city/city_msg_handler.hpp"
#include "field_boss/field_boss_msg_handle.hpp"
#include "mail/mail_msg_handle.hpp"
#include "item/item_msg_handler.hpp"
#include "challenge/challenge_msg_handle.hpp"
#include "king_war/king_war_msg_handle.hpp"
#include "shop/shop_msg_handle.hpp"
#include "scene/global_scene_msg_handle.hpp"
#include "goods/goods_msg_handle.hpp"
#include "redbag/redbag_msg_handle.hpp"
#include "family/family_msg_handle.hpp"
#include "offline_fight/offline_fight_msg_handle.hpp"
#include "user_troop/user_troop_msg_handler.hpp"
#include "sys_notice/sys_notice_msg_handle.hpp"
#include "look_role_info/simple_data_msg_handle.hpp"
#include "offline_arena/offline_arena_msg_handle.hpp"
#include "country/country_msg_handle.hpp"
#include "luckydraw/luckydraw_msg_handle.hpp"
#include "general_event/general_event_msg_handler.hpp"

USING_NS_NETWORK;
USING_NS_COMMON;

server_msg_handler_t<op_cmd::GE_BEGIN, op_cmd::GE_END, game_msg_handler_t::msg_handler_func> game_msg_handler_t::m_game_msg_handle;

bool game_msg_handler_t::init_msg_handler()
{
	// user message
	if (!global_user_msg_handle_t::init_game_msg_handler())
	{
		log_error("init global user msg handler failed!");
		return false;
	}

	// scene messgae
	if (!global_scene_msg_handle_t::init_game_msg_handler())
	{
		log_error("init global_scenemsg handler failed!");
		return false;
	}

	// city message
	if (!city_msg_handler_t::init_msg_handler())
	{
        log_error("init city msg handler failed!");
		return false;
	}

	// boss messgae
	if (!field_boss_msg_handle_t::init_game_msg_handler())
	{
        log_error("init field_boss msg handler failed!");
		return false;
	}

	// mail messgae
	if (!mail_msg_handle_t::init_game_msg_handler())
	{
        log_error("init mail msg handler failed!");
		return false;
	}

	// item message
	if (!item_msg_handler_t::init_msg_handler())
	{
        log_error("init item msg handler failed!");
		return false;
	}

	// chanllenge message
	if (!challenge_msg_handle_t::init_game_msg_handler())
	{
        log_error("init chanllenge msg handler failed!");
		return false;
	}

	// king war messgae
	if (!king_war_msg_handle_t::init_game_msg_handler())
	{
        log_error("init king_war msg handler failed!");
		return false;
	}

	// shop messgae
	if (!shop_msg_handle_t::init_game_msg_handler())
	{
        log_error("init shop msg handler failed!");
		return false;
	}

	// goods messgae
	if (!goods_msg_handle_t::init_game_msg_handler())
	{
		log_error("init goods msg handler failed!");
		return false;
	}

	// offline_gight messgae
	if (!offline_fight_msg_handle_t::init_game_msg_handler())
	{
		log_error("init offline_gight msg handler failed!");
		return false;
	}

	// country message
	if (!country_msg_handle_t::init_game_msg_handler())
	{
		log_error("init country game msg handler failed!");
		return false;
	}

	// family message
	if (!family_msg_handle::init_game_msg_handler())
	{
		log_error("init family game_msg handler failed!");
		return false;
	}

	// troop message
	if (!user_troop_msg_handle_t::init_game_msg_handler())
	{
		log_error("init troop game_msg handler failed!");
		return false;
	}

	// sys_notify messgae
	if (!sys_notice_msg_handle_t::init_msg_handler())
	{
		log_error("init sys_notify msg handler failed!");
		return false;
	}

	// redbag messgae
	if (!redbag_msg_handle_t::init_game_msg_handler())
	{
		log_error("init redbag msg handler failed!");
		return false;
	}

	// look_role_info messgae
	if (!simple_data_msg_handle_t::init_game_msg_handler())
	{
		log_error("init look_role_info msg handler failed!");
		return false;
	}

	// offline_arena messgae
	if (!offline_arena_msg_handle_t::init_game_msg_handler())
	{
		log_error("init offline_arena msg handler failed!");
		return false;
	}
	
	if (!luckydraw_msg_handle_t::init_game_msg_handler())
	{
		log_error("init luckydraw msg handler failed!");
		return false;
	}

	if (!general_event_msg_handler_t::init_game_msg_handler())
	{
		log_error("init general_event msg handler failed!");
		return false;
	}


    return true;
}
