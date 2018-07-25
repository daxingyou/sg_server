#pragma once

#include "rank_sort.hpp"

class rank_recruit_t : public rank_sort_t
{
public:
	rank_recruit_t() = delete;
	rank_recruit_t(common::rank_common::rank_type type, const char* name);
	virtual ~rank_recruit_t() = default;

public:
	// game更新的数据
	void update_rank_from_game(const proto::server::ge_update_rank_notify& notify);
};

typedef boost::shared_ptr<rank_recruit_t> rank_recruit_ptr;