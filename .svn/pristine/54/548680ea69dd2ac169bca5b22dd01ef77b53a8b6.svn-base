#pragma once

#include <redis/deps/hiredis/hiredis.h>
#include <google/protobuf/message.h>
#include <boost/noncopyable.hpp>

#include "utility.hpp"
#include "log.hpp"
#include <zlib.h>
static const uint32_t MAX_UZLIB_CHAR = 400 * 1024;							/**< 未压缩数据缓存 */
#define unzip_size(zip_size) ((zip_size) * 120 / 100 + 12)					/**< 压缩需要的缓存 > compressBound */
#define BLOB_ZIP 0
/**
 * \brief redis基类组件，封装了一些redis基本操作
 */

class redis_base_t : private boost::noncopyable
{
public:
	virtual ~redis_base_t();

	// 初始化
	virtual bool init(const char* host, int32_t port, const char* passwd, int32_t db);
	// 关闭
	virtual void close();

protected:
	redis_base_t();
	// 建立redis连接
	bool connect(const char* host, int32_t port, const char* passwd, int32_t db);
	// 断开
	void disconnect();

	//////////////////////////////////////////////////////////////////////////
	// Hash
public:
	// 具体类型接口，方便调用
	template<typename KEY_TYPE>
	bool set_hash_string(const char* table, const char* field, const KEY_TYPE& key, const std::string& value);
	template<typename KEY_TYPE>
	bool set_hash_uint64(const char* table, const char* field, const KEY_TYPE& key, const uint64_t value);
	template<typename KEY_TYPE>
	bool set_hash_uint32(const char* table, const char* field, const KEY_TYPE& key, const uint32_t value);
	template<typename KEY_TYPE>
	bool set_hash_float(const char* table, const char* field, const KEY_TYPE& key, const float value);
	template<typename KEY_TYPE, typename PROTOBUF_TYPE>
	bool set_hash_protobuf(const char* table, const char* field, const KEY_TYPE& key, const PROTOBUF_TYPE& proto);

	// 具体类型接口，方便调用
	template<typename KEY_TYPE>
	std::string get_hash_string(const char* table, const char* field, const KEY_TYPE& key);
	template<typename KEY_TYPE>
	uint64_t get_hash_uint64(const char* table, const char* field, const KEY_TYPE& key);
	template<typename KEY_TYPE>
	uint32_t get_hash_uint32(const char* table, const char* field, const KEY_TYPE& key);
	template<typename KEY_TYPE>
	float get_hash_float(const char* table, const char* field, const KEY_TYPE& key);
	template<typename KEY_TYPE, typename PROTOBUF_TYPE>
	bool get_hash_protobuf(const char* table, const char* field, const KEY_TYPE& key, PROTOBUF_TYPE& result);

	// key是否存在
	template<typename KEY_TYPE>
	bool is_hash_exist(const char* table, const KEY_TYPE& key);

	// 根据表名查找key(有性能问题只能在起服的时候用用)
	template<typename KEY_TYPE, typename CONTAINER>
	bool get_all_hash_keys(const char* table, CONTAINER& result);

protected:
	// 保存到redis操作
	virtual bool save_to_hash_redis(const char* table, const std::string& key, const char* field, const char* value, uint32_t length, const bool binary = false);

	// 从redis读取操作
	virtual bool load_from_hash_redis(const char* table, const std::string& key, const char* field, std::string& result);

	// 从redis读取一张表的keys，有性能问题，只能在起服的时候用下
	// redis原话:KEYS 的速度非常快，但在一个大的数据库中使用它仍然可能造成性能问题，如果你需要从一个数据集中查找特定的 key ，你最好还是用 Redis 的集合结构(set)来代替。
	bool get_keys_from_hash_redis(const char* table, std::vector<std::string>& ret);

	// 获得一个hash的所有键和值
	bool get_hash_all(const std::string& key, std::map<std::string, std::string>& ret);

	// 获得一个hash的所有键和值
	bool get_hash_all(const std::string& table, const std::string& key, std::map<std::string, std::string>& ret);

	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// set
public:
	// 是否在set中
	template<typename KEY_TYPE>
	bool is_set_member(const char* table, const KEY_TYPE& key);

