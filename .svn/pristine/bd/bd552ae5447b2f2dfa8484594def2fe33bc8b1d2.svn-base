#ifndef __COMBAT_HPP__
#define __COMBAT_HPP__

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include "tblh/SkillEffectTable.tbls.h"
#include "fight_obj.hpp"
#include "common/simple_timer.hpp"
#include "fight_common.hpp"

class buff_t;
typedef boost::shared_ptr<buff_t> buff_ptr;

class fight_hero_t;
typedef boost::shared_ptr<fight_hero_t> fight_hero_ptr;
typedef boost::weak_ptr<fight_hero_t> fight_hero_wptr;

typedef std::map<uint64_t, fight_obj_ptr> fight_obj_map;
typedef std::map<uint64_t, fight_hero_ptr> fight_hero_map;

typedef std::map<uint32_t, uint64_t> pos_hero;

typedef std::map<uint32_t, proto::common::combat_action_data> process_map;
typedef boost::shared_ptr<boost::asio::deadline_timer> boost_timer;

struct fight_camp_data
{
	uint32_t camp_obj_type	= 0;
	bool initiative	= false;		// 先手
};

// 使用技能的参数信息
struct round_use_skill_info
{
	SkillEffectTable* skill_conf = NULL;
	fight_hero_ptr attacker = NULL;
	fight_hero_ptr select = NULL;
	proto::common::combat_act_type type;
	bool trigger_by_buff = false;
	bool can_counter = false;
};

// 回合制战斗
class combat_t : public boost::enable_shared_from_this<combat_t>
{
public:
	combat_t(uint64_t fight_uid);
	~combat_t();

	void close();

	// 初始化
	bool init(proto::common::fight_data& fd, bool can_form_before_fight, combat_type type);
	uint32_t gen_buff_id() { return m_gen_buff_id++; }

	combat_type get_combat_type() { return m_combat_type; }
	formation_ptr get_formation(uint32_t camp);
// 	// 修改阵法
// 	bool change_tactic(uint64_t role_uid, uint32_t tactic_id);
// 	// 修改阵容
// 	bool change_form(formation_ptr form);

	// 开始loading
	bool loading_start();
	// loading结束
	bool loading_end(uint64_t uid);
	// 战前布阵开始
	bool combat_before_form_start();
	// 战前布阵结束
	bool combat_before_form_end(uint64_t uid);
	// 部署
	bool disposition(uint64_t uid, const proto::common::combat_form_data& data);
	// 主动部署
	bool initiative_disposition(uint64_t uid, const proto::common::combat_form_data& data);
	// 战斗播放结束
	bool fight_play_end(uint64_t uid, uint32_t hero_att_round);
	// 玩家上线
	bool on_role_login(uint64_t uid);
	// 玩家下线
	bool on_role_logout(uint64_t uid);
	// 更新战斗
	bool update_fight(const proto::common::fight_state& state);
	bool update_fight(uint32_t camp, const proto::common::hero_state_data& state);
	// 请求逃跑
	bool request_escape(uint64_t uid);
	// 改变玩家自动战斗模式
	bool change_role_auto_fight(bool is_auto, uint64_t uid = 0, const proto::client::hero_auto_fight_data* hero_data = NULL);
	// 改变英雄自动战斗参数
	bool change_hero_auto_fight(const proto::client::hero_auto_fight_data* hero_data = NULL);
	// 自动部署
	bool auto_disposition(uint64_t obj_id = 0);
	// 战斗取消
	bool cancel();
	// 强制双方结束 失败
	bool all_fail();
	// 获取战斗开始时间
	uint32_t get_start_time() { return m_start_time; }
	// 获取战斗总时长
	uint32_t get_total_time() { return m_total_time; }

public:
	// 是否结束
	bool is_end();
	// 是否为单人pve
	bool is_single_pve();
	// 是否为竞技场pvp
	bool is_arena_pvp();
	// 是否为非竞技场pvp
	bool is_un_arena_pvp();
	// 获取战斗ID
	uint64_t get_fight_uid();
	// 获取回合数
	uint32_t get_round();
	// 获取武将攻击回合
	uint32_t get_hero_att_round();
	// 获取己方武将死亡数量
	uint32_t get_self_death_hero_num();
	// 获取武将受到的伤害
	uint32_t get_hero_injured(uint64_t hero_uid);
	// 获取战斗数据
	const proto::common::fight_data& get_fight_data();
	// 获取战斗类型
	proto::common::fight_type get_fight_type() { return m_fight_data.common().param().type(); }
	// 获取怒气
	uint32_t get_sp(uint64_t obj_id);
	// 设置怒气
	void set_sp(uint64_t obj_id, uint32_t value);
	void add_sp(uint64_t obj_id, uint32_t value);
	void sub_sp(uint64_t obj_id, uint32_t value);
	// 部署怒气
	void sub_disposition_sp(uint64_t obj_id, uint32_t value);
	uint32_t get_disposition_sp(uint64_t obj_id);
	// 增加出手次数
	void add_attack_count(uint64_t obj_id, uint32_t value = 1);
	// 是否pve
	bool is_pve(uint32_t& npc_camp);
	bool is_pve();
	// 获取优先出手的阵营
	uint32_t get_initiative_camp();
	// 获取role_uid的阵营
	uint32_t get_role_camp(uint64_t uid) const;
	// 获取阵亡武将，返回是否全部阵亡
	void get_self_death_hero(uint64_t role_uid, std::set<uint64_t>& death_hero_uids) const;
	
