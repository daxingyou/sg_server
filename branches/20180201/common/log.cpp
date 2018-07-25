#include "log.hpp"
#include <stdarg.h>
#include <time.h>
#ifdef COMMON_PLATFORM_LINUX
#include <sys/syscall.h>
#endif

NS_COMMON_BEGIN

static bool is_same_day(time_t t1, time_t t2)
{
//     struct tm *ptm = localtime(&t1);
//     int year1 = ptm->tm_year;
//     int mon1 = ptm->tm_mon;
//     int day1 = ptm->tm_mday;
//     ptm = localtime(&t2);
//     int year2 = ptm->tm_year;
//     int mon2 = ptm->tm_mon;
//     int day2 = ptm->tm_mday;
//     if (year1 == year2 && mon1 == mon2 && day1 == day2)
//         return true;
//     return false;

	if (common::time_util_t::is_same_day(t1, t2))
		return true;

	return false;
}

static bool is_same_hour(time_t t1, time_t t2)
{
//     struct tm *ptm = localtime(&t1);
//     int year1 = ptm->tm_year;
//     int mon1 = ptm->tm_mon;
//     int day1 = ptm->tm_mday;
//     int hour1 = ptm->tm_hour;
//     ptm = localtime(&t2);
//     int year2 = ptm->tm_year;
//     int mon2 = ptm->tm_mon;
//     int day2 = ptm->tm_mday;
//     int hour2 = ptm->tm_hour;
//     if (year1 == year2 && mon1 == mon2 && day1 == day2 && hour1 == hour2)
//         return true;
//     return false;

	if (t1 / 3600 == t2 / 3600)
		return true;

	return false;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
log_t::log_t()
    : m_last_write_time(0)
    , m_strand(m_io_service)
{
    set_ctrl(LOG_CONTROL_PRINT_CONSOLE);
    set_ctrl(LOG_CONTROL_WRITE_FILE);
    set_ctrl(LOG_CONTROL_THREAD);
    set_ctrl(LOG_CONTROL_FILEINFO);
    set_ctrl(LOG_CONTROL_WRITE_TRACE);
    set_ctrl(LOG_CONTROL_WRITE_DEBUG);
    set_ctrl(LOG_CONTROL_WRITE_INFO);
}

log_t::~log_t()
{

}

bool log_t::open(const std::string& log_path, const std::string& log_prefix, bool write_trace, bool write_debug, bool write_info, bool new_file_per_hour)
{
    if (log_path.empty())
    {
        printf("\033[31mopen log file failed: log_path empty!\033[0m\n");
        return false;
    }
    if (log_prefix.empty())
    {
        printf("\033[31mopen log file failed: log_prefix empty!\033[0m\n");
        return false;
    }

    if (!write_trace)
    {
        unset_ctrl(LOG_CONTROL_WRITE_TRACE);
    }
    if (!write_debug)
    {
        unset_ctrl(LOG_CONTROL_WRITE_DEBUG);
    }
    if (!write_info)
    {
        unset_ctrl(LOG_CONTROL_WRITE_INFO);
    }
    if (new_file_per_hour)
    {
        set_ctrl(LOG_CONTROL_NEW_FILE_PER_HOUR);
    }

    m_log_path = log_path;
    m_log_prefix = log_prefix;
    time_t now_time = time(NULL);

    m_work.reset(new boost::asio::io_service::work(m_io_service));
    m_thread.reset(new boost::thread(boost::bind(&boost::asio::io_service::run, &m_io_service)));
    m_io_service.post(boost::bind(&log_t::th_ofs_open, this, now_time));

    log_info("init log[error=1 warn=1 trace=%d info=%d debug=%d] success!", write_trace, write_info, write_debug);

    return true;
}

void log_t::close()
{
    m_io_service.post(boost::bind(&log_t::th_ofs_close, this));

    m_ctrl_mask.reset();

    m_io_service.stop();
    m_work.reset();
    if (m_thread)
    {
        m_thread->join();
        m_thread.reset();
    }

    printf("close log success!\n");
}

void log_t::write_log(log_level_t level, const char* file, int line, const char* func, const char* format, ...)
{
    time_t now_time = time(NULL);

    char str_tmp[1024] = { 0 };
    va_list va;
    va_start(va, format);
    vsprintf(str_tmp, format, va);
    va_end(va);
    std::string context(str_tmp);
    if (context.empty())
    {
        return;
    }

    std::string log = get_log_string(level, now_time, file, line, func, context.c_str());
    if (log.empty())
    {
        return;
    }

    // write to file
    if (m_ctrl_mask.is_bit_set(LOG_CONTROL_WRITE_FILE))
    {
        if (need_new_file(now_time))
        {
            m_io_service.post(boost::bind(&log_t::th_ofs_open, this, now_time));
        }
        m_io_service.post(boost::bind(&log_t::th_write_log_to_file, this, log));
    }

    // print to console
    if (m_ctrl_mask.is_bit_set(LOG_CONTROL_PRINT_CONSOLE))
    {
        m_io_service.post(boost::bind(&log_t::th_write_log_to_console, this, level, log));
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool log_t::th_ofs_open(const time_t& now_time)
{
    th_ofs_close();

    std::string file_path = m_log_path;
    std::string file_name = get_log_file_name();
    std::string tail = "/";

    if (!(file_path.compare(file_path.size() - tail.size(), tail.size(), tail) == 0))
    {
        file_path += "/";
    }
    file_path += m_log_prefix;
    file_path += file_name;

    bool rlt = false;
    do 
    {
        m_ofs.open(file_path.c_str(), std::ofstream::app);
        if (!m_ofs.is_open())
        {
            printf("\033[31mcreate file[%s] failed!\033[0m\n", file_path.c_str());
            break;
        }

        m_last_write_time = now_time;
        rlt = true;

    } while (0);
    
    if (!rlt)
    {
        COMMON_ASSERT(false);
    }

    return true;
}

void log_t::th_ofs_close()
{
    if (m_ofs.is_open())
    {
        m_ofs.close();
    }
}

void log_t::th_write_log_to_file(const std::string& msg)
{
    if (!m_ofs.is_open())
    {
        return;
    }
    m_ofs << msg << "\n";
    m_ofs.flush();
}

void log_t::th_write_log_to_console(log_level_t level, const std::string& msg)
{
#ifdef COMMON_PLATFORM_WIN32
    HANDLE hStdOutputHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    if (level == LOG_LEVEL_ERROR)
        SetConsoleTextAttribute(hStdOutputHandle, FOREGROUND_RED | FOREGROUND_INTENSITY);
    if (level == LOG_LEVEL_INFO)
        SetConsoleTextAttribute(hStdOutputHandle, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    if (level == LOG_LEVEL_TRACE)
        SetConsoleTextAttribute(hStdOutputHandle, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    if (level == LOG_LEVEL_WARN)
        SetConsoleTextAttribute(hStdOutputHandle, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    else
        SetConsoleTextAttribute(hStdOutputHandle, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    std::cout << msg;
    SetConsoleTextAttribute(hStdOutputHandle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
#elif defined(COMMON_PLATFORM_LINUX)
    std::cout << "\033[0m";
    if (level == LOG_LEVEL_ERROR)
        std::cout << "\033[31m";
    else if (level == LOG_LEVEL_INFO)
        std::cout << "\033[33m";
    else if (level == LOG_LEVEL_DEBUG)
        std::cout << "\033[37m";
    else if (level == LOG_LEVEL_WARN)
        std::cout << "\033[35m";
    else if (level == LOG_LEVEL_COMBAT)
        std::cout << "\033[36m";
    std::cout << msg;
    std::cout << "\033[0m\n";
#endif
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
std::string log_t::get_log_string(log_level_t level, const time_t& now_time, const char* file, int line, const char* func, const char* context)
{
    if (LOG_LEVEL_TRACE == level && !m_ctrl_mask.is_bit_set(LOG_CONTROL_WRITE_TRACE))
    {
        return ""; //skip trace log
    }
    else if (LOG_LEVEL_DEBUG == level && !m_ctrl_mask.is_bit_set(LOG_CONTROL_WRITE_DEBUG))
    {
        return ""; //skip debug log
    }
    else if (LOG_LEVEL_INFO == level && !m_ctrl_mask.is_bit_set(LOG_CONTROL_WRITE_INFO))
    {
        return ""; //skip info log
    }

    // time
    struct tm *ptm = localtime(&now_time);
    char timestamp[128] = { 0 };
    strftime(timestamp, 128, "%H:%M:%S ", ptm);

    // level
    char levelstamp[128] = { 0 };
    if (level == LOG_LEVEL_ERROR)
    {
        snprintf(levelstamp, sizeof(levelstamp), "[error] ");
    }
    else if (level == LOG_LEVEL_INFO)
    {
        snprintf(levelstamp, sizeof(levelstamp), "[info] ");
    }
    else if (level == LOG_LEVEL_DEBUG)
    {
        snprintf(levelstamp, sizeof(levelstamp), "[debug] ");
    }
    else if (level == LOG_LEVEL_TRACE)
    {
        snprintf(levelstamp, sizeof(levelstamp), "[trace] ");
    }
    else if (level == LOG_LEVEL_WARN)
    {
        snprintf(levelstamp, sizeof(levelstamp), "[warn] ");
    }
    else if (level == LOG_LEVEL_COMBAT)
    {
        snprintf(levelstamp, sizeof(levelstamp), "[combat] ");
    }

    // thread
    char threadstamp[64] = { 0 };
    if (m_ctrl_mask.is_bit_set(LOG_CONTROL_THREAD))
    {
#if defined(COMMON_PLATFORM_WIN32)
        sprint_s(threadstamp, sizeof(threadstamp), "[tid=%lu] ", GetCurrentThreadId());
#elif defined(COMMON_PLATFORM_LINUX)
        snprintf(threadstamp, sizeof(threadstamp), "[tid=%lu] ", syscall(SYS_gettid));
#endif
    }

    // file_info
    char filestamp[256] = { 0 };
    if (m_ctrl_mask.is_bit_set(LOG_CONTROL_FILEINFO))
    {
        snprintf(filestamp, sizeof(filestamp), "[%s:%d:%s] ", file, line, func);
    }

    // context
    char logtstamp[1024] = { 0 };
    snprintf(logtstamp, sizeof(logtstamp), "%s%s%s%s%s", timestamp, levelstamp, threadstamp, filestamp, context);

    std::string log_string(logtstamp);
    return log_string;
}

void log_t::set_ctrl(log_control_t ctrl)
{
    m_ctrl_mask.set_bit(ctrl);
}

void log_t::unset_ctrl(log_control_t ctrl)
{
    m_ctrl_mask.clear_bit(ctrl);
}

bool log_t::need_new_file(const time_t& now_time)
{
	if (m_ctrl_mask.is_bit_set(LOG_CONTROL_NEW_FILE_PER_HOUR))
	{
		return !(is_same_hour(m_last_write_time, now_time));
	}
	else
	{
		return !(is_same_day(m_last_write_time, now_time));
	}
}

std::string log_t::get_log_file_name()
{
	time_t t = time(NULL);
	struct tm *ptm = localtime(&t);
	char timestamp[128];
	if (m_ctrl_mask.is_bit_set(LOG_CONTROL_NEW_FILE_PER_HOUR))
	{
		strftime(timestamp, 128, "%Y%m%d-%H.log", ptm);
	}
	else
	{
		strftime(timestamp, 128, "%Y%m%d.log", ptm);
	}
	
	return std::string(timestamp);
}

NS_COMMON_END

common::log_t g_log;

