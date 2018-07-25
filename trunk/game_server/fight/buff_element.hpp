#ifndef __BUFF_ELEMENT_HPP__
#define __BUFF_ELEMENT_HPP__

#include "common/macros.hpp"
#include "tblh/BuffTable.tbls.h"
#include "combat.hpp"

typedef uint32_t(*elenment_func) ( fight_hero_ptr entry, buff_ptr buff, const buff_element_data& element, const int numchange, fight_hero_ptr p_event_owner, uint32_t event_num );



enum buff_element_index
{
	buff_element_none						= 0,
	// �ı�����
	buff_element_change_attr				= 1,
	// ��������
	buff_element_trigger_skill				= 2,
	// ����ŭ��ֵ
	buff_element_add_sp						= 3,
	//buff_element_use_skill = 4,
	// DOT�˺�
	buff_element_damage						= 4,
	// ͵ȡBUFF
	buff_element_steal_buff					= 5,
	// ����
	buff_element_heal						= 6,
	// ����
	buff_element_att_speed					= 7,
	// ���������
	buff_element_heal_overflow				= 8,
	// �ı�����(���ܲ���Ӱ��)
	buff_element_change_attr_without_num	= 9,
	// ����Ч��
	buff_element_relive						= 10,
	// �޸ļ���CD 
	buff_element_change_cd					= 11,
	// ��ɢ
	buff_element_cleanup					= 12,
	// ƽ��Ѫ��
	buff_element_balance_hp					= 13,
	// ����Ѫ��
	buff_element_switch_hp					= 14,
	// �����ٶ���
	buff_element_switch_speed				= 15,
	// ƽ���ٶ���
	buff_element_balance_speed				= 16,
	// �л�Ч��
	buff_element_union_attack				= 17,
	// bufferת��
	buff_element_switch_buff				= 18,
	// �ı��Ѿ�����
	buff_element_change_attr_all_team		= 19,
	// �ı�о�����
	buff_element_change_attr_all_enmey		= 20,
	// ���ܼ���
	buff_element_shield						= 21,
	// ը��Ч��
	buff_element_bomb						= 22,
	// ����ը��
	buff_element_detonate_bomb				= 23,
	// ���Ӷ���BUFF
	buff_element_addbuff_by_debuff			= 24,
	// ����תDEBUFF
	buff_element_profit_buff_to_debuff		= 25,
	// �Ƴ�BUFF����
	buff_element_remove_buff_layer			= 26,
	// ����ָ��BUFFʱ׷��BUFF
	buff_element_addbuff_when_exist			= 27,
	// ����ָ��BUFF��׷��BUFF
	buff_element_addbuff_when_addbuff		= 28,
	// ��������滻Ч��
	buff_element_addbuff_when_over_layer	= 29,
	// �Ƴ�ָ��BUFF
	buff_element_remove_buff				= 30,
	// ��Ŀ������ָ��BUFF
	buff_element_add_buff_to_target			= 31,
	// ׷��
	buff_element_pursue_and_attack			= 32,
	// ����
	buff_element_taunt						= 33,
	// ��ȴ
	buff_element_forget						= 34,
	// �����ѪЧ��
	buff_element_heal_special				= 35,
	// �����������BUFF
	buff_element_add_random_buff			= 36,
	// �˺�������BUFF
	buff_element_add_buff_after_damage		= 37,
	// �۳�Ѫ��
	buff_element_sub_hp						= 38,
	// ���������
	buff_element_dead_when_relive			= 39,
	// �����
	buff_element_activation_skill			= 40,
	// ����DEBUFF���������������BUFF
	buff_element_add_buff_by_debuf_ratio	= 41,
	// �ı�BUFF�غ���
	buff_element_change_buff_round			= 42,
	// ��Ĭ
	buff_element_silent						= 43,
	// ����
	buff_element_change_model				= 44,
	// ����
	buff_element_athanasia					= 45,
	// �Ʋ�ը��
	buff_element_strategy_bomb				= 46,
	// ����Ѫ������BUFF
	buff_element_add_buff_by_hp_ratio		= 47,
	// �����ٶ�����BUFF
	buff_element_add_buff_by_speed			= 48,
	// �������BUFF����
	buff_element_add_random_buff_layer		= 49,
	// ����BUFFС��������Ԫ��
	buff_element_add_buff_by_small_type		= 50,
	// �Ƴ�ָ��BUFF
	buff_element_remove_buff_by_small_type	= 51,
	// ����ָ��BUFF��׷��BUFF
	buff_element_addbuff_when_beadd_small_type = 52,
	// ����
	buff_element_immuno						= 53,
	// ����Ѫ����������
	buff_element_trigger_skill_by_hp		= 54,
	// ����һ����������
	buff_element_trigger_skill_by_critical_blow	= 55,
	// �ܹ���������������
	buff_element_trigger_skill_by_beattack_count= 56,
	// ����������������
	buff_element_trigger_skill_by_attack_count	= 57,
	// ����
	buff_element_trigger_story_play				= 58,
	// ���ݳ��ֻغ�������BUFF
	buff_element_addbuff_by_round_count			= 59,
	// �������Ѫ��
	buff_element_add_max_hp						= 60,
	// ����Ѫ���ı�����
	buff_element_change_attr_by_hp				= 61,
	// ����Ŀ��Ѫ���ı�����
	buff_element_change_attr_by_target_hp		= 62,
	// ����Ѫ����Ѫ
	buff_element_addhp_by_hpchange				= 63,
	// ��ʱ�ı�����(���ι�������Ч)
	buff_element_change_temp_attr				= 64,
	// ���ط���(ս����ÿ��Ѫ��С��X%ʱ�����м���CD-X������0�������������1���ж�����)
	buff_element_extreme_hit_back				= 65,
	// �˺�������BUFF
	buff_element_add_buff_behurt				= 66,
	// �����˺�
	buff_element_skill_ext_damage				= 67,
	// ���˺�ʱ����
	buff_element_speed_up_when_behurt			= 68,
	// �������Ѫ��
	buff_element_sub_max_hp						= 69,
	// �����������ж�����
	buff_element_other_action_count				= 70,
	// ���������ٶ�
	buff_element_in_control_speed				= 71,
	// �Ƴ����������ٶ�
	buff_element_out_control_speed				= 72,
	// �е��˺�
	buff_element_bear_hurt						= 73,
	// ���ǵ�
	buff_element_death_guard					= 74,
	// ���ǵ�����
	buff_element_death_guard_energy				= 75,
	// �������BUFF
	buff_element_add_rand_buff					= 76,
	// �˺��ۼ�
	buff_element_all_team_damage				= 77,
	// ����˺�
	buff_element_rand_damage					= 78,
	// ����ȫ��
	buff_element_heal_all_team					= 79,
	// ȫ�ӻ���
	buff_element_all_team_shield				= 80,
	// ȫ�ӻ���
	buff_element_all_team_damage_turn			= 81,
	// ��긴��
	buff_element_soul_relive					= 82,
	// �ٻ�
	buff_element_summon							= 83,
	//��������
	//(�غϿ�ʼǰ�����ļ��ܣ���Ҫ��ս��ʱ���� process ʹ�õ��ǻغ���key , ���ܵ��¿ͻ��˴���2��Ч��)
	buff_element_continue_heal					= 84,
	//նɱ
	buff_element_beheaded						= 85,
	//��������ת��buff
	buff_element_suck_and_turn					= 86,
	//������������buff_id
	buff_element_attr_add_buff_id				= 87,
	//��������buff_id
	buff_element_attr_add_coeff					= 88,
	//����С�����Ƿ��������buff
	buff_add_by_other_buff_type                 = 89,
	//���Ը�������buff
	buff_add_by_attribute                       = 90,
	//����buffת������buff(Ĭ�ϲ���ת��)			
	buff_element_turn_other_buff				= 91,
	//����
	buff_element_alomst_die						= 92,
	//���ָ�����ܽ��з���
	buff_element_cd_attack_back					= 93,
	//ָ���˷�̯�˺�
	buff_element_shared_hurt					= 94,
	//��Ѫ
	buff_element_suck_blood						= 95,
	//ָ��buff���й���
	buff_element_attack_by_buff					= 96,
	//�⻷���ܸı�����
	buff_element_special_buff_change_att		= 97,
	//buff����ת��ΪĿ������
	buff_element_buff_layer_change_att			= 98,	
	//�佫����BUFF
	buff_add_by_hero_alive                      = 100,
	//�ٶ�ֵ����BUFF
	buff_element_add_by_spped                   = 101,
	//����佫��������
	buff_element_alive_attr                     = 102,

