#ifndef __ACCOUNT_CHANNEL_MANAGER_HPP__
#define __ACCOUNT_CHANNEL_MANAGER_HPP__

#include "curl_wrapper.hpp"
#include "client.pb.h"
#include "channel.hpp"

#include <boost/scoped_ptr.hpp>
#include <boost/thread.hpp>

class channel_manager_t
{
public:
    static bool open(int32_t thread_num = 1);
    static void close();

public:
    static void add_channel(channel_ptr p_channel);
    static channel_ptr get_channel(uint32_t channel_id);

    // ÇþµÀµÇÂ½
public:
    static void channel_login(const network::tcp_socket_ptr& s, const proto::client::cn_server_info_request& req);
private:
    static void post_login_request(account_channel_ptr p_channel);
    static void handle_login_request(account_channel_ptr p_channel);
    static void post_login_respone(const account_channel_ptr& p_channel);
    static void handle_login_respone(const account_channel_ptr& p_channel);

public:
    static std::map<uint32_t, channel_ptr> m_channel_list;

    struct thread_data_t
    {
        std::map<uint32_t, curl_warpper_ptr> curl_warpper_list; //<channel_id, curl_warpper_ptr>
        boost::scoped_ptr<boost::asio::io_service::work> work;
    };
    static boost::thread_specific_ptr<thread_data_t> m_thread_data;
    static int32_t m_thread_num;
    static std::vector<boost::asio::io_service*> m_ios_list;
    static int32_t m_last_post_idx;
};


#endif
