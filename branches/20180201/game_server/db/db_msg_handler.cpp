#include "db_msg_handler.hpp"
#include "network.hpp"
#include "config/config_manager.hpp"

USING_NS_NETWORK;
USING_NS_COMMON;

server_msg_handler_t<op_cmd::DG_BEGIN, op_cmd::DG_END, db_msg_handler_t::msg_handler_func> db_msg_handler_t::m_db_msg_handle;

bool db_msg_handler_t::init_msg_handler()
{
    bool ret = true;
    ret &= m_db_msg_handle.register_func(op_cmd::dg_role_data_reply, handle_dg_role_data_reply);

    return ret;
}

bool db_msg_handler_t::handle_dg_role_data_reply(const network::msg_buf_ptr& msg_buf)
{
    PRE_S2S_MSG(proto::server::dg_role_data_reply);

	role_manager_t::on_user_record(uid, msg);

    return true;
}
