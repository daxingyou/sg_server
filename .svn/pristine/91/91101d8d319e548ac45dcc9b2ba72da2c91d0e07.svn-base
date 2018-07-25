#include "monster_manager.hpp"
#include "config/config_manager.hpp"
#include "main/game_server.hpp"
#include "main/game_server_fwd.hpp"
#include "func_time.hpp"

monster_manager_t::monster_map monster_manager_t::m_monsters;

std::vector<monster_manager_t::special_monsters> monster_manager_t::m_special_monsters;

uint32_t monster_manager_t::m_special_add = 0;

uint32_t monster_manager_t::m_special_step = 0;

boost::shared_ptr<boost::asio::deadline_timer> monster_manager_t::m_special_timer = NULL;

uint32_t monster_manager_t::m_special_size = 0;

bool monster_manager_t::init()
{
	if (SPECIAL_MAX_GROUP == 0)
	{
		log_error("SPECIAL_MAX_GROUP size 0 error");
		return false;
	}

	m_special_monsters.resize(SPECIAL_MAX_GROUP);

	// 定时器初始化
	if (NULL == env::server)
	{
		log_error("env::server null error");
		return false;
	}

	m_special_timer.reset(new boost::asio::deadline_timer(env::server->get_ios()));
	if (NULL == m_special_timer)
	{
		log_error("m_special_timer null error");
		return false;
	}

	// 没有AI就不调用
	// 每50毫秒执行一次 一个轮回刚好是200毫秒
	// m_special_timer->expires_from_now(boost::posix_time::millisec(SPECIAL_PRE_TIME));
	// m_special_timer->async_wait(boost::bind(&monster_manager_t::AI, boost::asio::placeholders::error));

	return true;
}

void monster_manager_t::close_clear()
{
	m_special_monsters.clear();
	m_monsters.clear();

	// 定时器清除
	if (NULL != m_special_timer)
	{
		m_special_timer->cancel();
	}
}

// 查找怪物
monster_ptr monster_manager_t::find_monster(uint64_t monster_id)
{
	monster_map::iterator itr = m_monsters.find(monster_id);
	if (itr != m_monsters.end())
	{
		return itr->second;
	}
	return NULL;
}

// 删除生物
void monster_manager_t::delete_monster(uint64_t creature_id)
{
	monster_map::iterator itr = m_monsters.find(creature_id);
	if (itr != m_monsters.end())
	{
		// 有定时器需要在删除前cancel掉
		monster_ptr p_monster = itr->second;
		if (NULL != p_monster)
		{
			p_monster->del_clear();
		}

        m_monsters.erase(itr);
	}
}

// 添加生物
bool monster_manager_t::add_monster(monster_ptr p_monster)
{
	if (p_monster == NULL)
	{
		log_error("monster null error");
		return false;
	}

	std::pair<monster_map::iterator, bool> ret = m_monsters.insert(std::make_pair(p_monster->get_object_id(), p_monster));
	if (!ret.second)
	{
		log_error("monster[%lu] insert error, it is exist", p_monster->get_object_id());
		return false;
	}

	return true;
}

void monster_manager_t::add_special_monster(uint64_t monster_id)
{
	m_special_monsters[m_special_add].insert(monster_id);

	m_special_add++;
	m_special_add = m_special_add % SPECIAL_MAX_GROUP;

	m_special_size++;
	if (m_special_size == 1) { // 当有一个副本时候说明从无到有，启动定时器
		// 每50毫秒执行一次 一个轮回刚好是200毫秒
		m_special_timer->expires_from_now(boost::posix_time::millisec(SPECIAL_PRE_TIME));
		m_special_timer->async_wait(boost::bind(&monster_manager_t::AI, boost::asio::placeholders::error));
	}
	
}

void monster_manager_t::AI(const boost::system::error_code& ec)
{
	if (ec == boost::asio::error::operation_aborted)
	{
		
		return;
	}
	else if (ec)
	{
		log_error("Timer error: %s", ec.message().c_str());
		return;
	}

	//func_timer times(2, __PRETTY_FUNCTION__);

	if (m_special_timer != NULL)
	{
		m_special_timer->expires_from_now(boost::posix_time::millisec(SPECIAL_PRE_TIME));
		m_special_timer->async_wait(boost::bind(&monster_manager_t::AI, boost::asio::placeholders::error));
	}

 	uint64_t curr_msecs = common::time_util_t::now_msecs();
// 	uint32_t all_size = 0;

	for (special_monsters::iterator itr = m_special_monsters[m_special_step].begin();
		itr != m_special_monsters[m_special_step].end();)
	{
		monster_ptr p_monster = find_monster(*itr);
		if (p_monster == NULL || p_monster->is_need_clear())
		{
			if (p_monster != NULL)
			{
				// 还在场景中先从场景中移除
				if (p_monster->is_in_scene()) p_monster->leave_scene();

				delete_monster(*itr);
			}

			m_special_monsters[m_special_step].erase(itr++);

			if (m_special_size > 0) m_special_size--;
		}
		else
		{
			p_monster->on_tick(curr_msecs);

			++itr;
		}

//		all_size++;
	}

// 	uint64_t next_msecs = common::time_util_t::now_msecs();
// 
// 	if (next_msecs > curr_msecs + 5)
// 	{
// 		printf("\033[31m step:%u monster:%u time cost:%lu \033[0m\n", m_special_step, all_size, next_msecs - curr_msecs);
// 	}

	if (m_special_size > 0) {
		m_special_step++;
		m_special_step = m_special_step % SPECIAL_MAX_GROUP;
	} else {
		m_special_step = 0;

		// 定时器清除
		if (NULL != m_special_timer) {
			m_special_timer->cancel();
		}
	}
	
}
