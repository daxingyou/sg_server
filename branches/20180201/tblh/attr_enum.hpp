#ifndef __COMMON_ATTR_CODE__
#define __COMMON_ATTR_CODE__
#include "macros.hpp"
NS_COMMON_BEGIN
//tolua_begin
namespace attrtype_enum
{
    enum attr_type
    {
        strength				= 1, // ����(����) 
        intelligent				= 2, // ����(����) 
        vitality				= 3, // ����(����)
        spirit					= 4, // ����(����)
        atk						= 10, // ������ 
        atk_range				= 11, // �������� 
        atk_max					= 12, // ��󹥻�
        def_physical			= 13, // ����
        attack_no_wakeup_rate	= 14, // ���������Ѹ���
        hp						= 15, // ����ֵ
        max_hp					= 16, // �������ֵ 
        max_hp_bonus			= 17, // �������ֵ�ӳ�
        atk_growth				= 18, // �����ɳ�
        def_physical_growth		= 19, // �����ɳ�
        share_damage			= 20, // ��̯�˺�
        max_hp_growth			= 21, // ����ֵ�ɳ�
        sp						= 22, // ŭ��ֵ(����)
        max_sp					= 23, // ���ŭ��ֵ(����)
        ranged_atk				= 24, // �����󹥻�
        action_bar				= 25, // �ж���(����)
        crit_rate				= 30, // ������
        crit_damage				= 31, // �����˺�
        crit_damage_bonus		= 32, // ���˼ӳ�
        crit_damage_reduce		= 33, // �����ֿ�
        block_rate				= 34, // ����(����)
        block_effect			= 35, // ��Ч��(����)
        block_effect_bonus		= 36, // �񵲼ӳ�(����)
        block_effect_reduce		= 37, // �ƻ�Ч��(����)
        accurate				= 38, // ��׼��(����)
        speed					= 39, // �ٶ�
        effect_hit_rate			= 40, // Ч������
        effect_dodge_rate		= 41, // Ч���ֿ�
		def_physical_reduce		= 42, // ��Ϊ(����)
        damage_bonus			= 50, // �˺��ӳ�
        damage_reduce			= 51, // �˺����� 
        get_damage_bonus		= 52, // ����Ч��(����) 
        get_damage_reduce		= 53, // ����Ч�� 
        heal_bonus				= 54, // ���Ƽӳ� 
        heal_reduce				= 55, // ���ƽ��� 
        get_heal_bonus			= 56, // �ܵ����Ƽӳ� 
        get_heal_reduce			= 57, // �ܵ����ƽ��� 
        leech					= 58, // ��ѪЧ��  
        reflect					= 59, // ����Ч��  
        shield					= 60, // ����Ч��(����) 
        shield_bonus			= 61, // ����Ч���ӳ�(����) 
        shield_points			= 62, // ����ֵ 
        dodge					= 70, // ���� 
        combo_rate				= 71, // ������ 
        combo_max				= 72, // ��������� 
        counter_rate			= 73, // ������ 
        counter_max				= 74, // ��󷴻��� 
        skill_damage			= 80, // �����˺��ٷֱ�(����) 
        skill_damage_extra		= 81, // ���ܸ����˺�ֵ(����) 
        skill_heal				= 82, // �����������ٷֱ� 
        skill_heal_extra		= 83, // ���ܸ�������ֵ(����) 
        skill_shield			= 84, // ���ܻ��ܰٷֱ�(����)  
        skill_shield_basic		= 85, // ���ܻ�������ֵ(����)  
        skill_leech_basic		= 86, // ���ܻ�����Ѫֵ(����)  
        skill_leech				= 87, // ������Ѫ�ٷֱ�(����)
        skill_reflect_basic		= 88, // ���ܻ�������ֵ(����) 
        spurting				= 89, // ���� 
        true_damage				= 90, // ��ʵ�˺� 
        true_def				= 91, // ��ʵ���� 
        pvp_arena_damage		= 92, // PVP���������� 
        pvp_arena_def			= 93, // PVP���������� 
        pvp_no_arena_damage		= 94, // PVP�Ǿ�������     
        pvp_no_arena_def		= 95, // PVP�Ǿ��������� 
        true_cure				= 96, // ��ʵ����(����)  
        stage_atk_bonus			= 100, // ���׹���������(����)   
        stage_def_physical_bonus = 101, // ���������������(����)   
        stage_def_spell_bonus	= 102, // ���׷�����������(����)   
        stage_max_hp_bonus		= 103, // �����������ֵ����(����)   
        add_speed				= 104, // ����(����)   
		all_team_damage_turn	= 105, // �˺�����ת��  
        atk_add_per				= 106, // ���������Ӱٷֱ�
        atk_cut_per				= 107, // ���������ٰٷֱ� 
        def_add_per				= 108, // �������Ӱٷֱ� 
        def_cut_per				= 109, // �������ٰٷֱ� 
        crit_damage_cut_per		= 110, // ���˼��ٰٷֱ� 
        hp_cut_per				= 111, // �������ٰٷֱ� 
        speed_add_per			= 112, // �ٶ����Ӱٷֱ� 
		speed_cut_per			= 113, // �ٶȼ��ٰٷֱ� 
		def_ignore_rate			= 114, // ���ӷ������� 
		dp_convert_hurt			= 115, // ���ת�˺�ת���� 
		dp_convert_reflect_rate = 116, // ���ת����Ч��ת���� 
		target_debuff_attack_addition	= 117,	// ����״̬׷�ӹ�����ֱ�
		target_status_attack_id			= 118,	// ָ��״̬׷�ӹ���ID
		target_status_attack_addition	= 119,	// ָ��״̬׷�ӹ�����ֱ�
		debuff_damage_strengthen		= 120,	// DEBUFF�˺���ǿ
		thunder_cloud_buff_id			= 121,	// ����BUFFID
		thunder_clond_hurt_ratio		= 122,	// �����˺�ϵ��
		damnation_buff_id				= 123,	// ����BUFFID
		damnation_hurt_ratio			= 124,	// �����˺�ϵ��
		lose_blood_volume_to_attack		= 125,	//	Ѫ����ʧ�ٷֱ�ת������
		blood_volume_to_attack_limit_weapon = 126,	// Ѫ���˺��ӳɽ���(����)
		blood_volume_to_attack_ratio_weapon = 127,	// Ѫ���˺��ӳɱ���(����)
		blood_volume_to_attack_limit_skill = 128,	// Ѫ���˺��ӳɽ���(����)
		blood_volume_to_attack_ratio_skill = 129,	// Ѫ���˺��ӳɱ���(����)
		self_attack_addition			   = 130,	// ׷�ӹ�����ֱ�
		counter_attack_addition			= 131,	// �����˺��ӳ�		
		def_ignore_value				= 132,	// ���ӷ�����ֵ
		speed_effect_ratio				= 133,	// �����ӳ���ֱ�
		stratagem_buff_id				= 134,	// �Ʋ�BUFFID
		stratagem_buff_ratio			= 135,	// �Ʋ��˺�����
		lose_blood_volume_to_defend		= 136,	// Ѫ����ʧ�ٷֱ�ת������
		once_attack_lose_max_hp_ratio	= 137,	// �����˺�Ѫ������
		reflect_by_attack				= 138,	// ���ݹ��������˺�
		reflect_by_damage				= 139,	// �����˺������˺�
		reflect_limit					= 140,	// �����˺�����
		once_trigger_skill_limit		= 141,	// ���غϴ�����������
		one_round_all_damage_to_injury	= 142,	// �غ��������˺�ת��Ϊֱ��
		dodge_single_target_damage		= 143,	// ���ܵ��幥���˺�
		combo_buffer_id					= 144,	// ����BUFFID
		single_attack_behurt_ratio		= 145,	// �ܵ����幥�����˺��仯
		range_attack_behurt_ratio		= 146,	// �ܵ�Ⱥ�幥�����˺��仯
		skill_cd_change					= 147,	// ����CD�仯
		anti_dodge						= 148,  // ������
		in_control_attack_addition		= 149,	// �Դ��ڱ�����״̬�ĵ�λ����ɶ����˺�
		other_action_addition_hurt		= 150,	// �����ж������ĸ����˺�
		lose_blood_volume_to_crit_effect = 151,	// Ѫ����ʧ�ٷֱ�ת������Ч��
		all_team_damage_to_injury		= 152,	// ���ж��������˺�ת��Ϊֱ��

