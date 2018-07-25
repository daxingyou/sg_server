#include "redis_client.hpp"
#include "log.hpp"
#include <boost/asio.hpp>
#include "utility.hpp"
USING_NS_COMMON;
static const size_t CACHE_VALUEX_LEN_MA = 1024 * 256; // 256K
redisContext* redis_client_t::m_redis_connect = NULL;
redis_client_t::key_list redis_client_t::m_need_save_to_sql;
uint32_t redis_client_t::m_save_interval = 1;
uint32_t redis_client_t::m_save_count_limit = 500;
boost::shared_ptr<boost::asio::deadline_timer> redis_client_t::m_save_to_sql_timer = NULL;
bool redis_client_t::m_save_cancel = false;
redis_client_t::table_list redis_client_t::m_field_list;
redis_client_t::field_list redis_client_t::const_error_list;

// 读取
redis_read_t g_redis_read;

// 保存
redis_save_t g_redis_save;

class redis_reply_t
{
public:
	redis_reply_t()
		: m_reply_data(NULL)
	{
	}

	~redis_reply_t()
	{
		if (m_reply_data != NULL)
		{
			freeReplyObject(m_reply_data);
			m_reply_data = NULL;
		}
	}

public:
	redisReply* m_reply_data;
};

redis_client_t::redis_client_t()
{
}

redis_client_t::~redis_client_t()
{
}

bool redis_client_t::init(
    const std::string& host,
    int port,
    const char* passwd,
    int db,
	uint32_t work_thread,
    uint32_t save_interval,
    uint32_t save_count_limit,
    struct ConnectionPool_S* zdb_pool,
    const simple_table_field_list& load_tables,
	boost::function<void()> read_Callback /*= NULL*/)
{
    table_field_list list;
    for (auto table : load_tables)
    {
        field_name_list field_name;
        field_name.insert(table.second);
        list.insert(std::make_pair(table.first, field_name));
    }
    return init(host, port, passwd, db, work_thread, save_interval, save_count_limit, zdb_pool, list, read_Callback);
}

bool redis_client_t::init(
    const std::string& host,
    int port,
    const char* passwd,
    int db,
	uint32_t work_thread,
    uint32_t save_interval,
    uint32_t save_count_limit,
    struct ConnectionPool_S* zdb_pool,
    table_field_list& load_tables,
    const simple_table_field_list& load_simple_tables, 
	boost::function<void()> read_Callback /*= NULL*/)
{
    for (auto simple : load_simple_tables)
    {
        field_name_list field_name;
        field_name.insert(simple.second);

        load_tables.insert(std::make_pair(simple.first, field_name));
    }
    return init(host, port, passwd, db, work_thread, save_interval, save_count_limit, zdb_pool, load_tables, read_Callback);
}

bool redis_client_t::init(
	const std::string& host, 
	int port, 
	const char* passwd, 
	int db,
	uint32_t work_thread,
	uint32_t save_interval,
	uint32_t save_count_limit, 
	struct ConnectionPool_S* zdb_pool,
	const table_field_list& load_tables, 
	boost::function<void()> read_Callback /*= NULL*/)
{
	if (!connect(host, port, passwd, db))
	{
		return false;
	}

	if (!g_redis_save.init(host, port, passwd, db, save_interval, save_count_limit, zdb_pool))
	{
		log_error("init redis_save client failed!");
		return false;
	}

	if (read_Callback == NULL) { // 单线程读
		log_info("redis load data from sql...Start");
		for (auto table : load_tables)
		{
			if (!load_from_sql(table.first, table.second, zdb_pool))
			{
				return false;
			}
		}
		log_info("redis load data from sql...Done");
	} else {
		if (!g_redis_read.init(host, port, passwd, db, work_thread, work_thread, zdb_pool, read_Callback))
		{
			log_error("init redis_read client failed!");
			return false;
		}

		g_redis_read.load_all_data(load_tables, m_field_list, zdb_pool);
	}

	m_save_interval = save_interval;
	m_save_count_limit = save_count_limit;
    m_save_cancel = false;
//     m_save_to_sql_timer.reset(new boost::asio::deadline_timer(ios));
//     if (!start_save_to_sql_timer(zdb_pool))
//     {
//         return false;
//     }

    log_info("init redis[ip=%s db=%d] success!", host.c_str(), db);

	return true;
}

void redis_client_t::close(struct ConnectionPool_S* zdb_pool)
{
	//stop_save_to_sql_timer();
	//do_save_all(zdb_pool, true);
	disconnect();

	g_redis_read.close();
	g_redis_save.close(zdb_pool);

    log_info("close redis success!");
}

bool redis_client_t::connect(const std::string& host, int port, const char* passwd, int db)
{
	m_redis_connect = redisConnect(host.c_str(), port);

	if (m_redis_connect == NULL)
	{
		log_error("m_redis_connect == NULL");
		return false;
	}

	if (m_redis_connect->err)
	{
		log_error("redis client connect error: %s", m_redis_connect->errstr);
		return false;
	}

	if (passwd != NULL)
	{
		redis_reply_t auth_reply;
		auth_reply.m_reply_data = (redisReply*)redisCommand(m_redis_connect, "AUTH %s", passwd);
	}

	redis_reply_t select_reply;
	select_reply.m_reply_data = (redisReply*)redisCommand(m_redis_connect, "SELECT %d", db);

	return true;
}

void redis_client_t::disconnect()
{
	if (m_redis_connect != NULL)
	{
		redisFree(m_redis_connect);
		m_redis_connect = NULL;
	}
}

bool redis_client_t::save_to_redis(const std::string& key, const std::string& field, const std::string& value, const bool binary, const bool update_sql)
{
	if (m_redis_connect == NULL)
		return false;

	if (value.length() > CACHE_VALUEX_LEN_MA)
	{
		log_error("redis client set key[%s] value[%s] too long!", key.c_str(), value.c_str());
		//return false;
	}

	redis_reply_t reply;
	//reply.m_reply_data = (redisReply*)redisCommand(m_redis_connect, "SET %s %s", key.c_str(), value.c_str());
	if (field.empty())
	{
		std::string time_str = string_util_t::uint64_to_string(time(NULL));
		reply.m_reply_data = (redisReply*)redisCommand(m_redis_connect, "HSET %s %s %s", key.c_str(), TIME_FIELD.c_str(), time_str.c_str());
	} 
	else
	{
		if (binary)
		{
			reply.m_reply_data = (redisReply*)redisCommand(m_redis_connect, "HSET %s %s %b", key.c_str(), field.c_str(), value.c_str(), value.length());
		}
		else
		{
			reply.m_reply_data = (redisReply*)redisCommand(m_redis_connect, "HSET %s %s %s", key.c_str(), field.c_str(), value.c_str());
		}
	}
	
    if (!field.empty())
    {
        std::vector<std::string> vec_data_key;
        common::string_util_t::split<std::string>(key, vec_data_key, ":");
        if (vec_data_key.size() == 2)
        {
            const std::string& table = vec_data_key[0];
            const std::string& key_data = vec_data_key[1];
            save_field_mapping(table, field, value, key_data);
        }
    }

	if (update_sql)
	{
		save_time_to_redis(key);
		save_to_sql(key);
	}
	
	return true;
}

