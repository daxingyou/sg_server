#include "rank.hpp"
#include "redis_client.hpp"
#include "tblh/RankType.tbls.h"
#include "log.hpp"
#include "config_mgr.h"

rank_t::rank_t(common::rank_common::rank_type type, const char* name)
	: m_rank_type(type)
	, m_rank_name(name)
	, m_is_time_temp(false)
{

}

bool rank_t::init()
{
	RankType* p_conf = GET_CONF(RankType, m_rank_type);
	if (!p_conf) {
		log_error("rank type[%u] not define", m_rank_type);
		return false;
	}

	m_min_condi = p_conf->min_value();
	m_max_count = p_conf->count();
	m_is_time_temp = p_conf->type() > 0 ? true : false;
	return true;
}

void rank_t::clear_rank()
{
	if (m_is_time_temp) { // 如果是
		std::vector<uint64_t> vecs;
		redis_client_t::get_all_keys<uint64_t, std::vector<uint64_t>>(m_rank_name.c_str(), vecs);
		for (const auto& uid : vecs) {
			redis_client_t::remove_key(m_rank_name.c_str(), uid);
		}
	} 

	redis_client_t::m_redis_cli.del_zset(m_rank_name);
}

uint32_t rank_t::get_rank_count()
{
	return redis_client_t::m_redis_cli.get_zset_count(m_rank_name);
}

uint32_t rank_t::get_rank_count_by_score(uint32_t min, uint32_t max)
{
	return redis_client_t::m_redis_cli.get_zset_count_by_score(m_rank_name, min, max);
}

bool rank_t::update_rank_score(uint64_t uid, uint32_t score)
{
	return redis_client_t::m_redis_cli.set_zset_data(m_rank_name, score, uid);
}

bool rank_t::update_rank_score_batch(const std::vector<std::pair<uint64_t, uint32_t>>& vecs)
{
	return redis_client_t::m_redis_cli.batch_set_zset_data(m_rank_name, vecs);
}

bool rank_t::is_in_rank(uint64_t uid)
{
	return redis_client_t::m_redis_cli.is_zset_key_exsit(m_rank_name, uid);
}

bool rank_t::rem_rank_by_uid(uint64_t uid)
{
	return redis_client_t::m_redis_cli.rem_zset_data(m_rank_name, uid);
}

bool rank_t::batch_rem_rank_by_rank(int32_t start, int32_t stop)
{
	return redis_client_t::m_redis_cli.batch_rem_zset_data_by_rank(m_rank_name, start, stop);
}

bool rank_t::batch_rem_rank_by_score(uint32_t min, uint32_t max)
{
	return redis_client_t::m_redis_cli.batch_rem_zset_data_by_score(m_rank_name, min, max);
}

uint32_t rank_t::get_rank_score(uint64_t uid)
{
	return redis_client_t::m_redis_cli.get_zset_score<uint32_t, uint64_t>(m_rank_name, uid);
}

uint32_t rank_t::get_rank_value(uint64_t uid)
{
	int32_t rank = redis_client_t::m_redis_cli.get_zset_revrank(m_rank_name, uid);
	if (rank == -1) {
		return 0;
	}

	return rank + 1;
}

bool rank_t::get_range_by_rank(int32_t start, int32_t stop, std::vector<uint64_t>& result)
{
	return redis_client_t::m_redis_cli.get_zset_revrange(m_rank_name, start, stop, result);
}

bool rank_t::get_range_by_rank_withscores(int32_t start, int32_t stop, std::vector<std::pair<uint64_t, uint32_t>>& result)
{
	return redis_client_t::m_redis_cli.get_zset_revrange_withscores(m_rank_name, start, stop, result);
}

bool rank_t::get_range_by_score(uint32_t max, uint32_t min, std::vector<uint64_t>& result)
{
	return redis_client_t::m_redis_cli.get_zset_revrange_by_score(m_rank_name, max, min, result);
}

bool rank_t::update_sort(uint64_t uid, uint32_t score)
{
	if (!m_open) return false;

	if (m_min_condi != 0 && score < m_min_condi) { // 小于入榜条件
		if (is_in_rank(uid)) { // 是否在榜单中，在榜中就移除
			remove_rank(uid);
			update_rank();
		}
		return false;
	}

	if (m_max_count != 0 && m_curr_count >= m_max_count && score < m_min_value) {
		if (uid == m_min_uid) {
			if (!update_rank_score(uid, score)) {
				log_error("rank[%s] update single[%lu:%u] failed", m_rank_name.c_str(), uid, score);
				return false;
			}
			redis_client_t::set_uint32(m_rank_name.c_str(), "score", uid, score);
			m_min_value = score;
			return true;
		} else {
			return false;
		}
	}

	if (!update_rank_score(uid, score)) {
		log_error("rank[%s] update single[%lu:%u] failed", m_rank_name.c_str(), uid, score);
		return false;
	}

	redis_client_t::set_uint32(m_rank_name.c_str(), "score", uid, score);

	update_rank();

	return true;
}

bool rank_t::update_sort_add(uint64_t uid, uint32_t score_add)
{
	if (!m_open) return false;

	uint32_t new_score = redis_client_t::get_uint32(m_rank_name.c_str(), "score", uid) + score_add;
	bool ret = true;
	do {
		if (m_min_condi != 0 && new_score < m_min_condi) { // 小于入榜条件
			break;
		}

		if (m_max_count != 0 && m_curr_count >= m_max_count && new_score < m_min_value) {
			break;
		}

		if (!update_rank_score(uid, new_score)) {
			log_error("rank[%s] update single[%lu:%u] failed", m_rank_name.c_str(), uid, new_score);
			ret = false;
			break;
		}

		update_rank();
	} while (0);

	if (ret)
		redis_client_t::set_uint32(m_rank_name.c_str(), "score", uid, new_score);

	return ret;
}

void rank_t::update_rank()
{
	m_curr_count = get_rank_count();

	if (!m_max_count) return;

	if (m_curr_count > m_max_count) {
		std::vector<uint64_t> vec;
		redis_client_t::m_redis_cli.get_zset_range(m_rank_name, 0, m_curr_count - m_max_count - 1, vec);
		if (!vec.empty()) {
			for (std::vector<uint64_t>::const_iterator citr = vec.begin();
				citr != vec.end(); ++citr) {
				remove_rank(*citr);
			}
		}

		m_curr_count = get_rank_count();
		if (m_curr_count > m_max_count) {
			log_error("rank[%s] size[%u] > max_count[%u] error", m_rank_name.c_str(), m_curr_count, m_max_count);
		}
	}

	if (m_curr_count >= m_max_count) { // 到达最大数量 找到阈值用于判断
		uint32_t Threshold = m_curr_count == m_max_count ? 0 : m_curr_count - m_max_count;
		std::vector<uint64_t> vec;
		redis_client_t::m_redis_cli.get_zset_range(m_rank_name, Threshold, Threshold, vec);
		if (vec.empty()) {
			log_error("rank[%s] get Threshold error", m_rank_name.c_str());
			m_min_uid = 0;
			m_min_value = 0;
		} else {
			m_min_uid = vec[0];
			m_min_value = get_rank_score(m_min_uid);
		}
	} else {
		m_min_uid = 0;
		m_min_value = 0;
	}
}

void rank_t::remove_rank(uint64_t uid)
{
	rem_rank_by_uid(uid);

	if (!m_is_time_temp) {
		redis_client_t::remove_key(m_rank_name.c_str(), uid);
	}
}
