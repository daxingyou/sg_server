#pragma once

#include "rank_sort.hpp"

class rank_charm_t : public rank_sort_t
{
public:
	rank_charm_t() = delete;
	rank_charm_t(common::rank_common::rank_type type, const char* name);
	virtual ~rank_charm_t() = default;

public:
	// 更新等级排行榜
	void update_charm(uint64_t uid, uint32_t val);
};

typedef boost::shared_ptr<rank_charm_t> rank_charm_ptr; 