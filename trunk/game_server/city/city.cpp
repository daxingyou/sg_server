#include "city.hpp"
#include "config_mgr.h"
#include "scene/scene_manager.hpp"
#include "scene/scene.hpp"
#include <vector>
#include "client.pb.h"
#include "role/role_manager.hpp"
#include "role/role.hpp"
#include "monster/monster.hpp"
#include "tblh/Item.tbls.h"
#include "tblh/Trade.tbls.h"
#include "tblh/TradeGoodsType.tbls.h"
#include "tblh/TradeProsperity.tbls.h"
#include "tblh/Comprehensive.tbls.h"
#include "city_manager.hpp"


city_t::city_t(uint32_t city_id, uint32_t country_id) : /*cur_state(proto::common::city_state_attackee),*/ m_capture_country_id(country_id)
{
	m_id = city_id;
	m_country_id = country_id;
}

void city_t::init()
{
	MapTable* scene = GET_CONF(MapTable, m_id);
	if (NULL != scene)
	{
// 		if (scene->get_scene_attr_type() != city_type_normal && scene->get_scene_attr_type() != city_type_near_king)
// 		{
// 			cur_state = proto::common::city_state_invincible;
// 		}
// 		else
// 		{
// 			cur_state = proto::common::city_state_attackee;
// 		}
		m_capture_country_id = m_country_id;

		if (scene->get_scene_attr_type() == city_type_attacker_assemble)
		{
			//cur_state = proto::common::city_state_attacker;
			m_capture_country_id = m_peer_country_id;
		}
	}
}


void city_t::set_prosperity_value(uint32_t prosperity_value)
{
	m_prosperity_value = prosperity_value;

	std::map<uint32_t, TradeProsperity *> confs;
	GET_ALL_CONF(TradeProsperity,confs);
	for (auto& it : confs)
	{
		if (m_prosperity_value < it.second->prosperity())
		{
			set_prosperity_level(it.first);
			TRADE_LOG("update prosperity_level success! new level[%d]", m_prosperity_level);
			break;
		}
	}
}

void city_t::set_prosperity_level(uint32_t prosperity_level)
{
	m_prosperity_level = prosperity_level; 
}

void city_t::set_hold_value(uint32_t hold_value)
{
	m_hold_value = hold_value;
}


