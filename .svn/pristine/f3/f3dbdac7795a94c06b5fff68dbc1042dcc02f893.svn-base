#include "buff_element_manager.hpp"
#include "common/log.hpp"

buff_element_manager_t::buff_element_manager_t()
{
}

buff_element_manager_t::~buff_element_manager_t()
{
}

void buff_element_manager_t::set_element_value(const uint32_t type, const int value)
{
	attr_map::iterator it = m_element_attr.find(type);
	if (it == m_element_attr.end())
	{
		m_element_attr.insert(std::make_pair(type, value));
	}
	else
	{
		it->second = value;
	}
}

int buff_element_manager_t::get_element_value(const uint32_t type) const
{
	attr_map::const_iterator it = m_element_attr.find(type);
	if (it == m_element_attr.end())
	{
		return 0;
	}

	return it->second;
}

void buff_element_manager_t::clear_element_value(const uint32_t type)
{
	attr_map::iterator it = m_element_attr.find(type);
	if (it != m_element_attr.end())
	{
		it->second = 0;
	}
}

void buff_element_manager_t::remove_element_value(const uint32_t type)
{
	m_element_attr.erase(type);
}

void buff_element_manager_t::inc_element_value_addi(const uint32_t type, const int value)
{
	attr_map::iterator it = m_element_attr.find(type);
	if (it == m_element_attr.end())
	{
		m_element_attr.insert(std::make_pair(type, value));
	}
	else
	{
		it->second += value;
	}
}

void buff_element_manager_t::inc_element_value(const uint32_t type, const int value, const int chg_type)
{
	if (chg_type == 1)
	{
		inc_element_value_percent(type, value);
	}
	else
	{
		inc_element_value_addi(type, value);
	}
}

void buff_element_manager_t::dec_element_value_addi(const uint32_t type, const int value)
{
	attr_map::iterator it = m_element_attr.find(type);
	if (it != m_element_attr.end())
	{
		if (it->second < value)
		{
			log_error("dec_element_value error %u %d %d", type, value, it->second);
		}
	
		it->second -= value;
	}
}

void buff_element_manager_t::dec_element_value(const uint32_t type, const int value, const int chg_type)
{
	if (chg_type == 1)
	{
		dec_element_value_percent(type, value);
	}
	else
	{
		dec_element_value_addi(type, value);
	}
}

int buff_element_manager_t::get_buff_attr(uint32_t type)
{
	attr_map::const_iterator it = m_element_attr.find(type);
	if (it == m_element_attr.end())
	{
		return 0;
	}

	return it->second;
}

void buff_element_manager_t::set_element_value_percent(const uint32_t type, const int value)
{
	attr_map::iterator it = m_element_attr_percent.find(type);
	if (it == m_element_attr_percent.end())
	{
		m_element_attr_percent.insert(std::make_pair(type, value));
	}
	else
	{
		it->second = value;
	}
}

uint32_t buff_element_manager_t::get_element_value_percent(const uint32_t type) const
{
	attr_map::const_iterator it = m_element_attr_percent.find(type);
	if (it == m_element_attr_percent.end())
	{
		return 0;
	}

	return it->second;
}

void buff_element_manager_t::clear_element_value_percent(const uint32_t type)
{
	attr_map::iterator it = m_element_attr_percent.find(type);
	if (it != m_element_attr_percent.end())
	{
		it->second = 0;
	}
}

void buff_element_manager_t::remove_element_value_percent(const uint32_t type)
{
	m_element_attr_percent.erase(type);
}

void buff_element_manager_t::inc_element_value_percent(const uint32_t type, const int value)
{
	attr_map::iterator it = m_element_attr_percent.find(type);
	if (it == m_element_attr_percent.end())
	{
		m_element_attr_percent.insert(std::make_pair(type, value));
	}
	else
	{
		it->second += value;
	}
}

void buff_element_manager_t::dec_element_value_percent(const uint32_t type, const int value)
{
	attr_map::iterator it = m_element_attr_percent.find(type);
	if (it != m_element_attr_percent.end())
	{
		if (it->second < value)
		{
			log_error("dec_element_value error %u %d %d", type, value, it->second);
		}

		it->second -= value;
	}
}

int buff_element_manager_t::get_buff_attr_percent(uint32_t type)
{
	attr_map::const_iterator it = m_element_attr_percent.find(type);
	if (it == m_element_attr_percent.end())
	{
		return 0;
	}

	return it->second;
}

void buff_element_manager_t::clear_all_element()
{
	m_element_attr.clear();
	m_element_attr_percent.clear();
}