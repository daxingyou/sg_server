#pragma once
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/asio/deadline_timer.hpp>

#include "log.hpp"

// ��boost::asio::deadline_timer�ļ򵥷�װ
// start������������ʵ�����࣬�������ھ�̬���Լ��ص�ȫ�ֺ��������
// start_no_instance���������ھ�̬���Լ��ص�ȫ�ֺ��������
class simple_timer_t
{
public:
	simple_timer_t()
	{
	}

	~simple_timer_t()
	{
	}

	// ���ó�ʼ����ֱ�ӵ���start����time��
	// �Լ����thisָ�룬��ĳ�Ա����(�ص���Ա����ֻ��һ��const boost::system::error_code& ec����)���Լ�env::server->get_ios()
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

	// ���ó�ʼ����ֱ�ӵ���start����time��
	// �Լ��ص�����(�ص���Ա����ֻ��һ��const boost::system::error_code& ec����)���Լ�env::server->get_ios()
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

	// ֹͣ����
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