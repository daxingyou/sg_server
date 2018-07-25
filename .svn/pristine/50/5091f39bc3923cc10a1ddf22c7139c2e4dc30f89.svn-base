#ifndef __GAME_MONSTER_MANAGER_H__
#define __GAME_MONSTER_MANAGER_H__

#include <unordered_map>
#include <set>
#include <vector>
#include "monster.hpp"
#include "global_id.hpp"
#include <boost/asio/deadline_timer.hpp>
#include <boost/system/error_code.hpp>

// 怪物管理对象
class monster_manager_t 
{
public:
	// 初始化id生成器
	static bool init();
	static void close_clear();
	// 查找怪物
	static monster_ptr find_monster(uint64_t monster_id);
	// 删除生物
	static void delete_monster(uint64_t monster_id);
	// 添加生物
	static bool add_monster(monster_ptr p_monster);

	static inline uint64_t get_monster_uid();

	static void add_special_monster(uint64_t monster_id);

public:
	typedef std::set<uint64_t> special_monsters;
	typedef std::unordered_map<uint64_t, monster_ptr> monster_map;

	static const uint32_t SPECIAL_MAX_GROUP = 4;
	// 暂时没有移动的ai，触发时间是1s
	static const uint32_t SPECIAL_PRE_TIME = 250;

	static void AI(const boost::system::error_code& ec);

public:
	static monster_map m_monsters;
	static std::vector<special_monsters> m_special_monsters;

private:
	static uint32_t m_special_add;
	static uint32_t m_special_step;
	static boost::shared_ptr<boost::asio::deadline_timer> m_special_timer;	// ai定时器
};


inline uint64_t monster_manager_t::get_monster_uid()
{
	return common::g_id_allocator.gen_new_id(common::ID_ALLOCATOR_TYPE_MONSTER);
}


#endif // !__GAME_MONSTER_MANAGER_H__
