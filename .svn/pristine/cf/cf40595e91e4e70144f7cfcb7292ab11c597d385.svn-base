#include "redis.hpp"
#include "log.hpp"
#include "redis_common.hpp"

redis_base_t::redis_base_t()
	: m_redis_connect(NULL)
{

}

redis_base_t::~redis_base_t()
{

}

bool redis_base_t::init(const char* host, int32_t port, const char* passwd, int32_t db)
{
	if (!connect(host, port, passwd, db)) {
		log_error("redis init failed");
		return false;
	}

	return true;
}

void redis_base_t::close()
{
	disconnect();
}

bool redis_base_t::connect(const char* host, int32_t port, const char* passwd, int32_t db)
{
	m_redis_connect = redisConnect(host, port);

	if (m_redis_connect == NULL) {
		log_error("m_redis_connect == NULL");
		return false;
	}

	if (m_redis_connect->err) {
		log_error("redis client connect error: %s", m_redis_connect->errstr);
		close();
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

void redis_base_t::disconnect()
{
	if (m_redis_connect != NULL) {
		redisFree(m_redis_connect);
		m_redis_connect = NULL;
	}
}

bool redis_base_t::save_to_hash_redis(const char* table, const std::string& key, const char* field, const char* value, uint32_t length, const bool binary/* = false*/)
{
	if (m_redis_connect == NULL) {
		return false;
	}

	if (length > CACHE_VALUEX_LEN_MA) {
		log_error("redis client set key[%s] value too long!", key.c_str());
		//return false;
	}

	redis_reply_t reply;
	if (binary) {
		reply.m_reply_data = (redisReply*)redisCommand(m_redis_connect, "HSET %s:%s %s %b", table, key.c_str(), field, value, length);
	} else {
		reply.m_reply_data = (redisReply*)redisCommand(m_redis_connect, "HSET %s:%s %s %s", table, key.c_str(), field, value);
	}

	if (!reply.m_reply_data || reply.m_reply_data->type == REDIS_REPLY_ERROR) {
		log_error("Redis reply Error!");
		return false;
	}

	return true;
}

bool redis_base_t::load_from_hash_redis(const char* table, const std::string& key, const char* field, std::string& result)
{
	if (m_redis_connect == NULL) {
		return false;
	}

	redis_reply_t reply;
	reply.m_reply_data = (redisReply*)redisCommand(m_redis_connect, "HGET %s:%s %s", table, key.c_str(), field);

	if (!reply.m_reply_data || reply.m_reply_data->type == REDIS_REPLY_ERROR) {
		log_error("Redis reply Error!");
		return false;
	}

	// 对于空说明不存在不做处理
	if (reply.m_reply_data->type != REDIS_REPLY_STRING) {
		return false;
	}

	result.assign(reply.m_reply_data->str, reply.m_reply_data->len);
	return true;
}

bool redis_base_t::check_key_exist(const std::string& key)
{
	if (m_redis_connect == NULL) {
		return false;
	}

	redis_reply_t reply;
	reply.m_reply_data = (redisReply*)redisCommand(m_redis_connect, "EXISTS %s", key.c_str());

	if (!reply.m_reply_data || reply.m_reply_data->type == REDIS_REPLY_ERROR) {
		log_error("Redis reply Error!");
		return false;
	}

	if (reply.m_reply_data->type != REDIS_REPLY_INTEGER) {
		return false;
	}

	return (reply.m_reply_data->integer != 0);
}

bool redis_base_t::check_key_exist(const char* table, const std::string& key)
{
	if (m_redis_connect == NULL) {
		return false;
	}

	redis_reply_t reply;
	reply.m_reply_data = (redisReply*)redisCommand(m_redis_connect, "EXISTS %s:%s", table, key.c_str());

	if (!reply.m_reply_data || reply.m_reply_data->type == REDIS_REPLY_ERROR) {
		log_error("Redis reply Error!");
		return false;
	}

	if (reply.m_reply_data->type != REDIS_REPLY_INTEGER) {
		return false;
	}

	return (reply.m_reply_data->integer != 0);
}

bool redis_base_t::check_key_exist(const char* table, const char* field, const std::string& key)
{
	if (m_redis_connect == NULL) {
		return false;
	}

	redis_reply_t reply;
	reply.m_reply_data = (redisReply*)redisCommand(m_redis_connect, "EXISTS %s:%s:%s", table, field, key.c_str());

	if (!reply.m_reply_data || reply.m_reply_data->type == REDIS_REPLY_ERROR) {
		log_error("Redis reply Error!");
		return false;
	}

	if (reply.m_reply_data->type != REDIS_REPLY_INTEGER) {
		return false;
	}

	return (reply.m_reply_data->integer != 0);
}

bool redis_base_t::get_keys_from_hash_redis(const char* table, std::vector<std::string>& ret)
{
	if (m_redis_connect == NULL) {
		return false;
	}

	redis_reply_t reply;
	reply.m_reply_data = (redisReply*)redisCommand(m_redis_connect, "KEYS %s:*", table);

	if (!reply.m_reply_data || reply.m_reply_data->type == REDIS_REPLY_ERROR) {
		log_error("Redis reply Error!");
		return false;
	}

	if (reply.m_reply_data->type != REDIS_REPLY_ARRAY) {
		return false;
	}

	for (size_t i = 0; i < reply.m_reply_data->elements; ++i) {
		redisReply* elem = reply.m_reply_data->element[i];
		if (NULL != elem) {
			ret.push_back(std::move(std::string(elem->str, elem->len)));
		}
	}

	return true;
}

bool redis_base_t::get_hash_all(const std::string& key, std::map<std::string, std::string>& ret)
{
	if (m_redis_connect == NULL) {
		return false;
	}

	redis_reply_t reply;
	reply.m_reply_data = (redisReply*)redisCommand(m_redis_connect, "HGETALL %s", key.c_str());

	if (reply.m_reply_data == NULL) {
		return false;
	}

	if (reply.m_reply_data->type != REDIS_REPLY_ARRAY) {
		return false;
	}

	std::string temp;
	for (size_t i = 0; i < reply.m_reply_data->elements; ++i) {
		redisReply* elem = reply.m_reply_data->element[i];
		if (NULL != elem) {
			if (i % 2 != 0) {
				ret.insert(std::make_pair(temp, std::string(elem->str, elem->len)));
			} else {
				temp = std::move(std::string(elem->str, elem->len));
			}
		}
	}

	return true;
}

bool redis_base_t::get_hash_all(const std::string& table, const std::string& key, std::map<std::string, std::string>& ret)
{
	if (m_redis_connect == NULL) {
		return false;
	}

	redis_reply_t reply;
	reply.m_reply_data = (redisReply*)redisCommand(m_redis_connect, "HGETALL %s:%s", table.c_str(), key.c_str());

	if (reply.m_reply_data == NULL) {
		return false;
	}

	if (reply.m_reply_data->type != REDIS_REPLY_ARRAY) {
		return false;
	}

	std::string temp;
	for (size_t i = 0; i < reply.m_reply_data->elements; ++i) {
		redisReply* elem = reply.m_reply_data->element[i];
		if (NULL != elem) {
			if (i % 2 != 0) {
				ret.insert(std::make_pair(temp, std::string(elem->str, elem->len)));
			} else {
				temp = std::move(std::string(elem->str, elem->len));
			}
		}
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////
bool redis_base_t::pop_set_member(const char* table, std::string& result)
{
	if (m_redis_connect == NULL) {
		return false;
	}

	redis_reply_t reply;
	reply.m_reply_data = (redisReply*)redisCommand(m_redis_connect, "SPOP %s", table);

	if (!reply.m_reply_data || reply.m_reply_data->type == REDIS_REPLY_ERROR) {
		log_error("Redis reply Error!");
		return false;
	}

	// 对于空说明不存在不做处理
	if (reply.m_reply_data->type != REDIS_REPLY_STRING) {
		return false;
	}

	result.assign(reply.m_reply_data->str, reply.m_reply_data->len);
	return true;
}

uint32_t redis_base_t::get_set_size(const char* table)
{
	if (m_redis_connect == NULL) {
		return 0;
	}

	redis_reply_t reply;
	reply.m_reply_data = (redisReply*)redisCommand(m_redis_connect, "SCARD %s", table);

	if (!reply.m_reply_data || reply.m_reply_data->type == REDIS_REPLY_ERROR) {
		log_error("Redis reply Error!");
		return 0;
	}

	if (reply.m_reply_data->type != REDIS_REPLY_INTEGER) {
		log_error("redis reply type != REDIS_REPLY_INTEGER!");
		return 0;
	}

	return reply.m_reply_data->integer;
}

bool redis_base_t::save_to_set(const char* table, const std::string& value)
{
	if (m_redis_connect == NULL) {
		return false;
	}

	redis_reply_t reply;
	reply.m_reply_data = (redisReply*)redisCommand(m_redis_connect, "SADD %s %s", table, value.c_str());

	if (!reply.m_reply_data || reply.m_reply_data->type == REDIS_REPLY_ERROR) {
		log_error("Redis reply Error!");
		return false;
	}

	return true;
}

bool redis_base_t::save_to_set(const char* table, const char* field, const std::string& key, const std::string& value)
{
	if (m_redis_connect == NULL) {
		return false;
	}

	redis_reply_t reply;
	reply.m_reply_data = (redisReply*)redisCommand(m_redis_connect, "SADD %s:%s:%s %s", table, field, key.c_str(), value.c_str());

	if (!reply.m_reply_data || reply.m_reply_data->type == REDIS_REPLY_ERROR) {
		log_error("Redis reply Error!");
		return false;
	}

	return true;
}

bool redis_base_t::remove_set_member(const char* table, const char* field, const std::string& key, const std::string& value)
{
	if (m_redis_connect == NULL) {
		return false;
	}

	redis_reply_t reply;
	reply.m_reply_data = (redisReply*)redisCommand(m_redis_connect, "SREM %s:%s:%s %s", table, field, key.c_str(), value.c_str());

	if (!reply.m_reply_data || reply.m_reply_data->type == REDIS_REPLY_ERROR) {
		log_error("Redis reply Error!");
		return false;
	}

	return true;
}

bool redis_base_t::get_keys_from_set(const char* table, const char* field, const std::string& key, std::vector<std::string>& ret)
{
	if (m_redis_connect == NULL) {
		return false;
	}

	redis_reply_t reply;
	reply.m_reply_data = (redisReply*)redisCommand(m_redis_connect, "SMEMBERS %s:%s:%s", table, field, key.c_str());

	if (!reply.m_reply_data || reply.m_reply_data->type == REDIS_REPLY_ERROR) {
		log_error("Redis reply Error!");
		return false;
	}

	if (reply.m_reply_data->type != REDIS_REPLY_ARRAY) {
		return false;
	}

	for (size_t i = 0; i < reply.m_reply_data->elements; ++i) {
		redisReply* elem = reply.m_reply_data->element[i];
		if (NULL != elem) {
			ret.push_back(std::move(std::string(elem->str, elem->len)));
		}
	}

	return true;
}

bool redis_base_t::remove_redis_key(const char* table, const std::string& key)
{
	if (m_redis_connect == NULL) {
		return false;
	}

	redis_reply_t reply;
	reply.m_reply_data = (redisReply*)redisCommand(m_redis_connect, "DEL %s:%s", table, key.c_str());

	if (!reply.m_reply_data || reply.m_reply_data->type == REDIS_REPLY_ERROR) {
		log_error("Redis reply Error!");
		return false;
	}

	if (reply.m_reply_data->type != REDIS_REPLY_INTEGER) {
		return false;
	}

	return (reply.m_reply_data->integer != 0);
}

bool redis_base_t::del_redis_key(const std::string& key)
{
	if (m_redis_connect == NULL) {
		return false;
	}

	redis_reply_t reply;
	reply.m_reply_data = (redisReply*)redisCommand(m_redis_connect, "DEL %s", key.c_str());

	if (!reply.m_reply_data || reply.m_reply_data->type == REDIS_REPLY_ERROR) {
		log_error("Redis reply Error!");
		return false;
	}

	if (reply.m_reply_data->type != REDIS_REPLY_INTEGER) {
		return false;
	}

	return (reply.m_reply_data->integer != 0);
}

bool redis_base_t::set_expire_second(const std::string& key, uint32_t time)
{
	if (m_redis_connect == NULL) {
		return false;
	}

	redis_reply_t reply;
	reply.m_reply_data = (redisReply*)redisCommand(m_redis_connect, "EXPIRE %s %u", key.c_str(), time);

	if (!reply.m_reply_data || reply.m_reply_data->type == REDIS_REPLY_ERROR) {
		log_error("Redis reply Error!");
		return false;
	}

	if (reply.m_reply_data->type != REDIS_REPLY_INTEGER) {
		return false;
	}

	return (reply.m_reply_data->integer != 0);
}

bool redis_base_t::check_set_key(const char* table, const std::string& key)
{
	if (m_redis_connect == NULL) {
		return false;
	}

	redis_reply_t reply;
	reply.m_reply_data = (redisReply*)redisCommand(m_redis_connect, "SISMEMBER %s %s", table, key.c_str());

	if (!reply.m_reply_data || reply.m_reply_data->type == REDIS_REPLY_ERROR) {
		log_error("Redis reply Error!");
		return false;
	}

	if (reply.m_reply_data->type != REDIS_REPLY_INTEGER) {
		return false;
	}

	return (reply.m_reply_data->integer != 0);
}

void redis_base_t::del_set_all(const char* table)
{
	if (m_redis_connect == NULL) {
		return;
	}

	redis_reply_t reply;
	reply.m_reply_data = (redisReply*)redisCommand(m_redis_connect, "DEL %s", table);

	if (!reply.m_reply_data || reply.m_reply_data->type == REDIS_REPLY_ERROR) {
		log_error("Redis reply Error!");
		return;
	}
}

//////////////////////////////////////////////////////////////////////////
bool redis_base_t::del_zset(const std::string& key)
{
	if (!m_redis_connect) return false;

	// 静默模式
	redis_reply_t reply;
	reply.m_reply_data = (redisReply*)redisCommand(m_redis_connect, "DEL %s", key.c_str());

	if (!reply.m_reply_data || reply.m_reply_data->type == REDIS_REPLY_ERROR) {
		log_error("Redis reply Error!");
		return false;
	}

	return true;
}

int32_t redis_base_t::get_zset_count(const std::string& key)
{
	if (!m_redis_connect) return 0;

	redis_reply_t reply;
	reply.m_reply_data = (redisReply*)redisCommand(m_redis_connect, "ZCARD %s", key.c_str());

	if (!reply.m_reply_data || reply.m_reply_data->type == REDIS_REPLY_ERROR) {
		log_error("Redis reply Error!");
		return 0;
	}

	if (reply.m_reply_data->type != REDIS_REPLY_INTEGER) {
		return 0;
	}

	return reply.m_reply_data->integer;
}

bool redis_base_t::do_zset_cmd(const std::string& cmd)
{
	if (!m_redis_connect) return false;

	redis_reply_t reply;
	reply.m_reply_data = (redisReply*)redisCommand(m_redis_connect, cmd.c_str());

	if (!reply.m_reply_data || reply.m_reply_data->type == REDIS_REPLY_ERROR) {
		log_error("Redis reply Error!");
		return false;
	}

	if (reply.m_reply_data->type == REDIS_REPLY_NIL) {
		// log_error("Redis reply nil!");
		return false;
	}

	return true;
}

bool redis_base_t::do_zset_cmd(const std::string& cmd, int32_t& value)
{
	if (!m_redis_connect) return false;

	redis_reply_t reply;
	reply.m_reply_data = (redisReply*)redisCommand(m_redis_connect, cmd.c_str());

	if (!reply.m_reply_data || reply.m_reply_data->type == REDIS_REPLY_ERROR) {
		log_error("Redis reply Error!");
		return false;
	}

	if (reply.m_reply_data->type == REDIS_REPLY_NIL) {
		// log_error("Redis reply nil!");
		return false;
	}

	if (reply.m_reply_data->type != REDIS_REPLY_INTEGER) {
		log_error("redis reply type != REDIS_REPLY_INTEGER!");
		return false;
	}

	value = reply.m_reply_data->integer;

	return true;
}

bool redis_base_t::do_zset_cmd(const std::string& cmd, std::string& value)
{
	if (!m_redis_connect) return false;

	redis_reply_t reply;
	reply.m_reply_data = (redisReply*)redisCommand(m_redis_connect, cmd.c_str());

	if (!reply.m_reply_data || reply.m_reply_data->type == REDIS_REPLY_ERROR) {
		log_error("redis reply Error!");
		return false;
	}

	if (reply.m_reply_data->type == REDIS_REPLY_NIL) {
		log_error("Redis reply nil!");
		return false;
	}

	if (reply.m_reply_data->type != REDIS_REPLY_STRING) {
		log_error("redis reply type != REDIS_REPLY_STRING!");
		return false;
	}

	value.assign(reply.m_reply_data->str, reply.m_reply_data->len);

	return true;
}

bool redis_base_t::do_zset_cmd(const std::string& cmd, std::vector<uint64_t>& result)
{
	if (!m_redis_connect) return false;

	redis_reply_t reply;
	reply.m_reply_data = (redisReply*)redisCommand(m_redis_connect, cmd.c_str());

	if (!reply.m_reply_data || reply.m_reply_data->type == REDIS_REPLY_ERROR) {
		log_error("redis reply Error!");
		return false;
	}

	if (reply.m_reply_data->type == REDIS_REPLY_NIL) {
		log_error("Redis reply nil!");
		return false;
	}

	if (reply.m_reply_data->type != REDIS_REPLY_ARRAY) {
		log_error("redis reply type != REDIS_REPLY_ARRAY!");
		return false;
	}

	for (size_t i = 0; i < reply.m_reply_data->elements; ++i) {
		redisReply* elem = reply.m_reply_data->element[i];
		if (NULL != elem) {
			uint64_t data = 0;
			if (unserialize_data(std::string(elem->str, elem->len), data)) {
				result.push_back(data);
			}
		}
	}

	return true;
}

bool redis_base_t::do_zset_cmd(const std::string& cmd, std::vector<std::pair<uint64_t, uint32_t>>& result)
{
	if (!m_redis_connect) return false;

	redis_reply_t reply;
	reply.m_reply_data = (redisReply*)redisCommand(m_redis_connect, cmd.c_str());

	if (!reply.m_reply_data || reply.m_reply_data->type == REDIS_REPLY_ERROR) {
		log_error("redis reply Error!");
		return false;
	}

	if (reply.m_reply_data->type == REDIS_REPLY_NIL) {
		log_error("Redis reply nil!");
		return false;
	}

	if (reply.m_reply_data->type != REDIS_REPLY_ARRAY) {
		log_error("redis reply type != REDIS_REPLY_ARRAY!");
		return false;
	}

	uint64_t uid = 0;
	for (size_t i = 0; i < reply.m_reply_data->elements; ++i) {
		redisReply* elem = reply.m_reply_data->element[i];
		if (NULL != elem) {
			if (i % 2 != 0) {
				uint32_t score = 0;
				if (unserialize_data(std::string(elem->str, elem->len), score)) {
					result.push_back(std::pair<uint64_t, uint32_t>(uid, score));
				}
				uid = 0;
			} else {
				unserialize_data(std::string(elem->str, elem->len), uid);
			}
		}
	}

	return true;
}

bool redis_base_t::batch_rem_zset_data_by_rank(const std::string& key, int32_t start, int32_t stop)
{
	std::ostringstream cmd;
	cmd << "ZREMRANGEBYRANK "
		<< key.c_str()
		<< " "
		<< start
		<< " "
		<< stop;

	return do_zset_cmd(cmd.str());
}

void redis_base_t::save_to_string_redis(const char* key, const char* value)
{
	if (!m_redis_connect) return;

	redis_reply_t reply;
	reply.m_reply_data = (redisReply*)redisCommand(m_redis_connect, "SET %s %s", key, value);

	if (!reply.m_reply_data || reply.m_reply_data->type == REDIS_REPLY_ERROR) {
		log_error("Redis reply Error!");
	}
}

bool redis_base_t::load_from_string_redis(const char* key, std::string& str)
{
	if (!m_redis_connect) return false;

	redis_reply_t reply;
	reply.m_reply_data = (redisReply*)redisCommand(m_redis_connect, "GET %s", key);

	if (!reply.m_reply_data || reply.m_reply_data->type == REDIS_REPLY_ERROR) {
		log_error("redis reply Error!");
		return false;
	}

	if (reply.m_reply_data->type == REDIS_REPLY_NIL) {
		// log_error("Redis reply nil!");
		return false;
	}

	if (reply.m_reply_data->type != REDIS_REPLY_STRING) {
		log_error("redis reply type != REDIS_REPLY_STRING!");
		return false;
	}

	str.assign(reply.m_reply_data->str, reply.m_reply_data->len);

	return true;
}

int32_t redis_base_t::get_list_len(const std::string& key)
{
	if (!m_redis_connect) return -1;

	redis_reply_t reply;
	reply.m_reply_data = (redisReply*)redisCommand(m_redis_connect, "LLEN %s", key.c_str());

	if (!reply.m_reply_data || reply.m_reply_data->type == REDIS_REPLY_ERROR) {
		log_error("Redis reply Error!");
		return -1;
	}

	if (reply.m_reply_data->type != REDIS_REPLY_INTEGER) {
		return -1;
	}

	return reply.m_reply_data->integer;
}

int32_t redis_base_t::do_list_lpush(const std::string& key, const std::string& values)
{
	if (!m_redis_connect) return -1;

	redis_reply_t reply;
	reply.m_reply_data = (redisReply*)redisCommand(m_redis_connect, "LPUSH %s %s", key.c_str(), values.c_str());

	if (!reply.m_reply_data || reply.m_reply_data->type == REDIS_REPLY_ERROR) {
		log_error("Redis reply Error!");
		return -1;
	}

	if (reply.m_reply_data->type != REDIS_REPLY_INTEGER) {
		return -1;
	}

	return reply.m_reply_data->integer;
}

int32_t redis_base_t::do_list_rpush(const std::string& key, const std::string& cmd)
{
	if (!m_redis_connect) return -1;

	redis_reply_t reply;
	reply.m_reply_data = (redisReply*)redisCommand(m_redis_connect, cmd.c_str());

	if (!reply.m_reply_data || reply.m_reply_data->type == REDIS_REPLY_ERROR) {
		log_error("Redis reply Error!");
		return -1;
	}

	if (reply.m_reply_data->type != REDIS_REPLY_INTEGER) {
		return -1;
	}

	return reply.m_reply_data->integer;
}

bool redis_base_t::do_list_rpop(const std::string& key, std::string& pop_value)
{
	if (m_redis_connect == NULL) {
		return false;
	}

	redis_reply_t reply;
	reply.m_reply_data = (redisReply*)redisCommand(m_redis_connect, "RPOP %s", key.c_str());

	if (!reply.m_reply_data || reply.m_reply_data->type == REDIS_REPLY_ERROR) {
		log_error("Redis reply Error!");
		return false;
	}

	// 对于空说明不存在不做处理
	if (reply.m_reply_data->type != REDIS_REPLY_STRING) {
		return false;
	}

	pop_value.assign(reply.m_reply_data->str, reply.m_reply_data->len);
	return true;
}

bool redis_base_t::do_list_index(const std::string& key, int32_t index, std::string& pop_value)
{
	if (m_redis_connect == NULL) {
		return false;
	}

	redis_reply_t reply;
	reply.m_reply_data = (redisReply*)redisCommand(m_redis_connect, "LINDEX %s %d", key.c_str(), index);

	if (!reply.m_reply_data || reply.m_reply_data->type == REDIS_REPLY_ERROR) {
		log_error("Redis reply Error!");
		return false;
	}

	// 对于空说明不存在不做处理
	if (reply.m_reply_data->type != REDIS_REPLY_STRING) {
		return false;
	}

	pop_value.assign(reply.m_reply_data->str, reply.m_reply_data->len);
	return true;
}
