#include "package.hpp"
#include "common/log.hpp"
#include "config/config_manager.hpp"
#include "tblh/Item.tbls.h"
#include "item_manager.hpp"

package_t::package_t(uint64_t owner, proto::common::package_type type, uint32_t size)
	: m_owner(owner)
	, m_type(type)
	, m_max_size(size)
{
}

package_t::~package_t()
{
}

void package_t::set_max_size(uint32_t size)
{
	m_max_size = size;
}

uint32_t package_t::get_max_size()
{
	return m_max_size;
}

uint32_t package_t::get_cur_size()
{
	//return m_item_list.size();
	uint32_t slot = 0;
	item_map::iterator it = m_item_list.begin();
	item_map::iterator it_end = m_item_list.end();
	item_ptr p_item;

	for (; it != it_end; ++it)
	{
		p_item = it->second;
		if (NULL != p_item)
		{
			slot += p_item->get_need_grid();
		}
	}

	return slot;
}

proto::common::package_type package_t::get_type()
{
	return m_type;
}

bool package_t::peek_all_datas(proto::common::item_data* p_data)
{
	if (NULL == p_data)
	{
		log_error("package p_data NULL role_uid[%lu]", m_owner);
		return false;
	}

	item_map::iterator it = m_item_list.begin();
	item_map::iterator it_end = m_item_list.end();
	item_ptr p_item;

	for (; it != it_end; ++it)
	{
		p_item = it->second;
		if (NULL != p_item)
		{
			if (p_item->get_cur_num() <= 0)
			{
				continue;
			}

			proto::common::item_single* p_single = p_data->add_item_list();
			p_item->peek_data(p_single);
		}
	}

	return true;
}

bool package_t::peek_hero_equip_datas(proto::common::item_data* p_data)
{
	if (NULL == p_data)
	{
		log_error("package p_data NULL role_uid[%lu]", m_owner);
		return false;
	}

	item_map::iterator it = m_item_list.begin();
	item_map::iterator it_end = m_item_list.end();
	item_ptr p_item;

	for (; it != it_end; ++it)
	{
		p_item = it->second;
		if (NULL != p_item)
		{
			if (p_item->get_cur_num() <= 0)
			{
				continue;
			}
			if (p_item->get_hero_uid() > 0)
			{
				proto::common::item_single* p_single = p_data->add_item_list();
				p_item->peek_data(p_single);
			}
		}
	}
	return true;
}

void package_t::save_all_items(uint64_t role_uid)
{
	item_map::iterator it = m_item_list.begin();
	item_map::iterator it_end = m_item_list.end();
	item_ptr p_item;
	for (; it != it_end; ++it)
	{
		p_item = it->second;
		if (NULL != p_item)
		{
			p_item->save_self(role_uid, false, true);
		}
	}

	//ITME_LOG("role[%lu] save package [%d] all items to db: size[%d]", m_owner, (int)m_type, m_item_list.size());
}

item_ptr package_t::has_same_item(uint32_t tid)
{
	item_map::iterator it = m_item_list.begin();
	item_map::iterator it_end = m_item_list.end();
	for (; it != it_end; ++it)
	{
		item_ptr p_item = it->second;
		if (NULL != p_item && p_item->get_tid() == tid)
		{
			return p_item;
		}
	}

	return item_ptr();
}

item_ptr package_t::get_item_by_tid(uint32_t tid)
{
	item_map::iterator it = m_item_list.begin();
	item_map::iterator it_end = m_item_list.end();
	for (; it != it_end; ++it)
	{
		item_ptr p_item = it->second;
		if (NULL != p_item && p_item->get_tid() == tid && p_item->get_cur_num() != 0 )
		{
			return p_item;
		}
	}
	
	return item_ptr();
}

item_ptr package_t::get_item_by_num(uint32_t tid, int32_t num)
{
	item_map::iterator it = m_item_list.begin();
	item_map::iterator it_end = m_item_list.end();
	for (; it != it_end; ++it)
	{
		item_ptr p_item = it->second;
		if (NULL != p_item && p_item->get_tid() == tid && p_item->get_cur_num() >= num)
		{
			return p_item;
		}
	}

	log_error("role[%lu] item[%d] was not enough!", m_owner, tid);
	return item_ptr();
}

item_ptr package_t::get_item_by_uid(uint64_t uid)
{
	item_map::iterator it = m_item_list.find(uid);
	item_map::iterator it_end = m_item_list.end();
	if (it == it_end)
	{
		log_error("role[%lu] get item[%lu] failed: not have the item!", m_owner, uid);
		return item_ptr();
	}

	return it->second;
}

bool package_t::add_item(item_ptr p_item)
{
	if (p_item == NULL)
	{
		log_error("item p_item NULL");
		return false;
	}

	if (get_cur_size() >= m_max_size)
	{
		return false;
	}

	m_item_list.insert(std::make_pair(p_item->get_uid(), p_item));
	p_item->set_package(m_type);
	return true;
}

void package_t::del_item(uint64_t uid)
{
	m_item_list.erase(uid);
}

