#ifndef __GAME_LIANDAN_H__
#define __GAME_LIANDAN_H__

#include "protos_fwd.hpp"
#include "macros.hpp"

class liandan_t
{
public:
	liandan_t();
    ~liandan_t();
public:
	void load_data(const proto::common::liandan_single& ls);
	void peek_data(proto::common::liandan_single* p_data);

	//检测炼丹时间是否到期
	void check_doing();
public:
	void set_state(uint32_t val) { m_state = val; }
	uint32_t get_state() { return m_state; }
	void set_free_time(uint32_t val) { m_free_time = val; }
	uint32_t get_free_time() { return m_free_time; }
	void set_liandan_tid(uint32_t val) { m_liandan_tid = val; }
	uint32_t get_liandan_tid() { return m_liandan_tid; }
	void set_count(uint32_t val) { m_count = val; }
	uint32_t get_count() { return m_count; }
private:
	uint32_t m_state = 0;
	uint32_t m_free_time = 0;
	uint32_t m_liandan_tid = 0;
	uint32_t m_count = 0;
};
typedef boost::shared_ptr<liandan_t> liandan_ptr;

#endif