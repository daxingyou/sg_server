#pragma once

#include <boost/shared_ptr.hpp>

#include "server.pb.h"
#include "rank.hpp"
#include "user/global_user.hpp"

class rank_sort_t : public rank_t
{
public:
	rank_sort_t() = delete;
	rank_sort_t(common::rank_common::rank_type type, const char* name);
	virtual ~rank_sort_t() = default;

	virtual bool init();

public:
	// 发奖
	void award();
public:
	// 获得一个id列表
	uint32_t get_page_list(uint32_t page, proto::client::ec_rank_reply& reply);
	// peek数据
	virtual bool peek_data(uint64_t uid, proto::common::rank_single_data* p_data);
	// game更新的数据
	virtual void update_rank_from_game(const proto::server::ge_update_rank_notify& notify) {};

protected:
	// 获得玩家信息
	void peek_role_base(uint64_t uid, proto::common::rank_role_base* p_role_base);
	// 获得家族信息
	void peek_family_base(uint64_t family_id, proto::common::rank_family_base* p_family_base);
};

typedef boost::shared_ptr<rank_sort_t> rank_sort_ptr;