bool redis_client_t::load_from_redis(const std::string& key, const std::string& field, std::string& result)
{
	if (m_redis_connect == NULL)
		return false;

	redis_reply_t reply;
	//reply.m_reply_data = (redisReply*)redisCommand(m_redis_connect, "GET %s", key.c_str());
	reply.m_reply_data = (redisReply*)redisCommand(m_redis_connect, "HGET %s %s", key.c_str(), field.c_str());
	
	if (reply.m_reply_data == NULL)
	{
		log_error("redis client do_load key[%s] replay is null!", key.c_str());
		return false;
	}

	if (reply.m_reply_data->type != REDIS_REPLY_STRING)
	{
		//log_error("redis client do_load key[%s] field[%s] reply_type[%d] replay error!", key.c_str(), field.c_str(), reply.m_reply_data->type);
		return false;
	}

	result.assign(reply.m_reply_data->str, reply.m_reply_data->len);
	return true;
}

time_t redis_client_t::get_time_from_redis(const std::string& data_key)
{
	std::string res;
	if (!load_from_redis(data_key, TIME_FIELD, res))
		return 0;

	return boost::lexical_cast<time_t>(res);
}

bool redis_client_t::save_time_to_redis(const std::string& data_key)
{
	std::string time_str = string_util_t::uint64_to_string(time(NULL));
	return save_to_redis(data_key, TIME_FIELD, time_str, false, false);
}

bool redis_client_t::remove_from_redis(const std::string& key, const std::string& field)
{
	if (m_redis_connect == NULL)
		return false;

	std::string data;
	load_from_redis(key, field, data);

	redis_reply_t reply;
	if (field.empty())
	{
		reply.m_reply_data = (redisReply*)redisCommand(m_redis_connect, "DEL %s", key.c_str());
	}
	else
	{
		reply.m_reply_data = (redisReply*)redisCommand(m_redis_connect, "HDEL %s %s", key.c_str(), field.c_str());
	}
	
	save_to_sql(key);

	std::vector<std::string> vec_data_key;
	common::string_util_t::split<std::string>(key, vec_data_key, ":");
	if (vec_data_key.size() == 2)
	{
		const std::string& table = vec_data_key[0];
		const std::string& key_data = vec_data_key[1];
        del_field_mapping(table, field, data, key_data);
	}

	return true;
}

bool redis_client_t::do_save_single(const Connection_T& conn, const std::string& data_key)
{
	std::vector<std::string> vec_data_key;
	common::string_util_t::split<std::string>(data_key, vec_data_key, ":");
	if (vec_data_key.size() != 2)
	{
		log_error("redis client do_save_to_sql data_key[%s] error", data_key.c_str());
		return false;
	}

	const std::string& table = vec_data_key[0];
	const std::string& key_data = vec_data_key[1];
	bool ret = true;
	std::vector<std::string> keys;
	keys.push_back(KEY_FIELD);
	if (get_field_from_redis(data_key, keys))
	{
		std::ostringstream cols;
		std::map<std::string, std::string> values;
		for (auto field : keys)
		{
			if (!is_field_exist(table, field))
			{
				continue;
			}

			if (!cols.str().empty())
			{
				cols << ",";
			}
			
			cols << "`" << field << "` = ?";

			if (field == KEY_FIELD)
			{
				values[field] = key_data;
			}
			else
			{
				std::string str_data;
				load_from_redis(data_key, field, str_data);
				values[field] = str_data;
			}
		}

		std::ostringstream sql;
		sql << "REPLACE INTO `" << table << "` SET " << cols.str() << ";";
		TRY
		{
			PreparedStatement_T pre = Connection_prepareStatement(conn, sql.str().c_str());
			int index = 1;
			for (auto field : keys)
			{
				if (!is_field_exist(table, field))
				{
					continue;
				}

				if (is_protobuf(table, field))
				{
					PreparedStatement_setBlob(pre, index, values[field].c_str(), values[field].length());
				}
				else if (is_string(table, field))
				{
					PreparedStatement_setString(pre, index, values[field].c_str());
				}
				else if (is_uint64(table, field))
				{
					PreparedStatement_setLLong(pre, index, string_util_t::string_to_uint64(values[field]));
				}
				else if (is_uint32(table, field))
				{
					PreparedStatement_setInt(pre, index, boost::lexical_cast<uint32_t>(values[field]));
				}
				else if (is_float(table, field))
				{
					PreparedStatement_setDouble(pre, index, boost::lexical_cast<float>(values[field]));
				}
				
				++index;
			}

			PreparedStatement_execute(pre);
		}

		CATCH(SQLException)
		{
			log_error("redis client do_save_to_sql [%s] sql[%s] update error: %s", data_key.c_str(), sql.str().c_str(),Exception_frame.message);
			ret = false;
		}
		FINALLY
		{
			Connection_close(conn);
		}
		END_TRY;
	}
	else
	{
		log_error("redis client not find [%s]", data_key.c_str());
		Connection_close(conn);
		ret = false;
	}

	return ret;
}

bool redis_client_t::do_save_all(struct ConnectionPool_S* zdb_pool, bool is_final)
{
    if (NULL == zdb_pool)
    {
        log_error("NULL == zdb_pool");
        return false;
    }

	if (m_need_save_to_sql.empty())
    {
        return false;
    }

	Connection_T conn = common::zdb_util_t::get_zdb_conn(zdb_pool);
	if (NULL == conn)
	{
		log_error("redis client save_to_sql_timer_callback conn is null");
		return false;
	}

	uint32_t count = 0;
	key_list need_del;
	for (auto data_key : m_need_save_to_sql)
	{
		if (!do_save_single(conn, data_key))
			continue;

		need_del.insert(data_key);
		++count;

		if (!is_final)
		{
			if (count >= m_save_count_limit)
				break;
		}
	}

	for (auto data_key : need_del)
	{
		m_need_save_to_sql.erase(data_key);
	}

	return true;
}

void redis_client_t::save_to_sql_timer_callback(const boost::system::error_code& ec, struct ConnectionPool_S* zdb_pool)
{
    if (ec == boost::asio::error::operation_aborted)
    {
        
        return;
    }
    else if (ec)
    {
        log_error("Timer error: %s", ec.message().c_str());
        return;
    }
    if (m_save_cancel)
    {
        log_error("Timer was canceled");
        return;
    }

    do_save_all(zdb_pool, false);
    start_save_to_sql_timer(zdb_pool);
}

