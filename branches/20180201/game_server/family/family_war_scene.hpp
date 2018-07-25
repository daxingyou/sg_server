#pragma once

#include "scene/scene.hpp"

#include <boost/asio/deadline_timer.hpp>
#include <boost/system/error_code.hpp>
#include "family_war_common.hpp"
#include "scene/scene_state.hpp"
#include "fight/combat.hpp"

class family_war_scene_t : public scene_t
{
public:
	family_war_scene_t(uint64_t scene_id, uint32_t map_id, proto::server::em_scene_type scene_type, uint32_t type_param);
	virtual ~family_war_scene_t() {};

 	// 初始化放在这里
 	virtual bool init();
 	// 清空人和生物（删除地图首先调用）
 	virtual void clear();
 	// 启动场景
 	virtual bool start();
	// 加载所有地图生物(静态npc有客户端自己生成)
	virtual void load_map_object();

	// 是否是家族战副本
	virtual bool is_family_war_scene() { return true; }
	// 获取副本场景指针
	virtual family_war_scene_ptr get_family_war_scene_ptr() { return boost::dynamic_pointer_cast<family_war_scene_t>(shared_from_this()); }

	/*场景相关*/
 public:
 	// 得到场景玩家进入后的出生坐标
 	virtual sPos get_scene_born_position(const role_ptr& p_role);

	// 到达最大时间限制
	bool reach_max_time(uint32_t curr) const { return m_last_time <= curr; }

	// 副本定时器回调
	virtual void update(uint32_t curr_time);

	virtual void forced_all_leave();

 	// 家族战的一些信息
public:
	// 家族战状态
	proto::common::family_war_state get_war_state() const { return m_state_type; }
	// 设置家族战副本信息
	void set_family_war_date(const proto::server::family_war_dungeon_data& data);
	// 备战家族战
	void prepare_family_war(uint32_t prepare_time);
 	// 开始家族战
 	void start_family_war(uint32_t end_time);
 	// 家族战战结束
 	void end_family_war(bool is_early_end = false);
	// 设置赢的家族
	uint64_t get_win_family() const { return m_win_family; }
	// 结算胜利家族
	family_war_side_type settlement_win_family();

	// 家族战id
	uint32_t get_family_war_id() const { return m_family_war_id; }

	// 家族战人数
	uint32_t get_family_war_member(family_war_side_type type);

	// 是否是参战家族
	family_war_side_type get_family_war_side_type(uint64_t family_id);

	// 得到积分
	uint32_t get_family_score(family_war_side_type type);

	// 得到敌对家族
	family_war_side_type get_enemy_side_type(family_war_side_type type);

	// 开战数据
	void peek_family_war_fight_info(proto::client::gc_family_war_fight_info_notify& notify);

	// 召唤神龙
	void family_war_summon_long(family_war_long_type type);

	// 全场景刷新家族战信息
	void notify_scene_war_info();

	// 家族战领日月台
	void family_occupied_platform(const role_ptr& p_role, family_war_platform_type platform_type, family_war_side_type side_type);

	// 家族战对战旗赵成伤害(返回类型是否继续)
	bool long_hurt_family_flag(family_war_side_type type, uint32_t hurt);

	// 家族战对战旗造成伤害(返回类型是否继续)
	bool platform_hurt_family_flag(family_war_platform_type platform_type, family_war_side_type type, uint32_t hurt);

	// 定时排名
	void rank_all_score();

	// 进攻龙柱
	uint32_t attack_long(const role_ptr& p_role);
	// 回合伤害通知
	void attack_long_round(uint64_t uid, uint64_t fight_id, const proto::common::fight_camp& enemy_camp, const combat_ptr& p_combat);
	// 战斗结束通知
	void attack_long_end(uint64_t uid, uint64_t fight_id, const combat_ptr& p_combat);
	// 通知龙柱死亡
	void family_war_long_death();

