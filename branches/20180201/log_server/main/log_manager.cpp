#include "log_manager.hpp"
#include "log.hpp"
#include "protos_fwd.hpp"
#include "game/game_msg_handler.hpp"
#include "center/center_msg_handler.hpp"
#include "account/account_msg_handler.hpp"
#include "source_type_converter.hpp"

USING_NS_NETWORK;
USING_NS_COMMON;

static boost::thread_group g_threads;

void thread_main(boost::asio::io_service* ios, uint32_t update_to_db_time)
{
	FUNC_CHECK_START
        log_manager_t::m_thread_data.reset(new log_manager_t::thread_data_t(*ios, update_to_db_time));
        log_manager_t::m_thread_data->work.reset(new boost::asio::io_service::work(*ios));
        ios->run();
	FUNC_CHECK_END

    //Ïß³ÌÍË³ö
    SAFE_DELETE_P(ios);
}

void thread_close()
{
    log_manager_t::m_thread_data->log_data.close_timer();
    log_manager_t::m_thread_data->log_data.flush_all();
    log_manager_t::m_thread_data->work.reset();
}

//---------------------------------------------------------------------------------------
int32_t log_manager_t::m_thread_num;
std::vector<boost::asio::io_service*> log_manager_t::m_ios_list;
boost::thread_specific_ptr<log_manager_t::thread_data_t> log_manager_t::m_thread_data;

bool log_manager_t::init(int32_t thread_num, uint32_t update_to_db_time)
{
    m_thread_num = thread_num;
    m_ios_list.reserve(thread_num);
    for (int i = 0; i < thread_num; ++i)
    {
        m_ios_list.push_back(new boost::asio::io_service());
        g_threads.create_thread(boost::bind(&thread_main, m_ios_list.back(), update_to_db_time));
    }

    if (!source_type_converter_t::init_desc())
    {
        log_error("init source type description failed!");
        return false;
    }

    return true;
}

void log_manager_t::close()
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

    log_info("close log manager success!");
}

log_data_t& log_manager_t::get_cur_thread_log_data()
{
    return m_thread_data->log_data;
}

void log_manager_t::post_game_message(const tcp_socket_ptr& s, uint16_t cmd, const msg_buf_ptr& buf, uint64_t uid)
{
    m_ios_list[uid%m_thread_num]->post(boost::bind(&game_msg_handler_t::handle_server_msg, s,uid, cmd, buf));
}

void log_manager_t::post_center_message(const tcp_socket_ptr& s, uint16_t cmd, const msg_buf_ptr& buf, uint64_t uid)
{
    m_ios_list[uid%m_thread_num]->post(boost::bind(&center_msg_handler_t::handle_server_msg, s,uid, cmd, buf));
}

void log_manager_t::post_account_message(const tcp_socket_ptr& s, uint16_t cmd, const msg_buf_ptr& buf, uint64_t uid)
{
    m_ios_list[uid%m_thread_num]->post(boost::bind(&account_msg_handler_t::handle_server_msg, s, cmd, buf));
}