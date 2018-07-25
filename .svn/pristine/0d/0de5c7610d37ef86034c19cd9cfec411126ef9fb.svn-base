#include "friend_data_mgr.hpp"

#include <vector>

#include "redis_client.hpp"
#include "utility.hpp"

#include "log/log_wrapper.hpp"
#include "family/family_manager.hpp"
#include "user/global_user_manager.hpp"
#include "country/country_mgr.hpp"

#define WRITE_FRIEND_LOG_TO_DBS(uid, friend_uid, oldtype, newtype, type, param) \
    log_wrapper_t::send_friend_log(uid, friend_uid, oldtype, newtype, type, param);


bool friend_data_mgr_t::is_friend_key_exsit(const std::string& key)
{
	return redis_client_t::is_hash_exist("friend", key);
}

bool friend_data_mgr_t::update_apply_time(uint64_t uid, const std::string& key, uint32_t time)
{
	// 不存在直接return
	if (!is_friend_key_exsit(key))
	{
		log_error("role[%lu] friend_key[%s] is not find when update_apply_time", uid, key.c_str());
		return false;
	}

	redis_client_t::set_uint32("friend", "time", key, time);

	return true;
}


bool friend_data_mgr_t::update_contact_time(uint64_t uid, const std::string& key, uint32_t time)
{
	// 不存在直接return
	if (!is_friend_key_exsit(key))
	{
		log_error("role[%lu] friend_key[%s] is not find when update_contact_time", uid, key.c_str());
		return false;
	}

	redis_client_t::set_uint32("friend", "contact_time", key, time);

	return true;
}


bool friend_data_mgr_t::update_top_time(uint64_t uid, const std::string& key, bool is_top, uint32_t time /*= 0*/)
{
	// 不存在直接return
	if (!is_friend_key_exsit(key))
	{
		log_error("role[%lu] friend_key[%s] is not find when update_top_time", uid, key.c_str());
		return false;
	}

	if (is_top)
	{
		redis_client_t::set_uint32("friend", "is_top", key, 1);
		redis_client_t::set_uint32("friend", "time", key, time);
	}
	else
	{
		redis_client_t::set_uint32("friend", "is_top", key, 0);
		redis_client_t::set_uint32("friend", "time", key, 0);
	}

	return true;
}

bool friend_data_mgr_t::clear_friend_info(uint64_t uid, const std::string& key)
{
	// 不存在直接return
	if (!is_friend_key_exsit(key))
	{
		log_error("role[%lu] friend_key[%s] is not find when clear_friend_info", uid, key.c_str());
		return false;
	}

	redis_client_t::remove_key("friend", key);

	return true;
}

