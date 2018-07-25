#ifndef __CENTER_SHOP_COMMON_H__
#define __CENTER_SHOP_COMMON_H__

#include "macros.hpp"

#include "common.pb.h"

// ��Ʒ�ϼ�����
enum shop_puton_type : uint32_t
{
	shop_puton_type_none = 0,
	shop_puton_type_time = 1,
	shop_puton_type_gm = 2,
};

struct shop_goods_t
{
	uint32_t m_id = 0;
	uint32_t m_shoptype = 0;			// ��Ʒ����
	uint32_t m_buy_count = 0;			// �����޹�ʱ�乺�������
	proto::common::shop_refresh_type m_refresh_type = proto::common::shop_refresh_type_none;	// �޹�ˢ�·�ʽ
	uint32_t m_start_time = 0;			// �ϼ�ʱ��
	uint32_t m_end_time = 0;			// �¼�ʱ��
	uint32_t m_puton_time = 0;			// �ϼ�ʱ�䣬ʱ���id
	uint32_t m_last_refresh_time = 0;	// �޹��´θ���ʱ��
};

struct shop_user_t
{
	proto::common::user_goods m_user_goods;	// �޹���Ϣ
	uint32_t m_shoptype = 0;				// ��Ʒ����
};

#endif
