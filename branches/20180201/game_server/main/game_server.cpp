#include "game_server.hpp"
#include "common/log.hpp"
#include "common/global_id.hpp"
#include "gate/gate_msg_handler.hpp"
#include "gate/gate_manager.hpp"
#include "db/db_msg_handler.hpp"
#include "center/center_msg_handler.hpp"
#include "chat/chat_msg_handler.hpp"
#include "config/config_manager.hpp"
#include "scene/scene_manager.hpp"
#include "monster/monster_manager.hpp"
#include "common/ScriptManager.hpp"
#include "script/game_script_func.h"
#include "role/role_manager.hpp"
#include "city/city_manager.hpp"
#include "trade/trade_manager.hpp"
#include "task/task_manager.hpp"
#include "task/bounty_manager.hpp"
#include "fight/fight_manager.hpp"
#include "fight/buff_manager.hpp"
#include "common/time_manager.hpp"
#include "area/area_msg_handler.hpp"
#include "sys_notice/sys_notice_manager.hpp"
#include "king_war/king_war_manager.hpp"
#include "monster/monster_manager.hpp"
#include "fsmai/fsm_xml_manager.hpp"
#include "dungeon/dungeon_manager.hpp"
#include "achieve/achieve_msg_handle.hpp"
#include "virtual_notice/virtual_notice_manager.hpp"
#include "network/msg_count.hpp"
#include "arena/arena_manager.hpp"
#include "tblh/time_type_enum.hpp"
#include "fight/combat_log.hpp"
#include "hero/cultivate_manager.hpp"
#include "multi_time/multi_time_manager.hpp"
#include "gm_cmd/gm_cmd.hpp"

USING_NS_NETWORK;
USING_NS_COMMON;

game_server_t::game_server_t()
{
}

game_server_t::~game_server_t()
{
}

bool game_server_t::on_init_server()
{
	CMD_INIT;
    // xml
    if (!load_xml())
    {
        close_xml();
        COMMON_ASSERT(false);
        return false;
    }

    // logs
    if (!g_log.open(env::xml->log_path, server_name(), env::xml->is_log_trace, env::xml->is_log_debug, env::xml->is_log_info, env::xml->is_new_logfile_per_hour))
    {
        COMMON_ASSERT(false);
        return false;
    }

	if (game_server_type_normal == env::server->get_type())
	{
		// config
		if (!load_config())
		{
			COMMON_ASSERT(false);
			return false;
		}
	}
	else if (game_server_type_cross == env::server->get_type())
	{
		// config
		if (!load_cross_config())
		{
			COMMON_ASSERT(false);
			return false;
		}
	}

	//初始化super_gm指令列表
	gm_cmd_t::init_super_cmd_list();

    // init logic
    if (!init_logic())
    {
        COMMON_ASSERT(false);
        return false;
    }

    // network
    if (!init_network())
    {
        COMMON_ASSERT(false);
        return false;
    }

    return true;
}

bool game_server_t::on_close_server()
{
	if (game_server_type_normal == env::server->get_type())
	{
		clear_game_logic();
		release_config();
	}
	else if (game_server_type_cross == env::server->get_type())
	{
		clear_cross_logic();
		release_cross_config();
	}

    close_network();

    close_xml();

    g_log.close();

	CMD_CLOSE;

    return true;
}

void game_server_t::on_run_server()
{
    log_info("game[%d] server running...", env::server->get_id());
}

void game_server_t::really_save_data_to_db()
{
    role_unify_save::save();
    log_unify_save::save();
}

