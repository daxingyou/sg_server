#ifndef __FIGHT_HERO_HPP__
#define __FIGHT_HERO_HPP__

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include "combat.hpp"
#include "buff_manager.hpp"
#include "buff_element_manager.hpp"
#include "fight_common.hpp"

class script_fight_hero_t;


struct fight_hero_round_data
{
	int damage = 0;
	int injured = 0;
};


struct fight_calc_result_t
{
	fight_calc_result_t(void)
	{
		reset();
	}

	~fight_calc_result_t(void) {};

	void reset()
	{
		dodge = false;
		crit = false;
		beheaded = false;
		immuno = false;
		damage = 0;
		heal = 0;
		block = 0;
		absorb = 0;
		leech = 0;
		reflect = 0;
		add_buff_id = 0;
		remove_buff_id = 0;
		over_layer_buff_id = 0;
		phase = 0;
		act_type = 0;
		skill = 0;
	}

	bool		dodge;
	bool		crit;
	bool		beheaded;
	bool		immuno;
	int32_t		damage;
	int32_t		heal;
	int32_t		block;
	int32_t		absorb;
	int32_t		leech;
	int32_t		reflect;
	uint32_t	add_buff_id;		// 添加的buff id
	uint32_t	remove_buff_id;		// 移除的buff id
	uint32_t	over_layer_buff_id;	// 层数溢出的buff id
	uint32_t    phase;
	uint32_t	act_type;		// 攻击类型(普通、追击、反击、连击)
	uint32_t	skill;			// 当前使用的技能ID
};

// 统计数据
struct stat_data
{
public:
	void add_heal(uint32_t value) { heal += value; }
	void add_damage(uint32_t value) { damage += value; }
	void add_accept_damage(uint32_t value) { accept_damage += value; }

	uint32_t get_heal_count() { return heal; }
	uint32_t get_damage_count() { return damage; }
	uint32_t get_accept_damage_count() { return accept_damage; }
private:
	uint32_t heal = 0;				// 治疗
	uint32_t damage = 0;			// 伤害
	uint32_t accept_damage = 0;		// 承受伤害
};

// 次数数据
struct count_data
{
public:
	void resetByRound()
	{
		counter_count = 0;						// 反击次数
		combo_count = 0;						// 连击次数
		trigger_skill_count = 0;				// 触发技能次数	
		one_round_injured = 0;					// 一个回合内受到的所有伤害
		one_round_damage = 0;					// 一个回合内造成的所有伤害
		other_action_count = 0;					// 其他武将行动次数
	}
	void reset()
	{
		trigger_skill_by_hp = 0;	// 血量触发技能次数
		trigger_skill_by_critical_blow = 0;	// 致命一击触发技能次数
		trigger_skill_by_beattack_count = 0;	// 被攻击触发技能次数
		trigger_skill_by_attack_count = 0;		// 攻击触发技能次数
		beattack_count = 0;	// 被攻击次数
		attack_count = 0;		// 攻击次数

		relive_count = 0;						// 复活次数

		counter_count = 0;						// 反击次数
		combo_count = 0;							// 连击次数
		trigger_skill_count = 0;					// 触发技能次数

		athanasia_count = 0;						// 不死次数
		add_buff_after_damage_count = 0;			// 受伤害后添加BUFF次数
		event_trigger_count = 0;					// 事件触发计数

		one_round_injured = 0;					// 一个回合内受到的所有伤害
		one_round_damage = 0;					// 一个回合内造成的所有伤害
		round_count = 0;						// 其他武将行动次数
		other_action_count = 0;					// 其他武将行动次数
		in_control_speed = 0;
		all_team_damage = 0;					// 伤害累计
		//death_guard_energy = 0;
		//max_death_guard_energy = 0;
	}
public:
	uint32_t trigger_skill_by_hp			= 0;	// 血量触发技能次数
	uint32_t trigger_skill_by_critical_blow	= 0;	// 致命一击触发技能次数
	uint32_t trigger_skill_by_beattack_count = 0;	// 被攻击触发技能次数
	uint32_t trigger_skill_by_attack_count	= 0;	// 攻击触发技能次数
	uint32_t beattack_count					= 0;	// 被攻击次数
	uint32_t attack_count					= 0;	// 攻击次数

	uint32_t relive_count					= 0;	// 复活次数