	// 增加一个对象
	template<typename KEY_TYPE>
	void add_set_member(const char* table, const KEY_TYPE& key);

	// 增加一个对象
	template<typename KEY_TYPE, typename VALUE_TYPE>
	void add_set_member(const char* table, const char* field, const KEY_TYPE& key, const VALUE_TYPE& value);

	// 弹出一个对象
	bool pop_set_member(const char* table, std::string& result);

	// 元素个数
	uint32_t get_set_size(const char* table);

	// 返回所有索引元素
	template<typename KEY_TYPE, typename VALUE_TYPE, typename CONTAINER>
	bool get_set_keys(const char* table, const char* field, const VALUE_TYPE& key, CONTAINER& result);

	// 这个set字段是否存在
	template<typename KEY_TYPE>
	bool is_set_exist(const char* table, const char* field, const KEY_TYPE& key);

	// 删除一个set
	void del_set_all(const char* table);

protected:
	// 保存到set
	bool save_to_set(const char* table, const std::string& value);
	// 保存到set
	bool save_to_set(const char* table, const char* field, const std::string& key, const std::string& value);
	// 删除一个set的值
	bool remove_set_member(const char* table, const char* field, const std::string& key, const std::string& value);
	// 从指定set中取
	bool get_keys_from_set(const char* table, const char* field, const std::string& key, std::vector<std::string>& ret);

public:
	// 删除一个key
	template<typename KEY_TYPE>
	bool remove_key(const char* table, const KEY_TYPE& key);

protected:
	// 数据序列化
	template<typename SCORE_TYPE>
	inline std::string serialize_data(const SCORE_TYPE& value);

	// 数据反序列化
	template<typename TARGET_TYPE>
	inline bool unserialize_data(const std::string& data, TARGET_TYPE& result);

	// key是否存在
	bool check_key_exist(const std::string& key);

	// key是否存在
	bool check_key_exist(const char* table, const std::string& key);

	// key是否存在
	bool check_key_exist(const char* table, const char* field, const std::string& key);

	// 删除redis操作
	virtual bool remove_redis_key(const char* table, const std::string& key);

	// 是否是set中的元素
	bool check_set_key(const char* table, const std::string& key);

	// 删除一个redis的key(有些重复)
	bool del_redis_key(const std::string& key);

	// 设置key生存时间
	bool set_expire_second(const std::string& key, uint32_t time);

	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// redis sorted set 
public:
	// zset 是否存在
	bool is_zset_exist(const std::string& key) { return check_key_exist(key); }

	// del zset
	bool del_zset(const std::string& key);

	// get zset count
	int32_t get_zset_count(const std::string& key);

	// get zset count
	template<typename SCORE_TYPE>
	int32_t get_zset_count_by_score(const std::string& key, const SCORE_TYPE& min, const SCORE_TYPE& max);

	// add|update zset member
	template<typename SCORE_TYPE, typename MEMBER_TYPE>
	bool set_zset_data(const std::string& key, const SCORE_TYPE& score, const MEMBER_TYPE& member);

	// batch add zset member
	template<typename CONTAINER>
	bool batch_set_zset_data(const std::string& key, const CONTAINER& datas);

	// remove zset member
	template<typename MEMBER_TYPE>
	bool rem_zset_data(const std::string& key, const MEMBER_TYPE& member);

	// batch remove zset member [start,stop]
	bool batch_rem_zset_data_by_rank(const std::string& key, int32_t start, int32_t stop);

	// batch remove zset member [min,max]
	template<typename SCORE_TYPE>
	bool batch_rem_zset_data_by_score(const std::string& key, const SCORE_TYPE& min, const SCORE_TYPE& max);

	// is zset member exsit
	template<typename MEMBER_TYPE>
	bool is_zset_key_exsit(const std::string& key, const MEMBER_TYPE& member);

	// get zset member score
	template<typename SCORE_TYPE, typename MEMBER_TYPE>
	SCORE_TYPE get_zset_score(const std::string& key, const MEMBER_TYPE& member);

