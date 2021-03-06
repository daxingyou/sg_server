#include "redis_srv.hpp"

redis_srv_t::table_data_t redis_srv_t::const_error_list;

redis_srv_t::redis_srv_t()
	: m_strand(m_io_service)
	, m_is_init_done(false)
	, m_is_closed(true)
{
	
}

redis_srv_t::~redis_srv_t()
{
	
}

bool redis_srv_t::init(const char* host, // redis ip
		int32_t port,			// redis port
		const char* passwd,		// redis auth
		int32_t db,				// redis select
		const sql_table *db_table, // 需要加载的字段
		uint32_t thread_num,	// 线程数
		uint32_t read_num,		// 读取处理单元
		uint32_t save_interval, // 保存间隔
		uint32_t save_count_limit, // 单次保存数量
		uint32_t save_num,		// 保存处理单元
		struct ConnectionPool_S* zdb_pool)
{
	if (thread_num == 0 || read_num == 0 || save_num == 0) {
		log_error("redis_srv_t init_server param error");
		return false;
	}

	if (!redis_base_t::init(host, port, passwd, db)) {
		log_error("redis_srv_t connect redis error");
		return false;
	}

	// 初始化字段
	if (!init_table_field(zdb_pool, db_table)) {
		log_error("redis_srv_t init_table_field error");
		return false;
	}

	// 初始化工作线程
	m_work.reset(new boost::asio::io_service::work(m_io_service));
	for (size_t i = 0; i < thread_num; ++i) {
		m_thread_group.create_thread(boost::bind(&boost::asio::io_service::run, &m_io_service));
	}

	// 线程开始了
	m_is_closed = false;

	// 初始化读取单元
	if (!init_read_unit_list(host, port, passwd, db, zdb_pool, read_num)) {
		log_error("redis_srv_t init_read_unit failed");
		close();
		return false;
	}

	// 初始化保存单元
	if (!init_save_unit_list(host, port, passwd, db, zdb_pool, save_interval, save_count_limit, save_num)) {
		log_error("redis_srv_t init_save_unit failed");
		close();
		return false;
	}

	uint32_t state = get_redis_state();
	switch (state) {
		case redis_server_state::redis_server_none: {
			set_string(REDIS_DB_STATE.c_str(), redis_server_state::redis_server_init);
			log_info("start load sql to redis, redis state[%u]", redis_server_state::redis_server_init);
#if 0 // 多线程的处理
			m_strand.post(boost::bind(&redis_srv_t::start_read_from_db, this));
#else
			if (!start_read_from_db())
				return false;
#endif
			break;
		}
		case redis_server_state::redis_server_runing: {
			read_db_done();
			break;
		}
		default: {
			log_error("redis_srv init failed, redis state[%u] error", state);
			close();
			return false;
			break;
		}
	}

	return true;
}

void redis_srv_t::close(bool is_del /*= false*/)
{
	// 设置关闭
	m_is_closed = true;

	// 暂停所有定时器
	for (const auto& p_save_unit_ptr : m_save_unit_list) {
		if (p_save_unit_ptr) {
			p_save_unit_ptr->stop_save_to_sql_timer();
		}
	}

	m_work.reset();
	m_io_service.stop();
	m_thread_group.join_all();

	// 这里为止线程全部中止，开始做清理工作
	if (is_del) {
		set_string(REDIS_DB_STATE.c_str(), redis_server_state::redis_server_closing);
		log_info("redis srv successfully exited, start save all to sql and flush redis db");
	}

	// 关服的时候主线程全力写数据库
	if (get_set_size(SAVE_SET_NAME.c_str()) > 0) {
		server_save_unit_list::const_iterator find_citr = m_save_unit_list.begin();
		if (find_citr != m_save_unit_list.end()) {
			const redis_save_unit_ptr& p_save_unit_ptr = *find_citr;
			if (p_save_unit_ptr) {
				p_save_unit_ptr->do_save_all();
			}
		}
	}

	if (is_del) {
		if (get_set_size(SAVE_SET_NAME.c_str()) > 0) {
			log_error("redis flush db error, save set not empty");
		} else {
			do_flush_db();
		}
	}
	
	// clear 
	m_read_unit_list.clear();
	m_save_unit_list.clear();

	redis_base_t::close();

	log_info("close redis_srv_t success!");
}

