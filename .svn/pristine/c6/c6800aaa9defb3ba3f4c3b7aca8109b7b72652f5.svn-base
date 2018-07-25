#ifndef _TALENT_H_
#define _TALENT_H_
#include <map>
#include <boost/shared_ptr.hpp>

// 武将天赋系统
class talent_t
{
public:
	talent_t(uint32_t tid, uint32_t level);
	~talent_t();
public:
	uint32_t get_tid() { return m_tid; }
	uint32_t get_level() { return m_level; }

	void set_level(uint32_t level) { m_level = level; }
private:
	uint32_t m_tid;
	uint32_t m_level;
};
typedef boost::shared_ptr<talent_t> talent_ptr;
typedef std::map<uint32_t, talent_ptr> talent_map;
#endif // !_HERO_TALENT_H_