	// get zset member rank
	template<typename MEMBER_TYPE>
	int32_t get_zset_rank(const std::string& key, const MEMBER_TYPE& member);

	// get zset member revrank
	template<typename MEMBER_TYPE>
	int32_t get_zset_revrank(const std::string& key, const MEMBER_TYPE& member);

	// get zset member by rank [start,end]
	template<typename CONTAINER>
	bool get_zset_range(const std::string& key, int32_t start, int32_t stop, CONTAINER& result);

	// get zset member by rev rank [start,end]
	template<typename CONTAINER>
	bool get_zset_revrange(const std::string& key, int32_t start, int32_t stop, CONTAINER& result);

	template<typename CONTAINER>
	bool get_zset_revrange_withscores(const std::string& key, int32_t start, int32_t stop, CONTAINER& result);

	// get zset member by score [min,max]
	template<typename SCORE_TYPE, typename CONTAINER>
	bool get_zset_range_by_score(const std::string& key, const SCORE_TYPE& min, const SCORE_TYPE& max, CONTAINER& result);

	// get zset member by rev score [min,max]
	template<typename SCORE_TYPE, typename CONTAINER>
	bool get_zset_revrange_by_score(const std::string& key, const SCORE_TYPE& max, const SCORE_TYPE& min, CONTAINER& result);

protected:
	bool do_zset_cmd(const std::string& cmd);
	bool do_zset_cmd(const std::string& cmd, int32_t& value);
	bool do_zset_cmd(const std::string& cmd, std::string& value);
	bool do_zset_cmd(const std::string& cmd, std::vector<uint64_t>& result);
	bool do_zset_cmd(const std::string& cmd, std::vector<std::pair<uint64_t, uint32_t>>& result);

	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// string
public:
	// 添加一个string
	template<typename VALUE_TYPE>
	void set_string(const char* key, const VALUE_TYPE& value);

	template<typename VALUE_TYPE>
	VALUE_TYPE get_string(const char* key);

protected:
	void save_to_string_redis(const char* key, const char* value);

	bool load_from_string_redis(const char* key, std::string& str);
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// redis list
public:
	// get list len
	int32_t get_list_len(const std::string& key);

	// Left push
	template<typename VALUE_TYPE>
	int32_t list_lpush(const std::string& key, const VALUE_TYPE& value);

	// batch Left push
	template<typename CONTAINER>
	int32_t batch_list_lpush(const std::string& key, const CONTAINER& values);

	// right pop
	template<typename VALUE_TYPE>
	VALUE_TYPE list_rpop(const std::string& key);

	// index
	template<typename VALUE_TYPE>
	VALUE_TYPE list_index(const std::string& key, int32_t index);

protected:
	int32_t do_list_lpush(const std::string& key, const std::string& values);
	int32_t do_list_rpush(const std::string& key, const std::string& cmd);
	bool do_list_rpop(const std::string& key, std::string& pop_value);
	bool do_list_index(const std::string& key, int32_t index, std::string& pop_value);

	//////////////////////////////////////////////////////////////////////////

protected:
	class redis_reply_t
	{
	public:
		redis_reply_t() : m_reply_data(NULL)
		{
		}

		~redis_reply_t()
		{
			if (m_reply_data != NULL) {
				freeReplyObject(m_reply_data);
				m_reply_data = NULL;
			}
		}

	public:
		redisReply* m_reply_data;
	};

protected:
	// redis连接
	redisContext* m_redis_connect = NULL;
};

template<typename KEY_TYPE>
bool redis_base_t::is_set_member(const char* table, const KEY_TYPE& key)
{
	std::string _key = std::move(serialize_data(key));
	return check_set_key(table, _key);
}

template<typename KEY_TYPE>
bool redis_base_t::remove_key(const char* table, const KEY_TYPE& key)
{
	std::string _key = std::move(serialize_data(key));
	return remove_redis_key(table, _key);
}

template<typename KEY_TYPE>
bool redis_base_t::set_hash_string(const char* table, const char* field, const KEY_TYPE& key, const std::string& value)
{
	std::string _key = std::move(serialize_data(key));
	return save_to_hash_redis(table, _key, field, value.c_str(), value.length());
}

