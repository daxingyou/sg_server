#include "package_manager.hpp"
#include "config_mgr.h"
#include "tblh/Comprehensive.tbls.h"

package_manager_t::package_manager_t(uint64_t owner)
	: m_owner(owner)
{
	auto conf = GET_CONF(Comprehensive, comprehensive_common::bag_limit);
	uint32_t bag_limit = GET_COMPREHENSIVE_VALUE_1(conf);
	add_package(proto::common::package_type::package_type_main, bag_limit);
	add_package(proto::common::package_type::package_type_trade, 1);	// ×øÆï»áĞŞ¸Ä
}

package_manager_t::~package_manager_t()
{
}

bool package_manager_t::add_package(proto::common::package_type type, uint32_t size)
{
	package_ptr p_package = get_package(type);
	if (p_package != NULL)
	{
		p_package->set_max_size(size);
	}
	else
	{
		p_package.reset(new package_t(m_owner, type, size));
		m_package_list.insert(std::make_pair(type, p_package));
	}

	return true;
}

package_ptr package_manager_t::get_package(proto::common::package_type type)
{
	package_map::iterator it = m_package_list.find(type);
	if (it == m_package_list.end())
	{
		return package_ptr();
	}

	return it->second;
}

bool package_manager_t::peek_all_datas(proto::common::item_data* p_data)
{
	if (NULL == p_data)
	{
		log_error("package p_data NULL");
		return false;
	}

	for (auto package : m_package_list)
	{
		if (package.second == NULL)
		{
			continue;
		}

		package.second->peek_all_datas(p_data);
	}

	return true;
}

bool package_manager_t::peek_hero_equip_datas(proto::common::item_data* p_data)
{
	if (NULL == p_data)
	{
		log_error("package p_data NULL");
		return false;
	}

	for (auto package : m_package_list)
	{
		if (package.second == NULL)
		{
			continue;
		}

		package.second->peek_hero_equip_datas(p_data);
	}

	return true;
}

bool package_manager_t::save_all_packages()
{
	for (auto package : m_package_list)
	{
		if (package.second == NULL)
		{
			continue;
		}

		package.second->save_all_items(m_owner);
	}

	return true;
}

item_ptr package_manager_t::has_same_item(uint32_t tid, proto::common::package_type type)
{
	package_ptr package = get_package(type);
	if (NULL == package)
	{
		log_error("package package NULL type[%d]", type);
		return item_ptr();
	}

	return package->has_same_item(tid);
}

item_ptr package_manager_t::get_item_by_tid(uint32_t tid, proto::common::package_type type)
{
	package_ptr package = get_package(type);
	if (NULL == package)
	{
		log_error("package package NULL type[%d]", type);
		return item_ptr();
	}

	return package->get_item_by_tid(tid);
}

item_ptr package_manager_t::get_item_by_num(uint32_t tid, int32_t num, proto::common::package_type type)
{
	package_ptr package = get_package(type);
	if (NULL == package)
	{
		log_error("package package NULL type[%d]", type);
		return item_ptr();
	}

	return package->get_item_by_num(tid, num);
}

item_ptr package_manager_t::get_item_by_uid(uint64_t uid, proto::common::package_type type)
{
	package_ptr package = get_package(type);
	if (NULL == package)
	{
		log_error("package package NULL type[%d]", type);
		return item_ptr();
	}

	return package->get_item_by_uid(uid);
}

bool package_manager_t::add_item(item_ptr p_item, proto::common::package_type type)
{
	package_ptr package = get_package(type);
	if (NULL == package)
	{
		log_error("package package NULL type[%d]", type);
		return false;
	}

	return package->add_item(p_item);
}

void package_manager_t::del_item(uint64_t uid, proto::common::package_type type)
{
	package_ptr package = get_package(type);
	if (NULL == package)
	{
		log_error("package package NULL type[%d]", type);
		return;
	}

	package->del_item(uid);
}
