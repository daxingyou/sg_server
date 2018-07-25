#pragma once

#include "server_base.hpp"
#include "common/zdb_ex.hpp"
#include "protobuf.hpp"
#include "network.hpp"
#include "cmd_desc.hpp"
#include "common/redis_client.hpp"

class db_server_t
    : public common::server_base_t
{
public:
    db_server_t(uint32_t id, std::string db_name);
    virtual ~db_server_t();

    virtual const char* server_name() { return "db"; }
    virtual bool on_init_server();
    virtual bool on_close_server();
    virtual void on_run_server();

private:
    bool load_xml();
    bool load_server_xml();
    void close_xml();

    bool init_zdb();
    void close_zdb();

    bool init_redis(uint32_t id);
    void close_redis();

    bool init_network();
    void close_network();

public:
   
	bool force_save(std::string table_name);
	//static boost::scoped_ptr<boost::asio::io_service::work> m_work;
	//static std::vector<boost::asio::io_service*> m_ios_list;
private:
    URL_T m_db_url;
	redis_client_t::simple_table_field_list m_tables;
	//boost::thread_group m_threads;

public:
	/*struct thread_data_t
	{
		thread_data_t()
		{
		}
		boost::scoped_ptr<boost::asio::io_service::work> work;
	};*/
	static bool init_save(int32_t thread_num);
	static void close_save();

	struct thread_data_t
	{
		thread_data_t(boost::asio::io_service& ios)
		{
		}
		boost::scoped_ptr<boost::asio::io_service::work> work;
	};

	static boost::thread_specific_ptr<thread_data_t> m_thread_data;
	static int32_t m_thread_num;
	static std::vector<boost::asio::io_service*> m_ios_list;
	uint32_t m_id;
	std::string m_db_name;
};