void redis_client_t::save_to_sql(const std::string& data_key)
{
	// m_need_save_to_sql.insert(data_key);
	g_redis_save.save_to_sql(data_key);
}

bool redis_client_t::start_save_to_sql_timer(struct ConnectionPool_S* zdb_pool)
{
    if (m_save_cancel)
    {
        
        return false;
    }

    if (0 == m_save_interval)
    {
        log_error("m_save_interval is 0 !");
        return false;
    }

    if (NULL == m_save_to_sql_timer)
    {
        log_error("NULL == m_save_to_sql_timer");
        return false;
    }

	m_save_to_sql_timer->expires_from_now(boost::posix_time::seconds(m_save_interval));
	m_save_to_sql_timer->async_wait(boost::bind(&redis_client_t::save_to_sql_timer_callback, boost::asio::placeholders::error, zdb_pool));

    return true;
}

void redis_client_t::stop_save_to_sql_timer()
{
	if (NULL == m_save_to_sql_timer)
	{
		return;
	}
    m_save_cancel = true;
	m_save_to_sql_timer->cancel();
}

bool redis_client_t::get_field_from_redis(const std::string& data_key, std::vector<std::string>& ret)
{
	if (m_redis_connect == NULL)
		return false;

	redis_reply_t reply;
	reply.m_reply_data = (redisReply*)redisCommand(m_redis_connect, "HKEYS %s", data_key.c_str());

	if (reply.m_reply_data == NULL)
		return false;

	if (reply.m_reply_data->type != REDIS_REPLY_ARRAY)
		return false;

	for (size_t i = 0; i < reply.m_reply_data->elements; ++i)
	{
		redisReply* elem = reply.m_reply_data->element[i];
		if (NULL != elem)
		{
			ret.push_back(std::string(elem->str));
		}
	}

	return true;
}

bool redis_client_t::load_from_sql(const std::string& table, const field_name_list& mapping_field, struct ConnectionPool_S* zdb_pool)
{
    uint32_t rows = 0;
    uint32_t field_data_size = 0;
    uint32_t time1 = time(NULL);

	Connection_T conn = common::zdb_util_t::get_zdb_conn(zdb_pool);
	if (NULL == conn)
	{
		log_error("redis client load_from_sql conn is null");
		return false;
	}

	bool ret = true;
	TRY
	{
		std::ostringstream sql;
		sql << "SHOW COLUMNS FROM `" << table << "`;";
		ResultSet_T result = Connection_executeQuery(conn, sql.str().c_str());

		while (ResultSet_next(result))
		{
			std::string field_name = ResultSet_getString(result, 1);
			std::string field_type = ResultSet_getString(result, 2);
			m_field_list[table].insert(std::make_pair(field_name, field_data(field_type, ((mapping_field.find(field_name) != mapping_field.end()) ? true : false))));
		}
	}
	CATCH(SQLException)
	{
		log_error("redis client load_from_sql [%s] error: %s", table.c_str(), Exception_frame.message);
		ret = false;
	}
	FINALLY
	{
	}
	END_TRY;

    if (!ret)
    {
        Connection_close(conn);
        return false;
    }

	const field_list& cols = m_field_list[table];
	if (cols.empty())
	{
        log_error("cols is empty!");
        Connection_close(conn);
        return false;
	}

	std::string str_col;
	for (auto column : cols)
	{
		if (!str_col.empty())
		{
			str_col += ",";
		}

		str_col += "`";
		str_col += column.first;
		str_col += "`";
	}

	TRY
	{
		ResultSet_T result = Connection_executeQuery(conn, "SELECT %s FROM %s", str_col.c_str(), table.c_str());
		while (ResultSet_next(result))
        {
            rows += 1;

			std::map<std::string, std::string> field_data;

            /*std::string rk_data("");
            if (is_string(table, KEY_FIELD))
            {
                const char* str = (const char*)ResultSet_getStringByName(result, KEY_FIELD.c_str());
                if (str != NULL)
                    rk_data.assign(str);
            }
            else if (is_uint64(table, KEY_FIELD))
            {
                rk_data = string_util_t::uint64_to_string(ResultSet_getLLongByName(result, KEY_FIELD.c_str()));
            }
            else if (is_uint32(table, KEY_FIELD))
            {
                rk_data = string_util_t::uint64_to_string(ResultSet_getIntByName(result, KEY_FIELD.c_str()));
            }
            else
            {
                log_error("table[%s] column[%s] error!", table.c_str(), KEY_FIELD.c_str());
                COMMON_ASSERT(false);
                return false;
            }
            field_data[KEY_FIELD] = rk_data;*/

			int index = 1;
			for (auto column : cols)
			{
     //           if (column.first == KEY_FIELD)
     //           {
					//// 不累加数据全部错乱
					//index += 1;
     //               continue;
     //           }

				std::string str_data("");
				if (is_protobuf(table, column.first))
				{
					int size = 0;
					const char* blob = (const char*)ResultSet_getBlob(result, index, &size);
					if(blob != NULL)
						str_data.assign(blob, size);
				}
				else if (is_string(table, column.first))
				{
					const char* str = (const char*)ResultSet_getString(result, index);
					if (str != NULL)
						str_data.assign(str);
				}
				else if (is_uint64(table, column.first))
				{
					str_data = string_util_t::uint64_to_string(ResultSet_getLLong(result, index));
				}
				else if (is_uint32(table, column.first))
				{
					str_data = string_util_t::uint64_to_string(ResultSet_getInt(result, index));
				}
				else if (is_float(table, column.first))
				{
					str_data = string_util_t::uint64_to_string(ResultSet_getDouble(result, index));
				}
				
				index += 1;
				field_data[column.first] = str_data;

                ///////////////////item | role_uid | 72057641282568531 | 72450952392474669
                //save_field_mapping(table, column.first, str_data, field_data[KEY_FIELD]);
			}

            std::string data_key = gen_data_key(table, field_data[KEY_FIELD]);
            time_t redis_data_time = get_time_from_redis(data_key);
            time_t sql_data_time = boost::lexical_cast<time_t>(field_data[TIME_FIELD]);
            if (redis_data_time == 0 || sql_data_time > redis_data_time)
            {
                for (auto sql_data : field_data)
                {
                    if (sql_data.first == KEY_FIELD)
                        continue;

                    if (is_protobuf(table, sql_data.first))
                    {
                        save_to_redis(data_key, sql_data.first, sql_data.second, true, false);
                    }
                    else
                    {
                        save_to_redis(data_key, sql_data.first, sql_data.second, false, false);
                    }
                }
            }
            else
            {
                for (auto sql_data : field_data)
                {
                    if (sql_data.first == KEY_FIELD)
                        continue;

                    save_field_mapping(table, sql_data.first, sql_data.second, field_data[KEY_FIELD]);
                }
            }

            field_data_size = field_data.size();
		}
        
	}
	CATCH(SQLException)
	{
		log_error("redis client load_from_sql [%s] data error: %s", table.c_str(), Exception_frame.message);
		ret = false;
	}
	FINALLY
	{
		Connection_close(conn);
	}
	END_TRY;

    uint32_t time2 = time(NULL);
    log_debug("%d seconds, %d loops(%d rows, %d columns, %d fields): %s", time2 - time1, rows*(cols.size()+ field_data_size), rows, cols.size(), field_data_size, table.c_str());

	return true;
}

