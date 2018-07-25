#ifndef __GAME_ROLE_H__
#define __GAME_ROLE_H__

#include "object/object_position.hpp"
#include "main/game_server.hpp"
#include "main/game_server_fwd.hpp"
#include "item/item.hpp"
#include "hero/hero.hpp"
#include "formation/formation.hpp"
#include "common.pb.h"
#include "gate/gate_manager.hpp"
#include "item/package_manager.hpp"
#include "trade/role_trade.hpp"
#include "money.hpp"
#include "formation/formation_manager.hpp"
#include "role_scene_info.hpp"
#include "mount/mount.hpp"
#include "task/task_manager.hpp"
#include "task/bounty_manager.hpp"
#include "activity/activity_manager.hpp"
#include "fight_study/fight_study_manager.hpp"
#include "arena/arena.hpp"
#include "common/simple_timer.hpp"
#include "achieve/achieve_manager.hpp"
#include "tower/tower.hpp"
#include "luckydraw/luckydraw.hpp"
#include "fight/role_combat_result.hpp"
#include "dungeon/role_dungeon_data.hpp"
#include "formation/tactic_manager.hpp"
#include "role_buff/role_buff_manager.hpp"
#include "expedition/expedition.hpp"
#include "achieve/active_reward.hpp"
#include "shop/role_random_shop.hpp"
#include "level_reward/role_level_reward.hpp"
#include "level_reward/role_level_reward_mail.hpp"
#include "business_activity/business_activity_manager.hpp"
#include "liandan/liandan_manager.hpp"
#include "family_skill/family_skill_manager.hpp"
#include "equip/equip_plan_manager.hpp"
#include "patrol_hire/patrol_hire.hpp"
#include "business_activity/activity_shop.hpp"
#include "general_event/general_event_manager.hpp"
#include "friend/friendliness.h"
#include "recharge/recharge.hpp"
#include "office/office.hpp"

//typedef boost::shared_ptr<task_manager_t> task_mgr_ptr;
class script_role_t;
class formation_t;
typedef boost::shared_ptr<formation_t> formation_ptr;

enum ROLE_STATE : uint32_t
{
	ROLE_STATE_NONE,
	ROLE_STATE_LOADING,     //登陆获取数据中
	ROLE_STATE_OK          //登陆获取数据成功
};

enum role_job
{
	role_job_qingyun = 3,
	role_job_baihua = 7,
};

enum NEW_ROLE_GUIDE_STATUS : uint32_t
{
    NEW_ROLE_GUIDE_STATUS_DOING         = 1, //待完成
    NEW_ROLE_GUIDE_STATUS_READY_DONE    = 2, //准备完成（需要等待客户端的完成消息，才能算真正完成）
    NEW_ROLE_GUIDE_STATUS_REALLY_DONE   = 3, //真正完成（收到客户端的完成消息或为不需要等待的新手引导）
};

struct new_role_guide_t
{
    uint32_t tid                    = 0;
    bool is_not_waiting             = true; //false--需要等待客户端返回完成状态；true--直接完成，无需等待
	uint32_t event_type				= 1;	//1--新手引导;2--剧情
    NEW_ROLE_GUIDE_STATUS status    = NEW_ROLE_GUIDE_STATUS_DOING;
};

class role_t : public object_base_t
{
    /*
    logic:  角色自身数据相关
    author: ervery one
    */
public:
    role_t(uint64_t uid);
    virtual ~role_t();

	virtual void del_clear();

    template<typename T_MSG>
    void send_msg_to_client(uint16_t cmd, const T_MSG& protobuf_msg)
    {
        const network::tcp_socket_ptr& p_gate_socket = gate_manager_t::get_gate_socket_by_id(m_gate_id, m_server_id);
        if (NULL == p_gate_socket)
        {
            log_error("NULL == p_gate_socket: send cmd[%d]", cmd);
            return;
        }
        env::server->send_msg_to_gate(p_gate_socket, cmd, m_uid, protobuf_msg);
    }

	template<typename T_MSG>
	void send_msg_to_gate(uint16_t cmd, const T_MSG& protobuf_msg)
	{
		const network::tcp_socket_ptr& p_gate_socket = gate_manager_t::get_gate_socket_by_id(m_gate_id, m_server_id);
		if (NULL == p_gate_socket)
		{
			log_error("NULL == p_gate_socket [%u][%u]", m_gate_id, m_server_id);
			return;
		}

		env::server->send_msg_to_gate(p_gate_socket, cmd, m_uid, protobuf_msg);
	}

    virtual role_ptr get_role() { return boost::dynamic_pointer_cast<role_t>(shared_from_this()); } // 获取角色指针
    virtual proto::common::EM_SCENEOBJECT_TYPE get_object_type() { return proto::common::SCENEOBJECT_USER; }
    virtual bool is_role() { return true; } // 是否是角色
    virtual void get_object_info(proto::common::object_common_info* object_info);

	void load_role_data(const proto::common::role_data& rd);
	void load_cross_data(const proto::common::role_data& rd);

    void peek_all_datas(proto::client::gc_enter_game_reply* egn);
    void peek_data(proto::common::role_data* data, bool is_to_client);
    void peek_global_user_data(proto::server::global_user_data* data);
	void peek_cross_data(proto::common::role_cross_data* data, proto::common::role_cross_type type);
	void peek_fight_data(proto::common::mirror_fight_data* data);

    void save_all_datas();
    void save_other_datas();
    void save_self(bool is_right_now = false);
	bool is_new_role() { return m_is_new_role; }

	// 发送错误码
	void send_error_code( uint32_t code );

    void get_personal_info(proto::common::personal_info* personal);
    void set_personal_info(const proto::common::personal_info& personal);

	//////////////////////////////////////////////////////////////////////////
	// 是否是登陆
	bool is_login_scene() const { return m_login_type != proto::server::scene_change_none; }
	// 设置登陆类型
	void set_login_type(proto::server::scene_change_type type) { m_login_type = type; }

	// 处理中心服下发数据
	void set_center_user_data(const proto::server::eg_reg_user_scene_request& data);

