#ifndef __GAME_CENTER_MSG_HANDLER_HPP__
#define __GAME_CENTER_MSG_HANDLER_HPP__

#include "main/game_server.hpp"
#include "main/game_server_fwd.hpp"
#include "msg_handler.hpp"
#include "protos_fwd.hpp"
#include "cmd_desc.hpp"

class center_msg_handler_t
{
public:
    typedef bool (*msg_handler_func)(const network::msg_buf_ptr& msg_buf);
    typedef bool (*data_sync_func)(uint64_t uid, std::string val);

    static bool init_msg_handler();
    static bool init_data_syncers();
    static bool handle_server_msg(uint64_t uid, uint16_t cmd, const network::msg_buf_ptr msg_buf )
    {
        msg_handler_func func = NULL;
        if (cmd > op_cmd::EG_BEGIN && cmd < op_cmd::EG_END)
        {
            log_trace("recv role[%lu] cmd[%s:%d] message from center", uid, CMD_DESC(cmd), cmd);
            func = m_center_msg_handle.find_func(cmd);

        }
        else if (cmd > op_cmd::MG_BEGIN && cmd < op_cmd::MG_END)
        {
            log_trace("recv role[%lu] cmd[%s:%d] message from gm", uid, CMD_DESC(cmd), cmd);
            func = m_gm_msg_handle.find_func(cmd);
		}
		else if (cmd > op_cmd::RG_BEGIN && cmd < op_cmd::RG_END)
		{
			log_trace("recv role[%lu] cmd[%s:%d] message from area", uid, CMD_DESC(cmd), cmd);
			func = m_area_2_game_msg_handle.find_func(cmd);
		}
        else
        {
            log_error("no register func for msg cmd[%s:%d]", CMD_DESC(cmd), cmd);
            return false;
        }

        if (NULL == func)
        {
            log_error("no handler func for msg cmd[%s:%d]", CMD_DESC(cmd), cmd);
            return false;
        }

        func(msg_buf);

        //role_unify_save::save();
        //log_unify_save::save();

        return true;
    }

public:

	// center => game
    static bool handle_eg_sync_data_request(const network::msg_buf_ptr& msg_buf);
	static bool handle_eg_reg_user_scene_request(const network::msg_buf_ptr& msg_buf);
	static bool handle_eg_relogin_scene_request(const network::msg_buf_ptr& msg_buf);
	static bool handle_eg_unreg_user_scene_notify(const network::msg_buf_ptr& msg_buf);
	static bool handle_eg_chat_get_role_info_request(const network::msg_buf_ptr& msg_buf);
	static bool handle_eg_user_cross_scene_reply(const network::msg_buf_ptr& msg_buf);
	static bool handle_eg_offline_data_notify(const network::msg_buf_ptr& msg_buf);
	static bool handle_eg_tower_trigger_notify(const network::msg_buf_ptr& msg_buf);
	static bool handle_eg_two_side_friend_notify(const network::msg_buf_ptr& msg_buf);
	static bool handle_eg_redbag_draw_notify(const network::msg_buf_ptr& msg_buf);
	static bool handle_eg_offline_arena_fight_request(const network::msg_buf_ptr& msg_buf);
	static bool handle_eg_offline_arena_record_award_notify(const network::msg_buf_ptr& msg_buf);
	static bool handle_eg_offline_arena_class_award_notify(const network::msg_buf_ptr& msg_buf);
	static bool handle_eg_offline_arena_buy_reply(const network::msg_buf_ptr& msg_buf);
	static bool handle_eg_offline_arena_join_notify(const network::msg_buf_ptr& msg_buf);
	static bool handle_eg_offline_arena_score_award_notify(const network::msg_buf_ptr& msg_buf);
	static bool handle_eg_offline_arena_sweep_award_notify(const network::msg_buf_ptr& msg_buf);
	static bool handle_eg_gm_transport_ntf(const network::msg_buf_ptr& msg_buf);
	static bool handle_eg_reload_config_notify(const network::msg_buf_ptr& msg_buf);
	static bool handle_eg_kick_all_user_notify(const network::msg_buf_ptr& msg_buf);
	static bool handle_eg_gm_need_reload_xml_request(const network::msg_buf_ptr& msg_buf);
	
	// gm => game
	static bool handle_mg_reload_config_notify(const network::msg_buf_ptr& msg_buf);

	// area => game
	static bool handle_rg_arena_match_reply(const network::msg_buf_ptr& msg_buf);
	static bool handle_rg_arena_battle_result_notify(const network::msg_buf_ptr& msg_buf);
	static bool handle_rg_get_arena_rank_data_reply(const network::msg_buf_ptr& msg_buf);
	static bool handle_rg_world_cup_play_reply(const network::msg_buf_ptr& msg_buf);
	static bool handle_rg_world_cup_bet_reply(const network::msg_buf_ptr& msg_buf);
	static bool handle_rg_world_cup_bet_reward_reply(const network::msg_buf_ptr& msg_buf);
	static bool handle_rg_arena_cancel_match_reply(const network::msg_buf_ptr& msg_buf);
	static bool handle_rg_arena_self_rank_reply(const network::msg_buf_ptr& msg_buf);
    //data syncers
    static bool on_sync_country_officer(uint64_t uid, std::string val);
	static bool on_sync_family_officer(uint64_t uid, std::string val);
	static bool on_sync_charm(uint64_t uid, std::string val);
	static bool on_sync_server_time(uint64_t uid, std::string val);
public:
    static network::server_msg_handler_t<op_cmd::EG_BEGIN, op_cmd::EG_END, msg_handler_func> m_center_msg_handle;
	static network::server_msg_handler_t<op_cmd::MG_BEGIN, op_cmd::MG_END, msg_handler_func> m_gm_msg_handle;
	static network::server_msg_handler_t<op_cmd::RG_BEGIN, op_cmd::RG_END, msg_handler_func> m_area_2_game_msg_handle;

	// 数据同步
public:
	template<typename valtype>
	static void sync_data_to_center(uint64_t uid, proto::server::data_sync_type type, valtype val, bool is_sync_now = false)
	{
		if (env::server->get_type() == game_server_type_cross)
			return;

		if (m_sync_uid != uid && m_sync_datas.items_size() > 0)
		{
			env::server->send_msg_to_center(op_cmd::ge_sync_data_request, m_sync_uid, m_sync_datas);
			proto::server::ge_sync_data_request temp1;
			m_sync_datas.Swap(&temp1);
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
			env::server->send_msg_to_center(op_cmd::ge_sync_data_request, m_sync_uid, m_sync_datas);
            proto::server::ge_sync_data_request temp2;
            m_sync_datas.Swap(&temp2);
			m_sync_uid = 0;
		}
	}

public:
    static network::server_msg_handler_t<proto::server::data_sync_type_begin, proto::server::data_sync_type_end, data_sync_func> m_data_syncer;
	static proto::server::ge_sync_data_request m_sync_datas;
	static uint64_t m_sync_uid;
};

#endif
