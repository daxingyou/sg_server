#include "tower_video.hpp"
#include "main/video_server_fwd.hpp"
#include "network/protobuf.hpp"

tower_video_service * tower_video_service::instance = NULL;

bool tower_video_service::init_unit(const char* host, int32_t port, const char* passwd, int32_t db, struct ConnectionPool_S* zdb_pool, uint32_t num)
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
		video_unit_ptr p_unit(new tower_video(m_io_service));
		if (!p_unit) {
			log_error("%s null error", m_service_name.c_str());
			return false;
		}

		if (!p_unit->init(host, port, passwd, db, zdb_pool)) {
			log_error("%s init error", m_service_name.c_str());
			return false;
		}

		m_video_unit_list.push_back(p_unit);
	}

	return true;
}

bool tower_video_service::start_read_from_db(struct ConnectionPool_S* zdb_pool)
{
	char sql[2048];
	bzero(sql, sizeof(sql));
	snprintf(sql, sizeof(sql) - 1, "SELECT * FROM %s WHERE %s >= %u AND (%s = 1 OR %s = 1 OR %s = 1) ORDER BY rk DESC;", m_service_name.c_str(), VERSION_ID.c_str(), env::xml->fight_version, FIRST_ID.c_str(), LIMIT_ID.c_str(), RECENT_ID.c_str());

	fight_top_data rank_fight_uids;
	if (!load_rank_data(zdb_pool, sql, rank_fight_uids)) {
		log_error("[%s] start_read_from_db error", m_service_name.c_str());
		return false;
	}

	for (const auto& pair_group : rank_fight_uids) {
		for (const auto& pair_layer : pair_group.second) {
			for (const auto& pair_top : pair_layer.second) {
				std::ostringstream key;
				key << m_service_name << ":" << pair_group.first  << ":" << pair_layer.first << ":" << CLASS_LIST[pair_top.first];

				if (batch_list_lpush(key.str(), pair_top.second) < 0) {
					log_error("[%s] batch_list_lpush error", m_service_name.c_str());
					return false;
				}
			}
		}
	}

	return true;
}

tower_video::tower_video(boost::asio::io_service& ios)
{

}

tower_video::~tower_video()
{

}

void tower_video::do_save_video(uint64_t id, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::gv_tower_video_notify);

	log_trace("recv server[%lu] video[%s] cmd[%s:%d] message from game", id, msg.base().video_uid().c_str(), CMD_DESC(op_cmd::gv_tower_video_notify), op_cmd::gv_tower_video_notify);

	const proto::common::video_single_base& base = msg.base();
	uint64_t video_uid = common::string_util_t::string_to_uint64(base.video_uid());
	if (video_uid == 0) {
		log_error("tower_video::do_save_video video_uid == 0");
		return;
	}

	if (base.objs_size() == 0) {
		log_error("tower_video::do_save_video objs_size() == 0");
		return;
	}

	const proto::common::video_obj_base& obj_base = base.objs(0);

	Connection_T conn = common::zdb_util_t::get_zdb_conn(m_zdb_pool);
	if (!conn) {
		log_error("tower_video do_save_video conn is null");
		return;
	}

	do {
		uint32_t first = 0;
		std::ostringstream key_first;
		key_first << TOWER << ":" << id << ":" << base.layer_id() << ":" << FIRST_ID;
		uint64_t pop_video_first = list_index<uint64_t>(key_first.str(), 0);
		if (pop_video_first == 0) {
			first = 1;
		}

		uint32_t limit = 0;
		std::ostringstream key_limit;
		key_limit << TOWER << ":" << id << ":" << base.layer_id() << ":" << LIMIT_ID;
		uint64_t pop_video_limit = list_index<uint64_t>(key_limit.str(), 0);
		if (pop_video_limit != 0) {
			uint32_t fighting = get_hash_uint32(TOWER.c_str(), TOWER_FIGHT.c_str(), pop_video_limit);
			if (obj_base.fighting() < fighting) {
				limit = 1;
			}
		} else {
			limit = 1;
		}

		if (!save_fight_data(conn, video_uid, id, first, limit, obj_base.fighting(), base, msg.fight_data()) {
			log_error("tower_video save server[%lu] video[%lu] failed", id, video_uid);
			break;
		}

		update_rank_list(conn, video_uid, id, first, limit);
	} while (0);
}

