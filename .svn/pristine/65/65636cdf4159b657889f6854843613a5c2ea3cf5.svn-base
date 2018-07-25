#ifndef __TASK_MANAGER_H__
#define __TASK_MANAGER_H__

#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include "common.pb.h"
#include "client.pb.h"
#include "macros.hpp"
#include "task.hpp"

class task_manager_t : public boost::enable_shared_from_this<task_manager_t>
{
public:
	task_manager_t(uint64_t role_uid);
	
    ~task_manager_t() {}

	void init()
	{
		m_task_map.clear();
        m_task_type_count.clear();
		m_circle_task_map.clear();
	}
	// 获取任务唯一Id
	static uint64_t gen_task_uid();
	void add_task(task_ptr p_task, bool by_load = false);
	void drop_task(uint32_t task_id);
	const std::map<uint64_t, task_ptr>& get_all_task() { return m_task_map; }
	//客户端请求完成任务的入口（某些任务是自动完成的）
	bool request_commit_task(const proto::client::cg_commit_task_request& msg);
	//完成任务(is_auto 0不是自动， 1是自动)
	bool finish_task(uint32_t task_id, proto::common::role_change_data& rcd, uint32_t is_auto = 0, uint32_t coeff = 1, uint32_t choice = 0);

	bool try_kill_task_monster(uint32_t task_id);
	//完成交付任务
	bool try_finish_pay_task(uint32_t task_id, uint64_t prop_id, proto::common::role_change_data& rcd, uint32_t is_auto = 0);
	//GM命令完成任务
	uint16_t gm_finish_task(uint32_t task_id);
	//GM接完所有任务
	bool gm_finish_all_task();

	//找出进行中的任务中任务完成类型为target_type和完成目标id为target_id的任务状态信息，没有则返回空
	void get_specified_state_tasks_by_target_type(task_state state, task_target_type target_type, std::vector<uint64_t>& task_uids, uint32_t target_id = 0);
	//修改目标完成条件的任务进度
	void try_change_task_state(task_target_type target_type, proto::common::role_change_data& rcd, uint32_t target_id, uint32_t add_count, bool is_add = true);

	void do_after_finish_task(uint32_t task_id);
	// 计算奖励
	void calc_reward(uint32_t task_id, proto::common::role_change_data& rcd, uint32_t reply_code = 0, uint32_t coeff = 1, uint32_t choice = 0);

	task_ptr get_task(uint32_t task_id);
	task_ptr get_task_by_uid(uint64_t uid);
	//获取指定类型的环任务指针
	task_ptr get_circle_task(uint32_t task_type);

	void active_task_by_cond(proto::common::role_change_data& rcd, bool isNotify = false);
	//普通任务的接取（唯一入口）
	bool active_task_by_id(uint32_t id, accept_task_type type, proto::common::role_change_data& rcd, bool is_notify = true);
	void reset_task_by_type(task_type type, proto::common::task_data& reply);
	bool is_activated(uint32_t id);
	bool is_done(uint32_t id);
	bool is_doing(uint32_t id);
	bool is_doing2(uint32_t id);	// 只判断正在进行
	bool is_done_or_can_commmit(uint32_t id);	//是否完成或者可提交

	void peek_all_datas(proto::common::task_data* data);
	void peek_recruit_task(proto::common::task_data* data);
	void notify_client_tasklist_changed(std::vector<task_ptr>& task_changed, proto::common::role_change_data* rcd = NULL);
    template<typename MessageType>void notify_client_taskstate(role_ptr p_role, task_ptr p_task, uint16_t cmd, const proto::common::role_change_data& rcd, uint32_t reply_code = 0, uint32_t is_auto = 0);
	bool can_accept_by_type(uint32_t task_id, accept_task_type type = ACCEPT_TASK_BY_SYSTEM);
	void get_task_can_do_manul(std::vector<uint32_t>& task_can_do_manual);
    //db op
    void update_state_to_db(uint32_t task_id);
    void load_all_task_data(const proto::common::task_data& task_data);
    void peek_all_task_data(proto::common::task_data* task_data);
    void save_all_task_data();
	void save_self();

	//试炼
	uint32_t refresh_shilian(proto::common::role_change_data* p_data);

	// task trigger
public:
	// 添加任务暗雷
	void add_task_trigger(uint32_t task_id);
	
	// 帮助任务
public:
	// 添加帮助任务
	void task_seek_help(uint32_t task_tid);
	// 帮助完成任务
	uint32_t help_finish_task(uint64_t help_role_uid, uint32_t task_tid);

public:
	// 完成战斗
	void on_finish_pve(uint64_t against_id, proto::common::role_change_data& rcd);

	void add_task_type_count(uint32_t type, uint32_t count);
	uint32_t get_task_type_count(uint32_t type);
	uint32_t get_task_type_max(uint32_t type);

	bool check_task_type_count(uint32_t type);

	void load_task_type_data(const proto::common::task_type_data& ttd);
	void peek_task_type_data(proto::common::task_type_data* ttd);

