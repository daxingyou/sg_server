#include "redis_data_manager.hpp"
#include "main/db_server.hpp"
#include "main/db_server_fwd.hpp"
#include "redis_client.hpp"
#include "common/cache_key.hpp"
#include "item_data_manager.hpp"
#include "task_data_manager.hpp"
#include "role_data_manager.hpp"
#include "hero_data_manager.hpp"
#include "trade_item_data_manager.hpp"
#include "mount_data_manager.hpp"
#include "country_bounty_data_manager.hpp"
#include "activity_data_manager.hpp"
#include "arena_data_manager.hpp"
#include "achieve_data_manager.hpp"
#include "tower_data_manager.hpp"
#include "luckydraw_data_manager.hpp"
#include "dungeon_data_manager.hpp"
#include "expedition_data_manager.hpp"
#include "active_reward_data_manager.hpp"
#include "random_shop_manager.hpp"
#include "level_reward_manager.hpp"
#include "level_reward_mail_manager.hpp"
#include "business_act_data_manager.hpp"
#include "liandan_data_manager.hpp"
#include "family_skill_data_manager.hpp"
#include "equip_plan_data_manager.hpp"
#include "patrol_hire_data_manager.hpp"
#include "redis_db.hpp"
#include "general_event_data_manager.hpp"
#include "recharge_data_manager.hpp"
#include "office_data_manager.hpp"

USING_NS_NETWORK;
USING_NS_COMMON;

redis_data_manager_t::redis_data_manager_t()
{

}

redis_data_manager_t::~redis_data_manager_t()
{

}

void redis_data_manager_t::handle_get_role_data(uint64_t uid, const network::tcp_socket_ptr& sock)
{
	if (role_data_manager::is_role_exist(uid)) {
		proto::server::dg_role_data_reply reply;
		if (load_data_from_redis(uid, reply)) {
			reply.set_reply_code(0);
		} else {
			reply.set_reply_code(1);
		}
		env::server->send_msg_to_game(sock, op_cmd::dg_role_data_reply, uid, reply);
	} else {
		uint32_t game_id = game_manager_t::get_game_id_by_socket(sock);
		if (!g_redis_db.load_user_from_db(uid, game_id)) {
			proto::server::dg_role_data_reply reply;
			reply.set_reply_code(1);
			env::server->send_msg_to_game(sock, op_cmd::dg_role_data_reply, uid, reply);
		}
	}
}

void redis_data_manager_t::handle_batch_save_to_db(const proto::server::gd_batch_save_to_db_notify& msg)
{
    save_data_to_redis(msg);
}

void redis_data_manager_t::handle_kick_role(uint64_t uid, const tcp_socket_ptr& p_game_socket)
{
	game_manager_t::del_role_from_game(p_game_socket, uid);
}

void redis_data_manager_t::handle_syn_role_data(uint64_t uid)
{
	uint32_t level = redis_client_t::get_uint32("role", "level", uid);
	uint32_t plugin = redis_client_t::get_uint32("role", "plugin", uid);
	std::string name = redis_client_t::get_string("role", "name", uid);
	if (level == 0 || plugin == 0)
	{
		//log_warn("role[%lu] level[%d] plugin[%d] invalid", uid, level, plugin);
		return;
	}
	proto::server::dn_role_data_update_notify ntf;
	proto::common::account_role_data* p_role_data = ntf.mutable_role_data();
	p_role_data->set_level(level);
	p_role_data->set_plugin(plugin);
	p_role_data->set_name(name);
	env::server->send_msg_to_account(op_cmd::dn_role_data_update_notify, uid, ntf);
}

