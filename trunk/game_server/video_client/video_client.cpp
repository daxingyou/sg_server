#include "video_client.hpp"
#include "log.hpp"
#include "protobuf.hpp"
#include "main/game_server_fwd.hpp"
#include <zlib.h>

video_client_t * video_client_t::instance = NULL;

#define MAX_COMBAT_DATA 200 * 1024			// 最大200k战斗数据
#define MAX_COMBAT_COMPRESS_DATA 10 * 1024	// 压缩完10k战斗数据(redis 对于大于10k的数据处理比较慢)

void video_client_t::set_socket(const network::tcp_socket_ptr& s)
{
	m_io_service.post([this, s]()
	{
		if (!s) {
			log_error("s == NULL");
			return;
		}

		m_client_socket = s;
	});
}

void video_client_t::clear_socket()
{
	m_io_service.post([this]()
	{
		m_client_socket.reset();
	});
}

void video_client_t::init()
{
	m_work.reset(new boost::asio::io_service::work(m_io_service));
	m_thread.reset(new boost::thread(boost::bind(&boost::asio::io_service::run, &m_io_service)));

	log_info("video_client_t init success!");
}

void video_client_t::close()
{
	m_work.reset();
	//m_io_service.stop();

	if (m_thread) {
		m_thread->join();
		m_thread.reset();
	}

	log_info("video_client_t close success!");
}

void video_client_t::post_fight_video(proto::common::fight_type fight_type, uint64_t fight_uid, video_data_ptr& p_video)
{
	m_io_service.post([this, fight_type, fight_uid, p_video]()
	{
		if (!p_video) {
			log_error("video[%u:%lu] == NULL", fight_type, fight_uid);
			return;
		}

		network::tcp_socket_ptr p_tcp_socket = m_client_socket.lock();
		if (!p_tcp_socket) {
			log_error("video[%u:%lu] p_tcp_socket == NULL", fight_type, fight_uid);
			return;
		}

		switch (fight_type) {
			case proto::common::fight_type_cross_arena_rank: {
				proto::server::sv_cross_area_video_notify notify;
				proto::common::video_single_base* p_base = notify.mutable_base();
				if (p_base) {
					p_base->Swap(&p_video->m_fight_base);
				}

				std::string fight_data;
				if (compress_fight_data(fight_uid, p_video->m_fight_combat_data, fight_data)) {
					notify.set_fight_data(fight_data);
					log_trace("fight[%lu] compress fight_data [%d] -> [%u]", fight_uid, p_video->m_fight_combat_data.ByteSize(), (uint32_t)fight_data.length());
				}

				network::msg_buf_ptr send_buf = network::wrap_msg(op_cmd::sv_cross_area_video_notify, env::xml->srv_group.group_id, notify);
				if (NULL == send_buf) {
					log_error("video[%u:%lu] wrap_msg cmd[%s:%d] failed: NULL == send_buf", fight_type, fight_uid, CMD_DESC(op_cmd::sv_cross_area_video_notify), op_cmd::sv_cross_area_video_notify);
					return;
				}

				p_tcp_socket->write(send_buf);

				log_trace("send video[%lu] cmd[%s:%d] to video", fight_uid, CMD_DESC(op_cmd::sv_cross_area_video_notify), op_cmd::sv_cross_area_video_notify);
				break;
			}
			default: {
				log_error("video fight_type[%u] not define", fight_type);
				break;
			}
		}
	});
}

bool video_client_t::compress_fight_data(uint64_t fight_uid, const proto::common::fight_video_data& data, std::string& out)
{
	if (data.ByteSize() > MAX_COMBAT_DATA) {
		log_error("fight[%lu] compress size[%u] > %u", fight_uid, data.ByteSize(), MAX_COMBAT_DATA);
		return false;
	}

	char unBuf[data.ByteSize() + 1];
	bzero(unBuf, sizeof(unBuf));
	data.SerializeToArray(unBuf, data.ByteSize());

	uLong destLen = MAX_COMBAT_COMPRESS_DATA;
	char destBuffer[destLen] = {0};

	int32_t retcode = compress((Bytef*)destBuffer, &destLen, (const Bytef *)unBuf, data.ByteSize());
	if (retcode == Z_OK) {
		out.assign(destBuffer, destLen);
	} else {
		log_error("fight[%lu] compress failed, error:%u", fight_uid, retcode);
		return false;
	}

	return true;
}