	// 开始进入场景
	bool reg_scene();

	// 加载读档数据
	void load_all_datas(proto::server::dg_role_data_reply const& rdr);

	// 下发人物数据，登陆场景
	bool do_login_scene(const scene_ptr& p_login_scene);

	// 准备登陆场景
	void ready_enter_scene();

	// 登出
	void leave_game();

	// 离线
	void offline();

	// 通知其他服务器登陆场景
	void notify_other_server_ready_enter_scene();

	// 通知其他服务器人物登陆
	void notify_other_server_role_login_scene();

	// 通知其他服务器删除人物
	void notify_the_other_servers_del_role();

	//////////////////////////////////////////////////////////////////////////

	void notify_new_guide(); //zwx

    void init_new_role_datas(uint32_t role_type); //新角色初始化

    inline const std::string& get_key() { return m_key; }

    inline uint64_t get_uid() { return get_object_id(); }

    inline std::string get_name() { return m_name; }
    inline void set_name(std::string name) { m_name = name; }

    inline void set_gm(uint32_t gm) { m_gm = gm; }
    inline bool is_gm() { return m_gm >= 1 ? true : false; }
    inline bool is_super_gm() { return m_gm >= 2 ? true : false; }

    inline ROLE_STATE get_state() const { return m_state; }
    inline void set_state(ROLE_STATE st) { m_state = st; }

    inline uint32_t get_gate_id() const { return m_gate_id; }
	inline void set_gate_id(uint32_t val, uint32_t server_id) { m_gate_id = val; set_server_id(server_id); }

	inline uint32_t get_server_id() const { return m_server_id; }
	inline void set_server_id(uint32_t val) { m_server_id = val; }
	void syn_cross_id(uint32_t cross_id);

    inline uint32_t get_create_time() const { return m_create_time; }

    inline void set_last_login_time(uint32_t val) { m_last_login_time = val; }
    inline uint32_t get_last_login_time() const { return m_last_login_time; }

    void set_last_logout_time(uint32_t logout_time);
    inline uint32_t get_last_logout_time() const { return m_last_logout_time; }

    inline void set_online_time(uint32_t val) { m_online_time = val; }
    inline uint32_t get_online_time() const { return m_online_time; }

    inline void set_cumulative_login_days(uint32_t val) { m_cumulative_login_days = val; }
    inline uint32_t get_cumulative_login_days() const { return m_cumulative_login_days; }

    inline void set_continue_login_days(uint32_t val) { m_continue_login_days = val; }
    inline uint32_t get_continue_login_days() const { return m_continue_login_days; }

    inline void set_online_state(ROLE_ONLINE_STATE val) { m_online_state = val; }
    inline ROLE_ONLINE_STATE get_online_state() const { return m_online_state; }

    inline void set_role_type(uint32_t role_type) { m_role_type = role_type; }

    inline script_role_t* get_script_role() const { return m_script_role; }

	virtual bool is_auto_ignore_lineup(uint32_t fight_type);

	// 跨天（是否跨周）
    void on_next_day(uint32_t curr_time, bool is_pass_week);
	// 跨天处理（是否是在线处理还是登陆处理）
    void do_next_day(uint32_t curr_time, bool is_online);
	// 跨周处理
    void do_next_week(uint32_t curr_time);

    // 经验等级
    uint32_t get_exp() { return m_exp; }
    virtual uint32_t get_level() { return m_level; }
    bool add_exp(uint32_t add_val, uint32_t source_type, uint32_t source_param, proto::common::role_change_data* p_data = NULL);
    void set_exp(uint32_t set_val, uint32_t source_typ, uint32_t source_param, proto::common::role_change_data* p_data = NULL);
    void set_level(uint32_t set_val, uint32_t source_type, uint32_t source_param, proto::common::role_change_data* p_data = NULL);
    void set_exp_ex(uint32_t set_val, uint32_t source_typ, uint32_t source_param, proto::common::role_change_data* p_data = NULL);

	//限制升级
	bool is_limit_level();
	uint32_t get_limit_exp();

    // 待实现
    uint32_t get_hp() { return 0; }
    uint32_t get_max_hp() { return 0; }

    // 更新登陆时间
    void update_login_time();

	// 得到特权等级（目前没有）
	uint32_t get_privilege_level() const { return 1; }

	///=====================================================================
	///						战斗数据相关填充接口开始
	///=====================================================================
	virtual bool fill_fight_camp(
		uint32_t camp_index,
		uint64_t unique_offset,
		uint32_t team_member_size,
		proto::common::fight_camp* camp,
		proto::common::fight_type fight_type,
		proto::common::hero_state_data* state,
		int32_t team_member_pos = -1);

	virtual uint32_t get_camp_unique_offset(uint32_t camp);

	virtual hero_map* get_hero_list();

	bool fill_obj_single(
		uint32_t camp_index, 
		proto::common::obj_single* single, 
		proto::common::fight_type fight_type,
		uint32_t team_member_size,
		int32_t team_member_pos);

	virtual item_ptr get_item_by_uid(uint64_t uid, proto::common::package_type package);

	virtual bool get_hero_list(uint32_t fight_type, uint32_t team_member_size, std::vector<uint64_t>& list);

	formation_ptr get_form_data(uint32_t fight_type, uint32_t team_member_size);

	// 填充武将列表
	virtual bool fill_hero_data(
		uint32_t camp_index,
		uint64_t unique_offset,
		uint32_t team_member_size,
		proto::common::fight_type fight_type,
		proto::common::hero_data* fhd,
		proto::common::hero_state_data* state = NULL,
		int32_t team_member_pos = -1);

	bool fill_hero_single(
		uint32_t camp_index,
		uint64_t unique_offset,
		hero_ptr p_hero,
		proto::common::hero_single* single,
		uint32_t pos,
		proto::common::fight_type fight_type,
		proto::common::hero_state* state);

	bool fill_hero_skill(
		proto::common::fight_type fight_type, 
		hero_ptr p_hero, 
		proto::common::hero_single* single);

