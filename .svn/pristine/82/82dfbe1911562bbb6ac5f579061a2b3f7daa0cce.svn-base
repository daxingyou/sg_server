#ifndef _COMMON_XML_DEFINE_HPP_
#define _COMMON_XML_DEFINE_HPP_

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#define GLOBAL_SERVER_XML_PATH      "../global_server.xml"
#define SERVER_XML_PATH             "../server.xml"
#define CHANNEL_XML_PATH            "../channel.xml"

#define GATE_XML_PATH               "./config.xml"
#define GAME_XML_PATH               "./config.xml"
#define TRANSFER_XML_PATH           "./config.xml"
#define ACCOUNT_XML_PATH            "./config.xml"
#define LOG_XML_PATH                "./config.xml"

#define PLATFORM_MAX 2
#define CHANNEL_ID_MAX 255
#define SERVER_GROUP_ID_MAX 1500
#define GLOBAL_SERVER_GROUP_ID_MIN 1501
#define GLOBAL_SERVER_GROUP_ID_MAX 2047

NS_COMMON_BEGIN

struct server_group_t
{
    uint32_t platform = 0; // ios:1 andriod:2
    std::string group_name = "";
    uint32_t group_id = 0;
    std::string version = "";
    std::string start_time = "";
    std::vector<uint32_t> channel_id_list;
};

struct thread_num_t
{
    int32_t network = 0;
    uint32_t mysql = 0;
    uint32_t user = 0;
    uint32_t channel = 0;
    uint32_t log = 0;
};

struct listen_t
{
    std::string ip = "";
    uint32_t port = 0;

    std::string out_ip = "";
    uint32_t out_port = 0;
};

struct connect_t
{
    uint32_t id = 0;
    std::string id_list = "";
    std::string ip = "";
    uint32_t port = 0;
};

struct mysql_t
{
    std::string host = "";
    std::string db = "";
    std::string user = "";
    std::string pwd = "";
    uint32_t init_conn = 0;
    uint32_t max_conn = 0;
};

struct redis_t
{
	std::string host = "";
	uint32_t port = 0;
	std::string pwd = "";
	uint32_t db = 0;
	uint32_t thread_num = 0;
	uint32_t read_unit_num = 0;
	uint32_t save_unit_num = 0;
	uint32_t save_interval = 0;
	uint32_t save_count_limit = 0;
	uint32_t conn_overtime = 0;	// 等待db读取超时时间
};

struct timeout_t
{
    uint32_t session;
    uint32_t alive;
    uint32_t pingpong;
    uint32_t reconnect;
	uint32_t msg_bomb_interval;
    uint32_t msg_bomb_count;
    uint32_t notify_login_time;
};

struct log_debug_t
{
	bool role;
	bool hero;
	bool equip;
	bool trade;
	bool task;
	bool scene;
	bool item;
	bool boss;
	bool fight;
	bool treasure;
	bool pk;
	bool rank;
	bool achieve;
	bool worldcup;
	bool red_point;
	bool lifeforce;
	bool new_guide;
	bool role_buff;
	bool dungeon;
	bool kingwar;
	bool shop;
	bool mail;
	bool friends;
	bool goods;
	bool family;
	bool active_reward;
	bool page;
	bool expedition;
	bool troop;
	bool arena;
	bool money;
	bool tower;
	bool multi;
	bool redbag;
	bool offline_arena;
	bool country;
	bool liandan;
	bool patrol_hire;
	bool activity;
};

struct xml_base_t
{
    thread_num_t thread_num;
    std::string log_path;
    bool is_log_trace;
    bool is_log_debug;
    bool is_log_info;
	bool is_new_logfile_per_hour;
	bool is_log_msg_count;
    std::string config_path;
    uint32_t reconnect_interval_time;
    std::string script_path;
    std::string map_path;
    std::string ai_path;
    std::string combat_path;
	uint32_t pingpong;
	log_debug_t log_debug;		//是否显示调试日志
	log_debug_t log_sql;		//是否写日志数据库
    uint32_t db_log_count_limit;
	uint32_t enter_game_max_byte;
	uint32_t enter_game_hero_max_byte;
	uint32_t enter_game_item_max_byte;
    std::vector<std::string> ip_white_list;
};

struct url_t
{
    std::string login_list_url = "";
    std::string sdk_login_url = "";
    std::string cdkey_url = "";
};

//-------------------------------------------------------------------
struct account_xml_t : public xml_base_t
{
    server_group_t srv_group;
    mysql_t mysql;
    url_t url;
    listen_t listen_at_client;
    listen_t listen_at_login;
	listen_t listen_at_db;
	listen_t listen_at_transfer;
    connect_t connect_to_log;
    timeout_t timeout;
	std::string  web_server_url;	//ADD_BY_HY
	std::string version;			//服务器版本号
	uint32_t check_login_server_flag;		//检查login标记
};

