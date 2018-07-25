#pragma once

#include <vector>
#include "macros.hpp"
#include <boost/shared_ptr.hpp>

class role_t;
typedef boost::shared_ptr<role_t> role_ptr;

class map_patrol_t
{
public:
	map_patrol_t() {};
	~map_patrol_t();

public:
	void add_dark_item(uint32_t tid, uint32_t weight);

	uint32_t get_trigger_min() { return m_trigger_min; }
	void set_trigger_min(uint32_t min) { m_trigger_min = min; }

	uint32_t get_trigger_max() { return m_trigger_max; }
	void set_trigger_max(uint32_t max) { m_trigger_max = max; }

	// 随机一个暗雷怪
	uint32_t random_dark_monster();

protected:
	// 暗雷怪对战id
	std::vector<std::pair<uint32_t, uint32_t>> m_normal_dark_vec;
	uint32_t m_weights = 0;
	// 触发频率
	uint32_t m_trigger_min = 0;
	uint32_t m_trigger_max = 0;
};