	bool fill_hero_skill_data(
		proto::common::skill_single* skill,
		proto::common::fight_type fight_type, 
		uint64_t hero_unique_id, 
		uint32_t skill_id,
		uint32_t skill_level, 
		uint32_t skill_type);

	virtual bool get_team_member_list(std::vector<object_base_ptr> &team_member_list);
	virtual bool is_team_leader();
	virtual uint32_t get_team_member_size();

	void fill_hero_to_formation(formation_ptr& form, uint32_t team_member_size, bool needleader);

	virtual hero_ptr get_hero_by_unique_id(uint64_t unique_id);
	///=====================================================================
	///						战斗数据相关填充接口结束
	///=====================================================================

    //初始化新手引导
    void init_new_role_guide(uint32_t guide_tid, bool is_not_waiting,uint32_t event_type);
    //完成新手引导 来自脚本
    void finish_new_role_guide_from_script(uint32_t guide_tid);
    //
    void traverse_guide_id(std::function<void(uint32_t)> func);
    //完成新手引导 来自客户端
    void finish_new_role_guide_from_client(uint32_t guide_tid);
    //脚本事件：升级
    void script_event_level_up(uint32_t level);
    //脚本事件：创建道具
    void script_event_create_item(uint32_t item_itd);
    //脚本事件：进入区域
    void script_event_enter_area(uint32_t area_itd);
	//脚本事件：进入指定对阵的指定回合(对阵ID，回合数)
	void script_event_enter_fight(uint32_t fight_tid, uint32_t round_tid);
	//脚本事件：进入指定对阵的指定回合部署战斗(对阵ID，回合数)
	void script_event_ready_fight(uint32_t fight_tid, uint32_t round_tid);
	//脚本事件：战斗结束
	void script_event_combat_end(uint32_t fight_tid);
	//脚本事件：使用指定道具(道具ID)
	void script_event_use_item(uint32_t item_tid);
	//脚本事件: 新角色进入场景
	void script_event_new_role_enter_scene();
	//脚本事件: 完成一个新手引导调用(用于触发后续的连续事件)
	void script_event_finish_new_guide(uint32_t guide_tid);

	bool init_new_guide();

	void init_gm_level();
private:
    std::string m_key = "";                 //cache key
    uint64_t m_uid = 0;                     //角色唯一ID
    ROLE_STATE m_state = ROLE_STATE_NONE;   //角色状态
    std::string m_name = "";                //角色名
    int32_t m_gm = 0;                       //该角色是否有gm权限
    uint32_t m_create_time = 0;             //角色创建时间
    uint32_t m_last_login_time = 0;         //角色上一次的登陆时间
    uint32_t m_last_logout_time = 0;        //角色上一次的登出时间
    uint32_t m_online_time = 0;             //角色在线时间
    uint32_t m_gate_id = 0;                 //角色所在的gate_server的id
	uint32_t m_server_id = 0;				//跨服时server_id不一定是本服server_id
    script_role_t* m_script_role = NULL;    //角色的脚本指针
    uint32_t m_mode_id = 0;                 //角色模型ID
    proto::common::ROLE_MODE m_role_mode = proto::common::ROLE_MODE_USER;   //角色模型状态
	ROLE_ONLINE_STATE m_online_state = ROLE_ONLINE_STATE_LOGOUT;               //角色是否下线(但不会删除缓存)
	bool m_is_new_role = false;             //是否是新用户
    uint32_t m_role_type = 0;               //创建角色的序号HeroJobTransfer表的key
	uint32_t m_exp = 0;						//经验
	uint32_t m_level = 0;					//等级
    uint32_t m_continue_login_days = 0;		//连续登陆天数
    uint32_t m_cumulative_login_days = 0;	//累计登陆天数
	uint32_t m_update_day_time = 0;         //角色上一次跨天处理的时间
	uint32_t m_update_week_time = 0;        //角色上一次跨周处理的时间
    std::map<uint32_t, new_role_guide_t> m_new_role_guide_list; //新手引导列表
	proto::server::scene_change_type m_login_type = proto::server::scene_change_none; // 场景登陆类型

	//玩家扩展经验， 用于卡级
	uint32_t m_exp_ex = 0;		//扩展经验

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /*
    logic:  任务系统
    author: 方剑
    */
public:
    inline task_mgr_ptr get_task_mgr(){ return m_task_mgr;}
	inline bounty_mgr_ptr get_bounty_mgr() { return m_bounty_mgr; }
	inline activity_mgr_ptr get_activity_mgr() { return m_activity_mgr; }
	void script_event_task_accept(uint32_t task_id);
	void script_event_task_finish(uint32_t task_id);
	void accept_sync_task(const proto::common::task_state &task_info, proto::common::role_change_data &rcd);
	void drop_circle_task(uint32_t task_type);
private:
    task_mgr_ptr m_task_mgr;
	bounty_mgr_ptr m_bounty_mgr;
	activity_mgr_ptr m_activity_mgr;
    /*
    logic:  布阵系统
    author: 王斌
    */
public:
	formation_manager_t m_formation_mgr;
    /*
    logic:  战斗系统(回合制pve\pvp、即时制)
    author: 王斌
    */
    /*
    logic:  战斗结算缓存
    author: 王山月
    */
	role_combat_result m_combat_result;
public:
//     void set_fight_data(const proto::common::fight_data& data); //保存战斗数据 wsy 注释 旧的战斗
//     proto::common::fight_data& get_fight_data();                //获取战斗数据 wsy 注释 旧的战斗
	uint32_t get_fighting() { return m_fighting; }                                   //???
	void calc_fighting();
	uint32_t get_charm() { return m_charm; }
	void set_charm(uint32_t value) { m_charm = value; }
	uint64_t get_fight_uid() { return m_role_fight_data.fight_uid(); }
	virtual void set_fight_uid(uint64_t uid);
	bool can_enter_fight(bool self = true);
	virtual void check_update_fighting(uint64_t hero_uid);
	bool get_auto_fight_flag();
	void save_auto_fight_flag(uint32_t auto_fight_flag);
	bool get_ignore_lineup_flag();
	void save_ignore_lineup_flag(uint32_t ignore_lineup_flag);
	uint64_t get_watching_fight_uid() { return m_role_fight_data.watching_fight_uid(); }
	uint32_t set_watching_fight_uid(uint64_t uid, bool need_delete=true);
    proto::common::role_fight_data& get_role_fight_data() { return m_role_fight_data; }