	//����ר������
	buff_element_xiangyu_skill2					= 103,
	buff_element_xiangyu_skill3					= 104,

	//ı���
	buff_element_move_after_think				= 105,
	//ı�����Э
	buff_element_move_with_think				= 106,
	//�������ͼ���(ר������)
	buff_element_lsm_gift						= 107,
	//buff�ﵽָ������׷��buff
	buff_element_add_buff_by_buff_layer			= 108,
	//����Ԫ����������ָ����buff���ӵ���ֵ(A˳��BUFF�ӵ�ĳ����ֵ����Buff�Ƴ�ʱ�����������)
	buff_element_clear_buff_add_attr			= 109,
	//�ܵ�������������
	buff_element_miss_by_beattack_num			= 110,
	//Ѫ������ʱ�����ж���
	buff_element_att_speed_by_hp_sub			= 111,
	//�����˺�����ը
	buff_element_suck_and_bomb					= 113,
	//��¼���غ϶��Լ���ɵ��˺�
	buff_element_record_hurt					= 114,
	//�����ٶ�
	buff_element_limit_max_speed				= 115,
	//����buffʱ׷��buff
	buff_element_addbuff_when_add_small_type    = 116,
	//�����ܹ��������Ƴ�buff
	buff_element_add_buff_of_hurt_count			= 117,
};


