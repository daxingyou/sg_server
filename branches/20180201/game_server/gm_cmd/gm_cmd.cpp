#include "gm_cmd.hpp"
#include "gm_cmd_opt.hpp"
#include "role/role_manager.hpp"
#include <boost/format.hpp>
#include <boost/tokenizer.hpp>
#include <boost/algorithm/string.hpp>

USING_NS_NETWORK;
USING_NS_COMMON;

std::set<std::string> gm_cmd_t::super_cmd_list;

void gm_cmd_t::init_super_cmd_list()
{
	gm_cmd_t::super_cmd_list.insert("/rsconfig");
	gm_cmd_t::super_cmd_list.insert("/rslua");
	gm_cmd_t::super_cmd_list.insert("/chgtime");
	gm_cmd_t::super_cmd_list.insert("/resettime");
}

bool gm_cmd_t::handle_gm_cmd(const msg_buf_ptr& msg_buf)
{
    PRE_S2S_MSG(proto::client::cg_gm_cmd_request);
    
	role_ptr p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role");
		return false;
	}
	if (!p_role->is_gm())
	{
		log_error("role[%lu] is not gm!", uid);
		return false;
	}
    std::string gm_cmd = msg.cmd();
    if (gm_cmd.empty())
    {
        log_error("gm_cmd is empty");
        return false;
    }

	ROLE_LOG("%s", gm_cmd.c_str());
    
    std::vector<std::string> gm_cmd_list;
    parse_gm_cmd(gm_cmd, gm_cmd_list);
    allot_gm_cmd(p_role, gm_cmd_list);
	p_role->unify_save_data();

	return true;
}

void gm_cmd_t::parse_gm_cmd(std::string gm_cmd, std::vector<std::string>& gm_cmd_list)
{
    gm_cmd_list.clear();

    boost::char_separator<char> sep(" ");
    typedef boost::tokenizer<boost::char_separator<char>> CustonTokenizer;
    CustonTokenizer tok(gm_cmd,sep);

    for(CustonTokenizer::iterator beg = tok.begin(); beg != tok.end(); ++beg)
    {
        gm_cmd_list.push_back(*beg);
    }
}

