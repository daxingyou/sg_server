#ifndef __ACCOUNT_LOG_WRAPPER_HPP__
#define __ACCOUNT_LOG_WRAPPER_HPP__

#include "main/account_server_fwd.hpp"
#include "protos_fwd.hpp"
#include "log_enum.hpp"

class log_wrapper_t
{
public:
    static void send_mobile_log(const std::string& uuid, const std::string& device_model, const std::string& device_sys_version, 
        const std::string& cpu_type, uint32_t cpu_count, uint32_t memory_size, const std::string& grph_device_name);

    static void send_account_log(uint64_t account_uid, uint32_t channel_id, uint64_t role_uid,
        const std::string& mobile_uuid, uint32_t source_type);
};

#endif
