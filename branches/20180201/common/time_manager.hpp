#ifndef __TIME_MANAGER_HPP__
#define __TIME_MANAGER_HPP__

#include "timer_common.hpp"

#include <boost/asio/deadline_timer.hpp>
#include <boost/system/error_code.hpp>
#include <boost/shared_ptr.hpp>

typedef boost::shared_ptr<time_item_t> timer_item_ptr;

class time_manager_t
{
public:
    static bool init(boost::asio::io_service& ios, std::string server_start_time);
	static bool is_in_time(uint32_t id);
	static uint32_t register_func(uint32_t id, on_timer_callback func);
	static void unregister(uint32_t id, uint32_t key);
	static void unregister_all(uint32_t id);
	static bool is_timer_activating(uint32_t id);

    static void on_timer(const boost::system::error_code& ec);
    static void cancel_timer();

	static bool is_same_period(uint32_t id, uint32_t time_val);

	/**
	 * \brief �Ƿ�����һ��ʱ��
	 * \brief ���������Ƿ���������
	 */
	static bool check_previous_day(uint32_t curr_time, uint32_t check_time);

	/**
	* \Author qhj
	* \brief �Ƿ���ͬ��ʱ��(ֵ��һ������������)������ˢ���ɶ�ʱ������
	* \brief ���ͨ���û�time_manager����oneday�ȵ��ɶ�ʱ�������ı���ˢ��
	* \brief ��ˢ��ʱ���¼�����ݿ⣬��ˢ��ʱ��ȥ�ж��Ƿ���ͬ�������ڣ����Ǿ�ˢ��һ��
	*/
	static bool check_same_period(uint32_t id, uint32_t time_val);
	
	/**
	* \Author qhj
	* \brief �õ��´�ˢ��ʱ�䣨����ˢ���ɿͻ��˷���
	* \brief ���ͨ������һ���б�ʱˢ�£�ÿ���û������ʱ���ж��Ƿ���ˢ��ʱ��
	* \brief û�е����ʱ��Ͳ�ˢ��
	*/
	static uint32_t get_next_refresh_time(uint32_t id);

	// �õ�һ��id�����ˢ��ʱ��[qhj] std::pair[start_time, end_time]
	static std::pair<uint32_t, uint32_t> get_refresh_time(uint32_t id);

	static const std::vector<uint32_t>& get_server_start_date() { return m_server_start_date_vec; }

private:
    static boost::shared_ptr<boost::asio::deadline_timer> m_timer;
    static boost::shared_ptr<boost::asio::io_service::strand> m_strand;
	static bool m_timer_cancel;

	static std::vector<uint32_t> m_server_start_date_vec;
	static std::vector<uint32_t> m_server_start_time_vec;

	typedef std::unordered_map<uint32_t, timer_item_ptr> time_item_map;
	static time_item_map m_time_item_map;
};

#endif
