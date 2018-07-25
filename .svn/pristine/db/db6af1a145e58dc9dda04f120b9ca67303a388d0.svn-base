#ifndef __CENTER_CITY_MANAGER_HPP__
#define __CENTER_CITY_MANAGER_HPP__

/*
author: wys
time:2016.11.15
desc:game city manager
*/

#include "city.hpp"
#include "protos_fwd.hpp"
#include <map>
#include <boost/system/error_code.hpp>
#include <boost/asio/deadline_timer.hpp>


class city_manager_t
{
public:
	typedef std::map<uint32_t, city_ptr> city_map_type;
public:
	city_manager_t();
	~city_manager_t();

	static city_ptr		get_city(uint32_t city_id);
	static const city_map_type	get_all_city() { return m_city_map; }
	// 从center初始化城市列表
	static bool			load_from_center(proto::server::eg_get_city_list_reply &msg);
	static void			close();
private:
	// ------------wys add 2016.11.16--------------------
	static city_map_type m_city_map;

};

#endif