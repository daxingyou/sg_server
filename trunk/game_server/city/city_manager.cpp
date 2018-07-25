#include "city_manager.hpp"
#include "main/game_server_fwd.hpp"
#include "main/game_server.hpp"
#include "city.hpp"
#include "config_mgr.h"
#include "tblh/Comprehensive.tbls.h"
#include "tblh/City.tbls.h"
#include "trade/trade_manager.hpp"

city_manager_t::city_map_type city_manager_t::m_city_map;

city_manager_t::city_manager_t()
{
}

city_manager_t::~city_manager_t()
{
	m_city_map.clear();
}

void city_manager_t::close()
{
	m_city_map.clear();
}

city_ptr city_manager_t::get_city(uint32_t city_id)
{
	city_map_type::iterator itr = m_city_map.find(city_id);
	if (itr != m_city_map.end())
	{
		return (itr->second);
	}
	return city_ptr();
}

bool city_manager_t::load_from_center(proto::server::eg_get_city_list_reply &msg)
{
	city_ptr p_city = city_ptr();
	for (int i = 0; i < msg.city_list_size(); ++i)
	{
		const proto::common::city_base_info &city_info = msg.city_list(i);
		p_city = get_city(msg.city_list(i).city_id());
		if(p_city)
			continue;

		p_city.reset(new city_t(city_info.city_id(), 0));
		if (nullptr == p_city)
		{
			log_error("[wys] city_manager_t new city is null");
			return false;
		}
		p_city->set_prosperity_value(city_info.prosperity_value());
		p_city->set_prosperity_level(city_info.prosperity_level());
		p_city->set_hold_value(city_info.city_hold_value());
		m_city_map.insert(std::make_pair(p_city->get_id(), p_city));
	}
	if (!trade_manager_t::init_all_trade())
	{
		log_error("init trade fail!!!");
		return false;
	}
	
	return true;
}