	// 获取战斗最大阀值
	int32_t get_max_att_speed();
	// 修改标记
	void set_mark_id(uint32_t mark_id, uint64_t fight_hero_uid);

protected:
	// loading定时器触发
	static void on_loading_timer(const boost::weak_ptr<combat_t> pw_combat, const boost::system::error_code& ec);
	// 战前布阵定时器触发
	static void on_form_timer(const boost::weak_ptr<combat_t> pw_combat, const boost::system::error_code& ec);
	// 战前布阵就绪定时器触发
	static void on_form_ready_timer(const boost::weak_ptr<combat_t> pw_combat, const boost::system::error_code& ec);
	// 部署就绪定时器触发
	static void on_disposition_ready_timer(const boost::weak_ptr<combat_t> pw_combat, const boost::system::error_code& ec);
	// 准备定时器触发
	static void on_prepare_timer(const boost::weak_ptr<combat_t> pw_combat, const boost::system::error_code& ec);
	// 播放定时器触发
	static void on_play_timer(const boost::weak_ptr<combat_t> pw_combat, const boost::system::error_code& ec);

	// loading定时器触发
	void deal_on_loading_timer();
	// 战前布阵定时器触发
	void deal_on_form_timer();
	// 战前布阵就绪定时器触发
	void deal_on_form_ready_timer();
	// 部署就绪定时器触发
	void deal_on_disposition_ready_timer();
	// 准备定时器触发
	void deal_on_prepare_timer();
	// 播放定时器触发
	void deal_on_play_timer();

protected:

