#pragma once

/**
 * \brief 跨服竞技录像服务
 */

#include "common/video_service.hpp"
#include "server.pb.h"

static const std::string CROSS_ARENA = "cross_arena";
static const std::string CROSS_ARENA_ROLE = "cross_arena_role";

class cross_arena_service : public video_service_t
{
public:
	~cross_arena_service()
	{
		instance = NULL;
	}

	/**
	 * \brief 获取类的唯一实例
	 *
	 * \return 类的唯一实例
	 */
	static cross_arena_service &getInstance()
	{
		if (NULL == instance)
			instance = new cross_arena_service();

		return *instance;
	}

	/**
	 * \brief 释放类的唯一实例
	 *
	 */
	static void delInstance()
	{
		SAFE_DELETE_P(instance);
	}

protected:
	cross_arena_service() : video_service_t(CROSS_ARENA) {};

	// 初始化处理单元
	bool init_unit(const char* host, int32_t port, const char* passwd, int32_t db, struct ConnectionPool_S* zdb_pool, uint32_t num);
	// 从mysql加载数据
	bool start_read_from_db(struct ConnectionPool_S* zdb_pool);

private:
	/**
	 * \brief 类的唯一实例指针
	 */
	static cross_arena_service *instance;
};

class cross_area_video : public video_unit_t
{
public:
	explicit cross_area_video(boost::asio::io_service& ios);
	virtual ~cross_area_video();

protected:
	// 保存处理
	void do_save_video(uint64_t id, const network::msg_buf_ptr& msg_buf);
	// 写数据库
	bool save_fight_data(const Connection_T& conn, uint64_t video_uid, uint32_t group_id, uint32_t top, const proto::common::video_single_base& base, const std::string& fight_data);
	// 整理在榜数据
	void update_rank_list(const Connection_T& conn, uint64_t video_uid, uint32_t group_id);
};