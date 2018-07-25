#ifndef __PACKAGE_H__
#define __PACKAGE_H__

#include "protos_fwd.hpp"
#include "item.hpp"
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

class package_t : public boost::enable_shared_from_this<package_t>
{
public:
	package_t(uint64_t owner, proto::common::package_type type, uint32_t size);
	~package_t();

	bool check_can_add(uint32_t tid, uint32_t add_num);
	bool check_can_add_items(const std::map<uint32_t, uint32_t> &item_tid_num_map);
	uint32_t get_bag_use_num();
	uint32_t get_bag_can_use_num();
	void set_max_size(uint32_t size);
	uint32_t get_max_size();
	uint32_t get_cur_size();
	proto::common::package_type get_type();

	bool peek_all_datas(proto::common::item_data* p_data);
	bool peek_hero_equip_datas(proto::common::item_data* p_data);
	void save_all_items(uint64_t role_uid);

	item_ptr has_same_item(uint32_t tid);
	item_ptr get_item_by_tid(uint32_t tid);
	item_ptr get_item_by_num(uint32_t tid, int32_t num);
	item_ptr get_item_by_uid(uint64_t uid);

	bool add_item(item_ptr p_item);
	void del_item(uint64_t uid);

	uint32_t get_equip_strengthen_count(uint32_t level);

	void get_use_equip_list(std::vector<uint64_t>& list);
private:
	uint64_t					m_owner;
	proto::common::package_type m_type;
	uint32_t					m_max_size;
	item_map					m_item_list;
};

typedef boost::shared_ptr<package_t> package_ptr;
typedef std::map<uint64_t, package_ptr> package_map;



#endif//__PACKAGE_H__
