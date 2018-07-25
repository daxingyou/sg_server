#include "luckydraw_data_manager.hpp"
#include "common/redis_client.hpp"
#include "utility.hpp"
USING_NS_COMMON;
luckydraw_data_manager::luckydraw_data_manager()
{
}

luckydraw_data_manager::~luckydraw_data_manager()
{
}

//count  type$count;type$count
//seq type:level$level;type:level$level;
bool luckydraw_data_manager::save_luckydraw_data_to_redis(uint64_t uid, const proto::common::luckydraw_data& ld)
{
	std::ostringstream sss_count;
	std::ostringstream sss_seq;

	for (int i = 0; i < ld.lucky_draw_list_size(); ++i)
	{
		const proto::common::luckydraw_info &tmp_luckydraw_info = ld.lucky_draw_list(i);
		uint32_t type = tmp_luckydraw_info.type();
		uint32_t count = tmp_luckydraw_info.senior_count();
		if (!sss_count.str().empty())
		{
			sss_count << ";";
		}
		sss_count << type << "$" << count;

		if (!sss_seq.str().empty())
		{
			sss_seq << ";";
		}

		sss_seq << type << ":";
		auto &seq_list = tmp_luckydraw_info.senior_seq();
		for (int n = 0; n < seq_list.size(); ++n)
		{
			if (!sss_seq.str().empty())
			{
				sss_seq << "$";
			}
			sss_seq << tmp_luckydraw_info.senior_seq(n);
		}
	}
	redis_client_t::set_string("role_ex", "luckydraw_senior_count", uid, sss_count.str());
	redis_client_t::set_string("role_ex", "luckydraw_senior_seq", uid, sss_seq.str());
	return true;
}

bool luckydraw_data_manager::load_luckydraw_data_from_redis(uint64_t uid, proto::common::luckydraw_data* p_data)
{
	if (NULL == p_data)
	{
		return false;
	}

	std::map<uint32_t, uint32_t> count_map;
	std::string count_info = redis_client_t::get_string("role_ex", "luckydraw_senior_count", uid);
	if (!count_info.empty())
	{
		std::vector<std::string> count_list;
		string_util_t::split<std::string>(count_info, count_list, ";");
		for ( auto count_str : count_list )
		{
			std::vector<uint32_t> counts;
			string_util_t::split<uint32_t>(count_str, counts, "$");
			if (counts.size() != 2)
			{
				//兼容旧数据
				log_error("user load data count_info type error[%lu]", uid);
				continue;
			}
			count_map[counts[0]] = counts[1];
		}
	}

	std::string seq_info = redis_client_t::get_string("role_ex", "luckydraw_senior_seq", uid);
	if (!seq_info.empty())
	{
		std::vector<std::string> seq_list;
		string_util_t::split<std::string>(seq_info, seq_list, ";");
		for (auto seq : seq_list)
		{
			proto::common::luckydraw_info *tmp_luckydraw_info_ptr = p_data->add_lucky_draw_list();
			std::vector<std::string> seq_info;
			string_util_t::split<std::string>(seq, seq_info , ":");
			if (seq_info.size() != 2)
			{
				//兼容旧数据
				log_error("user load data seq_info type error[%lu]", uid);
				continue;
			}
			uint32_t type = boost::lexical_cast<uint32_t>(seq_info[0]); 
			tmp_luckydraw_info_ptr->set_type(type);
			tmp_luckydraw_info_ptr->set_senior_count( count_map[type] );	//默认没有就给0好了

			std::vector<uint32_t> levels;
			string_util_t::split<uint32_t>(seq_info[1], levels, "$");
			for (auto level : levels)
			{
				tmp_luckydraw_info_ptr->add_senior_seq(level);
			}
		}
	}
	return true;
}
