#include "country_bounty_data_manager.hpp"
#include "redis_client.hpp"
#include "utility.hpp"
#include "log.hpp"
USING_NS_COMMON;
country_bounty_data_manager::country_bounty_data_manager()
{
}

country_bounty_data_manager::~country_bounty_data_manager()
{
}

bool country_bounty_data_manager::save_country_bounty_data_to_redis(uint64_t uid, const proto::common::country_bounty_data& country_bounty)
{
	std::ostringstream ss;
	for (const proto::common::bounty_single& bs : country_bounty.bounty_data())
	{
		if (!ss.str().empty())
		{
			ss << "$";
		}

		ss << bs.bounty_id() << ":" << bs.state();
	}

	redis_client_t::set_string("country_bounty", "country_bounty", uid, ss.str());

	std::ostringstream sss;
	for (auto box : country_bounty.box_list())
	{
		if (box.state() < 2)
		{
			continue;
		}
		if (!sss.str().empty())
		{
			sss << "$";
		}

		sss << box.star();
	}

	redis_client_t::set_string("country_bounty", "star_box", uid, sss.str());
	redis_client_t::set_uint32("country_bounty", "refresh_count", uid, country_bounty.refresh_count());
	redis_client_t::set_uint32("country_bounty", "star_count", uid, country_bounty.star_count());
	redis_client_t::set_uint32("country_bounty", "accept_level", uid, country_bounty.accept_level());
	return true;
}

bool country_bounty_data_manager::load_country_bounty_data_from_redis(uint64_t uid, proto::common::country_bounty_data* p_country_bounty)
{
	if (NULL == p_country_bounty)
	{
		return false;
	}

	std::string bounty_info = redis_client_t::get_string("country_bounty", "country_bounty", uid);
	if (!bounty_info.empty())
	{
		std::vector<std::string> bountys;
		string_util_t::split<std::string>(bounty_info, bountys, "$");
		for (auto bounty : bountys)
		{
			std::vector<std::string> info_vec;
			string_util_t::split<std::string>(bounty, info_vec, ":");
			if (info_vec.size() != 2)
			{
				log_error("load country_bounty fail uid[%lu] data[%s]", uid, bounty.c_str());
				continue;
			}

			proto::common::bounty_single* p_single = p_country_bounty->add_bounty_data();
			
			p_single->set_bounty_id(boost::lexical_cast<uint32_t>(info_vec[0]));
			p_single->set_state(boost::lexical_cast<uint32_t>(info_vec[1]));
		}
	}

	std::string box_info = redis_client_t::get_string("country_bounty", "star_box", uid);
	if (!box_info.empty())
	{
		std::vector<std::string> boxes;
		string_util_t::split<std::string>(box_info, boxes, "$");
		for (auto box : boxes)
		{
			uint32_t star = boost::lexical_cast<uint32_t>(box);
			proto::common::star_box* p_star_box = p_country_bounty->add_box_list();
			p_star_box->set_star(star);
		}
	}
	p_country_bounty->set_refresh_count(redis_client_t::get_uint32("country_bounty", "refresh_count", uid));
	p_country_bounty->set_star_count(redis_client_t::get_uint32("country_bounty", "star_count", uid));
	p_country_bounty->set_accept_level(redis_client_t::get_uint32("country_bounty", "accept_level", uid));
	return true;
}
