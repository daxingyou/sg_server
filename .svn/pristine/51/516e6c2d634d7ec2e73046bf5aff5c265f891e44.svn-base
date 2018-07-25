#ifndef __LOG_RANDOM_SHOP_LOG_HPP__
#define __LOG_RANDOM_SHOP_LOG_HPP__

#include <boost/shared_ptr.hpp>
#include <vector>

#include "main/log_server_fwd.hpp"
#include "main/source_type_converter.hpp"
#include "log.hpp"
#include "log_enum.hpp"
#include "common_struct.hpp"
#include "common/zdb_ex.hpp"

class random_shop_log_t
{
public:
	random_shop_log_t(uint64_t role_uid,
		uint32_t shop_tid,
		uint32_t goods_tid,
		uint32_t cost_type,
		uint32_t cost_value,
		uint32_t cost_item,
		uint32_t cost_item_value,
		uint32_t old_num,
		uint32_t new_num,
		uint32_t source_type,
		uint32_t log_time)
		: m_role_uid(role_uid)
		, m_shop_tid(shop_tid)
		, m_goods_tid(goods_tid)
		, m_cost_type(cost_type)
		, m_cost_value(cost_value)
		, m_cost_item(cost_item)
		, m_cost_item_value(cost_item_value)
		, m_old_num(old_num)
		, m_new_num(new_num)
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
			"insert into `random_shop_log` set \
                `role_uid`=?, \
				`shop_tid`=?, \
				`goods_tid`=?, \
				`cost_type`=?, \
				`cost_value`=?, \
				`cost_item`=?, \
				`cost_item_value`=?, \
				`old_num`=?, \
				`new_num`=?, \
                `source_type`=?, \
                `create_time`=from_unixtime(?), \
                `source_type_desc`=?;");
		PreparedStatement_setLLong(pre, 1, m_role_uid);
		PreparedStatement_setInt(pre, 2, m_shop_tid);
		PreparedStatement_setInt(pre, 3, m_goods_tid);
		PreparedStatement_setInt(pre, 4, m_cost_type);
		PreparedStatement_setInt(pre, 5, m_cost_value);
		PreparedStatement_setInt(pre, 6, m_cost_item);
		PreparedStatement_setInt(pre, 7, m_cost_item_value);
		PreparedStatement_setInt(pre, 8, m_old_num);
		PreparedStatement_setInt(pre, 9, m_new_num);
		PreparedStatement_setInt(pre, 10, m_source_type);
		PreparedStatement_setInt(pre, 11, m_log_time);
		PreparedStatement_setString(pre, 12, sourec_type_str.c_str());
		PreparedStatement_execute(pre);
		}
			CATCH(SQLException)
		{
			log_error("role[%lu] insert random_shop log to db error: %s", m_role_uid, Exception_frame.message);
		}
		FINALLY
		{
		}
		END_TRY;
	}

private:
	uint64_t            m_role_uid;
	uint32_t            m_shop_tid;
	uint32_t            m_goods_tid;
	uint32_t			m_cost_type;
	uint32_t			m_cost_value;
	uint32_t			m_cost_item;
	uint32_t			m_cost_item_value;
	uint32_t			m_old_num;
	uint32_t			m_new_num;
	uint32_t			m_source_type;
	uint32_t            m_log_time;
};
typedef boost::shared_ptr<random_shop_log_t> random_shop_log_ptr;

///////////////////////////////////////////////////////////////////////////////////////////////////////
class random_shop_log_manager_t
{
public:
	void add_log(random_shop_log_ptr log_ptr)
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
	typedef std::vector<random_shop_log_ptr> random_shop_log_vec_type;
	random_shop_log_vec_type m_logs_list;
};

#endif