	// 得到title
	family_war_kill_title get_kill_title(uint32_t kill_num);
	// 击杀获得的奖励
	uint32_t get_reward_by_self_title(family_war_kill_title title);
	// 击杀称号玩家奖励
	uint32_t get_reward_by_other_title(family_war_kill_title title);
	// title name
	std::string get_title_name(family_war_kill_title title);

	// 得到龙柱id
	uint32_t get_long_type_monster_tid(family_war_long_type type);

	// 得到战况数据
	void peek_family_war_fight_data(uint64_t uid, proto::client::gc_family_war_fight_data_reply& reply);

	// 进入战场
	uint32_t role_enter_battlefield(role_ptr p_role);

	// 请求战斗结果
	void get_family_war_result(uint64_t uid, proto::client::gc_family_war_result_info_reply& reply);
 
 	/*人物相关*/
 public:
 	virtual void on_role_enter(role_ptr p_role);
 	virtual void on_role_leave(role_ptr p_role);
 
 	// 虚函数玩家离开场景中的一个区域的回调
 	virtual void role_leave_area(role_ptr p_role, uint32_t area_id);
 	// 虚函数玩家进入场景中的一个区域的回调
 	virtual void role_enter_area(role_ptr p_role, uint32_t area_id);
 
 	// 复活某玩家
 	virtual void on_revive_role(role_ptr p_role);
 
	// 有玩家杀了别人
	virtual void on_role_kill_other(role_ptr p_role, object_position_ptr p_enemy);

	// 自己被别人击杀
	virtual void on_other_kill_me(role_ptr p_role, object_position_ptr p_enemy);

 	// 显示复活界面
 	virtual void show_revive_ui(role_ptr p_role);
 
 	// 登陆检测要不要给他复活
 	virtual void on_login_die(role_ptr p_role);
 
public:
	// 家族战据点信息
	struct family_war_stronghold
	{
		uint64_t m_family_id = 0;
		uint32_t m_jump_id = 0;				// 出生和复活点
		uint32_t m_area_id = 0;				// 出生和复活点区域
		uint32_t m_war_id = 0;				// 战场的跳转点
		std::set<uint64_t> m_family_roles;	// 家族玩家
		uint64_t m_kill_max_uid = 0;		// 杀人最多
		uint32_t m_kill_max_cnt = 0;		// 最多杀人数
		uint64_t m_hurt_max_uid = 0;		// 伤害最多
		uint32_t m_hurt_max_cnt = 0;		// 最多伤害数
		uint32_t m_occupied_cnt = 0;		// 占领次数
		uint32_t m_fight_win_cnt = 0;		// 战斗胜利次数

		proto::client::family_war_fight_data m_data;

		void add_war_member(uint64_t uid);
		void del_war_member(uint64_t uid);

		bool is_flag_die() const;
		void hurt_flag(uint32_t val);

		void set_platform_state(family_war_platform_type type, bool is_occupied);

		void add_occupied_long();

		void add_score(uint32_t val);

		void add_fight_win_cnt();

		const std::string& get_family_name() const;
	};

	// 家族战龙柱信息
	struct family_war_long_info
	{
		uint64_t m_object_id = 0;
		uint32_t m_area_id = 0;
		uint32_t m_refresh_times = 0;
		family_war_long_state m_long_state = family_war_long_state_die;

		std::set<uint64_t> m_near_roles[family_war_side_max];	// 附近玩家

		void add_nearby_role(uint64_t uid, family_war_side_type type);
		void del_nearby_role(uint64_t uid, family_war_side_type type);

		// 龙威
		bool long_wei(const role_ptr& p_role, family_war_side_type type);
	};

	// 日月台信息
	struct family_war_platform_info
	{
		uint32_t m_interval = 0;
		uint32_t m_hurt = 0;
		uint64_t m_object_id = 0;
		family_war_side_type m_hurt_side = family_war_side_none;	// 对哪个家族战旗造成伤害
	};