uint32_t redis_srv_t::get_redis_state()
{
	return get_string<uint32_t>(REDIS_DB_STATE.c_str());
}

bool redis_srv_t::init_table_field(struct ConnectionPool_S* zdb_pool, const sql_table *db_table)
{
	if (db_table == NULL) {
		log_error("redis_srv_t db_table null");
		return false;
	}

	Connection_T conn = common::zdb_util_t::get_zdb_conn(zdb_pool);
	if (NULL == conn) {
		log_error("redis_srv_t init_table_field conn is null");
		return false;
	}

	const sql_table *temp = db_table;
	while (temp->table) {
		std::set<std::string> mapping;
		if (temp->fields != NULL && strlen(temp->fields) > 0) {
			std::vector<std::string> vec_data_key;
			common::string_util_t::split<std::string>(temp->fields, vec_data_key, ":");
			mapping.insert(vec_data_key.begin(), vec_data_key.end());
		}

		table_data_t table_data;
		table_data.m_is_loaded = temp->is_load;
		if (temp->where != NULL && strlen(temp->where) > 0) {
			table_data.m_where.assign(temp->where);
		}

		bool ret = true;
		TRY
		{
			bool is_first = true; // 用于检测表的第一个字段是否为rk
			ResultSet_T result = Connection_executeQuery(conn, "SHOW COLUMNS FROM `%s`;", temp->table);
			while (ResultSet_next(result)) {
				std::string field_name = ResultSet_getString(result, 1);
				std::string type = ResultSet_getString(result, 2);
				DB_TYPE filed_type = DB_NONE;
				if (type.find("blob") != std::string::npos) {
					filed_type = DB_BIN;
				} else if (type.find("varchar") != std::string::npos){
					filed_type = DB_STR;
				} else if (type.find("bigint") != std::string::npos) {
					filed_type = DB_QWORD;
				} else if (type.find("int") != std::string::npos) {
					filed_type = DB_DWORD;
				} else if (type.find("float") != std::string::npos) {
					filed_type = DB_FLOAT;
				} else {
					log_error("table[%s] filed[%s] type[%s] not define", temp->table, field_name.c_str(), type.c_str());
					ret = false;
					break;
				}

				if (is_first) {
					if (field_name != PRIMARY_KEY) {
						log_error("table[%s] first column filed[%s] != rk", temp->table, field_name.c_str());
						ret = false;
						break;
					}
					is_first = false;
				}

				table_data.m_filed_map.insert(std::make_pair(field_name, filed_type));
				table_data.m_field_list.push_back(field_data(field_name, filed_type, ((mapping.find(field_name) != mapping.end()) ? true : false)));
			}
		}
		CATCH(SQLException)
		{
			log_error("redis_srv_t init_table_field table[%s] error: %s", temp->table, Exception_frame.message);
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

		if (temp->is_load) {
			TRY
			{
				ResultSet_T result = NULL;
				if (temp->where != NULL && strlen(temp->where) > 0) {
					result = Connection_executeQuery(conn, "SELECT COUNT(*) FROM `%s` WHERE %s;", temp->table, temp->where);
				} else {
					result = Connection_executeQuery(conn, "SELECT COUNT(*) FROM `%s`;", temp->table);
				}

				while (ResultSet_next(result)) {
					table_data.m_row_count = ResultSet_getInt(result, 1);
				}
			}
				CATCH(SQLException)
			{
				log_error("redis_srv_t init_table_field table[%s] error: %s", temp->table, Exception_frame.message);
				ret = false;
			}
			FINALLY
			{
			}
			END_TRY;
		}

		if (!ret) {
			Connection_close(conn);
			return false;
		}

		m_table_list.insert(std::make_pair(std::string(temp->table), table_data));
		temp++;
	}

	Connection_close(conn);
	return true;
}

bool redis_srv_t::init_read_unit_list(const char* host, int32_t port, const char* passwd, int32_t db, struct ConnectionPool_S* zdb_pool, uint32_t num)
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
		redis_read_unit_ptr p_unit(new redis_read_unit_t(m_io_service, *this));
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

bool redis_srv_t::init_save_unit_list(const char* host, int32_t port, const char* passwd, int32_t db, struct ConnectionPool_S* zdb_pool, uint32_t save_interval, uint32_t save_count_limit, uint32_t num)
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
		redis_save_unit_ptr p_unit(new redis_save_unit_t(m_io_service, *this));
		if (!p_unit) {
			log_error("redis_save_unit_ptr null error");
			return false;
		}

		if (!p_unit->init(host, port, passwd, db, zdb_pool, save_interval, save_count_limit)) {
			log_error("redis_save_unit_ptr init error");
			return false;
		}

		m_save_unit_list.push_back(p_unit);
	}

	return true;
}

