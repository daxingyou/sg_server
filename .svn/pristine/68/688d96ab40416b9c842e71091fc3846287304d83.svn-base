#ifndef __CENTER_CITY_HPP__
#define __CENTER_CITY_HPP__
/*
author: wys
time:2016.11.14
desc:center city
*/
#include "protos_fwd.hpp"
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

class city_t : public boost::enable_shared_from_this<city_t>
{
public:
	city_t(uint32_t city_id,uint32_t city_type);
	~city_t();

	uint32_t get_city_id() { return m_city_id; }
	uint32_t get_prosperity_value() { return m_prosperity_value; }
	uint32_t get_hold_value() { return m_hold_value; }
	uint32_t get_hold_country_id() { return m_hold_country_id; }
	uint32_t get_prosperity_level() { return m_prosperity_level; }

	void set_prosperity_value(uint32_t prosperity_value);
	void set_hold_value(uint32_t hold_value);
	void set_hold_country_id(uint32_t hold_country_id);
	void set_prosperity_level(uint32_t prosperity_level);

	void	load_from_db();
private:
	uint32_t m_city_id = 0;				///城市tid
	uint32_t m_type = 0;				///城市类型
	uint32_t m_hold_country_id = 0;		///所属国家
	uint32_t m_prosperity_value = 0;	///繁荣度
	uint32_t m_prosperity_level = 1;	///繁荣度等级
	uint32_t m_hold_value = 0;			///占有度
};
typedef boost::shared_ptr<city_t> city_ptr;

#endif