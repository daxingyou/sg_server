#ifndef __GAME_ITEM_MANAGER_H__
#define __GAME_ITEM_MANAGER_H__

#include "item.hpp"
#include "role/role.hpp"
#include <map>
#include "config/config_manager.hpp"
#include "item_limit.hpp"

class Item;

class item_manager_t
{
public:
    item_manager_t();
    ~item_manager_t();

    // load datas from db
    static void load_all_datas(role_ptr p_role, const proto::common::item_data& data);
    // send datas to client
	static void peek_all_datas(role_ptr p_role, proto::common::item_data* p_data);
	// send datas to area
	static void peek_hero_equip_datas(role_ptr p_role, proto::common::item_data* p_data);
    // save datas to db
    static void save_all_datas(role_ptr p_role);
	// load item_limit from center
	static void load_item_limit_from_center(proto::server::eg_syn_item_limit_notify &msg);

public:
	static item_ptr get_item_by_uid(role_ptr p_role, uint64_t uid,
		proto::common::package_type package = proto::common::package_type::package_type_main);
    static item_ptr get_item_by_tid(role_ptr p_role, uint32_t tid, 
		proto::common::package_type package = proto::common::package_type::package_type_main);
	static item_ptr has_same_item(role_ptr p_role, uint32_t tid,
		proto::common::package_type package = proto::common::package_type::package_type_main);
	bool can_overlap(role_ptr p_role, uint32_t tid,
		proto::common::package_type package = proto::common::package_type::package_type_main);
//	static item_ptr get_item_by_num(role_ptr p_role, uint32_t tid, int32_t num,
//		proto::common::package_type package = proto::common::package_type::package_type_main);

    static uint64_t gen_item_uid();
	
    static proto::common::MONEY_TYPE is_money_item(uint32_t tid);

	static bool check_add_item(role_ptr p_role, uint32_t item_tid, uint32_t add_num, 
		proto::common::package_type package = proto::common::package_type::package_type_main);
	static bool check_add_items(role_ptr p_role, const std::map<uint32_t, uint32_t> &item_tid_num_map,
		proto::common::package_type package = proto::common::package_type::package_type_main);
    
	static item_ptr add_item(uint64_t role_uid, uint32_t item_tid, uint32_t add_num, uint32_t source_type, uint32_t source_param, 
		proto::common::role_change_data* p_data = NULL,
		proto::common::package_type package = proto::common::package_type::package_type_main,
		const std::string& creater_name = "", uint32_t param = 0);
    
	static item_ptr add_item(role_ptr p_role, uint32_t item_tid, uint32_t add_num, uint32_t source_type, uint32_t source_param, 
		proto::common::role_change_data* p_data = NULL,
		proto::common::package_type package = proto::common::package_type::package_type_main,
		const std::string& creater_name = "", uint32_t param = 0);

	static bool add_item_ex(role_ptr p_role, uint32_t item_tid, uint32_t add_num, uint32_t source_type, uint32_t source_param,
		proto::common::role_change_data* p_data = NULL,
		proto::common::package_type package = proto::common::package_type::package_type_main,
		const std::string& creater_name = "", uint32_t param = 0);

	static uint32_t add_items(role_ptr p_role, const std::map<uint32_t, uint32_t> &item_tid_num_map, uint32_t source_type, uint32_t source_param,
		proto::common::role_change_data* p_data = NULL,
		proto::common::package_type package = proto::common::package_type::package_type_main);
		
	static uint32_t use_item(role_ptr p_role, uint64_t item_uid, uint32_t item_num, uint32_t &item_tid, proto::common::role_change_data *p_data,
		proto::common::package_type package = proto::common::package_type::package_type_main);

	static bool has_item(role_ptr p_role, uint32_t item_tid, uint32_t item_num,
		proto::common::package_type package = proto::common::package_type::package_type_main);

	static bool has_items(role_ptr p_role, const std::map<uint32_t, uint32_t> &item_tid_num_map,
		proto::common::package_type package = proto::common::package_type::package_type_main);

	static uint32_t get_item_count(role_ptr p_role, uint32_t item_tid, proto::common::package_type package = proto::common::package_type::package_type_main);
	
	static uint32_t remove_item(uint64_t role_uid, uint32_t item_tid, uint32_t item_num, uint32_t source_type, uint32_t source_param, 
		proto::common::role_change_data* p_data = NULL,
		proto::common::package_type package = proto::common::package_type::package_type_main);
	
