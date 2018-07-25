#include "global_user_data_mgr.hpp"
#include "redis_client.hpp"
#include "common/global_id.hpp"
#include "user/global_user_manager.hpp"
#include "rank/rank_manager.hpp"

void global_user_data_mgr_t::set_rank(uint64_t uid, uint32_t rank_id, uint32_t rank)
{

}

uint32_t global_user_data_mgr_t::get_rank(uint64_t uid, uint32_t rank_id)
{
	return 0;
}

std::string global_user_data_mgr_t::get_name(uint64_t uid)
{
	if (!is_user_key_exsit(uid))
	{
		log_error("global_user[%lu] not find", uid);
		return std::string("");
	}

	return redis_client_t::get_string("global_user", "name", uid);
}

void global_user_data_mgr_t::set_name(uint64_t uid, const std::string& name)
{
	if (!is_user_key_exsit(uid))
	{
		log_error("global_user[%lu] not find", uid);
		return;
	}

	redis_client_t::set_string("global_user", "name", uid, name);
	redis_client_t::set_uint32("global_user", "rt", uid, common::time_util_t::now_time()); // 设置数据时间，用于比较好友数据是否要更新
}

uint32_t global_user_data_mgr_t::get_did(uint64_t uid)
{
	if (!is_user_key_exsit(uid))
	{
		log_error("global_user[%lu] not find", uid);
		return 0;
	}

	return redis_client_t::get_uint32("global_user", "did", uid);
}

uint32_t global_user_data_mgr_t::get_user_country_id(uint64_t uid)
{
	if (!is_user_key_exsit(uid))
	{
		log_error("global_user[%lu] not find", uid);
		return 0;
	}

	return redis_client_t::get_uint32("global_user", "country_id", uid);
}

void global_user_data_mgr_t::set_user_country_id(uint64_t uid, uint32_t val)
{
	if (!is_user_key_exsit(uid))
	{
		log_error("global_user[%lu] not find", uid);
		return;
	}

	redis_client_t::set_uint32("global_user", "country_id", uid, val);
	redis_client_t::set_uint32("global_user", "rt", uid, common::time_util_t::now_time()); // 设置数据时间，用于比较好友数据是否要更新
}

uint32_t global_user_data_mgr_t::get_user_country_officer(uint64_t uid)
{
	if (!is_user_key_exsit(uid))
	{
		log_error("global_user[%lu] not find", uid);
		return 0;
	}

	return redis_client_t::get_uint32("global_user", "country_officer_type", uid);
}

void global_user_data_mgr_t::set_user_country_officer(uint64_t uid, uint32_t val)
{
	if (!is_user_key_exsit(uid))
	{
		log_error("global_user[%lu] not find", uid);
		return;
	}

	redis_client_t::set_uint32("global_user", "country_officer_type", uid, val);
	redis_client_t::set_uint32("global_user", "rt", uid, common::time_util_t::now_time()); // 设置数据时间，用于比较好友数据是否要更新
}

uint64_t global_user_data_mgr_t::get_user_family_id(uint64_t uid)
{
	if (!is_user_key_exsit(uid))
	{
		log_error("global_user[%lu] not find", uid);
		return 0;
	}

	return redis_client_t::get_uint64("global_user", "family_id", uid);
}

void global_user_data_mgr_t::set_user_family_id(uint64_t uid, uint64_t val)
{
	if (!is_user_key_exsit(uid))
	{
		log_error("global_user[%lu] not find", uid);
		return;
	}

	redis_client_t::set_uint64("global_user", "family_id", uid, val);
	redis_client_t::set_uint32("global_user", "rt", uid, common::time_util_t::now_time()); // 设置数据时间，用于比较好友数据是否要更新
}

uint32_t global_user_data_mgr_t::get_user_family_officer(uint64_t uid)
{
	if (!is_user_key_exsit(uid))
	{
		log_error("global_user[%lu] not find", uid);
		return 0;
	}

	return redis_client_t::get_uint32("global_user", "family_officer_type", uid);
}

