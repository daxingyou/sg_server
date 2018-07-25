#include "map_patrol.hpp"
#include "log.hpp"
#include "random_util.hpp"

map_patrol_t::~map_patrol_t()
{

}

void map_patrol_t::add_dark_item(uint32_t tid, uint32_t weight)
{
	m_weights += weight;

	m_normal_dark_vec.push_back(std::make_pair(tid, m_weights));
}

uint32_t map_patrol_t::random_dark_monster()
{
	if (m_weights == 0)
	{
		return 0;
	}

	if (m_normal_dark_vec.size() == 1)
	{
		return m_normal_dark_vec[0].first;
	}

	int32_t rate = random_util_t::randBetween(0, m_weights - 1);
    if (rate < 0)
    {
        return 0;
    }

	for (const auto& pair : m_normal_dark_vec)
	{
		if (pair.second > (uint32_t)rate)
		{
			return pair.first;
		}
	}

	return 0;
}