	static uint32_t remove_item(role_ptr p_role, uint32_t item_tid, uint32_t item_num, uint32_t source_type, uint32_t source_param, 
		proto::common::role_change_data* p_data = NULL,
		proto::common::package_type package = proto::common::package_type::package_type_main);

	static uint32_t remove_item(role_ptr p_role, uint64_t item_uid, uint32_t item_num, uint32_t source_type, uint32_t source_param,
		proto::common::role_change_data* p_data = NULL,
		proto::common::package_type package = proto::common::package_type::package_type_main);
	
	static uint32_t remove_items(role_ptr p_role, std::map<uint32_t, uint32_t> item_tid_num_map, uint32_t source_type, uint32_t source_param, 
		proto::common::role_change_data* p_data = NULL,
		proto::common::package_type package = proto::common::package_type::package_type_main);

	static uint32_t remove_items(role_ptr p_role, const std::vector<uint64_t>& item_list, uint32_t source_type, uint32_t source_param,
		proto::common::role_change_data* p_data = NULL,
		proto::common::package_type package = proto::common::package_type::package_type_main);

	static package_ptr get_package(role_ptr p_role, proto::common::package_type package = proto::common::package_type::package_type_main);

public:
	static uint32_t sell_bag_item(role_ptr p_role, uint64_t item_uid, uint32_t item_num, proto::common::role_change_data* p_data);
	static uint32_t get_equip_strengthen_count(role_ptr p_role, uint32_t level);

	//合成道具 add by hy
	static uint32_t merage_item(role_ptr p_role, uint32_t item_tid, uint32_t merage_num, proto::common::role_change_data* p_data);

	//分解道具
	static uint32_t decompose_item(role_ptr p_role, uint64_t item_uid, uint32_t item_num, proto::common::role_change_data* p_data);
public:
	// 物品发放校验
	static item_limit_ptr get_item_limit(uint32_t item_tid);
	static bool is_need_check(uint32_t item_tid);
	static bool check_limit_item(uint32_t item_tid, uint32_t item_num);
	static void update_item_limit(uint32_t item_tid, uint32_t item_num, uint32_t last_give_time);
	static bool is_equip(uint32_t item_tid);
	static bool is_special(uint32_t item_tid);
private:
	static bool add_item(role_ptr p_role, item_ptr p_item, proto::common::package_type package = proto::common::package_type::package_type_main);

	static void add_item(role_ptr p_role, const proto::common::item_single& single);

	static item_ptr add_item(role_ptr p_role, Item* p_conf, uint32_t add_num, uint32_t source_type, uint32_t source_param,
		proto::common::role_change_data* p_data = NULL,
		proto::common::package_type package = proto::common::package_type::package_type_main,
		const std::string& creater_name = "", uint32_t param = 0);

	static uint32_t use_item_task(role_ptr p_role, item_ptr use_item, uint32_t item_num, Item *config, proto::common::role_change_data *p_data = NULL);

	static uint32_t use_item_equip(role_ptr p_role, item_ptr use_item, uint32_t item_num);

	static uint32_t use_item_gift_package(role_ptr p_role, item_ptr use_item, Item *config, proto::common::role_change_data *p_data);

	static uint32_t use_item_source_package(role_ptr p_role, item_ptr use_item, Item *config, proto::common::role_change_data *p_data);

	static uint32_t use_item_mount(role_ptr p_role, item_ptr use_item, Item *config, uint32_t item_num, proto::common::role_change_data *p_data);

	static uint32_t use_item_reduce_pk_value(role_ptr p_role, item_ptr use_item, Item *config, uint32_t item_num, proto::common::role_change_data *p_data);

	static uint32_t use_item_plugin(role_ptr p_role, item_ptr use_item, Item *config, uint32_t item_num, proto::common::role_change_data *p_data);

	static uint32_t use_item_multi_time(role_ptr p_role, item_ptr use_item, Item *config, uint32_t item_num, proto::common::role_change_data *p_data);

	static uint32_t user_vigour_pill(const role_ptr& p_role, item_ptr use_item, Item *config, uint32_t item_num, proto::common::role_change_data *p_data);
	
	static uint32_t user_item_add_attr(const role_ptr& p_role, item_ptr use_item, Item *config, uint32_t item_num, proto::common::role_change_data *p_data);
private:
	static uint32_t get_item_use_range();
private:
	typedef std::map<uint32_t, item_limit_ptr> item_limit_map;
	static item_limit_map m_item_limit_map;
};

#endif
