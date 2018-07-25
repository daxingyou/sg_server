#ifndef _COMMON_ZDB_EX_HPP_
#define _COMMON_ZDB_EX_HPP_

#include <zdb.h>
#include <boost/tuple/tuple.hpp>
#include "log.hpp"

NS_COMMON_BEGIN

#ifndef SQL_DEFAULT_INIT_CONNECTIONS
#define SQL_DEFAULT_INIT_CONNECTIONS 5
#endif

struct zdb_util_t
{
    static boost::tuple<URL_T, ConnectionPool_T> open_zdb(const std::string host, const std::string db, const std::string user, const std::string pwd, uint32_t max_conn, uint32_t init_conn)
    {
        URL_T url = NULL;
        ConnectionPool_T pool = NULL;

        char buf[256] = {0};
        snprintf(buf, sizeof(buf), "mysql://%s/%s?user=%s&password=%s&charset=utf8", host.c_str(), db.c_str(), user.c_str(), pwd.c_str());
        url = URL_new(buf);
        if (NULL == url)
        {
            log_error("NULL == url: %s", buf);
            return boost::make_tuple(url, pool);
        }

        pool = ConnectionPool_new(url);
        if (NULL == pool)
        {
            log_error("ConnectionPool is NULL!");
            URL_free(&url);
            return boost::make_tuple(url, pool);
        }

		// max_conn 
		if (max_conn < SQL_DEFAULT_INIT_CONNECTIONS) {
			max_conn = SQL_DEFAULT_INIT_CONNECTIONS;
		}

        ConnectionPool_setMaxConnections(pool, max_conn);
        ConnectionPool_setInitialConnections(pool, init_conn);
        ConnectionPool_start(pool);

        log_info("init zdb connection[max=%d, init=%d] pool[%s] success!",
            ConnectionPool_getMaxConnections(pool),
            ConnectionPool_getInitialConnections(pool),
            buf);

        return boost::make_tuple(url, pool);
    }

    static void close_zdb(ConnectionPool_T pool, URL_T url)
    {
        if (NULL != pool)
        {
            ConnectionPool_stop(pool);
            ConnectionPool_free(&(pool));
        }

        if (NULL != url)
        {
            URL_free(&url);
        }

        log_info("close zdb success!");
    }

    static Connection_T get_zdb_conn(ConnectionPool_T pool)
    {
        Connection_T conn = NULL;
        TRY
        {
            conn = ConnectionPool_getConnection(pool);
        }
        CATCH(SQLException)
        {
            log_error("get zdb connection error[%d/%d]: %s", ConnectionPool_active(pool), ConnectionPool_size(pool), Exception_frame.message);
            return NULL;
        }
        FINALLY
        {
        }
        END_TRY;
        return conn;
    }
};

NS_COMMON_END

#endif