void friend_data_mgr_t::load_all_friends(uint64_t uid, friend_info_map& friends,
	friend_contact_list& contact_list, uint32_t& contact_size, 
	friend_apply_list& apply_list, uint32_t& apply_size,
	uint32_t& friend_size, uint32_t& black_size,
	uint32_t& top_size)
{
	friends.clear();
	contact_list.clear();
	apply_list.clear();
	contact_size = 0;
	apply_size = 0;
	friend_size = 0;
	black_size = 0;
	top_size = 0;

	std::vector<std::string> keys;
	redis_client_t::get_keys_by_field_condition<std::string, uint64_t, std::vector<std::string>>("friend", "role_uid", uid, keys);

	for (const auto& key : keys)
	{
		uint64_t friend_uid = redis_client_t::get_uint64("friend", "friend_uid", key);

		if (friend_uid == 0)
		{
			log_error("friend_info[%s] role[%lu] error friend info, friend id == 0", key.c_str(), uid);
			continue;
		}

		proto::common::friend_type type = (proto::common::friend_type)redis_client_t::get_uint32("friend", "friend_type", key);
		if (type == proto::common::friend_type_none)
		{
			log_error("friend_info[%s] role[%lu] error friend type, friend type == friend_type_none", key.c_str(), uid);
			continue;
		}

		friend_info_ptr p_friend = friend_info_ptr(new friend_info_t());
		if (NULL == p_friend)
		{
			log_error("p_friend null error");
			continue;
		}

		p_friend->m_friend_id = friend_uid;
		p_friend->m_redis_key = key;

		p_friend->m_member.set_friend_uid(common::string_util_t::uint64_to_string(friend_uid));
		p_friend->m_member.set_ftype(type);
		p_friend->m_member.set_contact_time(redis_client_t::get_uint32("friend", "contact_time", key));
		p_friend->m_member.set_top_time(redis_client_t::get_uint32("friend", "top_time", key));
		p_friend->m_member.set_is_top(redis_client_t::get_uint32("friend", "is_top", key));
		p_friend->m_member.set_time(redis_client_t::get_uint32("friend", "time", key));
		p_friend->m_member.set_shadow_time(redis_client_t::get_uint32("friend", "shadow_time", key));
		p_friend->m_member.set_shadow_count(redis_client_t::get_uint32("friend", "shadow_count", key));
		p_friend->m_member.set_friendliness(redis_client_t::get_uint32("friend", "friendliness", key));
		p_friend->m_member.set_friendliness_added_today(redis_client_t::get_uint32("friend", "friendliness_added_today", key));
		p_friend->m_member.set_friendliness_refresh_time(redis_client_t::get_uint32("friend", "friendliness_refresh_time", key));
		p_friend->m_member.set_can_get_friendship_point(redis_client_t::get_uint32("friend", "can_get_fp", key));
		p_friend->m_member.set_last_get_fp_time(redis_client_t::get_uint32("friend", "last_get_fp_time", key));
		p_friend->m_member.set_last_sent_fp_time(redis_client_t::get_uint32("friend", "last_sent_fp_time", key));
		// 设置一些最基本的信息
		load_user_base_info(friend_uid, p_friend->m_member.mutable_info());

		friends.insert(std::make_pair(friend_uid, p_friend));

		// 好友数量
		if (type == proto::common::friend_type_one_side_friend || type == proto::common::friend_type_two_side_friend)
		{
			friend_size++;

			if (p_friend->m_member.is_top() > 0)
			{
				top_size++;
			}
		}

		// 黑名单数量
		if (type == proto::common::friend_type_black)
			black_size++;

		// 最近联系人,拉黑不能聊天
		if (p_friend->m_member.contact_time() > 0 && type != proto::common::friend_type_black)
		{
			contact_list.push_back(p_friend);
			contact_size++;
		}

		// 好友申请人(只能是陌生人或者组队好友，其他状态time()应该为0)
		if (p_friend->m_member.time() > 0 && 
			(type == proto::common::friend_type_stranger || type == proto::common::friend_type_troop_stranger))
		{
			apply_list.push_back(p_friend);
			apply_size++;
		}
	}
}

void friend_data_mgr_t::load_all_friend_related(uint64_t uid, uint32_t& fp_has_sent_today, uint32_t& refresh_sent_time, uint32_t& fp_has_received_today,
	uint32_t& refresh_received_time, uint32_t& send_limit, uint32_t& receive_limit, int8_t& initialization, uint32_t& got_relation_award)
{
	fp_has_sent_today = redis_client_t::get_uint32("friend", "fp_has_sent_today", uid);
	refresh_sent_time = redis_client_t::get_uint32("friend", "refresh_sent_time", uid);
	fp_has_received_today = redis_client_t::get_uint32("friend", "fp_has_received_today", uid);
	refresh_received_time = redis_client_t::get_uint32("friend", "refresh_received_time", uid);
	send_limit = redis_client_t::get_uint32("friend", "send_limit", uid);
	receive_limit = redis_client_t::get_uint32("friend", "receive_limit", uid);
	initialization = redis_client_t::get_uint32("friend", "initialization", uid);
	got_relation_award = redis_client_t::get_uint32("friend", "got_relation_award", uid);
}

void friend_data_mgr_t::load_user_base_info(uint64_t friend_uid, proto::common::user_info* p_user_info)
{
	if (NULL == p_user_info)
	{
		log_error("p_user_info null error");
		return;
	}

	if (!is_user_key_exsit(friend_uid))
	{
		log_error("friend[%lu] is not exsit", friend_uid);
		return;
	}

	p_user_info->set_uid(common::string_util_t::uint64_to_string(friend_uid));
	p_user_info->set_name(redis_client_t::get_string("global_user", "name", friend_uid));
}


