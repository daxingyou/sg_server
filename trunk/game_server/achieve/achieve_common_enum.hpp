#pragma once

#include "macros.hpp"


///成就基础类别(成就、活跃奖励)
enum achieve_common_type : uint32_t
{
	achieve_common_achieve,
	achieve_common_active_reward,
};


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