//--------------------------------------------------------------------------------------------
bool game_server_t::load_xml()
{
    if (!load_config_xml())
    {
        printf("\033[31mload [%s] failed!\033[0m\n", GATE_XML_PATH);
        return false;
    }
    printf("load [%s] success!\n", GATE_XML_PATH);

    if (!load_server_xml())
    {
        printf("\033[31mload [%s] failed!\033[0m\n", SERVER_XML_PATH);
        return false;
    }
    printf("load [%s] success!\n", SERVER_XML_PATH);

    if (!load_global_server_xml())
    {
        printf("\033[31mload [%s] failed!\033[0m\n", GLOBAL_SERVER_XML_PATH);
        return false;
    }
    printf("load [%s] success!\n", GLOBAL_SERVER_XML_PATH);

    return true;
}
bool game_server_t::load_config_xml()
{
    if (env::server->get_id() > 0)
    {
        return true;
    }

    try
    {
        boost::property_tree::ptree pt_config;
        boost::property_tree::xml_parser::read_xml(GAME_XML_PATH, pt_config);
        uint32_t id = pt_config.get<uint32_t>("server.game_server.id");
		if (id == 0)
		{
			printf("\033[31m game server type[%u] error\033[0m\n", id);
			return false;
		}

        uint32_t type = 0;
        auto item = pt_config.get_child_optional("server.game_server.type");
        if (item)
        {
            type = pt_config.get<uint32_t>("server.game_server.type");
        }

        env::server->set_id(id);
        env::server->set_type(type);

		if (id == common::GAME_SERVER_STATIC)
		{
			env::server->set_server_type(common::GAME_SERVER_STATIC);
		}
		else
		{
			env::server->set_server_type(common::GAME_SERVER_DYNAMIC);
		}
    }
    catch (boost::property_tree::ptree_error& e)
    {
        printf("\033[31mthrow error[%s]\033[0m\n", e.what());
        return false;
    }

    return true;
}
bool game_server_t::load_server_xml()
{
    if (env::server->get_type() != game_server_type_normal)
    {
        return true;
    }
    try
    {
        boost::property_tree::ptree pt;
        boost::property_tree::xml_parser::read_xml(SERVER_XML_PATH, pt);

        env::xml = new game_xml_t;

        env::xml->thread_num.network = pt.get<uint32_t>("server.game_server.thread_num.network");
		env::xml->srv_group.group_id = pt.get<uint32_t>("server.server_id");
		env::xml->srv_group.group_name = pt.get<std::string>("server.server_name");
        env::xml->srv_group.start_time = pt.get<std::string>("server.start_time");

		env::xml->enter_game_max_byte = pt.get<uint32_t>("server.game_server.enter_game_bag.bag_max_byte");
		env::xml->enter_game_hero_max_byte = pt.get<uint32_t>("server.game_server.enter_game_bag.hero_max_byte");
		env::xml->enter_game_item_max_byte = pt.get<uint32_t>("server.game_server.enter_game_bag.item_max_byte");
		env::xml->gm_level = pt.get<uint32_t>("server.gm_level");

        char buf[256] = { 0 };

        for (uint32_t i = 1; ; ++i)
        {
            snprintf(buf, sizeof(buf), "server.gate_server.gate%d", i);
            auto item = pt.get_child_optional(buf);
            if (item)
            {
                std::string path(buf);
                connect_t gate_connect;
                gate_connect.ip = pt.get<std::string>(path + ".listen.game.ip");
                gate_connect.port = pt.get<uint32_t>(path + ".listen.game.port");
                env::xml->connect_to_gate_list.push_back(gate_connect);
            }
            else
            {
                break;
            }
        }
        m_gate_connector_list.resize(env::xml->connect_to_gate_list.size());

        env::xml->connect_to_db.ip = pt.get<std::string>("server.db_server.listen.game.ip");
        env::xml->connect_to_db.port = pt.get<uint32_t>("server.db_server.listen.game.port");
        CHECK_PORT(env::xml->connect_to_db.port);

        env::xml->connect_to_center.ip = pt.get<std::string>("server.center_server.listen.game.ip");
        env::xml->connect_to_center.port = pt.get<uint32_t>("server.center_server.listen.game.port");
        CHECK_PORT(env::xml->connect_to_center.port);

        env::xml->connect_to_chat.ip = pt.get<std::string>("server.chat_server.listen.game.ip");
        env::xml->connect_to_chat.port = pt.get<uint32_t>("server.chat_server.listen.game.port");
        CHECK_PORT(env::xml->connect_to_chat.port);

        env::xml->connect_to_log.ip = pt.get<std::string>("server.log_server.listen.game.ip");
        env::xml->connect_to_log.port = pt.get<uint32_t>("server.log_server.listen.game.port");
        CHECK_PORT(env::xml->connect_to_log.port);

        std::string str_ip_white_list = pt.get<std::string>("server.common.ip_white_list");
        string_util_t::split(str_ip_white_list, env::xml->ip_white_list, ";");
        if (env::xml->ip_white_list.empty())
        {
            printf("\033[31mip_white_list is empty!\033[0m\n");
            return false;
        }

        env::xml->log_path = pt.get<std::string>("server.common.log_path");
        env::xml->is_log_trace = pt.get<bool>("server.common.log_trace");
        env::xml->is_log_debug = pt.get<bool>("server.common.log_debug");
		env::xml->is_log_info = pt.get<bool>("server.common.log_info");
		env::xml->is_log_msg_count = pt.get<bool>("server.common.log_msg_count");
        env::xml->is_new_logfile_per_hour = pt.get<bool>("server.common.new_logfile_per_hour");
        env::xml->reconnect_interval_time = pt.get<uint32_t>("server.common.reconnect_interval_time");
        env::xml->db_log_count_limit = pt.get<uint32_t>("server.common.db_log_count_limit");
        env::xml->config_path = pt.get<std::string>("server.common.path.config");
        env::xml->script_path = pt.get<std::string>("server.common.path.script");
        env::xml->map_path = pt.get<std::string>("server.common.path.map");
        env::xml->ai_path = pt.get<std::string>("server.common.path.ai");

		env::xml->log_debug.role = pt.get<bool>("server.common.log.role");
        env::xml->log_debug.hero = pt.get<bool>("server.common.log.hero");
        env::xml->log_debug.equip = pt.get<bool>("server.common.log.equip");
        env::xml->log_debug.trade = pt.get<bool>("server.common.log.trade");
        env::xml->log_debug.task = pt.get<bool>("server.common.log.task");
        env::xml->log_debug.scene = pt.get<bool>("server.common.log.scene");
        env::xml->log_debug.item = pt.get<bool>("server.common.log.item");
        env::xml->log_debug.boss = pt.get<bool>("server.common.log.boss");
        env::xml->log_debug.fight = pt.get<bool>("server.common.log.fight");
        env::xml->log_debug.treasure = pt.get<bool>("server.common.log.treasure");
        env::xml->log_debug.pk = pt.get<bool>("server.common.log.pk");

		env::xml->log_debug.achieve = pt.get<bool>("server.common.log.achieve");
		
		env::xml->log_debug.red_point = pt.get<bool>("server.common.log.red_point");
		env::xml->log_debug.lifeforce = pt.get<bool>("server.common.log.lifeforce");
		env::xml->log_debug.new_guide = pt.get<bool>("server.common.log.new_guide");
		env::xml->log_debug.role_buff = pt.get<bool>("server.common.log.role_buff");
		env::xml->log_debug.dungeon = pt.get<bool>("server.common.log.dungeon");
		env::xml->log_debug.kingwar = pt.get<bool>("server.common.log.kingwar");
		env::xml->log_debug.shop = pt.get<bool>("server.common.log.shop");
		env::xml->log_debug.mail = pt.get<bool>("server.common.log.mail");
		env::xml->log_debug.goods = pt.get<bool>("server.common.log.goods");
		env::xml->log_debug.family = pt.get<bool>("server.common.log.family");
		env::xml->log_debug.active_reward = pt.get<bool>("server.common.log.active_reward");
		env::xml->log_debug.page = pt.get<bool>("server.common.log.page");
		env::xml->log_debug.expedition = pt.get<bool>("server.common.log.expedition");
		env::xml->log_debug.troop = pt.get<bool>("server.common.log.troop");
		env::xml->log_debug.arena = pt.get<bool>("server.common.log.arena");
		env::xml->log_debug.money = pt.get<bool>("server.common.log.money");
		env::xml->log_debug.tower = pt.get<bool>("server.common.log.tower");
		env::xml->log_debug.multi = pt.get<bool>("server.common.log.multi");

		env::xml->max_nine_objects = pt.get<uint32_t>("server.game_server.max_nine_objects");
    }
    catch (boost::property_tree::ptree_error& e)
    {
        printf("\033[31mthrow error[%s]\033[0m\n", e.what());
        return false;
    }

    if (NULL == env::xml)
    {
        printf("\033[31menv::xml is NULL!\033[0m\n");
        return false;
    }

    return true;
}
bool game_server_t::load_global_server_xml()
{
    if (env::server->get_type() != game_server_type_cross)
    {
        return true;
    }
    try
    {
        boost::property_tree::ptree pt;
        boost::property_tree::xml_parser::read_xml(GLOBAL_SERVER_XML_PATH, pt);

        uint32_t srv_group_id = pt.get<uint32_t>("server.server_id");
        if (srv_group_id < GLOBAL_SERVER_GROUP_ID_MIN || srv_group_id > GLOBAL_SERVER_GROUP_ID_MAX)
        {
            log_error("server.server_id[%d] not in [%d, %d]", srv_group_id, GLOBAL_SERVER_GROUP_ID_MIN, GLOBAL_SERVER_GROUP_ID_MAX);
            return false;
        }

        env::xml = new game_xml_t;

        env::xml->srv_group.group_id = srv_group_id;
        env::xml->srv_group.start_time = pt.get<std::string>("server.start_time");

        env::xml->thread_num.network = pt.get<uint32_t>("server.cross_server.thread_num.network");

        char buf[256];
        snprintf(buf, sizeof(buf), "server.cross_server.cross%d.listen.gate.ip", env::server->get_id());
        env::xml->listen_at_gate.ip = pt.get<std::string>(buf);

        snprintf(buf, sizeof(buf), "server.cross_server.cross%d.listen.gate.port", env::server->get_id());
        env::xml->listen_at_gate.port = pt.get<uint32_t>(buf);
        CHECK_PORT(env::xml->listen_at_gate.port);

        env::xml->connect_to_area.ip = pt.get<std::string>("server.area_server.listen.cross.ip");
        env::xml->connect_to_area.port = pt.get<uint32_t>("server.area_server.listen.cross.port");
        CHECK_PORT(env::xml->connect_to_area.port);
		
        std::string str_ip_white_list = pt.get<std::string>("server.common.ip_white_list");
        string_util_t::split(str_ip_white_list, env::xml->ip_white_list, ";");
        if (env::xml->ip_white_list.empty())
        {
            printf("\033[31mip_white_list is empty!\033[0m\n");
            return false;
        }

		env::xml->log_path = pt.get<std::string>("server.common.log_path");
		env::xml->is_log_trace = pt.get<bool>("server.common.log_trace");
		env::xml->is_log_debug = pt.get<bool>("server.common.log_debug");
		env::xml->is_log_info = pt.get<bool>("server.common.log_info");
		env::xml->is_log_msg_count = pt.get<bool>("server.common.log_msg_count");
		env::xml->is_new_logfile_per_hour = pt.get<bool>("server.common.new_logfile_per_hour");
		env::xml->reconnect_interval_time = pt.get<uint32_t>("server.common.reconnect_interval_time");
		env::xml->config_path = pt.get<std::string>("server.common.path.config");
		env::xml->script_path = pt.get<std::string>("server.common.path.script");
		env::xml->map_path = pt.get<std::string>("server.common.path.map");
		env::xml->ai_path = pt.get<std::string>("server.common.path.ai");

		env::xml->log_debug.role = pt.get<bool>("server.common.log.role");
		env::xml->log_debug.hero = pt.get<bool>("server.common.log.hero");
		env::xml->log_debug.equip = pt.get<bool>("server.common.log.equip");
		env::xml->log_debug.trade = pt.get<bool>("server.common.log.trade");
		env::xml->log_debug.task = pt.get<bool>("server.common.log.task");
		env::xml->log_debug.scene = pt.get<bool>("server.common.log.scene");
		env::xml->log_debug.item = pt.get<bool>("server.common.log.item");
		env::xml->log_debug.boss = pt.get<bool>("server.common.log.boss");
		env::xml->log_debug.fight = pt.get<bool>("server.common.log.fight");
		env::xml->log_debug.treasure = pt.get<bool>("server.common.log.treasure");
		env::xml->log_debug.pk = pt.get<bool>("server.common.log.pk");
		env::xml->log_debug.rank = pt.get<bool>("server.common.log.rank");
		env::xml->log_debug.role_buff = pt.get<bool>("server.common.log.role_buff");
		env::xml->log_debug.achieve = pt.get<bool>("server.common.log.achieve");
		env::xml->log_debug.red_point = pt.get<bool>("server.common.log.red_point");
		env::xml->log_debug.lifeforce = pt.get<bool>("server.common.log.lifeforce");
		env::xml->log_debug.new_guide = pt.get<bool>("server.common.log.new_guide");
		env::xml->log_debug.dungeon = pt.get<bool>("server.common.log.dungeon");
		env::xml->log_debug.kingwar = pt.get<bool>("server.common.log.kingwar");
		env::xml->log_debug.shop = pt.get<bool>("server.common.log.shop");
		env::xml->log_debug.mail = pt.get<bool>("server.common.log.mail");
		env::xml->log_debug.friends = pt.get<bool>("server.common.log.friend");
		env::xml->log_debug.worldcup = pt.get<bool>("server.common.log.worldcup");
		env::xml->log_debug.arena = pt.get<bool>("server.common.log.arena");

    }
    catch (boost::property_tree::ptree_error& e)
    {
        printf("\033[31mthrow error[%s]\033[0m\n", e.what());
        return false;
    }

    return true;
}
void game_server_t::close_xml()
{
    SAFE_DELETE_P(env::xml);
    log_info("close xml success!");
}

