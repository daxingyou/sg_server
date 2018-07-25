#ifndef __REDIS_CLIENT_HPP__
#define __REDIS_CLIENT_HPP__

#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/thread.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/asio.hpp>

#include <boost/asio/deadline_timer.hpp>
#include <google/protobuf/message.h>
#include <redis/deps/hiredis/hiredis.h>
#include <map>
#include <deque>

#include "utility.hpp"
#include "zdb_ex.hpp"
#include <zlib.h>

// 每张表默认必须有的两个字段：键字段和时间字段
static const std::string KEY_FIELD = "rk";
static const std::string TIME_FIELD = "rt";

// redis客户端
class redis_client_t
{
	friend class redis_read_t;
	friend class redis_save_t;

public:
    //typedef std::unordered_set<std::string> field_name_list;
    typedef std::set<std::string> field_name_list;
	typedef std::map<std::string, field_name_list> table_field_list;
	typedef std::map<std::string, std::string> simple_table_field_list;

public:
	redis_client_t();
	~redis_client_t();

	/**
	 * redis客户端初始化
	 * @host redis服务器ip
	 * @port redis服务器端口
	 * @db redis数据库编号
	 * @save_interval 保存到sql时间间隔
	 * @ios ioservice 用于初始化定时器
	 * @zdb_pool zdb连接池 用于保存到sql
	 * @load_tables 需要加载以及保存到数据库的表
	**/
	static bool init(const std::string& host,
		int port,
		const char* passwd,
		int db,
		uint32_t work_thread,
		uint32_t save_interval,
		uint32_t save_count_limit,
		struct ConnectionPool_S* zdb_pool,
		const table_field_list& load_tables, 
		boost::function<void()> read_Callback = NULL);

	static bool init(const std::string& host,
		int port,
		const char* passwd,
		int db,
		uint32_t work_thread,
		uint32_t save_interval,
		uint32_t save_count_limit,
		struct ConnectionPool_S* zdb_pool,
		const simple_table_field_list& load_tables, 
		boost::function<void()> read_Callback = NULL);

	static bool init(const std::string& host,
		int port,
		const char* passwd,
		int db,
		uint32_t work_thread,
		uint32_t save_interval,
		uint32_t save_count_limit,
		struct ConnectionPool_S* zdb_pool,
		table_field_list& load_tables,
		const simple_table_field_list& load_simple_tables, 
		boost::function<void()> read_Callback = NULL);

	static void close(struct ConnectionPool_S* zdb_pool);

	/**
	* 保存数据 KEY_TYPE支持uint32_t, uint64_t, std::string VALUE_TYPE支持uint32_t, uint64_t, std::string, float
	* @table 表名
	* @field 字段名
	* @key 键值
	* @value 字段值
	**/
	template<typename KEY_TYPE, typename VALUE_TYPE>
	static bool set_data(const std::string& table, const std::string& field, const KEY_TYPE& key, const VALUE_TYPE& value);

	// 具体类型接口，方便调用
	template<typename KEY_TYPE>
	static bool set_string(const std::string& table, const std::string& field, const KEY_TYPE& key, const std::string& value);
	template<typename KEY_TYPE>
	static bool set_uint64(const std::string& table, const std::string& field, const KEY_TYPE& key, const uint64_t value);
	template<typename KEY_TYPE>
	static bool set_uint32(const std::string& table, const std::string& field, const KEY_TYPE& key, const uint32_t value);
	template<typename KEY_TYPE>
	static bool set_float(const std::string& table, const std::string& field, const KEY_TYPE& key, const float value);

	/**
	* 保存数据 KEY_TYPE支持uint32_t, uint64_t, std::string PROTOBUF_TYPE支持protobuf结构
	* @table 表名
	* @field 字段名
	* @key 键值
	* @value 字段值
	**/
	template<typename KEY_TYPE, typename PROTOBUF_TYPE>
	static bool set_protobuf(const std::string& table, const std::string& field, const KEY_TYPE& key, const PROTOBUF_TYPE& proto);