// ���㷽ʽ(1-�̶���ֵ��2-�����������ٷֱȣ�3-�����������ٷֱȣ�4-�����������ٷֱȣ�5-�������ٶȰٷֱȣ�
// 6-���������ٷֱ�,7-������ս�����,8-������ǰ�����ٷֱ� ,9-�˺��ٷֱ�,10-����ֵ��11-���������ӻ��ܣ�12-������ʼ�������)
enum calc_attr_mode
{
	calc_attr_mode_value			= 1,
	calc_attr_mode_attacker_attack	= 2,
	calc_attr_mode_attacker_hp		= 3,
	calc_attr_mode_attacker_defend	= 4,
	calc_attr_mode_attacker_speed	= 5,
	calc_attr_mode_self_hp			= 6,
	calc_attr_mode_attacker_damage	= 7,
	calc_attr_mode_self_cur_hp		= 8,
	calc_attr_mode_attacker_round_damage	= 9,
	calc_attr_mode_lucky_point      = 10,
	calc_attr_mode_heal_to_shield   = 11,
	calc_attr_mode_self_max_hp_without_buff = 12,
};

class buff_element_t
{
public:
	buff_element_t();
	~buff_element_t();

	static bool regist(std::map<uint32_t, elenment_func>& container);

	static uint32_t element_notify_type(uint32_t type);

	static uint32_t element_test(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner, uint32_t event_num);
	static uint32_t change_attr(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner , uint32_t event_num);
	static uint32_t change_temp_attr(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner , uint32_t event_num);
	static uint32_t change_attr_by_hp(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner , uint32_t event_num);
	static uint32_t change_attr_by_target_hp(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner , uint32_t event_num);
	static uint32_t trigger_skill(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner , uint32_t event_num);

	static uint32_t trigger_skill_by_hp(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner , uint32_t event_num);
	static uint32_t trigger_skill_by_critical_blow(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner , uint32_t event_num);
	static uint32_t trigger_skill_by_beattack_count(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner , uint32_t event_num);
	static uint32_t trigger_skill_by_attack_count(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner , uint32_t event_num);

	static uint32_t use_skill(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner , uint32_t event_num);
	static uint32_t add_sp(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner , uint32_t event_num);
	static uint32_t steal_buff(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner , uint32_t event_num);
	static uint32_t damage(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner , uint32_t event_num);
	static uint32_t heal(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner , uint32_t event_num);
	static uint32_t heal_special(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner , uint32_t event_num);
	static uint32_t heal_all_team(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner , uint32_t event_num);
	static uint32_t addhp_by_hpchange(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner , uint32_t event_num);
	static uint32_t add_max_hp(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner , uint32_t event_num);
	static uint32_t sub_max_hp(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner , uint32_t event_num);
	static uint32_t change_att_speed(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner , uint32_t event_num);
	static uint32_t add_by_spped(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner , uint32_t event_num);