void global_user_data_mgr_t::set_user_family_officer(uint64_t uid, uint32_t val)
{
	if (!is_user_key_exsit(uid))
	{
		log_error("global_user[%lu] not find", uid);
		return;
	}

	redis_client_t::set_uint32("global_user", "family_officer_type", uid, val);
	redis_client_t::set_uint32("global_user", "rt", uid, common::time_util_t::now_time()); // 设置数据时间，用于比较好友数据是否要更新
}

uint32_t global_user_data_mgr_t::get_family_contribution(uint64_t uid)
{
	if (!is_user_key_exsit(uid))
	{
		log_error("global_user[%lu] not find", uid);
		return 0;
	}

	return redis_client_t::get_uint32("global_user", "family_contribution", uid);
}

void global_user_data_mgr_t::set_family_contribution(uint64_t uid, uint32_t val)
{
	if (!is_user_key_exsit(uid))
	{
		log_error("global_user[%lu] not find", uid);
		return;
	}

	redis_client_t::set_uint32("global_user", "family_contribution", uid, val);
	redis_client_t::set_uint32("global_user", "rt", uid, common::time_util_t::now_time()); // 设置数据时间，用于比较好友数据是否要更新
}

uint32_t global_user_data_mgr_t::get_last_login_time(uint64_t uid)
{
	if (!is_user_key_exsit(uid))
	{
		log_error("global_user[%lu] not find", uid);
		return 0;
	}

	return redis_client_t::get_uint32("global_user", "last_login_time", uid);
}

void global_user_data_mgr_t::set_last_login_time(uint64_t uid, uint32_t val)
{
	if (!is_user_key_exsit(uid))
	{
		log_error("global_user[%lu] not find", uid);
		return;
	}

	redis_client_t::set_uint32("global_user", "last_login_time", uid, val);
	redis_client_t::set_uint32("global_user", "rt", uid, common::time_util_t::now_time()); // 设置数据时间，用于比较好友数据是否要更新
}

uint32_t global_user_data_mgr_t::get_last_logout_time(uint64_t uid)
{
	if (!is_user_key_exsit(uid))
	{
		log_error("global_user[%lu] not find", uid);
		return 0;
	}

	return redis_client_t::get_uint32("global_user", "last_logout_time", uid);
}

void global_user_data_mgr_t::set_last_logout_time(uint64_t uid, uint32_t val)
{
	if (!is_user_key_exsit(uid))
	{
		log_error("global_user[%lu] not find", uid);
		return;
	}

	redis_client_t::set_uint32("global_user", "last_logout_time", uid, val);
	redis_client_t::set_uint32("global_user", "rt", uid, common::time_util_t::now_time()); // 设置数据时间，用于比较好友数据是否要更新
}

uint32_t global_user_data_mgr_t::get_update_time(uint64_t uid)
{
	if (!is_user_key_exsit(uid))
	{
		log_error("global_user[%lu] not find", uid);
		return 0;
	}

	return redis_client_t::get_uint32("global_user", "update_time", uid);
}

void global_user_data_mgr_t::set_update_time(uint64_t uid, uint32_t val)
{
	if (!is_user_key_exsit(uid))
	{
		log_error("global_user[%lu] not find", uid);
		return;
	}

	redis_client_t::set_uint32("global_user", "update_time", uid, val);
	//redis_client_t::set_uint32("global_user", "rt", uid, common::time_util_t::now_time()); // 设置数据时间，用于比较好友数据是否要更新
}

uint32_t global_user_data_mgr_t::get_troop_friend_count(uint64_t uid)
{
	if (!is_user_key_exsit(uid))
	{
		log_error("global_user[%lu] not find", uid);
		return 0;
	}

	return redis_client_t::get_uint32("global_user", "troop_friend_count", uid);
}

void global_user_data_mgr_t::set_troop_friend_count(uint64_t uid, uint32_t val)
{
	if (!is_user_key_exsit(uid))
	{
		log_error("global_user[%lu] not find", uid);
		return;
	}

	redis_client_t::set_uint32("global_user", "troop_friend_count", uid, val);
}

uint32_t global_user_data_mgr_t::get_user_level(uint64_t uid)
{
	if (!is_user_key_exsit(uid))
	{
		log_error("global_user[%lu] not find", uid);
		return 0;
	}

	return redis_client_t::get_uint32("global_user", "level", uid);
}

