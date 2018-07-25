#pragma once

#include "redis_cli.hpp"
#include <boost/asio/deadline_timer.hpp>
#include <boost/system/error_code.hpp>

class redis_client_t
{
public:
	// 初始化 redis-cli 阻塞等待
	static bool init(boost::asio::io_service& ios,	// io_service
		const char* host,							// redis ip
		int32_t port,								// redis port
		const char* passwd,							// redis auth
		int32_t db,									// redis select
		const sql_table *db_table,					// 需要加载的字段
		uint32_t overtime							// 连接超时时间
	);

	// 关闭
	static void close();

	// redis db state
	static uint32_t get_redis_state();

public:
	// 具体类型接口，方便调用
	template<typename KEY_TYPE>
	static bool set_string(const char* table, const char* field, const KEY_TYPE& key, const std::string& value);
	template<typename KEY_TYPE>
	static bool set_uint64(const char* table, const char* field, const KEY_TYPE& key, const uint64_t value);
	template<typename KEY_TYPE>
	static bool set_uint32(const char* table, const char* field, const KEY_TYPE& key, const uint32_t value);
	template<typename KEY_TYPE>
	static bool set_float(const char* table, const char* field, const KEY_TYPE& key, const float value);
	template<typename KEY_TYPE, typename PROTOBUF_TYPE>
	static bool set_protobuf(const char* table, const char* field, const KEY_TYPE& key, const PROTOBUF_TYPE& proto);

	// 具体类型接口，方便调用
	template<typename KEY_TYPE>
	static std::string get_string(const char* table, const char* field, const KEY_TYPE& key);
	template<typename KEY_TYPE>
	static uint64_t get_uint64(const char* table, const char* field, const KEY_TYPE& key);
	template<typename KEY_TYPE>
	static uint32_t get_uint32(const char* table, const char* field, const KEY_TYPE& key);
	template<typename KEY_TYPE>
	static float get_float(const char* table, const char* field, const KEY_TYPE& key);
	template<typename KEY_TYPE, typename PROTOBUF_TYPE>
	static bool get_protobuf(const char* table, const char* field, const KEY_TYPE& key, PROTOBUF_TYPE& result);

	// 根据字段值查找key
	template<typename KEY_TYPE, typename VALUE_TYPE, typename CONTAINER>
	static bool get_keys_by_field_condition(const char* table, const char* field, const VALUE_TYPE& conditon_value, CONTAINER& result);

	// 根据表名查找key
	template<typename KEY_TYPE, typename CONTAINER>
	static bool get_all_keys(const char* table, CONTAINER& result);

	// key是否存在
	template<typename KEY_TYPE>
	static bool is_hash_exist(const char* table, const KEY_TYPE& key);

	template<typename KEY_TYPE>
	static bool is_set_exist(const char* table, const char* field, const KEY_TYPE& key);

	// 删除key，并清理索引
	template<typename KEY_TYPE>
	static bool remove_key(const char* table, const KEY_TYPE& key);

	// 是否在set中
	template<typename KEY_TYPE>
	static bool is_set_member(const char* table, const KEY_TYPE& key);

	// 往set中加
	template<typename KEY_TYPE>
	static void add_set_member(const char* table, const KEY_TYPE& key);

	static void del_set_all(const char* table);

public:
	static redis_cli_t m_redis_cli;
};

template<typename KEY_TYPE>
bool redis_client_t::set_string(const char* table, const char* field, const KEY_TYPE& key, const std::string& value)
{
	return m_redis_cli.set_hash_string(table, field, key, value);
}

template<typename KEY_TYPE>
bool redis_client_t::set_uint64(const char* table, const char* field, const KEY_TYPE& key, const uint64_t value)
{
	return m_redis_cli.set_hash_uint64(table, field, key, value);
}

template<typename KEY_TYPE>
bool redis_client_t::set_uint32(const char* table, const char* field, const KEY_TYPE& key, const uint32_t value)
{
	return m_redis_cli.set_hash_uint32(table, field, key, value);
}

template<typename KEY_TYPE>
bool redis_client_t::set_float(const char* table, const char* field, const KEY_TYPE& key, const float value)
{
	return m_redis_cli.set_hash_float(table, field, key, value);
}

template<typename KEY_TYPE, typename PROTOBUF_TYPE>
bool redis_client_t::set_protobuf(const char* table, const char* field, const KEY_TYPE& key, const PROTOBUF_TYPE& proto)
{
	return m_redis_cli.set_hash_protobuf(table, field, key, proto);
}

template<typename KEY_TYPE>
std::string redis_client_t::get_string(const char* table, const char* field, const KEY_TYPE& key)
{
	return std::move(m_redis_cli.get_hash_string(table, field, key));
}

template<typename KEY_TYPE>
uint64_t redis_client_t::get_uint64(const char* table, const char* field, const KEY_TYPE& key)
{
	return m_redis_cli.get_hash_uint64(table, field, key);
}

template<typename KEY_TYPE>
uint32_t redis_client_t::get_uint32(const char* table, const char* field, const KEY_TYPE& key)
{
	return m_redis_cli.get_hash_uint32(table, field, key);
}

template<typename KEY_TYPE>
float redis_client_t::get_float(const char* table, const char* field, const KEY_TYPE& key)
{
	return m_redis_cli.get_hash_float(table, field, key);
}

template<typename KEY_TYPE, typename PROTOBUF_TYPE>
bool redis_client_t::get_protobuf(const char* table, const char* field, const KEY_TYPE& key, PROTOBUF_TYPE& result)
{
	return m_redis_cli.get_hash_protobuf(table, field, key, result);
}

template<typename KEY_TYPE, typename VALUE_TYPE, typename CONTAINER>
bool redis_client_t::get_keys_by_field_condition(const char* table, const char* field, const VALUE_TYPE& conditon_value, CONTAINER& result)
{
	return m_redis_cli.get_set_keys<KEY_TYPE, VALUE_TYPE, CONTAINER>(table, field, conditon_value, result);
}

template<typename KEY_TYPE, typename CONTAINER>
bool redis_client_t::get_all_keys(const char* table, CONTAINER& result)
{
	return m_redis_cli.get_all_hash_keys<KEY_TYPE, CONTAINER>(table, result);
}

template<typename KEY_TYPE>
bool redis_client_t::is_set_exist(const char* table, const char* field, const KEY_TYPE& key)
{
	return m_redis_cli.is_set_exist(table, field, key);
}

template<typename KEY_TYPE>
bool redis_client_t::is_hash_exist(const char* table, const KEY_TYPE& key)
{
	return m_redis_cli.is_hash_exist(table, key);
}

template<typename KEY_TYPE>
bool redis_client_t::remove_key(const char* table, const KEY_TYPE& key)
{
	return m_redis_cli.remove_key(table, key);
}

template<typename KEY_TYPE>
bool redis_client_t::is_set_member(const char* table, const KEY_TYPE& key)
{
	return m_redis_cli.is_set_member(table, key);
}

template<typename KEY_TYPE>
void redis_client_t::add_set_member(const char* table, const KEY_TYPE& key)
{
	m_redis_cli.add_set_member(table, key);
}

#define REDIS_BATCH_START redis_client_t::m_redis_cli.do_batch_start(); \
	{

#define REDIS_BATCH_DONE	} \
	redis_client_t::m_redis_cli.do_batch_end();
