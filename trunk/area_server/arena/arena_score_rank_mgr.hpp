#ifndef __ARENA_SCORE_RANK_MGR_HPP__
#define __ARENA_SCORE_RANK_MGR_HPP__

#include <boost/shared_ptr.hpp>
#include "rank.hpp"
#include "server.pb.h"

// 武将攻略展示数量
#define hero_strategy_show_num 5

class arena_rank_t : public rank_t
{
public:
	arena_rank_t() = delete;
	arena_rank_t(common::rank_common::rank_type type, const char* name);
	virtual ~arena_rank_t() = default;

	bool load_from_redis(uint32_t time_id);

public:
	uint32_t update_arena_score_rank(uint64_t role_uid, uint32_t score, uint32_t stage, uint32_t server_id,
		const char *server_name, const char *name, uint32_t fighting_value,
		const proto::common::hero_data &hero_info,
		const proto::common::item_data &hero_equip);

	// 获得列表
	void get_rank_list(uint32_t start, uint64_t uid, proto::server::rg_get_arena_rank_data_reply& reply);

	void peek_data(uint64_t uid, proto::common::arena_role_rank_info* p_data);

	void get_16th_final_player(std::vector<uint64_t>& player_vec);
	uint32_t reset_stage(uint32_t& stage);
	void reset_stage();
};

typedef boost::shared_ptr<arena_rank_t> arena_rank_ptr;

class arena_score_rank_mgr_t
{
public:
	static bool init(uint32_t time_id);
	static const arena_rank_ptr& get_arena_rank_ptr() { return m_arena_rank; }

public:
	static void get_16th_final_player(std::vector<uint64_t>& player_vec);
	static uint32_t get_arena_rank(uint64_t uid);
	static void reset_stage();

protected:
	static arena_rank_ptr m_arena_rank;
};

#endif