template<typename KEY_TYPE>
bool redis_base_t::set_hash_uint64(const char* table, const char* field, const KEY_TYPE& key, const uint64_t value)
{
	std::string _key = std::move(serialize_data(key));
	std::string _value = std::move(serialize_data(value));
	return save_to_hash_redis(table, _key, field, _value.c_str(), _value.length());
}

template<typename KEY_TYPE>
bool redis_base_t::set_hash_uint32(const char* table, const char* field, const KEY_TYPE& key, const uint32_t value)
{
	std::string _key = std::move(serialize_data(key));
	std::string _value = std::move(serialize_data(value));
	return save_to_hash_redis(table, _key, field, _value.c_str(), _value.length());
}

template<typename KEY_TYPE>
bool redis_base_t::set_hash_float(const char* table, const char* field, const KEY_TYPE& key, const float value)
{
	std::string _key = std::move(serialize_data(key));
	std::string _value = std::move(serialize_data(value));
	return save_to_hash_redis(table, _key, field, _value.c_str(), _value.length());
}

template<typename KEY_TYPE, typename PROTOBUF_TYPE>
bool redis_base_t::set_hash_protobuf(const char* table, const char* field, const KEY_TYPE& key, const PROTOBUF_TYPE& proto)
{
	std::string _key = std::move(serialize_data(key));
	std::string _value = proto.SerializeAsString();
#if BLOB_ZIP
	uint32_t uzSize = _value.length();
	uLong destLen = unzip_size(uzSize);
	char zlibBuf[destLen];
	int32_t retcode = compress((unsigned char *)zlibBuf[0], &destLen, (const Bytef *)_value.c_str(), uzSize);
	switch (retcode) {
		case Z_OK:
			break;
		case Z_MEM_ERROR:
			log_error("table[%s:%s] (%s)Z_MEM_ERROR.", table, field, __PRETTY_FUNCTION__);
			break;
		case Z_BUF_ERROR:
			log_error("table[%s:%s] (%s)Z_BUF_ERROR.", table, field, __PRETTY_FUNCTION__);
			break;
	}
	// log_warn("table[%s:%s] set_protobuf[%d]", table, field, destLen);
	return save_to_hash_redis(table, _key, field, zlibBuf, destLen, true);
#else
	return save_to_hash_redis(table, _key, field, _value.c_str(), _value.length(), true);
#endif
	
}

template<typename KEY_TYPE>
std::string redis_base_t::get_hash_string(const char* table, const char* field, const KEY_TYPE& key)
{
	std::string _key = std::move(serialize_data(key));
	std::string _result("");
	load_from_hash_redis(table, _key, field, _result);
	return std::move(_result);
}

template<typename KEY_TYPE>
uint64_t redis_base_t::get_hash_uint64(const char* table, const char* field, const KEY_TYPE& key)
{
	std::string _key = std::move(serialize_data(key));
	std::string _result;
	if (!load_from_hash_redis(table, _key, field, _result)) {
		return 0;
	}

	if (_result.empty()) {
		return 0;
	}

	uint64_t result = 0;
	if (!unserialize_data(_result, result)) {
		return 0;
	}

	return result;
}

template<typename KEY_TYPE>
uint32_t redis_base_t::get_hash_uint32(const char* table, const char* field, const KEY_TYPE& key)
{
	std::string _key = std::move(serialize_data(key));
	std::string _result;
	if (!load_from_hash_redis(table, _key, field, _result)) {
		return 0;
	}

	if (_result.empty()) {
		return 0;
	}

	uint32_t result = 0;
	if (!unserialize_data(_result, result)) {
		return 0;
	}

	return result;
}

template<typename KEY_TYPE>
float redis_base_t::get_hash_float(const char* table, const char* field, const KEY_TYPE& key)
{
	std::string _key = std::move(serialize_data(key));
	std::string _result;
	if (!load_from_hash_redis(table, _key, field, _result)) {
		return 0.f;
	}

	if (_result.empty()) {
		return 0.f;
	}

	float result = 0.f;
	if (!unserialize_data(_result, result)) {
		return 0.f;
	}

	return result;
}

