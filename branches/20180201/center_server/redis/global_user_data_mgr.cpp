#include "global_user_data_mgr.hpp"
#include "common/redis_client.hpp"
#include "common/global_id.hpp"

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

void global_user_data_mgr_t::set_user_level(uint64_t uid, uint32_t val)
{
	if (!is_user_key_exsit(uid))
	{
		log_error("global_user[%lu] not find", uid);
		return;
	}

	redis_client_t::set_uint32("global_user", "level", uid, val);
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
}

bool global_user_data_mgr_t::is_user_key_exsit(uint64_t uid)
{
	return redis_client_t::is_key_exist("global_user", uid);
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

void global_user_data_mgr_t::load_global_user_common(uint64_t uid, global_user_common_t& common_info)
{
	// 可能是新玩家
	if (!is_user_key_exsit(uid))
	{
		//log_warn("global_user[%lu] have no this id, may be new", uid);
		return;
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
}

void global_user_data_mgr_t::create_new_global_user(uint64_t uid, const std::string& name, uint32_t create_time, uint32_t channel_id )
{
	redis_client_t::set_string("global_user", "name", uid, name);
	redis_client_t::set_uint32("global_user", "create_time", uid, create_time);
	redis_client_t::set_uint32("global_user", "level", uid, 1);
	redis_client_t::set_uint32("global_user", "fighting", uid, 0);
	redis_client_t::set_uint32("global_user", "update_time", uid, common::time_util_t::now_time());
	redis_client_t::set_uint32("global_user", "channel_id", uid, channel_id);

	std::string did_str = user_did_generator::gen_user_did(uid);
	uint32_t did = string_util_t::string_to_uint32(did_str);
	redis_client_t::set_uint32("global_user", "did", uid, did);
}
