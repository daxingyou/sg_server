#pragma once

/**
* \brief 定义录像服务基类
*/

#include <boost/scoped_ptr.hpp>
#include <boost/thread.hpp>
#include <map>

#include "video_unit.hpp"
#include "video_common.hpp"

class video_service_t : public redis_base_t
{
public:
	// 析构
	virtual ~video_service_t();

	// 初始化
	bool init(const char* host, // redis ip
		int32_t port,			// redis port
		const char* passwd,		// redis auth
		int32_t db,				// redis select
		uint32_t process_num,	// 处理单元个数（等于线程数）
		struct ConnectionPool_S* zdb_pool);

	// 关闭
	void close();

	// 分发录像消息
	void recv_video_message(uint64_t id, const network::msg_buf_ptr& msg_buf);

public:
	struct field_data
	{
		field_data(const std::string& name, DB_TYPE type, bool is_group, bool is_layer, uint32_t ctype)
			: field_name(name)
			, field_type(type)
			, need_group(is_group)
			, need_layer(is_layer)
			, class_type(ctype)
		{
		}

		std::string field_name;
		DB_TYPE field_type;
		bool need_group;
		bool need_layer;
		uint32_t class_type;
	};
	typedef std::vector<field_data> field_list;

	const field_list& get_field_list() const { return m_field_list; }

	// 上榜数据（大区，分层（如果有），细分类型）
	typedef std::map<uint32_t, std::map<uint32_t, std::map<uint32_t, std::vector<uint64_t>>>> fight_top_data;

protected:
	// 构造函数
	video_service_t(const std::string &name);
	// 建立表结构
	bool init_table_field(struct ConnectionPool_S* zdb_pool);
	// 加载榜上数据
	bool load_rank_data(struct ConnectionPool_S* zdb_pool, const char* sql, fight_top_data& fight_uids);
	// 初始化处理单元
	virtual bool init_unit(const char* host, int32_t port, const char* passwd, int32_t db, struct ConnectionPool_S* zdb_pool, uint32_t num) = 0;
	// 从mysql加载数据
	virtual bool start_read_from_db(struct ConnectionPool_S* zdb_pool) = 0;

protected:
	// 获得redis状态
	uint32_t get_redis_state();
	// 设置redis状态
	void set_redis_state(uint32_t state);
	// 读取结束的回调
	void read_db_done();

protected:
	boost::asio::io_service m_io_service;
	boost::scoped_ptr<boost::asio::io_service::work> m_work;
	boost::thread_group m_thread_group;

	field_list m_field_list;

	typedef std::vector<video_unit_ptr> video_unit_list;
	video_unit_list m_video_unit_list;

	std::string m_service_name;
	std::string m_redis_state;
};