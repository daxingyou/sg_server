#ifndef __GAME_SCENE_COMMON_H__
#define __GAME_SCENE_COMMON_H__

#include "macros.hpp"

// 场景运行状态
enum SceneState : uint32_t
{
	SCENE_STATE_NONE = 0,
	SCENE_STATE_NORMAL,//正常运行
	SCENE_STATE_UNLOADING,//正在卸载
	SCENE_STATE_UNLOADED,//已经卸载
};

// 场景活动状态枚举
enum SceneActiveState : uint32_t
{
	None_State = 0, // 默认状态
	Run_State = 1,	// 场景活动进行中
	Stop_State = 2,	// 场景活动结束处理中
	End_State = 3,	// 场景活动已结束处理
};

// 场景基类

class scene_base
{
private:
	uint32_t m_state;

public:
	scene_base() : m_state(SCENE_STATE_NONE) {};
	virtual ~scene_base() {};

	uint32_t get_scene_state() const { return m_state; }
	void set_scene_state(uint32_t set) { m_state = set; }
};

#endif
