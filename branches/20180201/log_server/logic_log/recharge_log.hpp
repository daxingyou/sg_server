#pragma once

#include <boost/shared_ptr.hpp>
#include <vector>
#include "main/log_server_fwd.hpp"
#include "log.hpp"
#include "log_enum.hpp"
#include "common_struct.hpp"
#include "common/zdb_ex.hpp"

class recharge_log_t
{
public:
	recharge_log_t(uint64_t role_uid, const std::string& order_id, uint32_t recharge_tid, 
		uint32_t recharge_rmb,	uint32_t channel_id, const std::string& create_date, uint32_t log_time)
		: m_role_uid(role_uid)
		, m_order_id(order_id)
		, m_recharge_tid(recharge_tid)
		, m_recharge_rmb(recharge_rmb)
		, m_channel_id(channel_id)
		, m_create_date(create_date)
		, m_log_time(log_time)
	{
	}

	void update_to_db(const Connection_T& conn)
	{
		TRY
		{
		PreparedStatement_T pre = Connection_prepareStatement(conn,
			"insert into `recharge_log` set \
				`role_uid`=?, \
                `order_id`=?, \
			   `recharge_tid`=?, \
			   `recharge_rmb`=?, \
				`channel_id`=?, \
				`create_date`=?, \
                `create_time`=from_unixtime(?);");
		PreparedStatement_setLLong(pre,	1, m_role_uid);
		PreparedStatement_setString(pre, 2, m_order_id.c_str());
		PreparedStatement_setInt(pre, 3, m_recharge_tid);
		PreparedStatement_setInt(pre, 4, m_recharge_rmb);
		PreparedStatement_setInt(pre, 5, m_channel_id);
		PreparedStatement_setString(pre, 6, m_create_date.c_str());
		PreparedStatement_setInt(pre, 7, m_log_time);
		PreparedStatement_execute(pre);
		}
			CATCH(SQLException)
		{
			log_error("user[%lu] insert recharge log to db error: %s", m_role_uid, Exception_frame.message);
		}
		FINALLY
		{
		}
		END_TRY;
	}

private:
	uint64_t            m_role_uid;
	std::string			m_order_id;
	uint32_t			m_recharge_tid;
	uint32_t			m_recharge_rmb;
	uint32_t			m_channel_id;
	std::string			m_create_date;
	uint32_t            m_log_time;
};
typedef boost::shared_ptr<recharge_log_t> recharge_log_ptr;

///////////////////////////////////////////////////////////////////////////////////////////////////////
class recharge_log_manager_t
{
public:
	void add_log(recharge_log_ptr log_ptr)
	{
		m_logs_list.push_back(log_ptr);
	}

	void update_to_db(const Connection_T& conn)
	{
		int32_t vec_size = m_logs_list.size();
		for (int32_t i = 0; i < vec_size; ++i)
		{
			if (m_logs_list[i])
			{
				m_logs_list[i]->update_to_db(conn);
			}
		}
		m_logs_list.clear();
	}

private:
	typedef std::vector<recharge_log_ptr> recharge_log_vec_type;
	recharge_log_vec_type m_logs_list;
};