	// 初始化战斗对象
	bool init_fight_obj(proto::common::obj_data* od);
	// 初始化loading阶段的战斗武将(不真正添加进战斗阵营里 只是做数据的初始化 发给客户端展示)
	bool init_loading_fight_hero(proto::common::hero_data* hd);
	// 初始化战斗武将
	bool init_fight_hero(proto::common::hero_data* hd);
	// 初始化辅助武将
	bool init_fight_help_hero(proto::common::hero_data* hd);
	// 自动loading
	bool auto_loading();
	// 自动播放
	bool auto_play();

protected:
	// 开战前整理双方阵容的布阵信息
	bool reset_all_camp_form_data();
	// 开战前整理单一阵容的布阵信息
	bool reset_single_camp_form_data(uint32_t camp);
	// 回合开始前
	bool round_start_before();
	// 回合开始
	bool round_start();
	// 战斗开始事件
	void fight_start_event();
	// 查找当前出手武将
	void find_cur_att_hero();
	// 开始准备
	bool prepare_start();
	// 准备结束
	bool prepare_end();
	// 开始播放
	bool play_start();
	// 播放结束
	bool play_end();
	// 回合结束
	bool round_end();
	// 战斗结束
	bool combat_end();
	// 添加先手队列
	void add_first_att_hero_list(fight_hero_ptr p_fight_hero);
	// 移除先手队列
	void remove_first_att_hero_list(fight_hero_ptr p_fight_hero);
	// 排序先手队列
	void sort_first_att_hero_list();
	// 排序基础速度值队列
	void sort_att_speed_list(std::vector<fight_hero_ptr> &res);

	// 检查事件
	void check_event(combat_event_step_type type);
	// 检查波次转换
	bool check_wave_change();
	// 检查隐藏武将事件
	void check_hide_event(combat_event_step_type type);
	// 添加隐藏武将到战斗
	void add_hide_hero_to_combat(fight_hero_ptr p_hero);
	// 检测战斗是否结束
	bool check_combat_end();
	// 检测loading是否结束
	bool check_loading_finish();
	// 检测部署是否结束
	bool check_disposition_finish();
	// 检测播放是否结束
	bool check_play_finish();

public:

	// 给本场战斗的玩家广播消息
	template<typename T_MSG>
	bool send_msg_to_all(uint16_t cmd, const T_MSG& protobuf_msg);
	// 给本场战斗的一方阵营玩家广播消息
	template<typename T_MSG>
	bool send_msg_to_camp(uint32_t camp, uint16_t cmd, const T_MSG& protobuf_msg);

	// 填充combat_data数据
	void fill_combat_data(
		proto::common::combat_data* data, 
		const proto::common::combat_action_data* process = NULL, 
		bool syn_fight_data = false);

	// 填充统计数据
	void fill_count_data(proto::common::combat_count_data* data);

	// 填充玩家状态
	void fill_combat_obj_state(proto::common::fight_obj_state_data* data);

	// 填充武将出售顺序
	void fill_combat_hero_att_order(proto::common::combat_hero_att_order* data, const std::vector<fight_hero_ptr>& res);

	// 填充战斗恢复数据
	void fill_combat_recovery_data(proto::common::fight_recovery_data* data);

	// 查找战斗对象
	fight_obj_ptr find_obj(uint64_t uid);
	// 查找战斗武将
	fight_hero_ptr find_hero(uint64_t uid);

	// 同步战斗数据
	void syn_combat_data(
		const proto::common::combat_action_data* process = NULL, 
		uint64_t uid = 0, 
		bool syn_fight_data = false);

	// 同步玩家状态
	void syn_combat_obj_state(uint64_t uid = 0);

	// 同步恢复战斗数据
	void syn_combat_recovery_data(uint64_t uid = 0);

	// 检测一方是否全部死亡
	bool check_camp_all_dead(uint32_t camp);
	// 获取存活武将数量
	uint32_t get_alive_hero_num(uint32_t camp);

protected:

	// 出手顺序(活的)
	bool get_alive_attack_order(std::vector<fight_hero_ptr>& res);
	// 全部武将的出手顺序
	bool get_all_attack_order(std::vector<fight_hero_ptr>& res);
	// 计算播放时间
	uint32_t get_fight_play_time(uint32_t round);
	// 到时结束战斗
	static void on_end_timer(const boost::weak_ptr<combat_t> pw_combat, const boost::system::error_code& ec);

public:

