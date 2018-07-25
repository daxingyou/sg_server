#include "account_msg_handler.hpp"
#include "main/log_server.hpp"
#include "main/log_server_fwd.hpp"
#include "network.hpp"
#include "common_struct.hpp"
#include "main/log_manager.hpp"

USING_NS_NETWORK;
USING_NS_COMMON;

server_msg_handler_t<op_cmd::NO_BEGIN, op_cmd::NO_END, account_msg_handler_t::msg_handler_func> account_msg_handler_t::m_account_msg_handle;

bool account_msg_handler_t::init_msg_handler()
{
    bool ret = true;

    m_account_msg_handle.register_func(op_cmd::no_batch_save_logs_notify, handle_batch_save_logs_notify);

    return ret;
}
bool account_msg_handler_t::handle_batch_save_logs_notify(const tcp_socket_ptr& s, const msg_buf_ptr& msg_buf)
{
    PRE_S2S_MSG_NO_UID(proto::log::no_batch_save_logs_notify);

    for (int32_t i = 0; i < (int32_t)msg.logs_size(); ++i)
    {
        uint16_t log_cmd = (uint16_t)msg.logs(i).cmd();
        add_logs(log_cmd, msg.logs(i).data());
    }

    return true;
}

void account_msg_handler_t::add_logs(uint16_t cmd, const std::string& msg_buf)
{
    log_data_t& log = log_manager_t::get_cur_thread_log_data();
    switch (cmd)
    {
    case op_cmd::no_mobile_log_notify:
    {
        PRE_S2LOG_MSG(proto::log::no_mobile_log_notify);
        mobile_log_ptr new_log(new mobile_log_t(msg.uuid(), msg.device_model(), msg.device_sys_version(), msg.cpu_type(), msg.cpu_count(), msg.memory_size(), msg.grph_device_name(), msg.log_time()));
        log.m_mobile_log_mgr.add_log(new_log);
    }
    break;
    case op_cmd::no_account_log_notify:
        {
            PRE_S2LOG_MSG(proto::log::no_account_log_notify);
            account_log_ptr new_log(new account_log_t(msg.account_uid(), msg.channel_id(), msg.role_uid(), msg.mobile_uuid(), msg.source_type(), msg.log_time()));
            log.m_account_log_mgr.add_log(new_log);
        }
        break;
    default:
        {
            log_error("op_cmd is unknown! cmd=%d", cmd);
            return;
        }
        break;
    }
}