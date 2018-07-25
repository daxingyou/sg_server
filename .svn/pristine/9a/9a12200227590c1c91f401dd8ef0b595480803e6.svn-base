#include "red_point.hpp"
#include "utility.hpp"
#include "cache_key.hpp"
#include "client.pb.h"
#include "role/role_unify_save.hpp"
#include "common/config_mgr.h"
#include <set>

red_point_t::red_point_t(proto::common::RED_POINT_TYPE type, uint64_t role_uid)
	: m_red_point_type(type)
	, m_role_uid(role_uid)
	, m_b_state(true)
{}


void red_point_t::append_to_list(uint32_t val)
{
	if (0 == val)
	{
		set_state();
		return;
	}
		
	if (m_list.empty())
	{
		m_list.append(string_util_t::uint32_to_string(val).c_str());
	}
	else
	{
		m_list.append(":");
		m_list.append(string_util_t::uint32_to_string(val).c_str());
	}
	
}

void red_point_t::append_to_list(std::string& dec)
{
	if (dec.empty())
	{
		set_state();
		return;
	}
		
	if (m_list.empty())
	{
		m_list = dec;
	}
	else
	{
		m_list.append(":");
		m_list.append(dec.c_str());
	}
	
}

void red_point_t::remove_from_list(uint32_t val)
{
	std::set<uint32_t> sets;
	sets.clear();
	slipt_to_set(sets);

	auto iter_set = sets.find(val);
	if (iter_set == sets.end())
	{
		reset_state();
		return;
	}
		

	sets.erase(val);
	m_list.clear();

	for (auto& it : sets )
	{
		append_to_list(it);
	}

	if (sets.empty())
		reset_state();
}

void red_point_t::slipt_to_set(std::set<uint32_t>& set_list)
{
	std::vector<uint32_t> strvec;
	const char* dec = ":";
	string_util_t::split(m_list, strvec, dec);
	if (strvec.empty())
	{
		RED_POINT_LOG("strvec.empty() m_role_uid[%lu] red_point[%d] strlist[%s] not exist!", m_role_uid, m_red_point_type, m_list.c_str());
		return;
	}
	for (auto& it : strvec)
	{
		set_list.insert(it);
	}
}