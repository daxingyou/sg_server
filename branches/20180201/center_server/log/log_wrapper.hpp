#ifndef __CENTER_LOG_WRAPPER_HPP__
#define __CENTER_LOG_WRAPPER_HPP__

#include "main/center_server.hpp"
#include "main/center_server_fwd.hpp"
#include "log_enum.hpp"
class log_wrapper_t
{
public:
    static void send_country_log(uint32_t country, uint32_t type, uint64_t param, uint32_t param2, uint32_t param3);
	static void send_family_log(uint64_t family_uid, uint64_t role_uid, uint64_t target_uid, uint32_t type, uint32_t param, uint32_t param2 = 0);
    static void send_friend_log(uint64_t role_uid, uint64_t friend_uid, uint32_t old_type, uint32_t new_type, uint32_t type, uint32_t param);
	static void send_mail_log(uint64_t role_uid, uint64_t mail_id, uint32_t mail_type, uint32_t item_type, const std::string& items, uint32_t item_state, uint32_t type, uint32_t param);
	static void send_goods_log(uint64_t role_uid, const std::string& role_name, uint64_t goods_uid, 
		uint32_t item_tid, uint32_t item_num, uint32_t price, uint32_t selled_num, 
		uint32_t profit, uint32_t goods_state, uint32_t type);
	static void send_scene_log(uint64_t role_uid, uint32_t old_map, uint32_t new_map, uint32_t type);
	static void send_redbag_log(uint64_t role_uid, const std::string& role_name, uint64_t redbag_uid, uint32_t total_count,
		uint32_t total_money, uint32_t draw_money, uint32_t rest_count, uint32_t rest_money, uint32_t type);
};

// #define write_new_log(cmd, uid, msg) if (NULL != env::server) { env::server->send_msg_to_log(cmd, uid, msg); } else { log_error("write new log error"); }

#endif