bool redis_srv_t::start_read_from_db()
{
	if (m_read_unit_list.empty()) {
		log_error("read thread num == 0");
		return false;
	}

#if 0 // 多线程的处理
	for (table_list::const_iterator citr = m_table_list.begin();
		citr != m_table_list.end(); ++citr) {
		if (!citr->second.m_is_loaded) continue;
		if (citr->second.m_row_count == 0) continue;

		// 将任务平均的分配给所有处理单元
		uint32_t each_num = citr->second.m_row_count / m_read_unit_list.size();
		if (each_num < 1) each_num = 1;
		uint32_t cur_row = 0;
		uint32_t count = 0;
		for (const auto& p_unit_ptr : m_read_unit_list) {
			if (p_unit_ptr == NULL) {
				log_error("p_unit_ptr null error");
				return;
			}

			++count;
			if (count >= m_read_unit_list.size()) { // 最后一个把所有该读的读取
				p_unit_ptr->post_batch_read(citr->first, cur_row, citr->second.m_row_count);
				break;
			} else {
				p_unit_ptr->post_batch_read(citr->first, cur_row, each_num);
				cur_row += each_num;

				if (cur_row >= citr->second.m_row_count) {
					break;
				}
			}
		}
	}

	// 一些其他数据的读取
	start_read_extra_data();

	check_read_db_done();
#else
	redis_read_unit_ptr p_read_unit = m_read_unit_list.at(0); // 取第一个
	if (!p_read_unit) {
		log_error("p_read_unit null error");
		return false;
	}

	for (table_list::const_iterator citr = m_table_list.begin();
		citr != m_table_list.end(); ++citr) {
		if (!citr->second.m_is_loaded) continue;
		if (citr->second.m_row_count == 0) continue;

		p_read_unit->do_batch_read(citr->first, 0, citr->second.m_row_count);
	}

	// 一些其他数据的读取
	if (!start_read_extra_data())
		return false;

	read_db_done();
#endif

	return true;
}

void redis_srv_t::check_read_db_done()
{
	for (const auto& p_read_unit : m_read_unit_list) {
		if (p_read_unit && !p_read_unit->is_batch_read_done()) {
			return;
		}
	}

	read_db_done();
}

void redis_srv_t::do_read_finish(const redis_read_unit_wptr& p_wunit)
{
	if (m_is_closed) {
		return;
	}

	redis_read_unit_ptr p_read_unit_ptr = p_wunit.lock();
	if (p_read_unit_ptr == NULL) {
		log_error("p_read_unit_ptr is free");
		return;
	}

	if (!p_read_unit_ptr->is_batch_read_done()) {
		return;
	}

	check_read_db_done();
}

void redis_srv_t::read_db_done()
{
	// 设置redis运行中
	if (get_redis_state() == redis_server_state::redis_server_init) {
		set_string(REDIS_DB_STATE.c_str(), redis_server_state::redis_server_runing);
		log_info("redis srv init done, change state [redis_server_runing]");
	} else if (get_redis_state() == redis_server_state::redis_server_runing) {
		log_warn("redis srv abnormally exited and restarted successfully");
	} else {
		log_error("redis srv read_db_done state error");
	}

	m_is_init_done = true;
}

