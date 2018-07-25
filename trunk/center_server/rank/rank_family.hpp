#pragma once

#include "rank_sort.hpp"

class rank_family_t : public rank_sort_t
{
public:
	rank_family_t() = delete;
	rank_family_t(common::rank_common::rank_type type, const char* name);
	virtual ~rank_family_t() = default;

public:
	// ���µȼ����а�
	void update_prestige(uint64_t uid, uint32_t val);
	// peek����
	bool peek_data(uint64_t uid, proto::common::rank_single_data* p_data);
	// �����а�ɾ������
	void del_family(uint64_t uid);
};

typedef boost::shared_ptr<rank_family_t> rank_family_ptr;