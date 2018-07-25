#include "db_save.hpp"
#include "db_save_fwd.hpp"
#include "log.hpp"
#include "global_id.hpp"
#include "msg_count.hpp"

USING_NS_NETWORK;
USING_NS_COMMON;

//void thread_main(boost::asio::io_service* ios)
//{
//	FUNC_CHECK_START
//	db_server_t::m_work.reset(new boost::asio::io_service::work(*ios));
//	ios->run();
//	FUNC_CHECK_END
//
//	//线程退出
//	SAFE_DELETE_P(ios);
//}
//
//void thread_close()
//{
//	db_server_t::m_work.reset();
//}

static boost::thread_group g_threads;
void thread_main(boost::asio::io_service* ios)
{
	FUNC_CHECK_START
	db_server_t::m_thread_data.reset(new db_server_t::thread_data_t(*ios));
	db_server_t::m_thread_data->work.reset(new boost::asio::io_service::work(*ios));
	ios->run();
	FUNC_CHECK_END

	//线程退出
	SAFE_DELETE_P(ios);
}

void thread_close()
{
	db_server_t::m_thread_data->work.reset();
}

//---------------------------------------------------------------------------------------
int32_t db_server_t::m_thread_num;
std::vector<boost::asio::io_service*> db_server_t::m_ios_list;
boost::thread_specific_ptr<db_server_t::thread_data_t> db_server_t::m_thread_data;

bool db_server_t::init_save(int32_t thread_num)
{
	m_thread_num = thread_num;
	m_ios_list.reserve(thread_num);
	for (int i = 0; i < thread_num; ++i)
	{
		m_ios_list.push_back(new boost::asio::io_service());
		g_threads.create_thread(boost::bind(&thread_main, m_ios_list.back()));
	}

	return true;
}

void db_server_t::close_save()
{
	size_t vec_size = m_ios_list.size();
	for (size_t i = 0; i < vec_size; ++i)
	{
		if (m_ios_list[i])
		{
			m_ios_list[i]->post(boost::bind(&thread_close));
		}
	}
	m_ios_list.clear();
	g_threads.join_all();

	log_info("close_save success!");
}

//-------------------------------------------------------------------------------
db_server_t::db_server_t(uint32_t id, std::string db_name)
    : server_base_t()
	, m_id(id)
	, m_db_name(db_name)
{
}

db_server_t::~db_server_t()
{
}

bool db_server_t::on_init_server()
{
    CMD_INIT;

    // xml
    if (!load_xml())
    {
        close_xml();
        COMMON_ASSERT(false);
        return false;
    }

    // log
    if (!g_log.open(env::xml->log_path, server_name(), env::xml->is_log_trace, env::xml->is_log_debug, env::xml->is_log_info, env::xml->is_new_logfile_per_hour))
    {
        COMMON_ASSERT(false);
        return false;
    }

    // zdb
    if (!init_zdb())
    {
        COMMON_ASSERT(false);
        return false;
    }

    // redis
    if (!init_redis(m_id))
    {
        COMMON_ASSERT(false);
        return false;
    }


    // network
//     if (!init_network())
//     {
//         COMMON_ASSERT(false);
//         return false;
//     }
	close_server();
    return true;
}

bool db_server_t::on_close_server()
{    
	close_redis();

	close_zdb();

//	close_save();

	close_xml();

    g_log.close();

    CMD_CLOSE;

    return true;
}

void db_server_t::on_run_server()
{
    log_info("db server running...");
}

//---------------------------------------------------------------------------------------
bool db_server_t::load_xml()
{
    if (!load_server_xml())
    {
        printf("\033[31mload [%s] failed!\033[0m\n", SERVER_XML_PATH);
        return false;
    }
    //printf("load [%s] success!\n", SERVER_XML_PATH);

    return true;
}
bool db_server_t::load_server_xml()
{
    try
    {
        boost::property_tree::ptree pt;
        boost::property_tree::xml_parser::read_xml(SERVER_XML_PATH, pt);

        env::xml = new db_xml_t;

        env::xml->mysql.host = pt.get<std::string>("server.db_server.mysql.host");
        //env::xml->mysql.db = pt.get<std::string>("server.db_server.mysql.db");
		env::xml->mysql.db = m_db_name;
        env::xml->mysql.user = pt.get<std::string>("server.db_server.mysql.user");
        env::xml->mysql.pwd = pt.get<std::string>("server.db_server.mysql.pwd");
		env::xml->mysql.init_conn = 1;/*pt.get<uint32_t>("server.db_server.mysql.init_connections");*/
		env::xml->mysql.max_conn = 5;/* pt.get<uint32_t>("server.db_server.mysql.max_connections");*/

		env::xml->redis.host = pt.get<std::string>("server.db_server.redis.host");
		env::xml->redis.port = pt.get<uint32_t>("server.db_server.redis.port");
		env::xml->redis.pwd = pt.get<std::string>("server.db_server.redis.pwd");
		env::xml->redis.db = pt.get<uint32_t>("server.db_server.redis.db");
		env::xml->redis.save_interval = pt.get<uint32_t>("server.db_server.redis.save_interval");
		env::xml->redis.save_count_limit = pt.get<uint32_t>("server.db_server.redis.save_count_limit");
		env::xml->log_path = pt.get<std::string>("server.common.log_path");
		env::xml->config_path = pt.get<std::string>("server.common.path.config");

    }
    catch (boost::property_tree::ptree_error& e)
    {
        printf("\033[31mthrow error[%s]\033[0m\n", e.what());
        return false;
    }

    if (NULL == env::xml)
    {
        printf("\033[31menv::xml is NULL!\033[0m\n");
        return false;
    }

    return true;
}

