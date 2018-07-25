#pragma once

#include "role/role.hpp"
#include "config/config_manager.hpp"
#include "tblh/DropBagTable.tbls.h"
#include "tblh/DropTable.tbls.h"

/*
author:wys
date:2016.12.19
desc:掉落管理器
*/

#include "protos_fwd.hpp"

class drop_manager_t
{
public:

	// 掉落 掉落包id(DropWorldTable) 来源
	static bool drop(role_ptr p_role, uint32_t drop_bag_id, uint32_t source_type, proto::common::drop_sys_type drop_sys_type, 
					 uint32_t source_param = 0, proto::common::role_change_data* rcd = NULL);

	static bool check_bag_num(role_ptr p_role, uint32_t drop_bag_id);
	

	//这里只是假掉落，并不会掉落真正获得，用来做多次掉落的优化
	static bool pre_drop(role_ptr p_role, uint32_t drop_bag_id, std::map<uint32_t, uint32_t> &item_map, uint32_t count = 1);

	static bool drop_once(role_ptr p_role, uint32_t drop_bag_id, uint32_t source_type, proto::common::drop_sys_type drop_sys_type, 
					 uint32_t source_param, std::vector<proto::common::item_num_info> &item_vec, proto::common::role_change_data* rcd = NULL);
private:
	// 掉落 根据条件筛选出全部掉落
	static bool get_drop_list(role_ptr p_role, DropBagTable *p_drop_group, const std::set<uint32_t> &drop_list,
							  std::vector<DropTable*> &drop_all_result);

	static bool check_drop_cond(role_ptr p_role, DropTable *p_drop);

	static bool check_drop_relative(role_ptr p_role, uint32_t drop_num, uint32_t rand_type,
									const std::set<uint32_t> &drop_list, std::vector<DropTable*> &drop_result);

	static bool roll_drop_relative(const std::vector<DropTable*> &drop_list, std::vector<DropTable*> &drop_result, 
									uint32_t total_prob, int32_t &roll_pos);

	static bool check_drop_absolute(role_ptr p_role, uint32_t drop_num, uint32_t rand_type,
									const std::set<uint32_t> &drop_list, std::vector<DropTable*> &drop_result);

	static bool roll_drop_absolute(uint32_t drop_num, const std::vector<DropTable*> &drop_list, std::vector<DropTable*> &drop_result);
};