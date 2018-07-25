#include "arena_score_rank_mgr.hpp"
#include "utility.hpp"
#include "redis_client.hpp"
#include "common/zdb_ex.hpp"
#include "main/area_server.hpp"
#include "main/area_server_fwd.hpp"
#include "config/config_manager.hpp"
#include "tblh/Comprehensive.tbls.h"
#include "tblh/ArenaClassTable.tbls.h"
#include "time_manager.hpp"

arena_rank_t::arena_rank_t(common::rank_common::rank_type type, const char* name)
	: rank_t(type, name)
{

}

bool arena_rank_t::load_from_redis(uint32_t time_id)
{
	if (!rank_t::init()) {
		return false;
	}

	uint32_t now_time = time_util_t::now_time();
	std::vector<uint64_t> vecs;
	redis_client_t::get_all_keys<uint64_t, std::vector<uint64_t>>(m_rank_name.c_str(), vecs);

	std::vector<std::pair<uint64_t, uint32_t>> batch_set;
	for (std::vector<uint64_t>::const_iterator citr = vecs.begin();
		citr != vecs.end(); ++citr) {
		uint32_t score = redis_client_t::get_uint32(m_rank_name.c_str(), "score", *citr);
		if (score < m_min_condi) {
			if (redis_client_t::is_hash_exist(m_rank_name.c_str(), *citr)) {
				redis_client_t::remove_key(m_rank_name.c_str(), *citr);
			}
			continue;
		}

		if (!time_manager_t::check_same_period(time_id, redis_client_t::get_uint32(m_rank_name.c_str(), "time", *citr))) {
			ARENA_LOG("role[%lu] arena score reset", *citr);
			// 重置
			uint32_t stage = redis_client_t::get_uint32(m_rank_name.c_str(), "stage", *citr);
			score = reset_stage(stage);
			
			REDIS_BATCH_START
			redis_client_t::set_uint32(m_rank_name.c_str(), "score", *citr, score);
			redis_client_t::set_uint32(m_rank_name.c_str(), "stage", *citr, stage);
			redis_client_t::set_uint32(m_rank_name.c_str(), "time", *citr, now_time);
			REDIS_BATCH_DONE
		}

		batch_set.push_back(std::pair<uint64_t, uint32_t>(*citr, score));
	}

	// 起服批量设置
	update_rank_score_batch(batch_set);

	update_rank();

	return true;
}

uint32_t arena_rank_t::update_arena_score_rank(uint64_t role_uid, uint32_t score, uint32_t stage, uint32_t server_id, const char *server_name, const char *name, uint32_t fighting_value, const proto::common::hero_data &hero_info, const proto::common::item_data &hero_equip)
{
	if (!update_sort(role_uid, score)) { // 失败表示未入榜
		return 0;
	}

	REDIS_BATCH_START
	redis_client_t::set_uint32(m_rank_name.c_str(), "score", role_uid, score);
	redis_client_t::set_uint32(m_rank_name.c_str(), "stage", role_uid, stage);
	redis_client_t::set_uint32(m_rank_name.c_str(), "time", role_uid, common::time_util_t::now_time());
	redis_client_t::set_uint32(m_rank_name.c_str(), "server_id", role_uid, server_id);
	redis_client_t::set_string(m_rank_name.c_str(), "server_name", role_uid, server_name);
	redis_client_t::set_uint32(m_rank_name.c_str(), "fighting_value", role_uid, fighting_value);
	redis_client_t::set_string(m_rank_name.c_str(), "name", role_uid, name);
	redis_client_t::set_protobuf(m_rank_name.c_str(), "fight_heroes", role_uid, hero_info);
	redis_client_t::set_protobuf(m_rank_name.c_str(), "hero_equips", role_uid, hero_equip);
	REDIS_BATCH_DONE

	return get_rank_value(role_uid);
}

void arena_rank_t::get_rank_list(uint32_t start, uint64_t uid, proto::server::rg_get_arena_rank_data_reply& reply)
{
	// 0代表查找玩家自己
	if (start == 0) {
		uint32_t rank_index = get_rank_value(uid);
		if (rank_index == 0) { // 未入榜
			return;
		}

		proto::common::arena_role_rank_info *p_rank_info = reply.mutable_rand_data()->add_info_list();
		if (p_rank_info) {
			peek_data(uid, p_rank_info);
			p_rank_info->set_rank(rank_index);
		}
	} else {
		if (start > get_rank_count()) {
			return;
		}

		std::vector<uint64_t> list;
		int32_t _start = start - 1;
		int32_t _end = (int32_t)m_curr_count < _start + RANK_PAGE_COUNT ? (int32_t)m_curr_count - 1 : _start + RANK_PAGE_COUNT - 1;
		get_range_by_rank(_start, _end, list);

		for (std::vector<uint64_t>::const_iterator citr = list.begin();
			citr != list.end(); ++citr) {
			proto::common::arena_role_rank_info* p_rank_info = reply.mutable_rand_data()->add_info_list();
			if (p_rank_info) {
				peek_data(*citr, p_rank_info);
				p_rank_info->set_rank(get_rank_value(*citr));
			}
		}
	}
}