bool friend_data_mgr_t::search_user_info_by_did(uint32_t did, proto::common::user_info* p_user_info)
{
	std::vector<uint64_t> users;
	redis_client_t::get_keys_by_field_condition<uint64_t, uint32_t, std::vector<uint64_t>>("global_user", "did", did, users);
	if (users.empty())
	{
		log_error("did[%d] not exist", did);
		return false;
	}
	uint64_t uid = users.at(0);
	if (NULL == p_user_info)
	{
		log_error("p_user_info null error");
		return false;
	}

	if (!is_user_key_exsit(uid))
	{
		log_error("uid[%lu] is not exsit", uid);
		return false;
	}

	p_user_info->set_uid(common::string_util_t::uint64_to_string(uid));
	p_user_info->set_name(redis_client_t::get_string("global_user", "name", uid));
	p_user_info->set_level(redis_client_t::get_uint32("global_user", "level", uid));
	uint32_t country_id = redis_client_t::get_uint32("global_user", "country_id", uid);
	if (country_id != 0) {
		country_mgr_t::get_country_common(country_id, p_user_info->mutable_country());
	}


	uint64_t family_id = redis_client_t::get_uint64("global_user", "family_id", uid);
	if (family_id != 0)
	{
		const family_ptr& p_family = family_manager_t::get_family(family_id);
		if (NULL != p_family)
		{
			p_user_info->set_family(p_family->get_name());
		}
		else
		{
			p_user_info->set_family("");
		}
	}
	else
	{
		p_user_info->set_family("");
	}

	p_user_info->set_fighting(redis_client_t::get_uint32("global_user", "fighting", uid));
	p_user_info->set_plugin(redis_client_t::get_uint32("global_user", "hero_tid", uid));

	// 上线redis rt会刷新
	global_user_ptr p_user = global_user_manager_t::get_global_user_by_uid(uid);
	uint32_t offline_time = 0;
	if (NULL == p_user || p_user->is_logout())
	{
		offline_time = common::time_util_t::now_time() - redis_client_t::get_uint32("global_user", "last_logout_time", uid);
	}
	p_user_info->set_offline_time(offline_time);

	return true;
}


bool friend_data_mgr_t::search_user_info_by_uid(uint64_t uid, proto::common::user_info* p_user_info)
{
	if (NULL == p_user_info)
	{
		log_error("p_user_info null error");
		return false;
	}

	if (!is_user_key_exsit(uid))
	{
		log_error("uid[%lu] is not exsit", uid);
		return false;
	}

	p_user_info->set_uid(common::string_util_t::uint64_to_string(uid));
	p_user_info->set_name(redis_client_t::get_string("global_user", "name", uid));
	p_user_info->set_level(redis_client_t::get_uint32("global_user", "level", uid));
	uint32_t country_id = redis_client_t::get_uint32("global_user", "country_id", uid);
	if (country_id != 0) {
		country_mgr_t::get_country_common(country_id, p_user_info->mutable_country());
	}


	uint64_t family_id = redis_client_t::get_uint64("global_user", "family_id", uid);
	if (family_id != 0)
	{
		const family_ptr& p_family = family_manager_t::get_family(family_id);
		if (NULL != p_family)
		{
			p_user_info->set_family(p_family->get_name());
		}
		else
		{
			p_user_info->set_family("");
		}
	}
	else
	{
		p_user_info->set_family("");
	}

	p_user_info->set_fighting(redis_client_t::get_uint32("global_user", "fighting", uid));
	p_user_info->set_plugin(redis_client_t::get_uint32("global_user", "hero_tid", uid));

	// 上线redis rt会刷新
	global_user_ptr p_user = global_user_manager_t::get_global_user_by_uid(uid);
	uint32_t offline_time = 0;
	if (NULL == p_user || p_user->is_logout())
	{
		offline_time = common::time_util_t::now_time() - redis_client_t::get_uint32("global_user", "last_logout_time", uid);
	}
	p_user_info->set_offline_time(offline_time);

	return true;
}

bool friend_data_mgr_t::search_user_info_by_name(const std::string& name, proto::common::user_info* p_user_info)
{
	if (NULL == p_user_info)
	{
		log_error("p_user_info null error");
		return false;
	}

	std::vector<uint64_t> users;
	redis_client_t::get_keys_by_field_condition<uint64_t, std::string, std::vector<uint64_t>>("global_user", "name", name, users);

	if (users.empty())
	{
		// 没有这个人正常返回
		return false;
	}

	return search_user_info_by_uid(users[0], p_user_info);
}

