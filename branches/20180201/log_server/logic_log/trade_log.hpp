#pragma once



#include <boost/shared_ptr.hpp>
#include <vector>

#include "main/log_server_fwd.hpp"
#include "main/source_type_converter.hpp"
#include "log.hpp"
#include "log_enum.hpp"
#include "common_struct.hpp"
#include "common/zdb_ex.hpp"

class trade_log_t
{
public:
	trade_log_t(uint64_t role_uid, uint32_t item_tid,uint32_t price,uint32_t num,
		uint32_t tax,uint32_t total_num, uint32_t total_cost, uint32_t gain,
		uint32_t trade_id,uint32_t op_type,	uint32_t source_type,uint32_t log_time)
		: m_role_uid(role_uid)
		, m_item_tid(item_tid)
		, m_price(price)
		, m_num(num)
		, m_tax(tax)
		, m_total_num(total_num)
		, m_total_cost(total_cost)
		, m_gain(gain)
		, m_trade_id(trade_id)
		, m_op_type(op_type)
		, m_source_type(source_type)
		, m_log_time(log_time)
	{
	}

	void update_to_db(const Connection_T& conn)
	{
		TRY
		{
			std::string sourec_type_str;
		if (!source_type_converter_t::find_desc(m_source_type, sourec_type_str))
		{
			log_error("can not find a description of the source_type[%d]!", m_source_type);
			return;
		}

		PreparedStatement_T pre = Connection_prepareStatement(conn,
			"insert into `trade_log` set \
				`role_uid`=?, \
                `item_tid`=?, \
			   `price`=?, \
			   `num`=?, \
				`tax`=?, \
			   `total_num`=?, \
				`total_cost`=?, \
				`gain`=?, \
			   `trade_id`=?, \
				`op_type`=?, \
                `source_type`=?, \
                `create_time`=from_unixtime(?), \
                `source_type_desc`=?;");
		PreparedStatement_setLLong(pre,	1, m_role_uid);
		PreparedStatement_setInt(pre, 2, m_item_tid);
		PreparedStatement_setInt(pre, 3, m_price);
		PreparedStatement_setInt(pre, 4, m_num);
		PreparedStatement_setInt(pre, 5, m_tax);
		PreparedStatement_setInt(pre, 6, m_total_num);
		PreparedStatement_setInt(pre, 7, m_total_cost);
		PreparedStatement_setInt(pre, 8, m_gain);
		PreparedStatement_setInt(pre, 9, m_trade_id);
		PreparedStatement_setInt(pre, 10, m_op_type);
		PreparedStatement_setInt(pre, 11, m_source_type);
		PreparedStatement_setInt(pre, 12, m_log_time);
		PreparedStatement_setString(pre, 13, sourec_type_str.c_str());
		PreparedStatement_execute(pre);
		}
			CATCH(SQLException)
		{
			log_error("user[%lu] insert trade log to db error: %s", m_role_uid, Exception_frame.message);
		}
		FINALLY
		{
		}
		END_TRY;
	}

private:
	uint64_t            m_role_uid;
	uint32_t			m_item_tid;
	uint32_t			m_price;
	uint32_t			m_num;
	uint32_t			m_tax;
	uint32_t			m_total_num;
	uint32_t			m_total_cost;
	uint32_t			m_gain;
	uint32_t			m_trade_id;
	uint32_t			m_op_type;
	uint32_t			m_source_type;
	uint32_t            m_log_time;
};
typedef boost::shared_ptr<trade_log_t> trade_log_ptr;

///////////////////////////////////////////////////////////////////////////////////////////////////////
class trade_log_manager_t
{
public:
	void add_log(trade_log_ptr log_ptr)
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
	typedef std::vector<trade_log_ptr> trade_log_vec_type;
	trade_log_vec_type m_logs_list;
};