	uint32_t counter_count					= 0;	// 反击次数
	uint32_t combo_count					= 0;	// 连击次数
	uint32_t trigger_skill_count			= 0;	// 触发技能次数

	uint32_t athanasia_count				= 0;	// 不死次数
	uint32_t add_buff_after_damage_count	= 0;	// 受伤害后添加BUFF次数
	uint32_t event_trigger_count			= 0;	// 事件触发计数

	uint32_t one_round_injured = 0;					// 一个回合内受到的所有伤害
	uint32_t one_round_damage = 0;					// 一个回合内造成的所有伤害
	uint32_t round_count					= 0;	// 回合数

	uint32_t other_action_count				= 0;	// 其他武将行动次数
	uint32_t in_control_speed				= 0;	// 控制中速度
	uint32_t all_team_damage				= 0;	// 伤害累计
	uint32_t all_team_damage_turn			= 0;	// 伤害转换
	uint32_t shield_small_type				= 0;	// 盾小类型ID	
	uint32_t all_hero_shield_small_type		= 0;	// 全体盾小类型ID	
	//uint32_t death_guard_energy				= 0;	// 死亡守护能量	
	//uint32_t max_death_guard_energy			= 0;	// 死亡守护能量上限
};

struct skill_data
{
	uint32_t skill_id = 0;
	uint32_t skill_level = 0;
	proto::common::skill_type type;
};

struct taunt_data
{
	std::map<uint32_t, fight_hero_wptr> taunt_target_map;
	uint32_t taunt_buff_type = 0;
};

// 战斗武将
class fight_hero_t : public boost::enable_shared_from_this<fight_hero_t>
{
public:
	fight_hero_t();
	~fight_hero_t();

	void close();

	// 初始化
	bool init(combat_ptr combat, proto::common::hero_single* hs);

	void on_init();
	// 部署
	void set_cur_action(const proto::common::combat_form_single& action);
	// 清除上次部署
	void clear_cur_action();
	// 重置波次转换时的数据
	void reset_wave_change_data();

	// 是否死亡(此处命名略不好 判断血量 状态是否符合死亡条件)
	bool is_dead();
	// 是否死过了
	bool has_dead();
	// 是否免疫
	bool is_immuno();
	// 是否自动战斗
	bool is_auto_fight();
	// 修改自动战斗
	void change_auto_fight(uint32_t auto_fight_mode);
	// 获取自动战斗模式
	uint32_t get_auto_fight_mode();
	// 获取速度
	uint32_t get_speed();
	// 获取先攻值
	int32_t get_att_speed();
	// 获取排序先攻值
	int32_t get_order_att_speed();
	// 技能修改先攻值
	void skill_change_att_speed(int32_t change_value, fight_hero_ptr attacker, bool is_percent = false, uint32_t param1 = 0);
	// 修改先攻值
	void set_att_speed(int32_t att_speed);
	// 获取拉条溢出值
	int32_t get_att_more_speed();
	// 修改拉条溢出值
	void set_att_more_speed(int32_t value);
	// 修改拉条排序值
	void set_att_order_speed(int32_t value);
	// 获取位置
	uint32_t get_pos();
	// 获取主人
	uint64_t get_master();
	// 获取阵营
	uint32_t get_camp();
	// 获取敌对阵营
	uint32_t get_enemy_camp();
	// 获取武将类型
	uint32_t get_type(){	return m_hero_data.base_attr().type();	}
	// 获取UID
	uint64_t get_uid();
	const std::string& get_str_uid();
	// 获取武将唯一ID
	uint64_t get_unique_id();
	// 获取TID
	uint32_t get_tid();
	// 是否觉醒
	uint32_t get_wakeup();
	// 获取模型
	uint32_t get_plugin();
	// 获取脚本ID
	uint32_t get_script_id();
	// 获取攻击模式
	uint32_t get_atk_mode();
	// 获取系别
	uint32_t get_terry();
	// 获取性别
	uint32_t get_sex();
	// 获取职业
	uint32_t get_profession();
	bool is_weak_profession();
	// 获取最大连击数
	int get_max_combo_count();
	// 获取波次
	uint32_t get_wave_num();
	// 设置先手队列存在标记
	void set_first_att_order_pos(int32_t pos) { m_first_order_pos = pos; }
	int32_t get_first_att_order_pos() { return m_first_order_pos; }

	stat_data& get_stat_data() { return m_stat_data; }
	count_data& get_count_data() { return m_count_data; }
	const count_data& get_count_data() const { return m_count_data; }

