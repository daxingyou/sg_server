#include "rank_manager.hpp"
#include "client.pb.h"
#include "tblh/RankReward.tbls.h"
#include "tblh/RankType.tbls.h"
#include "common/utility.hpp"
#include "common/config_mgr.h"
#include "mail/mail_manager.hpp"
#include "sys_notice/sys_notice_manager.hpp"
#include "user/global_user_manager.hpp"
#include "family/family_manager.hpp"
#include "tblh/rank_common.hpp"
#include "common/time_manager.hpp"

USING_NS_COMMON;

rank_t::rank_t()
{
}

rank_t::~rank_t()
{
}

void rank_t::request_rank(global_user_ptr user)
{
	RANK_LOG("rank_t::request_rank");
	if (NULL == user)
	{
		log_error("NULL == user");
		return;
	}
	auto p_rank_conf = GET_CONF(RankType, get_id());
	if (NULL == p_rank_conf)
	{
		log_error("NULL == p_rank_conf[%d]", get_id());
		return;
	}
	uint64_t self_id = 0;
	if (p_rank_conf->owner_type() == rot_user)
	{
		self_id = user->get_uid();
	}
	else if (p_rank_conf->owner_type() == rot_family)
	{
		self_id = user->get_family_id();
	}
	
	uint32_t num = p_rank_conf->count();

	proto::client::ec_rank_reply reply;
	reply.set_rank_type(get_id());

	uint32_t query_num = m_record_num;
	if (num != 0)
	{
		query_num = num;
	}

	bool find_self = false;
	uint32_t index = 1;

	for (sort_iter it = m_rank.begin(); it != m_rank.end() && index <= query_num; ++it)
	{
		proto::common::client_rank_single* add = reply.mutable_rank_data()->add_rank_list();
		if (NULL == add)
			return;

		fill_rank_info(add, it->second, index);

		if ((it->second.owner) == self_id)
		{
			find_self = true;
			fill_rank_info(reply.mutable_rank_data()->mutable_self(), it->second, index);
		}

		index++;
	}

	if (!find_self)
	{
		get_rank_data(reply.mutable_rank_data()->mutable_self(), self_id);
	}

	//send to client
	user->send_msg_to_client(op_cmd::ec_rank_reply, reply);
	RANK_LOG("user[%llu] send rank to client, count[%d]", user->get_uid(), reply.rank_data().rank_list_size());
}

void rank_t::rank_reward(uint32_t param)
{
	// 防止空遍历，先判断第一名有木有奖励
	RankReward* entry = get_rank_reward_entry(get_id(), 1, param);
	if (NULL == entry || entry->item().empty())
		return;

	uint32_t index = 1;
	for (sort_iter it = m_rank.begin(); it != m_rank.end(); ++it, ++index)
	{
		entry = get_rank_reward_entry(get_id(), index, param);
		if (NULL == entry || entry->item().empty())
			return;

		switch (get_owner_type())
		{
			case rot_user:
			{
				// 发邮件
			}
			break;
			case rot_family:
			{
				// 帮派邮件
			}
			break;
			default:
				break;
		}
	}
}


void rank_t::rank_reward(uint32_t index, uint64_t object_uid)
{
	// 防止空遍历，先判断第一名有木有奖励
	RankReward* entry = get_rank_reward_entry(get_id(), index, 0);
	if (NULL == entry || entry->item().empty())
		return;

	switch (get_owner_type())
	{
	case rot_user:
	{
		// 发邮件
		std::map<uint32_t, uint32_t> items_id_count;
		for (auto item : entry->item())
		{
			items_id_count[item.first] = item.second;
		}
		mail_manager_t::send_mail(object_uid, proto::common::MAIL_TYPE_SYS, 7, items_id_count);
	}
	break;
	case rot_family:
	{
		// 帮派邮件
	}
	break;
	default:
		break;
	}
}

void rank_t::day_reward()
{
	// 防止空遍历，先判断第一名有木有奖励
	RankReward* entry = get_rank_reward_entry(get_id(), 1, 0);
	if (NULL == entry || entry->day_reward().empty())
		return;

	uint32_t index = 1;
	for (sort_iter it = m_rank.begin(); it != m_rank.end(); ++it, ++index)
	{
		entry = get_rank_reward_entry(get_id(), index, 0);
		if (NULL == entry || entry->day_reward().empty())
			return;

		switch (get_owner_type())
		{
			case rot_user:
			{
				// 发邮件
			}
			break;
			case rot_family:
			{
				// 帮派邮件
			}
			break;
			default:
				break;
		}
	}
}