	// 获取所有战斗武将
	const fight_hero_map& get_all_heros() { return m_heros; }
	fight_hero_map& get_all_heros_ptr() { return m_heros; }
	// 获取技能攻击目标阵营武将
	const pos_hero* get_skill_target_camp_pos_hero(fight_hero_ptr attaker, SkillEffectTable* skill_conf);
	// 获取技能攻击目标阵营武将
	void get_skill_target_heroes(fight_hero_ptr attaker, SkillEffectTable* skill_conf, std::vector<fight_hero_ptr>& result);
	// 检测技能目标类型
	bool check_skill_target_type(SkillEffectTable* skill_conf, fight_hero_ptr attacker, fight_hero_ptr target, bool disposition = false);
	// 获取某阵营武将
	const pos_hero* get_camp_pos_hero(uint32_t camp);
	// 获取某阵营某位置武将
	fight_hero_ptr get_hero_by_camp_and_pos(uint32_t camp, uint32_t pos);
	// 获取某阵营某tid武将
	fight_hero_ptr get_hero_by_camp_and_tid(uint32_t camp, uint32_t tid);
	// 获取某阵营武将
	bool get_camp_heros(std::vector<fight_hero_ptr>& result, uint32_t camp, uint32_t except_pos = 0);
	// 获取某阵营存活武将
	bool get_camp_alive_heros(std::vector<fight_hero_ptr>& result, uint32_t camp, uint32_t except_pos = 0);
	// 清空敌方阵营死亡武将
	void clear_enemy_camp_death_heros();
	// 判断阵营是否是逃跑了
	bool is_escape_camp(uint32_t camp);
	// 获取战斗发起人的保存数据
	bool fill_initiator_save_data(proto::common::fight_save_data* p_data);
	// 获取NPC（阵营二）的保存数据 hero_uid为key，hp为value
	bool fill_enemy_save_data(proto::common::fight_save_data* p_data);
	// 获取场上空位置(1-9顺序查找)
	uint32_t get_empty_pos(uint32_t camp);
	// 查找目标
	fight_hero_ptr find_default_target(fight_hero_ptr attacker, SkillEffectTable* skill_conf);

	// 查找目标
	bool find_target(
		fight_hero_ptr attaker, 
		SkillEffectTable* skill_conf,
		std::vector<fight_hero_ptr>& targets, 
		std::vector<fight_hero_ptr>& spurting_heroes,
		fight_hero_ptr select = NULL);

	// 查找目标
	bool find_target_by_skill_conf(
		fight_hero_ptr attaker,
		SkillEffectTable* skill_conf,
		std::vector<fight_hero_ptr>& targets,
		std::vector<fight_hero_ptr>& spurting_heroes,
		fight_hero_ptr select = NULL);

	// 随机目标
	bool find_random_target(
		fight_hero_ptr attaker, 
		SkillEffectTable* skill_conf,
		std::vector<fight_hero_ptr>& targets, 
		fight_hero_ptr select,
		uint32_t num = 1,
		bool b_has_select = true);

	// 溅射目标
	bool find_spurting_target(
		fight_hero_ptr attaker,
		SkillEffectTable* skill_conf,
		std::vector<fight_hero_ptr>& targets,
		std::vector<fight_hero_ptr>& spurting_heroes,
		fight_hero_ptr select,
		uint32_t num = 1);

// 	// 所在列的所有目标
// 	bool find_vertical_target(
// 		fight_hero_ptr attaker,
// 		SkillEffectTable* skill_conf,
// 		std::vector<fight_hero_ptr>& targets,
// 		fight_hero_ptr select);
// 
// 	// 所在排的所有目标
// 	bool find_horizon_target(
// 		fight_hero_ptr attaker,
// 		SkillEffectTable* skill_conf,
// 		std::vector<fight_hero_ptr>& targets,
// 		fight_hero_ptr select);

	// 所有目标
	bool find_all_target(
		fight_hero_ptr attaker,
		SkillEffectTable* skill_conf,
		std::vector<fight_hero_ptr>& targets,
		fight_hero_ptr select);

