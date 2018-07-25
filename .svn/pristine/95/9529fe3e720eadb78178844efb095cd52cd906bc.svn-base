#pragma once

#include "rank_sort.hpp"

class rank_criminal_t : public rank_sort_t
{
public:
	rank_criminal_t() = delete;
	rank_criminal_t(common::rank_common::rank_type type, const char* name);
	virtual ~rank_criminal_t() = default;

public:
	// game更新的数据
	void update_rank_from_game(const proto::server::ge_update_rank_notify& notify);
};

typedef boost::shared_ptr<rank_criminal_t> rank_criminal_ptr;
