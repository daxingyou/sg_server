#ifndef __GATE_SERVER_HPP__
#define __GATE_SERVER_HPP__

#include "server_base.hpp"
#include "protobuf.hpp"
#include "network.hpp"
#include "cmd_desc.hpp"
#include "game/game_manager.hpp"
#include "game/game_listener.hpp"
#include "game/cross_connector.hpp"
#include "game/game_msg_handler.hpp"
#include "user/user_listener.hpp"
#include "user/user_manager.hpp"
#include "user/user_msg_checker.hpp"
#include "chat/chat_connector.hpp"
#include "login/login_connector.hpp"
#include "center/center_connector.hpp"
#include "errcode_enum.hpp"

USING_NS_COMMON;

class gate_server_t
    : public common::server_base_t
{
public:
    gate_server_t();
    virtual ~gate_server_t();

    virtual const char* server_name() { return "gate"; }
    virtual bool on_init_server();
    virtual bool on_close_server();
    virtual void on_run_server();

private:
    bool load_xml();
    bool load_config_xml();
    bool load_server_xml();
    void close_xml();

    bool init_network();
    void close_network();

public:
	void post_network_send_msg(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& buf);

    void post_network_close_socket(const network::tcp_socket_ptr& s) { m_network.post_close_socket(s); }

public:
    /*
    ****client 的消息处理
    */
    void on_add_client(const network::tcp_socket_ptr& s);
    void on_del_client(const network::tcp_socket_ptr& s);
    void on_client_msg(const network::tcp_socket_ptr& s,uint64_t uid, uint16_t cmd, const network::msg_buf_ptr& msg_buf);

    void send_msg_to_client(uint16_t cmd, const network::msg_buf_ptr& buf, uint64_t uid)
    {
        user_ptr p_user = user_manager_t::get_user(uid);
        if (NULL == p_user)
        {
            log_error("send uid[%lu] cmd[%s:%d] to client failed: NULL == p_user", uid, CMD_DESC(cmd), cmd);
            return;
		}

		if (p_user->get_cross_id() != 0)
		{
			if (!game_msg_handler_t::check_cross_msg(cmd))
			{
				log_error("cross to client send msg pass cmd[%d] cross[%d] uid[%lu]", cmd, p_user->get_cross_id(), uid);
				return;
			}
		}

        p_user->send_msg_to_client(cmd, buf);
    }

    template<typename T_MSG>
    void send_msg_to_client(const network::tcp_socket_ptr& s, uint64_t uid, uint16_t cmd, const T_MSG& msg)
    {
        if (NULL == s)
        {
            log_error("send uid[%lu] cmd[%s:%d] to client failed: NULL == s", uid, CMD_DESC(cmd), cmd);
            return;
		}

		user_ptr p_user = user_manager_t::get_user(uid);
		if (NULL != p_user)
		{
			if (p_user->get_cross_id() != 0)
			{
				if (!game_msg_handler_t::check_cross_msg(cmd))
				{
					log_error("cross to client send msg pass cmd[%d] cross[%d] uid[%lu]", cmd, p_user->get_cross_id(), uid);
					return;
				}
			}
		}
		
#ifdef __ENCRYPT__
		network::msg_buf_ptr send_buf = network::wrap_msg_encrypt<T_MSG>(cmd, msg, SG_SERVER_TO_CLIENT_KEY);
#else
		network::msg_buf_ptr send_buf = network::wrap_msg_to_client<T_MSG>(cmd, msg);
#endif
		if (NULL == send_buf)
		{
			log_error("send uid[%lu] cmd[%s:%d] to client failed: NULL == send_buf", uid, CMD_DESC(cmd), cmd);
			return;
		}
		post_network_send_msg(s, send_buf);
		log_trace("send user[%lu] cmd[%s:%d] size[%d] to client", uid, CMD_DESC(cmd), cmd, send_buf->size());
    }

public:
    /*
    ****login 的消息处理
    */
    void on_add_login(const network::tcp_socket_ptr& s);
    void on_del_login(const network::tcp_socket_ptr& s);
    void on_login_msg(uint64_t uid,uint16_t cmd, const network::msg_buf_ptr& msg_buf);
    void syn_gate_info_to_login();

    template<typename T_MSG>
    void send_msg_to_login(uint16_t cmd, uint64_t uid, const T_MSG& msg)
    {
        network::msg_buf_ptr send_buf = network::wrap_msg<T_MSG>(cmd, uid, msg);
        if (NULL == send_buf)
        {
            log_error("send uid[%lu] cmd[%s:%d] to login failed: NULL == send_buf", uid, CMD_DESC(cmd), cmd);
            return;
        }
		//m_login_connector.send_msg_to_login(send_buf);
		post_network_send_msg(m_login_connector.get_socket(), send_buf);
        log_trace("send user[%lu] cmd[%s:%d] to login", uid, CMD_DESC(cmd), cmd);
    }

public:
    /*
    ****game 的消息处理
    */
    void on_add_game(const network::tcp_socket_ptr& s);
    void on_del_game(const network::tcp_socket_ptr& s);
    void on_game_msg(const network::tcp_socket_ptr& s, uint16_t cmd, const network::msg_buf_ptr& msg_buf, uint64_t uid);

    template<typename T_MSG>
    bool send_msg_to_game(const network::tcp_socket_ptr& s, uint16_t cmd, uint64_t uid, const T_MSG& msg)
    {
		if (NULL == s)
		{
			log_error("send uid[%lu] cmd[%s:%d] to game failed: NULL == s", uid, CMD_DESC(cmd), cmd);
			return false;
		}

        uint32_t game_id = game_manager_t::get_game_id_by_socket(s);
        uint32_t game_type = game_manager_t::get_game_type_by_socket(s);

        network::msg_buf_ptr send_buf = network::wrap_msg<T_MSG>(cmd, uid, msg);
        if (NULL == send_buf)
        {
            log_error("send uid[%lu] cmd[%s:%d] to game[%d] type[%u failed: NULL == send_buf", uid, CMD_DESC(cmd), cmd, game_id, game_type);
            return false;
        }

        post_network_send_msg(s, send_buf);

        log_trace("send user[%lu] cmd[%s:%d] to game[%d] type[%u]", uid, CMD_DESC(cmd), cmd, game_id, game_type);
        return true;
    }

    template<typename T_MSG>
    bool send_msg_to_all_games(uint16_t cmd, uint64_t uid, const T_MSG& msg, uint32_t type=game_server_type_normal)
    {
        network::msg_buf_ptr send_buf = network::wrap_msg<T_MSG>(cmd, uid, msg);
        if (NULL == send_buf)
        {
            log_error("send uid[%lu] cmd[%s:%d] to all games type[%d] failed: NULL == send_buf", uid, CMD_DESC(cmd), cmd, type);
            return false;
        }

        const game_vec& game_list = game_manager_t::get_game_list();
        game_vec::const_iterator it = game_list.begin();
        for (; it != game_list.end(); ++it)
        {
            const game_ptr p_game = *it;
            if (NULL == p_game)
            {
                log_error("send uid[%lu] cmd[%s:%d] to game type[%d] failed: NULL == p_game", uid, CMD_DESC(cmd), cmd, type);
                continue;
            }

            if (p_game->get_type() != type)
            {
                continue;
            }

            post_network_send_msg(p_game->get_socket(), send_buf);
            log_trace("send user[%lu] cmd[%s:%d] to game[%d] type[%u]", uid, CMD_DESC(cmd), cmd, p_game->get_id(), p_game->get_type());
        }

        return true;
    }

    template<typename T_BUF>
    void send_msg_from_client_to_game(const user_ptr& p_user, uint16_t cmd, const T_BUF& msg_buf)
    {
        if (NULL == p_user)
        {
            log_error("send cmd[%s:%d] to game failed: NULL == p_user", CMD_DESC(cmd), cmd);
            return;
        }
        
        uint64_t uid = p_user->get_uid();

        network::tcp_socket_ptr p_game_socket = NULL;
        if (p_user->get_cross_id() != 0 && game_msg_handler_t::check_cross_msg(cmd))
        {
            //send to cross
            p_game_socket = game_manager_t::get_game_socket_by_id(p_user->get_cross_id(), game_server_type_cross);
            if (NULL == p_game_socket)
            {
                log_error("send uid[%lu] cmd[%s:%d] to game type[%d] failed: NULL == p_game_socket", uid, CMD_DESC(cmd), cmd, game_server_type_cross);
                return;
            }
        }
        else
        {
            //send to game
            p_game_socket = game_manager_t::get_game_socket_by_user_uid(uid);
            if (NULL == p_game_socket)
            {
                log_error("send uid[%lu] cmd[%s:%d] to game type[%d] failed: NULL == p_game_socket", uid, CMD_DESC(cmd), cmd, game_server_type_normal);
                return;
            }
        }

        uint32_t game_id = game_manager_t::get_game_id_by_socket(p_game_socket);
        uint32_t game_type = game_manager_t::get_game_type_by_socket(p_game_socket);

#ifdef __ENCRYPT__
        network::unwrap_msg_decrypt<T_BUF>(msg_buf, SG_CLIENT_TO_SERVER_KEY);
        network::msg_buf_ptr send_buf = network::re_wrap_msg_to_server<T_BUF>(cmd, uid, msg_buf);
#else
        network::msg_buf_ptr send_buf = network::re_wrap_msg_to_server<T_BUF>(cmd, uid, msg_buf);
#endif
        if (NULL == send_buf)
        {
            log_error("send uid[%lu] cmd[%s:%d] to game[%d] type[%d] failed: NULL == send_buf", uid, CMD_DESC(cmd), cmd, game_id, game_type);
            return;
        }

        post_network_send_msg(p_game_socket, send_buf);

        switch (cmd)
        {
        case op_cmd::cg_move_request:
            break;
        default:
            log_trace("send user[%lu] cmd[%s:%d] from client to game[%d] type[%u]", uid, CMD_DESC(cmd), cmd, game_id, game_type);
            break;
        }
        return;
    }

public:
    /*
    ****cross 的消息处理
    */
    template<typename T_MSG>
    bool send_to_all_crosses(uint16_t cmd, uint64_t uid, const T_MSG& msg)
    {
        return send_msg_to_all_games(cmd, uid, msg, game_server_type_cross);
    }

public:
    /*
    ****chat 的消息处理
    */
    void on_add_chat(const network::tcp_socket_ptr& s);
    void on_del_chat(const network::tcp_socket_ptr& s);
    void on_chat_msg(uint16_t cmd, const network::msg_buf_ptr& msg_buf, uint64_t uid);

    template<typename T_MSG>
    bool send_msg_to_chat(uint16_t cmd, uint64_t uid, const T_MSG& msg)
    {
        network::msg_buf_ptr send_buf = network::wrap_msg<T_MSG>(cmd, uid, msg);
        if (NULL == send_buf)
        {
            log_error("send uid[%lu] cmd[%s:%d] to chat failed: NULL == send_buf", uid, CMD_DESC(cmd), cmd);
            return false;
        }
        //m_chat_connector.send_msg_to_chat(send_buf);
		post_network_send_msg(m_chat_connector.get_socket(), send_buf);
        log_trace("send user[%lu] cmd[%s:%d] to chat", uid, CMD_DESC(cmd), cmd);
        return true;
    }

    template<typename T_BUF>
    void send_msg_from_client_to_chat(const user_ptr& p_user, uint16_t cmd, const T_BUF& msg_buf)
    {
        if (NULL == p_user)
        {
            log_error("send cmd[%s:%d] to chat failed: NULL == p_user", CMD_DESC(cmd), cmd);
            return;
        }

        uint64_t uid = p_user->get_uid();
        network::msg_buf_ptr send_buf = network::re_wrap_msg_to_server<T_BUF>(cmd, uid, msg_buf);
        if (NULL == send_buf)
        {
            log_error("send uid[%lu] cmd[%s:%d] to chat failed: NULL == send_buf", uid, CMD_DESC(cmd), cmd);
            return;
        }
		//m_chat_connector.send_msg_to_chat(send_buf);
		post_network_send_msg(m_chat_connector.get_socket(), send_buf);
        log_trace("send user[%lu] cmd[%s:%d] from client to chat", uid, CMD_DESC(cmd), cmd);
    }

public:
    /*
    ****center 的消息处理
    */
    void on_add_center(const network::tcp_socket_ptr& s);
    void on_del_center(const network::tcp_socket_ptr& s);
    void on_center_msg(uint16_t cmd, const network::msg_buf_ptr& msg_buf, uint64_t uid);

    template<typename T_MSG>
    void send_msg_to_center(uint16_t cmd, uint64_t uid, const T_MSG& msg)
    {
        network::msg_buf_ptr send_buf = network::wrap_msg<T_MSG>(cmd, uid, msg);
        if (NULL == send_buf)
        {
            log_error("send uid[%lu] cmd[%s:%d] to center failed: NULL == send_buf", uid, CMD_DESC(cmd), cmd);
            return;
        }
		//m_center_connector.send_msg_to_center(send_buf);
		post_network_send_msg(m_center_connector.get_socket(), send_buf);
        log_trace("send user[%lu] cmd[%s:%d] to center", uid, CMD_DESC(cmd), cmd);
    }

    template<typename T_BUF>
    void send_msg_from_client_to_center(const user_ptr& p_user, uint16_t cmd, const T_BUF& msg_buf)
    {
        if (NULL == p_user)
        {
            log_error("send cmd[%s:%d] to center failed: NULL == p_user", CMD_DESC(cmd), cmd);
            return;
        }
        uint64_t uid = p_user->get_uid();
        network::msg_buf_ptr send_buf = network::re_wrap_msg_to_server<T_BUF>(cmd, uid, msg_buf);
        if (NULL == send_buf)
        {
            log_error("send uid[%lu] cmd[%s:%d] to center failed: NULL == send_buf", uid, CMD_DESC(cmd), cmd);
            return;
        }

		//m_center_connector.send_msg_to_center(send_buf);
		post_network_send_msg(m_center_connector.get_socket(), send_buf);
        log_trace("send user[%lu] cmd[%s:%d] from client to center", uid, CMD_DESC(cmd), cmd);
    }

public:
    uint32_t get_id() const { return m_id; }
    void set_id(uint32_t val) { m_id = val; }

private:
    uint32_t m_id = 0;
    network::network_t m_network;
    user_listener_t m_user_listener;
    game_listener_t m_game_listener;
    std::vector<cross_connector_t> m_cross_connector_list;
    chat_connector_t m_chat_connector;
    login_connector_t m_login_connector;
    center_connector_t m_center_connector;
};

#endif
