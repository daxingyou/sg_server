#include "gate_msg_handler.hpp"
#include "network.hpp"
#include "item/item_msg_handle.hpp"
#include "role/role_msg_handle.hpp"
#include "scene/scene_msg_handle.hpp"
#include "formation/formation_msg_handle.hpp"
#include "task/task_msg_handle.hpp"
#include "country/country_msg_handle.hpp"
#include "family/family_msg_handle.hpp"
#include "item/item_msg_handle.hpp"
//#include "equip/equip_msg_handle.hpp"
#include "utility.hpp"
#include "fight/fight_manager.hpp"
#include "fight/fight_msg_handle.hpp"
#include "trade/trade_msg_handler.hpp"
#include "mount/mount_msg_handler.hpp"
#include "field_boss/field_boss_msg_handle.hpp"
#include "fight_study/fight_study_msg_handle.hpp"
#include "activity/activity_msg_handle.hpp"
#include "arena/arena_msg_handler.hpp"
#include "treasure/treasure_msg_handle.hpp"
#include "collect/collect_msg_handler.hpp"
#include "pk/pk_msg_handle.hpp"
#include "king_war/king_war_msg_handle.hpp"
#include "shop/shop_msg_handle.hpp"
#include "equip/equip_msg_handle.hpp"
#include "achieve/achieve_msg_handle.hpp"
#include "tower/tower_msg_handle.hpp"
#include "red_point/red_point_msg_handle.hpp"
#include "luckydraw/luckydraw_msg_handle.hpp"
#include "lifeforce/lifeforce_msg_handle.hpp"
#include "dungeon/dungeon_msg_handle.hpp"
#include "expedition/expedition_msg_handle.hpp"
#include "page/page_msg_handle.hpp"
#include "user_troop/user_troop_msg_handler.hpp"
#include "multi_time/multi_time_msg_handler.hpp"
#include "business_activity/business_activity_msg_handler.hpp"

USING_NS_NETWORK;
USING_NS_COMMON;

server_msg_handler_t<op_cmd::CG_BEGIN, op_cmd::CG_END, gate_msg_handler_t::msg_handler_func> gate_msg_handler_t::m_gate_msg_handle_from_client;
server_msg_handler_t<op_cmd::AG_BEGIN, op_cmd::AG_END, gate_msg_handler_t::msg_handler_func> gate_msg_handler_t::m_gate_msg_handle;

