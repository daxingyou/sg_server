#ifndef __GAME_MONSTER_MANAGER_H__
#define __GAME_MONSTER_MANAGER_H__

#include <unordered_map>
#include <set>
#include <vector>
#include "monster.hpp"
#include "global_id.hpp"
#include <boost/asio/deadline_timer.hpp>
#include <boost/system/error_code.hpp>

// ����������
class monster_manager_t 
{
public:
	// ��ʼ��id������
	static bool init();
	static void close_clear();
	// ���ҹ���
	static monster_ptr find_monster(uint64_t monster_id);
	// ɾ������
	static void delete_monster(uint64_t monster_id);
	// �������
	static bool add_monster(monster_ptr p_monster);

	static inline uint64_t get_monster_uid();

	static void add_special_monster(uint64_t monster_id);

public:
	typedef std::set<uint64_t> special_monsters;
	typedef std::unordered_map<uint64_t, monster_ptr> monster_map;

	static const uint32_t SPECIAL_MAX_GROUP = 4;
	// ��ʱû���ƶ���ai������ʱ����1s
	static const uint32_t SPECIAL_PRE_TIME = 250;

	static void AI(const boost::system::error_code& ec);

public:
	static monster_map m_monsters;
	static std::vector<special_monsters> m_special_monsters;

private:
	static uint32_t m_special_add;
	static uint32_t m_special_step;
	static boost::shared_ptr<boost::asio::deadline_timer> m_special_timer;	// ai��ʱ��
};


inline uint64_t monster_manager_t::get_monster_uid()
{
	return common::g_id_allocator.gen_new_id(common::ID_ALLOCATOR_TYPE_MONSTER);
}


#endif // !__GAME_MONSTER_MANAGER_H__
