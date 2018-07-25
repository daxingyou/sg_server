#include "gate_server.hpp"
#include "gate_server_fwd.hpp"
#include "user/user_manager.hpp"

gate_server_t* env::server = NULL;
boost::asio::signal_set* env::signals = NULL;
common::gate_xml_t* env::xml = NULL;

USING_NS_NETWORK;
USING_NS_COMMON;

static void handle_signal(int32_t signal)
{
    log_trace("recv signal[%d]", signal);
    if (signal == SIGINT || signal == SIGTERM || signal == SIGQUIT)
    {
        log_info("gate server stopping...");
        env::server->close_server();
        google::protobuf::ShutdownProtobufLibrary();
    }
}

int main(int argc, char** argv)
{
    uint32_t gate_id = 0;
    if (argc > 2)
    {
        printf("\033[31margc[%d] > 2\033[0m\n", argc);
        return 1;
    }
    else if (argc == 2)
    {
        gate_id = atoi(argv[1]);
    }

    gate_server_t server;
    env::server = &server;
    env::server->set_id(gate_id);

    signal(SIGHUP, SIG_IGN);
    signal(SIGPIPE, SIG_IGN);

    boost::asio::signal_set signals(env::server->get_ios());
    signals.add(SIGINT);
    signals.add(SIGTERM);
    signals.add(SIGQUIT);
    signals.async_wait(boost::bind(handle_signal, boost::asio::placeholders::signal_number));
    env::signals = &signals;

    server.run_server();

    printf("gate server stop!\n");
    return 0;
}
