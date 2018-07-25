#ifndef __GAME_MODE_EVENT__
#define __GAME_MODE_EVENT__

#include "ai_action.hpp"
#include "tblh/Monster.tbls.h"
#include "monster/monster_define.hpp"

/**
* \brief ģ��
*/

class ai_mode_t;
typedef boost::shared_ptr<ai_mode_t> ai_mode_ptr;

class ai_mode_t
{
public:
	static ai_mode_ptr create_ai_mode(ai::ai_mode type);

public:
	ai_mode_t() {};
	virtual ~ai_mode_t() {};

	virtual bool init(const fsm::ai_mode& node, const monster_common::monster_define_t& define, Monster* monster);
	void reset() { m_valid = true; }
	inline bool is_valid() { return m_valid; }

	// �������ģ��
	virtual void start_up(const monster_ptr& p_monster) {};
	// ����
	virtual void do_mode(uint64_t msecs, const monster_ptr& p_monster) = 0;
	// do act
	void exe_action(const monster_ptr& p_monster);

protected:
	ai::ai_mode m_type = ai::ai_mode_none;
	ai_action_vec m_ai_action_vec;
	bool m_valid = true;
};

/**
 * \brief ��ʱʬ����ʧ
 */
class mode_time_disappear : public ai_mode_t
{
public:
	virtual bool init(const fsm::ai_mode& node, const monster_common::monster_define_t& define, Monster* monster);
	virtual void start_up(const monster_ptr& p_monster);
	virtual void do_mode(uint64_t msecs, const monster_ptr& p_monster);

protected:
	uint32_t m_disappear_time = 0;		// ��ʧʱ��
};

/**
* \brief ��ʱ����
*/
class mode_time_revive : public ai_mode_t
{
public:
	virtual bool init(const fsm::ai_mode& node, const monster_common::monster_define_t& define, Monster* monster);
	virtual void start_up(const monster_ptr& p_monster);
	virtual void do_mode(uint64_t msecs, const monster_ptr& p_monster);

protected:
	// �������
	uint32_t m_revive_interval = 0;	// ����ʱ����
};

class mode_trigger_alert : public ai_mode_t
{
public:
	virtual bool init(const fsm::ai_mode& node, const monster_common::monster_define_t& define, Monster* monster);
	virtual void start_up(const monster_ptr& p_monster);
	virtual void do_mode(uint64_t msecs, const monster_ptr& p_monster);

protected:
	// ���䷶Χ
	uint32_t m_alert_radius = 0;
};

/**
 * \brief ����
 */
class mode_time_alert : public ai_mode_t
{
public:
	virtual bool init(const fsm::ai_mode& node, const monster_common::monster_define_t& define, Monster* monster);
	virtual void start_up(const monster_ptr& p_monster);
	virtual void do_mode(uint64_t msecs, const monster_ptr& p_monster);

protected:
	// ���䷶Χ
	uint32_t m_alert_radius = 0;
	// ����ʱ��
	uint32_t m_alert_interval = 0;
	// ���ʱ��
	uint32_t m_alert_end_time = 0;
};

#endif