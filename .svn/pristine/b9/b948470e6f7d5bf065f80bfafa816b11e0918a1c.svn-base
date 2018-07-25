#include "cross_arena_video.hpp"
#include "main/video_server_fwd.hpp"
#include "network/protobuf.hpp"

cross_arena_service * cross_arena_service::instance = NULL;

bool cross_arena_service::init_unit(const char* host, int32_t port, const char* passwd, int32_t db, struct ConnectionPool_S* zdb_pool, uint32_t num)
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
		video_unit_ptr p_unit(new cross_area_video(m_io_service));
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

bool cross_arena_service::start_read_from_db(struct ConnectionPool_S* zdb_pool)
{
	char sql[2048];
	bzero(sql, sizeof(sql));
	snprintf(sql, sizeof(sql) - 1, "SELECT * FROM %s WHERE %s >= %u AND %s = 1 ORDER BY rk DESC;", m_service_name.c_str(), VERSION_ID.c_str(), env::xml->fight_version, TOP_ID.c_str());

	fight_top_data rank_fight_uids;
	if (!load_rank_data(zdb_pool, sql, rank_fight_uids)) {
		log_error("[%s] start_read_from_db error", m_service_name.c_str());
		return false;
	}

	for (const auto& pair_group : rank_fight_uids) {
		for (const auto& pair_layer : pair_group.second) {
			for (const auto& pair_top : pair_layer.second) {
				if (pair_top.first == class_type_top) {
					std::ostringstream key;
					key << m_service_name << ":" << pair_group.first << ":" << CLASS_LIST[pair_top.first];

					if (batch_list_lpush(key.str(), pair_top.second) < 0) {
						log_error("[%s] batch_list_lpush error", m_service_name.c_str());
						return false;
					}
				}
			}
		}
	}

	return true;
}

cross_area_video::cross_area_video(boost::asio::io_service& ios)
	: video_unit_t(ios)
{

}

cross_area_video::~cross_area_video()
{

}

void cross_area_video::do_save_video(uint64_t id, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::sv_cross_area_video_notify);

	log_trace("recv group[%lu] video[%s] cmd[%s:%d] message from cross", id, msg.base().video_uid().c_str(), CMD_DESC(op_cmd::sv_cross_area_video_notify), op_cmd::sv_cross_area_video_notify);

	const proto::common::video_single_base& base = msg.base();
	uint64_t video_uid = common::string_util_t::string_to_uint64(base.video_uid());
	if (video_uid == 0) {
		log_error("cross_area_video::do_save_video video_uid == 0");
		return;
	}

	Connection_T conn = common::zdb_util_t::get_zdb_conn(m_zdb_pool);
	if (!conn) {
		log_error("cross_area_video do_save_video conn is null");
		return;
	}

	do {
		uint32_t top = 0;
		// ¼ì²âÊÇ·ñÉÏ°ñ
		for (int32_t i = 0; i < base.objs_size(); ++i) {
			if (base.objs(i).cross_arena().rank_lv() != 0 && base.objs(i).cross_arena().rank_lv() < env::xml->cross_arena.strongest_king.rank_lv) {
				top = 1;
				break;
			}
		}

		if (!save_fight_data(conn, video_uid, id, top, base, msg.fight_data())) {
			log_error("cross_area_video save group[%lu] video[%lu] failed", id, video_uid);
			break;
		}

		if (top) update_rank_list(conn, video_uid, id);
	} while (0);

	Connection_close(conn);
}

