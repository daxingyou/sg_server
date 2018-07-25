#include "user_msg_handler.hpp"
#include "main/gate_server.hpp"
#include "main/gate_server_fwd.hpp"
#include "network.hpp"
#include "user_manager.hpp"
#include "utility.hpp"


USING_NS_NETWORK;
USING_NS_COMMON;

client_msg_handler_t<op_cmd::CA_BEGIN, op_cmd::CA_END> user_msg_handler_t::m_user_msg_handle;

bool user_msg_handler_t::init_msg_handler()
{
    m_user_msg_handle.register_func(op_cmd::ca_connect_request, handle_ca_connect_request);
    m_user_msg_handle.register_func(op_cmd::ca_pingpong_request, handle_ca_pingpong_request);
    m_user_msg_handle.register_func(op_cmd::ca_lock_notify, handle_ca_lock_notify);
    m_user_msg_handle.register_func(op_cmd::ca_unlock_notify, handle_ca_unlock_notify);
	m_user_msg_handle.register_func(op_cmd::ca_login_game_request, handle_ca_login_game_request);
	m_user_msg_handle.register_func(op_cmd::ca_create_role_request, handle_ca_create_role_request);

    return true;
}

bool user_msg_handler_t::handle_ca_pingpong_request(const tcp_socket_ptr& s, const msg_buf_ptr& msg_buf)
{
    if (NULL == s)
    {
        log_error("s == NULL!");
        return false;
    }

    socket_data_t* p_socket_data = (socket_data_t*)s->get_bind_data();
	if (NULL == p_socket_data)
	{
		log_error("p_socket_data == NULL!");
		return false;
	}

	const user_ptr& p_user = p_socket_data->get_user();
	if (NULL == p_user)
	{
		log_error("p_user == NULL!");
		return false;
	}

	p_user->start_pingpong_timer();
	p_user->send_msg_to_client(op_cmd::ac_pingpong_reply);

    return true;
}

bool user_msg_handler_t::handle_ca_connect_request(const tcp_socket_ptr& s, const msg_buf_ptr& msg_buf)
{
    PRE_C2S_MSG(proto::client::ca_connect_request);

    uint64_t user_uid = string_util_t::string_to_uint64(msg.uid());

    user_ptr p_user = NULL;
    bool rlt = false;
    do 
    {
        if (NULL == s)
        {
            log_error("NULL == s");
            return false;
        }

		p_user = user_manager_t::get_user(user_uid);
		if (NULL == p_user)
		{
			log_error("NULL == p_user, user_id[%lu]", user_uid);
			break;
		}

		// 状态判断，已经登陆gate，不处理，这个状况可能是网络波动，消息发了多次，也可能是恶意发送这个消息，直接return
		if (p_user->is_gate_state() || p_user->is_login_state())
		{
			log_warn("user_id[%lu] state[%u] is already login gate", user_uid, p_user->get_game_state());
			return false;
		}

		if (!msg.has_guid())
		{
			log_error("uid[%lu] guid == NULL", user_uid);
			break;
		}

        if (0 != strcmp(p_user->get_guid().c_str(), msg.guid().c_str()))
        {
            log_error("user[%lu] server_guid[%s] != client_guid[%s]", user_uid, p_user->get_guid().c_str(), msg.guid().c_str());
            break;
        }

		if (!msg.has_session_id())
		{
			log_error("uid[%lu] session_id == NULL", user_uid);
			break;
		}

		if (0 != strcmp(p_user->get_session_id().c_str(), msg.session_id().c_str()))
		{
			log_error("uid[%lu] p_user.session_id[%s] != msg.session_id[%s]", user_uid, p_user->get_session_id().c_str(), msg.session_id().c_str());
			break;
		}

        //客户端正常连接或者重连
		if (0 == msg.reconnect())
		{
			ROLE_LOG("user[%lu] try to connect......", user_uid);
		}
		else if (1 == msg.reconnect())
		{
			ROLE_LOG("user[%lu] try to reconnect......", user_uid);
		}
		else
		{
			log_error("reconnect[%d] from client error!", msg.reconnect());
			break;
		}

        if (!user_manager_t::socket_bind_user(s, p_user))
        {
            log_error("socket[%d] bind user[%lu] failed!", s->socket().native(), user_uid);
            break;
        }

        rlt = true;

		//设置登陆状态
		p_user->set_login_gate();

		ROLE_LOG("user[%lu], session_id[%s] connect to gate[%d] successfully!", user_uid, msg.session_id().c_str(), env::server->get_id());
    } while (0);

    proto::client::ac_connect_reply rep;
    if (rlt)
    {
        rep.set_reply_code(0);
    }
    else
    {
        rep.set_reply_code(1);
    }

	env::server->send_msg_to_client(s, user_uid, op_cmd::ac_connect_reply, rep);

    if (!rlt)
    {
        // 发生错误的处理，对于初始状态说明登陆失败了，但是其实是删除不掉的，因为还未绑定这个socket
		// 对于离线或者注销状态不处理
		// 删除s绑定的socket
		user_manager_t::del_socket(s, true);

        log_error("user[%lu] need to relogin!", user_uid);
    }

    return true;
}