template<typename KEY_TYPE, typename PROTOBUF_TYPE>
bool redis_base_t::get_hash_protobuf(const char* table, const char* field, const KEY_TYPE& key, PROTOBUF_TYPE& result)
{
	std::string _key = std::move(serialize_data(key));
	std::string _result;
	if (!load_from_hash_redis(table, _key, field, _result)) {
		return false;
	}

	if (_result.empty()) {
		return false;
	}
#if BLOB_ZIP
	char uzBuf[MAX_UZLIB_CHAR];
	bzero(uzBuf, sizeof(uzBuf));
	uLongf bufSize = MAX_UZLIB_CHAR;
	int32_t retcode = uncompress((unsigned char *)uzBuf, &bufSize, (const Bytef *)_result.c_str(), _result.length());
	switch (retcode)
	{
		case Z_OK:
			break;
		case Z_MEM_ERROR:
			log_error("table[%s:%s] (%s)Z_MEM_ERROR.", table, field, __PRETTY_FUNCTION__);
			break;
		case Z_BUF_ERROR:
			log_error("table[%s:%s] (%s)Z_BUF_ERROR.", table, field, __PRETTY_FUNCTION__);
			break;
		case Z_DATA_ERROR:
			log_error("table[%s:%s] (%s)Z_DATA_ERROR.", table, field, __PRETTY_FUNCTION__);
			break;
	}
	return result.ParseFromString(uzBuf, bufSize);
#else
	return result.ParseFromString(_result);
#endif
	
}

template<typename KEY_TYPE>
bool redis_base_t::is_hash_exist(const char* table, const KEY_TYPE& key)
{
	std::string _key = std::move(serialize_data(key));
	return check_key_exist(table, _key);
}

template<typename KEY_TYPE>
bool redis_base_t::is_set_exist(const char* table, const char* field, const KEY_TYPE& key)
{
	std::string _key = std::move(serialize_data(key));
	return check_key_exist(table, field, _key);
}

template<typename KEY_TYPE, typename CONTAINER>
bool redis_base_t::get_all_hash_keys(const char* table, CONTAINER& result)
{
	std::vector<std::string> _result;
	if (!get_keys_from_hash_redis(table, _result)) {
		return false;
	}

	std::vector<std::string> vec_data_key;
	for (std::vector<std::string>::const_iterator citr = _result.begin();
		citr != _result.end(); ++citr) {
		common::string_util_t::split<std::string>(*citr, vec_data_key, ":");
		if (vec_data_key.size() != 2) continue;
		KEY_TYPE key;
		unserialize_data(vec_data_key[1], key);
		result.push_back(key);
	}

	return true;
}

template<typename SCORE_TYPE>
inline std::string redis_base_t::serialize_data(const SCORE_TYPE& value)
{
	return std::move(boost::lexical_cast<std::string>(value));
}

