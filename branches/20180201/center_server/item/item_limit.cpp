#include "item/item_limit.hpp"
#include "common/config_mgr.h"
#include "tblh/DropWorldTable.tbls.h"
#include "common/redis_client.hpp"
#include "main/center_server.hpp"
#include "main/center_server_fwd.hpp"
#include "protos_fwd.hpp"

USING_NS_COMMON;

item_limit_t::item_limit_t(uint32_t item_tid)
	: m_item_tid(item_tid)
{
}

item_limit_t::~item_limit_t()
{

}

bool item_limit_t::init()
{
	m_timer_reset.reset(new boost::asio::deadline_timer(env::server->get_ios()));
	start_reset_timer();
	return true;
}

bool item_limit_t::add_count(uint32_t count)
{
// 	if (!can_add(count))
// 	{
// 		return false;
// 	}
	m_count += count;
	m_last_give_time = common::time_util_t::now_time();
	redis_client_t::set_uint32("item_limit", "item_count", m_item_tid, m_count);
	return true;
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
        ITEM_LOG("item_limit can't give item[%d] count[%d] add[%d] reach max num[%d]", m_item_tid, m_count,
					item_num, p_drop_world->drop_max_num());
		return false;
	}
	return true;
}

void item_limit_t::reset()
{
	m_count = 0;
	m_last_give_time = 0;

	proto::server::eg_check_drop_items_reply msg;
	proto::common::item_limit_data *p_item_limit_single = NULL;

	p_item_limit_single = msg.add_item_list();
	if (NULL == p_item_limit_single)
	{
		log_error("p_item_limit_single NULL");
		return;
	}

	p_item_limit_single->set_item_tid(m_item_tid);
	p_item_limit_single->set_item_num(m_count);
	p_item_limit_single->set_last_give_time(m_last_give_time);
	
	env::server->send_msg_to_all_games(op_cmd::eg_check_drop_items_reply, 0, msg);
}

void item_limit_t::load_from_db()
{
	m_count = redis_client_t::get_uint32("item_limit", "item_count", m_item_tid);
}

void item_limit_t::reset_on_time(const boost::weak_ptr<item_limit_t>& p_witem_limit, const boost::system::error_code& ec)
{
    item_limit_ptr p_item_limit(p_witem_limit.lock());
    if (NULL == p_item_limit)
    {
        log_error("NULL == p_item_limit");
        return;
    }

    if (ec == boost::asio::error::operation_aborted)
    {
        
        return;
    }
    else if (ec)
    {
        log_error("Timer error: %s", ec.message().c_str());
        return;
    }

    p_item_limit->reset();
    p_item_limit->start_reset_timer();
}

bool item_limit_t::start_reset_timer()
{
	if (NULL == m_timer_reset)
	{
		log_error("item_limit m_timer_reset NULL");
		return false;
	}
	DropWorldTable *p_drop_world = GET_CONF(DropWorldTable, m_item_tid);
	if (NULL == p_drop_world)
	{
		log_error("[wys] item_limit DropWorldTable[%d] not find", m_item_tid);
		return false;
	}

	if (0 == p_drop_world->time_interval())
	{
		log_error("item_limit time_interval = 0");
		return false;
	}

	m_timer_reset->expires_from_now(boost::posix_time::seconds(p_drop_world->time_interval()));
	m_timer_reset->async_wait(boost::bind(&item_limit_t::reset_on_time, boost::weak_ptr<item_limit_t>(shared_from_this()), boost::asio::placeholders::error));
	return true;
}

void item_limit_t::cancel_reset_timer()
{
	if (m_timer_reset != NULL)
		m_timer_reset->cancel();
}

void item_limit_t::close()
{
	cancel_reset_timer();
}