void db_server_t::close_xml()
{
    SAFE_DELETE_P(env::xml);
    log_info("close xml success!");
}

//----------------------------------------------------------------------------------------
bool db_server_t::init_zdb()
{
    boost::tie(m_db_url, env::conn_game_zdb_pool) = zdb_util_t::open_zdb(
        env::xml->mysql.host, 
        env::xml->mysql.db, 
        env::xml->mysql.user, 
        env::xml->mysql.pwd,
        env::xml->mysql.max_conn,
        env::xml->mysql.init_conn);
    if (NULL == env::conn_game_zdb_pool)
    {
        log_error("init zdb connection pool failed!");
        return false;
    }

    return true;
}
void db_server_t::close_zdb()
{
    zdb_util_t::close_zdb(env::conn_game_zdb_pool, m_db_url);
}

//----------------------------------------------------------------------------------------
bool db_server_t::init_redis(uint32_t id)
{
	if (id == 1)
		m_tables.insert(std::make_pair("task", "role_uid"));
	else if (id == 2)
		m_tables.insert(std::make_pair("heroes", "role_uid"));
	else if (id == 3)
		m_tables.insert(std::make_pair("achieve", "role_uid"));
	else if (id == 4)
		m_tables.insert(std::make_pair("active_reward", "role_uid"));
	else if (id == 5)
		m_tables.insert(std::make_pair("talent", "hero_uid"));
	else if (id == 6)
		m_tables.insert(std::make_pair("mail", "to_uid"));
	else if (id == 7)
		m_tables.insert(std::make_pair("role_ex", ""));
	else if (id == 8)
		m_tables.insert(std::make_pair("role_money", ""));
	else if (id == 9)
		m_tables.insert(std::make_pair("role_fight", ""));
	else if (id == 10)
		m_tables.insert(std::make_pair("role_tower", ""));
	else if (id == 11)
		m_tables.insert(std::make_pair("country_bounty", ""));
	else if (id == 12)
		m_tables.insert(std::make_pair("role_mount", "role_uid"));
	else if (id == 13)
		m_tables.insert(std::make_pair("arena_role_data", "role_uid"));
	else if (id == 14)
		m_tables.insert(std::make_pair("role", "name"));
	else if (id == 15)
		m_tables.insert(std::make_pair("role_dungeon", "role_uid"));
	else if (id == 16)
		m_tables.insert(std::make_pair("expedition", ""));
	else if (id == 17)
		m_tables.insert(std::make_pair("item", "role_uid"));
	else if (id == 18)
		m_tables.insert(std::make_pair("global_server_data", ""));
	else if (id == 19)
	{
		m_tables.insert(std::make_pair("city", ""));
		m_tables.insert(std::make_pair("country", ""));
		m_tables.insert(std::make_pair("country_office", "country_id"));
		m_tables.insert(std::make_pair("global_mail", ""));
		m_tables.insert(std::make_pair("goods", ""));
		m_tables.insert(std::make_pair("ranklist", ""));
		m_tables.insert(std::make_pair("user_goods", "role_uid"));
		m_tables.insert(std::make_pair("offline_arena", ""));
	}
	else if (id == 20)
	{
		m_tables.insert(std::make_pair("random_shop", "role_uid"));
		m_tables.insert(std::make_pair("family", "name"));
		m_tables.insert(std::make_pair("family_member", "family_id"));
		m_tables.insert(std::make_pair("family_build", "id"));
		m_tables.insert(std::make_pair("family_war", "week"));
		m_tables.insert(std::make_pair("king_war_apply", "king_war_id"));
		m_tables.insert(std::make_pair("goods_care", ""));
		m_tables.insert(std::make_pair("redbag", ""));
		m_tables.insert(std::make_pair("challenge", ""));
		m_tables.insert(std::make_pair("rarity_price", ""));
		m_tables.insert(std::make_pair("arena_fight_data", ""));
		m_tables.insert(std::make_pair("shop_goods", ""));
		m_tables.insert(std::make_pair("item_limit", ""));
	}
	else if (id == 21)
		m_tables.insert(std::make_pair("friend", "role_uid"));
	else if (id == 22)
		m_tables.insert(std::make_pair("role_trade_item", "role_uid"));
	else if (id == 23)
		m_tables.insert(std::make_pair("offline_role_data", ""));
	else if (id == 24)
		m_tables.insert(std::make_pair("role_simple_data", ""));
	else if (id == 25)
		m_tables.insert(std::make_pair("global_user", ""));

	if (!redis_client_t::init(env::xml->redis.host,
		env::xml->redis.port,
		env::xml->redis.pwd.c_str(),
		env::xml->redis.db,
		env::xml->redis.save_interval,
		env::xml->redis.save_count_limit,
		env::server->get_ios(),
		env::conn_game_zdb_pool,
		m_tables))
	{
		log_error("init redis client failed!");
		return false;
	}
	
	//init_save(m_tables.size());
	//uint32_t i = 0;
	for (auto& pair : m_tables)
	{
		//m_ios_list[i%m_thread_num]->post(boost::bind(&db_server_t::force_save, pair.first));
		force_save(pair.first);
		//i += 1;
	}

    return true;
}

bool db_server_t::force_save(std::string table_name)
{
	std::vector<std::string> ret;
	redis_client_t::get_keys_from_redis(table_name, ret);
	log_warn("save table[%s] to mysql start", table_name.c_str());
	for (auto& pair : ret)
	{
		redis_client_t::save_to_sql(pair);
	}
	redis_client_t::do_save_all(env::conn_game_zdb_pool, true);
	log_warn("save table[%s] to mysql end size[%d]", table_name.c_str(), ret.size());
}

void db_server_t::close_redis()
{
    redis_client_t::close(env::conn_game_zdb_pool);
}