bool cross_area_video::save_fight_data(const Connection_T& conn, uint64_t video_uid, uint32_t group_id, uint32_t top, const proto::common::video_single_base& base, const std::string& fight_data)
{
	if (!conn) {
		log_error("cross_arena_video save_fight_data conn is null");
		return false;
	}

	uint32_t base_buf_lenyh = base.ByteSize();
	if (base_buf_lenyh == 0) {
		log_error("cross_arena_video save_fight_data base size == 0");
		return false;
	}

	char base_buf[base_buf_lenyh + 1];
	bzero(base_buf, sizeof(base_buf));
	base.SerializeToArray(base_buf, base_buf_lenyh);
	
	bool ret = true;
	TRY
	{
		PreparedStatement_T pre = Connection_prepareStatement(conn, "INSERT INTO `%s` VALUES (?,?,?,?,?,?);", CROSS_ARENA.c_str());
		PreparedStatement_setLLong(pre, 1, video_uid);
		PreparedStatement_setInt(pre,	2, base.version());
		PreparedStatement_setInt(pre,	3, group_id);
		PreparedStatement_setInt(pre,	4, top);
		PreparedStatement_setBlob(pre,	5, base_buf, base_buf_lenyh);
		PreparedStatement_setBlob(pre,	6, fight_data.c_str(), fight_data.length());
		PreparedStatement_execute(pre);
	}
	CATCH(SQLException)
	{
		log_error("cross_area_video [%lu] insert db error: %s", video_uid, Exception_frame.message);
		ret = false;
	}
	FINALLY
	{
	}
	END_TRY;

	if (!ret) return false;

	if (top) {
		uint32_t cmd = 0;
		do {
			if (REDIS_OK != redisAppendCommand(m_redis_connect, "HSET %s:%lu %s %u", CROSS_ARENA.c_str(), video_uid, VERSION_ID.c_str(), base.version())) {
				log_error("table[%s] add video[%lu] error", CROSS_ARENA.c_str(), video_uid);
			} else {
				++cmd;
			}

			if (REDIS_OK != redisAppendCommand(m_redis_connect, "HSET %s:%lu %s %u", CROSS_ARENA.c_str(), video_uid, GROUP_ID.c_str(), group_id)) {
				log_error("table[%s] add video[%lu] error", CROSS_ARENA.c_str(), video_uid);
			} else {
				++cmd;
			}

			if (REDIS_OK != redisAppendCommand(m_redis_connect, "HSET %s:%lu %s %u", CROSS_ARENA.c_str(), video_uid, TOP_ID.c_str(), top)) {
				log_error("table[%s] add video[%lu] error", CROSS_ARENA.c_str(), video_uid);
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

	if (base.objs_size() != 2) {
		log_error("cross_area_video [%lu] insert obj size != 2", video_uid);
		return false;
	}

	TRY
	{
		PreparedStatement_T pre = NULL;
		if (base.objs(1).object_type() == proto::common::SCENEOBJECT_USER) {
			pre = Connection_prepareStatement(conn, "INSERT INTO `%s` (`%s`, `%s`, `%s`) VALUES (?,?,?), (?,?,?);", CROSS_ARENA_ROLE.c_str(), UID_ID.c_str(), VIDEO_ID.c_str(), FIGHT_BASE_ID.c_str());
		} else {
			pre = Connection_prepareStatement(conn, "INSERT INTO `%s` (`%s`, `%s`, `%s`) VALUES (?,?,?);", CROSS_ARENA_ROLE.c_str(), UID_ID.c_str(), VIDEO_ID.c_str(), FIGHT_BASE_ID.c_str());
		}
		PreparedStatement_setLLong(pre, 1, common::string_util_t::string_to_uint64(base.objs(0).uid()));
		PreparedStatement_setLLong(pre, 2, video_uid);
		PreparedStatement_setBlob(pre,	3, base_buf, base_buf_lenyh);

		if (base.objs(1).object_type() == proto::common::SCENEOBJECT_USER) {
			PreparedStatement_setLLong(pre, 4, common::string_util_t::string_to_uint64(base.objs(1).uid()));
			PreparedStatement_setLLong(pre, 5, video_uid);
			PreparedStatement_setBlob(pre, 6, base_buf, base_buf_lenyh);
		}
		PreparedStatement_execute(pre);
	}
	CATCH(SQLException)
	{
		log_error("cross_area_video[%lu] role insert db error: %s", video_uid, Exception_frame.message);
	}
	FINALLY
	{
	}
	END_TRY;

	return true;
}

void cross_area_video::update_rank_list(const Connection_T& conn, uint64_t video_uid, uint32_t group_id)
{
	std::ostringstream key;
	key << CROSS_ARENA << ":" << group_id << ":" << TOP_ID;

	uint32_t cmd = 0;
	do {
		int32_t list_len = get_list_len(key.str());
		if (list_len < 0) {
			break;
		}

		if ((uint32_t)list_len >= env::xml->cross_arena.strongest_king.recent_count) {
			uint64_t pop_video_uid = list_rpop<uint64_t>(key.str());
			if (pop_video_uid != 0) {
				if (REDIS_OK != redisAppendCommand(m_redis_connect, "EXPIRE %s:%lu %u", CROSS_ARENA.c_str(), pop_video_uid, env::xml->fight_ttl)) {
					log_error("table[%s] key[%lu] set EXPIRE second error", CROSS_ARENA.c_str(), pop_video_uid);
				} else {
					++cmd;
				}

				if (REDIS_OK != redisAppendCommand(m_redis_connect, "HSET %s:%lu %s %u", CROSS_ARENA.c_str(), pop_video_uid, TOP_ID.c_str(), 0)) {
					log_error("table[%s] key[%lu] field[%s] update error", CROSS_ARENA.c_str(), pop_video_uid, TOP_ID.c_str());
				} else {
					++cmd;
				}

				if (conn) {
					TRY
					{
						PreparedStatement_T pre = Connection_prepareStatement(conn, "UPDATE `%s` SET %s = 0 WHERE rk = %lu", CROSS_ARENA.c_str(), TOP_ID.c_str(), pop_video_uid);
						PreparedStatement_execute(pre);
					}
					CATCH(SQLException)
					{
						log_error("[%s:%lu] update video error error: %s", CROSS_ARENA.c_str(), pop_video_uid, Exception_frame.message);
					}
					FINALLY
					{
					}
					END_TRY;
				}
			}
		}

		if (REDIS_OK != redisAppendCommand(m_redis_connect, "LPUSH %s %lu", key.str().c_str(), video_uid)) {
			log_error("table[%s] add top video[%lu] error", CROSS_ARENA.c_str(), video_uid);
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
