#include "user_msg_handler.hpp"
#include "main/login_server.hpp"
#include "main/login_server_fwd.hpp"
#include "network.hpp"
#include "log.hpp"
#include "user_manager.hpp"
#include "gate/gate_manager.hpp"
#include "common/common_struct.hpp"
#include "utility.hpp"

USING_NS_NETWORK;
USING_NS_COMMON;

client_msg_handler_t<op_cmd::CL_BEGIN, op_cmd::CL_END> user_msg_handler_t::m_user_msg_handle;

bool user_msg_handler_t::init_msg_handler()
{
    m_user_msg_handle.register_func(op_cmd::cl_login_request, handle_cl_login_request);
    return true;
}

bool user_msg_handler_t::handle_cl_login_request(const tcp_socket_ptr& s, const msg_buf_ptr& msg_buf)
{
    PRE_C2S_MSG(proto::client::cl_login_request);
	user_manager_t::handle_cl_login_request(s, string_util_t::string_to_uint64(msg.uid()), msg.guid());
    return true;
}
