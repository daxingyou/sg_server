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
		ai_event_killed,		// 被击杀
		ai_event_enter_fight,	// 进入战斗事件
		ai_event_win_fight,		// 战斗胜利

		ai_event_max,
	};

	enum ai_act : uint32_t
	{
		ai_act_none = 0,
		ai_act_death,			// 死亡
		ai_act_disappear,		// 离开场景
		ai_act_delete,			// 删除怪物
		ai_act_revive,			// 复活
		ai_act_start_alert,		// 警戒
		ai_act_start_fight,		// 战斗
		ai_act_change_state,	// 状态切换

		ai_act_max,
	};

	enum ai_mode : uint32_t
	{
		ai_mode_none = 0,
		ai_mode_time_disappear,		// 尸体到时间消失
		ai_mode_time_revive,		// 到时间了复活
		ai_mode_trigger_alert,		// 警戒
		ai_mode_timer_alert,		// 时间到了进入战斗

		ai_mode_max,
	};
}

namespace fsm
{
	struct ai_act
	{
		uint32_t act_type = 0;		// 类型
		std::string act_param = "";	// 参数
	};

	typedef std::vector<ai_act> act_vec;

	struct ai_event
	{
		uint32_t event_type = 0;	// 事件
		act_vec acts;				// 行为
	};

	typedef std::vector<ai_event> event_vec;

	struct ai_mode
	{
		uint32_t mode_type = 0;		// 模块
		act_vec acts;				// 行为
	};

	typedef std::vector<ai_mode> mode_vec;

	struct ai_state;
	typedef std::vector<ai_state> state_vec;

	struct ai_state
	{
		proto::common::object_state_type state_type = proto::common::object_state_none;	// 状态类型
		event_vec events;																// 处理的事件
		mode_vec modes;																	// 模块
		state_vec children;																// 子行为
	};
}

#endif