//---------------------------------------------------------------------------------------------
bool game_server_t::init_logic()
{
	// global id
	bool is_global_server = false;
	if (game_server_type_normal == env::server->get_type())
	{
		is_global_server = false;
	}
	else if (game_server_type_cross == env::server->get_type())
	{
		is_global_server = true;
	}
	if (!g_id_allocator.init(env::xml->srv_group.group_id, env::server->get_id(), env::xml->srv_group.start_time, is_global_server))
	{
		return false;
	}

	if (is_global_server)
		return init_cross_logic();
	else
		return init_game_logic();
}


bool game_server_t::init_game_logic()
{
    // script-lua
	if (NULL == ScriptManager::getInstancePtr())
	{
		log_error("NULL == ScriptManager::getInstancePtr()");
		return false;
	}
    if (!ScriptManager::getInstancePtr()->init(std::string(c_get_root_dir())))
    {
        return false;
    }

    // 活动时间管理器初始化
    if (!time_manager_t::init(env::server->get_ios(), env::xml->srv_group.start_time))
    {
        log_error("init timer failed!");
        return false;
    }
    time_manager_t::register_func(time_type_enum::time_one_day, game_server_t::one_day);
    log_info("init timer success!");

    bounty_manager_t::load_comprehensive();
    log_info("init bounty comprehensive success!");

	if (!task_manager_t::init_conf())
	{
		log_error("init task manager failed!");
		return false;
	}
	log_info("init task manager success!");

	if (!fsm_xml_manager_t::init())
	{
		log_error("init ai xml manager failed!");
		return false;
	}
	log_info("init ai xml manager success!");

	if (!monster_manager_t::init())
	{
		log_error("init monster_manager failed!");
		return false;
	}
	log_info("init monster_manager success!");

	// 放在scene_manager_t之前
	if (!dungeon_manager_t::init())
	{
		log_error("init dungeon_manager failed!");
		return false;
	}
	log_info("init dungeon_manager success!");

    if (!scene_manager_t::init())
    {
        log_error("init scene_manager failed!");
        return false;
    }
    log_info("init scene_manager success!");

	if (!trade_manager_t::init())
	{
		log_error("init trade failed!");
		return false;
	}
	log_info("init trade success!");

	if (!fight_manager_t::init())
	{
		log_error("init fight failed!");
		return false;
	}
	log_info("init fight success!");

	if (!buff_manager_t::init())
	{
		log_error("init buff failed!");
		return false;
	}
	log_info("init buff success!");

    if (!sys_notice_manager_t::init())
    {
        log_error("init sys_notice failed!");
        return false;
    }
    log_info("init sys_notice success!");

	if (!virtual_notice_manager_t::init())
	{
		log_error("init virtual_notice_manager failed!");
		return false;
	}
	log_info("init virtual_notice_manager success!");

	if (!achieve_msg_handle_t::init_event_handler())
	{
		log_error("init achieve_bind_func failed!");
		return false;
	}
	log_info("init achieve_bind_func success!");

	if (!arena_manager_t::init())
	{
		log_error("init arena_manager failed!");
		return false;
	}
	log_info("init arena_manager success!");

	if (!cultivate_manager_t::init())
	{
		log_error("init cultivate_manager failed!");
		return false;
	}
	log_info("init cultivate_manager success!");

	if (!multi_time_manager_t::init())
	{
		log_error("init multi_time_manager_t failed!");
		return false;
	}
	log_info("init multi_time_manager_t success!");

    return true;
}
void game_server_t::clear_game_logic()
{
	//log_error("close start");
	time_manager_t::cancel_timer();
	//log_error("cancel_timer");
	role_manager_t::del_all_roles(KICK_ROLE_REASON_GAME_SERVER_CLOSED);
	//log_error("del_all_roles");
	gate_manager_t::clear_gate();
	//log_error("clear_gate");
	trade_manager_t::close();
	//log_error("trade_manager_t");
	// 先清理场景中的怪物，在把那些不在场景的怪物（正在复活的怪物）清理掉
	{
		dungeon_manager_t::close();
		//log_error("dungeon_manager_t");
		scene_manager_t::close();
		//log_error("scene_manager_t");
		monster_manager_t::close_clear();	// 这个要放场景清理后面
		//log_error("monster_manager_t");
		collect_manager_t::close();
		//log_error("collect_manager_t");
	}

	task_manager_t::clear_circle_info();
	//log_error("task_manager_t");
	sys_notice_manager_t::close();
	//log_error("sys_notice_manager_t");
	king_war_manager_t::close_clear();
	//log_error("king_war_manager_t");
	fsm_xml_manager_t::clear();
	//log_error("fsm_xml_manager_t");
	virtual_notice_manager_t::close();
	//log_error("virtual_notice_manager_t");
	fight_manager_t::close();
	//log_error("fight_manager_t");
	city_manager_t::close();
	//log_error("city_manager_t");

	if (NULL == ScriptManager::getInstancePtr())
	{
		log_error("NULL == ScriptManager::getInstancePtr()");
		return;
	}
	ScriptManager::getInstancePtr()->close();
	//log_error("ScriptManager");
	ScriptManager::delInstance();
	//log_error("delInstance");

    log_info("clear game logic success!");
}

