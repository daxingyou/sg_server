#include "redis_client.hpp"
#include <boost/asio/deadline_timer.hpp>
#include <boost/system/error_code.hpp>
#include "server_base.hpp"

redis_cli_t redis_client_t::m_redis_cli;

bool redis_client_t::init(boost::asio::io_service& ios,
	const char* host,
	int32_t port,
	const char* passwd,
	int32_t db,
	const sql_table *db_table,
	uint32_t overtime)
{
	if (!m_redis_cli.init(host, port, passwd, db, db_table)) {
		return false;
	}

	uint32_t conn_overtime = common::time_util_t::now_time() + overtime;
	bool is_success = false;
	do {
		if (get_redis_state() == redis_server_state::redis_server_runing) {
			is_success = true;
			break;
		}

		log_warn("redis-cli waiting for redis-srv start!");
		boost::asio::deadline_timer timer(ios);
		timer.expires_from_now(boost::posix_time::seconds(5));
		boost::system::error_code ignored_ec;
		timer.wait(ignored_ec);

		if (common::server_base_t::serverInstance()->is_terminate()) {
			log_warn("redis-client init stop by server is terminate");
			return false;
		}
	} while (common::time_util_t::now_time() < conn_overtime);

	if (!is_success) {
		log_error("redis-cli waiting for redis-srv start time out!");
		return false;
	}

	log_info("redis-cli init success!");

	return true;
}

void redis_client_t::close()
{
	m_redis_cli.close();
}

void redis_client_t::del_set_all(const char* table)
{
	m_redis_cli.del_set_all(table);
}

uint32_t redis_client_t::get_redis_state()
{
	return m_redis_cli.get_string<uint32_t>(REDIS_DB_STATE.c_str());
}