uint32_t redis_srv_t::get_db_size()
{
	if (m_redis_connect == NULL) {
		return 0;
	}

	redis_reply_t reply;
	reply.m_reply_data = (redisReply*)redisCommand(m_redis_connect, "DBSIZE");

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

void redis_srv_t::do_flush_db()
{
	if (m_redis_connect == NULL) {
		return;
	}

	redis_reply_t reply;
	reply.m_reply_data = (redisReply*)redisCommand(m_redis_connect, "FLUSHDB");

	if (!reply.m_reply_data || reply.m_reply_data->type == REDIS_REPLY_ERROR) {
		log_error("Redis reply Error!");
	}

	log_info("redis flush_db done!");
}

void redis_srv_t::unit_read_finish(const redis_read_unit_ptr& p_read_unit_ptr)
{
	if (p_read_unit_ptr == NULL) {
		log_error("p_read_unit_ptr is free");
		return;
	}

	if (m_is_closed) {
		return;
	}

	m_strand.post(boost::bind(&redis_srv_t::do_read_finish, this, redis_read_unit_wptr(p_read_unit_ptr)));
}

const redis_srv_t::table_data_t& redis_srv_t::get_table_field_cols(const std::string& table)
{
	table_list::const_iterator citr = m_table_list.find(table);
	if (citr != m_table_list.end()) {
		return citr->second;
	}

	log_error("redis_srv_t get table[%s] field error", table.c_str());
	return const_error_list;
}

const redis_srv_t::field_type_map& redis_srv_t::get_table_field_map(const std::string& table)
{
	table_list::const_iterator citr = m_table_list.find(table);
	if (citr != m_table_list.end()) {
		return citr->second.m_filed_map;
	}

	log_error("redis_srv_t get table[%s] field error", table.c_str());
	return const_error_list.m_filed_map;
}

//////////////////////////////////////////////////////////////////////////

redis_read_unit_t::redis_read_unit_t(boost::asio::io_service& ios, redis_srv_t& server)
	: redis_unit_t(ios)
	, m_redis_server(server)
{

}

redis_read_unit_t::~redis_read_unit_t()
{

}

void redis_read_unit_t::post_batch_read(const std::string& table, uint32_t cursor, uint32_t row_num)
{
	add_batch_read_task();
	m_strand.post(boost::bind(&redis_read_unit_t::batch_read_sql, this, table, cursor, row_num));
}

void redis_read_unit_t::batch_read_sql(const std::string& table, uint32_t cursor, uint32_t row_num)
{
	do_batch_read(table, cursor, row_num);
	add_done_batch_task();
}

bool redis_read_unit_t::do_batch_read(const std::string& table, uint32_t cursor, uint32_t row_num)
{
	if (!m_redis_connect) {
		log_error("m_redis_connect null error");
		return false;
	}

	const redis_srv_t::table_data_t& table_data = m_redis_server.get_table_field_cols(table);
	if (table_data.m_field_list.empty()) {
		log_error("table[%s] columns is empty!", table.c_str());
		return false;
	}

	Connection_T conn = common::zdb_util_t::get_zdb_conn(m_zdb_pool);
	if (NULL == conn) {
		log_error("redis_read_unit_t do_batch_read conn is null");
		return false;
	}

	TRY
	{
		ResultSet_T result = NULL;
#if 0 // 多线程
		if (table_data.m_where.empty()) {
			result = Connection_executeQuery(conn, "SELECT * FROM %s WHERE invalid=0 LIMIT %u,%u;", table.c_str(), cursor, row_num);
		} else {
			result = Connection_executeQuery(conn, "SELECT * FROM %s WHERE invalid=0 AND %s LIMIT %u,%u;", table.c_str(), table_data.m_where.c_str(), cursor, row_num);
		}
#else
		if (table_data.m_where.empty()) {
			result = Connection_executeQuery(conn, "SELECT * FROM %s WHERE invalid=0;", table.c_str());
		} else {
			result = Connection_executeQuery(conn, "SELECT * FROM %s WHERE invalid=0 AND %s;", table.c_str(), table_data.m_where.c_str());
		}
#endif

		while (ResultSet_next(result)) {
			int index = 1;
			uint32_t cmd = 0;
			bool is_key_read = false;
			std::string key_field;
			bool ret = true;
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
							ret = false;
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
							ret = false;
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
						ret = false;
						break;
					}
				}

				if (!ret) 
					break;

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
		log_error("redis_srv_t do_batch_read table[%s] error: %s", table.c_str(), Exception_frame.message);
	}
	FINALLY
	{
		Connection_close(conn);
	}
	END_TRY;

	return true;
}

void redis_read_unit_t::add_batch_read_task()
{
	m_batch_read_task++;
}