bool redis_client_t::get_field_type(const std::string& table, const std::string& field, std::string& result)
{
	if (m_field_list.find(table) == m_field_list.end())
		return false;

	field_list::iterator it = m_field_list[table].find(field);
	if (it == m_field_list[table].end())
		return false;

	result = it->second.field_type;
	return true;
}

bool redis_client_t::is_protobuf(const std::string& table, const std::string& field)
{
	std::string field_type;
	if (!get_field_type(table, field, field_type))
		return false;

	return field_type.find("blob") != std::string::npos;
}

bool redis_client_t::is_string(const std::string& table, const std::string& field)
{
	std::string field_type;
	if (!get_field_type(table, field, field_type))
		return false;

	return field_type.find("varchar") != std::string::npos;
}

bool redis_client_t::is_uint64(const std::string& table, const std::string& field)
{
	std::string field_type;
	if (!get_field_type(table, field, field_type))
		return false;

	return field_type.find("bigint") != std::string::npos;
}

bool redis_client_t::is_uint32(const std::string& table, const std::string& field)
{
	std::string field_type;
	if (!get_field_type(table, field, field_type))
		return false;

	return field_type.find("int") != std::string::npos;
}

bool redis_client_t::is_float(const std::string& table, const std::string& field)
{
	std::string field_type;
	if (!get_field_type(table, field, field_type))
		return false;

	return field_type.find("float") != std::string::npos;
}

std::string redis_client_t::gen_data_key(const std::string& table, const std::string& key)
{
	std::string result(table + ":" + key);
	return result;
}

std::string redis_client_t::gen_data_key(const std::string& table, const uint64_t& key)
{
	std::string result(table + ":" + boost::str(boost::format("%lu") % key));
	return result;
}

std::string redis_client_t::gen_data_key(const std::string& table, const uint32_t& key)
{
	std::string result(table + ":" + boost::str(boost::format("%u") % key));
	return result;
}

std::string redis_client_t::serialize_data(const std::string& value)
{
	std::string result(value);
	return result;
}

std::string redis_client_t::serialize_data(const uint64_t& value)
{
	std::string result(boost::str(boost::format("%lu") % value));
	return result;
}

std::string redis_client_t::serialize_data(const uint32_t& value)
{
	std::string result(boost::str(boost::format("%u") % value));
	return result;
}

std::string redis_client_t::serialize_data(const float& value)
{
	std::string result(boost::str(boost::format("%f") % value));
	return result;
}

void redis_client_t::unserialize_data(const std::string& data, std::string& result)
{
	result.assign(data);
}

void redis_client_t::unserialize_data(const std::string& data, uint64_t& result)
{
	result = string_util_t::string_to_uint64(data);
}

void redis_client_t::unserialize_data(const std::string& data, uint32_t& result)
{
	result = boost::lexical_cast<uint32_t>(data);
}

void redis_client_t::unserialize_data(const std::string& data, float& result)
{
	result = boost::lexical_cast<float>(data);
}

bool redis_client_t::is_field_exist(const std::string& table, const std::string& field)
{
	if (m_field_list.find(table) == m_field_list.end())
		return false;

	if (m_field_list[table].find(field) == m_field_list[table].end())
		return false;

	return true;
}

bool redis_client_t::is_field_need_mapping(const std::string& table, const std::string& field)
{
	if (m_field_list.find(table) == m_field_list.end())
		return false;

	field_list::iterator it = m_field_list[table].find(field);
	if (it == m_field_list[table].end())
		return false;

	return it->second.need_mapping;
}

void redis_client_t::save_field_mapping(const std::string& table, const std::string& field, const std::string& field_value, const std::string& key_value)
{
    if (m_field_list.find(table) == m_field_list.end())
        return;

    field_list::iterator it = m_field_list[table].find(field);
    if (it == m_field_list[table].end())
        return;

    if (!it->second.need_mapping)
        return;

    it->second.value_mapping[field_value].insert(key_value);
}

void redis_client_t::del_field_mapping(const std::string& table, const std::string& field, const std::string& field_value, const std::string& key_value)
{
    if (m_field_list.find(table) == m_field_list.end())
        return;

    field_list::iterator it = m_field_list[table].find(field);
    if (it == m_field_list[table].end())
        return;

    if (!it->second.need_mapping)
        return;

	it->second.value_mapping[field_value].erase(key_value);
}

bool redis_client_t::is_key_exist(const std::string& key)
{
	if (m_redis_connect == NULL)
		return false;

	redis_reply_t reply;
	reply.m_reply_data = (redisReply*)redisCommand(m_redis_connect, "EXISTS %s", key.c_str());

	if (reply.m_reply_data == NULL)
	{
		log_error("redis client is_key_exist key[%s] replay is null!", key.c_str());
		return false;
	}

	if (reply.m_reply_data->type != REDIS_REPLY_INTEGER)
	{
		return false;
	}

	return (reply.m_reply_data->integer != 0);
}

bool redis_client_t::get_keys_from_redis(const std::string& table, std::vector<std::string>& ret)
{
	if (m_redis_connect == NULL)
		return false;

	redis_reply_t reply;
	reply.m_reply_data = (redisReply*)redisCommand(m_redis_connect, "KEYS %s%s*", table.c_str(), ":");

	if (reply.m_reply_data == NULL)
		return false;

	if (reply.m_reply_data->type != REDIS_REPLY_ARRAY)
		return false;

	for (size_t i = 0; i < reply.m_reply_data->elements; ++i)
	{
		redisReply* elem = reply.m_reply_data->element[i];
		if (NULL != elem)
		{
			ret.push_back(std::string(elem->str));
		}
		
	}

	return true;
}

const redis_client_t::field_list& redis_client_t::get_table_field(const std::string& table)
{
	table_list::const_iterator citr = m_field_list.find(table);
	if (citr != m_field_list.end()) {
		return citr->second;
	}

	log_error("redis get table[%s] field error", table.c_str());
	return const_error_list;
}