	void set_is_story(bool is_story) { m_is_story = is_story; }
	bool is_story() { return m_is_story; }

	void add_skill(const int skill, const int level, const int init_cd, bool is_change_model = false);
	void remove_skill(const int skill);
	int get_skill_level(const int skill);
	// 获取默认技能
	int32_t get_default_skill();
	// 获取反击技能
	int32_t get_counter_skill();

	// 发送触发被动给自己
	void send_trigger_passive_to_user(uint32_t skill_id, uint32_t buff_id, uint32_t buff_tid);

	// 计算效果命中结果
	bool do_effect_hit_result(fight_hero_ptr target, uint32_t buff_add_rate);
	// 检测目标是不是自己的讽刺对象
	bool check_target_taunt(fight_hero_ptr target);

	// 获取技能效果
	SkillEffectTable* get_skill_effect(uint32_t skill_id);
	// 检查技能是否能用
	bool check_skill(int32_t skill_id);
	// 获取当前最适合的技能
	uint32_t get_best_skill();
	// 出手
	bool do_attack();
	// ai自动选择
	bool do_auto_fight();
	// 找技能AI
	uint32_t find_skill_ai(std::vector<SkillEffectTable*>& skill_ai_list);
	// 找目标AI
	uint32_t find_target_ai(uint32_t skill_id);

	// 使用技能
	bool do_attack(
		SkillEffectTable* skill_conf,
		fight_hero_ptr select = NULL,
		proto::common::combat_act_type type = proto::common::combat_act_type_skill,
		const std::vector<fight_hero_ptr>* targets = NULL,
		bool trigger_by_buff = false,
		bool can_counter = true);

	// buff触发使用技能
	bool trigger_skill_by_buff(
		SkillEffectTable* skill_conf,
		fight_hero_ptr select = NULL,
		proto::common::combat_act_type type = proto::common::combat_act_type_skill,
		bool can_counter = true);

	bool check_do_attack(
		SkillEffectTable* skill_conf,
		fight_hero_ptr select = NULL,
		proto::common::combat_act_type type = proto::common::combat_act_type_skill,
		const std::vector<fight_hero_ptr>* targets = NULL,
		bool trigger_by_buff = false);

	void do_counter(std::vector<fight_hero_ptr>& counters, bool can_counter = true);

	size_t get_end_phase(SkillEffectTable* skill_conf);

	// 查找目标并攻击
	bool find_target_and_attack(
		SkillEffectTable* skill_effect_conf,
		std::vector<fight_hero_ptr>& counters,
		bool& trigger_combo,
		size_t& phase,
		bool trigger_by_buff,
		fight_hero_ptr select = NULL,
		proto::common::combat_act_type type = proto::common::combat_act_type_skill,
		bool is_refind = false);

	// 攻击查找到的目标
	bool do_attack_after_find_target(
		SkillEffectTable* skill_conf,
		std::vector<fight_hero_ptr>& counters,
		bool& trigger_combo,
		size_t& phase,
		bool trigger_by_buff,
		const std::vector<fight_hero_ptr>& targets,
		const std::vector<fight_hero_ptr>& spurting_targets,
		proto::common::combat_act_type type = proto::common::combat_act_type_skill,
		bool is_refind = false);

	// 攻击一个目标
	bool do_attack_target(
		fight_hero_ptr target,
		SkillEffectTable* skill_conf,
		bool& trigger_counter,
		bool& trigger_combo,
		size_t& phase,
		bool trigger_by_buff,
		proto::common::combat_act_type type = proto::common::combat_act_type_skill);

	// 攻击一个目标
	bool check_do_attack_target(
		fight_hero_ptr target,
		SkillEffectTable* skill_conf,
		size_t& phase,
		bool trigger_by_buff,
		proto::common::combat_act_type type = proto::common::combat_act_type_skill);

	void get_pvp_param(int32_t& pvp_add_param, int32_t& pvp_dec_param, fight_hero_ptr target);

	uint32_t get_hero_immuno_attr();
	uint32_t get_hero_attr_relation_type(fight_hero_ptr p_target);

	// 攻击一个目标后
	bool do_after_attack_target(fight_hero_ptr target, SkillEffectTable* skill_conf, bool trigger_by_buff);
	