	/**
	* 读取数据 KEY_TYPE支持uint32_t, uint64_t, std::string VALUE_TYPE支持uint32_t, uint64_t, std::string, float
	* @table 表名
	* @field 字段名
	* @key 键值
	* @value 字段值
	**/
	template<typename KEY_TYPE, typename VALUE_TYPE>
	static bool get_data(const std::string& table, const std::string& field, const KEY_TYPE& key, VALUE_TYPE& result);

	// 具体类型接口，方便调用
	template<typename KEY_TYPE>
	static std::string get_string(const std::string& table, const std::string& field, const KEY_TYPE& key);
	template<typename KEY_TYPE>
	static uint64_t get_uint64(const std::string& table, const std::string& field, const KEY_TYPE& key);
	template<typename KEY_TYPE>
	static uint32_t get_uint32(const std::string& table, const std::string& field, const KEY_TYPE& key);
	template<typename KEY_TYPE>
	static float get_float(const std::string& table, const std::string& field, const KEY_TYPE& key);

	/**
	* 读取数据 KEY_TYPE支持uint32_t, uint64_t, std::string PROTOBUF_TYPE支持protobuf结构
	* @table 表名
	* @field 字段名
	* @key 键值
	* @value 字段值
	**/
	template<typename KEY_TYPE, typename PROTOBUF_TYPE>
	static bool get_protobuf(const std::string& table, const std::string& field, const KEY_TYPE& key, PROTOBUF_TYPE& result);

	/**
	* 删除数据 KEY_TYPE支持uint32_t, uint64_t, std::string
	* @table 表名
	* @field 字段名
	* @key 键值
	**/
	template<typename KEY_TYPE>
	static bool remove_data(const std::string& table, const std::string& field, const KEY_TYPE& key);

	// 根据字段值查找key
	template<typename KEY_TYPE, typename VALUE_TYPE, typename CONTAINER>
	static bool get_keys_by_field_condition(const std::string& table, const std::string& field, const VALUE_TYPE& conditon_value, CONTAINER& result);

	// 根据表名查找key
	template<typename KEY_TYPE, typename CONTAINER>
	static bool get_all_keys(const std::string& table, CONTAINER& result);

	// key是否存在
	template<typename KEY_TYPE>
	static bool is_key_exist(const std::string& table, const KEY_TYPE& key);

	// 增加key，针对没有数据字段的表，增加一条记录
	template<typename KEY_TYPE>
	static bool add_key(const std::string& table, const KEY_TYPE& key);

	// 删除key，针对没有数据字段的表，删除一条记录
	template<typename KEY_TYPE>
	static bool remove_key(const std::string& table, const KEY_TYPE& key);

private:

	// 保存到redis操作
	static bool save_to_redis(const std::string& key, 
		const std::string& field,
		const std::string& value,
		const bool binary = false, 
		const bool update_sql = true);

	// 从redis读取操作
	static bool load_from_redis(const std::string& key, const std::string& field, std::string& result);

	// 读取键值为data_key的数据保存时间
	static time_t get_time_from_redis(const std::string& data_key);
	// 保存键值为data_key的数据保存时间
	static bool save_time_to_redis(const std::string& data_key);

	// 从redis删除数据操作
	static bool remove_from_redis(const std::string& key, const std::string& field);
	
	// 四种类型的数据序列化
	static std::string serialize_data(const std::string& value);
	static std::string serialize_data(const uint64_t& value);
	static std::string serialize_data(const uint32_t& value);
	static std::string serialize_data(const float& value);

	// 四种类型的数据反序列化
	static void unserialize_data(const std::string& data, std::string& result);
	static void unserialize_data(const std::string& data, uint64_t& result);
	static void unserialize_data(const std::string& data, uint32_t& result);
	static void unserialize_data(const std::string& data, float& result);