bool redis_data_manager_t::save_data_to_redis(const proto::server::gd_batch_save_to_db_notify& save)
{
    uint32_t type = save.type();
    const ::google::protobuf::RepeatedPtrField< ::proto::server::save_to_db >& data_list = save.datas();

    static uint32_t recv_num = 0;
    log_info("##### save datas to redis: save_type[%d] list[%d] size[%d] recv_num[%d]", type, data_list.size(), save.ByteSize(), ++recv_num);
   
	bool ret = true;
	for (auto data : data_list)
	{
		std::string cache_name;
		uint32_t sec_id;
		cache_key_t::extract(data.key(), cache_name, sec_id);
		uint64_t uid = data.uid();

        if (2 == type && cache_name != cache_name::roles && cache_name != cache_name::items && cache_name != cache_name::heros)
        {
            log_error("uid[%lu] save_type[%d] size[%d] error: cache_name[%s] is not [cache_name::roles]", uid, type, data.ByteSize(), cache_name.c_str());
        }

        //log_warn("##### uid[%lu] save_type[%d] cache_name[%s] size[%d]", uid, type, cache_name.c_str(), data.ByteSize());

		if (cache_name == cache_name::roles)
		{
			proto::common::role_data ud;
			network::unwrap_msg(ud, data.data());
			ret &= role_data_manager::save_role_data_to_redis(uid, ud);
		}
		else if (cache_name == cache_name::items)
		{
			proto::common::item_single item_single;
			network::unwrap_msg(item_single, data.data());
			ret &= item_data_manager::save_item_data_to_redis(uid, item_single);
		}
		else if (cache_name == cache_name::heros)
		{
			proto::common::hero_single hs;
			network::unwrap_msg(hs, data.data());
			ret &= hero_data_manager::save_hero_data_to_redis(uid, hs);
		}
		else if (cache_name == cache_name::shilian)
		{
			proto::common::task_data task;
			network::unwrap_msg(task, data.data());
			ret &= task_data_manager::save_shilian_data_to_redis(uid, task);
		}
		else if (cache_name == cache_name::task_event)
		{
			proto::common::task_event te;
			network::unwrap_msg(te, data.data());
			ret &= task_data_manager::save_task_event_data_to_redis(uid, te);
		}
		else if (cache_name == cache_name::role_trade_item)
		{
			proto::common::role_trade_item trade_item;
			network::unwrap_msg(trade_item, data.data());
			ret &= trade_item_data_manager::save_trade_item_data_to_redis(uid, trade_item);
		}
		else if (cache_name == cache_name::mount_data)
		{
			proto::common::mount_data mount_data;
			network::unwrap_msg(mount_data, data.data());
			ret &= mount_data_manager::save_mount_data_to_redis(uid, mount_data);
		}
		else if (cache_name == cache_name::bounty)
		{
			proto::common::country_bounty_data country_bounty;
			network::unwrap_msg(country_bounty, data.data());
			ret &= country_bounty_data_manager::save_country_bounty_data_to_redis(uid, country_bounty);
		}
		else if (cache_name == cache_name::activity)
		{
			proto::common::activities_data activities_data;
			network::unwrap_msg(activities_data, data.data());
			ret &= activity_data_manager::save_activity_data_to_redis(uid, activities_data);
		}
		else if (cache_name == cache_name::arena)
		{
			proto::common::arena_role_data arena_data;
			network::unwrap_msg(arena_data, data.data());
			ret &= arena_data_manager::save_arena_data_to_redis(uid, arena_data);
		}
		else if (cache_name == cache_name::achieve)
		{
			proto::common::achieve_data achieve_data;
			network::unwrap_msg(achieve_data, data.data());
			ret &= achieve_data_manager::save_achieve_data_to_redis(uid, achieve_data);
		}
		else if (cache_name == cache_name::tower)
		{
			proto::common::tower_data td;
			network::unwrap_msg(td, data.data());
			ret &= tower_data_manager::save_tower_data_to_redis(uid, td);
		}
		else if (cache_name == cache_name::recharge)
		{
			proto::common::recharge_data rd;
			network::unwrap_msg(rd, data.data());
			ret &= recharge_data_manager::save_recharge_data_to_redis(uid, rd);
		}
		else if (cache_name == cache_name::office)
		{
			proto::common::office_data od;
			network::unwrap_msg(od, data.data());
			ret &= office_data_manager::save_office_data_to_redis(uid, od);
		}
		else if (cache_name == cache_name::luckydraw)
		{
			proto::common::luckydraw_data ld;
			network::unwrap_msg(ld, data.data());
			ret &= luckydraw_data_manager::save_luckydraw_data_to_redis(uid, ld);
		}
		else if (cache_name == cache_name::dungeon)
		{
			proto::common::role_dungeon_data ld;
			network::unwrap_msg(ld, data.data());
			ret &= dungeon_data_manager::save_dungeon_data_to_redis(uid, ld);
		}
		else if (cache_name == cache_name::dungeon_times)
		{
			proto::common::role_all_dungeon_times_data radtd;
			network::unwrap_msg(radtd, data.data());
			ret &= dungeon_data_manager::save_dungeon_times_data_to_redis(uid, radtd);
		}
        else if (cache_name == cache_name::expedition)
        {

            proto::common::expedition_data ed;
            network::unwrap_msg(ed, data.data());
            ret &= expedition_data_manager::save_expedition_data_to_redis(uid, ed);
        }
		else if (cache_name == cache_name::active_reward)
		{
			proto::common::active_reward_data ad;
			network::unwrap_msg(ad, data.data());
			ret &= active_reward_data_manager::save_active_reward_data_to_redis(uid, ad);
		}
		else if (cache_name == cache_name::random_shop)
		{
			proto::common::random_shop_data ld;
			network::unwrap_msg(ld, data.data());
			ret &= random_shop_manager::save_random_shop_to_redis(uid, ld);
		}
		else if (cache_name == cache_name::level_reward)
		{
			proto::common::level_reward_recived_list ld;
			network::unwrap_msg(ld, data.data());
			ret &= level_reward_manager::save_level_reward_to_redis(uid, ld);
		}
		else if (cache_name == cache_name::level_reward_mail) {
			proto::common::level_reward_mail_recived_list ld;
			network::unwrap_msg(ld, data.data());
			ret &= level_reward_mail_manager::save_level_reward_mail_to_redis(uid, ld);
		}
		else if (cache_name == cache_name::shake_money_tree)
		{
			proto::common::money_tree_data ld;
			network::unwrap_msg(ld, data.data());
			ret &= business_act_data_manager_t::save_money_tree_data_to_redis(uid, ld);
		}
		else if (cache_name == cache_name::sevenday_login)
		{
			proto::common::sevenday_login_save_data ld;
			network::unwrap_msg(ld, data.data());
			ret &= business_act_data_manager_t::save_sevenday_login_data_to_redis(uid, ld);
		}
		else if (cache_name == cache_name::recharge_total)
		{
			proto::common::recharge_total_data ld;
			network::unwrap_msg(ld, data.data());
			ret &= business_act_data_manager_t::save_recharger_total_data_to_redis(uid, ld);
		}
		else if (cache_name == cache_name::gold_luckydraw)
		{
			proto::common::gold_luckydraw_data ld;
			network::unwrap_msg(ld, data.data());
			ret &= business_act_data_manager_t::save_gold_luckydraw_data_to_redis(uid, ld);
		}
		else if (cache_name == cache_name::comp_task) 
		{
			proto::common::comp_task_date ld;
			network::unwrap_msg(ld, data.data());
			ret &= task_data_manager::save_comp_task_data_to_redis(uid, ld);
		}
		else if (cache_name == cache_name::cur_task)
		{
			proto::common::cur_task_date ld;
			network::unwrap_msg(ld, data.data());
			ret &= task_data_manager::save_cur_task_data_to_redis(uid, ld);
		}
		else if (cache_name == cache_name::month_card)
		{
			proto::common::month_card_save_data ld;
			network::unwrap_msg(ld, data.data());
			ret &= business_act_data_manager_t::save_month_card_data_to_redis(uid, ld);
		}
		else if (cache_name == cache_name::liandan)
		{
			proto::common::liandan_data ld;
			network::unwrap_msg(ld, data.data());
			ret &= liandan_data_manager::save_liandan_data_to_redis(uid, ld);
		}
		else if (cache_name == cache_name::family_skill)
		{
			proto::common::family_skill_data fsd;
			network::unwrap_msg(fsd, data.data());
			ret &= family_skill_data_manager::save_family_skill_data_to_redis(uid, fsd);
		}
		else if (cache_name == cache_name::equip_plan)
		{
			proto::common::equip_plan_data epd;
			network::unwrap_msg(epd, data.data());
			ret &= equip_plan_data_manager::save_equip_plan_data_to_redis(uid, epd);
		}
		else if (cache_name == cache_name::level_fund)
		{
			proto::common::level_fund_save_info ld;
			network::unwrap_msg(ld, data.data());
			ret &= business_act_data_manager_t::save_level_fund_data_to_redis(uid, ld);
		}
		else if (cache_name == cache_name::patrol_hire)
		{
			proto::common::patrol_hire_data ld;
			network::unwrap_msg(ld, data.data());
			ret &= patrol_hire_data_manager::save_patrol_hire_data_to_redis(uid, ld);
		}
		else if (cache_name == cache_name::limit_buy)
		{
			proto::common::limit_buy_save_data ld;
			network::unwrap_msg(ld, data.data());
			ret &= business_act_data_manager_t::save_limit_buy_data_to_redis(uid, ld);
		}
		else if (cache_name == cache_name::statistical_key)
		{
			proto::common::statistical_save_data ld;
			network::unwrap_msg(ld, data.data());
			ret &= business_act_data_manager_t::save_statistical_data_to_redis(uid, ld);
		}
		else if (cache_name == cache_name::sevenday_target)
		{
			proto::common::sd_target_save_data ld;
			network::unwrap_msg(ld, data.data());
			ret &= business_act_data_manager_t::save_sevenday_target_data_to_redis(uid, ld);
		}
		else if (cache_name == cache_name::general_event)
		{
			proto::common::general_event_save_data ld;
			network::unwrap_msg(ld, data.data());
			ret &= general_event_data_manager_t::save_general_event_data(uid, ld);
		}
	}

	return ret;
}

