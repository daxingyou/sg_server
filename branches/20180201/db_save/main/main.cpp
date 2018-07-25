#include "db_save.hpp"
#include "db_save_fwd.hpp"

db_server_t* env::server = NULL;
boost::asio::signal_set* env::signals = NULL;
common::db_xml_t* env::xml = NULL;
ConnectionPool_S* env::conn_game_zdb_pool = NULL;
ConnectionPool_S* env::conn_account_db_pool = NULL;

USING_NS_NETWORK;
USING_NS_COMMON;

static void handle_signal(int32_t signal)
{
    log_trace("recv signal[%d]", signal);
    if (signal == SIGINT || signal == SIGTERM || signal == SIGQUIT)
    {
        log_info("db server stopping...");
        env::server->close_server();
        google::protobuf::ShutdownProtobufLibrary();
    }
}

int main(int argc, char** argv)
{
	uint32_t id = 0;
	std::string db_name;
	if (argc == 3)
	{
		id = atoi(argv[1]);
		db_name = argv[2];
	}
	else
	{
		printf("\033[31m./db_save argv null \033[0m\n");
		return 1;
	}

	db_server_t server(id, db_name);
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

    return 0;
}