	// 查找目标
	bool find_target(SkillEffectTable* skill_conf, std::vector<fight_hero_ptr>& targets, std::vector<fight_hero_ptr>& spurting_targets, fight_hero_ptr select = NULL);

	// 技能附加buff
	bool do_skill_buff(
		size_t phase,
		FIGHT_EVENT when,
		skill_event_target_type to,
		SkillEffectTable* skill_conf, 
		fight_hero_ptr target = NULL);

	// 技能伤害计算
	bool do_skill_hurt(
		size_t phase,
		SkillEffectTable* skill_conf,
		fight_hero_ptr target,
		proto::common::combat_act_type type,
		bool & combo);

	void do_crit(fight_hero_ptr target, int & crit_multiple, int & heal_crit_multiple);

	bool do_dodge(SkillEffectTable* skill_conf,fight_hero_ptr target);
	bool do_immuno(SkillEffectTable* skill_conf, fight_hero_ptr target);

	bool do_skill_hurt_imp(
		size_t phase,
		SkillEffectTable* skill_conf,
		fight_hero_ptr target,
		proto::common::combat_act_type type,
		int crit_multiple,
		bool & combo);

	void do_attack_result_notify(proto::common::combat_act_type type, fight_hero_ptr target);

	// 吸血
	void do_leech(SkillEffectTable* skill_conf,fight_hero_ptr target);
	// 吸血计算
	int calc_leech(SkillEffectTable* skill_conf, fight_hero_ptr target);
	// 反射
	void do_reflect(SkillEffectTable* skill_conf, fight_hero_ptr target);
	// 检测连击
	bool check_combo(SkillEffectTable* skill_conf, fight_hero_ptr target);

	// 计算直伤
	uint32_t calc_direct_injury(
		size_t phase,
		SkillEffectTable* skill_conf,
		fight_hero_ptr target,
		proto::common::combat_act_type type,
		int crit_multiple
		);

	// 计算伤害限制
	uint32_t calc_limit_injury(
		size_t phase,
		SkillEffectTable* skill_conf,
		fight_hero_ptr target,
		proto::common::combat_act_type type,
		int crit_multiple
		);

	uint32_t get_attack();

	// 计算攻击力
	int calc_attack(
		size_t phase,
		SkillEffectTable* skill_conf,
		fight_hero_ptr target,
		proto::common::combat_act_type type,
		int crit_multiple);

	// 计算防御力
	int calc_defend(
		size_t phase,
		SkillEffectTable* skill_conf,
		fight_hero_ptr target,
		proto::common::combat_act_type type,
		int crit_multiple);

	// 计算伤害附加百分比
	float calc_injury_addition_percent(
		size_t phase,
		SkillEffectTable* skill_conf,
		fight_hero_ptr target,
		proto::common::combat_act_type type,
		int crit_multiple);

	// 通过BUFF目标类型获取目标
	void get_target_by_buff_select_target(fight_hero_ptr select, int type, int rand_count, std::vector<fight_hero_ptr>& out);

	// 计算伤害
	int calc_injury(
		size_t phase,
		SkillEffectTable* skill_conf,
		fight_hero_ptr target,
		proto::common::combat_act_type type,
		int crit_multiple);

	// 计算反伤
	int calc_reflect(
		size_t phase,
		SkillEffectTable* skill_conf,
		fight_hero_ptr target,
		proto::common::combat_act_type type,
		int crit_multiple); 

	// 治疗
	bool do_skill_heal(size_t phase, SkillEffectTable* skill_conf, fight_hero_ptr target, int heal_crit_multiple);

	// 加buff
	bool do_add_skill_buff(fight_hero_ptr p_hero, fight_hero_ptr p_attacker, uint32_t skill_id, bool b_use_rate, uint32_t buff_add_rate, const buff_info_vec& buffs);
	// 驱散buff
	bool do_dispel_skill_buff(fight_hero_ptr p_hero, fight_hero_ptr p_attacker, uint32_t skill_id, uint32_t buff_dispel_rate, uint32_t buff_dispel_num, const buff_dispel_info_vec& buffs);
	// 更新战斗
	bool update_fight(const proto::common::hero_state& state);
	void do_update_fight();

public:

