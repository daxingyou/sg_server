#ifndef __GAME_EQUIP_MANAGER_H__
#define __GAME_EQUIP_MANAGER_H__

#include "item/item.hpp"
#include "role/role.hpp"
#include "tblh/Equip.tbls.h"
#include "tblh/EquipExattr.tbls.h"
#include "tblh/EquipExchangePool.tbls.h"
#include "tblh/EquipName.tbls.h"
#include "tblh/EquipSpecial.tbls.h"

#include <map>
#include <vector>
typedef std::vector<EquipExattr*> EquipExattrVec;
typedef std::map<uint32_t, EquipExattrVec> EquipExattrPoolMap;
typedef std::vector<EquipExchangePool*> EquipExchangePoolVec;
typedef std::map<uint32_t, EquipExchangePoolVec> EquipExchangePoolMap;
typedef std::vector<EquipName*> EquipNameVec;
typedef std::map<uint32_t, EquipNameVec> EquipNamePoolMap;
typedef std::vector<EquipSpecial*> EquipSpecialVec;
typedef std::map<uint32_t, EquipSpecialVec> EquipSpecialPoolMap;

enum equip_smelt_box_state
{
	equip_smelt_box_state_none = 0,	//未达到领取条件
	equip_smelt_box_state_finish = 1,	//达到领取条件未领取
	equip_smelt_box_state_over = 2,	//已领取
};

class achieve_method_t;
class equip_manager_t
{

public:
	static item_ptr get_equip_by_uid(role_ptr p_role, uint64_t uid);
    static bool make_equip(role_ptr p_role, item_ptr p_equip);

public:
	static uint32_t exchange_equip(role_ptr p_role, uint32_t type, uint32_t exchange_id, proto::common::role_change_data* p_data);
	static uint32_t equip_on(role_ptr p_role, uint64_t equip_uid, uint64_t hero_uid, proto::common::role_change_data* p_data);
	static uint32_t equip_off(role_ptr p_role, uint64_t equip_uid, uint64_t hero_uid, proto::common::role_change_data* p_data);
	static uint32_t remake_attr(role_ptr p_role, uint64_t equip_uid, uint32_t gold_flag, proto::common::role_change_data* p_data);
	static uint32_t remake_exattr(role_ptr p_role, uint64_t equip_uid, const std::vector<uint32_t>& lock_vec, proto::common::role_change_data* p_data);
	static uint32_t replace_exattr(role_ptr p_role, uint64_t equip_uid, proto::common::role_change_data* p_data);
	static uint32_t equip_smelt(role_ptr p_role, const std::vector<uint64_t>& equip_list, proto::common::role_change_data* p_data);
	static void peek_equip_smelt_data(role_ptr p_role, proto::client::gc_equip_smelt_data_reply& reply);
	static uint32_t equip_smelt_box(role_ptr p_role, uint32_t box_id, proto::common::role_change_data* p_data);
	static uint32_t equip_strengthen(role_ptr p_role, uint64_t equip_uid, uint32_t count, proto::common::role_change_data* p_data);
private:
	static void make_attr(Equip* p_equip_conf, std::map<uint32_t, uint32_t>& attr_map);
	static void make_exattr(uint32_t count, uint32_t exattr_pool, std::vector<uint32_t>& exattr_vec);
	static uint32_t make_name(uint32_t name_pool, uint32_t prob);
	static uint32_t make_special(uint32_t special_pool, uint32_t prob);
	static uint32_t get_equip_id_by_exchange_pool(uint32_t pool_id);
	static uint32_t get_remake_stone();
	static uint32_t get_lock_material();
	static uint32_t get_strengthen_material();
	static uint32_t get_strengthen_material_ex();
	static uint32_t equip_strengthen_single(role_ptr p_role, item_ptr p_equip, proto::common::role_change_data* p_data);
	static uint32_t get_equip_make_lucky_max();
public:
	static bool make_equip_info();
	static void clear_equip_info();
private:
	static bool make_equip_exattr_pool_map();
	static bool make_equip_exchange_pool_map();
	static bool make_equip_name_pool_map();
	static bool make_equip_special_pool_map();
	static void make_equip_smelt_box();
private:
	static EquipExattrPoolMap equip_exattr_pool_map;
	static EquipExchangePoolMap equip_exchange_pool_map;
	static EquipNamePoolMap equip_name_pool_map;
	static EquipSpecialPoolMap equip_special_pool_map;
	static uint32_t equip_remake_stone_gold;
	static std::map<uint32_t, uint32_t> s_box_info;  //宝箱需要的星数:掉落
};

#endif