	void combat_notify_round(uint64_t against_id, uint32_t round);
	void combat_notify_prepare(uint64_t against_id, uint32_t round);
public:
	fight_study_manager_t m_fight_study_mgr;

private:
//    proto::common::fight_data		m_fight_data;		// 基于客户端计算的战斗数据 wsy 注释 旧的战斗
	proto::common::role_fight_data	m_role_fight_data;	// 新版战斗数据
	uint32_t m_fighting = 0;
	uint32_t m_charm = 0;
    /*
    logic:  聊天系统
    author: 王山月
    */
public:
    inline uint32_t get_chat_channel() { return m_chat_channel; }
    inline void set_chat_channel(uint32_t val) { m_chat_channel = val; }
private:
    uint32_t m_chat_channel = 0;    //当前聊天频道
	uint32_t m_luck_gift_gold = 0;  //返还的聊天红包金币数

    /*
    logic:  跑商系统
    author: 王山月
    */
public:
    inline role_trade_t& get_trade_info() { return m_trade; }
    inline void set_city_id(uint32_t city_id) { m_city_id = city_id; }
    uint32_t get_city_id() { return m_city_id; }
private:
    role_trade_t m_trade;
    uint32_t m_city_id = 0; //当前所在的城市

	/*
	logic: 坐骑
	author: 王山月
	*/
public:
    inline mount_data_ptr get_mount_data() { return m_mount_data; }
    inline mount_ptr get_cur_use_mount() 
    { 
        if (NULL != m_mount_data)
        {
            return m_mount_data->get_use_mount();
        }
        return mount_ptr();
    }

private:
	mount_data_ptr m_mount_data = mount_data_ptr();

    /*
    logic:  道具背包系统
    author: 王山月
    */
public:
    package_manager_t  m_package_manager;

	/*
	logic:  竞技场系统
	author: 王山月
	*/
public:
	arena_t& get_arena() { return m_arena; }
private:
	arena_t  m_arena;

	/*
	logic:  装备系统
	author: 方剑
	*/
public:
	uint32_t get_equip_smelt_value() { return m_equip_smelt_value; }
	void add_equip_smelt_value(uint32_t val) { m_equip_smelt_value += val; }

	void add_equip_smelt_box(uint32_t box_id) { m_equip_smelt_box_vec.push_back(box_id); }
	const std::vector<uint32_t>& get_equip_smelt_box_vec() { return m_equip_smelt_box_vec; }

	uint32_t get_equip_lucky() { return m_equip_lucky; }
	void set_equip_lucky(uint32_t val) { m_equip_lucky = val; }
private:
	std::vector<uint32_t>  m_equip_smelt_box_vec;
	uint32_t m_equip_smelt_value = 0;
	uint32_t m_equip_lucky = 0;
	/*
	logic:  角色状态
	author: 王山月
	*/
public:
	enum speed_update_type
	{
		speed_change_update = 0,	// 修改才更新
		speed_only_set = 1,			// 只设置不更新
		speed_forced_update = 2,	// 强制更新，不管有没改变速度
	};
	/**
	* \brief 速度相关
	*/
	void change_real_speed(speed_update_type type = speed_change_update);
	void update_troop_mount(proto::common::role_mode_state team_state, uint32_t speed);
	void update_troop_speed(uint32_t speed);

	// 得到速度
	uint32_t get_speed_by_type(proto::common::role_mode_state state);

	void set_buff_speed(int32_t speed) { m_buff_speed = speed; }
	int32_t get_buff_speed() const { return m_buff_speed; }

	void set_buff_prec_speed(int32_t speed) { m_buff_prec_speed = speed; }
	int32_t get_buff_prec_speed() const { return m_buff_prec_speed; }

	/**
	 * \brief 改变状态
	 * \brief state_param 状态辅助参数
	 */
	uint32_t change_object_state(proto::common::object_state_type new_state, uint64_t state_param = 0);
	// 状态辅助参数
	uint64_t get_object_state_param() const { return m_obj_state_param; }
	// 离开状态
	void on_leave_object_state(proto::common::object_state_type old_state);

	// 客户端上马下马独立出来
	uint32_t toggle_ride_mount(proto::common::role_mode_state new_state);
	// 设置跑商状态
	uint32_t toggle_trade_state(proto::common::role_mode_state new_state);

	proto::common::role_mode_state get_mode_state() const { return m_mode_state; }
	
	// 死亡处理
	virtual void on_object_die();
	
	// 被复活回调
	virtual void on_object_revive();

	// 设置复活间隔
	inline void set_revive_time(uint32_t val) { m_revive_time = val; }

	// 获得复活的时间
	uint32_t get_revive_time() const { return m_die_time + m_revive_time; }

	// 设置传送读条
	uint32_t start_transmission(proto::common::transmission_type type = proto::common::transmission_type_user);
	uint32_t get_transmission_type() const { return m_transmission_type; }

	// 是否在传送
	bool is_troo_transmission() const { return m_transmission_type == proto::common::transmission_type_troop; }
	// 主动取消传送读条
	void cancel_transmission();
	// 打断读条
	void do_break_transmission();
	// 打断读条操作
	void on_break_transmission(bool is_notify = true);

	// 传送定时器触发
	void on_transmission_timer();
	// 传送定时器触发
	static void transmission_callback(const boost::weak_ptr<role_t>& p_wrole, const boost::system::error_code& ec);

private:
	proto::common::role_mode_state m_mode_state = proto::common::role_mode_state_normal;	// 角色
	uint64_t m_obj_state_param = 0;		// 状态辅助参数
	uint32_t m_transmission_type = proto::common::transmission_type_none;
	boost::shared_ptr<boost::asio::deadline_timer> m_transmission_timer;

	// 设置死亡时间
	uint32_t m_die_time = 0;
	// 设置复活时间
	uint32_t m_revive_time = 0;

