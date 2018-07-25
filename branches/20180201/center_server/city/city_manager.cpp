#include "city_manager.hpp"
#include "main/center_server_fwd.hpp"
#include "main/center_server.hpp"
#include "city.hpp"
#include "tblh/MapTable.tbls.h"
#include "common/config_mgr.h"
#include "op_cmd.hpp"
#include "cmd_desc.hpp"
#include "common/redis_client.hpp"
#include "tblh/Comprehensive.tbls.h"

USING_NS_NETWORK;
USING_NS_COMMON;

city_manager_t::city_map_type city_manager_t::m_city_map;

city_manager_t::city_manager_t()
{

}

city_manager_t::~city_manager_t()
{
	m_city_map.clear();
}

bool city_manager_t::init()
{
	bool bret = true;
	bret &= load_from_config();
	bret &= load_all_from_db();
	return bret;
}

city_ptr city_manager_t::get_city(uint32_t city_id)
{
	city_map_type::iterator it = m_city_map.find(city_id);
	if (it != m_city_map.end())
	{
		return (it->second);
	}
	return city_ptr();
}


bool city_manager_t::add_city_prosperity_value(uint32_t city_id, uint32_t add_prosperity_value)
{
	city_ptr p_city = get_city(city_id);
	if (nullptr == p_city)
	{
		log_error("[wys] add_city_prosperity_value not find city[%d]", city_id);
		return false;
	}
	uint32_t new_value = p_city->get_prosperity_value() + add_prosperity_value;
	p_city->set_prosperity_value(new_value);
	syn_to_game_server(p_city);
	return true;
}


void city_manager_t::peek_city_list(proto::server::eg_get_city_list_reply& msg)
{

	proto::common::city_base_info *p_city_single = NULL;

	for (auto conf : m_city_map)
	{
		p_city_single = msg.add_city_list();
		if (NULL == p_city_single)
		{
			log_error("city p_city_single NULL");
			return;
		}
		p_city_single->set_city_id(conf.second->get_city_id());
		p_city_single->set_prosperity_value(conf.second->get_prosperity_value());
		p_city_single->set_city_hold_value(conf.second->get_hold_value());
		p_city_single->set_hold_country_id(conf.second->get_hold_country_id());
		p_city_single->set_prosperity_level(conf.second->get_prosperity_level());
	}
}


bool city_manager_t::handle_ge_get_city_list_notify(const network::tcp_socket_ptr& s)
{
	proto::server::eg_get_city_list_reply reply;
	city_manager_t::peek_city_list(reply);

	env::server->send_msg_to_game(s, op_cmd::eg_get_city_list_reply, 0, reply);
	return true;
}

bool city_manager_t::load_from_config()
{
	auto conf_init_prosperity = GET_CONF(Comprehensive, comprehensive_common::trade_prosperity_init);
	uint32_t init_prosperity = GET_COMPREHENSIVE_VALUE_1(conf_init_prosperity);
	if (0 == init_prosperity)
	{
		log_error("init city_manager failed! init_prosperity[%d]", init_prosperity);
		return false;
	}

	city_ptr p_city = city_ptr();
	std::map<uint32_t, MapTable*> confs;
	GET_ALL_CONF(MapTable, confs);

	for (auto& conf : confs)
	{
		if (NULL == conf.second)
		{
			log_error("scene[%d] not config", conf.first);
			continue;
		}
		if (conf.second->city_attr() != 1)
		{
			continue;
		}
		uint32_t id = conf.first;

		p_city.reset(new city_t(id,conf.second->get_scene_attr_type()));
		if (nullptr == p_city)
		{
			log_error("city_manager_t new city is null");
			return false;
		}

		m_city_map.insert(std::make_pair(p_city->get_city_id(), p_city));
		p_city->set_prosperity_value(init_prosperity);
	}
	return true;
}

void city_manager_t::syn_to_game_server(city_ptr p_city)
{
	if (nullptr == p_city)
		return;
	proto::server::eg_city_base_info_notify msg;
	msg.mutable_city_info()->set_city_id(p_city->get_city_id());
	msg.mutable_city_info()->set_city_hold_value(p_city->get_hold_value());
	msg.mutable_city_info()->set_prosperity_value(p_city->get_prosperity_value());
	env::server->send_msg_to_all_games(op_cmd::eg_city_base_info_notify, 0, msg);
}

bool city_manager_t::load_all_from_db()
{
	std::vector<uint32_t> city_ids;
	redis_client_t::get_all_keys<uint64_t, std::vector<uint32_t>>("city", city_ids);

	for (auto city_id : city_ids)
	{
		city_ptr p_city = get_city(city_id);
		if (NULL == p_city)
		{
			log_warn("city_manager not find city[%d]", city_id);
			continue;
		}
		p_city->load_from_db();
	}

	return true;
}

