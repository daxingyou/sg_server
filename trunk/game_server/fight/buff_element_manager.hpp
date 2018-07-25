#ifndef __BUFF_ELEMENT_MANAGER_HPP__
#define __BUFF_ELEMENT_MANAGER_HPP__

#include <map>
#include "common/macros.hpp"
#include "combat.hpp"

enum
{
	//��Ӧ���ԵĶ�������ֵ��ƫ�ƣ���Ҫ��֤�߻����õ����Է��ϴ�ֵҪ��
	EXTRA_ATTR_PLUS = 1000,//Ŀǰֻ֧�ְ�ֵ��������֧�ְ��ٷֱ�����
};

class buff_element_manager_t
{
public:
	buff_element_manager_t();
	~buff_element_manager_t();

public:
	int get_buff_attr(uint32_t type);
	int get_buff_attr_percent(uint32_t type);

	void inc_element_value(const uint32_t type, const int value, const int chg_type = 0);//chg_type, 0��ֵ��1����
	void dec_element_value(const uint32_t type, const int value, const int chg_type = 0);
	

	void clear_all_element();
	void clear_element_value(const uint32_t type);
	void clear_element_value_percent(const uint32_t type);

	void remove_element_value_percent(const uint32_t type);

	void get_all_element_attr_id(std::set<uint32_t>& ids);

public:
	void inc_element_value_addi(const uint32_t type, const int value);
	void dec_element_value_addi(const uint32_t type, const int value);

	void set_element_value(const uint32_t type, const int value);
	int get_element_value(const uint32_t type) const;

	void remove_element_value(const uint32_t type);
	
	void inc_element_value_percent(const uint32_t type, const int value);
	void dec_element_value_percent(const uint32_t type, const int value);

	void set_element_value_percent(const uint32_t type, const int value);
	uint32_t get_element_value_percent(const uint32_t type) const;

private:
	attr_map m_element_attr;
	attr_map m_element_attr_percent;
};

#endif//__BUFF_ELEMENT_MANAGER_HPP__
