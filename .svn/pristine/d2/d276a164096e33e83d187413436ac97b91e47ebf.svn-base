#pragma once
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/asio/deadline_timer.hpp>

#include "log.hpp"

// 对boost::asio::deadline_timer的简单封装
// start函数适用于有实例的类，不适用于静态类以及回调全局函数的情况
// start_no_instance函数适用于静态类以及回调全局函数的情况
class simple_timer_t
{
public:
	simple_timer_t()
	{
	}

	~simple_timer_t()
	{
	}

	// 不用初始化，直接调用start传入time，
	// 以及类的this指针，类的成员函数(回调成员函数只有一个const boost::system::error_code& ec参数)，以及env::server->get_ios()
	template<typename INST, typename FUNC>
	void start(uint32_t expired_time, INST instance, FUNC callback, boost::asio::io_service& ios)
	{
        if (0 == expired_time)
        {
            log_warn("error: expired_time is 0 !");
            return;
        }

		if (NULL == m_timer)
		{
			m_timer.reset(new boost::asio::deadline_timer(ios));
		}

        
		m_timer->expires_from_now(boost::posix_time::millisec(expired_time));
		m_timer->async_wait(boost::bind(callback, instance, boost::asio::placeholders::error));
		m_start_time = (uint32_t)time(NULL);
		m_expired_time = m_start_time + expired_time;
	}

	// 不用初始化，直接调用start传入time，
	// 以及回调函数(回调成员函数只有一个const boost::system::error_code& ec参数)，以及env::server->get_ios()
	template<typename FUNC>
	void start_no_instance(uint32_t expired_time, FUNC callback, boost::asio::io_service& ios)
	{
        if (0 == expired_time)
        {
            log_warn("error: expired_time is 0 !");
            return;
        }

		if (NULL == m_timer)
		{
			m_timer.reset(new boost::asio::deadline_timer(ios));
		}
        
		m_timer->expires_from_now(boost::posix_time::seconds(expired_time));
		m_timer->async_wait(boost::bind(callback, boost::asio::placeholders::error));
		m_start_time = (uint32_t)time(NULL);
		m_expired_time = m_start_time + expired_time;
	}

	// 停止函数
	void stop()
	{
		if (NULL == m_timer)
		{
			return;
		}

		m_timer->cancel();
		m_start_time = 0;
		m_expired_time = 0;
	}

	uint32_t get_remain_time()
	{
		if (m_start_time == 0 || m_expired_time == 0)
		{
			return 0;
		}
		uint32_t now_time = (uint32_t)time(NULL);
		return m_expired_time - now_time;
	}

private:
	typedef boost::shared_ptr<boost::asio::deadline_timer> boost_timer;
	boost_timer m_timer = NULL;
	uint32_t m_start_time = 0;
	uint32_t m_expired_time = 0;
};