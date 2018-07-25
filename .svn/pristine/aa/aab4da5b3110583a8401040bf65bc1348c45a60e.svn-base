#include "account_msg_handler.hpp"
#include "main/login_server.hpp"
#include "main/login_server_fwd.hpp"
#include "network.hpp"

USING_NS_NETWORK;
USING_NS_COMMON;

server_msg_handler_t<op_cmd::NL_BEGIN, op_cmd::NL_END, account_msg_handler_t::msg_handler_func> account_msg_handler_t::m_account_msg_handle;

bool account_msg_handler_t::init_msg_handler()
{
    m_account_msg_handle.register_func(op_cmd::nl_syn_account_info_notify,     handle_nl_syn_account_info_notify);

    return true;
}

bool account_msg_handler_t::handle_nl_syn_account_info_notify(const tcp_socket_ptr& s, const msg_buf_ptr& msg_buf)
{
    PRE_S2S_MSG(proto::server::nl_syn_account_info_notify);

    account_manager_t::set_account(s, msg.account_id());
    
    return true;
}