void rank_t::week_reward()
{
	// 防止空遍历，先判断第一名有木有奖励
	RankReward* entry = get_rank_reward_entry(get_id(), 1, 0);
	if (NULL == entry || entry->week_reward().empty())
		return;

	uint32_t index = 1;
	for (sort_iter it = m_rank.begin(); it != m_rank.end(); ++it, ++index)
	{
		entry = get_rank_reward_entry(get_id(), index, 0);
		if (NULL == entry || entry->week_reward().empty())
			return;

		switch (get_owner_type())
		{
			case rot_user:
			{
				// 发邮件
			}
			break;
			case rot_family:
			{
				// 帮派邮件
			}
			break;
			default:
				break;
		}
	}
}

void rank_t::month_reward()
{
	// 防止空遍历，先判断第一名有木有奖励
	RankReward* entry = get_rank_reward_entry(get_id(), 1, 0);
	if (NULL == entry || entry->month_reward().empty())
		return;

	uint32_t index = 1;
	for (sort_iter it = m_rank.begin(); it != m_rank.end(); ++it, ++index)
	{
		entry = get_rank_reward_entry(get_id(), index, 0);
		if (NULL == entry || entry->month_reward().empty())
			return;

		switch (get_owner_type())
		{
			case rot_user:
			{
				// 发邮件
			}
			break;
			case rot_family:
			{
				// 帮派邮件
			}
			break;
			default:
				break;
		}
	}
}


void rank_t::show()
{
	uint32_t index = 1;
	for (sort_iter it = m_rank.begin(); it != m_rank.end() && index <= m_record_num; ++it)
	{
		switch (get_owner_type())
		{
			case rot_user:
			{
				RANK_LOG("排行榜 %u 名次 %u 玩家 %u %s key %u",
					get_id(), index, (it->second).owner, global_user_data_mgr_t::get_name((it->second).owner).c_str(), (it->second).key.key);
			}
			break;
			case rot_family:
			{
				RANK_LOG("排行榜 %u 名次 %u 军团 %u %s key %u",
					get_id(), index, (it->second).owner, "", (it->second).key.key);
			}
			break;
			default:
				break;
		}

		index++;
	}
}

void rank_t::fill_rank_info(proto::common::client_rank_single* add, const rank_record_t& info, const uint32_t index)
{
	if (NULL == add)
		return;
	rank_owner_type owner_type = get_owner_type();
	if (owner_type == rot_user)
	{
		add->set_id(string_util_t::uint64_to_string(info.owner));
		add->set_index(index);
		add->set_name(global_user_data_mgr_t::get_name(info.owner));
		add->set_level(global_user_data_mgr_t::get_user_level(info.owner));
		add->set_country_id(global_user_data_mgr_t::get_user_country_id(info.owner));
		add->set_family_id(string_util_t::uint64_to_string(global_user_data_mgr_t::get_user_family_id(info.owner)));
		add->set_plugin(global_user_data_mgr_t::get_user_hero_plugin(info.owner));
		add->set_key(info.key.key);
		for (auto str : info.custom_data)
		{
			add->add_custom_data(str);
		}
	}
	else if (owner_type == rot_family)
	{
		family_ptr p_family = family_manager_t::get_family(info.owner);
		if (NULL == p_family)
		{
			log_error("NULL == p_family[%lu]", info.owner);
			return;
		}
		add->set_id(string_util_t::uint64_to_string(info.owner));
		add->set_index(index);
		add->set_name(p_family->get_name());
		add->set_level(p_family->get_level());
		add->set_country_id(p_family->get_country_id());
		add->set_key(info.key.key);
		for (auto str : info.custom_data)
		{
			add->add_custom_data(str);
		}
	}
	
}

bool rank_t::get_rank_data(const uint32_t down, const uint32_t up, std::vector< rank_record_t >& list)
{
	uint32_t index = 1;
	for (sort_iter it = m_rank.begin(); it != m_rank.end() && index <= m_record_num; ++it)
	{
		if (it->second.key.key >= down && it->second.key.key <= up)
		{
			list.push_back(it->second);
		}

		index++;
	}

	return true;
}

