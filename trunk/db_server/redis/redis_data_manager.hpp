#ifndef __DB_REDIS_DATA_MANAGER_HPP__
#define __DB_REDIS_DATA_MANAGER_HPP__

#include "tcp_socket.hpp"
#include "protos_fwd.hpp"
#include "common/macros.hpp"

class redis_data_manager_t
{
public:
    redis_data_manager_t();
    ~redis_data_manager_t();

public:
    static void handle_get_role_data(uint64_t uid, const network::tcp_socket_ptr& sock);
    static void handle_batch_save_to_db(const proto::server::gd_batch_save_to_db_notify& msg);
    static void handle_kick_role(uint64_t uid, const network::tcp_socket_ptr& p_game_socket);
	static void handle_syn_role_data(uint64_t uid);

	static bool save_data_to_redis(const proto::server::gd_batch_save_to_db_notify& save);
	static bool load_data_from_redis(uint64_t uid, proto::server::dg_role_data_reply& reply);

public:
	static void load_data_from_db_done(uint64_t uid, bool result, uint32_t game_id);
};

#endif//__DB_REDIS_DATA_MANAGER_HPP__