	// 连接
	static bool connect(const std::string& host, int port, const char* passwd, int db);
	// 断开
	static void disconnect();

	// 三种数据类型生成data_key
	static std::string gen_data_key(const std::string& table, const std::string& key);
	static std::string gen_data_key(const std::string& table, const uint64_t& key);
	static std::string gen_data_key(const std::string& table, const uint32_t& key);

	// 保存到sql操作
	static bool do_save_single(const Connection_T& conn, const std::string& data_key);
public:
	static bool do_save_all(struct ConnectionPool_S* zdb_pool, bool is_final);
	static void save_to_sql(const std::string& data_key);
private:
	// 保存到sql定时器
	static bool start_save_to_sql_timer(struct ConnectionPool_S* zdb_pool);
	static void stop_save_to_sql_timer();
	static void save_to_sql_timer_callback(const boost::system::error_code& ec, struct ConnectionPool_S* zdb_pool);

	// 从sql加载操作
	static bool get_field_from_redis(const std::string& data_key, std::vector<std::string>& ret);
	static bool load_from_sql(const std::string& table, const field_name_list& mapping_field, struct ConnectionPool_S* zdb_pool);

	// 根据表名以及字段名判断数据类型
	static bool get_field_type(const std::string& table, const std::string& field, std::string& result);
	static bool is_protobuf(const std::string& table, const std::string& field);
	static bool is_string(const std::string& table, const std::string& field);
	static bool is_uint64(const std::string& table, const std::string& field);
	static bool is_uint32(const std::string& table, const std::string& field);
	static bool is_float(const std::string& table, const std::string& field);

	// 数据字段是否存在
	static bool is_field_exist(const std::string& table, const std::string& field);
	// 数据字段是否需要映射
	static bool is_field_need_mapping(const std::string& table, const std::string& field);
	// 保存映射
	static void save_field_mapping(
		const std::string& table, 
		const std::string& field, 
		const std::string& field_value,
		const std::string& key_value);
	// 删除映射
	static void del_field_mapping(
		const std::string& table,
		const std::string& field,
		const std::string& field_value,
		const std::string& key_value);

	// key是否存在
	static bool is_key_exist(const std::string& key);
public:
	// 从redis读取一张表的key
	static bool get_keys_from_redis(const std::string& table, std::vector<std::string>& ret);

public:
    //typedef std::unordered_set<std::string> key_list;
    typedef std::set<std::string> key_list;
	typedef std::map <std::string, key_list> value_key;

	struct field_data
	{
		field_data(const std::string& type, bool mapping)
			: field_type(type)
			, need_mapping(mapping)
		{
		}

		std::string field_type;
		bool need_mapping;
		value_key value_mapping;
	};

	typedef std::map<std::string, field_data> field_list;
	typedef std::map<std::string, field_list> table_list;

	// 读取表结构
	static const field_list& get_table_field(const std::string& table);

private:	
	
	static redisContext* m_redis_connect;	// redis连接
	static key_list m_need_save_to_sql;		// 需要保存到sql的data_key
	static uint32_t m_save_interval;		// 定时器间隔
	static boost::shared_ptr<boost::asio::deadline_timer> m_save_to_sql_timer;	// 定时器
    static bool m_save_cancel;
	static table_list m_field_list;			// 表字段列表
	static uint32_t m_save_count_limit;		// 单次保存数量限制
	static field_list const_error_list;
};

template<typename KEY_TYPE, typename VALUE_TYPE>
bool redis_client_t::set_data(const std::string& table, const std::string& field, const KEY_TYPE& key, const VALUE_TYPE& value)
{
	if (!is_field_exist(table, field))
	{
		log_error("redis client set data table[%s] field[%s] not exist", table.c_str(), field.c_str());
		return false;
	}

	std::string data = serialize_data(value);
	return save_to_redis(gen_data_key(table, key), field, data);
}