bool game_server_t::init_cross_logic()
{
	if (NULL == ScriptManager::getInstancePtr())
	{
		log_error("NULL == ScriptManager::getInstancePtr()");
		return false;
	}
	// script-lua
	if (!ScriptManager::getInstancePtr()->init(std::string(c_get_root_dir())))
	{
		return false;
	}

	if (!fight_manager_t::init())
	{
		log_error("init fight failed!");
		return false;
	}
	log_info("init fight success!");

	if (!buff_manager_t::init())
	{
		log_error("init buff failed!");
		return false;
	}
	log_info("init buff success!");
	return true;
}

void game_server_t::clear_cross_logic()
{
	fight_manager_t::close();

	if (NULL == ScriptManager::getInstancePtr())
	{
		log_error("NULL == ScriptManager::getInstancePtr()");
		return;
	}
	ScriptManager::getInstancePtr()->close();
	ScriptManager::delInstance();

	log_info("clear cross logic success!");
}

//--------------------------------------------------------------------------------------------
bool game_server_t::init_network()
{
    if (!m_network.init(env::xml->thread_num.network, env::xml->ip_white_list))
    {
        log_error("init network failed!");
        return false;
    }

    //game连接gate
    if (env::server->get_type() == game_server_type_normal)
    {
        for (size_t idx = 0; idx < env::xml->connect_to_gate_list.size(); ++idx)
        {
            connect_t connector = env::xml->connect_to_gate_list.at(idx);
            if (!m_network.connect_to(connector.ip, connector.port, &m_gate_connector_list[idx], env::xml->reconnect_interval_time))
            {
                return false;
            }
        }
        if (!gate_msg_handler_t::init_msg_handler())
        {
            log_error("init gate msg handler failed!");
            return false;
        }
    }
    // cross监听gate
    else if (env::server->get_type() == game_server_type_cross)
    {
        // listen for gate
        if (!m_network.listen_at(env::xml->listen_at_gate.ip, env::xml->listen_at_gate.port, &m_gate_listener))
        {
            return false;
        }
        else
        {
            if (!gate_msg_handler_t::init_msg_handler())
            {
                log_error("init gate msg handler failed!");
                return false;
            }
        }
    }
    

    if (env::server->get_type() == game_server_type_normal)
    {
        // connect to db
        if (!m_network.connect_to(env::xml->connect_to_db.ip, env::xml->connect_to_db.port, &m_db_connector, env::xml->reconnect_interval_time))
        {
            return false;
        }
        else
        {
            if (!db_msg_handler_t::init_msg_handler())
            {
                log_error("init db msg handler failed!");
                return false;
            }
        }

        // connect to center
        if (!m_network.connect_to(env::xml->connect_to_center.ip, env::xml->connect_to_center.port, &m_center_connector, env::xml->reconnect_interval_time))
        {
            return false;
        }
        else
        {
            if (!center_msg_handler_t::init_msg_handler())
            {
                log_error("init center msg handler failed!");
                return false;
            }
        }

        // connect to chat 
        if (!m_network.connect_to(env::xml->connect_to_chat.ip, env::xml->connect_to_chat.port, &m_chat_connector, env::xml->reconnect_interval_time))
        {
            return false;
        }
        else
        {
            if (!chat_msg_handler_t::init_msg_handler())
            {
                log_error("init chat msg handler failed!");
                return false;
            }
        }

        // connect to log
        if (!m_network.connect_to(env::xml->connect_to_log.ip, env::xml->connect_to_log.port, &m_log_connector, env::xml->reconnect_interval_time))
        {
            return false;
        }
    }
    else if (env::server->get_type() == game_server_type_cross)
    {
        // connect to area
        if (!m_network.connect_to(env::xml->connect_to_area.ip, env::xml->connect_to_area.port, &m_area_connector, env::xml->reconnect_interval_time))
        {
            return false;
        }
        else
        {
            if (!area_msg_handler_t::init_msg_handler())
            {
                log_error("init area msg handler failed!");
                return false;
            }
        }
    }

    return true;
}
void game_server_t::close_network()
{
	msg_count_t::write_msg_recv_count_data_file(env::xml->is_log_msg_count);
	msg_count_t::write_msg_send_count_data_file(env::xml->is_log_msg_count);

    m_network.close();
}

