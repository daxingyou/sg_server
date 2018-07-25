#ifndef __GAME_DUNGEON_SCENE_H__
#define __GAME_DUNGEON_SCENE_H__

#include "scene/scene.hpp"
#include "dungeon_common.hpp"
#include "dungeon_state.hpp"

class script_dungeon_t;

class dungeon_scene_t : public scene_t
{
public:
	dungeon_scene_t(uint64_t scene_id, uint32_t map_id, proto::server::em_scene_type scene_type, uint32_t type_param);
	virtual ~dungeon_scene_t();

	// 初始化放在这里
	virtual bool init();
	// 清空人和生物（删除地图首先调用）
	virtual void clear();
	// 启动场景
	virtual bool start();
	// 加载所有地图生物(静态npc有客户端自己生成)
	virtual void load_map_object();

	// 是否是副本
	virtual bool is_dungeon_scene() { return true; }
	// 获取副本场景指针
	virtual dungeon_scene_ptr get_dungeon_scene_ptr() { return boost::dynamic_pointer_cast<dungeon_scene_t>(shared_from_this()); }
	// 获得副本id
	virtual uint32_t get_dungeon_id() { return m_dungeon_id; }

	// 副本的一些信息
public:
	// 初始化
	virtual bool init_dugeon(uint32_t dungeon_id);
	void set_create_date(const proto::server::dungeon_create_data& data);

	bool is_can_restart();

	// 重新进入
	void restart_dungeon();

	// 副本定时器回调
	virtual void update(uint32_t curr_time);

	// 副本中怪物战斗结果
	void on_monster_fight_result(monster_ptr p_monster, bool is_win, uint32_t fight_time, uint32_t death_value);

	// 到达最大时间限制
	virtual bool reach_max_time(uint32_t curr) const { return m_last_time <= curr; }
	// 到达副本时间限制
	virtual bool reach_end_time(uint32_t curr) const { return 0 != m_end_time && m_end_time <= curr; }
	// 到达最大空闲时间
	bool reach_idle_time(uint32_t curr) const { return 0 != m_idle_time && m_idle_time <= curr; }
	// 设置开启空闲定时器
	void start_idle_time(uint32_t curr) { m_idle_time = curr + m_wait_time; }
	// 清楚空闲定时器
	void clear_idle_time() { m_idle_time = 0; }
	// 是否在空闲等待
	bool is_idle_wait() const { return m_idle_time != 0; }

	/*人物相关*/
public:
	virtual void on_role_enter(role_ptr p_role);
	virtual void on_role_leave(role_ptr p_role);

	void notify_dungeon_schedule(role_ptr p_role = NULL);

	// 自己被别人击杀
	virtual void on_other_kill_me(role_ptr p_role, object_base_ptr p_enemy);

	// 显示复活界面
	virtual void show_revive_ui(role_ptr p_role);

	// 登陆检测要不要给他复活
	virtual void on_login_die(role_ptr p_role);

	// 复活某玩家
	virtual void on_revive_role(role_ptr p_role);
	// 副本复活场景中所有玩家
	virtual void revive_all_role();

	// 队长进入开始战斗
	void start_fight();

	// 结算副本
	void start_settlement();

	// 验证是否达到通关要求
	void check_key_monster();

	// 超时未完成
	void failed_time_long();

	// 脚本调用召唤怪物
	void summon_dynamic_monster(uint32_t tid, uint32_t num);

	// 定时调用，延时召唤
	void check_daley_summon(uint32_t curr_time);

	// 得到怪物死亡次数
	uint32_t get_death_count(uint32_t tid);

	// 强制所有玩家离开场景
	virtual void forced_all_leave();

	/**
	* \brief 副本相关
	*/
public:
	struct schedule_monster_t
	{
		schedule_monster_t(uint32_t tid, proto::common::EM_MONSTER_TYPE type);

		// 增加一个进度怪物
		void add_schedule_monster(const monster_ptr& p_monster);
		// 增加计数
		void add_schedule_count(uint64_t object_id);
		// 增加计数
		void add_schedule_count(bool is_del = false);
		// 是否已经完成
		bool is_complete_schedule() { return m_data.curr_num() >= m_data.max_num(); }
		//
		void peek_data(proto::common::dungeon_schedule_data* p_data);

		uint32_t m_monster_tid = 0;
		proto::common::EM_MONSTER_TYPE m_monster_type = proto::common::EM_NONE;
		proto::common::dungeon_schedule_data m_data;
		uint32_t m_all_round_time = 0;	// 所有回合数战斗时间
		uint32_t m_all_death_score = 0;	// 所有死亡数得分
		uint32_t m_all_death_value = 0;	// 总的死亡次数

		std::list<uint64_t> m_monster_list;
	};
	typedef boost::shared_ptr<schedule_monster_t> schedule_monster_ptr;

	schedule_monster_ptr get_schedule_monster(uint32_t tid);
	void add_schedule_monster(const monster_ptr& p_monster);

	bool is_event_done(uint32_t id) const;
	void add_event_done(uint32_t id);
	void ask_do_event(uint32_t id);
	void add_schedule_count(uint32_t monster_tid, bool is_del);

	// 副本基本信息
protected:
	uint32_t m_last_time = 0;		// 最大持续时间，目前是一天
	uint32_t m_start_time = 0;		// 副本开始时间
	uint32_t m_end_time = 0;		// 结束时间(副本时间限制)
	uint32_t m_pass_time_limit = 0;	// 副本时间限制
	uint32_t m_idle_time = 0;		// 无人空闲时间
	uint32_t m_wait_time = 0;		// 无人空闲时间配置

	dungeon_state_controler m_state_controler;

	uint32_t m_dungeon_id = 0;		// 副本id
	dungeon::dungeon_type_enum m_dungeon_type = dungeon::dungeon_type_none;	// 副本类型
	int32_t m_revive_limit = 0;	// 复活次数限制
	uint32_t m_curr_revive_time = 0;// 当前复活次数
	dungeon::pass_type_enum m_pass_type = dungeon::pass_type_none;	// 通关类型
	uint32_t m_schedule_value = 0;	// 进度要求
	std::vector<schedule_monster_ptr> m_schedule_monster_vec; // 通关相关怪物列表

	// 死亡的怪物数量
	std::map<uint32_t, uint32_t> m_death_count;

	// 副本定时召唤怪物列表
	dungeon_summon_delay_vec m_delay_vec;
	dungeon_summon_dynamic_vec m_dynamic_vec;

	// 副本人物信息
protected:
	struct dugenon_create_t
	{
		proto::server::dungeon_create_type m_create_type = proto::server::dungeon_create_single;
		uint64_t m_id = 0;
	};

	dugenon_create_t m_dugenon_create;

	// 副本进度控制
protected:
	proto::common::dungeon_result_type m_dungeon_result = proto::common::dungeon_result_none;
	
	script_dungeon_t* m_script_dungeon = NULL;	// 副本脚本

	uint32_t m_restart_count = 0;				// 最多能够重复挑战次数

	// 副本事件
protected:
	typedef std::set<uint32_t> EventList;
	EventList m_eventList;
};

#endif