bool rank_t::get_rank_data(proto::common::client_rank_data* out, uint64_t self_uid, uint32_t num /*= 0*/, uint32_t condition /*= 0*/)
{
	if (NULL == out)
	{
		return false;
	}

	uint32_t query_num = m_record_num;
	if (num != 0)
	{
		query_num = num;
	}

	//bool find_self = false;
	uint32_t index = 1;

	for (sort_iter it = m_rank.begin(); it != m_rank.end() && index <= query_num; ++it)
	{
		if (condition && it->second.key.key < condition)
			continue;

		proto::common::client_rank_single* add = out->add_rank_list();
		if (NULL == add)
			continue;;

		fill_rank_info(add, it->second, index);

		if ((it->second.owner) == self_uid)
		{
			//find_self = true;
			fill_rank_info(out->mutable_self(), it->second, index);
		}

		index++;
	}

	return true;
}

bool rank_t::get_rank_data(proto::common::client_rank_single* out, uint64_t self_uid)
{
	if (NULL == out)
	{
		return false;
	}

	//bool find_self = false;
	uint32_t index = 1;

	for (sort_iter it = m_rank.begin(); it != m_rank.end(); ++it)
	{
		if ((it->second.owner) == self_uid)
		{
			//find_self = true;
			fill_rank_info(out, it->second, index);
			return true;
		}
		index++;
	}

	return false;
}

std::string rank_t::get_rank_name()
{
	return rank_manager_t::get_rank_name(get_id());
}

std::string rank_t::get_owner_name(uint64_t owner)
{
	rank_owner_type owner_type = get_owner_type();
	if (owner_type == rot_user)
	{
		return global_user_data_mgr_t::get_name(owner);
	}
	else
	{
		return "";
	}
}

void rank_t::check_clean()
{
	uint32_t index = 1;
	std::set< uint32_t > _del;
	{
		for (sort_iter it = m_rank.begin(); it != m_rank.end() && index <= m_record_num; ++it)
		{
			_del.insert((it->second).owner);
			index++;
		}
	}


	{
		std::set< uint32_t >::const_iterator it = _del.begin();
		for (; it != _del.end(); ++it)
		{
			delete_by_owner(*it);
		}
	}
}

void rank_t::check_del(const uint64_t owner)
{
	delete_by_owner(owner);
}

uint32_t rank_t::get_rank(uint64_t self_uid)
{
	uint32_t index = 1;
	for (sort_iter it = m_rank.begin(); it != m_rank.end() && index <= m_record_num; ++it) {
		if ((it->second).owner == self_uid) return index;
		index++;
	}
	
	return 0;
}


void rank_t::get_actives(uint32_t rank, std::map<uint32_t, uint32_t>& actives)
{
	uint32_t curr_time = common::time_util_t::now_time();
	uint32_t index = 1;
	for (sort_iter it = m_rank.begin(); it != m_rank.end() && index <= rank; ++it, ++index) {
		uint32_t country_id = global_user_data_mgr_t::get_user_country_id((it->second).owner);
		if (country_id == 0) continue;

		uint32_t update_time = global_user_data_mgr_t::get_update_time((it->second).owner);
		if (time_manager_t::check_previous_day(curr_time, update_time)) {
			actives[country_id] += 1;
		}
	}
}

std::set< uint32_t > rank_manager_t::m_day_rank;		// 日排行，一天清除一次
std::set< uint32_t > rank_manager_t::m_week_rank;		// 周排行，一周清除一次
std::set< uint32_t > rank_manager_t::m_month_rank;	// 月排行，一月清除一次
rank_manager_t::rank_register_map rank_manager_t::m_register;
rank_map rank_manager_t::m_ranks;

rank_manager_t::rank_manager_t()
{
}

rank_manager_t::~rank_manager_t()
{
}

bool rank_manager_t::init()
{
	std::map<uint32_t, RankType*> confs;
	GET_ALL_CONF(RankType, confs);
	for (auto rank_info : confs)
	{
		if (NULL == rank_info.second)
		{
			continue;
		}

		// 注册所有排行的owner类型和名字
		m_register.insert(std::make_pair(rank_info.first, rank_reg((rank_owner_type)rank_info.second->owner_type(), rank_info.second->name())));

		// 每日清除的排行
		if (rank_info.second->clear_type() == clear_type_day)
		{
			m_day_rank.insert(rank_info.first);
		}

		// 每周清除的排行
		if (rank_info.second->clear_type() == clear_type_week)
		{
			m_week_rank.insert(rank_info.first);
		}

		// 每月清除的排行
		if (rank_info.second->clear_type() == clear_type_month)
		{
			m_month_rank.insert(rank_info.first);
		}

		rank_ptr rank(new rank_t());
		if (NULL == rank)
			continue;

		if (!rank->init(rank_info.first, rank_info.second->min_value(), rank_info.second->name().c_str(), (rank_owner_type)rank_info.second->owner_type()))
		{
			return false;
		}
		RANK_LOG("rank[%d] init", rank_info.first);
		m_ranks.insert(std::make_pair(rank_info.first, rank));
	}

	return true;
}