bool friend_data_mgr_t::is_user_key_exsit(uint64_t uid)
{
	return redis_client_t::is_hash_exist("global_user", uid);
}

void friend_data_mgr_t::update_user_info(friend_info_ptr p_info)	//将redis中的数据更新到p_info中
{
	if (NULL == p_info)
	{
		log_error("p_info null error");
		return;
	}

	if (!is_user_key_exsit(p_info->m_friend_id))
	{
		log_error("friend[%lu] is not exsit", p_info->m_friend_id);
		return;
	}

	// 时间比redis的小就更新一下
	uint32_t rk_time = redis_client_t::get_uint32("global_user", "rt", p_info->m_friend_id);
	if (p_info->m_rt < rk_time)
	{
		proto::common::user_info* p_user_info = p_info->m_member.mutable_info();
		if (p_user_info == NULL)
		{
			log_error("friend[%lu] p_user_info null error", p_info->m_friend_id);
			return;
		}

		p_user_info->set_level(redis_client_t::get_uint32("global_user", "level", p_info->m_friend_id));
		p_user_info->set_name(redis_client_t::get_string("global_user", "name", p_info->m_friend_id));
		p_user_info->set_charm(redis_client_t::get_uint32("friend_related", "charm", p_info->m_friend_id));

		uint32_t country_id = redis_client_t::get_uint32("global_user", "country_id", p_info->m_friend_id);
		if (country_id != 0) {
			country_mgr_t::get_country_common(country_id, p_user_info->mutable_country());
		}

		uint64_t family_id = redis_client_t::get_uint64("global_user", "family_id", p_info->m_friend_id);
		if (family_id != 0)
		{
			const family_ptr& p_family = family_manager_t::get_family(family_id);
			if (NULL != p_family)
			{
				p_user_info->set_family(p_family->get_name());
			}
			else
			{
				p_user_info->set_family("");
			}
		}
		else
		{
			p_user_info->set_family("");
		}

		p_user_info->set_fighting(redis_client_t::get_uint32("global_user", "fighting", p_info->m_friend_id));
		p_user_info->set_plugin(redis_client_t::get_uint32("global_user", "hero_tid", p_info->m_friend_id));

		// 上线redis rt会刷新
		global_user_ptr p_user = global_user_manager_t::get_global_user_by_uid(p_info->m_friend_id);
		uint32_t offline_time = 0;
		if (NULL == p_user || !p_user->is_online())
		{
			offline_time = common::time_util_t::now_time() - redis_client_t::get_uint32("global_user", "last_logout_time", p_info->m_friend_id);
		}
		p_user_info->set_offline_time(offline_time);

		p_info->m_rt = rk_time;
	}
}

proto::common::friend_type friend_data_mgr_t::get_offline_friend_type(uint64_t uid, uint64_t friend_uid)
{
	std::string rk;
	std::ostringstream oss;
	oss << uid << friend_uid;
	rk = oss.str();

	// 不存在说明和这个人从来没有建立过社会关系
	if (!is_friend_key_exsit(rk))
	{
		return proto::common::friend_type_none;
	}

	// 返回0 说明两则不存在社会关系或者社会关系已经被解除
	return (proto::common::friend_type)redis_client_t::get_uint32("friend", "friend_type", rk);
}