	// 回合开始
	void on_round_start_imp();
	// 回合结束
	void on_round_end_imp();
	// 回合开始
	void on_round_start();
	// 回合结束
	void on_round_end();
	// 攻击前
	void before_attack(SkillEffectTable* skill_conf, bool trigger_by_buff);
	// 攻击后
	void after_attack(SkillEffectTable* skill_conf, bool trigger_by_buff);
	// 脚本使用技能后
	void after_script_use_skill(SkillEffectTable* skill_conf);
	// 处理技能消耗
	void do_skill_consum(SkillEffectTable* skill_conf);
	// 死亡
	void on_dead(fight_hero_ptr attaker, SkillEffectTable* skill_conf);
	// 复活
	void on_relive(fight_hero_ptr attaker, uint32_t buffid, uint32_t bufftid,  uint32_t skill, int relive_hp, int relive_speed = -1);
	// 被召唤出来
	void on_summon(fight_hero_ptr attaker, uint32_t skill);

	uint32_t get_relive_count();

	bool is_single_attack(SkillEffectTable* skill_conf);
public:

	// 获取回合数
	uint32_t get_round();
	// 获取战斗指针
	combat_ptr get_combat();
	// 获取战斗id
	uint64_t get_fight_uid();

	// 获取技能cd
	uint32_t get_skill_cd(uint32_t skill);
	// 减技能cd
	void sub_skill_cd(uint32_t cd = 1);
	// 加技能cd
	void add_skill_cd(uint32_t skill);
// 	// 校验开场技能cd
// 	bool check_start_skill_cd(SkillEffectTable* conf);
//	// 增加目标
//	void add_cur_target(fight_hero_ptr target);
	// 设置被嘲讽信息
	void set_be_taunt_info(fight_hero_ptr target, bool is_add, uint32_t taunt_buff_type = 0);
	// 设置嘲讽buff_id
	void set_taunt_buff_info(uint32_t buff_type) { m_taunt_data.taunt_buff_type = buff_type; }
	// 添加嘲讽目标
	void add_taunt_target(fight_hero_ptr p_target);
	// 移除嘲讽目标
	void remove_taunt_target(fight_hero_ptr p_target);
	// 清除嘲讽信息
	void clear_taunt_info();
	// 设置部署选择
	void set_cur_select(const std::string& select);

	// 分发光环BUFF
	void distribute_special_buff();

	// 回收光环BUFF
	void recovery_special_buff();

	// 激活被动BUFF
	void activation_passive_buff();

	// 清除被动BUFF
	void clear_passive_buff();
public:

	// 出手顺序排序
	static bool hero_speed_sort(const fight_hero_ptr& h1, const fight_hero_ptr& h2);
	// 先手队列拉条溢出值排序
	static bool hero_att_more_speed_sort(const fight_hero_ptr& h1, const fight_hero_ptr& h2);

public:

	// 获取buff以外的属性
	int get_attr_without_buff(uint32_t type);
	// 获取属性
	int get_attr(uint32_t type);
	// 设置属性
	void set_attr(uint32_t type, int value);
	// 减属性
	void sub_attr(uint32_t type, int value);

	int get_real_sub_hp_value( int value );
	uint32_t get_hp() { return get_attr(attrtype_enum::hp); }
	uint32_t get_max_hp() { return get_attr(attrtype_enum::max_hp); }
	uint32_t get_hp_percent();
	// 加hp
	void add_hp(int value);
	// 治疗HP
	uint32_t heal_hp(fight_hero_ptr attacker, int value);
	// 减hp
	void sub_hp(int value, fight_hero_ptr attacker, uint32_t skill = 0);
	// 被攻击
	void on_be_attack(int damage, fight_hero_ptr attacker, uint32_t skill = 0, bool is_buff = false);
	// 分摊伤害
	int do_share_damage(int damage, fight_hero_ptr attacker, uint32_t skill, bool is_buff);
	// 承担伤害
	int do_target_bear_damage(int damage, fight_hero_ptr attacker, uint32_t skill, bool is_buff);
	// 获取怒气
	uint32_t get_sp();
	// 减怒气
	void sub_sp(uint32_t value);
	// 加怒气
	void add_sp(uint32_t value);
	// 减部署怒气
	void sub_disposition_sp(uint32_t value);
	// 获取部署怒气
	uint32_t get_disposition_sp();
	// 修改标记
	void set_mark_id(uint32_t mark_id) { m_mark_id = mark_id; }
	// 获取标记
	uint32_t get_mark_id() { return m_mark_id; }
	// 是否被控
	bool is_in_control();
	// 添加所有技能CD 
	void inc_all_skill_cd(fight_hero_ptr attacker, uint32_t cd);
	// 减少所有技能CD
	void dec_all_skill_cd(fight_hero_ptr attacker, uint32_t cd);

