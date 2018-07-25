#include "activity_data_manager.hpp"
#include "common/redis_client.hpp"
#include "utility.hpp"
USING_NS_COMMON;
activity_data_manager::activity_data_manager()
{
}

activity_data_manager::~activity_data_manager()
{
}

bool activity_data_manager::save_activity_data_to_redis(uint64_t uid, const proto::common::activities_data& activities_data)
{
	std::ostringstream ss;
	for (const proto::common::activity_single& as : activities_data.activity_data())
	{
		if (!ss.str().empty())
		{
			ss << "$";
		}

		ss << as.activity_id() << ":" << as.count() << ":" << as.last_reset_time();
	}

	redis_client_t::set_string("role_ex", "activity_data", uid, ss.str());

	std::ostringstream sss;
	for (auto box : activities_data.box_list())
	{
		if (box.state() < 2)
		{
			continue;
		}
		if (!sss.str().empty())
		{
			sss << "$";
		}

		sss << box.liveness();
	}

	redis_client_t::set_string("role_ex", "liveness_box", uid, sss.str());
	redis_client_t::set_uint32("role_ex", "liveness", uid, activities_data.liveness());
	return true;
}

bool activity_data_manager::load_activity_data_from_redis(uint64_t uid, proto::common::activities_data* p_activities_data)
{
	if (NULL == p_activities_data)
	{
		return false;
	}

	std::string activity_info = redis_client_t::get_string("role_ex", "activity_data", uid);
	if (!activity_info.empty())
	{
		std::vector<std::string> activities;
		string_util_t::split<std::string>(activity_info, activities, "$");
		for (auto activity : activities)
		{
			std::vector<std::string> info_vec;
			string_util_t::split<std::string>(activity, info_vec, ":");
			if (info_vec.size() != 3)
			{
				log_error("load activity fail uid[%lu] data[%s]", uid, activity.c_str());
				continue;
			}

			proto::common::activity_single* p_single = p_activities_data->add_activity_data();
			
			p_single->set_activity_id(boost::lexical_cast<uint32_t>(info_vec[0]));
			p_single->set_count(boost::lexical_cast<uint32_t>(info_vec[1]));
			p_single->set_last_reset_time(boost::lexical_cast<uint32_t>(info_vec[2]));
		}
	}

	std::string box_info = redis_client_t::get_string("role_ex", "liveness_box", uid);
	if (!box_info.empty())
	{
		std::vector<std::string> boxes;
		string_util_t::split<std::string>(box_info, boxes, "$");
		for (auto box : boxes)
		{
			uint32_t liveness = boost::lexical_cast<uint32_t>(box);
			proto::common::liveness_box* p_liveness_box = p_activities_data->add_box_list();
			p_liveness_box->set_liveness(liveness);
		}
	}
	p_activities_data->set_liveness(redis_client_t::get_uint32("role_ex", "liveness", uid));
	return true;
}