bool rank_manager_t::request_rank(global_user_ptr user, uint32_t type)
{
	RANK_LOG("type[%d]", type);
	rank_map_iter it = m_ranks.find(type);
	if (it != m_ranks.end() && NULL != it->second)
	{
		(it->second)->request_rank(user);
	}

	return true;
}

void rank_manager_t::update_rank(const proto::server::ge_update_rank_notify& msg)
{
	uint64_t owner = msg.owner();
	uint32_t type = msg.rank_type();
	uint32_t key = msg.key();
	rank_map_iter it = m_ranks.find(type);
	if (it == m_ranks.end())
	{
		return;
	}

	rank_record_t info;
	info.key.key = key;
	info.key.time = common::time_util_t::now_time();
	info.owner = owner;
	for (int32_t i = 0; i < msg.custom_data_size(); ++i)
	{
		info.custom_data.push_back(msg.custom_data(i));
	}
	
	if (NULL != it->second)
	{
		(it->second)->update_rank(owner, info);
	}
}

void rank_manager_t::update_rank(
	uint64_t owner,
	uint32_t type,
	uint32_t key)
{
	rank_map_iter it = m_ranks.find(type);
	if (it == m_ranks.end())
	{
		return;
	}

	rank_record_t info;
	info.key.key = key;
	info.key.time = common::time_util_t::now_time();
	info.owner = owner;
	
	if (NULL != it->second)
	{
		(it->second)->update_rank(owner, info);
	}
}

void rank_manager_t::clear_rank(uint32_t type)
{
	rank_map_iter it = m_ranks.find(type);
	if (it != m_ranks.end() && NULL != it->second)
	{
		(it->second)->clear();
	}
}

void rank_manager_t::rank_reward(uint32_t type, uint32_t param)
{
	rank_map_iter it = m_ranks.find(type);
	if (it != m_ranks.end() && NULL != it->second)
	{
		(it->second)->rank_reward(param);
	}
}


void rank_manager_t::rank_reward(uint32_t type, uint32_t index, uint64_t object_uid)
{
	rank_map_iter it = m_ranks.find(type);
	if (it != m_ranks.end() && NULL != it->second)
	{
		(it->second)->rank_reward(index, object_uid);
	}
}

void rank_manager_t::delete_from_rank(uint64_t owner, uint32_t type)
{
	rank_map_iter it = m_ranks.find(type);
	if (it != m_ranks.end() && NULL != it->second)
	{
		(it->second)->delete_from_rank(owner);
	}
}

rank_owner_type rank_manager_t::get_owner_type(uint32_t type)
{
	rank_register_map_iter it = m_register.find(type);
	if (it != m_register.end())
		return it->second.type;

	return rot_none;
}


const std::string rank_manager_t::get_rank_name(uint32_t type)
{
	rank_register_map_iter it = m_register.find(type);
	if (it != m_register.end())
		return it->second.name;

	return "";
}

void rank_manager_t::one_day()
{
	rank_map_iter it = m_ranks.begin();
	for (; it != m_ranks.end(); ++it)
	{
		if (NULL != it->second)
			(it->second)->day_reward();
	}

	std::set< uint32_t >::const_iterator it_d = m_day_rank.begin();
	for (; it_d != m_day_rank.end(); ++it_d)
	{
		rank_map_iter it = m_ranks.find(*it_d);
		if (it != m_ranks.end())
		{
			if (NULL != it->second)
				(it->second)->clear();
		}
	}
}


void rank_manager_t::one_week()
{
	rank_map_iter it = m_ranks.begin();
	for (; it != m_ranks.end(); ++it)
	{
		if (NULL != it->second)
			(it->second)->week_reward();
	}

	std::set< uint32_t >::const_iterator it_d = m_week_rank.begin();
	for (; it_d != m_week_rank.end(); ++it_d)
	{
		rank_map_iter it = m_ranks.find(*it_d);
		if (it != m_ranks.end())
		{
			if (NULL != it->second)
				(it->second)->clear();
		}
	}
}