bool gate_msg_handler_t::init_msg_handler()
{
    // gate --> game
    m_gate_msg_handle.register_func(op_cmd::ag_syn_gate_info_notify,    handle_ag_syn_gate_info_notify);
	m_gate_msg_handle.register_func(op_cmd::ag_unreg_user_scene_notify, handle_ag_unreg_user_scene_notify);
	m_gate_msg_handle.register_func(op_cmd::ag_role_enter_game_notify, handle_ag_role_enter_game_notify);
    
    // client --> gate -- > game
    //user_msg_handle_t::init_msg_handler();
	//item_msg_handle_t::init_msg_handler();
	// role message
	if (!role_msg_handle_t::init_msg_handler())
	{
		log_error("init role msg handler failed!");
		return false;
	}
	// scene message
	if (!scene_msg_handle_t::init_gate_msg_handler())
	{
		log_error("init scene msg handler failed!");
		return false;
	}
	// dungeon message
	if (!dungeon_msg_handle_t::init_gate_msg_handler())
	{
		log_error("init dungeon_msg handler failed!");
		return false;
	}

	// formation message
	if (!formation_msg_handle_t::init_msg_handler())
	{
		log_error("init formation msg handler failed!");
		return false;
	}

	// task message
	if (!task_msg_handle_t::init_msg_handler())
	{
		log_error("init task msg handler failed!");
		return false;
	}
	// country message
	if (!country_msg_handle_t::init_gate_msg_handler())
	{
		log_error("init country msg handler failed!");
		return false;
	}

    // family message
	if (!family_msg_handle_t::init_gate_msg_handler())
	{
		log_error("init family msg handler failed!");
		return false;
	}

	// troop message
	if (!user_troop_msg_handle_t::init_gate_msg_handler())
	{
		log_error("init troop msg handler failed!");
		return false;
	}

	if (!item_msg_handle_t::init_msg_handler())
	{
		log_error("init item msg handler failed!");
		return false;
	}

	// fight
	if (!fight_msg_handle_t::init_msg_handler())
	{
		log_error("init fight msg handler failed");
		return false;
	}


	//trade_item
	if (!trade_msg_handler_t::init_msg_handler())
	{
		log_error("init trade_item msg handler failed");
		return false;
	}
	//mount
	if (!mount_msg_handler_t::init_msg_handler())
	{
		log_error("init mount msg handler failed");
		return false;
	}

	// field boss message
	if (!field_boss_msg_handle_t::init_gate_msg_handler())
	{
		log_error("init field_boss msg handler failed!");
		return false;
	}

	// fight study
	if (!fight_study_msg_handle_t::init_msg_handler())
	{
		log_error("init fight_study msg handler failed!");
		return false;
	}
	
	// activity
	if (!activity_msg_handle_t::init_msg_handler())
	{
		log_error("init activity msg handler failed!");
		return false;
	}

	// arena
	if (!arena_msg_handler_t::init_msg_handler())
	{
		log_error("init arena msg handler failed");
		return false;
	}

	// treasure
	if (!treasure_msg_handle_t::init_msg_handler())
	{
		log_error("init treasure msg handler failed");
		return false;
	}

	// collect
	if (!collect_msg_handler_t::init_msg_handler())
	{
		log_error("init collect msg handler failed");
		return false;
	}
	
	// pk
	if (!pk_msg_handle_t::init_msg_handler())
	{
		log_error("init pk msg handler failed");
		return false;
	}

	// king war message
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

	// equip message
	if (!equip_msg_handle_t::init_msg_handler())
	{
		log_error("init equip msg handler failed!");
		return false;
	}

	//achieve message
	if (!achieve_msg_handle_t::init_msg_handler())
	{
		log_error("init achieve msg handler failed!");
		return false;
	}

	//tower message
	if (!tower_msg_handle_t::init_msg_handler())
	{
		log_error("init tower msg handler failed!");
		return false;
	}

	// red_point message
	if (!red_point_msg_handle_t::init_gate_msg_handler())
	{
		log_error("init red_point msg handler failed!");
		return false;
	}

	//luckydraw message
	if (!luckydraw_msg_handle_t::init_msg_handler())
	{
		log_error("init luckydraw msg handler failed!");
		return false;
	}
	//lifeforce message
	if (!lifeforce_msg_handle_t::init_msg_handler())
	{
		log_error("init lifeforce msg handler failed!");
		return false;
	}
	//expedition message
	if (!expedition_msg_handle_t::init_client_msg_handler())
	{
		log_error("init expedition msg handler failed!");
		return false;
	}

	//page message
	if (!page_msg_handler_t::init_msg_handler())
	{
		log_error("init page msg handler failed!");
		return false;
	}

	//multi_time message
	if (!multi_time_msg_handler_t::init_msg_handler())
	{
		log_error("init multi_time msg handler failed!");
		return false;
	}

	//business_activity_msg_handler
	if (!business_activity_msg_handler_t::init_msg_handler())
	{
		log_error("init business_activity_msg_handler msg handler failed!");
		return false;
	}

    return true;
}

// gate --> game------------------------------------------------------------------------------------
bool gate_msg_handler_t::handle_ag_syn_gate_info_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
    PRE_S2S_MSG(proto::server::ag_syn_gate_info_notify);

    gate_manager_t::set_gate(s, msg.gate_id(), msg.server_id());

    return true;
}

bool gate_msg_handler_t::handle_ag_unreg_user_scene_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::ag_unreg_user_scene_notify);

	role_manager_t::gate_unreg_user(uid, msg.unreg_code(), msg.kick_reason());

	return true;
}

bool gate_msg_handler_t::handle_ag_role_enter_game_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::ag_role_enter_game_notify);

	if (env::server->get_type() == game_server_type_cross)
	{
		role_ptr p_role = role_manager_t::find_role(uid);
		if (NULL != p_role)
		{
			p_role->syn_cross_id(env::server->get_id());
			fight_manager_t::on_role_login(p_role);
		}
	}

	return true;
}
