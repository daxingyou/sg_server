#pragma once

#include "macros.hpp"

// ����״̬
enum family_impeach_type : uint32_t
{
	family_impeach_type_none = 0,
	family_impeach_type_agree = 1,		// ͬ��
	family_impeach_type_disagree =2,	// ��ͬ��
};

enum family_war_side : uint32_t
{
	family_war_side_none = 0,	// ��Ч
	family_war_side_left = 1,	// ��
	family_war_side_right = 2,	// �ҷ�	
};

// ����������ȼ�
#define FAMILY_BUILD_MAX_LEVEL 5

// ÿ�η��ͼ����б��������
#define FAMILY_PAGE_COUNT 7

// ��ʷ��ʾ����
#define FAMILY_WAR_HISTORY_SIZE 3

// ������ս����������һ���Կ���������ս����������
#define FAMILY_WAR_MAX_COUNT 100