template<typename TARGET_TYPE>
inline bool redis_base_t::unserialize_data(const std::string& data, TARGET_TYPE& result)
{
	try {
		result = boost::lexical_cast<TARGET_TYPE>(data);
	} catch (boost::bad_lexical_cast&) {
		// 异常直接不处理
		return false;
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////
template<typename KEY_TYPE>
void redis_base_t::add_set_member(const char* table, const KEY_TYPE& key)
{
	std::string _key = std::move(serialize_data(key));
	save_to_set(table, _key);
}

template<typename KEY_TYPE, typename VALUE_TYPE>
void redis_base_t::add_set_member(const char* table, const char* field, const KEY_TYPE& key, const VALUE_TYPE& value)
{
	std::string _key = std::move(serialize_data(key));
	std::string _value = std::move(serialize_data(value));
	save_to_set(table, field, _key, _value);
}

template<typename KEY_TYPE, typename VALUE_TYPE, typename CONTAINER>
bool redis_base_t::get_set_keys(const char* table, const char* field, const VALUE_TYPE& key, CONTAINER& result)
{
	std::string _key = std::move(serialize_data(key));
	std::vector<std::string> _result;
	if (!get_keys_from_set(table, field, _key, _result)) {
		return false;
	}

	for (std::vector<std::string>::const_iterator citr = _result.begin();
		citr != _result.end(); ++citr) {
		KEY_TYPE value;
		unserialize_data(*citr, value);
		result.push_back(value);
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////
template<typename SCORE_TYPE>
int32_t redis_base_t::get_zset_count_by_score(const std::string& key, const SCORE_TYPE& min, const SCORE_TYPE& max)
{
	std::ostringstream cmd;
	cmd << "ZCOUNT "
		<< key.c_str()
		<< " "
		<< min
		<< " "
		<< max;

	int32_t count = 0;
	do_zset_cmd(cmd.str(), count);
	return count;
}

template<typename SCORE_TYPE, typename MEMBER_TYPE>
bool redis_base_t::set_zset_data(const std::string& key, const SCORE_TYPE& score, const MEMBER_TYPE& member)
{
	std::ostringstream cmd;
	cmd << "ZADD "
		<< key.c_str()
		<< " "
		<< score
		<< " "
		<< member;

	return do_zset_cmd(cmd.str());
}

template<typename CONTAINER>
bool redis_base_t::batch_set_zset_data(const std::string& key, const CONTAINER& datas)
{
	if (datas.empty()) return false;

	// 目前是用拼接长字符串实现
	// 后期可以用管线，redisGetReply
	std::ostringstream cmd;
	cmd << "ZADD "
		<< key.c_str();

	for (const auto& data : datas) {
		cmd << " "
			<< data.second
			<< " "
			<< data.first;
	}

	return do_zset_cmd(cmd.str());
}

template<typename MEMBER_TYPE>
bool redis_base_t::rem_zset_data(const std::string& key, const MEMBER_TYPE& member)
{
	std::ostringstream cmd;
	cmd << "ZREM "
		<< key.c_str()
		<< " "
		<< member;

	int32_t count = 0;
	return do_zset_cmd(cmd.str(), count);
}

template<typename SCORE_TYPE>
bool redis_base_t::batch_rem_zset_data_by_score(const std::string& key, const SCORE_TYPE& min, const SCORE_TYPE& max)
{
	std::ostringstream cmd;
	cmd << "ZREMRANGEBYSCORE "
		<< key.c_str()
		<< " "
		<< min
		<< " "
		<< max;

	int32_t count = 0;
	return do_zset_cmd(cmd.str(), count);
}

template<typename MEMBER_TYPE>
bool redis_base_t::is_zset_key_exsit(const std::string& key, const MEMBER_TYPE& member)
{
	std::ostringstream cmd;
	cmd << "ZSCORE "
		<< key.c_str()
		<< " "
		<< member;

	return do_zset_cmd(cmd.str());
}

template<typename uint32_t, typename MEMBER_TYPE>
uint32_t redis_base_t::get_zset_score(const std::string& key, const MEMBER_TYPE& member)
{
	std::ostringstream cmd;
	cmd << "ZSCORE "
		<< key.c_str()
		<< " "
		<< member;

	std::string str;
	if (!do_zset_cmd(cmd.str(), str)) {
		return 0;
	}

	if (str.empty()) {
		return 0;
	}

	uint32_t result = 0;
	unserialize_data(str, result);
	return result;
}

template<typename MEMBER_TYPE>
int32_t redis_base_t::get_zset_rank(const std::string& key, const MEMBER_TYPE& member)
{
	std::ostringstream cmd;
	cmd << "ZRANK "
		<< key.c_str()
		<< " "
		<< member;

	int32_t rank;
	if (!do_zset_cmd(cmd.str(), rank)) {
		return -1;
	}

	return rank;
}

template<typename MEMBER_TYPE>
int32_t redis_base_t::get_zset_revrank(const std::string& key, const MEMBER_TYPE& member)
{
	std::ostringstream cmd;
	cmd << "ZREVRANK "
		<< key.c_str()
		<< " "
		<< member;

	int32_t rank;
	if (!do_zset_cmd(cmd.str(), rank)) {
		return -1;
	}

	return rank;
}

template<typename CONTAINER>
bool redis_base_t::get_zset_range(const std::string& key, int32_t start, int32_t stop, CONTAINER& result)
{
	std::ostringstream cmd;
	cmd << "ZRANGE "
		<< key.c_str()
		<< " "
		<< start
		<< " "
		<< stop;

	return do_zset_cmd(cmd.str(), result);
}

template<typename CONTAINER>
bool redis_base_t::get_zset_revrange(const std::string& key, int32_t start, int32_t stop, CONTAINER& result)
{
	std::ostringstream cmd;
	cmd << "ZREVRANGE "
		<< key.c_str()
		<< " "
		<< start
		<< " "
		<< stop;

	return do_zset_cmd(cmd.str(), result);
}

template<typename CONTAINER>
bool redis_base_t::get_zset_revrange_withscores(const std::string& key, int32_t start, int32_t stop, CONTAINER& result)
{
	std::ostringstream cmd;
	cmd << "ZREVRANGE "
		<< key.c_str()
		<< " "
		<< start
		<< " "
		<< stop
		<< " WITHSCORES";

	return do_zset_cmd(cmd.str(), result);
}

template<typename SCORE_TYPE, typename CONTAINER>
bool redis_base_t::get_zset_range_by_score(const std::string& key, const SCORE_TYPE& min, const SCORE_TYPE& max, CONTAINER& result)
{
	if (min > max) {
		log_error("redis get_zset_range_by_score min > max");
		return false;
	}

	std::ostringstream cmd;
	cmd << "ZRANGEBYSCORE "
		<< key.c_str()
		<< " "
		<< min
		<< " "
		<< max;

	return do_zset_cmd(cmd.str(), result);
}

template<typename SCORE_TYPE, typename CONTAINER>
bool redis_base_t::get_zset_revrange_by_score(const std::string& key, const SCORE_TYPE& max, const SCORE_TYPE& min, CONTAINER& result)
{
	if (min > max) {
		log_error("redis get_zset_revrange_by_score min > max");
		return false;
	}

	std::ostringstream cmd;
	cmd << "ZREVRANGEBYSCORE "
		<< key.c_str()
		<< " "
		<< max
		<< " "
		<< min;

	return do_zset_cmd(cmd.str(), result);
}

template<typename VALUE_TYPE>
void redis_base_t::set_string(const char* key, const VALUE_TYPE& value)
{
	std::string _value = std::move(serialize_data(value));
	save_to_string_redis(key, _value.c_str());
}

template<>
inline uint32_t redis_base_t::get_string(const char* key)
{
	std::string str;
	if (!load_from_string_redis(key, str)) {
		return 0;
	}

	if (str.empty()) {
		return 0;
	}

	uint32_t result = 0;
	unserialize_data(str, result);
	return result;
}

template<>
inline std::string redis_base_t::get_string(const char* key)
{
	std::string result;
	load_from_string_redis(key, result);
	return std::move(result);
}

template<>
inline uint64_t redis_base_t::get_string(const char* key)
{
	std::string str;
	if (!load_from_string_redis(key, str)) {
		return 0;
	}

	if (str.empty()) {
		return 0;
	}

	uint64_t result = 0;
	unserialize_data(str, result);
	return result;
}

template<typename VALUE_TYPE>
int32_t redis_base_t::list_lpush(const std::string& key, const VALUE_TYPE& value)
{
	std::string _value = std::move(serialize_data(value));
	return do_list_lpush(key, _value);
}

template<typename CONTAINER>
int32_t redis_base_t::batch_list_lpush(const std::string& key, const CONTAINER& values)
{
	if (values.empty()) return -1;

	std::ostringstream cmd;
	cmd << "RPUSH " << key.c_str();

	for (const auto& value : values) {
		cmd << " " << value;
	}

	return do_list_rpush(key, cmd.str());
}

template<>
inline uint64_t redis_base_t::list_rpop(const std::string& key)
{
	std::string _result("");
	if (!do_list_rpop(key, _result)) {
		return 0;
	}

	uint64_t result = 0;
	unserialize_data(_result, result);
	return result;
}

template<>
inline uint64_t redis_base_t::list_index(const std::string& key, int32_t index)
{
	std::string _result("");
	if (!do_list_index(key, index, _result)) {
		return 0;
	}

	uint64_t result = 0;
	unserialize_data(_result, result);
	return result;
}
