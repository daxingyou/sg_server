#include "log_wrapper.hpp"
#include "log_unify_save.hpp"
#include "common/common_struct.hpp"
#include "protos_fwd.hpp"

USING_NS_COMMON;

void log_wrapper_t::send_mobile_log(const std::string& uuid, const std::string& device_model, const std::string& device_sys_version, 
    const std::string& cpu_type, uint32_t cpu_count, uint32_t memory_size, const std::string& grph_device_name)
{
    proto::log::no_mobile_log_notify msg;
    msg.set_uuid(uuid.c_str());
    msg.set_device_model(device_model.c_str());
    msg.set_device_sys_version(device_sys_version.c_str());
    msg.set_cpu_type(cpu_type.c_str());
    msg.set_cpu_count(cpu_count);
    msg.set_memory_size(memory_size);
    msg.set_grph_device_name(grph_device_name.c_str());
    msg.set_log_time((uint32_t)time(0));
    write_new_log(op_cmd::no_mobile_log_notify, msg);
}

void log_wrapper_t::send_account_log(uint64_t account_uid, uint32_t channel_id, uint64_t role_uid, 
    const std::string& mobile_uuid, uint32_t source_type)
{
    proto::log::no_account_log_notify msg;
    msg.set_account_uid(account_uid);
    msg.set_channel_id(channel_id);
    msg.set_role_uid(role_uid);
    msg.set_mobile_uuid(mobile_uuid.c_str());
    msg.set_source_type(source_type);
    msg.set_log_time((uint32_t)time(0));
    write_new_log(op_cmd::no_account_log_notify, msg);
}