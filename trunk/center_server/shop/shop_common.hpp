#ifndef __CENTER_SHOP_COMMON_H__
#define __CENTER_SHOP_COMMON_H__

#include "macros.hpp"

#include "common.pb.h"

// 商品上架类型
enum shop_puton_type : uint32_t
{
	shop_puton_type_none = 0,
	shop_puton_type_time = 1,
	shop_puton_type_gm = 2,
};

struct shop_goods_t
{
	uint32_t m_id = 0;
	uint32_t m_shoptype = 0;			// 商品类型
	uint32_t m_buy_count = 0;			// 本次限购时间购买的数量
	proto::common::shop_refresh_type m_refresh_type = proto::common::shop_refresh_type_none;	// 限购刷新方式
	uint32_t m_start_time = 0;			// 上架时间
	uint32_t m_end_time = 0;			// 下架时间
	uint32_t m_puton_time = 0;			// 上架时间，时间表id
	uint32_t m_last_refresh_time = 0;	// 限购下次更新时间
};

struct shop_user_t
{
	proto::common::user_goods m_user_goods;	// 限购信息
	uint32_t m_shoptype = 0;				// 商品类型
};

#endif