//////////////////////////////////////////////////////////////////////////

redis_save_t::redis_save_t()
	: m_strand(m_io_service)
{

}

redis_save_t::~redis_save_t()
{

}


bool redis_save_t::init(const std::string& host, int port, const char* passwd, int db, uint32_t save_interval, uint32_t save_count_limit, struct ConnectionPool_S* zdb_pool)
{
	if (!connect(host, port, passwd, db))
	{
		return false;
	}

	m_work.reset(new boost::asio::io_service::work(m_io_service));
	m_thread.reset(new boost::thread(boost::bind(&boost::asio::io_service::run, &m_io_service)));

	m_save_interval = save_interval;
	m_save_count_limit = save_count_limit;
	m_save_cancel = false;
	m_save_to_sql_timer.reset(new boost::asio::deadline_timer(m_io_service));
	if (m_save_to_sql_timer == NULL) {
		log_error("m_save_to_sql_timer null error");
		return false;
	}

	if (!start_save_to_sql_timer(zdb_pool))
	{
		return false;
	}

	log_info("init redis_save[ip=%s db=%d] success!", host.c_str(), db);
	return true;
}

void redis_save_t::close(struct ConnectionPool_S* zdb_pool)
{
	// 关闭定时器
	stop_save_to_sql_timer();
	//m_io_service.post(boost::bind(&redis_save_t::do_save_all, this, zdb_pool, true));

	m_work.reset();
	m_io_service.stop();

	if (m_thread)
	{
		m_thread->join();
		m_thread.reset();
	}


	redis_save_t::do_save_all(zdb_pool, true);
	disconnect();

	log_info("close redis_save success!");
}

bool redis_save_t::connect(const std::string& host, int port, const char* passwd, int db)
{
	m_redis_connect = redisConnect(host.c_str(), port);

	if (m_redis_connect == NULL)
	{
		log_error("m_redis_connect == NULL");
		return false;
	}

	if (m_redis_connect->err)
	{
		log_error("redis client connect error: %s", m_redis_connect->errstr);
		return false;
	}

	if (passwd != NULL)
	{
		redis_reply_t auth_reply;
		auth_reply.m_reply_data = (redisReply*)redisCommand(m_redis_connect, "AUTH %s", passwd);
	}

	redis_reply_t select_reply;
	select_reply.m_reply_data = (redisReply*)redisCommand(m_redis_connect, "SELECT %d", db);

	return true;
}

void redis_save_t::disconnect()
{
	if (m_redis_connect != NULL)
	{
		redisFree(m_redis_connect);
		m_redis_connect = NULL;
	}
}

bool redis_save_t::start_save_to_sql_timer(struct ConnectionPool_S* zdb_pool)
{
	if (m_save_cancel)
	{

		return false;
	}

	if (0 == m_save_interval)
	{
		log_error("m_save_interval is 0 !");
		return false;
	}

	if (NULL == m_save_to_sql_timer)
	{
		log_error("NULL == m_save_to_sql_timer");
		return false;
	}

	m_save_to_sql_timer->expires_from_now(boost::posix_time::seconds(m_save_interval));
	m_save_to_sql_timer->async_wait(boost::bind(&redis_save_t::save_to_sql_timer_callback, this, boost::asio::placeholders::error, zdb_pool));

	return true;
}

void redis_save_t::stop_save_to_sql_timer()
{
	if (NULL == m_save_to_sql_timer)
	{
		return;
	}

	m_save_cancel = true;
	m_save_to_sql_timer->cancel();
}

void redis_save_t::save_to_sql_timer_callback(const boost::system::error_code& ec, struct ConnectionPool_S* zdb_pool)
{
	if (ec == boost::asio::error::operation_aborted)
	{

		return;
	} else if (ec)
	{
		log_error("Timer error: %s", ec.message().c_str());
		return;
	}
	if (m_save_cancel)
	{
		log_error("Timer was canceled");
		return;
	}

	do_save_all(zdb_pool, false);
	start_save_to_sql_timer(zdb_pool);
}

bool redis_save_t::do_save_single(const Connection_T& conn, const std::string& data_key)
{
	std::vector<std::string> vec_data_key;
	common::string_util_t::split<std::string>(data_key, vec_data_key, ":");
	if (vec_data_key.size() != 2)
	{
		log_error("redis client do_save_to_sql data_key[%s] error", data_key.c_str());
		return false;
	}

	const std::string& table = vec_data_key[0];
	const std::string& key_data = vec_data_key[1];
	bool ret = true;
	std::vector<std::string> keys;
	keys.push_back(KEY_FIELD);
	if (get_field_from_redis(data_key, keys))
	{
		std::ostringstream cols;
		std::map<std::string, std::string> values;
		for (auto field : keys)
		{
			if (!redis_client_t::is_field_exist(table, field))
			{
				continue;
			}

			if (!cols.str().empty())
			{
				cols << ",";
			}

			cols << "`" << field << "` = ?";

			if (field == KEY_FIELD)
			{
				values[field] = key_data;
			} else
			{
				std::string str_data;
				load_from_redis(data_key, field, str_data);
				values[field] = str_data;
			}
		}

		std::ostringstream sql;
		sql << "REPLACE INTO `" << table << "` SET " << cols.str() << ";";
		TRY
		{
			PreparedStatement_T pre = Connection_prepareStatement(conn, sql.str().c_str());
		int index = 1;
		for (auto field : keys)
		{
			if (!redis_client_t::is_field_exist(table, field))
			{
				continue;
			}

			if (redis_client_t::is_protobuf(table, field))
			{
				PreparedStatement_setBlob(pre, index, values[field].c_str(), values[field].length());
			} else if (redis_client_t::is_string(table, field))
			{
				PreparedStatement_setString(pre, index, values[field].c_str());
			} else if (redis_client_t::is_uint64(table, field))
			{
				PreparedStatement_setLLong(pre, index, string_util_t::string_to_uint64(values[field]));
			} else if (redis_client_t::is_uint32(table, field))
			{
				PreparedStatement_setInt(pre, index, boost::lexical_cast<uint32_t>(values[field]));
			} else if (redis_client_t::is_float(table, field))
			{
				PreparedStatement_setDouble(pre, index, boost::lexical_cast<float>(values[field]));
			}

			++index;
		}

		PreparedStatement_execute(pre);
		}

			CATCH(SQLException)
		{
 			/*for (auto field : keys)
 			{
 				if (!redis_client_t::is_field_exist(table, field))
 				{
 					continue;
 				}
 
 				if (redis_client_t::is_protobuf(table, field))
 				{
 					log_error("field[%s] is_protobuf value[%s]", field.c_str(), values[field].c_str());
 				} 
                else if (redis_client_t::is_string(table, field))
 				{
 					log_error("field[%s] is_string value[%s]", field.c_str(), values[field].c_str());
 				} 
                else if (redis_client_t::is_uint64(table, field))
 				{
 					log_error("field[%s] is_uint64 value[%s]", field.c_str(), values[field].c_str());
 				} 
                else if (redis_client_t::is_uint32(table, field))
 				{
 					log_error("field[%s] is_uint32 value[%s]", field.c_str(), values[field].c_str());
 				} 
                else if (redis_client_t::is_float(table, field))
 				{
 					log_error("field[%s] is_float value[%s]", field.c_str(), values[field].c_str());
 				}
 			}*/

            //log_error("[%s] sql[%s] error: %s", data_key.c_str(), sql.str().c_str(), Exception_frame.message);
            log_error("[%s] error: %s", data_key.c_str(), Exception_frame.message);
			ret = false;
		}
		FINALLY
		{
			Connection_close(conn);
		}
		END_TRY;
	}
	else
	{
		log_error("redis client not find [%s]", data_key.c_str());
		Connection_close(conn);
		ret = false;
	}

	return ret;
}

