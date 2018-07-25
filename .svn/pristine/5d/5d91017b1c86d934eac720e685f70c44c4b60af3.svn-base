#pragma once
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/system/error_code.hpp>
#include <boost/asio/deadline_timer.hpp>

/*
author: wys
time:2016.12.21
desc:center 物品限制类 限制全局投放量
*/

class item_limit_t : public boost::enable_shared_from_this<item_limit_t>
{
public:
	item_limit_t(uint32_t item_tid);
	~item_limit_t();

	bool		init();
	void		close();
	uint32_t	get_item_tid() { return m_item_tid; }
	uint32_t	get_count() { return m_count; }
	uint32_t	get_last_give_time() { return m_last_give_time; }

	bool		add_count(uint32_t count);
	bool		can_add(uint32_t item_num);

	void		reset();
	void		load_from_db();

	static void reset_on_time(const boost::weak_ptr<item_limit_t>& p_witem_limit, const boost::system::error_code& ec);
	bool		start_reset_timer();
	void		cancel_reset_timer();
private:
	uint32_t m_item_tid = 0;		// 物品tid
	uint32_t m_count = 0;			// 累计发放数量
	uint32_t m_last_give_time = 0;	// 上次发放时间
	boost::shared_ptr<boost::asio::deadline_timer> m_timer_reset;		// 重置timer
};

typedef boost::shared_ptr<item_limit_t> item_limit_ptr;