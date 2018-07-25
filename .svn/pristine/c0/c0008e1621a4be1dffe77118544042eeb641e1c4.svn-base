#include "luckydraw.hpp"
#include "main/game_server_fwd.hpp"
#include "main/game_server.hpp"
#include "cache_key.hpp"
#include "role/role_unify_save.hpp"
#include "tblh/Comprehensive.tbls.h"
#include "config/config_manager.hpp"
#include "tblh/LuckydrawSeq.tbls.h"
#include "common/utility.hpp"
#include "luckydraw_manager.hpp"
#include "general_event/general_info_manager.hpp"

USING_NS_COMMON;

std::map<uint32_t, seq_list> luckydraw_t::s_senior_seq;

luckydraw_t::luckydraw_t(uint64_t uid)
	: m_owner(uid)
{
	m_key = cache_key_t::create(m_owner, cache_name::luckydraw);
}

luckydraw_t::~luckydraw_t()
{

}

void luckydraw_t::load_data(const proto::common::luckydraw_data& ld)
{
	m_senior_seq.clear();
	m_senior_count.clear();

	init_seq();	//初始化一次，可以让策划自由增加
	m_lucky = ld.lucky();
	m_lucky_ex = ld.lucky_ex();
	for (int i = 0; i < ld.lucky_draw_list_size(); ++i)
	{
		const proto::common::luckydraw_info &tmp_luckydraw_info =  ld.lucky_draw_list(i);
		uint32_t type = tmp_luckydraw_info.type();
		uint32_t count = tmp_luckydraw_info.senior_count();
		uint32_t today_count = tmp_luckydraw_info.today_count();

		std::vector<uint32_t> tmp_seq_list;
		auto &seq_list = tmp_luckydraw_info.senior_seq();
		for (int n = 0; n < seq_list.size(); ++n)
		{
			tmp_seq_list.push_back(tmp_luckydraw_info.senior_seq(n) );
		}
		m_senior_count[type] = count;
		m_today_count[type] = today_count;
		m_senior_seq[type] = tmp_seq_list;
	}
}

void luckydraw_t::peek_data(proto::common::luckydraw_data* p_data)
{
	if (NULL == p_data)
	{
		log_error("NULL == p_data");
		return;
	}
	p_data->set_lucky(m_lucky);
	p_data->set_lucky_ex(m_lucky_ex);
	for (auto iter : m_senior_seq)
	{
		proto::common::luckydraw_info *tmp_luckydraw_info_ptr =  p_data->add_lucky_draw_list();
		tmp_luckydraw_info_ptr->set_type(iter.first);
		tmp_luckydraw_info_ptr->set_senior_count(get_senior_count(iter.first));
		tmp_luckydraw_info_ptr->set_today_count(get_today_count(iter.first));

		auto tmp_seq_list = iter.second;
		for (auto level : tmp_seq_list)
		{
			tmp_luckydraw_info_ptr->add_senior_seq(level);
		}
	}
}

void luckydraw_t::save_self()
{
	proto::common::luckydraw_data luckydraw_data;
	peek_data(&luckydraw_data);
	role_unify_save::add_task(m_owner, m_key, luckydraw_data);
}


void luckydraw_t::peek_client_data(proto::common::luckydraw_client_data* p_data)
{
	if (NULL == p_data)
	{
		log_error("NULL == p_data");
		return;
	}
	for (uint32_t i = proto::common::luckydraw_type_junior; i <= proto::common::luckydraw_type_general; i++)
	{
		proto::common::luckydraw_client_single* p_single = p_data->add_client_data();
		p_single->set_type(i);
		uint32_t today_count = 0;
		if (luckydraw_manager_t::is_today_limit() && luckydraw_manager_t::get_today_draw_limit_number() > get_today_count(i))
		{
			today_count = luckydraw_manager_t::get_today_draw_limit_number() - get_today_count(i);
		}
		p_single->set_today_count(today_count);
		p_single->set_card_id(luckydraw_manager_t::get_card_id(i));
	}
	p_data->set_remain_time(general_info_manager_t::get_general_card_remain_time());
	p_data->set_lucky(m_lucky);
	p_data->set_lucky_ex(m_lucky_ex);
	if (luckydraw_manager_t::is_today_limit())
	{
		p_data->set_today_count(get_today_count());
		p_data->set_today_count_limit(luckydraw_manager_t::get_today_draw_total_limit_number());
	}
}

uint32_t luckydraw_t::get_senior_level(uint32_t type, uint32_t index)
{
	auto iter_f = m_senior_seq.find(type);
	if (iter_f == m_senior_seq.end())
	{
		log_error("type[%d] error", type);
		return 0;
	}
	
	auto &senior_seq = iter_f->second;
	if (index >= senior_seq.size())
	{
		log_error("index[%d] error", index);
		return 0;
	}
	return senior_seq[index];
}


void luckydraw_t::use_lucky()
{
	//log_warn("before use[%d:%d]", m_lucky, m_lucky_ex);
	uint32_t lucky_max = get_lucky_max();
	if (m_lucky_ex >= lucky_max)
	{
		m_lucky_ex -= lucky_max;
	}
	else
	{
		lucky_max -= m_lucky_ex;
		m_lucky_ex = 0;
		if (m_lucky >= lucky_max)
		{
			m_lucky -= lucky_max;
		}
		else
		{
			m_lucky = 0;
		}
	}
	//log_warn("after use[%d:%d]", m_lucky, m_lucky_ex);
}

