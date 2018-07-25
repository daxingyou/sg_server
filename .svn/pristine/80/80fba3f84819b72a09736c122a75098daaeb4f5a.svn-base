#pragma once

#include "rank_sort.hpp"

class rank_level_t : public rank_sort_t
{
public:
	rank_level_t() = delete;
	rank_level_t(common::rank_common::rank_type type, const char* name);
	virtual ~rank_level_t() = default;

public:
	// 更新等级排行榜
	void update_level(uint64_t uid, uint32_t val);

public:
	// 获得各个国家排名前N的玩家
	void get_rank_user(uint32_t rank, std::vector<uint64_t>& uids);
};

typedef boost::shared_ptr<rank_level_t> rank_level_ptr;