template<typename KEY_TYPE>
bool redis_client_t::set_string(const std::string& table, const std::string& field, const KEY_TYPE& key, const std::string& value)
{
	return set_data(table, field, key, value);
}

template<typename KEY_TYPE>
bool redis_client_t::set_uint64(const std::string& table, const std::string& field, const KEY_TYPE& key, const uint64_t value)
{
	return set_data(table, field, key, value);
}

template<typename KEY_TYPE>
bool redis_client_t::set_uint32(const std::string& table, const std::string& field, const KEY_TYPE& key, const uint32_t value)
{
	return set_data(table, field, key, value);
}

template<typename KEY_TYPE>
bool redis_client_t::set_float(const std::string& table, const std::string& field, const KEY_TYPE& key, const float value)
{
	return set_data(table, field, key, value);
}

template<typename KEY_TYPE, typename PROTOBUF_TYPE>
bool redis_client_t::set_protobuf(const std::string& table, const std::string& field, const KEY_TYPE& key, const PROTOBUF_TYPE& proto)
{
	std::string data = proto.SerializeAsString();

	return save_to_redis(gen_data_key(table, key), field, str, true);
}

template<typename KEY_TYPE, typename VALUE_TYPE>
bool redis_client_t::get_data(const std::string& table, const std::string& field, const KEY_TYPE& key, VALUE_TYPE& result)
{
	if (!is_field_exist(table, field))
	{
		log_error("redis client get data table[%s] field[%s] not exist", table.c_str(), field.c_str());
		return false;
	}

	std::string data;
	if (!load_from_redis(gen_data_key(table, key), field, data))
		return false;

	if (data.empty())
		return false;

	unserialize_data(data, result);
	return true;
}

template<typename KEY_TYPE>
std::string redis_client_t::get_string(const std::string& table, const std::string& field, const KEY_TYPE& key)
{
	std::string data("");
	get_data(table, field, key, data);

	return data;
}

template<typename KEY_TYPE>
uint64_t redis_client_t::get_uint64(const std::string& table, const std::string& field, const KEY_TYPE& key)
{
	uint64_t data = 0;
	get_data(table, field, key, data);

	return data;
}

template<typename KEY_TYPE>
uint32_t redis_client_t::get_uint32(const std::string& table, const std::string& field, const KEY_TYPE& key)
{
	uint32_t data = 0;
	get_data(table, field, key, data);

	return data;
}

template<typename KEY_TYPE>
float redis_client_t::get_float(const std::string& table, const std::string& field, const KEY_TYPE& key)
{
	float data = 0.0f;
	get_data(table, field, key, data);

	return data;
}

template<typename KEY_TYPE, typename PROTOBUF_TYPE>
bool redis_client_t::get_protobuf(const std::string& table, const std::string& field, const KEY_TYPE& key, PROTOBUF_TYPE& result)
{
	std::string data;
	if (!load_from_redis(gen_data_key(table, key), field, data))
		return false;

	if (data.empty())
		return false;
	char uzBuf[MAX_UZLIB_CHAR];
	bzero(uzBuf, sizeof(uzBuf));
	uLongf bufSize = MAX_UZLIB_CHAR;
	int32_t retcode = uncompress((unsigned char *)uzBuf, &bufSize, (Bytef *)data->c_str(), data.size());
	if (retcode != Z_OK) {
		log_error("table[%s:%s] uncompress error, reason:%d", table.c_str(), field.c_str(), retcode);
		return false;
	}
	string str = uzBuf;
	return result.ParseFromString(str);
}

template<typename KEY_TYPE>
bool redis_client_t::remove_data(const std::string& table, const std::string& field, const KEY_TYPE& key)
{
	if (m_field_list.find(table) == m_field_list.end())
	{ 
		log_error("redis client remove data table[%s] not exist", table.c_str());
		return false;
	}
	
	if (m_field_list[table].find(field) == m_field_list[table].end())
	{
		log_error("redis client remove data table[%s] field[%s] not exist", table.c_str(), field.c_str());
		return false;
	}

	return remove_from_redis(gen_data_key(table, key), field);
}