uint32_t global_user_data_mgr_t::get_user_charm(uint64_t uid)
{
	if (!is_user_key_exsit(uid))
	{
		log_error("global_user[%lu] not find", uid);
		return 0;
	}

	return redis_client_t::get_uint32("global_user", "charm", uid);
}

void global_user_data_mgr_t::set_user_charm(uint64_t uid, uint32_t val)
{
	if (!is_user_key_exsit(uid))
	{
		log_error("global_user[%lu] not find", uid);
		return;
	}

	redis_client_t::set_uint32("global_user", "charm", uid, val);
}
void global_user_data_mgr_t::set_user_level(uint64_t uid, uint32_t val)
{
	if (!is_user_key_exsit(uid))
	{
		log_error("global_user[%lu] not find", uid);
		return;
	}

	redis_client_t::set_uint32("global_user", "level", uid, val);
	redis_client_t::set_uint32("global_user", "rt", uid, common::time_util_t::now_time()); // 设置数据时间，用于比较好友数据是否要更新
}

uint32_t global_user_data_mgr_t::get_user_fighting_value(uint64_t uid)
{
	if (!is_user_key_exsit(uid))
	{
		log_error("global_user[%lu] not find", uid);
		return 0;
	}

	return redis_client_t::get_uint32("global_user", "fighting", uid);
}

void global_user_data_mgr_t::set_user_fighting_value(uint64_t uid, uint32_t val)
{
	if (!is_user_key_exsit(uid))
	{
		log_error("global_user[%lu] not find", uid);
		return;
	}

	redis_client_t::set_uint32("global_user", "fighting", uid, val);
	redis_client_t::set_uint32("global_user", "rt", uid, common::time_util_t::now_time()); // 设置数据时间，用于比较好友数据是否要更新
	uint32_t record = redis_client_t::get_uint32("global_user", "fighting_record", uid);
	if (val > record)
	{
		redis_client_t::set_uint32("global_user", "fighting_record", uid, val);
		rank_manager_t::update_rank_power(uid, val);
	}
}

uint32_t global_user_data_mgr_t::get_user_hero_plugin(uint64_t uid)
{
	if (!is_user_key_exsit(uid))
	{
		log_error("global_user[%lu] not find", uid);
		return 0;
	}

	return redis_client_t::get_uint32("global_user", "hero_tid", uid);
}

void global_user_data_mgr_t::set_user_hero_plugin(uint64_t uid, uint32_t val)
{
	if (!is_user_key_exsit(uid))
	{
		log_error("global_user[%lu] not find", uid);
		return;
	}

	redis_client_t::set_uint32("global_user", "hero_tid", uid, val);
	redis_client_t::set_uint32("global_user", "rt", uid, common::time_util_t::now_time()); // 设置数据时间，用于比较好友数据是否要更新
}

bool global_user_data_mgr_t::is_user_key_exsit(uint64_t uid)
{
	return redis_client_t::is_hash_exist("global_user", uid);
}

bool global_user_data_mgr_t::is_user_ban(uint64_t uid)
{
	uint32_t unblock_time = get_user_unblock(uid);
	
	return unblock_time > 0;
}

uint32_t global_user_data_mgr_t::get_user_unblock(uint64_t uid)
{
	if (!is_user_key_exsit(uid))
	{
		log_error("global_user[%lu] not find", uid);
		return 0;
	}
	uint32_t ban_time = redis_client_t::get_uint32("global_user", "ban_time", uid);
	uint32_t now_time = common::time_util_t::now_time();
	if (ban_time  > now_time) 
	{
		return ban_time - now_time;
	}
	else
	{
		return 0;
	}
}

uint64_t global_user_data_mgr_t::get_scene_id(uint64_t uid)
{
	if (!is_user_key_exsit(uid))
	{
		log_error("global_user[%lu] not find", uid);
		return 0;
	}

	return redis_client_t::get_uint64("global_user", "scene_id", uid);
}