//-------------------------------------------------------------------
struct area_xml_t : public xml_base_t
{
    server_group_t srv_group;
    mysql_t mysql;
	listen_t listen_at_transfer;
    timeout_t timeout;
	redis_t redis;
	listen_t listen_at_cross;
};

//-------------------------------------------------------------------
struct transfer_xml_t : public xml_base_t
{
	listen_t listen_at_chat;
	listen_t listen_at_center;
	connect_t connect_to_area;
};

//-------------------------------------------------------------------
struct login_xml_t : public xml_base_t
{
    server_group_t srv_group;
    uint32_t user_num_max;
	uint32_t login_queue_timer;
	uint32_t login_queue_process;
	uint32_t ip_access;
    mysql_t mysql;
    listen_t listen_at_client;
    listen_t listen_at_gate;
	listen_t listen_at_center;
    std::vector<connect_t> connect_to_account_list;
    timeout_t timeout;
	redis_t redis;
	uint32_t can_create_new_role;	//是否能创建新角色 0 是可以 1表示限制
};

//-------------------------------------------------------------------
struct gate_xml_t : public xml_base_t
{
	server_group_t srv_group;
    listen_t listen_at_client;
    listen_t listen_at_game;
	std::vector<connect_t> connect_to_cross_list;
    connect_t connect_to_cross;
    connect_t connect_to_login;
    connect_t connect_to_chat;
    connect_t connect_to_center;
    timeout_t timeout;
	uint32_t notify_login_time;
};

//-------------------------------------------------------------------
struct game_xml_t : public xml_base_t
{
    server_group_t srv_group;
	listen_t listen_at_gate; //for nomal game
    std::vector<connect_t> connect_to_gate_list; //for cross
    connect_t connect_to_chat;
    connect_t connect_to_db;
    connect_t connect_to_center;
    connect_t connect_to_log;
	connect_t connect_to_area;
	connect_t connect_to_video;
	uint32_t max_nine_objects;
	uint32_t gm_level;
	uint32_t max_role_data_save_size;
	uint32_t max_role_data_save_time;
	uint32_t fight_version;
};

//-------------------------------------------------------------------
struct chat_xml_t : public xml_base_t
{
    server_group_t srv_group;
    listen_t listen_at_gate;
    listen_t listen_at_game;
    connect_t connect_to_game;
	connect_t connect_to_center;
	connect_t connect_to_video;
	std::vector<connect_t> connect_to_transfer_list;
};

//-------------------------------------------------------------------
struct db_xml_t : public xml_base_t
{
    server_group_t srv_group;
    mysql_t mysql;
    listen_t listen_at_game;
    listen_t listen_at_center;
    std::vector<connect_t> connect_to_account_list;
    redis_t redis;
	uint32_t max_preloading_count;
	uint32_t max_preloading_time;
};

//-------------------------------------------------------------------
struct center_xml_t : public xml_base_t
{
    server_group_t srv_group;
    mysql_t mysql;
    listen_t listen_at_gate;
    listen_t listen_at_game;
    listen_t listen_at_gm;
    listen_t listen_at_recharge;
	listen_t listen_at_chat;
    connect_t connect_to_db;
	connect_t connect_to_log;
	std::vector<connect_t> connect_to_transfer_list;
	redis_t redis;
};

//-------------------------------------------------------------------
struct log_xml_t : public xml_base_t
{
    mysql_t mysql;
    uint32_t update_to_db_time;
    listen_t listen_at_game;
    listen_t listen_at_center;
    listen_t listen_at_account;
};

//-------------------------------------------------------------------
struct gm_xml_t : public xml_base_t
{
    server_group_t srv_group;
    url_t url;
    listen_t listen_at_admin;
    connect_t connect_to_center;
	mysql_t  mysql;
	std::string ip_access;
	uint32_t channel_id;
	std::string cdkey_url;
	redis_t redis;
};

//--------------------------------------------------------------------
struct recharge_xml_t : public xml_base_t
{
	listen_t listen_at_center;
    connect_t connect_to_log;
	std::string ip_access;
};

//-------------------------------------------------------------------
//struct recharge_xml_t : public xml_base_t
//{
//    uint32_t server_id;
//    listen_t listen_at_channel;
//    connect_t connect_to_center;
//};

//-------------------------------------------------------------------
//struct monitor_xml_t: public xml_base_t
//{
//    uint32_t                  server_id;
//    uint32_t                  gate_num;
//    listen_t                  listen_at_http;
//    std::vector<std::string>  server_list;
//};

NS_COMMON_END
#endif