	// 血量最少的目标(可能随机)
	bool find_min_hp_target(
		fight_hero_ptr attaker,
		SkillEffectTable* skill_conf,
		std::vector<fight_hero_ptr>& targets,
		fight_hero_ptr select);

	// 血量最少的几个目标
	bool find_some_min_hp_target(
		fight_hero_ptr attaker,
		SkillEffectTable* skill_conf,
		std::vector<fight_hero_ptr>& targets,
		fight_hero_ptr select);

	// 混乱目标
	fight_hero_ptr select_confused_target(fight_hero_ptr attaker);

	// 攻击力最高的目标  
	fight_hero_ptr select_max_atk_target(fight_hero_ptr attaker, SkillEffectTable* skill_conf);
	// 职业为4、5、6的目标（脆皮）
	fight_hero_ptr select_weak_target(fight_hero_ptr attaker, SkillEffectTable* skill_conf);

	// 血量最少的1个目标
	fight_hero_ptr select_min_hp_target(fight_hero_ptr attaker, SkillEffectTable* skill_conf);
	// 血量最少的1个目标
	fight_hero_ptr select_min_hp_target(uint32_t camp);
	// 血量最少的几个目标
	bool select_min_hp_target(fight_hero_ptr attaker, SkillEffectTable* skill_conf, std::vector<fight_hero_ptr>& targets);

	// 行动条最低的1个目标
	fight_hero_ptr select_min_att_speed_target(uint32_t camp);

public:

	// 战斗武将死亡
	bool on_fight_hero_dead(fight_hero_ptr hero);
	// 获取当前过程
	proto::common::combat_action_data* get_cur_process();
	// 添加战斗行为
	void add_combat_act(proto::common::combat_act_type type, uint32_t value, uint64_t target, uint32_t remain_value);
	// 获取攻击步骤
	proto::common::combat_act_step get_attack_step() { return m_attack_step; }
	// 设置攻击步骤
	void set_attack_step(proto::common::combat_act_step step) { m_attack_step = step; }

protected:
	fight_hero_ptr get_attack_hero();
	uint64_t get_attack_obj_uid();

	// 检测阵法是否有效
	bool check_tactic_valid(uint32_t camp);
	// 执行阵法效果
	bool do_tactic_buff(uint32_t camp, bool is_add);

	//----------------------------------------打怪获得经验----------------------------------
public:
	// 获取对阵ID
	uint32_t get_against_id();

	// 获取经验
	std::pair<uint32_t,uint32_t> get_exp(bool is_win);

	// 获取组队人数
	uint32_t get_obj_count(uint32_t camp);

	// 获取玩家上阵的武将列表,参数为hero_uid的vector
	void get_role_heros(uint64_t role_uid, std::vector<uint64_t>& hero_list);

	// 获取杀死的怪物列表 任务用, 参数为怪物武将表中的task_sign任务标识的vector
	void get_task_monster(std::vector<uint32_t>& monster_list);

	void fight_hero_on_init();

	fight_hero_ptr get_att_fight_hero();
	void get_camp_fight_obj_uid(uint32_t camp, std::vector<uint64_t>& obj_uid_list);

	// 是否可观战
	bool can_watch(uint32_t watch_num = 0);
	// 添加观战玩家
	void add_watching_role(uint64_t uid) { m_watching_roles.insert(uid); }
	// 获取观战玩家
	const std::set<uint64_t>& get_watching_role_list() { return m_watching_roles; }
	// 移除观战玩家 
    // 调用该函数的外部不允许出现迭代器++!!!
	void remove_watching_role(uint64_t uid) { m_watching_roles.erase(uid); }
	// 移除全部观战玩家
	void remove_all_watching_roles();
	// 获取观战玩家数
	uint32_t get_watching_role_num() { return m_watching_roles.size(); }

	// 添加本回合使用的技能队列
	void add_round_use_skill_list(const round_use_skill_info& skill_info);
	// 执行本回合使用的技能队列
	void deal_round_use_skill_list();
	// 移除全部本回合使用的技能队列
	void remove_all_round_use_skill_list();

