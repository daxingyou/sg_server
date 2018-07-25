#include "channel_manager.hpp"
#include "main/account_server.hpp"
#include "main/account_server_fwd.hpp"
#include "log.hpp"
#include "curl_wrapper.hpp"
#include "client.pb.h"
#include "user/user_manager.hpp"
#include "serverlist/server_info_manager.hpp"

USING_NS_NETWORK;
USING_NS_COMMON;

static boost::thread_group g_threads;

void thread_main(boost::asio::io_service* ios)
{
	FUNC_CHECK_START
        channel_manager_t::m_thread_data.reset(new channel_manager_t::thread_data_t());
        std::map<uint32_t, channel_ptr>::const_iterator it = channel_manager_t::m_channel_list.begin();
        std::map<uint32_t, channel_ptr>::const_iterator it_end = channel_manager_t::m_channel_list.end();
        for ( ; it != it_end; ++it)
        {
            uint32_t channel_id = it->first;
            channel_ptr p_channel = it->second;
            if (NULL == p_channel)
            {
                continue;
            }

            curl_warpper_ptr p_curl(new curl_warpper_t());
            if (!p_curl->init(p_channel->m_login_url))
            {
                p_curl->clean();
                COMMON_ASSERT(false);
                return;
            }

            channel_manager_t::m_thread_data->curl_warpper_list.insert(std::pair<uint32_t, curl_warpper_ptr>(channel_id, p_curl));
        }
        channel_manager_t::m_thread_data->work.reset(new boost::asio::io_service::work(*ios));
        ios->run();
	FUNC_CHECK_END

    // 线程退出
    std::map<uint32_t, curl_warpper_ptr>::iterator it = channel_manager_t::m_thread_data->curl_warpper_list.begin();
    std::map<uint32_t, curl_warpper_ptr>::iterator it_end = channel_manager_t::m_thread_data->curl_warpper_list.end();
    for ( ; it != it_end; ++it)
    {
        curl_warpper_ptr p_curl = it->second;
        if (NULL == p_curl)
        {
            continue;
        }
        p_curl->clean();
    }
    SAFE_DELETE_P(ios);
}

void thread_close()
{
    channel_manager_t::m_thread_data->work.reset();
}

//--------------------------------------------------------------------------------------
std::map<uint32_t, channel_ptr> channel_manager_t::m_channel_list;
int32_t channel_manager_t::m_last_post_idx = 0;
int32_t channel_manager_t::m_thread_num = 0;
std::vector<boost::asio::io_service*> channel_manager_t::m_ios_list;
boost::thread_specific_ptr<channel_manager_t::thread_data_t> channel_manager_t::m_thread_data;

bool channel_manager_t::open(int32_t thread_num)
{
    CURLcode code = curl_global_init(CURL_GLOBAL_DEFAULT);
    if (CURLE_OK != code)
    {
        log_error("init global curl failed!, code[%d]", code);
        return false;
    }

    m_thread_num = thread_num;
    m_ios_list.reserve(thread_num);
    for (int32_t i = 0; i < thread_num; ++i)
    {
        m_ios_list.push_back(new boost::asio::io_service());
        g_threads.create_thread(boost::bind(&thread_main, m_ios_list.back()));
    }

    return true;
}

void channel_manager_t::close()
{
    size_t vec_size = m_ios_list.size();
    for (size_t i = 0; i < vec_size; ++i)
    {
        if (m_ios_list[i])
        {
            m_ios_list[i]->post(boost::bind(&thread_close));
        }
    }
    m_ios_list.clear();

    g_threads.join_all();

    curl_global_cleanup();
}

void channel_manager_t::add_channel(channel_ptr p_channel)
{
    if (NULL == p_channel)
    {
        log_error("NULL == p_channel");
        return;
    }

    m_channel_list.insert(std::pair<uint32_t, channel_ptr>(p_channel->m_id, p_channel));
}

