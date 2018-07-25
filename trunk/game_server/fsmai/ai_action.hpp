#ifndef __GAME_AI_ACTION__
#define __GAME_AI_ACTION__

#include "fsm_ai_define.hpp"
#include <boost/shared_ptr.hpp>
#include <vector>

class monster_t;
typedef boost::shared_ptr<monster_t> monster_ptr;

/**
 * \brief ai ִ�нڵ�
 */

class ai_action_t;
typedef boost::shared_ptr<ai_action_t> ai_action_ptr;
typedef std::vector<ai_action_ptr> ai_action_vec;

class ai_action_t
{
public:
	static ai_action_ptr create_ai_action(ai::ai_act type);

public:
	ai_action_t() {};
	virtual ~ai_action_t() {};

	virtual bool init(const fsm::ai_act& node);
	virtual bool do_act(const monster_ptr& p_monster) = 0;

protected:
	ai::ai_act m_type = ai::ai_act_none;
};

/**
 * \brief �����¼�
 */
class act_death : public ai_action_t
{
public:
	virtual bool do_act(const monster_ptr& p_monster);
};

/**
 * \brief �ӳ�����ɾ��
 */
class act_leave_scene : public ai_action_t
{
public:
	virtual bool do_act(const monster_ptr& p_monster);
};

/**
 * \brief �ӹ�����ɾ��
 */
class act_delete_obj : public ai_action_t
{
public:
	virtual bool do_act(const monster_ptr& p_monster);
};

/**
* \brief ����
*/
class act_revive : public ai_action_t
{
public:
	virtual bool do_act(const monster_ptr& p_monster);
};

/**
* \brief ���뾯��
*/
class act_start_alert : public ai_action_t
{
public:
	virtual bool do_act(const monster_ptr& p_monster);
};

/**
* \brief ����ս��
*/
class act_start_fight : public ai_action_t
{
public:
	virtual bool do_act(const monster_ptr& p_monster);
};

/**
 * \brief ״̬�л�
 */
class act_change_state : public ai_action_t
{
public:
	virtual bool init(const fsm::ai_act& node);
	virtual bool do_act(const monster_ptr& p_monster);

protected:
	proto::common::object_state_type m_new_state = proto::common::object_state_none;
};

#endif
