#ifndef __ACCOUNT_CHANNEL_HPP__
#define __ACCOUNT_CHANNEL_HPP__

#include "tcp_socket.hpp"
#include "client.pb.h"

enum CHANNEL_ID : uint32_t
{
    CHANNEL_ID_TEST     = 0,    //��������
    CHANNEL_ID_APPLE    = 1,	//ƻ��
    CHANNEL_ID_JY		= 2,	//����
    CHANNEL_ID_DL       = 3,    //����
    CHANNEL_ID_END      = 100,
};

// �ͻ��˴�������������������login request postʹ��
struct channel_login_request_data
{
    std::string m_session_id = "";  //���������˺�ΨһID�������׶�д�˺�
    std::string m_uid = "";         //�����������ǿգ���ֵΪm_session_id
};

// login request post�󷵻ص���Ӧ����
struct channel_login_respone_data
{
    std::string m_guid = ""; //������֤����󣬷��ص��˻�ΨһID�����Խ׶μ�Ϊ�˺�
};

class channel_t
{
public:
    channel_t() {}
    ~channel_t() {}

public:
    uint32_t    m_id = 0;               //����Ψһid
    std::string m_game_name = "";       //��Ϸ����
    std::string m_channel_name = "";    //��������
    std::string m_login_url = "";       //��½url
    std::string m_login_sk = "";        //��½key
    std::string m_recharge_sk = "";     //��ֵkey
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
    std::string m_mobile_uuid = "";     //�����豸uuid��Ϊ�˼���־��������������½�߼��޹�

    //login
public:
    channel_login_request_data m_login_request_data;
    channel_login_respone_data m_login_respone_data;
    std::string m_post_login_fields = ""; //http post login fields
	std::string m_login_mark_type	= "0"; //�������б���
};
typedef boost::shared_ptr<account_channel_t> account_channel_ptr;

#endif
