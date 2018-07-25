#pragma once

#include "rank_sort.hpp"

class rank_power_t : public rank_sort_t
{
public:
	rank_power_t() = delete;
	rank_power_t(common::rank_common::rank_type type, const char* name);
	virtual ~rank_power_t() = default;

public:
	// ����ս�������а�
	void update_power(uint64_t uid, uint32_t val);
};

typedef boost::shared_ptr<rank_power_t> rank_power_ptr;