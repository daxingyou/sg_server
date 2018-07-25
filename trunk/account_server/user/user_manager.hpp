#ifndef __ACCOUNT_USER_MANAGER_HPP__
#define __ACCOUNT_USER_MANAGER_HPP__

#include "protocpp/protos_fwd.hpp"
#include "network.hpp"
#include "channel/channel.hpp"

#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/asio/deadline_timer.hpp>
#include <boost/tuple/tuple.hpp>
#include "network/list.hpp"

#define ROLE_INDEX_MAX 0x1
#define ACCOUNT_ID_MAX 0xFFFFFFFF

class user_manager_t
{
public:
    static bool find_socket(const network::tcp_socket_ptr& s);
    static void add_socket(const network::tcp_socket_ptr& s);
    static void del_socket(const network::tcp_socket_ptr& s);
    static void clear_socket();
    static void socket_session_timeout_callback(const boost::system::error_code& ec, const network::tcp_socket_ptr& s);

public:
    //渠道登陆的反馈
	static void channel_login_respone(const account_channel_ptr& p_channel);

private:
    //根据渠道反馈得来的账号guid，到我们自己的游戏账号数据库中再检查一遍
	static boost::tuple<uint64_t, bool> db_check_guid(std::string const& guid, uint32_t channel_id);

    //检查无误后，返回所有角色数据给客户端
	static void load_all_roles_data(std::string &guid, uint64_t account_uid, uint32_t channel_id, const std::string login_server_mark_type, proto::client::nc_server_info_reply& reply);

public:
	static network::list_t<network::tcp_socket_wptr>::type m_socket_list;
};

class socket_data_t
{

public:
	socket_data_t() {};

    bool check_msg_bomb(uint32_t cmd);
    void reset_msg_bomb()
    {
        m_last_msg_time = 0;
        m_msg_bomb_count = 0;
    }

public:
	boost::shared_ptr<boost::asio::deadline_timer> p_session_timer = NULL;

private:
	uint64_t					m_last_msg_time = 0;
	uint32_t					m_msg_bomb_count = 0;
    //uint32_t					m_last_cmd = 0;
};
#endif