template<typename KEY_TYPE, typename VALUE_TYPE, typename CONTAINER>
bool redis_client_t::get_keys_by_field_condition(const std::string& table, const std::string& field, const VALUE_TYPE& conditon_value, CONTAINER& result)
{
    if (m_field_list.find(table) == m_field_list.end())
        return false;

    field_list::iterator it = m_field_list[table].find(field);
    if (it == m_field_list[table].end())
        return false;

    if (!it->second.need_mapping)
        return false;

	std::string data = serialize_data(conditon_value);
	if (it->second.value_mapping.find(data) == it->second.value_mapping.end())
		return false;

	for (auto str_key : it->second.value_mapping[data])
	{
		KEY_TYPE key;
		unserialize_data(str_key, key);
		result.push_back(key);
	}

	return true;
}

template<typename KEY_TYPE, typename CONTAINER>
bool redis_client_t::get_all_keys(const std::string& table, CONTAINER& result)
{
	table_list::iterator it = m_field_list.find(table);
	if (it == m_field_list.end())
		return false;

	std::vector<std::string> ret;
	if (!get_keys_from_redis(table, ret))
		return false;
	
	for (auto data_key : ret)
	{
		std::vector<std::string> vec_data_key;
		common::string_util_t::split<std::string>(data_key, vec_data_key, ":");
		if (vec_data_key.size() != 2)
			continue;

		const std::string& key_data = vec_data_key[1];

		KEY_TYPE key;
		unserialize_data(key_data, key);
		result.push_back(key);
	}

	return true;
}

template<typename KEY_TYPE>
bool redis_client_t::is_key_exist(const std::string& table, const KEY_TYPE& key)
{
	return is_key_exist(gen_data_key(table, key));
}

template<typename KEY_TYPE>
bool redis_client_t::add_key(const std::string& table, const KEY_TYPE& key)
{
	return save_to_redis(gen_data_key(table, key), "", "");
}

template<typename KEY_TYPE>
bool redis_client_t::remove_key(const std::string& table, const KEY_TYPE& key)
{
	return remove_from_redis(gen_data_key(table, key), "");
}

//////////////////////////////////////////////////////////////////////////
class redis_save_t
{
public:
	redis_save_t();
	~redis_save_t();

public:
	bool init(const std::string& host,
		int port,
		const char* passwd,
		int db,
		uint32_t save_interval,
		uint32_t save_count_limit,
		struct ConnectionPool_S* zdb_pool);

	void close(struct ConnectionPool_S* zdb_pool);

	void save_to_sql(const std::string& data_key);

private:
	// 连接
	bool connect(const std::string& host, int port, const char* passwd, int db);
	// 断开
	void disconnect();

	// 保存到sql定时器
	bool start_save_to_sql_timer(struct ConnectionPool_S* zdb_pool);
	void stop_save_to_sql_timer();
	void save_to_sql_timer_callback(const boost::system::error_code& ec, struct ConnectionPool_S* zdb_pool);

	// 保存到sql操作
	bool do_save_single(const Connection_T& conn, const std::string& data_key);
	bool do_save_all(struct ConnectionPool_S* zdb_pool, bool is_final);
	void do_save_to_sql(const std::string& data_key);

	// 从redis读取操作
	bool get_field_from_redis(const std::string& data_key, std::vector<std::string>& ret);
	bool load_from_redis(const std::string& key, const std::string& field, std::string& result);

private:
	typedef std::set<std::string> save_key_set;
	typedef std::deque<std::string> save_key_list;

private:
	boost::asio::io_service m_io_service;
	boost::asio::io_service::strand m_strand;
	boost::scoped_ptr<boost::asio::io_service::work> m_work;
	boost::scoped_ptr<boost::thread> m_thread;