	// 玩家信息
	struct family_war_role_info_t
	{
		uint64_t m_role_uid = 0;
		std::map<uint64_t, proto::common::family_war_hero_type> m_heros;
		proto::client::family_war_role_data m_data;
		uint32_t m_all_hurts = 0;
		uint32_t m_kill_count = 0;
		uint32_t m_death_count = 0;
		uint32_t m_kills = 0;
		family_war_kill_title m_kill_title = family_war_kill_title_0;

		bool m_update_hero = false;

		family_war_role_info_t(uint64_t uid);

		void init();

		void peek_hero_data(proto::client::gc_formations_info_notify& notify, bool peek_hero = true);

		void peek_hero_data(proto::client::gc_ask_formations_info_reply& notify);

		bool have_hero_live();

		void init_role_heros(const role_ptr& p_role);

		void notify_role_info(const role_ptr& p_role);

		// 上阵武将
		uint32_t role_hero_up(const role_ptr& p_role, uint64_t hero_uid);

		// 下阵武将
		uint32_t role_hero_down(const role_ptr& p_role, uint64_t hero_uid);

		// 复活武将
		uint32_t role_hero_revive(const role_ptr& p_role, uint64_t hero_uid);

		// 武将死亡
		void role_hero_die(const role_ptr& p_role, uint64_t hero_uid);

		// 更新排名
		void update_rank(uint32_t rank);

		// 增加积分
		void add_score(const role_ptr& p_role, uint32_t score);

		// 扣除行动力
		void less_action_point(const role_ptr& p_role, uint32_t val, bool is_update = true);

		// 增加杀人数
		void add_one_kill();

		// 增加死亡次数
		void add_self_death();
	};

	typedef boost::shared_ptr<family_war_role_info_t> family_war_role_info_ptr;
	typedef std::map<uint64_t, family_war_role_info_ptr> family_war_role_map;

	family_war_role_info_ptr get_family_war_role_info(uint64_t uid);

	family_war_role_info_ptr add_family_war_role_info(uint64_t uid);

	struct score_sort_t
	{
		uint64_t m_uid = 0;
		uint32_t m_score = 0;
		boost::weak_ptr<family_war_role_info_t> m_wptr;	// 为了加速索引

		score_sort_t(uint64_t uid, uint32_t score, const family_war_role_info_ptr& ptr) : m_uid(uid), m_score(score), m_wptr(ptr) {};

		static bool score_sort_ex(const score_sort_t& lhs, const score_sort_t& rhs);
	};

protected:
	uint32_t m_last_time = 0;		// 最大持续时间，目前是一天
	scene_state_controler m_state_controler;
	bool m_update_rank = false;		// 更新排名变化

protected:
	uint32_t m_family_war_id = 0;	// 家族战编号
	proto::common::family_war_state m_state_type = proto::common::family_war_state_end;
	uint32_t m_state_time = 0;		// 当前状态时间
	uint64_t m_win_family = 0;

	// 家族战龙柱信息
	family_war_long_info m_family_war_long_info;

	// 家族战家族信息
	family_war_stronghold m_stronghold_side[family_war_side_max];

	// 家族战玩家信息
	family_war_role_map m_family_war_role_map;

	// 日月台
	family_war_platform_info m_family_war_platform[family_war_platform_max];

	// 定时器
public:
	static void on_long_refresh_timer(const boost::weak_ptr<family_war_scene_t>& p_wlong, const boost::system::error_code& ec);
	void on_long_refresh();
	void on_update_long_state();

	static void on_platform_hurt_timer(const boost::weak_ptr<family_war_scene_t>& p_wlong, family_war_platform_type type, const boost::system::error_code& ec);
	void on_platform_hurt(family_war_platform_type type);
	
protected:
	boost::shared_ptr<boost::asio::deadline_timer> m_long_timer = NULL;	// 龙柱刷新定时器

	boost::shared_ptr<boost::asio::deadline_timer> m_platform_timer[family_war_platform_max] = { NULL };	// 龙柱刷新定时器
};