void redis_read_unit_t::add_done_batch_task()
{
	m_done_batch_task++;

	// 检测是否任务完成（这里的结果不一定正确）
	if (is_batch_read_done()) {
		m_redis_server.unit_read_finish(shared_from_this());
	}
}

//////////////////////////////////////////////////////////////////////////
redis_save_unit_t::redis_save_unit_t(boost::asio::io_service& ios, redis_srv_t& server)
	: redis_unit_t(ios)
	, m_save_to_sql_timer(ios)
	, m_redis_server(server)
{

}

redis_save_unit_t::~redis_save_unit_t()
{

}

bool redis_save_unit_t::init(const char* host, int32_t port, const char* passwd, int32_t db, struct ConnectionPool_S* zdb_pool, uint32_t save_interval, uint32_t save_count_limit)
{
	if (NULL == zdb_pool) {
		log_error("NULL == zdb_pool");
		return false;
	}

	if (!redis_unit_t::init(host, port, passwd, db, zdb_pool)) {
		log_error("redis_unit_t connect redis error");
		return false;
	}

	m_save_interval = save_interval;
	m_save_count_limit = save_count_limit;
	m_save_cancel = false;

	return start_save_to_sql_timer();
}

bool redis_save_unit_t::start_save_to_sql_timer()
{
	if (m_save_cancel) {
		return false;
	}

	if (0 == m_save_interval) {
		log_error("m_save_interval is 0 !");
		return false;
	}

	if (0 == m_save_count_limit) {
		log_error("m_save_count_limit is 0 !");
		return false;
	}

	m_save_to_sql_timer.expires_from_now(boost::posix_time::seconds(m_save_interval));
	m_save_to_sql_timer.async_wait(m_strand.wrap(boost::bind(&redis_save_unit_t::save_to_sql_timer_callback, this, boost::asio::placeholders::error)));

	return true;
}

void redis_save_unit_t::stop_save_to_sql_timer()
{
	if (m_save_cancel) {
		return;
	}

	m_save_cancel = true;
	m_save_to_sql_timer.cancel();
}

void redis_save_unit_t::save_to_sql_timer_callback(const boost::system::error_code& ec)
{
	if (ec == boost::asio::error::operation_aborted)
	{

		return;
	} else if (ec) {
		log_error("Timer error: %s", ec.message().c_str());
		return;
	}

	if (m_save_cancel) {
		log_error("Timer was canceled");
		return;
	}

	Connection_T conn = common::zdb_util_t::get_zdb_conn(m_zdb_pool);
	if (conn) {
		// 保存一定条目
		for (uint32_t i = 0; i < m_save_count_limit; ++i) {
			if (do_save_one(conn) == save_result_empty) break;
		}

		Connection_close(conn);
	} else {
		log_error("redis_save_unit_t save_to_sql_timer_callback conn is null");
	}

	start_save_to_sql_timer();
}

void redis_save_unit_t::do_save_all()
{
	Connection_T conn = common::zdb_util_t::get_zdb_conn(m_zdb_pool);
	if (!conn) {
		log_error("redis_save_unit_t do_save_all conn is null");
		return;
	}

	uint32_t curr_time = common::time_util_t::now_time();
	do  { // 全力写数据库
		if (do_save_one(conn) == save_result_empty) break;

		// 每5秒答应一次还有多少条
		if (common::time_util_t::now_time() - curr_time > 5) {
			log_info("redis save to db, left:%u", get_set_size(SAVE_SET_NAME.c_str()));
			
			// 等待一下
			boost::asio::deadline_timer timer(m_ios);
			timer.expires_from_now(boost::posix_time::seconds(2));
			boost::system::error_code ignored_ec;
			timer.wait(ignored_ec);

			curr_time = common::time_util_t::now_time();
		}
	} while (true);

	Connection_close(conn);

	log_info("redis save to db done!");
}

