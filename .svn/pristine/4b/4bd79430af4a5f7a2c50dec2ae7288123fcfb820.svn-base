#include "general_event_data_manager.hpp"
#include "utility.hpp"
#include "redis_client.hpp"

bool general_event_data_manager_t::load_general_event_data(uint64_t uid, proto::common::general_event_save_data * p_list)
{
	std::string red_list_str = redis_client_t::get_string("role_ex", "general_event_red", uid);
	if (!red_list_str.empty())
	{
		std::vector<std::uint32_t> data_list;
		common::string_util_t::split<std::uint32_t>(red_list_str, data_list, "$");
		for (auto it : data_list)
		{
			p_list->add_red(it);
		}
	}
	
	std::string data_list_str = redis_client_t::get_string("role_ex", "general_event_data", uid);
	if (!data_list_str.empty())
	{
		std::vector<std::uint32_t> data_list;
		common::string_util_t::split<std::uint32_t>(data_list_str, data_list, "$");
		for (auto it : data_list)
		{
			p_list->add_data(it);
		}
	}

	return true;
}

bool general_event_data_manager_t::save_general_event_data(uint64_t uid, const proto::common::general_event_save_data  &list)
{
	std::ostringstream sss1;
	for (int32_t i = 0; i < list.red_size(); ++i)
	{
		if (!sss1.str().empty())
			sss1 << "$";
		sss1 << list.red(i);
	}
	redis_client_t::set_string("role_ex", "general_event_red", uid, sss1.str());

	std::ostringstream sss;
	for (int32_t i = 0; i < list.data_size(); ++i)
	{
		if (!sss.str().empty())
			sss << "$";
		sss << list.data(i);
	}
	redis_client_t::set_string("role_ex", "general_event_data", uid, sss.str());
	return true;
}