#include "game_msg_handler.hpp"
#include "main/log_server.hpp"
#include "main/log_server_fwd.hpp"
#include "network.hpp"
#include "main/log_manager.hpp"

USING_NS_NETWORK;
USING_NS_COMMON;

server_msg_handler_t<op_cmd::GO_BEGIN, op_cmd::GO_END, game_msg_handler_t::msg_handler_func> game_msg_handler_t::m_game_msg_handle;

bool game_msg_handler_t::init_msg_handler()
{
    m_game_msg_handle.register_func(op_cmd::go_batch_save_logs_notify, handle_batch_save_logs_notify);
    return true;
}

bool game_msg_handler_t::handle_batch_save_logs_notify(const tcp_socket_ptr& s, const msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG_NO_UID(proto::log::go_batch_save_logs_notify);

    for (int32_t i = 0; i < (int32_t)msg.logs_size(); ++i)
    {
        uint16_t log_cmd = (uint16_t)msg.logs(i).cmd();
        add_logs(log_cmd, msg.logs(i).data());
    }

    return true;
}

void game_msg_handler_t::add_logs(uint16_t cmd, const std::string& msg_buf)
{
    log_data_t& log = log_manager_t::get_cur_thread_log_data();
    switch (cmd)
    {
    case op_cmd::go_role_login_log_notify:
        {
            PRE_S2LOG_MSG(proto::log::go_role_login_log_notify);
            login_log_ptr new_log(new login_log_t(msg.role_uid(), msg.gate_id(), msg.game_id(), msg.log_time()));
            log.m_role_log_mgr.add_role_login_log(new_log);
        }
        break;
    case op_cmd::go_role_logout_log_notify:
        {
            PRE_S2LOG_MSG(proto::log::go_role_logout_log_notify);
            logout_log_ptr new_log(new logout_log_t(msg.role_uid(), msg.online_time(), msg.gate_id(), msg.game_id(), msg.kick_reason(), msg.log_time()));
            log.m_role_log_mgr.add_role_logout_log(new_log);
        }
        break;
    case op_cmd::go_item_log_notify:
        {
            PRE_S2LOG_MSG(proto::log::go_item_log_notify);
            item_log_ptr new_log(new item_log_t(msg.role_uid(), msg.role_level(), msg.item_uid(), msg.item_tid(), msg.old_num(), msg.up_num(), msg.new_num(), msg.op_type(), msg.source_type(), msg.source_param(), msg.log_time()));
            log.m_item_log_mgr.add_log(new_log);
        }
		break;
	case op_cmd::go_money_log_notify:
		{
			PRE_S2LOG_MSG(proto::log::go_money_log_notify);
			money_log_ptr new_log(new money_log_t(msg.role_uid(), msg.role_level(), msg.old_num(), msg.up_num(), msg.new_num(), msg.money_type(), msg.op_type(), msg.source_type(), msg.source_param(), msg.log_time()));
			log.m_money_log_mgr.add_log(new_log);
		}
		break;
	case op_cmd::go_luck_gift_log_notify:
		{
			PRE_S2LOG_MSG(proto::log::go_luck_gift_log_notify);
			luck_gift_log_ptr new_log(new luck_gift_log_t(msg.src_role_uid(), msg.grab_role_uid(), msg.luck_gift_uid(), msg.old_num(), msg.up_num(), msg.new_num(), msg.log_time()));
			log.m_luck_gift_log_mgr.add_log(new_log);
		}
		break;
	case op_cmd::go_hero_log_notify:
		{
			PRE_S2LOG_MSG(proto::log::go_hero_log_notify);
			hero_log_ptr new_log(new hero_log_t(msg.role_uid(), msg.role_level(), msg.hero_uid(), msg.hero_tid(), msg.op_type(), msg.source_type(), msg.source_param(), msg.old_num(), msg.up_num(), msg.new_num(), msg.log_time()));
			log.m_hero_log_mgr.add_log(new_log);
		}
		break;
	case op_cmd::go_task_log_notify:
		{
			PRE_S2LOG_MSG(proto::log::go_task_log_notify);
			task_log_ptr new_log(new task_log_t(msg.role_uid(), msg.task_id(), msg.task_type(), msg.task_state(), msg.task_progress(), msg.accept_time(), msg.op_type(), msg.log_time()));
			log.m_task_log_mgr.add_log(new_log);
		}
		break;
	case op_cmd::go_prestige_log_notify:
		{
			PRE_S2LOG_MSG(proto::log::go_prestige_log_notify);
			prestige_log_ptr new_log(new prestige_log_t(msg.role_uid(), msg.role_level(), msg.old_prestige(), msg.old_prestige_total(), msg.old_prestige_level(), 
														msg.up_prestige(), msg.new_prestige(), msg.new_prestige_total(), msg.new_prestige_level(), 
														msg.op_type(), msg.source_type(), msg.source_param(), msg.log_time()));
			log.m_prestige_log_mgr.add_log(new_log);
		}
		break;
	case op_cmd::go_pk_log_notify:
		{
			PRE_S2LOG_MSG(proto::log::go_pk_log_notify);
			pk_log_ptr new_log(new pk_log_t(msg.role_uid(), msg.role_level(), msg.old_num(), msg.up_num(), msg.new_num(), msg.value_type(), msg.op_type(), msg.source_type(), msg.source_param(), msg.log_time()));
			log.m_pk_log_mgr.add_log(new_log);
		}
	break;
	case op_cmd::go_shop_log_notify:
		{
			PRE_S2LOG_MSG(proto::log::go_shop_log_notify);
			shop_log_ptr new_log(new shop_log_t(msg.role_uid(), msg.goods_tid(), msg.goods_num(),
				msg.cost_type(), msg.cost_value(),
				msg.item_id(), msg.item_num(),
				msg.source_type(), msg.source_param(), msg.log_time()));
			log.m_shop_log_mgr.add_log(new_log);
		}
		break;
	case op_cmd::go_random_shop_log_notify:
		{
			PRE_S2LOG_MSG(proto::log::go_random_shop_log_notify);
			random_shop_log_ptr new_log(new random_shop_log_t(msg.role_uid(), msg.shop_tid(), msg.goods_tid(),
				msg.cost_type(), msg.cost_value(),
				msg.cost_item(), msg.cost_item_value(),
				msg.old_num(), msg.new_num(),
				msg.source_type(), msg.log_time()));
			log.m_random_shop_log_mgr.add_log(new_log);
		}
	break;
	case op_cmd::go_equip_log_notify:
	{
		PRE_S2LOG_MSG(proto::log::go_equip_log_notify);
		
		std::ostringstream ss;
		//基础属性
		for (auto equip_attr : msg.attr())
		{
			ss << equip_attr.type() << ":" << equip_attr.value() << "$";
		}
		std::string attr = ss.str().substr(0, ss.str().length() - 1);
		ss.str("");
		ss.clear();
		//判词
		for (int i = 0; i < msg.exattr_size(); ++i)
		{
			ss << msg.exattr(i) << "$";
		}
		std::string exattr = ss.str().substr(0, ss.str().length() - 1);
		ss.str("");
		ss.clear();
		//判词重铸
		for (int i = 0; i < msg.remake_exattr_size(); ++i)
		{
			ss << msg.remake_exattr(i) << "$";
		}
		std::string remake_exattr = ss.str().substr(0, ss.str().length() - 1);
		ss.str("");
		ss.clear();
		equip_log_ptr new_log(new equip_log_t(msg.role_uid(), msg.role_level(), msg.item_uid(),
			msg.item_tid(), msg.hero_uid(), attr, exattr, remake_exattr, msg.op_type(),
			msg.source_type(), msg.source_param(), msg.log_time()));
		log.m_equip_log_mgr.add_log(new_log);
	}
	break;
	case op_cmd::go_achieve_log_notify:
	{
		PRE_S2LOG_MSG(proto::log::go_achieve_log_notify);
		achieve_log_ptr new_log(new achieve_log_t(msg.role_uid(), msg.achieve_id(), msg.old_level(),
			msg.up_level(), msg.new_level(),msg.old_count(),msg.new_count(),
			msg.old_achieve_point(),msg.add_achieve_point(),msg.new_achieve_point(),msg.old_achieve_level(),
			msg.up_achieve_level(),msg.new_achieve_level(),msg.old_reward_get_level(),msg.new_reward_get_level(),
			msg.source_type(),msg.source_param(), msg.log_time(),msg.sourec_type_str()));
		log.m_achieve_log_mgr.add_log(new_log);
	}
	break;
	case op_cmd::go_lifeforce_log_notify:
	{
		PRE_S2LOG_MSG(proto::log::go_lifeforce_log_notify);
		lifeforce_log_ptr new_log(new lifeforce_log_t(msg.role_uid(), msg.old_lifestate_level(), msg.old_lifestar_level(),
			msg.up_lifestate(), msg.up_lifestar(),
			msg.new_lifestate_level(), msg.new_lifestar_level(),
			msg.source_type(), msg.source_param(), msg.log_time()));
		log.m_lifeforce_log_mgr.add_log(new_log);
	}
	break;
	case op_cmd::go_dungeon_log_notify:
		{
			PRE_S2LOG_MSG(proto::log::go_dungeon_log_notify);
			dungeon_log_ptr new_log(new dungeon_log_t(msg.role_uid(), msg.dungeon_id(), msg.create_type(), msg.create_id(),
				msg.source_type(), msg.source_param(), msg.log_time()));
			log.m_dungeon_log_mgr.add_log(new_log);
		}
		break;
	case op_cmd::go_role_buff_log_notify:
	{
		PRE_S2LOG_MSG(proto::log::go_role_buff_log_notify);
		role_buff_log_ptr new_log(new role_buff_log_t(msg.role_uid(), msg.buff_id(), msg.op_type(), msg.speed(),
			msg.source_type(), msg.source_param(), msg.log_time()));
		log.m_role_buff_log_mgr.add_log(new_log);
	}
	break;
	case op_cmd::go_role_new_guide_log_notify:
	{
		PRE_S2LOG_MSG(proto::log::go_role_new_guide_log_notify);
		new_guide_log_ptr new_log(new new_guide_log_t(msg.role_uid(), msg.new_guide_id(),
			msg.source_type(), msg.source_param(), msg.log_time()));
		log.m_new_guide_log_mgr.add_log(new_log);
	}
	break;
	case op_cmd::go_role_trade_log_notify:
	{
		PRE_S2LOG_MSG(proto::log::go_role_trade_log_notify);
		trade_log_ptr new_log(new trade_log_t(msg.role_uid(), msg.item_tid(), msg.price(),msg.num(),
			msg.tax(),msg.total_num(),msg.total_cost(),msg.gain(),msg.trade_id(),msg.op_type(),
			msg.source_type(), msg.log_time()));
		log.m_trade_log_mgr.add_log(new_log);
	}
	break;
	case op_cmd::go_role_recharge_log_notify:
	{
		PRE_S2LOG_MSG(proto::log::go_role_recharge_log_notify);
		recharge_log_ptr new_log(new recharge_log_t(msg.role_uid(), msg.order_id(), msg.recharge_tid(), msg.recharge_rmb(), 
			msg.channel_id(), msg.log_time()));
		log.m_recharge_log_mgr.add_log(new_log);
	}
	break;
	case op_cmd::go_rmb_log_notify:
	{
		PRE_S2LOG_MSG(proto::log::go_rmb_log_notify);
		rmb_log_ptr new_log(new rmb_log_t(msg.role_uid(), msg.role_level(), msg.old_num(), msg.up_num(), msg.new_num(), msg.op_type(), msg.source_type(), msg.source_param(), msg.log_time()));
		log.m_rmb_log_mgr.add_log(new_log);
	}
	break;
	case op_cmd::go_expedition_log_notify:
	{
		PRE_S2LOG_MSG(proto::log::go_expedition_log_notify);
		expedition_log_ptr new_log(new expedition_log_t(msg.role_uid(), msg.role_level(), msg.source_type(), 
			msg.source_param(), msg.log_time()));
		log.m_expedition_log_mgr.add_log(new_log);
	}
	break;
	case op_cmd::go_exp_log_notify:
	{
		PRE_S2LOG_MSG(proto::log::go_exp_log_notify);
		role_exp_log_ptr new_log(new role_exp_log_t(msg.role_uid(), msg.old_level(), msg.old_exp(), msg.up_exp(),
			msg.new_level(), msg.new_exp(), msg.source_type(), msg.source_param(), msg.log_time()));
		log.m_role_exp_log_mgr.add_log(new_log);
	}
	break;
	case op_cmd::go_combat_log_notify:
	{
		PRE_S2LOG_MSG(proto::log::go_combat_log_notify);
		std::ostringstream ss;
		// 队员
		for (auto pair : msg.team_members())
		{
			ss << pair << "|";
		}
		std::string team_members = ss.str().substr(0, ss.str().length() - 1);
		ss.str("");
		ss.clear();
		// 目标
		for (auto pair : msg.targets())
		{
			ss << pair << "|";
		}
		std::string targets = ss.str().substr(0, ss.str().length() - 1);
		ss.str("");
		ss.clear();
		combat_log_ptr new_log(new combat_log_t(msg.role_uid(), msg.role_level(), msg.fight_type(), msg.combat_type(),
			team_members, targets, msg.fight_uid(), msg.is_win(), msg.start_time(), msg.fight_time(), msg.finish_time(), msg.log_time()));
		log.m_combat_log_mgr.add_log(new_log);
	}
	break;
	case op_cmd::go_hero_exp_log_notify:
	{
		PRE_S2LOG_MSG(proto::log::go_hero_exp_log_notify);
		hero_exp_log_ptr new_log(new hero_exp_log_t(msg.role_uid(), msg.role_level(), msg.hero_uid(), msg.hero_tid(), 
			msg.old_level(), msg.old_exp(), msg.up_exp(), msg.new_level(), msg.new_exp(), msg.source_type(), 
			msg.source_param(), msg.log_time()));
		log.m_hero_exp_log_mgr.add_log(new_log);
	}
	break;
	case op_cmd::go_patrol_hire_log_notify:
	{
		PRE_S2LOG_MSG(proto::log::go_patrol_hire_log_notify);
		patrol_hire_log_ptr new_log(new patrol_hire_log_t(msg.role_uid(), msg.begin_time(), msg.end_time(), msg.count(),
			msg.total_exp(), msg.use_multi_point(), msg.map_id(), msg.source_type(), msg.log_time()));
		log.m_patrol_hire_log_mgr.add_log(new_log);
	}
	break;
    default:
        {
            log_error("op_cmd is unknown! cmd=%d", cmd);
            return;
        }
        break;
    }
}