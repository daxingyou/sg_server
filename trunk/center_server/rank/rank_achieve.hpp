#pragma once

#include "rank_sort.hpp"

class rank_achieve_t : public rank_sort_t
{
public:
	rank_achieve_t() = delete;
	rank_achieve_t(common::rank_common::rank_type type, const char* name);
	virtual ~rank_achieve_t() = default;

public:
	// peek数据
	bool peek_data(uint64_t uid, proto::common::rank_single_data* p_data);
	// game更新的数据
	void update_rank_from_game(const proto::server::ge_update_rank_notify& notify);
};

typedef boost::shared_ptr<rank_achieve_t> rank_achieve_ptr;