	// 随机添加技能CD 
	void inc_random_skill_cd(fight_hero_ptr attacker, uint32_t cd);
	// 随机减少技能CD
	void dec_random_skill_cd(fight_hero_ptr attacker, uint32_t cd);

	// 变身
	void change_model(uint32_t change_type, uint32_t change_phase, uint32_t change_skill_level);
	void restore_model();

	uint32_t athanasia_count() const;
	void athanasia_count(uint32_t value);
public:
	// 处理收到的伤害
	void deal_be_attack(int32_t damage, fight_hero_ptr attacker, uint32_t skill);
// 	// 记录伤害
// 	void add_damage(int damage);
// 	// 记录受到的伤害
// 	void add_injured(int injured);
// 	// 获取造成的伤害
// 	uint32_t get_round_damage(uint32_t round);
// 	// 获取受到的伤害
// 	uint32_t get_round_injured(uint32_t round);
	// 获取最近一次伤害计算结果
	const fight_calc_result_t& get_fight_calc_result() { return m_calc_result; }
	// 设置分摊伤害
	void set_bear_damage(uint32_t ratio, fight_hero_ptr target);
public:

	// 获取脚本代理
	inline script_fight_hero_t* get_script_proxy() const { return m_script_proxy; }
	// 获取当前战斗过程
	proto::common::combat_action_data* get_cur_process();
	// 获取buff管理器
	buff_manager_t& get_buff_manager() { return m_buff_mgr; }

	// 添加战斗行为
	//void add_combat_act(proto::common::combat_act_type type, uint32_t value, uint64_t target, uint32_t remain_value = 0);
	void add_combat_act(proto::common::combat_act_type type, uint32_t value, fight_hero_ptr target, fight_hero_ptr attacker,
						uint32_t remain_value = 0, uint32_t remain_value2 = 0, uint32_t param = 0);
	// 过滤一些战斗行为
	bool pass_combat_act_type(proto::common::combat_act_type type);

	// 技能伤害
	void skill_damage(fight_hero_ptr target, uint32_t damage, uint32_t skill);
	// 技能附加伤害
	void skill_ext_damage(fight_hero_ptr target, uint32_t damage, uint32_t skill);
	// 分摊伤害
	void share_damage_to(fight_hero_ptr teammate, fight_hero_ptr attacker, uint32_t skill, uint32_t damage);
	// 添加战斗技能行为
	void add_combat_skill_act(proto::common::combat_act_type type, uint32_t value, uint64_t attacker, std::vector<uint64_t>& targets);

	// buff移除时
	void on_remove_buff(uint32_t id, uint32_t tid, uint32_t layer, uint32_t round, uint64_t attacker_id = 0);
	// buff添加时
	void on_add_buff(uint32_t id, uint32_t tid, uint32_t layer, uint32_t round, uint64_t attacker_id = 0);
	// 触发免疫buff
	void on_buff_immuno();
	// 添加buff行为
	void add_combat_buff_act(proto::common::combat_act_type type, uint32_t id, uint32_t tid, uint32_t layer, uint32_t round, uint64_t attacker_id = 0);
	
	// 设置脚本变量
	void set_script_var(const std::string& var, int value);
	// 获取脚本变量
	int get_script_var(const std::string& var);

	// 获取攻击步骤
	proto::common::combat_act_step get_attack_step();
	// 设置攻击步骤
	void set_attack_step(proto::common::combat_act_step step);

	void pack_attack_targets(std::string &targets);
	void peek_recovery_data(proto::common::fight_recovery_hero_single_data *hero_data);
	void peek_fight_skill_data(proto::common::fight_hero_skill_data *skill_data);
	void peek_save_data(proto::common::hero_save_data *hero_data);


	// 事件
	void do_team_script_event(FIGHT_EVENT event);

	// 事件
	bool on_event(FIGHT_EVENT event);


	// 添加特殊BUFF给所有人
	void add_special_buff_to_all(int buff, int target_type, int skill );
	// 删除所有人BUFF
	void remove_special_buff_from_all(int buff, int target_type, int skill);


	fight_hero_ptr get_first_target();
	fight_hero_ptr get_cur_target();
	fight_hero_ptr get_special_target();
	fight_hero_ptr get_bear_damage_target();
	