void global_user_data_mgr_t::set_scene_id(uint64_t uid, uint64_t scene_id, uint32_t map_tid)
{
	if (!is_user_key_exsit(uid))
	{
		log_error("global_user[%lu] not find", uid);
		return;
	}

	redis_client_t::set_uint64("global_user", "scene_id", uid, scene_id);
	redis_client_t::set_uint32("global_user", "map_tid", uid, map_tid);
}

uint32_t global_user_data_mgr_t::get_map_tid(uint64_t uid)
{
	if (!is_user_key_exsit(uid))
	{
		log_error("global_user[%lu] not find", uid);
		return 0;
	}

	return redis_client_t::get_uint32("global_user", "map_tid", uid);
}

uint32_t global_user_data_mgr_t::get_last_map_tid(uint64_t uid)
{
	if (!is_user_key_exsit(uid))
	{
		log_error("global_user[%lu] not find", uid);
		return 0;
	}

	return redis_client_t::get_uint32("global_user", "last_map_tid", uid);
}

void global_user_data_mgr_t::set_last_map_tid(uint64_t uid, uint32_t val)
{
	if (!is_user_key_exsit(uid))
	{
		log_error("global_user[%lu] not find", uid);
		return;
	}

	redis_client_t::set_uint32("global_user", "last_map_tid", uid, val);
}

uint32_t global_user_data_mgr_t::get_user_plugin(uint64_t uid)
{
	if (!is_user_key_exsit(uid))
	{
		log_error("global_user[%lu] not find", uid);
		return 0;
	}

	return redis_client_t::get_uint32("global_user", "plugin", uid);
}

void global_user_data_mgr_t::set_user_plugin(uint64_t uid, uint32_t val)
{
	if (!is_user_key_exsit(uid))
	{
		log_error("global_user[%lu] not find", uid);
		return;
	}

	redis_client_t::set_uint32("global_user", "plugin", uid, val);
	redis_client_t::set_uint32("global_user", "rt", uid, common::time_util_t::now_time()); // 设置数据时间，用于比较好友数据是否要更新
}

//void global_user_data_mgr_t::load_global_user_common_charm(uint64_t uid, global_user_common_t& common_info)
//{
//	//common_info.m_charm = redis_client_t::get_uint32("global_user", "charm", uid);
//}

bool global_user_data_mgr_t::load_global_user_common(uint64_t uid, global_user_common_t& common_info)
{
	// 可能是新玩家
	if (!is_user_key_exsit(uid))
	{
		log_warn("global_user[%lu] have no this id, may be new", uid);
		return false;
	}

	common_info.m_uid = uid;
	common_info.m_name = redis_client_t::get_string("global_user", "name", uid);
	common_info.m_country_id = redis_client_t::get_uint32("global_user", "country_id", uid);
	common_info.m_family_id = redis_client_t::get_uint64("global_user", "family_id", uid);
	common_info.m_last_logout_time = redis_client_t::get_uint32("global_user", "last_logout_time", uid);
	common_info.m_update_time = redis_client_t::get_uint32("global_user", "update_time", uid);
	common_info.m_level = redis_client_t::get_uint32("global_user", "level", uid);
	common_info.m_fighting_value = redis_client_t::get_uint32("global_user", "fighting", uid);
	common_info.m_hero_plugin = redis_client_t::get_uint32("global_user", "hero_tid", uid);
	common_info.m_scene_id = redis_client_t::get_uint64("global_user", "scene_id", uid);
	common_info.m_map_tid = redis_client_t::get_uint32("global_user", "map_tid", uid);
	common_info.m_last_map_tid = redis_client_t::get_uint32("global_user", "last_map_tid", uid);
	common_info.m_family_officer = redis_client_t::get_uint32("global_user", "family_officer_type", uid);
	common_info.m_family_contribution = redis_client_t::get_uint32("global_user", "family_contribution", uid);
	common_info.m_country_officer = redis_client_t::get_uint32("global_user", "country_officer_type", uid);
	common_info.m_channel_id = redis_client_t::get_uint32("global_user", "channel_id", uid);
	common_info.m_create_time = redis_client_t::get_uint32("global_user", "create_time", uid);
	common_info.m_did = redis_client_t::get_uint32("global_user", "did", uid);
	common_info.m_plugin = redis_client_t::get_uint32("global_user", "plugin", uid);
	common_info.m_charm = redis_client_t::get_uint32("global_user", "charm", uid);
	return true;
}