	int32_t m_buff_speed = 0;		// buf 固定值数值
	int32_t m_buff_prec_speed = 0; // buf 百分比数值

    /*
    logic:  武将系统
    author: 方剑
    */
public:
    virtual uint64_t get_main_hero_uid() { return m_main_hero_uid; }
	inline void set_main_hero_uid(uint64_t uid) { m_main_hero_uid = uid; }
	virtual uint32_t get_main_hero_tid() { return m_main_hero_tid; }
	void set_main_hero_tid(uint32_t tid);
	inline uint32_t get_main_hero_plugin() { return m_main_hero_plugin; }
	uint32_t get_sync_plugin();
	void set_main_hero_plugin(uint32_t plugin);
    inline uint32_t get_job() { return m_job; }
	inline void set_job(uint32_t val) { m_job = val; }
	void set_change_plugin(uint32_t val);
	void set_task_plugin(uint32_t val);
	uint32_t get_all_hero_star();
private:
    uint64_t m_main_hero_uid = 0; //属于角色的英雄
	uint32_t m_main_hero_tid = 0;
	uint32_t m_main_hero_plugin = 0;
	uint32_t m_job = 0;	//玩家职业（主角武将的职业）
	uint32_t m_change_plugin = 0;	//变身
	uint32_t m_task_plugin = 0;		//任务变身
public:
    hero_map m_hero_list;

    /*
    logic:  货币系统
    author: 方剑
    */
public:
	inline money_ptr get_money() { return m_money; }
private:
	money_ptr m_money;
	
	/*
	logic:  抽卡系统
	author: 方剑
	*/
public:
	inline luckydraw_ptr get_luckydraw() { return m_luckydraw; }
private:
	luckydraw_ptr m_luckydraw;

    /*
	logic:  炼丹系统
	author: 方剑
	*/
public:
	inline liandan_mgr_ptr get_liandan_mgr() { return m_liandan_mgr; }
private:
	liandan_mgr_ptr m_liandan_mgr;

	/*
	logic:  家族技能系统
	author: 方剑
	*/
public:
	inline family_skill_mgr_ptr get_family_skill_mgr() { return m_family_skill_mgr; }
private:
	family_skill_mgr_ptr m_family_skill_mgr;

    /*
	logic:  装备方案系统
	author: 方剑
	*/
public:
	inline equip_plan_mgr_ptr get_equip_plan_mgr() { return m_equip_plan_mgr; }
private:
	equip_plan_mgr_ptr m_equip_plan_mgr;

    /*
    logic:  场景管理
    author: 钱海江
    */
public:
	// 移动函数（）
	virtual void move_to(float x, float y, uint64_t time_stamp, float direction, uint32_t high = 0);
    // 进入场景
    virtual void on_enter_scene(scene_ptr p_scene);
    // 离开场景
    virtual void on_leave_scene(scene_ptr p_scene);
    // 移动处理
    virtual void on_move_event(scene_ptr p_scene);
	// 区域处理
	void do_something_for_area(scene_ptr p_scene);

    // 更新暗雷
    void update_area_trigger();
    // 通知周围玩家自己的场景信息
    void notify_object_common_info_for_nearbies();
	// 通知周围玩家自己的场景信息
	void notify_object_common_info_for_me();
	// 得到玩家区域id
	uint32_t get_role_area_id() { return m_role_scene.get_current_area_tid(); }
	// 得到玩家区域信息
	const role_scene_t& get_role_scene_info() { return m_role_scene; }
	// 客户端获取场景信息
	void role_enter_scene(uint64_t scene_id, proto::client::gc_enter_scene_reply& reply);

protected:
	role_scene_t m_role_scene;

    /*
    logic:  组队系统
    author: qhj
    */
public:
    uint64_t get_troop_id() const { return m_troop_id; }
	bool is_leader() { return is_team_leader(); }
	uint32_t get_troop_state() const { return m_troop_state; }

	uint32_t can_create_troop();
	uint32_t can_join_troop();
	// 判断是否是队员，并且没暂离
	bool is_troop_member_not_leave() 
	{ 
		return get_troop_id() != 0
			&& !is_team_leader() 
			&& m_troop_state == proto::common::troop_state_normal; 
	}
	// 队伍信息更新
	void update_troop_info(uint64_t troop_id, uint32_t state = 0, bool is_leader = false);

private:
    uint64_t m_troop_id = 0;
	bool m_is_leader = false;
	uint32_t m_troop_state = proto::common::troop_state_normal;

    /*
    logic:  家族系统
    author: 孙际文
    */
public:
	void join_family(uint64_t family_id, const std::string& family_name, uint32_t officer, uint32_t country_id, uint32_t family_lv);
	void leave_family();

    uint64_t get_family_id() const { return m_family_id; }
    const std::string& get_family_name() { return m_family_name; }

	void set_family_officer(uint32_t val);
	uint32_t get_family_officer() const { return m_family_officer; }

	void set_family_level(uint32_t val) { m_family_level = val; }
	uint32_t get_family_level() const { return m_family_level; }

	uint32_t get_country_id() const { return m_country_id; }
	void set_country_id(uint32_t country_id);
	void on_join_country();
	void set_country_officer(uint32_t val);
	uint32_t get_country_officer() const { return m_country_officer; }

	/*
		运营活动管理
	*/
	business_activity_manager_t &get_business_act_mgr() { return m_business_act_mgr; };
	business_activity_manager_t  m_business_act_mgr;
private:
    uint64_t m_family_id = 0;
    std::string m_family_name = "";
	uint32_t m_family_level = 0;
	uint32_t m_family_officer = 0;
	uint32_t m_country_id = 0;
	uint32_t m_country_officer = 0;
	
	//天下大势
public:
	general_event_manager_t &get_general_event() { return m_general_event; }
private:
	general_event_manager_t m_general_event;

    /*
    logic:  PK模式状态
    author: 待定
    */
public:
	uint32_t get_errantry() { return m_errantry; }
	void set_errantry(uint32_t val) { m_errantry = val; }
	void add_errantry(uint32_t val, uint32_t source_type, uint32_t source_param);

