#pragma once

#include "macros.hpp"

enum family_war_side_type : uint32_t
{
	family_war_side_left = 0,	// ��
	family_war_side_right = 1,	// �ҷ�

	family_war_side_max,		// ���ֵ
	family_war_side_none = family_war_side_max,	// ���ǲ�ս��
};

enum family_war_kill_title : uint32_t
{
	family_war_kill_title_0 = 0,	// û��
	family_war_kill_title_1 = 1,	// ��ɱ��ɱ
	family_war_kill_title_2 = 2,	// �����ܵ�
	family_war_kill_title_3 = 3,	// ��������
	family_war_kill_title_4 = 4,	// ����ɱ¾
};

enum family_war_long_type : uint32_t
{
	family_war_long_wind = 0,
	family_war_long_water,
	family_war_long_fire,
	family_war_long_gold,
};

enum family_war_long_state : uint32_t
{
	family_war_long_state_normal = 0,
	family_war_long_state_active,
	family_war_long_state_die,
};

enum family_war_platform_type : uint32_t
{
	family_war_platform_sun = 0,	// ��
	family_war_platform_moon = 1,	// ��

	family_war_platform_max,		// ���ֵ
};

#define FAMILY_WAR_RANK_SCORE 10
