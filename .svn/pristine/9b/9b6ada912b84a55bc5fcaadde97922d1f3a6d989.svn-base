#include "friend_data_mgr.hpp"

#include <vector>

#include "common/redis_client.hpp"
#include "utility.hpp"

#include "log/log_wrapper.hpp"
#include "family/family_manager.hpp"
#include "user/global_user_manager.hpp"
#include "country/country_mgr.hpp"

#define WRITE_FRIEND_LOG_TO_DBS(uid, friend_uid, oldtype, newtype, type, param) \
    log_wrapper_t::send_friend_log(uid, friend_uid, oldtype, newtype, type, param);


bool friend_data_mgr_t::is_friend_key_exsit(const std::string& key)
{
	return redis_client_t::is_key_exist("friend", key);
}

bool friend_data_mgr_t::update_apply_time(uint32_t uid, const std::string& key, uint32_t time)
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


bool friend_data_mgr_t::update_contact_time(uint32_t uid, const std::string& key, uint32_t time)
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


bool friend_data_mgr_t::update_top_time(uint32_t uid, const std::string& key, bool is_top, uint32_t time /*= 0*/)
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

bool friend_data_mgr_t::clear_friend_info(uint32_t uid, const std::string& key)
{
	// 不存在直接return
	if (!is_friend_key_exsit(key))
	{
		log_error("role[%lu] friend_key[%s] is not find when clear_friend_info", uid, key.c_str());
		return false;
	}

	redis_client_t::set_uint64("friend", "friend_uid", key, 0);
	redis_client_t::set_uint64("friend", "role_uid", key, 0);
	redis_client_t::set_uint32("friend", "friend_type", key, 0);
	redis_client_t::set_uint32("friend", "contact_time", key, 0);
	redis_client_t::set_uint32("friend", "top_time", key, 0);
	redis_client_t::set_uint32("friend", "is_top", key, 0);
	redis_client_t::set_uint32("friend", "time", key, 0);

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


bool friend_data_mgr_t::search_user_info_by_id(uint64_t uid, proto::common::user_info* p_user_info)
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

	return search_user_info_by_id(users[0], p_user_info);
}

bool friend_data_mgr_t::is_user_key_exsit(uint64_t uid)
{
	return redis_client_t::is_key_exist("global_user", uid);
}

void friend_data_mgr_t::update_user_info(friend_info_ptr p_info)
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
	uint64_t rk_time = redis_client_t::get_uint64("global_user", "rt", p_info->m_friend_id);
	if (p_info->m_rt < rk_time)
	{
		proto::common::user_info* p_user_info = p_info->m_member.mutable_info();
		if (p_user_info == NULL)
		{
			log_error("friend[%lu] p_user_info null error", p_info->m_friend_id);
			return;
		}

		p_user_info->set_level(redis_client_t::get_uint32("global_user", "level", p_info->m_friend_id));
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
		if (NULL == p_user || p_user->is_logout())
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
				redis_client_t::set_uint64("friend", "friend_uid", rk, friend_uid);
				redis_client_t::set_uint64("friend", "role_uid", rk, uid);
				redis_client_t::set_uint32("friend", "friend_type", rk, (uint32_t)proto::common::friend_type_stranger);
				redis_client_t::set_uint32("friend", "contact_time", rk, 0);
				redis_client_t::set_uint32("friend", "top_time", rk, 0);
				redis_client_t::set_uint32("friend", "is_top", rk, 0);
				redis_client_t::set_uint32("friend", "time", rk, time_now);

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
		redis_client_t::set_uint64("friend", "friend_uid", rk, friend_uid);
		redis_client_t::set_uint64("friend", "role_uid", rk, uid);
		redis_client_t::set_uint32("friend", "friend_type", rk, (uint32_t)proto::common::friend_type_stranger);
		redis_client_t::set_uint32("friend", "contact_time", rk, 0);
		redis_client_t::set_uint32("friend", "top_time", rk, 0);
		redis_client_t::set_uint32("friend", "is_top", rk, 0);
		redis_client_t::set_uint32("friend", "time", rk, time_now);

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
			redis_client_t::set_uint64("friend", "friend_uid", rk, friend_uid);
			redis_client_t::set_uint64("friend", "role_uid", rk, uid);
			redis_client_t::set_uint32("friend", "friend_type", rk, (uint32_t)proto::common::friend_type_stranger);
			redis_client_t::set_uint32("friend", "contact_time", rk, time_now);
			redis_client_t::set_uint32("friend", "top_time", rk, 0);
			redis_client_t::set_uint32("friend", "is_top", rk, 0);
			redis_client_t::set_uint32("friend", "time", rk, 0);

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
		redis_client_t::set_uint64("friend", "friend_uid", rk, friend_uid);
		redis_client_t::set_uint64("friend", "role_uid", rk, uid);
		redis_client_t::set_uint32("friend", "friend_type", rk, (uint32_t)proto::common::friend_type_stranger);
		redis_client_t::set_uint32("friend", "contact_time", rk, time_now);
		redis_client_t::set_uint32("friend", "top_time", rk, 0);
		redis_client_t::set_uint32("friend", "is_top", rk, 0);
		redis_client_t::set_uint32("friend", "time", rk, 0);

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


uint32_t friend_data_mgr_t::get_offline_logout_time(uint64_t uid)
{
	if (!is_user_key_exsit(uid))
	{
		log_error("friend[%lu] is not exsit", uid);
		return 0;
	}

	return redis_client_t::get_uint32("global_user", "last_logout_time", uid);
}

void friend_data_mgr_t::update_all_friend_info(uint64_t uid, friend_info_ptr p_friend)
{
	if (NULL == p_friend)
	{
		log_error("role[%lu] p_friend null error", uid);
		return;
	}

	redis_client_t::set_uint64("friend", "friend_uid", p_friend->m_redis_key, p_friend->m_friend_id);
	redis_client_t::set_uint64("friend", "role_uid", p_friend->m_redis_key, uid);
	redis_client_t::set_uint32("friend", "friend_type", p_friend->m_redis_key, p_friend->m_member.ftype());
	redis_client_t::set_uint32("friend", "contact_time", p_friend->m_redis_key, p_friend->m_member.contact_time());
	redis_client_t::set_uint32("friend", "top_time", p_friend->m_redis_key, p_friend->m_member.top_time());
	redis_client_t::set_uint32("friend", "is_top", p_friend->m_redis_key, p_friend->m_member.is_top());
	redis_client_t::set_uint32("friend", "time", p_friend->m_redis_key, p_friend->m_member.time());
}

bool friend_data_mgr_t::update_friend_type(uint32_t uid, const std::string& key, proto::common::friend_type type)
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
