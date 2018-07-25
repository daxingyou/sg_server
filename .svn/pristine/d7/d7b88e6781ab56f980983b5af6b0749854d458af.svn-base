#ifndef __COMMON_ATTR_CODE__
#define __COMMON_ATTR_CODE__
#include "macros.hpp"
NS_COMMON_BEGIN
//tolua_begin
namespace attrtype_enum
{
    enum attr_type
    {
        strength				= 1, // 武力(废弃) 
        intelligent				= 2, // 智力(废弃) 
        vitality				= 3, // 体质(废弃)
        spirit					= 4, // 精神(废弃)
        atk						= 10, // 攻击力 
        atk_range				= 11, // 攻击浮动 
        atk_max					= 12, // 最大攻击
        def_physical			= 13, // 防御
        attack_no_wakeup_rate	= 14, // 攻击不吵醒概率
        hp						= 15, // 生命值
        max_hp					= 16, // 最大生命值 
        max_hp_bonus			= 17, // 最大生命值加成
        atk_growth				= 18, // 攻击成长
        def_physical_growth		= 19, // 防御成长
        share_damage			= 20, // 分摊伤害
        max_hp_growth			= 21, // 生命值成长
        sp						= 22, // 怒气值(废弃)
        max_sp					= 23, // 最大怒气值(废弃)
        ranged_atk				= 24, // 浮动后攻击
        action_bar				= 25, // 行动条(废弃)
        crit_rate				= 30, // 暴击率
        crit_damage				= 31, // 暴击伤害
        crit_damage_bonus		= 32, // 暴伤加成
        crit_damage_reduce		= 33, // 暴击抵抗
        block_rate				= 34, // 格挡率(废弃)
        block_effect			= 35, // 格挡效果(废弃)
        block_effect_bonus		= 36, // 格挡加成(废弃)
        block_effect_reduce		= 37, // 破击效果(废弃)
        accurate				= 38, // 精准率(废弃)
        speed					= 39, // 速度
        effect_hit_rate			= 40, // 效果命中
        effect_dodge_rate		= 41, // 效果抵抗
		def_physical_reduce		= 42, // 修为(废弃)
        damage_bonus			= 50, // 伤害加成
        damage_reduce			= 51, // 伤害减少 
        get_damage_bonus		= 52, // 易伤效果(废弃) 
        get_damage_reduce		= 53, // 减伤效果 
        heal_bonus				= 54, // 治疗加成 
        heal_reduce				= 55, // 治疗降低 
        get_heal_bonus			= 56, // 受到治疗加成 
        get_heal_reduce			= 57, // 受到治疗降低 
        leech					= 58, // 吸血效果  
        reflect					= 59, // 反伤效果  
        shield					= 60, // 护盾效果(废弃) 
        shield_bonus			= 61, // 护盾效果加成(废弃) 
        shield_points			= 62, // 护盾值 
        dodge					= 70, // 闪避 
        combo_rate				= 71, // 连击率 
        combo_max				= 72, // 最大连击数 
        counter_rate			= 73, // 反击率 
        counter_max				= 74, // 最大反击数 
        skill_damage			= 80, // 技能伤害百分比(废弃) 
        skill_damage_extra		= 81, // 技能附加伤害值(废弃) 
        skill_heal				= 82, // 技能治疗量百分比 
        skill_heal_extra		= 83, // 技能附加治疗值(废弃) 
        skill_shield			= 84, // 技能护盾百分比(废弃)  
        skill_shield_basic		= 85, // 技能基础护盾值(废弃)  
        skill_leech_basic		= 86, // 技能基础吸血值(废弃)  
        skill_leech				= 87, // 技能吸血百分比(废弃)
        skill_reflect_basic		= 88, // 技能基础反伤值(废弃) 
        spurting				= 89, // 溅射 
        true_damage				= 90, // 真实伤害 
        true_def				= 91, // 真实防御 
        pvp_arena_damage		= 92, // PVP竞技场增伤 
        pvp_arena_def			= 93, // PVP竞技场减伤 
        pvp_no_arena_damage		= 94, // PVP非竞技场增     
        pvp_no_arena_def		= 95, // PVP非竞技场减伤 
        true_cure				= 96, // 真实治疗(废弃)  
        stage_atk_bonus			= 100, // 进阶攻击力收益(废弃)   
        stage_def_physical_bonus = 101, // 进阶物理防御收益(废弃)   
        stage_def_spell_bonus	= 102, // 进阶法术防御收益(废弃)   
        stage_max_hp_bonus		= 103, // 进阶最大生命值收益(废弃)   
        add_speed				= 104, // 吸速(废弃)   
		all_team_damage_turn	= 105, // 伤害吸收转换  
        atk_add_per				= 106, // 攻击力增加百分比
        atk_cut_per				= 107, // 攻击力减少百分比 
        def_add_per				= 108, // 防御增加百分比 
        def_cut_per				= 109, // 防御减少百分比 
        crit_damage_cut_per		= 110, // 暴伤减少百分比 
        hp_cut_per				= 111, // 生命减少百分比 
        speed_add_per			= 112, // 速度增加百分比 
		speed_cut_per			= 113, // 速度减少百分比 
		def_ignore_rate			= 114, // 无视防御概率 
		dp_convert_hurt			= 115, // 物防转伤害转换率 
		dp_convert_reflect_rate = 116, // 物防转反伤效果转换率 
		target_debuff_attack_addition	= 117,	// 负面状态追加攻击万分比
		target_status_attack_id			= 118,	// 指定状态追加攻击ID
		target_status_attack_addition	= 119,	// 指定状态追加攻击万分比
		debuff_damage_strengthen		= 120,	// DEBUFF伤害加强
		thunder_cloud_buff_id			= 121,	// 雷云BUFFID
		thunder_clond_hurt_ratio		= 122,	// 雷云伤害系数
		damnation_buff_id				= 123,	// 诅咒BUFFID
		damnation_hurt_ratio			= 124,	// 诅咒伤害系数
		lose_blood_volume_to_attack		= 125,	//	血量损失百分比转化攻击
		blood_volume_to_attack_limit_weapon = 126,	// 血量伤害加成界限(武器)
		blood_volume_to_attack_ratio_weapon = 127,	// 血量伤害加成比例(武器)
		blood_volume_to_attack_limit_skill = 128,	// 血量伤害加成界限(技能)
		blood_volume_to_attack_ratio_skill = 129,	// 血量伤害加成比例(技能)
		self_attack_addition			   = 130,	// 追加攻击万分比
		counter_attack_addition			= 131,	// 反击伤害加成		
		def_ignore_value				= 132,	// 无视防御数值
		speed_effect_ratio				= 133,	// 拉条加成万分比
		stratagem_buff_id				= 134,	// 计策BUFFID
		stratagem_buff_ratio			= 135,	// 计策伤害比例
		lose_blood_volume_to_defend		= 136,	// 血量损失百分比转换防御
		once_attack_lose_max_hp_ratio	= 137,	// 单次伤害血量上限
		reflect_by_attack				= 138,	// 根据攻击反射伤害
		reflect_by_damage				= 139,	// 根据伤害反射伤害
		reflect_limit					= 140,	// 反射伤害限制
		once_trigger_skill_limit		= 141,	// 单回合触发技能上限
		one_round_all_damage_to_injury	= 142,	// 回合内所受伤害转换为直伤
		dodge_single_target_damage		= 143,	// 闪避单体攻击伤害
		combo_buffer_id					= 144,	// 连击BUFFID
		single_attack_behurt_ratio		= 145,	// 受到单体攻击的伤害变化
		range_attack_behurt_ratio		= 146,	// 受到群体攻击的伤害变化
		skill_cd_change					= 147,	// 技能CD变化
		anti_dodge						= 148,  // 抗闪避
		in_control_attack_addition		= 149,	// 对处于被控制状态的单位，造成额外伤害
		other_action_addition_hurt		= 150,	// 别人行动带来的附加伤害
		lose_blood_volume_to_crit_effect = 151,	// 血量损失百分比转换暴击效果
		all_team_damage_to_injury		= 152,	// 所有队友所受伤害转换为直伤

