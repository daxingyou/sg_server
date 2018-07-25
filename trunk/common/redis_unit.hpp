#pragma once

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/asio/strand.hpp>
#include <boost/enable_shared_from_this.hpp>

#include "redis.hpp"
#include "zdb_ex.hpp"

class redis_unit_t : public redis_base_t
{
public:
	 explicit redis_unit_t(boost::asio::io_service& ios);
	 virtual ~redis_unit_t();

	 // 初始化
	 virtual bool init(const char* host, int32_t port, const char* passwd, int32_t db, struct ConnectionPool_S* zdb_pool);

	 // 得到zdb_pool
	 struct ConnectionPool_S* get_zdb_pool() const { return m_zdb_pool; }

protected:
	boost::asio::io_service& m_ios;
	boost::asio::io_service::strand m_strand;
	struct ConnectionPool_S* m_zdb_pool = NULL; // zdb连接
};

