#ifndef __BUFF_ELEMENT_MANAGER_HPP__
#define __BUFF_ELEMENT_MANAGER_HPP__

#include <map>
#include "common/macros.hpp"
#include "combat.hpp"

class buff_element_manager_t
{
public:
	buff_element_manager_t();
	~buff_element_manager_t();

public:
	int get_buff_attr(uint32_t type);
	int get_buff_attr_percent(uint32_t type);

	void inc_element_value(const uint32_t type, const int value, const int chg_type = 0);
	void dec_element_value(const uint32_t type, const int value, const int chg_type = 0);
	

	void clear_all_element();

protected:
	void inc_element_value_addi(const uint32_t type, const int value);
	void dec_element_value_addi(const uint32_t type, const int value);

	void set_element_value(const uint32_t type, const int value);
	int get_element_value(const uint32_t type) const;

	void clear_element_value(const uint32_t type);
	void remove_element_value(const uint32_t type);
	
	void inc_element_value_percent(const uint32_t type, const int value);
	void dec_element_value_percent(const uint32_t type, const int value);

	void set_element_value_percent(const uint32_t type, const int value);
	uint32_t get_element_value_percent(const uint32_t type) const;

	void clear_element_value_percent(const uint32_t type);
	void remove_element_value_percent(const uint32_t type);
private:
	attr_map m_element_attr;
	attr_map m_element_attr_percent;
};

#endif//__BUFF_ELEMENT_MANAGER_HPP__
