#pragma once

#include "macros.hpp"


///�ɾͻ������(�ɾ͡���Ծ����)
enum achieve_common_type : uint32_t
{
	achieve_common_achieve,
	achieve_common_active_reward,
};

/**
* @brief �ɾ�����
*/
// �ɾ�����
//enum Achieve_Event_Type : uint32_t
//{
//	ACHIEVE_EVENT_TYPE_BEGIN,
//	ACHIEVE_MAIN_ROLE_LEVEL = 1,							///���ǵȼ�
//	ACHIEVE_MAIN_ROLE_FIGHT = 2,							///����ս����
//	ACHIEVE_EQUIP_TYPE_NUMBER = 3,							///���[n]��[ĳ]Ʒ�ʵ�װ��
//	ACHIEVE_EQUIP_TYPE_SAME = 4,							///����[ĳ]Ʒ����װ
//	ACHIEVE_EQUIP_RECAST = 5,								///����[n��]װ��
//	ACHIEVE_EQUIP_EXATTR_NUMBER_STAR = 6,					///װ��ͬʱӵ��[n]��[n]���д�
//	ACHIEVE_HERO_NUMBER = 7,								///ӵ��[n]���佫
//	ACHIEVE_HERO_NUMBER_LEVEL = 8,							///ӵ��[n]��[n]���佫
//	ACHIEVE_HERO_NUMBER_TYPE = 9,							///ӵ��[n]Ʒ���佫[n]��
//	ACHIEVE_HERO_NUMBER_STAR = 10,							///ӵ��[n]��[n]�Ǽ��佫
//	ACHIEVE_HERO_NUMBER_TALENT_LEVEL = 11,					///��[n]���佫�츳������[n]��
//	ACHIEVE_HERO_NUMBER_SKILL_LEVEL = 12,					///��[n]���佫����������[n]��
//	ACHIEVE_TOTAL_COPPER = 13,								///�ۼƻ��[n]��ͭ��
//	ACHIEVE_SHOP_BUY_NUMBER = 14,							///��[n]���̵깺��[n]��
//
//	ACHIEVE_MOUNT_NUMBER_TYPE = 15,							///���[n]Ʒ������[n]ƥ
//	ACHIEVE_TASK_FINISH_NUMBER = 16,						///���[]������
//	ACHIEVE_TASK_FINISH_COUNTRY_NUMBER = 17,				///��ɹ�������[]��
//	ACHIEVE_TASK_FINISH_FAMILY_NUMBER = 18,					///��ɼ�������[]��
//	ACHIEVE_TRADE_NUMBER = 19,								///�������[]��
//	ACHIEVE_TRADE_TOTAL_GAIN_NUMBER = 20,					///�����ۼ�����[]
//	ACHIEVE_COUNTRY_BOUNTY_NUMBER = 21,						///���[]�ǹ�������[]��
//	ACHIEVE_TREASURE_ACTIVITY_NUMBER = 22,					///���Ѱ���[]��
//	ACHIEVE_TREASURE_ACTIVITY_SPECIAL_EVENT_NUMBER = 23,	///Ѱ�������[]�������¼�
//	ACHIEVE_CHANNEL_NUMBER = 24,								///ʹ��[]Ƶ��[n]��
//	ACHIEVE_FAMILY = 25,									///����һ������:�������ι�ְ
//	ACHIEVE_COUNTRY = 26,									///����һ������:�������ι�ְ
//	ACHIEVE_EMPIRE_CITY_CHALLENGE_NUMBER = 28,				///����[]�λʳ�Լս
//	ACHIEVE_PK_VALUE = 29,									///PKֵ�ﵽ100
//	ACHIEVE_TRIAL_TASK_NUMBER = 30,								///���[]��������
//	ACHIEVE_TREASURE_ONCE_GAIN_VALUE = 31,						///���̵�������[]
//	ACHIEVE_BOUNTY_FILL_START_NUMBER = 32,					///�������͵��մﵽ[n]��[]��
//	ACHIEVE_PACKAGE_COPPER_NUMBER = 33,						///�����ڳ���[]ͭ��
//
//	ACHIEVE_CHANNEL_CHAT = 34,								///[n]Ƶ������[m]��
//	ACHIEVE_HAVE_FRIEND_NUMBER = 35,						///ӵ��[n]����
//	ACHIEVE_TRADE_SINGLE_GAIN = 36,							///���̵�������ﵽ[]
//	ACHIEVE_TRIAL_TASK_NUMBER_STAR = 37,					///���[n]��[m]����������
//	ACHIEVE_TOTAL_LOGIN = 38,								///�ۼƵ�½
//	ACHIEVE_SUCCESSION_LOGIN = 39,							///������½
//	ACHIEVE_RANK_TYPE_LEVEL = 40,							///[]���а�ﵽ[]����
//	ACHIEVE_CHASE_GHOST_TASK_NUMBER = 41,					///׽���������[]��
//	ACHIEVE_CHASE_GHOST_KILL_MONSTER = 42,					///׽������ɱ��[]��
//	ACHIEVE_FIELD_BOSS_KILL_NUMBER = 43,					///Ұ��BOSS��ɱ[]��
//	ACHIEVE_ARENA_BATTLE_SINGLE_WIN_NUMBER = 44,			///�����������[]ʤ
//	ACHIEVE_ARENA_BATTLE_TOTAL_WIN_NUMBER = 45,				///��������ܼ�[]ʤ
//	ACHIEVE_GENERAL_CHALLENGE_TOTAL_PASS = 46,				///������ս�ۼ�ͨ��[]��
//	ACHIEVE_TOWER_LEVEL = 47,								///ǧ�����ﵽ[]��
//	ACHIEVE_EXPEDITION_SINGLE_POINT = 48,					///Զ�����λ��ִﵽ[]
//	ACHIEVE_EXPEDITION_STORE_BUY_NUMBER = 49,				///Զ���������̵깺��[]�λ���
//	ACHIEVE_EXPEDITION_STORE_TOTAL_COST_HONOUR = 50,		///�����̵��ۼ�����[]��ҫֵ
//	ACHIEVE_EXPEDITION_HELP_OTHERS = 51,					///Զ����������[]��
//	ACHIEVE_EXPEDITION_CHALLENGE_PASS_NUMBER = 52,			///Զ����ս[N]�ιؿ�[M]��
//	ACHIEVE_FAMILY_WAR_NUMBER = 53,							///�μӼ���ս[]��
//	ACHIEVE_FAMILY_WAR_KILL_PLAYER_NUMBER = 54,				///����ս��ɱ���[]��
//	ACHIEVE_FAMILY_WAR_DRAGON_HARM = 55,					///����ս�ж���������˺�[]
//	ACHIEVE_FAMILY_WAR_OCCUPIED_SUN_OR_MOON_NUMBER = 56,	///����ս��ռ��������̨[]��
//	ACHIEVE_KING_WAR_NUMBER = 57,							///����ս[]����
//	ACHIEVE_KING_WAR_KILL_PLAYER_NUMBER = 58,				///����ս��ɱ���[]��
//	ACHIEVE_KING_WAR_DRAGON_HARM = 59,						///����ս�ж���������˺�[]
//	ACHIEVE_KING_WAR_QUICK_REVIVE = 60,						///����ս����������[]��
//	ACHIEVE_KING_WAR_START_WEAPON = 61,						///����ս����������[]��
//	ACHIEVE_GRADE_PASS_DUNGEON_NUMBER = 62,					///��[]����ͨ�ظ���[]��
//	ACHIEVE_EVENT_TYPE_END,
//};

