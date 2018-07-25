#ifndef __GAME_DUNDEON_COMMON_H__
#define __GAME_DUNDEON_COMMON_H__

#include "macros.hpp"

namespace dungeon
{
	enum dungeon_type_enum : uint32_t
	{
		dungeon_type_none = 0,			// ��Ч
		dungeon_type_plot = 1,			// ����
		dungeon_type_daily = 2,			// �ճ�
		dungeon_type_tower = 5,			// ����ר��
	};

	enum refresh_type_enum : uint32_t
	{
		refresh_type_none = 0,	// ����ˢ��
		refresh_type_day = 1,	// ÿ��
		refresh_type_week = 2,	// ÿ��
	};

	enum pass_type_enum : uint32_t
	{
		pass_type_none = 0,	// ��Ч
		pass_type_kill_any = 1,		// ��ɱ������ﵽ����ֵ
		pass_type_kill_all = 2,		// ��ɱ���й��ﵽ����ֵ
	};

	enum team_type_enum : uint32_t
	{
		team_type_none = 0,			// ������
		team_type_single = 1,		// ���뵥��
		team_type_team_2 = 2,		// ��� ����>=2
		team_type_team_3 = 3,		// ��� ����>=3
	};

	enum team_check_enum : uint32_t
	{
		team_need_team_check = 1,		// ��λȷ��
		team_no_team_check = 2,			// ����Ҫ��λȷ��
	};
}

#define MAX_DUNGEON_FIGHT_TIME	100		// һ�����������ս����

#endif