	static uint32_t change_attr_without_num(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner , uint32_t event_num);
	static uint32_t relive(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner , uint32_t event_num);
	static uint32_t change_cd(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner , uint32_t event_num);
	
	static uint32_t cleanup(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner , uint32_t event_num);
	static uint32_t balance_hp(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner , uint32_t event_num);
	static uint32_t balance_speed(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner , uint32_t event_num);
	static uint32_t switch_hp(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner , uint32_t event_num);
	static uint32_t switch_speed(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner , uint32_t event_num);
	
	static uint32_t union_attack(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner , uint32_t event_num);
	static uint32_t switch_buff(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner , uint32_t event_num);
	static uint32_t change_attr_all_team(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner , uint32_t event_num);
	static uint32_t change_attr_all_enemy(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner , uint32_t event_num);
	static uint32_t shield(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner , uint32_t event_num);
	static uint32_t bomb(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner , uint32_t event_num);
	static uint32_t detonate_bomb(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner , uint32_t event_num);
	static uint32_t addbuff_by_debuff(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner , uint32_t event_num);
	static uint32_t addbuff_by_round_count(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner , uint32_t event_num);
	static uint32_t profit_buff_to_debuff(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner , uint32_t event_num);

	static uint32_t remove_buff_layer(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner , uint32_t event_num);
	static uint32_t heal_overflow(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner , uint32_t event_num);
	static uint32_t addbuff_when_exist(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner , uint32_t event_num);
	static uint32_t add_rand_buff(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner , uint32_t event_num);
	static uint32_t addbuff_when_addbuff(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner , uint32_t event_num);
	static uint32_t addbuff_when_over_layer(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner , uint32_t event_num);

	static uint32_t remove_buff(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner , uint32_t event_num);

	static uint32_t add_buff_to_target(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner , uint32_t event_num);

	static uint32_t pursue_and_attack(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner , uint32_t event_num);
	static uint32_t taunt(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner , uint32_t event_num);
	static uint32_t forget(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner , uint32_t event_num);
	static uint32_t silent(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner , uint32_t event_num);

	static uint32_t add_random_buff(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner , uint32_t event_num);
	static uint32_t add_buff_after_damage(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner , uint32_t event_num);
	static uint32_t add_buff_behurt(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner , uint32_t event_num);
	static uint32_t sub_hp(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner , uint32_t event_num);
	static uint32_t dead_when_relive(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner , uint32_t event_num);
	static uint32_t activation_skill(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner , uint32_t event_num);
	static uint32_t add_buff_by_debuf_ratio(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner , uint32_t event_num);
	static uint32_t change_buff_round(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner , uint32_t event_num);

	static uint32_t change_model(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner , uint32_t event_num);
	static uint32_t athanasia(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner , uint32_t event_num);

	static uint32_t strategy_bomb(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner , uint32_t event_num);
	static uint32_t add_buff_by_hp_ratio(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner , uint32_t event_num);
	static uint32_t add_buff_by_speed(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner , uint32_t event_num);

	static uint32_t add_random_buff_layer(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner , uint32_t event_num);
	static uint32_t add_buff_by_small_type(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner , uint32_t event_num);
	static uint32_t remove_buff_by_small_type(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner , uint32_t event_num);
	static uint32_t addbuff_when_beadd_small_type(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner , uint32_t event_num);
	static uint32_t addbuff_when_add_small_type(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner , uint32_t event_num);


