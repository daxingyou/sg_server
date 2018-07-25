#pragma once

#include "scene/scene.hpp"
#include <boost/weak_ptr.hpp>

class scene_state_controler;

// 场景通用状态控制
struct scene_state_t
{
	virtual ~scene_state_t() {}

	// 状态
	virtual SceneActiveState get_state() const = 0;
	// 进入这个状态
	virtual void on_enter(scene_state_controler& controler) {};
	// 状态动作处理
	virtual void handle(scene_state_controler& controler, uint32_t curr_time) = 0;
};

typedef boost::shared_ptr<scene_state_t> scene_state_ptr;

typedef std::map<SceneActiveState, scene_state_ptr> scene_state_map;

// 状态机控制器
class scene_state_controler
{
public:
	static scene_state_ptr create_scene_state(SceneActiveState type);
public:
	// 初始化
	virtual bool init(const scene_ptr& p_scene, SceneActiveState init_state);

	// 执行状态操作
	void action(uint32_t curr_time);

	// 状态转换
	void change_state(SceneActiveState new_state);

	// 当前状态
	bool is_running() const
	{
		if (NULL == m_curr_state)
			return false;

		return m_curr_state->get_state() == SceneActiveState::Run_State;
	}

public:
	typedef boost::weak_ptr<scene_t> scene_wptr;
	// 得到场景指针
	scene_ptr get_scene() const { return m_scene_wptr.lock(); }

private:
	scene_wptr m_scene_wptr;

	scene_state_ptr m_curr_state = NULL;
	scene_state_map m_scene_state_map;
};

struct scene_state_none_t : public scene_state_t
{
	// 状态
	virtual SceneActiveState get_state() const { return SceneActiveState::None_State; }
	//状态动作处理
	virtual void handle(scene_state_controler& controler, uint32_t curr_time) {}
};

struct scene_state_run_t : public scene_state_t
{
	// 状态
	virtual SceneActiveState get_state() const { return SceneActiveState::Run_State; }
	//状态动作处理
	virtual void handle(scene_state_controler& controler, uint32_t curr_time);
};

struct scene_state_stop_t : public scene_state_t
{
	// 状态
	virtual SceneActiveState get_state() const { return SceneActiveState::Stop_State; }
	// 进入这个状态
	virtual void on_enter(scene_state_controler& controler);
	//状态动作处理
	virtual void handle(scene_state_controler& controler, uint32_t curr_time);

	uint32_t m_left_time = 0;	// 等待离开的时间
};

struct scene_state_end_t : public scene_state_t
{
	// 状态
	virtual SceneActiveState get_state() const { return SceneActiveState::End_State; }
	//状态动作处理
	virtual void handle(scene_state_controler& controler, uint32_t curr_time);
};