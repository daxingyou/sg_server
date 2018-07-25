#include "city.hpp"
#include "redis_client.hpp"
#include "tblh/TradeProsperity.tbls.h"
#include "config_mgr.h"

city_t::city_t(uint32_t city_id,uint32_t city_type)
	: m_city_id(city_id)
	, m_type(city_type)
{

}
city_t::~city_t()
{

}

void city_t::set_prosperity_value(uint32_t prosperity_value)
{
	m_prosperity_value = prosperity_value;
	redis_client_t::set_uint32("city", "prosperity_value", m_city_id, m_prosperity_value);
	std::map<uint32_t, TradeProsperity*> confs;
	GET_ALL_CONF(TradeProsperity, confs);
	for (auto& it : confs)
	{
		if (!it.second)
		{
			log_error("TradeProsperity table config error!!!");
			return;
		}
		if (m_prosperity_value < it.second->prosperity())
		{
			set_prosperity_level(it.second->level());
			break;
		}
	}
}

void city_t::set_hold_value(uint32_t hold_value)
{
	m_hold_value = hold_value;
	redis_client_t::set_uint32("city", "hold_value", m_city_id, m_hold_value);
}

void city_t::set_hold_country_id(uint32_t hold_country_id)
{
	m_hold_country_id = hold_country_id;
	redis_client_t::set_uint32("city", "hold_country", m_city_id, m_hold_country_id);
}


void city_t::set_prosperity_level(uint32_t prosperity_level)
{
	m_prosperity_level = prosperity_level;
}

void city_t::load_from_db()
{
	uint32_t prosperity_value = redis_client_t::get_uint32("city", "prosperity_value", m_city_id);
	uint32_t hold_value = redis_client_t::get_uint32("city", "hold_value", m_city_id);
	uint32_t hold_country_id = redis_client_t::get_uint32("city", "hold_country", m_city_id);

	set_prosperity_value(prosperity_value);
	set_hold_value(hold_value);
	set_hold_country_id(hold_country_id);
}
