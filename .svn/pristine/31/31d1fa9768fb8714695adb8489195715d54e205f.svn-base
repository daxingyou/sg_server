#ifndef _GAME_DUNGEON_SCENE_STATE_
#define _GAME_DUNGEON_SCENE_STATE_

#include "scene/scene.hpp"
#include <boost/weak_ptr.hpp>

class dungeon_state_controler;

// 副本状态机
struct dungeon_state_t
{
	virtual ~dungeon_state_t() {}

	// 状态
	virtual proto::common::dungeon_state_type get_state() const = 0;
	// 进入这个状态
	virtual void on_enter(dungeon_state_controler& controler) {};
	//状态动作处理
	virtual void handle(dungeon_state_controler& controler, uint32_t curr_time) = 0;
};

typedef boost::shared_ptr<dungeon_state_t> dungeon_state_ptr;

typedef std::map<proto::common::dungeon_state_type, dungeon_state_ptr> dungeon_state_map;

// 状态机控制器
class dungeon_state_controler
{
public:
	// 初始化
	bool init(const dungeon_scene_ptr& p_scene, proto::common::dungeon_state_type init_state);

	// 执行状态操作
	void action(uint32_t curr_time);

	// 状态转换
	void change_state(proto::common::dungeon_state_type new_state);

	// 当前状态
	proto::common::dungeon_state_type get_state_type() const
	{
		if (NULL == m_curr_state)
			return proto::common::dungeon_state_none;

		return m_curr_state->get_state();
	}

public:
	typedef boost::weak_ptr<dungeon_scene_t> dungeon_scene_wptr;
	// 得到场景指针
	dungeon_scene_ptr get_scene() const { return m_dungeon_scene.lock(); }

private:
	dungeon_scene_wptr m_dungeon_scene;

	dungeon_state_ptr m_curr_state = NULL;
	dungeon_state_map m_dungeon_state_map;
};

struct dungeon_state_none_t : public dungeon_state_t
{
	// 状态
	virtual proto::common::dungeon_state_type get_state() const { return proto::common::dungeon_state_none; }
	//状态动作处理
	virtual void handle(dungeon_state_controler& controler, uint32_t curr_time) {}
};

struct dungeon_state_wait_t : public dungeon_state_t
{
	// 状态
	virtual proto::common::dungeon_state_type get_state() const { return proto::common::dungeon_state_wait; }
	// 进入这个状态
	virtual void on_enter(dungeon_state_controler& controler);
	//状态动作处理
	virtual void handle(dungeon_state_controler& controler, uint32_t curr_time);

	uint32_t m_idle_time = 0;		// 无人空闲时间
};

struct dungeon_state_start_t : public dungeon_state_t
{
	// 状态
	virtual proto::common::dungeon_state_type get_state() const { return proto::common::dungeon_state_start; }
	//状态动作处理
	virtual void handle(dungeon_state_controler& controler, uint32_t curr_time);
};

struct dungeon_state_stop_t : public dungeon_state_t
{
	// 状态
	virtual proto::common::dungeon_state_type get_state() const { return proto::common::dungeon_state_stop; }
	// 进入这个状态
	virtual void on_enter(dungeon_state_controler& controler);
	//状态动作处理
	virtual void handle(dungeon_state_controler& controler, uint32_t curr_time);

	uint32_t m_left_time = 0;	// 等待离开的时间
};

#endif