void arena_rank_t::peek_data(uint64_t uid, proto::common::arena_role_rank_info* p_data)
{
	if (p_data) {
		if (!redis_client_t::is_hash_exist(m_rank_name.c_str(), uid)) {
			log_error("role[%lu] in rank, not exist error", uid);
			return;
		}

		p_data->set_uid(common::string_util_t::uint64_to_string(uid));
		p_data->set_score(redis_client_t::get_uint32(m_rank_name.c_str(), "score", uid));
		p_data->set_stage(redis_client_t::get_uint32(m_rank_name.c_str(), "stage", uid));
		p_data->set_time(redis_client_t::get_uint32(m_rank_name.c_str(), "time", uid));
		p_data->set_server_id(redis_client_t::get_uint32(m_rank_name.c_str(), "server_id", uid));
		p_data->set_fighting_value(redis_client_t::get_uint32(m_rank_name.c_str(), "fighting_value", uid));
		p_data->set_name(redis_client_t::get_string(m_rank_name.c_str(), "name", uid));
		p_data->set_server_name(redis_client_t::get_string(m_rank_name.c_str(), "server_name", uid));

		proto::common::hero_data* p_hero = p_data->mutable_fight_heroes();
		if (p_hero) {
			redis_client_t::get_protobuf(m_rank_name.c_str(), "fight_heroes", uid, *p_hero);
		}

		proto::common::item_data* p_equip = p_data->mutable_hero_equips();
		if (p_equip) {
			redis_client_t::get_protobuf(m_rank_name.c_str(), "hero_equips", uid, *p_equip);
		}
	}
}

void arena_rank_t::get_16th_final_player(std::vector<uint64_t>& player_vec)
{
	// 老代码，取出前32名？
// 	uint32_t count = 0;
// 	for (auto it : m_arena_score_rank_list)
// 	{
// 		uint64_t player_uid = it->get_role_uid();
// 		if (player_uid > 0)
// 		{
// 			player_vec.push_back(player_uid);
// 			++count;
// 		}
// 		if (count >= 32)
// 		{
// 			break;
// 		}
// 	}

	get_range_by_rank(0, 31, player_vec);
}

uint32_t arena_rank_t::reset_stage(uint32_t& stage)
{
	auto p_conf = GET_CONF(ArenaClassTable, stage);
	if (NULL == p_conf)
	{
		log_error("NULL == p_conf[%d]", stage);
		return 0;
	}
	stage = p_conf->reset_stage();
	p_conf = GET_CONF(ArenaClassTable, stage);
	if (NULL == p_conf)
	{
		log_error("NULL == p_conf[%d]", stage);
		return 0;
	}
	return p_conf->score();
}

void arena_rank_t::reset_stage()
{
	uint32_t now_time = time_util_t::now_time();

	std::vector<uint64_t> list;
	get_range_by_rank(0, -1, list);
	for (std::vector<uint64_t>::const_iterator citr = list.begin();
		citr != list.end(); ++citr) {
		uint32_t stage = redis_client_t::get_uint32(m_rank_name.c_str(), "stage", *citr);
		uint32_t score = reset_stage(stage);

		if (!update_sort(*citr, score)) {
			continue;
		}

		REDIS_BATCH_START
		redis_client_t::set_uint32(m_rank_name.c_str(), "score", *citr, score);
		redis_client_t::set_uint32(m_rank_name.c_str(), "stage", *citr, stage);
		redis_client_t::set_uint32(m_rank_name.c_str(), "time", *citr, now_time);
		REDIS_BATCH_DONE
	}
}

//////////////////////////////////////////////////////////////////////////
arena_rank_ptr arena_score_rank_mgr_t::m_arena_rank = NULL;

void arena_score_rank_mgr_t::get_16th_final_player(std::vector<uint64_t>& player_vec)
{
	if (m_arena_rank) {
		m_arena_rank->get_16th_final_player(player_vec);
	}
}


uint32_t arena_score_rank_mgr_t::get_arena_rank(uint64_t uid)
{
	if (m_arena_rank) {
		return m_arena_rank->get_rank_value(uid);
	}

	return 0;
}

void arena_score_rank_mgr_t::reset_stage()
{
	if (m_arena_rank) {
		m_arena_rank->reset_stage();
	}
}

bool arena_score_rank_mgr_t::init(uint32_t time_id)
{
	m_arena_rank.reset(new arena_rank_t(common::rank_common::arena_rank_0, "arena_score_rank"));

	if (!m_arena_rank) {
		log_error("m_arena_rank null error");
		return false;
	}

	return m_arena_rank->load_from_redis(time_id);
}