//role ������������(�Ǽ�/�츳�ȼ�/���ܵȼ�/Ʒ��)
enum Achieve_Hero_Cultivate_Type
{
	Achieve_Role_level = 0,
	Achieve_Role_Quality = 1,
	Achieve_Role_Star = 2,
	Achieve_Role_Talent = 3,
	Achieve_Role_Skill = 4,
};


enum PERSON_RECORD_TYPE : uint32_t
{
	ACHIEVE_PERSON_REOCRD_CREATETIME = 1,
	ACHIEVE_PERSON_REOCRD_PROFESSION = 2,
	ACHIEVE_PERSON_REOCRD_FIGHT = 3,
	ACHIEVE_PERSON_REOCRD_COUNTRY = 4,
	ACHIEVE_PERSON_REOCRD_COUNTRY_POSITION = 5,
	ACHIEVE_PERSON_REOCRD_FAMILY = 6,
	ACHIEVE_PERSON_REOCRD_FAMILY_POSITION = 7,
	ACHIEVE_PERSON_REOCRD_HERO_NUMBER = 8,
	ACHIEVE_PERSON_REOCRD_HERO_FINGHT_MAX = 9,
	ACHIEVE_PERSON_REOCRD_EQUIP_MAX = 10,
};

enum ACHIEVE_LEVEL_TYPE : uint32_t
{
	ACHIEVE_LEVEL_0 = 0,
	ACHIEVE_LEVEL_1 = 1,
	ACHIEVE_LEVEL_2 = 2,
};


#define MAX_BIT 32

///���ȼ���1:[0,9] 2:[10,19] 3:[20,29].....
#define MAX_LEVEL_RANGE 100