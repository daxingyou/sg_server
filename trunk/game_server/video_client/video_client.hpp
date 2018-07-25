#pragma once

#include <boost/scoped_ptr.hpp>
#include <boost/thread.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/asio.hpp>

#include "common.pb.h"
#include "server.pb.h"
#include "network.hpp"

struct video_data_t
{
	proto::common::video_single_base m_fight_base;
	proto::common::fight_video_data m_fight_combat_data;
};
typedef boost::shared_ptr<video_data_t> video_data_ptr;

class video_client_t
{
public:
	video_client_t() {}
	virtual ~video_client_t()
	{
		instance = NULL;
	}

	void set_socket(const network::tcp_socket_ptr& s);
	void clear_socket();

public:
	void init();
	void close();
	void post_fight_video(proto::common::fight_type fight_type, uint64_t fight_uid, video_data_ptr& p_video);

	/**
	 * \brief 获取类的唯一实例
	 *
	 * \return 类的唯一实例
	 */
	static video_client_t &getInstance()
	{
		if (NULL == instance)
			instance = new video_client_t();

		return *instance;
	}

	/**
	 * \brief 获取类的实例指针(不做new操作)
	 */
	static video_client_t* getInstancePtr()
	{
		return instance;
	}

	/**
	 * \brief 释放类的唯一实例
	 *
	 */
	static void delInstance()
	{
		SAFE_DELETE_P(instance);
	}

private:
	// 压缩战斗数据
	bool compress_fight_data(uint64_t fight_uid, const proto::common::fight_video_data& data, std::string& out);

private:
	boost::asio::io_service m_io_service;
	boost::scoped_ptr<boost::asio::io_service::work> m_work;
	boost::scoped_ptr<boost::thread> m_thread;

	network::tcp_socket_wptr m_client_socket;

	/**
	 * \brief 类的唯一实例指针
	 */
	static video_client_t *instance;
};