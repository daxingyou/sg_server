#ifndef __GAME_AI_EVENT__
#define __GAME_AI_EVENT__

#include "ai_action.hpp"

/**
 * \brief ÊÂ¼þ
 */

class ai_event_t;
typedef boost::shared_ptr<ai_event_t> ai_event_ptr;

class ai_event_t
{
public:
	ai_event_t() {};
	virtual ~ai_event_t() {};

	bool init(const fsm::ai_event& node);
	bool do_evnet(const monster_ptr& p_monster);

protected:
	ai::ai_event m_type = ai::ai_event_none;
	ai_action_vec m_ai_action_vec;
};

#endif
