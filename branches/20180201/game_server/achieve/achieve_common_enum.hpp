#pragma once

#include "macros.hpp"


///成就基础类别(成就、活跃奖励)
enum achieve_common_type : uint32_t
{
	achieve_common_achieve,
	achieve_common_active_reward,
};

/**
* @brief 成就类型
*/
// 成就类型
//enum Achieve_Event_Type : uint32_t
//{
//	ACHIEVE_EVENT_TYPE_BEGIN,
//	ACHIEVE_MAIN_ROLE_LEVEL = 1,							///主角等级
//	ACHIEVE_MAIN_ROLE_FIGHT = 2,							///主角战斗力
//	ACHIEVE_EQUIP_TYPE_NUMBER = 3,							///获得[n]件[某]品质的装备
//	ACHIEVE_EQUIP_TYPE_SAME = 4,							///集齐[某]品质套装
//	ACHIEVE_EQUIP_RECAST = 5,								///重铸[n次]装备
//	ACHIEVE_EQUIP_EXATTR_NUMBER_STAR = 6,					///装备同时拥有[n]个[n]星判词
//	ACHIEVE_HERO_NUMBER = 7,								///拥有[n]名武将
//	ACHIEVE_HERO_NUMBER_LEVEL = 8,							///拥有[n]名[n]级武将
//	ACHIEVE_HERO_NUMBER_TYPE = 9,							///拥有[n]品质武将[n]名
//	ACHIEVE_HERO_NUMBER_STAR = 10,							///拥有[n]名[n]星级武将
//	ACHIEVE_HERO_NUMBER_TALENT_LEVEL = 11,					///将[n]名武将天赋提升至[n]级
//	ACHIEVE_HERO_NUMBER_SKILL_LEVEL = 12,					///将[n]名武将技能提升至[n]级
//	ACHIEVE_TOTAL_COPPER = 13,								///累计获得[n]个铜币
//	ACHIEVE_SHOP_BUY_NUMBER = 14,							///在[n]类商店购物[n]次
//
//	ACHIEVE_MOUNT_NUMBER_TYPE = 15,							///获得[n]品质坐骑[n]匹
//	ACHIEVE_TASK_FINISH_NUMBER = 16,						///完成[]个任务
//	ACHIEVE_TASK_FINISH_COUNTRY_NUMBER = 17,				///完成国家任务[]次
//	ACHIEVE_TASK_FINISH_FAMILY_NUMBER = 18,					///完成家族任务[]次
//	ACHIEVE_TRADE_NUMBER = 19,								///完成跑商[]次
//	ACHIEVE_TRADE_TOTAL_GAIN_NUMBER = 20,					///跑商累计利润[]
//	ACHIEVE_COUNTRY_BOUNTY_NUMBER = 21,						///完成[]星国家悬赏[]次
//	ACHIEVE_TREASURE_ACTIVITY_NUMBER = 22,					///完成寻宝活动[]次
//	ACHIEVE_TREASURE_ACTIVITY_SPECIAL_EVENT_NUMBER = 23,	///寻宝活动遭遇[]次特殊事件
//	ACHIEVE_CHANNEL_NUMBER = 24,								///使用[]频道[n]次
//	ACHIEVE_FAMILY = 25,									///加入一个家族:包括担任官职
//	ACHIEVE_COUNTRY = 26,									///加入一个国家:包括担任官职
//	ACHIEVE_EMPIRE_CITY_CHALLENGE_NUMBER = 28,				///进行[]次皇城约战
//	ACHIEVE_PK_VALUE = 29,									///PK值达到100
//	ACHIEVE_TRIAL_TASK_NUMBER = 30,								///完成[]试炼任务
//	ACHIEVE_TREASURE_ONCE_GAIN_VALUE = 31,						///跑商单次利润[]
//	ACHIEVE_BOUNTY_FILL_START_NUMBER = 32,					///国家悬赏当日达到[n]星[]次
//	ACHIEVE_PACKAGE_COPPER_NUMBER = 33,						///包裹内持有[]铜币
//
//	ACHIEVE_CHANNEL_CHAT = 34,								///[n]频道聊天[m]次
//	ACHIEVE_HAVE_FRIEND_NUMBER = 35,						///拥有[n]好友
//	ACHIEVE_TRADE_SINGLE_GAIN = 36,							///跑商单次利润达到[]
//	ACHIEVE_TRIAL_TASK_NUMBER_STAR = 37,					///完成[n]次[m]星试炼任务
//	ACHIEVE_TOTAL_LOGIN = 38,								///累计登陆
//	ACHIEVE_SUCCESSION_LOGIN = 39,							///连续登陆
//	ACHIEVE_RANK_TYPE_LEVEL = 40,							///[]排行榜达到[]名次
//	ACHIEVE_CHASE_GHOST_TASK_NUMBER = 41,					///捉鬼任务完成[]次
//	ACHIEVE_CHASE_GHOST_KILL_MONSTER = 42,					///捉鬼任务杀怪[]数
//	ACHIEVE_FIELD_BOSS_KILL_NUMBER = 43,					///野外BOSS击杀[]数
//	ACHIEVE_ARENA_BATTLE_SINGLE_WIN_NUMBER = 44,			///跨服竞技单次[]胜
//	ACHIEVE_ARENA_BATTLE_TOTAL_WIN_NUMBER = 45,				///跨服竞技总计[]胜
//	ACHIEVE_GENERAL_CHALLENGE_TOTAL_PASS = 46,				///名将挑战累计通过[]次
//	ACHIEVE_TOWER_LEVEL = 47,								///千层塔达到[]层
//	ACHIEVE_EXPEDITION_SINGLE_POINT = 48,					///远征单次积分达到[]
//	ACHIEVE_EXPEDITION_STORE_BUY_NUMBER = 49,				///远征在神秘商店购买[]次货物
//	ACHIEVE_EXPEDITION_STORE_TOTAL_COST_HONOUR = 50,		///神秘商店累计消耗[]荣耀值
//	ACHIEVE_EXPEDITION_HELP_OTHERS = 51,					///远征帮助他人[]次
//	ACHIEVE_EXPEDITION_CHALLENGE_PASS_NUMBER = 52,			///远征挑战[N]次关卡[M]次
//	ACHIEVE_FAMILY_WAR_NUMBER = 53,							///参加家族战[]次
//	ACHIEVE_FAMILY_WAR_KILL_PLAYER_NUMBER = 54,				///家族战击杀玩家[]次
//	ACHIEVE_FAMILY_WAR_DRAGON_HARM = 55,					///家族战中对龙柱造成伤害[]
//	ACHIEVE_FAMILY_WAR_OCCUPIED_SUN_OR_MOON_NUMBER = 56,	///家族战中占领日月曜台[]次
//	ACHIEVE_KING_WAR_NUMBER = 57,							///王城战[]次数
//	ACHIEVE_KING_WAR_KILL_PLAYER_NUMBER = 58,				///王城战击杀玩家[]次
//	ACHIEVE_KING_WAR_DRAGON_HARM = 59,						///王城战中对龙脉造成伤害[]
//	ACHIEVE_KING_WAR_QUICK_REVIVE = 60,						///王城战中立即复活[]次
//	ACHIEVE_KING_WAR_START_WEAPON = 61,						///王城战中启动神器[]次
//	ACHIEVE_GRADE_PASS_DUNGEON_NUMBER = 62,					///以[]评分通关副本[]次
//	ACHIEVE_EVENT_TYPE_END,
//};

//role 基础养成类型(星级/天赋等级/技能等级/品质)
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

///最大等级段1:[0,9] 2:[10,19] 3:[20,29].....
#define MAX_LEVEL_RANGE 100