	SkillEffectTable* get_cur_skill();
	fight_hero_ptr get_cur_attacker();
	void set_cur_attacker(fight_hero_ptr attacker);

	uint32_t counter_count() const;
	void counter_count(uint32_t value);

	uint32_t add_buff_after_damage_count() const;
	void add_buff_after_damage_count(uint32_t value);

	uint32_t trigger_skill_count() const;
	void trigger_skill_count(uint32_t value);

	uint32_t combo_count() const;
	void combo_count(uint32_t value);

	bool do_skill_attack_target(SkillEffectTable* skill_conf, const std::vector<fight_hero_ptr>& targets, proto::common::combat_act_type type);

	// 目标护盾只
	void do_target_shield_points(fight_hero_ptr target);

	void activation_skill(uint32_t skill, uint32_t level);

	// 添加免疫的buff类型
	void add_immuno_buff_type(uint32_t type);
	// 清空免疫的buff类型
	void clear_immuno_buff_type() { m_immuno_buff_types.clear(); }
	const std::vector<uint32_t>& get_immuno_buff_types() { return m_immuno_buff_types; }
public:

	proto::common::hero_single m_hero_data;			// 武将初始数据

	buff_manager_t m_buff_mgr;						// buff管理器
	buff_element_manager_t m_buff_element_mgr;		// buff元素管理器
	buff_element_manager_t m_buff_element_moment_mgr;// 瞬间BUFF元素管理器
	combat_wptr m_combat;							// 战斗指针
	proto::common::combat_form_single m_cur_form;	// 部署
	attr_map m_cur_attr;							// 属性
	std::map<uint32_t, uint32_t> m_skill_cd;		// 技能cd
//	std::map<uint32_t, uint32_t> m_start_skill_cd;	// 开场技能cd
//	std::vector<fight_hero_ptr> m_cur_targets;		// 当前目标
	std::map<uint32_t, skill_data>	m_skills;		// 全部技能
	std::map<uint32_t, skill_data>	m_change_model_old_skills;	// 变身前的技能

	fight_calc_result_t m_calc_result;				// 战斗结算结果
public:

// 	typedef std::map<uint32_t, fight_hero_round_data> round_data;
// 	round_data m_round_data;						// 武将回合数据
public:

	script_fight_hero_t*			m_script_proxy = NULL;		// 脚本代理
	std::map<std::string, int>		m_script_var;				// 脚本变量
	std::vector<fight_hero_wptr>	m_attack_targets;			// 每次攻击的所有目标
	std::vector<uint32_t>			m_immuno_buff_types;		// 免疫的所有buff类型
private:
	stat_data m_stat_data;								// 统计数据
	count_data m_count_data;							// 次数数据
	SkillEffectTable* m_cur_skill_conf = NULL;			// 当前使用的技能
	fight_hero_wptr m_cur_target;						// 单次攻击的当前目标
	fight_hero_wptr m_bear_damage_target;				// 分摊伤害目标
	fight_hero_wptr m_special_target;					// 特殊攻击的目标
	fight_hero_wptr m_cur_attacker;						// 当前攻击者
	uint32_t m_mark_id = 0;								// 标记ID
	uint32_t m_round = 1;								// 当前回合(每个武将单独计算自己的回合)
	uint32_t m_att_speed = 0;							// 先攻值(不会超过阀值)
	uint32_t m_order_att_speed = 0;						// 排序先攻值(会超过阀值 无必要不应开放修改接口)
	uint32_t m_att_more_speed = 0;						// 拉条溢出值
	uint32_t m_auto_fight_mode = 0;						// 自动战斗模式
	uint32_t m_change_model_phase = 0;					// 变身阶段

//	int		m_last_hp = 0;								// 上次hp		//wys 注释 设计不合理 判断重复死亡应该用标记 2017.6.16
	int32_t m_first_order_pos = -1;						// 在先手队列的位置

	uint32_t m_activation_skill = 0;					// 激活技能 
	uint32_t m_activation_skill_level = 0;				// 激活技能等级


	uint32_t m_target_count = 0;						// 本次目标数量
	taunt_data m_taunt_data;							// 嘲讽数据


	bool	m_is_story = false;							// 是否是剧情武将
	bool	m_dead_flag = false;						// 死亡标记

	uint32_t m_bear_damage_ratio = 0;					// 分摊伤害比例
};

#endif//__FIGHT_HERO_HPP__