#ifndef __BOUNTY_H__
#define __BOUNTY_H__
#include "macros.hpp"
#include "protos_fwd.hpp"

enum bounty_state
{
	bounty_state_not_accept				= 0,	//未接取
	bounty_state_doing					= 1,	//进行中
	bounty_state_finish					= 2,	//已完成
	bounty_state_drop					= 3,	//已放弃
};

class bounty_t
{
public:
	bounty_t(){}
	bounty_t(uint32_t bounty_id);

	void set_tid(uint32_t val) { m_tid = val; }
	uint32_t get_tid() { return m_tid; }
	void set_state(uint32_t val) { m_state = val; }
	uint32_t get_state() { return m_state; }
	void set_star(uint32_t val) { m_star = val; }
	uint32_t get_star() { return m_star; }
	void add_star() { ++m_star; }

	bool is_doing() { return m_state == bounty_state_doing; }
	bool is_not_accepted() { return m_state == bounty_state_not_accept; }
	bool is_finished() { return m_state == bounty_state_finish; }
	bool is_droped() { return m_state == bounty_state_drop; }

	void drop() { m_state = 0; }
public:
	void load_data(const proto::common::bounty_single& bounty_single);
	void peek_data(proto::common::bounty_single* p_bounty_single);
private:
	uint32_t		m_tid = 0;
	uint32_t		m_state = 0;
	uint32_t		m_star = 0;
};
typedef boost::shared_ptr<bounty_t> bounty_ptr;

#endif // !__BOUNTY_H__