bool redis_save_t::do_save_all(struct ConnectionPool_S* zdb_pool, bool is_final)
{
	if (NULL == zdb_pool)
	{
		log_error("NULL == zdb_pool");
		return false;
	}

	if (m_need_save_deque.empty())
	{
		return false;
	}

	Connection_T conn = common::zdb_util_t::get_zdb_conn(zdb_pool);
	if (NULL == conn)
	{
		log_error("redis client save_to_sql_timer_callback conn is null");
		return false;
	}

	uint32_t count = 0;
	//log_error("redis need save size[%lu]", m_need_save_deque.size());
	for (;;) {
		const std::string& data_key = m_need_save_deque.back();
		m_need_save_map.erase(data_key);
		if (!do_save_single(conn, data_key)) {
			m_need_save_deque.pop_back();
			if (m_need_save_deque.empty()) {
				break;
			}
			continue;
		}

		++count;
		m_need_save_deque.pop_back();
		if (m_need_save_deque.empty()) {
			break;
		}

		if (!is_final && count >= m_save_count_limit) {
			break;
		}
	}

	return true;
}

void redis_save_t::do_save_to_sql(const std::string& data_key)
{
	save_key_set::iterator itr = m_need_save_map.find(data_key);
	if (itr != m_need_save_map.end()) {
		return;
	}

	m_need_save_deque.push_front(data_key);
	m_need_save_map.insert(data_key);
}

bool redis_save_t::get_field_from_redis(const std::string& data_key, std::vector<std::string>& ret)
{
	if (m_redis_connect == NULL)
		return false;

	redis_reply_t reply;
	reply.m_reply_data = (redisReply*)redisCommand(m_redis_connect, "HKEYS %s", data_key.c_str());

	if (reply.m_reply_data == NULL)
		return false;

	if (reply.m_reply_data->type != REDIS_REPLY_ARRAY)
		return false;

	for (size_t i = 0; i < reply.m_reply_data->elements; ++i)
	{
		redisReply* elem = reply.m_reply_data->element[i];
		if (NULL != elem)
		{
			ret.push_back(std::string(elem->str));
		}
	}

	return true;
}

bool redis_save_t::load_from_redis(const std::string& key, const std::string& field, std::string& result)
{
	if (m_redis_connect == NULL)
		return false;

	redis_reply_t reply;
	//reply.m_reply_data = (redisReply*)redisCommand(m_redis_connect, "GET %s", key.c_str());
	reply.m_reply_data = (redisReply*)redisCommand(m_redis_connect, "HGET %s %s", key.c_str(), field.c_str());

	if (reply.m_reply_data == NULL)
	{
		log_error("redis client do_load key[%s] replay is null!", key.c_str());
		return false;
	}

	if (reply.m_reply_data->type != REDIS_REPLY_STRING)
	{
		//log_error("redis client do_load key[%s] field[%s] reply_type[%d] replay error!", key.c_str(), field.c_str(), reply.m_reply_data->type);
		return false;
	}

	result.assign(reply.m_reply_data->str, reply.m_reply_data->len);
	return true;
}

void redis_save_t::save_to_sql(const std::string& data_key)
{
	m_io_service.post(boost::bind(&redis_save_t::do_save_to_sql, this, data_key));
}

//////////////////////////////////////////////////////////////////////////
redis_read_t::redis_read_unit::redis_read_unit(boost::asio::io_service& ios)
	: m_strand(ios)
	, m_is_work(false)
{

}

redis_read_t::redis_read_unit::~redis_read_unit()
{
	if (m_redis_connect != NULL) {
		redisFree(m_redis_connect);
		m_redis_connect = NULL;
	}

	if (m_zdb_conn != NULL) {
		Connection_close(m_zdb_conn);
		m_zdb_conn = NULL;
	}
}

void redis_read_t::redis_read_unit::read(const boost::weak_ptr<redis_read_unit>& p_wunit, const std::string& table, const std::string& data_col, uint32_t row)
{
	redis_read_unit_ptr p_read_unit_ptr = p_wunit.lock();
	if (p_read_unit_ptr == NULL) {
		log_error("p_read_unit_ptr is free");
		return;
	}

	std::map<std::string, std::string> field_data;
	p_read_unit_ptr->do_read(table, data_col, row, field_data);
	g_redis_read.unit_read_finish(p_read_unit_ptr, table, field_data);
}