	uint32_t get_pk_count() { return m_pk_count; }
	void set_pk_count(uint32_t val) { m_pk_count = val; }
	uint32_t get_target_count() { return m_target_count; }
	void set_target_count(uint32_t val) { m_target_count = val; }
	uint32_t get_pk_value() { return m_pk_value; }
	void add_pk_value(uint32_t val, uint32_t source_type, uint32_t source_param);
	void del_pk_value(uint32_t val, uint32_t source_type, uint32_t source_param);
	//因为PK计算结果的PK值要按战斗开始的那一刻计算，所以要记录下来
	uint32_t get_calc_pk_value() { return m_calc_pk_value; }
	void record_calc_pk_value() { m_calc_pk_value = m_pk_value; }
    uint32_t get_pk_mode() const { return m_pk_mode; }
	uint32_t get_pk_mode_ex() const { return m_pk_ex; }
    uint32_t get_pk_punish_time() { return m_pk_punishment_time; }
    bool change_pk_mode(proto::common::EM_PK_MODE mode, uint32_t ex = 0); //改变PK状态
	//进入监狱
	void move_to_prison();
	// 降低PK值定时器触发
	void on_reduce_pk_value_timer();
	// 监狱定时器触发
	void on_prison_timer();
	// 是否在皇城
	bool is_in_king_city();
	// 降低PK值定时器触发
	static void on_reduce_pk_timer(const boost::weak_ptr<role_t>& p_wrole, const boost::system::error_code& ec);
	// 监狱定时器触发
	static void on_prison_timeout(const boost::weak_ptr<role_t>& p_wrole, const boost::system::error_code& ec);

public:
	//添加家族资金 
	void add_family_money(uint32_t money, uint32_t source_type, uint32_t source_param = 0);
	//添加国家资金
	void add_country_money(uint32_t money, uint32_t source_type, uint32_t source_param = 0);
private:
	uint32_t m_pk_value = 0;
	uint32_t m_calc_pk_value = 0;
    uint32_t m_pk_mode = proto::common::PK_PEACE; //PK模式
	uint32_t m_pk_ex = 0;
    uint32_t m_pk_punishment_time = 0; // 监狱惩罚时间
	uint32_t m_pk_value_reduce_time = 0; //上一次降低PK值的时间
	uint32_t m_pk_count = 0; //剩余攻击次数
	uint32_t m_target_count = 0; //剩余被攻击次数
	uint32_t m_errantry = 0; //侠义值
	uint32_t m_fight_end_time = 0;	// 战斗结算时间(用来判断状态)
	boost::shared_ptr<boost::asio::deadline_timer> m_pk_reduce_timer;
	boost::shared_ptr<boost::asio::deadline_timer> m_pk_punish_timer;
	/**
	 * \brief 王城战
	 */
public:
	bool is_in_king_war() const { return m_king_war_warfare > 0; }
	void set_king_war_warfare(uint32_t val) { m_king_war_warfare = val; }
	uint32_t get_king_war_warfare() const { return m_king_war_warfare; }

private:
	uint32_t m_king_war_warfare = 0;	// 战意值（有这个值表示正在王城战）

	/**
	 * \brief 家族战
	 */
public:
	bool is_in_family_war() const { return m_is_in_family_war; }
	void set_in_family_war(bool val);
	void set_family_war_heros(std::set<uint64_t>& heros);
	void add_family_war_hero(uint64_t hero_uid);
	void del_family_war_hero(uint64_t hero_uid);
	bool is_hero_active(uint64_t hero_uid);
	bool have_hero_live() { return m_family_war_hero.size() > 0; }
	const std::set<uint64_t>& get_family_war_hero() { return m_family_war_hero; }
private:
	bool m_is_in_family_war = false;	// 是否在家族战
	std::set<uint64_t> m_family_war_hero;

	/*
	*@ logic:  成就系统
	*@ author: 赵文学
	*/
public:
	std::map<uint32_t, achieve_ptr>& get_achieve_map() { return m_achieve_map; }
	inline uint32_t get_achieve_level() { return m_achieve_level; }
	inline void set_achieve_level(uint32_t val) { m_achieve_level = val; }
	inline uint32_t get_achieve_recv_reward_level() { return m_achieve_recv_reward_level; }
	inline void set_achieve_recv_reward_level(uint32_t val) { m_achieve_recv_reward_level = val; }

	//inline achieve_mgr_t get_achieve_mgr() { return m_achieve_mgr; }
	uint32_t get_statistical_id(uint32_t type, uint32_t eventid) { return (type * 1000) + eventid; }
	uint32_t get_statistical_type(uint32_t id) { return  id / 1000;  }
	uint32_t get_statistical_eventid(uint32_t id) { return id % 1000;  }
	void add_statistical_count( uint32_t id , uint32_t count, uint32_t option );
	uint32_t get_statistical_count(uint32_t id);
	void save_statistical_data();	//存储
	void load_statistical_data( const proto::common::statistical_save_data &data);	//存储
private:
	std::string m_statistical_key = "";
	std::map<uint32_t, achieve_ptr>	m_achieve_map;
	uint32_t m_achieve_level = 1;	//成就等级
	uint32_t m_achieve_recv_reward_level = 0;	//成就领取奖励等级 表示领取到第几级
	std::map<uint32_t, uint32_t> m_statistical_map;	//统计数据， ( type * 1000 + n ) key,
	/*
	logic:  爬塔系统
	author: 方剑
	*/
public:
	inline tower_ptr get_tower() { return m_tower; }
	
	inline void clear_tower_timer() { m_tower->del_auto_fight_timer(); }
private:
	tower_ptr m_tower;
	
	/*
	logic:  远征系统
	author: 方剑
	*/
public:
	inline expedition_ptr get_expedition() { return m_expedition; }

private:
	expedition_ptr m_expedition;
	/*
	*@ logic:  副本
	*@ author: qhj
	*/
public:
	inline role_dungeon_data_t& get_dungeon_data() { return m_role_dungeon_data_t; }

private:
	role_dungeon_data_t m_role_dungeon_data_t;

