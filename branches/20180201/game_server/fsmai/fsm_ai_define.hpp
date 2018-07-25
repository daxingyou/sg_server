#ifndef __GAME_FSM_AI_DEFINE__
#define __GAME_FSM_AI_DEFINE__

#include "macros.hpp"
#include "common.pb.h"

#include <vector>

namespace ai
{
	enum ai_event : uint32_t
	{
		ai_event_none = 0,
		ai_event_killed,		// ����ɱ
		ai_event_enter_fight,	// ����ս���¼�
		ai_event_win_fight,		// ս��ʤ��

		ai_event_max,
	};

	enum ai_act : uint32_t
	{
		ai_act_none = 0,
		ai_act_death,			// ����
		ai_act_disappear,		// �뿪����
		ai_act_delete,			// ɾ������
		ai_act_revive,			// ����
		ai_act_start_alert,		// ����
		ai_act_start_fight,		// ս��
		ai_act_change_state,	// ״̬�л�

		ai_act_max,
	};

	enum ai_mode : uint32_t
	{
		ai_mode_none = 0,
		ai_mode_time_disappear,		// ʬ�嵽ʱ����ʧ
		ai_mode_time_revive,		// ��ʱ���˸���
		ai_mode_trigger_alert,		// ����
		ai_mode_timer_alert,		// ʱ�䵽�˽���ս��

		ai_mode_max,
	};
}

namespace fsm
{
	struct ai_act
	{
		uint32_t act_type = 0;		// ����
		std::string act_param = "";	// ����
	};

	typedef std::vector<ai_act> act_vec;

	struct ai_event
	{
		uint32_t event_type = 0;	// �¼�
		act_vec acts;				// ��Ϊ
	};

	typedef std::vector<ai_event> event_vec;

	struct ai_mode
	{
		uint32_t mode_type = 0;		// ģ��
		act_vec acts;				// ��Ϊ
	};

	typedef std::vector<ai_mode> mode_vec;

	struct ai_state;
	typedef std::vector<ai_state> state_vec;

	struct ai_state
	{
		proto::common::object_state_type state_type = proto::common::object_state_none;	// ״̬����
		event_vec events;																// ������¼�
		mode_vec modes;																	// ģ��
		state_vec children;																// ����Ϊ
	};
}

#endif