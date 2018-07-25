#ifndef __GAME_FAMILY_SKILL_H__
#define __GAME_FAMILY_SKILL_H__

#include "protos_fwd.hpp"
#include "macros.hpp"

class family_skill_t
{
public:
	family_skill_t();
    ~family_skill_t();
public:
	void load_data(const proto::common::family_skill_single& ls);
	void peek_data(proto::common::family_skill_single* p_data);

public:
	void set_tid(uint32_t val) { m_tid = val; }
	uint32_t get_tid() { return m_tid; }
	void set_level(uint32_t val) { m_level = val; }
	uint32_t get_level() { return m_level; }
private:
	uint32_t m_tid = 0;
	uint32_t m_level = 0;
};
typedef boost::shared_ptr<family_skill_t> family_skill_ptr;

#endif