void rank_manager_t::one_month()
{
	rank_map_iter it = m_ranks.begin();
	for (; it != m_ranks.end(); ++it)
	{
		if (NULL != it->second)
			(it->second)->month_reward();
	}

	std::set< uint32_t >::const_iterator it_d = m_month_rank.begin();
	for (; it_d != m_month_rank.end(); ++it_d)
	{
		rank_map_iter it = m_ranks.find(*it_d);
		if (it != m_ranks.end())
		{
			if (NULL != it->second)
				(it->second)->clear();
		}
	}
}

void rank_manager_t::one_sec()
{
	/// 每秒检测是否需要保存，保证每秒最多只保存一次
	//check_save();
}

void rank_manager_t::check_save(bool immediately)
{
	rank_map_iter it = m_ranks.begin();
	for (; it != m_ranks.end(); ++it)
	{
		if (NULL != it->second)
			(it->second)->check_save(immediately);
	}
}

void rank_manager_t::show_rank(uint32_t type)
{
	rank_map_iter it = m_ranks.find(type);
	if (it != m_ranks.end())
	{
		if (NULL != it->second)
			(it->second)->show();
	}
}

bool rank_manager_t::get_rank_data(uint32_t type, const uint32_t down, const uint32_t up, std::vector< rank_record_t >& list)
{
	rank_map_iter it = m_ranks.find(type);
	if (it != m_ranks.end())
	{
		if (NULL != it->second)
			return (it->second)->get_rank_data(down, up, list);
	}

	return false;
}

bool rank_manager_t::get_rank_data(uint32_t type, proto::common::client_rank_data* out, uint64_t self_uid, uint32_t num /*= 0*/, uint32_t condition /*= 0*/)
{
	if (NULL == out)
	{
		return false;
	}

	rank_map_iter it = m_ranks.find(type);
	if (it != m_ranks.end())
	{
		if (NULL != it->second)
			return (it->second)->get_rank_data(out, self_uid, num, condition);
	}

	return false;
}

bool rank_manager_t::get_rank_data(uint32_t type, proto::common::client_rank_single* out, uint64_t self_uid)
{
	if (NULL == out)
	{
		return false;
	}

	rank_map_iter it = m_ranks.find(type);
	if (it != m_ranks.end())
	{
		if (NULL != it->second)
			return (it->second)->get_rank_data(out, self_uid);
	}

	return false;
}

bool rank_manager_t::request_self_rank(global_user_t* user, uint32_t type)
{
	if (NULL == user)
		return false;

	return true;
}

void rank_manager_t::final()
{
	rank_map_iter it = m_ranks.begin();
	for (; it != m_ranks.end(); ++it)
	{
		if (NULL != it->second)
			(it->second)->final();
	}
}

void rank_manager_t::check_clean(uint32_t type)
{
	rank_map_iter it = m_ranks.find(type);
	if (it != m_ranks.end())
	{
		if (NULL != it->second)
			return (it->second)->check_clean();
	}
}

void rank_manager_t::check_del(uint32_t type, uint32_t id)
{
	rank_map_iter it = m_ranks.find(type);
	if (it != m_ranks.end())
	{
		if (NULL != it->second)
			return (it->second)->check_del(id);
	}
}

void rank_manager_t::beat_criminal_reward(uint64_t self_uid, uint64_t criminal_uid)
{
	proto::common::client_rank_single crs;
	get_rank_data(rank_type_criminal, &crs, criminal_uid);
	rank_reward(rank_type_criminal, crs.index(), self_uid);
	global_user_ptr p_self = global_user_manager_t::get_global_user_by_uid(self_uid);
	global_user_ptr p_criminal = global_user_manager_t::get_global_user_by_uid(criminal_uid);
	if (NULL != p_self && NULL != p_criminal)
	{
		BROADCAST_SYS_NOTICE(sys_notice_enum::sys_notice_10, 0, 0, 0, p_self->get_name().c_str(), crs.index(), p_criminal->get_name().c_str())
	}
	
}

uint32_t rank_manager_t::get_rank_by_type(uint32_t type, uint64_t uid)
{
	rank_map_iter it = m_ranks.find(type);
	if (it == m_ranks.end()) return 0;

	if (NULL != it->second) {
		return (it->second)->get_rank(uid);
	}

	return 0;
}

void rank_manager_t::get_active_user(uint32_t rank, std::map<uint32_t, uint32_t>& actives)
{
	rank_map_iter it = m_ranks.find(common::rank_common::lv_rank_0);
	if (it == m_ranks.end()) return;

	if (NULL != it->second) {
		return (it->second)->get_actives(rank, actives);
	}
}