void luckydraw_t::init_seq()
{
	m_senior_seq.clear();
	for (auto iter = s_senior_seq.begin(); iter != s_senior_seq.end(); ++iter)
	{
		auto tmp_seq_list = iter->second;
		std::vector<uint32_t> tmp_list;
		tmp_list.assign(tmp_seq_list.begin(), tmp_seq_list.end());
		std::random_shuffle(tmp_list.begin(), tmp_list.end());
		m_senior_seq.insert(std::make_pair( iter->first, tmp_list));
	}
}

void luckydraw_t::make_seq(uint32_t type)
{
	auto s_iter_f = s_senior_seq.find(type);
	if (s_iter_f == s_senior_seq.end())
	{
		log_error("user[%lu], make_seq_type[%u] not found", m_owner, type);
		return;
	}
	auto tmp_seq_list = s_iter_f->second;

	auto m_iter_f = m_senior_seq.find(type);
	if (m_iter_f == m_senior_seq.end())
	{
		std::vector<uint32_t> tmp_list;
		tmp_list.assign(tmp_seq_list.begin(), tmp_seq_list.end());
		std::random_shuffle(tmp_list.begin(), tmp_list.end());
		m_senior_seq.insert(std::make_pair( type, tmp_list) );
	}
	else 
	{
		auto &tmp_list = m_iter_f->second;
		tmp_list.clear();
		tmp_list.assign(tmp_seq_list.begin(), tmp_seq_list.end());
		std::random_shuffle(tmp_list.begin(), tmp_list.end());
	}
}


void luckydraw_t::one_day()
{
	m_today_count.clear();
}

uint32_t luckydraw_t::get_senoir_seq(uint32_t type)
{
	auto iter_f = s_senior_seq.find(type);
	if (iter_f == s_senior_seq.end())
	{
		return 0;
	}
	auto &it = iter_f->second;
	return it.size();
}

bool luckydraw_t::init_senoir_seq()
{
	s_senior_seq.clear();
	
	std::map<uint32_t, LuckydrawSeq*> luckydraw_seq_map;
	GET_ALL_CONF(LuckydrawSeq, luckydraw_seq_map);
	for (auto it : luckydraw_seq_map)
	{
		auto p_conf = it.second;
		if (NULL == p_conf)
			continue;
		
		std::map<uint32_t, uint32_t> senior_seq_map;
		if (!string_util_t::ParseIdcnt(p_conf->seq().c_str(), senior_seq_map, ';', ','))
		{
			log_error("senior_seq_str ParseIdcnt error");
			return false;
		}
	
		std::vector<uint32_t> tmp_seq_list;
		
		for (auto iter : senior_seq_map)
		{
			uint32_t level = iter.first;
			uint32_t count = iter.second;
			tmp_seq_list.insert(tmp_seq_list.end(), count, level);
		}
		s_senior_seq.insert(std::make_pair(it.first, tmp_seq_list));
	}

	if (s_senior_seq.empty())
	{
		log_error("s_senior_seq is empty");
		return false;
	}
	return true;
}


uint32_t luckydraw_t::get_senior_count(uint32_t type)
{
	auto iter_f = m_senior_count.find(type);
	if (iter_f == m_senior_count.end())
	{
		return 0;
	}

	return iter_f->second;
}

void luckydraw_t::set_senior_count(uint32_t type, uint32_t val)
{
	auto iter_f = m_senior_count.find(type);
	if (iter_f == m_senior_count.end())
	{
		m_senior_count[type] = 1;
		return;
	}
	m_senior_count[type] = val;
}

uint32_t luckydraw_t::get_today_count(uint32_t type)
{
	auto iter_f = m_today_count.find(type);
	if (iter_f == m_today_count.end())
	{
		return 0;
	}

	return iter_f->second;
}


uint32_t luckydraw_t::get_today_count()
{
	uint32_t count = 0;
	for (auto it : m_today_count)
	{
		count += it.second;
	}
	return count;
}

void luckydraw_t::set_today_count(uint32_t type, uint32_t val)
{
	auto iter_f = m_today_count.find(type);
	if (iter_f == m_today_count.end())
	{
		m_today_count[type] = 1;
		return;
	}
	m_today_count[type] = val;
}

uint32_t luckydraw_t::get_luckydraw_pool_size(uint32_t type)
{
	auto iter_f = s_senior_seq.find(type);
	if (iter_f == s_senior_seq.end())
	{
		log_error("luckydraw_pool_size is 0[%lu], type[%u]", m_owner, type);
		return 1;
	}
	
	auto &tmp_list = iter_f->second;
	return tmp_list.size();
}

bool luckydraw_t::lucky_full()
{
	return m_lucky + m_lucky_ex >= get_lucky_max();
}

uint32_t luckydraw_t::get_lucky_max()
{
	auto lucky_max = GET_CONF(Comprehensive, comprehensive_common::max_fate);
	uint32_t val = GET_COMPREHENSIVE_VALUE_1(lucky_max);
	return val;
}
