#include "item/item_limit.hpp"
#include "common/config_mgr.h"
#include "tblh/DropWorldTable.tbls.h"
#include "main/game_server_fwd.hpp"

item_limit_t::item_limit_t(uint32_t item_tid, uint32_t item_num, uint32_t last_give_time)
	: m_item_tid(item_tid)
	, m_count(item_num)
	, m_last_give_time(last_give_time)
{
}

item_limit_t::~item_limit_t()
{

}

void item_limit_t::set_count(uint32_t count)
{
	m_count = count;
}

bool item_limit_t::can_add(uint32_t item_num)
{
	DropWorldTable *p_drop_world = GET_CONF(DropWorldTable, m_item_tid);
	if (NULL == p_drop_world)
	{
		log_error("[wys] item_limit DropWorldTable[%d] not find", m_item_tid);
		return false;
	}
	uint32_t now_time = common::time_util_t::now_time();
	// 时间间隔不满足
	if (now_time - m_last_give_time < p_drop_world->drop_interval())
	{
		ITEM_LOG("[wys] item_limit can't give item[%d] time not reach now[%d] last[%d] interval[%d]", m_item_tid,
				  now_time, m_last_give_time, p_drop_world->drop_interval());
		return false;
	}
	// 最大数量不满足
	if (m_count + item_num > p_drop_world->drop_max_num())
	{
		ITEM_LOG("[wys] item_limit can't give item[%d] count[%d] add[%d] reach max num[%d]", m_item_tid, m_count,
					item_num, p_drop_world->drop_max_num());
		return false;
	}
	return true;
}

