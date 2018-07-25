#include "db_msg_handler.hpp"
#include "main/account_server.hpp"
#include "main/account_server_fwd.hpp"
#include "network.hpp"
#include "zdb_ex.hpp"
#include "log/log_wrapper.hpp"
#include "user/user_manager.hpp"

USING_NS_NETWORK;
USING_NS_COMMON;

server_msg_handler_t<op_cmd::DN_BEGIN, op_cmd::DN_END, db_msg_handler_t::msg_handler_func> db_msg_handler_t::m_db_msg_handle;

bool db_msg_handler_t::init_msg_handler()
{
	m_db_msg_handle.register_func(op_cmd::dn_create_role_request, handle_dn_create_role_request);
    m_db_msg_handle.register_func(op_cmd::dn_role_data_update_notify, handle_dn_role_data_update_notify);
    return true;
}

bool db_msg_handler_t::handle_dn_create_role_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::dn_create_role_request);
	Connection_T conn = zdb_util_t::get_zdb_conn(env::conn_zdb_pool);
	if (NULL == conn)
	{
		log_error("NULL == conn");
		return false;
	}

	const proto::common::account_role_data& rd = msg.role_data();
	proto::server::nd_create_role_reply reply;
	reply.set_gate_id(msg.gate_id());
	reply.mutable_role_data()->CopyFrom(rd);

	uint32_t b_ret = 1;
	uint32_t channel_id = 0;
	TRY
	{
		do
		{
			//先查询拥有多少个角色
			ResultSet_T result = Connection_executeQuery(conn, "SELECT count(*) FROM `role` WHERE `account_uid`=%lu and `server_id` =%u;", msg.account_id(), msg.server_id());
			if (ResultSet_next(result))
			{
				uint32_t role_num = ResultSet_getInt(result, 1);
				if (role_num >= ROLE_INDEX_MAX)
				{
					log_error(" create new role faild [uid=%lu server_id=%d]  role_num[%u] > max_size[%u]", uid, msg.server_id(), role_num, ROLE_INDEX_MAX);
					break;
				}
			}
			//取出account的channel_id
			ResultSet_T account_sql = Connection_executeQuery(conn, "SELECT `channel_id` FROM `account` WHERE `id`=\'%lu\';", msg.account_id());
			if (ResultSet_next(account_sql))
			{
				channel_id = ResultSet_getInt(account_sql, 1);
			}

			PreparedStatement_T pre = Connection_prepareStatement(conn, "REPLACE INTO `role`(`uid`,`account_uid`, `channel_id`, `name`,`create_time`,`species`,`server_id`,`level`,`plugin`) values(?,?,?,?,FROM_UNIXTIME(%d),?,?,?,?);", rd.create_time());
			if (pre)
			{
				PreparedStatement_setLLong(pre,		1, uid);
				PreparedStatement_setInt(pre,		2, msg.account_id());
				PreparedStatement_setInt(pre,		3, channel_id );
				PreparedStatement_setString(pre,	4, rd.name().c_str());
				PreparedStatement_setInt(pre,		5, rd.species());
				PreparedStatement_setInt(pre,		6, msg.server_id());
				PreparedStatement_setInt(pre,		7, rd.level());
				PreparedStatement_setInt(pre,		8, rd.plugin());
				PreparedStatement_execute(pre);
			}
			else
			{
				log_error("execue insert new role[uid=%lu:name=%s:server_id=%d] sql failed!", uid, rd.name().c_str(), msg.server_id());
				break;
			}
			b_ret = 0;

		} while (0);

	}
		CATCH(SQLException)
	{
		log_error("create role[channel_id=%d|server_id=%d|uid=%lu|name=%s] failed, error:%s", channel_id, msg.server_id(), uid, rd.name().c_str(), Exception_frame.message);
	}
	FINALLY
	{
		Connection_close(conn);
	}
	END_TRY;

    if (0 == b_ret)
    {
        log_wrapper_t::send_account_log(msg.account_id(), channel_id, uid, "", log_enum::source_type_account_create_role);

		ROLE_LOG("create role[channel_id=%d|server_id=%d|uid=%lu|name=%s] success!", channel_id, msg.server_id(), uid, rd.name().c_str());
    }

	reply.set_reply_code(b_ret);
	proto::common::account_role_data* p_role_data = msg.mutable_role_data();
	p_role_data->set_channel_id(channel_id);

    reply.set_time(time_util_t::now_msecs());
    env::server->send_msg_to_db(s, op_cmd::nd_create_role_reply, uid, reply);
	return true;
}

bool db_msg_handler_t::handle_dn_role_data_update_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::dn_role_data_update_notify);
	Connection_T conn = zdb_util_t::get_zdb_conn(env::conn_zdb_pool);
	if (NULL == conn)
	{
		log_error("NULL == conn");
		return false;
	}
	uint32_t level = msg.role_data().level();
	uint32_t plugin = msg.role_data().plugin();
	std::string name = msg.role_data().name();
	TRY
	{
		do
		{
			PreparedStatement_T pre = Connection_prepareStatement(conn, "UPDATE `role` SET `name` = ?, `level` = ?, `plugin` = ? WHERE `uid` = ?;");
			if (pre)
			{
				PreparedStatement_setString(pre,	1, name.c_str());
				PreparedStatement_setInt(pre,		2, level);
				PreparedStatement_setInt(pre,		3, plugin);
				PreparedStatement_setLLong(pre,		4, uid);
				PreparedStatement_execute(pre);
			}
			else
			{
				log_error("execue update role[uid=%lu] sql failed!", uid);
				break;
			}

		} while (0);

	}
		CATCH(SQLException)
	{
		log_error("update role[uid=%lu] error: %s", uid, Exception_frame.message);
	}
	FINALLY
	{
		Connection_close(conn);
	}
	END_TRY;

    ROLE_LOG("role[%lu] update data[name=%s level=%d plugin=%d] success!", uid, name.c_str(), level, plugin);

	return true;
}