bool redis_read_t::redis_read_unit::do_read(const std::string& table, const std::string& data_col, uint32_t row, std::map<std::string, std::string>& datas)
{
	if (!m_redis_connect) {
		log_error("m_redis_connect null error");
		return false;
	}

	if (!m_zdb_conn) {
		log_error("m_zdb_conn null error");
		return false;
	}

	const redis_client_t::field_list& cols = redis_client_t::get_table_field(table);
	if (cols.empty()) {
		log_error("cols is empty!");
		return false;
	}

	TRY
	{
		char sql[2048] = "";
		sprintf(sql, "SELECT %s FROM %s LIMIT %u,1", data_col.c_str(), table.c_str(), row);
		//log_warn("deal sql[%s] start time[%d]", sql, time_util_t::now_msecs());
		ResultSet_T result = Connection_executeQuery(m_zdb_conn, sql);
		//log_warn("deal sql[%s] end time[%d]", sql, time_util_t::now_msecs());
		while (ResultSet_next(result)) {
			int index = 1;
			for (const auto& column : cols) {
				std::string str_data("");
				if (redis_client_t::is_protobuf(table, column.first)) {
					int size = 0;
					const char* blob = (const char*)ResultSet_getBlob(result, index, &size);
					if (blob != NULL) str_data.assign(blob, size);
				} else if (redis_client_t::is_string(table, column.first)) {
					const char* str = (const char*)ResultSet_getString(result, index);
					if (str != NULL) str_data.assign(str);
				} else if (redis_client_t::is_uint64(table, column.first)) {
					str_data = common::string_util_t::uint64_to_string(ResultSet_getLLong(result, index));
				} else if (redis_client_t::is_uint32(table, column.first)) {
					str_data = common::string_util_t::uint64_to_string(ResultSet_getInt(result, index));
				} else if (redis_client_t::is_float(table, column.first)) {
					str_data = common::string_util_t::uint64_to_string(ResultSet_getDouble(result, index));
				} else {
					log_error("redis read table[%s] filed[%s] type error", table.c_str(), column.first.c_str());
					return false;
				}

				index += 1;
				datas[column.first] = std::move(str_data);
			}
			//log_warn("deal sql[%s] get_result[%d]", sql, time_util_t::now_msecs());

			std::string data_key = redis_client_t::gen_data_key(table, datas[KEY_FIELD]);
			time_t redis_data_time = get_time_from_redis(data_key);
			time_t sql_data_time = boost::lexical_cast<time_t>(datas[TIME_FIELD]);

			if (redis_data_time == 0 || sql_data_time > redis_data_time) {
				//log_warn("deal sql[%s] save to redis start time[%d]", sql, time_util_t::now_msecs());
				for (const auto& sql_data : datas) {
					if (sql_data.first == KEY_FIELD)
						continue;

					if (redis_client_t::is_protobuf(table, sql_data.first)) {
						save_to_redis(data_key, sql_data.first, sql_data.second, true);
					} else {
						save_to_redis(data_key, sql_data.first, sql_data.second, false);
					}
				}
				//log_warn("deal sql[%s] save to redis end time[%d]", sql, time_util_t::now_msecs());
			}
		}
	}
	CATCH(SQLException)
	{
		log_error("redis_read do_read [%s] data error: %s", table.c_str(), Exception_frame.message);
	}
	FINALLY
	{

	}
	END_TRY;

	return true;
}

bool redis_read_t::redis_read_unit::load_from_redis(const std::string& key, const std::string& field, std::string& result)
{
	if (m_redis_connect == NULL)
		return false;

	redis_reply_t reply;
	//reply.m_reply_data = (redisReply*)redisCommand(m_redis_connect, "GET %s", key.c_str());
	reply.m_reply_data = (redisReply*)redisCommand(m_redis_connect, "HGET %s %s", key.c_str(), field.c_str());

	if (reply.m_reply_data == NULL)
	{
		log_error("redis client do_load key[%s] replay is null!", key.c_str());
		return false;
	}

	if (reply.m_reply_data->type != REDIS_REPLY_STRING)
	{
		//log_error("redis client do_load key[%s] field[%s] reply_type[%d] replay error!", key.c_str(), field.c_str(), reply.m_reply_data->type);
		return false;
	}

	result.assign(reply.m_reply_data->str, reply.m_reply_data->len);
	return true;
}

time_t redis_read_t::redis_read_unit::get_time_from_redis(const std::string& data_key)
{
	std::string res;
	if (!load_from_redis(data_key, TIME_FIELD, res))
		return 0;

	return boost::lexical_cast<time_t>(res);
}

