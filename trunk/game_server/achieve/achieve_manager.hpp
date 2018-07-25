#ifndef	__ACHIEVE_MANAGER_H__
#define __ACHIEVE_MANAGER_H__

#include "protocpp/common.pb.h"
#include "protocpp/client.pb.h"
#include "achieve.hpp"
#include "achieve_common_enum.hpp"

enum achieve_get_reward_type : uint32_t
{
	achieve_get_person_reward = 0,		//人物成就
	achieve_get_single_reward = 1,		//单个成就奖励
};

struct achieve_struct
{
	proto::common::Achieve_Event_Type type;
	uint32_t event_id;
	uint32_t count;
};

class achieve_manager_t
{
public:
	achieve_manager_t() {};
	~achieve_manager_t(){}

public:
	 static void init();
	 static void load_all_data(uint64_t uid,const proto::common::achieve_data& achieve_data);
	 static void peek_all_data(uint64_t uid, proto::common::achieve_data* p_achieve_data,bool is_client = true);
	 static void save_all_self(uint64_t uid);

public:
	//客户端加载所有的成就信息列表
	static bool get_achieve_info_list(uint64_t uid);

public:
	//人物成就相关
	//成就升级
	static bool is_up_achieve_level(uint64_t uid);
	static bool add_achieve_level(uint64_t uid);
	static void add_level(uint64_t uid);

	static uint32_t get_achieve_reward(uint64_t uid,uint32_t type, uint32_t tid,uint32_t level, proto::common::role_change_data* rcd = NULL);
public:
	// 获取成就唯一Id
	static uint64_t gen_achieve_uid();
	static achieve_ptr get_achieve(uint64_t uid, uint32_t achieve_id);

	static bool add_achieve(const uint64_t uid,const uint32_t achieve_id, bool is_load = false);
	static bool add_achieve(uint64_t uid,achieve_ptr p_achieve,bool is_load = false);

	//成就完成通知客户端
	static void notify_client_achieve_finish(uint64_t uid,uint32_t achieve_id,proto::client::gc_achieve_finish_notify& notify);

	//成就统一API
	static void notify_achieve_state(uint64_t uid, proto::common::Achieve_Event_Type type,uint32_t param1 = 1,uint32_t param2 = 1);

	//根据触发类型和事件ID 获取配置列表
	//static void get_achieve_list(proto::common::Achieve_Event_Type achieve_event_type,std::map<uint32_t ,Achievelist*>& vec,uint32_t event_id = 0);
	static bool achieve_up_last_level(const proto::common::Achieve_Event_Type achieve_event_type,const uint32_t cur_count ,std::deque<uint32_t>& drop_id_deque,uint32_t& last_level, uint32_t event_id = 0);

	static bool get_achieve_id(uint32_t achieve_event_type,uint32_t& achieve_id, uint32_t event_id = 0);

	//获取配置表成就完成条件
	static uint32_t get_finish_target_count(uint32_t achieve_id);

	//成就ID 是否合法
	static bool is_legal_achieve_id(const uint32_t achieve_id);

	//获取成就注释说明
	static void get_source_type_str(std::string& str,uint32_t tid);

	//获取日志type
	static uint32_t get_achieve_log_type(uint32_t type);
public:

	//个人历程
	static void get_person_achieve_record_list(uint64_t uid);

	//获取奖杯数和成就类别完成数列表
	static void get_cup_and_type_number_list(uint64_t uid,std::string& str_cup_list, std::string& str_type_list);

	static bool get_hero_achieve_total(const uint64_t role_uid,const proto::common::Achieve_Event_Type type,const uint32_t event_id,uint32_t& total);
public:
	//成就完成 如果有延迟 push to delay list
	static bool push_to_delay_list(uint64_t role_uid,proto::common::Achieve_Event_Type type, uint32_t event_id = 0,uint32_t num = 1);

	//统一发送延迟的成就完成通知
	static void check_send_delay_list(uint64_t role_uid,bool is_new_role = false);

};

static std::map<uint64_t, std::deque<achieve_struct>> m_all_role_delay_notify_list;
typedef boost::shared_ptr<achieve_manager_t> achieve_mgr_t;

static std::map<uint32_t, uint32_t> achieve_log_type_map;
#endif	// __ACHIEVE_MANAGER_H__
