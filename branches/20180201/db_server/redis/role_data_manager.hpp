#ifndef __ROLE_DATA_MANAGER__
#define __ROLE_DATA_MANAGER__

#include "common/macros.hpp"
#include "protos_fwd.hpp"
#include <set>

class role_data_manager
{
public:
	role_data_manager();
	~role_data_manager();

	static bool save_role_data_to_redis(uint64_t uid, const proto::common::role_data& data);
	static bool load_role_data_from_redis(uint64_t uid, proto::common::role_data* ud);

	static uint32_t check_nickname_valid(const std::string& name);
	static bool is_role_exist(uint64_t uid);

	//static bool save_interior_data_to_redis(uint64_t uid, const proto::common::interior_info& ii);
	//static bool load_interior_data_from_redis(uint64_t uid, proto::common::interior_info* ii);

	static bool save_role_ex_to_redis(uint64_t uid, const proto::common::role_data& ud);
	static bool load_role_ex_from_redis(uint64_t uid, proto::common::role_data* ud);

	static bool save_role_money_to_redis(uint64_t uid, const proto::common::role_data& ud);
	static bool load_role_money_from_redis(uint64_t uid, proto::common::role_data* ud);

	static bool save_role_fight_to_redis(uint64_t uid, const proto::common::role_data& ud);
	static bool load_role_fight_from_redis(uint64_t uid, proto::common::role_data* ud);

	//////////////////////////////////////////////////////////////////////////
	// 创建角色
	static uint32_t check_new_role(uint64_t uid, uint32_t gate_id, const std::string& name, uint32_t role_type, int32_t create_time);

	// 写入数据库
	static void create_new_role(uint64_t uid, const proto::common::account_role_data& data);

	// 删除临时名字
	static void del_temp_name(const std::string& temp_name);

	// 查询角色
	static bool query_role_data(uint64_t uid, uint32_t gate_id);

	// 通过role_type查找默认的plugin
	static uint32_t get_plugin(uint32_t role_type);

protected:
	static std::set<std::string> m_temp_name_occupancy;
};

#endif//__ROLE_DATA_MANAGER__