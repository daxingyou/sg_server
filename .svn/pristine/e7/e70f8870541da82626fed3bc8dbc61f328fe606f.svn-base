#ifndef __ACCOUNT_CHANNEL_HPP__
#define __ACCOUNT_CHANNEL_HPP__

#include "tcp_socket.hpp"
#include "client.pb.h"

enum CHANNEL_ID : uint32_t
{
    CHANNEL_ID_TEST     = 0,    //测试渠道
    CHANNEL_ID_APPLE    = 1,	//苹果
    CHANNEL_ID_JY		= 2,	//九游
    CHANNEL_ID_DL       = 3,    //当乐
    CHANNEL_ID_END      = 100,
};

// 客户端传来的渠道参数，用于login request post使用
struct channel_login_request_data
{
    std::string m_session_id = "";  //渠道给的账号唯一ID，开发阶段写账号
    std::string m_uid = "";         //若渠道给的是空，则赋值为m_session_id
};

// login request post后返回的响应数据
struct channel_login_respone_data
{
    std::string m_guid = ""; //渠道验证无误后，返回的账户唯一ID，测试阶段即为账号
};

class channel_t
{
public:
    channel_t() {}
    ~channel_t() {}

public:
    uint32_t    m_id = 0;               //渠道唯一id
    std::string m_game_name = "";       //游戏名称
    std::string m_channel_name = "";    //渠道名称
    std::string m_login_url = "";       //登陆url
    std::string m_login_sk = "";        //登陆key
    std::string m_recharge_sk = "";     //充值key
};
typedef boost::shared_ptr<channel_t> channel_ptr;

class account_channel_t
{
public:
    account_channel_t() {}
    ~account_channel_t() {}

    bool load_data(const channel_ptr& p_channel, const network::tcp_socket_ptr& s, const proto::client::cn_server_info_request& req);

    std::string get_http_post_login_fields(const channel_ptr& p_channel);

    network::tcp_socket_ptr get_client_socket() { return m_client_socket.lock(); }

private:
    network::tcp_socket_wptr m_client_socket;

public:
    uint32_t m_channel_id = 0;
    std::string m_mobile_uuid = "";     //带着设备uuid是为了记日志，与具体的渠道登陆逻辑无关

    //login
public:
    channel_login_request_data m_login_request_data;
    channel_login_respone_data m_login_respone_data;
    std::string m_post_login_fields = ""; //http post login fields
	std::string m_login_mark_type	= "0"; //服务器列表标记
};
typedef boost::shared_ptr<account_channel_t> account_channel_ptr;

#endif