//---------------------------------------------------------------------------------
void game_server_t::on_add_gate(const network::tcp_socket_ptr& s)
{
    gate_manager_t::add_gate(s);
}
void game_server_t::on_del_gate(const network::tcp_socket_ptr& s)
{
    gate_manager_t::del_gate(s);
}
void game_server_t::on_gate_msg(const network::tcp_socket_ptr& s, uint16_t cmd, const msg_buf_ptr& msg_buf, uint64_t uid)
{
	msg_count_t::push_msg_recv_info(msg_buf, cmd, env::xml->is_log_msg_count);
    gate_msg_handler_t::handle_server_msg(s, cmd, msg_buf, uid);
}

//---------------------------------------------------------------------------------
void game_server_t::on_add_db(const network::tcp_socket_ptr& s)
{
    m_db_connector.set_socket(s);

    // 同步game服务器信息给db服务器
    proto::server::gd_syn_game_info_notify notify;
    notify.set_game_id(env::server->get_id());
    env::server->send_msg_to_db(op_cmd::gd_syn_game_info_notify, 0, notify);
}
void game_server_t::on_del_db(const network::tcp_socket_ptr& s)
{
    env::server->post_network_close_socket(s);
    m_db_connector.close_socket();
}
void game_server_t::on_db_msg(uint64_t uid, uint16_t cmd, const msg_buf_ptr& msg_buf)
{
	msg_count_t::push_msg_recv_info(msg_buf, cmd, env::xml->is_log_msg_count);
    db_msg_handler_t::handle_server_msg(uid, cmd, msg_buf);
}