	/*
	*@ logic:  随机商店
	*@ author: qhj
	*/
public:
	inline role_random_shop_t& get_random_shop_data() { return m_role_random_shop; }

private:
	role_random_shop_t m_role_random_shop;
private:
	friendliness_t m_friendliness;
public:
	inline friendliness_t& get_friendliness_data() { return m_friendliness; }
	/*
	*@ logic:  亲密度
	*@ author: 王湄杰
	*/	
	/*
	 *@ logic:  命力  
	 *@ author: 赵文学 
	 */

public:
	void reset_lifestar_level() { m_lifestar_level = 0; }		//命星重置
	bool level_up_lifestar();									//命星升级
	bool level_up_lifestate(uint32_t& recode);					//境界升级
	bool level_up_lifelabel();									//命师称号升级

	uint32_t get_lifestar_level() { return m_lifestar_level; }
	uint32_t get_lifestate_level() { return m_lifestate_level; }
	uint32_t get_lifeforce_label() { return m_lifeforce_label; }
private:
	uint32_t m_lifestar_level = 0;	//命星等级(初始化 0)
	uint32_t m_lifestate_level = 1;	//境界等级(初始化 1)
	uint32_t m_lifeforce_label = 0;	//命师称号

	/*
	 *@ logic:  功能代码
	 *@ author: 方剑
	*/

public:
	//功能是否开启
	bool is_function_open(uint32_t func);
	//功能开启
	void open_function(uint32_t func);
	//功能关闭
	void close_function(uint32_t func);
	//开启所有功能
	void open_all_function();
private:
	uint64_t m_function_code = 0;	//功能代码(初始化 0)

	/*
	 *@ logic:  阵法代码
	 *@ author: 方剑
	*/

public:
	inline tactic_mgr_ptr get_tactic_mgr() { return m_tactic_mgr; }
private:
	tactic_mgr_ptr m_tactic_mgr;

	/*
	*@ logic:  BUFF代码
	*@ author: 方剑
	*/

public:
	inline role_buff_mgr_ptr get_role_buff_mgr() { return m_role_buff_mgr; }
	inline uint32_t get_buff_addexp() { return m_buff_addexp; }
	inline void set_buff_addexp(uint32_t val) { m_buff_addexp = val; }
private:
	role_buff_mgr_ptr m_role_buff_mgr;
	uint32_t m_buff_addexp = 0;
	/*
	*@ logic:  npc显示控制
	*@ author: qhj
	*/

public:
	void set_npc_code(uint32_t func);
private:
	uint32_t m_npc_code = 0;	//功能代码(初始化 0)

	/*
	*@ logic:  采集物显示控制
	*@ author: qhj
	*/

public:
	void set_collect_code(uint32_t func);
private:
	uint32_t m_collect_code = 0;	//功能代码(初始化 0)

	/*
	*@ logic:  章节大厅
	*@ author: zwx
	*/
public:
	std::set<uint32_t>& get_page_list() { return m_page_list; }
	std::map<uint32_t, uint32_t>& get_page_task_map() { return m_page_task_map; }
	std::map<uint32_t, uint32_t>& get_page_elite_map() { return m_page_elite_map; }
	std::set<uint32_t>&	get_page_elite_open_list() { return m_page_elite_open_list; }
	std::map<uint32_t, uint32_t>&	get_quick_elite_page_map() { return m_quick_elite_page_map; }
	std::map<uint32_t, uint32_t>&	get_reset_quick_elite_page_map() { return m_reset_quick_elite_page_map; }
private:

	std::set<uint32_t>					m_page_list;		///已经通关的章节ID

	std::map<uint32_t, uint32_t>		m_page_task_map;	///章节任务通关奖励列表 {章节任务表tid 领取状态(1:领取 0:未领取)}

	std::map<uint32_t, uint32_t>		m_page_elite_map;	///章节精英奖励列表{精英序列ID  领取状态(1:领取 0:未领取)}

	std::set<uint32_t>					m_page_elite_open_list; ///章节精英开启列表(可战斗的)

	std::map<uint32_t, uint32_t>		m_quick_elite_page_map;		///精英副本的扫荡次数

	std::map<uint32_t, uint32_t>		m_reset_quick_elite_page_map;	///精英副本的重置次数
	/*
	*@ logic:  活跃奖励(每日刷新的单日成就)
	*@ author: zwx
	*/
public:
	std::map<uint32_t, active_reward_ptr>& get_active_reward_map() { return m_active_reward_map; }

	std::set<uint32_t>&		get_active_reward_task_list() { return m_daily_task_list; }

private:
	///活跃奖励数据(存DB)
	std::map<uint32_t, active_reward_ptr>		m_active_reward_map;


	///每天固定任务列表
	std::set<uint32_t>							m_daily_task_list;
	
	/*
	*@ logic:  充值
	*@ author: fangjian
	*/
public:

	recharge_ptr get_recharge() { return m_recharge; }
private:
	recharge_ptr m_recharge;

	/*
	*@ logic:  逆命师
	*@ author: fangjian
	*/
public:

	office_ptr get_office() { return m_office; }
private:
	office_ptr m_office;
		/*
	*@ logic:  巡逻
	*@ author: fangjian
	*/
public:
	uint32_t	patrol_request(uint32_t map_id, uint32_t patrol);
	bool		set_patrol(bool val);
	void		troop_patrol(bool val);
	bool		is_patrol() { return m_role_scene.is_patrol_state(); }

	uint32_t	get_patrol_count() { return m_patrol_count; }
	void		set_patrol_count(uint32_t val) { m_patrol_count = val; }
	uint32_t	get_patrol_exp();
private:
	uint32_t	m_patrol_count = 0;

	/*
	*@ logic:  离线数据
	*@ author: fangjian
	*/
public:
	void		update_offline_data(const proto::common::offline_role_data& ord);

	/**
	 * \brief 等级奖励
	 * \auther qhj
	 */
public:
	inline role_level_reward_t& get_level_reward_data() { return m_role_level_reward; }
	uint32_t get_level_reward(uint32_t id, proto::common::role_change_data* p_data);

private:
	role_level_reward_t m_role_level_reward;