void friend_data_mgr_t::update_offline_friend_apply_time(uint64_t uid, uint64_t friend_uid, uint32_t time_now)
{
	std::string rk;
	std::ostringstream oss;
	oss << uid << friend_uid;
	rk = oss.str();

	proto::common::friend_type old_type = proto::common::friend_type_none;
	proto::common::friend_type new_type = proto::common::friend_type_none;
	// 存在
	if (is_friend_key_exsit(rk))
	{
		old_type = (proto::common::friend_type)redis_client_t::get_uint32("friend", "friend_type", rk);

		switch (old_type)
		{
			// 无效数据重新赋值
		case proto::common::friend_type_none:
			{
				REDIS_BATCH_START
				redis_client_t::set_uint64("friend", "friend_uid", rk, friend_uid);
				redis_client_t::set_uint64("friend", "role_uid", rk, uid);
				redis_client_t::set_uint32("friend", "friend_type", rk, (uint32_t)proto::common::friend_type_stranger);
				redis_client_t::set_uint32("friend", "contact_time", rk, 0);
				redis_client_t::set_uint32("friend", "top_time", rk, 0);
				redis_client_t::set_uint32("friend", "is_top", rk, 0);
				redis_client_t::set_uint32("friend", "time", rk, time_now);
				REDIS_BATCH_DONE

				new_type = proto::common::friend_type_stranger;
			}
			break;
			// 只需要赋值申请时间
		case proto::common::friend_type_stranger:		
		case proto::common::friend_type_troop_stranger:
			{
				redis_client_t::set_uint32("friend", "time", rk, time_now);

				new_type = old_type;
			}
			break;
		default:
			{
				log_error("role[%lu] friend[%lu] update_offline_friend_apply_time type[%u] error", uid, friend_uid, (uint32_t)old_type);
			}
			break;
		}
	}
	else
	{
		REDIS_BATCH_START
		redis_client_t::set_uint64("friend", "friend_uid", rk, friend_uid);
		redis_client_t::set_uint64("friend", "role_uid", rk, uid);
		redis_client_t::set_uint32("friend", "friend_type", rk, (uint32_t)proto::common::friend_type_stranger);
		redis_client_t::set_uint32("friend", "contact_time", rk, 0);
		redis_client_t::set_uint32("friend", "top_time", rk, 0);
		redis_client_t::set_uint32("friend", "is_top", rk, 0);
		redis_client_t::set_uint32("friend", "time", rk, time_now);
		REDIS_BATCH_DONE

		new_type = proto::common::friend_type_stranger;
	}

	WRITE_FRIEND_LOG_TO_DBS(uid, friend_uid, old_type, new_type, common::log_enum::source_type_friend_update_apply_time, EM_FRIEND_PARAM::FRIEND_ADD_FRIEND_ME);
}


void friend_data_mgr_t::update_offline_friend_contact_time(uint64_t uid, uint64_t friend_uid, uint32_t time_now)
{
	std::string rk;
	std::ostringstream oss;
	oss << uid << friend_uid;
	rk = oss.str();

	proto::common::friend_type old_type = proto::common::friend_type_none;
	proto::common::friend_type new_type = proto::common::friend_type_none;

	// 存在
	if (is_friend_key_exsit(rk))
	{
		old_type = (proto::common::friend_type)redis_client_t::get_uint32("friend", "friend_type", rk);

		switch (old_type)
		{
			// 无效数据重新赋值
		case proto::common::friend_type_none:
		{
			REDIS_BATCH_START
			redis_client_t::set_uint64("friend", "friend_uid", rk, friend_uid);
			redis_client_t::set_uint64("friend", "role_uid", rk, uid);
			redis_client_t::set_uint32("friend", "friend_type", rk, (uint32_t)proto::common::friend_type_stranger);
			redis_client_t::set_uint32("friend", "contact_time", rk, time_now);
			redis_client_t::set_uint32("friend", "top_time", rk, 0);
			redis_client_t::set_uint32("friend", "is_top", rk, 0);
			redis_client_t::set_uint32("friend", "time", rk, 0);
			REDIS_BATCH_DONE

			new_type = proto::common::friend_type_stranger;
		}
		break;
		// 只需要赋值最近联系时间
		case proto::common::friend_type_stranger:
		case proto::common::friend_type_troop_stranger:
		case proto::common::friend_type_one_side_friend:
		case proto::common::friend_type_two_side_friend:
		{
			redis_client_t::set_uint32("friend", "contact_time", rk, time_now);

			new_type = old_type;
		}
		break;
		default:
		{
			log_error("role[%lu] friend[%lu] update_offline_friend_apply_time type[%u] error", uid, friend_uid, (uint32_t)old_type);
			return;
		}
		break;
		}
	}
	else
	{
		REDIS_BATCH_START
		redis_client_t::set_uint64("friend", "friend_uid", rk, friend_uid);
		redis_client_t::set_uint64("friend", "role_uid", rk, uid);
		redis_client_t::set_uint32("friend", "friend_type", rk, (uint32_t)proto::common::friend_type_stranger);
		redis_client_t::set_uint32("friend", "contact_time", rk, time_now);
		redis_client_t::set_uint32("friend", "top_time", rk, 0);
		redis_client_t::set_uint32("friend", "is_top", rk, 0);
		redis_client_t::set_uint32("friend", "time", rk, 0);
		REDIS_BATCH_DONE

		new_type = proto::common::friend_type_stranger;
	}

	WRITE_FRIEND_LOG_TO_DBS(uid, friend_uid, old_type, new_type, common::log_enum::source_type_friend_update_content_time, 0);
}

