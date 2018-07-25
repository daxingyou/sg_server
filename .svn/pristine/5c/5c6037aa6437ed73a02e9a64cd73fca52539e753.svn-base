#include "redis_db.hpp"
#include "protos_fwd.hpp"
#include "redis_data_manager.hpp"
#include "main/db_server.hpp"
#include "main/db_server_fwd.hpp"

redis_db_t g_redis_db;

redis_db_t::redis_db_t()
{

}

redis_db_t::~redis_db_t()
{

}

bool redis_db_t::load_user_from_db(uint64_t uid, uint32_t game_id)
{
	if (!is_init_done()) {
		log_error("redis db is not init done");
		return false;
	}

	redis_db_read_ptr p_unit = m_read_unit_list[m_read_step]->get_db_read_ptr();
	if (!p_unit) {
		log_error("redis_db_read_ptr null error");
		return false;
	}

	p_unit->post_read_user_data(uid, game_id);

	m_read_step++;
	m_read_step = m_read_step % m_read_unit_list.size();
	return true;
}

bool redis_db_t::init_table_field(struct ConnectionPool_S* zdb_pool, const sql_table *db_table)
{
	if (!redis_srv_t::init_table_field(zdb_pool, db_table)) {
		return false;
	}

	Connection_T conn = common::zdb_util_t::get_zdb_conn(zdb_pool);
	if (NULL == conn) {
		log_error("redis_db_t init_table_field conn is null");
		return false;
	}

	bool ret = true;
	TRY
	{
		ResultSet_T result = Connection_executeQuery(conn, "SELECT `rk` FROM global_user WHERE last_login_time>%u ORDER BY last_login_time desc;", common::time_util_t::now_time() - env::xml->max_preloading_time * DAY_SECOND);
		while (ResultSet_next(result)) {
			uint64_t uid = ResultSet_getLLong(result, 1);
			m_preloading_uids.push_back(uid);

			// 超过预加载最大数
			if (env::xml->max_preloading_count != 0 && m_preloading_uids.size() >= env::xml->max_preloading_count) {
				break;
			}
		}
	}
	CATCH(SQLException)
	{
		log_error("redis_db_t init_table_field error: %s", Exception_frame.message);
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

	Connection_close(conn);
	return true;
}

bool redis_db_t::init_read_unit_list(const char* host, int32_t port, const char* passwd, int32_t db, struct ConnectionPool_S* zdb_pool, uint32_t num)
{
	if (NULL == zdb_pool) {
		log_error("NULL == zdb_pool");
		return false;
	}

	if (num == 0) {
		log_error("read thread num == 0");
		return false;
	}

	for (uint32_t i = 0; i < num; ++i) {
		redis_db_read_ptr p_unit(new redis_db_read_t(m_io_service, *this));
		if (!p_unit) {
			log_error("redis_read_unit_ptr null error");
			return false;
		}

		if (!p_unit->init(host, port, passwd, db, zdb_pool)) {
			log_error("redis_read_unit_ptr init error");
			return false;
		}

		m_read_unit_list.push_back(p_unit);
	}

	return true;
}

bool redis_db_t::start_read_extra_data()
{
	if (m_read_unit_list.empty()) {
		log_error("read thread num == 0");
		return false;
	}

	if (m_preloading_uids.empty()) {
		return true;
	}

#if 0 // 多线程的处理
	// 将预加载玩家平均分配给所有读取单元
	uint32_t each_num = m_preloading_uids.size() / m_read_unit_list.size();
	if (each_num < 1) each_num = 1;
	uint32_t cur_row = 0;
	uint32_t count = 0;
	for (const auto& p_unit_ptr : m_read_unit_list) {
		if (p_unit_ptr == NULL) {
			log_error("p_unit_ptr null error");
			return;
		}

		redis_db_read_ptr p_read_unit_ptr = p_unit_ptr->get_db_read_ptr();
		if (p_read_unit_ptr == NULL) {
			log_error("p_unit_ptr null error");
			return;
		}

		++count;
		if (count >= m_read_unit_list.size()) { // 最后一个把所有该读的读取
			std::vector<uint64_t> uids(m_preloading_uids.begin() + cur_row, m_preloading_uids.end());
			p_read_unit_ptr->post_pre_read_user_data(uids);
			break;
		} else {
			std::vector<uint64_t> uids(m_preloading_uids.begin() + cur_row, m_preloading_uids.begin() + cur_row + each_num);
			p_read_unit_ptr->post_pre_read_user_data(uids);
			cur_row += each_num;

			if (cur_row >= m_preloading_uids.size()) {
				break;
			}
		}
	}
#else
	redis_read_unit_ptr p_read_unit = m_read_unit_list.at(0); // 取第一个
	if (!p_read_unit) {
		log_error("p_read_unit null error");
		return false;
	}

	redis_db_read_ptr p_read_unit_ptr = p_read_unit->get_db_read_ptr();
	if (!p_read_unit_ptr) {
		log_error("p_read_unit_ptr null error");
		return false;
	}

	Connection_T conn = common::zdb_util_t::get_zdb_conn(p_read_unit_ptr->get_zdb_pool());
	if (conn) {
		for (const auto& uid : m_preloading_uids) {
			if (!p_read_unit_ptr->do_read_user_data(uid, conn)) {
				log_error("user[%lu] load data from sql failed", uid);
			}
		}

		Connection_close(conn);
	} else {
		log_error("redis_db_read_t read_user_data conn is null");
		return false;
	}
#endif

	return true;
}

//////////////////////////////////////////////////////////////////////////

redis_db_read_t::redis_db_read_t(boost::asio::io_service& ios, redis_srv_t& server)
	: redis_read_unit_t(ios, server)
{

}

redis_db_read_t::~redis_db_read_t()
{

}

void redis_db_read_t::post_read_user_data(uint64_t uid, uint32_t game_id)
{
	m_strand.post(boost::bind(&redis_db_read_t::read_user_data, this, uid, game_id));
}

void redis_db_read_t::read_user_data(uint64_t uid, uint32_t game_id)
{
	bool is_success = false;
	Connection_T conn = common::zdb_util_t::get_zdb_conn(m_zdb_pool);
	if (conn) {
		is_success = do_read_user_data(uid, conn);
		if (!is_success) {
			log_error("user[%lu] load data from sql failed", uid);
		}

		Connection_close(conn);
	} else {
		log_error("redis_db_read_t read_user_data conn is null");
	}

	env::server->get_ios().post(boost::bind(&redis_data_manager_t::load_data_from_db_done, uid, is_success, game_id));
}

bool redis_db_read_t::do_read_user_data(uint64_t uid, const Connection_T& conn)
{
	if (!m_redis_connect) {
		log_error("m_redis_connect null error");
		return false;
	}

	if (!conn) {
		log_error("conn null error");
		return false;
	}

	// 先检测这个玩家在db中是否存在
	TRY
	{
		ResultSet_T result = Connection_executeQuery(conn, "SELECT COUNT(1) FROM `role` WHERE rk=%lu AND invalid=0;", uid);
		while (ResultSet_next(result)) {
			uint32_t count = ResultSet_getInt(result, 1);
			if (count == 0) {
				log_error("role[%lu] not find in db", uid);
				return false;
			} else if (count > 1) {
				log_error("role[%lu] count[%u] > 1 in db", uid, count);
			}
		}
	}
	CATCH(SQLException)
	{
		log_error("redis_db_read_t do_read_user_data role[%lu] error: %s", uid, Exception_frame.message);
		return false;
	}
	FINALLY
	{
	}
	END_TRY;

	// log_debug("start load user[%lu] data from db to redis", uid);

	const redis_srv_t::table_list& all_table = m_redis_server.get_table_list();
	if (all_table.empty()) {
		log_error("table_list is empty!");
		return false;
	}

	for (redis_srv_t::table_list::const_iterator citr = all_table.begin();
		citr != all_table.end(); ++citr) {
		if (citr->second.m_is_loaded) {
			continue;
		}

		const std::string& table = citr->first;
		const redis_srv_t::table_data_t& table_data = citr->second;

		if (table_data.m_where.empty()) {
			log_error("do_read_user_data table[%s] where is empty", table.c_str());
			continue;
		}

		std::ostringstream temp;
		temp << "SELECT * FROM %s WHERE invalid=0 AND " << table_data.m_where << ";";
		std::string sql_str = std::move(temp.str());
		TRY
		{
			ResultSet_T result = Connection_executeQuery(conn, sql_str.c_str(), table.c_str(), uid);
		while (ResultSet_next(result)) {
			int index = 1;
			uint32_t cmd = 0;
			bool is_key_read = false;
			std::string key_field;
			for (const auto& column : table_data.m_field_list) { // 第一个一定是rk
				switch (column.field_type) {
					case DB_DWORD: {
						uint32_t value = ResultSet_getInt(result, index);
						if (!is_key_read) {
							key_field = std::move(serialize_data(value));
							is_key_read = true;
						} else {
							if (REDIS_OK != redisAppendCommand(m_redis_connect, "HSET %s:%s %s %u", table.c_str(), key_field.c_str(), column.field_name.c_str(), value)) {
								log_error("table[%s] key[%s] field[%s] insert error", table.c_str(), key_field.c_str(), column.field_name.c_str());
							} else {
								++cmd;
							}

							if (column.need_mapping) {
								if (REDIS_OK != redisAppendCommand(m_redis_connect, "SADD %s:%s:%u %s", table.c_str(), column.field_name.c_str(), value, key_field.c_str())) {
									log_error("table[%s] key[%s] field[%s] mapping insert error", table.c_str(), key_field.c_str(), column.field_name.c_str());
								} else {
									++cmd;
								}
							}
						}
						break;
					}
					case DB_QWORD: {
						uint64_t value = ResultSet_getLLong(result, index);
						if (!is_key_read) {
							key_field = std::move(serialize_data(value));
							is_key_read = true;
						} else {
							if (REDIS_OK != redisAppendCommand(m_redis_connect, "HSET %s:%s %s %lu", table.c_str(), key_field.c_str(), column.field_name.c_str(), value)) {
								log_error("table[%s] key[%s] field[%s] insert error", table.c_str(), key_field.c_str(), column.field_name.c_str());
							} else {
								++cmd;
							}

							if (column.need_mapping) {
								if (REDIS_OK != redisAppendCommand(m_redis_connect, "SADD %s:%s:%lu %s", table.c_str(), column.field_name.c_str(), value, key_field.c_str())) {
									log_error("table[%s] key[%s] field[%s] mapping insert error", table.c_str(), key_field.c_str(), column.field_name.c_str());
								} else {
									++cmd;
								}
							}
						}
						break;
					}
					case DB_FLOAT: {
						float value = ResultSet_getDouble(result, index);
						if (!is_key_read) {
							log_error("table[%s] field[%s] can not be primary key", table.c_str(), column.field_name.c_str());
							return false;
						} else {
							if (REDIS_OK != redisAppendCommand(m_redis_connect, "HSET %s:%s %s %f", table.c_str(), key_field.c_str(), column.field_name.c_str(), value)) {
								log_error("table[%s] key[%s] field[%s] insert error", table.c_str(), key_field.c_str(), column.field_name.c_str());
							} else {
								++cmd;
							}
						}
						break;
					}
					case DB_STR: {
						std::string str_data("");
						const char* str = (const char*)ResultSet_getString(result, index);
						if (str != NULL) str_data.assign(str);
						if (!is_key_read) {
							key_field = std::move(str_data);
							is_key_read = true;
						} else {
							if (REDIS_OK != redisAppendCommand(m_redis_connect, "HSET %s:%s %s %s", table.c_str(), key_field.c_str(), column.field_name.c_str(), str_data.c_str())) {
								log_error("table[%s] key[%s] field[%s] insert error", table.c_str(), key_field.c_str(), column.field_name.c_str());
							} else {
								++cmd;
							}

							if (column.need_mapping) {
								if (REDIS_OK != redisAppendCommand(m_redis_connect, "SADD %s:%s:%s %s", table.c_str(), column.field_name.c_str(), str_data.c_str(), key_field.c_str())) {
									log_error("table[%s] key[%s] field[%s] mapping insert error", table.c_str(), key_field.c_str(), column.field_name.c_str());
								} else {
									++cmd;
								}
							}
						}
						break;
					}
					case DB_BIN: {
						if (!is_key_read) {
							log_error("table[%s] field[%s] can not be primary key", table.c_str(), column.field_name.c_str());
							return false;
						} else {
							int size = 0;
							const char* blob = (const char*)ResultSet_getBlob(result, index, &size);

							if (blob && size > 0) {
								if (REDIS_OK != redisAppendCommand(m_redis_connect, "HSET %s:%s %s %b", table.c_str(), key_field.c_str(), column.field_name.c_str(), blob, size)) {
									log_error("table[%s] key[%s] field[%s] insert error", table.c_str(), key_field.c_str(), column.field_name.c_str());
								} else {
									++cmd;
								}
							}
						}
						break;
					}
					default: {
						log_error("table[%s] field[%s] type[%u] not define", table.c_str(), column.field_name.c_str(), column.field_type);
						return false;
					}
				}

				++index;
			}

			while (cmd-- > 0) {
				redis_reply_t reply;
				int r = redisGetReply(m_redis_connect, (void **)&reply.m_reply_data);
				if (r == REDIS_ERR) {
					log_error("table[%s] row[%s] insert redis error", table.c_str(), key_field.c_str());
				} else {
					if (!reply.m_reply_data || reply.m_reply_data->type == REDIS_REPLY_ERROR) {
						log_error("table[%s] row[%s] insert redis error", table.c_str(), key_field.c_str());
					}
				}
			}
		}
		}
			CATCH(SQLException)
		{
			log_error("redis_srv_t do_read_user_data uid[%lu] table[%s] error: %s", uid, table.c_str(), Exception_frame.message);
			return false;
		}
		FINALLY
		{
		}
		END_TRY;
	}

	return true;
}

void redis_db_read_t::post_pre_read_user_data(const std::vector<uint64_t>& uids)
{
	add_batch_read_task();
	m_strand.post(boost::bind(&redis_db_read_t::pre_read_user_data, this, uids));
}

void redis_db_read_t::pre_read_user_data(const std::vector<uint64_t>& uids)
{
	Connection_T conn = common::zdb_util_t::get_zdb_conn(m_zdb_pool);
	if (conn) {
		for (const auto& uid : uids) {
			if (!do_read_user_data(uid, conn)) {
				log_error("user[%lu] load data from sql failed", uid);
			}
		}

		Connection_close(conn);
	} else {
		log_error("redis_db_read_t read_user_data conn is null");
	}

	add_done_batch_task();
}
