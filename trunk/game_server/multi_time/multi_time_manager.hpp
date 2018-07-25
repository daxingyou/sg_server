#pragma once
#include "protocpp/common.pb.h"
#include "protocpp/client.pb.h"
#include <map>
#include <vector>
#include "common/utility.hpp"
#include "common/MyTuple.h"

enum MULTI_STATE : uint32_t
{
	multi_close = 0,
	multi_open,
};

class multi_time_manager_t
{
public:
	multi_time_manager_t() {}
	~multi_time_manager_t() {}

public:
	///初始化
	static bool init();

	///改变多倍开关状态
	static uint32_t change_multi_state(const uint64_t uid, uint32_t type,proto::common::role_change_data* p_data = NULL);

	///添加多倍
	static uint32_t push_to_multi_list(const uint64_t uid,const uint32_t item_id, proto::common::role_change_data* p_data = NULL);

	///多倍收益倍数计算
	static uint32_t multi_exp_buff(const uint64_t uid, proto::common::role_change_data* p_data = NULL);

public:
	///更新多倍列表
	static void update_multi_list(const uint64_t uid);

	///该道具是否可以用于添加多倍时长BUFF
	static bool is_exist_add_multi_time(const uint32_t item_id);

public:
	///获取多倍点数的倍数
	static uint32_t get_multi_point_times();

	///获取多倍时长最大上限数
	static uint32_t get_multi_time_max_upper();

	///获取多倍数据(打包结构)
	static void get_multi_time_list(const uint64_t uid, proto::common::multi_time_data* p_data = NULL);

	static uint32_t get_multi_time(const uint64_t uid, uint32_t type);

	static uint32_t get_max_multi_type(const uint64_t uid);

private:
	static std::vector<Dawn::triple<uint32_t, uint32_t, uint32_t>> m_add_multi_time_conf_list;
};

