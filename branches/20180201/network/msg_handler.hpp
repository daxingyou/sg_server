#ifndef __NETWORK_MSG_HANDLER_HPP__
#define __NETWORK_MSG_HANDLER_HPP__

#include "common_fwd.hpp"
#include "message.hpp"
#include "protobuf.hpp"
#include <map>
#include "log.hpp"
#include "tcp_socket.hpp"
#include "op_cmd.hpp"
#include "cmd_desc.hpp"

NS_NETWORK_BEGIN

template<uint16_t cmd_begin, uint16_t cmd_end, typename msg_handler_func>
class server_msg_handler_t
{
public:
    typedef std::map<uint16_t, msg_handler_func> msg_handler_func_map;

    bool register_func(uint16_t cmd, msg_handler_func func)
    {
        if (cmd <= cmd_begin || cmd >= cmd_end || NULL == func)
        {
            log_error("register func for cmd[%s:%d] error", CMD_DESC(cmd), cmd);
            return false;
        }
        if (NULL != find_func(cmd))
        {
            log_error("duplicate register func for cmd[%s:%d]", CMD_DESC(cmd), cmd);
            return false;
        }
        m_funcs[cmd] = func;
        return true;
    }

    msg_handler_func find_func(uint16_t cmd)
    {
        if (cmd <= cmd_begin || cmd >= cmd_end)
        {
            log_error("find func for cmd[%s:%d] error", CMD_DESC(cmd), cmd);
            return NULL;
        }
        typename msg_handler_func_map::iterator itr = m_funcs.find(cmd);
        if (itr != m_funcs.end())
        {
            return itr->second;
        }
        else
        {
            return NULL;
        }
    }

private:
    msg_handler_func_map m_funcs;
};

template<uint16_t cmd_begin, uint16_t cmd_end>
class client_msg_handler_t
{
public:
    typedef bool (*msg_handler_func)(const tcp_socket_ptr& s, const msg_buf_ptr& msg_buf);
    typedef std::map<uint16_t, msg_handler_func> msg_handler_func_map;

    bool register_func( uint16_t cmd, msg_handler_func func )
    {
        if (cmd <= cmd_begin || cmd >= cmd_end || NULL == func)
        {
            log_error("register func for cmd[%s:%d] error, cmd_begin[%d]->cmd_end[%d]", CMD_DESC(cmd), cmd, cmd_begin, cmd_end);
            return false;
        }
        if (NULL != find_func(cmd))
        {
            log_error("duplicate register func for cmd[%s:%d]", CMD_DESC(cmd), cmd);
            return false;
        }
        m_funcs[cmd] = func;
        return true;
    }

    msg_handler_func find_func( uint16_t cmd )
    {
        if (cmd <= cmd_begin || cmd >= cmd_end)
        {
            if (op_cmd::ca_pingpong_request != cmd)
            {
                log_error("find func for cmd[%s:%d] error", CMD_DESC(cmd), cmd);
            }
            return NULL;
        }
        msg_handler_func_map::iterator itr = m_funcs.find(cmd);
        if (itr != m_funcs.end())
        {
            return itr->second;
        }
        else
        {
            return NULL;
        }
    }

    bool handle_client_msg( const tcp_socket_ptr& s, uint64_t uid, uint16_t cmd, const msg_buf_ptr msg_buf )
    {
        if (op_cmd::ca_pingpong_request != cmd)
        {
            log_trace("recv role[%lu] cmd[%s:%d] message from client",uid, CMD_DESC(cmd), cmd);
        }

        msg_handler_func func = find_func(cmd);
        if ( NULL == func )
        {
            if (op_cmd::ca_pingpong_request != cmd)
            {
                log_error("no handler func for msg cmd[%s:%d]", CMD_DESC(cmd), cmd);
            }
            return false;
        }
        func(s, msg_buf);

        return true;
    };

private:
    msg_handler_func_map m_funcs;
};

NS_NETWORK_END

#endif