	redisContext* m_redis_connect = NULL;	// redis连接
	save_key_set m_need_save_map;			// 需要保存到sql的map
	save_key_list m_need_save_deque;		// 需要保存到sql的deque
	uint32_t m_save_interval = 1;			// 定时器间隔
	uint32_t m_save_count_limit = 500;		// 单次保存数量限制
	boost::shared_ptr<boost::asio::deadline_timer> m_save_to_sql_timer;	// 定时器
	bool m_save_cancel = false;
};

extern redis_save_t g_redis_save;

//////////////////////////////////////////////////////////////////////////
class redis_read_t
{
public:
	redis_read_t();
	~redis_read_t();

public:
	bool init(const std::string& host,
		int port,
		const char* passwd,
		int db,
		uint32_t thread_num,
		uint32_t read_unit_num,
		struct ConnectionPool_S* zdb_pool,
		boost::function<void()> read_Callback);

	void close();

	bool load_all_data(const redis_client_t::table_field_list& load_tables, 
		redis_client_t::table_list& all_table_list, 
		struct ConnectionPool_S* zdb_pool);

private:
	struct redis_read_unit
	{
		redis_read_unit(boost::asio::io_service& ios);
		~redis_read_unit();

		static void read(const boost::weak_ptr<redis_read_unit>& p_wunit, const std::string& table, const std::string& data_col, uint32_t row);
		bool do_read(const std::string& table, const std::string& data_col, uint32_t row, std::map<std::string, std::string>& datas);

		bool is_work() const { return m_is_work; }
		void set_work(bool is_work) { m_is_work = is_work; }
		bool load_from_redis(const std::string& key, const std::string& field, std::string& result);
		time_t get_time_from_redis(const std::string& data_key);
		bool save_to_redis(const std::string& key,const std::string& field, const std::string& value, const bool binary);

		boost::asio::io_service::strand m_strand;
		redisContext* m_redis_connect = NULL;	// redis连接
		Connection_T m_zdb_conn = NULL;			// 一个zdb链接
		bool m_is_work = false;
	};

	typedef boost::shared_ptr<redis_read_unit> redis_read_unit_ptr;
	typedef std::set<redis_read_unit_ptr> redis_read_unit_list;

	struct table_field_t
	{
		table_field_t(const std::string& table) : m_table_name(table) {};

		std::string m_table_name;
		std::string m_str_col;
		uint32_t m_cursor = 0;
		uint32_t m_max_cursor = 0;
	};

	typedef std::vector<table_field_t> table_field_vec;

private:
	bool init_read_unit(const std::string& host, int port, const char* passwd, int db, struct ConnectionPool_S* zdb_pool, uint32_t num);

	void start_read_from_db();
	bool do_read_single(const redis_read_unit_ptr& p_read_unit_ptr);
	bool is_table_end() const;
	void check_read_db_done();
	void do_read_finish(const boost::weak_ptr<redis_read_unit>& p_wunit, const std::string& table, const std::map<std::string, std::string>& datas);

public:
	void unit_read_finish(const redis_read_unit_ptr& p_read_unit_ptr, const std::string& table, const std::map<std::string, std::string>& datas);

private:
	boost::asio::io_service m_io_service;
	boost::asio::io_service::strand m_strand;
	boost::scoped_ptr<boost::asio::io_service::work> m_work;
	boost::thread_group m_thread_group;
	
	uint32_t m_read_unit_num = 0;
	redis_read_unit_list m_work_list;
	table_field_vec m_read_table_list;
	uint32_t m_curr_read_table = 0;

	bool m_is_init = false;
	bool m_is_close = true;
	boost::function<void()> m_call_back = NULL;
};

extern redis_read_t g_redis_read;

#endif//__REDIS_CLIENT_HPP__