void gm_cmd_t::allot_gm_cmd(role_ptr p_role, std::vector<std::string>& gm_cmd_list)
{
    if (NULL == p_role)
    {
        log_error("NULL == p_role");
        return;
    }

    if (gm_cmd_list.size() <= 0)
    {
        log_error("cmd_list.size() <= 0");
        return;
    }
	proto::client::gc_gm_cmd_reply reply;
	proto::common::role_change_data *p_data = reply.mutable_rcd();

    std::string gm_cmd = gm_cmd_list[0].c_str();
    std::string show_info = gm_cmd.c_str();

	if (gm_cmd_t::super_cmd_list.find(gm_cmd.c_str()) != gm_cmd_t::super_cmd_list.end())
	{
		if (!p_role->is_super_gm()) 
		{
			log_error("role[%lu] is not super gm!", p_role->get_uid());
			return;
		}
	}

    if (0 == strcmp(gm_cmd.c_str(), "/additem"))
    {
        gm_cmd_opt_t::opt_add_item(p_role, gm_cmd_list, p_data);
    }
	else if (0 == strcmp(gm_cmd.c_str(), "/setlevel"))
	{
		gm_cmd_opt_t::opt_set_level(p_role, gm_cmd_list, p_data);
	}
	else if (0 == strcmp(gm_cmd.c_str(), "/addexp"))
	{
		gm_cmd_opt_t::opt_add_exp(p_role, gm_cmd_list, p_data);
	}
	else if (0 == strcmp(gm_cmd.c_str(), "/addprestige"))
	{
		gm_cmd_opt_t::opt_add_prestige(p_role, gm_cmd_list);
	}
	else if (0 == strcmp(gm_cmd.c_str(), "/addmoney"))
	{
		gm_cmd_opt_t::opt_add_money(p_role, gm_cmd_list);
	}
    else if (0 == strcmp(gm_cmd.c_str(), "/king"))
	{
		gm_cmd_opt_t::opt_king_me(p_role, gm_cmd_list);
	}
    else if (0 == strcmp(gm_cmd.c_str(), "/joincountry"))
	{
		gm_cmd_opt_t::opt_join_country(p_role, gm_cmd_list);
	}
    else if (0 == strcmp(gm_cmd.c_str(), "/joinfamily"))
	{
		gm_cmd_opt_t::opt_join_family(p_role, gm_cmd_list);
	}
	else if (0 == strcmp(gm_cmd.c_str(), "/addhero"))
	{
		gm_cmd_opt_t::opt_add_hero(p_role, gm_cmd_list, p_data);
	}
	else if (0 == strcmp(gm_cmd.c_str(), "/addpk"))
	{
		gm_cmd_opt_t::opt_add_pk(p_role, gm_cmd_list, p_data);
	}
    else if( 0 == strcmp(gm_cmd.c_str(), "/approvejoinfamily"))
    {
        gm_cmd_opt_t::opt_approve_join_family(p_role, gm_cmd_list);
    }
	else if (0 == strcmp(gm_cmd.c_str(), "/createchannel"))
	{
		gm_cmd_opt_t::opt_chat_cmd(p_role, gm_cmd_list);
	}
	else if (0 == strcmp(gm_cmd.c_str(), "/addfamilymoney"))
	{
		gm_cmd_opt_t::opt_add_family_money(p_role, gm_cmd_list);
	}
	else if (0 == strcmp(gm_cmd.c_str(), "/rslua"))
	{
		gm_cmd_opt_t::opt_reload_script();
	}
	else if (0 == strcmp(gm_cmd.c_str(), "/rsconfig"))
	{
		gm_cmd_opt_t::opt_reload_config();
	}
	else if (0 == strcmp(gm_cmd.c_str(), "/finishtask"))
	{
		gm_cmd_opt_t::opt_finish_task(p_role, gm_cmd_list);
	}
	else if (0 == strcmp(gm_cmd.c_str(), "/activetask"))
	{
		gm_cmd_opt_t::opt_active_task(p_role, gm_cmd_list);
	}
	else if (0 == strcmp(gm_cmd.c_str(), "/pve"))
	{
		gm_cmd_opt_t::opt_fight_pve(p_role, gm_cmd_list);
	}
	else if (0 == strcmp(gm_cmd.c_str(), "/pvp"))
	{
		gm_cmd_opt_t::opt_fight_pvp(p_role, gm_cmd_list);
	}
	else if (0 == strcmp(gm_cmd.c_str(), "/fightle"))
	{
		gm_cmd_opt_t::opt_fight_loading_end(p_role, gm_cmd_list);
	}
	else if (0 == strcmp(gm_cmd.c_str(), "/jumpscene"))
	{
		gm_cmd_opt_t::opt_jump_scene(p_role, gm_cmd_list);
	}
	else if (0 == strcmp(gm_cmd.c_str(), "/rstoday"))
	{
		gm_cmd_opt_t::opt_reset_today(p_role);
	}
	else if (0 == strcmp(gm_cmd.c_str(), "/addmount"))
	{
		gm_cmd_opt_t::opt_add_mount(p_role, gm_cmd_list, p_data);
	}
	else if (0 == strcmp(gm_cmd.c_str(), "/summon"))
	{
		gm_cmd_opt_t::opt_summon_npc(p_role, gm_cmd_list);
	}
	else if (0 == strcmp(gm_cmd.c_str(), "/drop"))
	{
		gm_cmd_opt_t::opt_drop(p_role, gm_cmd_list, p_data);
	}
	else if (0 == strcmp(gm_cmd.c_str(), "/jump"))
	{
		gm_cmd_opt_t::opt_jump(p_role, gm_cmd_list);
	}
	else if (0 == strcmp(gm_cmd.c_str(), "/taskpve"))
	{
		gm_cmd_opt_t::opt_task_pve(p_role, gm_cmd_list, p_data);
	}
	else if (0 == strcmp(gm_cmd.c_str(), "/gmmail"))
	{
		gm_cmd_opt_t::opt_gm_mail(p_role, gm_cmd_list);
	}
	else if (0 == strcmp(gm_cmd.c_str(), "/randommail"))
	{
		gm_cmd_opt_t::opt_gm_random_mail(p_role, gm_cmd_list);
	}
	else if (0 == strcmp(gm_cmd.c_str(), "/mailitem"))
	{
		gm_cmd_opt_t::opt_gm_mail_item(p_role, gm_cmd_list);
	}
	else if (0 == strcmp(gm_cmd.c_str(), "/crossfight"))
	{
		gm_cmd_opt_t::opt_cross_fight(p_role);
	}
	else if (0 == strcmp(gm_cmd.c_str(), "/arena_join"))
	{
		gm_cmd_opt_t::opt_arena_join(p_role, p_data);
	}
	else if (0 == strcmp(gm_cmd.c_str(), "/arena_buy_ticket"))
	{
		gm_cmd_opt_t::opt_arena_buy_ticket(p_role, p_data);
	}
	else if (0 == strcmp(gm_cmd.c_str(), "/arena_match"))
	{
		gm_cmd_opt_t::arena_match(p_role);
	}
	else if (0 == strcmp(gm_cmd.c_str(), "/sys_notice"))
	{
		gm_cmd_opt_t::opt_sys_notice(p_role, gm_cmd_list);
	}
	else if (0 == strcmp(gm_cmd.c_str(), "/pkmode"))
	{
		gm_cmd_opt_t::opt_sys_pkmode(p_role, gm_cmd_list);
    }
    else if (0 == strcmp(gm_cmd.c_str(), "/chgtime"))
    {
        gm_cmd_opt_t::opt_change_now_time(gm_cmd_list);
    }
	else if (0 == strcmp(gm_cmd.c_str(), "/resettime"))
	{
		gm_cmd_opt_t::opt_reset_now_time();
	}
    else if (0 == strcmp(gm_cmd.c_str(), "/nowtime"))
    {
        show_info = common::time_util_t::FormatTime_to_string(common::time_util_t::now_time(), true);
    }
	else if (0 == strcmp(gm_cmd.c_str(), "/addgas"))
	{
		gm_cmd_opt_t::opt_king_war_add_gas(p_role, gm_cmd_list);
	}
	else if (0 == strcmp(gm_cmd.c_str(), "/addexploit"))
	{
		gm_cmd_opt_t::opt_king_war_add_exploit(p_role, gm_cmd_list);
	}
	else if (0 == strcmp(gm_cmd.c_str(), "/addhuoyuedu"))
	{
		gm_cmd_opt_t::opt_add_liveness(p_role, gm_cmd_list);
	}
	else if (0 == strcmp(gm_cmd.c_str(), "/dungeon"))
	{
		gm_cmd_opt_t::opt_open_dungeon(p_role, gm_cmd_list);
	}
	else if (0 == strcmp(gm_cmd.c_str(), "/dungeon_event"))
	{
		gm_cmd_opt_t::opt_dungeon_event(p_role, gm_cmd_list);
	}
	else if (0 == strcmp(gm_cmd.c_str(), "/dungeon_reset"))
	{
		gm_cmd_opt_t::opt_restart_dungeon(p_role);
	}
	else if (0 == strcmp(gm_cmd.c_str(), "/funcopen"))
	{
		gm_cmd_opt_t::opt_open_function(p_role, gm_cmd_list);
	}
	else if (0 == strcmp(gm_cmd.c_str(), "/funcopenall"))
	{
		gm_cmd_opt_t::opt_open_all_function(p_role);
	}
	else if (0 == strcmp(gm_cmd.c_str(), "/npccode"))
	{
		gm_cmd_opt_t::opt_npc_code(p_role, gm_cmd_list);
	}
	else if (0 == strcmp(gm_cmd.c_str(), "/collectcode"))
	{
		gm_cmd_opt_t::opt_collect_code(p_role, gm_cmd_list);
	}
	else if (0 == strcmp(gm_cmd.c_str(), "/change_arena_score"))
	{
		gm_cmd_opt_t::opt_change_arena_score(p_role, gm_cmd_list);
	}
	else if(0 == strcmp(gm_cmd.c_str(), "/addplugin"))
	{
		gm_cmd_opt_t::opt_add_plugin(p_role, gm_cmd_list);
	}
	else if (0 == strcmp(gm_cmd.c_str(), "/addtradepoint"))
	{
		gm_cmd_opt_t::opt_add_trade_point(p_role, gm_cmd_list);
	}
	else if (0 == strcmp(gm_cmd.c_str(), "/recharge"))
	{
		gm_cmd_opt_t::opt_recharge(p_role, gm_cmd_list, p_data);
	}
	else if (0 == strcmp(gm_cmd.c_str(), "/query_recharge"))
	{
		show_info = gm_cmd_opt_t::opt_query_recharge(p_role);
	}
	else if (0 == strcmp(gm_cmd.c_str(), "/addhonor"))
	{
		gm_cmd_opt_t::opt_add_honor(p_role, gm_cmd_list);
	}
	else if (0 == strcmp(gm_cmd.c_str(), "/prison"))
	{
		gm_cmd_opt_t::opt_prison(p_role);
	}
	else if (0 == strcmp(gm_cmd.c_str(), "/clearnotice"))
	{
		gm_cmd_opt_t::opt_clear_notice();
	}
	else if (0 == strcmp(gm_cmd.c_str(), "/levelreward"))
	{
		gm_cmd_opt_t::opt_level_reward(p_role, gm_cmd_list, p_data);
	}
	else if (0 == strcmp(gm_cmd.c_str(), "/arenarank"))
	{
		gm_cmd_opt_t::opt_offline_arena_rank(p_role, gm_cmd_list, p_data);
	}
	else if (0 == strcmp(gm_cmd.c_str(), "/fieldboss"))
	{
		gm_cmd_opt_t::opt_summon_field_boss(gm_cmd_list);
	}
	else if (0 == strcmp(gm_cmd.c_str(), "/merage_item"))
	{
		gm_cmd_opt_t::opt_merage_item(p_role,gm_cmd_list, p_data);
	}
	else if (0 == strcmp(gm_cmd.c_str(), "/kingfamily"))
	{
		gm_cmd_opt_t::opt_country_king_family(p_role, gm_cmd_list);
	}
	else if (0 == strcmp(gm_cmd.c_str(), "/hycmd"))
	{
		gm_cmd_opt_t::opt_hy_cmd(p_role, gm_cmd_list);
	}
	else if (0 == strcmp(gm_cmd.c_str(), "/addvigour"))
	{
		gm_cmd_opt_t::opt_add_vigour(p_role, gm_cmd_list);
	}
	else if (0 == strcmp(gm_cmd.c_str(), "/reducevigour"))
	{
		gm_cmd_opt_t::opt_reduce_vigour(p_role, gm_cmd_list);
	}

    reply.set_reply_code(0);
	proto::common::personal_info* p_info = p_data->mutable_per_info();
	p_role->get_personal_info(p_info);
	reply.set_gm_cmd(show_info);
	p_role->send_msg_to_client(op_cmd::gc_gm_cmd_reply, reply);
}