void friend_data_mgr_t::update_offline_friend_type(uint64_t uid, uint64_t friend_uid, proto::common::friend_type type)
{
	std::string rk;
	std::ostringstream oss;
	oss << uid << friend_uid;
	rk = oss.str();

	if (!is_friend_key_exsit(rk))
	{
		log_error("role[%lu] friend_key[%s] is not find when update_offline_friend_type", uid, rk.c_str());
		return;
	}

	proto::common::friend_type old_type = (proto::common::friend_type)redis_client_t::get_uint32("friend", "friend_type", rk);

	redis_client_t::set_uint32("friend", "friend_type", rk, (uint32_t)type);

	WRITE_FRIEND_LOG_TO_DBS(uid, friend_uid, old_type, type, common::log_enum::source_type_friend_update_friend_type, 0);
}

void friend_data_mgr_t::update_friend_friendliness_in_redis(uint64_t uid, uint64_t friend_uid, uint32_t friendliness_value)
{
	std::string rk;
	std::ostringstream oss;
	oss << uid << friend_uid;
	rk = oss.str();

	if (!is_friend_key_exsit(rk))
	{
		log_error("role[%lu] friend_key[%s] is not find when update_offline_friend_friendliness", uid, rk.c_str());
		return;
	}

	redis_client_t::set_uint32("friend", "friendliness", rk, friendliness_value);
}

void friend_data_mgr_t::update_i_can_get_fp_from_friend(uint64_t uid, uint64_t friend_uid, uint32_t value)
{
	std::string rk;
	std::ostringstream oss;
	oss << uid << friend_uid;
	rk = oss.str();

	if (!is_friend_key_exsit(rk))
	{
		log_error("role[%lu] friend_key[%s] is not find when update_offline_friend_can_get_fp", uid, rk.c_str());
		return;
	}
	redis_client_t::set_uint32("friend", "can_get_fp", rk, value);
	//WRITE_FRIEND_LOG_TO_DBS(uid, friend_uid, old_value, friendliness_value, common::log_enum::source_type_friend_update_friendliness, 0);
}

void friend_data_mgr_t::update_last_i_got_fp_timestamp(uint64_t uid, uint64_t friend_uid, uint32_t timestamp)
{
	std::string rk;
	std::ostringstream oss;
	oss << uid << friend_uid;
	rk = oss.str();

	if (!is_friend_key_exsit(rk))
	{
		log_error("role[%lu] friend_key[%s] is not find when update_last_i_got_fp_timestamp", uid, rk.c_str());
		return;
	}
	redis_client_t::set_uint32("friend", "last_get_fp_time", rk, timestamp);
}

void friend_data_mgr_t::update_last_i_sent_fp_timestamp(uint64_t uid, uint64_t friend_uid, uint32_t timestamp)
{
	std::string rk;
	std::ostringstream oss;
	oss << uid << friend_uid;
	rk = oss.str();

	if (!is_friend_key_exsit(rk))
	{
		log_error("role[%lu] friend_key[%s] is not find when update_last_i_sent_fp_timestamp", uid, rk.c_str());
		return;
	}
	redis_client_t::set_uint32("friend", "last_sent_fp_time", rk, timestamp);
}

uint32_t friend_data_mgr_t::get_offline_friend_friendliness(uint64_t uid, uint64_t friend_uid)
{
	std::string rk;
	std::ostringstream oss;
	oss << uid << friend_uid;
	rk = oss.str();

	if (!is_friend_key_exsit(rk))
	{
		log_error("role[%lu] friend_key[%s] is not find when get_offline_friend_friendliness", uid, rk.c_str());
		return 0;
	}
	return redis_client_t::get_uint32("friend", "friendliness", rk);
}

