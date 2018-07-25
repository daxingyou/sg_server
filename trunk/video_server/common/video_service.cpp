#include "video_service.hpp"

video_service_t::video_service_t(const std::string &name)
	: m_service_name(name)
{
	// redis 状态键值
	m_redis_state = name + "_state";
}

video_service_t::~video_service_t()
{

}

bool video_service_t::init(
	const char* host, /* redis ip */ 
	int32_t port, /* redis port */ 
	const char* passwd, /* redis auth */ 
	int32_t db, /* redis select */ 
	uint32_t process_num, /* 处理单元个数（等于线程数） */ 
	struct ConnectionPool_S* zdb_pool)
{
	if (process_num == 0) {
		log_error("%s video_service init process_num == 0", m_service_name.c_str());
		return false;
	}

	// 初始化字段
	if (!init_table_field(zdb_pool)) {
		log_error("%s video_service init_table_field error", m_service_name.c_str());
		return false;
	}

	if (!redis_base_t::init(host, port, passwd, db)) {
		log_error("%s video_service connect redis error", m_service_name.c_str());
		return false;
	}

	// 初始化工作线程
	m_work.reset(new boost::asio::io_service::work(m_io_service));
	for (size_t i = 0; i < process_num; ++i) {
		m_thread_group.create_thread(boost::bind(&boost::asio::io_service::run, &m_io_service));
	}

	// 初始化处理单元
	if (!init_unit(host, port, passwd, db, zdb_pool, process_num)) {
		log_error("%s video_service init_unit failed", m_service_name.c_str());
		close();
		return false;
	}

	uint32_t state = get_redis_state();
	switch (state) {
		case redis_server_state::redis_server_none: {
			set_redis_state(redis_server_state::redis_server_init);
			log_info("%s video_service load data to redis, redis state[%u]", m_service_name.c_str(), redis_server_state::redis_server_init);
			if (!start_read_from_db(zdb_pool))
				return false;
			read_db_done();
			break;
		}
		case redis_server_state::redis_server_runing: {
			read_db_done();
			break;
		}
		default: {
			log_error("%s video_service init failed, redis state[%u] error", m_service_name.c_str(), state);
			close();
			return false;
			break;
		}
	}

	return true;
}

uint32_t video_service_t::get_redis_state()
{
	return get_string<uint32_t>(m_redis_state.c_str());
}

void video_service_t::set_redis_state(uint32_t state)
{
	set_string(m_redis_state.c_str(), state);
}

void video_service_t::read_db_done()
{
	// 设置redis运行中
	uint32_t state = get_redis_state();
	if (state == redis_server_state::redis_server_init) {
		set_redis_state(redis_server_state::redis_server_runing);
		log_info("%s video_service init done, change state [redis_server_runing]", m_service_name.c_str());
	} else if (get_redis_state() == redis_server_state::redis_server_runing) {
		log_warn("%s video_service abnormally exited and restarted successfully", m_service_name.c_str());
	} else {
		log_error("%s video_service read_db_done state error", m_service_name.c_str());
	}
}

void video_service_t::close()
{
	for (video_unit_list::const_iterator citr = m_video_unit_list.begin();
		citr != m_video_unit_list.end(); ++citr) {
		const video_unit_ptr& p_unit = *citr;
		if (p_unit) {
			p_unit->set_stop();
		}
	}

	if (m_work) m_work.reset();
	// m_io_service.stop(); // 不能强制退出
	m_thread_group.join_all();

	redis_base_t::close();

	log_info("%s video_service stop success!", m_service_name.c_str());
}

void video_service_t::recv_video_message(uint64_t id, const network::msg_buf_ptr& msg_buf)
{
	uint32_t index = id % m_video_unit_list.size();
	if (m_video_unit_list[index]) {
		m_video_unit_list[index]->post_new_video(id, msg_buf);
	}
}

bool video_service_t::init_table_field(struct ConnectionPool_S* zdb_pool)
{
	Connection_T conn = common::zdb_util_t::get_zdb_conn(zdb_pool);
	if (NULL == conn) {
		log_error("%s video_service init_table_field conn is null", m_service_name.c_str());
		return false;
	}

	std::map<std::string, uint32_t> mapping;
	for (uint32_t i = 0; i < class_type_max; ++i) {
		mapping.insert(std::make_pair(CLASS_LIST[i], i));
	}

	bool ret = true;
	TRY
	{
		bool is_first = true; // 用于检测表的第一个字段是否为rk
		ResultSet_T result = Connection_executeQuery(conn, "SHOW COLUMNS FROM `%s`;", m_service_name.c_str());
		while (ResultSet_next(result)) {
			std::string field_name = ResultSet_getString(result, 1);
			std::string type = ResultSet_getString(result, 2);
			DB_TYPE filed_type = DB_NONE;
			if (type.find("blob") != std::string::npos) {
				filed_type = DB_BIN;
			} else if (type.find("varchar") != std::string::npos) {
				filed_type = DB_STR;
			} else if (type.find("bigint") != std::string::npos) {
				filed_type = DB_QWORD;
			} else if (type.find("int") != std::string::npos) {
				filed_type = DB_DWORD;
			} else if (type.find("float") != std::string::npos) {
				filed_type = DB_FLOAT;
			} else {
				log_error("table[%s] filed[%s] type[%s] not define", m_service_name.c_str(), field_name.c_str(), type.c_str());
				ret = false;
				break;
			}

			if (is_first) {
				if (field_name != PRIMARY_KEY) {
					log_error("table[%s] first column filed[%s] != rk", m_service_name.c_str(), field_name.c_str());
					ret = false;
					break;
				}
				is_first = false;
			}

			std::map<std::string, uint32_t>::const_iterator citr = mapping.find(field_name);
			m_field_list.push_back(field_data(field_name, filed_type, field_name == GROUP_ID, field_name == LAYER_ID, (citr != mapping.end()) ? citr->second : class_type_none));
		}
	}
	CATCH(SQLException)
	{
		log_error("video_service table[%s] error: %s", m_service_name.c_str(), Exception_frame.message);
		ret = false;
	}
	FINALLY
	{
		Connection_close(conn);
	}
	END_TRY;

	return ret;
}

