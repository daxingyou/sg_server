#include "redis_unit.hpp"
#include "log.hpp"

redis_unit_t::redis_unit_t(boost::asio::io_service& ios)
	: m_ios(ios)
	, m_strand(m_ios)
{

}

redis_unit_t::~redis_unit_t()
{
	close();
}

bool redis_unit_t::init(const char* host, int32_t port, const char* passwd, int32_t db, struct ConnectionPool_S* zdb_pool)
{
	if (NULL == zdb_pool) {
		log_error("NULL == zdb_pool");
		return false;
	}

	m_zdb_pool = zdb_pool;

	if (!redis_base_t::init(host, port, passwd, db)) {
		log_error("redis_base_t connect redis error");
		return false;
	}

	return true;
}
