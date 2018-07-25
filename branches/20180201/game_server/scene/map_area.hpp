#ifndef __GAME_MAP_AREA_H__
#define __GAME_MAP_AREA_H__

#include <vector>
#include "macros.hpp"
#include <boost/shared_ptr.hpp>

class role_t;
typedef boost::shared_ptr<role_t> role_ptr;

class map_area_t
{
public:
	map_area_t(uint32_t id) : m_id(id) {};
	~map_area_t();

public:
	void add_dark_item(uint32_t tid, uint32_t weight, uint32_t task_id);

	uint32_t get_trigger_min() { return m_trigger_min; }
	void set_trigger_min(uint32_t min) { m_trigger_min = min; }

	uint32_t get_trigger_max() { return m_trigger_max; }
	void set_trigger_max(uint32_t max) { m_trigger_max = max; }

	std::pair<uint32_t, uint32_t> has_task_dark(const role_ptr& p_role);

	bool is_have_dark() const { return !m_normal_dark_vec.empty(); }

	// 随机一个暗雷怪
	uint32_t random_dark_monster();

protected:
	// 区域id
	uint32_t m_id;
	// 暗雷怪对战id
	std::vector<std::pair<uint32_t, uint32_t>> m_normal_dark_vec;
	uint32_t m_weights = 0;
	// 任务(没有权重)
	std::vector<std::pair<uint32_t, uint32_t>> m_task_dark_vec;
	// 触发频率
	uint32_t m_trigger_min = 0;
	uint32_t m_trigger_max = 0;
};

#endif