channel_ptr channel_manager_t::get_channel(uint32_t channel_id)
{
    std::map<uint32_t, channel_ptr>::iterator it = m_channel_list.find(channel_id);
    std::map<uint32_t, channel_ptr>::iterator it_end = m_channel_list.end();
    if (it == it_end)
    {
        return channel_ptr();
    }

    return it->second;
}

//-------------------------------------------------------------------------------------------------------------------------------
void channel_manager_t::channel_login(const network::tcp_socket_ptr& s, const proto::client::cn_server_info_request& req)
{
    //channel id 默认为test渠道
    uint32_t channel_id = 0;
    if (req.has_channel_id())
    {
        channel_id = req.channel_id();
    }
    channel_ptr p_channel = get_channel(channel_id);
    if (NULL == p_channel)
    {
        log_error("NULL == p_channel[%d]", channel_id);
        return;
    }
			
	//验证版本号
	std::string version = "0.0.0.0";
	if (req.has_version())
	{
		version = req.version();
	}

	if (!server_info_manager_t::is_commpatible_version(req.mark_type(), version))
	{
        proto::client::nc_server_info_reply reply;
        reply.set_reply_code(3);
        env::server->send_msg_to_client(s, op_cmd::nc_server_info_reply, reply);
		log_error("client[%s] is not commpatible version [%s]", req.mark_type().c_str(), version.c_str());
        return;
	}
	
	//	
    account_channel_ptr p_account_channle(new account_channel_t());
    if (!p_account_channle->load_data(p_channel, s, req))
    {
        proto::client::nc_server_info_reply reply;
        reply.set_reply_code(2);
        env::server->send_msg_to_client(s, op_cmd::nc_server_info_reply, reply);
		log_error("load channel[%d] info not found", channel_id);
        return;
    }

	if (req.has_mark_type()){
		p_account_channle->m_login_mark_type = req.mark_type();
	}


    post_login_request(p_account_channle);
}

void channel_manager_t::post_login_request(account_channel_ptr p_account_channle)
{
    m_ios_list[m_last_post_idx%m_thread_num]->post(boost::bind(&channel_manager_t::handle_login_request, p_account_channle));
    m_last_post_idx++;
    if (m_last_post_idx > m_thread_num)
    {
        m_last_post_idx = 0;
    }
}

void channel_manager_t::handle_login_request(account_channel_ptr p_account_channle)
{
    if (NULL == p_account_channle)
    {
        log_error("NULL == p_account_channle");
        return;
    }
    thread_data_t* p_thread = m_thread_data.get();
    if (NULL == p_thread)
    {
        log_error("NULL == p_thread");
        return;
    }

    if (CHANNEL_ID_TEST == p_account_channle->m_channel_id)
    {
        p_account_channle->m_login_respone_data.m_guid = p_account_channle->m_login_request_data.m_uid;
    }
    else if (p_account_channle->m_channel_id > CHANNEL_ID_TEST && p_account_channle->m_channel_id < CHANNEL_ID_END)
    {
        //除了测试渠道test之外的渠道一律走这里
        curl_warpper_ptr p_curl = p_thread->curl_warpper_list[p_account_channle->m_channel_id];
        if (NULL == p_curl)
        {
            log_error("NULL == p_curl");
            return;
        }
        p_account_channle->m_login_respone_data.m_guid = p_curl->post_http_request(p_account_channle->m_post_login_fields);
        if (p_account_channle->m_login_respone_data.m_guid.empty())
        {
            log_error("m_guid is empty");
            return;
        }
    }

    //post到主线程
    post_login_respone(p_account_channle);
}

//-----------------------------------------------------------------------------
void channel_manager_t::post_login_respone(const account_channel_ptr& p_account_channle)
{
    env::server->get_ios().post(boost::bind(&channel_manager_t::handle_login_respone, p_account_channle));
}

void channel_manager_t::handle_login_respone(const account_channel_ptr& p_account_channle)
{
    user_manager_t::channel_login_respone(p_account_channle);
}