#ifndef __LOG_LOG_MANAGER_HPP__
#define __LOG_LOG_MANAGER_HPP__

#include "log_data.hpp"
#include "tcp_socket.hpp"
#include <boost/scoped_ptr.hpp>
#include <boost/thread.hpp>

class log_manager_t
{
public:
    static bool init(int32_t thread_num, uint32_t update_to_db_time);
    static void close();

    static log_data_t& get_cur_thread_log_data();

    static void post_game_message(const network::tcp_socket_ptr& s, uint16_t cmd, const network::msg_buf_ptr& buf, uint64_t uid);

    static void post_center_message(const network::tcp_socket_ptr& s, uint16_t cmd, const network::msg_buf_ptr& buf, uint64_t uid);

    static void post_account_message(const network::tcp_socket_ptr& s, uint16_t cmd, const network::msg_buf_ptr& buf, uint64_t uid);

public:
    struct thread_data_t
    {
        thread_data_t(boost::asio::io_service& ios, uint32_t update_to_db_time)
            : log_data(ios, update_to_db_time)
        {
            log_data.start_timer();
        }
        boost::scoped_ptr<boost::asio::io_service::work> work;
        log_data_t log_data;
    };

    static boost::thread_specific_ptr<thread_data_t> m_thread_data;
    static int32_t m_thread_num;
    static std::vector<boost::asio::io_service*> m_ios_list;
};

#endif
