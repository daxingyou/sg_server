#include "server_base.hpp"
#include "log.hpp"


NS_COMMON_BEGIN

server_base_t::server_base_t()
{

}

server_base_t::~server_base_t()
{
}

bool server_base_t::run_server()
{
    if (!init_server())
    {
        return false;
    }

    on_run_server();

#if defined(COMMON_PLATFORM_WIN32)
    while (true)
    {
        m_ios.run();
    }
#elif defined(COMMON_PLATFORM_LINUX)
    m_work.reset(new boost::asio::io_service::work(m_ios));
    m_ios.run();
#endif
    return true;
}

bool server_base_t::init_server()
{
	// 初始化时区
	const std::string& tz = common::time_util_t::get_local_tz();
	//log_info("main thread io_service start, zt[%s]!", tz.c_str());

    if (!on_init_server())
    {
        return false;
    }

    return true;
}

bool server_base_t::close_server()
{
    m_ios.stop();
    m_work.reset();

    if (!on_close_server())
    {
        return false;
    }

    return true;
}

NS_COMMON_END