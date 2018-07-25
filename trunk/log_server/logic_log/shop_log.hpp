#ifndef __LOG_SHOP_LOG_HPP__
#define __LOG_SHOP_LOG_HPP__

#include <boost/shared_ptr.hpp>
#include <vector>

#include "main/log_server_fwd.hpp"
#include "main/source_type_converter.hpp"
#include "log.hpp"
#include "log_enum.hpp"
#include "common_struct.hpp"
#include "common/zdb_ex.hpp"

class shop_log_t
{
public:
	shop_log_t(uint64_t role_uid, uint32_t goods_tid, uint32_t goods_num,
		uint32_t cost_type, uint32_t cost_value,
		uint32_t item_id, uint32_t item_num,
		uint32_t source_type, uint32_t source_param,
		uint32_t log_time)
		: m_role_uid(role_uid)
		, m_goods_tid(goods_tid)
		, m_goods_num(goods_num)
		, m_cost_type(cost_type)
		, m_cost_value(cost_value)
		, m_item_id(item_id)
		, m_item_num(item_num)
		, m_source_type(source_type)
		, m_source_param(source_param)
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
			"insert into `shop_log` set \
				`role_uid`=?, \
                `goods_tid`=?, \
			   `goods_num`=?, \
			   `cost_type`=?, \
			   `cost_value`=?, \
				`item_id`=?, \
			   `item_num`=?, \
                `source_type`=?, \
			  `source_param`=?, \
                `create_time`=from_unixtime(?), \
                `source_type_desc`=?;");
		PreparedStatement_setLLong(pre,	1, m_role_uid);
		PreparedStatement_setInt(pre, 2, m_goods_tid);
		PreparedStatement_setInt(pre, 3, m_goods_num);
		PreparedStatement_setInt(pre, 4, m_cost_type);
		PreparedStatement_setInt(pre, 5, m_cost_value);
		PreparedStatement_setInt(pre, 6, m_item_id);
		PreparedStatement_setInt(pre, 7, m_item_num);
		PreparedStatement_setInt(pre, 8, m_source_type);
		PreparedStatement_setInt(pre, 9, m_source_param);
		PreparedStatement_setInt(pre, 10, m_log_time);
		PreparedStatement_setString(pre, 11, sourec_type_str.c_str());
		PreparedStatement_execute(pre);
		}
			CATCH(SQLException)
		{
			log_error("user[%lu] insert shop user log to db error: %s", m_role_uid, Exception_frame.message);
		}
		FINALLY
		{
		}
		END_TRY;
	}

private:
	uint64_t            m_role_uid;
	uint32_t			m_goods_tid;
	uint32_t			m_goods_num;
	uint32_t			m_cost_type;
	uint32_t			m_cost_value;
	uint32_t			m_item_id;
	uint32_t			m_item_num;
	uint32_t			m_source_type;
	uint32_t			m_source_param;
	uint32_t            m_log_time;
};
typedef boost::shared_ptr<shop_log_t> shop_log_ptr;

///////////////////////////////////////////////////////////////////////////////////////////////////////
class shop_log_manager_t
{
public:
	void add_log(shop_log_ptr log_ptr)
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
	typedef std::vector<shop_log_ptr> shop_log_vec_type;
	shop_log_vec_type m_logs_list;
};

#endif