// 得到离线数据 : 今日增加的好友亲密度
uint32_t friend_data_mgr_t::get_offline_friend_friendliness_added_today(uint64_t uid, uint64_t friend_uid)
{
	std::string rk;
	std::ostringstream oss;
	oss << uid << friend_uid;
	rk = oss.str();

	if (!is_friend_key_exsit(rk))
	{
		log_error("role[%lu] friend_key[%s] is not find when get_offline_friend_friendliness_added_today", uid, rk.c_str());
		return 0;
	}
	return redis_client_t::get_uint32("friend", "friendliness_added_today", rk);
}

// 得到离线数据 : 上次刷新日增亲密度的时间
uint32_t friend_data_mgr_t::get_offline_friend_friendliness_last_refresh_time(uint64_t uid, uint64_t friend_uid)
{
	std::string rk;
	std::ostringstream oss;
	oss << uid << friend_uid;
	rk = oss.str();

	if (!is_friend_key_exsit(rk))
	{
		log_error("role[%lu] friend_key[%s] is not find when get_offline_friend_friendliness_last_refresh_time", uid, rk.c_str());
		return 0;
	}
	return redis_client_t::get_uint32("friend", "friendliness_refresh_time", rk);
}

// 处理离线数据 : 重置日增亲密度
void friend_data_mgr_t::reset_offline_friend_friendliness_added_today(uint64_t uid, uint64_t friend_uid)
{
	std::string rk;
	std::ostringstream oss;
	oss << uid << friend_uid;
	rk = oss.str();

	if (!is_friend_key_exsit(rk))
	{
		log_error("role[%lu] friend_key[%s] is not find when reset_offline_friend_friendliness_added_today", uid, rk.c_str());
		return;
	}
	redis_client_t::set_uint32("friend", "friendliness_added_today", rk, 0);
	redis_client_t::set_uint32("friend", "friendliness_refresh_time", rk, common::time_util_t::now_time());
}

void friend_data_mgr_t::update_offline_friend_friendliness_added_today(uint64_t uid, uint64_t friend_uid, uint32_t friendliness_value_added_today)
{
	std::string rk;
	std::ostringstream oss;
	oss << uid << friend_uid;
	rk = oss.str();

	if (!is_friend_key_exsit(rk))
	{
		log_error("role[%lu] friend_key[%s] is not find when update_offline_friend_friendliness", uid, rk.c_str());
		return;
	}
	redis_client_t::set_uint32("friend", "friendliness_added_today", rk, friendliness_value_added_today);
}

uint32_t friend_data_mgr_t::get_offline_logout_time(uint64_t uid)
{
	if (!is_user_key_exsit(uid))
	{
		log_error("friend[%lu] is not exsit", uid);
		return 0;
	}

	return redis_client_t::get_uint32("global_user", "last_logout_time", uid);
}

//uint32_t friend_data_mgr_t::get_offline_player_charm(uint64_t uid)
//{
//	return redis_client_t::get_uint32("friend_related", "charm", uid);
//}

//void friend_data_mgr_t::update_player_charm(uint64_t uid, uint32_t value)
//{
//	redis_client_t::set_uint32("friend_related", "charm", uid, value);
//	redis_client_t::set_uint32("global_user", "charm", uid, value);
//}

void friend_data_mgr_t::update_all_friend_info(uint64_t uid, const friend_info_ptr& p_friend)
{
	if (NULL == p_friend) {
		log_error("role[%lu] p_friend null error", uid);
		return;
	}

	REDIS_BATCH_START
	redis_client_t::set_uint64("friend", "friend_uid", p_friend->m_redis_key, p_friend->m_friend_id);
	redis_client_t::set_uint64("friend", "role_uid", p_friend->m_redis_key, uid);
	redis_client_t::set_uint32("friend", "friend_type", p_friend->m_redis_key, p_friend->m_member.ftype());
	redis_client_t::set_uint32("friend", "contact_time", p_friend->m_redis_key, p_friend->m_member.contact_time());
	redis_client_t::set_uint32("friend", "top_time", p_friend->m_redis_key, p_friend->m_member.top_time());
	redis_client_t::set_uint32("friend", "is_top", p_friend->m_redis_key, p_friend->m_member.is_top());
	redis_client_t::set_uint32("friend", "time", p_friend->m_redis_key, p_friend->m_member.time());
	redis_client_t::set_uint32("friend", "shadow_time", p_friend->m_redis_key, p_friend->m_member.shadow_time());
	redis_client_t::set_uint32("friend", "shadow_count", p_friend->m_redis_key, p_friend->m_member.shadow_count());
	redis_client_t::set_uint32("friend", "friendliness", p_friend->m_redis_key, p_friend->m_member.friendliness());
	redis_client_t::set_uint32("friend", "friendliness_added_today", p_friend->m_redis_key, p_friend->m_member.friendliness_added_today());
	redis_client_t::set_uint32("friend", "friendliness_refresh_time", p_friend->m_redis_key, p_friend->m_member.friendliness_refresh_time());
	redis_client_t::set_uint32("friend", "can_get_fp", p_friend->m_redis_key, p_friend->m_member.can_get_friendship_point());
	redis_client_t::set_uint32("friend", "last_get_fp_time", p_friend->m_redis_key, p_friend->m_member.last_get_fp_time());
	redis_client_t::set_uint32("friend", "last_sent_fp_time", p_friend->m_redis_key, p_friend->m_member.last_sent_fp_time());
	REDIS_BATCH_DONE
}

