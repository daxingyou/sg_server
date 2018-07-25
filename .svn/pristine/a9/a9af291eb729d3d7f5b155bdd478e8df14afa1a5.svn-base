#ifndef __TACTIC_H__
#define __TACTIC_H__
#include "macros.hpp"
#include "protos_fwd.hpp"

class tactic_t
{
public:
	tactic_t(){}
	tactic_t(uint32_t tactic_id);

	void set_tid(uint32_t val) { m_tid = val; }
	uint32_t get_tid() { return m_tid; }
	void set_level(uint32_t val) { m_level = val; }
	uint32_t get_level() { return m_level; }
	
public:
	void load_data(const proto::common::tactic_single& tactic_single);
	void peek_data(proto::common::tactic_single* p_tactic_single);
private:
	uint32_t		m_tid = 0;
	uint32_t		m_level = 0;
};
typedef boost::shared_ptr<tactic_t> tactic_ptr;

#endif // !__BOUNTY_H__