redis_save_unit_t::save_result_type redis_save_unit_t::do_save_one(const Connection_T& conn)
{
	//uint64_t time_start = common::time_util_t::now_msecs();

	std::string pop_single;
	if (!pop_set_member(SAVE_SET_NAME.c_str(), pop_single)) {
		return save_result_empty;
	}

	std::vector<std::string> vec_data_key;
	common::string_util_t::split<std::string>(pop_single, vec_data_key, ":");
	if (vec_data_key.size() != 3) {
		log_error("redis_save_unit_t do_save_one data_key[%s] error", pop_single.c_str());
		return save_result_error;
	}

	const std::string& table = vec_data_key[0];
	const std::string& save_type = vec_data_key[1];
	const std::string& key_data = vec_data_key[2];

	if (!is_hash_exist(table.c_str(), key_data)) { // 对于找不到的处理成成功，因为可能是由于该数据已经被清除导致，是正常现象
		return save_result_success;
	}

	const redis_srv_t::field_type_map& filed_map = m_redis_server.get_table_field_map(table);
	if (filed_map.empty()) {
		log_error("table[%s] field_map is empty!", table.c_str());
		return save_result_error;
	}

	std::map<std::string, std::string> ret;
	if (save_type == "u") {
		if (!get_hash_all(table, key_data, ret)) {
			log_error("redis_save_unit_t do_save_one [%s] get_hash_all error!", pop_single.c_str());
			return save_result_error;
		}
	} else if (save_type == "d") {
		if (!get_hash_all(pop_single, ret)) {
			log_error("redis_save_unit_t do_save_one [%s] get_hash_all error!", pop_single.c_str());
			return save_result_error;
		}

		// 删除数据
		del_redis_key(pop_single);
	} else {
		log_error("redis_save_unit_t do_save_one [%s] get_hash_all error!", pop_single.c_str());
		return save_result_error;
	}

	if (ret.empty()) {
		log_error("redis_save_unit_t do_save_one [%s] get_hash_all empty!", pop_single.c_str());
		return save_result_error;
	}

	// replace into 必须加主键
	ret.insert(std::make_pair(PRIMARY_KEY, key_data));

	std::ostringstream temp;
	temp << "REPLACE INTO `" << table << "` SET ";
	for (std::map<std::string, std::string>::const_iterator citr = ret.begin();
		citr != ret.end(); ++citr) {
		redis_srv_t::field_type_map::const_iterator field_type_citr = filed_map.find(citr->first);
		if (field_type_citr == filed_map.end()) {
			log_error("table[%s] field[%s] not define error", table.c_str(), citr->first.c_str());
			continue;
		}

		if (citr != ret.begin()) {
			temp << ",";
		}

		temp << "`" << citr->first << "` = ?";
	}
	temp << ";";

	std::string sql_str = std::move(temp.str());
	TRY
	{
		PreparedStatement_T pre = Connection_prepareStatement(conn, sql_str.c_str());
		int index = 1;
		for (std::map<std::string, std::string>::const_iterator citr = ret.begin();
			citr != ret.end(); ++citr) {
			redis_srv_t::field_type_map::const_iterator field_type_citr = filed_map.find(citr->first);
			if (field_type_citr == filed_map.end()) {
				// log_error("table[%s] field[%s] not define error", table.c_str(), citr->first.c_str());
				continue;
			}

			switch (field_type_citr->second) {
				case DB_DWORD: {
					uint32_t _val = 0;
					unserialize_data(citr->second, _val);
					PreparedStatement_setInt(pre, index, _val);
					break;
				}
				case DB_QWORD: {
					uint64_t _val = 0;
					unserialize_data(citr->second, _val);
					PreparedStatement_setLLong(pre, index, _val);
					break;
				}
				case DB_FLOAT: {
					float _val = 0;
					unserialize_data(citr->second, _val);
					PreparedStatement_setDouble(pre, index, _val);
					break;
				}
				case DB_STR: {
					PreparedStatement_setString(pre, index, citr->second.c_str());
					break;
				}
				case DB_BIN: {
					PreparedStatement_setBlob(pre, index, citr->second.c_str(), citr->second.length());
					break;
				}
				default: {
					log_error("table[%s] field[%s] type[%u] not define", table.c_str(), citr->first.c_str(), field_type_citr->second);
					break;
				}
			}

			++index;
		}

		PreparedStatement_execute(pre);
	}
	CATCH(SQLException)
	{
		log_error("redis_save_unit_t do_save_one[%s] update error: %s", pop_single.c_str(), Exception_frame.message);
	}
	FINALLY
	{
		
	}
	END_TRY;

	//uint64_t time_end = common::time_util_t::now_msecs();
	//printf("\033[31m sql_str:%s cost:%lu\033[0m\n", sql_str.c_str(), time_end - time_start);

	return save_result_success;
}