bool friend_data_mgr_t::update_friend_type(uint64_t uid, const std::string& key, proto::common::friend_type type)
{
	// 不存在直接return
	if (!is_friend_key_exsit(key))
	{
		log_error("role[%lu] friend_key[%s] is not find when update_contact_time", uid, key.c_str());
		return false;
	}

	redis_client_t::set_uint32("friend", "friend_type", key, (uint32_t)type);
	return true;
}

bool friend_data_mgr_t::update_shadow_time(uint64_t uid, const friend_info_ptr& p_friend)
{
	if (NULL == p_friend) {
		log_error("role[%lu] p_friend null error", uid);
		return false;
	}

	redis_client_t::set_uint32("friend", "shadow_time", p_friend->m_redis_key, p_friend->m_member.shadow_time());
	redis_client_t::set_uint32("friend", "shadow_count", p_friend->m_redis_key, p_friend->m_member.shadow_count());
	return true;
}

void friend_data_mgr_t::update_fp_has_sent_today(uint64_t role_id, uint32_t value)
{
	if (!redis_client_t::set_uint32("friend_related", "fp_has_sent_today", role_id, value))
		log_error("update_fp_has_sent_today failed, role_id[%lu], value[%u]", role_id, value);
}

void friend_data_mgr_t::update_refresh_sent_time(uint64_t role_id, uint32_t value)
{
	if (!redis_client_t::set_uint32("friend_related", "refresh_sent_time", role_id, value))
		log_error("update_refresh_sent_time failed, role_id[%lu], value[%u]", role_id, value);
}



void friend_data_mgr_t::update_fp_has_received_today(uint64_t role_id, uint32_t value)
{
	if(!redis_client_t::set_uint32("friend_related", "fp_has_received_today", role_id, value))
		log_error("update_fp_has_received_today failed, role_id[%lu], value[%u]", role_id, value);
}

void friend_data_mgr_t::update_refresh_received_time(uint64_t role_id, uint32_t value)
{
	if(!redis_client_t::set_uint32("friend_related", "refresh_received_time", role_id, value))
		log_error("update_refresh_received_time failed, role_id[%lu], value[%u]", role_id, value);
}

void friend_data_mgr_t::update_receive_limit(uint64_t role_id, uint32_t value)
{
	if(!redis_client_t::set_uint32("friend_related", "receive_limit", role_id, value))
		log_error("update_receive_limit failed, role_id[%lu], value[%u]", role_id, value);
}

void friend_data_mgr_t::update_send_limit(uint64_t role_id, uint32_t value)
{
	if(!redis_client_t::set_uint32("friend_related", "send_limit", role_id, value))
		log_error("update_send_limit failed, role_id[%lu], value[%u]", role_id, value);
}

void friend_data_mgr_t::update_initialization(uint64_t role_id)
{
	if(!redis_client_t::set_uint32("friend_related", "initialization", role_id, 1))
		log_error("update_initialization failed, role_id[%lu], value[%u]", role_id, 1);
}

void friend_data_mgr_t::update_got_relation_award(uint64_t role_id, uint32_t level)
{
	if(!redis_client_t::set_uint32("friend_related", "got_relation_award", role_id, level))
		log_error("update_got_relation_award failed, role_id[%lu], value[%u]", role_id, level);
}
