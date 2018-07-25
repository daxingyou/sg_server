#include "arena_data_manager.hpp"
#include "common/redis_client.hpp"
#include "common/config_mgr.h"

arena_data_manager::arena_data_manager()
{
}

arena_data_manager::~arena_data_manager()
{
}

bool arena_data_manager::save_arena_data_to_redis(uint64_t uid, const proto::common::arena_role_data& arena_data)
{
	redis_client_t::set_uint32("arena_role_data", "win_times", uid, arena_data.win_times());
	redis_client_t::set_uint32("arena_role_data", "lose_times", uid, arena_data.lose_times());
	redis_client_t::set_uint32("arena_role_data", "is_start", uid, arena_data.is_start());
	redis_client_t::set_uint32("arena_role_data", "score", uid, arena_data.score());
	redis_client_t::set_uint32("arena_role_data", "arena_level", uid, arena_data.arena_level());
	redis_client_t::set_uint32("arena_role_data", "all_fight_times", uid, arena_data.all_fight_times());
	redis_client_t::set_uint32("arena_role_data", "all_win_times", uid, arena_data.all_win_times());
	redis_client_t::set_uint32("arena_role_data", "best_win_times", uid, arena_data.best_win_times());
	redis_client_t::set_uint32("arena_role_data", "last_reset_time", uid, arena_data.last_reset_time());
	return true;
}

bool arena_data_manager::load_arena_data_from_redis(uint64_t uid, proto::common::arena_role_data* arena_data)
{
	if (arena_data == NULL)
	{
		log_error("load_arena_data_from_redis [%lu] data == null", uid);
		return false;
	}
	
	arena_data->set_win_times(redis_client_t::get_uint32("arena_role_data", "win_times", uid));
	arena_data->set_lose_times(redis_client_t::get_uint32("arena_role_data", "lose_times", uid));
	arena_data->set_is_start(redis_client_t::get_uint32("arena_role_data", "is_start", uid));
	arena_data->set_score(redis_client_t::get_uint32("arena_role_data", "score", uid));
	arena_data->set_arena_level(redis_client_t::get_uint32("arena_role_data", "arena_level", uid));
	arena_data->set_all_fight_times(redis_client_t::get_uint32("arena_role_data", "all_fight_times", uid));
	arena_data->set_all_win_times(redis_client_t::get_uint32("arena_role_data", "all_win_times", uid));
	arena_data->set_best_win_times(redis_client_t::get_uint32("arena_role_data", "best_win_times", uid));
	arena_data->set_last_reset_time(redis_client_t::get_uint32("arena_role_data", "last_reset_time", uid));
	return true;
}
