#pragma once

#include "redis_srv.hpp"

class redis_db_t : public redis_srv_t
{
public:
	redis_db_t();
	virtual ~redis_db_t();

public:
	bool load_user_from_db(uint64_t uid, uint32_t game_id);

protected:
	// 建立表结构
	bool init_table_field(struct ConnectionPool_S* zdb_pool, const sql_table *db_table);
	// 初始化读取线程列表
	bool init_read_unit_list(const char* host, int32_t port, const char* passwd, int32_t db, struct ConnectionPool_S* zdb_pool, uint32_t num);
	// 除了读取表数据以外的额外数据读取
	bool start_read_extra_data();
protected:
	uint32_t m_read_step = 0;
	std::vector<uint64_t> m_preloading_uids;
};

//////////////////////////////////////////////////////////////////////////
class redis_db_read_t : public redis_read_unit_t
{
public:
	explicit redis_db_read_t(boost::asio::io_service& ios, redis_srv_t& server);
	virtual ~redis_db_read_t();

public:
	virtual redis_db_read_ptr get_db_read_ptr() { return boost::dynamic_pointer_cast<redis_db_read_t>(shared_from_this()); }

public:
	// 读取一个玩家的数据
	void post_read_user_data(uint64_t uid, uint32_t game_id);
	void read_user_data(uint64_t uid, uint32_t game_id);
	bool do_read_user_data(uint64_t uid, const Connection_T& conn);

	// 起服预加载
	void post_pre_read_user_data(const std::vector<uint64_t>& uids);
	void pre_read_user_data(const std::vector<uint64_t>& uids);
};

extern redis_db_t g_redis_db;