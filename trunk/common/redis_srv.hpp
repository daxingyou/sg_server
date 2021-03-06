#pragma once

#include <boost/scoped_ptr.hpp>
#include <boost/thread.hpp>
#include <boost/asio/deadline_timer.hpp>

#include "redis_unit.hpp"
#include "redis_common.hpp"
/**
* \brief redis服务器，处理redis数据加载以及redis数据保存到mysql的操作
*/

class redis_read_unit_t;
typedef boost::shared_ptr<redis_read_unit_t> redis_read_unit_ptr;
typedef boost::weak_ptr<redis_read_unit_t> redis_read_unit_wptr;

class redis_db_read_t;
typedef boost::shared_ptr<redis_db_read_t> redis_db_read_ptr;
typedef boost::weak_ptr<redis_db_read_t> redis_db_read_wptr;

class redis_save_unit_t;
typedef boost::shared_ptr<redis_save_unit_t> redis_save_unit_ptr;

class redis_srv_t : public redis_base_t
{
public:
	redis_srv_t();
	virtual ~redis_srv_t();

public:
	// 初始化
	bool init(const char* host, // redis ip
		int32_t port,			// redis port
		const char* passwd,		// redis auth
		int32_t db,				// redis select
		const sql_table *db_table, // 需要加载的字段
		uint32_t thread_num,	// 线程数
		uint32_t read_num,		// 读取处理单元
		uint32_t save_interval, // 保存间隔
		uint32_t save_count_limit, // 单次保存数量
		uint32_t save_num,		// 保存处理单元
		struct ConnectionPool_S* zdb_pool);

	// 关闭
	void close(bool is_del = true);

	// 是否初始化完成
	bool is_init_done() const { return m_is_init_done; }

	// 是否关闭
	bool is_closed() const { return m_is_closed; }

public:
	struct field_data
	{
		field_data(const std::string& name, DB_TYPE type, bool mapping)
			: field_name(name)
			, field_type(type)
			, need_mapping(mapping)
		{
		}

		std::string field_name;
		DB_TYPE field_type;
		bool need_mapping;
	};
	typedef std::vector<field_data> field_list;
	typedef std::map<std::string, DB_TYPE> field_type_map;

	struct table_data_t
	{
		bool m_is_loaded = false;	// 是否需要起服加载
		uint32_t m_row_count = 0;	// 加载的条数
		field_list m_field_list;	// 字段列表
		field_type_map m_filed_map; // 字段类型映射
		std::string m_where;
	};

	typedef std::map<std::string, table_data_t> table_list;

	typedef std::vector<redis_read_unit_ptr> server_read_unit_list;
	typedef std::vector<redis_save_unit_ptr> server_save_unit_list;

	// 读取表结构
	const table_data_t& get_table_field_cols(const std::string& table);
	const field_type_map& get_table_field_map(const std::string& table);
	const table_list& get_table_list() const { return m_table_list; }

protected:
	// 建立表结构
	virtual bool init_table_field(struct ConnectionPool_S* zdb_pool, const sql_table *db_table);

	// 初始化读取线程列表
	virtual bool init_read_unit_list(const char* host, int32_t port, const char* passwd, int32_t db, struct ConnectionPool_S* zdb_pool, uint32_t num);

	// 初始化保存线程列表
	virtual bool init_save_unit_list(const char* host, int32_t port, const char* passwd, int32_t db, struct ConnectionPool_S* zdb_pool, uint32_t save_interval, uint32_t save_count_limit, uint32_t num);

public:
	// 读取结束返回
	void unit_read_finish(const redis_read_unit_ptr& p_read_unit_ptr);

protected:
	// 获得redis状态
	uint32_t get_redis_state();
	// 从mysql加载数据
	bool start_read_from_db();
	// 除了读取表数据以外的额外数据读取
	virtual bool start_read_extra_data() { return true; }
	// 检测加载是否完成
	void check_read_db_done();
	// 检测一个线程是否读完
	void do_read_finish(const redis_read_unit_wptr& p_wunit);
	// 读取结束的回调
	virtual void read_db_done();

protected:
	// 返回当前数据库的 key 的数量
	uint32_t get_db_size();
	// 删除当前db(谨慎使用)
	void do_flush_db();

protected:
	boost::asio::io_service m_io_service;
	boost::asio::io_service::strand m_strand;
	boost::scoped_ptr<boost::asio::io_service::work> m_work;
	boost::thread_group m_thread_group;

	table_list m_table_list;				// 表结构列表

	server_read_unit_list m_read_unit_list;		// 读取操作的线程
	server_save_unit_list m_save_unit_list;		// 保存操作的线程

	bool m_is_init_done = false;
	bool m_is_closed = true;
	static table_data_t const_error_list;
};

//////////////////////////////////////////////////////////////////////////
class redis_read_unit_t : public redis_unit_t, public boost::enable_shared_from_this<redis_read_unit_t>
{
public:
	explicit redis_read_unit_t(boost::asio::io_service& ios, redis_srv_t& server);
	virtual ~redis_read_unit_t();

public:
	redis_read_unit_ptr get_read_unit_ptr() { return shared_from_this(); }

	virtual redis_db_read_ptr get_db_read_ptr() { return redis_db_read_ptr(); }

public:
	// 起服批量读取sql
	void post_batch_read(const std::string& table, uint32_t cursor, uint32_t row_num);
	void batch_read_sql(const std::string& table, uint32_t cursor, uint32_t row_num);
	bool do_batch_read(const std::string& table, uint32_t cursor, uint32_t row_num);

	bool is_batch_read_done() { return m_batch_read_task == m_done_batch_task; }

protected:
	void add_batch_read_task();
	void add_done_batch_task();

protected:
	uint32_t m_batch_read_task = 0;
	uint32_t m_done_batch_task = 0;

protected:
	redis_srv_t& m_redis_server;
};

//////////////////////////////////////////////////////////////////////////
class redis_save_unit_t : public redis_unit_t, public boost::enable_shared_from_this<redis_save_unit_t>
{
public:
	explicit redis_save_unit_t(boost::asio::io_service& ios, redis_srv_t& server);
	virtual ~redis_save_unit_t();

	// 初始化
	virtual bool init(const char* host, int32_t port, const char* passwd, int32_t db, struct ConnectionPool_S* zdb_pool, uint32_t save_interval, uint32_t save_count_limit);

public:
	// 保存到sql定时器
	bool start_save_to_sql_timer();
	void stop_save_to_sql_timer();
	void save_to_sql_timer_callback(const boost::system::error_code& ec);

	// 保存所有未保存的字段
	void do_save_all();

protected:
	enum save_result_type
	{
		save_result_success = 0,	// 保存成功
		save_result_empty = 1,		// 保存缓存列表空了
		save_result_error = 2,		// 保存发生严重错误
	};

	save_result_type do_save_one(const Connection_T& conn);

protected:
	uint32_t m_save_interval = 1;			// 定时器间隔
	uint32_t m_save_count_limit = 500;		// 单次保存数量限制
	boost::asio::deadline_timer m_save_to_sql_timer;	// 定时器
	bool m_save_cancel = true;

protected:
	redis_srv_t& m_redis_server;
};