bool redis_read_t::redis_read_unit::save_to_redis(const std::string& key, const std::string& field, const std::string& value, const bool binary)
{
	if (m_redis_connect == NULL)
		return false;

	if (value.length() > CACHE_VALUEX_LEN_MA) {
		log_error("redis client set key[%s] value[%s] too long!", key.c_str(), value.c_str());
		//return false;
	}

	redis_reply_t reply;
	if (field.empty()) {
		std::string time_str = string_util_t::uint64_to_string(time(NULL));
		reply.m_reply_data = (redisReply*)redisCommand(m_redis_connect, "HSET %s %s %s", key.c_str(), TIME_FIELD.c_str(), time_str.c_str());
	} else {
		if (binary) {
			reply.m_reply_data = (redisReply*)redisCommand(m_redis_connect, "HSET %s %s %b", key.c_str(), field.c_str(), value.c_str(), value.length());
		} else {
			reply.m_reply_data = (redisReply*)redisCommand(m_redis_connect, "HSET %s %s %s", key.c_str(), field.c_str(), value.c_str());
		}
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////
redis_read_t::redis_read_t()
	: m_strand(m_io_service)
{

}

redis_read_t::~redis_read_t()
{

}

bool redis_read_t::init(const std::string& host, int port, const char* passwd, int db, uint32_t thread_num, uint32_t read_unit_num, struct ConnectionPool_S* zdb_pool, boost::function<void()> read_Callback)
{
	if (!init_read_unit(host, port, passwd, db, zdb_pool, read_unit_num)) {
		m_work_list.clear();
		log_error("init read_unit failed");
		return false;
	}

	m_work.reset(new boost::asio::io_service::work(m_io_service));

	for (size_t i = 0; i < thread_num; ++i) {
		m_thread_group.create_thread(boost::bind(&boost::asio::io_service::run, &m_io_service));
	}

	m_call_back = read_Callback;
	m_is_init = true;
	m_is_close = false;

	log_info("init redis_read[ip=%s db=%d] success!", host.c_str(), db);
	return true;
}

void redis_read_t::close()
{
	if (!m_is_init) return;

	if (m_is_close) return;

	m_work.reset();
	m_io_service.stop();

	m_thread_group.join_all();

	// clear
	m_work_list.clear();
	m_read_table_list.clear();

	m_is_close = true;

	log_info("close redis_read close!");
}

bool redis_read_t::load_all_data(const redis_client_t::table_field_list& load_tables, 
	redis_client_t::table_list& all_table_list,
	struct ConnectionPool_S* zdb_pool)
{
	Connection_T conn = common::zdb_util_t::get_zdb_conn(zdb_pool);
	if (NULL == conn) {
		log_error("redis read load_all_data conn is null");
		return false;
	}

	for (redis_client_t::table_field_list::const_iterator citr = load_tables.begin();
		citr != load_tables.end(); ++citr) {
		table_field_t field(citr->first);
		bool ret = true;
		TRY
		{
			ResultSet_T result = Connection_executeQuery(conn, "SHOW COLUMNS FROM `%s`", citr->first.c_str());

			while (ResultSet_next(result)) {
				std::string field_name = ResultSet_getString(result, 1);
				std::string field_type = ResultSet_getString(result, 2);
				all_table_list[citr->first].insert(std::make_pair(field_name, redis_client_t::field_data(field_type, ((citr->second.find(field_name) != citr->second.end()) ? true : false))));
			}
		}
		CATCH(SQLException)
		{
			log_error("redis client load_all_data [%s] error: %s", citr->first.c_str(), Exception_frame.message);
			ret = false;
		}
		FINALLY
		{
		}
		END_TRY;

		if (!ret) {
			Connection_close(conn);
			return false;
		}

		const redis_client_t::field_list& cols = all_table_list[citr->first];
		if (cols.empty())
		{
			log_error("table[%s] cols is empty!", citr->first.c_str());
			Connection_close(conn);
			return false;
		}

		std::string str_col;
		for (const auto& column : cols)
		{
			if (!str_col.empty())
			{
				str_col += ",";
			}

			str_col += "`";
			str_col += column.first;
			str_col += "`";
		}

		field.m_str_col = str_col;

		TRY
		{
			ResultSet_T result = Connection_executeQuery(conn, "SELECT COUNT(*) FROM `%s`", citr->first.c_str());

			while (ResultSet_next(result)) {
				field.m_max_cursor = ResultSet_getInt(result, 1);
			}
		}
		CATCH(SQLException)
		{
			log_error("redis client load_all_data [%s] error: %s", citr->first.c_str(), Exception_frame.message);
			ret = false;
		}
		FINALLY
		{
			
		}
		END_TRY;

		if (!ret) {
			Connection_close(conn);
			return false;
		}

		m_read_table_list.push_back(field);
	}

	Connection_close(conn);

	m_strand.post(boost::bind(&redis_read_t::start_read_from_db, this));

	return true;
}

bool redis_read_t::init_read_unit(const std::string& host, int port, const char* passwd, int db, struct ConnectionPool_S* zdb_pool, uint32_t num)
{
	if (NULL == zdb_pool) {
		log_error("NULL == zdb_pool");
		return false;
	}

	for (uint32_t i = 0; i < num; ++i) {
		redis_read_unit_ptr p_unit(new redis_read_unit(m_io_service));
		if (!p_unit) {
			log_error("redis_read_unit_ptr null error");
			return false;
		}

		// 初始化redis连接
		p_unit->m_redis_connect = redisConnect(host.c_str(), port);

		if (p_unit->m_redis_connect == NULL)
		{
			log_error("m_redis_connect == NULL");
			return false;
		}

		if (p_unit->m_redis_connect->err)
		{
			log_error("redis client connect error: %s", p_unit->m_redis_connect->errstr);
			return false;
		}

		if (passwd != NULL)
		{
			redis_reply_t auth_reply;
			auth_reply.m_reply_data = (redisReply*)redisCommand(p_unit->m_redis_connect, "AUTH %s", passwd);
		}

		redis_reply_t select_reply;
		select_reply.m_reply_data = (redisReply*)redisCommand(p_unit->m_redis_connect, "SELECT %d", db);

		p_unit->m_zdb_conn = common::zdb_util_t::get_zdb_conn(zdb_pool);
		if (!p_unit->m_zdb_conn) {
			log_error("redis client init_read_unit conn is null");
			return false;
		}

		m_work_list.insert(p_unit);
	}

	m_read_unit_num = num;

	log_info("init read_unit success, num:%u", num);
	return true;
}

void redis_read_t::start_read_from_db()
{
	if (m_curr_read_table < m_read_table_list.size()) {
		log_info("redis read table[%s]", m_read_table_list[m_curr_read_table].m_table_name.c_str());
	}

	if (m_work_list.empty()) {
		log_error("m_work_list is empty");
		return;
	}

	for (redis_read_unit_list::const_iterator citr = m_work_list.begin();
		citr != m_work_list.end(); ++citr) {
		if (!do_read_single(*citr)) {
			break;
		}
	}

	check_read_db_done();
}

bool redis_read_t::do_read_single(const redis_read_unit_ptr& p_read_unit_ptr)
{
	if (!p_read_unit_ptr) {
		log_error("redis_read_unit_ptr null error");
		return false;
	}

	if (is_table_end()) {
		return false;
	}

	p_read_unit_ptr->set_work(true);
	p_read_unit_ptr->m_strand.post(boost::bind(&redis_read_unit::read, boost::weak_ptr<redis_read_unit>(p_read_unit_ptr), m_read_table_list[m_curr_read_table].m_table_name, m_read_table_list[m_curr_read_table].m_str_col, m_read_table_list[m_curr_read_table].m_cursor));

	m_read_table_list[m_curr_read_table].m_cursor++;
	if (m_read_table_list[m_curr_read_table].m_cursor >= m_read_table_list[m_curr_read_table].m_max_cursor) {
		m_curr_read_table++;

		if (m_curr_read_table < m_read_table_list.size()) {
			log_info("redis read table[%s]", m_read_table_list[m_curr_read_table].m_table_name.c_str());
		}
	}

	return true;
}

bool redis_read_t::is_table_end() const
{
	if (m_read_table_list.empty()) {
		return true;
	}

	if (m_curr_read_table >= m_read_table_list.size()) {
		return true;
	}

	return false;
}

void redis_read_t::check_read_db_done()
{
	if (!is_table_end()) return;

	uint32_t finish_count = 0;
	for (const auto& p_read_unit : m_work_list) {
		if (p_read_unit && !p_read_unit->is_work()) {
			finish_count++;
		}
	}

	if (finish_count < m_read_unit_num) return;

	log_info("redis read from db done!");

	if (!m_is_close && m_call_back) {
		m_call_back();
	}

	return;
}

void redis_read_t::do_read_finish(const boost::weak_ptr<redis_read_unit>& p_wunit, const std::string& table, const std::map<std::string, std::string>& datas)
{
	redis_read_unit_ptr p_read_unit_ptr = p_wunit.lock();
	if (p_read_unit_ptr == NULL) {
		log_error("p_read_unit_ptr is free");
		return;
	}

	if (!datas.empty()) {
		std::map<std::string, std::string>::const_iterator key = datas.find(KEY_FIELD);
		if (key != datas.end()) {
			for (std::map<std::string, std::string>::const_iterator citr = datas.begin();
				citr != datas.end(); ++citr) {
				if (citr->first == KEY_FIELD)
					continue;

				redis_client_t::save_field_mapping(table, citr->first, citr->second, key->second);
			}
		} else {
			log_error("redis read table[%s] key_field not find", table.c_str());
		}
	}

	if (!is_table_end()) {
		do_read_single(p_read_unit_ptr);
		return;
	} 

	p_read_unit_ptr->set_work(false);
	check_read_db_done();
}

void redis_read_t::unit_read_finish(const redis_read_unit_ptr& p_read_unit_ptr, const std::string& table, const std::map<std::string, std::string>& datas)
{
	m_strand.post(boost::bind(&redis_read_t::do_read_finish, this, boost::weak_ptr<redis_read_unit>(p_read_unit_ptr), table, datas));
}
