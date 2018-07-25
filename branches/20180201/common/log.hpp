#ifndef __COMMON_LOG_HPP__
#define __COMMON_LOG_HPP__

#include "common_fwd.hpp"
#include "utility.hpp"

#include <boost/scoped_ptr.hpp>
#include <boost/thread.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/asio.hpp>
#include <fstream>
#include <string>


NS_COMMON_BEGIN

enum log_level_t
{
    LOG_LEVEL_TRACE = 0,
    LOG_LEVEL_DEBUG = 1,
    LOG_LEVEL_INFO  = 2,
    LOG_LEVEL_WARN  = 3,
    LOG_LEVEL_ERROR = 4,
	LOG_LEVEL_COMBAT = 5,
};

enum log_control_t
{
    LOG_CONTROL_PRINT_CONSOLE		= 0,
    LOG_CONTROL_WRITE_FILE			= 1,
    LOG_CONTROL_THREAD				= 2,
    LOG_CONTROL_FILEINFO			= 3,

    LOG_CONTROL_WRITE_TRACE			= 4,
    LOG_CONTROL_WRITE_DEBUG			= 5,
    LOG_CONTROL_WRITE_INFO			= 6,

	LOG_CONTROL_NEW_FILE_PER_HOUR	= 7,
};

class log_t
{
public:
    log_t();
    ~log_t();

public:
    bool open(const std::string& log_path, const std::string& log_prefix, bool write_trace, bool write_debug, bool write_info, bool new_file_per_hour = false);
    void close();
    void write_log(log_level_t level, const char* file, int line, const char* func, const char* format, ...);

private:
    bool th_ofs_open(const time_t& now_time);
    void th_ofs_close();
    void th_write_log_to_console(log_level_t level, const std::string& msg);
    void th_write_log_to_file(const std::string& msg);

private:
    std::string get_log_string(log_level_t level, const time_t& now_time, const char* file, int line, const char* func, const char* context);
    bool need_new_file(const time_t& now_time);
    std::string get_log_file_name();
    void set_ctrl(log_control_t ctrl);
    void unset_ctrl(log_control_t ctrl);

private:
    boost::asio::io_service m_io_service;
    boost::asio::io_service::strand m_strand;
    boost::scoped_ptr<boost::asio::io_service::work> m_work;
    boost::scoped_ptr<boost::thread> m_thread;
    std::string m_log_path;
    std::string m_log_prefix;
    std::ofstream m_ofs;
    time_t m_last_write_time;
    bit_array m_ctrl_mask;
};


NS_COMMON_END

extern common::log_t g_log;

#define log_error(format, ...)  g_log.write_log(common::LOG_LEVEL_ERROR, __FILE__, __LINE__, __FUNCTION__, format, ##__VA_ARGS__)
#define log_warn(format, ...)  g_log.write_log(common::LOG_LEVEL_WARN, __FILE__, __LINE__, __FUNCTION__, format, ##__VA_ARGS__)
#define log_info(format, ...)  g_log.write_log(common::LOG_LEVEL_INFO, __FILE__, __LINE__, __FUNCTION__, format, ##__VA_ARGS__)
#define log_trace(format, ...)  g_log.write_log(common::LOG_LEVEL_TRACE, __FILE__, __LINE__, __FUNCTION__, format, ##__VA_ARGS__)
#define log_debug(format, ...)  g_log.write_log(common::LOG_LEVEL_DEBUG, __FILE__, __LINE__, __FUNCTION__, format, ##__VA_ARGS__)
#define log_combat(format, ...)  g_log.write_log(common::LOG_LEVEL_COMBAT, __FILE__, __LINE__, __FUNCTION__, format, ##__VA_ARGS__)
#endif //__COMMON_LOG_HPP__
