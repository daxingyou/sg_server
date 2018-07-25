#ifndef __PACKAGE_MANAGER_H__
#define __PACKAGE_MANAGER_H__

#include "package.hpp"

class package_manager_t
{
public:
	package_manager_t(uint64_t owner);
	~package_manager_t();

public:
	bool add_package(proto::common::package_type type, uint32_t size);
	package_ptr get_package(proto::common::package_type type);

public:

	bool peek_all_datas(proto::common::item_data* p_data);
	bool peek_hero_equip_datas(proto::common::item_data* p_data);
	bool save_all_packages();

	item_ptr has_same_item(uint32_t tid, proto::common::package_type type);
	item_ptr get_item_by_tid(uint32_t tid, proto::common::package_type type);
	item_ptr get_item_by_num(uint32_t tid, int32_t num, proto::common::package_type type);
	item_ptr get_item_by_uid(uint64_t uid, proto::common::package_type type);

	bool add_item(item_ptr p_item, proto::common::package_type type);
	void del_item(uint64_t uid, proto::common::package_type type);

	void add_search_val(uint32_t search_id, uint32_t count);

	void del_search_val(uint32_t search_id, uint32_t count);

	uint32_t get_search_val(uint32_t search_id);
private:
	uint64_t		m_owner;
	package_map		m_package_list;
	std::map<uint32_t, uint32_t> m_search_map;	//物品search_id为key,数量为value的索引
};



#endif//__PACKAGE_MANAGER_H__
