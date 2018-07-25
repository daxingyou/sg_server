#ifndef __GAME_DUNGEON_MANAGER_H__
#define __GAME_DUNGEON_MANAGER_H__

#include "dungeon_scene.hpp"
#include <boost/asio/deadline_timer.hpp>
#include <boost/system/error_code.hpp>

#include <set>
#include <vector>

class dungeon_manager_t
{
public:
	static bool init();
	static void close();

public:
	static void update(const boost::system::error_code& ec);

	static void add_dungeon_id(uint64_t id);

public:
	static void open_dungeon(role_ptr p_role, uint32_t dungeon_id);
	static void open_dungeon_by_team(uint64_t leader_id, uint64_t team_id, uint32_t dungeon_id);
	static bool check_dungeon(role_ptr p_role, uint32_t dungeon_id);

	static void create_dungeon(const proto::server::dungeon_create_data& data, proto::server::ge_create_dugeon_reply& reply);
	static family_war_scene_ptr create_family_war_scene(uint32_t map_tid, const proto::server::family_war_dungeon_data& data);

	static bool check_fight_again(const role_ptr& p_role);

	static bool check_fight_times(role_ptr p_role, uint32_t dungeon_id);
	static uint32_t buy_times(role_ptr p_role, uint32_t dungeon_type, proto::common::role_dungeon_times_data* p_data, proto::common::role_change_data* rcd);
	static void ask_dungeon_times(const role_ptr& p_role);
protected:
	typedef std::set<uint64_t> dungeon_set;
	static const uint32_t DUNGEON_MAX_GROUP = 4;
	static const uint32_t DUNGEON_PRE_TIME = 250;

	static std::vector<dungeon_set> m_dungeon_group;
	static uint32_t m_group_add;
	static uint32_t m_group_step;
	static boost::shared_ptr<boost::asio::deadline_timer> m_group_timer;	//  副本定时器
	static uint32_t m_dungeon_size;
};

#endif
