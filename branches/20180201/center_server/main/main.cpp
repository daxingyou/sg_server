#include "center_server.hpp"
#include "center_server_fwd.hpp"

center_server_t* env::server = NULL;
common::center_xml_t* env::xml = NULL;
ConnectionPool_S* env::conn_zdb_pool = NULL;

USING_NS_NETWORK;
USING_NS_COMMON;

static void handle_signal(int32_t signal)
{
    log_trace("recv signal[%d]", signal);
    if (signal == SIGINT || signal == SIGTERM || signal == SIGQUIT)
    {
        log_info("center server stopping...");
        env::server->close_server();
        google::protobuf::ShutdownProtobufLibrary();
    }
}

int main(int argc, char** argv)
{
	srand((unsigned)time(NULL));

    center_server_t server;
    env::server = &server;

    signal(SIGHUP, SIG_IGN);
    signal(SIGPIPE, SIG_IGN);

    boost::asio::signal_set signals(env::server->get_ios());
    signals.add(SIGINT);
    signals.add(SIGTERM);
    signals.add(SIGQUIT);
    signals.async_wait(boost::bind(handle_signal, boost::asio::placeholders::signal_number));


    server.run_server();

    printf("center server stop!\n");
    return 0;
}
