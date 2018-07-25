#include "redis_cli.hpp"
#include "log.hpp"

redis_cli_t::redis_cli_t()
	: m_is_batch(false)
	, m_batch_cmd(0)
{

}

redis_cli_t::~redis_cli_t()
{

}

bool redis_cli_t::init(const char* host, int32_t port, const char* passwd, int32_t db, const sql_table *db_table)
{
	if (!redis_base_t::init(host, port, passwd, db)) {
		log_error("redis_cli_t connect redis error");
		return false;
	}

	// 初始化字段
	if (!init_table_field(db_table)) {
		log_error("redis_cli_t init_table_field error");
		return false;
	}

	return true;
}

void redis_cli_t::close()
{
	do_batch_end();

	redis_base_t::close();

	log_info("close redis_cli_t success!");
}

void redis_cli_t::do_batch_start()
{
	if (m_is_batch || m_batch_cmd != 0) {
		log_error("redis_cli_t::do_batch_start state error");
		do_batch_end();
	}

	m_is_batch = true;
}

void redis_cli_t::do_batch_end()
{
	if (m_batch_cmd > 0) {
		if (!m_batch_save.empty()) {
			for (std::set<std::pair<std::string, std::string>>::const_iterator citr = m_batch_save.begin();
				citr != m_batch_save.end(); ++citr) {
				const std::pair<std::string, std::string> save_single = *citr;
				if (REDIS_OK != redisAppendCommand(m_redis_connect, "SADD %s %s:u:%s", SAVE_SET_NAME.c_str(), save_single.first.c_str(), save_single.second.c_str())) {
					log_error("redis add save set error!");
				} else {
					++m_batch_cmd;
				}
			}

			m_batch_save.clear();
		}

		while (m_batch_cmd-- > 0) {
			redis_reply_t reply;
			int r = redisGetReply(m_redis_connect, (void **)&reply.m_reply_data);
			if (r == REDIS_ERR) {
				log_error("redis_cli_t::do_batch error");
			} else {
				if (!reply.m_reply_data || reply.m_reply_data->type == REDIS_REPLY_ERROR) {
					log_error("redis_cli_t::do_batch error");
				}
			}
		}
	}

	m_batch_cmd = 0;
	m_is_batch = false;
}

bool redis_cli_t::save_to_hash_redis(const char* table, const std::string& key, const char* field, const char* value, uint32_t length, const bool binary/* = false*/)
{
	if (m_redis_connect == NULL) {
		return false;
	}

	if (length > CACHE_VALUEX_LEN_MA) {
		log_error("redis_cli_t redisAppendCommand table[%s] filed[%s] key[%s] value too long!", table, field, key.c_str());
		//return false;
	}

	if (binary) {
		if (REDIS_OK != redisAppendCommand(m_redis_connect, "HSET %s:%s %s %b", table, key.c_str(), field, value, length)) {
			log_error("redis_cli_t redisAppendCommand table[%s] filed[%s] key[%s] value error!", table, field, key.c_str());
			return false;
		} else {
			++m_batch_cmd;
		}
	} else {
		if (REDIS_OK != redisAppendCommand(m_redis_connect, "HSET %s:%s %s %s", table, key.c_str(), field, value)) {
			log_error("redis_cli_t redisAppendCommand table[%s] filed[%s] key[%s] value error!", table, field, key.c_str());
			return false;
		} else {
			++m_batch_cmd;
		}
	}

	if (m_is_batch) {
		m_batch_save.insert(std::make_pair(table, key));
	} else {
		if (REDIS_OK != redisAppendCommand(m_redis_connect, "SADD %s %s:u:%s", SAVE_SET_NAME.c_str(), table, key.c_str())) {
			log_error("redis table[%s] key[%s] add save set error!", table, key.c_str());
		} else {
			++m_batch_cmd;
		}
	}

	table_mapping_field::const_iterator citr = m_table_mapping_field.find(table);
	if (citr != m_table_mapping_field.end()) {
		mapping_field_list::const_iterator field_citr = citr->second.find(field);
		if (field_citr != citr->second.end()) {
			if (REDIS_OK != redisAppendCommand(m_redis_connect, "SADD %s:%s:%s %s", table, field, value, key.c_str())) {
				log_error("redis_cli_t redisAppendCommand table[%s] filed[%s] key[%s] value mapping error!", table, field, key.c_str());
			} else {
				++m_batch_cmd;
			}
		}
	}

	// 不是批量直接写入
	if (!m_is_batch) do_batch_end();

	return true;
}