        stun					= 200, // ѣ��״̬ 
        taunt					= 201, // ����״̬ 
        bleeding				= 202, // ��Ѫ״̬ 
        confused				= 203, // ����״̬ 
        drunk					= 204, // ����״̬������ࣩ 
        exempt_cd				= 205, // ��cd���� 
        immuno					= 206, // ���� 
        hostile_type_stamp		= 207, // �ж����ͱ�־ 
        beheaded_rate			= 208, // նɱ���� 
        beheaded_damage_times	= 209, // նɱ���� 
        soul_condition			= 210, // ���״̬ 
		no_revive_condition		= 211, // ���ɸ����״̬ 
		soul_guard				= 212,	// ����ػ�
		athanasia				= 213,	// ����
		invincibility			= 214,	// �޵�
		guide_team				= 215,	// ����
		frozen					= 216,	// ����
		sleep					= 217,	// ˯��״̬
		forget					= 218,	// ��ȴ
		forbid_strengthen		= 219,	// ��ֹǿ��
		silent					= 220,	// ��Ĭ
		def_break				= 221,	// �Ƽ�
		immuno_single_attack	= 222,	// ���ߵ��幥��


        water_immune			= 300, // ˮϵ���� 
        fire_immune				= 301, // ��ϵ���� 
        wind_immune				= 302, // ��ϵ����
		yin_immune				= 303, // ��ϵ����
		yang_immune				= 304, // ��ϵ����


    };
}
//tolua_end
NS_COMMON_END
#endif
