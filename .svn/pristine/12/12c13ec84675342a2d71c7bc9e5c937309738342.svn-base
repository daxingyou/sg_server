#ifndef __GAME_GM_CMD_OPT_H__
#define __GAME_GM_CMD_OPT_H__

#include "gm_cmd.hpp"
#include "protos_fwd.hpp"

class gm_cmd_opt_t
{
public:
    static void send_opt_reply_msg(role_ptr p_role, const proto::client::gc_gm_cmd_reply& reply_msg);

public:
	static void opt_add_item(role_ptr p_role, std::vector<std::string>& gm_cmd_list, proto::common::role_change_data* p_data);
	static void opt_set_level(role_ptr p_role, std::vector<std::string>& gm_cmd_list, proto::common::role_change_data* p_data);
	static void opt_add_exp(role_ptr p_role, std::vector<std::string>& gm_cmd_list, proto::common::role_change_data* p_data);
	static void opt_add_prestige(role_ptr p_role, std::vector<std::string>& gm_cmd_list);
	static void opt_add_money(role_ptr p_role, std::vector<std::string>& gm_cmd_list);
    static void opt_king_me(role_ptr p_role, std::vector<std::string>& gm_cmd_list);
    static void opt_join_country(role_ptr p_role, std::vector<std::string>& gm_cmd_list);
    static void opt_join_family(role_ptr p_role, std::vector<std::string>& gm_cmd_list);
    static void opt_approve_join_family(role_ptr p_role, std::vector<std::string>& gm_cmd_list);

	static void opt_add_hero(role_ptr p_role, std::vector<std::string>& gm_cmd_list, proto::common::role_change_data* p_data);
	static void opt_add_pk(role_ptr p_role, std::vector<std::string>& gm_cmd_list, proto::common::role_change_data* p_data);
	static void opt_chat_cmd(role_ptr p_role, std::vector<std::string>& gm_cmd_list);
    static void opt_add_family_money(role_ptr p_role, std::vector<std::string>& gm_cmd_list);
	static void opt_reload_script();
	static void opt_reload_config();
    static void opt_finish_task(role_ptr p_role, std::vector<std::string>& gm_cmd_list);
	static void opt_active_task(role_ptr p_role, std::vector<std::string>& gm_cmd_list);
	static void opt_finish_all_task(role_ptr p_role);
	static void opt_fight_pve(role_ptr p_role, std::vector<std::string>& gm_cmd_list);
	static void opt_fight_pvp(role_ptr p_role, std::vector<std::string>& gm_cmd_list);
	static void opt_fight_loading_end(role_ptr p_role, std::vector<std::string>& gm_cmd_list);
	static void opt_jump_scene(role_ptr p_role, std::vector<std::string>& gm_cmd_list);
	static void opt_reset_today(role_ptr p_role);
	static void opt_add_mount(role_ptr p_role, std::vector<std::string>& gm_cmd_list, proto::common::role_change_data* p_data);
	static void opt_summon_npc(role_ptr p_role, std::vector<std::string>& gm_cmd_list);
	static void opt_drop(role_ptr p_role, std::vector<std::string>& gm_cmd_list, proto::common::role_change_data* p_data);
	static void opt_jump(role_ptr p_role, std::vector<std::string>& gm_cmd_list);
	static void opt_task_pve(role_ptr p_role, std::vector<std::string>& gm_cmd_list, proto::common::role_change_data* p_data);
	static void opt_gm_mail(role_ptr p_role, std::vector<std::string>& gm_cmd_list);
	static void opt_gm_random_mail(role_ptr p_role, std::vector<std::string>& gm_cmd_list);
	static void opt_gm_mail_item(role_ptr p_role, std::vector<std::string>& gm_cmd_list);
	static void opt_cross_fight(role_ptr p_role);
	static void opt_arena_join(role_ptr p_role, proto::common::role_change_data* p_data);
	static void opt_arena_buy_ticket(role_ptr p_role, proto::common::role_change_data* p_data);
	static void arena_match(role_ptr p_role);
	static void opt_sys_notice(role_ptr p_role, std::vector<std::string>& gm_cmd_list);
	static void opt_sys_pkmode(role_ptr p_role, std::vector<std::string>& gm_cmd_list);
    static void opt_change_now_time(std::vector<std::string>& gm_cmd_list);
	static void opt_reset_now_time();
	static void opt_king_war_add_gas(role_ptr p_role, std::vector<std::string>& gm_cmd_list);
	static void opt_king_war_add_exploit(role_ptr p_role, std::vector<std::string>& gm_cmd_list);
	static void opt_add_liveness(role_ptr p_role, std::vector<std::string>& gm_cmd_list);
	static void opt_open_dungeon(role_ptr p_role, std::vector<std::string>& gm_cmd_list);
	static void opt_restart_dungeon(role_ptr p_role);
	static void opt_dungeon_event(role_ptr p_role, std::vector<std::string>& gm_cmd_list);
	static void opt_open_function(role_ptr p_role, std::vector<std::string>& gm_cmd_list);
	static void opt_open_all_function(role_ptr p_role);
	static void opt_npc_code(role_ptr p_role, std::vector<std::string>& gm_cmd_list);
	static void opt_collect_code(role_ptr p_role, std::vector<std::string>& gm_cmd_list);
	static void opt_change_arena_score(role_ptr p_role, std::vector<std::string>& gm_cmd_list);
	static void opt_add_plugin(role_ptr p_role, std::vector<std::string>& gm_cmd_list);
	static void opt_add_trade_point(role_ptr p_role, std::vector<std::string>& gm_cmd_list);
	static void opt_add_honor(role_ptr p_role, std::vector<std::string>& gm_cmd_list);
	static void opt_recharge(role_ptr p_role, std::vector<std::string>& gm_cmd_list, proto::common::role_change_data* p_data);
	static std::string opt_query_recharge(role_ptr p_role);
	static void opt_prison(role_ptr p_role);
	static void opt_clear_notice();
	static void opt_level_reward(role_ptr p_role, std::vector<std::string>& gm_cmd_list, proto::common::role_change_data* p_data);
	static void opt_offline_arena_rank(role_ptr p_role, std::vector<std::string>& gm_cmd_list, proto::common::role_change_data* p_data);
	static void opt_summon_field_boss(std::vector<std::string>& gm_cmd_list);
	static void opt_merage_item( role_ptr p_role, std::vector<std::string>& gm_cmd_list, proto::common::role_change_data* p_data);
	static void opt_country_king_family(role_ptr p_role, std::vector<std::string>& gm_cmd_list);
	static void opt_add_vigour(role_ptr p_role, std::vector<std::string>& gm_cmd_list);
	static void opt_hy_cmd(role_ptr p_role, std::vector<std::string>& gm_cmd_list);
	static void opt_reduce_vigour(role_ptr p_role, std::vector<std::string>& gm_cmd_list);
	static void opt_fight_win(role_ptr p_role);
	static void opt_clearcd(role_ptr p_role);
	///测试代码  测完删除 功能 给玩家加成就
	static void opt_test_achieve(role_ptr p_role); 
	static void opt_add_head(role_ptr p_role, std::vector<std::string>& gm_cmd_list);
	static void opt_change_name(role_ptr p_role, std::vector<std::string>& gm_cmd_list);
	static void opt_open_all_dungeon(role_ptr p_role);

	static void opt_finish_all_elite(role_ptr p_role);
	static void opt_set_cur_tower(role_ptr p_role, std::vector<std::string>& gm_cmd_list);
	static void opt_set_record_tower(role_ptr p_role, std::vector<std::string>& gm_cmd_list);
};

#endif // __GM_CMD_H__
