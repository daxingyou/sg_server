#include "user_msg_handler.hpp"
#include "main/account_server.hpp"
#include "main/account_server_fwd.hpp"
#include "network.hpp"
#include "log.hpp"
#include "user_manager.hpp"
#include "common/common_struct.hpp"
#include "channel/channel_manager.hpp"

USING_NS_NETWORK;
USING_NS_COMMON;

client_msg_handler_t<op_cmd::CN_BEGIN, op_cmd::CN_END> user_msg_handler_t::m_user_msg_handle;

bool user_msg_handler_t::init_msg_handler()
{
    m_user_msg_handle.register_func(op_cmd::cn_server_info_request, handle_server_info_request);
    return true;
}

bool user_msg_handler_t::handle_server_info_request(const tcp_socket_ptr& s, const msg_buf_ptr& msg_buf)
{
    PRE_C2S_MSG(proto::client::cn_server_info_request);

    channel_manager_t::channel_login(s, msg);

    return true;
}
