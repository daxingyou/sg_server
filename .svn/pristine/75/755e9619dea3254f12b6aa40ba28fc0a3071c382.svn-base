#include "achieve.hpp"
#include "utility.hpp"
#include "cache_key.hpp"
#include "client.pb.h"
#include "role/role_unify_save.hpp"
#include "common/config_mgr.h"
#include "tblh/Comprehensive.tbls.h"

achieve_t::achieve_t(uint64_t uid, uint32_t achieve_id,uint64_t role_id)
	: m_tid(achieve_id)
	, m_uid(uid)
	, m_role_uid(role_id)
{
	m_key = cache_key_t::create(m_uid, cache_name::achieve);
}

void achieve_t::load_data(uint64_t role_uid,const proto::common::achieve_data_single& achieve_data_single)
{
	m_uid = string_util_t::string_to_uint64(achieve_data_single.uid());
	m_key = cache_key_t::create(m_uid, cache_name::achieve);
	m_tid = achieve_data_single.tid();
	m_level = achieve_data_single.level();
	m_count = achieve_data_single.count();
	m_finish_info_list = achieve_data_single.finish_info_list();
	m_role_uid = role_uid;
}

void achieve_t::peek_data(proto::common::achieve_data_single* p_achieve_data_single,bool is_client)
{
	if (NULL == p_achieve_data_single)
	{
		log_error("NULL == p_achieve_info_single m_role_uid[%lu]",m_role_uid);
		return;
	}
	//common
	{
		p_achieve_data_single->set_tid(m_tid);
		p_achieve_data_single->set_level(m_level);
		p_achieve_data_single->set_count(m_count);
	}
	
	if (is_client)
	{
		get_achieve_finish_info(p_achieve_data_single->mutable_finish_list());
	}
	else
	{
		p_achieve_data_single->set_uid(string_util_t::uint64_to_string(m_uid));
		p_achieve_data_single->set_finish_info_list(m_finish_info_list);
		/*ACHIEVE_LOG("m_role_uid[%lu] achieve_uid[%lu] achieve_id[%d] achieve_level[%d] m_finish_info_list[%s]",
			m_role_uid, m_uid,m_tid,m_level, m_finish_info_list.c_str());*/
	}
}

void achieve_t::save_self(uint64_t role_uid)
{
	proto::common::achieve_data_single single;
	peek_data(&single, false);
	role_unify_save::add_task(role_uid, m_key, single);
}


void achieve_t::get_achieve_finish_info(proto::common::achieve_finish_info* p_achieve_finish_info)
{
	if (NULL == p_achieve_finish_info)
	{
		log_error("NULL == p_achieve_finish_info m_role_uid[%lu] achieve_id[%d] achieve_level[%d]",m_role_uid,m_tid,m_level);
		return;
	}

	if (m_finish_info_list.empty())
	{
		ACHIEVE_LOG("no achieve finish role_uid[%lu] achieve id[%d] achieve level[%d]",m_role_uid,m_tid,m_level);
		return;
	}

	std::vector<proto::common::achieve_finish_info_single> vec;
	split_achieve_finish_info(m_finish_info_list, vec);
	if (vec.empty())
	{
		return;
	}
	for (auto& it : vec)
	{
		proto::common::achieve_finish_info_single* p_single = p_achieve_finish_info->add_achieve_finish_info_list();
		p_single->CopyFrom(it);
	}
}

void achieve_t::split_achieve_finish_info(const std::string &str, std::vector<proto::common::achieve_finish_info_single>& list)
{
	if (str.empty())
	{
		log_error("str.empty() m_role_uid[%lu]",m_role_uid);
		return;
	}

	list.clear();
	std::vector<std::string> strvec;
	const char* dec = "|";
	string_util_t::split(str, strvec, dec);
	if (strvec.empty())
	{
		log_error("strvec.empty() m_role_uid[%lu] achieve_id[%d] achieve_level[%d] str[%s]",m_role_uid,m_tid,m_level,str.c_str());
		return;
	}
	
	const uint32_t size = strvec.size();
	for (uint32_t i = 0 ;i < size ; ++i)
	{
		std::string::size_type _pos = strvec[i].find_first_of(':');
		if(_pos != std::string::npos)
		{
			uint32_t level = string_util_t::string_to_uint32(strvec[i].substr(0, _pos));
			uint32_t time = string_util_t::string_to_uint32(strvec[i].substr(_pos + 1));
			proto::common::achieve_finish_info_single single;
			single.set_level(level);
			single.set_time(time);
			list.push_back(single);
			ACHIEVE_LOG("uid[%lu],level[%d] time[%d]",m_role_uid,level,time);
		}
	}
}



Achievelist* achieve_t::get_config(uint32_t level)
{
	if (0 == level)
	{
		++level;
	}
	Achievelist* p_config = GET_CONF(Achievelist, m_tid + level);
	if (NULL == p_config)
	{
		log_error("NULL == p_config role_uid[%lu] achieve_id[%d] achieve_level[%d]",m_role_uid, m_tid, level);
		return NULL;
	}
	return p_config;
}

bool achieve_t::add_level()
{
	Achievelist* p_config = get_config(m_level + 1);
	if (NULL == p_config)
	{
		log_error("NULL == p_config role_uid[%lu] achieve_id[%d] achieve_level[%d] already is max level", m_role_uid, m_tid, m_level);
		return false;
	}

	auto MAX_LEVEL_CONF = GET_CONF(Comprehensive, comprehensive_common::ACHIEVE_MAX_LEVEL);
	const uint32_t max_level = GET_COMPREHENSIVE_VALUE_1(MAX_LEVEL_CONF);

	const uint32_t old_level = m_level;
	++m_level;
	if (max_level < m_level)
	{
		m_level = max_level;
		ACHIEVE_LOG("role_uid[%lu],acheive_id[%d] achieve_level[%d]  already max ",m_role_uid,m_tid,m_level);
	}
	//¸Ä±äÖµ
	const uint32_t up_level = std::abs(m_level - old_level);
	ACHIEVE_LOG("role[%lu] up  achieve[%d] level old_level/new_level[%d/%d]", m_role_uid,m_tid,old_level, m_level);

	if (up_level != 0)
	{
		if (m_finish_info_list.empty())
		{
			m_finish_info_list.append(string_util_t::uint32_to_string(m_level).c_str());
			m_finish_info_list.append(":");
			m_finish_info_list.append(string_util_t::uint32_to_string(common::time_util_t::now_time()).c_str());
		}
		else
		{
			m_finish_info_list.append("|");
			m_finish_info_list.append(string_util_t::uint32_to_string(m_level).c_str());
			m_finish_info_list.append(":");
			m_finish_info_list.append(string_util_t::uint32_to_string(common::time_util_t::now_time()).c_str());
		}

		/*add_achieve_log(old_count, add_count, old_level, 1, type, log_enum::source_type_achieve_single_finish);*/
		ACHIEVE_LOG("role[%lu] up achieve[%d] level success  m_finish_info_list[%s] cur_level[%d]", m_role_uid,m_tid, m_finish_info_list.c_str(), m_level);
		return true;
	}
	ACHIEVE_LOG("role[%lu] up achieve[%d] level fail  m_finish_info_list[%s] cur_level[%d]", m_role_uid, m_tid, m_finish_info_list.c_str(), m_level);
	return false;
}