	/*
	*@ logic:  红包
	*@ author: fangjian
	*/
public:
	//通知领到红包
	void draw_notify(uint32_t money);
	//请求发送红包
	uint32_t send_redbag(uint32_t count, uint32_t money, proto::common::role_change_data* p_data);

	static uint32_t get_draw_min();

	static uint32_t get_draw_max();

	static proto::common::MONEY_TYPE get_redbag_money_type();

	/*
	 *@ 精力多倍奖励
	 *@ auther zwx
	 */
public:
	std::map<uint32_t, uint32_t>& get_multi_time_map() { return m_multi_time_list; }
	bool get_multi_state() { return m_multi_state; }
	void set_multi_state(bool state = false) { m_multi_state = state; }


public:
	/* 
	*@ logic:  等级奖励邮件
	*@ author: huangyang
	*/
	void check_send_level_reward_mail();

private:
	role_level_reward_mail_t m_role_level_reward_mail;

private:
	///多倍类型 结束时间(时间戳) 此列表目前约定最多三个不同种类的多倍类型
	std::map<uint32_t, uint32_t>	m_multi_time_list;

	bool m_multi_state = false;

	// 精力相关
public:
	// 得到玩家精力值
	uint32_t get_vigour();
	// 玩家精力基础上限
	uint32_t get_vigour_base_limit();
	// 玩家精力最大上限
	uint32_t get_vigour_max_limit();
	// 得到玩家最大精力次数
	uint32_t get_max_buy_times();
	// 增加精力值(调用这个接口之前先调用get_vigour，用来更新最新的精力值)
	void add_vigour(uint32_t val, uint32_t source_type, uint32_t source_param = 0);
	// 扣除精力值
	bool user_vigour(uint32_t val, uint32_t source_type, uint32_t source_param = 0);
	// 得到已经购买
	uint32_t get_vigour_buy_times() const { return m_vigour_buy; }
	// 增加购买次数
	void add_vigour_buy_times(uint32_t val = 1);
	// 清空精力购买次数
	void clear_vigour_buy_times(bool is_notify = true);

protected:
	uint32_t m_vigour = 0;					// 主角精力(一定要有个初始值，不然不会自动增加，自动增加开启时在扣的时候)
	uint32_t m_vigour_buy = 0;				// 主角精力购买次数
	uint32_t m_vigour_time = 0;				// 主角上次精力增加时间
	uint32_t m_next_add_time = 0;			// 主角下次精力增加时间

public:

	///同步武将以及装备信息到center 
	void update_role_info_to_center(proto::common::role_data_type type = proto::common::simple_info_type,bool is_save = true);


	/*
	*@ logic:  头像相关
	*@ author: zhaowenxue
	*/
public:
	///头像接口相关
	void		get_replace_head_frame(uint32_t type,uint32_t id);
	void		get_head_frame_list();

	///头像相关
	void		add_head_frame(proto::common::HEAD_FRAME_ADD_TYPE type,uint32_t param);
	void		init_head_frame(uint32_t head,uint32_t frame = 0);
	void		set_head_frame(uint32_t head, uint32_t frame = 0);
	void 		set_head(uint32_t val);
	void		set_frame(uint32_t val);

	void		add_head(uint32_t val);
	void		add_frame(uint32_t val);
	std::set<uint32_t>& get_head_list() { return m_head_list; }
	std::set<uint32_t>& get_frame_list() { return m_frame_list; }
	uint32_t	get_head_frame() { return m_head_frame; }

	//在线并且在场景
	bool is_client_in_scene() { return ((get_online_state() == common::ROLE_ONLINE_STATE_ONLINE) && (m_login_type == proto::server::scene_change_none)); }
private:
	uint32_t	get_frame();
	uint32_t	get_head();
	uint32_t	m_head_frame = 0;						///头像和边框ID  高16位为头像ID 低16位为边框ID
	
	///头像和边框数据库采用一个string存储 中间用|隔开然后头像和边框各自用:间隔 例如 "1:2:3|7:6:5"
	std::set<uint32_t>	m_head_list;			///头像列表
	std::set<uint32_t>	m_frame_list;			///边框列表


	/*
	logic:  雇佣巡逻
	author: 赵文学
	*/
public:
	inline patrol_hire_ptr get_patrol_hire() { return m_patrol_hire; }

private:
	patrol_hire_ptr m_patrol_hire;

	/*
	logic:  成长历程
	author: 方剑
	*/
public:
	uint32_t career_award(uint32_t career_id, proto::client::gc_career_award_reply& reply);

private:
	std::vector<uint32_t> m_career_id_vec;	//已经领取的成长历程奖励

	/*
	logic:  收集图鉴
	author: 方剑
	*/
public:
	uint32_t handbook_award(uint32_t handbook_id, proto::client::gc_handbook_award_reply& reply);

	const std::vector<uint32_t>& get_handbook() { return m_handbook_vec; }
private:
	std::vector<uint32_t> m_handbook_vec;	//已经领取的收集奖励

	/*
	logic:  改名
	author: 方剑
	*/
public:
	uint32_t change_name_request(const std::string& new_name);

	uint32_t get_change_name() { return m_change_name; }
	void set_change_name(uint32_t val) { m_change_name = val; }
private:
	uint32_t m_change_name;	//是否已经改名

	/*
	logic:  称号
	author: 方剑
	*/
public:
	void add_title(uint32_t title_id);

	uint32_t change_title(uint32_t title_id);

	// 增加临时称号，不记录数据库再特定场合使用(有些场合不需要多次广播)
	void change_temporary_title(uint32_t title_id, bool is_broadcast = true);

private:
	std::vector<uint32_t> m_title_id_vec;	//已经获得的称号
	uint32_t m_cur_title;

	// 是否显示临时称号
	bool m_show_temporary_title = false;
	uint32_t m_temporary_title = 0;
};
typedef boost::shared_ptr<role_t> role_ptr;

#endif // !__GAME_ROLE_H__