//---------------------------------------------------------------------------------
void game_server_t::on_add_center(const network::tcp_socket_ptr& s)
{
    m_center_connector.set_socket(s);

    // 同步game服务器信息给center服务器
    proto::server::ge_syn_game_info_notify notify;
    notify.set_game_id(env::server->get_id());
	// 注册本场景的静态场景
	scene_manager_t::reg_game_scene_to_center(notify.mutable_regs());

	env::server->send_msg_to_center(op_cmd::ge_syn_game_info_notify, 0, notify);
}
void game_server_t::on_del_center(const network::tcp_socket_ptr& s)
{
	role_manager_t::del_all_roles(KICK_ROLE_REASON_CENTER_UNREG);

    env::server->post_network_close_socket(s);
    m_center_connector.close_socket();
}
void game_server_t::on_center_msg(uint64_t uid, uint16_t cmd, const msg_buf_ptr& msg_buf)
{
	msg_count_t::push_msg_recv_info(msg_buf, cmd, env::xml->is_log_msg_count);
	if (cmd > op_cmd::MA_BEGIN && cmd < op_cmd::MA_END)
	{
		send_msg_from_gm_to_gate(cmd, uid, msg_buf);
	}
	else
	{
		center_msg_handler_t::handle_server_msg(uid, cmd, msg_buf);
	}
}