uint32_t global_user_data_mgr_t::ban_role(uint64_t uid, uint32_t offset_time)
{
	if (0 == offset_time)
	{
		//永久封号等于十年
		offset_time = 10 * 365 * 24 * 3600;
	}
	uint32_t ban_time = redis_client_t::get_uint32("global_user", "ban_time", uid);
	uint32_t now_time = time_util_t::now_time();
	uint32_t max_time = now_time + 10 * 365 * 24 * 3600;
	if (ban_time > now_time)
	{
		if (offset_time > max_time - ban_time)
		{
			ban_time = max_time;
		}
		else
		{
			ban_time += offset_time;
		}
	}
	else
	{
		if (offset_time > max_time - now_time)
		{
			ban_time = max_time;
		}
		else
		{
			ban_time = now_time + offset_time;
		}
	}
	redis_client_t::set_uint32("global_user", "ban_time", uid, ban_time);
	global_user_ptr p_role = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL != p_role)
	{
		proto::server::ea_unreg_user_center_notify ntf;
		ntf.set_uid(uid);
		ntf.set_unreg_code(common::UNREGUSER_RET_BAN);
		ntf.set_code_param(ban_time - now_time);
		env::server->send_msg_to_gate(p_role->get_gate_socket(), op_cmd::ea_unreg_user_center_notify, uid, ntf);
	}
	return ban_time;
}

uint32_t global_user_data_mgr_t::unban_role(uint64_t uid)
{
	redis_client_t::set_uint32("global_user", "ban_time", uid, 0);
	return 0;
}

uint32_t global_user_data_mgr_t::silence_role(uint64_t uid, uint32_t offset_time)
{
	if (0 == offset_time)
	{
		//永久禁言等于十年
		offset_time = 10 * 365 * 24 * 3600;
	}
	uint32_t silence_time = redis_client_t::get_uint32("global_user", "silence_time", uid);
	uint32_t now_time = time_util_t::now_time();
	uint32_t max_time = now_time + 10 * 365 * 24 * 3600;
	if (silence_time > now_time)
	{
		if (offset_time > max_time - silence_time)
		{
			silence_time = max_time;
		}
		else
		{
			silence_time += offset_time;
		}
	}
	else
	{
		if (offset_time > max_time - now_time)
		{
			silence_time = max_time;
		}
		else
		{
			silence_time = now_time + offset_time;
		}
	}
	redis_client_t::set_uint32("global_user", "silence_time", uid, silence_time);
	global_user_ptr p_role = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL != p_role)
	{
		proto::server::eh_silence_role_notify ntf;
		ntf.set_silence_time(silence_time);
		env::server->send_msg_to_chat(op_cmd::eh_silence_role_notify, uid, ntf);

		proto::client::ec_silence_role_ntf ntf_to_client;
		ntf_to_client.set_silence_time(silence_time);
		p_role->send_msg_to_client(op_cmd::ec_silence_role_ntf, ntf_to_client);
	}
	return silence_time;
}

uint32_t global_user_data_mgr_t::unsilence_role(uint64_t uid)
{
	redis_client_t::set_uint32("global_user", "silence_time", uid, 0);
	global_user_ptr p_role = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL != p_role)
	{
		proto::server::eh_silence_role_notify ntf;
		ntf.set_silence_time(0);
		env::server->send_msg_to_chat(op_cmd::eh_silence_role_notify, uid, ntf);

		proto::client::ec_silence_role_ntf ntf_to_client;
		ntf_to_client.set_silence_time(0);
		p_role->send_msg_to_client(op_cmd::ec_silence_role_ntf, ntf_to_client);
	}
	return 0;
}

uint32_t global_user_data_mgr_t::get_silence_time(uint64_t uid)
{
	if (!is_user_key_exsit(uid))
	{
		log_error("global_user[%lu] not find", uid);
		return 0;
	}

	return redis_client_t::get_uint32("global_user", "silence_time", uid);
}