bool tower_video::save_fight_data(const Connection_T& conn, uint64_t video_uid, uint32_t server_id, uint32_t first, uint32_t limit, uint32_t fighting, uint32_t recent, proto::common::video_single_base& base, const std::string& fight_data)
{
	if (!conn) {
		log_error("tower_video save_fight_data conn is null");
		return false;
	}

	uint32_t base_buf_lenyh = base.ByteSize();
	if (base_buf_lenyh == 0) {
		log_error("tower_video save_fight_data base size == 0");
		return false;
	}

	char base_buf[base_buf_lenyh + 1];
	bzero(base_buf, sizeof(base_buf));
	base.SerializeToArray(base_buf, base_buf_lenyh);

	bool ret = true;
	TRY
	{
		PreparedStatement_T pre = Connection_prepareStatement(conn, "INSERT INTO `%s` VALUES (?,?,?,?,?,?,?,?,?,?);", TOWER.c_str());
		PreparedStatement_setLLong(pre, 1, video_uid);
		PreparedStatement_setInt(pre,	2, base.version());
		PreparedStatement_setInt(pre,	3, server_id);
		PreparedStatement_setInt(pre,	4, base.layer_id());
		PreparedStatement_setInt(pre,	5, fighting);
		PreparedStatement_setInt(pre,	6, first);
		PreparedStatement_setInt(pre,	7, limit);
		PreparedStatement_setInt(pre,	8, recent);
		PreparedStatement_setBlob(pre,	9, base_buf, base_buf_lenyh);
		PreparedStatement_setBlob(pre,	10, fight_data.c_str(), fight_data.length());
		PreparedStatement_execute(pre);
	}
		CATCH(SQLException)
	{
		log_error("tower_video [%lu] insert db error: %s", video_uid, Exception_frame.message);
		ret = false;
	}
	FINALLY
	{
	}
	END_TRY;

	if (!ret) return false;

	uint32_t cmd = 0;
	do {
		if (REDIS_OK != redisAppendCommand(m_redis_connect, "HSET %s:%lu %s %u", TOWER.c_str(), video_uid, VERSION_ID.c_str(), base.version())) {
			log_error("table[%s] add video[%lu] error", TOWER.c_str(), video_uid);
		} else {
			++cmd;
		}

		if (REDIS_OK != redisAppendCommand(m_redis_connect, "HSET %s:%lu %s %u", TOWER.c_str(), video_uid, GROUP_ID.c_str(), server_id)) {
			log_error("table[%s] add video[%lu] error", TOWER.c_str(), video_uid);
		} else {
			++cmd;
		}

		if (REDIS_OK != redisAppendCommand(m_redis_connect, "HSET %s:%lu %s %u", TOWER.c_str(), video_uid, LAYER_ID.c_str(), base.layer_id())) {
			log_error("table[%s] add video[%lu] error", TOWER.c_str(), video_uid);
		} else {
			++cmd;
		}

		if (REDIS_OK != redisAppendCommand(m_redis_connect, "HSET %s:%lu %s %b", CROSS_ARENA.c_str(), video_uid, FIGHT_BASE_ID.c_str(), base_buf, base_buf_lenyh)) {
			log_error("table[%s] add video[%lu] error", CROSS_ARENA.c_str(), video_uid);
		} else {
			++cmd;
		}

		if (REDIS_OK != redisAppendCommand(m_redis_connect, "HSET %s:%lu %s %b", CROSS_ARENA.c_str(), video_uid, COMBAT_DATA_ID.c_str(), fight_data.c_str(), fight_data.length())) {
			log_error("table[%s] add video[%lu] error", CROSS_ARENA.c_str(), video_uid);
		} else {
			++cmd;
		}
	} while (0);

	while (cmd-- > 0) {
		redis_reply_t reply;
		int r = redisGetReply(m_redis_connect, (void **)&reply.m_reply_data);
		if (r == REDIS_ERR) {
			log_error("[%s] video[%lu] update insert redis error", CROSS_ARENA.c_str(), video_uid);
		} else {
			if (!reply.m_reply_data || reply.m_reply_data->type == REDIS_REPLY_ERROR) {
				log_error("[%s] video[%lu] update insert redis error", CROSS_ARENA.c_str(), video_uid);
			}
		}
	}
}

void tower_video::update_rank_list(const Connection_T& conn, uint64_t video_uid, uint32_t server_id, uint32_t first, uint32_t limit)
{

}
