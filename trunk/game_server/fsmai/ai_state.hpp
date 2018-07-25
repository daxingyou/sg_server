#ifndef __GAME_AI_STATE__
#define __GAME_AI_STATE__

#include "ai_action.hpp"
#include "ai_event.hpp"
#include "ai_mode.hpp"

#include <map>

class ai_state_t;
typedef boost::shared_ptr<ai_state_t> ai_state_ptr;

enum state_type_enum
{
	state_type_none = 0,	// 无效 
	state_type_root,		// 根状态
	state_type_child,		// 子状态
};

class ai_state_t
{
public:
	ai_state_t() {};

	bool init(state_type_enum type, const fsm::ai_state& node, const monster_common::monster_define_t& define, Monster* monster);
	void reset();

public:
	void change_state(const monster_ptr& p_monster, proto::common::object_state_type newstate);
	void on_enetr(const monster_ptr& p_monster, proto::common::object_state_type old = proto::common::object_state_none);
	void on_leave();
	void on_action(uint64_t msecs, const monster_ptr& p_monster);
	bool on_event(const monster_ptr& p_monster, ai::ai_event event);

public:
	inline proto::common::object_state_type get_state() const { return m_state; }
	ai_event_ptr get_ai_event(ai::ai_event event_id);
	ai_state_ptr get_ai_state(proto::common::object_state_type state);

protected:
	typedef std::map<ai::ai_event, ai_event_ptr> ai_event_map;
	ai_event_map m_ai_event_map;

	typedef std::vector<ai_mode_ptr> ai_mode_vec;
	ai_mode_vec m_ai_mode_vec;

	typedef std::map<proto::common::object_state_type, ai_state_ptr> ai_state_map;
	ai_state_map m_ai_state_map;

protected:
	state_type_enum m_state_type = state_type_none;
	proto::common::object_state_type m_state = proto::common::object_state_none;		// 对于根节点是当前什么状态，子节点表示状态名字
	ai_state_ptr m_curr_state = NULL;				// 当前状态的处理		
};

#endif