bool redis_cli_t::remove_redis_key(const char* table, const std::string& key)
{
	if (m_is_batch) { // 逻辑判断
		log_error("remove_redis_key table[%s] key[%s] is bacth state error", table, key.c_str());
		return false;
	}

	if (m_redis_connect == NULL) {
		return false;
	}

	if (!is_hash_exist(table, key)) {
		log_error("redis_cli_t::remove_redis_key table[%s] key[%s] not find", table, key.c_str());
		return false;
	}

	uint32_t cmd = 0;
	table_mapping_field::const_iterator citr = m_table_mapping_field.find(table);
	if (citr != m_table_mapping_field.end()) {
		// 先读取所有值
		std::map<std::string, std::string> field_value_list;
		for (mapping_field_list::const_iterator field_citr = citr->second.begin(); field_citr != citr->second.end(); ++field_citr) {
			const std::string& field = *field_citr;
			std::string result;
			if (load_from_hash_redis(table, key, field.c_str(), result)) {
				field_value_list.insert(std::make_pair(field, result));
			}
		}

		// 接着删除
		for (std::map<std::string, std::string>::const_iterator value_citr = field_value_list.begin();
			value_citr != field_value_list.end(); ++value_citr) {
			if (REDIS_OK != redisAppendCommand(m_redis_connect, "SREM %s:%s:%s %s", table, value_citr->first.c_str(), value_citr->second.c_str(), key.c_str())) {
				log_error("redis_cli_t redisAppendCommand del table[%s] filed[%s] key[%s] value[%s] mapping error!", table, value_citr->first.c_str(), key.c_str(), value_citr->second.c_str());
			} else {
				++cmd;
			}
		}
	}

	if (REDIS_OK != redisAppendCommand(m_redis_connect, "HSET %s:%s invalid 1", table, key.c_str())) {
		log_error("redis_cli_t redisAppendCommand table[%s] key[%s] invalid error!", table, key.c_str());
	} else {
		++cmd;
	}

	// 重新命名数据防止被再次加载
	if (REDIS_OK != redisAppendCommand(m_redis_connect, "RENAME %s:%s %s:d:%s", table, key.c_str(), table, key.c_str())) {
		log_error("redis_cli_t RENAME DEL table[%s] key[%s] INVALID error!", table, key.c_str());
	} else {
		++cmd;
	}

	if (REDIS_OK != redisAppendCommand(m_redis_connect, "SADD %s %s:d:%s", SAVE_SET_NAME.c_str(), table, key.c_str())) {
		log_error("redis table[%s] key[%s] add save set error!", table, key.c_str());
	} else {
		++cmd;
	}

	while (cmd-- > 0) {
		redis_reply_t reply;
		int r = redisGetReply(m_redis_connect, (void **)&reply.m_reply_data);
		if (r == REDIS_ERR) {
			log_error("redis_cli_t::remove_redis_key error");
		} else {
			if (!reply.m_reply_data || reply.m_reply_data->type == REDIS_REPLY_ERROR) {
				log_error("redis_cli_t::remove_redis_key error");
			}
		}
	}

	return true;
}

bool redis_cli_t::load_from_hash_redis(const char* table, const std::string& key, const char* field, std::string& result)
{
	if (m_is_batch) { // 逻辑判断
		log_error("load_from_hash_redis table[%s] key[%s] is bacth state error", table, key.c_str());
		return false;
	}

	return redis_base_t::load_from_hash_redis(table, key, field, result);
}

bool redis_cli_t::init_table_field(const sql_table *db_table)
{
	if (db_table == NULL) {
		log_error("redis_srv_t db_table null");
		return false;
	}

	const sql_table *temp = db_table;
	while (temp->table) {
		if (temp->fields != NULL && strlen(temp->fields) > 0) {
			std::vector<std::string> vec_data_key;
			common::string_util_t::split<std::string>(temp->fields, vec_data_key, ":");
			m_table_mapping_field[temp->table].insert(vec_data_key.begin(), vec_data_key.end());
		}

		temp++;
	}

	return true;
}