	void one_day();
	void one_week();
	void leave_family();

public:
	//生成试炼任务的星级序列
	void make_seq();
	//环任务的接取（唯一入口）
	void active_circle_task(uint32_t task_type, uint32_t circle_id, accept_task_type type, proto::common::role_change_data& rcd, uint32_t level = 0);
	//特殊任务处理 add hy
	void active_circle_task_by_sync( proto::common::task_state &task_info, proto::common::role_change_data &rcd );
	//放弃环任务
	void drop_circle_task(uint32_t task_type);
	//删除环任务
	void del_circle_task(uint32_t task_type);

	//环任务策划表数据重新组织
	static bool make_circle_info();
	static void clear_circle_info();
	//只有当前环总数
	static uint32_t get_max_circle_count(uint32_t role_level, uint32_t task_type, uint32_t task_id);
	static bool is_circle_task(uint32_t task_type);
	//读取全局表配置
	static bool init_conf();

	//捉鬼任务
	static uint32_t get_max_zhuogui_num();
	static uint32_t get_zhuogui_leader_drop();

//-------------------------------一件完成任务---------------
	//获取可以一键完成的任务列表
	uint32_t get_quick_task_info(proto::client::gc_task_quick_data_reply &msg);

	//一键完成任务
	uint32_t quick_complete_task(uint32_t activity_id, uint32_t count, proto::client::gc_task_quick_complete_reply& reply);

	//获取剩余任务次数
	uint32_t get_circle_task_num(uint32_t task_type);

	//快速完成任务
	bool quick_finish_task(uint32_t task_id, proto::common::role_change_data& rcd, uint32_t coeff = 1);

	//判断是否可解锁扫荡
	bool is_task_can_sd(uint32_t task_type);

	//设置任务解锁
	void unlock_task_sd(uint32_t task_type);

	//获取总任务次数
	static uint32_t get_total_circle_count(uint32_t role_level, uint32_t task_type, uint32_t task_id, uint32_t act_id);

	//获取当前环剩余次数
	uint32_t get_circle_count_left(uint32_t role_level, uint32_t task_type, uint32_t act_id);

	bool is_finish_all_circle(uint32_t type, uint32_t level, uint32_t aid);

	//快速完成单个任务
	static uint32_t auto_complete_task(role_ptr p_role, uint32_t task_id , uint64_t item_uid);
private:
	static bool load_circle_task(task_circle_single_ptr p_circle, TaskCircle* p_conf, uint32_t index = 0);
	static uint32_t make_circle_key(uint32_t task_type, uint32_t task_id);
	static void make_circle_task_single_level(task_circle_map_by_level& tcmbl, TaskCircle* p_conf);
	uint32_t make_circle_task(task_ptr p_circle_task, task_circle_ptr p_task_circle);

	//交付任务星级奖励系数
	static bool make_prop_task_coeff();
	//试炼任务星级序列
	static bool make_shilian_seq();
	//试炼任务刷新道具
	static uint32_t get_shilian_refresh_item();
public:
	//生成藏宝图任务（不能主动接取任务只能由藏宝活动自动产生）
	void active_treasure_task(uint32_t treasure_id, proto::common::role_change_data& rcd);
	//放弃藏宝任务（放弃藏宝活动之后自动调用）
	void drop_treasure_task();
	//藏宝任务策划表ID
	static uint32_t get_treasure_task_id();
public:
	//存入已完成列表
	void add_comp_task_list( task_ptr p_task );

	bool is_comp_task(uint32_t task_id) { return m_comp_task_list.find(task_id) != m_comp_task_list.end();  }

	bool save_comp_task_list();
	
	bool save_cur_task_list();

    void load_comp_task_list(const proto::common::task_data& task_data);

    void load_cur_task_list(proto::common::task_data* task_data);

	const std::set<uint32_t>& get_all_comp_task() { return m_comp_task_list; }

	//完成所有主线任务（GM命令）
	void finish_all_task();
public:
	//触发任务事件
	bool do_task_event();

	void save_event();

	void notify_task_event_to_client();

	uint32_t task_event_award(uint32_t event_id, proto::common::role_change_data* p_data, proto::common::task_event* p_event_data);
private:
	std::string m_key = "";
	std::string m_comp_task_key = "";	//完成任务列表key
	std::string m_cur_task_key = "";	//当前任务列表
	std::string m_task_event_key = "";	//任务事件

	uint64_t m_role_uid = 0;
	std::map<uint64_t, task_ptr> m_task_map;		// 以唯一ID为key的MAP
	std::map<uint32_t, task_ptr> m_circle_task_map; // 以任务类型为key的MAP(不仅仅是环任务 所有特殊任务都包括)
	uint32_t m_shilian_count = 0;					// 试炼次数
	std::vector<uint32_t> m_shilian_seq;			// 试炼随机序列
	std::map<uint32_t, uint32_t> m_task_type_count;
	std::set<uint32_t> m_circle_task_complate_list;	//环任务可扫荡记录
	std::set<uint32_t>	m_comp_task_list;			//已完成任务列表
	std::map<uint32_t, uint32_t> m_task_event;		//任务事件（时间ID:到期时间）

	static task_circle_map s_circle_map;
	static std::map<uint32_t, uint32_t> s_prop_task_coeff;
	static std::vector<uint32_t> s_shilian_seq;
	static std::map<uint32_t, uint32_t> s_quick_task_list;
};
typedef boost::shared_ptr<task_manager_t> task_mgr_ptr;
#endif // !__TASK_H__