	uint32_t get_round_add_buff_count() { return m_round_add_buff_count; }
	void add_round_buff_add_count() { ++m_round_add_buff_count; }
private:

	uint64_t m_fight_uid = 0;															// 战斗ID
	proto::common::combat_step m_step = proto::common::combat_step_loading;				// 战斗进程
	proto::common::combat_state m_state = proto::common::combat_state_prepare;			// 回合状态
	combat_type	m_combat_type = combat_type_single_role;                                // 战斗类型 单人或组队
	uint32_t m_round = 1;																// 回合数
	uint32_t m_wave = 1;																// 波次
	uint32_t m_start_time = 0;															// 开始时间
	uint32_t m_total_time = 0;															// 战斗总时长	
	uint32_t m_hero_att_round = 0;														// 武将攻击回合
	uint32_t m_gen_buff_id = 1;															// buff分配ID
	uint32_t m_end_time = 0;															// 当前进程或者状态结束时间
	int32_t m_max_att_speed = 0;														// 本场战斗出手阀值
	uint32_t m_story_addi_time = 0;														// 战斗中的剧情影响的时间
	uint32_t m_round_add_buff_count = 0;												// buff安全校验
	bool m_can_form_before_fight = true;												// 是否可战前布阵
	fight_obj_map m_objs;																// 所有战斗对象
	fight_hero_map m_heros;																// 所有战斗武将
	fight_hero_map m_hide_heros;														// 隐藏战斗武将
	fight_hero_map m_wave_heros;														// 波次战斗武将
	proto::common::fight_data m_fight_data;												// 战斗初始数据
	std::map<uint32_t, pos_hero> m_form;												// 布阵
	std::vector<fight_hero_ptr> m_first_att_hero_list;									// 优先出手的武将
	std::vector<fight_hero_ptr> m_alive_hero_list;										// 活着的武将
	std::vector<round_use_skill_info> m_round_use_skill_list;							// 本回合即将使用的技能队列
	fight_hero_ptr m_p_att_fight_hero;													// 本回合攻击武将
	process_map m_process;																// 战斗过程
	proto::common::combat_act_step m_attack_step = proto::common::combat_act_step_none;	// 攻击步骤
	std::map<uint32_t, fight_camp_data> m_camp_data;									// 双方阵营数据
	proto::common::combat_result m_combat_result;										// 战斗结果
private:
	std::set<uint64_t>	m_watching_roles;													// 观战玩家
	simple_timer_t	m_timer;																// 定时器
	boost_timer		m_end_timer;															// 结束定时器
};

typedef boost::shared_ptr<combat_t> combat_ptr;
typedef boost::weak_ptr<combat_t> combat_wptr;


template<typename T_MSG>
bool combat_t::send_msg_to_all(uint16_t cmd, const T_MSG& protobuf_msg)
{
	for (auto objpair : m_objs)
	{
		if (NULL != objpair.second)
		{
			objpair.second->send_msg_to_client(cmd, protobuf_msg);
		}
	}
	std::set<uint64_t>::iterator iter = m_watching_roles.begin();
	for (; iter != m_watching_roles.end(); ++iter)
	{
		role_ptr role = role_manager_t::find_role(*iter);
		if (NULL == role)
		{
			return false;
		}

		role->send_msg_to_client(cmd, protobuf_msg);
	}

	return true;
}

template<typename T_MSG>
bool combat_t::send_msg_to_camp(uint32_t camp, uint16_t cmd, const T_MSG& protobuf_msg)
{
	for (auto objpair : m_objs)
	{
		if (NULL != objpair.second && objpair.second->get_camp() == camp)
		{
			objpair.second->send_msg_to_client(cmd, protobuf_msg);
		}
	}

	return true;
}

#endif//__COMBAT_HPP__