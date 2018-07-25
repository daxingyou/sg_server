#include "redis_data_manager.hpp"
#include "main/db_server.hpp"
#include "main/db_server_fwd.hpp"
#include "common/redis_client.hpp"
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

USING_NS_NETWORK;
USING_NS_COMMON;

redis_data_manager_t::redis_data_manager_t()
{

}

redis_data_manager_t::~redis_data_manager_t()
{

}

void redis_data_manager_t::handle_get_role_data(uint64_t uid, const network::tcp_socket_ptr& sock, proto::server::gd_role_data_request req)
{
    proto::server::dg_role_data_reply reply_msg;
	if (load_data_from_redis(uid, reply_msg))
	{
		reply_msg.set_reply_code(0);
	}
	else
	{
		reply_msg.set_reply_code(1);
	}

	env::server->send_msg_to_game(sock, op_cmd::dg_role_data_reply, uid, reply_msg);
}

void redis_data_manager_t::handle_batch_save_to_db(const proto::server::batch_save_to_db_notify& msg)
{
	save_data_to_redis(msg.datas());
}

void redis_data_manager_t::handle_kick_role(uint64_t uid, const tcp_socket_ptr& p_game_socket)
{
	game_manager_t::del_role_from_game(p_game_socket, uid);
}

void redis_data_manager_t::handle_syn_role_data(uint64_t uid)
{
	uint32_t level = redis_client_t::get_uint32("role", "level", uid);
	uint32_t plugin = redis_client_t::get_uint32("role", "plugin", uid);
	if (level == 0 || plugin == 0)
	{
		//log_warn("role[%lu] level[%d] plugin[%d] invalid", uid, level, plugin);
		return;
	}
	proto::server::dn_role_data_update_notify ntf;
	proto::common::account_role_data* p_role_data = ntf.mutable_role_data();
	p_role_data->set_level(level);
	p_role_data->set_plugin(plugin);
	env::server->send_msg_to_account(op_cmd::dn_role_data_update_notify, uid, ntf);
}

bool redis_data_manager_t::save_data_to_redis(const ::google::protobuf::RepeatedPtrField< ::proto::server::save_to_db >& data_list)
{
	bool ret = true;
	for (auto data : data_list)
	{
		std::string cache_name;
		uint32_t sec_id;
		cache_key_t::extract(data.key(), cache_name, sec_id);

		uint64_t uid = data.uid();

		if (cache_name == cache_name::roles)
		{
			proto::common::role_data ud;
			network::unwrap_msg(ud, data.data());
			ret &= role_data_manager::save_role_data_to_redis(uid, ud);
		}
		else if (cache_name == cache_name::items)
		{
			proto::common::item_single single;
			network::unwrap_msg(single, data.data());
			ret &= item_data_manager::save_item_data_to_redis(uid, single);
		}
		else if (cache_name == cache_name::heros)
		{
			proto::common::hero_single single;
			network::unwrap_msg(single, data.data());
			ret &= hero_data_manager::save_hero_data_to_redis(uid, single);
		}
		else if (cache_name == cache_name::tasks)
		{
			proto::common::task_state task;
			network::unwrap_msg(task, data.data());
			ret &= task_data_manager::save_task_data_to_redis(uid, task);
		}
		else if (cache_name == cache_name::shilian)
		{
			proto::common::task_data task;
			network::unwrap_msg(task, data.data());
			ret &= task_data_manager::save_shilian_data_to_redis(uid, task);
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
			proto::common::achieve_data_single achieve_data;
			network::unwrap_msg(achieve_data, data.data());
			ret &= achieve_data_manager::save_achieve_data_to_redis(uid, achieve_data);
		}
		else if (cache_name == cache_name::tower)
		{
			proto::common::tower_data td;
			network::unwrap_msg(td, data.data());
			ret &= tower_data_manager::save_tower_data_to_redis(uid, td);
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
		else if (cache_name == cache_name::expedition)
		{
			proto::common::expedition_data ed;
			network::unwrap_msg(ed, data.data());
			ret &= expedition_data_manager::save_expedition_data_to_redis(uid, ed);
		}
		else if (cache_name == cache_name::active_reward)
		{
			proto::common::active_reward_data_single ad;
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
	}

	return ret;
}

bool redis_data_manager_t::load_data_from_redis(uint64_t uid, proto::server::dg_role_data_reply& udr)
{
	if (!role_data_manager::is_role_exist(uid))
	{
		ROLE_LOG("role[%lu] not exist for redis, so need to create role!", uid);
		return false;
	}

	bool ret = true;
	ret &= role_data_manager::load_role_data_from_redis(uid, udr.mutable_role());
	ret &= item_data_manager::load_item_data_from_redis(uid, udr.mutable_item());
	ret &= hero_data_manager::load_hero_data_from_redis(uid, udr.mutable_hero());
	ret &= task_data_manager::load_task_data_from_redis(uid, udr.mutable_task());
	ret &= trade_item_data_manager::load_trade_item_data_from_redis(uid, udr.mutable_trade_item());
	ret &= mount_data_manager::load_mount_data_from_redis(uid, udr.mutable_mount_data());
	ret &= country_bounty_data_manager::load_country_bounty_data_from_redis(uid, udr.mutable_country_bounty());
	ret &= activity_data_manager::load_activity_data_from_redis(uid, udr.mutable_activities_data());
	ret &= arena_data_manager::load_arena_data_from_redis(uid, udr.mutable_arena_role_data());
	ret &= achieve_data_manager::load_achieve_data_from_redis(uid, udr.mutable_achieve());
	ret &= tower_data_manager::load_tower_data_from_redis(uid, udr.mutable_tower());
	ret &= luckydraw_data_manager::load_luckydraw_data_from_redis(uid, udr.mutable_luckydraw());
	ret &= dungeon_data_manager::load_dungeon_data_from_redis(uid, udr.mutable_dungeon_data());
	ret &= expedition_data_manager::load_expedition_data_from_redis(uid, udr.mutable_expedition_data());
	ret &= active_reward_data_manager::load_active_reward_data_from_redis(uid, udr.mutable_active_reward_data());
	ret &= random_shop_manager::load_random_shop_from_redis(uid, udr.mutable_random_shop_data());
	ret &= level_reward_manager::load_level_reward_to_redis(uid, udr.mutable_level_rwd_data());
	ret &= level_reward_mail_manager::load_level_reward_mail_to_redis(uid, udr.mutable_level_rwd_mail_data());
	ret &= business_act_data_manager_t::load_business_act_data_to_redis(uid, udr.mutable_bussiness_act_data());
	return ret;
} 


