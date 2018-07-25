#include "map_area.hpp"
#include "common/log.hpp"
#include "role/role.hpp"
#include "task/task_manager.hpp"

map_area_t::~map_area_t()
{

}

void map_area_t::add_dark_item(uint32_t tid, uint32_t weight, uint32_t task_id)
{
	if (task_id)
	{
		m_task_dark_vec.push_back(std::make_pair(task_id, tid));
	}
	else
	{
		m_weights += weight;

		m_normal_dark_vec.push_back(std::make_pair(tid, m_weights));
	}
}

std::pair<uint32_t, uint32_t> map_area_t::has_task_dark(const role_ptr& p_role)
{
	if (NULL == p_role)
	{
		log_error("p_role null error");
		return std::pair<uint32_t, uint32_t>(0, 0);
	}

	if (m_task_dark_vec.empty())
	{
		return std::pair<uint32_t, uint32_t>(0, 0);
	}

	task_mgr_ptr p_task_mgr = p_role->get_task_mgr();
	if (NULL == p_task_mgr)
	{
		log_error("p_task_mgr null error");
		return std::pair<uint32_t, uint32_t>(0, 0);
	}

	for (const auto& pair : m_task_dark_vec)
	{
		if (p_task_mgr->is_doing2(pair.first))
		{
			return pair;
		}
	}

	return std::pair<uint32_t, uint32_t>(0, 0);
}

uint32_t map_area_t::random_dark_monster()
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
