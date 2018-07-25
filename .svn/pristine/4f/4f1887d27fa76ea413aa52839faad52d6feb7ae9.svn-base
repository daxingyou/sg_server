#include <signal.h>
#include <google/protobuf/io/coded_stream.h> 
#include <google/protobuf/io/zero_copy_stream_impl.h>

#include "server_base.hpp"
#include "log.hpp"

NS_COMMON_BEGIN

server_base_t* server_base_t::m_server = NULL;

static void handle_signal(int32_t signal)
{
	server_base_t* instance = server_base_t::serverInstance();
	if (instance && !instance->is_terminate()) {
		instance->terminate();
	}
}

server_base_t::server_base_t()
{
	m_server = this;

	m_terminate = false;
}

server_base_t::~server_base_t()
{
	m_server = NULL;
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
	//设置信号处理
	struct sigaction sig;

	sig.sa_handler = handle_signal;
	sigemptyset(&sig.sa_mask);
	sig.sa_flags = 0;
	sigaction(SIGINT, &sig, NULL);
	sigaction(SIGQUIT, &sig, NULL);
	//sigaction(SIGABRT, &sig, NULL);
	sigaction(SIGTERM, &sig, NULL);
	sig.sa_handler = SIG_IGN;
	sigaction(SIGHUP, &sig, NULL);
	sigaction(SIGPIPE, &sig, NULL);

	// 初始化随机数种子
	srand((unsigned)time(NULL));

	// 初始化时区
	const std::string& tz = common::time_util_t::get_local_tz();
	log_info("main thread io_service start, zt[%s]!", tz.c_str());

    if (!on_init_server())
    {
        return false;
    }

    return true;
}

bool server_base_t::close_server()
{
    if (!on_close_server())
    {
        return false;
    }

	m_work.reset();
	m_ios.stop();

    return true;
}

void server_base_t::terminate()
{
	if (check_terminate()) {
		m_terminate = true;

		m_ios.post([this]() {
			log_info("%s server stopping...", server_name());
			close_server();
			google::protobuf::ShutdownProtobufLibrary();
		});
	}
}

NS_COMMON_END