bool video_service_t::load_rank_data(struct ConnectionPool_S* zdb_pool, const char* sql, fight_top_data& fight_uids)
{
	if (!m_redis_connect) {
		log_error("%s video_service m_redis_connect null error", m_service_name.c_str());
		return false;
	}

	if (m_field_list.empty()) {
		log_error("table[%s] columns is empty!", m_service_name.c_str());
		return false;
	}

	Connection_T conn = common::zdb_util_t::get_zdb_conn(zdb_pool);
	if (NULL == conn) {
		log_error("%s video_service load_rank_data conn == null", m_service_name.c_str());
		return false;
	}

	TRY
	{
		ResultSet_T result = Connection_executeQuery(conn, sql);
	while (ResultSet_next(result)) {
		int index = 1;
		uint32_t cmd = 0;
		bool is_key_read = false;
		uint64_t key_field = 0;
		uint32_t group_id = 0;
		uint32_t layer_id = 0;
		bool ret = true;
		for (const auto& column : m_field_list) { // 第一个一定是rk
			switch (column.field_type) {
				case DB_DWORD: {
					uint32_t value = ResultSet_getInt(result, index);
					if (!is_key_read) {
						log_error("table[%s] field[%s] can not be primary key", m_service_name.c_str(), column.field_name.c_str());
						ret = false;
					} else {
						if (REDIS_OK != redisAppendCommand(m_redis_connect, "HSET %s:%lu %s %u", m_service_name.c_str(), key_field, column.field_name.c_str(), value)) {
							log_error("table[%s] key[%lu] field[%s] insert error", m_service_name.c_str(), key_field, column.field_name.c_str());
						} else {
							++cmd;
						}

						// 对上榜数据进行细分
						if (column.need_group) {
							if (group_id != 0) {
								log_error("group_id is already exist");
							}
							group_id = value;
						} else if (column.need_layer) {
							layer_id = value;
						} else if (column.class_type != class_type_none) {
							fight_uids[group_id][layer_id][column.class_type].push_back(key_field);
							//printf("group[%u] layer_id[%u] value[%u] key_field[%lu] \n", group_id, layer_id, column.class_type, key_field);
						}
					}
					break;
				}
				case DB_QWORD: {
					uint64_t value = ResultSet_getLLong(result, index);
					if (!is_key_read) {
						key_field = value;
						is_key_read = true;
					} else {
						if (REDIS_OK != redisAppendCommand(m_redis_connect, "HSET %s:%lu %s %lu", m_service_name.c_str(), key_field, column.field_name.c_str(), value)) {
							log_error("table[%s] key[%lu] field[%s] insert error", m_service_name.c_str(), key_field, column.field_name.c_str());
						} else {
							++cmd;
						}
					}
					break;
				}
				case DB_FLOAT: {
					float value = ResultSet_getDouble(result, index);
					if (!is_key_read) {
						log_error("table[%s] field[%s] can not be primary key", m_service_name.c_str(), column.field_name.c_str());
						ret = false;
					} else {
						if (REDIS_OK != redisAppendCommand(m_redis_connect, "HSET %s:%lu %s %f", m_service_name.c_str(), key_field, column.field_name.c_str(), value)) {
							log_error("table[%s] key[%lu] field[%s] insert error", m_service_name.c_str(), key_field, column.field_name.c_str());
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
						log_error("table[%s] field[%s] can not be primary key", m_service_name.c_str(), column.field_name.c_str());
						ret = false;
					} else {
						if (REDIS_OK != redisAppendCommand(m_redis_connect, "HSET %s:%lu %s %s", m_service_name.c_str(), key_field, column.field_name.c_str(), str_data.c_str())) {
							log_error("table[%s] key[%lu] field[%s] insert error", m_service_name.c_str(), key_field, column.field_name.c_str());
						} else {
							++cmd;
						}
					}
					break;
				}
				case DB_BIN: {
					if (!is_key_read) {
						log_error("table[%s] field[%s] can not be primary key", m_service_name.c_str(), column.field_name.c_str());
						ret = false;
					} else {
						int size = 0;
						const char* blob = (const char*)ResultSet_getBlob(result, index, &size);

						if (blob && size > 0) {
							if (REDIS_OK != redisAppendCommand(m_redis_connect, "HSET %s:%lu %s %b", m_service_name.c_str(), key_field, column.field_name.c_str(), blob, size)) {
								log_error("table[%s] key[%lu] field[%s] insert error", m_service_name.c_str(), key_field, column.field_name.c_str());
							} else {
								++cmd;
							}
						}
					}
					break;
				}
				default: {
					log_error("table[%s] field[%s] type[%u] not define", m_service_name.c_str(), column.field_name.c_str(), column.field_type);
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
				log_error("table[%s] row[%lu] insert redis error", m_service_name.c_str(), key_field);
			} else {
				if (!reply.m_reply_data || reply.m_reply_data->type == REDIS_REPLY_ERROR) {
					log_error("table[%s] row[%lu] insert redis error", m_service_name.c_str(), key_field);
				}
			}
		}
	}
	}
		CATCH(SQLException)
	{
		log_error("video_service load_rank_data table[%s] error: %s", m_service_name.c_str(), Exception_frame.message);
	}
	FINALLY
	{
		Connection_close(conn);
	}
	END_TRY;

	return true;
}