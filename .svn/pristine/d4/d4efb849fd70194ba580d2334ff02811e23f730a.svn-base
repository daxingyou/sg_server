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
    uint32_t save_interval,
    uint32_t save_count_limit,
    boost::asio::io_service& ios,
    struct ConnectionPool_S* zdb_pool,
    const simple_table_field_list& load_tables)
{
    table_field_list list;
    for (auto table : load_tables)
    {
        field_name_list field_name;
        field_name.insert(table.second);
        list.insert(std::make_pair(table.first, field_name));
    }
    return init(host, port, passwd, db, save_interval, save_count_limit, ios, zdb_pool, list);
}

bool redis_client_t::init(
    const std::string& host,
    int port,
    const char* passwd,
    int db,
    uint32_t save_interval,
    uint32_t save_count_limit,
    boost::asio::io_service& ios,
    struct ConnectionPool_S* zdb_pool,
    table_field_list& load_tables,
    const simple_table_field_list& load_simple_tables)
{
    for (auto simple : load_simple_tables)
    {
        field_name_list field_name;
        field_name.insert(simple.second);

        load_tables.insert(std::make_pair(simple.first, field_name));
    }
    return init(host, port, passwd, db, save_interval, save_count_limit, ios, zdb_pool, load_tables);
}

bool redis_client_t::init(
	const std::string& host, 
	int port, 
	const char* passwd, 
	int db,
	uint32_t save_interval,
	uint32_t save_count_limit,
	boost::asio::io_service& ios, 
	struct ConnectionPool_S* zdb_pool,
	const table_field_list& load_tables)
{
	if (!connect(host, port, passwd, db))
	{
		return false;
	}

    log_info("redis load data from sql...Start");
	for (auto table : load_tables)
	{
        if (!load_from_sql(table.first, table.second, zdb_pool))
        {
            return false;
        }
	}
    log_info("redis load data from sql...Done");

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
			log_error("redis client do_save_to_sql [%s] sql[%s] update error: %s", data_key.c_str(), sql.str().c_str(), Exception_frame.message);
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
	log_error("redis need save size[%lu]", m_need_save_deque.size());
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