//---------------------------------------------------------------------------------
void game_server_t::on_add_log(const network::tcp_socket_ptr& s)
{
    m_log_connector.set_socket(s);
}
void game_server_t::on_del_log(const network::tcp_socket_ptr& s)
{
    env::server->post_network_close_socket(s);
    m_log_connector.close_socket();
}

//---------------------------------------------------------------------------------
void game_server_t::on_add_chat(const network::tcp_socket_ptr& s)
{
	m_chat_connector.set_socket(s);

	// 同步game服务器信息给chat服务器
	proto::server::gh_syn_game_info_notify notify;
	notify.set_game_id(env::server->get_id());
	env::server->send_msg_to_chat(op_cmd::gh_syn_game_info_notify, 0, notify);
}
void game_server_t::on_del_chat(const network::tcp_socket_ptr& s)
{
    env::server->post_network_close_socket(s);
    m_chat_connector.close_socket();
}
void game_server_t::on_chat_msg(uint64_t uid, uint16_t cmd, const msg_buf_ptr& msg_buf)
{
	msg_count_t::push_msg_recv_info(msg_buf, cmd, env::xml->is_log_msg_count);
    chat_msg_handler_t::handle_server_msg(uid, cmd, msg_buf);
}
void game_server_t::on_chat_msg(uint16_t cmd, const msg_buf_ptr& msg_buf)
{
	msg_count_t::push_msg_recv_info(msg_buf, cmd, env::xml->is_log_msg_count);
    chat_msg_handler_t::handle_server_msg(cmd, msg_buf);
}