bool user_msg_handler_t::handle_ca_lock_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
    if (NULL == s)
    {
        log_error("socket is NULL!");
        return true;
    }

	socket_data_t* p_socket_data = (socket_data_t*)s->get_bind_data();
	if (NULL == p_socket_data)
	{
		log_error("p_socket_data == NULL!");
		return false;
	}

	const user_ptr& p_user = p_socket_data->get_user();
	if (NULL == p_user)
	{
		log_error("p_user == NULL!");
		return false;
	}

	//TODO: 这个时候没有任何一个定时器接管这个用户,可能会有问题！！！！！！！，锁屏的处理问题
	if (!p_user->is_play_state())
	{
		log_error("user[%lu] recv cl_lock_notify", p_user->get_uid());
		return false;
	}

	p_user->cancel_pingpong_timer();
	ROLE_LOG("user[%lu] lock!", p_user->get_uid());

    return true;
}

bool user_msg_handler_t::handle_ca_unlock_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
    if (NULL == s)
    {
        log_error("socket is NULL!");
        return true;
    }

	socket_data_t* p_socket_data = (socket_data_t*)s->get_bind_data();
	if (NULL == p_socket_data)
	{
		log_error("p_socket_data == NULL!");
		return false;
	}

	const user_ptr& p_user = p_socket_data->get_user();
	if (NULL == p_user)
	{
		log_error("p_user == NULL!");
		return false;
	}

	if (!p_user->is_play_state())
	{
		log_error("user[%lu] recv ca_unlock_notify", p_user->get_uid());
		return false;
	}

	p_user->start_pingpong_timer();
	ROLE_LOG("user[%lu] unlock!", p_user->get_uid());

    return true;
}

bool user_msg_handler_t::handle_ca_login_game_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	if (NULL == s)
	{
		log_error("s == NULL!");
		return false;
	}

	socket_data_t* p_socket_data = (socket_data_t*)s->get_bind_data();
	if (NULL == p_socket_data)
	{
		log_error("p_socket_data == NULL!");
		return false;
	}

	const user_ptr& p_user = p_socket_data->get_user();
	if (NULL == p_user)
	{
		log_error("p_user == NULL!");
		return false;
	}

	if (!p_user->is_gate_state())
	{
		log_error("user[%lu] state[%u] error, when recv ca_login_game_request", p_user->get_uid(), p_user->get_game_state());
		return false;
	}

	PRE_C2S_MSG(proto::client::ca_login_game_request);

	// 向center请求上线
	proto::server::ae_reg_user_center_request ae_reg_request;
	ae_reg_request.set_uid(p_user->get_uid());
	env::server->send_msg_to_center(op_cmd::ae_reg_user_center_request, p_user->get_uid(), ae_reg_request);

	// 设置正在登陆场景状态
	p_user->set_login_game();

	ROLE_LOG("gate[%u] user[%lu] start login game", env::server->get_id(), p_user->get_uid());

	return true;
}

bool user_msg_handler_t::handle_ca_create_role_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	if (NULL == s)
	{
		log_error("s == NULL!");
		return false;
	}

	socket_data_t* p_socket_data = (socket_data_t*)s->get_bind_data();
	if (NULL == p_socket_data)
	{
		log_error("p_socket_data == NULL!");
		return false;
	}

	const user_ptr& p_user = p_socket_data->get_user();
	if (NULL == p_user)
	{
		log_error("p_user == NULL!");
		return false;
	}

	if (!p_user->is_login_state())
	{
		log_error("user[%lu] state[%u] error, when recv ca_create_role_request", p_user->get_uid(), p_user->get_game_state());
		return false;
	}

	if (p_user->is_create_state()) 
	{
		log_error("user[%lu] create msg is not reply, abandon cmd[ca_create_role_request]", p_user->get_uid());
		return false;
	}

	p_user->set_create_state();

	PRE_C2S_MSG(proto::client::ca_create_role_request);

	// 向center转发创建角色
	proto::server::ae_create_role_request ae_create_request;
	ae_create_request.set_uid(p_user->get_uid());
	ae_create_request.set_nickname(msg.nickname());
	ae_create_request.set_role_type(msg.role_type());
	env::server->send_msg_to_center(op_cmd::ae_create_role_request, p_user->get_uid(), ae_create_request);

	return true;
}