        stun					= 200, // 眩晕状态 
        taunt					= 201, // 嘲讽状态 
        bleeding				= 202, // 流血状态 
        confused				= 203, // 混乱状态 
        drunk					= 204, // 醉眼状态（标记类） 
        exempt_cd				= 205, // 免cd概率 
        immuno					= 206, // 免疫 
        hostile_type_stamp		= 207, // 敌对类型标志 
        beheaded_rate			= 208, // 斩杀概率 
        beheaded_damage_times	= 209, // 斩杀倍数 
        soul_condition			= 210, // 灵魂状态 
		no_revive_condition		= 211, // 不可复活的状态 
		soul_guard				= 212,	// 灵魂守护
		athanasia				= 213,	// 不死
		invincibility			= 214,	// 无敌
		guide_team				= 215,	// 护卫
		frozen					= 216,	// 冰冻
		sleep					= 217,	// 睡眠状态
		forget					= 218,	// 忘却
		forbid_strengthen		= 219,	// 禁止强化
		silent					= 220,	// 沉默
		def_break				= 221,	// 破甲
		immuno_single_attack	= 222,	// 免疫单体攻击


        water_immune			= 300, // 水系免疫 
        fire_immune				= 301, // 火系免疫 
        wind_immune				= 302, // 风系免疫
		yin_immune				= 303, // 阴系免疫
		yang_immune				= 304, // 阳系免疫


    };
}
//tolua_end
NS_COMMON_END
#endif
