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
	///��ʼ��
	static bool init();

	///�ı�౶����״̬
	static uint32_t change_multi_state(const uint64_t uid, uint32_t type,proto::common::role_change_data* p_data = NULL);

	///��Ӷ౶
	static uint32_t push_to_multi_list(const uint64_t uid,const uint32_t item_id, proto::common::role_change_data* p_data = NULL);

	///�౶���汶������
	static uint32_t multi_exp_buff(const uint64_t uid, proto::common::role_change_data* p_data = NULL);

public:
	///���¶౶�б�
	static void update_multi_list(const uint64_t uid);

	///�õ����Ƿ����������Ӷ౶ʱ��BUFF
	static bool is_exist_add_multi_time(const uint32_t item_id);

public:
	///��ȡ�౶�����ı���
	static uint32_t get_multi_point_times();

	///��ȡ�౶ʱ�����������
	static uint32_t get_multi_time_max_upper();

	///��ȡ�౶����(����ṹ)
	static void get_multi_time_list(const uint64_t uid, proto::common::multi_time_data* p_data = NULL);

	static uint32_t get_multi_time(const uint64_t uid, uint32_t type);

	static uint32_t get_max_multi_type(const uint64_t uid);

private:
	static std::vector<Dawn::triple<uint32_t, uint32_t, uint32_t>> m_add_multi_time_conf_list;
};

