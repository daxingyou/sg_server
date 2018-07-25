#include "login_server.hpp"
#include "login_server_fwd.hpp"
#include "user/user_manager.hpp"

login_server_t* env::server = NULL;
boost::asio::signal_set* env::signals = NULL;
common::login_xml_t* env::xml = NULL;
ConnectionPool_S* env::conn_zdb_pool = NULL;

USING_NS_NETWORK;
USING_NS_COMMON;

static void handle_signal(int32_t signal)
{
	log_trace("recv signal[%d]", signal);
	if (signal == SIGINT || signal == SIGTERM || signal == SIGQUIT)
	{
		log_info("login server stopping...");

		env::server->close_server();
		google::protobuf::ShutdownProtobufLibrary();
	}
}

int main(int argc, char** argv)
{
    login_server_t server;
    env::server = &server;

    signal(SIGHUP, SIG_IGN);
    signal(SIGPIPE, SIG_IGN);

    boost::asio::signal_set signals(env::server->get_ios());
    signals.add(SIGINT);
    signals.add(SIGTERM);
    signals.add(SIGQUIT);
    signals.async_wait(boost::bind(handle_signal, boost::asio::placeholders::signal_number));
    env::signals = &signals;

    server.run_server();

    printf("login server stop!\n");
    return 0;
}