bool package_t::check_can_add(uint32_t tid, uint32_t add_num)
{
	if (add_num == 0)
	{
		log_error("item check_can_add role_uid[%lu] tid[%d] add_num = 0", m_owner, tid);
		return false;
	}
	Item *p_item_config = NULL;

	item_map::iterator it = m_item_list.begin();
	item_map::iterator it_end = m_item_list.end();

	item_ptr p_item = item_ptr();
	uint32_t use_num = 0;
	bool b_find = false;

	for (; it != it_end; ++it)
	{
		p_item = it->second;
		if (NULL == p_item)
		{
			log_error("package p_item NULL uid[%lu]", it->first);
			return false;
		}
		if(p_item->get_cur_num() == 0)
			continue;
		p_item_config = GET_CONF(Item, p_item->get_tid());
		if (NULL == p_item_config)
		{
			log_error("package item_tid[%d] is not found!", p_item->get_tid());
			return false;
		}
		if (item_manager_t::is_equip(tid) || 
			item_manager_t::is_special(tid))
		{
			use_num += 1;
			continue;
		}
		if (p_item_config->overlap_count() == 0)
		{
			use_num += 1;
			continue;
		}
		if (p_item->get_tid() == tid)
		{
			use_num += ( (p_item->get_cur_num() + add_num - 1) / p_item_config->overlap_count() + 1 );
			b_find = true;
		}
		else
		{
			use_num += ( (p_item->get_cur_num() - 1) / p_item_config->overlap_count() + 1);
		}
	}
	if (!b_find)
	{
		p_item_config = GET_CONF(Item, tid);
		if (NULL == p_item_config)
		{
			log_error("package item_tid[%d] is not found!", tid);
			return false;
		}
		if (p_item_config->overlap_count() == 0)
		{
			use_num += 1;
		}
		else
		{
			use_num += ((add_num - 1) / p_item_config->overlap_count() + 1);
		}
	}
	if (use_num > m_max_size)
	{
		log_error("use_num[%d] larger than max_size[%d]", use_num, m_max_size);
		return false;
	}

	return true;
}

uint32_t package_t::get_bag_use_num()
{
	Item *p_item_config = NULL;
	item_ptr p_item = item_ptr();
	uint32_t use_num = 0;

	item_map::iterator it = m_item_list.begin();
	item_map::iterator it_end = m_item_list.end();
	for (; it != it_end; ++it)
	{
		p_item = it->second;
		if (NULL == p_item)
			return false;
		p_item_config = GET_CONF(Item, p_item->get_tid());
		if (NULL == p_item_config)
		{
			log_error("item_tid[%d] is not found!", p_item->get_tid());
			return false;
		}
		if (p_item->get_cur_num() == 0)
			continue;
		if (p_item_config->overlap_count() == 0)
		{
			use_num += 1;
			continue;
		}

		use_num += ((p_item->get_cur_num() - 1) / p_item_config->overlap_count() + 1);		
	}
	return use_num;
}

uint32_t package_t::get_bag_can_use_num()
{
	if (m_max_size < get_cur_size())
		return 0;
	return m_max_size - get_cur_size();
}

bool package_t::check_can_add_items(const std::map<uint32_t, uint32_t> &item_tid_num_map)
{
	Item *p_item_config = NULL;

	item_map::iterator it = m_item_list.begin();
	item_map::iterator it_end = m_item_list.end();

	item_ptr p_item = item_ptr();
	uint32_t use_num = 0;
	uint32_t temp_total_num = 0;

	std::map<uint32_t, uint32_t> temp_add_items = item_tid_num_map;

	for (; it != it_end; ++it)
	{
		p_item = it->second;
		if (NULL == p_item)
		{
			log_error("package p_item NULL uid[%lu]", it->first);
			return false;
		}
		if (p_item->get_cur_num() == 0)
			continue;
		p_item_config = GET_CONF(Item, p_item->get_tid());
		if (NULL == p_item_config)
		{
			log_error("package item_tid[%d] is not found!", p_item->get_tid());
			return false;
		}
		if (p_item_config->overlap_count() == 0)
		{
			use_num += 1;
			continue;
		}
		temp_total_num = 0;

		for (auto it_check : temp_add_items)
		{
			if (p_item->get_tid() == it_check.first)
			{
				temp_total_num = it_check.second;
				temp_add_items.erase(it_check.first);
				break;
			}
		}
		use_num += ((p_item->get_cur_num() + temp_total_num - 1) / p_item_config->overlap_count() + 1);
	}
	for (auto it_check : temp_add_items)
	{
		p_item_config = GET_CONF(Item, it_check.first);
		if (NULL == p_item_config)
		{
			log_error("package item_tid[%d] is not found!", it_check.first);
			return false;
		}
		if (p_item_config->overlap_count() == 0)
		{
			use_num += 1;
		}
		else
		{
			use_num += ((it_check.second - 1) / p_item_config->overlap_count() + 1);
		}
	}
	if (use_num > m_max_size)
		return false;

	return true;
}

uint32_t package_t::get_equip_strengthen_count(uint32_t level)
{
	uint32_t count = 0;
	for (auto it : m_item_list)
	{
		auto p_item = it.second;
		if (NULL == p_item)
		{
			continue;;
		}
		if (!p_item->is_equip())
		{
			continue;
		}
		if (p_item->get_strengthen_level() >= level)
		{
			++count;
		}
	}
	return count;
}

void package_t::get_use_equip_list(std::vector<uint64_t>& list)
{
	list.clear();
	for (auto& it : m_item_list)
	{
		item_ptr p_item = it.second;
		if (NULL == p_item)
		{
			continue;;
		}
		if (!p_item->is_equip())
		{
			continue;
		}
		if (p_item->get_hero_uid() == 0)
		{
			continue;
		}
		list.push_back(it.first);
	}
}