bool redis_data_manager_t::load_data_from_redis(uint64_t uid, proto::server::dg_role_data_reply& reply)
{
	if (!role_data_manager::is_role_exist(uid)) { // �Ѿ���redis��
		log_error("role[%lu] not find in redis", uid);
		return false;
	}

	if (!role_data_manager::load_role_data_from_redis(uid, reply.mutable_role())) return false;
	if (!item_data_manager::load_item_data_from_redis(uid, reply.mutable_item())) return false;
	if (!hero_data_manager::load_hero_data_from_redis(uid, reply.mutable_hero())) return false;
	if (!task_data_manager::load_task_data_from_redis(uid, reply.mutable_task())) return false;
	if (!trade_item_data_manager::load_trade_item_data_from_redis(uid, reply.mutable_trade_item())) return false;
	if (!mount_data_manager::load_mount_data_from_redis(uid, reply.mutable_mount_data())) return false;
	if (!country_bounty_data_manager::load_country_bounty_data_from_redis(uid, reply.mutable_country_bounty())) return false;
	if (!activity_data_manager::load_activity_data_from_redis(uid, reply.mutable_activities_data())) return false;
	if (!arena_data_manager::load_arena_data_from_redis(uid, reply.mutable_arena_role_data())) return false;
	if (!achieve_data_manager::load_achieve_data_from_redis(uid, reply.mutable_achieve())) return false;
	if (!tower_data_manager::load_tower_data_from_redis(uid, reply.mutable_tower())) return false;
	if (!recharge_data_manager::load_recharge_data_from_redis(uid, reply.mutable_recharge_data())) return false;
	if (!office_data_manager::load_office_data_from_redis(uid, reply.mutable_office_data())) return false;
	if (!luckydraw_data_manager::load_luckydraw_data_from_redis(uid, reply.mutable_luckydraw())) return false;
	if (!dungeon_data_manager::load_dungeon_data_from_redis(uid, reply.mutable_dungeon_data())) return false;
	if (!dungeon_data_manager::load_dungeon_times_data_from_redis(uid, reply.mutable_dungeon_times_data())) return false;
	if (!expedition_data_manager::load_expedition_data_from_redis(uid, reply.mutable_expedition_data())) return false;
	if (!active_reward_data_manager::load_active_reward_data_from_redis(uid, reply.mutable_active_reward_data())) return false;
	if (!random_shop_manager::load_random_shop_from_redis(uid, reply.mutable_random_shop_data())) return false;
	if (!level_reward_manager::load_level_reward_to_redis(uid, reply.mutable_level_rwd_data())) return false;
	if (!level_reward_mail_manager::load_level_reward_mail_to_redis(uid, reply.mutable_level_rwd_mail_data())) return false;
	if (!business_act_data_manager_t::load_business_act_data_to_redis(uid, reply.mutable_bussiness_act_data())) return false;
	if (!liandan_data_manager::load_liandan_data_from_redis(uid, reply.mutable_liandan_data())) return false;
	if (!family_skill_data_manager::load_family_skill_data_from_redis(uid, reply.mutable_family_skill_data())) return false;
	if (!equip_plan_data_manager::load_equip_plan_data_from_redis(uid, reply.mutable_equip_plan_data())) return false;
	if (!patrol_hire_data_manager::load_patrol_hire_data_from_redis(uid, reply.mutable_patrol_data())) return false;
	if (!general_event_data_manager_t::load_general_event_data(uid, reply.mutable_general_event_data()  )) return false;

	return true;
} 

void redis_data_manager_t::load_data_from_db_done(uint64_t uid, bool result, uint32_t game_id)
{
	const network::tcp_socket_ptr p_sock = game_manager_t::get_game_socket_by_id(game_id);
	if (!p_sock) {
		log_error("game[%u] is not find", game_id);
		return;
	}

	proto::server::dg_role_data_reply reply;
	if (result && redis_data_manager_t::load_data_from_redis(uid, reply)) {
		reply.set_reply_code(0);
	} else {
		reply.set_reply_code(1);
	}
	env::server->send_msg_to_game(p_sock, op_cmd::dg_role_data_reply, uid, reply);
}

