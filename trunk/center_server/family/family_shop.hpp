#pragma once

#include "macros.hpp"
#include "protos_fwd.hpp"

/**
 * \brief 家族商店
 */

class family_shop_t
{
	friend class family_data_mgr_t;

public:
	explicit family_shop_t(uint64_t family_id);
	~family_shop_t();

public:
	// 得到刷新时间
	uint32_t get_next_refresh_time() const { return m_shop_data.next_refresh_time(); }

	// 得到家族商店数据
	const proto::common::family_shop_data& get_family_shop_data() const { return m_shop_data; }

	// 刷新家族商店
	void refresh_family_shop(uint32_t family_lv);

	// 玩家购买
	void add_shop_buy_count(uint32_t index, uint32_t num);

	// 购买失败回退数量
	void shop_failed_back(uint32_t index, uint32_t num);

protected:
	uint64_t m_family_id = 0;
	proto::common::family_shop_data m_shop_data;
};