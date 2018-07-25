#ifndef __GAME_MONSTER_AI_H__
#define __GAME_MONSTER_AI_H__

#include "macros.hpp"

#include <boost/shared_ptr.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include "fsmai/ai_state.hpp"
#include "tblh/Monster.tbls.h"
#include "object/object_position.hpp"

// 怪物的一些标志定义
enum monster_flag
{
	MF_ACTIVE_MODE		= 0x00000001,			// 主动怪物标志位					---1
	MF_EXCLUSIVE_MODE	= 0x00000002,			// 有玩家进入战斗不能在和它战斗		---2
};

class monster_t;
typedef boost::shared_ptr<monster_t> monster_ptr;

class ai_controller_t
{
public:
	typedef boost::weak_ptr<monster_t> monster_wptr;

public:
	ai_controller_t(const monster_ptr& p_monster, uint32_t id);

public:

	// 加载配置 flag为一些标记
	bool load_ai(const monster_common::monster_define_t& define, Monster* monster);

	void reset();

	// 发生了某个事件
	bool on_event(ai::ai_event evt);

	void change_state(proto::common::object_state_type newstate);

	// 定时器执行
	void on_action(uint64_t msecs);

protected:
	inline monster_ptr get_monster_ptr() { return m_monster_wptr.lock(); }

protected:
	uint32_t m_ai_id = 0;			// ai
	monster_wptr m_monster_wptr;	// 控制的怪物

protected:
	boost::scoped_ptr<ai_state_t> m_rootstate;
};

#endif