	static uint32_t immuno(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner , uint32_t event_num);
	static uint32_t story_play(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner , uint32_t event_num);
	static uint32_t extreme_hit_back(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner , uint32_t event_num);
	static uint32_t skill_ext_damage(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner , uint32_t event_num);
	static uint32_t speed_up_when_behurt(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner , uint32_t event_num);
	static uint32_t other_action_count(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner , uint32_t event_num);
	static uint32_t in_control_speed(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner , uint32_t event_num);
	static uint32_t out_control_speed(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner , uint32_t event_num);
	static uint32_t bear_hurt(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner , uint32_t event_num);
	static uint32_t death_guard_energy(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner , uint32_t event_num);
	static uint32_t death_guard(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner , uint32_t event_num);
	static uint32_t all_team_damage(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner , uint32_t event_num);
	static uint32_t rand_damage(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner , uint32_t event_num);
	static uint32_t all_team_shield(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner , uint32_t event_num);
	static uint32_t all_team_damage_turn(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner , uint32_t event_num);
	static uint32_t soul_relive(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner , uint32_t event_num);
	static uint32_t summon(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner , uint32_t event_num);
	static uint32_t continue_heal(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner , uint32_t event_num);
	static uint32_t be_buff_beheaded(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner, uint32_t event_num);
	static uint32_t be_buff_add_by_other_type(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner, uint32_t event_num);
	static uint32_t be_buff_add_by_attribute(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner, uint32_t event_num);
	static uint32_t be_buff_add_by_hero_alive(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner, uint32_t event_num);
	static uint32_t alive_attr(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner, uint32_t event_num);
	static uint32_t suck_attr_and_turn(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner, uint32_t event_num);
	static uint32_t attr_add_buff_id(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner, uint32_t event_num);
	static uint32_t attr_add_coeff(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner, uint32_t event_num);
	static uint32_t turn_other_buff(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner, uint32_t event_num);
	static uint32_t alomst_die(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner, uint32_t event_num);
	static uint32_t cd_attack_back(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner, uint32_t event_num);
	static uint32_t shared_hurts(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner, uint32_t event_num);
	static uint32_t suck_blood(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner, uint32_t event_num);
	static uint32_t attack_by_buff(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner, uint32_t event_num);
	static uint32_t special_buff_change_att(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner, uint32_t event_num);
	static uint32_t buff_layer_change_att(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner, uint32_t event_num);

	static uint32_t xiangyu_skill2(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner, uint32_t event_num);
	static uint32_t xiangyu_skill3(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner, uint32_t event_num);

	static uint32_t move_after_think(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner, uint32_t event_num);
	static uint32_t move_with_think(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner, uint32_t event_num);
	static uint32_t lsm_gift(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner, uint32_t event_num);
	static uint32_t add_buff_by_buff_layer(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner, uint32_t event_num);
	static uint32_t clear_buff_add_attr(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner, uint32_t event_num);
	static uint32_t miss_by_beattack_num(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner, uint32_t event_num);
	static uint32_t add_att_speed_by_hp_sub(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner, uint32_t event_num);
	static uint32_t suck_and_bomb(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner, uint32_t event_num);
	static uint32_t record_hurt(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner, uint32_t event_num);
	static uint32_t limit_max_speed(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner, uint32_t event_num);
	static uint32_t add_buff_of_hurt_count(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner, uint32_t event_num);
public:
	static void inc_all_team_attr(fight_hero_ptr owner, int type, int value, int chg_type, uint32_t buff_tid );

	static void dec_all_team_attr(fight_hero_ptr owner, int type, int value, int chg_type, uint32_t buff_tid );

	static void inc_all_enemy_attr(fight_hero_ptr owner, int type, int value, int chg_type, uint32_t buff_tid);

	static void dec_all_enemy_attr(fight_hero_ptr owner, int type, int value, int chg_type, uint32_t buff_tid);

	static void heal_all_team_hp(fight_hero_ptr owner, fight_hero_ptr attacker, buff_ptr buff, int value);

	static uint32_t calc_value(fight_hero_ptr owner, fight_hero_ptr attacker, int type, int value, int limit);

	static uint32_t calc_heal_special_value(fight_hero_ptr owner, fight_hero_ptr attacker, int type, int buff_id, int value, int param);

	static fight_hero_ptr get_target_by_type(fight_hero_ptr owner, fight_hero_ptr attacker, SkillEffectTable* skill_conf, buff_ptr buff, uint32_t target_type);
};

#endif//__BUFF_ELEMENT_HPP__