//---------------------------------------------------------------------------------
void game_server_t::on_add_area(const network::tcp_socket_ptr& s)
{
	m_area_connector.set_socket(s);

	// 同步cross服务器信息给area服务器
	proto::server::sr_syn_cross_info_notify notify;
	notify.set_cross_id(env::server->get_id());
	env::server->send_msg_to_area(op_cmd::sr_syn_cross_info_notify, 0, notify);
}
void game_server_t::on_del_area(const network::tcp_socket_ptr& s)
{
	env::server->post_network_close_socket(s);
	m_area_connector.close_socket();
}
void game_server_t::on_area_msg(const network::tcp_socket_ptr& s, uint16_t cmd, const msg_buf_ptr& msg_buf, uint64_t uid)
{
	msg_count_t::push_msg_recv_info(msg_buf, cmd, env::xml->is_log_msg_count);
	area_msg_handler_t::handle_server_msg(uid, cmd, msg_buf);
}

void game_server_t::one_day(uint32_t id, bool is_in_time)
{
	if (is_in_time) {
		log_info("one_day");
		role_manager_t::one_day(common::time_util_t::now_time(), common::time_util_t::get_weekday() == 1);
	}
}

void game_server_t::post_network_send_msg(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& buf)
{
	msg_count_t::push_msg_send_info(buf, env::xml->is_log_msg_count);
	m_network.post_send_msg(s, buf);
}

